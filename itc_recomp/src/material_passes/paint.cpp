// The paint passes over a part model's materials (see material_passes.h):
// the three colors given as hue, saturation and value turned into RGB
// (SetPaint, SetPaintBody), or the paint palette's entries by each
// material's category or shared pair (ApplyPalette). Their frames'
// temporaries are written as the originals leave them: the colors' integer
// and byte forms, the floats made from the bytes (overwritten by
// Color::HsvToRgb but for a negative hue), the (1, 1, 1, 1) blocks.

#include "material_passes/material_passes.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "models/model.h"
#include "schema/material_passes.h"

namespace material_passes {

using namespace camera_modes::steps;

namespace {

using G = schema::MaterialPassesGlobals;
using Material = schema::MaterialRecord;
using PaletteEntry = schema::PaintPaletteEntry;

// PaintPalette::Init's entry k (16 of 64 bytes).
PaletteEntry Palette(uint8_t* base, uint32_t k) {
  return PaletteEntry(base, G::paint_palette_address() + 64 * k);
}

uint64_t Load64(uint8_t* base, uint32_t a) {
  return (uint64_t(guest::LoadU32(base, a)) << 32) | guest::LoadU32(base, a + 4);
}
void Store64(uint8_t* base, uint32_t a, uint64_t value) {
  guest::StoreU32(base, a, uint32_t(value >> 32));
  guest::StoreU32(base, a + 4, uint32_t(value));
}

// The three colors' registers stored in the caller's frame (sp + 32 on),
// then each color's hue, saturation and value truncated (fctiwz) and stored
// as the packed form Color::HsvToRgb8 reads (frame +92, +96, +100, each
// integer passing through +88), and turned into bytes at +80, +84, +88.
void BytePath(uint8_t* base, uint32_t frame, uint32_t sp) {
  static constexpr uint32_t kFrom[9] = {32, 36, 40, 48, 52, 56, 64, 68, 72};
  int32_t ints[9];
  for (int i = 0; i < 9; i++) {
    ints[i] = Fctiwz(Lfs(base, sp + kFrom[i]));
  }
  const uint32_t temp = frame + 88;
  auto pass = [&](int i) {
    guest::StoreU32(base, temp, uint32_t(ints[i]));
    return guest::LoadU32(base, temp);
  };
  for (int c = 0; c < 3; c++) {
    const uint32_t packed = frame + 92 + 4 * c;
    guest::StoreU16(base, packed + 0, uint16_t(pass(3 * c + 0)));
    guest::StoreU8(base, packed + 2, uint8_t(pass(3 * c + 1)));
    guest::StoreU8(base, packed + 3, uint8_t(pass(3 * c + 2)));
  }
  HsvToRgb8(base, frame + 80, frame + 92);
  HsvToRgb8(base, frame + 84, frame + 96);
  HsvToRgb8(base, frame + 88, frame + 100);
}

// A color's bytes (frame + `bytes`) as floats 0-255 with w = 1.0 at `at`.
void ByteColor(uint8_t* base, uint32_t frame, uint32_t bytes, uint32_t at, double one) {
  for (uint32_t i = 0; i < 3; i++) {
    Stfs(base, at + 4 * i, Single(double(int64_t(guest::LoadU8(base, frame + bytes + i)))));
  }
  Stfs(base, at + 12, one);
}

// The 64-bit words the byte-to-float conversion leaves (each byte through
// a doubleword for fcfid): `slots[i]` holds byte `bytes[i]`.
void ByteSlots(uint8_t* base, uint32_t frame, const uint32_t (&slots)[5],
               const uint32_t (&bytes)[5]) {
  for (int i = 0; i < 5; i++) {
    Store64(base, frame + slots[i], guest::LoadU8(base, frame + bytes[i]));
  }
}

void Ones(uint8_t* base, uint32_t at, double one) {
  for (uint32_t i = 0; i < 4; i++) {
    Stfs(base, at + 4 * i, one);
  }
}

// A material's +0, +64 and +32 colors from three 16-byte blocks, with the
// original's doubleword loads and stores in their order.
void Apply(uint8_t* base, const Material& m, uint32_t at0, uint32_t at64, uint32_t at32) {
  const uint64_t a0 = Load64(base, at0 + 0);
  const uint64_t a1 = Load64(base, at0 + 8);
  const uint64_t b0 = Load64(base, at64 + 0);
  const uint64_t b1 = Load64(base, at64 + 8);
  const uint64_t c0 = Load64(base, at32 + 0);
  m.set_color_xy(a0);
  m.set_color_zw(a1);
  const uint64_t c1 = Load64(base, at32 + 8);
  m.set_pixel_c63_xy(b0);
  m.set_pixel_c63_zw(b1);
  m.set_pixel_c66_xy(c0);
  m.set_pixel_c66_zw(c1);
}

// A material's +64 (pixel c63) from 16 bytes at `from` (ld, ld, std, std).
void CopyC63(uint8_t* base, const Material& m, uint32_t from) {
  const uint64_t a = Load64(base, from + 0);
  const uint64_t b = Load64(base, from + 8);
  m.set_pixel_c63_xy(a);
  m.set_pixel_c63_zw(b);
}

void StoreArgs(uint8_t* base, uint32_t sp, const uint64_t regs[6]) {
  for (int i = 0; i < 6; i++) {
    Store64(base, sp + 32 + 8 * i, regs[i]);
  }
}

}  // namespace

uint32_t SetPaint(uint8_t* base, uint32_t model, const uint64_t regs[6], uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D0EA8 PartModel::SetPaint");
  const uint32_t frame = sp - 320;
  guest::StoreU32(base, frame, sp);  // the back chain, as stwu
  StoreArgs(base, sp, regs);
  if (model == 0) {
    return model;
  }
  BytePath(base, frame, sp);
  const double one = double(G::one(base));
  ByteSlots(base, frame, {112, 128, 144, 160, 176}, {89, 88, 86, 85, 90});
  ByteColor(base, frame, 80, frame + 208, one);  // A
  ByteColor(base, frame, 84, frame + 224, one);  // B
  ByteColor(base, frame, 88, frame + 192, one);  // C
  HsvToRgb(base, frame + 208, sp + 32, frame);
  HsvToRgb(base, frame + 224, sp + 48, frame);
  HsvToRgb(base, frame + 192, sp + 64, frame);
  uint32_t r3 = frame + 192;
  const int32_t shapes = int32_t(schema::PartModel(base, model).shape_count());
  for (int32_t shape = 0; shape < shapes; shape++) {
    const int32_t first = int32_t(models::MaterialCount(base, model, uint32_t(shape), 0));
    const int32_t second = int32_t(models::MaterialCount(base, model, uint32_t(shape), 1));
    r3 = uint32_t(second);
    if (first > 0) {
      Ones(base, frame + 176, one);
    }
    for (int32_t i = 0; i < first; i++) {
      const Material m(base, models::Material(base, model, uint32_t(shape), uint32_t(i), 0));
      r3 = m.address;
      CopyC63(base, m, frame + 176);
      if (int32_t(m.category()) == 1 || m.shared_pair() == 1000 || m.shared_pair() == 1001) {
        Ones(base, frame + 160, one);
        Apply(base, m, frame + 160, frame + 208, frame + 192);
      }
      if (int32_t(m.category()) == 2 || m.shared_pair() == 1004) {
        Ones(base, frame + 144, one);
        Apply(base, m, frame + 144, frame + 224, frame + 192);
      }
    }
    if (second > 0) {
      Ones(base, frame + 128, one);
    }
    for (int32_t i = 0; i < second; i++) {
      const Material m(base, models::Material(base, model, uint32_t(shape), uint32_t(i), 1));
      r3 = m.address;
      CopyC63(base, m, frame + 128);
      if (int32_t(m.category()) == 1 || m.shared_pair() == 1000 || m.shared_pair() == 1001) {
        Ones(base, frame + 112, one);
        Apply(base, m, frame + 112, frame + 208, frame + 192);
      }
      if (int32_t(m.category()) == 2 || m.shared_pair() == 1004) {
        Ones(base, frame + 240, one);
        Apply(base, m, frame + 240, frame + 224, frame + 192);
      }
    }
  }
  return r3;
}

uint32_t SetPaintBody(uint8_t* base, uint32_t model, const uint64_t regs[6], uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D1378 PartModel::SetPaintBody");
  const uint32_t frame = sp - 288;
  guest::StoreU32(base, frame, sp);  // the back chain, as stwu
  StoreArgs(base, sp, regs);
  if (model == 0) {
    return model;
  }
  BytePath(base, frame, sp);
  const double one = double(G::one(base));
  ByteSlots(base, frame, {104, 112, 144, 128, 160}, {89, 88, 85, 86, 90});
  ByteColor(base, frame, 80, frame + 192, one);  // A
  ByteColor(base, frame, 84, frame + 208, one);  // B
  ByteColor(base, frame, 88, frame + 176, one);  // C
  HsvToRgb(base, frame + 192, sp + 32, frame);
  HsvToRgb(base, frame + 208, sp + 48, frame);
  HsvToRgb(base, frame + 176, sp + 64, frame);
  uint32_t r3 = frame + 176;
  const int32_t shapes = int32_t(schema::PartModel(base, model).shape_count());
  for (int32_t shape = 0; shape < shapes; shape++) {
    const int32_t first = int32_t(models::MaterialCount(base, model, uint32_t(shape), 0));
    const int32_t second = int32_t(models::MaterialCount(base, model, uint32_t(shape), 1));
    r3 = uint32_t(second);
    for (int g = 0; g < 2; g++) {
      const int32_t count = g == 0 ? first : second;
      const uint32_t ones_a = frame + (g == 0 ? 160 : 128);
      const uint32_t ones_b = frame + (g == 0 ? 144 : 112);
      for (int32_t i = 0; i < count; i++) {
        const Material m(base, models::Material(base, model, uint32_t(shape), uint32_t(i), g));
        r3 = m.address;
        if (int32_t(m.category()) == 1 || m.shared_pair() == 1000) {
          Ones(base, ones_a, one);
          Apply(base, m, frame + 192, ones_a, frame + 176);
        }
        if (int32_t(m.category()) == 2) {
          Ones(base, ones_b, one);
          Apply(base, m, frame + 208, ones_b, frame + 176);
        }
      }
    }
  }
  return r3;
}

uint32_t ApplyPalette(uint8_t* base, uint32_t model, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820D2EA8 PartModel::ApplyPalette");
  const uint32_t frame = sp - 224;
  guest::StoreU32(base, frame, sp);  // the back chain, as stwu
  if (model == 0) {
    return model;
  }
  const int32_t shapes = int32_t(schema::PartModel(base, model).shape_count());
  uint32_t r3 = model;
  if (shapes <= 0) {
    return r3;
  }
  const double one = double(G::one(base));
  // The three colors at entry +0 (`a`), +16 (`b`) and `c` into the frame's
  // +80, +96 and +112, then the material's fields from them and entry +48
  // (`e` + 48); `third` whether +16 to +28 are written.
  auto colors = [&](const Material& m, const PaletteEntry& e, uint32_t c, bool third) {
    HsvToRgb(base, frame + 80, e.color_a_address(), frame);
    HsvToRgb(base, frame + 96, e.color_b_address(), frame);
    HsvToRgb(base, frame + 112, c, frame);
    r3 = frame + 112;
    m.set_color_r(float(Lfs(base, frame + 80)));
    m.set_color_b(float(Lfs(base, frame + 88)));
    if (third) {
      m.set_pixel_c66_r(float(Lfs(base, frame + 96)));
      const double g0 = Lfs(base, frame + 84);
      const double b1 = Lfs(base, frame + 104);
      m.set_color_g(float(g0));
      m.set_pixel_c66_b(float(b1));
      const double g1 = Lfs(base, frame + 100);
      const double r2 = Lfs(base, frame + 112);
      m.set_pixel_c66_g(float(g1));
      m.set_pixel_c65_r(float(r2));
      const double b2 = Lfs(base, frame + 120);
      const double g2 = Lfs(base, frame + 116);
      m.set_color_a(float(one));
      m.set_pixel_c66_a(float(one));
      m.set_pixel_c65_g(float(g2));
      m.set_pixel_c65_b(float(b2));
      m.set_pixel_c65_a(float(one));
    } else {
      const double g0 = Lfs(base, frame + 84);
      const double r1 = Lfs(base, frame + 96);
      m.set_color_g(float(g0));
      m.set_pixel_c66_r(float(r1));
      const double b1 = Lfs(base, frame + 104);
      const double g1 = Lfs(base, frame + 100);
      m.set_color_a(float(one));
      m.set_pixel_c66_g(float(g1));
      m.set_pixel_c66_b(float(b1));
      m.set_pixel_c66_a(float(one));
    }
    m.set_pixel_c64_x(e.float_0());
    m.set_pixel_c64_y(e.float_1());
    m.set_pixel_c64_z(e.float_2());
  };
  for (int32_t shape = 0; shape < shapes; shape++) {
    const int32_t first = int32_t(models::MaterialCount(base, model, uint32_t(shape), 0));
    const int32_t second = int32_t(models::MaterialCount(base, model, uint32_t(shape), 1));
    r3 = uint32_t(second);
    for (int g = 0; g < 2; g++) {
      const int32_t count = g == 0 ? first : second;
      for (int32_t i = 0; i < count; i++) {
        const Material m(base, models::Material(base, model, uint32_t(shape), uint32_t(i), g));
        r3 = m.address;
        const int32_t category = int32_t(m.category());
        for (int32_t k = 1; k <= 10; k++) {
          if (k == category) {
            const PaletteEntry e = Palette(base, uint32_t(k));
            colors(m, e, e.color_c_address(), true);
          }
        }
        const uint32_t pair = m.shared_pair();
        for (uint32_t j = 0; j < 5; j++) {
          if (1000 + j == pair) {
            colors(m, Palette(base, 11 + j), Palette(base, j).color_c_address(), g == 1);
          }
        }
      }
    }
  }
  return r3;
}

}  // namespace material_passes
