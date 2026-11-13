// The power graph screen's pages and Draw (docs/functions/
// power-graph-screen.md): MenuPage (the buttons that close the screen) and
// ClosedPage (after the close), which reply through the member's vtable
// slots, and Draw, which runs them from its table. Menu::Step,
// Menu::Pressed, MenuSound::Play and the slots go through their overrides.

#include "power_graph_screen/power_graph_screen.h"

#include "base/log.h"
#include "guest.h"

namespace power_graph_screen {

namespace {

using guest::LoadU32;
using guest::StoreU32;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// The member's vtable slot at `offset`.
uint32_t Slot(uint8_t* base, uint64_t member, uint32_t offset) {
  return LoadU32(base, LoadU32(base, uint32_t(member) + 0) + offset);
}

// The pages' table in Draw's frame: MenuPage, ClosedPage (8-byte entries,
// the function and a this-adjust 0).
constexpr uint32_t kMenuPage = 0x82250948;
constexpr uint32_t kClosedPage = 0x82250348;

}  // namespace

uint64_t MenuPage(uint8_t* base, uint64_t member, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250948 PowerGraphScreen67::MenuPage");
  const uint32_t frame = Frame(base, sp, 112);
  uint64_t answer = calls.Call(kMenuStep, frame, 0x82250968, LoadU32(base, kMenu));
  const uint32_t tab = LoadU32(base, LoadU32(base, kData) + 88);
  if (tab >= 6) {
    return answer;
  }
  // Back (158), then accept (159) below 3 or the opening button (163).
  answer = calls.Call(kMenuPressed, frame, tab < 3 ? 0x822509AC : 0x82250990,
                      LoadU32(base, kMenu), 158);
  if (int32_t(answer) == 0) {
    answer = calls.Call(kMenuPressed, frame, 0x822509C0, LoadU32(base, kMenu), tab < 3 ? 159 : 163);
    if (int32_t(answer) == 0) {
      return answer;
    }
  }
  calls.Call(kMenuSoundPlay, frame, 0x822509D0, 2);
  const uint32_t pages = LoadU32(base, kPages);
  const uint32_t before = LoadU32(base, pages + 0);
  if (before != 1) {
    StoreU32(base, pages + 8, before);
  }
  StoreU32(base, pages + 12, 1);
  StoreU32(base, pages + 16, 0xFFFFFFFFu);
  StoreU32(base, pages + 4, 1);
  StoreU32(base, pages + 0, 0xFFFFFFFFu);
  // The member's slot 2 (its closing message).
  return calls.Call(Slot(base, member, 8), frame, 0x82250A18, member, mode);
}

uint64_t ClosedPage(uint8_t* base, uint64_t member, uint64_t mode, uint32_t sp, uint32_t lr,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250348 PowerGraphScreen67::ClosedPage");
  StoreU32(base, LoadU32(base, kData) + 4, 9);
  // The member's slot 3 (its closed message), a tail call.
  return calls.Call(Slot(base, member, 12), sp, lr, member, mode);
}

uint64_t Draw(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250A20 PowerGraphScreen67::Draw");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, frame + 80, kMenuPage);
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 88, kClosedPage);
  StoreU32(base, frame + 92, 0);
  StoreU32(base, frame + 96, kClosedPage);
  StoreU32(base, frame + 100, 0);
  const uint32_t page = LoadU32(base, LoadU32(base, kPages) + 0);
  if (page == 0xFFFFFFFFu) {
    return r3;
  }
  // No bounds check, as the original: the words at the table's index. r9
  // the table, r10 the adjust as the original leaves them.
  const uint32_t adjust = LoadU32(base, frame + 84 + (page << 3));
  const uint32_t target = LoadU32(base, frame + 80 + (page << 3));
  return calls.CallArgs(target, frame, 0x82250A94,
                        {uint64_t(adjust) + r3, mode, 0, 0, 0, 0, frame + 80, adjust});
}

}  // namespace power_graph_screen
