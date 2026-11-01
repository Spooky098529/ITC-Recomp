// The graphics device's shader setters and the render queue's pass lists as
// plain C++ (docs/functions/device-shaders.md). The setters write the bound
// object (device +12436 the vertex shader, +12432 the pixel shader, +11552
// the vertex declaration), their bits in the shader state's dirty mask (+16)
// and, for a shader, the register patches its object carries applied to the
// register shadow at device +1024, exactly as the originals. Adapters:
// src/hooks/device_shaders.cpp (--native_device_shaders).

#pragma once

#include <cstdint>
#include <functional>

namespace graphics::device_shaders {

// The guest calls the shader setters keep, made by their adapters:
// Resource::Release (sub_822A67B0; its override, the lock-free count) with
// its r3 returned, and memcpy (sub_825A7410) where native::Buffers_Memcpy
// refuses a copy (`at` the original's return address).
struct ShaderCalls {
  std::function<uint32_t(uint32_t resource)> release;
  std::function<void(uint32_t destination, uint32_t source, uint32_t size, uint32_t at)> memcpy;
};

// Device::SetVertexShader (sub_822A8808): r3 = the device, r4 = a vertex
// shader or 0. Resource::AddRef on the new shader (refcount::AddRef) and
// Release on the bound one (+12436), both skipped when they're the same
// object (src/native/refcount.cpp's rebind skip, kept: the pair changes
// nothing); +12436 = the shader, +10810's top bit cleared, +16 bit 51; then
// the shader's patches (at shader + 592 + its +604): device +0 ORed with the
// float constant mask, +24 bit 1 when the doubleword at +8 isn't 0, then the
// copy records (memcpy into the shadow) up to one with a count of 0, and the
// masked records (shadow word = (word AND first) OR second) up to another
// count of 0 or the end. Returns r3 as the original leaves it (AddRef's,
// Release's or the last memcpy's).
uint32_t SetVertexShader(uint8_t* base, uint32_t device, uint32_t shader,
                         const ShaderCalls& calls);
// sub_822A8958 (D3DDevice_SetVertexShader): device +13156 = the shader, then
// SetVertexShader (the original's tail call).
uint32_t SetVertexShaderEntry(uint8_t* base, uint32_t device, uint32_t shader,
                              const ShaderCalls& calls);
// Device::SetPixelShader (sub_822A8618): +12432 = the shader (no reference
// taken), +16 bits 52 and 49; the shader's patches (at shader + 56 + its
// +68) as SetVertexShader's, into device +8's mask. Returns r3 as the
// original leaves it (the device or the last memcpy's).
uint32_t SetPixelShader(uint8_t* base, uint32_t device, uint32_t shader,
                        const ShaderCalls& calls);
// Device::SetVertexDeclaration (sub_822A8960): +11552 = the declaration,
// +16 bit 51.
void SetVertexDeclaration(uint8_t* base, uint32_t device, uint32_t declaration);

// PassList::Clear (sub_8258EB38): each bucket's (+12 of them at +24, 16
// bytes each) +0 and +4 = 0; +16, +28, +32 = 0.
void PassListClear(uint8_t* base, uint32_t list);
// PassList::PassList (sub_82590B38): +0 the vtable 0x8206CB88, +4 = 1 (a
// byte), +8 = `items`, +12 = `buckets`, +16 = 0; +20 = operator new[](items
// x 20), +24 = operator new[](buckets x 16) (-1 asked for past the 32-bit
// sizes); Clear; then each bucket's +8 = the next bucket and +12 = 0, the
// last's +8 and +12 = 0. False where operator new[] would take its error path
// (the original then runs from the start; never in play).
bool PassListConstruct(uint8_t* base, uint32_t list, uint32_t items, uint32_t buckets);
// RenderQueue::RenderQueue (sub_825915E0): +0 the vtable 0x8206CB8C; +4 =
// operator new(88) with PrimitiveDrawer::PrimitiveDrawer on its +4 and its
// vector (+72, +76, +80) = 0; +8 and +12 = operator new(36) each made a
// PassList(4096, 1024) (0 when operator new gives 0); both cleared again;
// the state's +0 = 0, +84 = 0.0 (the image's 0x82000A34), +48 to +64 = 0.
// False where an operator new would take its error path (as above).
bool RenderQueueConstruct(uint8_t* base, uint32_t queue);

}  // namespace graphics::device_shaders
