// The type 8 mode's first screen (docs/functions/type8-mode.md):
// Type8Screen0's constructor, destructors and its simpler handlers and
// pages, over movie 24 (the menu) and movie 16 (the yes/no dialog). See
// type8_mode.h. UiMovies::Find, IsLoaded, UiMovie::FindNode, the node
// setters, MenuList::SetSelection and Selection and ScreenMachine's
// constructor through their plain C++; UiMovie::SetText through the calls'
// set_text, MenuList::Init and Release through their plain C++ where they
// free natively; UiMovies::Open and Delete through the calls; FinishAll,
// operator delete and the pages not ours as guest calls.

#include "type8_mode/type8_mode.h"

#include "base/log.h"
#include "boot_mode/boot_mode.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "race_screens/race_screens.h"
#include "save_data/save_data.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/race_screens.h"
#include "schema/type8_mode.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace type8_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;

using S = schema::Type8Screen0;
using Stand = schema::Type8StandBlock;
using Pages = schema::PageObject;
using Machine = schema::StateMachine;
using G = schema::Type8ModeGlobals;

constexpr uint32_t kMinusOne = 0xFFFFFFFF;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kOperatorDelete = 0x825A70E8;
constexpr uint32_t kListInit = 0x8220BAC0;

// The menu list's r3 (the screen's r3 plus the list's offset, as the
// original's addi on the whole register).
uint64_t ListR3(uint64_t r3) {
  return r3 + S(0).menu_list_address();
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(frame, sp);
  return frame;
}

// The movie with id `id`: UiMovies::Find with the id as a value (the
// original's at the frame's +80; E5 step 6: Leave stores it there itself,
// as a guest call gets that slot's address).
uint32_t MovieAt([[maybe_unused]] uint32_t frame, uint32_t id) {
  return LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, id));
}

// A message (`message`, 0, 0) posted in the dispatcher's ring (+204), the
// words in the original's order (`id_last`: +8, +4, then the id).
void Post(uint32_t dispatcher, uint32_t message, bool id_last = false) {
  const schema::ModeDispatcher d(dispatcher);
  const uint32_t count = d.ring_count() + 1;
  const uint32_t end = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const game_modes::RingSlot slot{d.ring() + (end - (end / 50) * 50) * schema::ModeMessage::kSize};
  if (id_last) {
    slot.set_arg1(0);
    slot.set_arg0(0);
    slot.set_id(message);
  } else {
    slot.set_id(message);
    slot.set_arg0(0);
    slot.set_arg1(0);
  }
}

// UiMovie::ShowNode and HideNode's work (+80 = 1 or 0); the node.
uint32_t SetShown(uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const uint32_t node = ui_movies::FindNode(movie, sub_scene, child);
  StoreU32(node + 80, shown);
  return node;
}

uint32_t Texture(uint32_t movie, uint32_t child, uint32_t texture) {
  return ui_movies::SetNodeTexture(ui_movies::FindNode(movie, 6, child), texture);
}

}  // namespace

uint64_t Screen0Construct(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223D2F0 Type8Screen0::Type8Screen0");
  const S s(uint32_t(r3));
  (void)sp;  // its frame held only the back chain (E5 step 6)
  boot_mode::ScreenMachineConstruct(s.machine_address());
  s.set_vtable(S::kVtable);
  const schema::MenuList list(s.menu_list_address());
  list.set_enabled(0u);
  list.set_player(0);
  for (const uint32_t at : {s.stand_0_address(), s.stand_1_address()}) {
    const Stand block(at);
    block.set_loading_shown(0);
    block.set_loaded_anim(kMinusOne);
    block.set_loading_loop_anim(kMinusOne);
    block.set_loading_start_anim(kMinusOne);
    block.set_model_made(0);
  }
  const Stand stand0(s.stand_0_address()), stand1(s.stand_1_address());
  stand0.set_loading_shown(0);
  stand0.set_model_made(0);
  stand1.set_model_made(0);
  stand1.set_loading_shown(0);
  stand0.set_loading_start_anim(45);
  stand0.set_loading_loop_anim(44);
  stand0.set_loaded_anim(46);
  s.set_menu_input(1);
  stand1.set_loading_start_anim(48);
  stand1.set_loading_loop_anim(47);
  stand1.set_loaded_anim(49);
  return r3;
}

uint64_t Screen0Destruct(uint64_t r3, uint32_t sp, uint32_t lr,
                         const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BF00 Type8Screen0::~Type8Screen0");
  const S s(uint32_t(r3));
  s.set_vtable(S::kVtable);
  return type67_screens::ReleaseList(ListR3(r3), sp, lr, calls);
}

uint64_t Screen0DeletingDestruct(uint64_t r3, uint64_t flags, uint32_t sp,
                                 const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C528 Type8Screen0::DeletingDestructor");
  const uint32_t frame = Frame(sp, 112);
  const S s(uint32_t(r3));
  s.set_vtable(S::kVtable);
  type67_screens::ReleaseList(ListR3(r3), frame, 0x8223C558, calls);
  if ((flags & 1) != 0) {
    const uint64_t args[6] = {r3, 0, 0, 0, 0, 0};
    calls.call_set(kOperatorDelete, args, 1, frame, 0x8223C568);
  }
  return r3;
}

