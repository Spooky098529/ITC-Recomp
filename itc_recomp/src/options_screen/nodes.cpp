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

uint32_t ShowVolumeBar(uint8_t* base, uint32_t node, uint64_t volume, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BE5A8 Options::ShowVolumeBar");
  Frame(base, sp, 112);
  uint32_t child = 0;
  for (int32_t i = 1; i <= 15; i++) {
    child = ui_movies::Child(base, node, uint32_t(i) + 21);
    StoreU32(base, child + 80, i <= int32_t(uint32_t(volume)) ? 1 : 0);
  }
  return child;
}

uint64_t ShowVolumeNumbers(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BE820 Options::ShowVolumeNumbers");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t m = FindMovie(base, frame);
  const uint32_t music = ui_movies::FindNode(base, m, 6, 6);
  ShowVolumeNumber(base, music, LoadU32(base, LoadU32(base, kSettings) + 252), frame, calls);
  const uint32_t effects = ui_movies::FindNode(base, m, 6, 7);
  return ShowVolumeNumber(base, effects, LoadU32(base, LoadU32(base, kSettings) + 256), frame,
                          calls);
}

namespace {

// The three slider edits (o +260 to +268) copied to the frame's +88, node
// (7, 0), and `show` on its children 4 to 6 with each value. A 144-byte
// frame. `show`'s last answer.
template <typename Show>
uint64_t ShowSliders(uint8_t* base, uint32_t sp, Show show) {
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t m = FindMovie(base, frame);
  const uint32_t o = LoadU32(base, kSettings);
  for (uint32_t i = 0; i < 3; i++) {
    StoreU32(base, frame + 88 + 4 * i, LoadU32(base, o + 260 + 4 * i));
  }
  const uint32_t node = ui_movies::FindNode(base, m, 7, 0);
  uint64_t r3 = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t slider = ui_movies::Child(base, node, i + 4);
    r3 = show(slider, uint64_t(LoadU32(base, frame + 88 + 4 * i)), frame);
  }
  return r3;
}

}  // namespace

uint64_t ShowSliderNumbers(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821BECA8 Options::ShowSliderNumbers");
  return ShowSliders(base, sp, [&](uint32_t slider, uint64_t value, uint32_t frame) {
    return uint64_t(ShowSlider(base, slider, value, frame, calls));
  });
}

uint64_t ShowSliderMarks(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BED38 Options::ShowSliderMarks");
  return ShowSliders(base, sp, [&](uint32_t slider, uint64_t value, uint32_t frame) {
    return uint64_t(ShowSliderMark(base, slider, value, frame));
  });
}

uint64_t ShowHudValues(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821BE290 Options::ShowHudValues");
  const uint32_t frame = Frame(base, sp, 208);
  const uint32_t m = FindMovie(base, frame);
  const uint32_t o = LoadU32(base, kSettings);
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(base, frame + 96 + 4 * i, LoadU32(base, o + 236 + 4 * i));
  }
  StoreU32(base, frame + 112, LoadU32(base, o + 660));
  const uint32_t group = ui_movies::FindNode(base, m, 5, 2);
  uint64_t r3 = group;
  for (int32_t i = 0; i < int32_t(LoadU32(base, group + 104)); i++) {
    const uint32_t row = ui_movies::FindNode(base, m, 5, LoadU32(base, group + 100) + uint32_t(i));
    const uint32_t left = ui_movies::FindNode(base, m, 5, LoadU32(base, row + 100));
    const uint32_t left_frame = ui_movies::Child(base, left, 0);
    const uint32_t left_shown = ui_movies::Child(base, left, 2);
    const uint32_t right = ui_movies::FindNode(base, m, 5, LoadU32(base, row + 100) + 1);
    const uint32_t right_frame = ui_movies::Child(base, right, 0);
    const uint32_t right_shown = ui_movies::Child(base, right, 2);
    // The value as the frame holds it (rows past the five read the frame's
    // next words, as the original does).
    const uint32_t on = int32_t(LoadU32(base, frame + 96 + 4 * uint32_t(i))) == 1 ? 1 : 0;
    StoreU32(base, left_frame + 108, on ^ 1);
    StoreU32(base, left_shown + 80, on);
    StoreU32(base, right_frame + 108, on);
    StoreU32(base, right_shown + 80, on ^ 1);
    r3 = right_shown;
  }
  return r3;
}

}  // namespace options_screen
