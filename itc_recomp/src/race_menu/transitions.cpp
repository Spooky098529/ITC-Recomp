// The race menu's Update, its transitions and debug pages
// (docs/functions/race-menu.md): see race_menu.h.

#include "race_menu/race_menu.h"

#include <bit>

#include "base/log.h"
#include "guest.h"
#include "menu_helpers/menu_helpers.h"
#include "race_menu/internal.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_menu {

namespace {

constexpr uint64_t kSceneRoot = ~0ull;  // the sub-scene -1 (the scene's own children)
constexpr uint32_t kFinish = 0x8211DE38;
constexpr uint32_t kPlay = 0x8211D978;
constexpr uint32_t kLoop = 0x8211DDD8;
constexpr uint32_t kResetScroll = 0x8211C5F8;

// The mapper's slots (bytes into its vtable).
constexpr uint32_t kPressedOffset = menu_helpers::kPressedOffset;  // 10
constexpr uint32_t kFiredOffset = menu_helpers::kFiredOffset;      // 12

// The debug texts' strings: 0x82010000 + the originals' (negative) offsets.
constexpr uint32_t Text(int32_t offset) { return uint32_t(0x82010000 + offset); }

uint64_t Finish(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t m, uint64_t sub,
                uint64_t index) {
  return calls.Call(kFinish, frame, lr, m, sub, index);
}
// UiMovie::Play (r7 to r9 0).
uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t m, uint64_t sub,
              uint64_t index, uint64_t chained) {
  return calls.Call(kPlay, frame, lr, m, sub, index, chained);
}
uint64_t Loop(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t m, uint64_t sub,
              uint64_t index) {
  return calls.Call(kLoop, frame, lr, m, sub, index);
}

// The transition ended as the originals end it inline: the queued one
// (+16) next (+16 = -1, +20 = 0) or, with none, +0 = +4 (the page); +24 = 0.
void EndTransition() {
  const Pages pages = G::pages();
  uint32_t next = pages.queued_step();
  if (int32_t(next) == -1) {
    pages.set_page(pages.next_page());
  } else {
    next = pages.queued_step();
    pages.set_unknown_20(0);
    pages.set_queued_step(kNone);
  }
  pages.set_step(next);
  pages.set_step_started(0);
}

}  // namespace

uint64_t Transition0(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82189740 RaceMenu::Transition0");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t m = FindMovie(kMovie, frame + 80);
  if (Machine(machine).transitions_set_up() == 0) {
    Finish(calls, frame, 0x82189788, m, kSceneRoot, 0);
    Play(calls, frame, 0x821897A8, m, kSceneRoot, 0, 1);
    Finish(calls, frame, 0x821897B8, m, 0, 0);
    Play(calls, frame, 0x821897D8, m, 0, 0, 1);
    Loop(calls, frame, 0x821897E8, m, 0, 0);
    Finish(calls, frame, 0x821897F8, m, kSceneRoot, 5);
    Play(calls, frame, 0x82189818, m, kSceneRoot, 5, 1);
    Loop(calls, frame, 0x82189828, m, kSceneRoot, 5);
    Finish(calls, frame, 0x82189838, m, 9, 1);
    Play(calls, frame, 0x82189858, m, 9, 1, 0);
    Loop(calls, frame, 0x82189868, m, 9, 1);
    Machine(machine).set_transitions_set_up(1);
  }
  Finish(calls, frame, 0x82189880, m, 19, uint64_t(Machine(machine).race_mode()) + 2);
  Play(calls, frame, 0x821898A4, m, 19, uint64_t(Machine(machine).race_mode()) + 2, 0);
  return Loop(calls, frame, 0x821898B8, m, 19, uint64_t(Machine(machine).race_mode()) + 2);
}

