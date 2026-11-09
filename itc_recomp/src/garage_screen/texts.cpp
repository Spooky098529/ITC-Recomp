// The garage screen's movie texts and markers never seen in play
// (docs/functions/garage-screen.md): see garage_screen.h.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "garage_screen/internal.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace garage_screen {

namespace {

constexpr uint64_t kAll = 0xFFFFFFFFFFFFFFFFull;  // -1 (the root's sub-scene)
constexpr uint32_t kDialogMovie = 16;
constexpr uint32_t kDialogCursor = 40960;  // the dialog movie's cursor word

}  // namespace

uint64_t SetScrollArrows(uint8_t* base, uint32_t scroll, uint32_t count, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82193288 GarageScreen::SetScrollArrows");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  const uint32_t up = ui_movies::FindNode(base, movie, 6, 3);
  const uint32_t down = ui_movies::FindNode(base, movie, 6, 4);
  // Up shown past the first row; down shown at the first row, else unless
  // the last four rows show (compared for equality, as the original).
  if (scroll == 0) {
    Store(base, up + 80, 0);
    Store(base, down + 80, 1);
  } else {
    Store(base, up + 80, 1);
    Store(base, down + 80, scroll + 4 != count ? 1 : 0);
  }
  return down;
}

uint64_t SetActionMarker(uint8_t* base, uint32_t action, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821936B0 GarageScreen::SetActionMarker");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  const double y = IntToFloat(base, frame + 80, action);
  const uint32_t node = ui_movies::FindNode(base, movie, 7, 0);
  Stfs(base, node + 16, y);
  return node;
}

uint64_t SetOptionMarker(uint8_t* base, uint32_t option, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82193720 GarageScreen::SetOptionMarker");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  const uint32_t node = ui_movies::FindNode(base, movie, 2, 1);
  const double y = IntToFloat(base, frame + 80, option);
  const uint32_t child = ui_movies::Child(base, node, 0);
  Stfs(base, child + 16, y);
  return child;
}

uint64_t ShowSpecTexts(uint8_t* base, uint32_t record, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193798 GarageScreen::ShowSpecTexts");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  SetText(calls, frame, 0x821937E4, movie, 18, 5, uint64_t(Load(base, record + 0)) + 421);
  SetText(calls, frame, 0x821937FC, movie, 18, 6, uint64_t(Load(base, record + 4)) + 426);
  SetText(calls, frame, 0x82193814, movie, 18, 7, 8, Load(base, record + 12));
  {
    // The weight's integer through the doubleword at +80, scaled by the
    // double at 0x82009F00 in single precision, truncated into +80 (stfiwx;
    // r10 the word's address).
    const double f13 = IntToFloat(base, frame + 80, Load(base, record + 8));
    const double f0 = double(float(f13 * Lfd(base, 0x82009F00)));
    Store(base, frame + 80, Fctiwz(f0));
  }
  SetText(calls, frame, 0x8219385C, movie, 18, 8, 8, Load(base, frame + 80), frame + 80);
  SetText(calls, frame, 0x82193874, movie, 18, 11, 8, Load(base, record + 16));
  {
    // A float scaled by the double at 0x82009F08, passed as a double in f1
    // and in r7 (its bits through the frame's +48).
    const double f1 = double(float(Lfs(base, record + 20) * Lfd(base, 0x82009F08)));
    const uint64_t bits = std::bit_cast<uint64_t>(f1);
    Store64(base, frame + 48, bits);
    const uint64_t args[8] = {movie, 18, 12, 436, bits, 0, 0, 0};
    calls.call(calls.user, kSetText, args, &f1, frame, 0x821938A4);
  }
  SetText(calls, frame, 0x821938B8, movie, 18, 9, 438);
  SetText(calls, frame, 0x821938CC, movie, 18, 10, 440);
  SetText(calls, frame, 0x821938E0, movie, 18, 13, 437);
  return SetText(calls, frame, 0x821938F4, movie, 18, 14, 439);
}

uint64_t ShowCarNames(uint8_t* base, uint64_t id, uint64_t maker, uint32_t sp,
                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193910 GarageScreen::ShowCarNames");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  SetText(calls, frame, 0x8219395C, movie, kAll, 19, maker + 1934);
  SetText(calls, frame, 0x82193970, movie, kAll, 20, id + 1646);
  return SetText(calls, frame, 0x82193984, movie, kAll, 21, id + 1718);
}

uint64_t SetPlateQuestion(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193320 GarageScreen::SetPlateQuestion");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kDialogMovie, frame + 80);
  Store(base, movie + kDialogCursor, 0);
  const uint32_t question = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 0);
  const uint32_t yes = ui_movies::FindNode(base, movie, 1, 3);
  const uint32_t no = ui_movies::FindNode(base, movie, 1, 4);
  calls.Call(kSetNodeText, frame, 0x821933A4, movie, question, 96);
  calls.Call(kSetNodeText, frame, 0x821933B4, movie, yes, 2);
  return calls.Call(kSetNodeText, frame, 0x821933C4, movie, no, 3);
}

}  // namespace garage_screen
