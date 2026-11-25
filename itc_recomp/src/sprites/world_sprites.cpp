// The world-space sprites' recorders as plain C++ (sprites/world_sprites.h).

#include "sprites/world_sprites.h"

#include "guest.h"
#include "schema/cameras.h"
#include "schema/device_shaders.h"
#include "schema/race_overlays.h"
#include "schema/sprites.h"

#include "base/log.h"
#include "hooks/lifted_call.h"

#include <initializer_list>

namespace sprites {

namespace {

using G = schema::SpritesGlobals;
constexpr uint32_t kOpenChunk = 0x8258C9D0;
constexpr uint32_t kAllocateSlots = 0x8258CAA0;
constexpr uint32_t kAllocateConstants = 0x8258CB98;
constexpr uint32_t kAllocateVertices = 0x8258CB08;
constexpr uint32_t kCloseChunk = 0x8258CA50;
constexpr uint32_t kMultiply = 0x8209BD50;  // the 4x4 product into the constants

constexpr uint32_t kListCount = 0x109F0;
constexpr uint32_t kListState = 0x109F4;
constexpr uint32_t kMaxSprites = 500;

// A float moved as the original's lfs and stfs (through a double).
void MoveFloat(uint32_t to, uint32_t from) {
  guest::StoreFloat(to, float(double(guest::LoadFloat(from))));
}
void StoreDouble(uint32_t to, double value) {
  guest::StoreFloat(to, float(value));
}

// A corner's color packed from its four words' low bytes: the fourth on
// top, then the first, second and third.
uint32_t Pack(uint32_t words) {
  return (guest::LoadU32(words + 12) << 24) | ((guest::LoadU32(words) & 0xFF) << 16) |
         ((guest::LoadU32(words + 4) & 0xFF) << 8) | (guest::LoadU32(words + 8) & 0xFF);
}

// The recording's calls, from the recorder's frame.
class Recorder {
 public:
  Recorder(uint32_t frame, uint64_t r13) : frame_(frame), r13_(r13) {}

  uint32_t List() const { return G::command_list(); }
  uint64_t Call(uint32_t function, std::initializer_list<uint64_t> args, double f1 = 0.0) const {
    return lifted_call::Call(function, frame_, r13_, args, f1).r3;
  }
  // CommandList::AllocateSlots(list, 1), through its wrap.
  uint32_t Slot() const { return uint32_t(Call(kAllocateSlots, {List(), 1})); }
  void Put(uint32_t w0, uint32_t w4) const {
    const uint32_t slot = Slot();
    guest::StoreU32(slot, w0);
    guest::StoreU32(slot + 4, w4);
  }
  void Put(uint32_t w0, uint32_t w4, uint32_t w8) const {
    const uint32_t slot = Slot();
    guest::StoreU32(slot, w0);
    guest::StoreU32(slot + 4, w4);
    guest::StoreU32(slot + 8, w8);
  }

