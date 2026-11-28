// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 44's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"

#include <optional>

#include "guest.h"
#include "lifted/entries.h"
#include "lifted/originals.h"

#include "graphics/command_buffer.h"
#include "graphics/device_shaders.h"
#include "graphics/device_state.h"
#include "graphics/render_queue.h"
#include "graphics/resources.h"
#include "hooks/command_buffer.h"
#include "hooks/device_state_calls.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/models.h"
#include "hooks/stand_models.h"
#include "input/input_manager.h"
#include "schema/command_buffer.h"
#include "schema/device_state.h"
#include "schema/input.h"
#include "std_list/ptr_list.h"
#include "vehicle/vehicle.h"

namespace cb = graphics::command_buffer;

// --- The graphics library's command buffer (docs/functions/command-buffer.md) ---

// Device::SaveSegment, CommandChain::Close, Device::RestoreSegment: r3 the
// device (or chain), left as it came (the originals don't touch it).
ITC_DIRECT_ENTRY(8229DBF0) { cb::SaveSegment(ctx.r3.u32); }
ITC_DIRECT_ENTRY(8229DFB8) { cb::CloseChain(ctx.r3.u32); }
ITC_DIRECT_ENTRY(8229E0C0) { cb::RestoreSegment(ctx.r3.u32); }

// Device::NextSpace: r3 the device, r4 the kind, r5 the size's address (in,
// and out for a segment that's had: the word unchanged when it failed), r6
// the alignment; r3 the space (the deleted adapter's mapping).
ITC_DIRECT_ENTRY(8229E100) {
  uint32_t size = guest::LoadU32(ctx.r5.u32);
  const uint32_t space = cb::NextSpace(ctx.r3.u32, ctx.r4.u32, size, ctx.r6.u32);
  if (!(ctx.r4.u32 & 1)) {
    guest::StoreU32(ctx.r5.u32, size);
  }
  ctx.r3.u64 = space;
}

// Device::NextSegment: r3 the device, r4 the words; r3 the segment. The
// step runs the original with a recording command buffer (never in play).
ITC_DIRECT_ENTRY(8229EC00) { hooks::NextCommandSegment(ctx); }

// CommandChain::Init: r3 the chain, r4 the device; r3 NewBlock's cursor. A
// recording command buffer (never in play) runs the original, as the
// deleted adapter did.
ITC_DIRECT_ENTRY(8229ED60) {
  if (schema::Device(ctx.r4.u32).recorder().address != 0) {
    lifted::original::c8229ED60(ctx);
    return;
  }
  ctx.r3.u64 = cb::InitChain(ctx.r3.u32, ctx.r4.u32);
}

// Device::KickSegment: r3 the device; r3 as the original leaves it. A
// recording command buffer (never in play) runs the original.
ITC_DIRECT_ENTRY(8229EDA8) {
  if (cb::Recording(ctx.r3.u32)) {
    lifted::original::c8229EDA8(ctx);
    return;
  }
  const cb::Kick kick = hooks::CommandBufferKick(ctx);
  const uint32_t device = ctx.r3.u32;
  ctx.r3.u64 = cb::KickSegment(device, kick);
}

// --- The device's state (docs/functions/device-state.md) ---

// sub_8229F680: r3 the device, left as the device (the original's last call,
// Device::SetScreenScissor, is Dropped and leaves it). Its flush helpers are
// guest calls from a frame like the original's 192 bytes.
ITC_DIRECT_ENTRY(8229F680) {
  const uint32_t device = ctx.r3.u32;
  guest_call::InFrame(ctx, 192, 0x8229F6B8, [&](uint32_t) {
    graphics::device_state::FlushForGprAllocation(device,
                                                  hooks::DeviceStateCalls(ctx, device));
  });
  ctx.r3.u64 = device;
}

// --- The graphics resources (docs/functions/graphics-resources.md) ---

// Texture::BaseSize: r3 the texture (left as it came), r4, r5, r6 the width,
// height and depth out.
ITC_DIRECT_ENTRY(822A4348) {
  const graphics::Extent size = graphics::TextureBaseSize(ctx.r3.u32);
  guest::StoreU32(ctx.r4.u32, size.width);
  guest::StoreU32(ctx.r5.u32, size.height);
  guest::StoreU32(ctx.r6.u32, size.depth);
}

