// One vertex fetch as the graphics library's vertex shader patcher makes
// it: see shader_patch.h. The operations are the original's (sub_822AE340's
// first loop), its rlwinm and rlwimi on 32-bit words.

#include "graphics/shader_patch.h"

#include "guest.h"

#include "base/log.h"

#include <bit>
#include <cstdint>

namespace graphics::shader_patch {

namespace {

constexpr uint32_t kSwizzleMasks = 0x82014224;  // 8 halfwords

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

}  // namespace

Fetch FetchFromDeclaration(uint32_t usage_word, const uint32_t original[3], uint32_t declaration,
                           const uint8_t strides[16]) {
  // The original's first loop for one fetch. Its r8 carries a nibble from
  // an unwritten stack slot into the swizzle's component mask, of which
  // only bit 0 survives (| 14), and no swizzle mask (kSwizzleMasks) has bit
  // 0: it changes nothing, so 0 here.
  uint32_t r8 = 0;
  uint32_t r11 = original[0], r28 = original[1], r27 = original[2];
  const uint32_t elements = guest::LoadU32(declaration + 24);
  uint32_t element = declaration + 52;
  uint32_t i = 0;
  const uint32_t usage = Rotl(usage_word, 20) & 0xF;
  for (; i < elements; i++, element += 12) {
    if (guest::LoadU8(element + 9) == usage &&
        guest::LoadU8(element + 10) == (Rotl(usage_word, 16) & 0xF)) {
      break;
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
    r21 = strides[stream & 15];
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
    r5 = strides[0];
    r9 = r8 & 0xF;
    r11 = Insert(r11, 95, 20, 0x3FF00000);
    r10 = r28 & 0xBFC6CFFF;
    r8 = r27 & 0x80000000;
    r9 |= 0x9250;
    r10 |= 0x60000;
    r5 = r8 | r5;
  }
  r9 &= 0xFFFF;
  return {{r11, Swizzle(r10, r9 | 14), r5}};
}

}  // namespace graphics::shader_patch