uint64_t Transition3(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82189C28 RaceMenu::Transition3");
  const uint32_t frame = Frame(sp, 176);
  const uint32_t m = FindMovie(kMovie, frame + 80);
  if (Machine(machine).transitions_set_up() == 0) {
    // The nodes' floats (f31 the zero, f30 0x82002228 then 0x82000C34, f29
    // 0x8200A134), each node's stores in the original's order.
    const float zero = std::bit_cast<float>(G::float_zero());
    Node(ui_movies::FindNode(m, 20, 0)).set_y(zero);
    for (const uint32_t child : {2u, 3u, 4u}) {
      const Node node(ui_movies::FindNode(m, 11, child));
      node.set_x(std::bit_cast<float>(G::float_2228()));
      node.set_y(std::bit_cast<float>(G::float_a134()));
    }
    Node node(ui_movies::FindNode(m, 11, 0));
    node.set_x(std::bit_cast<float>(G::float_c34()));
    node.set_y(zero);
    node = Node(ui_movies::FindNode(m, 11, 1));
    node.set_x(std::bit_cast<float>(G::float_c34()));
    node.set_visible(1);
    node.set_y(zero);
    node = Node(ui_movies::FindNode(m, 10, 0));
    node.set_x(zero);
    node.set_y(zero);
    // The badge's animation (11, 5, 4, 3 or 2) by the progress bits.
    const uint32_t bits = Machine(machine).progress_bits();
    uint64_t index;
    uint32_t lr;
    if ((bits & 0x20) != 0) {
      index = 5, lr = 0x82189D68;
    } else if ((bits & 0x4) != 0 || (bits & 0x8) != 0) {
      index = 4, lr = 0x82189DC0;
    } else if ((bits & 0x10) != 0) {
      index = 3, lr = 0x82189D98;
    } else {
      index = 2, lr = 0x82189DA8;
    }
    Finish(calls, frame, lr, m, 11, index);
    Play(calls, frame, 0x82189DE0, m, 11, index, 0);
    Finish(calls, frame, 0x82189DF0, m, kSceneRoot, 1);
    Play(calls, frame, 0x82189E10, m, kSceneRoot, 1, 1);
    Finish(calls, frame, 0x82189E20, m, kSceneRoot, 5);
    Play(calls, frame, 0x82189E40, m, kSceneRoot, 5, 1);
    Loop(calls, frame, 0x82189E50, m, kSceneRoot, 5);
    Machine(machine).set_transitions_set_up(1);
  }
  Finish(calls, frame, 0x82189E64, m, 10, 0);
  Play(calls, frame, 0x82189E84, m, 10, 0, 0);
  Loop(calls, frame, 0x82189E94, m, 10, 0);
  Finish(calls, frame, 0x82189EA8, m, 20, uint64_t(Machine(machine).visible_row()) + 2);
  Play(calls, frame, 0x82189ECC, m, 20, uint64_t(Machine(machine).visible_row()) + 2, 0);
  Loop(calls, frame, 0x82189EE0, m, 20, uint64_t(Machine(machine).visible_row()) + 2);
  // Nodes (10, 1) to (10, 9): their children 6 to 9's +80 set (the
  // original's jump table on the index).
  uint64_t r3 = 0;
  auto mark = [&](uint32_t node, uint32_t child) {
    r3 = ui_movies::Child(node, child);
    Node(uint32_t(r3)).set_visible(1);
  };
  for (uint32_t k = 0; k < 9; k++) {
    const uint32_t node = ui_movies::FindNode(m, 10, k + 1);
    switch (k) {
      case 0: case 2: case 5: mark(node, 8); mark(node, 9); break;
      case 1: case 6: mark(node, 6); mark(node, 7); break;
      case 3: case 7: mark(node, 9); break;
      case 4: mark(node, 8); break;
      default: mark(node, 7); break;
    }
  }
  if (Machine(machine).back_from_question() != 0) {
    EndTransition();
    Machine(machine).set_back_from_question(0);
  }
  return r3;
}

uint64_t Transition5(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218A7F0 RaceMenu::Transition5");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t m = FindMovie(kMovie, frame + 80);
  if (Machine(machine).transitions_set_up() == 0) {
    Finish(calls, frame, 0x8218A83C, m, 20, uint64_t(Machine(machine).visible_row()) + 14);
    Play(calls, frame, 0x8218A860, m, 20, uint64_t(Machine(machine).visible_row()) + 14, 1);
    Finish(calls, frame, 0x8218A870, m, kSceneRoot, 2);
    Play(calls, frame, 0x8218A890, m, kSceneRoot, 2, 0);
    Machine(machine).set_transitions_set_up(1);
  }
  Finish(calls, frame, 0x8218A8A4, m, 11, 12);
  Play(calls, frame, 0x8218A8C4, m, 11, 12, 1);
  const uint64_t r3 = Loop(calls, frame, 0x8218A8D4, m, 11, 12);
  if (Machine(machine).back_from_question() != 0) {
    EndTransition();
    Machine(machine).set_back_from_question(0);
  }
  return r3;
}

