// The views of docs/functions/xact.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/xact.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// file; measure: XACTWaveEntry
struct XACTWaveEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  // 0 from the constructor, 1 once Set
  uint32_t valid() const { return guest::LoadU32(address + 0); }
  void set_valid(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t valid_address() const { return address + 0; }
  // the wave's index in its bank
  uint16_t index() const { return guest::LoadU16(address + 4); }
  void set_index(uint16_t v) const { guest::StoreU16(address + 4, v); }
  uint32_t index_address() const { return address + 4; }
  // the XACTWaveBank (vtable 0x82075AE8)
  uint32_t bank() const { return guest::LoadU32(address + 8); }
  void set_bank(uint32_t v) const { guest::StoreU32(address + 8, v); }
  uint32_t bank_address() const { return address + 8; }
  // the bank entry's first word: the duration in samples is >> 4
  uint32_t flags_duration() const { return guest::LoadU32(address + 12); }
  void set_flags_duration(uint32_t v) const { guest::StoreU32(address + 12, v); }
  uint32_t flags_duration_address() const { return address + 12; }
  // bits 0-1 the tag (0 PCM, 1 XMA), 2-4 the channels, 5-22 the rate, bit 31 16-bit samples
  uint32_t format() const { return guest::LoadU32(address + 16); }
  void set_format(uint32_t v) const { guest::StoreU32(address + 16, v); }
  uint32_t format_address() const { return address + 16; }
  // the play region's start, in bytes
  uint32_t play_offset() const { return guest::LoadU32(address + 20); }
  void set_play_offset(uint32_t v) const { guest::StoreU32(address + 20, v); }
  uint32_t play_offset_address() const { return address + 20; }
  // the play region's length, in bytes
  uint32_t play_length() const { return guest::LoadU32(address + 24); }
  void set_play_length(uint32_t v) const { guest::StoreU32(address + 24, v); }
  uint32_t play_length_address() const { return address + 24; }
  // the loop region's start, in samples
  uint32_t loop_start() const { return guest::LoadU32(address + 28); }
  void set_loop_start(uint32_t v) const { guest::StoreU32(address + 28, v); }
  uint32_t loop_start_address() const { return address + 28; }
  // the loop region's length, in samples; 0 for no loop
  uint32_t loop_length() const { return guest::LoadU32(address + 32); }
  void set_loop_length(uint32_t v) const { guest::StoreU32(address + 32, v); }
  uint32_t loop_length_address() const { return address + 32; }
};

// embedded; measure: SkeletonBase@+80
struct XACTWaveEntryAt80 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

}  // namespace schema
