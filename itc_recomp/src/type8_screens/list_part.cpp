// The type 8 list screen's list part (Type8ListPart over ListPartBase), its
// inner part (Type8ListPartInner over ListPartInnerBase) with a block a
// player (Type8PlayerBlock: two menus), and the inner parts' shared step 1
// and pages 1 and 2 (docs/functions/type8-screens.md): see type8_screens.h.
// Read from the generated code. UiMovies::Find, UiMovie::FindNode, Child,
// PlayNamed and IsNamedActive, MenuList::Selection and SetSelection, the
// base classes' constructors and destructors and ScreenFades::FadeOut
// through their plain C++; MenuSound::Play's plain C++ with its slot call
// through Calls; UiMovie::ResetPose, Run, FinishAll, FinishNamed,
// LoopNamed, SetNodeText (r6 to r10 0, the user's call for its unset
// varargs), MenuList::Update, Accepted and Cancelled guest calls to their
// overrides; UiMovie::SetText through Calls::set_text; MenuList::Init and
// Release by their plain C++ where they free natively, else guest calls.

#include "type8_screens/type8_screens.h"

#include "type8_screens/part_helpers.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "race_screens/race_screens.h"
#include "save_data/save_data.h"
#include "schema/tuning_screen.h"
#include "schema/type67_screens.h"
#include "schema/type8_screens.h"
#include "screen_constructors/screen_constructors.h"
#include "type67_screens/list_pages.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace type8_screens {

using guest::LoadU32;
using guest::StoreU32;

namespace ts = type67_screens;
namespace rs = race_screens;
using G = schema::Type8ScreensGlobals;

namespace part_detail {

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

uint32_t MovieAt(uint8_t* base, uint32_t id_word, uint32_t id) {
  StoreU32(base, id_word, id);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, id_word));
}

uint64_t CallTarget(uint8_t* base, uint32_t target, uint64_t r3, std::optional<uint64_t> r4,
                    std::optional<uint64_t> r5, uint32_t frame, uint32_t lr, const Calls& calls) {
  if (ts::SlotIsNative(target, r4.has_value(), r5.has_value())) {
    if (r4) {
      calls.set_r4(*r4);
    }
    return *ts::SlotNative(base, target, r3, r4, frame, lr, calls, r5);
  }
  const uint64_t args[6] = {r3, r4.value_or(0), r5.value_or(0), 0, 0, 0};
  return calls.call_set(target, args, r5 ? 3 : (r4 ? 2 : 1), frame, lr);
}

void Delete(uint64_t object, uint64_t flags, uint32_t frame, uint32_t lr, const Calls& calls) {
  const uint64_t args[6] = {object, flags, 0, 0, 0, 0};
  calls.call_set(car_list::kOperatorDelete, args, 2, frame, lr);
}

uint64_t SetNodeText(uint32_t movie, uint32_t node, uint32_t id, uint32_t frame, uint32_t lr,
                     const Calls& calls, uint64_t r6) {
  const uint64_t args[8] = {movie, node, id, r6, 0, 0, 0, 0};
  return calls.call8(kSetNodeText, args, frame, lr);
}

void InitList(uint8_t* base, uint32_t list, const save_data::MenuListArgs& a, uint32_t frame,
              uint32_t lr, const Calls& calls) {
  StoreU32(base, frame + 92, a.id6);
  StoreU32(base, frame + 84, a.id5);
  if (calls.native_heap && save_data::MenuListInitNative(base, list)) {
    save_data::MenuListInit(base, list, a);
    return;
  }
  const uint64_t args[8] = {list, a.count, a.shown, a.r6, a.r7, a.r8, a.r9, a.r10};
  calls.call8(rs::kListInit, args, frame, lr);
}

uint64_t Restart(uint8_t* base, uint32_t movie, uint32_t named_word, uint32_t frame,
                 uint32_t lr_finish, uint32_t lr_loop, const Calls& calls) {
  calls.call(kFinishNamed, frame, lr_finish, movie, LoadU32(base, named_word));
  ui_movies::PlayNamed(base, movie, LoadU32(base, named_word), 1, 0, 0, 0);
  return calls.call(kLoopNamed, frame, lr_loop, movie, LoadU32(base, named_word));
}