uint64_t AskConfirm(uint32_t machine, uint32_t mode, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218ACE8 RaceMenu::AskConfirm");
  const uint32_t frame = Frame(sp, 128);
  Store(frame + 80, 16);
  Machine(machine).set_takes_input(0);
  const uint32_t m = FindMovie(16, frame + 80);
  // The question and the two answers (SetText's varargs the call sites
  // leave unset passed 0).
  calls.CallArgs(kSetText, frame, 0x8218AD34, {m, kSceneRoot, 0, 10, 0, 0, 0, 0});
  calls.CallArgs(kSetText, frame, 0x8218AD48, {m, 1, 3, 0, 0, 0, 0, 0});
  calls.CallArgs(kSetText, frame, 0x8218AD5C, {m, 1, 4, 1, 0, 0, 0, 0});
  // The dialog's first cursor: yes when player 0's save +0x1F870 is set.
  const uint32_t save = vehicle_accessors::SaveDataOf(0);
  Store(m + 40960, Save(save).profile_option() != 0 ? 0 : 1);
  const Pages pages = G::pages();
  const uint32_t page = pages.page();
  if (page != 8) {
    pages.set_previous_page(page);
  }
  pages.set_page(8);
  pages.set_step(kNone);
  pages.set_queued_step(kNone);
  // Message 34 (open the yes/no dialog) on the mode's ring; r3 the slot's
  // quotient times 50, as the original's divide leaves it.
  const schema::RaceMenuMode ring(mode);
  const uint32_t at = ring.message_start() + ring.message_count();
  Post(mode, 34);
  const uint64_t r3 = uint64_t(at / 50) * 50;
  EndTransition();
  return r3;
}

uint64_t Transition12(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218AE48 RaceMenu::Transition12");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t m = FindMovie(kMovie, frame + 80);
  Finish(calls, frame, 0x8218AE84, m, 10, 0);
  Play(calls, frame, 0x8218AEA4, m, 10, 0, 0);
  const uint64_t r3 = Loop(calls, frame, 0x8218AEB4, m, 10, 0);
  EndTransition();
  return r3;
}

// The debug pages: their texts go to the empty sub_82106540 (audio.md):
// those calls, and the loads only they use, are left out; the frame words
// the originals store for them are written.
uint64_t DebugKindPage(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218BB68 RaceMenu::DebugKindPage");
  const uint32_t frame = Frame(sp, 128);
  Store(frame + 80, Text(-24252));
  Store(frame + 84, Text(-24244));
  Store(frame + 88, Text(-24264));
  return 190;
}

uint64_t DebugEventPage(uint32_t machine, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82188A98 RaceMenu::DebugEventPage");
  const uint32_t frame = Frame(sp, 240);
  static constexpr int32_t kTexts[] = {-24384, -24404, -24424, -24448, -24468,
                                       -24320, -24364, -24372, -24396, -24416,
                                       -24440, -24460, -24480, -24492, -24504};
  for (uint32_t k = 0; k < 15; k++) {
    Store(frame + 80 + 4 * k, Text(kTexts[k]));
  }
  return Machine(machine).debug_count() != 0 ? 190 : 170;
}

uint64_t DebugVariantPage(uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82188C00 RaceMenu::DebugVariantPage");
  const uint32_t frame = Frame(sp, 128);
  Store(frame + 84, Text(-24300));
  Store(frame + 80, Text(-24308));
  // The last menu step moved: to page 5 through transition 10.
  if (G::menu().moved() != 0) {
    const Pages pages = G::pages();
    const uint32_t page = pages.page();
    if (int32_t(page) != 5) {
      pages.set_previous_page(page);
    }
    pages.set_queued_step(kNone);
    pages.set_step(10);
    pages.set_next_page(5);
    pages.set_page(kNone);
  }
  return 190;
}

