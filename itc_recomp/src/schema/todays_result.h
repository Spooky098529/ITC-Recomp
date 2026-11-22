// The views of docs/functions/todays-result.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/todays-result.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: ModeType9, ModeType9::WaitForLoads.r4, ModeType9::WaitForShown.r4, ModeType9::LeaveScreen.r4, ModeType9::NextMode.r4, ModeType9::Leaving.r4
struct ModeType9 : View {
  using View::View;
  static constexpr uint32_t kSize = 852;
  static constexpr uint32_t kVtable = 0x82006CAC;
  // measure fault: ForceExit's r3 is the mode + 32, so this is the mode's +4 (delegate_ptr)
  uint32_t iface_delegate_ptr() const { return guest::LoadU32(base, address + -28); }
  void set_iface_delegate_ptr(uint32_t v) const { guest::StoreU32(base, address + -28, v); }
  uint32_t iface_delegate_ptr_address() const { return address + -28; }
  // 0x82006CAC (14 slots)
  uint32_t vtable() const { return guest::LoadU32(base, address + 0); }
  void set_vtable(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t vtable_address() const { return address + 0; }
  // measure fault: the next-mode answer's byte +0 (ModeAnswer), the delegates' r3
  uint8_t answer_changed() const { return guest::LoadU8(base, address + 0); }
  void set_answer_changed(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t answer_changed_address() const { return address + 0; }
  // measure fault: the next-mode answer's 8 bytes (ModeAnswer)
  uint64_t answer() const { return guest::LoadU64(base, address + 0); }
  void set_answer(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t answer_address() const { return address + 0; }
  // the delegate at +8, 0 none (also the answer's next type +4 by the measure's join)
  template <typename T = ModeDelegate>
  T delegate_ptr() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_delegate_ptr(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_delegate_ptr(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t delegate_ptr_address() const { return address + 4; }
  // 0x82010FA0 (ModeDelegate::Invoke)
  uint32_t delegate_vtable() const { return guest::LoadU32(base, address + 8); }
  void set_delegate_vtable(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t delegate_vtable_address() const { return address + 8; }
  // the mode
  uint32_t delegate_object() const { return guest::LoadU32(base, address + 12); }
  void set_delegate_object(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t delegate_object_address() const { return address + 12; }
  // a Mode::SlotNThunk or Mode::NextModeThunk 0x82235C08
  uint32_t delegate_function() const { return guest::LoadU32(base, address + 16); }
  void set_delegate_function(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t delegate_function_address() const { return address + 16; }
  uint32_t delegate_adjustment() const { return guest::LoadU32(base, address + 20); }
  void set_delegate_adjustment(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t delegate_adjustment_address() const { return address + 20; }
  // 0x82006CA0: IsLoaded, ForceExit, an empty function
  uint32_t vtable32() const { return guest::LoadU32(base, address + 32); }
  void set_vtable32(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t vtable32_address() const { return address + 32; }
  // the dispatcher (+36, ModeDispatcher): 50 handler pointers (the two machines)
  uint32_t handlers_address() const { return address + 36; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 236); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 236, v); }
  uint32_t handler_count_address() const { return address + 236; }
  // the dispatcher's ring: 50 messages of 12 bytes (ModeMessage)
  uint32_t ring_address() const { return address + 240; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 840); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 840, v); }
  uint32_t ring_count_address() const { return address + 840; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 844); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 844, v); }
  uint32_t ring_start_address() const { return address + 844; }
  // measure fault: IsLoaded's and ForceExit's r3 is the mode + 32, so this is the mode's +848 (block)
  template <typename T = ModeType9Block>
  T iface_block() const { return T(base, guest::LoadU32(base, address + 816)); }
  void set_iface_block(const View& v) const { guest::StoreU32(base, address + 816, v.address); }
  void set_iface_block(uint32_t v) const { guest::StoreU32(base, address + 816, v); }
  uint32_t iface_block_address() const { return address + 816; }
  // VirtualAlloc'ed by Init, freed by Release
  template <typename T = ModeType9Block>
  T block() const { return T(base, guest::LoadU32(base, address + 848)); }
  void set_block(const View& v) const { guest::StoreU32(base, address + 848, v.address); }
  void set_block(uint32_t v) const { guest::StoreU32(base, address + 848, v); }
  uint32_t block_address() const { return address + 848; }
  // measure fault: FindShown's r3 is the block (ModeType9Block.event_state)
  uint32_t block_event_state() const { return guest::LoadU32(base, address + 19244); }
  void set_block_event_state(uint32_t v) const { guest::StoreU32(base, address + 19244, v); }
  uint32_t block_event_state_address() const { return address + 19244; }
  // measure fault: FindShown's r3 is the block (ModeType9Block.shown)
  uint32_t block_shown() const { return guest::LoadU32(base, address + 19280); }
  void set_block_shown(uint32_t v) const { guest::StoreU32(base, address + 19280, v); }
  uint32_t block_shown_address() const { return address + 19280; }
};

// allocated; measure: new<19300>@ModeType9::Init, ModeType9.+816, ModeType9::WaitForLoads.r4.+848, ModeType9::NextMode.r4.+848, ModeType9::Leaving.r4.+848
struct ModeType9Block : View {
  using View::View;
  static constexpr uint32_t kSize = 19300;
  // today's result's machine (TodaysResultMachine)
  uint32_t result_address() const { return address + 0; }
  uint32_t result_state() const { return guest::LoadU32(base, address + 9604); }
  void set_result_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t result_state_address() const { return address + 9604; }
  // movie 27 loaded
  uint32_t result_loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_result_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t result_loaded_address() const { return address + 9608; }
  // 1 at Init
  uint32_t result_reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_result_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t result_reenter_address() const { return address + 9612; }
  // 0 at Init
  uint32_t result_delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_result_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t result_delete_movie_address() const { return address + 9616; }
  // today's event's machine (TodaysEventMachine)
  uint32_t event_address() const { return address + 9640; }
  uint32_t event_state() const { return guest::LoadU32(base, address + 19244); }
  void set_event_state(uint32_t v) const { guest::StoreU32(base, address + 19244, v); }
  uint32_t event_state_address() const { return address + 19244; }
  // movie 0 and the images loaded
  uint32_t event_loaded() const { return guest::LoadU32(base, address + 19248); }
  void set_event_loaded(uint32_t v) const { guest::StoreU32(base, address + 19248, v); }
  uint32_t event_loaded_address() const { return address + 19248; }
  // 1 at Init
  uint32_t event_reenter() const { return guest::LoadU32(base, address + 19252); }
  void set_event_reenter(uint32_t v) const { guest::StoreU32(base, address + 19252, v); }
  uint32_t event_reenter_address() const { return address + 19252; }
  // 0 at Init
  uint32_t event_delete_movie() const { return guest::LoadU32(base, address + 19256); }
  void set_event_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 19256, v); }
  uint32_t event_delete_movie_address() const { return address + 19256; }
  // the shown machine's index: 27 today's result, 0 today's event (FindShown)
  uint32_t shown() const { return guest::LoadU32(base, address + 19280); }
  void set_shown(uint32_t v) const { guest::StoreU32(base, address + 19280, v); }
  uint32_t shown_address() const { return address + 19280; }
  // the same at Init (readers not looked for)
  uint32_t shown_at_init() const { return guest::LoadU32(base, address + 19284); }
  void set_shown_at_init(uint32_t v) const { guest::StoreU32(base, address + 19284, v); }
  uint32_t shown_at_init_address() const { return address + 19284; }
  // the next mode's 8-byte answer ForceExit writes and NextModeAnswer hands to Modes::Tick (ModeAnswer)
  uint64_t answer() const { return guest::LoadU64(base, address + 19292); }
  void set_answer(uint64_t v) const { guest::StoreU64(base, address + 19292, v); }
  uint32_t answer_address() const { return address + 19292; }
  // the answer's next mode type
  uint32_t answer_next_type() const { return guest::LoadU32(base, address + 19296); }
  void set_answer_next_type(uint32_t v) const { guest::StoreU32(base, address + 19296, v); }
  uint32_t answer_next_type_address() const { return address + 19296; }
};

// embedded; measure: TodaysResult
struct TodaysResultMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  // measure fault: the transitions' r3 is the page object's +24 (PageObject.step_started)
  uint32_t step_started() const { return guest::LoadU32(base, address + 0); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t step_started_address() const { return address + 0; }
  // measure fault: LeavePage's r3 is the dispatcher (ModeDispatcher.ring_count)
  uint32_t ring_count() const { return guest::LoadU32(base, address + 804); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t ring_count_address() const { return address + 804; }
  // measure fault: LeavePage's r3 is the dispatcher (ModeDispatcher.ring_start)
  uint32_t ring_start() const { return guest::LoadU32(base, address + 808); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t ring_start_address() const { return address + 808; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 or 9 at Create, 4 shown, 7 leaving, 10 unloaded, 13 done, 20 done with a movie queued
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // movie 27 loaded
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForMovie go to state 9 (open on message 6)
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // nonzero: Unload deletes movie 27 and clears loaded
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// embedded; measure: TodaysEvent, TodaysEvent::EndPage.r4
struct TodaysEventMachine : View {
  using View::View;
  static constexpr uint32_t kSize = 9620;
  // 200 transitions, 48 bytes each (StateTransition)
  uint32_t transitions_address() const { return address + 0; }
  // measure fault: the transitions' r3 (PageObject.step_started), and the data's +0 (TodaysEventData.ready)
  uint32_t step_started() const { return guest::LoadU32(base, address + 0); }
  void set_step_started(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t step_started_address() const { return address + 0; }
  // measure fault: TodaysEventData.images_a's first record
  uint32_t data_image_records_a() const { return guest::LoadU32(base, address + 4); }
  void set_data_image_records_a(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t data_image_records_a_address() const { return address + 4; }
  // measure fault: TodaysEventData.image_count_a
  uint32_t data_image_count_a() const { return guest::LoadU32(base, address + 188); }
  void set_data_image_count_a(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t data_image_count_a_address() const { return address + 188; }
  // measure fault: TodaysEventData.images_b's first record
  uint32_t data_image_records_b() const { return guest::LoadU32(base, address + 192); }
  void set_data_image_records_b(uint32_t v) const { guest::StoreU32(base, address + 192, v); }
  uint32_t data_image_records_b_address() const { return address + 192; }
  // measure fault: TodaysEventData.image_count_b
  uint32_t data_image_count_b() const { return guest::LoadU32(base, address + 376); }
  void set_data_image_count_b(uint32_t v) const { guest::StoreU32(base, address + 376, v); }
  uint32_t data_image_count_b_address() const { return address + 376; }
  // measure fault: TodaysEventData.image_bits_0
  uint32_t data_image_bits_0() const { return guest::LoadU32(base, address + 380); }
  void set_data_image_bits_0(uint32_t v) const { guest::StoreU32(base, address + 380, v); }
  uint32_t data_image_bits_0_address() const { return address + 380; }
  // measure fault: TodaysEventData.image_bits_1
  uint32_t data_image_bits_1() const { return guest::LoadU32(base, address + 384); }
  void set_data_image_bits_1(uint32_t v) const { guest::StoreU32(base, address + 384, v); }
  uint32_t data_image_bits_1_address() const { return address + 384; }
  // measure fault: TodaysEventData.unlock_table
  uint32_t data_unlock_table() const { return guest::LoadU32(base, address + 392); }
  void set_data_unlock_table(uint32_t v) const { guest::StoreU32(base, address + 392, v); }
  uint32_t data_unlock_table_address() const { return address + 392; }
  // measure fault: TodaysEventData.unlock_entry
  uint32_t data_unlock_entry() const { return guest::LoadU32(base, address + 396); }
  void set_data_unlock_entry(uint32_t v) const { guest::StoreU32(base, address + 396, v); }
  uint32_t data_unlock_entry_address() const { return address + 396; }
  // measure fault: TodaysEventData.movie_object_36
  uint32_t data_movie_object_36() const { return guest::LoadU32(base, address + 400); }
  void set_data_movie_object_36(uint32_t v) const { guest::StoreU32(base, address + 400, v); }
  uint32_t data_movie_object_36_address() const { return address + 400; }
  // measure fault: TodaysEventData.movie_object_32
  uint32_t data_movie_object_32() const { return guest::LoadU32(base, address + 404); }
  void set_data_movie_object_32(uint32_t v) const { guest::StoreU32(base, address + 404, v); }
  uint32_t data_movie_object_32_address() const { return address + 404; }
  // measure fault: TodaysEventData.movies_queued
  uint32_t data_movies_queued() const { return guest::LoadU32(base, address + 408); }
  void set_data_movies_queued(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t data_movies_queued_address() const { return address + 408; }
  // measure fault: EndPage's r3 is the dispatcher (ModeDispatcher.ring_count)
  uint32_t ring_count() const { return guest::LoadU32(base, address + 804); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t ring_count_address() const { return address + 804; }
  // measure fault: EndPage's r3 is the dispatcher (ModeDispatcher.ring_start)
  uint32_t ring_start() const { return guest::LoadU32(base, address + 808); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t ring_start_address() const { return address + 808; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 or 9 at Create (9 with no unlock), 4 shown, 7 leaving, 9 after Unloaded, 10 at the end page, 20 done with movies queued
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // movie 0 and both image lists loaded
  uint32_t loaded() const { return guest::LoadU32(base, address + 9608); }
  void set_loaded(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t loaded_address() const { return address + 9608; }
  // nonzero: Create and WaitForMovie go to state 9
  uint32_t reenter() const { return guest::LoadU32(base, address + 9612); }
  void set_reenter(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t reenter_address() const { return address + 9612; }
  // nonzero: Unload deletes movie 0 and clears loaded
  uint32_t delete_movie() const { return guest::LoadU32(base, address + 9616); }
  void set_delete_movie(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t delete_movie_address() const { return address + 9616; }
};

// allocated; measure: new<68>@TodaysResult::Create#2
#pragma pack(push, 1)
struct TodaysResultDataData {
  uint32_t ready = 0;  // +0
  uint32_t movie_queued = 0;  // +4
  uint32_t day_wins = 0;  // +8
  uint32_t day_losses = 0;  // +12
  uint32_t day_outcome3 = 0;  // +16
  float day_win_rate = 0;  // +20
  uint32_t day_win_streak = 0;  // +24
  uint32_t day_word_4832 = 0;  // +28
  uint32_t career_wins = 0;  // +32
  uint32_t career_losses = 0;  // +36
  uint32_t career_outcome3 = 0;  // +40
  float career_win_rate = 0;  // +44
  uint32_t best_win_streak = 0;  // +48
  uint32_t money = 0;  // +52
  uint32_t mode = 0;  // +56
  uint32_t weekday = 0;  // +60
  uint32_t day = 0;  // +64
  uint8_t pick[20] = {};  // +68, guest order
};
#pragma pack(pop)
static_assert(sizeof(TodaysResultDataData) == 88);
inline constexpr memory::LayoutField kTodaysResultDataFields[] = {{0, 4}, {4, 4}, {8, 4}, {12, 4}, {16, 4}, {20, 4}, {24, 4}, {28, 4}, {32, 4}, {36, 4}, {40, 4}, {44, 4}, {48, 4}, {52, 4}, {56, 4}, {60, 4}, {64, 4}};
inline constexpr memory::Layout kTodaysResultDataLayout = {kTodaysResultDataFields, 17};

struct TodaysResultData : View {
  using View::View;
  static constexpr uint32_t kSize = 88;
  static constexpr bool kHost = true;
  static constexpr bool kDual = false;
  TodaysResultDataData* data() const { return static_cast<TodaysResultDataData*>(memory::HandleObject(address)); }
  static TodaysResultData New(uint8_t* base) {
    return TodaysResultData(base, memory::MakeHandle(new TodaysResultDataData(), sizeof(TodaysResultDataData), "TodaysResultData", &kTodaysResultDataLayout));
  }
  void Delete() const { delete data(); memory::ReleaseHandle(address); }
  // 1 once the texts are set (SetUpMovie)
  uint32_t ready() const { return data()->ready; }
  void set_ready(uint32_t v) const { data()->ready = v; }
  uint32_t ready_bits() const { return data()->ready; }
  void set_ready_bits(uint32_t v) const { data()->ready = v; }
  // 1 when LeavePage queued a movie; Destroy then goes to state 20
  uint32_t movie_queued() const { return data()->movie_queued; }
  void set_movie_queued(uint32_t v) const { data()->movie_queued = v; }
  uint32_t movie_queued_bits() const { return data()->movie_queued; }
  void set_movie_queued_bits(uint32_t v) const { data()->movie_queued = v; }
  uint32_t day_wins() const { return data()->day_wins; }
  void set_day_wins(uint32_t v) const { data()->day_wins = v; }
  uint32_t day_wins_bits() const { return data()->day_wins; }
  void set_day_wins_bits(uint32_t v) const { data()->day_wins = v; }
  uint32_t day_losses() const { return data()->day_losses; }
  void set_day_losses(uint32_t v) const { data()->day_losses = v; }
  uint32_t day_losses_bits() const { return data()->day_losses; }
  void set_day_losses_bits(uint32_t v) const { data()->day_losses = v; }
  // the day's outcome 3 count
  uint32_t day_outcome3() const { return data()->day_outcome3; }
  void set_day_outcome3(uint32_t v) const { data()->day_outcome3 = v; }
  uint32_t day_outcome3_bits() const { return data()->day_outcome3; }
  void set_day_outcome3_bits(uint32_t v) const { data()->day_outcome3 = v; }
  // the day's win rate in percent
  float day_win_rate() const { return data()->day_win_rate; }
  void set_day_win_rate(float v) const { data()->day_win_rate = v; }
  uint32_t day_win_rate_bits() const { return std::bit_cast<uint32_t>(data()->day_win_rate); }
  void set_day_win_rate_bits(uint32_t v) const { data()->day_win_rate = std::bit_cast<float>(v); }
  uint32_t day_win_streak() const { return data()->day_win_streak; }
  void set_day_win_streak(uint32_t v) const { data()->day_win_streak = v; }
  uint32_t day_win_streak_bits() const { return data()->day_win_streak; }
  void set_day_win_streak_bits(uint32_t v) const { data()->day_win_streak = v; }
  // the day block's +4832 (DayBlock::Word4832)
  uint32_t day_word_4832() const { return data()->day_word_4832; }
  void set_day_word_4832(uint32_t v) const { data()->day_word_4832 = v; }
  uint32_t day_word_4832_bits() const { return data()->day_word_4832; }
  void set_day_word_4832_bits(uint32_t v) const { data()->day_word_4832 = v; }
  uint32_t career_wins() const { return data()->career_wins; }
  void set_career_wins(uint32_t v) const { data()->career_wins = v; }
  uint32_t career_wins_bits() const { return data()->career_wins; }
  void set_career_wins_bits(uint32_t v) const { data()->career_wins = v; }
  uint32_t career_losses() const { return data()->career_losses; }
  void set_career_losses(uint32_t v) const { data()->career_losses = v; }
  uint32_t career_losses_bits() const { return data()->career_losses; }
  void set_career_losses_bits(uint32_t v) const { data()->career_losses = v; }
  uint32_t career_outcome3() const { return data()->career_outcome3; }
  void set_career_outcome3(uint32_t v) const { data()->career_outcome3 = v; }
  uint32_t career_outcome3_bits() const { return data()->career_outcome3; }
  void set_career_outcome3_bits(uint32_t v) const { data()->career_outcome3 = v; }
  // in percent
  float career_win_rate() const { return data()->career_win_rate; }
  void set_career_win_rate(float v) const { data()->career_win_rate = v; }
  uint32_t career_win_rate_bits() const { return std::bit_cast<uint32_t>(data()->career_win_rate); }
  void set_career_win_rate_bits(uint32_t v) const { data()->career_win_rate = std::bit_cast<float>(v); }
  // the stats' +5968
  uint32_t best_win_streak() const { return data()->best_win_streak; }
  void set_best_win_streak(uint32_t v) const { data()->best_win_streak = v; }
  uint32_t best_win_streak_bits() const { return data()->best_win_streak; }
  void set_best_win_streak_bits(uint32_t v) const { data()->best_win_streak = v; }
  // the save's +6116
  uint32_t money() const { return data()->money; }
  void set_money(uint32_t v) const { data()->money = v; }
  uint32_t money_bits() const { return data()->money; }
  void set_money_bits(uint32_t v) const { data()->money = v; }
  // the save's +6104
  uint32_t mode() const { return data()->mode; }
  void set_mode(uint32_t v) const { data()->mode = v; }
  uint32_t mode_bits() const { return data()->mode; }
  void set_mode_bits(uint32_t v) const { data()->mode = v; }
  // the day % 7
  uint32_t weekday() const { return data()->weekday; }
  void set_weekday(uint32_t v) const { data()->weekday = v; }
  uint32_t weekday_bits() const { return data()->weekday; }
  void set_weekday_bits(uint32_t v) const { data()->weekday = v; }
  // the save's +6100, at most 999
  uint32_t day() const { return data()->day; }
  void set_day(uint32_t v) const { data()->day = v; }
  uint32_t day_bits() const { return data()->day; }
  void set_day_bits(uint32_t v) const { data()->day = v; }
  // a copy of the save's 20-byte pick at +8 (five words)
  uint8_t* pick() const { return data()->pick; }
};

// allocated; measure: -
struct TodaysEventData : View {
  using View::View;
  static constexpr uint32_t kSize = 412;
  // 1 once the texts are set (SetUpTexts); 0 at Create
  uint32_t ready() const { return guest::LoadU32(base, address + 0); }
  void set_ready(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t ready_address() const { return address + 0; }
  // the first images' list (TodaysEventImageList: 46 record pointers, the count at +188); assets 22696 + i
  uint32_t images_a_address() const { return address + 4; }
  uint32_t image_count_a() const { return guest::LoadU32(base, address + 188); }
  void set_image_count_a(uint32_t v) const { guest::StoreU32(base, address + 188, v); }
  uint32_t image_count_a_address() const { return address + 188; }
  // the second images' list (TodaysEventImageList); assets 22741 + i
  uint32_t images_b_address() const { return address + 192; }
  uint32_t image_count_b() const { return guest::LoadU32(base, address + 376); }
  void set_image_count_b(uint32_t v) const { guest::StoreU32(base, address + 376, v); }
  uint32_t image_count_b_address() const { return address + 376; }
  // the images to load: bit i of 46 (MarkImages)
  uint32_t image_bits_0() const { return guest::LoadU32(base, address + 380); }
  void set_image_bits_0(uint32_t v) const { guest::StoreU32(base, address + 380, v); }
  uint32_t image_bits_0_address() const { return address + 380; }
  uint32_t image_bits_1() const { return guest::LoadU32(base, address + 384); }
  void set_image_bits_1(uint32_t v) const { guest::StoreU32(base, address + 384, v); }
  uint32_t image_bits_1_address() const { return address + 384; }
  // the unlock entry shown: a copy of the save's 12-byte entry
  uint32_t unlock_shown_address() const { return address + 388; }
  // the entry's +0 (its shown word)
  uint32_t unlock_word() const { return guest::LoadU32(base, address + 388); }
  void set_unlock_word(uint32_t v) const { guest::StoreU32(base, address + 388, v); }
  uint32_t unlock_word_address() const { return address + 388; }
  // the data table
  uint32_t unlock_table() const { return guest::LoadU32(base, address + 392); }
  void set_unlock_table(uint32_t v) const { guest::StoreU32(base, address + 392, v); }
  uint32_t unlock_table_address() const { return address + 392; }
  // its 56-byte entry, -1 none; moved along the entry's pages
  uint32_t unlock_entry() const { return guest::LoadU32(base, address + 396); }
  void set_unlock_entry(uint32_t v) const { guest::StoreU32(base, address + 396, v); }
  uint32_t unlock_entry_address() const { return address + 396; }
  // movie 0's +28 object's +36 as Start found it (put back by Unload and Leave)
  uint32_t movie_object_36() const { return guest::LoadU32(base, address + 400); }
  void set_movie_object_36(uint32_t v) const { guest::StoreU32(base, address + 400, v); }
  uint32_t movie_object_36_address() const { return address + 400; }
  // movie 0's +28 object's +32 as Start found it
  uint32_t movie_object_32() const { return guest::LoadU32(base, address + 404); }
  void set_movie_object_32(uint32_t v) const { guest::StoreU32(base, address + 404, v); }
  uint32_t movie_object_32_address() const { return address + 404; }
  // 1 when EndPage queued movies; Destroy then goes to state 20
  uint32_t movies_queued() const { return guest::LoadU32(base, address + 408); }
  void set_movies_queued(uint32_t v) const { guest::StoreU32(base, address + 408, v); }
  uint32_t movies_queued_address() const { return address + 408; }
};

// embedded; measure: -
struct TodaysEventImageList : View {
  using View::View;
  static constexpr uint32_t kSize = 188;
  // 46 Type2HubImage pointers (type2-screens.toml)
  uint32_t records_address() const { return address + 0; }
  uint32_t count() const { return guest::LoadU32(base, address + 184); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t count_address() const { return address + 184; }
};

// The image words docs/functions/todays-result.md's code uses.
struct TodaysResultGlobals {
  // today's result's page object (28 bytes)
  template <typename T = PageObject>
  static T result_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160C8u)); }
  static void set_result_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160C8u, v.address); }
  static void set_result_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160C8u, v); }
  static constexpr uint32_t result_pages_address() { return 0x828160C8u; }
  // today's result's menu (68 bytes; +16 = 4)
  template <typename T = FrontEndMenu>
  static T result_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160CCu)); }
  static void set_result_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160CCu, v.address); }
  static void set_result_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160CCu, v); }
  static constexpr uint32_t result_menu_address() { return 0x828160CCu; }
  template <typename T = TodaysResultData>
  static T result_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160D0u)); }
  static void set_result_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160D0u, v.address); }
  static void set_result_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160D0u, v); }
  static constexpr uint32_t result_data_address() { return 0x828160D0u; }
  // today's event's page object
  template <typename T = PageObject>
  static T event_pages(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160D4u)); }
  static void set_event_pages(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160D4u, v.address); }
  static void set_event_pages(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160D4u, v); }
  static constexpr uint32_t event_pages_address() { return 0x828160D4u; }
  // today's event's menu
  template <typename T = FrontEndMenu>
  static T event_menu(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160D8u)); }
  static void set_event_menu(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160D8u, v.address); }
  static void set_event_menu(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160D8u, v); }
  static constexpr uint32_t event_menu_address() { return 0x828160D8u; }
  template <typename T = TodaysEventData>
  static T event_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x828160DCu)); }
  static void set_event_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x828160DCu, v.address); }
  static void set_event_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x828160DCu, v); }
  static constexpr uint32_t event_data_address() { return 0x828160DCu; }
  // the string table Strings::Load gets (file 171)
  static uint32_t string_table(uint8_t* base) { return guest::LoadU32(base, 0x826C5C34u); }
  static void set_string_table(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C34u, v); }
  static constexpr uint32_t string_table_address() { return 0x826C5C34u; }
  // the passes word ScreenFades::FadeIn is given
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // the passes word ScreenFades::FadeOut is given
  static uint32_t fade_out_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fade_out_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fade_out_passes_address() { return 0x82001BF0u; }
  // the fades' counts the FadeOut transitions wait on
  static uint32_t fade_count_0(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_fade_count_0(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t fade_count_0_address() { return 0x826A50E4u; }
  static uint32_t fade_count_1(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_fade_count_1(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t fade_count_1_address() { return 0x826A50DCu; }
  // the boot movies' five ids (11 ending the list)
  static constexpr uint32_t boot_movie_ids_address() { return 0x826C5BACu; }
  // the mode type after the boot movies' list
  static uint32_t boot_next_type(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC0u); }
  static void set_boot_next_type(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC0u, v); }
  static constexpr uint32_t boot_next_type_address() { return 0x826C5BC0u; }
  static uint32_t boot_skippable(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC4u); }
  static void set_boot_skippable(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC4u, v); }
  static constexpr uint32_t boot_skippable_address() { return 0x826C5BC4u; }
  static uint32_t boot_movie_index(uint8_t* base) { return guest::LoadU32(base, 0x826C5BC8u); }
  static void set_boot_movie_index(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5BC8u, v); }
  static constexpr uint32_t boot_movie_index_address() { return 0x826C5BC8u; }
  // the sound manager (AudioManager): slot 14 IsCuePlaying, 13 StopCue
  static uint32_t sound_manager(uint8_t* base) { return guest::LoadU32(base, 0x826C5C80u); }
  static void set_sound_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5C80u, v); }
  static constexpr uint32_t sound_manager_address() { return 0x826C5C80u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 100.0
  static float hundred(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_hundred(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t hundred_address() { return 0x82002A0Cu; }
};

}  // namespace schema