void MoveOn(uint8_t* base, uint32_t inner) {
  const schema::Type8PartInner in(base, inner);
  const uint32_t queued = in.queued_step();
  if (queued == 0xFFFFFFFFu) {
    const uint32_t after = in.next_page();
    in.set_step(0xFFFFFFFFu);
    in.set_page(after);
  } else {
    in.set_unknown_24(0);
    in.set_step(queued);
    in.set_queued_step(0xFFFFFFFFu);
  }
  in.set_step_started(0);
}

uint64_t InnerInput(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t steps, uint32_t lr_step,
                    uint32_t lr_run, uint32_t sp, const Calls& calls) {
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  const schema::Type8PartInner in(base, inner);
  const uint32_t step = in.step();
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(r4));
  if (step == 0xFFFFFFFFu) {
    return calls.call(ts::kRun, frame, lr_run, movie, 0);
  }
  const uint32_t target = LoadU32(base, steps + (step << 2));
  const uint64_t done = CallTarget(base, target, r3, r4, std::nullopt, frame, lr_step, calls);
  if (int32_t(uint32_t(done)) != 0) {
    MoveOn(base, inner);
  }
  return done;
}

uint64_t InnerPick(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t pages,
                   uint32_t sp, uint32_t lr, const Calls& calls) {
  const uint32_t page = schema::Type8PartInner(base, uint32_t(r3)).page();
  if (page == 0xFFFFFFFFu) {
    return 0;
  }
  const uint32_t target = LoadU32(base, pages + (page << 2));
  return CallTarget(base, target, r3, r4, r5, sp, lr, calls);
}

void OpenBegin(uint8_t* base, uint32_t inner, uint32_t id, uint32_t frame, uint32_t lr,
               const Calls& calls) {
  const schema::Type8PartInner in(base, inner);
  in.set_next_page(0);
  in.set_page(0);
  in.set_previous_page(0);
  in.set_unknown_24(0xFFFFFFFFu);
  in.set_step(0xFFFFFFFFu);
  in.set_step_started(0);
  in.set_step(0);
  in.set_queued_step(0xFFFFFFFFu);
  in.set_next_page(0);
  const uint32_t movie = MovieAt(base, frame + 80, id);
  in.set_page(0xFFFFFFFFu);
  calls.call(ts::kResetPose, frame, lr, movie, frame + 80);
}

void Page0End(uint8_t* base, uint32_t inner, uint64_t first, uint64_t second, uint32_t backed,
              const Calls& calls) {
  const schema::Type8PartInner in(base, inner);
  if (int32_t(uint32_t(first)) != 0 && int32_t(uint32_t(second)) != 0) {
    const uint32_t page = in.page();
    if (page != 1) {
      in.set_previous_page(page);
    }
    in.set_page(1);
    in.set_step(0xFFFFFFFFu);
    in.set_queued_step(0xFFFFFFFFu);
    ts::pages::FadeOut(base, calls);
    return;
  }
  if (LoadU32(base, backed) == 0) {
    return;
  }
  const uint32_t page = in.page();
  if (page != 2) {
    in.set_previous_page(page);
  }
  in.set_step(1);
  in.set_queued_step(0xFFFFFFFFu);
  in.set_next_page(2);
  in.set_page(0xFFFFFFFFu);
}

}  // namespace part_detail

using namespace part_detail;

// --- The player block ---

