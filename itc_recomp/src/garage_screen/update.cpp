// The garage screen's Update (docs/functions/garage-screen.md): see
// garage_screen.h.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "garage_screen/internal.h"

namespace garage_screen {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;

// The transitions, in Update's table (its frame's +96 on): Transition0 to
// Transition17.
constexpr uint32_t kTransitions[18] = {
    0x82195BA0, 0x82192D60, 0x82194F30, 0x82192E20, 0x82195D10, 0x82192EF8,
    0x82194680, 0x821951A8, 0x82195E30, 0x82195410, 0x82195530, 0x82195F10,
    0x821955E0, 0x82192FD0, 0x821948B0, 0x82196030, 0x821930D8, 0x82196170};
// The debug print's format (the empty sub_82106540's r6).
constexpr uint32_t kFormat = 0x8200A3C8;

}  // namespace

uint64_t Update(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82196230 GarageScreen::Update");
  const uint32_t frame = Frame(base, sp, 272);
  calls.Call(kEmpty, frame, 0x82196254, 100, 30, 0, kFormat);
  const uint32_t movie = FindMovie(base, kMovie, frame + 80);
  for (uint32_t i = 0; i < 18; i++) {
    Store(base, frame + 96 + 4 * i, kTransitions[i]);
  }
  // The running transition: its handler from the table on the page
  // object's +24; a nonzero answer (or none below -1) starts the queued one
  // or, with none, the page becomes +4.
  uint64_t r3 = movie;
  uint32_t pages = Load(base, kPages);
  const uint32_t running = Load(base, pages + 12);
  if (running == kNone) {
    if (Load(base, pages + 0) != 4) {
      r3 = calls.Call(kRun, frame, 0x821963F4, movie, 0);
    }
  } else {
    bool finished = true;
    if (int32_t(running) >= 0) {
      // No bounds check: the word at the table's index. r4 to r10 as the
      // original leaves them (the table's build), not read by the
      // transitions.
      const uint32_t target = Load(base, frame + 96 + (running << 2));
      r3 = calls.CallArgs(target, frame, 0x82196380,
                          {pages + 24, kTransitions[11], kTransitions[12], kTransitions[13],
                           kTransitions[14], kTransitions[15], frame + 96, running << 2});
      pages = Load(base, kPages);
      finished = uint32_t(r3) != 0;
    }
    if (finished) {
      const uint32_t queued = Load(base, pages + 16);
      if (queued == kNone) {
        Store(base, pages + 0, Load(base, pages + 4));
        Store(base, pages + 12, kNone);
      } else {
        Store(base, pages + 16, kNone);
        Store(base, pages + 20, 0);
        Store(base, pages + 12, queued);
      }
      Store(base, pages + 24, 0);
    }
  }
  // The stand.
  if (Load(base, Data(base) + 72) == 0) {
    if (Load(base, Data(base) + 84) != 0) {
      r3 = calls.Call(kHideBusyNow, frame, 0x82196510);
      Store(base, Data(base) + 84, 0);
    }
    return r3;
  }
  r3 = calls.Call(kIsBusy, frame, 0x82196418, Holder(base));
  if (uint32_t(r3) != 0) {
    if (Load(base, Data(base) + 84) == 0) {
      r3 = calls.Call(kShowBusy, frame, 0x82196434);
      Store(base, Data(base) + 84, 1);
    }
  } else if (Load(base, Data(base) + 84) != 0) {
    r3 = calls.Call(kHideBusy, frame, 0x8219644C);
    Store(base, Data(base) + 84, 0);
  }
  const uint32_t data = Data(base);
  if (Load(base, data + 76) != 0) {
    return calls.Call(kStageDraw, frame, 0x821964FC, Holder(base), 0, 0);
  }
  // The selected garage car's model (the data's +44 + +48 in the save's
  // garage, its id at the car's +12).
  const uint32_t record =
      GarageRecord(base, Load(base, data + 36), Load(base, data + 48) + Load(base, data + 44));
  const uint64_t block = calls.Call(kBlockOf, frame, 0x821964A8, Load(base, record + 224));
  r3 = calls.Call(kReloadModel, frame, 0x821964B8, Holder(base), block);
  Store(base, Data(base) + 76, uint32_t(r3));
  if (Load(base, Data(base) + 76) == 0) {
    return r3;
  }
  calls.Call(kPutCarOnStand, frame, 0x821964D4);
  return calls.Call(kStageCommand, frame, 0x821964E4, Holder(base), 15);
}

}  // namespace garage_screen
