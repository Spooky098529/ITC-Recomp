// The data load at boot natively (docs/functions/data-load.md): plain C++
// over the guest layout. The adapters are in src/hooks/data_load.cpp.

#pragma once

#include <cstdint>
#include <optional>

namespace data_load {

// .rdata's floats the constructors load with lfs and store with stfs.
constexpr uint32_t kOneFloat = 0x82000A30;   // 1.0
constexpr uint32_t kZeroFloat = 0x82000A34;  // 0.0

// --- The save defaults' bit sets ---

constexpr uint32_t kDefaultsBase = 0x826C4064;  // the last set, and Defaults::Init's r3
constexpr uint32_t kTable3044 = 0x826C3044;     // 15 words
constexpr uint32_t kSets3080 = 0x826C3080;      // 48 sets of 15 words, 60 bytes apart
constexpr uint32_t kTable3C24 = 0x826C3C24;     // 7 words
constexpr uint32_t kTable50A8 = 0x826C50A8;     // 15 words
constexpr uint32_t kTable50E4 = 0x826C50E4;     // 7 words
// Everything Defaults::Init writes: 0x826C3044 to 0x826C5100.
constexpr uint32_t kDefaultsFirst = kTable3044;
constexpr uint32_t kDefaultsBytes = kTable50E4 + 28 - kTable3044;

// sub_820CCD10 Bits224::Fill: the 7 words at `set` cleared, then bits 0-198
// set.
void Bits224Fill(uint8_t* base, uint32_t set);
// sub_820CCD68 Bits480::Fill: the 15 words at `set` cleared, then bits 0-453
// set.
void Bits480Fill(uint8_t* base, uint32_t set);
// sub_820CCDC0 Defaults::Init: the save defaults' bit sets filled (each
// cleared first, so what they hold after doesn't depend on before): the
// table at 0x826C3044, 0x826C3C24 (bits 0-60), 0x826C50E4 (Bits224::Fill),
// 0x826C50A8, the 48 sets at 0x826C3080 (a range each by their index, none
// for 29), and 0x826C4064 (cleared, ORed with sets 17, 35, 38, 41 and 47,
// Bits480::Fill, cleared again: it ends 0). `frame` is the original's
// 128-byte frame: its +80 to +96 get the five set indices (17, 35, 38, 41,
// 47), as the original leaves them. Returns 0x826C4064 (the original's r3).
uint32_t DefaultsInit(uint8_t* base, uint32_t frame);

// --- The 5,232-byte records ---

constexpr uint32_t kRecordBytes = 5232;

// sub_821042A0 Part464::Part464: 0.0 into its float fields. Returns `part`.
uint32_t Part464Construct(uint8_t* base, uint32_t part);
// sub_82104690 Member4080::Member4080: Part464 on +16 and +480, then 0.0
// into +956, +972, +1068, +1084, +1116 and +1148. Returns `member`.
uint32_t Member4080Construct(uint8_t* base, uint32_t member);
// sub_82102888 Member1136::Member1136: 0.0 into its float fields. Returns
// `member` (r3 as the original leaves it).
uint32_t Member1136Construct(uint8_t* base, uint32_t member);
// sub_82104C20 Record5232::Record5232: 1.0 into +156 to +204 and four rows'
// four words (+252 to +588), 0.0 into fourteen words from +636 to +1116,
// then Member1136 (+1136) and Member4080 (+4080). Returns `record`.
uint32_t Record5232Construct(uint8_t* base, uint32_t record);

// --- The session's objects ---

constexpr uint32_t kRaceDataBytes = 0xA3B00;
constexpr uint32_t kRaceDataSlot = 0x826C5B2C;

// sub_82102208 RaceData::Create: 0xA3B00 bytes (operator new's plain C++)
// and, when not null, RaceData::RaceData (race_setup's plain C++); the block,
// or 0, into 0x826C5B2C. Returns it (r3 as the original leaves it), or
// nullopt where operator new's plain C++ can't answer (the original runs;
// nothing written yet).
std::optional<uint32_t> RaceDataCreate(uint8_t* base);

constexpr uint32_t kInputRecordingsBytes = 0x2225C0;
constexpr uint32_t kInputRecordingsSlot = 0x826C5B4C;
constexpr uint32_t kReplayBytes = 0x13C8F0;
constexpr uint32_t kReplaySlot = 0x826C5B50;

// sub_821054A8 InputRecordings::InputRecordings: Record5232 on seven records
// from +0x20FE90 and seven from +0x2196A0. Returns `block`.
uint32_t InputRecordingsConstruct(uint8_t* base, uint32_t block);
// sub_82105508 InputRecordings::Create: 0x2225C0 bytes (operator new's plain
// C++), InputRecordings::InputRecordings when not null; the block, or 0, into
// 0x826C5B4C, then the word at block + 0x2225B0 = 0 (at 0x2225B0 when the
// allocation failed, as the original). Returns the block (r3), or nullopt
// where operator new's plain C++ can't answer (the original runs; nothing
// written yet).
std::optional<uint32_t> InputRecordingsCreate(uint8_t* base);
// sub_82105560 Replay::Create: 0x13C8F0 bytes, Record5232 on seven records
// from +0x2BF10 when not null; the block, or 0, into 0x826C5B50, then the
// word at block + 0x2B600 = 1 (at 0x2B600 when the allocation failed).
// Returns r3 as the original leaves it (the last record, or 0), or nullopt
// where operator new's plain C++ can't answer.
std::optional<uint32_t> ReplayCreate(uint8_t* base);

}  // namespace data_load
