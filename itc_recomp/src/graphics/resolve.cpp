// Device::Resolve (sub_822AFA58): see resolve.h. Every kept read and write
// is the original's, in its order; the bit operations are its rlwinm,
// rlwimi and rldicr. Register names in the comments are the original's.

#include "graphics/resolve.h"

#include "guest.h"

#include "base/log.h"
#include "hooks/lifted_call.h"
#include "lifted/functions.h"
#include "native/state.h"

#include <bit>

namespace graphics::resolve {

namespace {

constexpr uint32_t kEndSegment = 0x8229EF48;          // Device::EndSegment (its override)
constexpr uint32_t kReserve = 0x8229E308;             // Device::AllocateFromCommandBuffer
constexpr uint32_t kLevelOffset = 0x822A4FF0;         // Texture::LevelOffset (its override)
constexpr uint32_t kDefaultDrawRegisters = 0x822AF978;  // its override
constexpr uint32_t kZero = 0x82000A34;                // 0.0f
constexpr uint32_t kFormatTable = 0x82013468;         // a halfword a texture format
constexpr uint32_t kDefaultPoint = 0x8204A9F4;        // (0, 0)

// The device's fields.
constexpr uint32_t kShaderStateDirty = 16;
constexpr uint32_t kRenderStateDirty = 24;
constexpr uint32_t kOtherStateDirty = 32;
constexpr uint32_t kWritePosition = 40;
constexpr uint32_t kSoftEnd = 48;
constexpr uint32_t kCopyControl = 10648;   // RB_COPY_CONTROL (0x2318)
constexpr uint32_t kCopyDestBase = 10652;  // RB_COPY_DEST_BASE
constexpr uint32_t kCopyDestPitch = 10656;
constexpr uint32_t kCopyDestInfo = 10660;
constexpr uint32_t kCopyFunc = 10692;
constexpr uint32_t kFlags1 = 10809;  // byte
constexpr uint32_t kFlags2 = 10810;  // byte
constexpr uint32_t kFence = 10780;
constexpr uint32_t kFenceMask = 10784;
constexpr uint32_t kFenceList = 13168;
constexpr uint32_t kFenceListEnd = 13172;

uint32_t Mask(int mb, int me) {
  const uint32_t from = 0xFFFFFFFFu >> mb;
  const uint32_t to = 0xFFFFFFFFu << (31 - me);
  return mb <= me ? (from & to) : (from | to);
}
uint32_t Rlwinm(uint32_t x, int sh, int mb, int me) {
  return std::rotl(x, sh) & Mask(mb, me);
}
uint32_t Rlwimi(uint32_t target, uint32_t x, int sh, int mb, int me) {
  const uint32_t m = Mask(mb, me);
  return (std::rotl(x, sh) & m) | (target & ~m);
}
int32_t Srawi(uint32_t x, int sh) {
  return int32_t(x) >> sh;
}

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(Load(base, address)) << 32) | Load(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
void Or64(uint8_t* base, uint32_t address, uint64_t bits) {
  Store64(base, address, Load64(base, address) | bits);
}
void And64(uint8_t* base, uint32_t address, uint64_t bits) {
  Store64(base, address, Load64(base, address) & bits);
}

}  // namespace

void Resolve(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t device, uint32_t flags,
             uint32_t rect, uint32_t texture, uint32_t point, uint32_t level, uint32_t slice,
             uint32_t color, double clear_depth) {
  ITC_NATIVE_RUNNING("sub_822AFA58 Device::Resolve");
  const uint32_t frame = lifted_call::PushFrame(base, sp, 368);
  auto call = [&](uint32_t function, std::initializer_list<uint64_t> args) {
    return lifted_call::Call(base, function, frame, r13, args).r3;
  };
  // The write position, after ending the segment when it has passed the
  // soft end (the original's checks before its packets).
  auto position = [&]() {
    const uint32_t p = Load(base, device + kWritePosition);
    if (p > Load(base, device + kSoftEnd)) {
      return uint32_t(call(kEndSegment, {device}));
    }
    return p;
  };

  // The source: the render target r4's low bits pick (4 the depth buffer),
  // its kind into bits 4-6 when none are given.
  const uint32_t target = flags & 7;
  Store(base, frame + 124, target);
  if ((flags & Mask(25, 27)) == 0) {
    const uint32_t surface = target == 4 ? Load(base, device + 12200)
                                         : Load(base, device + 4 * (target + 3046));
    const uint32_t kind = guest::LoadU16(base, surface + 24) & 3;
    flags |= kind == 0 ? 16 : kind == 1 ? 80 : 112;
  }
  const float zero = guest::LoadFloat(base, kZero);  // f31
  if (color == 0) {
    for (uint32_t i = 0; i < 4; i++) {
      guest::StoreFloat(base, frame + 176 + 4 * i, zero);
    }
    color = frame + 176;
  }

  // The destination's size and pitch from its header (or its level's).
  {
    const uint32_t w48 = Load(base, texture + 48) & Mask(21, 22);
    const uint32_t r11 = Rlwinm(Load(base, texture + 40), 2, 30, 30) + 1;
    const uint32_t w36 = Load(base, texture + 36);
    uint32_t r9, r10;
    if (w48 == 1024) {
      r9 = w36 & 0x7FF;
      r10 = Rlwinm(w36, 21, 21, 31);
    } else {
      r9 = w36 & 0x1FFF;
      r10 = Rlwinm(w36, 19, 19, 31);
    }
    Store(base, frame + 116, r9 + r11);
    Store(base, frame + 112, r10 + r11);
  }
  uint32_t height = Load(base, frame + 112);  // r25
  const uint32_t w32 = Load(base, texture + 32);
  uint32_t pitch = Rlwinm(Load(base, texture + 28), 15, 18, 26);  // r17
  uint32_t data = w32 & Mask(0, 19);                               // r9
  const uint32_t bytes = Rlwinm(guest::LoadU16(base, kFormatTable + Rlwinm(w32, 1, 25, 30)), 29,
                                27, 31);  // r14
  Store(base, frame + 120, data);
  if (level != 0 || slice != 0) {
    Store(base, frame + 92, frame + 136);
    Store(base, frame + 84, frame + 112);
    Store(base, frame + 100, frame + 120);
    call(kLevelOffset, {texture, slice, level, frame + 128, frame + 160, frame + 152, frame + 144,
                        frame + 116});
    const uint32_t row = Load(base, frame + 128);
    height = Load(base, frame + 112);
    data = Load(base, frame + 120);
    if (bytes == 0) {
      ITC_LOG_ERROR("Device::Resolve: a format of 0 bytes (the original traps)");
    }
    pitch = bytes != 0 ? row / bytes : 0;
  }
  if (rect == 0) {
    Store(base, frame + 160, 0);
    Store(base, frame + 164, 0);
    Store(base, frame + 172, height);
    Store(base, frame + 168, Load(base, frame + 116));
    rect = frame + 160;
  }
  if (point == 0) {
    point = kDefaultPoint;
  }

  // The copy registers.
  const uint32_t dest_x = Load(base, point + 0);
  const uint32_t dest_y = Load(base, point + 4);
  const uint32_t rows = height - dest_y;  // r20
  const uint32_t w40 = Load(base, texture + 40);
  const uint32_t w28 = Load(base, texture + 28);
  Store(base, frame + 128, rows);
  uint32_t offset = (dest_y - Load(base, rect + 4)) * pitch +
                    Rlwinm(dest_x - Load(base, rect + 0), 5, 0, 26);
  offset *= bytes;
  offset += (Rlwinm(data, 12, 20, 31) + 512) & Mask(19, 19);
  const uint32_t dest_base = offset + (data & 0x1FFFFFFF);  // r16
  const uint32_t endian = Rlwinm(std::countl_zero(w40 & Mask(28, 30)), 27, 31, 31) ^ 1;  // r26
  uint32_t r27 = Rlwinm(flags, 6, 26, 31);
  const uint32_t samples = Rlwinm(w28, 30, 30, 31);  // r19
  uint32_t format = w32 & 0x3F;                      // r30
  switch (format) {
    case 54: format = 7; break;
    case 55: format = 16; break;
    case 56: format = 17; break;
    case 27: format = 30; break;
    case 28: format = 31; break;
    case 29: format = 32; break;
    case 22:
    case 23: format = 6; break;
    default: break;
  }
  uint32_t number = Rlwinm(std::countl_zero(samples - 1), 27, 31, 31);  // r28
  if ((w40 & 1) == 1) {
    number += 2;
  }
  if (lifted::f822AF2D8(base, format) != 0) {
    number = 7;
  }
  r27 = Rlwimi(r27, endian, 8, 23, 23);
  const uint32_t tiled = Load(base, texture + 48) & Mask(21, 22);
  const uint32_t r7 = Rlwinm(w32, 26, 30, 31);
  number = Rlwimi(number, r27 & 319, 3, 0, 28);
  format = Rlwimi(format, number, 6, 0, 25);
  const uint32_t dest_pitch = Rlwinm(rows, 16, 2, 15) | (pitch & 0x3FFF);  // r8
  uint32_t dest_info = Rlwinm(format, 7, 0, 24) | r7;                        // r11
  if (tiled == 1024) {
    Store(base, device + kCopyFunc, pitch * height);
    dest_info |= Rlwinm(slice, 4, 25, 27);
    Or64(base, device + kOtherStateDirty, 4);
    dest_info |= 8;
  }
  uint32_t sample = 1;  // r9
  if (flags & Mask(18, 18)) {
    sample = 3;
  } else if (Load(base, frame + 124) == 4) {
    sample = 0;
  }
  const uint32_t r10 = (flags & 887) - 16;
  Store(base, device + kCopyDestInfo, dest_info);
  const uint32_t r9 = Rlwinm(sample, 20, 10, 11);
  Store(base, device + kCopyDestBase, dest_base);
  Store(base, device + kCopyDestPitch, dest_pitch);
  const uint32_t clear_argument = Load(base, sp + 100);  // r29
  const uint32_t control = r9 | r10;
  Store(base, device + kCopyControl, control);
  Or64(base, device + kOtherStateDirty, 15360);
  const bool clear_depth_flag = (flags & Mask(22, 22)) != 0;  // r25
  if (clear_depth_flag) {
    lifted::f822AF438(base, frame, r13, device, clear_argument, Load(base, sp + 92), clear_depth);
  }
  if (flags & Mask(23, 23)) {
    lifted::Vec v1;
    const uint8_t* source = base + (color & ~0xFu);
    for (int i = 0; i < 16; i++) {
      v1.u8[i] = source[15 - i];  // lvx128: the 16 bytes, reversed as the generated code's mask
    }
    lifted::f822AF8A0(base, frame, r13, device, clear_argument, control & 7, r7, dest_pitch, r9,
                      r10, clear_depth, v1);
  }

  // The source rectangle, 8-aligned (for the screen scissor, below).
  const uint32_t x0 = Load(base, rect + 0);
  const uint32_t y0 = Load(base, rect + 4);
  const uint32_t x1 = (Load(base, rect + 8) + 7) & ~7u;
  const uint32_t y1 = (Load(base, rect + 12) + 7) & ~7u;
  // Device::SetScreenScissor (sub_822A0F80, Dropped: only packets) when the
  // rectangle is outside the scissor (+10308, +10312): left out, and its
  // second call after the copy too.

  // The dirty masks' flush, the shader and render state first.
  if (const uint64_t shader = Load64(base, device + kShaderStateDirty); shader != 0) {
    if (shader & 0x100) {
      // sub_822ADC08 (Dropped: its override marks 0x2203 and returns the mask
      // without bit 8, which the original ors back in: +16 as it was).
      native::MarkRegistersDirty(0x2203, 1ull << 63);
    }
    uint64_t s = Load64(base, device + kShaderStateDirty);
    if (s & 0xFFF) {
      native::MarkRegistersDirty(8704, s << 52);
      Store64(base, device + kShaderStateDirty, Load64(base, device + kShaderStateDirty) & ~0xFFFull);
    }
    s = Load64(base, device + kShaderStateDirty);
    if (s & (31ull << 44)) {
      native::MarkRegistersDirty(8576, (s << 15) & 0xF800000000000000ull);
      And64(base, device + kShaderStateDirty, ~(31ull << 44));
    }
  }
  if (Load64(base, device + kRenderStateDirty) != 0) {
    uint64_t r = Load64(base, device + kRenderStateDirty);
    if (r & Mask(9, 29)) {
      native::MarkRegistersDirty(8448, (r << 41) & 0xFFFFF80000000000ull);
      And64(base, device + kRenderStateDirty, 0xFFFFFFFFFF800003ull);
    }
    r = Load64(base, device + kRenderStateDirty);
    if (r & 0x0000007FFF800000ull) {
      native::MarkRegistersDirty(8192, (r << 25) & 0xFFFF000000000000ull);
      And64(base, device + kRenderStateDirty, ~0x0000007FFF800000ull);
    }
    r = Load64(base, device + kRenderStateDirty);
    if (r & 0x0FFFFF8000000000ull) {
      native::MarkRegistersDirty(8832, (r << 4) & 0xFFFFF80000000000ull);
      And64(base, device + kRenderStateDirty, ~0x0FFFFF8000000000ull);
    }
  }
  if (const uint64_t o = Load64(base, device + kOtherStateDirty);
      o != 0 && (o & 0x0000003FFFFFFFFFull) != 0) {
    native::MarkRegistersDirty(8960, (o << 26) & 0xFFFFFFFFFC000000ull);
    And64(base, device + kOtherStateDirty, 0xFFFFFFC000000000ull);
  }

  // Under predicated tiling (+10808 bit 0x10, or 0x20 with the render
  // targets those the tiling began with), a copy a tile: packets only, the
  // segment checked before them and after each tile.
  {
    const uint8_t tiling = guest::LoadU8(base, device + 10808);
    bool tiled_copy = false;
    if (tiling & 0x10) {
      tiled_copy = true;
    } else if (tiling & 0x20) {
      tiled_copy = true;
      for (uint32_t i = 0; i < 5; i++) {
        const uint32_t now = Load(base, device + 12184 + 4 * i);
        if (Load(base, device + 12468 + 4 * i) != now && now != 0) {
          tiled_copy = false;
          break;
        }
      }
    }
    if (tiled_copy) {
      uint32_t q = position();
      for (uint32_t i = 0; i < Load(base, device + 12488); i++) {
        Store(base, device + kWritePosition, q);
        if (q > Load(base, device + kSoftEnd)) {
          q = uint32_t(call(kEndSegment, {device}));
        }
      }
      Store(base, device + kWritePosition, q);
    }
  }
  uint32_t p = position();
  if (guest::LoadU8(base, device + kFlags2) & 0x10) {
    // Three cache flush events: Dropped.
    guest::StoreU8(base, device + kFlags2, guest::LoadU8(base, device + kFlags2) & 239);
  }
  // The fixed registers' 60 bytes (memcpy) and the copy's packets: Dropped.
  if (clear_depth_flag) {
    // The depth control's value for sub_822ADC08's packet (from +10810 and
    // +10432) isn't made: its packet is Dropped.
    guest::StoreU8(base, device + kFlags1, uint8_t(guest::LoadU8(base, device + kFlags1) | 4));
    Or64(base, device + kShaderStateDirty, 1ull << 49);
  }
  Store(base, device + kWritePosition, p);
  native::MarkRegistersDirty(0x2203, 1ull << 63);  // sub_822ADC08 (Dropped)
  p = position();
  p = uint32_t(call(kDefaultDrawRegisters, {device, p}));
  Store(base, device + kWritePosition, p);
  if (samples == 3 && Load(base, frame + 124) != 4) {
    const uint32_t index = target == 0 ? 0 : target + 1;  // r30
    const uint32_t info = Load(base, device + 4 * (index + 2561));
    if ((info & Mask(12, 15)) == 0x10000) {
      p = position();
      const uint32_t bit = (282 + index) & 0xFF;
      Or64(base, device + kRenderStateDirty, (1ull << 63) >> bit);
      Store(base, device + kWritePosition, p);
    }
  }
  p = position();
  const uint32_t space = uint32_t(call(kReserve, {device, 6, 4}));
  if (space == 0) {
    Store(base, device + kWritePosition, p);
    return;
  }
  // The copy's rectangle as six floats in `space`: Dropped. Its integer
  // conversions' stack words are kept (the fence list's entry below takes
  // its top bits from frame +160, as the original).
  Store64(base, frame + 160, uint64_t(int64_t(int32_t(x0))));
  Store64(base, frame + 152, uint64_t(int64_t(int32_t(y0))));
  Store64(base, frame + 144, uint64_t(int64_t(int32_t(x1 - x0))));
  Store64(base, frame + 136, uint64_t(int64_t(int32_t(y1 - y0))));
  Store(base, device + kWritePosition, p);
  p = position();
  Store(base, device + kWritePosition, p);

  Store(base, texture + 8, Load(base, device + kFence));
  Or64(base, device + kShaderStateDirty,
       (1ull << 51) | 8 | (1ull << 43) | (3ull << 47) | 2048 | 256);
  p = position();
  Store(base, device + kCopyControl, 0);
  Store(base, device + kCopyDestInfo, 0);
  Store(base, device + kWritePosition, p);
  if (const uint32_t fence = Load(base, device + kFence); fence != 0) {
    Store(base, texture + 8, fence);
    Store(base, texture + 12, fence);
    return;
  }
  if ((Load(base, device + kFenceMask) & Load(base, texture + 0)) == 0) {
    return;
  }
  uint32_t entry = Load(base, device + kFenceList);
  if (entry >= Load(base, device + kFenceListEnd)) {
    entry = uint32_t(lifted::f822AD790(base, frame, r13, device, 0, 0, 0.0).r3);
  }
  const uint32_t word = Rlwimi(Load(base, frame + 160), texture, 30, 2, 31) | 0x40000000;
  Store(base, frame + 164, 0xFFFFFFFFu);
  Store(base, frame + 160, word);
  Store64(base, entry, Load64(base, frame + 160));
  Store(base, device + kFenceList, entry + 8);
}

}  // namespace graphics::resolve
