// The 852-byte modes' releases (docs/functions/front-end-screen-helpers.md):
// see front_end_screen_helpers.h.

#include "front_end_screen_helpers/front_end_screen_helpers.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"
#include "schema/menu_sounds.h"
#include "schema/network_mode.h"
#include "schema/strings_and_font.h"
#include "schema/title_screen.h"
#include "schema/todays_result.h"
#include "schema/type10_mode.h"
#include "schema/type2_mode.h"
#include "schema/type5_mode.h"
#include "type67_modes/type67_modes.h"

namespace front_end_screen_helpers {

namespace {

void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(frame, sp);
  return frame;
}

// The sound manager's slot (kStopCueSlot or kIsCuePlayingSlot) with r4 =
// the bank's word, r5 = the cue's.
uint64_t CallSlot(const Calls& calls, uint32_t slot, uint32_t frame, uint32_t lr,
                  uint32_t bank, uint32_t cue) {
  const schema::AudioManager manager = schema::MenuSoundsGlobals::sound_manager();
  const schema::AudioManagerVtable vtable = manager.vtable();
  const uint32_t target = slot == kStopCueSlot ? vtable.stop_cue() : vtable.is_cue_playing();
  return calls.Call(target, frame, lr, manager.address, bank, cue);
}

// The cue `cue` of bank 1 stopped when it's playing: its indexes at the
// frame's `at` (cue) and `at` + 4 (bank) for IsCuePlaying, `at` + 8 and
// + 12 for StopCue. r3 as the calls leave it.
uint64_t StopIfPlaying(const Calls& calls, uint32_t frame, uint32_t at,
                       uint32_t cue, uint32_t playing_lr, uint32_t stop_lr) {
  Store(frame + at, cue);
  Store(frame + at + 4, 1);
  uint64_t r3 =
      CallSlot(calls, kIsCuePlayingSlot, frame, playing_lr, frame + at + 4, frame + at);
  if (uint8_t(r3) != 0) {
    Store(frame + at + 8, cue);
    Store(frame + at + 12, 1);
    r3 = CallSlot(calls, kStopCueSlot, frame, stop_lr, frame + at + 12, frame + at + 8);
  }
  return r3;
}

// VirtualFree(the mode's block, 0, MEM_RELEASE) over the allocation layer,
// then the block = 0. VirtualFree's r3. `Mode` the mode type's view.
template <typename Mode>
uint64_t FreeBlock(const Mode& mode) {
  const uint64_t r3 = memory::FreePages(mode.template block<schema::View>().address) ? 1 : 0;
  mode.set_block(0u);
  return r3;
}

void StandSceneRelease(const Calls& calls, uint32_t frame, uint32_t lr,
                       uint64_t r3) {
  if (!type67_modes::StandSceneRelease()) {
    calls.Call(kStandSceneRelease, frame, lr, r3);
  }
}

// Strings::Release([0x826C5C34], files), then the jump to
// UiMovies::DeleteAll (its r4 0, as StandMode::Release's native code
// passes it). The jump's r3.
uint64_t ReleaseStringsAndMovies(const Calls& calls, uint32_t frame,
                                 uint32_t files, uint32_t strings_lr, uint32_t jump_lr) {
  const uint64_t r3 = calls.Call(kStringsRelease, frame, strings_lr,
                                schema::StringsAndFontGlobals::strings().address, files);
  return calls.Call(kDeleteAllJump, frame, jump_lr, r3, 0);
}

template <typename Mode>
bool Ready(const Mode& mode) {
  return memory::IsPages(mode.template block<schema::View>().address);
}

}  // namespace

std::optional<uint64_t> ModeType2Release(uint32_t mode, uint32_t sp,
                                         const Calls& calls) {
  const schema::ModeType2 m(mode);
  if (!Ready(m)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82196EC0 ModeType2::Release");
  const uint32_t frame = Frame(sp, 144);
  FreeBlock(m);
  StopIfPlaying(calls, frame, 80, 0, 0x82196F18, 0x82196F44);
  const uint64_t r3 = StopIfPlaying(calls, frame, 96, 2, 0x82196F6C, 0x82196F98);
  StandSceneRelease(calls, frame, 0x82196F9C, r3);
  return ReleaseStringsAndMovies(calls, frame, 0x100BB, 0x82196FB0, 0x82196FB4);
}

std::optional<uint64_t> ModeType4Release(uint32_t mode, uint32_t sp,
                                         const Calls& calls) {
  const schema::ModeType4 m(mode);
  if (!Ready(m)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82224AB8 ModeType4::Release");
  const uint32_t frame = Frame(sp, 112);
  FreeBlock(m);
  ReleaseStringsAndMovies(calls, frame, 3, 0x82224AF8, 0x82224AFC);
  Store(frame + 80, 1);
  Store(frame + 84, 1);
  return CallSlot(calls, kStopCueSlot, frame, 0x82224B28, frame + 84, frame + 80);
}

std::optional<uint64_t> ModeType5Release(uint32_t mode, uint32_t sp,
                                         const Calls& calls) {
  const schema::ModeType5 m(mode);
  if (!Ready(m)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8222FB90 ModeType5::Release");
  const uint32_t frame = Frame(sp, 128);
  FreeBlock(m);
  StopIfPlaying(calls, frame, 80, 3, 0x8222FBEC, 0x8222FC18);
  return ReleaseStringsAndMovies(calls, frame, 59, 0x8222FC28, 0x8222FC2C);
}

std::optional<uint64_t> ModeType9Release(uint32_t mode, uint32_t sp,
                                         const Calls& calls) {
  const schema::ModeType9 m(mode);
  if (!Ready(m)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8225D590 ModeType9::Release");
  const uint32_t frame = Frame(sp, 128);
  FreeBlock(m);
  StopIfPlaying(calls, frame, 80, 2, 0x8225D5EC, 0x8225D618);
  return ReleaseStringsAndMovies(calls, frame, 171, 0x8225D628, 0x8225D62C);
}

std::optional<uint64_t> ModeType10Release(uint32_t mode, uint32_t sp,
                                          const Calls& calls) {
  const schema::ModeType10 m(mode);
  if (!Ready(m)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82288898 ModeType10::Release");
  const uint32_t frame = Frame(sp, 96);
  FreeBlock(m);
  return ReleaseStringsAndMovies(calls, frame, 11, 0x822888D8, 0x822888DC);
}

std::optional<uint64_t> ModeType12Release(uint32_t mode, uint32_t sp,
                                          const Calls& calls) {
  const schema::ModeType12 m(mode);
  if (!Ready(m)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82277DB0 ModeType12::Release");
  const uint32_t frame = Frame(sp, 96);
  const uint64_t r3 = FreeBlock(m);
  StandSceneRelease(calls, frame, 0x82277DE4, r3);
  return ReleaseStringsAndMovies(calls, frame, 59, 0x82277DF4, 0x82277DF8);
}

}  // namespace front_end_screen_helpers
