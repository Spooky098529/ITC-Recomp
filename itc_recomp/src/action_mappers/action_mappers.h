// The action mappers as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/action-mappers.md): one mapper per player
// (2,896 bytes, vtable 0x82007F78) turning a logical pad's sticks and
// triggers and its listener's buttons into 30 actions (ids 151-180), the
// client list they're kept in, and their repeat and curve objects. The
// mappers are guest objects shared with the recompiled code (the menus, the
// pause menu and the car read them through the vtable), so these work on
// guest memory, with the originals' float steps (single rounding after each
// operation, as the translated code does; built with -ffp-model=strict and
// -ffp-contract=off, flush-to-zero off: the adapters' job). Adapters:
// src/hooks/action_mappers.cpp.
//
// The calls the originals make through a repeat or curve object's vtable
// (slot 1) are made natively only for the functions seen in play
// (ActionRepeat::Update, sub_8210B568, and ActionCurve::Apply,
// sub_8210B5B8): the functions that make them check every object first
// (MapperCallsKnown) and the adapter runs the original when one isn't.

#pragma once

#include <cstdint>

namespace action_mappers {

// The functions the objects' vtable slot 1 points at in play.
constexpr uint32_t kRepeatUpdateFunction = 0x8210B568;
constexpr uint32_t kCurveApplyFunction = 0x8210B5B8;

// The mapper's fields (docs/known-data.md, "action mapper").
constexpr uint32_t kActions = 30;
constexpr uint32_t kWords = 448;          // held, held before, pressed, released, fired, pending
constexpr uint32_t kConfig = 372;         // four pointers: kinds, links, analog kinds, links
constexpr uint32_t kRaw = 388;            // 30 u16s: the raw values
constexpr uint32_t kRepeatPointers = 472; // 30 pointers to the repeat objects
constexpr uint32_t kAnalog = 1072;        // 30 u16s, then 30 floats at +60
constexpr uint32_t kCurvePointers = 1252; // 30 pointers to the curve objects
constexpr uint32_t kThresholds = 1852;    // 30 entries of 20 bytes
constexpr uint32_t kListener = 2464;      // the listener (an input processor)
constexpr uint32_t kPadValues = 2880;     // pointer to the logical pad's values

// --- The leaves ---

// ActionCurve::Apply (sub_8210B5B8): the curve objects' slot 1, on `x` (the
// double in f1); returns f1.
double CurveApply(uint8_t* base, uint32_t curve, double x);
// ActionRepeat::Update (sub_8210B568): the repeat objects' slot 1; `held`
// is r4's low byte. Returns whether it fires (r3).
uint32_t RepeatUpdate(uint8_t* base, uint32_t repeat, uint32_t held);
// ActionMapper::IndexOf (sub_8210B140, its case 0 sub_8210B174 merged in):
// id - 151 for ids 151-180, else 30.
uint32_t IndexOf(uint32_t id);
// sub_825924E0: 1 when every bit of `mask` is set in the listener's held
// word (+272), else 0.
uint32_t ListenerHeld(uint8_t* base, uint32_t listener, uint32_t mask);
// sub_8258CF10: the same with the mask read from `mask_address`.
uint32_t ListenerTest(uint8_t* base, uint32_t listener, uint32_t mask_address);

// Diagnostics (--log_input_reads): counts a held test made from `caller`
// (the original's call to sub_8258CF10, from its return address) as the
// originals' wrappers did: sub_825924E0 from sub_8258CF10, then
// sub_8258CF10 itself. Nothing when the flag is off.
void NoteListenerTest(uint32_t caller, uint32_t listener, uint32_t mask, uint32_t result);

// --- The client list (one, at [0x82970C14]) and its nodes ---

constexpr uint32_t kClientList = 0x82970C14;

// ClientNode::ClientNode (sub_8259C6D0): vtable 0x82071380, +4 the type, +8
// the next node (0).
void ClientNodeConstruct(uint8_t* base, uint32_t node, uint32_t type);
// ClientList::ClientList (sub_8259C7B8): vtable 0x820713C0, the first and
// last nodes and six counts 0; kept at [0x82970C14].
void ClientListConstruct(uint8_t* base, uint32_t list);
// ClientList::Add (sub_8259C960): `node` appended to the list at
// [0x82970C14], its type's count up by one; nothing when there's no list.
void ClientListAdd(uint8_t* base, uint32_t node);
// ClientList::Count and BoundCount (sub_8259C868, sub_8259C898): the count
// for `type` at +12 or +24 (+ 4 × type), up by one first when `bump`'s low
// byte is nonzero.
uint32_t ClientListCount(uint8_t* base, uint32_t list, uint32_t type, uint32_t bump);
uint32_t ClientListBoundCount(uint8_t* base, uint32_t list, uint32_t type, uint32_t bump);
// ClientList::Find (sub_8259C818): the n-th node of `type` (compared as
// signed words) in the list at [0x82970C14], or 0.
uint32_t ClientListFind(uint8_t* base, uint32_t type, uint32_t n);

}  // namespace action_mappers
