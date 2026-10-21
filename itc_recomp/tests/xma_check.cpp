// Decodes one XMA wave from a game wave bank (.xwb) with our own upstream
// FFmpeg's xma2 decoder (cmake/ffmpeg.cmake), the check for B2's step 1
// (docs/rexglue-removal.md): prints the bank's duration, the frames the packet
// headers count (audio/xma_packets.h) and the samples decoded, and writes a
// 16-bit WAV to listen to. Build and run, from the repo root:
//   cmake --build itc_recomp/out/build/linux-amd64-release --target itc_xma_check
//   itc_recomp/out/build/linux-amd64-release/itc_xma_check \
//     "itc/Import Tuner Challenge (USA)/DATA/AUDIO/bgm_tmp_01.xwb" 0 /tmp/bgm_tmp_01.wav
//
// Banks are XACT version 39, big-endian: "DNBW", the version, five segments
// (offset, length: bank data, entries, seek tables, names, wave data). An
// entry: flags and duration (duration in samples above 4 bits), the format
// (tag 2 bits, 1 = XMA; channels 3; sample rate 18; block align 8; bits 1),
// the play region (offset into the wave data, length) and the loop region
// (start, length, in samples).

#include "audio/xma_packets.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/mem.h>
}

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <vector>

namespace {

uint32_t Be32(const uint8_t* p) {
  return uint32_t(p[0]) << 24 | uint32_t(p[1]) << 16 | uint32_t(p[2]) << 8 | p[3];
}

void Le16(std::vector<uint8_t>& out, uint16_t v) {
  out.push_back(uint8_t(v));
  out.push_back(uint8_t(v >> 8));
}
void Le32(std::vector<uint8_t>& out, uint32_t v) {
  Le16(out, uint16_t(v));
  Le16(out, uint16_t(v >> 16));
}

int Fail(const char* message) {
  std::fprintf(stderr, "xma_check: %s\n", message);
  return 1;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    return Fail("usage: itc_xma_check <bank.xwb> [entry] [out.wav]");
  }
  const uint32_t index = argc > 2 ? uint32_t(std::atoi(argv[2])) : 0;
  std::ifstream file(argv[1], std::ios::binary);
  const std::vector<uint8_t> bank{std::istreambuf_iterator<char>(file), {}};
  if (bank.size() < 0x90 || std::memcmp(bank.data(), "DNBW", 4) != 0 || Be32(&bank[4]) != 39) {
    return Fail("not a version 39 big-endian wave bank");
  }
  const uint32_t bank_data = Be32(&bank[8]), entries = Be32(&bank[16]), wave_data = Be32(&bank[40]);
  const uint32_t count = Be32(&bank[bank_data + 4]);
  const uint32_t entry_size = Be32(&bank[bank_data + 72]);
  if (index >= count) {
    return Fail("no such entry");
  }
  const uint8_t* entry = &bank[entries + index * entry_size];
  const uint32_t duration = Be32(entry) >> 4, format = Be32(entry + 4);
  const uint32_t play_offset = Be32(entry + 8), play_length = Be32(entry + 12);
  const uint32_t loop_start = Be32(entry + 16), loop_length = Be32(entry + 20);
  const uint32_t tag = format & 3, channels = (format >> 2) & 7;
  const uint32_t sample_rate = (format >> 5) & 0x3FFFF;
  std::printf("entry %u of %u: tag %u, %u channels, %u Hz, %u samples (%.2f s), "
              "%u bytes (%u packets), loop from %u for %u samples\n",
              index, count, tag, channels, sample_rate, duration, double(duration) / sample_rate,
              play_length, play_length / uint32_t(audio::xma::kPacketSize), loop_start,
              loop_length);
  if (tag != 1) {
    return Fail("not XMA");
  }
  const uint8_t* data = &bank[wave_data + play_offset];
  const uint32_t frames = audio::xma::CountFrames(data, play_length);
  std::printf("packet headers: %u frames, %u samples\n", frames,
              frames * audio::xma::kSamplesPerFrame);

