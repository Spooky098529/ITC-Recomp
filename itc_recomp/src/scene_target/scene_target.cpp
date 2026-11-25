// The scene target's passes (scene_target.h,
// docs/functions/scene-target.md): step for step as the recompiled
// functions, the command list as plain C++ (src/command_list/), the guest
// calls through the adapter's callbacks.

#include "scene_target/scene_target.h"

#include "guest.h"

#include "base/log.h"
#include "command_list/command_list.h"
#include "schema/app_start_up.h"
#include "schema/command_list.h"
#include "schema/scene_target.h"

#include <initializer_list>

namespace scene_target {

namespace {

using G = schema::SceneTargetGlobals;
using Rect = schema::TexturedRect;
using Surface = schema::View;  // the surfaces' addresses only

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
// std: the high word, then the low.
void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}

// Whether the list has room for `slots` more (a record inside a chunk
// already open: only the room).
bool Room(uint32_t list_address, uint64_t slots) {
  const schema::CommandList list(list_address);
  return uint64_t(list.used()) + slots < list.capacity();
}
// Whether a record opening its own chunk may go on: no chunk open (on this
// list or another), and room for `slots`.
bool Ready(uint32_t list, uint64_t slots) {
  return schema::CommandList(list).chunk_open() == 0 &&
         schema::CommandListGlobals::open_chunks() == 0 && Room(list, slots);
}

// --- The list's steps (room checked: none fails) ---

uint32_t Slot(uint32_t list) { return command_list::AllocateSlots(list, 1); }
// A slot with [a, b, c] (+12 left as it was).
uint32_t Slot3(uint32_t list, uint32_t a, uint32_t b, uint32_t c) {
  const uint32_t s = Slot(list);
  Store(s + 0, a);
  Store(s + 4, b);
  Store(s + 8, c);
  return s;
}
// A slot with [a, b] (+8, +12 left as they were).
uint32_t Slot2(uint32_t list, uint32_t a, uint32_t b) {
  const uint32_t s = Slot(list);
  Store(s + 0, a);
  Store(s + 4, b);
  return s;
}
// A slot with [a] (+4 to +12 left as they were).
uint32_t Slot1(uint32_t list, uint32_t a) {
  const uint32_t s = Slot(list);
  Store(s, a);
  return s;
}
// The chunk's header: [+2] = 5 (a halfword), [+4] = 0.
uint32_t Header(uint32_t list) {
  const uint32_t s = Slot(list);
  guest::StoreU16(s + 2, 5);
  Store(s + 4, 0);
  return s;
}

// Command 23 with render targets 0-3 all the scene target's color surface,
// then command 24 with its depth surface (the 1024 x 576 pair, or with
// `tile` the tile-sized pair; each loaded after its slot, as the original
// does).
uint32_t Targets(uint32_t scene_address, uint32_t list, bool tile) {
  const schema::SceneTarget scene(scene_address);
  Slot3(list, 23, 4, 0);
  for (uint32_t target = 0; target < 4; target++) {
    const uint32_t s = Slot(list);
    Store(s + 0, target);
    Store(s + 4,
          tile ? scene.tile_color<Surface>().address : scene.color<Surface>().address);
  }
  const uint32_t s = Slot(list);
  Store(s + 4, 1);
  Store(s + 0, 24);
  Store(s + 8, 0);
  const uint32_t last = Slot(list);
  Store(last, tile ? scene.tile_depth<Surface>().address : scene.depth<Surface>().address);
  return last;
}

}  // namespace

std::optional<uint32_t> RecordTargets(uint32_t scene, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8209AE48 SceneTarget::RecordTargets");
  if (!Room(list, 7)) {
    return std::nullopt;
  }
  return Targets(scene, list, false);
}

std::optional<uint32_t> RecordTileTargets(uint32_t scene, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8209AF40 SceneTarget::RecordTileTargets");
  if (!Room(list, 7)) {
    return std::nullopt;
  }
  return Targets(scene, list, true);
}

