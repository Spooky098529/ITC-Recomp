// The graphics library's vertex shader patcher: see shader_patch.h. Every
// read and write is the original's (sub_822AE340), in its order; the bit
// operations are its rlwinm and rlwimi, on 32-bit words.

#include "graphics/shader_patch.h"

#include "guest.h"

#include "base/log.h"

#include <bit>
#include <cstdint>

namespace graphics::shader_patch {

namespace {

constexpr uint32_t kSwizzleMasks = 0x82014224;  // 8 halfwords
constexpr uint32_t kFormatSizes = 0x82014238;   // a byte a fetch format (64)

uint32_t Rotl(uint32_t value, int bits) { return std::rotl(value, bits); }

// rlwimi: `target`'s bits outside `mask` kept, `value` rotated by `bits`
// inside it.
uint32_t Insert(uint32_t target, uint32_t value, int bits, uint32_t mask) {
  return (Rotl(value, bits) & mask) | (target & ~mask);
}

// The swizzle's four 3-bit selects at bits 0-2, 3-5, 6-8 and 9-11 of the
// fetch's second word, each folded through the format's mask (the table's
// halfword for the select) as the original does.
uint32_t Swizzle(uint32_t word, uint32_t components) {
  const uint32_t c = components & 0xFFFF;
  // Select 0 (bits 0-2), from the table by bits 0-2 of `word`.
  uint32_t r11 = guest::LoadU16(kSwizzleMasks + (Rotl(word, 1) & 0xE)) & c;
  r11 &= 0xFFFF;
  uint32_t r9 = r11 >> 3;
  r9 |= r11;
  r9 >>= 3;
  r9 |= r11;
  r9 >>= 3;
  r9 |= r11;
  r9 >>= 3;
  r11 = r9 | r11;
  uint32_t r10 = Insert(word, r11, 31, 0x7);
  r9 = r10;
  // Select 1 (bits 3-5).
  r11 = guest::LoadU16(kSwizzleMasks + (Rotl(r9, 30) & 0xE)) & c;
  r11 &= 0xFFFF;
  r10 = r11 >> 3;
  uint32_t r29 = r11 << 2;
  r10 |= r11;
  r10 >>= 3;
  r10 |= r11;
  r10 >>= 3;
  r11 = r10 | r11;
  r11 >>= 1;
  r10 = r11 | r29;
  r10 = (r10 & 0x38) | (r9 & ~0x38u);
  // Select 2 (bits 6-8).
  r11 = guest::LoadU16(kSwizzleMasks + (Rotl(r10, 27) & 0xE)) & c;
  r11 &= 0xFFFF;
  r9 = r11 >> 3;
  r29 = r11 << 3;
  r9 |= r11;
  r29 |= r11;
  r9 >>= 3;
  r29 <<= 2;
  r11 = r9 | r11;
  r11 >>= 1;
  r9 = r11 | r29;
  r9 = (r9 & 0x1C0) | (r10 & ~0x1C0u);
  // Select 3 (bits 9-11).
  r11 = guest::LoadU16(kSwizzleMasks + (Rotl(r9, 24) & 0xE)) & c;
  r11 &= 0xFFFF;
  r10 = r11 << 3;
  r29 = r11 >> 3;
  r10 |= r11;
  r29 |= r11;
  r10 <<= 3;
  r29 >>= 1;
  r11 = r10 | r11;
  r11 <<= 2;
  r11 |= r29;
  return (r11 & 0xE00) | (r9 & ~0xE00u);
}

// A record's group key: its first word with bits 12-13 taken from 19-20
// (rlwimi 25, 12, 13).
uint32_t GroupWord(uint32_t word) { return Insert(word, word, 25, 0x000C0000); }

// A record's third word's offset field: (word << 1) >> 9, signed.
int32_t OffsetOf(uint32_t word) { return int32_t(word << 1) >> 9; }

}  // namespace

uint32_t PatchVertexShader(uint32_t shader, uint32_t microcode,
                           uint32_t declaration, uint32_t streams, uint32_t sp, Copy copy,
                           void* context) {
  ITC_NATIVE_RUNNING("sub_822AE340 Device::PatchVertexShader");
  const uint32_t frame = sp - 592;
  guest::StoreU32(frame, sp);
  const uint32_t records = frame + 96;
  const uint32_t count = guest::LoadU32(shader + 60);
  const uint32_t fetches = shader + 64;
  const uint32_t elements = guest::LoadU32(declaration + 24);
  uint32_t r3 = shader;

  // The fetch records, one a fetch, from the instructions and the
  // declaration's elements.
  if (count != 0) {
    // The original reads its frame's +80 here without having written it.
    uint32_t r8 = guest::LoadU16(frame + 80);
    uint32_t fetch = fetches;
    uint32_t record = records;
    uint32_t instruction = shader + 192;
    for (uint32_t left = count; left != 0; left--) {
      uint32_t r11 = guest::LoadU32(instruction + 0);
      uint32_t r28 = guest::LoadU32(instruction + 4);
      uint32_t r27 = guest::LoadU32(instruction + 8);
      uint32_t element = declaration + 52;
      uint32_t i = 0;
      if (elements != 0) {
        const uint32_t usage_word = guest::LoadU32(fetch);
        const uint32_t usage = Rotl(usage_word, 20) & 0xF;
        for (; i < elements; i++, element += 12) {
          if (guest::LoadU8(element + 9) == usage &&
              guest::LoadU8(element + 10) == (Rotl(usage_word, 16) & 0xF)) {
            break;
          }
        }
      }
      uint32_t r5, r9, r10;
      if (i < elements) {
        // The element found: its type's fields into the instruction.
        r10 = guest::LoadU32(element + 4);
        r9 = r8 & 0xF;
        uint32_t r29 = Rotl(r10, 16) & 7;
        r8 = Rotl(r10, 22) & 0x38;
        uint32_t r21 = Rotl(r10, 13) & 7;
        r8 |= r29;
        r29 = r10 & 0xFC00;
        r8 = r8 << 3;
        const uint32_t r20 = r10 & 0xC0;
        r8 |= r21;
        r8 = r8 << 1;
        r8 |= r29;
        r8 = r8 << 3;
        r9 = r8 | r9;
        r9 &= 0xFFFF;
        if (r20 == 64) {
          // Each 3-bit select at or below its limit has its low bit flipped.
          r8 = r9 & 0xFFFF;
          if ((r8 & 0xE000) <= 0x6000) r9 = ((r8 & 0xE000) ^ 0x2000) | (r8 & 0x1FFF);
          r8 = r9 & 0xFFFF;
          if ((r8 & 0x1C00) <= 0xC00) r9 = ((r8 & 0x1C00) ^ 0x400) | (r8 & 0xE3FF);
          r8 = r9 & 0xFFFF;
          if ((r8 & 0x380) <= 0x180) r9 = ((r8 & 0x380) ^ 0x80) | (r8 & 0xFC7F);
          r8 = r9 & 0xFFFF;
          if ((r8 & 0x70) <= 0x30) r9 = ((r8 & 0x70) ^ 0x10) | (r8 & 0xFF8F);
        }
        r21 = Rotl(r10, 12) & 0x3F000;
        const uint32_t stream = guest::LoadU16(element + 0);
        r10 = r10 & 0x300;
        r5 = guest::LoadU16(element + 2);
        r10 = r21 | r10;
        r10 = r10 << 4;
        r21 = guest::LoadU8(stream + streams);
        r28 = r28 & 0xBFC0CFFF;
        r10 |= r28;
        const uint32_t product = (95u - stream) * 0x01560056u;
        r5 = Rotl(r5, 6) & 0x7FFFFF00;
        r29 = product;
        r27 = r27 & 0x80000000;
        r5 |= r21;
        r29 = Insert(r29, product, 11, 0x00F80000);
        r5 |= r27;
        r11 = Insert(r11, r29, 1, 0x07F00000);
        r11 &= 0xC7FFFFFF;
      } else {
        // No element: stream 95, the first stream byte.
        r5 = guest::LoadU8(streams);
        r9 = r8 & 0xF;
        r11 = Insert(r11, 95, 20, 0x3FF00000);
        r10 = r28 & 0xBFC6CFFF;
        r8 = r27 & 0x80000000;
        r9 |= 0x9250;
        r10 |= 0x60000;
        r5 = r8 | r5;
      }
      r9 &= 0xFFFF;
      guest::StoreU32(record + 0, r11);
      guest::StoreU32(record + 8, r5);
      r8 = r9 | 14;
      instruction += 12;
      fetch += 4;
      guest::StoreU32(record + 4, Swizzle(r10, r8));
      record += 12;
    }
    r3 = instruction;
  }

  // Each group (a fetch with bits 20-21 set to the next such fetch) sorted
  // and its neighbors merged.
  const uint32_t end = fetches + count * 4;
  uint32_t at = fetches;
  if (fetches < end) {
    while (at < end) {
      while (at < end && (guest::LoadU32(at) & 0x00300000) == 0) {
        at += 4;
      }
      if (at >= end) break;
      const uint32_t first = at;
      uint32_t next = at + 4;
      if (next >= end) break;
      while (next < end && (guest::LoadU32(next) & 0x00300000) == 0) {
        next += 4;
      }
      if (next >= end) break;
      const uint32_t next_word = guest::LoadU32(next);
      const uint32_t span = (int32_t(next - first) >> 2);
      const uint32_t group = records + (int32_t(first - fetches) >> 2) * 12;
      const uint32_t total = span + 1;
      at = next + 4;
      int32_t last_stream = -1;
      uint32_t sorted = total;   // r5
      uint32_t passes = span;    // r10
      if ((next_word & 0x00300000) == 0x00300000) {
        const uint32_t last = guest::LoadU32(records + (int32_t(next - fetches) >> 2) * 12);
        sorted = span;
        passes = span - 1;
        last_stream = int32_t(Rotl(GroupWord(last), 14) & 0x7F);
      }
      r3 = group;
      if (passes != 0) {
        uint32_t a = group;
        for (uint32_t pass = 1, left = passes; left != 0; left--, pass++, a += 12) {
          if (pass >= sorted) continue;
          uint32_t b = a + 12;
          for (uint32_t inner = sorted - pass; inner != 0; inner--, b += 12) {
            const uint32_t sa = Rotl(GroupWord(guest::LoadU32(a)), 14) & 0x7F;
            const uint32_t sb = Rotl(GroupWord(guest::LoadU32(b)), 14) & 0x7F;
            int32_t order = int32_t(sb - sa);
            bool swap;
            if (order != 0) {
              if (last_stream == int32_t(sa)) {
                swap = true;
              } else {
                if (last_stream == int32_t(sb)) order = -1;
                swap = order > 0;
              }
            } else {
              order = OffsetOf(guest::LoadU32(a + 8)) - OffsetOf(guest::LoadU32(b + 8));
              if (order == 0) {
                const uint32_t fa = guest::LoadU16(a + 4) & 0x3F;
                const uint32_t fb = guest::LoadU16(b + 4) & 0x3F;
                order = int32_t(guest::LoadU8(kFormatSizes + fa)) -
                        int32_t(guest::LoadU8(kFormatSizes + fb));
              }
              swap = order > 0;
            }
            if (swap) {
              const uint32_t b0 = guest::LoadU32(b + 0);
              const uint32_t a0 = guest::LoadU32(a + 0);
              const uint32_t a1 = guest::LoadU32(a + 4);
              const uint32_t a2 = guest::LoadU32(a + 8);
              guest::StoreU32(a + 0, b0);
              guest::StoreU32(a + 4, guest::LoadU32(b + 4));
              guest::StoreU32(a + 8, guest::LoadU32(b + 8));
              guest::StoreU32(b + 0, a0);
              guest::StoreU32(b + 4, a1);
              guest::StoreU32(b + 8, a2);
            }
          }
        }
      }
      // Neighbors of one stream within 8 words merged into the first.
      const uint32_t group_end = total * 12 + group;
      uint32_t head = group;
      for (uint32_t r = group + 12; r < group_end; r += 12) {
        const uint32_t rw = guest::LoadU32(r);
        const uint32_t hw = guest::LoadU32(head);
        if ((GroupWord(hw) & 0x01FC0000) != (GroupWord(rw) & 0x01FC0000)) {
          head = r;
          continue;
        }
        const uint32_t head_offset = uint32_t(OffsetOf(guest::LoadU32(head + 8)));
        const uint32_t offset = uint32_t(OffsetOf(guest::LoadU32(r + 8)));
        if (head_offset > offset) {
          head = r;
          continue;
        }
        const uint32_t size = guest::LoadU8(kFormatSizes + (guest::LoadU16(r + 4) & 0x3F));
        uint32_t reach = size - head_offset + offset;
        if (reach > 8) {
          head = r;
          continue;
        }
        const uint32_t current = Rotl(hw, 5) & 7;
        reach -= 1;
        if (current > reach) reach = current;
        guest::StoreU32(head, Insert(hw, reach, 27, 0x38000000));
        guest::StoreU32(r + 4, guest::LoadU32(r + 4) | 0x40000000);
      }
    }
  }

  // The records over the microcode's instructions.
  uint32_t record = records;
  for (uint32_t f = fetches; f < end; f += 4, record += 12) {
    r3 = (guest::LoadU32(f) & 0xFFF) * 12 + microcode;
    if (r3 != record) {
      copy(context, r3, record, 12);
    }
  }
  return r3;
}

}  // namespace graphics::shader_patch
