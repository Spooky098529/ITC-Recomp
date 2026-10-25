#include "movies/player.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/channel_layout.h>
#include <libavutil/log.h>
#include <libavutil/mem.h>
}

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <deque>
#include <mutex>
#include <string>
#include <thread>

namespace movies {

namespace {

constexpr int kSampleRate = 48000;  // our audio output's, and every movie's
constexpr size_t kFramesAhead = 6;
constexpr size_t kSamplesAhead = kSampleRate * 2;  // stereo samples kept decoded
constexpr size_t kIoBufferSize = 64 * 1024;

using Clock = std::chrono::steady_clock;

int ReadFile(void* opaque, uint8_t* buffer, int size) {
  size_t read = std::fread(buffer, 1, size_t(size), static_cast<FILE*>(opaque));
  return read ? int(read) : AVERROR_EOF;
}

int64_t SeekFile(void* opaque, int64_t offset, int whence) {
  FILE* file = static_cast<FILE*>(opaque);
  if (whence == AVSEEK_SIZE) {
    int64_t at = ftello(file);
    fseeko(file, 0, SEEK_END);
    int64_t size = ftello(file);
    fseeko(file, at, SEEK_SET);
    return size;
  }
  return fseeko(file, offset, whence & ~AVSEEK_FORCE) == 0 ? ftello(file) : -1;
}

struct Decoder {
  int stream = -1;
  AVCodecContext* context = nullptr;
  ~Decoder() { avcodec_free_context(&context); }
  bool Open(AVFormatContext* format, AVMediaType type, AVCodecID codec_id) {
    stream = av_find_best_stream(format, type, -1, -1, nullptr, 0);
    if (stream < 0 || format->streams[stream]->codecpar->codec_id != codec_id) {
      stream = -1;
      return false;
    }
    const AVCodec* codec = avcodec_find_decoder(codec_id);
    context = codec ? avcodec_alloc_context3(codec) : nullptr;
    if (!context ||
        avcodec_parameters_to_context(context, format->streams[stream]->codecpar) < 0 ||
        avcodec_open2(context, codec, nullptr) < 0) {
      stream = -1;
      return false;
    }
    return true;
  }
};

std::mutex g_sounding_mutex;
Player* g_sounding = nullptr;

}  // namespace

struct Player::Impl {
  std::filesystem::path path;
  FILE* file = nullptr;
  AVIOContext* io = nullptr;
  AVFormatContext* format = nullptr;
  Decoder video, audio;
  double video_time_base = 0;
  int64_t first_video_pts = AV_NOPTS_VALUE;
  int64_t frame_duration_us = 33367;  // 29.97 fps, replaced by the stream's

  std::thread thread;
  std::mutex mutex;
  std::condition_variable wake;
  bool stop = false;
  bool decoded_all = false;
  std::deque<Frame> frames;    // decoded, not yet shown
  std::vector<Frame> unused;   // planes to reuse
  Frame shown;
  bool have_shown = false;
  std::deque<float> samples;   // interleaved stereo

  // The clock.
  bool started = false, paused = false;
  Clock::time_point start_time;
  Clock::duration paused_for{};
  Clock::time_point paused_at;

  ~Impl() {
    {
      std::lock_guard<std::mutex> lock(mutex);
      stop = true;
    }
    wake.notify_all();
    if (thread.joinable()) {
      thread.join();
    }
    avformat_close_input(&format);
    if (io) {
      av_freep(&io->buffer);
      avio_context_free(&io);
    }
    if (file) {
      std::fclose(file);
    }
  }

  // Under the mutex.
  int64_t NowUs() const {
    if (!started) {
      return 0;
    }
    Clock::time_point now = paused ? paused_at : Clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now - start_time - paused_for)
        .count();
  }

