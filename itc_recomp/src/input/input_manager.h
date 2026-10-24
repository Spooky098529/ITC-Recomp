// The input manager, its pads and its processors' set-up and accessors as
// plain C++ over their guest layouts (docs/functions/input.md; the fields in
// docs/known-data.md). The objects stay in guest memory: B1's native update
// (src/hooks/input_update.cpp), the action mappers and the recompiled
// menus read them there. Adapters: src/hooks/input_manager.cpp.
//
// The manager (3,984 bytes, vtable 0x82000F48): five logical pads (196
// bytes from +8), four physical pads (212 bytes from +988), five processors
// (416 bytes from +1840) and five listener lists (std::lists of pointers,
// 12 bytes from +3920).

#pragma once

#include <cstdint>
#include <optional>

namespace input {

constexpr uint32_t kManagerBytes = 3984;
constexpr uint32_t kManagerLogicalPads = 8, kLogicalPadBytes = 196;
constexpr uint32_t kManagerPhysicalPads = 988, kPhysicalPadBytes = 212;
constexpr uint32_t kManagerProcessors = 1840, kProcessorBytes = 416;
constexpr uint32_t kManagerListeners = 3920, kListenerListBytes = 12;

// --- The pads ---

// LogicalPad::LogicalPad (sub_82591B38): vtable 0x8206CBEC, the four
// bindings (+4 to +35) cleared.
void LogicalPadConstruct(uint8_t* base, uint32_t pad);
// PhysicalPad::PhysicalPad (sub_82591B68): vtable 0x8206CBF4, +4 = 0, the
// two records at +168 and +180 cleared (sub_825A18D8).
void PhysicalPadConstruct(uint8_t* base, uint32_t pad);
// LogicalPad::Bind (sub_825918F0): binding `index` enabled (the byte at
// +4 + 8 x index = 1) with `device` (+8 + 8 x index).
void LogicalPadBind(uint8_t* base, uint32_t pad, uint32_t index, uint32_t device);
// LogicalPad::Values and PhysicalPad::Values (sub_82591B30, sub_820988B8,
// their vtables' slot 1): the pad's 40 values, +36 or +8.
uint32_t LogicalPadValues(uint32_t pad);
uint32_t PhysicalPadValues(uint32_t pad);

// --- The processors ---

// InputProcessorBase::InputProcessorBase (sub_825920D0): vtable 0x8206CC00,
// the dead zones (0.5, 0.5, 0, 0), the six vectors' w 1.0, the button words
// 0, the repeat delay 20 and period 4, the 32 counters cleared.
void InputProcessorBaseConstruct(uint8_t* base, uint32_t processor);
// InputProcessor::InputProcessor (sub_8258CE60): the base's, then vtable
// 0x8206C0C4.
void InputProcessorConstruct(uint8_t* base, uint32_t processor);
// InputProcessor::Assign (sub_82098D60): `source`'s +16 to +411 copied into
// `destination` (+128 to +263 as floats, the rest as words).
void InputProcessorAssign(uint8_t* base, uint32_t destination, uint32_t source);
// InputProcessor::AnyPressed (sub_82592500): whether any bit of `mask` is set
// in the pressed word (+264).
uint32_t InputProcessorAnyPressed(uint8_t* base, uint32_t processor, uint32_t mask);
// InputProcessor::PressedOrRepeat (sub_82592520): for `mask`'s lowest set
// bit, whether it's held (+272) on its first tick or at the repeat delay.
uint32_t InputProcessorPressedOrRepeat(uint8_t* base, uint32_t processor, uint32_t mask);
// TestPressed and TestRepeat (sub_8258CEB0, sub_8258CEE0) are the two above
// with the mask read from its address: their adapters do that.

// --- The manager's slots (vtable 0x82000F48) ---

// Slots 1, 2 and 5 (sub_82098CB8, sub_82098CD0, sub_82098FE0): processor,
// logical pad or physical pad `index`.
uint32_t ManagerProcessor(uint32_t manager, uint32_t index);
uint32_t ManagerLogicalPad(uint32_t manager, uint32_t index);
uint32_t ManagerPhysicalPad(uint32_t manager, uint32_t index);

// --- The manager ---
//
// Its constructor (sub_820991E0) registers two handlers with the message
// dispatcher (the argument block's +0) through the dispatcher's vtable
// slots +8 and +4: guest calls, made by its adapter between the two parts
// below.

constexpr uint32_t kManagerVtable = 0x82000F48;

// Input::Create's allocation (sub_820993E8): the manager's 3,984 bytes from
// operator new. nullopt where the original must run (operator new's error
// paths).
std::optional<uint32_t> ManagerAllocate(uint8_t* base);
// InputManager::InputManager (sub_820991E0) up to its handlers: vtable
// 0x82000F48, the five logical pads, four physical pads, five processors and
// five listener lists made, +4 = the argument block's +4. False, with
// nothing done, when the lists' head nodes can't be allocated natively.
bool ManagerConstructMembers(uint8_t* base, uint32_t manager, uint32_t block);
// The rest, after the handlers: logical pad i (0-3) bound (binding 0) to
// physical pad i, and the "any pad" (4) bindings 0-3 to logical pads 0-3.
void ManagerBindPads(uint8_t* base, uint32_t manager);
// InputManager::AddListener (sub_82099430, the vtable's slot 3): `listener`
// pushed at the end of pad `index`'s list and processor `index` copied into
// it (Assign). False, with nothing done, when the list isn't empty (the
// original resets its last listener first, sub_82592400, never seen in play)
// or the node can't be allocated natively.
bool ManagerAddListener(uint8_t* base, uint32_t manager, uint32_t index, uint32_t listener);

}  // namespace input
