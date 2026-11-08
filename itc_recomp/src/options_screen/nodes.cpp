// The options screen's node helpers on its movie
// (docs/functions/options-screen.md): see options_screen.h.

#include "options_screen/options_screen.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace options_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

// A helper's frame (`bytes` below `sp`, its back chain) and `m`.
uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// A value's 64-bit word (extsw, std at frame + 80), as the originals leave
// it.
void StoreWord(uint8_t* base, uint32_t at, int64_t word) {
  StoreU32(base, at, uint32_t(uint64_t(word) >> 32));
  StoreU32(base, at + 4, uint32_t(word));
}

// 16 - trunc((value + 10) x [0x8200A6AC]) as the sliders' x, with the
// original's frame words at frame + 80 (the last its 64-bit result).
double SliderX(uint8_t* base, uint64_t value, uint32_t frame) {
  using namespace camera_modes::steps;
  const int64_t shifted = int64_t(int32_t(uint32_t(value + 10)));
  StoreU32(base, frame + 84, uint32_t(shifted));
  const int32_t truncated = Fctiwz(Fmuls(Single(double(shifted)), Lfs(base, 0x8200A6AC)));
  const int64_t rest = int64_t(int32_t(16u - uint32_t(truncated)));
  StoreWord(base, frame + 80, rest);
  return Single(double(rest));
}

// The pictures: FindNode(m, sub_scene, 6 + i) for i below `count`, +80 = 1
// on `shown`, else 0. The last node.
uint32_t ShowPicture(uint8_t* base, uint32_t shown, uint32_t sub_scene, int32_t count,
                     uint32_t sp) {
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = FindMovie(base, frame);
  uint32_t node = 0;
  for (int32_t i = 0; i < count; i++) {
    node = ui_movies::FindNode(base, m, sub_scene, uint32_t(i) + 6);
    StoreU32(base, node + 80, int32_t(shown) == i ? 1 : 0);
  }
  return node;
}

}  // namespace

uint32_t SetCategoryArrow(uint8_t* base, uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BE218 Options::SetCategoryArrow");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t node = ui_movies::FindNode(base, FindMovie(base, frame), 0xFFFFFFFFu, 21);
  const int64_t word = int64_t(int32_t(uint32_t(value)));
  StoreWord(base, frame + 80, word);
  const uint32_t child = ui_movies::Child(base, node, 0);
  StoreU32(base, child + 16, std::bit_cast<uint32_t>(float(double(word))));
  return child;
}

uint64_t ShowVolumeNumber(uint8_t* base, uint32_t node, uint64_t volume, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BE518 Options::ShowVolumeNumber");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = FindMovie(base, frame);
  const uint32_t text = ui_movies::Child(base, node, 2);
  // UiMovie::SetNodeText: its unset varargs (r7 to r10) 0 (the user's call,
  // 2026-10-30).
  calls.Call(kSetNodeText, frame, 0x821BE56C, m, text, 8, volume);
  uint32_t child = 0;
  for (int32_t i = 1; i <= 15; i++) {
    child = ui_movies::Child(base, node, uint32_t(i) + 3);
    StoreU32(base, child + 80, i == int32_t(uint32_t(volume)) ? 1 : 0);
  }
  return child;
}

uint64_t ShowMusicName(uint8_t* base, uint64_t value, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BE930 Options::ShowMusicName");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = FindMovie(base, frame);
  // UiMovie::SetText(m, 6, 8, 501 + value): its unset varargs 0.
  return calls.Call(kSetText, frame, 0x821BE970, m, 6, 8, value + 501);
}

uint32_t ShowSlider(uint8_t* base, uint32_t slider, uint64_t value, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BE988 Options::ShowSlider");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = FindMovie(base, frame);
  const uint32_t text = ui_movies::Child(base, slider, 0);
  calls.Call(kSetNodeText, frame, 0x821BE9E0, m, text, 8, value);
  const double x = SliderX(base, value, frame);
  const uint32_t bar = ui_movies::Child(base, slider, 2);
  camera_modes::steps::Stfs(base, bar + 12, x);
  return bar;
}

uint32_t ShowSliderMark(uint8_t* base, uint32_t slider, uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BEA48 Options::ShowSliderMark");
  const uint32_t frame = Frame(base, sp, 112);
  const double x = SliderX(base, value, frame);
  const uint32_t mark = ui_movies::Child(base, slider, 3);
  camera_modes::steps::Stfs(base, mark + 12, x);
  return mark;
}

uint32_t ShowButtonPicture(uint8_t* base, uint32_t shown, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BF0F0 Options::ShowButtonPicture");
  return ShowPicture(base, shown, 9, 17, sp);
}

uint32_t ShowAnalogPicture(uint8_t* base, uint32_t shown, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BF630 Options::ShowAnalogPicture");
  return ShowPicture(base, shown, 11, 11, sp);
}

uint64_t ResetNode(uint8_t* base, uint32_t group, uint32_t child, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BFA68 Options::ResetNode");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t m = FindMovie(base, frame);
  auto node = [&](uint32_t scene) {
    const uint32_t root = LoadU32(base, m + scene);
    return ui_movies::Child(
        base, int32_t(group) == -1 ? root : ui_movies::Child(base, root, group), child);
  };
  const uint32_t file = node(ui_movies::kMovieFile);
  const uint32_t live = node(ui_movies::kMovieLive);
  const uint32_t saved = node(ui_movies::kMovieSaved);
  calls.Call(kRestorePose, frame, 0x821BFB24, m, live, file, 0);
  return calls.Call(kRestorePose, frame, 0x821BFB3C, m, saved, file, 0);
}

}  // namespace options_screen
