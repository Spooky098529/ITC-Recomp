#include "audio/xma_stream.h"

#include "audio/xma_packets.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/log.h>
#include <libavutil/mem.h>
}

#include <algorithm>
#include <cmath>
#include <cstring>
#include <mutex>

namespace audio::xma {

namespace {

constexpr uint32_t kDecoderDelay = 64;  // samples the decoder drops at the start
constexpr uint32_t kLoopForever = 255;

void Le16(uint8_t*& p, uint16_t v) {
  *p++ = uint8_t(v);
  *p++ = uint8_t(v >> 8);
}
void Le32(uint8_t*& p, uint32_t v) {
  Le16(p, uint16_t(v));
  Le16(p, uint16_t(v >> 16));
}

}  // namespace

struct Stream::Decoder {
  AVCodecContext* context = nullptr;
  AVPacket* packet = nullptr;
  AVFrame* frame = nullptr;
  ~Decoder() {
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&context);
  }
};

Stream::Stream() = default;
Stream::~Stream() = default;

uint32_t Stream::BufferSamples(const uint8_t* data, uint32_t size) {
  return CountFrames(data, size) * kSamplesPerFrame;
}

uint32_t Stream::ToBuffer(uint32_t timeline) {
  return timeline > kDecoderDelay ? timeline - kDecoderDelay : 0;
}

bool Stream::Begin(const uint8_t* data, uint32_t size, uint32_t channels, uint32_t sample_rate,
                   uint8_t* buffer) {
  End();
  static std::once_flag log_level;
  std::call_once(log_level, [] { av_log_set_level(AV_LOG_ERROR); });
  const AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_XMA2);
  if (!codec || channels < 1 || channels > 2) {
    return false;
  }
  auto decoder = std::make_unique<Decoder>();
  decoder->context = avcodec_alloc_context3(codec);
  decoder->context->sample_rate = int(sample_rate);
  av_channel_layout_default(&decoder->context->ch_layout, int(channels));
  // XMA2WAVEFORMATEX after WAVEFORMATEX (34 bytes, little-endian): the
  // decoder reads the stream count and the channel mask.
  constexpr int kExtraSize = 34;
  decoder->context->extradata =
      static_cast<uint8_t*>(av_mallocz(kExtraSize + AV_INPUT_BUFFER_PADDING_SIZE));
  decoder->context->extradata_size = kExtraSize;
  uint8_t* p = decoder->context->extradata;
  Le16(p, 1);
  Le32(p, channels == 2 ? 3 : 4);
  if (avcodec_open2(decoder->context, codec, nullptr) < 0) {
    return false;
  }
  decoder->packet = av_packet_alloc();
  decoder->frame = av_frame_alloc();
  decoder_ = std::move(decoder);
  data_ = data;
  size_ = size - size % uint32_t(kPacketSize);
  channels_ = channels;
  buffer_ = buffer;
  capacity_ = BufferSamples(data, size);
  total_ = capacity_;
  return true;
}

void Stream::End() {
  decoder_.reset();
  data_ = nullptr;
  buffer_ = nullptr;
  size_ = capacity_ = fed_ = decoded_ = total_ = position_ = 0;
  loop_start_ = loop_end_ = loops_left_ = 0;
  flushed_ = false;
}

void Stream::SetStart(uint32_t bit, uint32_t skip_subframes) {
  if (!active()) {
    return;
  }
  const uint32_t frame = FrameAtBitOffset(data_, size_, 0, bit).value_or(0);
  position_ = ToBuffer(frame * kSamplesPerFrame + skip_subframes * kSamplesPerSubframe);
}

void Stream::SetLoop(uint32_t start_bit, uint32_t end_bit, uint32_t skip_subframes,
                     uint32_t end_subframe, uint32_t count) {
  if (!active()) {
    return;
  }
  const auto start = FrameAtBitOffset(data_, size_, 0, start_bit);
  const auto end = FrameAtBitOffset(data_, size_, 0, end_bit);
  if (!start || !end || count == 0) {
    loops_left_ = 0;
    return;
  }
  loop_start_ = ToBuffer(*start * kSamplesPerFrame + skip_subframes * kSamplesPerSubframe);
  loop_end_ = ToBuffer(*end * kSamplesPerFrame + (end_subframe + 1) * kSamplesPerSubframe);
  loops_left_ = loop_end_ > loop_start_ ? count : 0;
}

void Stream::Drain() {
  AVFrame* frame = decoder_->frame;
  while (avcodec_receive_frame(decoder_->context, frame) == 0) {
    const uint32_t samples = std::min(uint32_t(frame->nb_samples), capacity_ - decoded_);
    uint8_t* out = buffer_ + size_t(decoded_) * channels_ * 2;
    for (uint32_t i = 0; i < samples; i++) {
      for (uint32_t c = 0; c < channels_; c++) {
        const float s = reinterpret_cast<const float*>(frame->extended_data[c])[i];
        const int32_t v = int32_t(std::lrintf(std::clamp(s, -1.0f, 1.0f) * 32767.0f));
        *out++ = uint8_t(uint16_t(v) >> 8);
        *out++ = uint8_t(v);
      }
    }
    decoded_ += samples;
  }
}

void Stream::DecodeAhead(uint32_t ahead) {
  if (!active() || flushed_) {
    return;
  }
  const uint32_t want = std::min(capacity_, position_ + ahead);
  while (decoded_ < want && !flushed_) {
    if (fed_ + kPacketSize <= size_) {
      av_new_packet(decoder_->packet, int(kPacketSize));
      std::memcpy(decoder_->packet->data, data_ + fed_, kPacketSize);
      avcodec_send_packet(decoder_->context, decoder_->packet);
      av_packet_unref(decoder_->packet);
      fed_ += uint32_t(kPacketSize);
    } else {
      avcodec_send_packet(decoder_->context, nullptr);
      flushed_ = true;
    }
    Drain();
  }
  if (flushed_) {
    total_ = decoded_;
  }
}

uint32_t Stream::Limit() const {
  const uint32_t end = loops_left_ ? std::min(loop_end_, total_) : total_;
  return std::min(end, decoded_);
}

uint32_t Stream::Contiguous(uint32_t cap) const {
  const uint32_t limit = Limit();
  return std::min(cap, limit > position_ ? limit - position_ : 0);
}

uint32_t Stream::Ready(uint32_t cap) const {
  uint32_t ready = Contiguous(cap);
  if (loops_left_ && position_ + ready >= loop_end_ && decoded_ > loop_start_) {
    ready += std::min(decoded_, loop_end_) - loop_start_;
  }
  return std::min(cap, ready);
}

uint32_t Stream::Consume(uint32_t count) {
  const uint32_t taken = std::min(count, Contiguous(count));
  position_ += taken;
  if (loops_left_ && position_ >= loop_end_) {
    position_ = loop_start_;
    if (loops_left_ != kLoopForever) {
      loops_left_--;
    }
  }
  return taken;
}

bool Stream::playing() const {
  return active() && (loops_left_ || !flushed_ || position_ < total_);
}

}  // namespace audio::xma
