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
#include <optional>

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

// --- Sound banks (vtable 0x82075A90) over a .xsb file used in place ---
//
// The functions reading the file call the bank's slot +64 (GetHeader) for
// it each time. They run natively only while that slot is GetHeader
// (sub_825C7AA8, the only one in the game's vtables): std::nullopt where it
// isn't, with nothing changed, and the adapter runs the original.

// XACTSoundBank::GetHeader (sub_825C7AA8): +260, the file's buffer.
uint32_t SoundBankGetHeader(uint8_t* base, uint32_t bank);
// The header through the bank's slot +64, when that's GetHeader.
std::optional<uint32_t> SoundBankHeader(uint8_t* base, uint32_t bank);
// XACTSoundBank::GetSimpleCues and GetWaveBankNames (sub_825C7AB0,
// sub_825C7B70): the buffer + the header's word at +26 (+50).
std::optional<uint32_t> SoundBankGetSimpleCues(uint8_t* base, uint32_t bank);
std::optional<uint32_t> SoundBankGetWaveBankNames(uint8_t* base, uint32_t bank);
// XACTSoundBank::GetPointer (sub_825C7C90): the buffer + `offset`.
std::optional<uint32_t> SoundBankGetPointer(uint8_t* base, uint32_t bank, uint32_t offset);
// XACTSoundBank::GetCueCount (sub_825C6928): the header's 16-bit +9 plus
// +11, 16-bit.
std::optional<uint32_t> SoundBankGetCueCount(uint8_t* base, uint32_t bank);
// XACTSoundBank::FirstEvent and NextEvent (sub_825C7F30, sub_825C7F98): an
// event iterator (+0 the offset of a count byte, +4 the cursor, +8 the
// count, +9 the index); the event's address in the buffer, or 0 after the
// last (NextEvent).
std::optional<uint32_t> SoundBankFirstEvent(uint8_t* base, uint32_t bank, uint32_t iterator);
std::optional<uint32_t> SoundBankNextEvent(uint8_t* base, uint32_t bank, uint32_t iterator);
// XACTSoundBank::GetCueRecord (sub_825C8230): the simple cue table + 5 x
// the 16-bit index below the header's simple cue count (+9). nullopt for
// a complex cue (sub_825C7B10, never ran: the game's banks have none).
std::optional<uint32_t> SoundBankGetCueRecord(uint8_t* base, uint32_t bank, uint32_t index);

// --- Wave banks (vtable 0x82075AE8) ---

// XACTWaveBank::GetEntry (sub_825C8398): the entry table (+272) + 24 x the
// 16-bit index.
uint32_t WaveBankGetEntry(uint8_t* base, uint32_t bank, uint32_t index);
// XACTWaveBank::GetSeekTable (sub_825C8580): the entry's XMA seek table
// (the segment at +288: an offset a wave, -1 for none, then the tables), or
// 0.
uint32_t WaveBankGetSeekTable(uint8_t* base, uint32_t bank, uint32_t index);
// XACTWaveBank::GetName and GetEntryCount (sub_825C85B8, sub_825C85E8): the
// bank data (+284) + 8, and its +4 word.
uint32_t WaveBankGetName(uint8_t* base, uint32_t bank);
uint32_t WaveBankGetEntryCount(uint8_t* base, uint32_t bank);
// XACTWaveBank::GetWaveFormat (sub_825CC320): the entry's 4-byte format
// (+4, through the bank's slot +16) copied byte by byte to `out`; false,
// with nothing changed, where slot +16 isn't GetEntry.
bool WaveBankGetWaveFormat(uint8_t* base, uint32_t bank, uint32_t index, uint32_t out);

// --- Wave entry accessors (+0 valid, +4 the 16-bit index, +8 the bank,
// +12 a copy of the 24-byte entry: +12 flags and duration, +16 the format,
// +20/+24 the play region, +28/+32 the loop region) ---

// XACTWaveEntry::XACTWaveEntry (sub_825CED88): +0 = 0. (Folded: the movie
// player calls it too.)
void WaveEntryConstruct(uint8_t* base, uint32_t entry);
// XACTWaveEntry::Set (sub_825CED98): the index, the bank, the entry copied
// (memcpy) from the bank's slot +16, valid. False, with nothing changed,
// where slot +16 isn't GetEntry or the copy isn't one the native memcpy
// makes (into the physical views, or ranges sharing memory).
bool WaveEntrySet(uint8_t* base, uint32_t entry, uint32_t index, uint32_t bank);
// XACTWaveEntry::GetLoopRegion (sub_825CEDF8): entry + 28.
uint32_t WaveEntryGetLoopRegion(uint32_t entry);
// XACTWaveEntry::GetDuration (sub_825CEE00): the word at +12 >> 4.
uint32_t WaveEntryGetDuration(uint8_t* base, uint32_t entry);
// XACTWaveEntry::SamplesToMs (sub_825CEE70): samples x 1000 / the format's
// rate (bits 5-22); false for a rate of 0 (the original traps).
bool WaveEntrySamplesToMs(uint8_t* base, uint32_t entry, uint32_t samples, uint32_t* ms);
// XACTWaveEntry::FindXmaBlock (sub_825CEF50): the 64 KB block holding the
// sample position in the seek table (+0 the count, then cumulative sample
// counts): *block_offset (guest word) its byte offset, *into (guest word)
// the samples into it rounded to 128.
void WaveEntryFindXmaBlock(uint8_t* base, uint32_t entry, uint32_t position, uint32_t seek_table,
                           uint32_t block_offset, uint32_t into);