uint64_t AnimateVariantPage(uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218A928 RaceMenu::AnimateVariantPage");
  const uint32_t frame = Frame(sp, 128);
  // Player 0's mapper entry, read once.
  const uint32_t entry =
      menu_helpers::kMapperEntries + (Load(menu_helpers::kControllerEntries) << 3);
  const uint32_t pair = FindPair(kMovie, frame + 80);
  const uint32_t m = Load(pair);
  uint64_t r3 = pair;
  // The mapper's slot on `action`, unless the entry's +4 is 1; whether its
  // answer's low byte is set.
  auto ask = [&](uint32_t slot, uint64_t action, uint32_t lr) {
    if (int32_t(Entry(entry).unread()) == 1) {
      return false;
    }
    const uint32_t mapper = Entry(entry).mapper<schema::View>().address;
    r3 = calls.Call(Load(Load(mapper) + slot), frame, lr, mapper, action);
    return (r3 & 0xFF) != 0;
  };
  if (ask(kFiredOffset, 153, 0x8218A990) || ask(kFiredOffset, 167, 0x8218A9BC)) {
    // Up: with +10104 1, back to the first variant.
    if (int32_t(Machine(machine).previous_variant_cursor()) != 1) {
      return r3;
    }
    Finish(calls, frame, 0x8218AC74, m, 8, 3);
    Play(calls, frame, 0x8218AC98, m, 8, uint64_t(Machine(machine).variant_cursor()) + 2, 1);
    Loop(calls, frame, 0x8218ACAC, m, 8, uint64_t(Machine(machine).variant_cursor()) + 2);
    Finish(calls, frame, 0x8218ACBC, m, 8, 12);
    return Play(calls, frame, 0x8218ACDC, m, 8, 12, 1);
  }
  if (ask(kFiredOffset, 154, 0x8218A9E8) || ask(kFiredOffset, 168, 0x8218AA14)) {
    // Down: with a second variant (the cursor's +9984 not -1) and +10104 0.
    const uint32_t cursor = Machine(machine).event_cursor();
    if (int32_t(EventKindAt(machine, cursor).variant_kind()) == -1 ||
        Machine(machine).previous_variant_cursor() != 0) {
      return r3;
    }
    Finish(calls, frame, 0x8218AC08, m, 8, 2);
    Play(calls, frame, 0x8218AC2C, m, 8, uint64_t(Machine(machine).variant_cursor()) + 2, 1);
    Loop(calls, frame, 0x8218AC40, m, 8, uint64_t(Machine(machine).variant_cursor()) + 2);
    Finish(calls, frame, 0x8218AC50, m, 8, 13);
    return Play(calls, frame, 0x8218ACDC, m, 8, 13, 1);
  }
  if (ask(kPressedOffset, 159, 0x8218AA40)) {
    Finish(calls, frame, 0x8218AA58, m, 8, 2);
    Finish(calls, frame, 0x8218AA68, m, 8, 3);
    Finish(calls, frame, 0x8218AA78, m, kSceneRoot, 2);
    Finish(calls, frame, 0x8218AA88, m, 11, 12);
    return Finish(calls, frame, 0x8218AA98, m, 10, 0);
  }
  if (!ask(kPressedOffset, 158, 0x8218AAC0)) {
    return r3;
  }
  calls.Call(kResetScroll, frame, 0x8218AAD8, m, 1, 0);
  Finish(calls, frame, 0x8218AAE8, m, 8, 2);
  Finish(calls, frame, 0x8218AAF8, m, 8, 3);
  Finish(calls, frame, 0x8218AB08, m, 10, 0);
  Finish(calls, frame, 0x8218AB18, m, kSceneRoot, 2);
  Finish(calls, frame, 0x8218AB28, m, 11, 12);
  Finish(calls, frame, 0x8218AB38, m, kSceneRoot, 6);
  Finish(calls, frame, 0x8218AB48, m, kSceneRoot, 4);
  Play(calls, frame, 0x8218AB68, m, kSceneRoot, 4, 1);
  if (int32_t(Machine(machine).variant_cursor()) == 1) {
    Finish(calls, frame, 0x8218AB84, m, 8, 12);
    Play(calls, frame, 0x8218ABA4, m, 8, 12, 1);
    Machine(machine).set_variant_cursor(0);
    Machine(machine).set_previous_variant_cursor(0);
  }
  Finish(calls, frame, 0x8218ABC4, m, 20, uint64_t(Machine(machine).visible_row()) + 28);
  return Play(calls, frame, 0x8218ACDC, m, 20, uint64_t(Machine(machine).visible_row()) + 28, 1);
}

