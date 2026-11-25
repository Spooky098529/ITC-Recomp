// The location screen's transitions that move the course view's camera
// (Transition4 and Transition5: the location's camera animation evaluated
// into the screen's view), the transitions back to the location's menu
// (Transition8, Transition24, Transition34) and the screen's start
// (docs/functions/location-screen.md). The text setters through their plain
// C++ (the adapters' check: hooks::LocationScreenTextsNative), the stops and
// runs through `calls`.

#include "location_screen/location_screen.h"

#include "base/log.h"
#include "camera_modes/camera_modes.h"
#include "car_specs/car_specs.h"
#include "course_scenery/course_scenery.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "location_screen/internal.h"
#include "mode_tick/mode_tick.h"
#include "race_event/race_event.h"
#include "race_setup/race_setup.h"
#include "schema/car_draw.h"
#include "schema/career.h"
#include "schema/garage_screen.h"
#include "schema/race_event.h"
#include "schema/race_hud_machine.h"
#include "schema/race_states.h"
#include "schema/scene_models.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace location_screen {

using namespace detail;

namespace {

// The location's camera animation `which` (0x82636518 + 16 x the location +
// 4 x which) evaluated at frame `time` into the frame's +128 (CameraAnims::Evaluate,
// its frame 144 bytes below `frame`; EvalCamera's original through `calls`
// where its plain C++ can't), after the original's set-up: two vectors of
// (0, 0, 0, 1) at +96 and +112 copied to +128 and +144 (their w words
// stored again first), +160 and +164 0.0. Then the screen's +592 = the
// frame's +144 (16 bytes), +608 = +128, +664 and +668 = +160 and +164
// (through a float). The floats go through a float register as lfs and
// stfs leave them.
uint32_t EvaluateView(uint32_t frame, uint32_t which, uint32_t time,
                      const Calls& calls) {
  const uint32_t zero = ThroughFloat(0x82000A34);
  const uint32_t one = ThroughFloat(0x82000A30);
  // The two (0, 0, 0, 1) vectors: the original's +96 and +112, read only
  // by the copy below (values since E5 step 6).
  const uint32_t vectors[8] = {zero, zero, zero, one, zero, zero, zero, one};
  guest::StoreU32(frame + 140, one);
  guest::StoreU32(frame + 156, one);
  for (uint32_t i = 0; i < 32; i += 4) {
    guest::StoreU32(frame + 128 + i, vectors[i / 4]);
  }
  guest::StoreU32(frame + 160, zero);
  guest::StoreU32(frame + 164, zero);
  const uint32_t location = Screen().location_index();
  const uint32_t id =
      guest::LoadU32(((location << 4) & 0xFFFFFFF0u) + G::locations_address() + 4 * which);
  const uint32_t out = frame + 128;
  std::optional<double> f1;
  const uint32_t evaluated = camera_modes::AnimsEvaluate(out, id, time,
      [&](uint32_t object) {
        calls.Call(0x82150350, frame - 144, 0x821509C4, object, object, out, time);
      },
      &f1);
  // The view (the screen's +560) of the screen loaded at each store.
  auto view = [&] { return schema::LocationScreenView(Screen().view_address()); };
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(view().camera_vector_1_address() + i,
                    guest::LoadU32(frame + 144 + i));
  }
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(view().camera_vector_0_address() + i,
                    guest::LoadU32(frame + 128 + i));
  }
  const schema::LocationScreenView v = view();
  // The floats' bits as lfs and stfs leave them.
  guest::StoreU32(v.camera_float_0_address(), ThroughFloat(frame + 160));
  guest::StoreU32(v.camera_float_1_address(), ThroughFloat(frame + 164));
  return evaluated;
}

}  // namespace

