// The tuning screen machine's handlers (docs/functions/tuning-machine.md):
// see tuning_machine.h.

#include "tuning_machine/tuning_machine.h"

#include "base/log.h"
#include "tuning_pages/tuning_pages.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace tuning_machine {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemCommit = 4096;
constexpr uint32_t kReadWrite = 4;
constexpr uint32_t kMemRelease = 0x8000;
// The float words the originals load (lfs) and store (stfs) unchanged.
constexpr uint32_t kZeroFloat = 0x82000A34;  // 0.0
constexpr uint32_t kOneFloat = 0x82000A30;   // 1.0

inline uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
inline void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(base, frame, sp);
  return frame;
}

// UiMovies::Find with the id stored at `key` (the original's frame word):
// the pair's value's address.
inline uint32_t FindPair(uint8_t* base, uint32_t id, uint32_t key) {
  Store(base, key, id);
  return ui_movies::PairsFind(base, ui_movies::kMovieTable, key);
}

// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const uint32_t ring = mode + 204;
  const uint32_t count = Load(base, ring + 600) + 1;
  const uint32_t start = Load(base, ring + 604);
  Store(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
  Store(base, slot + 0, id);
  Store(base, slot + 4, 0);
  Store(base, slot + 8, 0);
}

// A front-end menu's first words, as Create writes them (+44, +56 and +60
// not written).
void ResetMenu(uint8_t* base, uint32_t menu) {
  Store(base, menu + 12, 0);
  Store(base, menu + 16, 6);
  Store(base, menu + 20, 1);
  for (const uint32_t field : {0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + field, 0);
  }
}

}  // namespace

void ZeroRecord16(uint8_t* base, uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821FE540 TuningMachine::ZeroRecord16");
  const uint32_t zero = Load(base, kZeroFloat);
  Store(base, record + 8, zero);
  Store(base, record + 0, 0);
  Store(base, record + 12, zero);
  Store(base, record + 4, 0);
}

void ResetImages(uint8_t* base, uint32_t images) {
  ITC_NATIVE_RUNNING("sub_821FE590 DecalImages::Reset");
  for (uint32_t i = 0; i < 200; i++) {
    const uint32_t entry = images + 36 * i;
    for (uint32_t k = 4; k < 36; k += 4) {
      Store(base, entry + k, 0);
    }
    Store(base, entry + 0, kNone);
  }
  Store(base, images + 8852, 0);
}