uint64_t Update(uint64_t machine_r3, uint64_t mode_r4, uint32_t sp,
                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218C678 RaceMenu::Update");
  const uint32_t machine = uint32_t(machine_r3);
  const uint32_t frame = Frame(sp, 176);
  // The debug print through the empty sub_82106540: left out.
  const uint32_t m = FindMovie(kMovie, frame + 80);
  // UiMovie::AnimationTime's plain C++ (r3 the movie, left; the time in f1,
  // passed to the next guest call Update makes itself: its callees don't
  // read it).
  double f1 = 0.0;
  const double* pending = nullptr;
  auto time = [&](uint32_t index) {
    f1 = double(std::bit_cast<float>(ui_movies::AnimationTime(m, kNone, index)));
    pending = &f1;
    return f1;
  };
  auto call = [&](uint32_t function, uint32_t lr, uint64_t r4, uint64_t r5, uint64_t r6) {
    const double* f = pending;
    pending = nullptr;
    return calls.Call(function, frame, lr, m, r4, r5, r6, f);
  };
  bool scene = time(1) == double(G::scene_time());
  const double b = double(G::variant_time());
  if (!scene) {
    scene = time(4) == b;
  }
  if (scene) {
    call(kFinish, 0x8218C718, kSceneRoot, 6, 0);
    call(kPlay, 0x8218C738, kSceneRoot, 6, 0);
    call(kLoop, 0x8218C748, kSceneRoot, 6, 0);
  }
  if (time(2) == b ||
      (Machine(machine).back_from_question() == 1 && Machine(machine).page() == 5)) {
    call(kShowNode, 0x8218C788, 8, 0, 0);
    call(kFinish, 0x8218C79C, 8, uint64_t(Machine(machine).variant_cursor()) + 2, 0);
    call(kPlay, 0x8218C7C0, 8, uint64_t(Machine(machine).variant_cursor()) + 2, 0);
    call(kLoop, 0x8218C7D4, 8, uint64_t(Machine(machine).variant_cursor()) + 2, 0);
  }
  if (Machine(machine).refresh_texts() == 1) {
    const uint64_t refreshed = Refresh(machine_r3, frame, calls);
    Machine(machine).set_refresh_texts(uint32_t(refreshed) == 0 ? 1 : 0);
  }
  switch (Machine(machine).page()) {
    case 3: DebugKindPage(frame); break;
    case 4: DebugEventPage(machine, frame); break;
    case 5: DebugVariantPage(frame); break;
    default: break;
  }
  // The page object's transition (+12): its set-up once (+24), then
  // UiMovie::Run(movie 1, 0); the transition ended when Run answers 0.
  const Pages pages = G::pages();
  const uint32_t transition = pages.step();
  const uint32_t movie = FindMovie(kMovie, frame + 80);
  auto run = [&](uint32_t lr) {
    const double* f = pending;
    pending = nullptr;
    return calls.Call(kRun, frame, lr, movie, 0, 0, 0, f);
  };
  if (transition == kNone) {
    return run(0x8218C9BC);
  }
  if (transition > 12) {
    const uint64_t r3 = run(0x8218C974);
    EndTransition();
    return r3;
  }
  if (pages.step_started() == 0) {
    pages.set_step_started(1);
    switch (transition) {
      case 0: Transition0(machine, frame, calls); break;  // the piece sub_8218C8A8
      case 1: EndTransition(); break;
      case 3: Transition3(machine, frame, calls); break;
      case 5: Transition5(machine, frame, calls); break;
      case 6: AnimateVariantPage(machine, frame, calls); break;
      case 11: AskConfirm(machine, uint32_t(mode_r4), frame, calls); break;
      case 12: Transition12(frame, calls); break;
      default: break;
    }
  }
  const uint64_t r3 = run(0x8218C934);
  if (uint32_t(r3) == 0) {
    EndTransition();
  }
  return r3;
}

}  // namespace race_menu
