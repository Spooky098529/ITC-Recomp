// Plays one of the game's movies through our own FFmpeg (src/movies/player.h)
// in real time, as the game's movie class will: the frame due every 1/30 s
// (one a tick, as command 28 draws), the sound taken 256 samples at a time.
// Prints the frames shown, any repeated or skipped, the sound's level, and
// how long it ran; with a third argument, writes the frame at that many
// seconds as a binary PPM (RGB by the movie library's BT.709 numbers,
// docs/native-movies.md). Build and run, from the repo root:
//   cmake --build itc_recomp/out/build/linux-amd64-release --target itc_movie_check
//   itc_recomp/out/build/linux-amd64-release/itc_movie_check \
//     "itc/Import Tuner Challenge (USA)/DATA/TXR/MOVIE/UBI_LOGO.wmv" [seconds out.ppm]

#include "movies/player.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>

namespace {

void WritePpm(const movies::Frame& frame, const char* path) {
  FILE* out = std::fopen(path, "wb");
  if (!out) {
    std::printf("can't write %s\n", path);
    return;
  }
  std::fprintf(out, "P6\n%u %u\n255\n", frame.width, frame.height);
  std::vector<uint8_t> row(size_t(frame.width) * 3);
  auto clamp = [](float v) { return uint8_t(std::clamp(v, 0.0f, 1.0f) * 255.0f + 0.5f); };
  for (uint32_t y = 0; y < frame.height; y++) {
    for (uint32_t x = 0; x < frame.width; x++) {
      size_t c = size_t(y / 2) * (frame.width / 2) + x / 2;
      float luma = 1.164383f * (frame.y[size_t(y) * frame.width + x] / 255.0f - 0.0625f);
      float u = frame.u[c] / 255.0f - 0.5f, v = frame.v[c] / 255.0f - 0.5f;
      row[3 * x + 0] = clamp(luma + 1.792652f * v);
      row[3 * x + 1] = clamp(luma - 0.213238f * u - 0.533004f * v);
      row[3 * x + 2] = clamp(luma + 2.112418f * u);
    }
    std::fwrite(row.data(), 1, row.size(), out);
  }
  std::fclose(out);
  std::printf("wrote %s\n", path);
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    std::printf("usage: itc_movie_check <movie.wmv> [seconds out.ppm]\n");
    return 1;
  }
  std::string error;
  auto player = movies::Player::Open(argv[1], &error);
  if (!player) {
    std::printf("%s: %s\n", argv[1], error.c_str());
    return 1;
  }
  double dump_at = argc > 3 ? std::atof(argv[2]) : -1;
  std::printf("sound: %s\n", player->HasSound() ? "yes" : "no");
  auto start = std::chrono::steady_clock::now();
  player->Start();
  int ticks = 0, shown = 0, repeated = 0, skipped = 0;
  int64_t last_time = -1;
  double sum_squares = 0;
  size_t sound_samples = 0;
  bool dumped = false;
  std::vector<float> audio(512);
  while (!player->Ended()) {
    std::this_thread::sleep_until(start + std::chrono::microseconds(33333LL * (ticks + 1)));
    ticks++;
    // The sound the audio output would have taken in that time.
    for (int k = 0; k < 6; k++) {
      std::fill(audio.begin(), audio.end(), 0.0f);
      player->MixAudio(audio.data(), 256);
      for (float s : audio) {
        sum_squares += double(s) * s;
      }
      sound_samples += 256;
    }
    const movies::Frame* frame = player->CurrentFrame();
    if (!frame) {
      continue;
    }
    if (frame->time_us == last_time) {
      repeated++;
    } else {
      if (last_time >= 0 && frame->time_us - last_time > 40000) {
        skipped++;
      }
      shown++;
      last_time = frame->time_us;
    }
    if (!dumped && dump_at >= 0 && frame->time_us >= int64_t(dump_at * 1e6)) {
      WritePpm(*frame, argv[3]);
      dumped = true;
    }
  }
  double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
  std::printf("ran %.2f s, %d ticks: %d frames shown (last at %.3f s), %d ticks repeated a frame, "
              "%d jumps over a frame\n",
              seconds, ticks, shown, last_time / 1e6, repeated, skipped);
  std::printf("sound: %zu samples taken, RMS %.4f\n", sound_samples,
              std::sqrt(sum_squares / std::max<double>(1, 2.0 * sound_samples)));
  return 0;
}