uint64_t PlayerBlockConstruct(uint8_t* base, uint64_t r3, uint64_t player) {
  ITC_NATIVE_RUNNING("sub_822542B8 Type8PlayerBlock::Type8PlayerBlock");
  const uint32_t b = uint32_t(r3), p4 = uint32_t(player) << 2, p8 = uint32_t(player) << 3;
  const schema::Type8PlayerBlock blk(base, b);
  constexpr uint32_t t = G::player_block_ids_address() + 40;  // 0x82011458
  blk.set_menu_block(0);
  blk.set_menu_player(0);
  blk.set_setting_block(0);
  blk.set_setting_player(0);
  blk.set_player(uint32_t(player));
  blk.set_backed_out(0);
  blk.set_node(5);
  blk.set_unknown_164(7);
  blk.set_node_set(LoadU32(base, t + 48 + p4));
  blk.set_item_anim0(LoadU32(base, t + 8 + p4));
  blk.set_item_anim1(LoadU32(base, t + 16 + p4));
  blk.set_unknown_188(LoadU32(base, t - 40 + p8));
  blk.set_ready_anim(LoadU32(base, t - 36 + p8));
  blk.set_opening_anim_a(LoadU32(base, t - 24 + p8));
  blk.set_opening_anim_b(LoadU32(base, t - 20 + p8));
  blk.set_setting_anim0(LoadU32(base, t + 24 + p8));
  blk.set_setting_anim1(LoadU32(base, t + 28 + p8));
  blk.set_text_node(6);
  blk.set_text_child_a(10);
  blk.set_text_child_b(11);
  blk.set_setting_item_anim(LoadU32(base, t + 40 + p4));
  blk.set_selection_anim_a(LoadU32(base, t - 8 + p4));
  blk.set_cursor_child_a(8);
  blk.set_cursor_child_b(9);
  blk.set_selection_anim_b(LoadU32(base, t + p4));
  return r3;
}

uint64_t PlayerBlockSetUp(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822545D0 Type8PlayerBlock::SetUp");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 160);
  const schema::Type8PlayerBlock blk(base, b);
  const uint32_t pad = LoadU32(base, G::pad_players_address() + (blk.player() << 2));
  InitList(base, blk.menu_address(), {2, 2, 1, 154, 153, 155, 156, 159, 158}, frame, 0x8225462C,
           calls);
  blk.set_menu_player(pad);
  InitList(base, blk.setting_address(), {2, 2, 1, 156, 155, 154, 153, 159, 158}, frame, 0x8225465C,
           calls);
  const uint32_t movie = MovieAt(base, frame + 96, uint32_t(id));
  blk.set_setting_player(pad);
  calls.set_text(frame, 0x8225468C, movie, blk.node_set(), blk.text_node(), 11);
  const uint32_t node =
      ui_movies::FindNode(base, movie, blk.node_set(), blk.node());
  SetNodeText(movie, ui_movies::Child(base, node, blk.text_child_a()), 0, frame, 0x822546B8,
              calls);
  const uint64_t result = SetNodeText(
      movie, ui_movies::Child(base, node, blk.text_child_b()), 1, frame, 0x822546D4, calls);
  StoreU32(base, movie + 40960, 1);
  blk.set_ready(0);
  blk.set_backed_out(0);
  return result;
}

uint64_t PlayerBlockPlayOpening(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822546F8 Type8PlayerBlock::PlayOpening");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 112);
  const schema::Type8PlayerBlock blk(base, b);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  ui_movies::PlayNamed(base, movie, blk.opening_anim_a(), 1, 0, 0, 0);
  ui_movies::PlayNamed(base, movie, blk.opening_anim_b(), 1, 0, 0, 0);
  return ui_movies::PlayNamed(base, movie, blk.setting_anim0(), 1, 0, 0, 0);
}

uint64_t PlayerBlockShowMenuMove(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254798 Type8PlayerBlock::ShowMenuMove");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 144);
  const schema::Type8PlayerBlock blk(base, b);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  const uint32_t selection = rs::ListSelection(base, blk.menu_address());
  const uint32_t previous = blk.menu_selection_before();
  calls.call(kFinishNamed, frame, 0x822547F8, movie,
             LoadU32(base, blk.item_anims_address() + (previous << 2)));
  uint64_t result = Restart(base, movie, blk.item_anims_address() + (selection << 2), frame,
                            0x8225480C, 0x82254834, calls);
  if (previous == 0) {
    const uint32_t setting = rs::ListSelection(base, blk.setting_address());
    result = calls.call(kFinishNamed, frame, 0x8225485C, movie,
                        LoadU32(base, blk.setting_anims_address() + (setting << 2)));
  }
  if (selection == 1) {
    result =
        Restart(base, movie, blk.setting_item_anim_address(), frame, 0x82254870, 0x82254898, calls);
  } else if (previous == 1) {
    result = calls.call(kFinishNamed, frame, 0x822548B0, movie, blk.setting_item_anim());
  }
  return result;
}

