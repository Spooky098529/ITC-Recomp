// A movie played through our own FFmpeg (the asf demuxer, the wmv3 and wmav2
// decoders), in place of the movie library linked into the game: the file
// decoded on a thread of its own a little ahead of a clock, the frames as
// three 8-bit planes (YUV 4:2:0) for the renderer to draw, and the sound as
// 48 kHz stereo for our audio output. Plain C++. See docs/native-movies.md.

#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace movies {

// A decoded frame: the Y plane (width x height) and the two chroma planes
// (width / 2 x height / 2), each row after row with no padding.
struct Frame {
  int64_t time_us = 0;  // from the movie's start
  uint32_t width = 0, height = 0;
  std::vector<uint8_t> y, u, v;
};

class Player {
 public:
  // Opens a movie and starts decoding it; nothing if the file can't be read
  // or has no WMV3 video (and why in `error`).
  static std::unique_ptr<Player> Open(const std::filesystem::path& path,
                                      std::string* error = nullptr);
  ~Player();
  Player(const Player&) = delete;
  Player& operator=(const Player&) = delete;

  // Whether it has 48 kHz stereo WMA v2 sound (it plays silent otherwise).
  bool HasSound() const;

  // The clock: stopped until Start, held while paused.
  void Start();
  void Pause();
  void Resume();

  // The frame due at the clock's time (the first before Start), or null
  // when none is decoded yet. Valid until the next call. Any thread, but one
  // at a time.
  const Frame* CurrentFrame();

  // Whether the clock has passed the last frame's end.
  bool Ended();

  // Adds the next `samples` stereo samples (interleaved, left then right) to
  // `out` while the clock runs; nothing while stopped or paused, and
  // silence where the decoder is behind. The audio thread's.
  void MixAudio(float* out, size_t samples);

 private:
  struct Impl;
  explicit Player(std::unique_ptr<Impl> impl);
  std::unique_ptr<Impl> impl_;
};

// The movie whose sound goes to our audio output (set while one plays).
void SetSounding(Player* player);
// Our audio output's thread: adds the sounding movie's next samples, as
// MixAudio.
void MixSoundingAudio(float* out, size_t samples);

}  // namespace movies