// TextureLayout::LevelSize: r3, r4, r5 the width, height and depth's
// addresses (aligned in place), r6 the bits a pixel, r7 the format, r8 the
// dimension, r9 tiled; r3 the level's bytes.
ITC_DIRECT_ENTRY(822A45B8) {
  uint32_t width = guest::LoadU32(ctx.r3.u32);
  uint32_t height = guest::LoadU32(ctx.r4.u32);
  uint32_t depth = guest::LoadU32(ctx.r5.u32);
  const uint32_t bytes = graphics::TextureLevelSize(width, height, depth, ctx.r6.u32,
                                                    ctx.r7.u32, ctx.r8.u32, ctx.r9.u32);
  guest::StoreU32(ctx.r3.u32, width);
  guest::StoreU32(ctx.r4.u32, height);
  guest::StoreU32(ctx.r5.u32, depth);
  ctx.r3.u64 = bytes;
}

// TextureLayout::MipTailOffset: r3 the level's index in the tail, r4-r6 the
// tail's width, height, depth, r7 the pitch, r8 the slice's bytes, r9 the
// format; out: r10 the blocks across, the caller's stack +84 the blocks
// down, +92 the slices (in the original's order); r3 the bytes from the
// tail's start.
ITC_DIRECT_ENTRY(822A4A40) {
  const graphics::MipTailPlace place =
      graphics::MipTailOffset(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                              ctx.r8.u32, ctx.r9.u32);
  const uint32_t stack = ctx.r1.u32;
  guest::StoreU32(ctx.r10.u32, place.x);
  guest::StoreU32(guest::LoadU32(stack + 84), place.y);
  guest::StoreU32(guest::LoadU32(stack + 92), place.z);
  ctx.r3.u64 = place.offset;
}

