// The views of docs/functions/type2-screens.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/type2-screens.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: Type2Hub
struct Type2Hub : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateMachine's); the measure also files Type2HubData's words here
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once movies 0, 9, 10 and both image lists are loaded (WaitForLoad)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForLoad go to state 9; the mode's Init sets 1
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // nonzero: Unload deletes movie 0 (UiMovies::Delete) and clears `loaded`
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// allocated; measure: -
struct Type2HubData : View {
  using View::View;
  static constexpr uint32_t kSize = 448;
  // 1 once set up (SetUpStatus); Reenter's transition 4 when set, else 0
  uint32_t set_up() const { return guest::LoadU32(base, address + 0); }
  void set_set_up(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t set_up_address() const { return address + 0; }
  // set by Choose for item 7 (the quit question); the machine to state 14 on the destroy
  uint32_t quitting() const { return guest::LoadU32(base, address + 4); }
  void set_quitting(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t quitting_address() const { return address + 4; }
  // GarageStage::ReloadModel's answer (Update): the stand drawn when set
  uint32_t stand_model() const { return guest::LoadU32(base, address + 8); }
  void set_stand_model(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t stand_model_address() const { return address + 8; }
  // 1 when the selected garage car's spec has byte +771 1 (items 1 and 2 refused; FillMenu)
  uint32_t car_refused() const { return guest::LoadU32(base, address + 12); }
  void set_car_refused(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t car_refused_address() const { return address + 12; }
  // the stand's busy marker shown (ShowBusy, HideBusy)
  uint32_t busy_shown() const { return guest::LoadU32(base, address + 16); }
  void set_busy_shown(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t busy_shown_address() const { return address + 16; }
  // 0 from Create, Start, Unload, Destroy; 1 from message 48
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  // the dialogs' step (OnDialogDone)
  uint32_t dialog_step() const { return guest::LoadU32(base, address + 24); }
  void set_dialog_step(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t dialog_step_address() const { return address + 24; }
  // the dialog's answer: 0 OnYes, 1 OnNo
  uint32_t dialog_answer() const { return guest::LoadU32(base, address + 28); }
  void set_dialog_answer(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t dialog_answer_address() const { return address + 28; }
  // the first list: 46 pointers to Type2HubImage (assets 22696 +)
  uint32_t images_address() const { return address + 32; }
  uint32_t image_count() const { return guest::LoadU32(base, address + 216); }
  void set_image_count(uint32_t v) const { guest::StoreU32(base, address + 216, v); }
  uint32_t image_count_address() const { return address + 216; }
  // the second list: 46 pointers to Type2HubImage (assets 22741 +)
  uint32_t images2_address() const { return address + 220; }
  uint32_t image2_count() const { return guest::LoadU32(base, address + 404); }
  void set_image2_count(uint32_t v) const { guest::StoreU32(base, address + 404, v); }
  uint32_t image2_count_address() const { return address + 404; }
  // the images' bit set (46 bits, two words; MarkNoticeImages)
  uint32_t image_bits() const { return guest::LoadU32(base, address + 408); }
  void set_image_bits(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t image_bits_address() const { return address + 408; }
  uint32_t image_bits2() const { return guest::LoadU32(base, address + 412); }
  void set_image_bits2(uint32_t v) const { guest::StoreU32(base, address + 412, v); }
  uint32_t image_bits2_address() const { return address + 412; }
  // the second list's notice found (NextNotice2: a 12-byte save entry)
  uint32_t notice2_address() const { return address + 416; }
  uint32_t notice2_read() const { return guest::LoadU32(base, address + 416); }
  void set_notice2_read(uint32_t v) const { guest::StoreU32(base, address + 416, v); }
  uint32_t notice2_read_address() const { return address + 416; }
  uint32_t notice2_table() const { return guest::LoadU32(base, address + 420); }
  void set_notice2_table(uint32_t v) const { guest::StoreU32(base, address + 420, v); }
  uint32_t notice2_table_address() const { return address + 420; }
  // the notice2's third word: the record's string (7527 + it, SetRecordText); -1 from Create
  uint32_t record_string() const { return guest::LoadU32(base, address + 424); }
  void set_record_string(uint32_t v) const { guest::StoreU32(base, address + 424, v); }
  uint32_t record_string_address() const { return address + 424; }
  // the first list's notice found (NextNotice: a 12-byte save entry)
  uint32_t notice_address() const { return address + 428; }
  uint32_t notice_read() const { return guest::LoadU32(base, address + 428); }
  void set_notice_read(uint32_t v) const { guest::StoreU32(base, address + 428, v); }
  uint32_t notice_read_address() const { return address + 428; }
  // the data table (DataTables::Entry56)
  uint32_t notice_table() const { return guest::LoadU32(base, address + 432); }
  void set_notice_table(uint32_t v) const { guest::StoreU32(base, address + 432, v); }
  uint32_t notice_table_address() const { return address + 432; }
  // the entry; -1 none (Create)
  uint32_t notice_entry() const { return guest::LoadU32(base, address + 436); }
  void set_notice_entry(uint32_t v) const { guest::StoreU32(base, address + 436, v); }
  uint32_t notice_entry_address() const { return address + 436; }
  // movie 0's object's +36, kept by Start, put back by Unload and Close
  uint32_t movie_image() const { return guest::LoadU32(base, address + 440); }
  void set_movie_image(uint32_t v) const { guest::StoreU32(base, address + 440, v); }
  uint32_t movie_image_address() const { return address + 440; }
  // movie 0's object's +32, kept and put back with it
  uint32_t movie_image2() const { return guest::LoadU32(base, address + 444); }
  void set_movie_image2(uint32_t v) const { guest::StoreU32(base, address + 444, v); }
  uint32_t movie_image2_address() const { return address + 444; }
};

// allocated; measure: new<12>@Type2Hub::ImageLoaded
struct Type2HubTexture : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // XPR::GetTexture(0)
  uint32_t texture() const { return guest::LoadU32(base, address + 0); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t texture_address() const { return address + 0; }
  // level 0's (Texture::GetLevelDesc)
  uint32_t width() const { return guest::LoadU32(base, address + 4); }
  void set_width(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t width_address() const { return address + 4; }
  uint32_t height() const { return guest::LoadU32(base, address + 8); }
  void set_height(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t height_address() const { return address + 8; }
};

// allocated; measure: new<16>@Type2Hub::LoadNoticeImage, new<16>@Type2Hub::LoadNoticeImage#2, Type2Hub::ImageLoaded.r6, Type2Hub.+32, Type2Hub.+220, Type2Hub.+220.+4, new<16>@TodaysEvent::LoadImagePair, new<16>@TodaysEvent::LoadImagePair#2, TodaysEvent.+4, TodaysEvent.+192, TodaysEvent.+192.+4
struct Type2HubImageData {
  uint32_t loaded = 0;  // +0
  uint32_t index = 0;  // +4
  uint32_t texture = 0;  // +8
  uint32_t xpr = 0;  // +12
};

struct Type2HubImage : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  static constexpr bool kHost = true;
  Type2HubImageData* data() const { return static_cast<Type2HubImageData*>(memory::HandleObject(address)); }
  static Type2HubImage New(uint8_t* base) {
    return Type2HubImage(base, memory::MakeHandle(new Type2HubImageData(), kSize, "Type2HubImage"));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  // 1 once ImageLoaded ran
  uint32_t loaded() const { return data()->loaded; }
  void set_loaded(uint32_t v) const { data()->loaded = v; }
  uint32_t loaded_bits() const { return data()->loaded; }
  void set_loaded_bits(uint32_t v) const { data()->loaded = v; }
  // the image (0 to 45)
  uint32_t index() const { return data()->index; }
  void set_index(uint32_t v) const { data()->index = v; }
  uint32_t index_bits() const { return data()->index; }
  void set_index_bits(uint32_t v) const { data()->index = v; }
  template <typename T = Type2HubTexture>
  T texture() const { return T(base, data()->texture); }
  void set_texture(const View& v) const { data()->texture = v.address; }
  void set_texture(uint32_t v) const { data()->texture = v; }
  uint32_t texture_bits() const { return data()->texture; }
  void set_texture_bits(uint32_t v) const { data()->texture = v; }
  // XPR::Create's 24-byte block
  uint32_t xpr() const { return data()->xpr; }
  void set_xpr(uint32_t v) const { data()->xpr = v; }
  uint32_t xpr_bits() const { return data()->xpr; }
  void set_xpr_bits(uint32_t v) const { data()->xpr = v; }
};

// embedded; measure: Type2Hub::OpenSelected.r4, Type2Hub::Close.r4, Type2Hub::OnDialogDone.r4, PlateEditor::Close.r4, BuildScreen::Close.r4, PowerGraphScreen::Close.r4, Screen::LeaveToHub.r4
struct Type2ScreensMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  // 50 messages of 12 bytes (id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint32_t message_count() const { return guest::LoadU32(base, address + 804); }
  void set_message_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t message_count_address() const { return address + 804; }
  // the ring's first slot
  uint32_t message_start() const { return guest::LoadU32(base, address + 808); }
  void set_message_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t message_start_address() const { return address + 808; }
};

// embedded; measure: PlateEditor
struct PlateEditor : View {
  using View::View;
  static constexpr uint32_t kSize = 9616;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // UpdatePage sets the data's next_state here
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once movie 26 is loaded (WaitForLoad)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForLoad go to state 9
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
};

// embedded; measure: -
struct Type2Plate : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  uint8_t unknown_0() const { return guest::LoadU8(base, address + 0); }
  void set_unknown_0(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // 0 to 13 (strings 5411 +)
  uint8_t region() const { return guest::LoadU8(base, address + 1); }
  void set_region(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t region_address() const { return address + 1; }
  // 0 to 9, 10 blank
  uint32_t digits_address() const { return address + 2; }
  // 0 to 37 (strings 5425 +; 10 probably a separator)
  uint32_t chars_address() const { return address + 5; }
};

// allocated; measure: -
struct PlateEditorData : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  // 0 from Create
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // the plate being edited (a Type2Plate)
  uint32_t plate_address() const { return address + 4; }
  // plate +1
  uint8_t region() const { return guest::LoadU8(base, address + 5); }
  void set_region(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t region_address() const { return address + 5; }
  // plate +2 to +4
  uint32_t digits_address() const { return address + 6; }
  // plate +5 to +11
  uint32_t chars_address() const { return address + 9; }
  // the confirmation's selection (plate +16)
  uint32_t confirm_selection() const { return guest::LoadU32(base, address + 20); }
  void set_confirm_selection(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t confirm_selection_address() const { return address + 20; }
  // the plate it starts from (a Type2Plate)
  uint32_t start_plate_address() const { return address + 24; }
  // CarDealer::CheckLimits' class (RandomPlate's r3, not read)
  uint32_t car_class() const { return guest::LoadU32(base, address + 44); }
  void set_car_class(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t car_class_address() const { return address + 44; }
  // LoadPlate makes the start plate at random when set
  uint32_t make_random() const { return guest::LoadU32(base, address + 48); }
  void set_make_random(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t make_random_address() const { return address + 48; }
  // the cursor's column
  uint32_t column() const { return guest::LoadU32(base, address + 52); }
  void set_column(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t column_address() const { return address + 52; }
  // the digit or character edited
  uint32_t edited() const { return guest::LoadU32(base, address + 56); }
  void set_edited(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t edited_address() const { return address + 56; }
  // CharLineKind's answer (ShowCharLine)
  uint32_t char_line_kind() const { return guest::LoadU32(base, address + 60); }
  void set_char_line_kind(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t char_line_kind_address() const { return address + 60; }
  // the region menu's rows (5)
  uint32_t region_rows() const { return guest::LoadU32(base, address + 64); }
  void set_region_rows(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t region_rows_address() const { return address + 64; }
  // the character menu's rows (4)
  uint32_t char_rows() const { return guest::LoadU32(base, address + 68); }
  void set_char_rows(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t char_rows_address() const { return address + 68; }
  // the state UpdatePage puts the machine in; 21 none
  uint32_t next_state() const { return guest::LoadU32(base, address + 72); }
  void set_next_state(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t next_state_address() const { return address + 72; }
};

// embedded; measure: BuildScreen
struct BuildScreen : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once movie 6 is loaded (WaitForLoad)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForLoad go to state 9
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // nonzero: Unload deletes movie 6 and clears `loaded`
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// allocated; measure: BuildList
struct BuildScreenData : View {
  using View::View;
  static constexpr uint32_t kSize = 206444;
  // 1 once set up (SetUpStatus)
  uint32_t set_up() const { return guest::LoadU32(base, address + 0); }
  void set_set_up(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t set_up_address() const { return address + 0; }
  // the player's name (save data +8)
  uint32_t name_address() const { return address + 4; }
  // save data +6116
  uint32_t money() const { return guest::LoadU32(base, address + 24); }
  void set_money(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t money_address() const { return address + 24; }
  // the day mod 7
  uint32_t weekday() const { return guest::LoadU32(base, address + 28); }
  void set_weekday(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t weekday_address() const { return address + 28; }
  // save data +6100, at most 999
  uint32_t day() const { return guest::LoadU32(base, address + 32); }
  void set_day(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t day_address() const { return address + 32; }
  // 56 BuildGroup of 3,252 bytes
  uint32_t groups_address() const { return address + 36; }
  // 400 BuildRecord of 52 bytes (+0x2C784)
  uint32_t builds_address() const { return address + 182148; }
  // pointers to the category's groups shown (+0x318C4)
  uint32_t shown_groups_address() const { return address + 202948; }
  // +0x319A4
  uint32_t shown_group_count() const { return guest::LoadU32(base, address + 203172); }
  void set_shown_group_count(uint32_t v) const { guest::StoreU32(base, address + 203172, v); }
  uint32_t shown_group_count_address() const { return address + 203172; }
  // the chosen group's builds (a BuildSlotList, +0x319A8)
  uint32_t build_list_address() const { return address + 203176; }
  // +0x32634
  uint32_t category() const { return guest::LoadU32(base, address + 206388); }
  void set_category(uint32_t v) const { guest::StoreU32(base, address + 206388, v); }
  uint32_t category_address() const { return address + 206388; }
  // the groups' list's selection kept
  uint32_t group_selection() const { return guest::LoadU32(base, address + 206392); }
  void set_group_selection(uint32_t v) const { guest::StoreU32(base, address + 206392, v); }
  uint32_t group_selection_address() const { return address + 206392; }
  uint32_t group_scroll() const { return guest::LoadU32(base, address + 206396); }
  void set_group_scroll(uint32_t v) const { guest::StoreU32(base, address + 206396, v); }
  uint32_t group_scroll_address() const { return address + 206396; }
  // the builds' list's selection kept
  uint32_t build_selection() const { return guest::LoadU32(base, address + 206400); }
  void set_build_selection(uint32_t v) const { guest::StoreU32(base, address + 206400, v); }
  uint32_t build_selection_address() const { return address + 206400; }
  uint32_t build_scroll() const { return guest::LoadU32(base, address + 206404); }
  void set_build_scroll(uint32_t v) const { guest::StoreU32(base, address + 206404, v); }
  uint32_t build_scroll_address() const { return address + 206404; }
  // the stand's list's (for the details)
  uint32_t stand_selection() const { return guest::LoadU32(base, address + 206408); }
  void set_stand_selection(uint32_t v) const { guest::StoreU32(base, address + 206408, v); }
  uint32_t stand_selection_address() const { return address + 206408; }
  uint32_t stand_scroll() const { return guest::LoadU32(base, address + 206412); }
  void set_stand_scroll(uint32_t v) const { guest::StoreU32(base, address + 206412, v); }
  uint32_t stand_scroll_address() const { return address + 206412; }
  uint32_t unknown_206416() const { return guest::LoadU32(base, address + 206416); }
  void set_unknown_206416(uint32_t v) const { guest::StoreU32(base, address + 206416, v); }
  uint32_t unknown_206416_address() const { return address + 206416; }
  // the state-2 builds as a percentage (probably), shown in digits (+0x32654)
  uint32_t number() const { return guest::LoadU32(base, address + 206420); }
  void set_number(uint32_t v) const { guest::StoreU32(base, address + 206420, v); }
  uint32_t number_address() const { return address + 206420; }
  // Fill: whether a kind-3 group has builds; 0: animation 81 played on opening (+0x32658)
  uint32_t opened() const { return guest::LoadU32(base, address + 206424); }
  void set_opened(uint32_t v) const { guest::StoreU32(base, address + 206424, v); }
  uint32_t opened_address() const { return address + 206424; }
  // a car shown on the stand (1 on level 2; +0x3265C)
  uint32_t car_shown() const { return guest::LoadU32(base, address + 206428); }
  void set_car_shown(uint32_t v) const { guest::StoreU32(base, address + 206428, v); }
  uint32_t car_shown_address() const { return address + 206428; }
  // GarageStage::ReloadModel's answer, then the stand drawn (+0x32660)
  uint32_t stand_model() const { return guest::LoadU32(base, address + 206432); }
  void set_stand_model(uint32_t v) const { guest::StoreU32(base, address + 206432, v); }
  uint32_t stand_model_address() const { return address + 206432; }
  // +0x32664
  uint32_t build_shown() const { return guest::LoadU32(base, address + 206436); }
  void set_build_shown(uint32_t v) const { guest::StoreU32(base, address + 206436, v); }
  uint32_t build_shown_address() const { return address + 206436; }
  // the stand's busy marker shown (+0x32668)
  uint32_t busy_shown() const { return guest::LoadU32(base, address + 206440); }
  void set_busy_shown(uint32_t v) const { guest::StoreU32(base, address + 206440, v); }
  uint32_t busy_shown_address() const { return address + 206440; }
};

// embedded; measure: new<206444>@BuildScreen::Create.+202948
struct BuildGroup : View {
  using View::View;
  static constexpr uint32_t kSize = 3252;
  // the group's index (string ids 5355 + it and 5299 + it; a frame)
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
  // 0 to 4 (its builds' +24)
  uint32_t category() const { return guest::LoadU32(base, address + 4); }
  void set_category(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t category_address() const { return address + 4; }
  // its last build's +28
  uint32_t level() const { return guest::LoadU32(base, address + 8); }
  void set_level(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t level_address() const { return address + 8; }
  // its builds' common +20 (0 to 9, BuildScreen::ValueText), -2 mixed, -1 none
  uint32_t value() const { return guest::LoadU32(base, address + 12); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t value_address() const { return address + 12; }
  // the state-2 builds' share, scaled by [0x82002A0C]
  uint32_t percent() const { return guest::LoadU32(base, address + 16); }
  void set_percent(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t percent_address() const { return address + 16; }
  // its builds (0 when none)
  uint32_t count() const { return guest::LoadU32(base, address + 20); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t count_address() const { return address + 20; }
  // its state-2 builds
  uint32_t owned() const { return guest::LoadU32(base, address + 24); }
  void set_owned(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t owned_address() const { return address + 24; }
  // 1 when every build is state 2 (a row's node)
  uint32_t all_owned() const { return guest::LoadU32(base, address + 28); }
  void set_all_owned(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t all_owned_address() const { return address + 28; }
  // set when any of its builds has +32 not +36 (Fill, BuildScreen::MarkGroups)
  uint32_t mark() const { return guest::LoadU32(base, address + 32); }
  void set_mark(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t mark_address() const { return address + 32; }
  // 2 for groups 48 to 51, 1 for 52 to 55, else 0
  uint32_t kind() const { return guest::LoadU32(base, address + 36); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t kind_address() const { return address + 36; }
  // its builds (a BuildSlotList; the count at the group's +1648)
  uint32_t list_address() const { return address + 40; }
  // the list's count
  uint16_t list_count() const { return guest::LoadU16(base, address + 1648); }
  void set_list_count(uint16_t v) const { guest::StoreU16(base, address + 1648, v); }
  uint32_t list_count_address() const { return address + 1648; }
};

// embedded; measure: -
struct BuildSlotList : View {
  using View::View;
  static constexpr uint32_t kSize = 3212;
  // 400 nodes, two halfwords each: +0 the next node's index (-1 the end), +2 the node's slot
  uint32_t links_address() const { return address + 0; }
  // the free list's head
  uint16_t free() const { return guest::LoadU16(base, address + 1600); }
  void set_free(uint16_t v) const { guest::StoreU16(base, address + 1600, v); }
  uint32_t free_address() const { return address + 1600; }
  // -1 (Fill)
  uint16_t unknown_1602() const { return guest::LoadU16(base, address + 1602); }
  void set_unknown_1602(uint16_t v) const { guest::StoreU16(base, address + 1602, v); }
  uint32_t unknown_1602_address() const { return address + 1602; }
  // -1 empty
  uint16_t head() const { return guest::LoadU16(base, address + 1604); }
  void set_head(uint16_t v) const { guest::StoreU16(base, address + 1604, v); }
  uint32_t head_address() const { return address + 1604; }
  // -1 (Fill)
  uint16_t unknown_1606() const { return guest::LoadU16(base, address + 1606); }
  void set_unknown_1606(uint16_t v) const { guest::StoreU16(base, address + 1606, v); }
  uint32_t unknown_1606_address() const { return address + 1606; }
  uint16_t count() const { return guest::LoadU16(base, address + 1608); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 1608, v); }
  uint32_t count_address() const { return address + 1608; }
  // 400 pointers to builds (BuildRecord)
  uint32_t slots_address() const { return address + 1612; }
};

// embedded; measure: -
struct BuildRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 52;
  // the item's halfword +0
  uint32_t index() const { return guest::LoadU32(base, address + 0); }
  void set_index(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t index_address() const { return address + 0; }
  // the item's byte +34 (48 and 49 remapped by the kind)
  uint32_t group() const { return guest::LoadU32(base, address + 4); }
  void set_group(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t group_address() const { return address + 4; }
  // 0 locked, 1 seen, 2 owned (which texts the details show)
  uint32_t state() const { return guest::LoadU32(base, address + 8); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t state_address() const { return address + 8; }
  // 0 (Fill)
  uint32_t unknown_12() const { return guest::LoadU32(base, address + 12); }
  void set_unknown_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t unknown_12_address() const { return address + 12; }
  // 2 for group 48, 1 for 49, else 0 (node 17 shown, frame +0 - 310)
  uint32_t special() const { return guest::LoadU32(base, address + 16); }
  void set_special(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t special_address() const { return address + 16; }
  // the item's byte +36 (0 to 9)
  uint32_t value() const { return guest::LoadU32(base, address + 20); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t value_address() const { return address + 20; }
  // by the item's halfword +40: 1 0, 2 1, 4 2, 7 3
  uint32_t category() const { return guest::LoadU32(base, address + 24); }
  void set_category(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t category_address() const { return address + 24; }
  // the item's halfword +42; builds above the player's stage + 1 left out
  uint32_t level() const { return guest::LoadU32(base, address + 28); }
  void set_level(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t level_address() const { return address + 28; }
  // 1, 3 or 7 by the state, | 8 when seen; bit 0x2 clear: the stand dark
  uint32_t flags() const { return guest::LoadU32(base, address + 32); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t flags_address() const { return address + 32; }
  // the save's build entry +8 (SaveBuildStates writes it back)
  uint32_t saved_flags() const { return guest::LoadU32(base, address + 36); }
  void set_saved_flags(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t saved_flags_address() const { return address + 36; }
  uint32_t seen() const { return guest::LoadU32(base, address + 40); }
  void set_seen(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t seen_address() const { return address + 40; }
  // 0 (Fill)
  uint32_t unknown_44() const { return guest::LoadU32(base, address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  // the item's +160 (a car build, Car::FromBuild)
  uint32_t car_build() const { return guest::LoadU32(base, address + 48); }
  void set_car_build(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t car_build_address() const { return address + 48; }
};

// embedded; measure: PowerGraphScreen, sub_821B8F60.r3
struct SpecSheet : View {
  using View::View;
  static constexpr uint32_t kSize = 9616;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 1 once movie 9 is loaded (WaitForLoad)
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create goes to state 1
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
};

// allocated; measure: new<1968>@PowerGraphScreen::Create
struct SpecSheetData : View {
  using View::View;
  static constexpr uint32_t kSize = 1968;
  // 1 once loaded (WaitForLoad)
  uint32_t movie_loaded() const { return guest::LoadU32(base, address + 0); }
  void set_movie_loaded(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t movie_loaded_address() const { return address + 0; }
  // the graph drawn when set
  uint32_t graph_drawn() const { return guest::LoadU32(base, address + 4); }
  void set_graph_drawn(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t graph_drawn_address() const { return address + 4; }
  // the state to put the machine in (21 none)
  uint32_t next_state() const { return guest::LoadU32(base, address + 8); }
  void set_next_state(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_state_address() const { return address + 8; }
  // cleared on loading (memset)
  uint32_t sheet_address() const { return address + 16; }
  // record +0: int(the car's +1892 float)
  uint32_t car_value() const { return guest::LoadU32(base, address + 16); }
  void set_car_value(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t car_value_address() const { return address + 16; }
  // record +4: the car data's power
  uint32_t rating() const { return guest::LoadU32(base, address + 20); }
  void set_rating(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t rating_address() const { return address + 20; }
  // record +8: the spec's +76
  uint32_t spec_word_0() const { return guest::LoadU32(base, address + 24); }
  void set_spec_word_0(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t spec_word_0_address() const { return address + 24; }
  // record +12: the spec's +84
  uint32_t spec_word_1() const { return guest::LoadU32(base, address + 28); }
  void set_spec_word_1(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t spec_word_1_address() const { return address + 28; }
  // record +16: the spec's +80
  uint32_t spec_word_2() const { return guest::LoadU32(base, address + 32); }
  void set_spec_word_2(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t spec_word_2_address() const { return address + 32; }
  // record +20: the garage car's +12
  uint32_t car_id() const { return guest::LoadU32(base, address + 36); }
  void set_car_id(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t car_id_address() const { return address + 36; }
  // record +24: the spec's byte +129
  uint32_t spec_byte_0() const { return guest::LoadU32(base, address + 40); }
  void set_spec_byte_0(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t spec_byte_0_address() const { return address + 40; }
  // record +28: the spec's byte +130
  uint32_t spec_byte_1() const { return guest::LoadU32(base, address + 44); }
  void set_spec_byte_1(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t spec_byte_1_address() const { return address + 44; }
  // record +32: the drive from the spec's byte +68
  uint32_t engine_class() const { return guest::LoadU32(base, address + 48); }
  void set_engine_class(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t engine_class_address() const { return address + 48; }
  // record +36: the spec's byte +69 set, 1 or 0
  uint32_t spec_flag() const { return guest::LoadU32(base, address + 52); }
  void set_spec_flag(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t spec_flag_address() const { return address + 52; }
  // record +40: the spec's +8
  uint32_t spec_word_3() const { return guest::LoadU32(base, address + 56); }
  void set_spec_word_3(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t spec_word_3_address() const { return address + 56; }
  // record +44: int(the power curve's +1056)
  uint32_t curve_value_0() const { return guest::LoadU32(base, address + 60); }
  void set_curve_value_0(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t curve_value_0_address() const { return address + 60; }
  // record +48: int(the curve's +1060)
  uint32_t curve_value_1() const { return guest::LoadU32(base, address + 64); }
  void set_curve_value_1(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t curve_value_1_address() const { return address + 64; }
  // record +52
  float peak_torque() const { return guest::LoadFloat(base, address + 68); }
  void set_peak_torque(float v) const { guest::StoreFloat(base, address + 68, v); }
  uint32_t peak_torque_address() const { return address + 68; }
  // record +56: int(the curve's +1052)
  uint32_t peak_index() const { return guest::LoadU32(base, address + 72); }
  void set_peak_index(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t peak_index_address() const { return address + 72; }
  // record +60: the garage car's +1568 (its plate, five words)
  uint32_t parts_address() const { return address + 76; }
  uint32_t parts_word_0() const { return guest::LoadU32(base, address + 76); }
  void set_parts_word_0(uint32_t v) const { guest::StoreU32(base, address + 76, v); }
  uint32_t parts_word_0_address() const { return address + 76; }
  // record +80: which buttons page 0 takes (2 from SetUpSpecSheet)
  uint32_t tab() const { return guest::LoadU32(base, address + 96); }
  void set_tab(uint32_t v) const { guest::StoreU32(base, address + 96, v); }
  uint32_t tab_address() const { return address + 96; }
  // a car data (the vehicle: CarSetup::FromCar)
  uint32_t car_data_address() const { return address + 112; }
  // the car data's +0
  float car_data_0() const { return guest::LoadFloat(base, address + 112); }
  void set_car_data_0(float v) const { guest::StoreFloat(base, address + 112, v); }
  uint32_t car_data_0_address() const { return address + 112; }
  // the car data's +400
  float car_data_400() const { return guest::LoadFloat(base, address + 512); }
  void set_car_data_400(float v) const { guest::StoreFloat(base, address + 512, v); }
  uint32_t car_data_400_address() const { return address + 512; }
  // the car data's +672
  float car_data_672() const { return guest::LoadFloat(base, address + 784); }
  void set_car_data_672(float v) const { guest::StoreFloat(base, address + 784, v); }
  uint32_t car_data_672_address() const { return address + 784; }
};

// embedded; measure: GetLocalTime.r3
struct Type2SystemTime : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // probably, as in a SYSTEMTIME
  uint16_t year() const { return guest::LoadU16(base, address + 0); }
  void set_year(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t year_address() const { return address + 0; }
  uint16_t month() const { return guest::LoadU16(base, address + 2); }
  void set_month(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t month_address() const { return address + 2; }
  uint16_t weekday() const { return guest::LoadU16(base, address + 4); }
  void set_weekday(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t weekday_address() const { return address + 4; }
  uint16_t day() const { return guest::LoadU16(base, address + 6); }
  void set_day(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t day_address() const { return address + 6; }
  uint16_t hour() const { return guest::LoadU16(base, address + 8); }
  void set_hour(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t hour_address() const { return address + 8; }
  uint16_t minute() const { return guest::LoadU16(base, address + 10); }
  void set_minute(uint16_t v) const { guest::StoreU16(base, address + 10, v); }
  uint32_t minute_address() const { return address + 10; }
  uint16_t second() const { return guest::LoadU16(base, address + 12); }
  void set_second(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t second_address() const { return address + 12; }
  // RandomPlate's factor
  uint16_t milliseconds() const { return guest::LoadU16(base, address + 14); }
  void set_milliseconds(uint16_t v) const { guest::StoreU16(base, address + 14, v); }
  uint32_t milliseconds_address() const { return address + 14; }
};

// embedded; measure: new<206444>@BuildScreen::Create
struct BuildScreenDataPart : View {
  using View::View;
  static constexpr uint32_t kSize = 204782;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
  uint32_t unknown_32() const { return guest::LoadU32(base, address + 32); }
  void set_unknown_32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t unknown_32_address() const { return address + 32; }
  uint32_t unknown_36() const { return guest::LoadU32(base, address + 36); }
  void set_unknown_36(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t unknown_36_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  uint32_t unknown_44() const { return guest::LoadU32(base, address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_56() const { return guest::LoadU32(base, address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(base, address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(base, address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(base, address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(base, address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
  uint16_t unknown_76() const { return guest::LoadU16(base, address + 76); }
  void set_unknown_76(uint16_t v) const { guest::StoreU16(base, address + 76, v); }
  uint32_t unknown_76_address() const { return address + 76; }
  uint16_t unknown_78() const { return guest::LoadU16(base, address + 78); }
  void set_unknown_78(uint16_t v) const { guest::StoreU16(base, address + 78, v); }
  uint32_t unknown_78_address() const { return address + 78; }
  uint16_t unknown_1672() const { return guest::LoadU16(base, address + 1672); }
  void set_unknown_1672(uint16_t v) const { guest::StoreU16(base, address + 1672, v); }
  uint32_t unknown_1672_address() const { return address + 1672; }
  uint16_t unknown_1676() const { return guest::LoadU16(base, address + 1676); }
  void set_unknown_1676(uint16_t v) const { guest::StoreU16(base, address + 1676, v); }
  uint32_t unknown_1676_address() const { return address + 1676; }
  uint16_t unknown_1678() const { return guest::LoadU16(base, address + 1678); }
  void set_unknown_1678(uint16_t v) const { guest::StoreU16(base, address + 1678, v); }
  uint32_t unknown_1678_address() const { return address + 1678; }
  uint16_t unknown_1680() const { return guest::LoadU16(base, address + 1680); }
  void set_unknown_1680(uint16_t v) const { guest::StoreU16(base, address + 1680, v); }
  uint32_t unknown_1680_address() const { return address + 1680; }
  uint16_t unknown_1682() const { return guest::LoadU16(base, address + 1682); }
  void set_unknown_1682(uint16_t v) const { guest::StoreU16(base, address + 1682, v); }
  uint32_t unknown_1682_address() const { return address + 1682; }
  uint16_t unknown_1684() const { return guest::LoadU16(base, address + 1684); }
  void set_unknown_1684(uint16_t v) const { guest::StoreU16(base, address + 1684, v); }
  uint32_t unknown_1684_address() const { return address + 1684; }
  uint32_t unknown_182152() const { return guest::LoadU32(base, address + 182152); }
  void set_unknown_182152(uint32_t v) const { guest::StoreU32(base, address + 182152, v); }
  uint32_t unknown_182152_address() const { return address + 182152; }
  uint32_t unknown_182156() const { return guest::LoadU32(base, address + 182156); }
  void set_unknown_182156(uint32_t v) const { guest::StoreU32(base, address + 182156, v); }
  uint32_t unknown_182156_address() const { return address + 182156; }
  uint32_t unknown_202948() const { return guest::LoadU32(base, address + 202948); }
  void set_unknown_202948(uint32_t v) const { guest::StoreU32(base, address + 202948, v); }
  uint32_t unknown_202948_address() const { return address + 202948; }
  uint16_t unknown_204780() const { return guest::LoadU16(base, address + 204780); }
  void set_unknown_204780(uint16_t v) const { guest::StoreU16(base, address + 204780, v); }
  uint32_t unknown_204780_address() const { return address + 204780; }
};

// embedded; measure: new<76>@PlateEditor::Create
struct PlateEditorPart : View {
  using View::View;
  static constexpr uint32_t kSize = 76;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  uint8_t unknown_5() const { return guest::LoadU8(base, address + 5); }
  void set_unknown_5(uint8_t v) const { guest::StoreU8(base, address + 5, v); }
  uint32_t unknown_5_address() const { return address + 5; }
  uint8_t unknown_6() const { return guest::LoadU8(base, address + 6); }
  void set_unknown_6(uint8_t v) const { guest::StoreU8(base, address + 6, v); }
  uint32_t unknown_6_address() const { return address + 6; }
  uint8_t unknown_9() const { return guest::LoadU8(base, address + 9); }
  void set_unknown_9(uint8_t v) const { guest::StoreU8(base, address + 9, v); }
  uint32_t unknown_9_address() const { return address + 9; }
  uint32_t unknown_20() const { return guest::LoadU32(base, address + 20); }
  void set_unknown_20(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t unknown_20_address() const { return address + 20; }
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_44() const { return guest::LoadU32(base, address + 44); }
  void set_unknown_44(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t unknown_44_address() const { return address + 44; }
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
  uint32_t unknown_56() const { return guest::LoadU32(base, address + 56); }
  void set_unknown_56(uint32_t v) const { guest::StoreU32(base, address + 56, v); }
  uint32_t unknown_56_address() const { return address + 56; }
  uint32_t unknown_60() const { return guest::LoadU32(base, address + 60); }
  void set_unknown_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t unknown_60_address() const { return address + 60; }
  uint32_t unknown_64() const { return guest::LoadU32(base, address + 64); }
  void set_unknown_64(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t unknown_64_address() const { return address + 64; }
  uint32_t unknown_68() const { return guest::LoadU32(base, address + 68); }
  void set_unknown_68(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t unknown_68_address() const { return address + 68; }
  uint32_t unknown_72() const { return guest::LoadU32(base, address + 72); }
  void set_unknown_72(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t unknown_72_address() const { return address + 72; }
};

// embedded; measure: new<76>@PlateEditor::Create@+24
struct PlateEditorAt24 : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  uint8_t unknown_1() const { return guest::LoadU8(base, address + 1); }
  void set_unknown_1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
};

// embedded; measure: new<206444>@BuildScreen::Create@+76
struct BuildScreenDataAt76 : View {
  using View::View;
  static constexpr uint32_t kSize = 1610;
  uint16_t unknown_1600() const { return guest::LoadU16(base, address + 1600); }
  void set_unknown_1600(uint16_t v) const { guest::StoreU16(base, address + 1600, v); }
  uint32_t unknown_1600_address() const { return address + 1600; }
  uint16_t unknown_1604() const { return guest::LoadU16(base, address + 1604); }
  void set_unknown_1604(uint16_t v) const { guest::StoreU16(base, address + 1604, v); }
  uint32_t unknown_1604_address() const { return address + 1604; }
  uint16_t unknown_1608() const { return guest::LoadU16(base, address + 1608); }
  void set_unknown_1608(uint16_t v) const { guest::StoreU16(base, address + 1608, v); }
  uint32_t unknown_1608_address() const { return address + 1608; }
};

// embedded; measure: new<76>@PlateEditor::Create@+4
struct PlateEditorAt4 : View {
  using View::View;
  static constexpr uint32_t kSize = 2;
  uint8_t unknown_1() const { return guest::LoadU8(base, address + 1); }
  void set_unknown_1(uint8_t v) const { guest::StoreU8(base, address + 1, v); }
  uint32_t unknown_1_address() const { return address + 1; }
};

// The image words docs/functions/type2-screens.md's code uses.
struct Type2ScreensGlobals {
  // pages 0 the menu, 3 the choice made, 4 the notices, 5 the record, 6 a save prompt; 13 transitions
  template <typename T = PageObject>
  static T hub_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8410u)); }
  static void set_hub_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8410u, v.address); }
  static void set_hub_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8410u, v); }
  static constexpr uint32_t hub_pages_address() { return 0x826D8410u; }
  // items 0 to 7, wrapping
  template <typename T = FrontEndMenu>
  static T hub_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8414u)); }
  static void set_hub_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8414u, v.address); }
  static void set_hub_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8414u, v); }
  static constexpr uint32_t hub_menu_address() { return 0x826D8414u; }
  template <typename T = Type2HubData>
  static T hub_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8418u)); }
  static void set_hub_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8418u, v.address); }
  static void set_hub_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8418u, v); }
  static constexpr uint32_t hub_data_address() { return 0x826D8418u; }
  // Type2Hub::WaitForLoad's counter
  static uint32_t hub_counter(uint8_t* base) { return guest::LoadU32(base, 0x826D841Cu); }
  static void set_hub_counter(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D841Cu, v); }
  static constexpr uint32_t hub_counter_address() { return 0x826D841Cu; }
  // pages 0 the region, 1 the number, 2 the characters, 3 a confirmation, 4 leaving; 20 transitions
  template <typename T = PageObject>
  static T plate_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8430u)); }
  static void set_plate_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8430u, v.address); }
  static void set_plate_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8430u, v); }
  static constexpr uint32_t plate_pages_address() { return 0x826D8430u; }
  // a MenuList (72 bytes, race-screens.toml)
  template <typename T = MenuList>
  static T plate_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8434u)); }
  static void set_plate_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8434u, v.address); }
  static void set_plate_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8434u, v); }
  static constexpr uint32_t plate_list_address() { return 0x826D8434u; }
  template <typename T = PlateEditorData>
  static T plate_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8438u)); }
  static void set_plate_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8438u, v.address); }
  static void set_plate_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8438u, v); }
  static constexpr uint32_t plate_data_address() { return 0x826D8438u; }
  // the build screen's transition machine: pages 0 to 3 the levels, 4 leaving
  template <typename T = PageObject>
  static T build_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D843Cu)); }
  static void set_build_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D843Cu, v.address); }
  static void set_build_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D843Cu, v); }
  static constexpr uint32_t build_pages_address() { return 0x826D843Cu; }
  // the level's MenuList (72 bytes)
  template <typename T = MenuList>
  static T build_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8440u)); }
  static void set_build_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8440u, v.address); }
  static void set_build_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8440u, v); }
  static constexpr uint32_t build_list_address() { return 0x826D8440u; }
  // the groups' MenuList (72 bytes)
  template <typename T = MenuList>
  static T build_group_list(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8444u)); }
  static void set_build_group_list(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8444u, v.address); }
  static void set_build_group_list(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8444u, v); }
  static constexpr uint32_t build_group_list_address() { return 0x826D8444u; }
  template <typename T = BuildScreenData>
  static T build_screen(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8448u)); }
  static void set_build_screen(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8448u, v.address); }
  static void set_build_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8448u, v); }
  static constexpr uint32_t build_screen_address() { return 0x826D8448u; }
  // two pages
  template <typename T = PageObject>
  static T sheet_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8464u)); }
  static void set_sheet_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8464u, v.address); }
  static void set_sheet_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8464u, v); }
  static constexpr uint32_t sheet_pages_address() { return 0x826D8464u; }
  template <typename T = FrontEndMenu>
  static T sheet_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8468u)); }
  static void set_sheet_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8468u, v.address); }
  static void set_sheet_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8468u, v); }
  static constexpr uint32_t sheet_menu_address() { return 0x826D8468u; }
  template <typename T = SpecSheetData>
  static T sheet_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D846Cu)); }
  static void set_sheet_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D846Cu, v.address); }
  static void set_sheet_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D846Cu, v); }
  static constexpr uint32_t sheet_data_address() { return 0x826D846Cu; }
  // the user info screen's data (user-info.md): +8 the player's record FillPlayerRecord fills
  static uint32_t record_screen(uint8_t* base) { return guest::LoadU32(base, 0x826D84CCu); }
  static void set_record_screen(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84CCu, v); }
  static constexpr uint32_t record_screen_address() { return 0x826D84CCu; }
};

}  // namespace schema
