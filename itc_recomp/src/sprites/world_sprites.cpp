// The world-space sprites' recorders as plain C++ (sprites/world_sprites.h).

#include "sprites/world_sprites.h"

#include "guest.h"

#include "base/log.h"
#include "hooks/lifted_call.h"

#include <initializer_list>

namespace sprites {

namespace {

constexpr uint32_t kQueueAndList = 0x826C5D5C;  // +0 the render queue, +4 the command list
constexpr uint32_t kOpenChunk = 0x8258C9D0;
constexpr uint32_t kAllocateSlots = 0x8258CAA0;
constexpr uint32_t kAllocateConstants = 0x8258CB98;
constexpr uint32_t kAllocateVertices = 0x8258CB08;
constexpr uint32_t kCloseChunk = 0x8258CA50;
constexpr uint32_t kMultiply = 0x8209BD50;  // the 4x4 product into the constants
constexpr uint32_t kOne = 0x82000A30;
constexpr uint32_t kZero = 0x82000A34;
constexpr uint32_t kTextureEnd = 0x82001BE4;

constexpr uint32_t kListCount = 0x109F0;
constexpr uint32_t kListState = 0x109F4;
constexpr uint32_t kMaxSprites = 500;

// A float moved as the original's lfs and stfs (through a double).
void MoveFloat(uint8_t* base, uint32_t to, uint32_t from) {
  guest::StoreFloat(base, to, float(double(guest::LoadFloat(base, from))));
}
void StoreDouble(uint8_t* base, uint32_t to, double value) {
  guest::StoreFloat(base, to, float(value));
}

// A corner's color packed from its four words' low bytes: the fourth on
// top, then the first, second and third.
uint32_t Pack(uint8_t* base, uint32_t words) {
  return (guest::LoadU32(base, words + 12) << 24) | ((guest::LoadU32(base, words) & 0xFF) << 16) |
         ((guest::LoadU32(base, words + 4) & 0xFF) << 8) | (guest::LoadU32(base, words + 8) & 0xFF);
}

// The recording's calls, from the recorder's frame.
class Recorder {
 public:
  Recorder(uint8_t* base, uint32_t frame, uint64_t r13) : base_(base), frame_(frame), r13_(r13) {}

  uint32_t List() const { return guest::LoadU32(base_, kQueueAndList + 4); }
  uint64_t Call(uint32_t function, std::initializer_list<uint64_t> args, double f1 = 0.0) const {
    return lifted_call::Call(base_, function, frame_, r13_, args, f1).r3;
  }
  // CommandList::AllocateSlots(list, 1), through its wrap.
  uint32_t Slot() const { return uint32_t(Call(kAllocateSlots, {List(), 1})); }
  void Put(uint32_t w0, uint32_t w4) const {
    const uint32_t slot = Slot();
    guest::StoreU32(base_, slot, w0);
    guest::StoreU32(base_, slot + 4, w4);
  }
  void Put(uint32_t w0, uint32_t w4, uint32_t w8) const {
    const uint32_t slot = Slot();
    guest::StoreU32(base_, slot, w0);
    guest::StoreU32(base_, slot + 4, w4);
    guest::StoreU32(base_, slot + 8, w8);
  }