  const AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_XMA2);
  if (!codec) {
    return Fail("no xma2 decoder");
  }
  AVCodecContext* context = avcodec_alloc_context3(codec);
  context->sample_rate = int(sample_rate);
  av_channel_layout_default(&context->ch_layout, int(channels));
  // XMA2WAVEFORMATEX's 34 bytes after WAVEFORMATEX (little-endian): the
  // decoder reads the stream count and the channel mask.
  std::vector<uint8_t> extra;
  Le16(extra, 1);                              // streams
  Le32(extra, channels == 2 ? 3 : 4);          // channel mask
  Le32(extra, duration);                       // samples encoded
  Le32(extra, play_length);                    // bytes a block
  Le32(extra, 0);                              // play begin
  Le32(extra, duration);                       // play length
  Le32(extra, loop_start);                     // loop begin
  Le32(extra, loop_length);                    // loop length
  extra.push_back(0);                          // loop count
  extra.push_back(4);                          // encoder version
  Le16(extra, 1);                              // blocks
  context->extradata = static_cast<uint8_t*>(av_mallocz(extra.size() + AV_INPUT_BUFFER_PADDING_SIZE));
  std::memcpy(context->extradata, extra.data(), extra.size());
  context->extradata_size = int(extra.size());
  if (avcodec_open2(context, codec, nullptr) < 0) {
    return Fail("couldn't open the decoder");
  }

  AVPacket* packet = av_packet_alloc();
  AVFrame* frame = av_frame_alloc();
  std::vector<int16_t> pcm;
  uint64_t decoded = 0;
  float peak = 0;
  auto drain = [&] {
    while (avcodec_receive_frame(context, frame) == 0) {
      decoded += uint64_t(frame->nb_samples);
      for (int i = 0; i < frame->nb_samples; i++) {
        for (uint32_t c = 0; c < channels; c++) {
          const float s = reinterpret_cast<const float*>(frame->extended_data[c])[i];
          peak = std::max(peak, std::fabs(s));
          pcm.push_back(int16_t(std::clamp(s, -1.0f, 1.0f) * 32767));
        }
      }
    }
  };
  int errors = 0;
  for (uint32_t at = 0; at + audio::xma::kPacketSize <= play_length; at += audio::xma::kPacketSize) {
    av_new_packet(packet, int(audio::xma::kPacketSize));
    std::memcpy(packet->data, data + at, audio::xma::kPacketSize);
    if (avcodec_send_packet(context, packet) < 0) {
      errors++;
    }
    av_packet_unref(packet);
    drain();
  }
  avcodec_send_packet(context, nullptr);
  drain();
  std::printf("decoded: %llu samples (%.2f s), peak %.3f, %d packets refused\n",
              static_cast<unsigned long long>(decoded), double(decoded) / sample_rate, peak,
              errors);

  if (argc > 3) {
    std::vector<uint8_t> wav;
    const uint32_t bytes = uint32_t(pcm.size() * 2);
    wav.insert(wav.end(), {'R', 'I', 'F', 'F'});
    Le32(wav, 36 + bytes);
    wav.insert(wav.end(), {'W', 'A', 'V', 'E', 'f', 'm', 't', ' '});
    Le32(wav, 16);
    Le16(wav, 1);
    Le16(wav, uint16_t(channels));
    Le32(wav, sample_rate);
    Le32(wav, sample_rate * channels * 2);
    Le16(wav, uint16_t(channels * 2));
    Le16(wav, 16);
    wav.insert(wav.end(), {'d', 'a', 't', 'a'});
    Le32(wav, bytes);
    std::ofstream out(argv[3], std::ios::binary);
    out.write(reinterpret_cast<const char*>(wav.data()), std::streamsize(wav.size()));
    out.write(reinterpret_cast<const char*>(pcm.data()), std::streamsize(bytes));
    std::printf("wrote %s\n", argv[3]);
  }
  av_frame_free(&frame);
  av_packet_free(&packet);
  avcodec_free_context(&context);
  return 0;
}