  bool Init(std::string& error) {
    file = std::fopen(path.string().c_str(), "rb");
    if (!file) {
      error = "can't be opened";
      return false;
    }
    uint8_t* buffer = static_cast<uint8_t*>(av_malloc(kIoBufferSize));
    io = buffer ? avio_alloc_context(buffer, int(kIoBufferSize), 0, file, ReadFile, nullptr,
                                     SeekFile)
                : nullptr;
    format = avformat_alloc_context();
    if (!io || !format) {
      av_free(buffer);
      error = "out of memory";
      return false;
    }
    format->pb = io;
    if (avformat_open_input(&format, nullptr, nullptr, nullptr) < 0 ||
        avformat_find_stream_info(format, nullptr) < 0) {
      error = "isn't a movie FFmpeg reads";
      return false;
    }
    if (!video.Open(format, AVMEDIA_TYPE_VIDEO, AV_CODEC_ID_WMV3)) {
      error = "has no WMV3 video";
      return false;
    }
    if (!audio.Open(format, AVMEDIA_TYPE_AUDIO, AV_CODEC_ID_WMAV2) ||
        audio.context->sample_rate != kSampleRate || audio.context->ch_layout.nb_channels != 2) {
      avcodec_free_context(&audio.context);
      audio.stream = -1;
    }
    AVStream* stream = format->streams[video.stream];
    video_time_base = av_q2d(stream->time_base);
    if (stream->avg_frame_rate.num > 0) {
      frame_duration_us = int64_t(1e6 / av_q2d(stream->avg_frame_rate));
    }
    thread = std::thread([this] { Decode(); });
    return true;
  }

  void TakeVideo(AVFrame* decoded) {
    int64_t pts = decoded->best_effort_timestamp;
    if (pts == AV_NOPTS_VALUE) {
      pts = decoded->pts;
    }
    std::unique_lock<std::mutex> lock(mutex);
    if (first_video_pts == AV_NOPTS_VALUE) {
      first_video_pts = pts;
    }
    Frame frame;
    if (!unused.empty()) {
      frame = std::move(unused.back());
      unused.pop_back();
    }
    lock.unlock();
    frame.time_us = int64_t((pts - first_video_pts) * video_time_base * 1e6);
    frame.width = uint32_t(decoded->width);
    frame.height = uint32_t(decoded->height);
    auto copy = [](std::vector<uint8_t>& out, const uint8_t* in, int pitch, uint32_t width,
                   uint32_t height) {
      out.resize(size_t(width) * height);
      for (uint32_t row = 0; row < height; row++) {
        std::memcpy(out.data() + size_t(row) * width, in + ptrdiff_t(row) * pitch, width);
      }
    };
    copy(frame.y, decoded->data[0], decoded->linesize[0], frame.width, frame.height);
    copy(frame.u, decoded->data[1], decoded->linesize[1], frame.width / 2, frame.height / 2);
    copy(frame.v, decoded->data[2], decoded->linesize[2], frame.width / 2, frame.height / 2);
    lock.lock();
    frames.push_back(std::move(frame));
  }

  void TakeAudio(AVFrame* decoded) {
    // wmav2 decodes to planar floats.
    if (decoded->format != AV_SAMPLE_FMT_FLTP) {
      return;
    }
    const float* left = reinterpret_cast<const float*>(decoded->extended_data[0]);
    const float* right = reinterpret_cast<const float*>(decoded->extended_data[1]);
    std::lock_guard<std::mutex> lock(mutex);
    for (int i = 0; i < decoded->nb_samples; i++) {
      samples.push_back(left[i]);
      samples.push_back(right[i]);
    }
  }

  // Sends a packet (null to flush) and takes what the decoder gives back.
  void Run(Decoder& decoder, const AVPacket* packet, AVFrame* decoded) {
    if (avcodec_send_packet(decoder.context, packet) < 0 && packet) {
      return;
    }
    while (avcodec_receive_frame(decoder.context, decoded) == 0) {
      if (&decoder == &video) {
        TakeVideo(decoded);
      } else {
        TakeAudio(decoded);
      }
      av_frame_unref(decoded);
    }
  }

