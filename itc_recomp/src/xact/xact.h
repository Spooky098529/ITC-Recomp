// XACT as plain C++ (B7 in docs/rexglue-removal.md; the functions are in
// docs/functions/xact.md): the Xbox 360's sound library linked into the
// game, translated from the leaves up. Only the work that doesn't call
// still-recompiled code is here (no XAudio, no critical sections, no calls
// through vtables into recompiled methods). XACT's objects are guest objects
// shared with the recompiled code (and with XAudio's thread, which runs the
// engine's audio callback), so these work on guest memory. Adapters:
// src/hooks/xact.cpp.

#pragma once

#include <cstdint>

namespace xact {

// --- Names ---

// XACTHashName (sub_825BFC50): the C string at `name` hashed to 16 bits
// (h = 3h + (h >> 1) + c, each byte sign-extended), modulo the 16-bit
// `table_size`. False when the size is 0 (the original traps).
bool HashName(uint8_t* base, uint32_t name, uint32_t table_size, uint32_t* hash);

// --- The node pools (216 bytes: +0 the free list, +4 the chunk list, +8
// the inline chunk of 16 12-byte nodes, +204/+208 the used list's head and
// tail, +212 its count; a node is +0 the value, +4 next, +8 previous) ---

// XACTNodePool::XACTNodePool (sub_825C2060).
void NodePoolConstruct(uint8_t* base, uint32_t pool);
// XACTNodePool::Free (sub_825C2C60): the node unlinked from the used list
// and pushed on the free list.
void NodePoolFree(uint8_t* base, uint32_t pool, uint32_t node);
// XACTNodePool::Alloc (sub_825C2CC8): a node from the free list, a new
// 196-byte chunk (XMemAlloc 0x20840001) of 16 linked first when it's empty;
// 0 when the allocation fails. Allocates through the native layer: call it
// only while hooks::NativeHeapActive().
uint32_t NodePoolAlloc(uint8_t* base, uint32_t pool);
// XACTNodePool::FreeChunks (sub_825C1F30): every chunk but the inline one
// freed (XMemFree 0x20840001); the chunk list left empty. Native layer only,
// as NodePoolAlloc. Returns what the original leaves in r3 (the pool for an
// empty list, the inline chunk when it came last, else XMemFree's r3).
uint32_t NodePoolFreeChunks(uint8_t* base, uint32_t pool);

// --- The engine ---

// XACTEngine::AddCue (sub_825C1518): the cue's node (+12, or address 0 for
// no cue, as the original) linked at the tail of the engine's list +28.
void EngineAddCue(uint8_t* base, uint32_t engine, uint32_t cue);

// --- The global settings ---

// XACTGlobalSettingsBase::XACTGlobalSettingsBase (sub_825C3508) and
// XACTGlobalSettings::XACTGlobalSettings (sub_825C21B0): r4 the engine.
void GlobalSettingsBaseConstruct(uint8_t* base, uint32_t settings, uint32_t engine);
void GlobalSettingsConstruct(uint8_t* base, uint32_t settings, uint32_t engine);

// --- Variables, cues, faders ---

// XACTVariable::Init (sub_825C1DE0).
void VariableInit(uint8_t* base, uint32_t variable, uint32_t engine, uint32_t cue,
                  uint32_t index, uint32_t record, uint32_t r8);
// XACTCue::DetachSound (sub_825C4FC0): +52 and +40 cleared.
void CueDetachSound(uint8_t* base, uint32_t cue);
// XACTCue::FindVariable (sub_825C5758): the local variable (list at +324,
// next at +4) with the 16-bit index, or 0.
uint32_t CueFindVariable(uint8_t* base, uint32_t cue, uint32_t index);
// XACTFader::XACTFader and ~XACTFader (sub_825CC0A0, sub_825CC0D0).
void FaderConstruct(uint8_t* base, uint32_t fader);
void FaderDestruct(uint8_t* base, uint32_t fader);

}  // namespace xact
