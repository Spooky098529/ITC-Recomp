// The views of docs/functions/career.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/career.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Career, Progress, Garage, Items, ItemRecords, memcpy_825A7410.ret
struct CareerSave : View {
  using View::View;
  static constexpr uint32_t kSize = 129400;
  // 1 for a new game (SaveData::Create)
  uint32_t new_game() const { return guest::LoadU32(base, address + 0); }
  void set_new_game(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t new_game_address() const { return address + 0; }
  uint32_t unknown_4() const { return guest::LoadU32(base, address + 4); }
  void set_unknown_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t unknown_4_address() const { return address + 4; }
  // the next pick (a CareerPick: Career::PickNext, Career::PickFromCar)
  uint32_t pick_address() const { return address + 8; }
  // the pick's copy (a CareerPick), kept by Career::PickNext when unchanged
  uint32_t pick_kept_address() const { return address + 28; }
  // the stats record (a CareerStats)
  uint32_t stats_address() const { return address + 48; }
  // measure fault: sub_820FF5A0's object's +60 (race-hud-object.md's best splits), not the save data's
  uint32_t splits_fault_60() const { return guest::LoadU32(base, address + 60); }
  void set_splits_fault_60(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t splits_fault_60_address() const { return address + 60; }
  // measure fault: sub_820FF5A0's object's +64
  uint32_t splits_fault_64() const { return guest::LoadU32(base, address + 64); }
  void set_splits_fault_64(uint32_t v) const { guest::StoreU32(base, address + 64, v); }
  uint32_t splits_fault_64_address() const { return address + 64; }
  // measure fault: sub_820FF5A0's object's +68
  uint32_t splits_fault_68() const { return guest::LoadU32(base, address + 68); }
  void set_splits_fault_68(uint32_t v) const { guest::StoreU32(base, address + 68, v); }
  uint32_t splits_fault_68_address() const { return address + 68; }
  // measure fault: sub_820FF5A0's object's +72
  uint32_t splits_fault_72() const { return guest::LoadU32(base, address + 72); }
  void set_splits_fault_72(uint32_t v) const { guest::StoreU32(base, address + 72, v); }
  uint32_t splits_fault_72_address() const { return address + 72; }
  // an item of kind 5 (byte +35) beaten (Stats::AddRaceResult; Items::AllWonKind5Or4 reads it)
  uint32_t kind5_won() const { return guest::LoadU32(base, address + 6048); }
  void set_kind5_won(uint32_t v) const { guest::StoreU32(base, address + 6048, v); }
  uint32_t kind5_won_address() const { return address + 6048; }
  // the frame counter [0x826C5B90] PlayTime::Add measures from (PlayTime::Mark)
  uint32_t play_time_mark() const { return guest::LoadU32(base, address + 6088); }
  void set_play_time_mark(uint32_t v) const { guest::StoreU32(base, address + 6088, v); }
  uint32_t play_time_mark_address() const { return address + 6088; }
  // the stages reached, a bit each (SaveData::FirstUnsetFlag)
  uint32_t stages() const { return guest::LoadU32(base, address + 6092); }
  void set_stages(uint32_t v) const { guest::StoreU32(base, address + 6092, v); }
  uint32_t stages_address() const { return address + 6092; }
  uint32_t unknown_6096() const { return guest::LoadU32(base, address + 6096); }
  void set_unknown_6096(uint32_t v) const { guest::StoreU32(base, address + 6096, v); }
  uint32_t unknown_6096_address() const { return address + 6096; }
  // the day (7 a week; 1 a fixed rival race)
  uint32_t day() const { return guest::LoadU32(base, address + 6100); }
  void set_day(uint32_t v) const { guest::StoreU32(base, address + 6100, v); }
  uint32_t day_address() const { return address + 6100; }
  // the race mode
  uint32_t mode() const { return guest::LoadU32(base, address + 6104); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 6104, v); }
  uint32_t mode_address() const { return address + 6104; }
  // 14 to 19, 48
  uint32_t location() const { return guest::LoadU32(base, address + 6108); }
  void set_location(uint32_t v) const { guest::StoreU32(base, address + 6108, v); }
  uint32_t location_address() const { return address + 6108; }
  // 48 at the set-up; else put into +6108 after the race
  uint32_t next_location() const { return guest::LoadU32(base, address + 6112); }
  void set_next_location(uint32_t v) const { guest::StoreU32(base, address + 6112, v); }
  uint32_t next_location_address() const { return address + 6112; }
  // 3000 for a new game, at most 9,999,999
  uint32_t money() const { return guest::LoadU32(base, address + 6116); }
  void set_money(uint32_t v) const { guest::StoreU32(base, address + 6116, v); }
  uint32_t money_address() const { return address + 6116; }
  // the garage's list (a CareerGarageList): 4-byte nodes, the cars of 1,968 bytes from +6332
  uint32_t garage_address() const { return address + 6120; }
  // the garage list's free list (+200)
  uint16_t garage_free() const { return guest::LoadU16(base, address + 6320); }
  void set_garage_free(uint16_t v) const { guest::StoreU16(base, address + 6320, v); }
  uint32_t garage_free_address() const { return address + 6320; }
  uint16_t unknown_6322() const { return guest::LoadU16(base, address + 6322); }
  void set_unknown_6322(uint16_t v) const { guest::StoreU16(base, address + 6322, v); }
  uint32_t unknown_6322_address() const { return address + 6322; }
  // the garage list's head (+204)
  uint16_t garage_head() const { return guest::LoadU16(base, address + 6324); }
  void set_garage_head(uint16_t v) const { guest::StoreU16(base, address + 6324, v); }
  uint32_t garage_head_address() const { return address + 6324; }
  uint16_t unknown_6326() const { return guest::LoadU16(base, address + 6326); }
  void set_unknown_6326(uint16_t v) const { guest::StoreU16(base, address + 6326, v); }
  uint32_t unknown_6326_address() const { return address + 6326; }
  // the garage list's count (+208)
  uint16_t garage_count() const { return guest::LoadU16(base, address + 6328); }
  void set_garage_count(uint16_t v) const { guest::StoreU16(base, address + 6328, v); }
  uint32_t garage_count_address() const { return address + 6328; }
  // 0x19920: the selected garage car, a position in the list
  uint32_t selected_car() const { return guest::LoadU32(base, address + 104736); }
  void set_selected_car(uint32_t v) const { guest::StoreU32(base, address + 104736, v); }
  uint32_t selected_car_address() const { return address + 104736; }
  // 0x19924: a bit set (Rewards::Grant's kind 38)
  uint32_t reward_cars_address() const { return address + 104740; }
  // 0x19930: the unlocks the rewards set (a copy of 0x826C50A8)
  uint32_t unlock_set_address() const { return address + 104752; }
  // 0x1996C: a copy of 0x826C3044 (less 0x826C3E34's bits)
  uint32_t unlock_set_2_address() const { return address + 104812; }
  // 0x199A8: a copy of 0x826C3C24 (Rewards::Grant's kind 40)
  uint32_t reward_set_40_address() const { return address + 104872; }
  // 0x199C4: a copy of 0x826C50E4 (Rewards::Grant's kind 41)
  uint32_t reward_set_41_address() const { return address + 104900; }
  // 0x199E0: the locations unlocked (Locations::Unlock; the race data's +20)
  uint32_t locations_address() const { return address + 104928; }
  // 0x199E8: Locations::Unlock2's bits
  uint32_t locations_2_address() const { return address + 104936; }
  // 0x199F8: 400 records of 12 bytes (CareerItemRecord) by item id
  uint32_t item_records_address() const { return address + 104952; }
  // 0x1ACE0: cleared by Career::NextDay
  uint32_t unknown_109792() const { return guest::LoadU32(base, address + 109792); }
  void set_unknown_109792(uint32_t v) const { guest::StoreU32(base, address + 109792, v); }
  uint32_t unknown_109792_address() const { return address + 109792; }
  // 0x1ACE4: a 400-bit set (ItemFlags::Set), all set but bits 310 to 375
  uint32_t item_flags_address() const { return address + 109796; }
  uint32_t unknown_109848() const { return guest::LoadU32(base, address + 109848); }
  void set_unknown_109848(uint32_t v) const { guest::StoreU32(base, address + 109848, v); }
  uint32_t unknown_109848_address() const { return address + 109848; }
  // 0x1AD20: 6 locations of 100 bytes (12-byte entries {0, table, entry}, the count at +96)
  uint32_t offers_address() const { return address + 109856; }
  // 0x1AF78: a word for each entry of tables 0 to 7, 768 bytes a table (table 1 at 0x1B278, tables 2 to 7 from 0x1B578; 2 done); table 0's probably
  uint32_t table_words_address() const { return address + 110456; }
  // 0x1C778: 64 12-byte entries {0, table, entry}, the unlocks to show
  uint32_t unlocks_shown_address() const { return address + 116600; }
  // 0x1CA78
  uint32_t unlocks_shown_count() const { return guest::LoadU32(base, address + 117368); }
  void set_unlocks_shown_count(uint32_t v) const { guest::StoreU32(base, address + 117368, v); }
  uint32_t unlocks_shown_count_address() const { return address + 117368; }
  // 0x1CA7C: a bit set, table 0's entries unlocked
  uint32_t table0_done_address() const { return address + 117372; }
  // 0x1CA84: 12-byte entries {0, 8, index}, the rewards to show
  uint32_t rewards_shown_address() const { return address + 117380; }
  // 0x1D504
  uint32_t rewards_shown_count() const { return guest::LoadU32(base, address + 120068); }
  void set_rewards_shown_count(uint32_t v) const { guest::StoreU32(base, address + 120068, v); }
  uint32_t rewards_shown_count_address() const { return address + 120068; }
  // 0x1D508: a bit set, the 40-byte entries granted
  uint32_t rewards_granted_address() const { return address + 120072; }
  // 0x1D524: a movie to play after the day's results, -1 none
  uint32_t movie_after_results() const { return guest::LoadU32(base, address + 120100); }
  void set_movie_after_results(uint32_t v) const { guest::StoreU32(base, address + 120100, v); }
  uint32_t movie_after_results_address() const { return address + 120100; }
  uint32_t unknown_120104() const { return guest::LoadU32(base, address + 120104); }
  void set_unknown_120104(uint32_t v) const { guest::StoreU32(base, address + 120104, v); }
  uint32_t unknown_120104_address() const { return address + 120104; }
};

// embedded; measure: sub_82110CA8.r4, sub_82111140.r4, sub_82112260.r4, sub_82115550.r4, sub_82115708.r4
struct CareerPick : View {
  using View::View;
  static constexpr uint32_t kSize = 20;
  // 0 to 15, from the stats' races (Career::PickNext)
  uint8_t level() const { return guest::LoadU8(base, address + 0); }
  void set_level(uint8_t v) const { guest::StoreU8(base, address + 0, v); }
  uint32_t level_address() const { return address + 0; }
  // the level's word, stored whole by sub_82112260
  uint32_t head() const { return guest::LoadU32(base, address + 0); }
  void set_head(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t head_address() const { return address + 0; }
  // 4 from a garage car; 5 with +8 17 to 70 makes it special (Pick::IsSpecial)
  uint32_t first_kind() const { return guest::LoadU32(base, address + 4); }
  void set_first_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t first_kind_address() const { return address + 4; }
  // a garage car's byte +1569 (Career::PickFromCar)
  uint16_t first_value() const { return guest::LoadU16(base, address + 8); }
  void set_first_value(uint16_t v) const { guest::StoreU16(base, address + 8, v); }
  uint32_t first_value_address() const { return address + 8; }
  uint8_t first_set() const { return guest::LoadU8(base, address + 10); }
  void set_first_set(uint8_t v) const { guest::StoreU8(base, address + 10, v); }
  uint32_t first_set_address() const { return address + 10; }
  // 3 from a garage car
  uint32_t second_kind() const { return guest::LoadU32(base, address + 12); }
  void set_second_kind(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t second_kind_address() const { return address + 12; }
  // a garage car's id % 4 (Career::PickFromCar)
  uint16_t second_value() const { return guest::LoadU16(base, address + 16); }
  void set_second_value(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t second_value_address() const { return address + 16; }
  uint8_t second_set() const { return guest::LoadU8(base, address + 18); }
  void set_second_set(uint8_t v) const { guest::StoreU8(base, address + 18, v); }
  uint32_t second_set_address() const { return address + 18; }
};

// embedded; measure: Stats
struct CareerStats : View {
  using View::View;
  static constexpr uint32_t kSize = 6040;
  // a CareerCounter
  uint32_t counter_address() const { return address + 0; }
  // the day block (a CareerCounter first), cleared by Stats::StartDay
  uint32_t day_address() const { return address + 24; }
  // the week's block (a CareerCounter first), cleared on day 0 of 7
  uint32_t week_address() const { return address + 4864; }
  // another CareerCounter
  uint32_t other_counter_address() const { return address + 4924; }
  // the last 15 results (a CareerResults)
  uint32_t results_address() const { return address + 4948; }
  // the last 10 days (a CareerDayLog)
  uint32_t day_log_address() const { return address + 5672; }
  // 14 bytes a mode: halfwords +2 wins, +4 losses, +6 outcome 3, +8, +10 races, +12
  uint32_t by_mode_address() const { return address + 5796; }
  // a running mean {float, count}: the result's +812 (Stats::AddRaceResult)
  float mean_812() const { return guest::LoadFloat(base, address + 5880); }
  void set_mean_812(float v) const { guest::StoreFloat(base, address + 5880, v); }
  uint32_t mean_812_address() const { return address + 5880; }
  uint32_t mean_812_count() const { return guest::LoadU32(base, address + 5884); }
  void set_mean_812_count(uint32_t v) const { guest::StoreU32(base, address + 5884, v); }
  uint32_t mean_812_count_address() const { return address + 5884; }
  // the result's +816
  float mean_816() const { return guest::LoadFloat(base, address + 5888); }
  void set_mean_816(float v) const { guest::StoreFloat(base, address + 5888, v); }
  uint32_t mean_816_address() const { return address + 5888; }
  uint32_t mean_816_count() const { return guest::LoadU32(base, address + 5892); }
  void set_mean_816_count(uint32_t v) const { guest::StoreU32(base, address + 5892, v); }
  uint32_t mean_816_count_address() const { return address + 5892; }
  // the result's +788 x [0x820082A0] (the stage's type 0)
  float mean_788() const { return guest::LoadFloat(base, address + 5896); }
  void set_mean_788(float v) const { guest::StoreFloat(base, address + 5896, v); }
  uint32_t mean_788_address() const { return address + 5896; }
  uint32_t mean_788_count() const { return guest::LoadU32(base, address + 5900); }
  void set_mean_788_count(uint32_t v) const { guest::StoreU32(base, address + 5900, v); }
  uint32_t mean_788_count_address() const { return address + 5900; }
  // the result's mean at +796 (RunningMean::MergeTwo)
  float mean_796() const { return guest::LoadFloat(base, address + 5904); }
  void set_mean_796(float v) const { guest::StoreFloat(base, address + 5904, v); }
  uint32_t mean_796_address() const { return address + 5904; }
  uint32_t mean_796_count() const { return guest::LoadU32(base, address + 5908); }
  void set_mean_796_count(uint32_t v) const { guest::StoreU32(base, address + 5908, v); }
  uint32_t mean_796_count_address() const { return address + 5908; }
  // the result's float +820
  float mean_820() const { return guest::LoadFloat(base, address + 5912); }
  void set_mean_820(float v) const { guest::StoreFloat(base, address + 5912, v); }
  uint32_t mean_820_address() const { return address + 5912; }
  uint32_t mean_820_count() const { return guest::LoadU32(base, address + 5916); }
  void set_mean_820_count(uint32_t v) const { guest::StoreU32(base, address + 5916, v); }
  uint32_t mean_820_count_address() const { return address + 5916; }
  // 100.0 a race the result's +784 is set, else 0.0
  float mean_won() const { return guest::LoadFloat(base, address + 5920); }
  void set_mean_won(float v) const { guest::StoreFloat(base, address + 5920, v); }
  uint32_t mean_won_address() const { return address + 5920; }
  uint32_t mean_won_count() const { return guest::LoadU32(base, address + 5924); }
  void set_mean_won_count(uint32_t v) const { guest::StoreU32(base, address + 5924, v); }
  uint32_t mean_won_count_address() const { return address + 5924; }
  // each day's +5952 x 100 / +5956 (Stats::CloseDay)
  float mean_day_ratio() const { return guest::LoadFloat(base, address + 5928); }
  void set_mean_day_ratio(float v) const { guest::StoreFloat(base, address + 5928, v); }
  uint32_t mean_day_ratio_address() const { return address + 5928; }
  uint32_t mean_day_ratio_count() const { return guest::LoadU32(base, address + 5932); }
  void set_mean_day_ratio_count(uint32_t v) const { guest::StoreU32(base, address + 5932, v); }
  uint32_t mean_day_ratio_count_address() const { return address + 5932; }
  // each day's +5960
  float mean_day_total_8() const { return guest::LoadFloat(base, address + 5936); }
  void set_mean_day_total_8(float v) const { guest::StoreFloat(base, address + 5936, v); }
  uint32_t mean_day_total_8_address() const { return address + 5936; }
  uint32_t mean_day_total_8_count() const { return guest::LoadU32(base, address + 5940); }
  void set_mean_day_total_8_count(uint32_t v) const { guest::StoreU32(base, address + 5940, v); }
  uint32_t mean_day_total_8_count_address() const { return address + 5940; }
  // each day's +5964
  float mean_day_total_12() const { return guest::LoadFloat(base, address + 5944); }
  void set_mean_day_total_12(float v) const { guest::StoreFloat(base, address + 5944, v); }
  uint32_t mean_day_total_12_address() const { return address + 5944; }
  uint32_t mean_day_total_12_count() const { return guest::LoadU32(base, address + 5948); }
  void set_mean_day_total_12_count(uint32_t v) const { guest::StoreU32(base, address + 5948, v); }
  uint32_t mean_day_total_12_count_address() const { return address + 5948; }
  // the day's sums of the race data's +0xA3AD4 (Progress::AfterRace)
  uint32_t day_total_4() const { return guest::LoadU32(base, address + 5952); }
  void set_day_total_4(uint32_t v) const { guest::StoreU32(base, address + 5952, v); }
  uint32_t day_total_4_address() const { return address + 5952; }
  // of +0xA3AD0 (1 at the day's start)
  uint32_t day_total_0() const { return guest::LoadU32(base, address + 5956); }
  void set_day_total_0(uint32_t v) const { guest::StoreU32(base, address + 5956, v); }
  uint32_t day_total_0_address() const { return address + 5956; }
  // of +0xA3AD8
  uint32_t day_total_8() const { return guest::LoadU32(base, address + 5960); }
  void set_day_total_8(uint32_t v) const { guest::StoreU32(base, address + 5960, v); }
  uint32_t day_total_8_address() const { return address + 5960; }
  // of +0xA3ADC / 50
  uint32_t day_total_12() const { return guest::LoadU32(base, address + 5964); }
  void set_day_total_12(uint32_t v) const { guest::StoreU32(base, address + 5964, v); }
  uint32_t day_total_12_address() const { return address + 5964; }
  uint16_t best_win_streak() const { return guest::LoadU16(base, address + 5968); }
  void set_best_win_streak(uint16_t v) const { guest::StoreU16(base, address + 5968, v); }
  uint32_t best_win_streak_address() const { return address + 5968; }
  // up to 9,999
  uint16_t win_streak() const { return guest::LoadU16(base, address + 5970); }
  void set_win_streak(uint16_t v) const { guest::StoreU16(base, address + 5970, v); }
  uint32_t win_streak_address() const { return address + 5970; }
  uint16_t loss_streak() const { return guest::LoadU16(base, address + 5972); }
  void set_loss_streak(uint16_t v) const { guest::StoreU16(base, address + 5972, v); }
  uint32_t loss_streak_address() const { return address + 5972; }
  // the largest result's +804
  float best_804() const { return guest::LoadFloat(base, address + 5976); }
  void set_best_804(float v) const { guest::StoreFloat(base, address + 5976, v); }
  uint32_t best_804_address() const { return address + 5976; }
  // the largest result's +828 x [0x820029D8] (the stage's type 0)
  float best_828() const { return guest::LoadFloat(base, address + 5980); }
  void set_best_828(float v) const { guest::StoreFloat(base, address + 5980, v); }
  uint32_t best_828_address() const { return address + 5980; }
  // as +5980
  float best_828_2() const { return guest::LoadFloat(base, address + 5984); }
  void set_best_828_2(float v) const { guest::StoreFloat(base, address + 5984, v); }
  uint32_t best_828_2_address() const { return address + 5984; }
  // the results' +820 added
  float sum_820() const { return guest::LoadFloat(base, address + 5988); }
  void set_sum_820(float v) const { guest::StoreFloat(base, address + 5988, v); }
  uint32_t sum_820_address() const { return address + 5988; }
  float unknown_5992() const { return guest::LoadFloat(base, address + 5992); }
  void set_unknown_5992(float v) const { guest::StoreFloat(base, address + 5992, v); }
  uint32_t unknown_5992_address() const { return address + 5992; }
  // the largest result's +16 (unsigned)
  uint32_t best_16() const { return guest::LoadU32(base, address + 5996); }
  void set_best_16(uint32_t v) const { guest::StoreU32(base, address + 5996, v); }
  uint32_t best_16_address() const { return address + 5996; }
  // the save data's +6048 (CareerSave.kind5_won)
  uint32_t kind5_won() const { return guest::LoadU32(base, address + 6000); }
  void set_kind5_won(uint32_t v) const { guest::StoreU32(base, address + 6000, v); }
  uint32_t kind5_won_address() const { return address + 6000; }
  // probably (Garage::Sell), up to 9,999
  uint16_t cars_sold() const { return guest::LoadU16(base, address + 6006); }
  void set_cars_sold(uint16_t v) const { guest::StoreU16(base, address + 6006, v); }
  uint32_t cars_sold_address() const { return address + 6006; }
  uint32_t unknown_6008() const { return guest::LoadU32(base, address + 6008); }
  void set_unknown_6008(uint32_t v) const { guest::StoreU32(base, address + 6008, v); }
  uint32_t unknown_6008_address() const { return address + 6008; }
  // probably the money earned, at most 9,999,999 (Stats::AddPoints)
  uint32_t points() const { return guest::LoadU32(base, address + 6024); }
  void set_points(uint32_t v) const { guest::StoreU32(base, address + 6024, v); }
  uint32_t points_address() const { return address + 6024; }
  // up to 9,999 (Stats::CloseDay)
  uint16_t days() const { return guest::LoadU16(base, address + 6028); }
  void set_days(uint16_t v) const { guest::StoreU16(base, address + 6028, v); }
  uint32_t days_address() const { return address + 6028; }
  uint32_t unknown_6032() const { return guest::LoadU32(base, address + 6032); }
  void set_unknown_6032(uint32_t v) const { guest::StoreU32(base, address + 6032, v); }
  uint32_t unknown_6032_address() const { return address + 6032; }
  uint8_t unknown_6036() const { return guest::LoadU8(base, address + 6036); }
  void set_unknown_6036(uint8_t v) const { guest::StoreU8(base, address + 6036, v); }
  uint32_t unknown_6036_address() const { return address + 6036; }
  uint8_t unknown_6037() const { return guest::LoadU8(base, address + 6037); }
  void set_unknown_6037(uint8_t v) const { guest::StoreU8(base, address + 6037, v); }
  uint32_t unknown_6037_address() const { return address + 6037; }
  uint8_t unknown_6038() const { return guest::LoadU8(base, address + 6038); }
  void set_unknown_6038(uint8_t v) const { guest::StoreU8(base, address + 6038, v); }
  uint32_t unknown_6038_address() const { return address + 6038; }
  // measure fault: Stats::AddRaceResult's r3 is the save data (stats +5980)
  float save_best_828() const { return guest::LoadFloat(base, address + 6028); }
  void set_save_best_828(float v) const { guest::StoreFloat(base, address + 6028, v); }
  uint32_t save_best_828_address() const { return address + 6028; }
  // measure fault: the save data's +6036 (stats +5988)
  float save_sum_820() const { return guest::LoadFloat(base, address + 6036); }
  void set_save_sum_820(float v) const { guest::StoreFloat(base, address + 6036, v); }
  uint32_t save_sum_820_address() const { return address + 6036; }
  // measure fault: the save data's +6044 (stats +5996)
  uint32_t save_best_16() const { return guest::LoadU32(base, address + 6044); }
  void set_save_best_16(uint32_t v) const { guest::StoreU32(base, address + 6044, v); }
  uint32_t save_best_16_address() const { return address + 6044; }
  // measure fault: the save data's +6048
  uint32_t save_kind5_won() const { return guest::LoadU32(base, address + 6048); }
  void set_save_kind5_won(uint32_t v) const { guest::StoreU32(base, address + 6048, v); }
  uint32_t save_kind5_won_address() const { return address + 6048; }
  // measure fault: the save data's +6104 (the mode)
  uint32_t save_mode() const { return guest::LoadU32(base, address + 6104); }
  void set_save_mode(uint32_t v) const { guest::StoreU32(base, address + 6104, v); }
  uint32_t save_mode_address() const { return address + 6104; }
};

// embedded; measure: Counter
struct CareerCounter : View {
  using View::View;
  static constexpr uint32_t kSize = 4840;
  // a bit set of the item kinds (byte +35) first won against
  uint32_t won_kinds() const { return guest::LoadU32(base, address + 0); }
  void set_won_kinds(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t won_kinds_address() const { return address + 0; }
  // first lost against
  uint32_t lost_kinds() const { return guest::LoadU32(base, address + 4); }
  void set_lost_kinds(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t lost_kinds_address() const { return address + 4; }
  // first outcome 3 against
  uint32_t outcome3_kinds() const { return guest::LoadU32(base, address + 8); }
  void set_outcome3_kinds(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t outcome3_kinds_address() const { return address + 8; }
  // up to 9,999
  uint16_t wins() const { return guest::LoadU16(base, address + 12); }
  void set_wins(uint16_t v) const { guest::StoreU16(base, address + 12, v); }
  uint32_t wins_address() const { return address + 12; }
  uint16_t losses() const { return guest::LoadU16(base, address + 14); }
  void set_losses(uint16_t v) const { guest::StoreU16(base, address + 14, v); }
  uint32_t losses_address() const { return address + 14; }
  uint16_t outcome3() const { return guest::LoadU16(base, address + 16); }
  void set_outcome3(uint16_t v) const { guest::StoreU16(base, address + 16, v); }
  uint32_t outcome3_address() const { return address + 16; }
  // counted by Stats::AddRaceResult on a loss whose result's +3920 is 3
  uint16_t unknown_18() const { return guest::LoadU16(base, address + 18); }
  void set_unknown_18(uint16_t v) const { guest::StoreU16(base, address + 18, v); }
  uint32_t unknown_18_address() const { return address + 18; }
  // probably (Career::AddRivalWin)
  uint16_t rival_wins() const { return guest::LoadU16(base, address + 20); }
  void set_rival_wins(uint16_t v) const { guest::StoreU16(base, address + 20, v); }
  uint32_t rival_wins_address() const { return address + 20; }
  uint16_t races() const { return guest::LoadU16(base, address + 22); }
  void set_races(uint16_t v) const { guest::StoreU16(base, address + 22, v); }
  uint32_t races_address() const { return address + 22; }
  // the day block's: the item ids raced today (400 words)
  uint32_t day_opponents_address() const { return address + 24; }
  uint32_t day_opponent_count() const { return guest::LoadU32(base, address + 1624); }
  void set_day_opponent_count(uint32_t v) const { guest::StoreU32(base, address + 1624, v); }
  uint32_t day_opponent_count_address() const { return address + 1624; }
  // 8 bytes an item id: halfwords {losses, wins, outcome 3}, at most 999
  uint32_t day_opponent_records_address() const { return address + 1628; }
  uint16_t day_win_streak() const { return guest::LoadU16(base, address + 4828); }
  void set_day_win_streak(uint16_t v) const { guest::StoreU16(base, address + 4828, v); }
  uint32_t day_win_streak_address() const { return address + 4828; }
  uint16_t day_best_streak() const { return guest::LoadU16(base, address + 4830); }
  void set_day_best_streak(uint16_t v) const { guest::StoreU16(base, address + 4830, v); }
  uint32_t day_best_streak_address() const { return address + 4830; }
  // probably the day's money earned (RaceHudMachine::AwardPrize adds to it with Stats::AddPoints)
  uint32_t day_points() const { return guest::LoadU32(base, address + 4832); }
  void set_day_points(uint32_t v) const { guest::StoreU32(base, address + 4832, v); }
  uint32_t day_points_address() const { return address + 4832; }
  float unknown_4836() const { return guest::LoadFloat(base, address + 4836); }
  void set_unknown_4836(float v) const { guest::StoreFloat(base, address + 4836, v); }
  uint32_t unknown_4836_address() const { return address + 4836; }
  // the week's block's: the largest result's +804
  float week_best_804() const { return guest::LoadFloat(base, address + 24); }
  void set_week_best_804(float v) const { guest::StoreFloat(base, address + 24, v); }
  uint32_t week_best_804_address() const { return address + 24; }
  // a running mean of the result's +796 (RunningMean::MergeTwo)
  float week_mean_796() const { return guest::LoadFloat(base, address + 28); }
  void set_week_mean_796(float v) const { guest::StoreFloat(base, address + 28, v); }
  uint32_t week_mean_796_address() const { return address + 28; }
  uint32_t week_mean_796_count() const { return guest::LoadU32(base, address + 32); }
  void set_week_mean_796_count(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t week_mean_796_count_address() const { return address + 32; }
  // the last result's +808
  uint32_t week_808() const { return guest::LoadU32(base, address + 36); }
  void set_week_808(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t week_808_address() const { return address + 36; }
  uint32_t unknown_40() const { return guest::LoadU32(base, address + 40); }
  void set_unknown_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t unknown_40_address() const { return address + 40; }
  // the results' +816 added
  uint32_t week_sum_816() const { return guest::LoadU32(base, address + 44); }
  void set_week_sum_816(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t week_sum_816_address() const { return address + 44; }
  // the results' +812 added
  uint32_t week_sum_812() const { return guest::LoadU32(base, address + 48); }
  void set_week_sum_812(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t week_sum_812_address() const { return address + 48; }
  // wins against items of kind (byte +35) 1
  uint16_t week_kind1_wins() const { return guest::LoadU16(base, address + 52); }
  void set_week_kind1_wins(uint16_t v) const { guest::StoreU16(base, address + 52, v); }
  uint32_t week_kind1_wins_address() const { return address + 52; }
  uint16_t unknown_54() const { return guest::LoadU16(base, address + 54); }
  void set_unknown_54(uint16_t v) const { guest::StoreU16(base, address + 54, v); }
  uint32_t unknown_54_address() const { return address + 54; }
  uint16_t week_kind4_losses() const { return guest::LoadU16(base, address + 56); }
  void set_week_kind4_losses(uint16_t v) const { guest::StoreU16(base, address + 56, v); }
  uint32_t week_kind4_losses_address() const { return address + 56; }
  uint16_t week_kind5_losses() const { return guest::LoadU16(base, address + 58); }
  void set_week_kind5_losses(uint16_t v) const { guest::StoreU16(base, address + 58, v); }
  uint32_t week_kind5_losses_address() const { return address + 58; }
};

// embedded; measure: Results
struct CareerResults : View {
  using View::View;
  static constexpr uint32_t kSize = 724;
  // up to 15
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // 15 CareerResultRecords, the newest first
  uint32_t records_address() const { return address + 4; }
};

// embedded; measure: ResultRecord
struct CareerResultRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 48;
  // the result's +812
  uint32_t value_812() const { return guest::LoadU32(base, address + 0); }
  void set_value_812(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_812_address() const { return address + 0; }
  // the result's +816
  uint32_t value_816() const { return guest::LoadU32(base, address + 4); }
  void set_value_816(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t value_816_address() const { return address + 4; }
  // the result's 8 bytes at +796 (a float, then a count)
  uint32_t mean_796() const { return guest::LoadU32(base, address + 8); }
  void set_mean_796(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t mean_796_address() const { return address + 8; }
  uint32_t mean_796_count() const { return guest::LoadU32(base, address + 12); }
  void set_mean_796_count(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t mean_796_count_address() const { return address + 12; }
  // the result's +3924 is 1
  uint32_t won() const { return guest::LoadU32(base, address + 16); }
  void set_won(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t won_address() const { return address + 16; }
  // the item id of the result's +980
  uint32_t opponent_1() const { return guest::LoadU32(base, address + 20); }
  void set_opponent_1(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t opponent_1_address() const { return address + 20; }
  // of +1952
  uint32_t opponent_2() const { return guest::LoadU32(base, address + 24); }
  void set_opponent_2(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t opponent_2_address() const { return address + 24; }
  // of +2924
  uint32_t opponent_3() const { return guest::LoadU32(base, address + 28); }
  void set_opponent_3(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t opponent_3_address() const { return address + 28; }
  // the result's +972
  uint32_t value_972() const { return guest::LoadU32(base, address + 32); }
  void set_value_972(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t value_972_address() const { return address + 32; }
  // the race data's +32
  uint32_t race_mode() const { return guest::LoadU32(base, address + 36); }
  void set_race_mode(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t race_mode_address() const { return address + 36; }
  // the racer record's +60
  uint32_t racer_60() const { return guest::LoadU32(base, address + 40); }
  void set_racer_60(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t racer_60_address() const { return address + 40; }
  // the result's halfword +968, sign-extended
  uint32_t value_968() const { return guest::LoadU32(base, address + 44); }
  void set_value_968(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t value_968_address() const { return address + 44; }
};

// embedded; measure: ResultRecord::Fill.r4
struct CareerFillRaceData : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // the race data's +32 (the mode)
  uint32_t mode() const { return guest::LoadU32(base, address + 32); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t mode_address() const { return address + 32; }
};

// embedded; measure: DayLog
struct CareerDayLog : View {
  using View::View;
  static constexpr uint32_t kSize = 124;
  // up to 10
  uint32_t count() const { return guest::LoadU32(base, address + 0); }
  void set_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t count_address() const { return address + 0; }
  // 10 records of 12 bytes {races, a float, a word}, the newest first
  uint32_t records_address() const { return address + 4; }
  // the day block's +22
  uint32_t newest_races() const { return guest::LoadU32(base, address + 4); }
  void set_newest_races(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t newest_races_address() const { return address + 4; }
  // the day block's float +4836
  float newest_4836() const { return guest::LoadFloat(base, address + 8); }
  void set_newest_4836(float v) const { guest::StoreFloat(base, address + 8, v); }
  uint32_t newest_4836_address() const { return address + 8; }
  // the day block's +18
  uint32_t newest_18() const { return guest::LoadU32(base, address + 12); }
  void set_newest_18(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t newest_18_address() const { return address + 12; }
};

// embedded; measure: Counter::AddWin.r4.+4, Counter::AddLoss.r4.+4, Counter::AddOutcome3.r4.+4
struct CareerItemRecord : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // at most 999
  uint16_t losses() const { return guest::LoadU16(base, address + 0); }
  void set_losses(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t losses_address() const { return address + 0; }
  uint16_t wins() const { return guest::LoadU16(base, address + 2); }
  void set_wins(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t wins_address() const { return address + 2; }
  uint16_t outcome3() const { return guest::LoadU16(base, address + 4); }
  void set_outcome3(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t outcome3_address() const { return address + 4; }
  // probably (sub_821116E0)
  uint16_t rival_wins() const { return guest::LoadU16(base, address + 6); }
  void set_rival_wins(uint16_t v) const { guest::StoreU16(base, address + 6, v); }
  uint32_t rival_wins_address() const { return address + 6; }
  uint32_t unknown_8() const { return guest::LoadU32(base, address + 8); }
  void set_unknown_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t unknown_8_address() const { return address + 8; }
};

// file; measure: Counter::AddWin.r4.+8, Counter::AddLoss.r4.+8, Counter::AddOutcome3.r4.+8
struct CareerOpponentItem : View {
  using View::View;
  static constexpr uint32_t kSize = 3008;
  // the item's kind (signed)
  uint8_t kind() const { return guest::LoadU8(base, address + 35); }
  void set_kind(uint8_t v) const { guest::StoreU8(base, address + 35, v); }
  uint32_t kind_address() const { return address + 35; }
};

// embedded; measure: Counter::AddWin.r4, Counter::AddLoss.r4, Counter::AddOutcome3.r4
struct CareerOpponent : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the item id
  uint32_t id() const { return guest::LoadU32(base, address + 0); }
  void set_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t id_address() const { return address + 0; }
  template <typename T = CareerItemRecord>
  T record() const { return T(base, guest::LoadU32(base, address + 4)); }
  void set_record(const View& v) const { guest::StoreU32(base, address + 4, v.address); }
  void set_record(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t record_address() const { return address + 4; }
  template <typename T = CareerOpponentItem>
  T item() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_item(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_item(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t item_address() const { return address + 8; }
};

// embedded; measure: GarageList
struct CareerGarageList : View {
  using View::View;
  static constexpr uint32_t kSize = 98612;
  // 4-byte nodes {+0 the next node, +2 the car's slot}
  uint32_t nodes_address() const { return address + 0; }
  // the free list's first node
  uint16_t free() const { return guest::LoadU16(base, address + 200); }
  void set_free(uint16_t v) const { guest::StoreU16(base, address + 200, v); }
  uint32_t free_address() const { return address + 200; }
  uint16_t head() const { return guest::LoadU16(base, address + 204); }
  void set_head(uint16_t v) const { guest::StoreU16(base, address + 204, v); }
  uint32_t head_address() const { return address + 204; }
  uint16_t count() const { return guest::LoadU16(base, address + 208); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 208, v); }
  uint32_t count_address() const { return address + 208; }
  // 50 cars of 1,968 bytes (the first 1,884 a car object's)
  uint32_t cars_address() const { return address + 212; }
};

// embedded; measure: Garage::Add.r4
struct CareerAddedCar : View {
  using View::View;
  static constexpr uint32_t kSize = 1968;
  uint32_t car_id() const { return guest::LoadU32(base, address + 12); }
  void set_car_id(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t car_id_address() const { return address + 12; }
};

// file; measure: DataTables::Entry56.ret, Entry56
struct CareerEntry56 : View {
  using View::View;
  static constexpr uint32_t kSize = 56;
  // -1 ends the table; 4 never unlocks
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  // bits 0 to 5 the locations 14 to 19, 6 location 48, 7 to 9 the modes 0, 1, other, 10 to 14 the stages
  uint32_t mask() const { return guest::LoadU32(base, address + 4); }
  void set_mask(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t mask_address() const { return address + 4; }
  // added to the entry's index for its next page, 0 none
  uint32_t next_page() const { return guest::LoadU32(base, address + 8); }
  void set_next_page(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t next_page_address() const { return address + 8; }
  // a string index from 4650
  uint32_t text() const { return guest::LoadU32(base, address + 12); }
  void set_text(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t text_address() const { return address + 12; }
  // an image bit (-1 none)
  uint32_t image() const { return guest::LoadU32(base, address + 16); }
  void set_image(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t image_address() const { return address + 16; }
  // nonzero: all three conditions needed, else any
  uint32_t all_conditions() const { return guest::LoadU32(base, address + 20); }
  void set_all_conditions(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t all_conditions_address() const { return address + 20; }
  // three CareerConditions, -1 ends
  uint32_t conditions_address() const { return address + 24; }
  uint32_t unknown_48() const { return guest::LoadU32(base, address + 48); }
  void set_unknown_48(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t unknown_48_address() const { return address + 48; }
  uint32_t unknown_52() const { return guest::LoadU32(base, address + 52); }
  void set_unknown_52(uint32_t v) const { guest::StoreU32(base, address + 52, v); }
  uint32_t unknown_52_address() const { return address + 52; }
};

// file; measure: DataTables::Entry40.ret
struct CareerEntry40 : View {
  using View::View;
  static constexpr uint32_t kSize = 40;
  // Condition::Test's mask of item kinds
  uint32_t kind_mask() const { return guest::LoadU32(base, address + 0); }
  void set_kind_mask(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_mask_address() const { return address + 0; }
  // 0 to 41 (Rewards::Grant)
  uint32_t reward_kind() const { return guest::LoadU32(base, address + 4); }
  void set_reward_kind(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t reward_kind_address() const { return address + 4; }
  uint32_t reward_value() const { return guest::LoadU32(base, address + 8); }
  void set_reward_value(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t reward_value_address() const { return address + 8; }
  uint32_t all_conditions() const { return guest::LoadU32(base, address + 12); }
  void set_all_conditions(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t all_conditions_address() const { return address + 12; }
  // three CareerConditions, -1 ends (-1 in the first ends the table)
  uint32_t conditions_address() const { return address + 16; }
};

// file; measure: Condition
struct CareerCondition : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // 0 to 9
  uint32_t kind() const { return guest::LoadU32(base, address + 0); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t kind_address() const { return address + 0; }
  uint32_t value() const { return guest::LoadU32(base, address + 4); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t value_address() const { return address + 4; }
};

// embedded; measure: Bits24
struct CareerBits24 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t bits() const { return guest::LoadU32(base, address + 0); }
  void set_bits(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t bits_address() const { return address + 0; }
};

// embedded; measure: sub_820CC480.r3
struct CareerCarPart480 : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
};

// embedded; measure: sub_820CC778.r3
struct CareerCarParts778 : View {
  using View::View;
  static constexpr uint32_t kSize = 1148;
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  uint32_t unknown_184() const { return guest::LoadU32(base, address + 184); }
  void set_unknown_184(uint32_t v) const { guest::StoreU32(base, address + 184, v); }
  uint32_t unknown_184_address() const { return address + 184; }
  uint32_t unknown_344() const { return guest::LoadU32(base, address + 344); }
  void set_unknown_344(uint32_t v) const { guest::StoreU32(base, address + 344, v); }
  uint32_t unknown_344_address() const { return address + 344; }
  uint32_t unknown_504() const { return guest::LoadU32(base, address + 504); }
  void set_unknown_504(uint32_t v) const { guest::StoreU32(base, address + 504, v); }
  uint32_t unknown_504_address() const { return address + 504; }
  uint32_t unknown_664() const { return guest::LoadU32(base, address + 664); }
  void set_unknown_664(uint32_t v) const { guest::StoreU32(base, address + 664, v); }
  uint32_t unknown_664_address() const { return address + 664; }
  uint32_t unknown_824() const { return guest::LoadU32(base, address + 824); }
  void set_unknown_824(uint32_t v) const { guest::StoreU32(base, address + 824, v); }
  uint32_t unknown_824_address() const { return address + 824; }
  uint32_t unknown_984() const { return guest::LoadU32(base, address + 984); }
  void set_unknown_984(uint32_t v) const { guest::StoreU32(base, address + 984, v); }
  uint32_t unknown_984_address() const { return address + 984; }
  uint32_t unknown_1144() const { return guest::LoadU32(base, address + 1144); }
  void set_unknown_1144(uint32_t v) const { guest::StoreU32(base, address + 1144, v); }
  uint32_t unknown_1144_address() const { return address + 1144; }
};

// The image words docs/functions/career.md's code uses.
struct CareerGlobals {
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 100.0
  static float hundred(uint8_t* base) { return guest::LoadFloat(base, 0x82002A0Cu); }
  static void set_hundred(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002A0Cu, v); }
  static constexpr uint32_t hundred_address() { return 0x82002A0Cu; }
  // Stats::AddRaceResult's scale of the result's +788 (not read)
  static float scale_788(uint8_t* base) { return guest::LoadFloat(base, 0x820082A0u); }
  static void set_scale_788(uint8_t* base, float v) { guest::StoreFloat(base, 0x820082A0u, v); }
  static constexpr uint32_t scale_788_address() { return 0x820082A0u; }
  // Stats::AddRaceResult's scale of the result's +828 (not read)
  static float scale_828(uint8_t* base) { return guest::LoadFloat(base, 0x820029D8u); }
  static void set_scale_828(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D8u, v); }
  static constexpr uint32_t scale_828_address() { return 0x820029D8u; }
  // DayLog::SumRaces' scale
  static float day_log_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82001FBCu); }
  static void set_day_log_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FBCu, v); }
  static constexpr uint32_t day_log_scale_address() { return 0x82001FBCu; }
  // the play time's frame counter (PlayTime::Add)
  static uint32_t play_time_frames(uint8_t* base) { return guest::LoadU32(base, 0x826C5B90u); }
  static void set_play_time_frames(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B90u, v); }
  static constexpr uint32_t play_time_frames_address() { return 0x826C5B90u; }
  // the race data
  static View race_data(uint8_t* base) { return View(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
};

}  // namespace schema
