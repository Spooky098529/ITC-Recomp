// sub_822AE340 in plain C++: see vertex_fetch_patch.h. The bit operations
// follow the original's step by step (the fields' meanings in the comments
// are probable, from the vfetch instruction's layout).

#include "vertex_fetch_patch.h"

#include "guest.h"

#include <algorithm>
#include <array>
#include <bit>

namespace native {

namespace {

constexpr uint32_t kSelectMasks = 0x82014224;
constexpr uint32_t kFormatSizes = 0x82014238;
// The original builds the instructions in its 592-byte frame from +96: room
// for 41.
constexpr size_t kMaxFetches = 41;

uint32_t Rotl(uint32_t value, int shift) {
  return std::rotl(value, shift);
}

void StoreBig(uint8_t* p, uint32_t value) {
  p[0] = uint8_t(value >> 24);
  p[1] = uint8_t(value >> 16);
  p[2] = uint8_t(value >> 8);
  p[3] = uint8_t(value);
}

struct Instruction {
  uint32_t w[3];
};

// The fetch constant an instruction reads, with its select (bits 25-26)
// moved under its index (bits 20-24): index × 4 + select.
uint32_t ConstantKey(uint32_t w0) {
  uint32_t moved = (w0 & ~0xC0000u) | (Rotl(w0, 25) & 0xC0000u);
  return Rotl(moved, 14) & 0x7F;
}

// The offset in dwords (word 2 bits 8-30, signed).
int32_t Offset(uint32_t w2) {
  return int32_t(w2 << 1) >> 9;
}

uint32_t Format(uint32_t w1) {
  return (w1 >> 16) & 0x3F;
}

// The destination selects (word 1 bits 0-11, three bits a component) kept
// to the components `components` holds, one step a component as the
// original works them out.
uint32_t FitSelects(const VertexFetchTables& tables, uint32_t w1, uint32_t components) {
  auto mask = [&](uint32_t select) { return tables.select_masks[select & 7] & components & 0xFFFF; };
  uint32_t s = mask(w1);
  uint32_t f = s;
  for (int i = 0; i < 4; i++) {
    f = (f >> 3) | s;
  }
  w1 = (w1 & ~7u) | ((f >> 1) & 7);

  s = mask(w1 >> 3);
  f = s;
  for (int i = 0; i < 3; i++) {
    f = (f >> 3) | s;
  }
  w1 = (w1 & ~0x38u) | (((f >> 1) | (s << 2)) & 0x38);

  s = mask(w1 >> 6);
  f = s;
  for (int i = 0; i < 2; i++) {
    f = (f >> 3) | s;
  }
  w1 = (w1 & ~0x1C0u) | (((f >> 1) | (((s << 3) | s) << 2)) & 0x1C0);

  s = mask(w1 >> 9);
  uint32_t up = (((s << 3) | s) << 3 | s) << 2;
  uint32_t down = ((s >> 3) | s) >> 1;
  w1 = (w1 & ~0xE00u) | ((up | down) & 0xE00);
  return w1;
}

}  // namespace

VertexFetchTables LoadVertexFetchTables(uint8_t* base) {
  VertexFetchTables tables;
  for (uint32_t i = 0; i < 8; i++) {
    tables.select_masks[i] = guest::LoadU16(base, kSelectMasks + 2 * i);
  }
  for (uint32_t i = 0; i < 64; i++) {
    tables.format_sizes[i] = guest::LoadU8(base, kFormatSizes + i);
  }
  return tables;
}

bool PatchVertexFetches(const VertexFetchTables& tables, const VertexFetches& fetches,
                        std::span<const DeclarationElement> elements, const uint8_t strides[16],
                        uint8_t* microcode, size_t size) {
  const size_t count = std::min(fetches.info.size(), kMaxFetches);
  std::array<Instruction, kMaxFetches> out;

  // Each fetch's instruction from its template and the element of its usage.
  // `components` is the original's r8: the last fetch's component selects
  // with bits 1-3 set (bit 0, from an uninitialized stack halfword at
  // first, never reaches the result).
  uint32_t components = 0;
  for (size_t i = 0; i < count; i++) {
    const uint32_t word = fetches.info[i];
    uint32_t w0 = fetches.templates[3 * i];
    uint32_t w1 = fetches.templates[3 * i + 1];
    uint32_t w2 = fetches.templates[3 * i + 2];
    const uint32_t usage = (word >> 12) & 0xF, usage_index = (word >> 16) & 0xF;
    const DeclarationElement* element = nullptr;
    for (const DeclarationElement& e : elements) {
      if (e.usage == usage && e.usage_index == usage_index) {
        element = &e;
        break;
      }
    }
    uint32_t selects;
    if (!element) {
      // Fetch constant 95 (stream 0) with format 6 (probably 8_8_8_8)
      // added to the template's, at offset 0.
      w0 = (w0 & ~0x3FF00000u) | (95u << 20);
      w1 = (w1 & 0xBFC6CFFFu) | 0x60000u;
      w2 = (w2 & 0x80000000u) | strides[0];
      selects = (components & 0xF) | 0x9250;
    } else {
      const uint32_t type = element->type;
      // The type's four component selects (bits 19-21, 16-18, 13-15, 10-12)
      // at bits 4, 7, 10 and 13.
      selects = ((((type >> 10) & 7) << 13) | (((type >> 13) & 7) << 10) |
                 (((type >> 16) & 7) << 7) | (((type >> 19) & 7) << 4) | (components & 0xF)) &
                0xFFFF;
      if ((type & 0xC0) == 0x40) {
        // Components 0-3 swapped in pairs (x with y, z with w).
        for (int shift : {13, 10, 7, 4}) {
          if (((selects >> shift) & 7) <= 3) {
            selects ^= 1u << shift;
          }
        }
      }
      // The format (type bits 0-5) and its number format (bits 8-9).
      w1 = (w1 & 0xBFC0CFFFu) | ((type & 0x3F) << 16) | ((type & 0x300) << 4);
      // Fetch constant 95 - stream: index (÷ 3) at bits 20-24, select
      // (mod 3) at 25-26, through the original's multiply by 0x01560056.
      const uint32_t product = (95u - element->stream) * 0x01560056u;
      const uint32_t constant = (product & ~0xF80000u) | (Rotl(product, 11) & 0xF80000u);
      w0 = ((w0 & ~0x7F00000u) | (Rotl(constant, 1) & 0x7F00000u)) & 0xC7FFFFFFu;
      const uint8_t stride = element->stream < 16 ? strides[element->stream] : 0;
      w2 = (w2 & 0x80000000u) | (Rotl(element->offset, 6) & 0x7FFFFF00u) | stride;
    }
    selects &= 0xFFFF;
    components = (selects | 14) & 0xFFFF;
    out[i] = {w0, FitSelects(tables, w1, components), w2};
  }

  // The groups between marked fetches (info bits 20-21): sorted, then
  // neighbors reading the same constant within 8 dwords joined.
  auto marked = [&](size_t i) { return (fetches.info[i] & 0x300000) != 0; };
  size_t next = 0;
  while (next < count) {
    while (next < count && !marked(next)) {
      next++;
    }
    if (next >= count) {
      break;
    }
    const size_t first = next;
    size_t last = first + 1;
    while (last < count && !marked(last)) {
      last++;
    }
    if (last >= count) {
      break;
    }
    next = last + 1;
    const uint32_t span = uint32_t(last - first);
    // A last fetch marked with both bits isn't sorted with the others, and
    // its constant's fetches sort after the rest.
    uint32_t sorted = span + 1, passes = span;
    int32_t last_key = -1;
    if ((fetches.info[last] & 0x300000) == 0x300000) {
      sorted = span;
      passes = span - 1;
      last_key = int32_t(ConstantKey(out[last].w[0]));
    }
    for (uint32_t pass = 0; pass < passes; pass++) {
      Instruction& x = out[first + pass];
      for (uint32_t j = pass + 1; j < sorted; j++) {
        Instruction& y = out[first + j];
        const int32_t key_x = int32_t(ConstantKey(x.w[0]));
        const int32_t key_y = int32_t(ConstantKey(y.w[0]));
        int32_t order = key_y - key_x;
        bool swap;
        if (order != 0) {
          if (last_key == key_x) {
            swap = true;
          } else {
            if (last_key == key_y) {
              order = -1;
            }
            swap = order > 0;
          }
        } else {
          order = Offset(x.w[2]) - Offset(y.w[2]);
          if (order == 0) {
            order = int32_t(tables.format_sizes[Format(x.w[1])]) -
                    int32_t(tables.format_sizes[Format(y.w[1])]);
          }
          swap = order > 0;
        }
        if (swap) {
          std::swap(x, y);
        }
      }
    }
    size_t previous = first;
    for (size_t i = first + 1; i < first + span + 1; i++) {
      Instruction& x = out[previous];
      Instruction& y = out[i];
      const uint32_t x_offset = uint32_t(Offset(x.w[2])), y_offset = uint32_t(Offset(y.w[2]));
      uint32_t reach = tables.format_sizes[Format(y.w[1])] - x_offset + y_offset;
      if (ConstantKey(x.w[0]) != ConstantKey(y.w[0]) || x_offset > y_offset || reach > 8) {
        previous = i;
        continue;
      }
      // x fetches far enough (bits 27-29) for y, which becomes a mini-fetch.
      reach = std::max((x.w[0] >> 27) & 7, reach - 1);
      x.w[0] = (x.w[0] & ~0x38000000u) | ((reach << 27) & 0x38000000u);
      y.w[1] |= 0x40000000u;
    }
  }

  bool inside = true;
  for (size_t i = 0; i < count; i++) {
    const size_t at = size_t(fetches.info[i] & 0xFFF) * 12;
    if (at + 12 > size) {
      inside = false;
      continue;
    }
    for (int k = 0; k < 3; k++) {
      StoreBig(microcode + at + 4 * k, out[i].w[k]);
    }
  }
  return inside && fetches.info.size() <= kMaxFetches;
}

bool ReadGuestVertexFetchInputs(uint8_t* base, uint32_t shader, uint32_t declaration,
                                GuestVertexFetchInputs& out) {
  out.fetch_count = guest::LoadU32(base, shader + 60);
  out.element_count = declaration ? guest::LoadU32(base, declaration + 24) : 0;
  if (out.fetch_count > 64 || out.element_count > 64) {
    return false;
  }
  for (uint32_t i = 0; i < out.fetch_count; i++) {
    out.info[i] = guest::LoadU32(base, shader + 64 + 4 * i);
    for (uint32_t k = 0; k < 3; k++) {
      out.templates[3 * i + k] = guest::LoadU32(base, shader + 192 + 12 * i + 4 * k);
    }
  }
  for (uint32_t i = 0; i < out.element_count; i++) {
    const uint32_t e = declaration + 52 + 12 * i;
    out.elements[i] = {guest::LoadU16(base, e), guest::LoadU16(base, e + 2),
                       guest::LoadU32(base, e + 4), guest::LoadU8(base, e + 8),
                       guest::LoadU8(base, e + 9), guest::LoadU8(base, e + 10)};
  }
  return true;
}

}  // namespace native