 private:
  uint8_t* base_;
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
Opened OpenAndRecordStates(uint8_t* base, const Recorder& r, uint32_t object, uint32_t state,
                           uint32_t camera) {
  r.Call(kOpenChunk, {r.List()});
  const uint32_t header = r.Slot();
  guest::StoreU16(base, header + 2, 5);
  guest::StoreU32(base, header + 4, 0);
  r.Put(4, 1, 0);
  r.Put(0, state);
  r.Put(1, 6, 0);
  r.Put(96, 0);
  r.Put(60, 1);
  r.Put(48, 0);
  const uint32_t blend = guest::LoadU32(base, object + 72);
  if (blend < 3) {
    r.Put(56, blend == 0 ? 0 : blend == 1 ? 2 : 6);
  }
  switch (guest::LoadU32(base, object + 68)) {
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
  r.Call(kMultiply, {constants, object, camera + 144});
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
uint64_t CloseAndSubmit(uint8_t* base, const Recorder& r, const Opened& opened, uint32_t layer) {
  r.Put(36, 0, 0);
  r.Put(1, 1, 0);
  r.Put(48, 1);
  r.Call(kCloseChunk, {r.List()});
  const uint32_t queue = guest::LoadU32(base, kQueueAndList);
  const uint32_t submit = guest::LoadU32(base, guest::LoadU32(base, queue) + 16);
  const double key = -double(guest::LoadFloat(base, opened.constants + 56));
  return r.Call(submit, {queue, opened.header, layer}, key);
}

// A vertex: x, y, z from `position`, the packed color, u and v.
void Vertex(uint8_t* base, uint32_t vertex, uint32_t position, uint32_t color, double u,
            double v) {
  MoveFloat(base, vertex, position);
  MoveFloat(base, vertex + 4, position + 4);
  MoveFloat(base, vertex + 8, position + 8);
  guest::StoreU32(base, vertex + 12, color);
  StoreDouble(base, vertex + 16, u);
  StoreDouble(base, vertex + 20, v);
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
void Copy64(uint8_t* base, uint32_t to, uint32_t from) {
  for (uint32_t k = 0; k < 64; k += 4) {
    guest::StoreU32(base, to + k, guest::LoadU32(base, from + k));
  }
}

}  // namespace

uint64_t SpriteRecord(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t sprite, uint32_t camera,
                      uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_821281A8 Sprite::Record");
  const uint32_t frame = lifted_call::PushFrame(base, sp, 272);
  const Recorder r(base, frame, r13);
  const Opened opened =
      OpenAndRecordStates(base, r, sprite, guest::LoadU32(base, sprite + 224), camera);
  const uint32_t vertices = QuadStates(r);
  // The corners copied to the frame's +80, then read from there.
  Copy64(base, frame + 80, sprite + 80);
  const double u0 = double(guest::LoadFloat(base, sprite + 208));
  const double v0 = double(guest::LoadFloat(base, sprite + 212));
  const double u1 = double(guest::LoadFloat(base, sprite + 216));
  const double v1 = double(guest::LoadFloat(base, sprite + 220));
  Vertex(base, vertices, frame + 80, Pack(base, sprite + 144), u0, v0);
  Vertex(base, vertices + 24, frame + 96, Pack(base, sprite + 160), u1, v0);
  Vertex(base, vertices + 48, frame + 112, Pack(base, sprite + 176), u0, v1);
  Vertex(base, vertices + 72, frame + 128, Pack(base, sprite + 192), u1, v1);
  QuadDraw(r);
  return CloseAndSubmit(base, r, opened, layer);
}

uint64_t SpriteListRecord(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t list,
                          uint32_t camera, uint32_t layer) {
  ITC_NATIVE_RUNNING("sub_82128750 SpriteList::Record");
  const uint32_t frame = lifted_call::PushFrame(base, sp, 368);
  uint32_t count = guest::LoadU32(base, list + kListCount);
  if (count > kMaxSprites) {
    count = kMaxSprites;
  }
  const Recorder r(base, frame, r13);
  const Opened opened =
      OpenAndRecordStates(base, r, list, guest::LoadU32(base, list + kListState), camera);
  const double end = double(guest::LoadFloat(base, kTextureEnd));
  const double zero = double(guest::LoadFloat(base, kZero));
  const double one = double(guest::LoadFloat(base, kOne));
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t corners = list + 80 + 64 * i;
    const uint32_t vertices = QuadStates(r);
    // The colors to the frame's +80, the corners to its +144.
    Copy64(base, frame + 80, corners + 32000);
    Copy64(base, frame + 144, corners);
    const Coordinates c =
        ListCoordinates(guest::LoadU32(base, list + 66080 + 4 * i),
                        guest::LoadU32(base, list + 64080 + 4 * i), one, end, zero);
    Vertex(base, vertices, frame + 144, Pack(base, frame + 80), c.f0, c.f12);
    Vertex(base, vertices + 24, frame + 160, Pack(base, frame + 96), c.f13, c.f12);
    Vertex(base, vertices + 48, frame + 176, Pack(base, frame + 112), c.f0, c.f11);
    Vertex(base, vertices + 72, frame + 192, Pack(base, frame + 128), c.f13, c.f11);
    QuadDraw(r);
  }
  return CloseAndSubmit(base, r, opened, layer);
}

}  // namespace sprites
