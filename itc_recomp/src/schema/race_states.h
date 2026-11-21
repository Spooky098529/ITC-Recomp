// The views of docs/functions/race-states.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/race-states.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// allocated; measure: Racers
struct RaceData : View {
  using View::View;
  static constexpr uint32_t kSize = 670464;
  // read as a float and a word by AiDriver::Setup and Racers::NearestHumanVehicle: probably a racer record's +0 (the measure's join)
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
  // the race's route (16 bytes, Route::Set; race-setup.md)
  uint32_t route_address() const { return address + 4; }
  // a course route's index (0 to 22, probably the course; AiPath::BranchMasks reads 0 to 7)
  uint32_t route_course() const { return guest::LoadU32(base, address + 4); }
  void set_route_course(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t route_course_address() const { return address + 4; }
  // the location (1 to 46 in Race::StartPosition)
  uint32_t route_location() const { return guest::LoadU32(base, address + 8); }
  void set_route_location(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t route_location_address() const { return address + 8; }
  // the route's first position (section, slice)
  uint32_t route_first_position() const { return guest::LoadU32(base, address + 12); }
  void set_route_first_position(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t route_first_position_address() const { return address + 12; }
  // the route's last position (section, slice)
  uint32_t route_last_position() const { return guest::LoadU32(base, address + 16); }
  void set_route_last_position(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t route_last_position_address() const { return address + 16; }
  // 0x4 to 0x20: which branch kinds the AI drivers take (AiPath::BranchMasksFor); from the save data's +0x199E0
  uint32_t branch_flags() const { return guest::LoadU32(base, address + 20); }
  void set_branch_flags(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t branch_flags_address() const { return address + 20; }
  // 0x1 to 0x800: which entries of the course set's +2124 list are drawn
  uint32_t drawn_entries() const { return guest::LoadU32(base, address + 24); }
  void set_drawn_entries(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t drawn_entries_address() const { return address + 24; }
  // 0 at the reset; passed to the overlays (their effects update with 0 or 3); read by RaceStates::OnRaceEnd
  uint32_t effects_word() const { return guest::LoadU32(base, address + 28); }
  void set_effects_word(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t effects_word_address() const { return address + 28; }
  // the mode (the save data's +6104, sub_820FF968)
  uint32_t mode() const { return guest::LoadU32(base, address + 32); }
  void set_mode(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t mode_address() const { return address + 32; }
  // 93 racer records of 3,936 bytes (the racers object, Racers::Reset)
  uint32_t racer_records_address() const { return address + 48; }
  // racer record 0's +12 (its first entry's place), read by Kind6Menu::SaveReplay and RaceHudMachine::ShowResults
  uint32_t record0_place() const { return guest::LoadU32(base, address + 60); }
  void set_record0_place(uint32_t v) const { guest::StoreU32(base, address + 60, v); }
  uint32_t record0_place_address() const { return address + 60; }
  // measure fault: a racer record's byte +76 (Vehicle::SetUp's record at +0)
  uint8_t fault_record_byte_76() const { return guest::LoadU8(base, address + 76); }
  void set_fault_record_byte_76(uint8_t v) const { guest::StoreU8(base, address + 76, v); }
  uint32_t fault_record_byte_76_address() const { return address + 76; }
  // measure fault: a racer record's byte +77 (Vehicle::SetUp's record at +0)
  uint8_t fault_record_byte_77() const { return guest::LoadU8(base, address + 77); }
  void set_fault_record_byte_77(uint8_t v) const { guest::StoreU8(base, address + 77, v); }
  uint32_t fault_record_byte_77_address() const { return address + 77; }
  // measure fault: the stage list's +800 (the stage count; Stages:: accessors' r3 is the list)
  uint32_t fault_stage_count() const { return guest::LoadU32(base, address + 800); }
  void set_fault_stage_count(uint32_t v) const { guest::StoreU32(base, address + 800, v); }
  uint32_t fault_stage_count_address() const { return address + 800; }
  // measure fault: the stage list's +804 (the current stage)
  uint32_t fault_current_stage() const { return guest::LoadU32(base, address + 804); }
  void set_fault_current_stage(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t fault_current_stage_address() const { return address + 804; }
  // measure fault: the stage list's +808 (sub_820FFAB0 sets it, sub_820FFC68 reads it: 48 rather than 7 after a battle called off)
  uint32_t fault_stages_word_808() const { return guest::LoadU32(base, address + 808); }
  void set_fault_stages_word_808(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t fault_stages_word_808_address() const { return address + 808; }
  // measure fault: a racer record's +1656 (its car's; sub_8210EBF0's horn scale below 2 or 5)
  uint32_t fault_record_word_1656() const { return guest::LoadU32(base, address + 1656); }
  void set_fault_record_word_1656(uint32_t v) const { guest::StoreU32(base, address + 1656, v); }
  uint32_t fault_record_word_1656_address() const { return address + 1656; }
  // measure fault: a racer record's +1672 (Vehicle::SetUp)
  uint32_t fault_record_word_1672() const { return guest::LoadU32(base, address + 1672); }
  void set_fault_record_word_1672(uint32_t v) const { guest::StoreU32(base, address + 1672, v); }
  uint32_t fault_record_word_1672_address() const { return address + 1672; }
  // measure fault: a racer record's +1676 (Vehicle::SetUp)
  uint32_t fault_record_word_1676() const { return guest::LoadU32(base, address + 1676); }
  void set_fault_record_word_1676(uint32_t v) const { guest::StoreU32(base, address + 1676, v); }
  uint32_t fault_record_word_1676_address() const { return address + 1676; }
  // measure fault: a racer record's car tuning (+1696) word +4 (EventCondition::Tuning4)
  uint32_t fault_record_tuning_4() const { return guest::LoadU32(base, address + 1700); }
  void set_fault_record_tuning_4(uint32_t v) const { guest::StoreU32(base, address + 1700, v); }
  uint32_t fault_record_tuning_4_address() const { return address + 1700; }
  // measure fault: the record's car tuning +8 (EventCondition::PartsAtLeast, StockParts)
  uint32_t fault_record_tuning_8() const { return guest::LoadU32(base, address + 1704); }
  void set_fault_record_tuning_8(uint32_t v) const { guest::StoreU32(base, address + 1704, v); }
  uint32_t fault_record_tuning_8_address() const { return address + 1704; }
  // measure fault: the record's car tuning +16
  uint32_t fault_record_tuning_16() const { return guest::LoadU32(base, address + 1712); }
  void set_fault_record_tuning_16(uint32_t v) const { guest::StoreU32(base, address + 1712, v); }
  uint32_t fault_record_tuning_16_address() const { return address + 1712; }
  // measure fault: the record's car tuning +20 (Vehicle::SetUpSurfaceGrip)
  uint32_t fault_record_tuning_20() const { return guest::LoadU32(base, address + 1716); }
  void set_fault_record_tuning_20(uint32_t v) const { guest::StoreU32(base, address + 1716, v); }
  uint32_t fault_record_tuning_20_address() const { return address + 1716; }
  // measure fault: the record's car tuning +24
  uint32_t fault_record_tuning_24() const { return guest::LoadU32(base, address + 1720); }
  void set_fault_record_tuning_24(uint32_t v) const { guest::StoreU32(base, address + 1720, v); }
  uint32_t fault_record_tuning_24_address() const { return address + 1720; }
  // measure fault: the record's car tuning +28
  uint32_t fault_record_tuning_28() const { return guest::LoadU32(base, address + 1724); }
  void set_fault_record_tuning_28(uint32_t v) const { guest::StoreU32(base, address + 1724, v); }
  uint32_t fault_record_tuning_28_address() const { return address + 1724; }
  // measure fault: the record's car tuning +32
  uint32_t fault_record_tuning_32() const { return guest::LoadU32(base, address + 1728); }
  void set_fault_record_tuning_32(uint32_t v) const { guest::StoreU32(base, address + 1728, v); }
  uint32_t fault_record_tuning_32_address() const { return address + 1728; }
  // measure fault: the record's car tuning +36
  uint32_t fault_record_tuning_36() const { return guest::LoadU32(base, address + 1732); }
  void set_fault_record_tuning_36(uint32_t v) const { guest::StoreU32(base, address + 1732, v); }
  uint32_t fault_record_tuning_36_address() const { return address + 1732; }
  // measure fault: the record's car tuning +40
  uint32_t fault_record_tuning_40() const { return guest::LoadU32(base, address + 1736); }
  void set_fault_record_tuning_40(uint32_t v) const { guest::StoreU32(base, address + 1736, v); }
  uint32_t fault_record_tuning_40_address() const { return address + 1736; }
  // measure fault: the record's car tuning +48
  uint32_t fault_record_tuning_48() const { return guest::LoadU32(base, address + 1744); }
  void set_fault_record_tuning_48(uint32_t v) const { guest::StoreU32(base, address + 1744, v); }
  uint32_t fault_record_tuning_48_address() const { return address + 1744; }
  // measure fault: the record's car tuning +56
  uint32_t fault_record_tuning_56() const { return guest::LoadU32(base, address + 1752); }
  void set_fault_record_tuning_56(uint32_t v) const { guest::StoreU32(base, address + 1752, v); }
  uint32_t fault_record_tuning_56_address() const { return address + 1752; }
  // measure fault: the record's car tuning +60
  uint32_t fault_record_tuning_60() const { return guest::LoadU32(base, address + 1756); }
  void set_fault_record_tuning_60(uint32_t v) const { guest::StoreU32(base, address + 1756, v); }
  uint32_t fault_record_tuning_60_address() const { return address + 1756; }
  // measure fault: the record's car tuning +64
  uint32_t fault_record_tuning_64() const { return guest::LoadU32(base, address + 1760); }
  void set_fault_record_tuning_64(uint32_t v) const { guest::StoreU32(base, address + 1760, v); }
  uint32_t fault_record_tuning_64_address() const { return address + 1760; }
  // measure fault: the record's car tuning +68
  uint32_t fault_record_tuning_68() const { return guest::LoadU32(base, address + 1764); }
  void set_fault_record_tuning_68(uint32_t v) const { guest::StoreU32(base, address + 1764, v); }
  uint32_t fault_record_tuning_68_address() const { return address + 1764; }
  // measure fault: the record's car tuning +72
  uint32_t fault_record_tuning_72() const { return guest::LoadU32(base, address + 1768); }
  void set_fault_record_tuning_72(uint32_t v) const { guest::StoreU32(base, address + 1768, v); }
  uint32_t fault_record_tuning_72_address() const { return address + 1768; }
  // measure fault: the record's car tuning +76
  uint32_t fault_record_tuning_76() const { return guest::LoadU32(base, address + 1772); }
  void set_fault_record_tuning_76(uint32_t v) const { guest::StoreU32(base, address + 1772, v); }
  uint32_t fault_record_tuning_76_address() const { return address + 1772; }
  // measure fault: the record's car tuning +80
  uint32_t fault_record_tuning_80() const { return guest::LoadU32(base, address + 1776); }
  void set_fault_record_tuning_80(uint32_t v) const { guest::StoreU32(base, address + 1776, v); }
  uint32_t fault_record_tuning_80_address() const { return address + 1776; }
  // measure fault: the record's car tuning +88
  uint32_t fault_record_tuning_88() const { return guest::LoadU32(base, address + 1784); }
  void set_fault_record_tuning_88(uint32_t v) const { guest::StoreU32(base, address + 1784, v); }
  uint32_t fault_record_tuning_88_address() const { return address + 1784; }
  // measure fault: the record's car tuning +96 (EventCondition::AeroFitted)
  uint32_t fault_record_tuning_96() const { return guest::LoadU32(base, address + 1792); }
  void set_fault_record_tuning_96(uint32_t v) const { guest::StoreU32(base, address + 1792, v); }
  uint32_t fault_record_tuning_96_address() const { return address + 1792; }
  // measure fault: the record's car tuning +100 (also HornSound::Update)
  uint32_t fault_record_tuning_100() const { return guest::LoadU32(base, address + 1796); }
  void set_fault_record_tuning_100(uint32_t v) const { guest::StoreU32(base, address + 1796, v); }
  uint32_t fault_record_tuning_100_address() const { return address + 1796; }
  // measure fault: the record's car tuning +104
  uint32_t fault_record_tuning_104() const { return guest::LoadU32(base, address + 1800); }
  void set_fault_record_tuning_104(uint32_t v) const { guest::StoreU32(base, address + 1800, v); }
  uint32_t fault_record_tuning_104_address() const { return address + 1800; }
  // measure fault: the record's car tuning +108
  uint32_t fault_record_tuning_108() const { return guest::LoadU32(base, address + 1804); }
  void set_fault_record_tuning_108(uint32_t v) const { guest::StoreU32(base, address + 1804, v); }
  uint32_t fault_record_tuning_108_address() const { return address + 1804; }
  // measure fault: the record's car tuning +112
  uint32_t fault_record_tuning_112() const { return guest::LoadU32(base, address + 1808); }
  void set_fault_record_tuning_112(uint32_t v) const { guest::StoreU32(base, address + 1808, v); }
  uint32_t fault_record_tuning_112_address() const { return address + 1808; }
  // measure fault: the record's car tuning +116
  uint32_t fault_record_tuning_116() const { return guest::LoadU32(base, address + 1812); }
  void set_fault_record_tuning_116(uint32_t v) const { guest::StoreU32(base, address + 1812, v); }
  uint32_t fault_record_tuning_116_address() const { return address + 1812; }
  // measure fault: the record's car tuning +120
  uint32_t fault_record_tuning_120() const { return guest::LoadU32(base, address + 1816); }
  void set_fault_record_tuning_120(uint32_t v) const { guest::StoreU32(base, address + 1816, v); }
  uint32_t fault_record_tuning_120_address() const { return address + 1816; }
  // measure fault: the record's car tuning +124
  uint32_t fault_record_tuning_124() const { return guest::LoadU32(base, address + 1820); }
  void set_fault_record_tuning_124(uint32_t v) const { guest::StoreU32(base, address + 1820, v); }
  uint32_t fault_record_tuning_124_address() const { return address + 1820; }
  // measure fault: the record's car tuning +128
  uint32_t fault_record_tuning_128() const { return guest::LoadU32(base, address + 1824); }
  void set_fault_record_tuning_128(uint32_t v) const { guest::StoreU32(base, address + 1824, v); }
  uint32_t fault_record_tuning_128_address() const { return address + 1824; }
  // measure fault: the record's car tuning +132
  uint32_t fault_record_tuning_132() const { return guest::LoadU32(base, address + 1828); }
  void set_fault_record_tuning_132(uint32_t v) const { guest::StoreU32(base, address + 1828, v); }
  uint32_t fault_record_tuning_132_address() const { return address + 1828; }
  // measure fault: the record's car tuning +136 (the extra gauges' part, RaceHud::SetUpExtraGauges)
  uint32_t fault_record_tuning_136() const { return guest::LoadU32(base, address + 1832); }
  void set_fault_record_tuning_136(uint32_t v) const { guest::StoreU32(base, address + 1832, v); }
  uint32_t fault_record_tuning_136_address() const { return address + 1832; }
  // measure fault: the record's car tuning +144
  uint32_t fault_record_tuning_144() const { return guest::LoadU32(base, address + 1840); }
  void set_fault_record_tuning_144(uint32_t v) const { guest::StoreU32(base, address + 1840, v); }
  uint32_t fault_record_tuning_144_address() const { return address + 1840; }
  // measure fault: the record's car tuning +148
  uint32_t fault_record_tuning_148() const { return guest::LoadU32(base, address + 1844); }
  void set_fault_record_tuning_148(uint32_t v) const { guest::StoreU32(base, address + 1844, v); }
  uint32_t fault_record_tuning_148_address() const { return address + 1844; }
  // measure fault: the record's car tuning +184
  uint32_t fault_record_tuning_184() const { return guest::LoadU32(base, address + 1880); }
  void set_fault_record_tuning_184(uint32_t v) const { guest::StoreU32(base, address + 1880, v); }
  uint32_t fault_record_tuning_184_address() const { return address + 1880; }
  // measure fault: a racer record's byte +2019 (sub_8210EBF0's horn scale when 1)
  uint8_t fault_record_byte_2019() const { return guest::LoadU8(base, address + 2019); }
  void set_fault_record_byte_2019(uint8_t v) const { guest::StoreU8(base, address + 2019, v); }
  uint32_t fault_record_byte_2019_address() const { return address + 2019; }
  // measure fault: a racer record's +3888 flags (0x1 and 0x80 the players, 0x2 rivals and opponents, 0x4 traffic, 0x100, 0x200, 0x400, 0x800)
  uint32_t fault_record_flags() const { return guest::LoadU32(base, address + 3888); }
  void set_fault_record_flags(uint32_t v) const { guest::StoreU32(base, address + 3888, v); }
  uint32_t fault_record_flags_address() const { return address + 3888; }
  // measure fault: a racer record's +3892 (RaceHud::Tick, Vehicle::SetUp)
  uint32_t fault_record_word_3892() const { return guest::LoadU32(base, address + 3892); }
  void set_fault_record_word_3892(uint32_t v) const { guest::StoreU32(base, address + 3892, v); }
  uint32_t fault_record_word_3892_address() const { return address + 3892; }
  // measure fault: a racer record's +3904 start position (section, slice) as a word (Racers::SetStart)
  uint32_t fault_record_start() const { return guest::LoadU32(base, address + 3904); }
  void set_fault_record_start(uint32_t v) const { guest::StoreU32(base, address + 3904, v); }
  uint32_t fault_record_start_address() const { return address + 3904; }
  // measure fault: the record's start section (RaceManager::AddRacers)
  uint16_t fault_record_start_section() const { return guest::LoadU16(base, address + 3904); }
  void set_fault_record_start_section(uint16_t v) const { guest::StoreU16(base, address + 3904, v); }
  uint32_t fault_record_start_section_address() const { return address + 3904; }
  // measure fault: the record's start slice
  uint16_t fault_record_start_slice() const { return guest::LoadU16(base, address + 3906); }
  void set_fault_record_start_slice(uint16_t v) const { guest::StoreU16(base, address + 3906, v); }
  uint32_t fault_record_start_slice_address() const { return address + 3906; }
  // measure fault: the record's +3908 float ([0x8200692C], or 0.0 for stage types 2 and 3)
  float fault_record_start_value() const { return guest::LoadFloat(base, address + 3908); }
  void set_fault_record_start_value(float v) const { guest::StoreFloat(base, address + 3908, v); }
  uint32_t fault_record_start_value_address() const { return address + 3908; }
  // measure fault: the record's racer id (Racers::IndexOf)
  uint32_t fault_record_racer_id() const { return guest::LoadU32(base, address + 3912); }
  void set_fault_record_racer_id(uint32_t v) const { guest::StoreU32(base, address + 3912, v); }
  uint32_t fault_record_racer_id_address() const { return address + 3912; }
  // measure fault: the record's index in the event's list
  uint32_t fault_record_event_index() const { return guest::LoadU32(base, address + 3916); }
  void set_fault_record_event_index(uint32_t v) const { guest::StoreU32(base, address + 3916, v); }
  uint32_t fault_record_event_index_address() const { return address + 3916; }
  // racer record 0's flags (+3888; 0x1 the player, RaceStates::RacersNear)
  uint32_t record0_flags() const { return guest::LoadU32(base, address + 3936); }
  void set_record0_flags(uint32_t v) const { guest::StoreU32(base, address + 3936, v); }
  uint32_t record0_flags_address() const { return address + 3936; }
  // racer record 0's +3892 (Replay::Keep)
  uint32_t record0_word_3892() const { return guest::LoadU32(base, address + 3940); }
  void set_record0_word_3892(uint32_t v) const { guest::StoreU32(base, address + 3940, v); }
  uint32_t record0_word_3892_address() const { return address + 3940; }
  // racer record 0's start section (Replay::Keep, SetUpRace)
  uint16_t record0_start_section() const { return guest::LoadU16(base, address + 3952); }
  void set_record0_start_section(uint16_t v) const { guest::StoreU16(base, address + 3952, v); }
  uint32_t record0_start_section_address() const { return address + 3952; }
  // racer record 0's start slice
  uint16_t record0_start_slice() const { return guest::LoadU16(base, address + 3954); }
  void set_record0_start_slice(uint16_t v) const { guest::StoreU16(base, address + 3954, v); }
  uint32_t record0_start_slice_address() const { return address + 3954; }
  // racer record 0's +3908 float
  float record0_start_value() const { return guest::LoadFloat(base, address + 3956); }
  void set_record0_start_value(float v) const { guest::StoreFloat(base, address + 3956, v); }
  uint32_t record0_start_value_address() const { return address + 3956; }
  // racer record 0's racer id (+3912)
  uint32_t record0_racer_id() const { return guest::LoadU32(base, address + 3960); }
  void set_record0_racer_id(uint32_t v) const { guest::StoreU32(base, address + 3960, v); }
  uint32_t record0_racer_id_address() const { return address + 3960; }
  // racer record 0's +3920 float (Racers::Add's f1: the garage car's +1892)
  float record0_car_value() const { return guest::LoadFloat(base, address + 3968); }
  void set_record0_car_value(float v) const { guest::StoreFloat(base, address + 3968, v); }
  uint32_t record0_car_value_address() const { return address + 3968; }
  // measure fault: racer record 1's start position as a word, the records met at +0 (RaceManager::AddRacers)
  uint32_t fault_record1_start() const { return guest::LoadU32(base, address + 7840); }
  void set_fault_record1_start(uint32_t v) const { guest::StoreU32(base, address + 7840, v); }
  uint32_t fault_record1_start_address() const { return address + 7840; }
  // measure fault: racer record 1's start section, the records met at +0
  uint16_t fault_record1_start_section() const { return guest::LoadU16(base, address + 7840); }
  void set_fault_record1_start_section(uint16_t v) const { guest::StoreU16(base, address + 7840, v); }
  uint32_t fault_record1_start_section_address() const { return address + 7840; }
  // measure fault: racer record 1's start slice
  uint16_t fault_record1_start_slice() const { return guest::LoadU16(base, address + 7842); }
  void set_fault_record1_start_slice(uint16_t v) const { guest::StoreU16(base, address + 7842, v); }
  uint32_t fault_record1_start_slice_address() const { return address + 7842; }
  // measure fault: probably a record list's word met at +0 (sub_821059D0, not read)
  uint32_t fault_records_word_303072() const { return guest::LoadU32(base, address + 303072); }
  void set_fault_records_word_303072(uint32_t v) const { guest::StoreU32(base, address + 303072, v); }
  uint32_t fault_records_word_303072_address() const { return address + 303072; }
  // measure fault: the racers object's (+48) car count, +0x59610 here (Racers::Add, SetStart)
  uint32_t fault_car_count() const { return guest::LoadU32(base, address + 366048); }
  void set_fault_car_count(uint32_t v) const { guest::StoreU32(base, address + 366048, v); }
  uint32_t fault_car_count_address() const { return address + 366048; }
  // +0x59610 the car count
  uint32_t car_count() const { return guest::LoadU32(base, address + 366096); }
  void set_car_count(uint32_t v) const { guest::StoreU32(base, address + 366096, v); }
  uint32_t car_count_address() const { return address + 366096; }
  // +0x59614 the racers Race::AddRacer added (also tested for 2 with the race kind 6)
  uint32_t added_count() const { return guest::LoadU32(base, address + 366100); }
  void set_added_count(uint32_t v) const { guest::StoreU32(base, address + 366100, v); }
  uint32_t added_count_address() const { return address + 366100; }
  // +0x59618: nonzero adds the 16 traffic cars (2 for mode 1, 1 for mode 2, else 3)
  uint32_t word_59618() const { return guest::LoadU32(base, address + 366104); }
  void set_word_59618(uint32_t v) const { guest::StoreU32(base, address + 366104, v); }
  uint32_t word_59618_address() const { return address + 366104; }
  // +0x5961C: 5 at the set-up
  uint32_t word_5961c() const { return guest::LoadU32(base, address + 366108); }
  void set_word_5961c(uint32_t v) const { guest::StoreU32(base, address + 366108, v); }
  uint32_t word_5961c_address() const { return address + 366108; }
  // +0x59620 the stage list (Stages::; its course record at +812, 408 bytes)
  uint32_t stages_address() const { return address + 366112; }
  // the stage list's +800: the stage count (Race::AddStage)
  uint32_t stage_count() const { return guest::LoadU32(base, address + 366912); }
  void set_stage_count(uint32_t v) const { guest::StoreU32(base, address + 366912, v); }
  uint32_t stage_count_address() const { return address + 366912; }
  // the stage list's +804: the current stage
  uint32_t current_stage() const { return guest::LoadU32(base, address + 366916); }
  void set_current_stage(uint32_t v) const { guest::StoreU32(base, address + 366916, v); }
  uint32_t current_stage_address() const { return address + 366916; }
  // the stage list's course record (+812)'s count (+48; CourseTimeList)
  uint16_t stages_course_record_count() const { return guest::LoadU16(base, address + 366972); }
  void set_stages_course_record_count(uint16_t v) const { guest::StoreU16(base, address + 366972, v); }
  uint32_t stages_course_record_count_address() const { return address + 366972; }
  // +0x59AE4: 0x1 the racers settled, 0x8 an event race, 0x10 a rival race, 0x20 the human cars' input recorded, 0x40 replayed, 0x80 a replay, 0x100 frozen, 0x800, 0x1000, 0x2000 no traffic, 0x4000 traffic removed, 0x8000 to the title, 0x10000 to type 12, 0x20000 a replay ran out
  uint32_t flags() const { return guest::LoadU32(base, address + 367332); }
  void set_flags(uint32_t v) const { guest::StoreU32(base, address + 367332, v); }
  uint32_t flags_address() const { return address + 367332; }
  // +0x59AE8: 77 results records of 3,936 bytes (RacerResultsRecord; RacerRecords::Add, At)
  uint32_t results_address() const { return address + 367336; }
  // +0xA3AC8: the results records' count
  uint32_t results_count() const { return guest::LoadU32(base, address + 670408); }
  void set_results_count(uint32_t v) const { guest::StoreU32(base, address + 670408, v); }
  uint32_t results_count_address() const { return address + 670408; }
  // +0xA3AD0: 1 at the reset; added to the stats' +5956 after a race (Progress::AfterRace); RaceStates::LinkTick
  uint32_t stat_0() const { return guest::LoadU32(base, address + 670416); }
  void set_stat_0(uint32_t v) const { guest::StoreU32(base, address + 670416, v); }
  uint32_t stat_0_address() const { return address + 670416; }
  // +0xA3AD4: added to the stats' +5952 (Progress::AfterRace); RaceStates::LinkTick
  uint32_t stat_1() const { return guest::LoadU32(base, address + 670420); }
  void set_stat_1(uint32_t v) const { guest::StoreU32(base, address + 670420, v); }
  uint32_t stat_1_address() const { return address + 670420; }
  // +0xA3AD8: raised by PlayerControl::Update (player 0 letting go of 0x20000, kind 1); added to the stats' +5960
  uint32_t finish_count() const { return guest::LoadU32(base, address + 670424); }
  void set_finish_count(uint32_t v) const { guest::StoreU32(base, address + 670424, v); }
  uint32_t finish_count_address() const { return address + 670424; }
  // +0xA3ADC: added to the stats' +5964 (divided by 50 first)
  uint32_t stat_3() const { return guest::LoadU32(base, address + 670428); }
  void set_stat_3(uint32_t v) const { guest::StoreU32(base, address + 670428, v); }
  uint32_t stat_3_address() const { return address + 670428; }
  // +0xA3AE0: rand() % 10 at the set-up
  uint32_t random_10() const { return guest::LoadU32(base, address + 670432); }
  void set_random_10(uint32_t v) const { guest::StoreU32(base, address + 670432, v); }
  uint32_t random_10_address() const { return address + 670432; }
  // +0xA3AE4: kept with a replay (Replay::Keep, SetUpRace)
  uint32_t replay_word() const { return guest::LoadU32(base, address + 670436); }
  void set_replay_word(uint32_t v) const { guest::StoreU32(base, address + 670436, v); }
  uint32_t replay_word_address() const { return address + 670436; }
  // +0xA3AE8: 0 at the reset
  uint32_t word_a3ae8() const { return guest::LoadU32(base, address + 670440); }
  void set_word_a3ae8(uint32_t v) const { guest::StoreU32(base, address + 670440, v); }
  uint32_t word_a3ae8_address() const { return address + 670440; }
  // +0xA3AEC: 0 at the reset
  uint32_t word_a3aec() const { return guest::LoadU32(base, address + 670444); }
  void set_word_a3aec(uint32_t v) const { guest::StoreU32(base, address + 670444, v); }
  uint32_t word_a3aec_address() const { return address + 670444; }
  // +0xA3AF0: the kind of race (0 the attract race's demo, 1 the career, 2 type 6, 3 type 7, 4 type 8, 5 the replay theater, 6 two human cars; 7 at the reset)
  uint32_t kind() const { return guest::LoadU32(base, address + 670448); }
  void set_kind(uint32_t v) const { guest::StoreU32(base, address + 670448, v); }
  uint32_t kind_address() const { return address + 670448; }
  // +0xA3AF4: probably the race mode (2 at the set-up, 3 a rival race of kind 4 or 5; 0 or 4 a course record)
  uint32_t race_mode() const { return guest::LoadU32(base, address + 670452); }
  void set_race_mode(uint32_t v) const { guest::StoreU32(base, address + 670452, v); }
  uint32_t race_mode_address() const { return address + 670452; }
};

// embedded; measure: RaceStates, sub_82167D88.r3, sub_82168018.r3, sub_821683A8.r3, sub_82168620.r3
struct RaceStates : View {
  using View::View;
  static constexpr uint32_t kSize = 9644;
  // 200 transitions, 48 bytes each (StateTransition, game-modes.toml); 87 added by RaceStates::Init
  uint32_t transitions_address() const { return address + 0; }
  // measure fault: RaceStates::RacersStill's count word (its r3 is the machine's +9628, not the machine)
  uint32_t fault_still_count() const { return guest::LoadU32(base, address + 0); }
  void set_fault_still_count(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t fault_still_count_address() const { return address + 0; }
  // measure fault: SceneModelRecord::FromCar's halfword +4
  uint16_t fault_record_word_4() const { return guest::LoadU16(base, address + 4); }
  void set_fault_record_word_4(uint16_t v) const { guest::StoreU16(base, address + 4, v); }
  uint32_t fault_record_word_4_address() const { return address + 4; }
  // measure fault: EventCondition::CarIs's +12
  uint32_t fault_car_word_12() const { return guest::LoadU32(base, address + 12); }
  void set_fault_car_word_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t fault_car_word_12_address() const { return address + 12; }
  // measure fault: SceneModelRecord::SetWheels's +1692
  uint32_t fault_wheels_1692() const { return guest::LoadU32(base, address + 1692); }
  void set_fault_wheels_1692(uint32_t v) const { guest::StoreU32(base, address + 1692, v); }
  uint32_t fault_wheels_1692_address() const { return address + 1692; }
  // measure fault: SceneModelRecord::SetWheels's +1696
  uint32_t fault_wheels_1696() const { return guest::LoadU32(base, address + 1696); }
  void set_fault_wheels_1696(uint32_t v) const { guest::StoreU32(base, address + 1696, v); }
  uint32_t fault_wheels_1696_address() const { return address + 1696; }
  uint32_t transition_count() const { return guest::LoadU32(base, address + 9600); }
  void set_transition_count(uint32_t v) const { guest::StoreU32(base, address + 9600, v); }
  uint32_t transition_count_address() const { return address + 9600; }
  // 1 at set-up; 3 waiting for the load, 16 a stage loading, 10 to 12 and 21 the starts, 22 the countdown, 23 racing, 25 paused, 26 the end, 17 and 18 the replay, 30 the menu after a race, 36, 37, 35 the end
  uint32_t state() const { return guest::LoadU32(base, address + 9604); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 9604, v); }
  uint32_t state_address() const { return address + 9604; }
  // 0 or 1 by the pause menu's two quits (OnQuit, OnQuit2)
  uint32_t pause_quit() const { return guest::LoadU32(base, address + 9608); }
  void set_pause_quit(uint32_t v) const { guest::StoreU32(base, address + 9608, v); }
  uint32_t pause_quit_address() const { return address + 9608; }
  // a step within a state
  uint32_t step() const { return guest::LoadU32(base, address + 9612); }
  void set_step(uint32_t v) const { guest::StoreU32(base, address + 9612, v); }
  uint32_t step_address() const { return address + 9612; }
  // a tick count
  uint32_t ticks() const { return guest::LoadU32(base, address + 9616); }
  void set_ticks(uint32_t v) const { guest::StoreU32(base, address + 9616, v); }
  uint32_t ticks_address() const { return address + 9616; }
  // cleared with the tick count; never read
  uint32_t word_9620() const { return guest::LoadU32(base, address + 9620); }
  void set_word_9620(uint32_t v) const { guest::StoreU32(base, address + 9620, v); }
  uint32_t word_9620_address() const { return address + 9620; }
  // [0x826D82E8] (RaceStates::Init)
  template <typename T = RaceProgress>
  T progress() const { return T(base, guest::LoadU32(base, address + 9624)); }
  void set_progress(const View& v) const { guest::StoreU32(base, address + 9624, v.address); }
  void set_progress(uint32_t v) const { guest::StoreU32(base, address + 9624, v); }
  uint32_t progress_address() const { return address + 9624; }
  // ticks with no racer touching anything (to 30, RaceStates::RacersStill)
  uint32_t still_ticks() const { return guest::LoadU32(base, address + 9628); }
  void set_still_ticks(uint32_t v) const { guest::StoreU32(base, address + 9628, v); }
  uint32_t still_ticks_address() const { return address + 9628; }
  // cleared with the still count
  uint32_t word_9632() const { return guest::LoadU32(base, address + 9632); }
  void set_word_9632(uint32_t v) const { guest::StoreU32(base, address + 9632, v); }
  uint32_t word_9632_address() const { return address + 9632; }
  // the racers settled (StandingStartTick)
  uint32_t settled() const { return guest::LoadU32(base, address + 9636); }
  void set_settled(uint32_t v) const { guest::StoreU32(base, address + 9636, v); }
  uint32_t settled_address() const { return address + 9636; }
  // ticks since the racers settled
  uint32_t settled_ticks() const { return guest::LoadU32(base, address + 9640); }
  void set_settled_ticks(uint32_t v) const { guest::StoreU32(base, address + 9640, v); }
  uint32_t settled_ticks_address() const { return address + 9640; }
};

// embedded; measure: RaceStates::AbortTick.r4, RaceStates::AfterRaceTick.r4, RaceStates::AfterReplayTick.r4, RaceStates::CountdownTick.r4, RaceStates::DemoTick.r4, RaceStates::EndWhenFaded.r4, RaceStates::FreeStartTick.r4, RaceStates::LinkTick.r4, RaceStates::MenuLoadTick.r4, RaceStates::OnContinue.r4, RaceStates::OnWorldLoaded.r4, RaceStates::QuitTick.r4, RaceStates::RaceTick.r4, RaceStates::ReplayLoadTick.r4, RaceStates::ReplaySetUpTick.r4, RaceStates::RollingLoadTick.r4, RaceStates::RollingStartTick.r4, RaceStates::StageLoadTick.r4, RaceStates::StandingLoadTick.r4, RaceStates::StandingStartTick.r4, sub_82167D88.r4, sub_82168018.r4, sub_821683A8.r4, sub_82168620.r4, sub_82168858.r4, sub_82168C80.r4
struct RaceStatesDispatcher : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  // 50 handler pointers (state machines)
  uint32_t handlers_address() const { return address + 0; }
  uint32_t handler_count() const { return guest::LoadU32(base, address + 200); }
  void set_handler_count(uint32_t v) const { guest::StoreU32(base, address + 200, v); }
  uint32_t handler_count_address() const { return address + 200; }
  // 50 messages of 12 bytes (ModeMessage: +0 the message, +4 and +8 0)
  uint32_t ring_address() const { return address + 204; }
  uint32_t ring_count() const { return guest::LoadU32(base, address + 804); }
  void set_ring_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t ring_count_address() const { return address + 804; }
  uint32_t ring_start() const { return guest::LoadU32(base, address + 808); }
  void set_ring_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t ring_start_address() const { return address + 808; }
  // measure fault: RollingLoadTick's and sub_82168018's r4 +1008 (the race mode's +1044, in the course set-up machine's table): another object kept in the register, not read further
  uint32_t fault_word_1008() const { return guest::LoadU32(base, address + 1008); }
  void set_fault_word_1008(uint32_t v) const { guest::StoreU32(base, address + 1008, v); }
  uint32_t fault_word_1008_address() const { return address + 1008; }
  // measure fault: as +1008
  uint32_t fault_word_1012() const { return guest::LoadU32(base, address + 1012); }
  void set_fault_word_1012(uint32_t v) const { guest::StoreU32(base, address + 1012, v); }
  uint32_t fault_word_1012_address() const { return address + 1012; }
};

// allocated; measure: RaceProgress, Achievements, sub_82167D88.r3.+9624
struct RaceProgress : View {
  using View::View;
  static constexpr uint32_t kSize = 7248;
  // ten racer record indexes (-1 none; racers whose item's byte +35 is set, RaceStates::SetUpStage)
  uint32_t racer_records_address() const { return address + 0; }
  // the first (also the checks' out pair's end, a measure fault: RaceCheckResult)
  uint32_t racer_record_0() const { return guest::LoadU32(base, address + 0); }
  void set_racer_record_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t racer_record_0_address() const { return address + 0; }
  // measure fault: TrackRoute::PathDistance's position's slice
  uint16_t fault_position_slice() const { return guest::LoadU16(base, address + 2); }
  void set_fault_position_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t fault_position_slice_address() const { return address + 2; }
  // the second (also the checks' out pair's reason, a measure fault)
  uint32_t racer_record_1() const { return guest::LoadU32(base, address + 4); }
  void set_racer_record_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t racer_record_1_address() const { return address + 4; }
  // four racer slots of 536 bytes (RaceProgressSlot)
  uint32_t slots_address() const { return address + 40; }
  // slot 0's +0 (0 at the reset; probably a 15-tick hold after hits)
  uint32_t slot0_hold_0() const { return guest::LoadU32(base, address + 40); }
  void set_slot0_hold_0(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t slot0_hold_0_address() const { return address + 40; }
  // slot 0's +4 (as +0)
  uint32_t slot0_hold_1() const { return guest::LoadU32(base, address + 44); }
  void set_slot0_hold_1(uint32_t v) const { guest::StoreU32(base, address + 44, v); }
  uint32_t slot0_hold_1_address() const { return address + 44; }
  // slot 0's manager entry (the racers' manager [0x826D82F4]'s)
  template <typename T = MinimapRacerEntry>
  T slot0_entry() const { return T(base, guest::LoadU32(base, address + 48)); }
  void set_slot0_entry(const View& v) const { guest::StoreU32(base, address + 48, v.address); }
  void set_slot0_entry(uint32_t v) const { guest::StoreU32(base, address + 48, v); }
  uint32_t slot0_entry_address() const { return address + 48; }
  // slot 0's route sections passed (256 halfwords)
  uint32_t slot0_sections_address() const { return address + 52; }
  uint32_t slot0_section_count() const { return guest::LoadU32(base, address + 564); }
  void set_slot0_section_count(uint32_t v) const { guest::StoreU32(base, address + 564, v); }
  uint32_t slot0_section_count_address() const { return address + 564; }
  // slot 0's stage slot word (a level, 0 to 4, scaling the SP drain; 2 at the reset)
  uint32_t slot0_level() const { return guest::LoadU32(base, address + 568); }
  void set_slot0_level(uint32_t v) const { guest::StoreU32(base, address + 568, v); }
  uint32_t slot0_level_address() const { return address + 568; }
  // slot 0's start (-1 at the reset)
  uint16_t slot0_start_section() const { return guest::LoadU16(base, address + 572); }
  void set_slot0_start_section(uint16_t v) const { guest::StoreU16(base, address + 572, v); }
  uint32_t slot0_start_section_address() const { return address + 572; }
  uint16_t slot0_start_slice() const { return guest::LoadU16(base, address + 574); }
  void set_slot0_start_slice(uint16_t v) const { guest::StoreU16(base, address + 574, v); }
  uint32_t slot0_start_slice_address() const { return address + 574; }
  // the results record (RacerResultsRecord, race-hud-machine.toml): its count, then 972-byte entries E at +2188 + 972i, one a racer
  uint32_t results_address() const { return address + 2184; }
  // the results' count
  uint32_t results_count() const { return guest::LoadU32(base, address + 2184); }
  void set_results_count(uint32_t v) const { guest::StoreU32(base, address + 2184, v); }
  uint32_t results_count_address() const { return address + 2184; }
  // entry 0's racer record index
  uint32_t e0_record_index() const { return guest::LoadU32(base, address + 2188); }
  void set_e0_record_index(uint32_t v) const { guest::StoreU32(base, address + 2188, v); }
  uint32_t e0_record_index_address() const { return address + 2188; }
  // entry 0's item
  uint32_t e0_item() const { return guest::LoadU32(base, address + 2192); }
  void set_e0_item(uint32_t v) const { guest::StoreU32(base, address + 2192, v); }
  uint32_t e0_item_address() const { return address + 2192; }
  // entry 0's place (0 the leader)
  uint32_t e0_place() const { return guest::LoadU32(base, address + 2196); }
  void set_e0_place(uint32_t v) const { guest::StoreU32(base, address + 2196, v); }
  uint32_t e0_place_address() const { return address + 2196; }
  // entry 0's ticks raced
  uint32_t e0_ticks() const { return guest::LoadU32(base, address + 2200); }
  void set_e0_ticks(uint32_t v) const { guest::StoreU32(base, address + 2200, v); }
  uint32_t e0_ticks_address() const { return address + 2200; }
  // entry 0's time (RaceTime::FromTicks, a packed time)
  uint32_t e0_time() const { return guest::LoadU32(base, address + 2204); }
  void set_e0_time(uint32_t v) const { guest::StoreU32(base, address + 2204, v); }
  uint32_t e0_time_address() const { return address + 2204; }
  // entry 0's +84: its first lap record's +64 (RaceStates::OnRaceEnd)
  uint32_t e0_lap_record_64() const { return guest::LoadU32(base, address + 2272); }
  void set_e0_lap_record_64(uint32_t v) const { guest::StoreU32(base, address + 2272, v); }
  uint32_t e0_lap_record_64_address() const { return address + 2272; }
  // entry 0's SP (+784; probably spirit points, 0 is out)
  uint32_t e0_sp() const { return guest::LoadU32(base, address + 2972); }
  void set_e0_sp(uint32_t v) const { guest::StoreU32(base, address + 2972, v); }
  uint32_t e0_sp_address() const { return address + 2972; }
  // entry 0's starting SP (+788)
  uint32_t e0_start_sp() const { return guest::LoadU32(base, address + 2976); }
  void set_e0_start_sp(uint32_t v) const { guest::StoreU32(base, address + 2976, v); }
  uint32_t e0_start_sp_address() const { return address + 2976; }
  // entry 0's running mean of the speed in km/h (+792; [0x82000A34] at the creation)
  float e0_mean_speed() const { return guest::LoadFloat(base, address + 2980); }
  void set_e0_mean_speed(float v) const { guest::StoreFloat(base, address + 2980, v); }
  uint32_t e0_mean_speed_address() const { return address + 2980; }
  // entry 0's running mean's count (+796; 0 at the creation)
  uint32_t e0_mean_count() const { return guest::LoadU32(base, address + 2984); }
  void set_e0_mean_count(uint32_t v) const { guest::StoreU32(base, address + 2984, v); }
  uint32_t e0_mean_count_address() const { return address + 2984; }
  // entry 0's track hits (+808)
  uint32_t e0_track_hits() const { return guest::LoadU32(base, address + 2996); }
  void set_e0_track_hits(uint32_t v) const { guest::StoreU32(base, address + 2996, v); }
  uint32_t e0_track_hits_address() const { return address + 2996; }
  // entry 0's car hits (+812)
  uint32_t e0_car_hits() const { return guest::LoadU32(base, address + 3000); }
  void set_e0_car_hits(uint32_t v) const { guest::StoreU32(base, address + 3000, v); }
  uint32_t e0_car_hits_address() const { return address + 3000; }
  // entry 0's distance along the stage's route (+824, the place's key)
  float e0_route_distance() const { return guest::LoadFloat(base, address + 3012); }
  void set_e0_route_distance(float v) const { guest::StoreFloat(base, address + 3012, v); }
  uint32_t e0_route_distance_address() const { return address + 3012; }
  // entry 0's lap record index (+832)
  uint32_t e0_lap_record() const { return guest::LoadU32(base, address + 3020); }
  void set_e0_lap_record(uint32_t v) const { guest::StoreU32(base, address + 3020, v); }
  uint32_t e0_lap_record_address() const { return address + 3020; }
  // entry 1's distance along the route (RaceManager::StreamTwoPlayers)
  float e1_route_distance() const { return guest::LoadFloat(base, address + 3984); }
  void set_e1_route_distance(float v) const { guest::StoreFloat(base, address + 3984, v); }
  uint32_t e1_route_distance_address() const { return address + 3984; }
  // the stage's route (16 bytes from the stage's +24)
  uint32_t route_address() const { return address + 6076; }
  // the route's course route index
  uint32_t route_course() const { return guest::LoadU32(base, address + 6076); }
  void set_route_course(uint32_t v) const { guest::StoreU32(base, address + 6076, v); }
  uint32_t route_course_address() const { return address + 6076; }
  uint32_t route_location() const { return guest::LoadU32(base, address + 6080); }
  void set_route_location(uint32_t v) const { guest::StoreU32(base, address + 6080, v); }
  uint32_t route_location_address() const { return address + 6080; }
  uint32_t route_first_position() const { return guest::LoadU32(base, address + 6084); }
  void set_route_first_position(uint32_t v) const { guest::StoreU32(base, address + 6084, v); }
  uint32_t route_first_position_address() const { return address + 6084; }
  // the finish position (section, slice)
  uint32_t route_finish() const { return guest::LoadU32(base, address + 6088); }
  void set_route_finish(uint32_t v) const { guest::StoreU32(base, address + 6088, v); }
  uint32_t route_finish_address() const { return address + 6088; }
  // the finish's section (RaceProgress::CheckFinish)
  uint16_t route_finish_section() const { return guest::LoadU16(base, address + 6088); }
  void set_route_finish_section(uint16_t v) const { guest::StoreU16(base, address + 6088, v); }
  uint32_t route_finish_section_address() const { return address + 6088; }
  // when set, an entry's lap records are indexed by its +832
  uint32_t lap_records_on() const { return guest::LoadU32(base, address + 6096); }
  void set_lap_records_on(uint32_t v) const { guest::StoreU32(base, address + 6096, v); }
  uint32_t lap_records_on_address() const { return address + 6096; }
  // the stage's type (RaceProgress::SetChecks; 1 or 3 also tested with the race mode 5)
  uint32_t stage_type() const { return guest::LoadU32(base, address + 6100); }
  void set_stage_type(uint32_t v) const { guest::StoreU32(base, address + 6100, v); }
  uint32_t stage_type_address() const { return address + 6100; }
  // the race's end (3 retired)
  uint32_t end() const { return guest::LoadU32(base, address + 6104); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 6104, v); }
  uint32_t end_address() const { return address + 6104; }
  // the end's reason (1 or 2 by the pause menu's quit)
  uint32_t end_reason() const { return guest::LoadU32(base, address + 6108); }
  void set_end_reason(uint32_t v) const { guest::StoreU32(base, address + 6108, v); }
  uint32_t end_reason_address() const { return address + 6108; }
  // the player's starting SP
  uint32_t player_start_sp() const { return guest::LoadU32(base, address + 6112); }
  void set_player_start_sp(uint32_t v) const { guest::StoreU32(base, address + 6112, v); }
  uint32_t player_start_sp_address() const { return address + 6112; }
  // the race's tick count (shown as a time by RaceTime::FromTicks)
  uint32_t ticks() const { return guest::LoadU32(base, address + 6116); }
  void set_ticks(uint32_t v) const { guest::StoreU32(base, address + 6116, v); }
  uint32_t ticks_address() const { return address + 6116; }
  // the route sections passed by the race (256 halfwords, -1 at the reset)
  uint32_t sections_passed_address() const { return address + 6120; }
  uint32_t sections_passed_count() const { return guest::LoadU32(base, address + 6632); }
  void set_sections_passed_count(uint32_t v) const { guest::StoreU32(base, address + 6632, v); }
  uint32_t sections_passed_count_address() const { return address + 6632; }
  // the stage's path (256 halfwords, -1 at the reset; RaceProgress::RouteDistance)
  uint32_t path_address() const { return address + 6636; }
  uint32_t path_count() const { return guest::LoadU32(base, address + 7148); }
  void set_path_count(uint32_t v) const { guest::StoreU32(base, address + 7148, v); }
  uint32_t path_count_address() const { return address + 7148; }
  // the start (section, slice) as a word
  uint32_t start() const { return guest::LoadU32(base, address + 7152); }
  void set_start(uint32_t v) const { guest::StoreU32(base, address + 7152, v); }
  uint32_t start_address() const { return address + 7152; }
  uint16_t start_section() const { return guest::LoadU16(base, address + 7152); }
  void set_start_section(uint16_t v) const { guest::StoreU16(base, address + 7152, v); }
  uint32_t start_section_address() const { return address + 7152; }
  uint16_t start_slice() const { return guest::LoadU16(base, address + 7154); }
  void set_start_slice(uint16_t v) const { guest::StoreU16(base, address + 7154, v); }
  uint32_t start_slice_address() const { return address + 7154; }
  // the finish distance ([0x82000A34] at the reset)
  float race_length() const { return guest::LoadFloat(base, address + 7156); }
  void set_race_length(float v) const { guest::StoreFloat(base, address + 7156, v); }
  uint32_t race_length_address() const { return address + 7156; }
  // 15 lap distances (floats; 0 at the reset)
  uint32_t lap_distances_address() const { return address + 7160; }
  float lap_distance_0() const { return guest::LoadFloat(base, address + 7160); }
  void set_lap_distance_0(float v) const { guest::StoreFloat(base, address + 7160, v); }
  uint32_t lap_distance_0_address() const { return address + 7160; }
  float lap_distance_1() const { return guest::LoadFloat(base, address + 7164); }
  void set_lap_distance_1(float v) const { guest::StoreFloat(base, address + 7164, v); }
  uint32_t lap_distance_1_address() const { return address + 7164; }
  // which race checks RaceStates::RaceTick runs (0x11 for stage types 1 and 3, 0x3 for 0 and 2, | 0x8 for kind 1 with the word; 0x8 sub_821653E8)
  uint32_t race_checks() const { return guest::LoadU32(base, address + 7220); }
  void set_race_checks(uint32_t v) const { guest::StoreU32(base, address + 7220, v); }
  uint32_t race_checks_address() const { return address + 7220; }
  // which checks run for each entry (0x417 for 1 and 3, 0x481 for 0 and 2; 0x4 RaceProgress::CheckLap)
  uint32_t entry_checks() const { return guest::LoadU32(base, address + 7224); }
  void set_entry_checks(uint32_t v) const { guest::StoreU32(base, address + 7224, v); }
  uint32_t entry_checks_address() const { return address + 7224; }
  // 0 at the reset
  uint32_t word_7228() const { return guest::LoadU32(base, address + 7228); }
  void set_word_7228(uint32_t v) const { guest::StoreU32(base, address + 7228, v); }
  uint32_t word_7228_address() const { return address + 7228; }
  // a hit cost factor (1.0 at the reset; RaceProgress::Damage)
  float track_hit_cost() const { return guest::LoadFloat(base, address + 7232); }
  void set_track_hit_cost(float v) const { guest::StoreFloat(base, address + 7232, v); }
  uint32_t track_hit_cost_address() const { return address + 7232; }
  // a hit cost factor (1.0 at the reset; RaceProgress::Damage)
  float car_hit_cost() const { return guest::LoadFloat(base, address + 7236); }
  void set_car_hit_cost(float v) const { guest::StoreFloat(base, address + 7236, v); }
  uint32_t car_hit_cost_address() const { return address + 7236; }
  // -1 at the reset; the race HUD's item shown (RaceHud::UpdateItem), set by RaceStates::LinkTick
  uint32_t item_shown() const { return guest::LoadU32(base, address + 7240); }
  void set_item_shown(uint32_t v) const { guest::StoreU32(base, address + 7240, v); }
  uint32_t item_shown_address() const { return address + 7240; }
  // a position (kind 8; SplitResults::Draw)
  uint32_t kind8_position() const { return guest::LoadU32(base, address + 7244); }
  void set_kind8_position(uint32_t v) const { guest::StoreU32(base, address + 7244, v); }
  uint32_t kind8_position_address() const { return address + 7244; }
  // its section (RaceStates::OnRaceEnd)
  uint16_t kind8_section() const { return guest::LoadU16(base, address + 7244); }
  void set_kind8_section(uint16_t v) const { guest::StoreU16(base, address + 7244, v); }
  uint32_t kind8_section_address() const { return address + 7244; }
  uint16_t kind8_slice() const { return guest::LoadU16(base, address + 7246); }
  void set_kind8_slice(uint16_t v) const { guest::StoreU16(base, address + 7246, v); }
  uint32_t kind8_slice_address() const { return address + 7246; }
  // measure fault: RaceProgress::StartRoute's machine +9624 (the progress)
  uint32_t fault_machine_progress() const { return guest::LoadU32(base, address + 9624); }
  void set_fault_machine_progress(uint32_t v) const { guest::StoreU32(base, address + 9624, v); }
  uint32_t fault_machine_progress_address() const { return address + 9624; }
};

// embedded; measure: -
struct RaceProgressSlot : View {
  using View::View;
  static constexpr uint32_t kSize = 536;
  // 0 at the reset (probably a 15-tick hold after hits)
  uint32_t hold_0() const { return guest::LoadU32(base, address + 0); }
  void set_hold_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t hold_0_address() const { return address + 0; }
  // 0 at the reset (as +0)
  uint32_t hold_1() const { return guest::LoadU32(base, address + 4); }
  void set_hold_1(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t hold_1_address() const { return address + 4; }
  // its manager entry (0 none)
  template <typename T = MinimapRacerEntry>
  T entry() const { return T(base, guest::LoadU32(base, address + 8)); }
  void set_entry(const View& v) const { guest::StoreU32(base, address + 8, v.address); }
  void set_entry(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t entry_address() const { return address + 8; }
  // the route sections passed (256 halfwords)
  uint32_t sections_address() const { return address + 12; }
  // at most 256
  uint32_t section_count() const { return guest::LoadU32(base, address + 524); }
  void set_section_count(uint32_t v) const { guest::StoreU32(base, address + 524, v); }
  uint32_t section_count_address() const { return address + 524; }
  // the stage's slot word (a level, 0 to 4, scaling the SP drain; 2 at the reset)
  uint32_t level() const { return guest::LoadU32(base, address + 528); }
  void set_level(uint32_t v) const { guest::StoreU32(base, address + 528, v); }
  uint32_t level_address() const { return address + 528; }
  // -1 at the reset
  uint16_t start_section() const { return guest::LoadU16(base, address + 532); }
  void set_start_section(uint16_t v) const { guest::StoreU16(base, address + 532, v); }
  uint32_t start_section_address() const { return address + 532; }
  // -1 at the reset
  uint16_t start_slice() const { return guest::LoadU16(base, address + 534); }
  void set_start_slice(uint16_t v) const { guest::StoreU16(base, address + 534, v); }
  uint32_t start_slice_address() const { return address + 534; }
};

// embedded; measure: RaceProgress::CheckFinish.ret, RaceProgress::Damage.ret, RaceProgress::DrainSp.ret, RaceProgress::FollowRoute.ret, sub_821653E8.r3, sub_821653E8.ret
struct RaceCheckResult : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  // the two words read as one
  uint64_t pair() const { return guest::LoadU64(base, address + 0); }
  void set_pair(uint64_t v) const { guest::StoreU64(base, address + 0, v); }
  uint32_t pair_address() const { return address + 0; }
  // the race's end (0 none)
  uint32_t end() const { return guest::LoadU32(base, address + 0); }
  void set_end(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t end_address() const { return address + 0; }
  uint32_t reason() const { return guest::LoadU32(base, address + 4); }
  void set_reason(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t reason_address() const { return address + 4; }
};

// embedded; measure: Time, Time::Add.r4, Time::Sub.r4, sub_820FF680.r3, sub_820FF680.r4, RaceProgress::TickFraction.r5
struct PackedTime : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t packed() const { return guest::LoadU32(base, address + 0); }
  void set_packed(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t packed_address() const { return address + 0; }
};

// embedded; measure: RoutePosition::MapToSection.r4, RoutePosition::NotAfter.r4, sub_820FFE98.ret, sub_820FFF10.ret
struct RoutePosition : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // section and slice read or written as one word
  uint32_t word() const { return guest::LoadU32(base, address + 0); }
  void set_word(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word_address() const { return address + 0; }
  // signed
  uint16_t section() const { return guest::LoadU16(base, address + 0); }
  void set_section(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t section_address() const { return address + 0; }
  // signed
  uint16_t slice() const { return guest::LoadU16(base, address + 2); }
  void set_slice(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t slice_address() const { return address + 2; }
};

// embedded; measure: sub_82163000.r5, sub_82163000.r6
struct SectionsJoinLink : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // -1 none
  uint32_t link() const { return guest::LoadU32(base, address + 0); }
  void set_link(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t link_address() const { return address + 0; }
};

// embedded; measure: sub_820FFD48.ret
struct StageRoute : View {
  using View::View;
  static constexpr uint32_t kSize = 16;
  // a course route's index
  uint32_t course() const { return guest::LoadU32(base, address + 0); }
  void set_course(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t course_address() const { return address + 0; }
  uint32_t location() const { return guest::LoadU32(base, address + 4); }
  void set_location(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t location_address() const { return address + 4; }
  // section and slice
  uint32_t first_position() const { return guest::LoadU32(base, address + 8); }
  void set_first_position(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t first_position_address() const { return address + 8; }
  // section and slice (the finish)
  uint32_t last_position() const { return guest::LoadU32(base, address + 12); }
  void set_last_position(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t last_position_address() const { return address + 12; }
};

// embedded; measure: CourseEntry
struct CourseTimeEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 28;
  // 0 at the clear
  uint32_t word_0() const { return guest::LoadU32(base, address + 0); }
  void set_word_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t word_0_address() const { return address + 0; }
  // 0 at the clear
  uint32_t word_4() const { return guest::LoadU32(base, address + 4); }
  void set_word_4(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t word_4_address() const { return address + 4; }
  // 0 at the clear
  uint32_t word_8() const { return guest::LoadU32(base, address + 8); }
  void set_word_8(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t word_8_address() const { return address + 8; }
  // 0 at the clear
  uint32_t word_12() const { return guest::LoadU32(base, address + 12); }
  void set_word_12(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t word_12_address() const { return address + 12; }
  // 0 at the clear
  uint32_t word_16() const { return guest::LoadU32(base, address + 16); }
  void set_word_16(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t word_16_address() const { return address + 16; }
  // a packed time ([0x82006924] empty), added to time_b for the entry's total
  uint32_t time_a() const { return guest::LoadU32(base, address + 20); }
  void set_time_a(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t time_a_address() const { return address + 20; }
  // a packed time
  uint32_t time_b() const { return guest::LoadU32(base, address + 24); }
  void set_time_b(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t time_b_address() const { return address + 24; }
};

// embedded; measure: CourseRecord
struct CourseTimeList : View {
  using View::View;
  static constexpr uint32_t kSize = 408;
  // ten nodes of two signed halfwords: +0 the next node, +2 an entry index
  uint32_t nodes_address() const { return address + 0; }
  // CourseRecord::Reset
  uint16_t node0_next() const { return guest::LoadU16(base, address + 0); }
  void set_node0_next(uint16_t v) const { guest::StoreU16(base, address + 0, v); }
  uint32_t node0_next_address() const { return address + 0; }
  uint16_t node0_entry() const { return guest::LoadU16(base, address + 2); }
  void set_node0_entry(uint16_t v) const { guest::StoreU16(base, address + 2, v); }
  uint32_t node0_entry_address() const { return address + 2; }
  // the last node's next (CourseRecord::Reset)
  uint16_t node9_next() const { return guest::LoadU16(base, address + 36); }
  void set_node9_next(uint16_t v) const { guest::StoreU16(base, address + 36, v); }
  uint32_t node9_next_address() const { return address + 36; }
  // the free list's first node (signed)
  uint16_t free_first() const { return guest::LoadU16(base, address + 40); }
  void set_free_first(uint16_t v) const { guest::StoreU16(base, address + 40, v); }
  uint32_t free_first_address() const { return address + 40; }
  // set by CourseRecord::Reset
  uint16_t word_42() const { return guest::LoadU16(base, address + 42); }
  void set_word_42(uint16_t v) const { guest::StoreU16(base, address + 42, v); }
  uint32_t word_42_address() const { return address + 42; }
  // the list's first node (signed)
  uint16_t first() const { return guest::LoadU16(base, address + 44); }
  void set_first(uint16_t v) const { guest::StoreU16(base, address + 44, v); }
  uint32_t first_address() const { return address + 44; }
  // set by CourseRecord::Reset
  uint16_t word_46() const { return guest::LoadU16(base, address + 46); }
  void set_word_46(uint16_t v) const { guest::StoreU16(base, address + 46, v); }
  uint32_t word_46_address() const { return address + 46; }
  // the list's count
  uint16_t count() const { return guest::LoadU16(base, address + 48); }
  void set_count(uint16_t v) const { guest::StoreU16(base, address + 48, v); }
  uint32_t count_address() const { return address + 48; }
  // ten 28-byte entries (CourseTimeEntry)
  uint32_t entries_address() const { return address + 52; }
  // the best's details
  uint32_t best_address() const { return address + 332; }
};

// The image words docs/functions/race-states.md's code uses.
struct RaceStatesGlobals {
  // the race data
  template <typename T = RaceData>
  static T race_data(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826C5B2Cu)); }
  static void set_race_data(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826C5B2Cu, v.address); }
  static void set_race_data(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B2Cu, v); }
  static constexpr uint32_t race_data_address() { return 0x826C5B2Cu; }
  // the race's progress
  template <typename T = RaceProgress>
  static T progress(uint8_t* base) { return T(base, guest::LoadU32(base, 0x826D82E8u)); }
  static void set_progress(uint8_t* base, const View& v) { guest::StoreU32(base, 0x826D82E8u, v.address); }
  static void set_progress(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82E8u, v); }
  static constexpr uint32_t progress_address() { return 0x826D82E8u; }
  // the racers' manager (a pointer)
  static uint32_t racers_manager(uint8_t* base) { return guest::LoadU32(base, 0x826D82F4u); }
  static void set_racers_manager(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D82F4u, v); }
  static constexpr uint32_t racers_manager_address() { return 0x826D82F4u; }
  // the course's route file (a pointer; +12 its section table)
  static uint32_t route(uint8_t* base) { return guest::LoadU32(base, 0x826C5B10u); }
  static void set_route(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B10u, v); }
  static constexpr uint32_t route_address() { return 0x826C5B10u; }
  // the first of the cars' input records (pointers)
  static uint32_t input_records(uint8_t* base) { return guest::LoadU32(base, 0x826C5B30u); }
  static void set_input_records(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B30u, v); }
  static constexpr uint32_t input_records_address() { return 0x826C5B30u; }
  // the input recordings (a pointer)
  static uint32_t recordings(uint8_t* base) { return guest::LoadU32(base, 0x826C5B4Cu); }
  static void set_recordings(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B4Cu, v); }
  static constexpr uint32_t recordings_address() { return 0x826C5B4Cu; }
  // the kept replay (a pointer)
  static uint32_t replay(uint8_t* base) { return guest::LoadU32(base, 0x826C5B50u); }
  static void set_replay(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C5B50u, v); }
  static constexpr uint32_t replay_address() { return 0x826C5B50u; }
  // 0 by RaceStates::OnStandingStart
  static uint32_t settled_word(uint8_t* base) { return guest::LoadU32(base, 0x826C6B04u); }
  static void set_settled_word(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826C6B04u, v); }
  static constexpr uint32_t settled_word_address() { return 0x826C6B04u; }
  // the screen fades' alpha (255 black)
  static uint32_t fade_alpha(uint8_t* base) { return guest::LoadU32(base, 0x826A50D4u); }
  static void set_fade_alpha(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50D4u, v); }
  static constexpr uint32_t fade_alpha_address() { return 0x826A50D4u; }
  // the screen fades' in count (mode-tick.md)
  static uint32_t fade_in_count(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_fade_in_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t fade_in_count_address() { return 0x826A50DCu; }
  // the screen fades' out count
  static uint32_t fade_out_count(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_fade_out_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t fade_out_count_address() { return 0x826A50E4u; }
  // set to 1 by RaceStates::StandingStartTick's kind 6 case when 0
  static uint32_t sign_in_state(uint8_t* base) { return guest::LoadU32(base, 0x826BAB3Cu); }
  static void set_sign_in_state(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB3Cu, v); }
  static constexpr uint32_t sign_in_state_address() { return 0x826BAB3Cu; }
  // 6 with it
  static uint32_t sign_in_reason(uint8_t* base) { return guest::LoadU32(base, 0x826BAB5Cu); }
  static void set_sign_in_reason(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826BAB5Cu, v); }
  static constexpr uint32_t sign_in_reason_address() { return 0x826BAB5Cu; }
  // 1/30
  static float tick_seconds(uint8_t* base) { return guest::LoadFloat(base, 0x826193A4u); }
  static void set_tick_seconds(uint8_t* base, float v) { guest::StoreFloat(base, 0x826193A4u, v); }
  static constexpr uint32_t tick_seconds_address() { return 0x826193A4u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // 1000.0
  static float thousand(uint8_t* base) { return guest::LoadFloat(base, 0x82001BDCu); }
  static void set_thousand(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BDCu, v); }
  static constexpr uint32_t thousand_address() { return 0x82001BDCu; }
  // 5
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // 15
  static uint32_t fade_out_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fade_out_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fade_out_passes_address() { return 0x82001BF0u; }
  // 40.0
  static float wall_hit(uint8_t* base) { return guest::LoadFloat(base, 0x82001FB0u); }
  static void set_wall_hit(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FB0u, v); }
  static constexpr uint32_t wall_hit_address() { return 0x82001FB0u; }
  // 10.0
  static float car_hit(uint8_t* base) { return guest::LoadFloat(base, 0x82001FD8u); }
  static void set_car_hit(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FD8u, v); }
  static constexpr uint32_t car_hit_address() { return 0x82001FD8u; }
  // the SP drain's base
  static float drain_base(uint8_t* base) { return guest::LoadFloat(base, 0x820021E8u); }
  static void set_drain_base(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021E8u, v); }
  static constexpr uint32_t drain_base_address() { return 0x820021E8u; }
  // 50.0 (RaceStates::RacersClose; the gap's least)
  static float close_distance(uint8_t* base) { return guest::LoadFloat(base, 0x8200257Cu); }
  static void set_close_distance(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200257Cu, v); }
  static constexpr uint32_t close_distance_address() { return 0x8200257Cu; }
  // 0.001
  static float km(uint8_t* base) { return guest::LoadFloat(base, 0x820029D8u); }
  static void set_km(uint8_t* base, float v) { guest::StoreFloat(base, 0x820029D8u, v); }
  static constexpr uint32_t km_address() { return 0x820029D8u; }
  // the SP drain's scale
  static float drain_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002C6Cu); }
  static void set_drain_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C6Cu, v); }
  static constexpr uint32_t drain_scale_address() { return 0x82002C6Cu; }
  // a course entry's empty time
  static uint32_t no_time(uint8_t* base) { return guest::LoadU32(base, 0x82006924u); }
  static void set_no_time(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82006924u, v); }
  static constexpr uint32_t no_time_address() { return 0x82006924u; }
  // the SP drain's slope
  static float drain_slope(uint8_t* base) { return guest::LoadFloat(base, 0x82008540u); }
  static void set_drain_slope(uint8_t* base, float v) { guest::StoreFloat(base, 0x82008540u, v); }
  static constexpr uint32_t drain_slope_address() { return 0x82008540u; }
  // 200.0
  static float gap_most(uint8_t* base) { return guest::LoadFloat(base, 0x82009808u); }
  static void set_gap_most(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009808u, v); }
  static constexpr uint32_t gap_most_address() { return 0x82009808u; }
  // 230.0 (a hard hit)
  static float hit_strength(uint8_t* base) { return guest::LoadFloat(base, 0x82009814u); }
  static void set_hit_strength(uint8_t* base, float v) { guest::StoreFloat(base, 0x82009814u, v); }
  static constexpr uint32_t hit_strength_address() { return 0x82009814u; }
};

}  // namespace schema
