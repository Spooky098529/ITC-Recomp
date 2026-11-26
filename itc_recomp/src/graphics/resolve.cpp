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

uint32_t Load(uint32_t address) {
  return guest::LoadU32(address);
}
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
uint64_t Load64(uint32_t address) {
  return (uint64_t(Load(address)) << 32) | Load(address + 4);
}
void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}
void Or64(uint32_t address, uint64_t bits) {
  Store64(address, Load64(address) | bits);
}
void And64(uint32_t address, uint64_t bits) {
  Store64(address, Load64(address) & bits);
}

}  // namespace

void Resolve(uint32_t sp, uint64_t r13, uint32_t device, uint32_t flags,
             uint32_t rect, uint32_t texture, uint32_t point, uint32_t level, uint32_t slice,
             uint32_t color, double clear_depth) {
  ITC_NATIVE_RUNNING("sub_822AFA58 Device::Resolve");
  const uint32_t frame = lifted_call::PushFrame(sp, 368);
  auto call = [&](uint32_t function, std::initializer_list<uint64_t> args) {
    return lifted_call::Call(function, frame, r13, args).r3;
  };
  // The write position, after ending the segment when it has passed the
  // soft end (the original's checks before its packets).
  auto position = [&]() {
    const uint32_t p = Load(device + kWritePosition);
    if (p > Load(device + kSoftEnd)) {
      return uint32_t(call(kEndSegment, {device}));
    }
    return p;
  };

  // The source: the render target r4's low bits pick (4 the depth buffer),
  // its kind into bits 4-6 when none are given.
  const uint32_t target = flags & 7;
  Store(frame + 124, target);
  if ((flags & Mask(25, 27)) == 0) {
    const uint32_t surface = target == 4 ? Load(device + 12200)
                                         : Load(device + 4 * (target + 3046));
    const uint32_t kind = guest::LoadU16(surface + 24) & 3;
    flags |= kind == 0 ? 16 : kind == 1 ? 80 : 112;
  }
  const float zero = guest::LoadFloat(kZero);  // f31
  if (color == 0) {
    for (uint32_t i = 0; i < 4; i++) {
      guest::StoreFloat(frame + 176 + 4 * i, zero);
    }
    color = frame + 176;
  }

  // The destination's size and pitch from its header (or its level's).
  {
    const uint32_t w48 = Load(texture + 48) & Mask(21, 22);
    const uint32_t r11 = Rlwinm(Load(texture + 40), 2, 30, 30) + 1;
    const uint32_t w36 = Load(texture + 36);
    uint32_t r9, r10;
    if (w48 == 1024) {
      r9 = w36 & 0x7FF;
      r10 = Rlwinm(w36, 21, 21, 31);
    } else {
      r9 = w36 & 0x1FFF;
      r10 = Rlwinm(w36, 19, 19, 31);
    }
    Store(frame + 116, r9 + r11);
    Store(frame + 112, r10 + r11);
  }
  uint32_t height = Load(frame + 112);  // r25
  const uint32_t w32 = Load(texture + 32);
  uint32_t pitch = Rlwinm(Load(texture + 28), 15, 18, 26);  // r17
  uint32_t data = w32 & Mask(0, 19);                               // r9
  const uint32_t bytes = Rlwinm(guest::LoadU16(kFormatTable + Rlwinm(w32, 1, 25, 30)), 29,
                                27, 31);  // r14
  Store(frame + 120, data);
  if (level != 0 || slice != 0) {
    Store(frame + 92, frame + 136);
    Store(frame + 84, frame + 112);
    Store(frame + 100, frame + 120);
    call(kLevelOffset, {texture, slice, level, frame + 128, frame + 160, frame + 152, frame + 144,
                        frame + 116});
    const uint32_t row = Load(frame + 128);
    height = Load(frame + 112);
    data = Load(frame + 120);
    if (bytes == 0) {
      ITC_LOG_ERROR("Device::Resolve: a format of 0 bytes (the original traps)");
    }
    pitch = bytes != 0 ? row / bytes : 0;
  }
  if (rect == 0) {
    Store(frame + 160, 0);
    Store(frame + 164, 0);
    Store(frame + 172, height);
    Store(frame + 168, Load(frame + 116));
    rect = frame + 160;
  }
  if (point == 0) {
    point = kDefaultPoint;
  }

  // The copy registers.
  const uint32_t dest_x = Load(point + 0);
  const uint32_t dest_y = Load(point + 4);
  const uint32_t rows = height - dest_y;  // r20
  const uint32_t w40 = Load(texture + 40);
  const uint32_t w28 = Load(texture + 28);
  Store(frame + 128, rows);
  uint32_t offset = (dest_y - Load(rect + 4)) * pitch +
                    Rlwinm(dest_x - Load(rect + 0), 5, 0, 26);
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
  if (lifted::f822AF2D8(format) != 0) {
    number = 7;
  }
  r27 = Rlwimi(r27, endian, 8, 23, 23);
  const uint32_t tiled = Load(texture + 48) & Mask(21, 22);
  const uint32_t r7 = Rlwinm(w32, 26, 30, 31);
  number = Rlwimi(number, r27 & 319, 3, 0, 28);
  format = Rlwimi(format, number, 6, 0, 25);
  const uint32_t dest_pitch = Rlwinm(rows, 16, 2, 15) | (pitch & 0x3FFF);  // r8
  uint32_t dest_info = Rlwinm(format, 7, 0, 24) | r7;                        // r11
  if (tiled == 1024) {
    Store(device + kCopyFunc, pitch * height);
    dest_info |= Rlwinm(slice, 4, 25, 27);
    Or64(device + kOtherStateDirty, 4);
    dest_info |= 8;
  }
  uint32_t sample = 1;  // r9
  if (flags & Mask(18, 18)) {
    sample = 3;
  } else if (Load(frame + 124) == 4) {
    sample = 0;
  }
  const uint32_t r10 = (flags & 887) - 16;
  Store(device + kCopyDestInfo, dest_info);
  const uint32_t r9 = Rlwinm(sample, 20, 10, 11);
  Store(device + kCopyDestBase, dest_base);
  Store(device + kCopyDestPitch, dest_pitch);
  const uint32_t clear_argument = Load(sp + 100);  // r29
  const uint32_t control = r9 | r10;
  Store(device + kCopyControl, control);
  Or64(device + kOtherStateDirty, 15360);
  const bool clear_depth_flag = (flags & Mask(22, 22)) != 0;  // r25
  if (clear_depth_flag) {
    lifted::f822AF438(frame, r13, device, clear_argument, Load(sp + 92), clear_depth);
  }
  if (flags & Mask(23, 23)) {
    lifted::Vec v1;
    const uint8_t* source = guest::Pointer(color & ~0xFu);
    for (int i = 0; i < 16; i++) {
      v1.u8[i] = source[15 - i];  // lvx128: the 16 bytes, reversed as the generated code's mask
    }
    lifted::f822AF8A0(frame, r13, device, clear_argument, control & 7, r7, dest_pitch, r9,
                      r10, clear_depth, v1);
  }

  // The source rectangle, 8-aligned (for the screen scissor, below).
  const uint32_t x0 = Load(rect + 0);
  const uint32_t y0 = Load(rect + 4);
  const uint32_t x1 = (Load(rect + 8) + 7) & ~7u;
  const uint32_t y1 = (Load(rect + 12) + 7) & ~7u;
  // Device::SetScreenScissor (sub_822A0F80, Dropped: only packets) when the
  // rectangle is outside the scissor (+10308, +10312): left out, and its
  // second call after the copy too.

  // The dirty masks' flush (their packets Dropped), the shader and render
  // state first. Bit 8 of the shader state (sub_822ADC08's, Dropped) stays.
  if (Load64(device + kShaderStateDirty) != 0) {
    uint64_t s = Load64(device + kShaderStateDirty);
    if (s & 0xFFF) {
      Store64(device + kShaderStateDirty, Load64(device + kShaderStateDirty) & ~0xFFFull);
    }
    s = Load64(device + kShaderStateDirty);
    if (s & (31ull << 44)) {
      And64(device + kShaderStateDirty, ~(31ull << 44));
    }
  }
  if (Load64(device + kRenderStateDirty) != 0) {
    uint64_t r = Load64(device + kRenderStateDirty);
    if (r & Mask(9, 29)) {
      And64(device + kRenderStateDirty, 0xFFFFFFFFFF800003ull);
    }
    r = Load64(device + kRenderStateDirty);
    if (r & 0x0000007FFF800000ull) {
      And64(device + kRenderStateDirty, ~0x0000007FFF800000ull);
    }
    r = Load64(device + kRenderStateDirty);
    if (r & 0x0FFFFF8000000000ull) {
      And64(device + kRenderStateDirty, ~0x0FFFFF8000000000ull);
    }
  }
  if (const uint64_t o = Load64(device + kOtherStateDirty);
      o != 0 && (o & 0x0000003FFFFFFFFFull) != 0) {
    And64(device + kOtherStateDirty, 0xFFFFFFC000000000ull);
  }

  // Under predicated tiling (+10808 bit 0x10, or 0x20 with the render
  // targets those the tiling began with), a copy a tile: packets only, the
  // segment checked before them and after each tile.
  {
    const uint8_t tiling = guest::LoadU8(device + 10808);
    bool tiled_copy = false;
    if (tiling & 0x10) {
      tiled_copy = true;
    } else if (tiling & 0x20) {
      tiled_copy = true;
      for (uint32_t i = 0; i < 5; i++) {
        const uint32_t now = Load(device + 12184 + 4 * i);
        if (Load(device + 12468 + 4 * i) != now && now != 0) {
          tiled_copy = false;
          break;
        }
      }
    }
    if (tiled_copy) {
      uint32_t q = position();
      for (uint32_t i = 0; i < Load(device + 12488); i++) {
        Store(device + kWritePosition, q);
        if (q > Load(device + kSoftEnd)) {
          q = uint32_t(call(kEndSegment, {device}));
        }
      }
      Store(device + kWritePosition, q);
    }
  }
  uint32_t p = position();
  if (guest::LoadU8(device + kFlags2) & 0x10) {
    // Three cache flush events: Dropped.
    guest::StoreU8(device + kFlags2, guest::LoadU8(device + kFlags2) & 239);
  }
  // The fixed registers' 60 bytes (memcpy) and the copy's packets: Dropped.
  if (clear_depth_flag) {
    // The depth control's value for sub_822ADC08's packet (from +10810 and
    // +10432) isn't made: its packet is Dropped.
    guest::StoreU8(device + kFlags1, uint8_t(guest::LoadU8(device + kFlags1) | 4));
    Or64(device + kShaderStateDirty, 1ull << 49);
  }
  Store(device + kWritePosition, p);
  p = position();
  p = uint32_t(call(kDefaultDrawRegisters, {device, p}));
  Store(device + kWritePosition, p);
  if (samples == 3 && Load(frame + 124) != 4) {
    const uint32_t index = target == 0 ? 0 : target + 1;  // r30
    const uint32_t info = Load(device + 4 * (index + 2561));
    if ((info & Mask(12, 15)) == 0x10000) {
      p = position();
      const uint32_t bit = (282 + index) & 0xFF;
      Or64(device + kRenderStateDirty, (1ull << 63) >> bit);
      Store(device + kWritePosition, p);
    }
  }
  p = position();
  const uint32_t space = uint32_t(call(kReserve, {device, 6, 4}));
  if (space == 0) {
    Store(device + kWritePosition, p);
    return;
  }
  // The copy's rectangle as six floats in `space`: Dropped. Its integer
  // conversions' stack words are kept (the fence list's entry below takes
  // its top bits from frame +160, as the original).
  Store64(frame + 160, uint64_t(int64_t(int32_t(x0))));
  Store64(frame + 152, uint64_t(int64_t(int32_t(y0))));
  Store64(frame + 144, uint64_t(int64_t(int32_t(x1 - x0))));
  Store64(frame + 136, uint64_t(int64_t(int32_t(y1 - y0))));
  Store(device + kWritePosition, p);
  p = position();
  Store(device + kWritePosition, p);

  Store(texture + 8, Load(device + kFence));
  Or64(device + kShaderStateDirty,
       (1ull << 51) | 8 | (1ull << 43) | (3ull << 47) | 2048 | 256);
  p = position();
  Store(device + kCopyControl, 0);
  Store(device + kCopyDestInfo, 0);
  Store(device + kWritePosition, p);
  if (const uint32_t fence = Load(device + kFence); fence != 0) {
    Store(texture + 8, fence);
    Store(texture + 12, fence);
    return;
  }
  if ((Load(device + kFenceMask) & Load(texture + 0)) == 0) {
    return;
  }
  uint32_t entry = Load(device + kFenceList);
  if (entry >= Load(device + kFenceListEnd)) {
    entry = uint32_t(lifted::f822AD790(frame, r13, device, 0, 0, 0.0).r3);
  }
  const uint32_t word = Rlwimi(Load(frame + 160), texture, 30, 2, 31) | 0x40000000;
  Store(frame + 164, 0xFFFFFFFFu);
  Store(frame + 160, word);
  Store64(entry, Load64(frame + 160));
  Store(device + kFenceList, entry + 8);
}

}  // namespace graphics::resolve
