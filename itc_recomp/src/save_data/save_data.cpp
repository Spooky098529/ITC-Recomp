// A player's save data in memory, natively (save_data.h,
// docs/functions/save-data.md).

#include "save_data/save_data.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "native/buffers.h"
#include "race_setup/race_setup.h"
#include "schema/career.h"  // CareerSave, CareerStats and their parts
#include "schema/race_event.h"  // SaveData
#include "schema/race_screens.h"  // MenuList
#include "schema/save_data.h"
#include "schema/title_screen.h"  // TitleScreenData
#include "sound_cues/sound_cues.h"

#include <bit>
#include <cstring>

namespace save_data {

namespace {

// The float the set-ups load with lfs (to a double) and store with stfs, as
// those leave its bits (no flush to zero).
uint32_t ZeroFloat() {
  const double value = double(std::bit_cast<float>(schema::SaveDataGlobals::zero()));
  return std::bit_cast<uint32_t>(float(value));
}

// memset (sub_825A71D0) with 0: the same bytes.
void Clear(uint32_t address, uint32_t bytes) {
  guest::ForEachHostPart(address, bytes, [](uint8_t* host, uint32_t, uint32_t size) {
    std::memset(host, 0, size);
  });
}

void Words(uint32_t address, uint32_t count, uint32_t value) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU32(address + 4 * i, value);
  }
}

void Halves(uint32_t address, uint32_t count, uint16_t value) {
  for (uint32_t i = 0; i < count; i++) {
    guest::StoreU16(address + 2 * i, value);
  }
}

}  // namespace

void StatsConstruct(uint32_t stats) {
  ITC_NATIVE_RUNNING("sub_821056C0 Stats::Stats");
  const uint32_t f = ZeroFloat();
  const schema::CareerStats st(stats);
  const schema::CareerCounter week(st.week_address());
  guest::StoreU32(week.week_mean_796_address(), f);  // +4892
  week.set_week_mean_796_count(0);
  const schema::CareerResults results(st.results_address());
  for (uint32_t i = 0; i < 15; i++) {
    const schema::CareerResultRecord record(results.records_address() + schema::CareerResultRecord::kSize * i);
    record.set_mean_796(f);  // +4960 + 48i
    record.set_mean_796_count(0);
  }
  // The nine running means {float, count} from +5880.
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(st.mean_812_address() + 8 * i, f);
    guest::StoreU32(st.mean_812_count_address() + 8 * i, 0);
  }
}

void StatsResetRecords(uint32_t records) {
  ITC_NATIVE_RUNNING("sub_821136D8 Stats::ResetRecords");
  const uint32_t f = ZeroFloat();
  const schema::CareerResults results(records);
  for (uint32_t i = 0; i < 15; i++) {
    const schema::CareerResultRecord r(results.records_address() + schema::CareerResultRecord::kSize * i);
    r.set_value_812(0);
    r.set_value_816(0);
    r.set_mean_796(f);
    r.set_mean_796_count(0);
    r.set_won(0);
    r.set_opponent_1(0xFFFFFFFF);
    r.set_opponent_2(0xFFFFFFFF);
    r.set_opponent_3(0xFFFFFFFF);
    r.set_value_972(0);
    r.set_race_mode(0);
    r.set_value_968(0);  // (+40 left)
  }
  results.set_count(0);
}

