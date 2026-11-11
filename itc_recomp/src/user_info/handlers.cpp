// The user info screen's machine handlers (docs/functions/user-info.md):
// see user_info.h.

#include "user_info/user_info.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace user_info {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemRelease = 0x8000;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// Movie 10 found with its id stored at `key` (the original's frame word).
uint32_t FindMovie(uint8_t* base, uint32_t key) {
  StoreU32(base, key, kMovie);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, key));
}

// The message (id, 0, 0) posted on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
void Post(uint8_t* base, uint32_t mode, uint32_t id) {
  const uint32_t ring = mode + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t start = LoadU32(base, ring + 604);
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + 12 * ((start + count - 1) % 50);
  StoreU32(base, slot + 0, id);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

// The pages' table in UpdatePage's frame: MenuPage, the empty sub_82106540.
constexpr uint32_t kPageTable[2] = {0x8220B0D8, 0x82106540};
// The transitions' table in Update's frame: Opening, Closing.
constexpr uint32_t kTransitionTable[2] = {0x8220B1C8, 0x8220AC60};

}  // namespace

uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220AA50 UserInfo::Create");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(calls.Call(kVirtualAlloc, frame, 0x8220AA84, 0, 28, 0x1000, 4));
  StoreU32(base, kPages, pages);
  StoreU32(base, pages + 4, 0);
  StoreU32(base, pages + 0, 0);
  StoreU32(base, pages + 8, 0);
  StoreU32(base, pages + 24, 0);
  StoreU32(base, pages + 20, kNone);
  StoreU32(base, pages + 12, kNone);
  const uint32_t menu = uint32_t(calls.Call(kVirtualAlloc, frame, 0x8220AAC0, 0, 68, 0x1000, 4));
  StoreU32(base, kMenu, menu);
  StoreU32(base, menu + 12, 0);
  StoreU32(base, menu + 20, 0);
  StoreU32(base, menu + 0, 0);
  StoreU32(base, menu + 4, 0);
  StoreU32(base, menu + 16, 7);
  for (const uint32_t field : {8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    StoreU32(base, menu + field, 0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x8220AB1C, 0, 60, 0x1000, 4);
  const uint32_t data = uint32_t(r3);
  StoreU32(base, kData, data);
  StoreU32(base, data + 0, 0);
  StoreU32(base, data + 4, 21);
  StoreU32(base, machine + kMachineState, LoadU32(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t OpenMovie(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220AA40 UserInfo::OpenMovie");
  return calls.Call(kMoviesOpen, sp, lr, kMovie, kAsset);
}

uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8220B070 UserInfo::WaitForLoad");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 96);
  if (LoadU32(base, machine + kMachineLoaded) == 0) {
    r3 = ui_movies::IsLoaded(base, kMovie);
    if (uint32_t(r3) != 0) {
      StoreU32(base, machine + kMachineLoaded, 1);
    }
    return r3;
  }
  StoreU32(base, LoadU32(base, kData) + 0, 1);
  StoreU32(base, machine + kMachineState, 9);
  return r3;
}

void Reenter(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8220AB60 UserInfo::Reenter");
  if (LoadU32(base, machine + kMachineLoaded) == 0) {
    StoreU32(base, machine + kMachineState, 1);
    return;
  }
  const uint32_t pages = LoadU32(base, kPages);
  const uint32_t page = LoadU32(base, pages + 0);
  if (page != 0) {
    StoreU32(base, pages + 8, page);
  }
  StoreU32(base, pages + 12, 0);
  StoreU32(base, pages + 16, kNone);
  StoreU32(base, pages + 4, 0);
  StoreU32(base, pages + 0, kNone);
  StoreU32(base, machine + kMachineState, 4);
}

uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220ABB8 UserInfo::Close");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, machine + kMachineState, 7);
  const uint32_t movie = FindMovie(base, frame + 80);
  calls.Call(kFinishAll, frame, 0x8220ABF8, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8220AC00, kMovie);
  Post(base, mode, 4);
  return r3;
}

uint64_t Destroy(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220A9D8 UserInfo::Destroy");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x8220AA00, LoadU32(base, kPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8220AA14, LoadU32(base, kMenu), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x8220AA28, LoadU32(base, kData), 0, kMemRelease);
  StoreU32(base, kPages, 0);
  StoreU32(base, kMenu, 0);
  StoreU32(base, kData, 0);
  return r3;
}

uint64_t UpdatePage(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220B260 UserInfo::UpdatePage");
  const uint32_t frame = Frame(base, sp, 96);
  StoreU32(base, frame + 80, kPageTable[0]);
  StoreU32(base, frame + 84, kPageTable[1]);
  const uint32_t page = LoadU32(base, LoadU32(base, kPages) + 0);
  if (page == kNone) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(LoadU32(base, frame + 80 + (page << 2)), frame, 0x8220B2B0, mode, mode);
}

uint64_t Update(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220B2C0 UserInfo::Update");
  // The empty sub_82106540(100, 30, 0, "TXR_MenuUserInfo") the original
  // calls first is left out: it returns at once.
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80);
  StoreU32(base, frame + 84, kTransitionTable[1]);
  StoreU32(base, frame + 80, kTransitionTable[0]);
  const uint32_t pages = LoadU32(base, kPages);
  const uint32_t running = LoadU32(base, pages + 12);
  uint64_t r3 = movie;
  if (running == kNone) {
    r3 = calls.Call(kRun, frame, 0x8220B3C8, movie, 0);
  } else {
    bool done = true;
    if (int32_t(running) >= 0) {
      // No bounds check: the word at the table's index.
      r3 = calls.Call(LoadU32(base, frame + 80 + (running << 2)), frame, 0x8220B358, pages + 24);
      done = uint32_t(r3) != 0;
    }
    if (done) {
      const uint32_t p = LoadU32(base, kPages);
      const uint32_t queued = LoadU32(base, p + 16);
      if (queued == kNone) {
        if (int32_t(running) >= 0) {
          StoreU32(base, p + 0, LoadU32(base, p + 4));
          StoreU32(base, p + 12, queued);
        } else {
          const uint32_t next = LoadU32(base, p + 4);
          StoreU32(base, p + 12, kNone);
          StoreU32(base, p + 0, next);
        }
      } else {
        StoreU32(base, p + 16, kNone);
        StoreU32(base, p + 20, 0);
        StoreU32(base, p + 12, queued);
      }
      StoreU32(base, p + 24, 0);
    }
  }
  const uint32_t state = LoadU32(base, LoadU32(base, kData) + 4);
  if (state != 21) {
    StoreU32(base, machine + kMachineState, state);
    StoreU32(base, LoadU32(base, kData) + 4, 21);
  }
  return r3;
}

}  // namespace user_info
