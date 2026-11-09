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

}  // namespace type2_dialog