uint32_t StatsReset(uint32_t s) {
  ITC_NATIVE_RUNNING("sub_82113730 Stats::Reset");
  const schema::CareerStats st(s);
  Clear(s, kStatsBytes);
  Clear(st.counter_address(), 24);
  const schema::CareerCounter day(st.day_address());
  Clear(day.day_opponents_address(), 4828);  // +24
  const uint32_t f = ZeroFloat();
  guest::StoreU32(day.unknown_4836_address(), f);  // +4860
  day.set_day_best_streak(0);
  day.set_day_win_streak(0);
  day.set_day_points(0);
  const schema::CareerCounter week(st.week_address());
  Clear(week.address, 24);  // the week's counter (+4864)
  guest::StoreU32(week.week_best_804_address(), f);  // +4888
  guest::StoreU32(week.week_mean_796_address(), f);
  week.set_week_mean_796_count(0);
  week.set_week_808(0);
  week.set_week_sum_812(0);
  week.set_week_sum_816(0);
  Halves(week.week_kind1_wins_address(), 4, 0);  // +4916
  Clear(st.other_counter_address(), 24);
  st.set_unknown_6032(0);
  st.set_unknown_6036(0);
  st.set_unknown_6037(0);
  st.set_unknown_6038(0);
  // The day log's 10 records {races, a float, a word} (+5676).
  const schema::CareerDayLog log(st.day_log_address());
  for (uint32_t i = 0; i < 10; i++) {
    guest::StoreU32(log.newest_4836_address() + 12 * i, f);
    guest::StoreU32(log.newest_races_address() + 12 * i, 0);
    guest::StoreU32(log.newest_18_address() + 12 * i, 0);
  }
  log.set_count(0);
  StatsResetRecords(st.results_address());
  st.set_unknown_6008(3);
  // The by-mode halfwords (+5796, 14 bytes a mode): modes 0 to 2 a mode at a
  // time, then +5838 (14 halfwords) and +5866 (6).
  for (uint32_t i = 0; i < 3; i++) {
    Halves(st.by_mode_address() + 14 * i, 7, 0);
  }
  Halves(st.by_mode_address() + 42, 14, 0);
  Halves(st.by_mode_address() + 70, 6, 0);
  st.set_day_total_0(1);  // +5956
  return st.results_address();
}

uint32_t ProgressClear(uint32_t p) {
  ITC_NATIVE_RUNNING("sub_82113BD8 Progress::Clear");
  const schema::CareerSave sv(p);
  Clear(p, kProgressBytes);
  sv.set_new_game(0);
  sv.set_unknown_4(0);
  for (uint32_t at : {sv.pick_address(), sv.pick_kept_address()}) {
    const schema::CareerPick pick(at);
    pick.set_level(0);
    pick.set_first_kind(6);
    pick.set_first_value(0);
    pick.set_first_set(1);
    pick.set_second_kind(6);
    pick.set_second_value(0);
    pick.set_second_set(1);
  }
  const uint32_t r3 = StatsReset(sv.stats_address());
  sv.set_stages(0);
  sv.set_unknown_6096(0);
  sv.set_location(14);
  sv.set_next_location(48);
  guest::StoreU32(p + 0x1AD1C, 4);  // (no field in CareerSave yet)
  sv.set_unknown_109848(0);  // +0x1AD18
  sv.set_day(0);
  sv.set_money(3000);
  // The garage list: 50 nodes {+0 next, +2 car}.
  const schema::CareerGarageList garage(sv.garage_address());
  for (uint32_t i = 0; i < 50; i++) {
    guest::StoreU16(garage.nodes_address() + 4 * i + 2, uint16_t(i));
    guest::StoreU16(garage.nodes_address() + 4 * i + 0, uint16_t(i + 1));
  }
  garage.set_free(0);
  garage.set_count(0);
  guest::StoreU16(garage.nodes_address() + 196, 0xFFFF);  // node 49's next
  garage.set_head(0xFFFF);
  sv.set_unknown_6326(0xFFFF);  // the list's +206
  sv.set_unknown_6322(0xFFFF);  // the list's +202
  guest::StoreU32(p + 0x1991C, 50);  // (no field in CareerSave yet)
  sv.set_selected_car(0);
  Words(sv.reward_cars_address(), 3, 0);
  Words(sv.unlock_set_address(), 15, 0);
  Words(sv.reward_set_40_address(), 7, 0);
  Words(sv.reward_set_41_address(), 7, 0);
  guest::StoreU32(sv.locations_address(), 0);
  guest::StoreU32(sv.locations_address() + 4, 0);
  // The 400 item records of 12 bytes.
  for (uint32_t i = 0; i < 400; i++) {
    const schema::CareerItemRecord item(sv.item_records_address() + schema::CareerItemRecord::kSize * i);
    item.set_unknown_8(0);
    item.set_rival_wins(0);
    item.set_outcome3(0);
    item.set_wins(0);
    item.set_losses(0);
  }
  sv.set_unknown_109792(0);  // +0x1ACE0
  // The offers' counts: 6 locations of 100 bytes, the count at +96.
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(sv.offers_address() + 96 + 100 * i, 0);
  }
  Words(sv.table_words_address(), 1536, 0);
  sv.set_movie_after_results(0xFFFFFFFF);
  sv.set_unlocks_shown_count(0);
  guest::StoreU32(sv.table0_done_address(), 0);
  guest::StoreU32(sv.table0_done_address() + 4, 0);
  sv.set_unknown_120104(0);  // +0x1D528
  sv.set_rewards_shown_count(0);
  Words(sv.rewards_granted_address(), 7, 0);
  return r3;
}