uint32_t Transition4(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222D828 LocationScreen::Transition4");
  const uint32_t frame = sp - 208;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  if (guest::LoadU32(step) == 0) {
    ui_movies::ResetPose(movie);
    calls.Call(kFinishAll, frame, 0x8222D870, movie);
    ShowPictures(frame);
    ui_movies::Play(movie, 0xFFFFFFFFu, 1, 0, 0, 0, 0);
    PlayNamed(movie, 103);
    LoopLast(frame, movie);
    ShowListEntry(frame, calls);
    PlayNamed(movie, G::list_menu().cursor() + 52);
    LoopLast(frame, movie);
    SetText(calls, frame, movie, 0, 0, 578);
    Required(ui_movies::ResetTextScroll(movie, 0, 0), "sub_8211C5F8");
    EvaluateView(frame, 0, 1, calls);
    guest::StoreU32(step, 1);
  }
  return uint32_t(Run(calls, frame, 0x8222DA00, movie)) == 0 ? 1 : 0;
}

uint32_t Transition5(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222DA10 LocationScreen::Transition5");
  const uint32_t frame = sp - 224;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  if (guest::LoadU32(step) == 0) {
    ResetMovie(frame, calls);
    HidePictures(frame);
    SetText(calls, frame, movie, 0, 0, 573);
    Required(ui_movies::ResetTextScroll(movie, 0, 0), "sub_8211C5F8");
    ui_movies::Play(movie, 0xFFFFFFFFu, 0, 0, 0, 0, 0);
    EvaluateView(frame, 0, 0, calls);
    guest::StoreU32(step, 1);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222DB84, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    ListNode(frame, 0);
    // The list's menu reset: its count (the location record's, +812) less 1
    // at +16, +20 = 1, the rest 0 (the original's store order).
    const uint32_t count = Screen().list_count() - 1;
    const schema::FrontEndMenu menu = G::list_menu();
    menu.set_first_item(0);
    menu.set_wraps(1);
    menu.set_cursor(0);
    menu.set_previous(0);
    menu.set_moved(0);
    menu.set_wrapped(0);
    menu.set_visible_row(0);
    menu.set_last_item(count);
    menu.set_previous_row(0);
    menu.set_side(0);
    menu.set_player(0);
    menu.set_up_down_locked(0);
    menu.set_left_right_steps(0);
    menu.set_shoulders_step(0);
    FinishNamed(calls, frame, 0x8222DBF4, movie, 103);
    SetCarColor(frame);
  }
  return done;
}

uint32_t Transition8(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222DC08 LocationScreen::Transition8");
  // The original's 112-byte frame held no slots (E5 step 6): the calls are
  // made from `sp`, their frames below it.
  const uint32_t movie = Movie(kMenusMovie);
  if (guest::LoadU32(step) == 0) {
    ui_movies::ResetNode(movie, 0xFFFFFFFFu, 11);
    ShowListEntry(sp, calls);
    ui_movies::Play(movie, 0xFFFFFFFFu, 10, 0, 0, 0, 0);
    PlayNamed(movie, G::list_menu().cursor() + 52);
    LoopLast(sp, movie);
    ShowPictures(sp);
    guest::StoreU32(step, 1);
  }
  return uint32_t(Run(calls, sp, 0x8222DCC4, movie)) == 0 ? 1 : 0;
}

uint32_t Transition24(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222D2E8 LocationScreen::Transition24");
  // The original's 112-byte frame held no slots (E5 step 6): the calls are
  // made from `sp`, their frames below it.
  const uint32_t movie = Movie(kMenusMovie);
  if (guest::LoadU32(step) == 0) {
    mode_tick::ScreenFadesFadeOut(60, 0, 0, 0, 0);
    PlayNamed(movie, 98);
    guest::StoreU32(step, 1);
  }
  const uint32_t done = Fading() ? 0 : 1;
  Run(calls, sp, 0x8222D38C, movie);
  if (done != 0) {
    ResetMovie(sp, calls);
    SetCarColor(sp);
  }
  return done;
}