uint64_t Create(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821CE4C8 TuningMachine::Create");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  // The screen kept in the caller's parameter area (read back before the
  // menus are written).
  Store(base, sp + 20, s);
  Store(base, s + 9608, 0);
  Store(base, s + 9652, 0);
  Store(base, s + 9656, 0);
  for (uint32_t b = 9660; b <= 9668; b++) {
    guest::StoreU8(base, s + b, 0);
  }
  for (uint32_t b = 9676; b <= 9680; b++) {
    guest::StoreU8(base, s + b, 0);
  }
  guest::StoreU8(base, s + 10008, 0);
  Store(base, s + 9708, 1);
  Store(base, s + 9712, 0);
  Store(base, s + 9728, 1);
  Store(base, s + 9684, 17);
  for (uint32_t w = 9688; w <= 9704; w += 4) {
    Store(base, s + w, kNone);
  }
  for (uint32_t w = 9740; w <= 9752; w += 4) {
    Store(base, s + w, 0);
  }
  Store(base, s + 10012, kNone);
  Store(base, s + 10016, 0);
  for (uint32_t w = 10020; w <= 10032; w += 4) {
    Store(base, s + w, kNone);
  }
  Store(base, s + 10036, 0);
  Store(base, s + 10040, 0);
  for (uint32_t w = 25196; w <= 25244; w += 4) {
    Store(base, s + w, 0);
  }
  for (uint32_t w = 30440; w <= 30472; w += 4) {
    Store(base, s + w, 0);
  }
  for (uint32_t w = 30492; w <= 30576; w += 4) {
    Store(base, s + w, 0);
  }
  for (uint32_t w = 30724; w <= 30756; w += 4) {
    Store(base, s + w, 0);
  }
  Store(base, s + 30764, 0);
  Store(base, s + 30768, 0);
  Store(base, s + 9672, 0);
  for (uint32_t w = 30772; w <= 30792; w += 4) {
    Store(base, s + w, 0);
  }
  // The vinyl layers' ids: two passes of 0, 1, 2 (the second word list
  // runs on, the third is written again).
  {
    uint32_t r10 = s + 31192;
    for (uint32_t pass = 0; pass < 2; pass++) {
      uint32_t r9 = s + 31216;
      for (uint32_t j = 0; j < 3; j++) {
        Store(base, r10 - 24, j);
        Store(base, r10, j);
        r10 += 4;
        Store(base, r9, j);
        r9 += 4;
      }
    }
  }
  // The stickers' ids: eight passes of 0 to 4.
  {
    uint32_t r10 = s + 33320;
    for (uint32_t pass = 0; pass < 8; pass++) {
      uint32_t r9 = s + 33480;
      for (uint32_t k = 0; k < 5; k++) {
        Store(base, r10 - 1120, k);
        Store(base, r10, k);
        r10 += 4;
        Store(base, r9, k);
        r9 += 4;
      }
    }
  }
  const uint32_t zero = Load(base, kZeroFloat);
  const uint32_t one = Load(base, kOneFloat);
  Store(base, s + 30832, 0);
  Store(base, s + 30836, 0);
  Store(base, s + 30848, 0);
  Store(base, s + 30852, 0);
  Store(base, s + 30840, zero);
  Store(base, s + 30844, one);
  // The decal records: 24 bytes each, +8 0.0 and +12 1.0.
  const auto record = [&](uint32_t at) {
    Store(base, at + 8, zero);
    Store(base, at + 0, 0);
    Store(base, at + 12, one);
    Store(base, at + 4, 0);
    Store(base, at + 16, 0);
    Store(base, at + 20, 0);
  };
  for (uint32_t j = 0; j < 6; j++) {
    record(s + 30880 + 24 * j);
    record(s + 31024 + 24 * j);
  }
  for (uint32_t j = 0; j < 40; j++) {
    record(s + 31240 + 24 * j);
    record(s + 32360 + 24 * j);
  }
  for (uint32_t w = 33540; w <= 33580; w += 4) {
    Store(base, s + w, 0);
  }
  Store(base, s + 25120, 0);
  Store(base, s + 20768, 0);
  Store(base, s + 20772, 0);
  Store(base, s + 20776, 0);
  Store(base, s + 20780, zero);
  Store(base, s + 36824, 0);
  ZeroRecord16(base, s + 30800);
  ZeroRecord16(base, s + 30816);
  for (uint32_t k = 0; k < 63; k++) {
    Store(base, s + 9756 + 4 * k, 0);
  }
  Store(base, s + kQueue,
        uint32_t(calls.Call(kVirtualAlloc, frame, 0x821CE8D8, 0, 28, kMemCommit, kReadWrite)));
  Store(base, s + kMenu,
        uint32_t(calls.Call(kVirtualAlloc, frame, 0x821CE8F4, 0, 68, kMemCommit, kReadWrite)));
  Store(base, s + kMenu2,
        uint32_t(calls.Call(kVirtualAlloc, frame, 0x821CE910, 0, 68, kMemCommit, kReadWrite)));
  ResetImages(base, s + kImages);
  // The queue: TUNING SHOP MAIN (3); +16 not written.
  const uint32_t queue = Load(base, s + kQueue);
  const uint32_t screen = Load(base, sp + 20);
  Store(base, queue + 4, 3);
  Store(base, queue + 0, 3);
  Store(base, queue + 8, 3);
  Store(base, queue + 20, kNone);
  Store(base, queue + 12, kNone);
  Store(base, queue + 24, 0);
  ResetMenu(base, Load(base, screen + kMenu));
  ResetMenu(base, Load(base, screen + kMenu2));
  Store(base, screen + kMachineState, Load(base, screen + kMachineReenter) != 0 ? 1 : 9);
  return r3 + kImages;
}

uint64_t OpenMovies(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821CBDB0 TuningMachine::OpenMovies");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMoviesOpen, frame, 0x821CBDD0, kShopMovie, 22547);
  calls.Call(kMoviesOpen, frame, 0x821CBDDC, kVinylMovie, 22548);
  const uint64_t r3 = calls.Call(kMoviesOpen, frame, 0x821CBDE8, kStickerMovie, 22549);
  Store(base, screen + kMachineLoaded, 0);
  return r3;
}

