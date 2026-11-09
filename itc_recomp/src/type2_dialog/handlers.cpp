// The type 2 dialog's machine handlers (docs/functions/type2-dialog.md):
// Draw and Update, which run the pages and steps from their tables (through
// the guest context, to the pages' and steps' overrides).

#include "type2_dialog/type2_dialog.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"

namespace type2_dialog {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// The pages' table in Draw's frame: ConfirmPage, NoticePage, Confirm2Page,
// Notice2Page.
constexpr uint32_t kPageTable[4] = {0x82187160, 0x82187508, 0x82187330, 0x82187618};
// The steps' table in Update's frame: OpenConfirmStep, CloseConfirmStep,
// OpenNoticeStep, CloseNoticeStep, OpenConfirm2Step, CloseConfirm2Step,
// OpenNotice2Step, CloseNotice2Step, MoveCursorStep, MoveCursor2Step.
constexpr uint32_t kStepTable[10] = {0x82187728, 0x821877F8, 0x821878B0, 0x82187948,
                                     0x821879F8, 0x82187AC8, 0x82187B80, 0x82187C18,
                                     0x82187CC8, 0x82187D80};

}  // namespace

uint64_t Draw(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82187E50 Type2Dialog::Draw");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t page = LoadU32(base, LoadU32(base, kPages) + 0);
  for (uint32_t i = 0; i < 4; i++) {
    StoreU32(base, frame + 80 + 4 * i, kPageTable[i]);
  }
  if (page == 0xFFFFFFFFu) {
    return r3;
  }
  // No bounds check, as the original: the word at the table's index.
  const uint32_t target = LoadU32(base, frame + 80 + (page << 2));
  return calls.CallArgs(target, frame, 0x82187EB8,
                        {mode, mode, 0, 0, kPageTable[0], kPageTable[1], kPageTable[2],
                         kPageTable[3]});
}