void BitSet400SetAll(uint32_t set) {
  ITC_NATIVE_RUNNING("sub_821133A8 BitSet400::SetAll");
  const schema::BitSet400 bits(set);
  Words(bits.words_address(), 13, 0);
  for (uint32_t i = 0; i < 400; i++) {
    const uint32_t word = bits.words_address() + 4 * (i >> 5);
    guest::StoreU32(word, guest::LoadU32(word) | (1u << (i & 31)));
  }
}

namespace {

// SetDefaults' copies from the tables: {the destination in the save data
// (CareerSave's field), bytes, the memcpy call's return address}.
struct TableCopy {
  uint32_t (schema::CareerSave::*destination)() const;
  uint32_t bytes;
  uint32_t caller;
};
constexpr TableCopy kCopy50A8{&schema::CareerSave::unlock_set_address, 60, 0x82113E94};
constexpr TableCopy kCopy3C24{&schema::CareerSave::reward_set_40_address, 28, 0x82113EAC};
constexpr TableCopy kCopy50E4{&schema::CareerSave::reward_set_41_address, 28, 0x82113EC4};

// The copy's destination (an address only: nothing read through `base`).
uint32_t Destination(uint32_t save, const TableCopy& copy) {
  return (schema::CareerSave(save).*copy.destination)();
}

void Copy(uint32_t save, uint32_t table, const TableCopy& copy) {
  if (!native::Buffers_Memcpy(Destination(save, copy), table, copy.bytes,
                              copy.caller)) {
    ITC_LOG_ERROR("save_data: Progress::SetDefaults's copy of {:08X} not made natively", table);
  }
}

}  // namespace

bool SetDefaultsNative(uint32_t save) {
  return native::Buffers_NativeMemcpy() &&
         !c_runtime::MayShare(Destination(nullptr, save, kCopy50A8), kCopy50A8.bytes,
                              schema::SaveDataGlobals::defaults_50a8_address(), 60) &&
         !c_runtime::MayShare(Destination(nullptr, save, kCopy3C24), kCopy3C24.bytes,
                              schema::SaveDataGlobals::defaults_3c24_address(), 28) &&
         !c_runtime::MayShare(Destination(nullptr, save, kCopy50E4), kCopy50E4.bytes,
                              schema::SaveDataGlobals::defaults_50e4_address(), 28);
}