  // The decoding thread: packets in file order until the frames and the
  // sound are both far enough ahead.
  void Decode() {
    AVPacket* packet = av_packet_alloc();
    AVFrame* decoded = av_frame_alloc();
    while (true) {
      {
        std::unique_lock<std::mutex> lock(mutex);
        wake.wait(lock, [this] {
          return stop || frames.size() < kFramesAhead ||
                 (audio.stream >= 0 && samples.size() < 2 * kSamplesAhead);
        });
        if (stop) {
          break;
        }
      }
      if (av_read_frame(format, packet) < 0) {
        Run(video, nullptr, decoded);
        if (audio.stream >= 0) {
          Run(audio, nullptr, decoded);
        }
        std::lock_guard<std::mutex> lock(mutex);
        decoded_all = true;
        break;
      }
      if (packet->stream_index == video.stream) {
        Run(video, packet, decoded);
      } else if (packet->stream_index == audio.stream) {
        Run(audio, packet, decoded);
      }
      av_packet_unref(packet);
    }
    av_frame_free(&decoded);
    av_packet_free(&packet);
  }
};

Player::Player(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {}
Player::~Player() {
  std::lock_guard<std::mutex> lock(g_sounding_mutex);
  if (g_sounding == this) {
    g_sounding = nullptr;
  }
}

std::unique_ptr<Player> Player::Open(const std::filesystem::path& path, std::string* error) {
  static std::once_flag log_level;
  std::call_once(log_level, [] { av_log_set_level(AV_LOG_ERROR); });
  auto impl = std::make_unique<Impl>();
  impl->path = path;
  std::string why;
  if (!impl->Init(why)) {
    if (error) {
      *error = why;
    }
    return nullptr;
  }
  return std::unique_ptr<Player>(new Player(std::move(impl)));
}

bool Player::HasSound() const { return impl_->audio.stream >= 0; }

void Player::Start() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->started = true;
  impl_->paused = false;
  impl_->start_time = Clock::now();
  impl_->paused_for = {};
}

void Player::Pause() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  if (impl_->started && !impl_->paused) {
    impl_->paused = true;
    impl_->paused_at = Clock::now();
  }
}

void Player::Resume() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  if (impl_->started && impl_->paused) {
    impl_->paused = false;
    impl_->paused_for += Clock::now() - impl_->paused_at;
  }
}

const Frame* Player::CurrentFrame() {
  Impl& m = *impl_;
  std::lock_guard<std::mutex> lock(m.mutex);
  int64_t now = m.NowUs();
  bool taken = false;
  while (!m.frames.empty() && (!m.have_shown || m.frames.front().time_us <= now)) {
    if (m.have_shown) {
      m.unused.push_back(std::move(m.shown));
    }
    m.shown = std::move(m.frames.front());
    m.frames.pop_front();
    m.have_shown = true;
    taken = true;
  }
  if (taken) {
    m.wake.notify_all();
  }
  return m.have_shown ? &m.shown : nullptr;
}

bool Player::Ended() {
  Impl& m = *impl_;
  std::lock_guard<std::mutex> lock(m.mutex);
  if (!m.decoded_all || !m.frames.empty()) {
    return false;
  }
  return !m.have_shown || m.NowUs() >= m.shown.time_us + m.frame_duration_us;
}

void Player::MixAudio(float* out, size_t count) {
  Impl& m = *impl_;
  std::lock_guard<std::mutex> lock(m.mutex);
  if (!m.started || m.paused) {
    return;
  }
  size_t available = std::min(m.samples.size() / 2, count);
  for (size_t i = 0; i < 2 * available; i++) {
    out[i] += m.samples[i];
  }
  m.samples.erase(m.samples.begin(), m.samples.begin() + ptrdiff_t(2 * available));
  if (available) {
    m.wake.notify_all();
  }
}

void SetSounding(Player* player) {
  std::lock_guard<std::mutex> lock(g_sounding_mutex);
  g_sounding = player;
}

void MixSoundingAudio(float* out, size_t samples) {
  std::lock_guard<std::mutex> lock(g_sounding_mutex);
  if (g_sounding) {
    g_sounding->MixAudio(out, samples);
  }
}

}  // namespace movies