uint32_t Transition34(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222D108 LocationScreen::Transition34");
  // The original's 112-byte frame held no slots (E5 step 6): the calls are
  // made from `sp`, their frames below it.
  const uint32_t movie = Movie(kMenusMovie);
  if (guest::LoadU32(step) == 0) {
    PlayNamed(movie, 98);
    guest::StoreU32(step, 1);
  }
  const uint32_t done = uint32_t(Run(calls, sp, 0x8222D178, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    ResetMovie(sp, calls);
  }
  return done;
}

uint32_t Start(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222F0B0 LocationScreen::Start");
  // The original's 128-byte frame held no slots (E5 step 6): the calls are
  // made from `sp`, their frames below it.
  // The garage's selected car: its list walked from the halfword at +6324
  // (the entries 4 bytes each from +6120, each a next and a slot) by the
  // count at +0x19920; the slot's car (1,968 bytes each from +6120) +224
  // the spec's id.
  uint32_t save = vehicle_accessors::SaveDataOf(0);
  const schema::CareerGarageList list(schema::SaveData(save).garage_address());
  const uint32_t garage = list.address;
  uint32_t count = schema::SaveData(save).selected_car();
  uint32_t entry = (uint32_t(int32_t(int16_t(list.head()))) << 2) + garage;
  for (; count != 0; count--) {
    entry = (uint32_t(int32_t(int16_t(guest::LoadU16(entry)))) << 2) + garage;
  }
  const uint32_t slot = uint32_t(int32_t(int16_t(guest::LoadU16(entry + 2))));
  const schema::CarSpec spec(car_specs::GetCarSpec(schema::CareerAddedCar(slot * 1968 + list.cars_address()).car_id()));
  Screen().set_special_car(spec.list_kind() == 1 ? 1 : 0);
  ResetMovie(sp, calls);
  save = vehicle_accessors::SaveDataOf(0);
  const schema::SaveData unlocks(save);
  const schema::RaceData race = schema::RaceStatesGlobals::race_data();
  const uint32_t record =
      vehicle_accessors::RacerRecordsAt(race.results_address(), 0xFFFFFFFFu);
  const schema::PageObject pages = G::pages();
  uint32_t page, transition;
  if (vehicle_accessors::StagesWord808(race.stages_address()) != 0 && record != 0 &&
      Screen().item2_taken() == 0) {
    // After a race: page 12 through transition 23.
    data_tables::Item(schema::RacerResultsRecord(record).e1_item());
    Screen().set_entry_offset(0);
    Screen().set_entry_table(0);
    page = 12;
    transition = 23;
  } else if (unlocks.unlocks_shown_count() != 0) {
    // An unlock to show: page 13 through transition 25.
    page = 13;
    transition = 25;
  } else {
    // Page 0, set directly.
    const uint32_t current = pages.page();
    if (current != 0) {
      pages.set_previous_page(current);
    }
    pages.set_step(0);
    pages.set_next_page(0);
    pages.set_queued_step(0xFFFFFFFFu);
    pages.set_page(0xFFFFFFFFu);
    mode_tick::ScreenFadesFadeIn(30, 0, 0, 0, 0);
    return 30;
  }
  const uint32_t current = pages.page();
  if (current != page) {
    pages.set_previous_page(current);
  }
  pages.set_page(0xFFFFFFFFu);
  pages.set_next_page(page);
  pages.set_queued_step(0xFFFFFFFFu);
  pages.set_step(transition);
  mode_tick::ScreenFadesFadeIn(30, 0, 0, 0, 0);
  return 30;
}

uint32_t ShowOpponentSpecs(uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222C188 LocationScreen::ShowOpponentSpecs");
  const uint32_t frame = sp - kShowOpponentSpecsFrame;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  // The picked opponent: the original's +80, read only here (a value since
  // E5 step 6).
  const uint32_t picked = Screen().picked_opponent();
  const uint32_t event = race_setup::EventCurrent();
  const uint32_t item = race_event::GroupItem(event, Screen().group_kind(), picked);
  Copy(calls, frame, 0x8222C1E4, frame + 96, item, 3008);
  // The item's car build's car id (+160 + 2788).
  const uint32_t id = guest::LoadU32(frame + 3044);
  const uint32_t spec = car_specs::GetCarSpec(id);
  const uint32_t n9 = ui_movies::FindNode(movie, 12, 9);
  const uint32_t n10 = ui_movies::FindNode(movie, 12, 10);
  const uint32_t n11 = ui_movies::FindNode(movie, 12, 11);
  SetNodeText(calls, frame, movie, n9, schema::CarSpec(spec).maker() + 1934);
  SetNodeText(calls, frame, movie, n10, id + 1646);
  return SetNodeText(calls, frame, movie, n11, id + 1718);
}

uint32_t Transition9(uint32_t sp, uint32_t step, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222DEE0 LocationScreen::Transition9");
  const uint32_t frame = sp - 240;
  guest::StoreU32(frame, sp);
  const uint32_t movie = Movie(kMenusMovie);
  if (guest::LoadU32(step) == 0) {
    ResetMovie(frame, calls);
    if (Screen().more_opponent_text() != 0) {
      HidePictures(frame);
    } else {
      const schema::FrontEndMenu menu = G::opponent_menu();
      const uint32_t cursor = menu.cursor();
      menu.set_cursor(0);
      menu.set_previous(cursor);
    }
    for (uint32_t i = 0; i < 10; i++) {
      FinishNamed(calls, frame, 0x8222DF60, movie, 13 + i);
    }
    for (uint32_t i = 0; i < Screen().opponent_count(); i++) {
      FinishNamed(calls, frame, 0x8222DF8C, movie, 84 + i);
    }
    HideNode(movie, 0xFFFFFFFFu, 18);
    HideNode(movie, 0xFFFFFFFFu, 17);
    ui_movies::Play(movie, 0xFFFFFFFFu, 2, 0, 0, 0, 0);
    PlayNamed(movie, Screen().picked_opponent() + 84);
    LoopLast(frame, movie);
    ShowOpponentInfo(frame, calls);
    // The location's second camera animation at the picked opponent's time
    // (0x82010780 + 4 x +800).
    const uint32_t picked = Screen().picked_opponent();
    EvaluateView(frame, 1,
                 guest::LoadU32(((picked << 2) & 0xFFFFFFFCu) + G::opponent_times_address()),
                 calls);
    ShowOpponentSpecs(frame, calls);
    ShowOpponentPictures(frame, calls);
    guest::StoreU32(step, 1);
  }
  const uint32_t done = uint32_t(Run(calls, frame, 0x8222E118, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    ShowArrows(frame);
    ui_movies::Play(movie, 0xFFFFFFFFu, 13, 0, 0, 0, 0);
    LoopLast(frame, movie);
    // The opponents' menu reset: 6 items (+16 = 6), the rest 0 (the
    // original's store order).
    const schema::FrontEndMenu menu = G::opponent_menu();
    menu.set_first_item(0);
    menu.set_last_item(6);
    menu.set_wraps(0);
    menu.set_cursor(0);
    menu.set_previous(0);
    menu.set_moved(0);
    menu.set_wrapped(0);
    menu.set_visible_row(0);
    menu.set_previous_row(0);
    menu.set_side(0);
    menu.set_player(0);
    menu.set_up_down_locked(0);
    menu.set_left_right_steps(0);
    menu.set_shoulders_step(0);
    SetCarColor(frame);
  }
  return done;
}

namespace {

// The course scenery's calls (course_scenery::Calls) through `calls`.
course_scenery::Calls SceneryCalls(const Calls& calls) {
  return {const_cast<Calls*>(&calls),
          [](void* user, uint32_t function, const uint32_t* args, int count, uint32_t frame,
             uint32_t lr) {
            const Calls& c = *static_cast<const Calls*>(user);
            uint64_t registers[4] = {};
            for (int i = 0; i < count && i < 4; i++) {
              registers[i] = args[i];
            }
            return uint32_t(c.call(c.user, function, registers, frame, lr));
          }};
}

// The location's camera animation `which`'s id (0x82636518 + 16 x the
// location + 4 x which).
uint32_t AnimId(uint32_t which) {
  const uint32_t location = Screen().location_index();
  return guest::LoadU32(((location << 4) & 0xFFFFFFF0u) + G::locations_address() + 4 * which);
}

constexpr uint32_t kCarSceneUpdate = 0x820C9608;
constexpr uint32_t kCarSceneDraw = 0x820C5DE8;
constexpr uint32_t kSceneModelPollLoad = 0x820DA318;
constexpr uint32_t kSceneryIsLoaded = 0x820F1F68;

}  // namespace

uint64_t PollLoad(uint32_t sp, uint32_t machine, const Calls& calls,
                  bool native_scenery, bool native_queue) {
  ITC_NATIVE_RUNNING("sub_82229238 LocationScreen::PollLoad");
  const uint32_t frame = sp - 336;
  guest::StoreU32(frame, sp);
  if (camera_modes::AnimsFind(AnimId(0)) == 0) {
    return 0;
  }
  const uint32_t evaluated = EvaluateView(frame, 0, 0, calls);
  const uint32_t scene = schema::CarDrawGlobals::scene().address;
  calls.Call(kCarSceneUpdate, frame, 0x82229350, evaluated);
  uint64_t r3 = calls.Call(kCarSceneDraw, frame, 0x8222935C, scene, 0);
  for (uint32_t i = 0; i < Screen().opponent_count(); i++) {
    const uint32_t index = guest::LoadU32(Screen().opponent_models_address() + 4 * i);
    const uint32_t model = vehicle_accessors::ScenePoolsGetB(scene, 7, index);
    if (camera_modes::AnimsFind(AnimId(2)) == 0) {
      return 0;
    }
    // The identity the original builds in vector registers and stores
    // (stvx: each row's address rounded down to 16), then copies to the
    // model's matrix (+4304) from the unrounded address.
    for (uint32_t row = 0; row < 4; row++) {
      const uint32_t at = (frame + 176 + 16 * row) & ~15u;
      for (uint32_t column = 0; column < 4; column++) {
        guest::StoreU32(at + 4 * column, row == column ? 0x3F800000u : 0);
      }
    }
    const uint32_t matrix = schema::SceneModel(model).matrix_address();
    Copy(calls, frame, 0x82229404, matrix, frame + 176, 64);
    // The third animation's object at the opponent's time (0x82010780 + 4i).
    const uint32_t time = guest::LoadU32(G::opponent_times_address() + 4 * i);
    if ((frame & 0xF) != 0) {
      calls.Call(0x82150A10, frame, 0x82229424, matrix, AnimId(2), time);
    } else {
      const uint32_t inner = frame - 96 - 320;
      camera_modes::AnimsObjectMatrix(matrix, AnimId(2), time, inner,
                                      [&](uint32_t to, uint32_t from, uint32_t size) {
                                        calls.Call(kMemcpy, inner, 0x82150600, to, from, size);
                                      });
    }
    const uint32_t block = schema::SceneModel(model).draw_block_address();
    if (!PoseCar(frame, block, i, calls)) {
      calls.Call(0x82228588, frame, 0x82229430, block, i);
    }
    r3 = calls.Call(kSceneModelPollLoad, frame, 0x82229438, model);
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  // Every picture of list A loaded (its +0).
  const schema::LocationScreen screen = Screen();
  for (uint32_t i = 0; i < screen.picture_count_a(); i++) {
    const schema::LocationScreenPicture picture(guest::LoadU32(screen.pictures_a_address() + 4 * i));
    if (picture.loaded() == 0) {
      return r3;
    }
  }
  uint32_t movies = 0;
  r3 = ui_movies::IsLoaded(21);
  if (uint32_t(r3) != 0) {
    r3 = ui_movies::IsLoaded(30);
    if (uint32_t(r3) != 0) {
      movies = 1;
    }
  }
  const uint32_t manager = Screen().scenery<schema::View>().address;
  if (native_scenery) {
    r3 = course_scenery::SceneryIsLoaded(SceneryCalls(calls), frame, manager, native_queue);
  } else {
    r3 = calls.Call(kSceneryIsLoaded, frame, 0x822294C0, manager);
  }
  if (uint32_t(r3) == 0 || movies == 0) {
    return r3;
  }
  if (Screen().load_faded() == 0) {
    mode_tick::ScreenFadesFadeOut(30, 0, 0, 0, 0);
    Screen().set_load_faded(1);
  }
  uint32_t movie = Movie(kMenusMovie);
  uint32_t picture = ui_movies::FindNode(movie, 6, 1);
  Screen().set_default_texture_a(
      guest::LoadU32(ui_movies::PictureTextureSlot(movie, picture)));
  movie = Movie(kMenusMovie);
  picture = ui_movies::FindNode(movie, 6, 0);
  const uint32_t texture = guest::LoadU32(ui_movies::PictureTextureSlot(movie, picture));
  Screen().set_default_texture_b(texture);
  const schema::LocationScreenMachine m(machine);
  m.set_loaded(1);
  m.set_state(4);
  return texture;
}

}  // namespace location_screen
