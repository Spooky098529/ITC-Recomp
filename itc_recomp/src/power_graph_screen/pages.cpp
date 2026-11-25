// The power graph screen's pages and Draw (docs/functions/
// power-graph-screen.md): MenuPage (the buttons that close the screen) and
// ClosedPage (after the close), which reply through the member's vtable
// slots, and Draw, which runs them from its table. Menu::Step,
// Menu::Pressed, MenuSound::Play and the slots go through their overrides.

#include "power_graph_screen/power_graph_screen.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/power_graph_screen.h"

namespace power_graph_screen {

namespace {

using guest::LoadU32;
using G = schema::PowerGraphScreenGlobals;
using Member = schema::PowerGraphScreen67;

// The pages' table the original builds in Draw's frame (+80): MenuPage,
// ClosedPage, ClosedPage (8-byte entries, the function and a this-adjust
// 0); a host table since E5 step 6.
constexpr uint32_t kMenuPage = 0x82250948;
constexpr uint32_t kClosedPage = 0x82250348;
struct Page {
  uint32_t function, adjust;
};
constexpr Page kPages[3] = {{kMenuPage, 0}, {kClosedPage, 0}, {kClosedPage, 0}};
constexpr uint32_t kDrawFrame = 112;

}  // namespace

uint64_t MenuPage(uint64_t member, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250948 PowerGraphScreen67::MenuPage");
  // The original's 112-byte frame held only its back chain: its calls run
  // from `sp` (E5 step 6; none reads its caller's frame).
  uint64_t answer = calls.Call(kMenuStep, sp, 0x82250968, G::menu().address);
  const uint32_t tab = G::data().tab();
  if (tab >= 6) {
    return answer;
  }
  // Back (158), then accept (159) below 3 or the opening button (163).
  answer = calls.Call(kMenuPressed, sp, tab < 3 ? 0x822509AC : 0x82250990,
                      G::menu().address, 158);
  if (int32_t(answer) == 0) {
    answer = calls.Call(kMenuPressed, sp, 0x822509C0, G::menu().address, tab < 3 ? 159 : 163);
    if (int32_t(answer) == 0) {
      return answer;
    }
  }
  calls.Call(kMenuSoundPlay, sp, 0x822509D0, 2);
  const schema::PageObject pages = G::pages();
  const uint32_t before = pages.page();
  if (before != 1) {
    pages.set_previous_page(before);
  }
  pages.set_step(1);
  pages.set_queued_step(0xFFFFFFFFu);
  pages.set_next_page(1);
  pages.set_page(0xFFFFFFFFu);
  // The member's slot 2 (its closing message).
  return calls.Call(Member(uint32_t(member)).vtable().post_closing(), sp, 0x82250A18, member, mode);
}

uint64_t ClosedPage(uint64_t member, uint64_t mode, uint32_t sp, uint32_t lr,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250348 PowerGraphScreen67::ClosedPage");
  G::data().set_queued_state(9);
  // The member's slot 3 (its closed message), a tail call.
  return calls.Call(Member(uint32_t(member)).vtable().post_closed(), sp, lr, member, mode);
}

uint64_t Draw(uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82250A20 PowerGraphScreen67::Draw");
  const uint32_t page = G::pages().page();
  if (page == 0xFFFFFFFFu) {
    return r3;
  }
  // The table a host one (E5 step 6), the page run from `sp` (MenuPage and
  // ClosedPage don't read their caller's frame). Pages 0 to 2 only (in
  // play 0 and 1); past them the original reads past its table without a
  // check: those words read where they would be on the stack. r9 the
  // table's address (its frame's +80), r10 the adjust as the original
  // leaves them.
  const uint32_t table = sp - kDrawFrame + 80;
  uint32_t adjust, target;
  if (page < 3) {
    adjust = kPages[page].adjust;
    target = kPages[page].function;
  } else {
    adjust = LoadU32(table + 4 + (page << 3));
    target = LoadU32(table + (page << 3));
  }
  return calls.CallArgs(target, sp, 0x82250A94,
                        {uint64_t(adjust) + r3, mode, 0, 0, 0, 0, table, adjust});
}

}  // namespace power_graph_screen
