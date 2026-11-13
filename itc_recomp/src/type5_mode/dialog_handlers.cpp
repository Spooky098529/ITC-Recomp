// The type 5 dialog's machine handlers (docs/functions/type5-mode.md): Draw
// and Update, which run the pages and steps from their tables (through the
// guest context, to the pages' and steps' overrides), and the transitions'
// handlers.

#include "type5_mode/type5_mode.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace type5_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kNone = 0xFFFFFFFFu;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// The pages' table in Draw's frame: ConfirmPage, NoticePage, ConfirmPage
// again (dialog 2), Notice2Page.
constexpr uint32_t kPageTable[4] = {0x82225D10, 0x82225F30, 0x82225D10, 0x82226088};
// The steps' table in Update's frame: OpenConfirmStep, CloseConfirmStep,
// the type 2 dialog's OpenNoticeStep, CloseNoticeStep, OpenConfirm2Step,
// CloseConfirm2Step, the type 2 dialog's OpenNotice2Step, CloseNotice2Step,
// MoveCursorStep, Step9.
constexpr uint32_t kStepTable[10] = {0x822261E0, 0x822262B8, 0x821878B0, 0x82226370,
                                     0x82226420, 0x822264B8, 0x82187B80, 0x82226568,
                                     0x82226618, 0x822266D0};

// Posts the message (id, 0, 0) on the dispatcher's ring (+204), the id
// stored first.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t id) {
  const uint32_t ring = dispatcher + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  StoreU32(base, slot + 0, id);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

}  // namespace

uint64_t Draw(uint8_t* base, uint64_t machine, uint64_t dispatcher, uint32_t sp,
              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822266F8 Type5Dialog::Draw");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t page = LoadU32(base, LoadU32(base, kPages) + 0);
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(base, frame + 80 + 4 * i, kPageTable[i]);
  }
  StoreU32(base, frame + 96, 0);
  if (page == kNone) {
    return dispatcher;
  }
  // No bounds check, as the original: the word at the table's index.
  const uint32_t target = LoadU32(base, frame + 80 + (page << 2));
  return calls.CallArgs(target, frame, 0x82226774,
                        {dispatcher, machine, machine, kPageTable[0], kPageTable[1],
                         kPageTable[2], kPageTable[3], frame + 80});
}