uint64_t Screen0OnEnter(uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B630 Type8Screen0::OnEnter");
  const S s(uint32_t(r3));
  const Pages pages(s.pages_address());
  const uint32_t frame = Frame(sp, 144);
  s.set_dialog_answer(kMinusOne);
  s.set_menu_input(1);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(kMinusOne);
  pages.set_step(kMinusOne);
  pages.set_step_started(0);
  const uint32_t list = s.menu_list_address();
  const save_data::MenuListArgs args{5, 5, 1, 154, 153, 155, 156, 159, 158};
  if (calls.native_heap && save_data::MenuListInitNative(list)) {
    save_data::MenuListInit(list, args);
  } else {
    // The ninth and tenth arguments (159, 158) on the stack at the frame's
    // +84 and +92, as the original passes them (only the guest call reads
    // them: stored here since E5 step 6).
    StoreU32(frame + 92, 158);
    StoreU32(frame + 84, 159);
    const uint64_t regs[8] = {ListR3(r3), 5, 5, 1, 154, 153, 155, 156};
    calls.call8(kListInit, regs, frame, 0x8223B6A4);
  }
  race_screens::ListSetSelection(list, 0);
  const Machine machine(s.machine_address());
  machine.set_state(machine.reenter() != 0 ? 1 : 9);
  return ListR3(r3);
}

uint64_t Screen0OpenMovie(uint32_t sp, uint32_t lr, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B530 Type8Screen0::OpenMovie");
  return calls.open(24, 22583, sp, lr);
}

uint64_t Screen0WaitForMovie(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223B6D8 Type8Screen0::WaitForMovie");
  const Machine machine(S(uint32_t(r3)).machine_address());
  (void)sp;  // its frame held only the back chain (E5 step 6)
  if (machine.loaded() != 0) {
    return r3;
  }
  const uint64_t loaded = ui_movies::IsLoaded(24);
  if (uint32_t(loaded) == 0) {
    return loaded;
  }
  machine.set_loaded(1);
  machine.set_state(G::mode_before() == 1 ? 4 : 9);
  return loaded;
}

uint64_t Screen0Suspend(uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B540 Type8Screen0::Suspend");
  const S s(uint32_t(r3));
  const uint32_t frame = Frame(sp, 96);
  if (s.delete_movie() == 0) {
    return r3;
  }
  const uint64_t result = calls.remove(24, frame, 0x8223B568);
  Machine(s.machine_address()).set_loaded(0);
  return result;
}

uint64_t Screen0Pick(uint64_t r3, uint32_t answer) {
  switch (answer) {
    case 1:
      ITC_NATIVE_RUNNING("sub_8223B590 Type8Screen0::PickYes");
      break;
    case 0:
      ITC_NATIVE_RUNNING("sub_8223B5A0 Type8Screen0::PickNo");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_8223B5B0 Type8Screen0::PickBack");
      break;
  }
  S(uint32_t(r3)).set_dialog_answer(answer);
  return r3;
}

uint64_t Screen0ShowPage(uint64_t a, uint64_t b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223BD20 Type8Screen0::ShowPage");
  // No guest frame (E5 step 6): the id 24 passed to UiMovies::Find as a value.
  (void)sp;
  const uint32_t movie =
      LoadU32(ui_movies::PairsFindKey(ui_movies::kMovieTable, 24));
  const bool first = int32_t(a) != 0, second = int32_t(b) != 0;
  if (!first && !second) {
    SetShown(movie, kMinusOne, 6, 0);
  } else {
    SetShown(movie, kMinusOne, 6, 1);
  }
  if (first) {
    SetShown(movie, 6, 4, 1);
    SetShown(movie, 6, 5, 1);
    Texture(movie, 4, 0);
    Texture(movie, 5, 0);
  } else {
    SetShown(movie, 6, 4, 0);
    SetShown(movie, 6, 5, 0);
  }
  if (second) {
    SetShown(movie, 6, 6, 1);
    SetShown(movie, 6, 7, 1);
    Texture(movie, 6, 1);
    Texture(movie, 7, 1);
  } else {
    SetShown(movie, 6, 6, 0);
    SetShown(movie, 6, 7, 0);
  }
  SetShown(movie, 6, 8, 0);
  return SetShown(movie, 6, 9, 0);
}

uint64_t Screen0OnDialogClosed(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223C098 Type8Screen0::OnDialogClosed");
  const S s(uint32_t(r3));
  const uint32_t answer = s.dialog_answer();
  s.set_menu_input(1);
  if (answer == 1) {
    return r3;
  }
  return Screen0ShowPage(1, 0, sp);
}

uint64_t Screen0Leave(uint32_t dispatcher, uint32_t sp,
                      const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BA48 Type8Screen0::Leave");
  const uint32_t frame = Frame(sp, 112);
  StoreU32(frame + 80, 24);  // FinishAll gets this slot's address
  const uint32_t movie = MovieAt(frame, 24);
  uint64_t r3 = movie;
  if (movie != 0) {
    calls.call(kFinishAll, frame, 0x8223BA84, movie, frame + 80);
    r3 = calls.remove(24, frame, 0x8223BA8C);
  }
  Post(dispatcher, 4);
  return r3;
}