std::optional<uint32_t> RecordBoundTargets(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8209B058 SceneTarget::RecordBoundTargets");
  return RecordTargets(G::scene_target<schema::View>().address, list);
}

std::optional<uint32_t> TexturedRectRecordFlat(uint32_t rect_address,
                                               uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8217AA98 TexturedRect::RecordFlat");
  const Rect rect(rect_address);
  const uint32_t list = G::rect_list();
  // The header, the texture (2), the render states (6), the samplers (3),
  // 13 and its item, 5, the vertices (6), 15, a slot left as it is and 36.
  if (!Ready(list, 24)) {
    return std::nullopt;
  }
  command_list::OpenChunk(list);
  const uint32_t chunk = Header(list);
  Slot3(list, 4, 1, 0);
  {
    const uint32_t s = Slot(list);
    Store(s + 0, 0);
    Store(s + 4, rect.texture());
  }
  Slot3(list, 1, 5, 0);
  const bool blend = rect.blend() != 0;
  Slot2(list, 56, 6);
  Slot2(list, 96, 0);
  Slot2(list, 60, blend ? 1 : 0);
  Slot2(list, 72, blend ? 6 : 1);
  Slot2(list, 76, blend ? 7 : 0);
  Slot3(list, 2, 2, 0);
  Slot3(list, 0, 20, 1);
  Slot3(list, 0, 16, 1);
  Slot3(list, 13, 1, 0);
  Slot3(list, 6, 1, 2);
  Slot3(list, 5, 4, 0);
  const uint32_t vertex = command_list::AllocateVertices(list, 4, false);

  // 1.0 four times in the frame (never read), as the original's loop.
  const float one = G::one();
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreFloat(frame + 124 + 16 * i, one);
  }
  const uint32_t x = uint32_t(rect.x()), y = uint32_t(rect.y());
  const int64_t left = int32_t(x), top = int32_t(y);
  const int64_t right = int32_t(x + rect.width());
  const int64_t bottom = int32_t(y + rect.height());
  // The integers' conversions, through the frame as the original's.
  Store64(frame + 80, uint64_t(left));
  Store64(frame + 88, uint64_t(top));
  Store64(frame + 96, uint64_t(right));
  Store64(frame + 104, uint64_t(bottom));
  const float fx[2] = {float(double(left)), float(double(right))};
  const float fy[2] = {float(double(top)), float(double(bottom))};
  // Texture coordinates copied as floats (lfs, stfs).
  auto coordinate = [](float value) { return float(double(value)); };
  for (uint32_t corner = 0; corner < 4; corner++) {
    const uint32_t v = vertex + 24 * corner;
    const uint32_t right_side = corner & 1, bottom_side = corner >> 1;
    guest::StoreFloat(v + 0, fx[right_side]);
    guest::StoreFloat(v + 4, fy[bottom_side]);
    guest::StoreFloat(v + 8, one);
    Store(v + 12, 0xFFFFFFFFu);
    guest::StoreFloat(v + 16, coordinate(right_side ? rect.u1() : rect.u0()));
    guest::StoreFloat(v + 20, coordinate(bottom_side ? rect.v1() : rect.v0()));
  }
  Slot3(list, 15, 1, 0);
  Slot(list);
  Slot3(list, 36, 0, 0);
  command_list::CloseChunk(list);
  return chunk;
}

