// The post effects natively (docs/functions/post-effects.md): the 15
// effects at 0x826C5D80 (PostEffects::), the glow's and the color filter's
// setters, and the race's toggle over them. Plain C++ over guest memory
// (the effects are shared with recompiled code); the calls into code that
// still takes the context (the effects' vtable slots) go through the
// callbacks the adapters (src/hooks/post_effects.cpp) give.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace post_effects {

constexpr uint32_t kEffects = 0x826C5D80;  // 15 effect pointers
constexpr uint32_t kCount = 15;

// PostEffects::Get (sub_82133AE8): [kEffects + 4 * index] (the address
// wraps at 32 bits, as the original's).
uint32_t Get(uint8_t* base, uint32_t index);
// PostEffects::MarkUpdated (sub_82133A08): +48 = 1 on each effect.
void MarkUpdated(uint8_t* base);
// PostEffects::ToggleNoHistory (sub_82133A38): +44 = !+44 on each effect.
void ToggleNoHistory(uint8_t* base);
// ScreenObject::ToggleHidden (sub_8212EDE0): byte +25 = !+25.
void ToggleScreenObjectHidden(uint8_t* base, uint32_t object);
// Race::ToggleEffects (sub_8215B690): r3 = the race object. Returns r3 as
// the original leaves it (the race's +31160).
uint32_t ToggleRaceEffects(uint8_t* base, uint32_t race);
// PostEffects::DestroyRace (sub_82133A70): each effect without flag bit 2
// (+32 & 4) destroyed through its vtable slot 1 (`destroy(function,
// effect)` with r4 = 1, returning r3) and its pointer cleared. Returns r3 as
// the original leaves it.
uint32_t DestroyRace(uint8_t* base, const std::function<uint32_t(uint32_t, uint32_t)>& destroy);

// The glow's setters (vtable slots 2-12, sub_82136EF8 to sub_82136F48): the
// float at `offset` (8436 to 8476) = value, rounded to single.
void GlowSet(uint8_t* base, uint32_t glow, uint32_t offset, double value);
// Glow::SetFromSpeed (sub_82133878, slot 24): t from the speed, then the
// glow's slots 2, 6 and 8 called with their values (`set(function, value)`,
// r3 = the glow).
void GlowSetFromSpeed(uint8_t* base, uint32_t glow, double speed,
                      const std::function<void(uint32_t, double)>& set);

// ColorFilter::SetColor0 (sub_821350F8): +8080, +8084, +8088 = the three
// values, +8092 = 0.
void ColorFilterSetColor0(uint8_t* base, uint32_t filter, double r, double g, double b);
// ColorFilter::Set8112, Set8116, Set8120 (sub_82135138, sub_82135140,
// sub_82135148): the float at `offset` = value.
void ColorFilterSet(uint8_t* base, uint32_t filter, uint32_t offset, double value);


// The effects' recording helpers (r3 = the effect, not read by those that
// take only the list): each writes its slots on the command list `list`
// (command_list::AllocateSlots, without the list's diagnostics) and
// returns r3 as the original leaves it (its last slot), or nothing, with
// nothing written, when the list hasn't room for all its slots (the
// original prints a message there and writes through address 0: left to
// it).
//
// PostEffect::RecordBeginStates (sub_82133340): 24 slots (command 34's
// one left as it was).
std::optional<uint32_t> RecordBeginStates(uint8_t* base, uint32_t list);
// PostEffect::RecordEndStates (sub_82133608): 13 slots (command 35's one
// left as it was).
std::optional<uint32_t> RecordEndStates(uint8_t* base, uint32_t list);
// PostEffect::RecordTargets (sub_82133790): render targets 0-3 = surface,
// the depth surface: 7 slots.
std::optional<uint32_t> RecordTargets(uint8_t* base, uint32_t list, uint32_t surface,
                                      uint32_t depth);
// PostEffect::RecordPixelConstants (sub_82133D00): command 7 (pixel floats,
// `reg`, `count`) and one slot copied from the 16 bytes at `source`.
std::optional<uint32_t> RecordPixelConstants(uint8_t* base, uint32_t list, uint32_t source,
                                             uint32_t reg, uint32_t count);
// PostEffect::RecordPixelBool (sub_82133D70): command 7 (pixel bool, `reg`)
// and 4 bytes holding `value`.
std::optional<uint32_t> RecordPixelBool(uint8_t* base, uint32_t list, uint32_t value, uint32_t reg);
// PostEffect::RecordTexelSize (sub_82133DD8): c32 = (1/width, 1/height, 0,
// 0) through RecordPixelConstants' slots.
std::optional<uint32_t> RecordTexelSize(uint8_t* base, uint32_t list, double width, double height);
// PostEffect::AddQuad (sub_82133E30): three 40-byte vertices in the
// effect's vertex buffers; returns the first. Never fails.
uint32_t AddQuad(uint8_t* base, uint32_t effect, uint32_t width, uint32_t height, double value,
                 bool at_view);
// PostEffect::RecordResolve (sub_82133F58): command 25 and its 68-byte
// item (6 slots).
std::optional<uint32_t> RecordResolve(uint8_t* base, uint32_t effect, uint32_t list,
                                      uint32_t texture, uint32_t flags, uint32_t more_flags,
                                      bool by_view);

}  // namespace post_effects
