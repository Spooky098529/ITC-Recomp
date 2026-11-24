// The views of docs/functions/race-menu.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-menu.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: RaceMenu
struct RaceMenu : View {
  using View::View;
  static constexpr uint32_t kSize = 10152;
  // 200 transitions, 48 bytes each (StateMachine's)
  uint32_t transitions_address() const { return address + 0; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 11 a race set up, 12 a parking area location, 20 on day 2 after the close
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // movie 1 loaded
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForMovie go to state 9 (ScreenMachine::Reenter)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // nonzero: Unload deletes movies 1 and 16 and clears `loaded`
  uint32_t delete_movies() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movies(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movies_address() const { return address + 9616; }
  // 3 the race mode, 4 the event, 5 the variant, 6 and 8 the question, 7 back, 9 leaving, 10 starting
  uint32_t page() const { return guest::LoadU32(base, address + 9640); }
  void set_page(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t page_address() const { return address + 9640; }
  uint32_t previous_page() const { return guest::LoadU32(base, address + 9644); }
  void set_previous_page(uint32_t v) const { guest::StoreU32(base, address + 9644, v); }
  uint32_t previous_page_address() const { return address + 9644; }
  // the race mode again (the menu's cursor)
  uint32_t menu_mode() const { return guest::LoadU32(base, address + 9648); }
  void set_menu_mode(uint32_t v) const { guest::StoreU32(base, address + 9648, v); }
  uint32_t menu_mode_address() const { return address + 9648; }
  // 1 after Create
  uint32_t unknown_9652() const { return guest::LoadU32(base, address + 9652); }
  void set_unknown_9652(uint32_t v) const { guest::StoreU32(base, address + 9652, v); }
  uint32_t unknown_9652_address() const { return address + 9652; }
  // 15 words (debug only)
  uint32_t debug_list_address() const { return address + 9656; }
  // the event cursor kept
  uint32_t kept_event_cursor() const { return guest::LoadU32(base, address + 9716); }
  void set_kept_event_cursor(uint32_t v) const { guest::StoreU32(base, address + 9716, v); }
  uint32_t kept_event_cursor_address() const { return address + 9716; }
  // debug_list's count
  uint32_t debug_count() const { return guest::LoadU32(base, address + 9720); }
  void set_debug_count(uint32_t v) const { guest::StoreU32(base, address + 9720, v); }
  uint32_t debug_count_address() const { return address + 9720; }
  // the variant picked
  uint32_t variant() const { return guest::LoadU32(base, address + 9724); }
  void set_variant(uint32_t v) const { guest::StoreU32(base, address + 9724, v); }
  uint32_t variant_address() const { return address + 9724; }
  uint32_t unknown_9728() const { return guest::LoadU32(base, address + 9728); }
  void set_unknown_9728(uint32_t v) const { guest::StoreU32(base, address + 9728, v); }
  uint32_t unknown_9728_address() const { return address + 9728; }
  // 11 page words
  uint32_t page_history_address() const { return address + 9732; }
  uint8_t history_depth() const { return guest::LoadU8(base, address + 9776); }
  void set_history_depth(uint8_t v) const { guest::StoreU8(base, address + 9776, v); }
  uint32_t history_depth_address() const { return address + 9776; }
  // 1: Update runs Refresh
  uint32_t refresh_texts() const { return guest::LoadU32(base, address + 9780); }
  void set_refresh_texts(uint32_t v) const { guest::StoreU32(base, address + 9780, v); }
  uint32_t refresh_texts_address() const { return address + 9780; }
  // the pages take input (Draw)
  uint32_t takes_input() const { return guest::LoadU32(base, address + 9784); }
  void set_takes_input(uint32_t v) const { guest::StoreU32(base, address + 9784, v); }
  uint32_t takes_input_address() const { return address + 9784; }
  // the question answered
  uint32_t answered() const { return guest::LoadU32(base, address + 9788); }
  void set_answered(uint32_t v) const { guest::StoreU32(base, address + 9788, v); }
  uint32_t answered_address() const { return address + 9788; }
  // 1 when Race::SetUp ran, 0 a parking area location
  uint32_t race_set_up() const { return guest::LoadU32(base, address + 9792); }
  void set_race_set_up(uint32_t v) const { guest::StoreU32(base, address + 9792, v); }
  uint32_t race_set_up_address() const { return address + 9792; }
  uint32_t back_from_question() const { return guest::LoadU32(base, address + 9796); }
  void set_back_from_question(uint32_t v) const { guest::StoreU32(base, address + 9796, v); }
  uint32_t back_from_question_address() const { return address + 9796; }
  // 1 when the question is asked from page 5
  uint32_t unknown_9800() const { return guest::LoadU32(base, address + 9800); }
  void set_unknown_9800(uint32_t v) const { guest::StoreU32(base, address + 9800, v); }
  uint32_t unknown_9800_address() const { return address + 9800; }
  uint32_t unknown_9804() const { return guest::LoadU32(base, address + 9804); }
  void set_unknown_9804(uint32_t v) const { guest::StoreU32(base, address + 9804, v); }
  uint32_t unknown_9804_address() const { return address + 9804; }
  // the message LeaveToHub posts (6)
  uint32_t leave_message() const { return guest::LoadU32(base, address + 9808); }
  void set_leave_message(uint32_t v) const { guest::StoreU32(base, address + 9808, v); }
  uint32_t leave_message_address() const { return address + 9808; }
  // the transitions' first-time set-up done
  uint32_t transitions_set_up() const { return guest::LoadU32(base, address + 9812); }
  void set_transitions_set_up(uint32_t v) const { guest::StoreU32(base, address + 9812, v); }
  uint32_t transitions_set_up_address() const { return address + 9812; }
  // cleared on every page change
  uint16_t unknown_9816() const { return guest::LoadU16(base, address + 9816); }
  void set_unknown_9816(uint16_t v) const { guest::StoreU16(base, address + 9816, v); }
  uint32_t unknown_9816_address() const { return address + 9816; }
  // 0 to 2
  uint32_t race_mode() const { return guest::LoadU32(base, address + 9820); }
  void set_race_mode(uint32_t v) const { guest::StoreU32(base, address + 9820, v); }
  uint32_t race_mode_address() const { return address + 9820; }
  uint32_t previous_race_mode() const { return guest::LoadU32(base, address + 9824); }
  void set_previous_race_mode(uint32_t v) const { guest::StoreU32(base, address + 9824, v); }
  uint32_t previous_race_mode_address() const { return address + 9824; }
  uint32_t event_cursor() const { return guest::LoadU32(base, address + 9828); }
  void set_event_cursor(uint32_t v) const { guest::StoreU32(base, address + 9828, v); }
  uint32_t event_cursor_address() const { return address + 9828; }
  uint32_t previous_event_cursor() const { return guest::LoadU32(base, address + 9832); }
  void set_previous_event_cursor(uint32_t v) const { guest::StoreU32(base, address + 9832, v); }
  uint32_t previous_event_cursor_address() const { return address + 9832; }
  // the event list's scroll: 1 up, 2 down
  uint32_t scroll_flag() const { return guest::LoadU32(base, address + 9836); }
  void set_scroll_flag(uint32_t v) const { guest::StoreU32(base, address + 9836, v); }
  uint32_t scroll_flag_address() const { return address + 9836; }
  uint32_t first_event() const { return guest::LoadU32(base, address + 9840); }
  void set_first_event(uint32_t v) const { guest::StoreU32(base, address + 9840, v); }
  uint32_t first_event_address() const { return address + 9840; }
  uint32_t last_event() const { return guest::LoadU32(base, address + 9844); }
  void set_last_event(uint32_t v) const { guest::StoreU32(base, address + 9844, v); }
  uint32_t last_event_address() const { return address + 9844; }
  // the events below it have a variant
  uint32_t variant_events() const { return guest::LoadU32(base, address + 9848); }
  void set_variant_events(uint32_t v) const { guest::StoreU32(base, address + 9848, v); }
  uint32_t variant_events_address() const { return address + 9848; }
  // 15 RaceMenuEvent
  uint32_t events_address() const { return address + 9852; }
  // 0 to 2
  uint32_t visible_row() const { return guest::LoadU32(base, address + 9972); }
  void set_visible_row(uint32_t v) const { guest::StoreU32(base, address + 9972, v); }
  uint32_t visible_row_address() const { return address + 9972; }
  uint32_t previous_visible_row() const { return guest::LoadU32(base, address + 9976); }
  void set_previous_visible_row(uint32_t v) const { guest::StoreU32(base, address + 9976, v); }
  uint32_t previous_visible_row_address() const { return address + 9976; }
  // 15 RaceMenuEventKind
  uint32_t event_kinds_address() const { return address + 9980; }
  uint32_t variant_cursor() const { return guest::LoadU32(base, address + 10100); }
  void set_variant_cursor(uint32_t v) const { guest::StoreU32(base, address + 10100, v); }
  uint32_t variant_cursor_address() const { return address + 10100; }
  uint32_t previous_variant_cursor() const { return guest::LoadU32(base, address + 10104); }
  void set_previous_variant_cursor(uint32_t v) const { guest::StoreU32(base, address + 10104, v); }
  uint32_t previous_variant_cursor_address() const { return address + 10104; }
  // 1 after a no
  uint32_t answered_no() const { return guest::LoadU32(base, address + 10108); }
  void set_answered_no(uint32_t v) const { guest::StoreU32(base, address + 10108, v); }
  uint32_t answered_no_address() const { return address + 10108; }
  // player 0's save +8 to +27
  uint32_t player_name_address() const { return address + 10112; }
  uint32_t money() const { return guest::LoadU32(base, address + 10132); }
  void set_money(uint32_t v) const { guest::StoreU32(base, address + 10132, v); }
  uint32_t money_address() const { return address + 10132; }
  // at most 999
  uint32_t day() const { return guest::LoadU32(base, address + 10136); }
  void set_day(uint32_t v) const { guest::StoreU32(base, address + 10136, v); }
  uint32_t day_address() const { return address + 10136; }
  uint32_t weekday() const { return guest::LoadU32(base, address + 10140); }
  void set_weekday(uint32_t v) const { guest::StoreU32(base, address + 10140, v); }
  uint32_t weekday_address() const { return address + 10140; }
  // player 0's save data (SaveData::Of)
  uint32_t save() const { return guest::LoadU32(base, address + 10144); }
  void set_save(uint32_t v) const { guest::StoreU32(base, address + 10144, v); }
  uint32_t save_address() const { return address + 10144; }
  // the save's +0x199E0: 0x20, 0x4 or 0x8, 0x10 open more events
  uint32_t progress_bits() const { return guest::LoadU32(base, address + 10148); }
  void set_progress_bits(uint32_t v) const { guest::StoreU32(base, address + 10148, v); }
  uint32_t progress_bits_address() const { return address + 10148; }
};

// embedded; measure: -
struct RaceMenuEvent : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 0 to 14: StartEvent's location by it
  uint32_t type() const { return guest::LoadU32(base, address + 0); }
  void set_type(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t type_address() const { return address + 0; }
  // 0 to 2
  uint32_t badge() const { return guest::LoadU32(base, address + 4); }
  void set_badge(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t badge_address() const { return address + 4; }
};

// embedded; measure: -
struct RaceMenuEventKind : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // -1 none
  uint32_t variant_kind() const { return guest::LoadU32(base, address + 4); }
  void set_variant_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t variant_kind_address() const { return address + 4; }
};

// embedded; measure: RaceMenu::EventPage.r4, RaceMenu::AskConfirm.r4, RaceMenu::LeaveToHub.r4
#pragma pack(push, 1)
struct RaceMenuModeData {
  uint8_t pad_0[204] = {};
  uint8_t messages[600] = {};  // +204, guest order
  uint32_t message_count = 0;  // +804
  uint32_t message_start = 0;  // +808
};
#pragma pack(pop)
static_assert(sizeof(RaceMenuModeData) == 812);
inline constexpr memory::LayoutField kRaceMenuModeFields[] = {{804, 4}, {808, 4}};
inline constexpr memory::Layout kRaceMenuModeLayout = {kRaceMenuModeFields, 2};

struct RaceMenuMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  static constexpr bool kHost = false;
  static constexpr bool kDual = true;
  RaceMenuModeData* data() const { return static_cast<RaceMenuModeData*>(memory::HandleObject(address)); }
  bool on_host() const { return memory::IsHandle(address); }
  static RaceMenuMode New(uint8_t* base) {
    return RaceMenuMode(base, memory::MakeHandle(new RaceMenuModeData(), sizeof(RaceMenuModeData), "RaceMenuMode", &kRaceMenuModeLayout));
  }
  template <uint32_t Stride = kSize>
  static RaceMenuMode NewArray(uint8_t* base, uint32_t count) {
    static_assert(sizeof(RaceMenuModeData) == Stride, "an array's stride is the guest size");
    auto* p = static_cast<RaceMenuModeData*>(::operator new(sizeof(RaceMenuModeData) * std::max<uint32_t>(count, 1)));
    for (uint32_t i = 0; i < count; i++) new (p + i) RaceMenuModeData();
    return RaceMenuMode(base, memory::MakeHandle(p, sizeof(RaceMenuModeData) * count, "RaceMenuMode", &kRaceMenuModeLayout, sizeof(RaceMenuModeData)));
  }
  RaceMenuMode at(uint32_t i) const { return RaceMenuMode(base, address + i * kSize); }
  void Delete() const { if (address) memory::FreeHandle(address); }
  RaceMenuModeData* direct() const { return static_cast<RaceMenuModeData*>(memory::HandleOwn(address, &kRaceMenuModeLayout)); }
  uint64_t load_at(uint32_t offset, uint32_t width) const { if (auto* d = direct()) return memory::LoadOverlay(reinterpret_cast<const uint8_t*>(d), kRaceMenuModeLayout, offset, width); return memory::IsHandle(address + offset) ? memory::OwnerLoad(address + offset, width) : width == 1 ? guest::LoadU8(base, address + offset) : width == 2 ? guest::LoadU16(base, address + offset) : width == 4 ? guest::LoadU32(base, address + offset) : guest::LoadU64(base, address + offset); }
  void store_at(uint32_t offset, uint32_t width, uint64_t v) const { if (auto* d = direct()) { memory::StoreOverlay(reinterpret_cast<uint8_t*>(d), kRaceMenuModeLayout, offset, width, v); return; } if (memory::IsHandle(address + offset)) memory::OwnerStore(address + offset, width, v); else if (width == 1) guest::StoreU8(base, address + offset, uint8_t(v)); else if (width == 2) guest::StoreU16(base, address + offset, uint16_t(v)); else if (width == 4) guest::StoreU32(base, address + offset, uint32_t(v)); else guest::StoreU64(base, address + offset, v); }
  // 50 messages of 12 bytes (id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint8_t* messages() const { return memory::IsHandle(address) ? static_cast<uint8_t*>(memory::HandleObject(address)) + 204 : guest::Pointer(base, address + 204); }
  uint32_t message_count_address() const { return address + 804; }
  uint32_t message_count_bits() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_message_count_bits(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  uint32_t message_count() const { if (auto* d = direct()) return d->message_count; return uint32_t((memory::IsHandle(address + 804) ? uint32_t(memory::OwnerLoad(address + 804, 4)) : guest::LoadU32(base, address + 804))); }
  void set_message_count(uint32_t v) const { if (auto* d = direct()) { d->message_count = v; return; } if (memory::IsHandle(address + 804)) memory::OwnerStore(address + 804, 4, v); else guest::StoreU32(base, address + 804, v); }
  // the ring's first slot
  uint32_t message_start_address() const { return address + 808; }
  uint32_t message_start_bits() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_message_start_bits(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
  uint32_t message_start() const { if (auto* d = direct()) return d->message_start; return uint32_t((memory::IsHandle(address + 808) ? uint32_t(memory::OwnerLoad(address + 808, 4)) : guest::LoadU32(base, address + 808))); }
  void set_message_start(uint32_t v) const { if (auto* d = direct()) { d->message_start = v; return; } if (memory::IsHandle(address + 808)) memory::OwnerStore(address + 808, 4, v); else guest::StoreU32(base, address + 808, v); }
};

// The image words docs/functions/race-menu.md's code uses.
struct RaceMenuGlobals {
  // VirtualAlloc'ed by Create, freed by Destroy
  template <typename T = PageObject>
  static T pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8404u)); }
  static void set_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8404u, v.address); }
  static void set_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8404u, v); }
  static constexpr uint32_t pages_address() { return 0x826D8404u; }
  // a second page object: Draw runs Back when its page is 7
  template <typename T = PageObject>
  static T pages2(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D8408u)); }
  static void set_pages2(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D8408u, v.address); }
  static void set_pages2(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D8408u, v); }
  static constexpr uint32_t pages2_address() { return 0x826D8408u; }
  // a front-end menu, first and last item 0
  template <typename T = FrontEndMenu>
  static T menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D840Cu)); }
  static void set_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D840Cu, v.address); }
  static void set_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D840Cu, v); }
  static constexpr uint32_t menu_address() { return 0x826D840Cu; }
  // the screen fades' fade-out counts (0 once done)
  static uint32_t fade_out_counts(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_fade_out_counts(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t fade_out_counts_address() { return 0x826A50E4u; }
  // the screen fades' fade-in counts (0 once done)
  static uint32_t fade_in_counts(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_fade_in_counts(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t fade_in_counts_address() { return 0x826A50DCu; }
  // ScreenFades::FadeIn's pass count (Start)
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // ScreenFades::FadeOut's pass count
  static uint32_t fade_out_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fade_out_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fade_out_passes_address() { return 0x82001BF0u; }
  // the sound manager (sound-cues.md): slots 13 StopCue, 14 IsCuePlaying
  static uint32_t sound_manager(uint8_t* base) { return guest::LoadU32(base, 0x826C5C80u); }
  static void set_sound_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C80u, v); }
  static constexpr uint32_t sound_manager_address() { return 0x826C5C80u; }
  // the boot movies' list (boot-mode.md), set by Destroy on day 2
  static constexpr uint32_t boot_movie_list_address() { return 0x826C5BACu; }
  // 0.0 (copied as bits onto the nodes)
  static uint32_t float_zero(uint8_t* base) { return guest::LoadU32(base, 0x82000A34u); }
  static void set_float_zero(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000A34u, v); }
  static constexpr uint32_t float_zero_address() { return 0x82000A34u; }
  // a float copied as bits onto the nodes' +12
  static uint32_t float_2228(uint8_t* base) { return guest::LoadU32(base, 0x82002228u); }
  static void set_float_2228(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82002228u, v); }
  static constexpr uint32_t float_2228_address() { return 0x82002228u; }
  // a float copied as bits onto the nodes' +16
  static uint32_t float_a134(uint8_t* base) { return guest::LoadU32(base, 0x8200A134u); }
  static void set_float_a134(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x8200A134u, v); }
  static constexpr uint32_t float_a134_address() { return 0x8200A134u; }
  // a float copied as bits onto the nodes' +12
  static uint32_t float_c34(uint8_t* base) { return guest::LoadU32(base, 0x82000C34u); }
  static void set_float_c34(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82000C34u, v); }
  static constexpr uint32_t float_c34_address() { return 0x82000C34u; }
  // the animation time Update tests for the scene's animation
  static float scene_time(uint8_t* base) { return guest::LoadFloat(base, 0x82001FB0u); }
  static void set_scene_time(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FB0u, v); }
  static constexpr uint32_t scene_time_address() { return 0x82001FB0u; }
  // the animation time Update tests for the variant's animation
  static float variant_time(uint8_t* base) { return guest::LoadFloat(base, 0x82001FD8u); }
  static void set_variant_time(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FD8u, v); }
  static constexpr uint32_t variant_time_address() { return 0x82001FD8u; }
};

}  // namespace schema