uint32_t ProgressSetDefaults(uint32_t s) {
  ITC_NATIVE_RUNNING("sub_82113E60 Progress::SetDefaults");
  const schema::CareerSave sv(s);
  ProgressClear(s);
  sv.set_new_game(1);
  Copy(s, DefaultsTable50A8(), kCopy50A8);
  Copy(s, DefaultsTable3C24(), kCopy3C24);
  Copy(s, DefaultsTable50E4(), kCopy50E4);
  // Bits set one store at a time: 0x703 at +0x199E0, then +0x199E8 and
  // +0x199EC cleared and 0xCF0F or'd into +0x199E8.
  uint32_t bits = 0;
  guest::StoreU32(sv.locations_address(), bits);
  for (uint32_t bit : {1u, 2u, 0x100u, 0x200u, 0x400u}) {
    bits |= bit;
    guest::StoreU32(sv.locations_address(), bits);
  }
  guest::StoreU32(sv.locations_2_address(), 0);
  guest::StoreU32(sv.locations_2_address() + 4, 0);
  bits = guest::LoadU32(sv.locations_2_address());
  for (uint32_t bit : {1u, 2u, 4u, 8u, 0x100u, 0x200u, 0x400u, 0x800u, 0x4000u, 0x8000u}) {
    bits |= bit;
    guest::StoreU32(sv.locations_2_address(), bits);
  }
  // Table3044's 15 words without Table3E34's bits (the original copies the
  // first to its stack, clears the bits there and copies the stack in).
  const uint32_t kept = DefaultsTable3044();
  const uint32_t taken = DefaultsTable3E34();
  uint32_t words[15];
  for (uint32_t i = 0; i < 15; i++) {
    words[i] = guest::LoadU32(kept + 4 * i) & ~guest::LoadU32(taken + 4 * i);
  }
  for (uint32_t i = 0; i < 15; i++) {
    guest::StoreU32(sv.unlock_set_2_address() + 4 * i, words[i]);
  }
  bits = guest::LoadU32(sv.reward_cars_address());
  for (uint32_t bit : {0x1u, 0x2u, 0x10u, 0x20u, 0x40u, 0x80u, 0x100u, 0x400u, 0x800u, 0x1000u,
                       0x2000u, 0x4000u, 0x8000u, 0x10000u, 0x20000u, 0x80000u, 0x200000u,
                       0x400000u}) {
    bits |= bit;
    guest::StoreU32(sv.reward_cars_address(), bits);
  }
  const uint32_t unlocks = sv.item_flags_address();
  BitSet400SetAll(unlocks);
  const schema::BitSet400 unlock_bits(unlocks);
  for (uint32_t i = 310; i <= 375; i++) {
    const uint32_t word = unlock_bits.words_address() + 4 * (i >> 5);
    guest::StoreU32(word, guest::LoadU32(word) & ~(1u << (i & 31)));
  }
  sv.set_play_time_mark(schema::SaveDataGlobals::play_time_frames());
  return unlocks;
}

void ControlMapSetDefault(uint32_t map) {
  ITC_NATIVE_RUNNING("sub_821058A8 ControlMap::SetDefault");
  const schema::ControlMap m(map);
  m.set_steering(18);
  m.set_throttle(14);
  m.set_brake(13);
  m.set_input_10(9);
  m.set_button_16(0);
  m.set_button_20(10);
  m.set_button_24(16);
  m.set_button_28(15);
  m.set_button_32(24);
  m.set_button_36(7);
  m.set_button_40(17);
  m.set_unknown_44(0);
  m.set_input_12(25);
  m.set_button_52(8);
  m.set_button_56(11);
}

uint32_t OptionsSetDefaults(uint32_t o) {
  ITC_NATIVE_RUNNING("sub_82105940 Options::SetDefaults");
  for (uint32_t offset : {16u, 0u, 4u, 8u, 20u, 12u}) {
    guest::StoreU32(o + offset, 1);
  }
  guest::StoreU32(o + 24, 2);
  for (uint32_t i = 36; i <= 41; i++) {
    guest::StoreU8(o + i, 0);
  }
  guest::StoreU8(o + 28, 15);
  guest::StoreU8(o + 29, 15);
  guest::StoreU32(o + 32, 0);
  for (uint32_t offset : {44u, 104u, 164u, 224u}) {
    ControlMapSetDefault(o + offset);
  }
  return o + 224;
}

