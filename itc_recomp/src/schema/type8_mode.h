// The views of docs/functions/type8-mode.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type8-mode.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Type8Screen0
struct Type8Screen0 : View {
  using View::View;
  static constexpr uint32_t kSize = 9792;
  static constexpr uint32_t kVtable = 0x82010B74;
  // 0x82010B74 (one slot, DeletingDestructor)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // a StateMachine (game-modes.toml): its state at the screen's +9608, its loaded (movie 24 loaded) at +9612, its reenter (open on its message) at +9616
  uint32_t machine_address() const { return address + 4; }
  // delete movie 24 when suspended (0 from ModeType8::Init: kept)
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9620); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9620, v); }
  uint32_t delete_movie_address() const { return address + 9620; }
  // the menu takes input
  uint32_t menu_input() const { return guest::LoadU32(base, address + 9644); }
  void set_menu_input(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t menu_input_address() const { return address + 9644; }
  // -1 none, 0 no, 1 yes, 2 back
  uint32_t dialog_answer() const { return guest::LoadU32(base, address + 9648); }
  void set_dialog_answer(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t dialog_answer_address() const { return address + 9648; }
  // a PageObject (menu-helpers.toml): pages 0 the menu, 1 the quit confirm, 2 none, 3 chosen; steps 0 the opening, 1 the item, 2 the cursor, 3 the fade out
  uint32_t pages_address() const { return address + 9652; }
  // a MenuList (race-screens.toml), 5 items; its +60 (+9740) and +68 (+9748) cleared by the constructor, its +24 (+9704) the last move
  uint32_t menu_list_address() const { return address + 9680; }
  // player 0's Type8StandBlock (animations 45, 44, 46)
  uint32_t stand_0_address() const { return address + 9752; }
  // player 1's Type8StandBlock (animations 48, 47, 49)
  uint32_t stand_1_address() const { return address + 9772; }
};

// embedded; measure: -
struct Type8StandBlock : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // the loading animations are playing
  uint32_t loading_shown() const { return guest::LoadU32(base, address + 0); }
  void set_loading_shown(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t loading_shown_address() const { return address + 0; }
  // a named animation of movie 24 (45, 48): played once when the loading starts
  uint32_t loading_start_anim() const { return guest::LoadU32(base, address + 4); }
  void set_loading_start_anim(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t loading_start_anim_address() const { return address + 4; }
  // a named animation (44, 47): played and looped while loading
  uint32_t loading_loop_anim() const { return guest::LoadU32(base, address + 8); }
  void set_loading_loop_anim(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t loading_loop_anim_address() const { return address + 8; }
  // a named animation (46, 49): played once the model is in
  uint32_t loaded_anim() const { return guest::LoadU32(base, address + 12); }
  void set_loaded_anim(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t loaded_anim_address() const { return address + 12; }
  // GarageStage::ReloadModel's answer once asked (0 until then; cleared by Show)
  uint32_t model_made() const { return guest::LoadU32(base, address + 16); }
  void set_model_made(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t model_made_address() const { return address + 16; }
};

// embedded; measure: -
struct Type8Dialog : View {
  using View::View;
  static constexpr uint32_t kSize = 9752;
  // a StateMachine (game-modes.toml): its state at +9604, its loaded (movies 16 to 20 loaded) at +9608, its reenter (open on its message) at +9612
  uint32_t machine_address() const { return address + 0; }
  // 0 from ModeType8::Init (as each machine's word after its reenter)
  uint32_t delete_movies() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movies(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movies_address() const { return address + 9616; }
  // a PageObject (menu-helpers.toml): pages 0 the yes/no, 1 the notice; steps 0 OpenConfirmStep, 1 CloseConfirmStep, 2 OpenNoticeStep, 3 CloseNoticeStep, 4 MoveCursorStep
  uint32_t pages_address() const { return address + 9640; }
  // a FrontEndMenu (menu-helpers.toml), items 0 to 1; its player (+9732) the asking player ([0x82814E6C])
  uint32_t menu_address() const { return address + 9668; }
  // 1 once its movies are loaded and drawn once
  uint32_t shown() const { return guest::LoadU32(base, address + 9736); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 9736, v); }
  uint32_t shown_address() const { return address + 9736; }
  // 21 none; 9 posts the closed message 13
  uint32_t queued_state() const { return guest::LoadU32(base, address + 9740); }
  void set_queued_state(uint32_t v) const { guest::StoreU32(base, address + 9740, v); }
  uint32_t queued_state_address() const { return address + 9740; }
  // the movie shown: 16 (the yes/no) or 19 (the notice)
  uint32_t movie() const { return guest::LoadU32(base, address + 9744); }
  void set_movie(uint32_t v) const { guest::StoreU32(base, address + 9744, v); }
  uint32_t movie_address() const { return address + 9744; }
  // a reopen queued (Type7Dialog::QueueReopen)
  uint32_t reopen() const { return guest::LoadU32(base, address + 9748); }
  void set_reopen(uint32_t v) const { guest::StoreU32(base, address + 9748, v); }
  uint32_t reopen_address() const { return address + 9748; }
};

// image; measure: -
struct Type8StandEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the garage stage's index into [0x826D83E0] (1, 2)
  uint32_t stage_index() const { return guest::LoadU32(base, address + 0); }
  void set_stage_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t stage_index_address() const { return address + 0; }
  // ORed into the stand car's +3888 (1, 0x80)
  uint32_t car_flag() const { return guest::LoadU32(base, address + 4); }
  void set_car_flag(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t car_flag_address() const { return address + 4; }
  // the stage's command once its model is in (56, 57)
  uint32_t command() const { return guest::LoadU32(base, address + 8); }
  void set_command(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t command_address() const { return address + 8; }
};

// The image words docs/functions/type8-mode.md's code uses.
struct Type8ModeGlobals {
  // the player the quit dialog was asked for (Type8PanelScreen::AskQuit, Type8Screen0::Confirm; read by Type8Dialog::OpenConfirm)
  static uint32_t dialog_player(uint8_t* base) { return guest::LoadU32(base, 0x82814E6Cu); }
  static void set_dialog_player(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E6Cu, v); }
  static constexpr uint32_t dialog_player_address() { return 0x82814E6Cu; }
  // Type8Screen0::Update's steps: four member pointers {function, adjustment}, built on its first call
  static constexpr uint32_t step_table_address() { return 0x82814E70u; }
  // bit 0: step_table built
  static uint32_t step_table_guard(uint8_t* base) { return guest::LoadU32(base, 0x82814E90u); }
  static void set_step_table_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814E90u, v); }
  static constexpr uint32_t step_table_guard_address() { return 0x82814E90u; }
  // Type8Screen0::Draw's pages: four member pointers {function, adjustment}, built on its first call
  static constexpr uint32_t page_table_address() { return 0x82814E94u; }
  // bit 0: page_table built
  static uint32_t page_table_guard(uint8_t* base) { return guest::LoadU32(base, 0x82814EB4u); }
  static void set_page_table_guard(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82814EB4u, v); }
  static constexpr uint32_t page_table_guard_address() { return 0x82814EB4u; }
  // two Type8StandEntry, one a player
  static constexpr uint32_t stand_table_address() { return 0x82636FDCu; }
  // the two players' garage cars (car-makers.toml's GarageCarSlot, 1,972 bytes each)
  static constexpr uint32_t garage_cars_address() { return 0x82814EB8u; }
  // the stand scene's holder (StandScene::Create): +4 and +8 the two players' garage stages' holders
  static View stand_holder(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826D83E0u)); }
  static void set_stand_holder(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D83E0u, v); }
  static constexpr uint32_t stand_holder_address() { return 0x826D83E0u; }
  // the game mode before (1 the race)
  static uint32_t mode_before(uint8_t* base) { return guest::LoadU32(base, 0x826032E0u); }
  static void set_mode_before(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826032E0u, v); }
  static constexpr uint32_t mode_before_address() { return 0x826032E0u; }
  // the passes word ScreenFades::FadeIn is given
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // the passes word ScreenFades::FadeOut is given
  static uint32_t fade_out_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fade_out_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fade_out_passes_address() { return 0x82001BF0u; }
};

}  // namespace schema