namespace {

// The clear (command 26 and its 280 bytes, no header): +0 = +4 = 0, +8 = 49
// (render target 0, Z and stencil), +268 = 0xFF000000, +272 = 0.0, +276 = 0;
// the rest left as it was.
void Clear(uint32_t list, uint32_t zero) {
  Slot3(list, 26, 1, 0);
  const uint32_t s = command_list::AllocateBytes(list, 280, false);
  Store(s + 272, zero);
  Store(s + 8, 49);
  Store(s + 268, 0xFF000000u);
  Store(s + 276, 0);
  Store(s + 4, 0);
  Store(s + 0, 0);
}

// The render targets and depth back from the saved set (command 29, mask
// 143).
void RestoreTargets(uint32_t list) {
  Slot3(list, 29, 1, 0);
  Slot1(list, 143);
}

// Command 1 with render state 40 (Z enable) off.
void ZOff(uint32_t list) {
  Slot3(list, 1, 1, 0);
  Slot2(list, 40, 0);
}

// The resolve item (AllocateResolve, no header) into the texture `texture`:
// +0 = `what` (2 a destination only, 3 a rectangle too), +4 = 0, +24 the
// texture, +36 = +40 = 0, the bytes +44 to +47 0, 0, 0, 255, +48 = 0.0, +52
// = 0, +56 and +64 the destination's words, +60 = 0; with a rectangle, +8
// = +12 = 0, +16 = 1024, +20 = 576.
void Resolve(uint32_t list, uint32_t what, uint32_t texture, uint32_t zero) {
  Slot3(list, 25, 1, 0);
  const uint32_t s = command_list::AllocateResolve(list, false);
  if (what == 2) {
    Store(s + 0, 2);
    Store(s + 4, 0);
  } else {
    Store(s + 4, 0);
    Store(s + 0, 3);
  }
  Store(s + 48, zero);
  Store(s + 36, 0);
  Store(s + 40, 0);
  Store(s + 52, 0);
  Store(s + 24, texture);
  guest::StoreU8(s + 44, 0);
  guest::StoreU8(s + 45, 0);
  guest::StoreU8(s + 46, 0);
  guest::StoreU8(s + 47, 255);
  Store(s + 56, 0x18280186u);
  Store(s + 60, 0);
  Store(s + 64, 0x1A220197u);
  if (what == 3) {
    Store(s + 8, 0);
    Store(s + 16, 1024);
    Store(s + 12, 0);
    Store(s + 20, 576);
  }
}

// A screen rectangle in the frame at `rect`, as the original builds it: an
// identity matrix at +0 (from its vector unit; the 1.0s it stores first at
// +12, +28, +44 and +60 overwritten by it), +64 = +68 = 0, +72 = 2, the
// position 0, 0 and the size, the texture coordinates 0.0, 0.0 to `u1`,
// `v1` (words), the texture, +116 = 0 (blending off); +76 not written.
void ScreenRect(uint32_t rect_address, uint32_t width, uint32_t height,
                uint32_t u1, uint32_t v1, uint32_t texture, uint32_t zero) {
  const Rect rect(rect_address);
  for (uint32_t row = 0; row < 4; row++) {
    for (uint32_t column = 0; column < 4; column++) {
      Store(rect.matrix_address() + 16 * row + 4 * column, row == column ? 0x3F800000u : 0);
    }
  }
  rect.set_kind(0);
  rect.set_unknown_68(0);
  rect.set_unknown_72(2);
  rect.set_x(0);
  rect.set_y(0);
  rect.set_width(width);
  rect.set_height(height);
  // The texture coordinates stored as words (stw), as the original's.
  Store(rect.u0_address(), zero);
  Store(rect.v0_address(), zero);
  Store(rect.u1_address(), u1);
  Store(rect.v1_address(), v1);
  rect.set_texture(texture);
  rect.set_blend(0);
}

}  // namespace

