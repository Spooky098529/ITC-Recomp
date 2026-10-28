// The front end's save layer natively (docs/functions/save-interface.md):
// the save slot of a controller and the slot wrappers by player, the save
// buffers at [0x828161F0], the replays' sort and the play time's update.
// Plain C++ over the guest layout as the recompiled functions; adapters in
// src/hooks/save_interface.cpp (behind --native_save_interface), register
// steps shared with the harness in src/hooks/save_interface.h.

#pragma once

#include <cstdint>
#include <optional>

namespace save_interface {

// The player-to-controller table (Players::ResetControllers sets it).
constexpr uint32_t kControllers = 0x826BABA4;
// The save buffers' header (24 bytes: +0 the game data's buffer, +4 a
// replay's, +8 the replay record being saved, +12 the replays' records, +16
// their list entries, +20 their count).
constexpr uint32_t kSaveBuffers = 0x828161F0;
constexpr uint32_t kGameDataBytes = 0x1F978;   // 129,400
constexpr uint32_t kReplayBytes = 0x12C8F0;    // 1,231,088
constexpr uint32_t kReplayRecordBytes = 36;
// The play time's frame counter.
constexpr uint32_t kPlayTimeFrames = 0x826C5B90;

// sub_8228F780 SaveSlots::OfController: the first i under the users' count
// (SaveInterface::UserCount) whose signed-in user (UserAt(i)) is
// `controller` (as int32), else the count. nullopt where the interface's
// targets aren't ours (nothing read but them).
std::optional<uint32_t> SlotOfController(uint8_t* base, uint32_t controller);
// The controller of a player: [0x826BABA4 + 4 x player].
uint32_t ControllerOf(uint8_t* base, uint32_t player);

// The slot wrappers by player (the slot of the player's controller), each
// answering r3 as the original leaves it; nullopt where a target isn't ours
// (nothing done).
std::optional<uint32_t> PlayerRequestDevice(uint8_t* base, uint32_t player);  // sub_8228F9B0
std::optional<uint32_t> PlayerSlotState(uint8_t* base, uint32_t player);      // sub_8228F9E8
std::optional<uint32_t> PlayerDevice(uint8_t* base, uint32_t player);         // sub_8228FA90
std::optional<uint64_t> PlayerPending(uint8_t* base, uint32_t player);        // sub_8228FAC8

// sub_8228F6B0 SaveBuffers::Create: when [0x828161F0] is 0, the header
// (VirtualAlloc's plain C++, 24 bytes) stored and cleared, then the game
// data's, a replay's and the record's buffers. Returns r3 as the original
// leaves it (the last buffer) when it made them, nullopt (r3 left) when the
// header was there. Only with the native heap (the adapter checks).
std::optional<uint32_t> SaveBuffersCreate(uint8_t* base);
// sub_8228F7E8 SaveBuffers::FreeReplayList: +12 freed (VirtualFree's plain
// C++) and cleared when not 0. Returns VirtualFree's r3 (1) when it freed,
// nullopt (r3 left) when not.
std::optional<uint32_t> SaveBuffersFreeReplayList(uint8_t* base);
// sub_8228F8F8 SaveBuffers::Destroy: the replays' records, then +0, +4 and
// +8 freed and cleared when set, the header freed and the global 0. Returns
// r3 as the original leaves it (the header's VirtualFree: 1).
// SaveBuffersDestroyNative says whether it can run natively: the native
// heap on, and the header and every buffer it frees pages the layer gave.
bool SaveBuffersDestroyNative(uint8_t* base);
uint32_t SaveBuffersDestroy(uint8_t* base);

// sub_8228F840 Replays::Sort: the records (count at the header's +20)
// sorted by +28, the pairs swapped through a 36-byte copy at `temp` (the
// original's frame + 80, left there as it leaves it), each copy by memcpy's
// plain C++. Returns r3 as the original leaves it: the last copy's
// destination (memcpy's answer), nullopt (r3 left) when nothing was swapped.
std::optional<uint32_t> ReplaysSort(uint8_t* base, uint32_t records, uint32_t temp);

// sub_821109B8: the frames since the last update ([0x826C5B90] less the
// stats' +6040, which is set to it) added to the play time (+6032 hours,
// +6036 minutes, +6037 seconds, +6038 frames) with the carries; `out` +0 the
// hours, +4 the word at +6036.
void PlayTimeAdd(uint8_t* base, uint32_t out, uint32_t stats);
// sub_82112838 PlayTime::Update: PlayTimeAdd on the save data's stats (+48)
// into `out` (the original's frame + 80). Returns r3 as the original leaves
// it (`out`).
uint32_t PlayTimeUpdate(uint8_t* base, uint32_t save, uint32_t out);

}  // namespace save_interface