// XACTWaveEntry::SamplesToBytes (sub_825CF080): the byte position of a
// sample position by the format tag (PCM, XMA through the seek table from
// the bank's slot +20); the samples into the XMA block into `out` (a guest
// word, when not 0). nullopt where the XMA path's slot +20 isn't
// GetSeekTable.
std::optional<uint32_t> WaveEntrySamplesToBytes(uint8_t* base, uint32_t entry, uint32_t position,
                                                uint32_t out);

// --- Batch 3: cue state, wave bank forgetting, sounds, faders ---

// XACTCue::GetStateInternal (sub_825C4D90): XACT_CUESTATE flags from the
// sound (+52): STOPPED (0x20) without one; else its state (+80, 1 to 6) as
// 1, 2, 4, 8, 0x10, 0x20 (0 out of range), plus PAUSED (0x40) when the
// sound's byte +52 has 0x20.
uint32_t CueGetStateInternal(uint8_t* base, uint32_t cue);
// XACTSoundBank::ForgetWaveBank (sub_825C72B0): the bank's wave bank array
// (+244) entry holding `wave_bank` cleared. Returns what the original leaves
// in r3 (the bank without an array, else the header). nullopt where the
// bank's slot +64 isn't GetHeader.
std::optional<uint32_t> SoundBankForgetWaveBank(uint8_t* base, uint32_t bank,
                                                uint32_t wave_bank);
// sub_825C0408: ForgetWaveBank on every sound bank in the engine's list
// +44 (nodes at bank + 12). Returns r3 as the original leaves it; nullopt,
// with nothing changed, where any bank's slot +64 isn't GetHeader.
std::optional<uint32_t> EngineForgetWaveBank(uint8_t* base, uint32_t engine, uint32_t wave_bank);
// XACTSound::SetTrackParameter (sub_825CA980): sub_825D0540 on each track
// (list +36/+40). Only a sound without tracks (as in play) is native: false
// otherwise (sub_825D0540 never ran; the original runs).
bool SoundSetTrackParameter(uint8_t* base, uint32_t sound);
// XACTFader::Step (sub_825CC1D0): 0 for an idle fader (state +16 neither 1
// nor 2). nullopt for a fading one (no fade ran in play: the original's
// fade steps, curve and completion slots run).
std::optional<uint32_t> FaderStep(uint8_t* base, uint32_t fader);

// --- Batch 4: constructors and listener lists ---

// XACTVariable::XACTVariable (sub_825C20B0): the 252-byte variable's
// defaults (a node pool at +24).
void VariableConstruct(uint8_t* base, uint32_t variable);
// XACTNodePool::PushBack (sub_825C60C0): a node (NodePoolAlloc) holding
// `value` linked at the used list's tail; 0 when out of memory. Native
// layer only, as NodePoolAlloc.
uint32_t NodePoolPushBack(uint8_t* base, uint32_t pool, uint32_t value);
// XACTCue::AddListener and XACTSoundBank::AddListener (sub_825C6140,
// sub_825C7858), through their +4 interfaces (`self`): PushBack on the
// pool at self + 72 (+24). Native layer only.
uint32_t CueAddListener(uint8_t* base, uint32_t self, uint32_t listener);
uint32_t SoundBankAddListener(uint8_t* base, uint32_t self, uint32_t listener);
// XACTCue::RemoveListener and XACTSoundBank::RemoveListener (sub_825C6040,
// sub_825C7758), through the +4 interfaces: the node holding `listener` in
// the used list (self + 276, + 228) freed to the pool (self + 72, + 24).
// Returns r3 as the original leaves it (self, or the pool when freed).
uint32_t CueRemoveListener(uint8_t* base, uint32_t self, uint32_t listener);
uint32_t SoundBankRemoveListener(uint8_t* base, uint32_t self, uint32_t listener);
// XACTSoundBankBase::XACTSoundBankBase and XACTSoundBank::XACTSoundBank
// (sub_825C76A0, sub_825C79B8): r4 the engine.
void SoundBankBaseConstruct(uint8_t* base, uint32_t bank, uint32_t engine);
void SoundBankConstruct(uint8_t* base, uint32_t bank, uint32_t engine);
// XACTWaveBankRoot::XACTWaveBankRoot, XACTWaveBankBase::XACTWaveBankBase
// and XACTWaveBank::XACTWaveBank (sub_825CC620, sub_825CEBE0,
// sub_825C8310): r4 the engine.
void WaveBankRootConstruct(uint8_t* base, uint32_t bank, uint32_t engine);
void WaveBankBaseConstruct(uint8_t* base, uint32_t bank, uint32_t engine);
void WaveBankConstruct(uint8_t* base, uint32_t bank, uint32_t engine);
// XACTCueBase::XACTCueBase and XACTCue::XACTCue (sub_825C5E58,
// sub_825C67E8): r4 the engine, r5 the sound bank, r6 the cue's record, r7
// the cue index; registers the cue with the bank (its +4 interface's slot
// +0, AddListener). False, with nothing changed, where that slot isn't
// sub_825C7858. Native layer only (AddListener allocates).
bool CueBaseConstruct(uint8_t* base, uint32_t cue, uint32_t engine, uint32_t bank,
                      uint32_t record, uint32_t index);
bool CueConstruct(uint8_t* base, uint32_t cue, uint32_t engine, uint32_t bank, uint32_t record,
                  uint32_t index);
// XACTWaveEntry::GetLoopStartBytes and GetLoopLengthBytes (sub_825CF210,
// sub_825CF288): the loop region in bytes (SamplesToBytes; nullopt where it
// is).
std::optional<uint32_t> WaveEntryGetLoopStartBytes(uint8_t* base, uint32_t entry, uint32_t out);
std::optional<uint32_t> WaveEntryGetLoopLengthBytes(uint8_t* base, uint32_t entry);

}  // namespace xact
