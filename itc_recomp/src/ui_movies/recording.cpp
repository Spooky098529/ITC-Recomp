// The UI movie library's recorders (sub_8259DAD0, sub_8259E098 and
// sub_8259D7B0's two chunks) as plain C++: see ui_movies.h.

#include "ui_movies/ui_movies.h"

#include "command_list/command_list.h"
#include "schema/ui_movies.h"

#include <initializer_list>
#include <vector>

namespace ui_movies {

namespace {

// One command slot's words, as the original stores them (+8 only where it
// does).
void Slot3(uint8_t* base, uint32_t slot, uint32_t w0, uint32_t w1, uint32_t w2) {
  guest::StoreU32(base, slot + 0, w0);
  guest::StoreU32(base, slot + 4, w1);
  guest::StoreU32(base, slot + 8, w2);
}
void Slot2(uint8_t* base, uint32_t slot, uint32_t w0, uint32_t w1) {
  guest::StoreU32(base, slot + 0, w0);
  guest::StoreU32(base, slot + 4, w1);
}

// The chunk's first slot: halfword +2 = 5 and +4 = 0 (its first halfword
// left as it was).
uint32_t OpenWithHeader(uint8_t* base, uint32_t list) {
  command_list::OpenChunk(base, list);
  const uint32_t chunk = command_list::AllocateSlots(base, list, 1);
  guest::StoreU16(base, chunk + 2, 5);
  guest::StoreU32(base, chunk + 4, 0);
  return chunk;
}

// The blend states by a quad's +68 (0 to 4; 5 and up records none), the
// same in both quad recorders. Each list is the slots' (+0, +4) words; the
// first slot also gets +8 = 0.
struct BlendSlots {
  uint32_t mode;
  std::initializer_list<std::pair<uint32_t, uint32_t>> slots;
};
const BlendSlots kBlendModes[] = {
    {0, {{1, 4}, {56, 0}, {60, 1}, {76, 7}, {72, 6}}},
    {1, {{1, 4}, {56, 0}, {60, 1}, {76, 4}, {72, 0}}},
    {2, {{1, 4}, {56, 0}, {60, 1}, {76, 1}, {72, 6}}},
    {3, {{1, 5}, {56, 0}, {60, 1}, {80, 4}, {76, 1}, {72, 6}}},
    {4, {{1, 3}, {56, 0}, {60, 0}, {40, 0}}},
};

uint32_t BlendSlotCount(uint32_t mode) {
  return mode < 5 ? uint32_t(kBlendModes[mode].slots.size()) : 0;
}

}  // namespace

bool ChunksFit(uint8_t* base, uint32_t list, std::initializer_list<std::vector<uint32_t>> chunks) {
  bool open = guest::LoadU8(base, list + 52) != 0;
  uint32_t open_chunks = guest::LoadU32(base, command_list::kOpenChunks);
  uint32_t used = guest::LoadU32(base, list + 12);
  const uint32_t limit = guest::LoadU32(base, list + 8);
  for (const std::vector<uint32_t>& allocations : chunks) {
    if (!open) {
      if (open_chunks != 0) {
        return false;
      }
      open = true;
      open_chunks++;
    }
    for (uint32_t count : allocations) {
      used += count;
      if (used >= limit) {
        return false;
      }
    }
    open = false;
    open_chunks--;
  }
  return true;
}

std::optional<uint32_t> RecordQuad(uint8_t* base, uint32_t list, uint32_t quad, bool textured) {
  const schema::UiMovieDrawQuad q(base, quad);
  const uint32_t clipped = q.clipped();
  const uint32_t mode = q.blend_mode();
  const uint32_t vertices = q.vertex_count();
  {
    // The allocations, in order: the header, the clip (2), the texture
    // (2, textured only), the blend states, the texture stage (3), 13, 6,
    // the vertices' header, the vertices, 15, an empty slot and 36.
    std::vector<uint32_t> allocations(textured ? 5 : 3, 1);
    allocations.insert(allocations.end(), BlendSlotCount(mode), 1);
    allocations.insert(allocations.end(), {1, 1, 1, 1, 1, 1, (vertices * 24 + 15) >> 4, 1, 1, 1});
    if (!ChunksFit(base, list, {allocations})) {
      return std::nullopt;
    }
  }
  auto slot = [&] { return command_list::AllocateSlots(base, list, 1); };
  const uint32_t chunk = OpenWithHeader(base, list);
  if (clipped != 0) {
    // Command 31 (the clip rectangle on) and the rectangle's four
    // halfwords (+104..+110).
    Slot2(base, slot(), 31, 1);
    const uint32_t s = slot();
    guest::StoreU16(base, s + 0, q.clip_left());
    guest::StoreU16(base, s + 2, q.clip_top());
    guest::StoreU16(base, s + 4, q.clip_right());
    guest::StoreU16(base, s + 6, q.clip_bottom());
  } else {
    Slot3(base, slot(), 1, 1, 0);
    Slot2(base, slot(), 200, 0);
  }
  if (textured) {
    Slot3(base, slot(), 4, 1, 0);
    Slot2(base, slot(), 0, q.texture());
  }
  if (mode < 5) {
    bool first = true;
    for (auto [w0, w1] : kBlendModes[mode].slots) {
      if (first) {
        Slot3(base, slot(), w0, w1, 0);
        first = false;
      } else {
        Slot2(base, slot(), w0, w1);
      }
    }
  }
  const uint32_t stage = q.texture_stage() != 0 ? 1 : 0;
  Slot3(base, slot(), 2, 2, 0);
  Slot3(base, slot(), 0, 20, stage);
  Slot3(base, slot(), 0, 16, stage);
  Slot3(base, slot(), 13, 1, 0);
  Slot3(base, slot(), 6, 1, textured ? 2 : 0);
  Slot3(base, slot(), 5, q.vertex_count(), 0);

  // The vertices (24 bytes): position from +88's (16 bytes each), the color
  // from +92's (the word +12's low byte, then bytes +3, +7, +11), texture
  // coordinates from +96's (textured only; +16 and +20 left as they were
  // otherwise). The floats go through registers in the original (lfs,
  // stfs), which the translated code stores unchanged: copied as floats, bit
  // for bit (no arithmetic).
  const uint32_t v = command_list::AllocateVertices(base, list, q.vertex_count(), false);
  for (uint32_t i = 0; i < q.vertex_count(); i++) {
    // Vertex i as the block's first (24 bytes each).
    const schema::CommandListVertices at(base, v + 24 * i);
    const schema::UiMovieQuadPosition position(base, q.positions().address + 16 * i);
    const schema::UiMovieQuadColor color(base, q.colors().address + 16 * i);
    if (textured) {
      const schema::UiMovieQuadCoord uv(base, q.coords().address + 16 * i);
      at.set_v0_u(uv.u());
      at.set_v0_v(uv.v());
    }
    at.set_v0_x(position.x());
    at.set_v0_y(position.y());
    at.set_v0_z(position.z());
    at.set_v0_color((color.alpha() << 24) | (uint32_t(color.red_low()) << 16) |
                    (uint32_t(color.green_low()) << 8) | color.blue_low());
  }
  Slot3(base, slot(), 15, 1, 0);
  slot();  // allocated and left as it was, as the original does
  Slot3(base, slot(), 36, 0, 0);
  command_list::CloseChunk(base, list);
  return chunk;
}

const std::vector<uint32_t> kResetChunkSlots = {1, 1, 1, 1};

uint32_t RecordClipReset(uint8_t* base, uint32_t list) {
  const uint32_t chunk = OpenWithHeader(base, list);
  auto slot = [&] { return command_list::AllocateSlots(base, list, 1); };
  Slot3(base, slot(), 31, 1, 0);
  const uint32_t s = slot();
  const uint16_t rectangle[4] = {0, 0, 1280, 720};
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU16(base, s + 2 * i, rectangle[i]);
  }
  Slot3(base, slot(), 36, 0, 0);
  command_list::CloseChunk(base, list);
  return chunk;
}

uint32_t RecordStateReset(uint8_t* base, uint32_t list) {
  const uint32_t chunk = OpenWithHeader(base, list);
  auto slot = [&] { return command_list::AllocateSlots(base, list, 1); };
  Slot3(base, slot(), 1, 1, 0);
  Slot2(base, slot(), 200, 0);
  Slot3(base, slot(), 36, 0, 0);
  command_list::CloseChunk(base, list);
  return chunk;
}

}  // namespace ui_movies