uint64_t Screen0OnLeft(uint64_t r3, uint32_t sp, uint32_t lr,
                       const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B588 Type8Screen0::OnLeft");
  return type67_screens::ReleaseList(ListR3(r3), sp, lr, calls);
}

uint64_t Screen0Confirm(uint64_t r3, uint32_t dispatcher, uint32_t sp,
                        const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C430 Type8Screen0::Confirm");
  const S s(uint32_t(r3));
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = MovieAt(frame, 16);
  calls.set_text(frame, 0x8223C474, movie, kMinusOne, 0, 27);
  calls.set_text(frame, 0x8223C488, movie, 1, 3, 2);
  calls.set_text(frame, 0x8223C49C, movie, 1, 4, 3);
  StoreU32(movie + 40960, 1);
  G::set_dialog_player(0);
  Post(dispatcher, 7);
  s.set_dialog_answer(kMinusOne);
  s.set_menu_input(0);
  return Screen0ShowPage(1, 1, frame);
}

uint64_t Screen0ConfirmPage(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8223B5C0 Type8Screen0::ConfirmPage");
  const S s(uint32_t(r3));
  const Pages pages(s.pages_address());
  const uint32_t answer = s.dialog_answer();
  if (answer == 0 || answer == 2) {
    const uint32_t page = pages.page();
    if (page != 0) {
      pages.set_previous_page(page);
    }
    pages.set_step(kMinusOne);
    pages.set_page(0);
    pages.set_queued_step(kMinusOne);
    s.set_dialog_answer(kMinusOne);
  } else if (answer == 1) {
    const uint32_t page = pages.page();
    if (page != 3) {
      pages.set_previous_page(page);
    }
    pages.set_step(3);
    pages.set_queued_step(kMinusOne);
    pages.set_next_page(3);
    pages.set_page(kMinusOne);
  }
  return r3;
}

uint64_t Screen0ChosenPage(uint64_t r3, uint32_t dispatcher, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223C0C0 Type8Screen0::ChosenPage");
  const S s(uint32_t(r3));
  const Machine machine(s.machine_address());
  // No guest frame (E5 step 6): its callees are native and read no slot.
  const uint32_t frame = sp - 112;
  const uint32_t selection = race_screens::ListSelection(s.menu_list_address());
  static constexpr uint32_t kMessages[4] = {16, 14, 19, 21};
  if (selection >= 4) {
    const uint64_t result = Screen0ShowPage(0, 0, frame);
    machine.set_state(6);
    return result;
  }
  machine.set_state(9);
  Post(dispatcher, kMessages[selection], true);
  return selection;
}

uint64_t Screen0PlaceCursor(uint64_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223BE88 Type8Screen0::PlaceCursor");
  // No guest frame (E5 step 6).
  const uint32_t movie = MovieAt(sp, 24);
  // mulli and extsw, the doubleword (the original's through the frame's +80:
  // std, lfd, fcfid, frsp; a value since E5 step 6).
  const int64_t value = int64_t(int32_t(uint32_t(item * 40)));
  const uint32_t node = ui_movies::FindNode(movie, 0, 0);
  guest::StoreFloat(node + 16, float(double(value)));
  return node;
}

uint64_t Screen0Draw(uint64_t r3, uint64_t r4, uint32_t sp,
                     const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C888 Type8Screen0::Draw");
  constexpr uint32_t kTable = G::page_table_address();
  const uint32_t frame = Frame(sp, 128);
  const uint32_t guard = G::page_table_guard();
  if ((guard & 1) == 0) {
    // The pages' member functors (the original builds them at the frame's
    // +80 to +108; a C++ array since E5 step 6): MenuPage, ConfirmPage, the
    // empty handler, ChosenPage, each with adjustment 0.
    static constexpr uint32_t kPages[8] = {0x8223C6B8, 0, 0x8223B5C0, 0,
                                           0x82106540, 0, 0x8223C0C0, 0};
    G::set_page_table_guard(guard | 1);
    for (uint32_t i = 0; i < 8; i++) {
      StoreU32(kTable + 4 * i, kPages[i]);
    }
  }
  const uint32_t page = Pages(S(uint32_t(r3)).pages_address()).page();
  if (page == kMinusOne) {
    return r3;
  }
  const uint32_t target = LoadU32(kTable + (page << 3));
  const uint32_t adjustment = LoadU32(kTable + 4 + (page << 3));
  const uint64_t object = uint64_t(adjustment) + r3;
  switch (target) {
    case 0x8223C6B8:
      return Screen0MenuPage(object, r4, frame, calls);
    case 0x8223B5C0:
      return Screen0ConfirmPage(object);
    case 0x8223C0C0:
      return Screen0ChosenPage(object, uint32_t(r4), frame);
    default:
      return calls.call(target, frame, 0x8223C940, object, r4);
  }
}

}  // namespace type8_mode
