// The title load's debug sequence (docs/functions/title-screen.md), never
// seen in play: its four pages (waiting for player 0's action 159) and its
// twelve tick-counting transitions. See title_screen.h. Their debug prints
// (the empty sub_82106540) are left out.

#include "base/log.h"
#include "schema/action_mappers.h"
#include "schema/racer_controllers.h"
#include "title_screen/internal.h"

namespace title_screen {

namespace {

constexpr uint32_t kPressedSlot = 40;  // the mapper's slot 10

struct DebugPageSpec {
  uint32_t transition;
  uint32_t lr;
};
constexpr DebugPageSpec kPages[4] = {
    {2, 0x82220D20},
    {5, 0x82220DA8},
    {8, 0x82220E30},
    {11, 0x82220EB8},
};

// Each step's frame size in the original (none made since E5 step 6), its
// tick limit, the transition and page it sets past
// it and whether it clears its started word then.
struct DebugStepSpec {
  uint32_t frame;
  int32_t limit;
  uint32_t transition;
  uint32_t page;
  bool reset;
};
constexpr DebugStepSpec kSteps[12] = {
    {128, 10, 1, 0, true},
    {112, 120, 2, kNone, true},
    {128, 10, 3, 1, true},
    {128, 10, 4, 1, true},
    {112, 120, 5, kNone, true},
    {112, 10, 6, 2, true},
    {112, 10, 7, 2, true},
    {112, 180, 8, kNone, true},
    {112, 10, 9, 3, true},
    {112, 10, 10, 3, true},
    {112, 240, 11, kNone, true},
    {96, 10, kNone, 4, false},
};

}  // namespace

namespace {

uint64_t DebugPage(uint32_t which, uint64_t r3, uint32_t sp, const Calls& calls) {
  const DebugPageSpec& spec = kPages[which];
  const uint32_t frame = Frame(sp, 96);
  // Player 0's mapper entry (racer-controllers' PlayerMapperEntry).
  const schema::PlayerMapperEntry entry(G::mapper_entries_address() + (Load(G::controllers_address()) << 3));
  if (int32_t(entry.unread()) == 1) {
    return r3;
  }
  const schema::ActionMapper mapper = entry.mapper();
  r3 = calls.Call(Load(mapper.vtable() + kPressedSlot), frame, spec.lr, mapper.address, 159);
  if ((r3 & 0xFF) != 0) {
    G::load_block().set_transition(spec.transition);
    G::load_block().set_page(kNone);
    G::load_block().set_transition_started(0);
  }
  return r3;
}

uint64_t DebugStep(uint32_t which, uint32_t started, uint32_t sp) {
  const DebugStepSpec& spec = kSteps[which];
  (void)sp;  // no guest frame (E5 step 6): nothing here calls guest code
  // `started` is the load block's +16 (Update's only caller): the block is
  // in host memory (E5 step 3), so the word is read through its view.
  (void)started;
  if (G::load_block().transition_started() == 0) {
    G::load_block().set_tick_count(0);
    G::load_block().set_transition_started(1);
  }
  const auto block = G::load_block();
  block.set_tick_count(block.tick_count() + 1);
  const auto now = G::load_block();
  if (int32_t(now.tick_count()) > spec.limit) {
    now.set_transition(spec.transition);
    G::load_block().set_page(spec.page);
    if (spec.reset) {
      G::load_block().set_transition_started(0);
    }
  }
  // The original's r3: the debug print's first argument.
  return 100;
}

}  // namespace

// Each page and step with its own print.
uint64_t LoadDebugPage(uint32_t which, uint64_t r3, uint32_t sp,
                       const Calls& calls) {
  switch (which) {
    case 0: ITC_NATIVE_RUNNING("sub_82220CD8 TitleLoad::DebugPage0"); break;
    case 1: ITC_NATIVE_RUNNING("sub_82220D60 TitleLoad::DebugPage1"); break;
    case 2: ITC_NATIVE_RUNNING("sub_82220DE8 TitleLoad::DebugPage2"); break;
    default: ITC_NATIVE_RUNNING("sub_82220E70 TitleLoad::DebugPage3"); break;
  }
  return DebugPage(which, r3, sp, calls);
}

uint64_t LoadDebugStep(uint32_t which, uint32_t started, uint32_t sp) {
  switch (which) {
    case 0: ITC_NATIVE_RUNNING("sub_822204F8 TitleLoad::DebugStep0"); break;
    case 1: ITC_NATIVE_RUNNING("sub_82220580 TitleLoad::DebugStep1"); break;
    case 2: ITC_NATIVE_RUNNING("sub_82220610 TitleLoad::DebugStep2"); break;
    case 3: ITC_NATIVE_RUNNING("sub_82220698 TitleLoad::DebugStep3"); break;
    case 4: ITC_NATIVE_RUNNING("sub_82220720 TitleLoad::DebugStep4"); break;
    case 5: ITC_NATIVE_RUNNING("sub_822207B0 TitleLoad::DebugStep5"); break;
    case 6: ITC_NATIVE_RUNNING("sub_82220840 TitleLoad::DebugStep6"); break;
    case 7: ITC_NATIVE_RUNNING("sub_822208D0 TitleLoad::DebugStep7"); break;
    case 8: ITC_NATIVE_RUNNING("sub_82220960 TitleLoad::DebugStep8"); break;
    case 9: ITC_NATIVE_RUNNING("sub_822209F0 TitleLoad::DebugStep9"); break;
    case 10: ITC_NATIVE_RUNNING("sub_82220A80 TitleLoad::DebugStep10"); break;
    default: ITC_NATIVE_RUNNING("sub_82220B10 TitleLoad::DebugStep11"); break;
  }
  return DebugStep(which, started, sp);
}

}  // namespace title_screen
