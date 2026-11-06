// The render command replay's single commands as plain C++
// (docs/functions/single-commands.md): the handlers of the table 0x8265ED48
// that run in play, each called with the slot ([id, arguments], 16 bytes,
// its data in the slots after) and returning how many of the following
// slots it used. They find the device at [0x8296BD90], the primitive drawer
// at [0x82970BB8] and its state at [0x82970BBC] (RenderQueue::Replay's
// globals). Every read and write is the original's, in its order. Calls to
// code still recompiled (the drawer's and the object's vtable slots, the
// shader constant setters not seen in play) are guest calls through `call`,
// from frames made where the originals make theirs; the device's setters
// that are native (src/graphics/device_state.h) are called as plain C++.
// Adapters: src/hooks/single_commands.cpp (--native_single_commands), and
// command 15's in src/native/replay_hooks.cpp, inside its wrap.

#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>

namespace graphics::single_commands {

// A guest call to `function` with r1 = `sp`, lr = `return_address` and r3,
// r4, ... set from `args` (the others as they are). Returns r3.
using GuestCall = std::function<uint64_t(uint32_t function, uint32_t sp, uint32_t return_address,
                                         std::initializer_list<uint64_t> args)>;

// The handlers' addresses.
constexpr uint32_t kInlineVertices = 0x8258E8E8;      // 5
constexpr uint32_t kSetShaderConstants = 0x8258F0F0;  // 7
constexpr uint32_t kSetUpDrawer = 0x8258E958;         // 13, 14
constexpr uint32_t kDrawerDraw = 0x8258F5C0;          // 15
constexpr uint32_t kCallback = 0x8258E9B8;            // 28

// Single::InlineVertices (sub_8258E8E8, 5): the drawer's state +12 = the
// slot + 16 (the vertices), +16 = the slot's +4 (their count). Returns the
// slots the vertices take, (count x 24 + 15) >> 4 (in 32 bits).
uint32_t InlineVertices(uint8_t* base, uint32_t slot);

// Single::SetUpDrawer (sub_8258E958, 13 and 14), in its 96-byte frame below
// `sp`: the drawer's state +0, +4, +8 = the slot's +4 (the primitive type),
// +8 and +12 (the shaders' indices); then the drawer's vtable slot 2
// (PrimitiveDrawer::SetShaders) with r4 = the device, r5 and r6 the indices.
// Returns 0.
uint32_t SetUpDrawer(uint8_t* base, uint32_t slot, uint32_t sp, const GuestCall& call);

// Single::SetShaderConstants (sub_8258F0F0, 7), in its 224-byte frame: the
// slot's +4 the shader (0 vertex, 1 pixel; another returns 0), +8 the type
// (0 the loop constants, 1 the floats, 2 the bools, 3 the debug string, 4 a
// bit mask expanded into words at the frame's +80; above 4 nothing), byte
// +12 the first register, byte +13 the count, the data from the slot + 16.
// The float setters and the pixel shader's bool setter are their plain C++
// (src/graphics/device_state.h), OutputDebugStringA helpers::DebugOutput;
// the loop constant setters and the vertex shader's bool setter (not seen in
// play) guest calls. Returns byte +13.
uint32_t SetShaderConstants(uint8_t* base, uint32_t slot, uint32_t sp, const GuestCall& call);

// Single::DrawerDraw (sub_8258F5C0, 15), in its 112-byte frame: nothing
// when the drawer's state +16 (the vertex count) is 0. Else the viewport
// setting kept, set to 0 when the state's +4 is 1 (else 1), the drawer's
// vtable slot 3 (PrimitiveDrawer::Draw) with r4 = the device, r5 = the
// state's +0, r6 = +12, r7 = +16; then stream 0 and texture 0 unset and the
// viewport setting put back (the device's setters as their plain C++; the
// originals, through their adapters, where SetStreamSource or SetTexture
// would fence on a full fence list). Returns 0. Its callers mark it as
// command 15 (native::g_current_command) around it, for the renderer's
// inline draw (src/native/replay_hooks.cpp).
uint32_t DrawerDraw(uint8_t* base, uint32_t sp, const GuestCall& call);

// Single::Callback (sub_8258E9B8, 28), in its 96-byte frame: the slot's +4
// an object, whose vtable slot 0 is called (a guest call) with r4 = the
// device. In play the movie (Movie::RenderFrame, overridden: our renderer
// draws the movie's frame there). Returns 0.
uint32_t Callback(uint8_t* base, uint32_t slot, uint32_t sp, const GuestCall& call);

}  // namespace graphics::single_commands
