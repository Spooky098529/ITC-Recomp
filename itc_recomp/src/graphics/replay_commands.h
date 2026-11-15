// The render command replay's command handlers as plain C++
// (docs/functions/functions.md, "Command::*"; the commands in
// docs/gpu-functions.md, "Command list format"): the handlers of the table
// 0x8265EDE0, each called with the command's payload (r3), its item count
// (r4) and, for some, a third word (r5), and returning how many 16-byte
// slots the payload took. They find the device at [0x8296BD90], the
// primitive drawer at [0x82970BB8] and its state at [0x82970BBC]
// (RenderQueue::Replay's globals), as the single commands do
// (src/graphics/single_commands.h). Every read and write is the original's,
// in its order. The device's setters that are native
// (src/graphics/device_state.h) and its render target getters
// (src/graphics/resources.h) are called as plain C++ (the precedent of the
// single commands); everything else (the setters reached through the
// device's tables, the shader setters, the draws, memcpy, Release, Clear,
// Resolve, the tiling, the drawer's and objects' vtable slots) is a guest
// call through `call`, from a frame made where the original makes its own,
// so the adapters and our renderer's wraps around those run as before.
// Adapters: src/native/replay_hooks.cpp (inside its wraps, --native_replay_commands)
// and command 7's in src/interpolation/blend_state.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <optional>

