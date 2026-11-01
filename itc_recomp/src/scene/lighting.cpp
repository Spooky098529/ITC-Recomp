// CarScene::InitLighting (sub_820C8528, see scene.h), load for load and
// store for store as the recompiled function: its float4s are built in its
// 288-byte frame and copied into the block as doublewords, so the frame's
// words are left as the original leaves them. Floats are kept as the
// registers hold them (doubles, rounded to single where the original's
// frsp and single-precision ops round).

#include "scene/scene.h"

#include "base/log.h"
#include "c_runtime/math.h"
#include "guest.h"

#include <bit>

namespace scene {

namespace {

constexpr uint32_t kFrameBytes = 288;

// The image's constants (lis 0x8200 and an offset).
constexpr uint32_t K(uint32_t offset) { return 0x82000000u + offset; }

uint64_t Load64(uint8_t* base, uint32_t address) {
  return (uint64_t(guest::LoadU32(base, address)) << 32) | guest::LoadU32(base, address + 4);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

double LoadFloat(uint8_t* base, uint32_t address) {
  return double(std::bit_cast<float>(guest::LoadU32(base, address)));
}
double LoadDouble(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>(Load64(base, address));
}
double Single(double x) { return double(float(x)); }


}  // namespace

uint64_t InitLighting(uint8_t* base, uint32_t block, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_820C8528 CarScene::InitLighting");
  const uint32_t frame = sp - kFrameBytes;
  guest::StoreU32(base, frame, sp);  // the back chain
  auto S = [&](uint32_t offset, double value) {
    guest::StoreU32(base, frame + offset, std::bit_cast<uint32_t>(float(value)));
  };
  auto ld = [&](uint32_t offset) { return Load64(base, frame + offset); };
  auto st = [&](uint32_t offset, uint64_t value) { Store64(base, block + offset, value); };

  double f0 = LoadFloat(base, K(9844));
  double f12 = LoadFloat(base, K(8696));
  S(128, f12), S(132, f12), S(136, f12);
  f12 = LoadFloat(base, K(10604));
  const double f31 = LoadFloat(base, K(2608));  // 1.0
  S(144, f12), S(148, f12), S(152, f12);
  f12 = LoadFloat(base, K(8712));
  double f13 = LoadFloat(base, K(8692));
  S(160, f12), S(164, f12), S(168, f12);
  f12 = LoadFloat(base, K(8680));
  S(80, f0), S(84, f0), S(88, f0), S(92, f31);
  uint64_t r4 = ld(80);
  f0 = LoadFloat(base, K(10600));
  uint64_t r11 = ld(88);
  S(96, f13), S(100, f13), S(104, f13), S(108, f31);
  const uint64_t r3 = ld(96);
  S(112, f0);
  uint64_t r10 = ld(104);
  S(116, f0);
  st(0, r4);
  S(120, f0);
  st(8, r11);
  f0 = LoadFloat(base, K(2612));  // 0.0
  S(124, f0);
  uint64_t r30 = ld(112);
  r11 = ld(120);
  S(140, f31);
  st(16, r3);
  S(156, f31);
  uint64_t r9 = ld(128);
  st(24, r10);
  uint64_t r8 = ld(136);
  S(172, f0);
  r4 = ld(144);
  S(176, f12), S(180, f12);
  st(32, r30);
  st(40, r11);
  st(48, r9);
  S(184, f12), S(188, f31);
  st(56, r8);
  const uint64_t r29 = ld(160);
  const uint64_t r28 = ld(176);
  uint64_t r7 = ld(152);
  uint64_t r6 = ld(168);
  uint64_t r5 = ld(184);
  st(64, r4), st(72, r7), st(80, r29), st(88, r6);
  S(160, f13), S(164, f13), S(168, f13);
  st(96, r28);
  S(172, f13);
  st(104, r5);
  f13 = LoadFloat(base, K(4208));
  S(144, f13);
  f13 = LoadFloat(base, K(10772));
  S(148, f13);
  f13 = LoadFloat(base, K(10744));
  S(152, f0), S(156, f31);
  r6 = ld(144);
  r10 = ld(152);
  S(180, f13), S(184, f0);
  const double f30 = LoadDouble(base, K(12256));
  r7 = ld(160);
  r11 = ld(168);
  S(188, f0), S(176, f31);
  r5 = ld(176);
  r9 = ld(184);
  S(132, f0), S(136, f0);
  st(112, r7);
  S(140, f0);
  st(120, r11);
  S(128, f31);
  st(128, r6);
  r4 = ld(128);
  st(136, r10);
  r8 = ld(136);
  st(144, r5), st(152, r9), st(160, r4), st(168, r8);

  // The first cone: cos of the angle and of two others, then its block.
  double f28 = Single(c_runtime::Cos(f30));
  double f29 = LoadDouble(base, K(12248));
  const double f29b = Single(c_runtime::Cos(f29));
  double f27 = c_runtime::Cos(f29);
  double f1 = c_runtime::Cos(f30);
  f12 = Single(f28 - f29b);
  S(180, Single(f27));
  S(188, LoadFloat(base, K(12240)));
  S(184, Single(f31 / f12));
  S(176, Single(f1));
  st(176, ld(176)), st(184, ld(184));

  f28 = Single(c_runtime::Cos(f30));
  f29 = LoadDouble(base, K(12232));
  const double f29c = Single(c_runtime::Cos(f29));
  f27 = c_runtime::Cos(f29);
  f1 = c_runtime::Cos(f30);
  f12 = Single(f28 - f29c);
  S(148, Single(f27));
  S(184, f31);
  const double f29d = LoadFloat(base, K(8748));
  S(156, LoadFloat(base, K(12224)));
  S(176, LoadFloat(base, K(11468)));
  S(160, LoadFloat(base, K(9592)));
  S(180, f29d);
  S(144, Single(f1));
  S(188, f31);
  r7 = ld(176);
  f13 = LoadFloat(base, K(10760));
  r10 = ld(184);
  S(164, f13);
  S(152, Single(f31 / f12));
  r8 = ld(144);
  r11 = ld(152);
  S(168, f31), S(172, f31);
  r6 = ld(160);
  r9 = ld(168);
  st(192, r8), st(200, r11), st(208, r7), st(216, r10), st(224, r6), st(232, r9);

  const double f27b = Single(c_runtime::Cos(f30));
  f28 = LoadDouble(base, K(12216));
  const double f28b = Single(c_runtime::Cos(f28));
  const double f26 = c_runtime::Cos(f28);
  f1 = c_runtime::Cos(f30);
  f13 = Single(f27b - f28b);
  S(164, Single(f26));
  S(180, f29d);
  S(184, f31);
  S(172, LoadFloat(base, K(12212)));
  S(176, LoadFloat(base, K(11460)));
  S(188, f31);
  S(168, Single(f31 / f13));
  S(160, Single(f1));
  st(240, ld(160)), st(248, ld(168));
  st(256, ld(176)), st(264, ld(184));
  guest::StoreU32(base, block + 272, std::bit_cast<uint32_t>(float(LoadFloat(base, K(9580)))));
  return r3;
}

}  // namespace scene