uint64_t Update(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82187EC8 Type2Dialog::Update");
  // The empty sub_82106540(100, 30, 0, ..) the original calls first is left
  // out: it returns at once.
  const uint32_t frame = Frame(base, sp, 208);
  StoreU32(base, frame + 80, LoadU32(base, LoadU32(base, kData) + 8));
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const uint32_t pages = LoadU32(base, kPages);
  for (uint32_t i = 0; i < 10; i++) {
    StoreU32(base, frame + 96 + 4 * i, kStepTable[i]);
  }
  const uint32_t step = LoadU32(base, pages + 12);
  uint64_t answer = movie;
  if (step == 0xFFFFFFFFu) {
    answer = calls.Call(kRun, frame, 0x82188030, movie, 0);
  } else {
    bool done = true;
    if (int32_t(step) >= 0) {
      // No bounds check: the word at the table's index. r4 to r10 as the
      // original leaves them (the table's build), not read by the steps.
      const uint32_t target = LoadU32(base, frame + 96 + (step << 2));
      answer = calls.CallArgs(target, frame, 0x82187FC8,
                              {pages + 24, kStepTable[3], kStepTable[4], kStepTable[5],
                               kStepTable[6], kStepTable[7], frame + 96, step << 2});
      done = int32_t(answer) != 0;
    }
    if (done) {
      const uint32_t p = LoadU32(base, kPages);
      const uint32_t queued = LoadU32(base, p + 16);
      if (queued == 0xFFFFFFFFu) {
        if (int32_t(step) >= 0) {
          StoreU32(base, p + 0, LoadU32(base, p + 4));
          StoreU32(base, p + 12, queued);
        } else {
          const uint32_t next = LoadU32(base, p + 4);
          StoreU32(base, p + 12, 0xFFFFFFFFu);
          StoreU32(base, p + 0, next);
        }
      } else {
        StoreU32(base, p + 16, 0xFFFFFFFFu);
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
  if (state == 9) {
    Post(base, mode, 42);
  }
  StoreU32(base, machine + kMachineState, LoadU32(base, LoadU32(base, kData) + 4));
  StoreU32(base, LoadU32(base, kData) + 4, 21);
  if (LoadU32(base, kStatus) != 5) {
    StoreU32(base, kStatus, 3);
  }
  return answer;
}

uint64_t OnEnter(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821869B8 Type2Dialog::OnEnter");
  const uint32_t frame = Frame(base, sp, 128);
  StoreU32(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(calls.Call(kVirtualAlloc, frame, 0x821869E4, 0, 28, 0x1000, 4));
  StoreU32(base, kPages, pages);
  StoreU32(base, pages + 4, 0);
  StoreU32(base, pages + 0, 0);
  StoreU32(base, pages + 8, 0);
  StoreU32(base, pages + 20, 0xFFFFFFFFu);
  StoreU32(base, pages + 12, 0xFFFFFFFFu);
  StoreU32(base, pages + 24, 0);
  const uint32_t menu = uint32_t(calls.Call(kVirtualAlloc, frame, 0x82186A20, 0, 68, 0x1000, 4));
  StoreU32(base, kMenu, menu);
  StoreU32(base, menu + 12, 0);
  StoreU32(base, menu + 16, 1);
  StoreU32(base, menu + 20, 1);
  for (const uint32_t field : {0u, 4u, 8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    StoreU32(base, menu + field, 0);
  }
  const uint64_t r3 = calls.Call(kVirtualAlloc, frame, 0x82186A7C, 0, 68, 0x1000, 4);
  const uint32_t data = uint32_t(r3);
  StoreU32(base, kData, data);
  StoreU32(base, data + 0, 0);
  StoreU32(base, data + 8, 0);
  StoreU32(base, data + 4, 21);
  StoreU32(base, kShowing, 0);
  if (LoadU32(base, machine + kMachineReenter) != 0) {
    StoreU32(base, machine + kMachineState, 1);
    StoreU32(base, kStatus, 0xFFFFFFFFu);
  } else {
    StoreU32(base, machine + kMachineState, 9);
    StoreU32(base, kStatus, 3);
  }
  return r3;
}

uint64_t WaitForMovies(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821880D0 Type2Dialog::WaitForMovies");
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
  StoreU32(base, kStatus, 3);
  return r3;
}

void Shown(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82186490 Type2Dialog::Shown");
  StoreU32(base, kStatus, 0);
}

void Wait(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_82186AD8 Type2Dialog::Wait");
  StoreU32(base, machine + kMachineState, 9);
  StoreU32(base, kStatus, 3);
}

uint64_t Cancel(uint8_t* base, uint64_t r3, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_82186E40 Type2Dialog::Cancel");
  // Each dialog in turn: on its page, the page object to its close step
  // and its message posted; else, with it showing (its step running), the
  // message alone. (+8, the page before, is never written: the page is the
  // dialog's own.)
  static constexpr uint32_t kClose[4] = {1, 3, 5, 7};
  for (uint32_t k = 0; k < 4; k++) {
    const uint32_t pages = LoadU32(base, kPages);
    const uint32_t message = k == 3 ? 40 : 41;
    if (LoadU32(base, pages + 0) == k) {
      switch (k) {
        case 0:
          StoreU32(base, pages + 12, 1);
          StoreU32(base, pages + 16, 0xFFFFFFFFu);
          StoreU32(base, pages + 4, 0);
          StoreU32(base, pages + 0, 0xFFFFFFFFu);
          break;
        case 1:
          StoreU32(base, pages + 4, 1);
          StoreU32(base, pages + 12, 3);
          StoreU32(base, pages + 16, 0xFFFFFFFFu);
          StoreU32(base, pages + 0, 0xFFFFFFFFu);
          break;
        case 2:
          StoreU32(base, pages + 16, 0xFFFFFFFFu);
          StoreU32(base, pages + 0, 0xFFFFFFFFu);
          StoreU32(base, pages + 12, 5);
          StoreU32(base, pages + 4, 2);
          break;
        default:
          StoreU32(base, pages + 16, 0xFFFFFFFFu);
          StoreU32(base, pages + 0, 0xFFFFFFFFu);
          StoreU32(base, pages + 4, 3);
          StoreU32(base, pages + 12, kClose[k]);
          break;
      }
    } else if (LoadU32(base, kShowing) != k) {
      continue;
    }
    Post(base, mode, message, k == 3);
    if (k != 3) {
      r3 = 0;
    }
  }
  StoreU32(base, kStatus, 2);
  return r3;
}

uint64_t OpenConfirm(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82186C30 Type2Dialog::OpenConfirm");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, LoadU32(base, kData) + 8, 16);
  StoreU32(base, frame + 80, LoadU32(base, LoadU32(base, kData) + 8));
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const uint64_t r3 = calls.Call(kResetPose, frame, 0x82186C80, movie);
  const uint32_t choice = LoadU32(base, movie + 40960);
  const uint32_t menu = LoadU32(base, kMenu);
  const uint32_t cursor = LoadU32(base, menu + 0);
  StoreU32(base, menu + 0, choice);
  StoreU32(base, menu + 4, cursor);
  const uint32_t pages = LoadU32(base, kPages);
  StoreU32(base, pages + 4, 0);
  StoreU32(base, pages + 0, 0);
  StoreU32(base, pages + 8, 0);
  StoreU32(base, pages + 20, 0xFFFFFFFFu);
  StoreU32(base, pages + 12, 0xFFFFFFFFu);
  StoreU32(base, pages + 24, 0);
  StoreU32(base, kShowing, 0);
  if (LoadU32(base, machine + kMachineLoaded) != 0) {
    const uint32_t p = LoadU32(base, kPages);
    const uint32_t before = LoadU32(base, p + 0);
    if (before != 0) {
      StoreU32(base, p + 8, before);
    }
    StoreU32(base, p + 12, 0);
    StoreU32(base, p + 16, 0xFFFFFFFFu);
    StoreU32(base, p + 4, 0);
    StoreU32(base, p + 0, 0xFFFFFFFFu);
    StoreU32(base, machine + kMachineState, 4);
  } else {
    StoreU32(base, machine + kMachineState, 1);
  }
  StoreU32(base, kStatus, 5);
  return r3;
}

uint64_t OpenConfirm2(uint8_t* base, uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82186D40 Type2Dialog::OpenConfirm2");
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
  StoreU32(base, pages + 20, 0xFFFFFFFFu);
  StoreU32(base, pages + 12, 0xFFFFFFFFu);
  StoreU32(base, pages + 24, 0);
  StoreU32(base, kShowing, 2);
  if (LoadU32(base, machine + kMachineLoaded) != 0) {
    const uint32_t p = LoadU32(base, kPages);
    const uint32_t before = LoadU32(base, p + 0);
    if (before != 2) {
      StoreU32(base, p + 8, before);
    }
    StoreU32(base, p + 16, 0xFFFFFFFFu);
    StoreU32(base, p + 4, 2);
    StoreU32(base, p + 0, 0xFFFFFFFFu);
    StoreU32(base, p + 12, 4);
    StoreU32(base, machine + kMachineState, 4);
  } else {
    StoreU32(base, machine + kMachineState, 1);
  }
  StoreU32(base, kStatus, 5);
  return pair;
}

void OpenNotice(uint8_t* base, uint32_t machine, bool second) {
  if (second) {
    ITC_NATIVE_RUNNING("sub_82186B90 Type2Dialog::OpenNotice2");
  } else {
    ITC_NATIVE_RUNNING("sub_82186AF0 Type2Dialog::OpenNotice");
  }
  const uint32_t page = second ? 3 : 1;
  StoreU32(base, LoadU32(base, kData) + 8, second ? 19 : 17);
  const uint32_t pages = LoadU32(base, kPages);
  StoreU32(base, pages + 24, 0);
  StoreU32(base, pages + 4, page);
  StoreU32(base, pages + 0, page);
  StoreU32(base, pages + 8, page);
  StoreU32(base, pages + 20, 0xFFFFFFFFu);
  StoreU32(base, pages + 12, 0xFFFFFFFFu);
  StoreU32(base, kShowing, page);
  if (LoadU32(base, machine + kMachineLoaded) != 0) {
    const uint32_t p = LoadU32(base, kPages);
    const uint32_t before = LoadU32(base, p + 0);
    if (before != page) {
      StoreU32(base, p + 8, before);
    }
    StoreU32(base, p + 16, 0xFFFFFFFFu);
    StoreU32(base, p + 4, page);
    StoreU32(base, p + 0, 0xFFFFFFFFu);
    StoreU32(base, p + 12, second ? 6 : 2);
    StoreU32(base, machine + kMachineState, 4);
  } else {
    StoreU32(base, machine + kMachineState, 1);
  }
  StoreU32(base, kStatus, 5);
}

uint64_t OnLeft(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82186428 Type2Dialog::OnLeft");
  const uint32_t frame = Frame(base, sp, 112);
  calls.Call(kVirtualFree, frame, 0x82186450, LoadU32(base, kPages), 0, 0x8000);
  calls.Call(kVirtualFree, frame, 0x82186464, LoadU32(base, kMenu), 0, 0x8000);
  const uint64_t r3 = calls.Call(kVirtualFree, frame, 0x82186478, LoadU32(base, kData), 0, 0x8000);
  StoreU32(base, kPages, 0);
  StoreU32(base, kMenu, 0);
  StoreU32(base, kData, 0);
  return r3;
}

uint32_t TakesInput(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821864A0 Type2Dialog::TakesInput");
  uint32_t status = LoadU32(base, kStatus);
  if (status == 0) {
    status = 4;
    StoreU32(base, kStatus, 4);
  }
  return status == 1 ? 1 : 0;
}

}  // namespace type2_dialog
