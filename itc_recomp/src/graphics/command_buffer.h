// The graphics library's command buffer (docs/functions/command-buffer.md):
// the segments its packet writers fill, the data space reserved beside
// them, the segments' tails (the fences and read positions our kick applies,
// src/native/gpu_link.cpp), the callback packets and predicated tiling's
// chains of blocks. Plain C++ over the device in guest memory, which
// recompiled packet writers share; the adapters are in
// src/hooks/command_buffer.cpp (--native_command_buffer).

#pragma once

#include <cstdint>
#include <functional>

namespace graphics::command_buffer {

// The device's fields used here.
constexpr uint32_t kWritePosition = 40;    // the last word written (pre-increment)
constexpr uint32_t kSegmentEnd = 44;
constexpr uint32_t kSoftEnd = 48;           // the end less 160: writers end the segment past it
constexpr uint32_t kProgressBlock = 10768;  // the fences and read position
constexpr uint32_t kCallbackBlock = 10772;
constexpr uint32_t kFence = 10780;          // the open segment's fence
constexpr uint32_t kFlags = 10809;          // bit 1 the GPU hung, bit 6 out of space
constexpr uint32_t kFlags2 = 10810;         // bit 3 the buffer wrapped
constexpr uint32_t kTilingEnd = 12960;      // predicated tiling: how far its segments reach
constexpr uint32_t kTilingGeneration = 12964;
constexpr uint32_t kTailPosition = 12968;   // the last tail's position and generation
constexpr uint32_t kTailGeneration = 12972;
constexpr uint32_t kSavedSegment = 13056;   // SaveSegment's six words
constexpr uint32_t kBufferStart = 13504;
constexpr uint32_t kBufferEnd = 13508;
constexpr uint32_t kSegmentSize = 13512;
constexpr uint32_t kDataCursor = 13516;
constexpr uint32_t kGeneration = 13520;
constexpr uint32_t kWrapCursor = 13524;
constexpr uint32_t kSegmentStart = 13528;
constexpr uint32_t kRecorder = 13160;      // a recording command buffer (never in play)
constexpr uint32_t kSegmentData = 13532;    // the data reserved for the open segment
constexpr uint32_t kSegmentDataBudget = 13536;
constexpr uint32_t kTailsDirect = 19980;    // set: the hung GPU's progress not written
constexpr uint32_t kDummySegment = 15176;   // the 4,800-byte segment writers get when out of space
constexpr uint32_t kRecording = 10808;      // bit 7 (0x80): a recording command buffer
constexpr uint32_t kSegmentChain = 13016;   // predicated tiling's chain of segments; the kick's list
constexpr uint32_t kThrottleFence = 13540;  // the last Swap's fence (+13544 with +20440 bit 0)
constexpr uint32_t kThrottleFence2 = 13544;
constexpr uint32_t kThrottleSelect = 20440;
constexpr uint32_t kPendingKicks = 10868;   // kicks deferred until the GPU's interrupts (always 0)
constexpr uint32_t kTrainingFailed = 0x829537BCu;  // a global, never set (Device::Init leaves it 0)

// A chain of 4 KB blocks (predicated tiling's segments and draws, at device
// +13016, +12976 and +12996): +0 its first block, +4 the current block, +8
// the write cursor, +12 the block's end, +16 the device.
constexpr uint32_t kChainFirst = 0;
constexpr uint32_t kChainBlock = 4;
constexpr uint32_t kChainCursor = 8;
constexpr uint32_t kChainEnd = 12;
constexpr uint32_t kChainDevice = 16;

// The address the GPU sees for a CPU address (the physical address, the
// 0xE0000000 view's 4 KB offset added).
inline uint32_t PhysicalAddress(uint32_t address) {
  return (address & 0x1FFFFFFFu) + ((((address >> 20) & 0xFFFu) + 0x200u) & 0x1000u);
}

// Device::WriteSegmentTail (sub_8229E768): the 10 words ending a segment at
// `position` (the invalidate and two EVENT_WRITE_SHD: the read position with
// the generation's low bits and the fence into the progress block), the
// position and generation kept at +12968 and +12972, and both written into
// the progress block when the GPU hung (+10809 bit 1, +19980 0); the fence
// +10780 advanced by 2. Returns position + 40.
uint32_t WriteSegmentTail(uint8_t* base, uint32_t device, uint32_t position);

// Device::SaveSegment (sub_8229DBF0): the write position, the end, the
// segment's start, the wrap cursor, the data cursor and the generation
// copied to +13056..+13076.
void SaveSegment(uint8_t* base, uint32_t device);

// Device::RestoreSegment (sub_8229E0C0): what SaveSegment saved put back
// (the write position, the end, the segment's start, the wrap cursor, the
// data cursor and the generation), the soft end +48 the end less 160. Only
// from predicated tiling's end (src/graphics/predicated_tiling.h), when out
// of space (never in play); no adapter.
void RestoreSegment(uint8_t* base, uint32_t device);

// CommandChain::Close (sub_8229DFB8): the current block's first word gets
// where its content ends (the cursor, 0xC0000000 + its physical address),
// the cursor the end marker 0xC0000000, and the block's end +12 is cleared.
void CloseChain(uint8_t* base, uint32_t chain);

// The callback packets (sub_8229E9D0) at `position` (pre-increment): a
// WAIT_UNTIL first unless `flags` has bits outside the CPUs (24-29); then
// CALLBACK_ADDRESS and CALLBACK_CONTEXT = `function`, `argument`, the CPUs
// (0: 4) into SCRATCH_REG0, a WAIT_REG_MEM on the callback block, the
// INTERRUPT our kick runs (src/native/gpu_link.cpp), CALLBACK_ACK, a
// WAIT_REG_MEM for the callbacks done and CALLBACK_ADDRESS = 0BADF00D.
// Returns the last word written.
uint32_t WriteCallback(uint8_t* base, uint32_t device, uint32_t position, uint32_t flags,
                       uint32_t function, uint32_t argument);

// The next space in the command buffer (sub_8229E100): `kind` bit 0 set,
// data (`size` bytes at `alignment`, from the data cursor +13516, or from
// the wrap cursor +13524 past the buffer's end, never past the open segment);
// clear, a segment (32-byte aligned, at least the segment size +13512 or
// the words asked, 54 when none; wrapping to the buffer's start, the
// generation + 1 and +10810 bit 3 set, when the end is too close; its size
// written back to `size` on success). Under predicated tiling (+12960) it fails rather
// than pass the tiled frame's segments. Returns the address, or 0. The
// original's wait for the GPU to have read past the space (sub_8229DC28) is
// left out: a wait (Dropped, the user's call on fence waits, 2026-10-24),
// which passes at once in play (our kick writes the read position as it
// kicks; its wait helpers never ran).
uint32_t NextSpace(uint8_t* base, uint32_t device, uint32_t kind, uint32_t& size,
                   uint32_t alignment);

// Device::AllocateFromCommandBuffer (sub_8229E308): `words` words of data
// space at `alignment` (NextSpace), or 0 at once when out of space (+10809
// bit 6; set when this fails). Counted in the segment's data +13532; past
// its budget +13536 outside tiling, the segment is cut short (the soft end
// +48 the write position, the end 160 bytes on) and the data cursor moved
// to the buffer's end unless the wrap cursor is at its start. With a
// recording command buffer (+13160, never in play) its adapter runs the
// original. The inline vertices' space (BeginVertices) comes from here.
uint32_t Reserve(uint8_t* base, uint32_t device, uint32_t words, uint32_t alignment);

// Device::WriteDefaultDrawRegisters (sub_822AF978) without its packets
// (the default vertex index range, clip, scissor-mode and viewport
// transform controls, the AA mask and the bin mask: 15 words nothing reads,
// as our renderer takes the registers from the shadow, not the command
// buffer; the graphics rule, as Device::SetDefaultState's, 2026-10-24): the
// registers they overwrote marked dirty as the original does (+24
// 0x400000, 0x200000 and 0x2000000, +16 0xE0, +32 0x80000), so the next
// draw writes them from the shadow. Returns `position` (no words written).
uint32_t WriteDefaultDrawRegisters(uint8_t* base, uint32_t device, uint32_t position);

// CommandChain::NewBlock (sub_8229EB10): a 4,224-byte block for `chain`
// from the command buffer's data space (Reserve, 4,224 words asked at
// 128-byte alignment, as the original asks). The old block's first word
// gets where its content ends (the cursor) and the old cursor the new
// block's address, both as 0xC0000000 + the physical address; the new
// block's first word its own start + 4 for now. On no space, the block is
// the word at the device's +15176 and the chain's first block cleared. The
// chain's block, cursor (block + 4) and end (block + 4,220) set; returns
// the cursor. The original's data cache flush of the new block
// (sub_822AC470) is left out (Dropped: no data cache to flush on PC).
uint32_t NewBlock(uint8_t* base, uint32_t chain);

// CommandChain::Init (sub_8229ED60): the chain's device set, its block,
// cursor and end cleared, a first block started (NewBlock) and kept at +0.
// Returns NewBlock's cursor.
uint32_t InitChain(uint8_t* base, uint32_t chain, uint32_t device);

// Whether `device` has a recording command buffer (+10808 bit 7, or the
// object at +13160; never in play): the segment flow's adapters run the
// originals whole then.
bool Recording(uint8_t* base, uint32_t device);

// The kick a segment's end hands its words to: Device::DeferredKick
// (sub_8229E848, overridden in src/native/gpu_link.cpp), a guest call, as
// the kick runs the segment's callbacks (guest functions). Its arguments are
// the original call's registers r3-r8: the device, a word returned as is
// (the tail's end), the segment's physical address, its word count, the
// interrupts it raises (0) and the list (+13016); returns r3.
using Kick = std::function<uint32_t(uint32_t device, uint32_t result, uint32_t address,
                                    uint32_t words, uint32_t interrupts, uint32_t list)>;

// The segment flow, mutually recursive as the originals are (the end kicks
// the segment and starts the next, whose throttle wait may end the segment
// again). Each returns r3 as the original leaves it.
//
// The segment's end (sub_8229EF48), called by every packet writer past the
// soft end +48: the segment kicked and the next started (KickSegment); with
// the "training failed" global 0x829537BC set (never) and the GPU not hung
// (+10809 bit 1), the segment's fence waited for and bit 1 set. Returns the
// write position +40. The original's pending coherency flush block
// (sub_8229E5D0, Dropped, answers none) is left out.
uint32_t EndSegment(uint8_t* base, uint32_t device, const Kick& kick);

// Device::KickSegment (sub_8229EDA8): the open segment (from +13528 to the
// write position + 4) ended, unless out of space (+10809 bit 6): under
// predicated tiling (+12960) appended to the segment chain at +13016
// (0x81000000 | its word count, its physical address; a new block when the
// chain's is full); else its tail written (WriteSegmentTail) and kicked.
// Then the next segment: at the next 32-byte boundary when it's before the
// soft end +48 (with +19980 set, the data cursor rounded up to 4 KB), else
// a new one (NextSegment).
uint32_t KickSegment(uint8_t* base, uint32_t device, const Kick& kick);

// The next segment (sub_8229EC00) of at least `words` + 14 words (0: the
// default), from NextSpace (32-byte aligned): its start +13528, the data
// cursor and count cleared, the write position, end (14 words left for the
// tail) and soft end set; when none can be had (or already out of space),
// +10809 bit 6 set and the writers pointed at the dummy segment ([+15176],
// 4,800 bytes). Then, unless it was already out of space, the frame
// throttle: the last Swap's fence (+13540, or +13544 with +20440 bit 0)
// waited for (WaitForFence, code 3) and cleared. Returns the segment, or 0.
uint32_t NextSegment(uint8_t* base, uint32_t device, uint32_t words, const Kick& kick);

// The fence wait (sub_8229E908): nothing for fence 0 or one the GPU is done
// with (the progress block's +0 against the open fence +10780); a wait on
// the open fence ends the segment first (EndSegment) outside predicated
// tiling, which kicks it and so completes it. The original then spins with
// the wait helpers (sub_8229D6F0, sub_8229D8A0, sub_8229D720) while it's
// still pending: left out, a wait on the GPU (Dropped, the user's call on
// fence waits, 2026-10-24), which never spins in play (our kick completes
// every fence as it kicks; the helpers never ran). `code` only named the
// wait for the helpers' time totals.
uint32_t WaitForFence(uint8_t* base, uint32_t device, uint32_t fence, uint32_t code,
                      const Kick& kick);

// Reserves `words` words at the write position (sub_8229F148): when they
// don't fit before the segment's end +44, the segment is ended
// (EndSegment) and, still short, a new one of at least those words started
// (NextSegment; 0 returned when none can be had). Returns the write
// position +40 (the words go after it), or 0. The vertex shader patch's copy
// (sub_822AEBF0) is made there.
uint32_t ReserveWords(uint8_t* base, uint32_t device, uint32_t words, const Kick& kick);

// Device::InsertCallback (sub_8229F228): the segment ended when past its
// soft end (EndSegment), the callback's packets (WriteCallback: `flags`'
// CPUs, `function`, `argument`) written at the write position, which moves
// past them. Returns the new write position.
uint32_t InsertCallback(uint8_t* base, uint32_t device, uint32_t flags, uint32_t function,
                        uint32_t argument, const Kick& kick);

// sub_8229F0E0 (only from Device::SetUpRing) without its packets: the
// segment ended when past its soft end (EndSegment), as the original does
// before writing them. Its WAIT_UNTIL (register 0x5C8 = 0x20000) and
// register 0xD04 = 0 aren't written: packets nothing reads (our kick keeps
// only the scratch registers' effects; the graphics rule, as
// WriteDefaultDrawRegisters'). Returns the write position +40 (unmoved).
uint32_t WriteIdlePackets(uint8_t* base, uint32_t device, const Kick& kick);

// The full GPU wait (sub_8229F1B0) without its waits: the segment ended
// when past its soft end, then the open fence waited for (WaitForFence,
// code 4), which ends the segment and so kicks the frame's last packets,
// fences and callbacks. Its WAIT_UNTIL packet (register 0x5C8 = 0x20000,
// nothing reads it) and its spin until the pending kick count +10868 is 0
// (always 0: the deferred kick's override never raises it; a wait on the
// GPU, the user's call on fence waits, 2026-10-24) are left out. Returns
// WaitForFence's r3.
uint32_t WaitForIdle(uint8_t* base, uint32_t device, const Kick& kick);

}  // namespace graphics::command_buffer