uint32_t CourseRecordsReset(uint32_t c) {
  ITC_NATIVE_RUNNING("sub_82105D08 CourseRecords::Reset");
  const schema::CourseRecords records(c);
  records.set_unknown_0(0);
  records.set_unknown_4(0);
  records.set_unknown_8(0);
  records.set_unknown_12(0);
  records.set_unknown_18(0);
  records.set_unknown_16(0);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 22; i++) {
    r3 = race_setup::CourseRecordReset(records.records_address() + 408 * i);
  }
  return r3;
}

uint64_t CourseRecordsOf(uint64_t records, uint32_t pair) {
  ITC_NATIVE_RUNNING("sub_82105BF8 CourseRecords::Of");
  const schema::CourseRecordKey key(pair);
  const uint32_t course = key.course(), traffic = key.traffic();
  // The 64-bit arithmetic of the original (course << 1 in 32 bits).
  const uint64_t index = uint64_t(uint32_t(course << 1)) + uint64_t(traffic);
  return records + index * 408 + 20;
}

std::optional<uint32_t> SaveDataDelete(uint32_t player) {
  ITC_NATIVE_RUNNING("sub_821055D8 SaveData::Delete");
  const uint32_t slot = Slot(player);
  const uint32_t block = guest::LoadU32(slot);
  if (block != 0 && !memory::IsHeapBlock(block)) {
    return std::nullopt;
  }
  memory::crt::OperatorDelete(block);
  guest::StoreU32(slot, 0);
  return block;
}

bool CreateNative() { return native::Buffers_NativeMemcpy(); }

std::optional<uint32_t> SaveDataCreate(uint32_t player) {
  ITC_NATIVE_RUNNING("sub_821057C8 SaveData::Create");
  const auto made = memory::crt::OperatorNew(kSaveDataBytes);
  if (!made) {
    return std::nullopt;
  }
  const uint32_t block = *made;
  if (block != 0) {
    StatsConstruct(schema::CareerSave(block).stats_address());
  }
  const uint32_t slot = Slot(player);
  guest::StoreU32(slot, block);
  // The course records (+0x1D52C) and the sound bank holder (+0x1F850): no
  // fields in SaveData yet.
  CourseRecordsReset(block + 0x1D52C);
  ProgressSetDefaults(guest::LoadU32(slot));
  OptionsSetDefaults(schema::SaveData(guest::LoadU32(slot)).options_address());
  const uint32_t holder = guest::LoadU32(slot) + 0x1F850;
  sound_cues::SoundBankHolderConstruct(holder);
  return holder;
}

void PlayersResetControllers() {
  ITC_NATIVE_RUNNING("sub_8209F5E0 Players::ResetControllers");
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(schema::SaveDataGlobals::controllers_address() + 4 * i, i);
  }
  schema::SaveDataGlobals::set_profile_read_asked(1);
}

bool MenuListInitNative(uint32_t list) {
  const uint32_t old = schema::MenuList(list).enabled().address;
  return old == 0 || memory::IsPages(old);
}

uint32_t MenuListInit(uint32_t list, const MenuListArgs& a) {
  ITC_NATIVE_RUNNING("sub_8220BAC0 MenuList::Init");
  const schema::MenuList m(list);
  m.set_count(a.count);
  m.set_scroll(0);
  m.set_scroll_before(0);
  m.set_selection(0);
  m.set_selection_before(0);
  m.set_shown(int32_t(a.count) < int32_t(a.shown) ? a.count : a.shown);
  const uint32_t old = m.enabled().address;
  m.set_down_action(a.r7);
  m.set_up_action(a.r8);
  m.set_left_action(a.r9);
  m.set_right_action(a.r10);
  m.set_accept_action(a.id5);
  m.set_back_action(a.id6);
  m.set_wraps(a.r6);
  m.set_move(0);
  m.set_side(0);
  if (old != 0) {
    memory::FreePages(old);  // VirtualFree(old, 0, MEM_RELEASE)
  }
  // VirtualAlloc(0, count x 4, MEM_COMMIT, PAGE_READWRITE): a blob (E5 step
  // 5, PC memory in the guest's byte order, zeroed as the pages were); 0 for
  // 0 bytes, as VirtualAlloc gave.
  const uint32_t bytes = m.count() << 2;
  const uint32_t flags = bytes == 0 ? 0 : schema::MenuListEnabled::New(bytes).address;
  m.set_enabled(flags);
  for (int32_t i = 0; i < int32_t(m.count()); i++) {
    guest::StoreU32(m.enabled().address + 4 * uint32_t(i), 1);
  }
  return flags;
}

