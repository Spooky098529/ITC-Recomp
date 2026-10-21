// One XMA sound decoded with FFmpeg's xma2 decoder into a flat buffer of
// 16-bit big-endian samples (interleaved for stereo), read by position: the
// game's XMA source voice asks how many samples are ready, where to read
// them and how many it took (B2 in docs/rexglue-removal.md). The decoding
// keeps ahead of the reading; a loop is a jump of the position. No ring,
// blocks or XMA context. Plain C++: the data and the buffer are given as
// host pointers (the adapters, src/hooks/xma.cpp, translate guest addresses).

#pragma once

#include <cstdint>
#include <memory>

namespace audio::xma {

class Stream {
 public:
  Stream();
  ~Stream();
  Stream(const Stream&) = delete;
  Stream& operator=(const Stream&) = delete;

  // Samples a channel the buffer has to hold for `data` (whole packets).
  static uint32_t BufferSamples(const uint8_t* data, uint32_t size);

  // Starts a sound: `data` and `size` the game's XMA data (stream 0),
  // `buffer` room for BufferSamples(data, size) samples a channel.
  bool Begin(const uint8_t* data, uint32_t size, uint32_t channels, uint32_t sample_rate,
             uint8_t* buffer);
  void End();
  bool active() const { return data_ != nullptr; }

  // Where playing starts: the frame at bit offset `bit` of the data, less
  // `skip_subframes` subframes of 128 samples.
  void SetStart(uint32_t bit, uint32_t skip_subframes);
  // The loop: from the frame at `start_bit` (after `skip_subframes`) to the
  // frame at `end_bit` (its subframes 0 to `end_subframe`), `count` times,
  // 255 forever.
  void SetLoop(uint32_t start_bit, uint32_t end_bit, uint32_t skip_subframes,
               uint32_t end_subframe, uint32_t count);

  // Decodes until `ahead` samples past the position are ready (or the data
  // ends).
  void DecodeAhead(uint32_t ahead);

  // Samples ready to read in one go from the position (up to the loop end or
  // what's decoded), and in all (past a loop jump too), at most `cap`.
  uint32_t Contiguous(uint32_t cap) const;
  uint32_t Ready(uint32_t cap) const;
  // Takes up to `count` samples from the position (no further than
  // Contiguous); returns how many. At the loop end, the position jumps back.
  uint32_t Consume(uint32_t count);
  // The position, in samples from the buffer's start.
  uint32_t position() const { return position_; }
  // False once the sound is played to its end with no loops left.
  bool playing() const;

  uint32_t channels() const { return channels_; }
  uint32_t loop_start() const { return loop_start_; }
  uint32_t loop_end() const { return loop_end_; }
  uint32_t decoded() const { return decoded_; }
  uint32_t total() const { return total_; }

 private:
  // Timeline samples (frames x 512 from the data's start) to buffer samples:
  // the decoder drops the first 64 (docs/rexglue-removal.md, B2 step 1).
  static uint32_t ToBuffer(uint32_t timeline);
  void Drain();
  uint32_t Limit() const;

  struct Decoder;
  std::unique_ptr<Decoder> decoder_;
  const uint8_t* data_ = nullptr;
  uint32_t size_ = 0;
  uint32_t channels_ = 0;
  uint8_t* buffer_ = nullptr;
  uint32_t capacity_ = 0;  // samples a channel
  uint32_t fed_ = 0;       // bytes of data given to the decoder
  bool flushed_ = false;
  uint32_t decoded_ = 0;  // samples a channel in the buffer
  uint32_t total_ = 0;    // samples a channel once all is decoded (capacity until then)
  uint32_t position_ = 0;
  uint32_t loop_start_ = 0;
  uint32_t loop_end_ = 0;
  uint32_t loops_left_ = 0;
};

}  // namespace audio::xma