bool RecordPasses(uint32_t frame, uint32_t scene_address, uint32_t list,
                  uint32_t bucket5, uint32_t bucket6, uint32_t bucket7, const PassCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8209B3D0 SceneTarget::RecordPasses");
  const schema::SceneTarget scene(scene_address);
  // The mode's passes.
  const uint32_t mode = scene.mode();
  const uint32_t bits = mode == 0 ? 7 : mode == 1 ? 4 : mode == 3 ? 1 : mode == 4 ? 8 : 0;
  // Every slot the scene's list takes, and the rectangles' (24 each, which
  // may be on the same list): declined when they wouldn't all fit.
  uint64_t slots = 0, rects = 0;
  if (bits & 8) {
    slots += 36;
    rects++;
  }
  if (bits & 1) {
    slots += 23;
  }
  if (bits & 6) {
    slots += (bits & 2) ? 36 : 30;
    rects += (bits & 2) ? 1 : 0;
  }
  if (bits & 4) {
    slots += 12;
    rects++;
  }
  if (!Ready(list, slots + 24 * rects)) {
    return false;
  }
  // r5 and r6 in the caller's frame, the bits at +80.
  Store(frame + kRecordPassesFrame + 36, bucket5);
  Store(frame + kRecordPassesFrame + 44, bucket6);
  Store(frame + 80, bits);
  const uint32_t one = Load(G::one_address()), zero = Load(G::zero_address());

  if (bits & 8) {
    // The menus' predicated tiling.
    command_list::OpenChunk(list);
    const uint32_t chunk = Header(list);
    RecordTileTargets(scene_address, list);  // room checked above
    Slot3(list, 32, 1, 0);  // BeginTiling
    {
      const uint32_t s = Slot(list);
      Store(s + 0, 0);
      // The tiling block's tile count and tiles (the pointer loaded for each).
      Store(s + 4, G::display_tiling().tile_count());
      Store(s + 8, G::display_tiling().tiles_address());
    }
    Clear(list, zero);
    Slot3(list, 36, 0, 0);
    const uint32_t second = Header(list);
    Slot3(list, 33, 1, 0);  // EndTiling into the texture
    Slot2(list, 771, scene.texture());
    RestoreTargets(list);
    Slot3(list, 36, 0, 0);
    command_list::CloseChunk(list);
    calls.submit(chunk, 108, 0x8209B634, 0x8209B644, 0x8209B660);
    calls.submit(second, 594, 0x8209B674, 0x8209B684, 0x8209B6A0);
    const uint32_t rect = frame + 96;
    ScreenRect(rect, 1280, 720, one, one, scene.texture(), zero);
    calls.rect(rect, 594, 0x8209B738);
  }
  if (bits & 1) {
    command_list::OpenChunk(list);
    const uint32_t chunk = Header(list);
    RestoreTargets(list);
    Clear(list, zero);
    Slot3(list, 36, 0, 0);
    command_list::CloseChunk(list);
    calls.submit(chunk, bucket7, 0x8209B810, 0x8209B820, 0x8209B83C);
  }
  if (bits & 6) {
    // The race: the frame drawn so far resolved into the texture, the
    // 1024 x 576 targets bound and cleared, the texture drawn into them.
    command_list::OpenChunk(list);
    const uint32_t chunk = Header(list);
    if (bits & 2) {
      Resolve(list, 2, scene.texture(), zero);
    }
    RecordTargets(scene_address, list);  // room checked above
    Clear(list, zero);
    ZOff(list);
    Slot3(list, 36, 0, 0);
    command_list::CloseChunk(list);
    calls.submit(chunk, bucket5, 0x8209B9B0, 0x8209B9C0, 0x8209B9E0);
    if (bits & 2) {
      const uint32_t rect = frame + 224;
      ScreenRect(rect, 1024, 576, one, one, scene.texture(), zero);
      calls.rect(rect, bucket5, 0x8209BA80);
    }
  }
  if (bits & 4) {
    // The upscale draw: the race's 1024 x 576 view resolved and drawn over
    // the screen.
    command_list::OpenChunk(list);
    const uint32_t chunk = Header(list);
    Resolve(list, 3, scene.texture(), zero);
    RestoreTargets(list);
    ZOff(list);
    Slot3(list, 36, 0, 0);
    command_list::CloseChunk(list);
    calls.submit(chunk, bucket6, 0x8209BBD4, 0x8209BBE4, 0x8209BC04);
    const uint32_t rect = frame + 352;
    const uint32_t scale = Load(G::four_fifths_address());  // 0.8
    ScreenRect(rect, 1280, 720, scale, scale, scene.texture(), zero);
    calls.rect(rect, bucket6, 0x8209BCA4);
  }
  return true;
}

}  // namespace scene_target
