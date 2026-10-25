// The app framework natively (docs/functions/framework.md): the framework
// object's getters and its interfaces' this-adjusting thunks, the debug
// text's position. Plain C++ over the guest layout; the adapters are in
// src/hooks/framework.cpp.

#pragma once

#include <cstdint>

namespace framework {

// The framework's flags read by App::Run each pass: the byte at +133
// (sub_82090760) and at +134 (sub_82090768).
uint32_t Flag133(uint8_t* base, uint32_t object);
uint32_t Flag134(uint8_t* base, uint32_t object);

// Getters the linker folded (one function for every class with a word at
// that offset): +60 (sub_82090770), +64 (sub_82090778), +88 (sub_82090788),
// +68 (sub_8248F030), +44 (sub_824892F8).
uint32_t Word60(uint8_t* base, uint32_t object);
uint32_t Word64(uint8_t* base, uint32_t object);
uint32_t Word88(uint8_t* base, uint32_t object);
uint32_t Word68(uint8_t* base, uint32_t object);
uint32_t Word44(uint8_t* base, uint32_t object);

// The interfaces' thunks: `self` is the interface; each adjusts it to the
// framework's sub-object and calls one of the getters above.
uint32_t InputManagerFrom36(uint8_t* base, uint32_t self);    // sub_82090830: -12, Word64
uint32_t RendererFrom52(uint8_t* base, uint32_t self);        // sub_82090828: -44, Word68
uint32_t WorkerPoolFrom52(uint8_t* base, uint32_t self);      // sub_82090838: -28, Word44
uint32_t RendererOrTextFrom24(uint8_t* base, uint32_t self);  // sub_82090860: -16, Word68
uint32_t SceneFrom52(uint8_t* base, uint32_t self);           // sub_82090868: -32, Word60
uint32_t RendererFrom36(uint8_t* base, uint32_t self);        // sub_820908E0: -28, Word68
uint32_t WorkerPoolFrom36(uint8_t* base, uint32_t self);      // sub_820908E8: -12, Word44
uint32_t SceneFrom36(uint8_t* base, uint32_t self);           // sub_82090908: -16, Word60

// The debug font's cursor and line start (0x8296BCE0: +0, +4 and +8, +12)
// set to (x, y) (sub_8258BE08, DebugFont::SetPosition).
constexpr uint32_t kDebugFontPosition = 0x8296BCE0;
void DebugFontSetPosition(uint8_t* base, uint32_t x, uint32_t y);
// DebugText::SetPosition (sub_82091050, vtable slot 2): `x` and `y` point to
// the words.
void DebugTextSetPosition(uint8_t* base, uint32_t x, uint32_t y);

}  // namespace framework