uint64_t PlayerBlockPlayReady(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822548B8 Type8PlayerBlock::PlayReady");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 112);
  const schema::Type8PlayerBlock blk(base, b);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  calls.call(kFinishNamed, frame, 0x822548F8, movie, blk.setting_item_anim());
  calls.call(kFinishNamed, frame, 0x82254904, movie, blk.opening_anim_b());
  calls.call(kFinishNamed, frame, 0x82254910, movie, blk.ready_anim());
  return ui_movies::PlayNamed(base, movie, blk.ready_anim(), 1, 0, 0, 0);
}

uint64_t PlayerBlockPlaceCursor(uint8_t* base, uint64_t r3, uint64_t id, uint64_t selection,
                                uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82254948 Type8PlayerBlock::PlaceCursor");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 128);
  const schema::Type8PlayerBlock blk(base, b);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  const uint32_t node =
      ui_movies::FindNode(base, movie, blk.node_set(), blk.node());
  const bool first = int32_t(uint32_t(selection)) == 0;
  const uint32_t off = first ? blk.cursor_child_a() : blk.cursor_child_b();
  const uint32_t on = first ? blk.cursor_child_b() : blk.cursor_child_a();
  schema::UiMovieNode(base, ui_movies::Child(base, node, off)).set_texture_entry(0);
  const uint32_t child = ui_movies::Child(base, node, on);
  schema::UiMovieNode(base, child).set_texture_entry(1);
  return child;
}

uint64_t PlayerBlockShowSelections(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254AD8 Type8PlayerBlock::ShowSelections");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 128);
  const schema::Type8PlayerBlock blk(base, b);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  const uint32_t selection = rs::ListSelection(base, blk.menu_address());
  Restart(base, movie, blk.item_anims_address() + (selection << 2), frame, 0x82254B28, 0x82254B50,
          calls);
  Restart(base, movie, blk.selection_anim_a_address(), frame, 0x82254B5C, 0x82254B84, calls);
  Restart(base, movie, blk.selection_anim_b_address(), frame, 0x82254B90, 0x82254BB8, calls);
  const uint32_t setting = rs::ListSelection(base, blk.setting_address());
  PlayerBlockPlaceCursor(base, r3, id, setting, frame);
  const uint32_t named = blk.setting_anims_address() + (setting << 2);
  Restart(base, movie, named, frame, 0x82254BE8, 0x82254C10, calls);
  return Restart(base, movie, named, frame, 0x82254C1C, 0x82254C44, calls);
}

uint64_t PlayerBlockWhileReady(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                               const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254A70 Type8PlayerBlock::WhileReady");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 112);
  const schema::Type8PlayerBlock blk(base, b);
  if (int32_t(uint32_t(ts::pages::Call1(calls, rs::kListCancelled, r3 + 12, frame,
                                        0x82254A94))) != 0) {
    ts::pages::PlaySound(base, 2, frame, calls);
    blk.set_ready(0);
    PlayerBlockShowMenuMove(base, r3, id, frame, calls);
  }
  return 1;
}

uint64_t PlayerBlockSettingMoved(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254C50 Type8PlayerBlock::SettingMoved");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 144);
  const schema::Type8PlayerBlock blk(base, b);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(id));
  const uint32_t setting = rs::ListSelection(base, blk.setting_address());
  const uint32_t previous = blk.setting_selection_before();
  calls.call(kFinishNamed, frame, 0x82254CB0, movie,
             LoadU32(base, blk.setting_anims_address() + (previous << 2)));
  Restart(base, movie, blk.setting_anims_address() + (setting << 2), frame, 0x82254CC4, 0x82254CEC,
          calls);
  return PlayerBlockPlaceCursor(base, r3, id, setting, frame);
}

uint64_t PlayerBlockUpdateSetting(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254DB8 Type8PlayerBlock::UpdateSetting");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 112);
  const schema::Type8PlayerBlock blk(base, b);
  const uint64_t result = ts::pages::Call1(calls, rs::kListUpdate, r3 + 84, frame, 0x82254DDC);
  if (blk.setting_move() == 0) {
    return result;
  }
  ts::pages::PlaySound(base, 4, frame, calls);
  return PlayerBlockSettingMoved(base, r3, id, frame, calls);
}

