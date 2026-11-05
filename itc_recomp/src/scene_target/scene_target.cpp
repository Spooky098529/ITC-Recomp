// The scene target's passes (scene_target.h,
// docs/functions/scene-target.md): step for step as the recompiled
// functions, the command list as plain C++ (src/command_list/), the guest
// calls through the adapter's callbacks.

#include "scene_target/scene_target.h"

#include "guest.h"

#include "base/log.h"
#include "command_list/command_list.h"

#include <initializer_list>

namespace scene_target {

namespace {

constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kZero = 0x82000A34;  // 0.0

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// std: the high word, then the low.
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}

// Whether the list has room for `slots` more (a record inside a chunk
// already open: only the room).
bool Room(uint8_t* base, uint32_t list, uint64_t slots) {
  return uint64_t(Load(base, list + 12)) + slots < Load(base, list + 8);
}
// Whether a record opening its own chunk may go on: no chunk open (on this
// list or another), and room for `slots`.
bool Ready(uint8_t* base, uint32_t list, uint64_t slots) {
  return guest::LoadU8(base, list + 52) == 0 && Load(base, command_list::kOpenChunks) == 0 &&
         Room(base, list, slots);
}

// --- The list's steps (room checked: none fails) ---

uint32_t Slot(uint8_t* base, uint32_t list) { return command_list::AllocateSlots(base, list, 1); }
// A slot with [a, b, c] (+12 left as it was).
uint32_t Slot3(uint8_t* base, uint32_t list, uint32_t a, uint32_t b, uint32_t c) {
  const uint32_t s = Slot(base, list);
  Store(base, s + 0, a);
  Store(base, s + 4, b);
  Store(base, s + 8, c);
  return s;
}
// A slot with [a, b] (+8, +12 left as they were).
uint32_t Slot2(uint8_t* base, uint32_t list, uint32_t a, uint32_t b) {
  const uint32_t s = Slot(base, list);
  Store(base, s + 0, a);
  Store(base, s + 4, b);
  return s;
}
// A slot with [a] (+4 to +12 left as they were).
uint32_t Slot1(uint8_t* base, uint32_t list, uint32_t a) {
  const uint32_t s = Slot(base, list);
  Store(base, s, a);
  return s;
}
// The chunk's header: [+2] = 5 (a halfword), [+4] = 0.
uint32_t Header(uint8_t* base, uint32_t list) {
  const uint32_t s = Slot(base, list);
  guest::StoreU16(base, s + 2, 5);
  Store(base, s + 4, 0);
  return s;
}

// Command 23 with render targets 0-3 all the scene target's `color` field,
// then command 24 with its `depth` field (each loaded after its slot, as the
// original does).
uint32_t Targets(uint8_t* base, uint32_t scene, uint32_t list, uint32_t color, uint32_t depth) {
  Slot3(base, list, 23, 4, 0);
  for (uint32_t target = 0; target < 4; target++) {
    const uint32_t s = Slot(base, list);
    Store(base, s + 0, target);
    Store(base, s + 4, Load(base, scene + color));
  }
  const uint32_t s = Slot(base, list);
  Store(base, s + 4, 1);
  Store(base, s + 0, 24);
  Store(base, s + 8, 0);
  const uint32_t last = Slot(base, list);
  Store(base, last, Load(base, scene + depth));
  return last;
}

}  // namespace

std::optional<uint32_t> RecordTargets(uint8_t* base, uint32_t scene, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8209AE48 SceneTarget::RecordTargets");
  if (!Room(base, list, 7)) {
    return std::nullopt;
  }
  return Targets(base, scene, list, 4, 0);
}

std::optional<uint32_t> RecordTileTargets(uint8_t* base, uint32_t scene, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8209AF40 SceneTarget::RecordTileTargets");
  if (!Room(base, list, 7)) {
    return std::nullopt;
  }
  return Targets(base, scene, list, 12, 8);
}

std::optional<uint32_t> RecordBoundTargets(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8209B058 SceneTarget::RecordBoundTargets");
  return RecordTargets(base, Load(base, kSceneTarget), list);
}

std::optional<uint32_t> TexturedRectRecordFlat(uint8_t* base, uint32_t rect, uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_8217AA98 TexturedRect::RecordFlat");
  const uint32_t list = Load(base, kRectLists + 4);
  // The header, the texture (2), the render states (6), the samplers (3),
  // 13 and its item, 5, the vertices (6), 15, a slot left as it is and 36.
  if (!Ready(base, list, 24)) {
    return std::nullopt;
  }
  command_list::OpenChunk(base, list);
  const uint32_t chunk = Header(base, list);
  Slot3(base, list, 4, 1, 0);
  {
    const uint32_t s = Slot(base, list);
    Store(base, s + 0, 0);
    Store(base, s + 4, Load(base, rect + 112));
  }
  Slot3(base, list, 1, 5, 0);
  const bool blend = Load(base, rect + 116) != 0;
  Slot2(base, list, 56, 6);
  Slot2(base, list, 96, 0);
  Slot2(base, list, 60, blend ? 1 : 0);
  Slot2(base, list, 72, blend ? 6 : 1);
  Slot2(base, list, 76, blend ? 7 : 0);
  Slot3(base, list, 2, 2, 0);
  Slot3(base, list, 0, 20, 1);
  Slot3(base, list, 0, 16, 1);
  Slot3(base, list, 13, 1, 0);
  Slot3(base, list, 6, 1, 2);
  Slot3(base, list, 5, 4, 0);
  const uint32_t vertex = command_list::AllocateVertices(base, list, 4, false);

  // 1.0 four times in the frame (never read), as the original's loop.
  const float one = guest::LoadFloat(base, kOne);
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreFloat(base, frame + 124 + 16 * i, one);
  }
  const uint32_t x = Load(base, rect + 80), y = Load(base, rect + 84);
  const int64_t left = int32_t(x), top = int32_t(y);
  const int64_t right = int32_t(x + Load(base, rect + 88));
  const int64_t bottom = int32_t(y + Load(base, rect + 92));
  // The integers' conversions, through the frame as the original's.
  Store64(base, frame + 80, uint64_t(left));
  Store64(base, frame + 88, uint64_t(top));
  Store64(base, frame + 96, uint64_t(right));
  Store64(base, frame + 104, uint64_t(bottom));
  const float fx[2] = {float(double(left)), float(double(right))};
  const float fy[2] = {float(double(top)), float(double(bottom))};
  // Texture coordinates copied as floats (lfs, stfs).
  auto coordinate = [&](uint32_t offset) {
    return float(double(guest::LoadFloat(base, rect + offset)));
  };
  for (uint32_t corner = 0; corner < 4; corner++) {
    const uint32_t v = vertex + 24 * corner;
    const uint32_t right_side = corner & 1, bottom_side = corner >> 1;
    guest::StoreFloat(base, v + 0, fx[right_side]);
    guest::StoreFloat(base, v + 4, fy[bottom_side]);
    guest::StoreFloat(base, v + 8, one);
    Store(base, v + 12, 0xFFFFFFFFu);
    guest::StoreFloat(base, v + 16, coordinate(right_side ? 104 : 96));
    guest::StoreFloat(base, v + 20, coordinate(bottom_side ? 108 : 100));
  }
  Slot3(base, list, 15, 1, 0);
  Slot(base, list);
  Slot3(base, list, 36, 0, 0);
  command_list::CloseChunk(base, list);
  return chunk;
}

}  // namespace scene_target
