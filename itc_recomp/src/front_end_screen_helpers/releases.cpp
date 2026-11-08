// The 852-byte modes' releases (docs/functions/front-end-screen-helpers.md):
// see front_end_screen_helpers.h.

#include "front_end_screen_helpers/front_end_screen_helpers.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"
#include "type67_modes/type67_modes.h"

namespace front_end_screen_helpers {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// The sound manager's slot (bytes into its vtable) with r4 = the bank's
// word, r5 = the cue's.
uint64_t CallSlot(uint8_t* base, const Calls& calls, uint32_t slot, uint32_t frame, uint32_t lr,
                  uint32_t bank, uint32_t cue) {
  const uint32_t manager = Load(base, kSoundManager);
  return calls.Call(Load(base, Load(base, manager) + slot), frame, lr, manager, bank, cue);
}

// The cue `cue` of bank 1 stopped when it's playing: its indexes at the
// frame's `at` (cue) and `at` + 4 (bank) for IsCuePlaying, `at` + 8 and
// + 12 for StopCue. r3 as the calls leave it.
uint64_t StopIfPlaying(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t at,
                       uint32_t cue, uint32_t playing_lr, uint32_t stop_lr) {
  Store(base, frame + at, cue);
  Store(base, frame + at + 4, 1);
  uint64_t r3 =
      CallSlot(base, calls, kIsCuePlayingSlot, frame, playing_lr, frame + at + 4, frame + at);
  if (uint8_t(r3) != 0) {
    Store(base, frame + at + 8, cue);
    Store(base, frame + at + 12, 1);
    r3 = CallSlot(base, calls, kStopCueSlot, frame, stop_lr, frame + at + 12, frame + at + 8);
  }
  return r3;
}

// VirtualFree(mode +848, 0, MEM_RELEASE) over the allocation layer, then
// +848 = 0. VirtualFree's r3.
uint64_t FreeBlock(uint8_t* base, uint32_t mode) {
  const uint64_t r3 = memory::FreePages(Load(base, mode + kModeBlock)) ? 1 : 0;
  Store(base, mode + kModeBlock, 0);
  return r3;
}

void StandSceneRelease(uint8_t* base, const Calls& calls, uint32_t frame, uint32_t lr,
                       uint64_t r3) {
  if (!type67_modes::StandSceneRelease(base)) {
    calls.Call(kStandSceneRelease, frame, lr, r3);
  }
}

// Strings::Release([0x826C5C34], files), then the jump to
// UiMovies::DeleteAll (its r4 0, as StandMode::Release's native code
// passes it). The jump's r3.
uint64_t ReleaseStringsAndMovies(uint8_t* base, const Calls& calls, uint32_t frame,
                                 uint32_t files, uint32_t strings_lr, uint32_t jump_lr) {
  const uint64_t r3 =
      calls.Call(kStringsRelease, frame, strings_lr, Load(base, kStringTable), files);
  return calls.Call(kDeleteAllJump, frame, jump_lr, r3, 0);
}

bool Ready(uint8_t* base, uint32_t mode) {
  return memory::IsPages(Load(base, mode + kModeBlock));
}

}  // namespace

std::optional<uint64_t> ModeType2Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                         const Calls& calls) {
  if (!Ready(base, mode)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82196EC0 ModeType2::Release");
  const uint32_t frame = Frame(base, sp, 144);
  FreeBlock(base, mode);
  StopIfPlaying(base, calls, frame, 80, 0, 0x82196F18, 0x82196F44);
  const uint64_t r3 = StopIfPlaying(base, calls, frame, 96, 2, 0x82196F6C, 0x82196F98);
  StandSceneRelease(base, calls, frame, 0x82196F9C, r3);
  return ReleaseStringsAndMovies(base, calls, frame, 0x100BB, 0x82196FB0, 0x82196FB4);
}

std::optional<uint64_t> ModeType4Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                         const Calls& calls) {
  if (!Ready(base, mode)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82224AB8 ModeType4::Release");
  const uint32_t frame = Frame(base, sp, 112);
  FreeBlock(base, mode);
  ReleaseStringsAndMovies(base, calls, frame, 3, 0x82224AF8, 0x82224AFC);
  Store(base, frame + 80, 1);
  Store(base, frame + 84, 1);
  return CallSlot(base, calls, kStopCueSlot, frame, 0x82224B28, frame + 84, frame + 80);
}

std::optional<uint64_t> ModeType5Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                         const Calls& calls) {
  if (!Ready(base, mode)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8222FB90 ModeType5::Release");
  const uint32_t frame = Frame(base, sp, 128);
  FreeBlock(base, mode);
  StopIfPlaying(base, calls, frame, 80, 3, 0x8222FBEC, 0x8222FC18);
  return ReleaseStringsAndMovies(base, calls, frame, 59, 0x8222FC28, 0x8222FC2C);
}

std::optional<uint64_t> ModeType9Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                         const Calls& calls) {
  if (!Ready(base, mode)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8225D590 ModeType9::Release");
  const uint32_t frame = Frame(base, sp, 128);
  FreeBlock(base, mode);
  StopIfPlaying(base, calls, frame, 80, 2, 0x8225D5EC, 0x8225D618);
  return ReleaseStringsAndMovies(base, calls, frame, 171, 0x8225D628, 0x8225D62C);
}

std::optional<uint64_t> ModeType10Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                          const Calls& calls) {
  if (!Ready(base, mode)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82288898 ModeType10::Release");
  const uint32_t frame = Frame(base, sp, 96);
  FreeBlock(base, mode);
  return ReleaseStringsAndMovies(base, calls, frame, 11, 0x822888D8, 0x822888DC);
}

std::optional<uint64_t> ModeType12Release(uint8_t* base, uint32_t mode, uint32_t sp,
                                          const Calls& calls) {
  if (!Ready(base, mode)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82277DB0 ModeType12::Release");
  const uint32_t frame = Frame(base, sp, 96);
  const uint64_t r3 = FreeBlock(base, mode);
  StandSceneRelease(base, calls, frame, 0x82277DE4, r3);
  return ReleaseStringsAndMovies(base, calls, frame, 59, 0x82277DF4, 0x82277DF8);
}

}  // namespace front_end_screen_helpers