bool ResetPlayerNative() {
  const uint32_t block = guest::LoadU32(Slot(0));
  return MenuListInitNative(schema::SaveDataGlobals::title_menu_list().address) &&
         (block == 0 || memory::IsHeapBlock(block)) && CreateNative();
}

std::optional<uint32_t> TitleScreenResetPlayer() {
  ITC_NATIVE_RUNNING("sub_82221D00 TitleScreen::ResetPlayer");
  MenuListInit(schema::SaveDataGlobals::title_menu_list().address,
               {1, 1, 0, 154, 153, 155, 156, 159, 158});
  schema::SaveDataGlobals::title_screen().set_idle_ticks(0);
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(schema::SaveDataGlobals::mode_checks_address() + 4 * i, 1);
  }
  SaveDataDelete(0);
  const auto r3 = SaveDataCreate(0);
  if (!r3) {
    return std::nullopt;
  }
  PlayersResetControllers();
  return r3;
}

uint32_t DefaultsTable50A8() {
  ITC_NATIVE_RUNNING("sub_820CCCC0 Defaults::Table50A8");
  return schema::SaveDataGlobals::defaults_50a8_address();
}

uint32_t DefaultsTable3044() {
  ITC_NATIVE_RUNNING("sub_820CCCD0 Defaults::Table3044");
  return schema::SaveDataGlobals::defaults_3044_address();
}

uint32_t DefaultsTable3E34() {
  ITC_NATIVE_RUNNING("sub_820CCCE0 Defaults::Table3E34");
  return schema::SaveDataGlobals::defaults_3e34_address();
}

uint32_t DefaultsTable3C24() {
  ITC_NATIVE_RUNNING("sub_820CCCF0 Defaults::Table3C24");
  return schema::SaveDataGlobals::defaults_3c24_address();
}

uint32_t DefaultsTable50E4() {
  ITC_NATIVE_RUNNING("sub_820CCD00 Defaults::Table50E4");
  return schema::SaveDataGlobals::defaults_50e4_address();
}

uint32_t TwoPlayerBlock() {
  ITC_NATIVE_RUNNING("sub_82105678 SaveData::TwoPlayerBlock");
  return schema::SaveDataGlobals::two_player_block();
}

std::optional<uint32_t> CreateTwoPlayerBlock() {
  if (!CreateNative()) {
    return std::nullopt;
  }
  const auto made = memory::crt::OperatorNew(kTwoPlayerBlockBytes);
  if (!made) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82105740 SaveData::CreateTwoPlayerBlock");
  constexpr uint32_t kHalf = 0x1D52C;
  const uint32_t block = *made;
  if (block != 0) {
    StatsConstruct(schema::CareerSave(block).stats_address());
    StatsConstruct(schema::CareerSave(block + kHalf).stats_address());
  }
  schema::SaveDataGlobals::set_two_player_block(block);
  ProgressSetDefaults(schema::SaveDataGlobals::two_player_block());
  ProgressSetDefaults(schema::SaveDataGlobals::two_player_block() + kHalf);
  return OptionsSetDefaults(schema::SaveDataGlobals::two_player_block() + 0x3AA58);
}

}  // namespace save_data