 private:
  uint32_t frame_;
  uint64_t r13_;
};

struct Opened {
  uint32_t header;     // the chunk's first slot (handed to the queue)
  uint32_t constants;  // c0 to c3: the object's matrix times the view-projection
};

// The chunk opened and the states both recorders write: the header slot,
// the render states by the object's +72 and +68, `state` (its +224 or the
// list's +0x109F4) in the third slot, and c0 to c3.
Opened OpenAndRecordStates(const Recorder& r, uint32_t object, uint32_t state,
                           uint32_t camera) {
  r.Call(kOpenChunk, {r.List()});
  const uint32_t header = r.Slot();
  guest::StoreU16(header + 2, 5);
  guest::StoreU32(header + 4, 0);
  r.Put(4, 1, 0);
  r.Put(0, state);
  r.Put(1, 6, 0);
  r.Put(96, 0);
  r.Put(60, 1);
  r.Put(48, 0);
  // The object (a sprite or a sprite list) is laid out as race-overlays'
  // OverlaySprite up to its corners.
  const schema::OverlaySprite o(object);
  const uint32_t blend = o.address_mode();
  if (blend < 3) {
    r.Put(56, blend == 0 ? 0 : blend == 1 ? 2 : 6);
  }
  switch (o.blend_mode()) {
    case 0:
      r.Put(72, 1);
      r.Put(76, 0);
      r.Put(72, 6);
      r.Put(76, 7);
      break;
    case 1:
    case 3:
      r.Put(72, 6);
      r.Put(76, 7);
      break;
    case 4:
      r.Put(72, 1);
      r.Put(76, 1);
      break;
    default:  // 2, 5 and above
      r.Put(72, 6);
      r.Put(76, 1);
      break;
  }
  r.Put(2, 2, 0);
  r.Put(0, 20, 1);
  r.Put(0, 16, 1);
  r.Put(7, 4, 0x10004);
  const uint32_t constants = uint32_t(r.Call(kAllocateConstants, {r.List(), 64, 0}));
  r.Call(kMultiply,
         {constants, object, schema::CameraPass(camera).view_projection_address()});
  return {header, constants};
}

// A quad's states before its four vertices; returns the vertices.
uint32_t QuadStates(const Recorder& r) {
  r.Put(13, 1, 0);
  r.Put(6, 0, 2);
  r.Put(5, 4, 0);
  return uint32_t(r.Call(kAllocateVertices, {r.List(), 4, 0}));
}

// A quad's draw after its vertices (and a slot left unwritten, as the
// original allocates it).
void QuadDraw(const Recorder& r) {
  r.Put(15, 1, 0);
  r.Slot();
}

// The chunk closed and handed to the render queue (its slot 4) with the
// header, the layer and the depth key (minus c3's z).
uint64_t CloseAndSubmit(const Recorder& r, const Opened& opened, uint32_t layer) {
  r.Put(36, 0, 0);
  r.Put(1, 1, 0);
  r.Put(48, 1);
  r.Call(kCloseChunk, {r.List()});
  const uint32_t queue = G::render_queue();
  const uint32_t submit = schema::RenderQueue(queue).vtable().add();
  const double key = -double(guest::LoadFloat(opened.constants + 56));
  return r.Call(submit, {queue, opened.header, layer}, key);
}

// A vertex: x, y, z from `position`, the packed color, u and v.
void Vertex(uint32_t vertex, uint32_t position, uint32_t color, double u,
            double v) {
  MoveFloat(vertex, position);
  MoveFloat(vertex + 4, position + 4);
  MoveFloat(vertex + 8, position + 8);
  guest::StoreU32(vertex + 12, color);
  StoreDouble(vertex + 16, u);
  StoreDouble(vertex + 20, v);
}

// The list's texture coordinates by its two choice words (`across` at
// +66080 + 4i, `down` at +64080 + 4i): f0, f11, f12 and f13 of the
// original, each 0, 1 or the end coordinate.
struct Coordinates {
  double f0, f11, f12, f13;
};
Coordinates ListCoordinates(uint32_t across, uint32_t down, double one, double end, double zero) {
  const Coordinates fallback{zero, one, zero, one};
  if (across >= 3 || down >= 4) {
    return fallback;
  }
  static constexpr char kTable[3][4][5] = {
      // across 0
      {"ZBZB", "BBZZ", "ZZBB", "BZBZ"},
      // across 1
      {"BBZA", "ABZB", "BZBA", "AZBB"},
      // across 2
      {"ZABB", "BABZ", "ZBAB", "BBAZ"},
  };
  auto value = [&](char c) { return c == 'A' ? one : c == 'B' ? end : zero; };
  const char* row = kTable[across][down];
  return {value(row[0]), value(row[1]), value(row[2]), value(row[3])};
}

// Copies 64 bytes in doublewords (the original's ld and std).
void Copy64(uint32_t to, uint32_t from) {
  for (uint32_t k = 0; k < 64; k += 4) {
    guest::StoreU32(to + k, guest::LoadU32(from + k));
  }
}

}  // namespace

uint64_t SpriteRecord(uint32_t sp, uint64_t r13, uint32_t sprite, uint32_t camera,
                      uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_821281A8 Sprite::Record");
  const uint32_t frame = lifted_call::PushFrame(sp, 272);
  const Recorder r(frame, r13);
  const Opened opened =
      OpenAndRecordStates(r, sprite, guest::LoadU32(sprite + 224), camera);
  const uint32_t vertices = QuadStates(r);
  // The corners copied to the frame's +80, then read from there.
  Copy64(frame + 80, schema::OverlaySprite(sprite).corners_address());
  const double u0 = double(guest::LoadFloat(sprite + 208));
  const double v0 = double(guest::LoadFloat(sprite + 212));
  const double u1 = double(guest::LoadFloat(sprite + 216));
  const double v1 = double(guest::LoadFloat(sprite + 220));
  Vertex(vertices, frame + 80, Pack(sprite + 144), u0, v0);
  Vertex(vertices + 24, frame + 96, Pack(sprite + 160), u1, v0);
  Vertex(vertices + 48, frame + 112, Pack(sprite + 176), u0, v1);
  Vertex(vertices + 72, frame + 128, Pack(sprite + 192), u1, v1);
  QuadDraw(r);
  return CloseAndSubmit(r, opened, layer);
}

uint64_t SpriteListRecord(uint32_t sp, uint64_t r13, uint32_t list,
                          uint32_t camera, uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_82128750 SpriteList::Record");
  const uint32_t frame = lifted_call::PushFrame(sp, 368);
  uint32_t count = guest::LoadU32(list + kListCount);
  if (count > kMaxSprites) {
    count = kMaxSprites;
  }
  const Recorder r(frame, r13);
  const Opened opened =
      OpenAndRecordStates(r, list, guest::LoadU32(list + kListState), camera);
  const double end = double(G::size_scale());
  const double zero = double(G::zero());
  const double one = double(G::one());
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t corners = schema::OverlaySprite(list).corners_address() + 64 * i;
    const uint32_t vertices = QuadStates(r);
    // The colors to the frame's +80, the corners to its +144.
    Copy64(frame + 80, corners + 32000);
    Copy64(frame + 144, corners);
    const Coordinates c =
        ListCoordinates(guest::LoadU32(list + 66080 + 4 * i),
                        guest::LoadU32(list + 64080 + 4 * i), one, end, zero);
    Vertex(vertices, frame + 144, Pack(frame + 80), c.f0, c.f12);
    Vertex(vertices + 24, frame + 160, Pack(frame + 96), c.f13, c.f12);
    Vertex(vertices + 48, frame + 176, Pack(frame + 112), c.f0, c.f11);
    Vertex(vertices + 72, frame + 192, Pack(frame + 128), c.f13, c.f11);
    QuadDraw(r);
  }
  return CloseAndSubmit(r, opened, layer);
}

}  // namespace sprites