uint64_t PlayerBlockInput(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254EC8 Type8PlayerBlock::Input");
  const uint32_t b = uint32_t(r3), frame = Frame(base, sp, 128);
  const schema::Type8PlayerBlock blk(base, b);
  const uint64_t list = r3 + 12;
  ts::pages::Call1(calls, rs::kListUpdate, list, frame, 0x82254EE8);
  const uint32_t selection = rs::ListSelection(base, uint32_t(list));
  if (int32_t(uint32_t(ts::pages::Call1(calls, rs::kListAccepted, list, frame, 0x82254EFC))) !=
      0) {
    ts::pages::PlaySound(base, 0, frame, calls);
    if (selection == 1) {
      blk.set_ready(1);
      PlayerBlockPlayReady(base, r3, id, frame, calls);
      return 0;
    }
    rs::ListSetSelection(base, uint32_t(list), 1);
    PlayerBlockShowMenuMove(base, r3, id, frame, calls);
    return 0;
  }
  if (int32_t(uint32_t(ts::pages::Call1(calls, rs::kListCancelled, list, frame, 0x82254F44))) !=
      0) {
    ts::pages::PlaySound(base, blk.player() != 0 ? 3 : 2, frame, calls);
    blk.set_backed_out(1);
    return 0;
  }
  if (blk.menu_move() != 0) {
    ts::pages::PlaySound(base, 4, frame, calls);
    PlayerBlockShowMenuMove(base, r3, id, frame, calls);
  } else if (selection == 0) {
    PlayerBlockUpdateSetting(base, r3, id, frame, calls);
  }
  return 0;
}

// --- The list part and its inner part ---

uint64_t ListInnerConstruct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822543C0 Type8ListPartInner::Type8ListPartInner");
  const uint32_t inner = uint32_t(r3);
  const schema::Type8ListPartInner in(base, inner);
  Frame(base, sp, 96);
  screen_constructors::ListPartInnerBase(base, inner);
  in.set_vtable(kListInnerVtable);
  PlayerBlockConstruct(base, r3 + 40, 0);
  PlayerBlockConstruct(base, r3 + 272, 1);
  return r3;
}

uint64_t ListPartConstruct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82254488 Type8ListPart::Type8ListPart");
  const uint32_t part = uint32_t(r3), frame = Frame(base, sp, 112);
  const schema::Type8ListPart pt(base, part);
  screen_constructors::ListPartBase(base, part);
  pt.set_vtable(kListPartVtable);
  ListInnerConstruct(base, r3 + 20, frame);
  pt.set_inner_part(part + 20);
  pt.set_movie_id(20);
  return r3;
}

uint64_t ListInnerDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254418 Type8ListPartInner::~Type8ListPartInner");
  const uint32_t frame = Frame(base, sp, 112);
  schema::Type8ListPartInner(base, uint32_t(r3)).set_vtable(kListInnerVtable);
  ts::ReleaseList(base, r3 + 272 + 84, frame, 0x82254448, calls);
  ts::ReleaseList(base, r3 + 272 + 12, frame, 0x82254450, calls);
  ts::ReleaseList(base, r3 + 40 + 84, frame, 0x8225445C, calls);
  ts::ReleaseList(base, r3 + 40 + 12, frame, 0x82254464, calls);
  return ts::ListPartInnerBaseDestruct(base, r3);
}

uint64_t ListInnerDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254530 Type8ListPartInner::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  ListInnerDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    Delete(r3, flags, frame, 0x82254560, calls);
  }
  return r3;
}

uint64_t ListPartDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822544E8 Type8ListPart::~Type8ListPart");
  const uint32_t frame = Frame(base, sp, 96);
  schema::Type8ListPart(base, uint32_t(r3)).set_vtable(kListPartVtable);
  ListInnerDestruct(base, r3 + 20, frame, calls);
  return ts::ListPartBaseDestruct(base, r3);
}