uint64_t WaitForMovies(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CE9E8 TuningMachine::WaitForMovies");
  const uint32_t s = uint32_t(r3);
  Frame(base, sp, 112);
  if (Load(base, s + kMachineLoaded) == 0) {
    bool all = true;
    for (const uint32_t id : {kShopMovie, kVinylMovie, kStickerMovie}) {
      r3 = ui_movies::IsLoaded(base, id);
      if (uint32_t(r3) == 0) {
        all = false;
        break;
      }
    }
    if (all) {
      Store(base, s + kMachineLoaded, 1);
    }
  }
  if (Load(base, s + kMachineLoaded) != 0) {
    const uint32_t reenter = Load(base, s + kMachineReenter);
    Store(base, s + 9708, 1);
    Store(base, s + kMachineState, reenter != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t OnDialogClosed(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821D8898 TuningMachine::OnDialogClosed");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  if (Load(base, s + 9652) == 11) {
    const uint32_t movie = Load(base, FindPair(base, kDialogMovie, frame + 80));
    calls.CallArgs(kSetText, frame, 0x821D88E8, {movie, kNone, 0, 15, 0, 0, 0, 0});
    calls.CallArgs(kSetText, frame, 0x821D88FC, {movie, 1, 3, 2, 0, 0, 0, 0});
    r3 = calls.CallArgs(kSetText, frame, 0x821D8910, {movie, 1, 4, 3, 0, 0, 0, 0});
    Store(base, movie + 40960, 0);
    Post(base, mode, 34);
  }
  Store(base, s + 9728, 1);
  return r3;
}

uint64_t Unload(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821D69A0 TuningMachine::Unload");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (Load(base, s + kMachineLoaded) != 0) {
    const uint32_t movie = Load(base, FindPair(base, kShopMovie, frame + 80));
    r3 = calls.Call(kFinishAll, frame, 0x821D69E0, movie);
  }
  if (Load(base, s + kMachineDelete) != 0) {
    calls.Call(kMoviesDelete, frame, 0x821D69F4, kShopMovie);
    calls.Call(kMoviesDelete, frame, 0x821D69FC, kVinylMovie);
    r3 = calls.Call(kMoviesDelete, frame, 0x821D6A04, kStickerMovie);
    Store(base, s + kMachineLoaded, 0);
  }
  return r3;
}

void Close(uint8_t* base, uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821D6A78 TuningMachine::Close");
  Store(base, machine + kMachineState, 7);
  Post(base, mode, 4);
}

uint64_t Destroy(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls,
                 const UnloadAllNative* unload) {
  ITC_NATIVE_RUNNING("sub_821CBD40 TuningMachine::Destroy");
  const uint32_t s = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  if (unload != nullptr) {
    tuning_pages::UnloadAll(base, *unload->call, r3 + kImages, frame, unload->native_free);
  } else {
    calls.Call(kUnloadAll, frame, 0x821CBD60, r3 + kImages);
  }
  calls.Call(kVirtualFree, frame, 0x821CBD78, Load(base, s + kQueue), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x821CBD88, Load(base, s + kMenu), 0, kMemRelease);
  return calls.Call(kVirtualFree, frame, 0x821CBD98, Load(base, s + kMenu2), 0, kMemRelease);
}

uint32_t SavePlaceholders14(uint8_t* base, uint32_t images, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8950 DecalImages::SavePlaceholders14");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t pair = FindPair(base, kStickerMovie, frame + 80);
  const uint32_t movie = Load(base, pair);
  Store(base, images + 8844, Load(base, Load(base, Load(base, movie + 28) + 640)));
  Store(base, images + 8848, Load(base, Load(base, Load(base, movie + 28) + 636)));
  return pair;
}

uint32_t SavePlaceholders(uint8_t* base, uint32_t images, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82208130 DecalImages::SavePlaceholders");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Load(base, FindPair(base, kVinylMovie, frame + 80));
  Store(base, images + 8836, Load(base, Load(base, Load(base, movie + 28) + 724)));
  Store(base, images + 8840, Load(base, Load(base, Load(base, movie + 28) + 700)));
  return SavePlaceholders14(base, images, frame);
}

}  // namespace tuning_machine