namespace graphics::replay_commands {

// A guest call to `function` with r1 = `sp`, lr = `return_address`, r3, r4,
// ... set from `args` (up to r10; the others as they are) and f1 when given.
// Returns r3.
using GuestCall =
    std::function<uint64_t(uint32_t function, uint32_t sp, uint32_t return_address,
                           std::initializer_list<uint64_t> args, std::optional<double> f1)>;

// The handlers, by command id. `items` is r3 (the payload), `count` r4,
// `sp` the stack pointer at the call. Each returns r3 as the original does.

// 1, sub_8258FBD0: for each 16-byte item, the render state setter at
// [device + item +0 + 56] (a guest call) with r4 = item +4.
uint32_t SetRenderStates(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                         const GuestCall& call);
// 2, sub_8258FC30: for each item, the sampler state setter at
// [device + item +4 + 440] with r4 = item +0 (the sampler), r5 = item +8.
uint32_t SetSamplerStates(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                          const GuestCall& call);
// 3, sub_8258FC90: Device::SetBlendControl(item +0, item +4) for each item.
uint32_t SetBlendControls(uint8_t* base, uint32_t items, uint32_t count);
// 4, sub_8258FCE0: Device::SetTexture(item +0, item +4) with the stage's
// dirty bit, 1 << 63 >> (stage + 20) (0 from 64 on).
uint32_t SetTextures(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                     const GuestCall& call);
// 5, sub_8258E9F8: the drawer's state +12 = the payload, +16 = the count.
// Returns (count x 24 + 15) >> 4.
uint32_t InlineVertices(uint8_t* base, uint32_t items, uint32_t count);
// 6, sub_8258EA20: the drawer's state +20, +24, +28 = r3, r4, r5. Returns
// ((r5 ? 4 : 2) x r4 + 15) >> 4.
uint32_t InlineIndices(uint8_t* base, uint32_t items, uint32_t count, uint32_t third);
// 7, sub_8258FD48: r5 = stage << 24 | type << 16 | first << 8 (the byte at
// bits 8-15); r4 the register count. Stage 0 (vertex) or 1 (pixel), else
// returns 0. Type 0 the loop constants (sub_822A84C8, sub_822A8518), 1 the
// floats with the dirty mask of their 4-register blocks, 2 the bools
// (sub_822A8418 a guest call; the pixel shader's plain C++), 3
// OutputDebugStringA("aaaa...\n"), above 3 nothing. Returns r4.
uint32_t SetShaderConstants(uint8_t* base, uint32_t payload, uint32_t count, uint32_t packed,
                            uint32_t sp, const GuestCall& call);
// 8, 9, 10 (sub_8258FF50, sub_8258FFA0, sub_8258FFF0): the vertex shader
// (D3DDevice_SetVertexShader sub_822A8958), the pixel shader (sub_822A8618)
// and the vertex declaration (sub_822A8960) set to item +0, for each item.
uint32_t SetVertexShaders(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                          const GuestCall& call);
uint32_t SetPixelShaders(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                         const GuestCall& call);
uint32_t SetVertexDeclarations(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                               const GuestCall& call);
// 11, sub_82590040: Device::SetStreamSource(item +0 the stream, +4, +8, +12)
// with the stream's dirty bit, 1 << 63 >> (((95 - stream) x 21846 >> 16) +
// 20).
uint32_t SetStreamSources(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                          const GuestCall& call);
// 12, sub_825900B8: Device::SetIndices(item +0).
uint32_t SetIndices(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                    const GuestCall& call);
// 13 and 14, sub_8258EA58: the drawer's state +4, +8, +0 = item +4, +8, +0,
// then the drawer's vtable slot 2 (PrimitiveDrawer::SetShaders) with r4 =
// the device, r5 = item +4, r6 = item +8.
uint32_t SetUpDrawer(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                     const GuestCall& call);
// 15, sub_82590108, `count` times: when the drawer's state +16 isn't 0,
// the viewport setting read (and dropped), set to 0 when the state's +4 is
// 1 (else 1), the drawer's vtable slot 3 (PrimitiveDrawer::Draw) with r4 =
// the device, r5, r6, r7 = the state's +0, +12, +16, then set to 1.
uint32_t DrawerDraw(uint8_t* base, uint32_t count, uint32_t sp, const GuestCall& call);
// 16, sub_825901B0: as 15 with slot 4 (r5 to r10 = the state's +0, +12,
// +16, +20, +24, +28), the viewport setting put back after.
uint32_t DrawerDrawIndexed(uint8_t* base, uint32_t count, uint32_t sp, const GuestCall& call);
// 17, sub_82590268: DrawVertices (sub_825BE300) for each 16-byte item, r4 =
// item +0 (the primitive type), r5 = +4, r6 = the vertex count from +8 (the
// type's multiplier and addend at 0x82000950).
uint32_t DrawVertices(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                      const GuestCall& call);
// 18, sub_825902E0: DrawIndexedVertices (sub_825BE708) for each 12-byte item,
// r4 = item +0, r5 = item +4 >> 16 (signed), r6 = its low half, r7 = the
// index count from +8.
uint32_t DrawIndexedVertices(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                             const GuestCall& call);
// 19, sub_82590360: for each 12-byte item, BeginVertices (sub_825BD8F0) with
// r4 = item +0, r5 = the halfword +4 (the count), r6 = +6 (the stride); when
// it returns an address, memcpy of count x stride bytes from item +8 there
// and EndVertices (sub_825BDDA0).
uint32_t DrawVerticesUP(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                        const GuestCall& call);
// 20, sub_825903D8: Resource::Release (sub_822A67B0) of each item's +0 that
// isn't 0.
uint32_t ReleaseResources(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                          const GuestCall& call);
// 21, sub_8258EAD0: nothing; returns (r4 + 15) >> 4.
uint32_t Skip(uint32_t count);
// 22, sub_82591798: for each item, the object at item +4: its vtable slot 2;
// when that returns 0 in its low byte and item +8 is 1, its slot 1, then
// PtrVector::PushBack (sub_82591540) of item + 4 onto the list at
// [0x8296BD8C], then its slot 3.
uint32_t DeferRelease(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                      const GuestCall& call);
// 23, sub_82590428: Device::SetRenderTarget_External(item +0, item +4).
uint32_t SetRenderTargets(uint8_t* base, uint32_t items, uint32_t count);
// 24, sub_82590478: Device::SetDepthStencilSurface(item +0).
uint32_t SetDepthStencilSurfaces(uint8_t* base, uint32_t items, uint32_t count);
// 25, sub_825904C8: Device::Resolve (sub_822AFA58) for each 68-byte item:
// the flags at +4, by the bits of +0 the source rectangle (+8), the
// destination texture (+24), the destination point (+28), the clear color
// (the bytes +44 to +47 times the float at 0x82002200, in the frame's +144)
// and the parameters (+56, the frame's +100); +36 and +40 (level, slice),
// the float +48 (clear Z) and +52 (the frame's +92). Returns (count x 68 +
// 15) >> 4.
uint32_t Resolves(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                  const GuestCall& call);
// 26, sub_82590620: Device::Clear (sub_822A04F8) for each 280-byte item: r4
// = +4 the rectangle count, r5 = item + 12 when it isn't 0, r6 = +8 the
// flags, r7 = +268 the color, f1 = the float +272, r9 = +276, r10 = 0.
// Returns (count x 280 + 15) >> 4.
uint32_t Clears(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                const GuestCall& call);
// 27, sub_82590698: for each 32-byte item, BeginIndexedVertices
// (sub_825BDDC0) with r4 to r9 = item +0 to +20 and the two addresses it
// returns at the frame's +96 and +100 (its 0 meaning success); then the
// indices (+20 x (+16 == 1 ? 2 : 4) bytes from +24) and the vertices (+12 x
// +8 bytes from +28) copied there with memcpy, and EndVertices. Returns
// (count x 32 + 15) >> 4.
uint32_t DrawIndexedVerticesUP(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                               const GuestCall& call);
// 28, sub_8258EAE0: for each item, the object at item +4: its vtable slot 0
// with r4 = the device.
uint32_t Callbacks(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                   const GuestCall& call);
// 29, sub_82590768: for each 4-byte item, by its bits 0-3 the render targets
// 0-3 set from the saved set at [0x82970BC0] (+0 to +12), and by bit 7 the
// depth surface from its +16.
uint32_t RestoreSurfaces(uint8_t* base, uint32_t items, uint32_t count);
// 30, sub_82590860: Device::SetViewport for each 16-byte item, from the
// halfwords +0 to +6 (x, y, width, height) and the floats +8 and +12 (min
// and max Z), built in the frame's +80.
uint32_t SetViewports(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp);
// 31, sub_825908E0: for each 8-byte item, the scissor test enabled and the
// scissor rectangle set from its four signed halfwords (in the frame's +80).
uint32_t SetScissorRects(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp);
// 32, sub_82590968: Device::BeginTiling (sub_825BED08) for each 12-byte
// item, r4 to r6 = +0 to +8, r7 = four copies of the float at 0x82000A34
// (the frame's +80), r9 = 0, f1 = that float.
uint32_t BeginTiling(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                     const GuestCall& call);
// 33, sub_825909E8: Device::EndTiling (sub_825BF200) for each item, r4 =
// +0, r5 = 0, r6 = +4, r7 as 32's, r9 = r10 = 0, f1 that float.
uint32_t EndTiling(uint8_t* base, uint32_t items, uint32_t count, uint32_t sp,
                   const GuestCall& call);
// 34, sub_82590A68: the device's viewport, render targets 0-3 and depth
// surface saved at 0x8296BD60 (+0, +24 to +36, +40; the device at +48).
uint32_t SaveSurfaces(uint8_t* base, uint32_t count, uint32_t sp, const GuestCall& call);
// 35, sub_82590AD0: what 34 saved set again (render targets, depth
// surface, viewport).
uint32_t RestoreSavedSurfaces(uint8_t* base, uint32_t count);

}  // namespace graphics::replay_commands