uint64_t ListPartDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254580 Type8ListPart::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  type8_screens::ListPartDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    Delete(r3, flags, frame, 0x822545B0, calls);
  }
  return r3;
}

uint64_t ListInnerOpen(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822549D8 Type8ListPartInner::Open");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  const schema::Type8ListPartInner in(base, inner);
  OpenBegin(base, inner, uint32_t(r4), frame, 0x82254A48, calls);
  PlayerBlockSetUp(base, r3 + 40, r4, frame, calls);
  const uint64_t result = PlayerBlockSetUp(base, r3 + 272, r4, frame, calls);
  in.set_yes(0);
  in.set_back(0);
  return result;
}

uint64_t ListInnerPick(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t sp,
                       uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822550C0 Type8ListPartInner::Pick");
  return InnerPick(base, r3, r4, r5, kListInnerPages, sp, lr, calls);
}

uint64_t ListInnerInput(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254E18 Type8ListPartInner::Input");
  return InnerInput(base, r3, r4, kListInnerSteps, 0x82254E74, 0x82254EBC, sp, calls);
}

uint64_t ListInnerStep0(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254D08 Type8ListPartInner::Step0");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  const schema::Type8ListPartInner in(base, inner);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(r4));
  if (in.step_started() == 0) {
    PlayerBlockPlayOpening(base, r3 + 40, r4, frame);
    PlayerBlockPlayOpening(base, r3 + 272, r4, frame);
    ui_movies::PlayNamed(base, movie, 47, 0, 0, 0, 0);
    in.set_step_started(1);
  }
  const uint64_t done = uint32_t(calls.call(ts::kRun, frame, 0x82254D88, movie, 0)) == 0 ? 1 : 0;
  if (done != 0) {
    PlayerBlockShowSelections(base, r3 + 40, r4, frame, calls);
    PlayerBlockShowSelections(base, r3 + 272, r4, frame, calls);
  }
  return done;
}

uint64_t ListInnerPage0(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82254FB8 Type8ListPartInner::Page0");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 128);
  const schema::Type8ListPartInner in(base, inner);
  // Each block by its state (+4): 0 its input (answers 0), 1 ready (1),
  // else 1 with no call.
  auto block = [&](uint64_t at) -> uint64_t {
    const uint32_t state = schema::Type8PlayerBlock(base, uint32_t(at)).ready();
    if (state == 0) return PlayerBlockInput(base, at, r4, frame, calls);
    if (state == 1) return PlayerBlockWhileReady(base, at, r4, frame, calls);
    return 1;
  };
  const uint64_t first = block(r3 + 40);
  const uint64_t second = block(r3 + 272);
  Page0End(base, inner, first, second, in.block0_backed_out_address(), calls);
  return 0;
}

uint64_t PartInnerStep1(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822538A8 Type8PartInner::Step1");
  const uint32_t inner = uint32_t(r3), frame = Frame(base, sp, 112);
  const schema::Type8PartInner in(base, inner);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(r4));
  if (in.step_started() == 0) {
    ui_movies::PlayNamed(base, movie, 48, 0, 0, 0, 0);
    in.set_step_started(1);
  }
  const uint64_t done = uint32_t(calls.call(ts::kRun, frame, 0x82253918, movie, 0)) == 0 ? 1 : 0;
  if (done != 0) {
    ts::pages::Call1(calls, ts::kFinishAll, movie, frame, 0x8225392C);
  }
  return done;
}

uint64_t PartInnerPage1(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82253108 Type8PartInner::Page1");
  if ((schema::Type67ScreensGlobals::fade_out_count(base) |
       schema::Type67ScreensGlobals::fade_in_count(base)) != 0) {
    return 0;
  }
  const schema::Type8PartInner in(base, uint32_t(r3));
  in.set_yes(1);
  in.set_back(0);
  return 1;
}

uint64_t PartInnerPage2(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82253838 Type8PartInner::Page2");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = MovieAt(base, frame + 80, uint32_t(r4));
  if (ui_movies::IsNamedActive(base, movie, 48) != 0) {
    return 0;
  }
  const schema::Type8PartInner in(base, uint32_t(r3));
  in.set_yes(0);
  in.set_back(1);
  return 1;
}

}  // namespace type8_screens