// Texture::LevelDesc: r3 the texture, r4 the level, r5 the description; r3
// the type.
ITC_DIRECT_ENTRY(822A4F60) {
  ctx.r3.u64 = graphics::TextureLevelDesc(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// Texture::LockRect: r3 the texture, r4 the face, r5 the level, r6 the
// locked rect out, r7 a rectangle or 0, r8 the flags; r3 the lock's
// address. Levels from 2^31 on run the original (Texture::LevelOffset's
// uninitialized stack words), as LockRectLevel's adapter does.
ITC_DIRECT_ENTRY(822A5578) {
  if (ctx.r5.u32 >= 0x80000000u) {
    lifted::original::c822A5578(ctx);
    return;
  }
  ctx.r3.u64 = graphics::LockTextureRect(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                         ctx.r7.u32, ctx.r8.u32);
}

// TextureHeader::Set: r3 the type, r4-r6 width, height, depth, r7 levels, r8
// usage, r9 format, r10 packed mips; the caller's stack +84 the border, +92
// the field, +100 the pitch, +108 the header, +116 and +124 the base and mip
// sizes out (stored always, as the original). Types other than those it
// knows run the original (an uninitialized stack word there).
ITC_DIRECT_ENTRY(822A5608) {
  if (!graphics::TextureTypeKnown(ctx.r3.u32)) {
    lifted::original::c822A5608(ctx);
    return;
  }
  const uint32_t stack = ctx.r1.u32;
  const graphics::TextureHeaderSizes sizes = graphics::SetTextureHeader(
      ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32, ctx.r8.u32, ctx.r9.u32,
      ctx.r10.u32, guest::LoadU32(stack + 84), guest::LoadU32(stack + 92),
      guest::LoadU32(stack + 100), guest::LoadU32(stack + 108));
  guest::StoreU32(guest::LoadU32(stack + 116), sizes.base_size);
  guest::StoreU32(guest::LoadU32(stack + 124), sizes.mip_size);
  ctx.r3.u64 = sizes.r3;
}

// SurfaceHeader::Set: r3 width, r4 height, r5 format, r6 the multisample
// type, r7 the parameters (or 0), r8 the header; r9, r10 the EDRAM tiles
// and the hi-z size out; r3 the tiles (as the original leaves it).
ITC_DIRECT_ENTRY(822A5910) {
  const graphics::SurfaceSizes sizes = graphics::SetSurfaceHeader(
      ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.s32, ctx.r7.u32, ctx.r8.u32);
  guest::StoreU32(ctx.r9.u32, sizes.tiles);
  guest::StoreU32(ctx.r10.u32, sizes.hi_z);
  ctx.r3.u64 = sizes.tiles;
}

// XGSetTextureHeaderEx: r3-r5 width, height, depth, r6 levels, r7 usage, r8
// format, r9 packed mips, r10 the border; the caller's stack +84 the field,
// +92 the base address, +100 the mip address, +108 the pitch, +116 the type,
// +124 the header out, +132 and +140 the base and mip sizes out; r3 the
// base size plus the mip size. A type it doesn't know, or a header copy
// native code may not do (XGSetTextureHeader's adapter's check), runs the
// original.
ITC_DIRECT_ENTRY(8235A720) {
  const uint32_t stack = ctx.r1.u32;
  graphics::XgTextureHeader args{};
  args.width = ctx.r3.u32;
  args.height = ctx.r4.u32;
  args.depth = ctx.r5.u32;
  args.levels = ctx.r6.u32;
  args.usage = ctx.r7.u32;
  args.format = ctx.r8.u32;
  args.packed = ctx.r9.u32;
  args.border = ctx.r10.u32;
  args.field = guest::LoadU32(stack + 84);
  args.base_address = guest::LoadU32(stack + 92);
  args.mip_address = guest::LoadU32(stack + 100);
  args.pitch = guest::LoadU32(stack + 108);
  args.type = guest::LoadU32(stack + 116);
  args.header = guest::LoadU32(stack + 124);
  args.base_size_out = guest::LoadU32(stack + 132);
  args.mip_size_out = guest::LoadU32(stack + 140);
  if (!graphics::TextureTypeKnown(args.type) || !graphics::XgHeaderCopyFits(args.header)) {
    lifted::original::c8235A720(ctx);
    return;
  }
  ctx.r3.u64 = graphics::XGSetTextureHeaderEx(args);
}

// --- The render queue's pass lists (docs/functions/device-shaders.md) ---

// PassList::Clear: r3 the list, left as it came.
ITC_DIRECT_ENTRY(8258EB38) { graphics::device_shaders::PassListClear(ctx.r3.u32); }

// PassList::Add, a leaf: r3 the list, r4 {the chunk, the key}, r5 {the
// sequence, a word}, r6 the bucket; stored at the caller's sp + 32 and + 40
// as the original does; r3 0 or 1.
ITC_DIRECT_ENTRY(8258EB88) {
  ctx.r3.u64 = graphics::render_queue::PassListAdd(ctx.r3.u32, ctx.r4.u64, ctx.r5.u64,
                                                   ctx.r6.u32, ctx.r1.u32);
}

// PassList::PassList: r3 the list (returned), r4 the items, r5 the buckets.
// Over the allocation layer only; where operator new[] would take its error
// path (never in play), the original.
ITC_DIRECT_ENTRY(82590B38) {
  const uint32_t list = ctx.r3.u32;
  if (!hooks::NativeHeapActive() ||
      !graphics::device_shaders::PassListConstruct(list, ctx.r4.u32, ctx.r5.u32)) {
    lifted::original::c82590B38(ctx);
    return;
  }
  ctx.r3.u64 = list;
}

// PtrVector::Insert: r3 the answer's address (returned), r4 the vector, r5
// where, r6 the value's address; the original where InsertN refuses, or
// where PushBack's adapter wouldn't allocate (hooks::ModelsNativeLifetime).
ITC_DIRECT_ENTRY(825911C8) {
  const std::optional<uint32_t> answer =
      hooks::ModelsNativeLifetime()
          ? graphics::render_queue::PtrVectorInsert(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                    ctx.r6.u32)
          : std::nullopt;
  if (!answer) {
    lifted::original::c825911C8(ctx);
    return;
  }
  ctx.r3.u64 = *answer;
}

// --- The stand's scene model holders (docs/functions/stand-models.md): their
// register steps (src/hooks/stand_models.cpp; the flagged ones run the
// originals with their callees' flags off) ---

ITC_DIRECT_ENTRY(820DF1A0) { hooks::StandModelsLightValue(ctx); }
ITC_DIRECT_ENTRY(820E7380) { hooks::StandModelsSetSizes(ctx); }
ITC_DIRECT_ENTRY(820E7470) { hooks::StandModelsSetTireGrades(ctx); }
ITC_DIRECT_ENTRY(82178B98) { hooks::StandModelsFitAxles(ctx); }
ITC_DIRECT_ENTRY(82179130) { hooks::StandModelsSetTireGrade(ctx); }
ITC_DIRECT_ENTRY(82179420) { hooks::StandModelsSetLightsOn(ctx); }
ITC_DIRECT_ENTRY(82179998) { hooks::StandModelsAddSecondWheels(ctx); }
ITC_DIRECT_ENTRY(821799D8) { hooks::StandModelsApplyWheelPaint(ctx); }
ITC_DIRECT_ENTRY(82179C48) { hooks::StandModelsSecondWheelsPending(ctx); }
ITC_DIRECT_ENTRY(8217DD38) { hooks::StandModelsSetWheel(ctx); }

// --- The pads and processors (docs/functions/input.md) ---

// InputProcessor::InputProcessor: r3 the processor, left as it came (the
// original returns it).
ITC_DIRECT_ENTRY(8258CE60) { input::InputProcessorConstruct(ctx.r3.u32); }

// LogicalPad::Bind: r3 the pad, r4 the index's address, r5 the device; r3
// left as it came.
ITC_DIRECT_ENTRY(825918F0) {
  input::LogicalPadBind(ctx.r3.u32, schema::InputIndex(ctx.r4.u32).index(), ctx.r5.u32);
}

// LogicalPad::LogicalPad: r3 the pad, left as it came.
ITC_DIRECT_ENTRY(82591B38) { input::LogicalPadConstruct(ctx.r3.u32); }

// PhysicalPad::PhysicalPad: r3 the pad, returned.
ITC_DIRECT_ENTRY(82591B68) { input::PhysicalPadConstruct(ctx.r3.u32); }

// InputProcessor::AnyPressed and PressedOrRepeat: r3 the processor, r4 the
// mask's address; r3 a bool.
ITC_DIRECT_ENTRY(82592500) {
  ctx.r3.u64 = input::InputProcessorAnyPressed(ctx.r3.u32,
                                               schema::InputButtonMask(ctx.r4.u32).mask());
}
ITC_DIRECT_ENTRY(82592520) {
  ctx.r3.u64 = input::InputProcessorPressedOrRepeat(
      ctx.r3.u32, schema::InputButtonMask(ctx.r4.u32).mask());
}

// --- The vehicles (docs/functions/vehicle.md) ---

// Vehicles::DestroyAll: r3 the block (r3 not set: the original leaves
// DestroyPhysics' there, which its caller doesn't read). Where a vehicle's
// DestroyPhysics wouldn't be native, the original.
ITC_DIRECT_ENTRY(820C21E0) {
  ctx.fpscr.disableFlushMode();
  if (!hooks::NativeHeapActive() || !vehicle::VehiclesDestroyAll(ctx.r3.u32)) {
    lifted::original::c820C21E0(ctx);
  }
}

// Vehicle::Construct: r3 the vehicle (returned); its frame the original's
// 96 bytes below the caller's sp.
ITC_DIRECT_ENTRY(820C30D8) {
  ctx.fpscr.disableFlushMode();
  const uint32_t vehicle = ctx.r3.u32;
  vehicle::VehicleConstructAll(vehicle, ctx.r1.u32 - 96);
  ctx.r3.u64 = vehicle;
}

// --- std::list of pointers (src/std_list/ptr_list.h) ---

// PtrList::IncSize: r3 the list (left as it came), r4 the count; the
// original (its length_error) where the size would pass the limit.
ITC_DIRECT_ENTRY(820958B8) {
  if (!std_list::IncSize(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c820958B8(ctx);
  }
}