uint64_t Update(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82226788 Type5Dialog::Update");
  // The empty sub_82106540(100, 30, 0, "TXR_PaCmnWindow") the original
  // calls first is left out: it returns at once.
  const uint32_t frame = Frame(base, sp, 208);
  StoreU32(base, frame + 80, LoadU32(base, LoadU32(base, kData) + 8));
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const uint32_t pages = LoadU32(base, kPages);
  StoreU32(base, frame + 132, kStepTable[9]);
  const uint32_t step = LoadU32(base, pages + 12);
  for (uint32_t i = 0; i < 9; i++) {
    StoreU32(base, frame + 96 + 4 * i, kStepTable[i]);
  }
  uint64_t answer = movie;
  if (step == kNone) {
    answer = calls.Call(kRun, frame, 0x822268F0, movie, 0);
  } else {
    bool done = true;
    uint32_t p = pages;
    if (int32_t(step) >= 0) {
      // No bounds check: the word at the table's index. r4 to r10 as the
      // original leaves them (the table's build), not read by the steps.
      const uint32_t target = LoadU32(base, frame + 96 + (step << 2));
      answer = calls.CallArgs(target, frame, 0x82226888,
                              {pages + 24, kStepTable[3], kStepTable[4], kStepTable[5],
                               kStepTable[6], kStepTable[7], frame + 96, step << 2});
      p = LoadU32(base, kPages);
      done = int32_t(answer) != 0;
    }
    if (done) {
      const uint32_t queued = LoadU32(base, p + 16);
      if (queued == kNone) {
        if (int32_t(step) >= 0) {
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
  if (state == 21) {
    return answer;
  }
  StoreU32(base, machine + kMachineState, state);
  const uint32_t data = LoadU32(base, kData);
  if (LoadU32(base, data + 4) == 9) {
    StoreU32(base, data + 12, 0);
    Post(base, dispatcher, 18);
    StoreU32(base, LoadU32(base, kData) + 4, 21);
  } else {
    StoreU32(base, data + 4, 21);
  }
  return answer;
}

uint64_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225878 Type5Dialog::OnEnter");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(calls.Call(kVirtualAlloc, frame, 0x822258A4, 0, 28, 0x1000, 4));
  StoreU32(base, kPages, pages);
  StoreU32(base, pages + 4, 0);
  StoreU32(base, pages + 0, 0);
  StoreU32(base, pages + 8, 0);
  StoreU32(base, pages + 24, 0);
  StoreU32(base, pages + 20, kNone);
  StoreU32(base, pages + 12, kNone);
  const uint32_t menu = uint32_t(calls.Call(kVirtualAlloc, frame, 0x822258E0, 0, 68, 0x1000, 4));
  StoreU32(base, kMenu, menu);
  StoreU32(base, menu + 12, 0);
  StoreU32(base, menu + 16, 1);
  StoreU32(base, menu + 20, 1);
  for (const uint32_t field : {0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    StoreU32(base, menu + field, 0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x8222593C, 0, 68, 0x1000, 4);
  const uint32_t data = uint32_t(r3);
  StoreU32(base, kData, data);
  StoreU32(base, data + 0, 0);
  StoreU32(base, data + 8, 0);
  StoreU32(base, data + 12, 0);
  StoreU32(base, data + 4, 21);
  StoreU32(base, machine + kMachineState,
           LoadU32(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t WaitForMovies(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82226978 Type5Dialog::WaitForMovies");
  Frame(base, sp, 96);
  const uint32_t machine = uint32_t(r3);
  if (LoadU32(base, machine + kMachineLoaded) == 0) {
    uint32_t loaded = 0;
    for (uint32_t id = 16; id <= 19; id++) {
      loaded = ui_movies::IsLoaded(base, id);
      if (int32_t(loaded) == 0) {
        return loaded;
      }
    }
    StoreU32(base, machine + kMachineLoaded, 1);
    return loaded;
  }
  StoreU32(base, LoadU32(base, kData) + 0, 1);
  StoreU32(base, machine + kMachineState, 9);
  return r3;
}

void OnInterrupt(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82225860 Type5Dialog::OnInterrupt");
  StoreU32(base, LoadU32(base, kData) + 12, 1);
}

uint64_t OpenConfirm(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82225A90 Type5Dialog::OpenConfirm");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, LoadU32(base, kData) + 8, 16);
  StoreU32(base, frame + 80, LoadU32(base, LoadU32(base, kData) + 8));
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const uint64_t r3 = calls.Call(kResetPose, frame, 0x82225AE0, movie);
  const uint32_t choice = LoadU32(base, movie + 40960);
  const uint32_t menu = LoadU32(base, kMenu);
  const uint32_t cursor = LoadU32(base, menu + 0);
  StoreU32(base, menu + 0, choice);
  StoreU32(base, menu + 4, cursor);
  const uint32_t pages = LoadU32(base, kPages);
  StoreU32(base, pages + 4, 0);
  StoreU32(base, pages + 0, 0);
  StoreU32(base, pages + 8, 0);
  StoreU32(base, pages + 20, kNone);
  StoreU32(base, pages + 12, kNone);
  StoreU32(base, pages + 24, 0);
  if (LoadU32(base, machine + kMachineLoaded) != 0) {
    const uint32_t p = LoadU32(base, kPages);
    const uint32_t before = LoadU32(base, p + 0);
    if (before != 0) {
      StoreU32(base, p + 8, before);
    }
    StoreU32(base, p + 12, 0);
    StoreU32(base, p + 16, kNone);
    StoreU32(base, p + 4, 0);
    StoreU32(base, p + 0, kNone);
    StoreU32(base, machine + kMachineState, 4);
  } else {
    StoreU32(base, machine + kMachineState, 1);
  }
  return r3;
}

uint64_t OpenConfirm2(uint8_t* base, uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82225B88 Type5Dialog::OpenConfirm2");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, LoadU32(base, kData) + 8, 18);
  StoreU32(base, frame + 80, LoadU32(base, LoadU32(base, kData) + 8));
  const uint32_t pair = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = LoadU32(base, pair);
  const uint32_t menu = LoadU32(base, kMenu);
  const uint32_t choice = LoadU32(base, movie + 40960);
  const uint32_t cursor = LoadU32(base, menu + 0);
  StoreU32(base, menu + 0, choice);
  StoreU32(base, menu + 4, cursor);
  const uint32_t pages = LoadU32(base, kPages);
  StoreU32(base, pages + 4, 2);
  StoreU32(base, pages + 0, 2);
  StoreU32(base, pages + 8, 2);
  StoreU32(base, pages + 20, kNone);
  StoreU32(base, pages + 12, kNone);
  StoreU32(base, pages + 24, 0);
  if (LoadU32(base, machine + kMachineLoaded) != 0) {
    const uint32_t p = LoadU32(base, kPages);
    const uint32_t before = LoadU32(base, p + 0);
    if (before != 2) {
      StoreU32(base, p + 8, before);
    }
    StoreU32(base, p + 16, kNone);
    StoreU32(base, p + 4, 2);
    StoreU32(base, p + 0, kNone);
    StoreU32(base, p + 12, 4);
    StoreU32(base, machine + kMachineState, 4);
  } else {
    StoreU32(base, machine + kMachineState, 1);
  }
  return pair;
}

void OpenNotice(uint8_t* base, uint32_t machine, bool second) {
  if (second) {
    ITC_NATIVE_RUNNING("sub_82225A08 Type5Dialog::OpenNotice2");
  } else {
    ITC_NATIVE_RUNNING("sub_82225980 Type5Dialog::OpenNotice");
  }
  const uint32_t page = second ? 3 : 1;
  StoreU32(base, LoadU32(base, kData) + 8, second ? 19 : 17);
  const uint32_t pages = LoadU32(base, kPages);
  StoreU32(base, pages + 4, page);
  StoreU32(base, pages + 0, page);
  StoreU32(base, pages + 8, page);
  StoreU32(base, pages + 20, kNone);
  StoreU32(base, pages + 12, kNone);
  StoreU32(base, pages + 24, 0);
  if (LoadU32(base, machine + kMachineLoaded) != 0) {
    const uint32_t p = LoadU32(base, kPages);
    const uint32_t before = LoadU32(base, p + 0);
    if (before != page) {
      StoreU32(base, p + 8, before);
    }
    StoreU32(base, p + 16, kNone);
    StoreU32(base, p + 4, page);
    StoreU32(base, p + 0, kNone);
    StoreU32(base, p + 12, second ? 6 : 2);
    StoreU32(base, machine + kMachineState, 4);
  } else {
    StoreU32(base, machine + kMachineState, 1);
  }
}

uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822257A8 Type5Dialog::OnLeft");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x822257D0, LoadU32(base, kPages), 0, 0x8000);
  calls.Call(kVirtualFree, frame, 0x822257E4, LoadU32(base, kMenu), 0, 0x8000);
  const uint64_t r3 = calls.Call(kVirtualFree, frame, 0x822257F8, LoadU32(base, kData), 0, 0x8000);
  StoreU32(base, kPages, 0);
  StoreU32(base, kMenu, 0);
  StoreU32(base, kData, 0);
  return r3;
}

}  // namespace type5_mode
