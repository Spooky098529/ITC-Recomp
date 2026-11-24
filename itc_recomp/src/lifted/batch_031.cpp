// Lift sweep batch 031 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/ai_drivers.h"
#include "schema/car_draw.h"
#include "schema/car_physics.h"
#include "schema/career.h"
#include "schema/collision_world.h"
#include "schema/location_screen.h"
#include "schema/menu_helpers.h"
#include "schema/options_screen.h"
#include "schema/race_event.h"
#include "schema/race_hud_machine.h"
#include "schema/scene_models.h"
#include "schema/texture_compression.h"
#include "schema/track_route.h"
#include "schema/tuning_screen.h"
#include "schema/type6_type7_modes.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_031, true, "ITC",
              "Run lift sweep batch 031's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_820B6170: lifted from generated/ by lift.py.
lifted::Result f820B6170(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r8, uint64_t r9, double f1, double f2, double f3) {
  ITC_NATIVE_RUNNING("sub_820B6170 (lifted)");
  uint64_t r4 = 0, r5 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // addi r12,r1,-24
  r12 = uint64_t(int64_t(int64_t(r1) + -24));
  // bl 0x825a7918
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // fmr f29,f1
  lifted::ScalarFloatMode();
  f29 = f1;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // fmr f11,f3
  f11 = f3;
  // lfs f0,48(r31)
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r31)).speed_address());
  f0 = double(temp.f32);
  // lwz r11,272(r31)
  r11 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r31)).car_data_address());
  // fabs f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) & ~0x8000000000000000));
  // stb r10,276(r31)
  guest::StoreU8(base, schema::Vehicle(base, uint32_t(r31)).speed_sensitive_steering_address(), uint8_t(r10));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f10,316(r11)
  temp.u32 = guest::LoadU32(base, schema::CarData(base, uint32_t(r11)).steering_316_address());
  f10 = double(temp.f32);
  // fmuls f10,f10,f2
  f10 = double(float(f10 * f2));
  // lfs f13,10736(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 10736);
  f13 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // fsubs f12,f0,f13
  f12 = double(float(f0 - f13));
  // lfs f0,320(r11)
  temp.u32 = guest::LoadU32(base, schema::CarData(base, uint32_t(r11)).steering_320_address());
  f0 = double(temp.f32);
  // lfs f31,2612(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2612);
  f31 = double(temp.f32);
  // fmuls f8,f0,f2
  f8 = double(float(f0 * f2));
  // fcmpu cr6,f12,f31
  cr6 = lifted::CompareFloat(f12, f31);
  // bge cr6,0x820b61dc
  if (!cr6.lt) goto loc_820B61DC;
  // fmr f12,f31
  f12 = f31;
loc_820B61DC:
  // lfs f13,1048(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r31)).wheels_address() + 632);
  f13 = double(temp.f32);
  // addi r7,r11,336
  r7 = uint64_t(int64_t(int64_t(r11) + 336));
  // lfs f9,680(r31)
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r31)).wheels_address() + 264);
  f9 = double(temp.f32);
  // lis r30,-32256
  r30 = uint64_t(int64_t(-2113929216));
  // fadds f9,f13,f9
  f9 = double(float(f13 + f9));
  // lfs f13,696(r31)
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r31)).wheels_address() + 280);
  f13 = double(temp.f32);
  // lfs f7,1064(r31)
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r31)).wheels_address() + 648);
  f7 = double(temp.f32);
  // addi r3,r31,2988
  r3 = uint64_t(int64_t(int64_t(r31) + 2988));
  // fadds f7,f7,f13
  f7 = double(float(f7 + f13));
  // lfs f0,328(r11)
  temp.u32 = guest::LoadU32(base, schema::CarData(base, uint32_t(r11)).steering_328_address());
  f0 = double(temp.f32);
  // lfs f3,484(r11)
  temp.u32 = guest::LoadU32(base, schema::CarData(base, uint32_t(r11)).front_axle_address() + 148);
  f3 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f2,2608(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 2608);
  f2 = double(temp.f32);
  // lfs f13,7140(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f13 = double(temp.f32);
  // fmuls f9,f9,f13
  f9 = double(float(f9 * f13));
  // fmuls f1,f7,f13
  f1 = double(float(f7 * f13));
  // fmuls f13,f0,f11
  f13 = double(float(f0 * f11));
  // fadds f0,f0,f12
  f0 = double(float(f0 + f12));
  // fdivs f0,f13,f0
  f0 = double(float(f13 / f0));
  // fdivs f0,f2,f0
  f0 = double(float(f2 / f0));
  // fmuls f30,f0,f10
  f30 = double(float(f0 * f10));
  // fmuls f28,f0,f8
  f28 = double(float(f0 * f8));
  // bl 0x820b0668
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B0668, LIFTED_ARGS(0x820B6238, r31)); r3 = o.r3; f1 = o.f1; }
  // fmr f1,f9
  lifted::ScalarFloatMode();
  f1 = f9;
  // bl 0x820b3e98
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820B3E98, LIFTED_ARGS(0x820B6240, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,304(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r31)).steering_address());
  f0 = double(temp.f32);
  // lfs f13,92(r31)
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r31)).slip_term_address());
  f13 = double(temp.f32);
  // fsubs f12,f0,f13
  f12 = double(float(f0 - f13));
  // lfs f13,280(r31)
  temp.u32 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r31)).steering_input_address());
  f13 = double(temp.f32);
  // fsubs f0,f29,f13
  f0 = double(float(f29 - f13));
  // stfs f1,3108(r31)
  temp.f32 = float(f1);
  guest::StoreU32(base, schema::Vehicle(base, uint32_t(r31)).steering_result_address(), temp.u32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // ble cr6,0x820b627c
  if (!cr6.gt) goto loc_820B627C;
  // fcmpu cr6,f12,f31
  cr6 = lifted::CompareFloat(f12, f31);
  // ble cr6,0x820b626c
  if (!cr6.gt) goto loc_820B626C;
  // fmr f30,f28
  f30 = f28;
loc_820B626C:
  // fcmpu cr6,f0,f30
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f30);
  // ble cr6,0x820b6298
  if (!cr6.gt) goto loc_820B6298;
  // fmr f0,f30
  f0 = f30;
  // b 0x820b6298
  goto loc_820B6298;
loc_820B627C:
  // fcmpu cr6,f12,f31
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f12, f31);
  // bge cr6,0x820b6288
  if (!cr6.lt) goto loc_820B6288;
  // fmr f30,f28
  f30 = f28;
loc_820B6288:
  // fneg f12,f30
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f30) ^ 0x8000000000000000));
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bge cr6,0x820b6298
  if (!cr6.lt) goto loc_820B6298;
  // fmr f0,f12
  f0 = f12;
loc_820B6298:
  // fadds f0,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f13 + f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f0,280(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Vehicle(base, uint32_t(r31)).steering_input_address(), temp.u32);
  // lfs f13,7136(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7136);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x820b62bc
  if (cr6.lt) goto loc_820B62BC;
  // lfs f13,2608(r30)
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 2608);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x820b62c0
  if (!cr6.gt) goto loc_820B62C0;
loc_820B62BC:
  // fmr f0,f13
  lifted::ScalarFloatMode();
  f0 = f13;
loc_820B62C0:
  // stfs f0,280(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Vehicle(base, uint32_t(r31)).steering_input_address(), temp.u32);
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r12,r1,-24
  r12 = uint64_t(int64_t(int64_t(r1) + -24));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_820F6C88: lifted from generated/ by lift.py.
uint64_t f820F6C88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820F6C88 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r27,r5
  r27 = r5;
  // mr r28,r4
  r28 = r4;
  // li r26,-1
  r26 = uint64_t(int64_t(-1));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r10,72(r31)
  r10 = guest::LoadU32(base, schema::TrackMeshSection(base, uint32_t(r31)).slices_address());
  // lhz r11,2(r31)
  r11 = guest::LoadU16(base, schema::TrackMeshSection(base, uint32_t(r31)).outline_address());
  // lfs f3,0(r27)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TrackNearestDistance(base, uint32_t(r27)).distance_address());
  f3 = double(temp.f32);
  // add r30,r10,r31
  r30 = r10 + r31;
  // add r4,r11,r31
  r4 = r11 + r31;
  // lha r9,0(r31)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::TrackMeshSection(base, uint32_t(r31)).slice_count_address()))));
  // cmpwi r9,0
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // ble 0x820f6e08
  if (!cr0.gt) goto loc_820F6E08;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f6,26652(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 26652);
  f6 = double(temp.f32);
  // lfs f7,2608(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f7 = double(temp.f32);
  // lfs f4,2612(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2612);
  f4 = double(temp.f32);
  // lfs f5,7132(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7132);
  f5 = double(temp.f32);
loc_820F6CE8:
  // lfs f0,20(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 20);
  f0 = double(temp.f32);
  // fadds f12,f0,f5
  f12 = double(float(f0 + f5));
  // lfs f13,4(r28)
  temp.u32 = guest::LoadU32(base, schema::TrackQueryPoint(base, uint32_t(r28)).y_address());
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // bgt cr6,0x820f6d0c
  if (cr6.gt) goto loc_820F6D0C;
  // fsubs f0,f0,f5
  f0 = double(float(f0 - f5));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x820f6d10
  if (!cr6.lt) goto loc_820F6D10;
loc_820F6D0C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_820F6D10:
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820f6de8
  if (cr0.eq) goto loc_820F6DE8;
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // lhz r11,18(r30)
  r11 = guest::LoadU16(base, uint32_t(r30) + 18);
  // fmr f12,f4
  lifted::ScalarFloatMode();
  f12 = f4;
  // ori r10,r10,65535
  r10 = r10 | 65535;
  // fmr f11,f4
  f11 = f4;
  // subf r11,r11,r10
  r11 = r10 - r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // addic. r8,r11,3
  ca = uint32_t(r11) > 4294967292;
  r8 = uint64_t(int64_t(int64_t(r11) + 3));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // ble 0x820f6d74
  if (!cr0.gt) goto loc_820F6D74;
  // addi r10,r30,12
  r10 = uint64_t(int64_t(int64_t(r30) + 12));
  // mr r11,r8
  r11 = r8;
loc_820F6D4C:
  // lhz r9,0(r10)
  r9 = guest::LoadU16(base, uint32_t(r10) + 0);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // rotlwi r9,r9,3
  r9 = __builtin_rotateleft32(uint32_t(r9), 3);
  // add r9,r9,r4
  r9 = r9 + r4;
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // lfs f13,4(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 4);
  f13 = double(temp.f32);
  // fadds f12,f0,f12
  f12 = double(float(f0 + f12));
  // fadds f11,f13,f11
  f11 = double(float(f13 + f11));
  // bne 0x820f6d4c
  if (!cr0.eq) goto loc_820F6D4C;
loc_820F6D74:
  // extsw r11,r8
  r11 = uint64_t(int64_t(int32_t(r8)));
  // lfs f0,0(r28)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::TrackQueryPoint(base, uint32_t(r28)).x_address());
  f0 = double(temp.f32);
  // lfs f10,8(r28)
  temp.u32 = guest::LoadU32(base, schema::TrackQueryPoint(base, uint32_t(r28)).z_address());
  f10 = double(temp.f32);
  // fmr f9,f10
  f9 = f10;
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fdivs f13,f7,f13
  f13 = double(float(f7 / f13));
  // fmuls f12,f13,f12
  f12 = double(float(f13 * f12));
  // fmuls f13,f13,f11
  f13 = double(float(f13 * f11));
  // fsubs f11,f12,f0
  f11 = double(float(f12 - f0));
  // fsubs f0,f12,f0
  f0 = double(float(f12 - f0));
  // fsubs f12,f13,f10
  f12 = double(float(f13 - f10));
  // fsubs f13,f13,f9
  f13 = double(float(f13 - f9));
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // fmadds f0,f12,f13,f0
  f0 = double(float(std::fma(f12, f13, f0)));
  // fcmpu cr6,f0,f6
  cr6 = lifted::CompareFloat(f0, f6);
  // bgt cr6,0x820f6de8
  if (cr6.gt) goto loc_820F6DE8;
  // fcmpu cr6,f3,f0
  cr6 = lifted::CompareFloat(f3, f0);
  // ble cr6,0x820f6dd4
  if (!cr6.gt) goto loc_820F6DD4;
  // stfs f0,0(r27)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TrackNearestDistance(base, uint32_t(r27)).distance_address(), temp.u32);
  // fmr f3,f0
  f3 = f0;
  // mr r26,r29
  r26 = r29;
loc_820F6DD4:
  // mr r5,r28
  r5 = r28;
  // mr r3,r30
  r3 = r30;
  // bl 0x820f6a38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F6A38, LIFTED_ARGS(0x820F6DE0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x820f6e00
  if (!cr0.eq) goto loc_820F6E00;
loc_820F6DE8:
  // lha r11,0(r31)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::TrackMeshSection(base, uint32_t(r31)).slice_count_address()))));
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r30,r30,28
  r30 = uint64_t(int64_t(int64_t(r30) + 28));
  // cmpw cr6,r29,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r11), so);
  // blt cr6,0x820f6ce8
  if (cr6.lt) goto loc_820F6CE8;
  // b 0x820f6e08
  goto loc_820F6E08;
loc_820F6E00:
  // stfs f4,0(r27)
  lifted::ScalarFloatMode();
  temp.f32 = float(f4);
  guest::StoreU32(base, schema::TrackNearestDistance(base, uint32_t(r27)).distance_address(), temp.u32);
  // mr r26,r29
  r26 = r29;
loc_820F6E08:
  // mr r3,r26
  r3 = r26;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_820F7E30: lifted from generated/ by lift.py.
uint64_t f820F7E30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820F7E30 (lifted)");
  uint64_t r9 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r5
  r30 = r5;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r26,r3
  r26 = r3;
  // mr r28,r4
  r28 = r4;
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // lwz r9,4(r30)
  r9 = guest::LoadU32(base, schema::TrackPosition(base, uint32_t(r30)).route_position_address());
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lfs f0,26660(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 26660);
  f0 = double(temp.f32);
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // lwz r11,0(r9)
  r11 = guest::LoadU32(base, schema::TrackMeshFile(base, uint32_t(r9)).section_count_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x820f7edc
  if (!cr6.gt) goto loc_820F7EDC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // lfs f2,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f2 = double(temp.f32);
loc_820F7E78:
  // extsh r29,r31
  r29 = uint64_t(int64_t(int16_t(r31)));
  // mr r4,r28
  r4 = r28;
  // mr r3,r30
  r3 = r30;
  // mr r5,r29
  r5 = r29;
  // bl 0x820f7d70
  // (left out: dead in C++)
  r3 = f820F7D70(base, r3, r4, r5);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820f7ec4
  if (!cr0.eq) goto loc_820F7EC4;
  // lwz r11,4(r9)
  r11 = guest::LoadU32(base, uint32_t(r9) + 4);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r3,r11,r27
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r27));
  // bl 0x820f6c88
  // (left out: dead in C++)
  r3 = f820F6C88(base, r1, r13, r3, r4, r5);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x820f7ec4
  if (cr6.eq) goto loc_820F7EC4;
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // fcmpu cr6,f0,f2
  cr6 = lifted::CompareFloat(f0, f2);
  // sth r3,2(r26)
  guest::StoreU16(base, schema::TrackPosition(base, uint32_t(r26)).slice_address(), uint16_t(r3));
  // sth r29,0(r26)
  guest::StoreU16(base, schema::TrackPosition(base, uint32_t(r26)).section_address(), uint16_t(r29));
  // beq cr6,0x820f7edc
  if (cr6.eq) goto loc_820F7EDC;
loc_820F7EC4:
  // lwz r9,4(r30)
  r9 = guest::LoadU32(base, schema::TrackPosition(base, uint32_t(r30)).route_position_address());
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r27,r27,4
  r27 = uint64_t(int64_t(int64_t(r27) + 4));
  // lwz r11,0(r9)
  r11 = guest::LoadU32(base, schema::TrackMeshFile(base, uint32_t(r9)).section_count_address());
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x820f7e78
  if (cr6.lt) goto loc_820F7E78;
loc_820F7EDC:
  // mr r3,r25
  r3 = r25;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return r3;
}

// sub_820FB2B0: lifted from generated/ by lift.py.
uint64_t f820FB2B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_820FB2B0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // extsh r26,r6
  r26 = uint64_t(int64_t(int16_t(r6)));
  // mr r31,r3
  r31 = r3;
  // mr r22,r4
  r22 = r4;
  // lfs f8,26660(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 26660);
  f8 = double(temp.f32);
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stfs f8,88(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + 88, temp.u32);
  // mr r25,r5
  r25 = r5;
  // mr r23,r7
  r23 = r7;
  // sth r24,86(r1)
  guest::StoreU16(base, uint32_t(r1) + 86, uint16_t(r24));
  // cmpwi cr6,r26,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r26), -1, so);
  // sth r11,84(r1)
  guest::StoreU16(base, uint32_t(r1) + 84, uint16_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f7,26652(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 26652);
  f7 = double(temp.f32);
  // beq cr6,0x820fb3d8
  if (cr6.eq) goto loc_820FB3D8;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::TrackRoute(base, uint32_t(r31)).file_address());
  // rlwinm r10,r6,2,14,29
  r10 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0x3FFFC;
  // li r27,12
  r27 = uint64_t(int64_t(12));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::TrackRouteFile(base, uint32_t(r11)).sections_address());
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // addi r28,r11,28
  r28 = uint64_t(int64_t(int64_t(r11) + 28));
loc_820FB314:
  // lhz r9,0(r28)
  r9 = guest::LoadU16(base, uint32_t(r28) + 0);
  // extsh r11,r9
  r11 = uint64_t(int64_t(int16_t(r9)));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // sth r9,80(r1)
  guest::StoreU16(base, uint32_t(r1) + 80, uint16_t(r9));
  // beq cr6,0x820fb3b8
  if (cr6.eq) goto loc_820FB3B8;
  // mr r5,r9
  r5 = r9;
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // bl 0x820f8ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8EF0, LIFTED_ARGS(0x820FB338, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820fb3b8
  if (!cr0.eq) goto loc_820FB3B8;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::TrackRoute(base, uint32_t(r31)).file_address());
  // rlwinm r10,r9,2,14,29
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x3FFFC;
  // sth r24,82(r1)
  guest::StoreU16(base, uint32_t(r1) + 82, uint16_t(r24));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::TrackRouteFile(base, uint32_t(r11)).sections_address());
  // lwzx r29,r10,r11
  r29 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lha r11,0(r29)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 0))));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820fb3b8
  if (!cr0.gt) goto loc_820FB3B8;
  // mr r30,r24
  r30 = r24;
loc_820FB364:
  // addi r6,r1,88
  r6 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // bl 0x820fb1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::TrackRoute::OnSlice(base, LIFTED_ARGS(0x820FB378, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x820fb398
  if (!cr6.eq) goto loc_820FB398;
  // lfs f0,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f8
  cr6 = lifted::CompareFloat(f0, f8);
  // bge cr6,0x820fb398
  if (!cr6.lt) goto loc_820FB398;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // fmr f8,f0
  f8 = f0;
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_820FB398:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // lha r10,0(r29)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 0))));
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // mr r30,r11
  r30 = r11;
  // sth r11,82(r1)
  guest::StoreU16(base, uint32_t(r1) + 82, uint16_t(r11));
  // mr r11,r10
  r11 = r10;
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x820fb364
  if (cr6.lt) goto loc_820FB364;
loc_820FB3B8:
  // addic. r27,r27,-1
  ca = uint32_t(r27) > 0;
  r27 = uint64_t(int64_t(int64_t(r27) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // bne 0x820fb314
  if (!cr0.eq) goto loc_820FB314;
  // fcmpu cr6,f8,f7
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f8, f7);
  // bge cr6,0x820fb3d8
  if (!cr6.lt) goto loc_820FB3D8;
  // lhz r11,84(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 84);
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // bne cr6,0x820fb670
  if (!cr6.eq) goto loc_820FB670;
loc_820FB3D8:
  // extsh r27,r23
  r27 = uint64_t(int64_t(int16_t(r23)));
  // cmpwi cr6,r27,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r27), -1, so);
  // beq cr6,0x820fb548
  if (cr6.eq) goto loc_820FB548;
  // cmpw cr6,r27,r26
  cr6 = lifted::Compare<int32_t>(int32_t(r27), int32_t(r26), so);
  // beq cr6,0x820fb548
  if (cr6.eq) goto loc_820FB548;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::TrackRoute(base, uint32_t(r31)).file_address());
  // rlwinm r10,r23,2,14,29
  r10 = __builtin_rotateleft64(uint32_t(r23) | (r23 << 32), 2) & 0x3FFFC;
  // mr r5,r23
  r5 = r23;
  // sth r23,80(r1)
  guest::StoreU16(base, uint32_t(r1) + 80, uint16_t(r23));
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::TrackRouteFile(base, uint32_t(r11)).sections_address());
  // lwzx r29,r10,r11
  r29 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // bl 0x820f8ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8EF0, LIFTED_ARGS(0x820FB410, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820fb480
  if (!cr0.eq) goto loc_820FB480;
  // lha r11,0(r29)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 0))));
  // sth r24,82(r1)
  guest::StoreU16(base, uint32_t(r1) + 82, uint16_t(r24));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820fb480
  if (!cr0.gt) goto loc_820FB480;
  // mr r30,r24
  r30 = r24;
loc_820FB42C:
  // addi r6,r1,88
  r6 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // bl 0x820fb1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::TrackRoute::OnSlice(base, LIFTED_ARGS(0x820FB440, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x820fb460
  if (!cr6.eq) goto loc_820FB460;
  // lfs f0,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f8
  cr6 = lifted::CompareFloat(f0, f8);
  // bge cr6,0x820fb460
  if (!cr6.lt) goto loc_820FB460;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // fmr f8,f0
  f8 = f0;
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_820FB460:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // lha r10,0(r29)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 0))));
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // mr r30,r11
  r30 = r11;
  // sth r11,82(r1)
  guest::StoreU16(base, uint32_t(r1) + 82, uint16_t(r11));
  // mr r11,r10
  r11 = r10;
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x820fb42c
  if (cr6.lt) goto loc_820FB42C;
loc_820FB480:
  // li r28,28
  r28 = uint64_t(int64_t(28));
loc_820FB484:
  // lhzx r9,r28,r29
  r9 = guest::LoadU16(base, uint32_t(r28) + uint32_t(r29));
  // extsh r11,r9
  r11 = uint64_t(int64_t(int16_t(r9)));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // sth r9,80(r1)
  guest::StoreU16(base, uint32_t(r1) + 80, uint16_t(r9));
  // beq cr6,0x820fb528
  if (cr6.eq) goto loc_820FB528;
  // mr r5,r9
  r5 = r9;
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // bl 0x820f8ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8EF0, LIFTED_ARGS(0x820FB4A8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820fb528
  if (!cr0.eq) goto loc_820FB528;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::TrackRoute(base, uint32_t(r31)).file_address());
  // rlwinm r10,r9,2,14,29
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x3FFFC;
  // sth r24,82(r1)
  guest::StoreU16(base, uint32_t(r1) + 82, uint16_t(r24));
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::TrackRouteFile(base, uint32_t(r11)).sections_address());
  // lwzx r29,r10,r11
  r29 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lha r11,0(r29)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 0))));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820fb528
  if (!cr0.gt) goto loc_820FB528;
  // mr r30,r24
  r30 = r24;
loc_820FB4D4:
  // addi r6,r1,88
  r6 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // bl 0x820fb1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::TrackRoute::OnSlice(base, LIFTED_ARGS(0x820FB4E8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x820fb508
  if (!cr6.eq) goto loc_820FB508;
  // lfs f0,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f8
  cr6 = lifted::CompareFloat(f0, f8);
  // bge cr6,0x820fb508
  if (!cr6.lt) goto loc_820FB508;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // fmr f8,f0
  f8 = f0;
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_820FB508:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // lha r10,0(r29)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 0))));
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // mr r30,r11
  r30 = r11;
  // sth r11,82(r1)
  guest::StoreU16(base, uint32_t(r1) + 82, uint16_t(r11));
  // mr r11,r10
  r11 = r10;
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x820fb4d4
  if (cr6.lt) goto loc_820FB4D4;
loc_820FB528:
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // cmpwi cr6,r28,76
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 76, so);
  // blt cr6,0x820fb484
  if (cr6.lt) goto loc_820FB484;
  // fcmpu cr6,f8,f7
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f8, f7);
  // bge cr6,0x820fb548
  if (!cr6.lt) goto loc_820FB548;
  // lhz r11,84(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 84);
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // bne cr6,0x820fb670
  if (!cr6.eq) goto loc_820FB670;
loc_820FB548:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::TrackRoute(base, uint32_t(r31)).file_address());
  // mr r5,r24
  r5 = r24;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::TrackRouteFile(base, uint32_t(r11)).section_count_address());
  // sth r5,80(r1)
  guest::StoreU16(base, uint32_t(r1) + 80, uint16_t(r5));
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble 0x820fb618
  if (!cr0.gt) goto loc_820FB618;
  // mr r28,r24
  r28 = r24;
loc_820FB564:
  // lwz r11,12(r11)
  r11 = guest::LoadU32(base, schema::TrackRouteFile(base, uint32_t(r11)).sections_address());
  // rlwinm r10,r5,2,14,29
  r10 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0x3FFFC;
  // lwzx r29,r10,r11
  r29 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lbz r11,2(r29)
  r11 = guest::LoadU8(base, uint32_t(r29) + 2);
  // cmplwi cr6,r11,48
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 48, so);
  // blt cr6,0x820fb5f8
  if (cr6.lt) goto loc_820FB5F8;
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // bl 0x820f8ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820F8EF0, LIFTED_ARGS(0x820FB588, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820fb5f8
  if (!cr0.eq) goto loc_820FB5F8;
  // lha r11,0(r29)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 0))));
  // sth r24,82(r1)
  guest::StoreU16(base, uint32_t(r1) + 82, uint16_t(r24));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820fb5f8
  if (!cr0.gt) goto loc_820FB5F8;
  // mr r30,r24
  r30 = r24;
loc_820FB5A4:
  // addi r6,r1,88
  r6 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r25
  r4 = r25;
  // mr r3,r31
  r3 = r31;
  // bl 0x820fb1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::TrackRoute::OnSlice(base, LIFTED_ARGS(0x820FB5B8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x820fb5d8
  if (!cr6.eq) goto loc_820FB5D8;
  // lfs f0,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f8
  cr6 = lifted::CompareFloat(f0, f8);
  // bge cr6,0x820fb5d8
  if (!cr6.lt) goto loc_820FB5D8;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // fmr f8,f0
  f8 = f0;
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_820FB5D8:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // lha r10,0(r29)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r29) + 0))));
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // mr r30,r11
  r30 = r11;
  // sth r11,82(r1)
  guest::StoreU16(base, uint32_t(r1) + 82, uint16_t(r11));
  // mr r11,r10
  r11 = r10;
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x820fb5a4
  if (cr6.lt) goto loc_820FB5A4;
loc_820FB5F8:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::TrackRoute(base, uint32_t(r31)).file_address());
  // addi r10,r28,1
  r10 = uint64_t(int64_t(int64_t(r28) + 1));
  // extsh r5,r10
  r5 = uint64_t(int64_t(int16_t(r10)));
  // mr r28,r5
  r28 = r5;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::TrackRouteFile(base, uint32_t(r11)).section_count_address());
  // sth r5,80(r1)
  guest::StoreU16(base, uint32_t(r1) + 80, uint16_t(r5));
  // cmpw cr6,r28,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r10), so);
  // blt cr6,0x820fb564
  if (cr6.lt) goto loc_820FB564;
loc_820FB618:
  // lhz r11,84(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 84);
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // bne cr6,0x820fb670
  if (!cr6.eq) goto loc_820FB670;
  // lhz r11,0(r22)
  r11 = guest::LoadU16(base, schema::TrackRoutePosition(base, uint32_t(r22)).section_address());
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x820fb648
  if (cr6.eq) goto loc_820FB648;
  // lhz r11,2(r22)
  r11 = guest::LoadU16(base, schema::TrackRoutePosition(base, uint32_t(r22)).slice_address());
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x820fb648
  if (cr6.eq) goto loc_820FB648;
  // lwz r11,0(r22)
  r11 = guest::LoadU32(base, schema::TrackRoutePosition(base, uint32_t(r22)).packed_address());
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // b 0x820fb670
  goto loc_820FB670;
loc_820FB648:
  // cmpwi cr6,r27,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r27), -1, so);
  // beq cr6,0x820fb668
  if (cr6.eq) goto loc_820FB668;
  // lhz r11,2(r22)
  r11 = guest::LoadU16(base, schema::TrackRoutePosition(base, uint32_t(r22)).slice_address());
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x820fb668
  if (cr6.eq) goto loc_820FB668;
  // sth r23,84(r1)
  guest::StoreU16(base, uint32_t(r1) + 84, uint16_t(r23));
  // sth r11,86(r1)
  guest::StoreU16(base, uint32_t(r1) + 86, uint16_t(r11));
  // b 0x820fb670
  goto loc_820FB670;
loc_820FB668:
  // sth r24,84(r1)
  guest::StoreU16(base, uint32_t(r1) + 84, uint16_t(r24));
  // sth r24,86(r1)
  guest::StoreU16(base, uint32_t(r1) + 86, uint16_t(r24));
loc_820FB670:
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // stw r11,0(r22)
  guest::StoreU32(base, schema::TrackRoutePosition(base, uint32_t(r22)).packed_address(), uint32_t(r11));
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return r3;
}

// sub_8210B958: lifted from generated/ by lift.py.
lifted::Result f8210B958(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8210B958 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r10,r3
  r10 = r3;
  // bl 0x8210b140
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8210B140, LIFTED_ARGS(0x8210B96C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,30
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 30, so);
  // beq cr6,0x8210b9b0
  if (cr6.eq) goto loc_8210B9B0;
  // addi r10,r10,448
  r10 = uint64_t(int64_t(int64_t(r10) + 448));
  // mr r11,r5
  r11 = r5;
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // bne cr6,0x8210b990
  if (!cr6.eq) goto loc_8210B990;
  // addi r11,r3,9
  r11 = uint64_t(int64_t(int64_t(r3) + 9));
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r11,r11,r10
  r11 = r11 + r10;
loc_8210B990:
  // addi r9,r3,6
  r9 = uint64_t(int64_t(int64_t(r3) + 6));
  // mr r3,r11
  r3 = r11;
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stwx r11,r9,r10
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r10), uint32_t(r11));
  // lwz r11,0(r11)
  r11 = lifted::VtableWord(base, uint32_t(r11));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8210B9B0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8210B9B0:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8210B9C0: lifted from generated/ by lift.py.
lifted::Result f8210B9C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8210B9C0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r10,r3
  r10 = r3;
  // bl 0x8210b140
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8210B140, LIFTED_ARGS(0x8210B9D4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,30
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 30, so);
  // beq cr6,0x8210ba18
  if (cr6.eq) goto loc_8210BA18;
  // mr r11,r5
  r11 = r5;
  // addi r10,r10,1072
  r10 = uint64_t(int64_t(int64_t(r10) + 1072));
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // bne cr6,0x8210b9f8
  if (!cr6.eq) goto loc_8210B9F8;
  // rlwinm r11,r3,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 4) & 0xFFFFFFF0;
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r11,r11,300
  r11 = uint64_t(int64_t(int64_t(r11) + 300));
loc_8210B9F8:
  // addi r9,r3,45
  r9 = uint64_t(int64_t(int64_t(r3) + 45));
  // mr r3,r11
  r3 = r11;
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // stwx r11,r9,r10
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r10), uint32_t(r11));
  // lwz r11,0(r11)
  r11 = lifted::VtableWord(base, uint32_t(r11));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8210BA18, r31)); r3 = o.r3; f1 = o.f1; }
loc_8210BA18:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82138FA8: lifted from generated/ by lift.py.
uint64_t f82138FA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82138FA8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lha r11,5648(r3)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r3)).path_entry_count_address()))));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // ble cr6,0x821390b8
  if (!cr6.gt) goto loc_821390B8;
  // addi r11,r3,512
  r11 = uint64_t(int64_t(int64_t(r3) + 512));
  // lha r10,5136(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r11) - 512).path_entry_count_address()))));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // ble cr6,0x82138ffc
  if (!cr6.gt) goto loc_82138FFC;
  // lha r10,5138(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r11) - 512).path_first_address()))));
  // li r8,80
  r8 = uint64_t(int64_t(80));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // divw r8,r10,r8
  r8 = uint32_t((int32_t(r8) && !(int32_t(r10) == INT32_MIN && int32_t(r8) == -1)) ? int32_t(r10) / int32_t(r8) : 0);
  // mulli r8,r8,80
  r8 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(80))));
  // subf r10,r8,r10
  r10 = r10 - r8;
  // rlwinm r10,r10,6,0,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 6) & 0xFFFFFFC0;
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // b 0x82139000
  goto loc_82139000;
loc_82138FFC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82139000:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x821390b8
  if (cr6.eq) goto loc_821390B8;
  // lwz r10,52(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 52);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x821390b8
  if (cr6.eq) goto loc_821390B8;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f13,16(r4)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r4)).direction_address());
  f13 = double(temp.f32);
  // lfs f0,2612(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // blt cr6,0x82139034
  if (cr6.lt) goto loc_82139034;
  // lfs f13,44(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 44);
  f13 = double(temp.f32);
  // lfs f12,144(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).road_offset_address());
  f12 = double(temp.f32);
  // b 0x8213903c
  goto loc_8213903C;
loc_82139034:
  // lfs f13,144(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).road_offset_address());
  f13 = double(temp.f32);
  // lfs f12,40(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 40);
  f12 = double(temp.f32);
loc_8213903C:
  // fsubs f13,f13,f12
  lifted::ScalarFloatMode();
  f13 = double(float(f13 - f12));
  // stfs f13,0(r5)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r5) + 0, temp.u32);
  // lfs f12,116(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).half_width_address());
  f12 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // ble cr6,0x821390b8
  if (!cr6.gt) goto loc_821390B8;
  // lfs f13,6116(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).target_angle_address());
  f13 = double(temp.f32);
  // lfs f12,16(r4)
  temp.u32 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r4)).direction_address());
  f12 = double(temp.f32);
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // fcmpu cr6,f12,f0
  cr6 = lifted::CompareFloat(f12, f0);
  // ble cr6,0x821390b4
  if (!cr6.gt) goto loc_821390B4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,6136(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).steer_request_address());
  f12 = double(temp.f32);
  // lfs f0,7140(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // lfs f13,6112(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).slide_angle_address());
  f13 = double(temp.f32);
  // fadds f13,f13,f12
  f13 = double(float(f13 + f12));
  // fadds f13,f13,f0
  f13 = double(float(f13 + f0));
  // stfs f13,6136(r3)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::AiDriver(base, uint32_t(r3)).steer_request_address(), temp.u32);
  // lfs f12,128(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).car_radius_address());
  f12 = double(temp.f32);
  // lfs f11,0(r5)
  temp.u32 = guest::LoadU32(base, uint32_t(r5) + 0);
  f11 = double(temp.f32);
  // fcmpu cr6,f12,f11
  cr6 = lifted::CompareFloat(f12, f11);
  // ble cr6,0x821390ac
  if (!cr6.gt) goto loc_821390AC;
  // fdivs f11,f11,f12
  f11 = double(float(f11 / f12));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f12 = double(temp.f32);
  // fsubs f12,f12,f11
  f12 = double(float(f12 - f11));
  // fmadds f0,f12,f0,f13
  f0 = double(float(std::fma(f12, f0, f13)));
  // stfs f0,6136(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AiDriver(base, uint32_t(r3)).steer_request_address(), temp.u32);
loc_821390AC:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x821390c0
  goto loc_821390C0;
loc_821390B4:
  // li r9,1
  r9 = uint64_t(int64_t(1));
loc_821390B8:
  // bl 0x82138f00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82138F00, LIFTED_ARGS(0x821390BC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r9
  r3 = r9;
loc_821390C0:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82139A08: lifted from generated/ by lift.py.
uint64_t f82139A08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82139A08 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,16(r4)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r4)).direction_address());
  f0 = double(temp.f32);
  // mr r7,r3
  r7 = r3;
  // fneg f7,f0
  f7 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // lfs f8,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f8 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,100(r7)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r7)).speed_kmh_address());
  f13 = double(temp.f32);
  // stfs f8,80(r1)
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // lfs f0,9576(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9576);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x82139ab4
  if (!cr6.gt) goto loc_82139AB4;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82138fa8
  // (left out: dead in C++)
  r3 = f82138FA8(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x82139ab8
  if (cr0.eq) goto loc_82139AB8;
  // lfs f0,6116(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 6116);
  f0 = double(temp.f32);
  // fmuls f0,f0,f7
  f0 = double(float(f0 * f7));
  // lfs f13,6088(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 6088);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f8
  cr6 = lifted::CompareFloat(f0, f8);
  // lfs f0,100(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 100);
  f0 = double(temp.f32);
  // blt cr6,0x82139a84
  if (cr6.lt) goto loc_82139A84;
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x82139ab8
  if (!cr6.gt) goto loc_82139AB8;
  // lfs f0,6112(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 6112);
  f0 = double(temp.f32);
  // fmuls f0,f0,f7
  f0 = double(float(f0 * f7));
  // fcmpu cr6,f0,f8
  cr6 = lifted::CompareFloat(f0, f8);
  // blt cr6,0x82139ab8
  if (cr6.lt) goto loc_82139AB8;
  // b 0x82139aac
  goto loc_82139AAC;
loc_82139A84:
  // fcmpu cr6,f0,f13
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x82139aac
  if (cr6.gt) goto loc_82139AAC;
  // lfs f0,144(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 144);
  f0 = double(temp.f32);
  // fmuls f0,f0,f7
  f0 = double(float(f0 * f7));
  // fcmpu cr6,f0,f8
  cr6 = lifted::CompareFloat(f0, f8);
  // bge cr6,0x82139ab8
  if (!cr6.lt) goto loc_82139AB8;
  // lfs f0,104(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 104);
  f0 = double(temp.f32);
  // fmuls f0,f0,f7
  f0 = double(float(f0 * f7));
  // fcmpu cr6,f0,f8
  cr6 = lifted::CompareFloat(f0, f8);
  // bge cr6,0x82139ab8
  if (!cr6.lt) goto loc_82139AB8;
loc_82139AAC:
  // stfs f8,6140(r7)
  lifted::ScalarFloatMode();
  temp.f32 = float(f8);
  guest::StoreU32(base, uint32_t(r7) + 6140, temp.u32);
  // b 0x82139ab8
  goto loc_82139AB8;
loc_82139AB4:
  // bl 0x82138f00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82138F00, LIFTED_ARGS(0x82139AB8, r31)); r3 = o.r3; f1 = o.f1; }
loc_82139AB8:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_821C2388 game::Options::AnalogPage2P: lifted from generated/ by lift.py.
lifted::Result f821C2388(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821C2388 (lifted)");
  uint64_t r3 = 0, r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31628(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -31628);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x821C23A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31628(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -31628);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821c23e4
  if (cr6.eq) goto loc_821C23E4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x821c23cc
  if (cr6.eq) goto loc_821C23CC;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_821C23CC:
  // li r9,38
  r9 = uint64_t(int64_t(38));
loc_821C23D0:
  // li r3,4
  r3 = uint64_t(int64_t(4));
loc_821C23D4:
  // li r8,8
  r8 = uint64_t(int64_t(8));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r8,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r8));
  // b 0x821c25fc
  goto loc_821C25FC;
loc_821C23E4:
  // bl 0x821babd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BABD8, LIFTED_ARGS(0x821C23E8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c2410
  if (cr0.eq) goto loc_821C2410;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x821c2408
  if (cr6.eq) goto loc_821C2408;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_821C2408:
  // li r9,39
  r9 = uint64_t(int64_t(39));
  // b 0x821c23d0
  goto loc_821C23D0;
loc_821C2410:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31628(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -31628);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x821C241C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c2554
  if (cr0.eq) goto loc_821C2554;
  // lwz r11,-31628(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -31628);
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r5,-31624(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + -31624);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // bne cr6,0x821c2514
  if (!cr6.eq) goto loc_821C2514;
  // lwz r9,660(r5)
  r9 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r5)).ok_default_address());
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne cr6,0x821c24e0
  if (!cr6.eq) goto loc_821C24E0;
  // bl 0x821bac78
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BAC78, LIFTED_ARGS(0x821C244C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821c24d8
  if (!cr0.eq) goto loc_821C24D8;
  // bl 0x821bb678
  // (left out: dead in C++)
  r3 = game::ControllerOptions::HasAnalogClash(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821c24d8
  if (!cr0.eq) goto loc_821C24D8;
  // addi r4,r5,380
  r4 = uint64_t(int64_t(int64_t(r5) + 380));
  // addi r3,r5,188
  r3 = uint64_t(int64_t(int64_t(r5) + 188));
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x821C2470, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -31624);
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // addi r4,r11,404
  r4 = uint64_t(int64_t(int64_t(r11) + 404));
  // addi r3,r11,212
  r3 = uint64_t(int64_t(int64_t(r11) + 212));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x821C2484, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821c249c
  if (cr6.eq) goto loc_821C249C;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_821C249C:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r8,40
  r8 = uint64_t(int64_t(40));
  // stw r9,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r9));
  // stw r11,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r11));
  // stw r8,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r8));
  // stw r11,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r11));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31604(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -31604);
  // lwz r9,0(r10)
  r9 = schema::PageObject(base, uint32_t(r10)).page_bits();
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // beq cr6,0x821c24d0
  if (cr6.eq) goto loc_821C24D0;
  // stw r9,8(r10)
  schema::PageObject(base, uint32_t(r10)).set_previous_page_bits(uint32_t(r9));
loc_821C24D0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x821c25f4
  goto loc_821C25F4;
loc_821C24D8:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // b 0x821c2608
  goto loc_821C2608;
loc_821C24E0:
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // bne cr6,0x821c2514
  if (!cr6.eq) goto loc_821C2514;
  // lwz r10,660(r5)
  r10 = guest::LoadU32(base, uint32_t(r5) + 660);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821c2514
  if (!cr6.eq) goto loc_821C2514;
  // addi r4,r5,572
  r4 = uint64_t(int64_t(int64_t(r5) + 572));
  // addi r3,r5,380
  r3 = uint64_t(int64_t(int64_t(r5) + 380));
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x821C2504, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf7f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Options::ShowAnalogList2P(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf908
  // (left out: dead in C++)
  { const lifted::Result o = game::Options::ShowAnalogClashes2P(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x821c2608
  goto loc_821C2608;
loc_821C2514:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,-31624(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -31624);
  // stw r8,660(r11)
  guest::StoreU32(base, schema::OptionsSettings(base, uint32_t(r11)).ok_default_address(), uint32_t(r8));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x821c2548
  if (cr6.eq) goto loc_821C2548;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_821C2548:
  // li r9,38
  r9 = uint64_t(int64_t(38));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x821c23d4
  goto loc_821C23D4;
loc_821C2554:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31628(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -31628);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Pressed(base, LIFTED_ARGS(0x821C2560, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c260c
  if (cr0.eq) goto loc_821C260C;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // lwz r11,-31624(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -31624);
  // addi r4,r11,188
  r4 = uint64_t(int64_t(int64_t(r11) + 188));
  // addi r3,r11,380
  r3 = uint64_t(int64_t(int64_t(r11) + 380));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x821C2580, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + -31624);
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // addi r4,r11,212
  r4 = uint64_t(int64_t(int64_t(r11) + 212));
  // addi r3,r11,404
  r3 = uint64_t(int64_t(int64_t(r11) + 404));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x821C2594, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf7f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Options::ShowAnalogList2P(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf908
  // (left out: dead in C++)
  { const lifted::Result o = game::Options::ShowAnalogClashes2P(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c0640
  // (left out: dead in C++)
  { const lifted::Result o = game::ControllerOptions::ShowAnalogList(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c0750
  // (left out: dead in C++)
  { const lifted::Result o = game::ControllerOptions::ShowAnalogClashes(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821c25bc
  if (cr6.eq) goto loc_821C25BC;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_821C25BC:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r8,40
  r8 = uint64_t(int64_t(40));
  // stw r9,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r9));
  // stw r11,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r11));
  // stw r8,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r8));
  // stw r11,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r11));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31604(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -31604);
  // lwz r9,0(r10)
  r9 = schema::PageObject(base, uint32_t(r10)).page_bits();
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // beq cr6,0x821c25f0
  if (cr6.eq) goto loc_821C25F0;
  // stw r9,8(r10)
  schema::PageObject(base, uint32_t(r10)).set_previous_page_bits(uint32_t(r9));
loc_821C25F0:
  // li r3,2
  r3 = uint64_t(int64_t(2));
loc_821C25F4:
  // stw r11,4(r10)
  schema::PageObject(base, uint32_t(r10)).set_next_page_bits(uint32_t(r11));
  // li r9,11
  r9 = uint64_t(int64_t(11));
loc_821C25FC:
  // stw r9,12(r10)
  schema::PageObject(base, uint32_t(r10)).set_step_bits(uint32_t(r9));
  // stw r11,16(r10)
  schema::PageObject(base, uint32_t(r10)).set_queued_step_bits(uint32_t(r11));
  // stw r11,0(r10)
  schema::PageObject(base, uint32_t(r10)).set_page_bits(uint32_t(r11));
loc_821C2608:
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x821C260C, r31)); r3 = o.r3; f1 = o.f1; }
loc_821C260C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821C3E48 game::Options::ChangeButton: lifted from generated/ by lift.py.
lifted::Result f821C3E48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C3E48 (lifted)");
  uint64_t r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-352(r1)
  ea = -352 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x821C3E70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, schema::OptionsMachine(base, uint32_t(r27)).transitions_address());
  // lwz r28,0(r3)
  r28 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c40f4
  if (!cr6.eq) goto loc_821C40F4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31628(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31628);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31624(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31624);
  // addi r9,r11,284
  r9 = uint64_t(int64_t(int64_t(r11) + 284));
  // stw r9,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r9));
  // addi r9,r11,288
  r9 = uint64_t(int64_t(int64_t(r11) + 288));
  // stw r9,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r9));
  // addi r9,r11,292
  r9 = uint64_t(int64_t(int64_t(r11) + 292));
  // stw r9,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r9));
  // addi r9,r11,296
  r9 = uint64_t(int64_t(int64_t(r11) + 296));
  // stw r9,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r9));
  // addi r9,r11,300
  r9 = uint64_t(int64_t(int64_t(r11) + 300));
  // stw r9,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r9));
  // addi r9,r11,304
  r9 = uint64_t(int64_t(int64_t(r11) + 304));
  // stw r9,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r9));
  // addi r9,r11,320
  r9 = uint64_t(int64_t(int64_t(r11) + 320));
  // stw r9,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r9));
  // addi r9,r11,312
  r9 = uint64_t(int64_t(int64_t(r11) + 312));
  // stw r9,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r9));
  // addi r9,r11,316
  r9 = uint64_t(int64_t(int64_t(r11) + 316));
  // stw r9,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r9));
  // addi r9,r11,308
  r9 = uint64_t(int64_t(int64_t(r11) + 308));
  // stw r9,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r9));
  // addi r9,r11,324
  r9 = uint64_t(int64_t(int64_t(r11) + 324));
  // stw r9,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r9));
  // addi r9,r11,328
  r9 = uint64_t(int64_t(int64_t(r11) + 328));
  // stw r9,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r9));
  // li r9,423
  r9 = uint64_t(int64_t(423));
  // stw r9,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r9));
  // li r9,425
  r9 = uint64_t(int64_t(425));
  // stw r9,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r9));
  // li r9,427
  r9 = uint64_t(int64_t(427));
  // stw r9,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r9));
  // li r9,429
  r9 = uint64_t(int64_t(429));
  // stw r9,156(r1)
  guest::StoreU32(base, uint32_t(r1) + 156, uint32_t(r9));
  // li r9,431
  r9 = uint64_t(int64_t(431));
  // stw r9,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r9));
  // li r9,433
  r9 = uint64_t(int64_t(433));
  // stw r9,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r9));
  // li r9,435
  r9 = uint64_t(int64_t(435));
  // stw r9,168(r1)
  guest::StoreU32(base, uint32_t(r1) + 168, uint32_t(r9));
  // li r9,437
  r9 = uint64_t(int64_t(437));
  // stw r9,172(r1)
  guest::StoreU32(base, uint32_t(r1) + 172, uint32_t(r9));
  // li r9,439
  r9 = uint64_t(int64_t(439));
  // stw r9,176(r1)
  guest::StoreU32(base, uint32_t(r1) + 176, uint32_t(r9));
  // li r9,441
  r9 = uint64_t(int64_t(441));
  // stw r9,180(r1)
  guest::StoreU32(base, uint32_t(r1) + 180, uint32_t(r9));
  // li r9,443
  r9 = uint64_t(int64_t(443));
  // stw r9,184(r1)
  guest::StoreU32(base, uint32_t(r1) + 184, uint32_t(r9));
  // li r9,445
  r9 = uint64_t(int64_t(445));
  // stw r9,188(r1)
  guest::StoreU32(base, uint32_t(r1) + 188, uint32_t(r9));
  // li r9,447
  r9 = uint64_t(int64_t(447));
  // stw r9,192(r1)
  guest::StoreU32(base, uint32_t(r1) + 192, uint32_t(r9));
  // li r9,449
  r9 = uint64_t(int64_t(449));
  // stw r9,196(r1)
  guest::StoreU32(base, uint32_t(r1) + 196, uint32_t(r9));
  // li r9,451
  r9 = uint64_t(int64_t(451));
  // stw r9,200(r1)
  guest::StoreU32(base, uint32_t(r1) + 200, uint32_t(r9));
  // li r9,453
  r9 = uint64_t(int64_t(453));
  // stw r9,204(r1)
  guest::StoreU32(base, uint32_t(r1) + 204, uint32_t(r9));
  // li r9,455
  r9 = uint64_t(int64_t(455));
  // stw r9,208(r1)
  guest::StoreU32(base, uint32_t(r1) + 208, uint32_t(r9));
  // li r9,422
  r9 = uint64_t(int64_t(422));
  // stw r9,224(r1)
  guest::StoreU32(base, uint32_t(r1) + 224, uint32_t(r9));
  // li r9,424
  r9 = uint64_t(int64_t(424));
  // stw r9,228(r1)
  guest::StoreU32(base, uint32_t(r1) + 228, uint32_t(r9));
  // li r9,426
  r9 = uint64_t(int64_t(426));
  // stw r9,232(r1)
  guest::StoreU32(base, uint32_t(r1) + 232, uint32_t(r9));
  // li r9,428
  r9 = uint64_t(int64_t(428));
  // stw r9,236(r1)
  guest::StoreU32(base, uint32_t(r1) + 236, uint32_t(r9));
  // li r9,430
  r9 = uint64_t(int64_t(430));
  // stw r9,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r9));
  // li r9,432
  r9 = uint64_t(int64_t(432));
  // stw r9,244(r1)
  guest::StoreU32(base, uint32_t(r1) + 244, uint32_t(r9));
  // li r9,434
  r9 = uint64_t(int64_t(434));
  // stw r9,248(r1)
  guest::StoreU32(base, uint32_t(r1) + 248, uint32_t(r9));
  // li r9,436
  r9 = uint64_t(int64_t(436));
  // stw r9,252(r1)
  guest::StoreU32(base, uint32_t(r1) + 252, uint32_t(r9));
  // li r9,438
  r9 = uint64_t(int64_t(438));
  // stw r9,256(r1)
  guest::StoreU32(base, uint32_t(r1) + 256, uint32_t(r9));
  // li r9,440
  r9 = uint64_t(int64_t(440));
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // stw r9,260(r1)
  guest::StoreU32(base, uint32_t(r1) + 260, uint32_t(r9));
  // li r9,442
  r9 = uint64_t(int64_t(442));
  // stw r9,264(r1)
  guest::StoreU32(base, uint32_t(r1) + 264, uint32_t(r9));
  // li r9,444
  r9 = uint64_t(int64_t(444));
  // stw r9,268(r1)
  guest::StoreU32(base, uint32_t(r1) + 268, uint32_t(r9));
  // li r9,446
  r9 = uint64_t(int64_t(446));
  // stw r9,272(r1)
  guest::StoreU32(base, uint32_t(r1) + 272, uint32_t(r9));
  // li r9,448
  r9 = uint64_t(int64_t(448));
  // stw r9,276(r1)
  guest::StoreU32(base, uint32_t(r1) + 276, uint32_t(r9));
  // li r9,450
  r9 = uint64_t(int64_t(450));
  // stw r9,280(r1)
  guest::StoreU32(base, uint32_t(r1) + 280, uint32_t(r9));
  // li r9,452
  r9 = uint64_t(int64_t(452));
  // stw r9,284(r1)
  guest::StoreU32(base, uint32_t(r1) + 284, uint32_t(r9));
  // li r9,454
  r9 = uint64_t(int64_t(454));
  // stw r9,288(r1)
  guest::StoreU32(base, uint32_t(r1) + 288, uint32_t(r9));
  // bgt cr6,0x821c414c
  if (cr6.gt) goto loc_821C414C;
  // beq cr6,0x821c412c
  if (cr6.eq) goto loc_821C412C;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821c412c
  if (cr6.lt) goto loc_821C412C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // blt cr6,0x821c410c
  if (cr6.lt) goto loc_821C410C;
  // cmplwi cr6,r10,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 10, so);
  // bge cr6,0x821c40e8
  if (!cr6.lt) goto loc_821C40E8;
loc_821C4028:
  // lwz r4,668(r11)
  r4 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).buttons_side_address());
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r30,r10,r11
  r30 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r30)
  r7 = guest::LoadU32(base, uint32_t(r30) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821ba5f0
  // (left out: dead in C++)
  r3 = game::Controls::CycleButton(base, r1, r3, r4);
loc_821C4044:
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r7,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r7), -1, so);
  // beq cr6,0x821c40e8
  if (cr6.eq) goto loc_821C40E8;
  // cmpwi cr6,r31,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r31), -1, so);
  // beq cr6,0x821c40e8
  if (cr6.eq) goto loc_821C40E8;
  // cmpw cr6,r7,r31
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r31), so);
  // beq cr6,0x821c40e0
  if (cr6.eq) goto loc_821C40E0;
  // rlwinm r29,r7,2,0,29
  r29 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,224
  r11 = uint64_t(int64_t(int64_t(r1) + 224));
  // mr r3,r28
  r3 = r28;
  // lwzx r4,r29,r11
  r4 = guest::LoadU32(base, uint32_t(r29) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C4074, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,144
  r11 = uint64_t(int64_t(int64_t(r1) + 144));
  // mr r3,r28
  r3 = r28;
  // lwzx r4,r29,r11
  r4 = guest::LoadU32(base, uint32_t(r29) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C4084, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r9,r1,224
  r9 = uint64_t(int64_t(int64_t(r1) + 224));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwzx r29,r11,r10
  r29 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // mr r3,r28
  r3 = r28;
  // lwzx r4,r11,r9
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r9));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C40B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r29
  r4 = r29;
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C40CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x821C40D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x821bf0f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BF0F0, LIFTED_ARGS(0x821C40E0, r31)); r3 = o.r3; f1 = o.f1; }
loc_821C40E0:
  // stw r31,0(r30)
  guest::StoreU32(base, uint32_t(r30) + 0, uint32_t(r31));
  // bl 0x821befe8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BEFE8, LIFTED_ARGS(0x821C40E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_821C40E8:
  // bl 0x821bf158
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BF158, LIFTED_ARGS(0x821C40EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  guest::StoreU32(base, schema::OptionsMachine(base, uint32_t(r27)).transitions_address(), uint32_t(r11));
loc_821C40F4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x821C4100, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,352
  r1 = uint64_t(int64_t(int64_t(r1) + 352));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_821C410C:
  // lwz r4,668(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 668);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r30,r10,r11
  r30 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r30)
  r7 = guest::LoadU32(base, uint32_t(r30) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821ba710
  // (left out: dead in C++)
  r3 = game::Controls::CycleButtonWide(base, r1, r3, r4);
  // b 0x821c4044
  goto loc_821C4044;
loc_821C412C:
  // lwz r4,668(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 668);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r30,r10,r11
  r30 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r30)
  r7 = guest::LoadU32(base, uint32_t(r30) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821ba6a0
  // (left out: dead in C++)
  r3 = game::Controls::CycleButtonPair(base, r1, r3, r4);
  // b 0x821c4044
  goto loc_821C4044;
loc_821C414C:
  // cmpwi cr6,r10,11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 11, so);
  // beq cr6,0x821c4028
  if (cr6.eq) goto loc_821C4028;
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // bne cr6,0x821c40e8
  if (!cr6.eq) goto loc_821C40E8;
  // lwz r11,660(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 660);
  // mr r3,r28
  r3 = r28;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821c41a0
  if (!cr6.eq) goto loc_821C41A0;
  // li r4,353
  r4 = uint64_t(int64_t(353));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C4174, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,354
  r4 = uint64_t(int64_t(354));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C4190, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,354
  r4 = uint64_t(int64_t(354));
loc_821C4194:
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x821C419C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821c40e8
  goto loc_821C40E8;
loc_821C41A0:
  // li r4,354
  r4 = uint64_t(int64_t(354));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C41A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,353
  r4 = uint64_t(int64_t(353));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C41C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,353
  r4 = uint64_t(int64_t(353));
  // b 0x821c4194
  goto loc_821C4194;
}

// sub_821C41D0 game::Options::ChangeButton2P: lifted from generated/ by lift.py.
lifted::Result f821C41D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C41D0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x821C4200, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::OptionsMachine(base, uint32_t(r30)).transitions_address());
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c42e8
  if (!cr6.eq) goto loc_821C42E8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31628(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31628);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // lwz r11,-31624(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31624);
  // addi r9,r11,284
  r9 = uint64_t(int64_t(int64_t(r11) + 284));
  // stw r9,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r9));
  // addi r9,r11,288
  r9 = uint64_t(int64_t(int64_t(r11) + 288));
  // stw r9,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r9));
  // addi r9,r11,292
  r9 = uint64_t(int64_t(int64_t(r11) + 292));
  // stw r9,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r9));
  // addi r9,r11,296
  r9 = uint64_t(int64_t(int64_t(r11) + 296));
  // stw r9,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r9));
  // addi r9,r11,300
  r9 = uint64_t(int64_t(int64_t(r11) + 300));
  // stw r9,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r9));
  // addi r9,r11,304
  r9 = uint64_t(int64_t(int64_t(r11) + 304));
  // stw r9,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r9));
  // addi r9,r11,320
  r9 = uint64_t(int64_t(int64_t(r11) + 320));
  // stw r9,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r9));
  // addi r9,r11,312
  r9 = uint64_t(int64_t(int64_t(r11) + 312));
  // stw r9,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r9));
  // addi r9,r11,316
  r9 = uint64_t(int64_t(int64_t(r11) + 316));
  // stw r9,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r9));
  // addi r9,r11,308
  r9 = uint64_t(int64_t(int64_t(r11) + 308));
  // stw r9,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r9));
  // addi r9,r11,324
  r9 = uint64_t(int64_t(int64_t(r11) + 324));
  // stw r9,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r9));
  // addi r9,r11,328
  r9 = uint64_t(int64_t(int64_t(r11) + 328));
  // stw r9,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r9));
  // bgt cr6,0x821c4350
  if (cr6.gt) goto loc_821C4350;
  // beq cr6,0x821c4330
  if (cr6.eq) goto loc_821C4330;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821c4330
  if (cr6.lt) goto loc_821C4330;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // blt cr6,0x821c4310
  if (cr6.lt) goto loc_821C4310;
  // cmplwi cr6,r10,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 10, so);
  // bge cr6,0x821c42dc
  if (!cr6.lt) goto loc_821C42DC;
loc_821C42A8:
  // lwz r4,668(r11)
  r4 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).buttons_side_address());
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r6)
  r7 = guest::LoadU32(base, uint32_t(r6) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821ba5f0
  // (left out: dead in C++)
  r3 = game::Controls::CycleButton(base, r1, r3, r4);
loc_821C42C4:
  // cmpwi cr6,r7,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r7), -1, so);
  // beq cr6,0x821c42dc
  if (cr6.eq) goto loc_821C42DC;
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x821c42dc
  if (cr6.eq) goto loc_821C42DC;
  // stw r3,0(r6)
  guest::StoreU32(base, uint32_t(r6) + 0, uint32_t(r3));
  // bl 0x821bf2b8
  // (left out: dead in C++)
  { const lifted::Result o = game::Options::ShowButtonList2P(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821C42DC:
  // bl 0x821bf3c0
  // (left out: dead in C++)
  { const lifted::Result o = game::Options::ShowButtonClashes2P(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::OptionsMachine(base, uint32_t(r30)).transitions_address(), uint32_t(r11));
loc_821C42E8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x821C42F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_821C4310:
  // lwz r4,668(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 668);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r6)
  r7 = guest::LoadU32(base, uint32_t(r6) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821ba710
  // (left out: dead in C++)
  r3 = game::Controls::CycleButtonWide(base, r1, r3, r4);
  // b 0x821c42c4
  goto loc_821C42C4;
loc_821C4330:
  // lwz r4,668(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 668);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r6)
  r7 = guest::LoadU32(base, uint32_t(r6) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821ba6a0
  // (left out: dead in C++)
  r3 = game::Controls::CycleButtonPair(base, r1, r3, r4);
  // b 0x821c42c4
  goto loc_821C42C4;
loc_821C4350:
  // cmpwi cr6,r10,11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 11, so);
  // beq cr6,0x821c42a8
  if (cr6.eq) goto loc_821C42A8;
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // bne cr6,0x821c42dc
  if (!cr6.eq) goto loc_821C42DC;
  // lwz r11,660(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 660);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821c43a4
  if (!cr6.eq) goto loc_821C43A4;
  // li r4,527
  r4 = uint64_t(int64_t(527));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C4378, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,528
  r4 = uint64_t(int64_t(528));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C4394, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,528
  r4 = uint64_t(int64_t(528));
loc_821C4398:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x821C43A0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821c42dc
  goto loc_821C42DC;
loc_821C43A4:
  // li r4,528
  r4 = uint64_t(int64_t(528));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C43AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,527
  r4 = uint64_t(int64_t(527));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C43C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,527
  r4 = uint64_t(int64_t(527));
  // b 0x821c4398
  goto loc_821C4398;
}

// sub_821C43D0 game::Options::MoveAnalogCursor: lifted from generated/ by lift.py.
lifted::Result f821C43D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C43D0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-336(r1)
  ea = -336 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x821C43F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,674
  r11 = uint64_t(int64_t(674));
  // lwz r10,0(r26)
  r10 = guest::LoadU32(base, schema::OptionsMachine(base, uint32_t(r26)).transitions_address());
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // stw r11,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r11));
  // lwz r11,-31624(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + -31624);
  // lwz r9,380(r11)
  r9 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).edit_analog_0_address());
  // stw r9,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r9));
  // li r9,676
  r9 = uint64_t(int64_t(676));
  // stw r9,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r9));
  // lwz r9,384(r11)
  r9 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).edit_analog_1_address());
  // stw r9,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r9));
  // li r9,678
  r9 = uint64_t(int64_t(678));
  // stw r9,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r9));
  // lwz r9,388(r11)
  r9 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).edit_analog_2_address());
  // stw r9,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r9));
  // li r9,680
  r9 = uint64_t(int64_t(680));
  // stw r9,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r9));
  // lwz r9,396(r11)
  r9 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).edit_analog_4_address());
  // stw r9,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r9));
  // li r9,682
  r9 = uint64_t(int64_t(682));
  // stw r9,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r9));
  // lwz r9,392(r11)
  r9 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).edit_analog_3_address());
  // stw r9,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r9));
  // li r9,684
  r9 = uint64_t(int64_t(684));
  // stw r9,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r9));
  // lwz r9,400(r11)
  r9 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).edit_analog_5_address());
  // stw r9,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r9));
  // li r9,686
  r9 = uint64_t(int64_t(686));
  // stw r9,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r9));
  // li r9,688
  r9 = uint64_t(int64_t(688));
  // stw r9,156(r1)
  guest::StoreU32(base, uint32_t(r1) + 156, uint32_t(r9));
  // li r9,690
  r9 = uint64_t(int64_t(690));
  // stw r9,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r9));
  // li r9,692
  r9 = uint64_t(int64_t(692));
  // stw r9,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r9));
  // li r9,694
  r9 = uint64_t(int64_t(694));
  // stw r9,168(r1)
  guest::StoreU32(base, uint32_t(r1) + 168, uint32_t(r9));
  // li r9,673
  r9 = uint64_t(int64_t(673));
  // stw r9,176(r1)
  guest::StoreU32(base, uint32_t(r1) + 176, uint32_t(r9));
  // li r9,675
  r9 = uint64_t(int64_t(675));
  // stw r9,180(r1)
  guest::StoreU32(base, uint32_t(r1) + 180, uint32_t(r9));
  // li r9,677
  r9 = uint64_t(int64_t(677));
  // stw r9,184(r1)
  guest::StoreU32(base, uint32_t(r1) + 184, uint32_t(r9));
  // li r9,679
  r9 = uint64_t(int64_t(679));
  // stw r9,188(r1)
  guest::StoreU32(base, uint32_t(r1) + 188, uint32_t(r9));
  // li r9,681
  r9 = uint64_t(int64_t(681));
  // stw r9,192(r1)
  guest::StoreU32(base, uint32_t(r1) + 192, uint32_t(r9));
  // li r9,683
  r9 = uint64_t(int64_t(683));
  // stw r9,196(r1)
  guest::StoreU32(base, uint32_t(r1) + 196, uint32_t(r9));
  // li r9,685
  r9 = uint64_t(int64_t(685));
  // stw r9,200(r1)
  guest::StoreU32(base, uint32_t(r1) + 200, uint32_t(r9));
  // li r9,687
  r9 = uint64_t(int64_t(687));
  // stw r9,204(r1)
  guest::StoreU32(base, uint32_t(r1) + 204, uint32_t(r9));
  // li r9,689
  r9 = uint64_t(int64_t(689));
  // stw r9,208(r1)
  guest::StoreU32(base, uint32_t(r1) + 208, uint32_t(r9));
  // li r9,691
  r9 = uint64_t(int64_t(691));
  // stw r9,212(r1)
  guest::StoreU32(base, uint32_t(r1) + 212, uint32_t(r9));
  // li r9,693
  r9 = uint64_t(int64_t(693));
  // stw r9,216(r1)
  guest::StoreU32(base, uint32_t(r1) + 216, uint32_t(r9));
  // li r9,607
  r9 = uint64_t(int64_t(607));
  // stw r9,224(r1)
  guest::StoreU32(base, uint32_t(r1) + 224, uint32_t(r9));
  // li r9,611
  r9 = uint64_t(int64_t(611));
  // stw r9,228(r1)
  guest::StoreU32(base, uint32_t(r1) + 228, uint32_t(r9));
  // li r9,615
  r9 = uint64_t(int64_t(615));
  // stw r9,232(r1)
  guest::StoreU32(base, uint32_t(r1) + 232, uint32_t(r9));
  // li r9,619
  r9 = uint64_t(int64_t(619));
  // stw r9,236(r1)
  guest::StoreU32(base, uint32_t(r1) + 236, uint32_t(r9));
  // li r9,623
  r9 = uint64_t(int64_t(623));
  // stw r9,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r9));
  // li r9,627
  r9 = uint64_t(int64_t(627));
  // stw r9,244(r1)
  guest::StoreU32(base, uint32_t(r1) + 244, uint32_t(r9));
  // li r9,631
  r9 = uint64_t(int64_t(631));
  // stw r9,248(r1)
  guest::StoreU32(base, uint32_t(r1) + 248, uint32_t(r9));
  // li r9,635
  r9 = uint64_t(int64_t(635));
  // stw r9,252(r1)
  guest::StoreU32(base, uint32_t(r1) + 252, uint32_t(r9));
  // li r9,639
  r9 = uint64_t(int64_t(639));
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r9,256(r1)
  guest::StoreU32(base, uint32_t(r1) + 256, uint32_t(r9));
  // li r9,643
  r9 = uint64_t(int64_t(643));
  // stw r9,260(r1)
  guest::StoreU32(base, uint32_t(r1) + 260, uint32_t(r9));
  // li r9,647
  r9 = uint64_t(int64_t(647));
  // stw r9,264(r1)
  guest::StoreU32(base, uint32_t(r1) + 264, uint32_t(r9));
  // li r9,651
  r9 = uint64_t(int64_t(651));
  // stw r9,268(r1)
  guest::StoreU32(base, uint32_t(r1) + 268, uint32_t(r9));
  // li r9,655
  r9 = uint64_t(int64_t(655));
  // stw r9,272(r1)
  guest::StoreU32(base, uint32_t(r1) + 272, uint32_t(r9));
  // bne cr6,0x821c4788
  if (!cr6.eq) goto loc_821C4788;
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r10,-31628(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -31628);
  // lwz r28,4(r10)
  r28 = guest::LoadU32(base, uint32_t(r10) + 4);
  // lwz r27,0(r10)
  r27 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmpwi cr6,r28,6
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 6, so);
  // bge cr6,0x821c4580
  if (!cr6.lt) goto loc_821C4580;
  // addi r4,r28,658
  r4 = uint64_t(int64_t(int64_t(r28) + 658));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C457C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821c45cc
  goto loc_821C45CC;
loc_821C4580:
  // lwz r11,660(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 660);
  // li r4,605
  r4 = uint64_t(int64_t(605));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821c4594
  if (cr6.eq) goto loc_821C4594;
  // li r4,604
  r4 = uint64_t(int64_t(604));
loc_821C4594:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C4598, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,603
  r4 = uint64_t(int64_t(603));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C45B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,603
  r4 = uint64_t(int64_t(603));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C45C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + -31624);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,660(r11)
  guest::StoreU32(base, schema::OptionsSettings(base, uint32_t(r11)).ok_default_address(), uint32_t(r10));
loc_821C45CC:
  // cmpwi cr6,r27,6
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 6, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bge cr6,0x821c45fc
  if (!cr6.lt) goto loc_821C45FC;
  // addi r30,r27,658
  r30 = uint64_t(int64_t(int64_t(r27) + 658));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C45F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // b 0x821c4660
  goto loc_821C4660;
loc_821C45FC:
  // rlwinm r11,r28,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,224
  r10 = uint64_t(int64_t(int64_t(r1) + 224));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwzx r30,r11,r10
  r30 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C4614, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C4620, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + -31624);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,660(r11)
  r11 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).ok_default_address());
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821c4654
  if (!cr6.eq) goto loc_821C4654;
  // li r4,605
  r4 = uint64_t(int64_t(605));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C464C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,605
  r4 = uint64_t(int64_t(605));
  // b 0x821c4660
  goto loc_821C4660;
loc_821C4654:
  // li r4,604
  r4 = uint64_t(int64_t(604));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C465C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,604
  r4 = uint64_t(int64_t(604));
loc_821C4660:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x821C4668, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r27,6
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 6, so);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // beq cr6,0x821c46e4
  if (cr6.eq) goto loc_821C46E4;
  // cmpwi cr6,r28,6
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 6, so);
  // beq cr6,0x821c4710
  if (cr6.eq) goto loc_821C4710;
  // rlwinm r11,r28,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x821bad08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BAD08, LIFTED_ARGS(0x821C4688, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r27,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r10,r3
  r10 = r3;
  // lwzx r3,r11,r9
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r9));
  // bl 0x821bad08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BAD08, LIFTED_ARGS(0x821C469C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpw cr6,r10,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r30), so);
  // beq cr6,0x821c4780
  if (cr6.eq) goto loc_821C4780;
  // rlwinm r29,r10,2,0,29
  r29 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,176
  r11 = uint64_t(int64_t(int64_t(r1) + 176));
  // mr r3,r31
  r3 = r31;
  // lwzx r4,r29,r11
  r4 = guest::LoadU32(base, uint32_t(r29) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C46BC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,128
  r11 = uint64_t(int64_t(int64_t(r1) + 128));
  // mr r3,r31
  r3 = r31;
  // lwzx r4,r29,r11
  r4 = guest::LoadU32(base, uint32_t(r29) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C46CC, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r9,r1,176
  r9 = uint64_t(int64_t(int64_t(r1) + 176));
  // lwzx r29,r11,r10
  r29 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // lwzx r4,r11,r9
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r9));
  // b 0x821c4738
  goto loc_821C4738;
loc_821C46E4:
  // rlwinm r11,r28,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x821bad08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BAD08, LIFTED_ARGS(0x821C46F0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r3,r31
  r3 = r31;
  // lwzx r4,r11,r10
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C4708, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // b 0x821c477c
  goto loc_821C477C;
loc_821C4710:
  // rlwinm r11,r27,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x821bad08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BAD08, LIFTED_ARGS(0x821C471C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // addi r11,r1,128
  r11 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r10,r1,176
  r10 = uint64_t(int64_t(int64_t(r1) + 176));
  // rlwinm r9,r30,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r4,r30,2,0,29
  r4 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // lwzx r29,r9,r11
  r29 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r11));
  // lwzx r4,r4,r10
  r4 = guest::LoadU32(base, uint32_t(r4) + uint32_t(r10));
loc_821C4738:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C4750, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C476C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x821C4778, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
loc_821C477C:
  // bl 0x821bf630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BF630, LIFTED_ARGS(0x821C4780, r31)); r3 = o.r3; f1 = o.f1; }
loc_821C4780:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  guest::StoreU32(base, schema::OptionsMachine(base, uint32_t(r26)).transitions_address(), uint32_t(r11));
loc_821C4788:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x821C4794, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,336
  r1 = uint64_t(int64_t(int64_t(r1) + 336));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C47A0 game::Options::ChangeAnalog: lifted from generated/ by lift.py.
lifted::Result f821C47A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821C47A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-288(r1)
  ea = -288 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x821C47C8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, schema::OptionsMachine(base, uint32_t(r26)).transitions_address());
  // lwz r30,0(r3)
  r30 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c4a40
  if (!cr6.eq) goto loc_821C4A40;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r31,-1
  r31 = uint64_t(int64_t(-1));
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // lwz r11,-31628(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31628);
  // lwz r27,0(r11)
  r27 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // cmplwi cr6,r27,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 6, so);
  // lwz r11,-31624(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31624);
  // addi r10,r11,380
  r10 = uint64_t(int64_t(int64_t(r11) + 380));
  // stw r10,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r10));
  // addi r10,r11,384
  r10 = uint64_t(int64_t(int64_t(r11) + 384));
  // stw r10,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r10));
  // addi r10,r11,388
  r10 = uint64_t(int64_t(int64_t(r11) + 388));
  // stw r10,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r10));
  // addi r10,r11,396
  r10 = uint64_t(int64_t(int64_t(r11) + 396));
  // stw r10,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r10));
  // addi r10,r11,392
  r10 = uint64_t(int64_t(int64_t(r11) + 392));
  // stw r10,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r10));
  // addi r10,r11,400
  r10 = uint64_t(int64_t(int64_t(r11) + 400));
  // stw r10,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r10));
  // li r10,674
  r10 = uint64_t(int64_t(674));
  // stw r10,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r10));
  // li r10,676
  r10 = uint64_t(int64_t(676));
  // stw r10,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r10));
  // li r10,678
  r10 = uint64_t(int64_t(678));
  // stw r10,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r10));
  // li r10,680
  r10 = uint64_t(int64_t(680));
  // stw r10,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r10));
  // li r10,682
  r10 = uint64_t(int64_t(682));
  // stw r10,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r10));
  // li r10,684
  r10 = uint64_t(int64_t(684));
  // stw r10,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r10));
  // li r10,686
  r10 = uint64_t(int64_t(686));
  // stw r10,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r10));
  // li r10,688
  r10 = uint64_t(int64_t(688));
  // stw r10,156(r1)
  guest::StoreU32(base, uint32_t(r1) + 156, uint32_t(r10));
  // li r10,690
  r10 = uint64_t(int64_t(690));
  // stw r10,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r10));
  // li r10,692
  r10 = uint64_t(int64_t(692));
  // stw r10,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r10));
  // li r10,694
  r10 = uint64_t(int64_t(694));
  // stw r10,168(r1)
  guest::StoreU32(base, uint32_t(r1) + 168, uint32_t(r10));
  // li r10,673
  r10 = uint64_t(int64_t(673));
  // stw r10,176(r1)
  guest::StoreU32(base, uint32_t(r1) + 176, uint32_t(r10));
  // li r10,675
  r10 = uint64_t(int64_t(675));
  // stw r10,180(r1)
  guest::StoreU32(base, uint32_t(r1) + 180, uint32_t(r10));
  // li r10,677
  r10 = uint64_t(int64_t(677));
  // stw r10,184(r1)
  guest::StoreU32(base, uint32_t(r1) + 184, uint32_t(r10));
  // li r10,679
  r10 = uint64_t(int64_t(679));
  // stw r10,188(r1)
  guest::StoreU32(base, uint32_t(r1) + 188, uint32_t(r10));
  // li r10,681
  r10 = uint64_t(int64_t(681));
  // stw r10,192(r1)
  guest::StoreU32(base, uint32_t(r1) + 192, uint32_t(r10));
  // li r10,683
  r10 = uint64_t(int64_t(683));
  // stw r10,196(r1)
  guest::StoreU32(base, uint32_t(r1) + 196, uint32_t(r10));
  // li r10,685
  r10 = uint64_t(int64_t(685));
  // stw r10,200(r1)
  guest::StoreU32(base, uint32_t(r1) + 200, uint32_t(r10));
  // li r10,687
  r10 = uint64_t(int64_t(687));
  // stw r10,204(r1)
  guest::StoreU32(base, uint32_t(r1) + 204, uint32_t(r10));
  // li r10,689
  r10 = uint64_t(int64_t(689));
  // stw r10,208(r1)
  guest::StoreU32(base, uint32_t(r1) + 208, uint32_t(r10));
  // li r10,691
  r10 = uint64_t(int64_t(691));
  // stw r10,212(r1)
  guest::StoreU32(base, uint32_t(r1) + 212, uint32_t(r10));
  // li r10,693
  r10 = uint64_t(int64_t(693));
  // stw r10,216(r1)
  guest::StoreU32(base, uint32_t(r1) + 216, uint32_t(r10));
  // blt cr6,0x821c4950
  if (cr6.lt) goto loc_821C4950;
  // bne cr6,0x821c4970
  if (!cr6.eq) goto loc_821C4970;
  // lwz r11,660(r11)
  r11 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).ok_default_address());
  // mr r3,r30
  r3 = r30;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821c491c
  if (!cr6.eq) goto loc_821C491C;
  // li r4,604
  r4 = uint64_t(int64_t(604));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C48F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,605
  r4 = uint64_t(int64_t(605));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C4914, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,605
  r4 = uint64_t(int64_t(605));
  // b 0x821c4944
  goto loc_821C4944;
loc_821C491C:
  // li r4,605
  r4 = uint64_t(int64_t(605));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C4924, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,604
  r4 = uint64_t(int64_t(604));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C4940, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,604
  r4 = uint64_t(int64_t(604));
loc_821C4944:
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x821C494C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821c4970
  goto loc_821C4970;
loc_821C4950:
  // lwz r4,676(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 676);
  // rlwinm r10,r27,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r31,0(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mr r3,r31
  r3 = r31;
  // bl 0x821bad80
  // (left out: dead in C++)
  r3 = game::Controls::CycleAnalog(base, r1, r3, r4);
  // mr r28,r3
  r28 = r3;
loc_821C4970:
  // mr r3,r31
  r3 = r31;
  // bl 0x821bad08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BAD08, LIFTED_ARGS(0x821C4978, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r3
  r10 = r3;
  // mr r3,r28
  r3 = r28;
  // bl 0x821bad08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BAD08, LIFTED_ARGS(0x821C4984, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // beq cr6,0x821c4a34
  if (cr6.eq) goto loc_821C4A34;
  // cmpwi cr6,r29,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), -1, so);
  // beq cr6,0x821c4a34
  if (cr6.eq) goto loc_821C4A34;
  // cmpw cr6,r31,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r28), so);
  // beq cr6,0x821c4a20
  if (cr6.eq) goto loc_821C4A20;
  // rlwinm r31,r10,2,0,29
  r31 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,176
  r11 = uint64_t(int64_t(int64_t(r1) + 176));
  // mr r3,r30
  r3 = r30;
  // lwzx r4,r31,r11
  r4 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C49B4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,128
  r11 = uint64_t(int64_t(int64_t(r1) + 128));
  // mr r3,r30
  r3 = r30;
  // lwzx r4,r31,r11
  r4 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C49C4, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r29,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r9,r1,176
  r9 = uint64_t(int64_t(int64_t(r1) + 176));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwzx r31,r11,r10
  r31 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // mr r3,r30
  r3 = r30;
  // lwzx r4,r11,r9
  r4 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r9));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C49F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C4A0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x821C4A18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x821bf630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BF630, LIFTED_ARGS(0x821C4A20, r31)); r3 = o.r3; f1 = o.f1; }
loc_821C4A20:
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // rlwinm r11,r27,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // stw r28,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r28));
  // bl 0x821bf520
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BF520, LIFTED_ARGS(0x821C4A34, r31)); r3 = o.r3; f1 = o.f1; }
loc_821C4A34:
  // bl 0x821bf698
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BF698, LIFTED_ARGS(0x821C4A38, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  guest::StoreU32(base, schema::OptionsMachine(base, uint32_t(r26)).transitions_address(), uint32_t(r11));
loc_821C4A40:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x821C4A4C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,288
  r1 = uint64_t(int64_t(int64_t(r1) + 288));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C4A58 game::Options::ChangeAnalog2P: lifted from generated/ by lift.py.
lifted::Result f821C4A58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C4A58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Find(base, LIFTED_ARGS(0x821C4A88, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::OptionsMachine(base, uint32_t(r30)).transitions_address());
  // lwz r31,0(r3)
  r31 = guest::LoadU32(base, schema::UiMovieTableValue(base, uint32_t(r3)).value_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c4bb4
  if (!cr6.eq) goto loc_821C4BB4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31628(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31628);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r11,-31624(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31624);
  // addi r9,r11,380
  r9 = uint64_t(int64_t(int64_t(r11) + 380));
  // stw r9,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r9));
  // addi r9,r11,384
  r9 = uint64_t(int64_t(int64_t(r11) + 384));
  // stw r9,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r9));
  // addi r9,r11,388
  r9 = uint64_t(int64_t(int64_t(r11) + 388));
  // stw r9,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r9));
  // addi r9,r11,396
  r9 = uint64_t(int64_t(int64_t(r11) + 396));
  // stw r9,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r9));
  // addi r9,r11,392
  r9 = uint64_t(int64_t(int64_t(r11) + 392));
  // stw r9,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r9));
  // addi r9,r11,400
  r9 = uint64_t(int64_t(int64_t(r11) + 400));
  // stw r9,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r9));
  // blt 0x821c4ba8
  if (cr0.lt) goto loc_821C4BA8;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // ble cr6,0x821c4b74
  if (!cr6.gt) goto loc_821C4B74;
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // ble cr6,0x821c4ba8
  if (!cr6.gt) goto loc_821C4BA8;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // ble cr6,0x821c4b74
  if (!cr6.gt) goto loc_821C4B74;
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // bne cr6,0x821c4ba8
  if (!cr6.eq) goto loc_821C4BA8;
  // lwz r11,660(r11)
  r11 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).ok_default_address());
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821c4b48
  if (!cr6.eq) goto loc_821C4B48;
  // li r4,766
  r4 = uint64_t(int64_t(766));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C4B1C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,767
  r4 = uint64_t(int64_t(767));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C4B38, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,767
  r4 = uint64_t(int64_t(767));
loc_821C4B3C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::LoopNamed(base, LIFTED_ARGS(0x821C4B44, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821c4ba8
  goto loc_821C4BA8;
loc_821C4B48:
  // li r4,767
  r4 = uint64_t(int64_t(767));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::FinishNamed(base, LIFTED_ARGS(0x821C4B50, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,766
  r4 = uint64_t(int64_t(766));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::PlayNamed(base, LIFTED_ARGS(0x821C4B6C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,766
  r4 = uint64_t(int64_t(766));
  // b 0x821c4b3c
  goto loc_821C4B3C;
loc_821C4B74:
  // lwz r4,676(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 676);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r6)
  r7 = guest::LoadU32(base, uint32_t(r6) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821bad80
  // (left out: dead in C++)
  r3 = game::Controls::CycleAnalog(base, r1, r3, r4);
  // cmpwi cr6,r7,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r7), -1, so);
  // beq cr6,0x821c4ba8
  if (cr6.eq) goto loc_821C4BA8;
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x821c4ba8
  if (cr6.eq) goto loc_821C4BA8;
  // stw r3,0(r6)
  guest::StoreU32(base, uint32_t(r6) + 0, uint32_t(r3));
  // bl 0x821bf7f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Options::ShowAnalogList2P(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821C4BA8:
  // bl 0x821bf908
  // (left out: dead in C++)
  { const lifted::Result o = game::Options::ShowAnalogClashes2P(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  guest::StoreU32(base, schema::OptionsMachine(base, uint32_t(r30)).transitions_address(), uint32_t(r11));
loc_821C4BB4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovie::Run(base, LIFTED_ARGS(0x821C4BC0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821C5F80 game::ControllerOptions::ChangePage1Value: lifted from generated/ by lift.py.
lifted::Result f821C5F80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C5F80 (lifted)");
  uint64_t r4 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::ControllerOptions(base, uint32_t(r31)).fault_step_started_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c60b8
  if (!cr6.eq) goto loc_821C60B8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31600(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31600);
  // lwz r10,0(r11)
  r10 = schema::FrontEndMenu(base, uint32_t(r11)).cursor_bits();
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // lwz r11,-31596(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31596);
  // addi r9,r11,332
  r9 = uint64_t(int64_t(int64_t(r11) + 332));
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // addi r9,r11,336
  r9 = uint64_t(int64_t(int64_t(r11) + 336));
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // addi r9,r11,340
  r9 = uint64_t(int64_t(int64_t(r11) + 340));
  // stw r9,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r9));
  // addi r9,r11,344
  r9 = uint64_t(int64_t(int64_t(r11) + 344));
  // stw r9,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r9));
  // addi r9,r11,348
  r9 = uint64_t(int64_t(int64_t(r11) + 348));
  // stw r9,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r9));
  // addi r9,r11,352
  r9 = uint64_t(int64_t(int64_t(r11) + 352));
  // stw r9,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r9));
  // addi r9,r11,368
  r9 = uint64_t(int64_t(int64_t(r11) + 368));
  // stw r9,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r9));
  // addi r9,r11,360
  r9 = uint64_t(int64_t(int64_t(r11) + 360));
  // stw r9,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r9));
  // addi r9,r11,364
  r9 = uint64_t(int64_t(int64_t(r11) + 364));
  // stw r9,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r9));
  // addi r9,r11,356
  r9 = uint64_t(int64_t(int64_t(r11) + 356));
  // stw r9,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r9));
  // addi r9,r11,372
  r9 = uint64_t(int64_t(int64_t(r11) + 372));
  // stw r9,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r9));
  // addi r9,r11,376
  r9 = uint64_t(int64_t(int64_t(r11) + 376));
  // stw r9,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r9));
  // blt cr6,0x821c6078
  if (cr6.lt) goto loc_821C6078;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // blt cr6,0x821c6058
  if (cr6.lt) goto loc_821C6058;
  // cmplwi cr6,r10,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 10, so);
  // blt cr6,0x821c6038
  if (cr6.lt) goto loc_821C6038;
  // beq cr6,0x821c6078
  if (cr6.eq) goto loc_821C6078;
  // cmplwi cr6,r10,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 12, so);
  // bge cr6,0x821c60ac
  if (!cr6.lt) goto loc_821C60AC;
loc_821C6038:
  // lwz r4,672(r11)
  r4 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).p1_buttons_side_address());
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r6)
  r7 = guest::LoadU32(base, uint32_t(r6) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821ba5f0
  // (left out: dead in C++)
  r3 = game::Controls::CycleButton(base, r1, r3, r4);
  // b 0x821c6094
  goto loc_821C6094;
loc_821C6058:
  // lwz r4,672(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 672);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r6)
  r7 = guest::LoadU32(base, uint32_t(r6) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821ba710
  // (left out: dead in C++)
  r3 = game::Controls::CycleButtonWide(base, r1, r3, r4);
  // b 0x821c6094
  goto loc_821C6094;
loc_821C6078:
  // lwz r4,672(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 672);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r6)
  r7 = guest::LoadU32(base, uint32_t(r6) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821ba6a0
  // (left out: dead in C++)
  r3 = game::Controls::CycleButtonPair(base, r1, r3, r4);
loc_821C6094:
  // cmpwi cr6,r7,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r7), -1, so);
  // beq cr6,0x821c60ac
  if (cr6.eq) goto loc_821C60AC;
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x821c60ac
  if (cr6.eq) goto loc_821C60AC;
  // stw r3,0(r6)
  guest::StoreU32(base, uint32_t(r6) + 0, uint32_t(r3));
  // bl 0x821c03d8
  // (left out: dead in C++)
  { const lifted::Result o = game::ControllerOptions::ShowButtonList(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821C60AC:
  // bl 0x821c04e0
  // (left out: dead in C++)
  { const lifted::Result o = game::ControllerOptions::ShowButtonClashes(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::ControllerOptions(base, uint32_t(r31)).fault_step_started_address(), uint32_t(r11));
loc_821C60B8:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821C60D0 game::ControllerOptions::ChangePage2Value: lifted from generated/ by lift.py.
lifted::Result f821C60D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C60D0 (lifted)");
  uint64_t r4 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::ControllerOptions(base, uint32_t(r31)).fault_step_started_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c6194
  if (!cr6.eq) goto loc_821C6194;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31600(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31600);
  // lwz r10,0(r11)
  r10 = schema::FrontEndMenu(base, uint32_t(r11)).cursor_bits();
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lwz r11,-31596(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -31596);
  // addi r9,r11,404
  r9 = uint64_t(int64_t(int64_t(r11) + 404));
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // addi r9,r11,408
  r9 = uint64_t(int64_t(int64_t(r11) + 408));
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // addi r9,r11,412
  r9 = uint64_t(int64_t(int64_t(r11) + 412));
  // stw r9,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r9));
  // addi r9,r11,420
  r9 = uint64_t(int64_t(int64_t(r11) + 420));
  // stw r9,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r9));
  // addi r9,r11,416
  r9 = uint64_t(int64_t(int64_t(r11) + 416));
  // stw r9,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r9));
  // addi r9,r11,424
  r9 = uint64_t(int64_t(int64_t(r11) + 424));
  // stw r9,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r9));
  // blt 0x821c6188
  if (cr0.lt) goto loc_821C6188;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // ble cr6,0x821c6154
  if (!cr6.gt) goto loc_821C6154;
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // ble cr6,0x821c6188
  if (!cr6.gt) goto loc_821C6188;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bgt cr6,0x821c6188
  if (cr6.gt) goto loc_821C6188;
loc_821C6154:
  // lwz r4,680(r11)
  r4 = guest::LoadU32(base, schema::OptionsSettings(base, uint32_t(r11)).p1_analog_side_address());
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r6,r10,r11
  r6 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // lwz r7,0(r6)
  r7 = guest::LoadU32(base, uint32_t(r6) + 0);
  // mr r3,r7
  r3 = r7;
  // bl 0x821bad80
  // (left out: dead in C++)
  r3 = game::Controls::CycleAnalog(base, r1, r3, r4);
  // cmpwi cr6,r7,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r7), -1, so);
  // beq cr6,0x821c6188
  if (cr6.eq) goto loc_821C6188;
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x821c6188
  if (cr6.eq) goto loc_821C6188;
  // stw r3,0(r6)
  guest::StoreU32(base, uint32_t(r6) + 0, uint32_t(r3));
  // bl 0x821c0640
  // (left out: dead in C++)
  { const lifted::Result o = game::ControllerOptions::ShowAnalogList(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821C6188:
  // bl 0x821c0750
  // (left out: dead in C++)
  { const lifted::Result o = game::ControllerOptions::ShowAnalogClashes(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::ControllerOptions(base, uint32_t(r31)).fault_step_started_address(), uint32_t(r11));
loc_821C6194:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821D7C90 game::TuningScreen::InputOptionRows: lifted from generated/ by lift.py.
lifted::Result f821D7C90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821D7C90 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -21596);
  // lwz r3,10040(r31)
  r3 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_menu_address());
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = game::Menu::Step(base, LIFTED_ARGS(0x821D7CC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_menu_address());
  // lwz r11,8(r11)
  r11 = schema::FrontEndMenu(base, uint32_t(r11)).moved_bits();
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821d811c
  if (cr6.eq) goto loc_821D811C;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821d811c
  if (cr6.eq) goto loc_821D811C;
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821d7f7c
  if (cr6.eq) goto loc_821D7F7C;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821D7D04, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821d7f7c
  if (cr0.eq) goto loc_821D7F7C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x821D7D14, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_address());
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821d7df4
  if (!cr6.eq) goto loc_821D7DF4;
  // lbz r11,9660(r31)
  r11 = guest::LoadU8(base, schema::TuningScreen(base, uint32_t(r31)).item_address());
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bne cr6,0x821d7df4
  if (!cr6.eq) goto loc_821D7DF4;
  // lbz r11,9663(r31)
  r11 = guest::LoadU8(base, schema::TuningScreen(base, uint32_t(r31)).bonnet_choice_address());
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x821d7df4
  if (!cr6.eq) goto loc_821D7DF4;
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_menu_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r7,8
  r7 = uint64_t(int64_t(8));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // lwz r6,0(r11)
  r6 = schema::FrontEndMenu(base, uint32_t(r11)).cursor_bits();
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).queue_address());
  // stw r7,9652(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).state_address(), uint32_t(r7));
  // stb r5,9680(r31)
  guest::StoreU8(base, schema::TuningScreen(base, uint32_t(r31)).option_count_address(), uint8_t(r5));
  // stb r10,9664(r31)
  guest::StoreU8(base, schema::TuningScreen(base, uint32_t(r31)).option_address(), uint8_t(r10));
  // stw r8,9708(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).refresh_pending_address(), uint32_t(r8));
  // stw r8,9712(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).bonnet_color_set_address(), uint32_t(r8));
  // stw r8,9716(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).command_first_run_address(), uint32_t(r8));
  // stw r6,9740(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).menu_category_address(), uint32_t(r6));
  // stb r6,9663(r31)
  guest::StoreU8(base, schema::TuningScreen(base, uint32_t(r31)).bonnet_choice_address(), uint8_t(r6));
  // stw r7,4(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).resume_address(), uint32_t(r7));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address(), uint32_t(r7));
  // stw r7,8(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).previous_page_address(), uint32_t(r7));
  // stw r9,20(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).current_address(), uint32_t(r9));
  // stw r10,24(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).started_address(), uint32_t(r10));
  // lbz r7,9680(r31)
  r7 = guest::LoadU8(base, schema::TuningScreen(base, uint32_t(r31)).option_count_address());
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_menu_address());
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r10,12(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_first_item_bits(uint32_t(r10));
  // stw r8,20(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_wraps_bits(uint32_t(r8));
  // stw r10,0(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_cursor_bits(uint32_t(r10));
  // stw r10,4(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_previous_bits(uint32_t(r10));
  // stw r10,8(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_moved_bits(uint32_t(r10));
  // stw r10,40(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_wrapped_bits(uint32_t(r10));
  // stw r7,16(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_last_item_bits(uint32_t(r7));
  // stw r10,48(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_visible_row_bits(uint32_t(r10));
  // stw r10,52(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_previous_row_bits(uint32_t(r10));
  // stw r10,36(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_side_bits(uint32_t(r10));
  // stw r10,64(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_player_bits(uint32_t(r10));
  // stw r10,24(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_up_down_locked_bits(uint32_t(r10));
  // stw r10,28(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_left_right_steps_bits(uint32_t(r10));
  // stw r10,32(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_shoulders_step_bits(uint32_t(r10));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).queue_address());
  // lwz r7,9652(r31)
  r7 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address());
  // cmpw cr6,r7,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r10), so);
  // beq cr6,0x821d7dec
  if (cr6.eq) goto loc_821D7DEC;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821D7DEC:
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // b 0x821d7f44
  goto loc_821D7F44;
loc_821D7DF4:
  // mr r3,r31
  r3 = r31;
  // bl 0x821cce30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CCE30, LIFTED_ARGS(0x821D7DFC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821d7e54
  if (cr0.eq) goto loc_821D7E54;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).queue_address());
  // li r7,11
  r7 = uint64_t(int64_t(11));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stb r10,9665(r31)
  guest::StoreU8(base, schema::TuningScreen(base, uint32_t(r31)).confirm_cursor_address(), uint8_t(r10));
  // stw r7,9652(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).state_address(), uint32_t(r7));
  // stw r7,4(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).resume_address(), uint32_t(r7));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address(), uint32_t(r7));
  // stw r7,8(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).previous_page_address(), uint32_t(r7));
  // stw r9,20(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).current_address(), uint32_t(r9));
  // stw r10,24(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).started_address(), uint32_t(r10));
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_menu_address());
  // stw r10,0(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_cursor_bits(uint32_t(r10));
  // stw r10,4(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_previous_bits(uint32_t(r10));
  // stw r10,40(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_wrapped_bits(uint32_t(r10));
  // stw r10,48(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_visible_row_bits(uint32_t(r10));
  // stw r10,52(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_previous_row_bits(uint32_t(r10));
  // stw r10,32(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_shoulders_step_bits(uint32_t(r10));
  // b 0x821d7f04
  goto loc_821D7F04;
loc_821D7E54:
  // mr r3,r31
  r3 = r31;
  // bl 0x821cd160
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CD160, LIFTED_ARGS(0x821D7E5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,25116(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).money_address());
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // cmpw cr6,r11,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r3), so);
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).queue_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // bge cr6,0x821d7ec0
  if (!cr6.lt) goto loc_821D7EC0;
  // li r7,10
  r7 = uint64_t(int64_t(10));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r7,9652(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).state_address(), uint32_t(r7));
  // stw r7,4(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).resume_address(), uint32_t(r7));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address(), uint32_t(r7));
  // stw r7,8(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).previous_page_address(), uint32_t(r7));
  // stw r9,20(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).current_address(), uint32_t(r9));
  // stw r10,24(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).started_address(), uint32_t(r10));
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_menu_address());
  // stw r10,12(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_first_item_bits(uint32_t(r10));
  // stw r10,16(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_last_item_bits(uint32_t(r10));
  // stw r10,0(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_cursor_bits(uint32_t(r10));
  // stw r10,4(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_previous_bits(uint32_t(r10));
  // stw r10,40(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_wrapped_bits(uint32_t(r10));
  // stw r10,48(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_visible_row_bits(uint32_t(r10));
  // stw r10,52(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_previous_row_bits(uint32_t(r10));
  // stw r10,32(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_shoulders_step_bits(uint32_t(r10));
  // b 0x821d7f10
  goto loc_821D7F10;
loc_821D7EC0:
  // li r7,9
  r7 = uint64_t(int64_t(9));
  // stb r10,9665(r31)
  guest::StoreU8(base, schema::TuningScreen(base, uint32_t(r31)).confirm_cursor_address(), uint8_t(r10));
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // stw r7,9652(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).state_address(), uint32_t(r7));
  // stw r7,4(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_previous_bits(uint32_t(r7));
  // stw r7,0(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_cursor_bits(uint32_t(r7));
  // stw r7,8(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_moved_bits(uint32_t(r7));
  // stw r9,20(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_wraps_bits(uint32_t(r9));
  // stw r9,12(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_first_item_bits(uint32_t(r9));
  // stw r10,24(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_up_down_locked_bits(uint32_t(r10));
  // lbz r7,9665(r31)
  r7 = guest::LoadU8(base, schema::TuningScreen(base, uint32_t(r31)).confirm_cursor_address());
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_menu_address());
  // stw r7,0(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_cursor_bits(uint32_t(r7));
  // stw r7,4(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_previous_bits(uint32_t(r7));
  // stw r10,44(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_list_wrapped_bits(uint32_t(r10));
  // stw r10,56(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_first_row_bits(uint32_t(r10));
  // stw r6,60(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_rows_shown_bits(uint32_t(r6));
loc_821D7F04:
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r10,12(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_first_item_bits(uint32_t(r10));
  // stw r8,16(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_last_item_bits(uint32_t(r8));
loc_821D7F10:
  // stw r8,20(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_wraps_bits(uint32_t(r8));
  // stw r10,8(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_moved_bits(uint32_t(r10));
  // stw r10,36(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_side_bits(uint32_t(r10));
  // stw r10,64(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_player_bits(uint32_t(r10));
  // stw r10,24(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_up_down_locked_bits(uint32_t(r10));
  // stw r10,28(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_left_right_steps_bits(uint32_t(r10));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).queue_address());
  // lwz r7,9652(r31)
  r7 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address());
  // cmpw cr6,r7,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r10), so);
  // beq cr6,0x821d7f40
  if (cr6.eq) goto loc_821D7F40;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821D7F40:
  // li r10,63
  r10 = uint64_t(int64_t(63));
loc_821D7F44:
  // stw r9,0(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r7,4(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).resume_address(), uint32_t(r7));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).next_address(), uint32_t(r9));
  // stw r10,12(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).current_address(), uint32_t(r10));
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(base, schema::TuningScreen(base, uint32_t(r31)).history_depth_address());
  // stw r8,9708(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).refresh_pending_address(), uint32_t(r8));
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r10,r11,r31
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r31), uint32_t(r10));
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(base, schema::TuningScreen(base, uint32_t(r31)).history_depth_address());
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stb r11,10008(r31)
  guest::StoreU8(base, schema::TuningScreen(base, uint32_t(r31)).history_depth_address(), uint8_t(r11));
  // b 0x821d813c
  goto loc_821D813C;
loc_821D7F7C:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821d813c
  if (cr6.eq) goto loc_821D813C;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,40(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821D7FA0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821d813c
  if (cr0.eq) goto loc_821D813C;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x821D7FB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,10008(r31)
  r11 = guest::LoadU8(base, schema::TuningScreen(base, uint32_t(r31)).history_depth_address());
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // addi r7,r11,255
  r7 = uint64_t(int64_t(int64_t(r11) + 255));
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).queue_address());
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // clrlwi r7,r7,24
  r7 = uint32_t(r7) & 0xFF;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // mr r6,r7
  r6 = r7;
  // stw r8,9708(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).refresh_pending_address(), uint32_t(r8));
  // mr r3,r31
  r3 = r31;
  // addi r6,r6,2439
  r6 = uint64_t(int64_t(int64_t(r6) + 2439));
  // stb r7,10008(r31)
  guest::StoreU8(base, schema::TuningScreen(base, uint32_t(r31)).history_depth_address(), uint8_t(r7));
  // rlwinm r6,r6,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 2) & 0xFFFFFFFC;
  // lwzx r7,r6,r31
  r7 = guest::LoadU32(base, uint32_t(r6) + uint32_t(r31));
  // stw r7,9652(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).state_address(), uint32_t(r7));
  // stw r9,20(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).step_address(), uint32_t(r9));
  // stw r9,12(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).current_address(), uint32_t(r9));
  // stw r10,24(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).started_address(), uint32_t(r10));
  // stw r7,4(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).resume_address(), uint32_t(r7));
  // stw r7,0(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address(), uint32_t(r7));
  // stw r7,8(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).previous_page_address(), uint32_t(r7));
  // bl 0x821ccdb0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CCDB0, LIFTED_ARGS(0x821D8008, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r7,r3
  r7 = r3;
  // mr r3,r31
  r3 = r31;
  // bl 0x821ccd38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CCD38, LIFTED_ARGS(0x821D8014, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,10040(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_menu_address());
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // stw r10,12(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_first_item_bits(uint32_t(r10));
  // stw r7,16(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_last_item_bits(uint32_t(r7));
  // stw r8,20(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_wraps_bits(uint32_t(r8));
  // stw r10,8(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_moved_bits(uint32_t(r10));
  // stw r3,0(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_cursor_bits(uint32_t(r3));
  // stw r3,4(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_previous_bits(uint32_t(r3));
  // stw r10,44(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_list_wrapped_bits(uint32_t(r10));
  // stw r10,56(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_first_row_bits(uint32_t(r10));
  // stw r6,60(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_rows_shown_bits(uint32_t(r6));
  // stw r10,36(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_side_bits(uint32_t(r10));
  // stw r10,64(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_player_bits(uint32_t(r10));
  // stw r10,24(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_up_down_locked_bits(uint32_t(r10));
  // stw r10,28(r11)
  schema::FrontEndMenu(base, uint32_t(r11)).set_left_right_steps_bits(uint32_t(r10));
  // lwz r11,9656(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_address());
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821d809c
  if (!cr6.eq) goto loc_821D809C;
  // lbz r10,9660(r31)
  r10 = guest::LoadU8(base, schema::TuningScreen(base, uint32_t(r31)).item_address());
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bne cr6,0x821d809c
  if (!cr6.eq) goto loc_821D809C;
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address());
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // beq cr6,0x821d8084
  if (cr6.eq) goto loc_821D8084;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821D8084:
  // li r10,3
  r10 = uint64_t(int64_t(3));
loc_821D8088:
  // stw r10,12(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).current_address(), uint32_t(r10));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).next_address(), uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).resume_address(), uint32_t(r8));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // b 0x821d813c
  goto loc_821D813C;
loc_821D809C:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821d80e8
  if (!cr6.eq) goto loc_821D80E8;
  // lbz r10,9660(r31)
  r10 = guest::LoadU8(base, schema::TuningScreen(base, uint32_t(r31)).item_address());
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // bne cr6,0x821d80e8
  if (!cr6.eq) goto loc_821D80E8;
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).queue_address());
  // lwz r10,9652(r31)
  r10 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).state_address());
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address());
  // cmpw cr6,r10,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r8), so);
  // beq cr6,0x821d80c8
  if (cr6.eq) goto loc_821D80C8;
  // stw r8,8(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).previous_page_address(), uint32_t(r8));
loc_821D80C8:
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r9,16(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).next_address(), uint32_t(r9));
  // li r7,10
  r7 = uint64_t(int64_t(10));
  // stw r10,4(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).resume_address(), uint32_t(r10));
  // stw r9,0(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address(), uint32_t(r9));
  // stw r8,12(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).current_address(), uint32_t(r8));
  // stw r7,10012(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).stand_action_address(), uint32_t(r7));
  // b 0x821d813c
  goto loc_821D813C;
loc_821D80E8:
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x821d813c
  if (!cr6.eq) goto loc_821D813C;
  // lbz r11,9660(r31)
  r11 = guest::LoadU8(base, schema::TuningScreen(base, uint32_t(r31)).item_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x821d813c
  if (!cr0.eq) goto loc_821D813C;
  // lwz r11,10036(r31)
  r11 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).queue_address());
  // lwz r8,9652(r31)
  r8 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).state_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).page_address());
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // beq cr6,0x821d8114
  if (cr6.eq) goto loc_821D8114;
  // stw r10,8(r11)
  guest::StoreU32(base, schema::TuningCommandQueue(base, uint32_t(r11)).previous_page_address(), uint32_t(r10));
loc_821D8114:
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // b 0x821d8088
  goto loc_821D8088;
loc_821D811C:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = game::MenuSound::Play(base, LIFTED_ARGS(0x821D8124, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r10,10040(r31)
  r10 = guest::LoadU32(base, schema::TuningScreen(base, uint32_t(r31)).category_menu_address());
  // stw r11,9708(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).refresh_pending_address(), uint32_t(r11));
  // lwz r11,0(r10)
  r11 = schema::FrontEndMenu(base, uint32_t(r10)).cursor_bits();
  // stw r11,9740(r31)
  guest::StoreU32(base, schema::TuningScreen(base, uint32_t(r31)).menu_category_address(), uint32_t(r11));
  // stb r11,9663(r31)
  guest::StoreU8(base, schema::TuningScreen(base, uint32_t(r31)).bonnet_choice_address(), uint8_t(r11));
loc_821D813C:
  // mr r3,r31
  r3 = r31;
  // bl 0x821d4540
  // (left out: dead in C++)
  { const lifted::Result o = game::TuningScreen::MoveOptionRow(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8222C300 game::LocationScreen::SetUp: lifted from generated/ by lift.py.
lifted::Result f8222C300(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_8222C300 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -4096);
  // ld r12,-8192(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -8192);
  // stwu r1,-11536(r1)
  ea = -11536 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(base, LIFTED_ARGS(0x8222C31C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32127
  r31 = uint64_t(int64_t(-2105475072));
  // mr r23,r3
  r23 = r3;
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r10,852(r11)
  r10 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11)).location_index_address());
  // lis r11,-32157
  r11 = uint64_t(int64_t(-2107441152));
  // addi r22,r11,25880
  r22 = uint64_t(int64_t(int64_t(r11) + 25880));
  // rlwinm r11,r10,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 4) & 0xFFFFFFF0;
  // lwzx r3,r11,r22
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r22));
  // bl 0x82150b10
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraAnims::Load(base, LIFTED_ARGS(0x8222C340, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r10,r22,4
  r10 = uint64_t(int64_t(int64_t(r22) + 4));
  // lwz r11,852(r11)
  r11 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11)).location_index_address());
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x82150b10
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraAnims::Load(base, LIFTED_ARGS(0x8222C358, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r10,r22,8
  r10 = uint64_t(int64_t(int64_t(r22) + 8));
  // lwz r11,852(r11)
  r11 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11)).location_index_address());
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // lwzx r3,r11,r10
  r3 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // bl 0x82150b10
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraAnims::Load(base, LIFTED_ARGS(0x8222C370, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r29,r23,2
  r29 = uint64_t(int64_t(int64_t(r23) + 131072));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // addi r29,r29,-14472
  r29 = uint64_t(int64_t(int64_t(r29) + -14472));
  // mr r27,r24
  r27 = r24;
  // lwz r11,768(r29)
  r11 = guest::LoadU32(base, schema::SaveData(base, uint32_t(r29) - 116600).unlocks_shown_count_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8222c460
  if (cr0.eq) goto loc_8222C460;
  // mr r28,r29
  r28 = r29;
loc_8222C390:
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, schema::SaveData(base, uint32_t(r28) - 116600).unlock_shown_word_address());
  // lwz r26,8(r28)
  r26 = guest::LoadU32(base, schema::SaveData(base, uint32_t(r28) - 116600).unlock_shown_entry_address());
  // lwz r25,4(r28)
  r25 = guest::LoadU32(base, schema::SaveData(base, uint32_t(r28) - 116600).unlock_shown_table_address());
  // stw r11,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r11));
loc_8222C3A0:
  // mr r4,r26
  r4 = r26;
  // mr r3,r25
  r3 = r25;
  // bl 0x82117190
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry56(base, LIFTED_ARGS(0x8222C3AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r8,16(r30)
  r8 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r30)).image_address());
  // cmpwi cr6,r8,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), -1, so);
  // beq cr6,0x8222c438
  if (cr6.eq) goto loc_8222C438;
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r11,r11,140
  r11 = uint64_t(int64_t(int64_t(r11) + 140));
  // lwz r10,128(r11)
  r10 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_count_a_address());
  // lwz r9,132(r11)
  r9 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_start_a_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,128(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_count_a_address(), uint32_t(r10));
  // rlwinm r9,r9,2,25,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x7C;
  // stwx r8,r9,r11
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r11), uint32_t(r8));
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r30)).image_address());
  // lwz r3,20048(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r4,r11,22696
  r4 = uint64_t(int64_t(int64_t(r11) + 22696));
  // bl 0x8222c270
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::LoadPicture(base, LIFTED_ARGS(0x8222C3F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r9,16(r30)
  r9 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r30)).image_address());
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r11,r11,408
  r11 = uint64_t(int64_t(int64_t(r11) + 408));
  // lwz r10,128(r11)
  r10 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_count_b_address());
  // lwz r8,132(r11)
  r8 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_start_b_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r8,r8,r10
  r8 = r8 + r10;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r10,128(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_count_b_address(), uint32_t(r10));
  // rlwinm r8,r8,2,25,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0x7C;
  // stwx r9,r8,r11
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r11), uint32_t(r9));
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r30)).image_address());
  // lwz r3,20048(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r4,r11,22741
  r4 = uint64_t(int64_t(int64_t(r11) + 22741));
  // bl 0x8222c270
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::LoadPicture(base, LIFTED_ARGS(0x8222C438, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222C438:
  // lwz r11,8(r30)
  r11 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r30)).next_page_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8222c44c
  if (cr0.eq) goto loc_8222C44C;
  // add r26,r11,r26
  r26 = r11 + r26;
  // b 0x8222c3a0
  goto loc_8222C3A0;
loc_8222C44C:
  // lwz r11,768(r29)
  r11 = guest::LoadU32(base, schema::SaveData(base, uint32_t(r29) - 116600).unlocks_shown_count_address());
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r28,r28,12
  r28 = uint64_t(int64_t(int64_t(r28) + 12));
  // cmplw cr6,r27,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), uint32_t(r11), so);
  // blt cr6,0x8222c390
  if (cr6.lt) goto loc_8222C390;
loc_8222C460:
  // lwz r30,20048(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r3,r1,160
  r3 = uint64_t(int64_t(int64_t(r1) + 160));
  // li r5,100
  r5 = uint64_t(int64_t(100));
  // lwz r11,852(r30)
  r11 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r30)).location_index_address());
  // mulli r11,r11,100
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(100))));
  // add r11,r11,r23
  r11 = r11 + r23;
  // addis r4,r11,2
  r4 = uint64_t(int64_t(int64_t(r11) + 131072));
  // addi r4,r4,-21216
  r4 = uint64_t(int64_t(int64_t(r4) + -21216));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8222C484, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,256(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 256);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8222c540
  if (cr6.eq) goto loc_8222C540;
  // addi r29,r1,164
  r29 = uint64_t(int64_t(int64_t(r1) + 164));
  // mr r28,r11
  r28 = r11;
loc_8222C498:
  // lwz r4,4(r29)
  r4 = guest::LoadU32(base, uint32_t(r29) + 4);
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 0);
  // bl 0x82117190
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry56(base, LIFTED_ARGS(0x8222C4A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r8,16(r30)
  r8 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r30)).image_address());
  // cmpwi cr6,r8,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), -1, so);
  // beq cr6,0x8222c530
  if (cr6.eq) goto loc_8222C530;
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r11,r11,140
  r11 = uint64_t(int64_t(int64_t(r11) + 140));
  // lwz r10,128(r11)
  r10 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_count_a_address());
  // lwz r9,132(r11)
  r9 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_start_a_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,128(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_count_a_address(), uint32_t(r10));
  // rlwinm r9,r9,2,25,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x7C;
  // stwx r8,r9,r11
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r11), uint32_t(r8));
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r30)).image_address());
  // lwz r3,20048(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r4,r11,22696
  r4 = uint64_t(int64_t(int64_t(r11) + 22696));
  // bl 0x8222c270
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::LoadPicture(base, LIFTED_ARGS(0x8222C4F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r9,16(r30)
  r9 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r30)).image_address());
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r11,r11,408
  r11 = uint64_t(int64_t(int64_t(r11) + 408));
  // lwz r10,128(r11)
  r10 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_count_b_address());
  // lwz r8,132(r11)
  r8 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_start_b_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r8,r8,r10
  r8 = r8 + r10;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r10,128(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_count_b_address(), uint32_t(r10));
  // rlwinm r8,r8,2,25,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0x7C;
  // stwx r9,r8,r11
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r11), uint32_t(r9));
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, schema::CareerEntry56(base, uint32_t(r30)).image_address());
  // lwz r3,20048(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r4,r11,22741
  r4 = uint64_t(int64_t(int64_t(r11) + 22741));
  // bl 0x8222c270
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::LoadPicture(base, LIFTED_ARGS(0x8222C530, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222C530:
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r29,r29,12
  r29 = uint64_t(int64_t(int64_t(r29) + 12));
  // bne 0x8222c498
  if (!cr0.eq) goto loc_8222C498;
  // lwz r30,20048(r31)
  r30 = guest::LoadU32(base, uint32_t(r31) + 20048);
loc_8222C540:
  // lwz r11,808(r30)
  r11 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r30)).opponent_count_address());
  // mr r28,r24
  r28 = r24;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8222c674
  if (!cr6.gt) goto loc_8222C674;
loc_8222C550:
  // stw r28,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r28));
  // bl 0x821434e8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceEvent::Current(base, LIFTED_ARGS(0x8222C558, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,804
  r4 = uint64_t(int64_t(int64_t(r11) + 804));
  // bl 0x821431c0
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceEvent::GroupItem(base, LIFTED_ARGS(0x8222C568, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,4176
  r3 = uint64_t(int64_t(int64_t(r1) + 4176));
  // li r5,3008
  r5 = uint64_t(int64_t(3008));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8222C578, r31)); r3 = o.r3; f1 = o.f1; }
  // lha r26,4176(r1)
  r26 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r1) + 4176))));
  // mr r27,r24
  r27 = r24;
loc_8222C580:
  // mr r4,r27
  r4 = r27;
  // mr r3,r26
  r3 = r26;
  // bl 0x821171b0
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Find20(base, LIFTED_ARGS(0x8222C58C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // bl 0x82117210
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry20(base, LIFTED_ARGS(0x8222C594, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).picture_address());
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x8222c654
  if (cr6.eq) goto loc_8222C654;
loc_8222C5A4:
  // lwz r6,16(r30)
  r6 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).picture_address());
  // mr r3,r6
  r3 = r6;
  // bl 0x822291e8
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::IsPictureQueued(base, LIFTED_ARGS(0x8222C5B0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8222c634
  if (!cr0.eq) goto loc_8222C634;
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r11,r11,140
  r11 = uint64_t(int64_t(int64_t(r11) + 140));
  // lwz r10,128(r11)
  r10 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_count_a_address());
  // lwz r9,132(r11)
  r9 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_start_a_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,128(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_count_a_address(), uint32_t(r10));
  // rlwinm r9,r9,2,25,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x7C;
  // stwx r6,r9,r11
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r11), uint32_t(r6));
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).picture_address());
  // lwz r3,20048(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r4,r11,22696
  r4 = uint64_t(int64_t(int64_t(r11) + 22696));
  // bl 0x8222c270
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::LoadPicture(base, LIFTED_ARGS(0x8222C5F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r9,16(r30)
  r9 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).picture_address());
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r11,r11,408
  r11 = uint64_t(int64_t(int64_t(r11) + 408));
  // lwz r10,128(r11)
  r10 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_count_b_address());
  // lwz r8,132(r11)
  r8 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_start_b_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r8,r8,r10
  r8 = r8 + r10;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r10,128(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_count_b_address(), uint32_t(r10));
  // rlwinm r8,r8,2,25,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0x7C;
  // stwx r9,r8,r11
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r11), uint32_t(r9));
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).picture_address());
  // lwz r3,20048(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r4,r11,22741
  r4 = uint64_t(int64_t(int64_t(r11) + 22741));
  // bl 0x8222c270
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::LoadPicture(base, LIFTED_ARGS(0x8222C634, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222C634:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).next_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8222c654
  if (cr0.eq) goto loc_8222C654;
  // add r29,r11,r29
  r29 = r11 + r29;
  // mr r3,r29
  r3 = r29;
  // bl 0x82117210
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry20(base, LIFTED_ARGS(0x8222C64C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // b 0x8222c5a4
  goto loc_8222C5A4;
loc_8222C654:
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // cmplwi cr6,r27,13
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 13, so);
  // blt cr6,0x8222c580
  if (cr6.lt) goto loc_8222C580;
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // lwz r11,808(r11)
  r11 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11)).opponent_count_address());
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // blt cr6,0x8222c550
  if (cr6.lt) goto loc_8222C550;
loc_8222C674:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 23340);
  // addis r30,r11,6
  r30 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r30,r30,-25880
  r30 = uint64_t(int64_t(int64_t(r30) + -25880));
  // mr r3,r30
  r3 = r30;
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = game::RacerRecords::At(base, LIFTED_ARGS(0x8222C690, r31)); r3 = o.r3; f1 = o.f1; }
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // mr. r29,r3
  r29 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq 0x8222c7f0
  if (cr0.eq) goto loc_8222C7F0;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x820ffe58
  // (left out: dead in C++)
  { const lifted::Result o = game::RacerRecords::At(base, LIFTED_ARGS(0x8222C6A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r28,980(r3)
  r28 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r3)).e1_item_address());
  // mr r3,r28
  r3 = r28;
  // bl 0x82117178
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Item(base, LIFTED_ARGS(0x8222C6B4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,4176
  r3 = uint64_t(int64_t(int64_t(r1) + 4176));
  // li r5,3008
  r5 = uint64_t(int64_t(3008));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8222C6C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,4264(r1)
  r11 = guest::LoadU16(base, uint32_t(r1) + 4264);
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bne cr6,0x8222c6d8
  if (!cr6.eq) goto loc_8222C6D8;
  // lwz r11,980(r29)
  r11 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r29)).e1_item_address());
  // b 0x8222c6dc
  goto loc_8222C6DC;
loc_8222C6D8:
  // mr r11,r28
  r11 = r28;
loc_8222C6DC:
  // lwz r10,3924(r29)
  r10 = guest::LoadU32(base, schema::RacerResultsRecord(base, uint32_t(r29)).finish_address());
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x8222c6f0
  if (cr6.eq) goto loc_8222C6F0;
  // li r29,12
  r29 = uint64_t(int64_t(12));
  // b 0x8222c718
  goto loc_8222C718;
loc_8222C6F0:
  // mulli r11,r11,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // add r11,r11,r23
  r11 = r11 + r23;
  // ori r10,r10,39418
  r10 = r10 | 39418;
  // lhax r11,r11,r10
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r11) + uint32_t(r10)))));
  // subf r11,r27,r11
  r11 = r11 - r27;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // addi r29,r11,10
  r29 = uint64_t(int64_t(int64_t(r11) + 10));
loc_8222C718:
  // mr r4,r29
  r4 = r29;
  // mr r3,r28
  r3 = r28;
  // bl 0x821171b0
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Find20(base, LIFTED_ARGS(0x8222C724, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r11,872(r11)
  r11 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11)).entry_offset_address());
  // add r3,r11,r3
  r3 = r11 + r3;
  // bl 0x82117210
  // (left out: dead in C++)
  { const lifted::Result o = game::DataTables::Entry20(base, LIFTED_ARGS(0x8222C734, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r6,16(r30)
  r6 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).picture_address());
  // mr r3,r6
  r3 = r6;
  // bl 0x822291e8
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::IsPictureQueued(base, LIFTED_ARGS(0x8222C744, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x8222c7d0
  if (!cr0.eq) goto loc_8222C7D0;
  // cmpwi cr6,r6,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r6), -1, so);
  // beq cr6,0x8222c7d0
  if (cr6.eq) goto loc_8222C7D0;
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r11,r11,140
  r11 = uint64_t(int64_t(int64_t(r11) + 140));
  // lwz r10,128(r11)
  r10 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_count_a_address());
  // lwz r9,132(r11)
  r9 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_start_a_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,128(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11) - 140).queued_count_a_address(), uint32_t(r10));
  // rlwinm r9,r9,2,25,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x7C;
  // stwx r6,r9,r11
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r11), uint32_t(r6));
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).picture_address());
  // lwz r3,20048(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r4,r11,22696
  r4 = uint64_t(int64_t(int64_t(r11) + 22696));
  // bl 0x8222c270
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::LoadPicture(base, LIFTED_ARGS(0x8222C790, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r9,16(r30)
  r9 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).picture_address());
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r11,r11,408
  r11 = uint64_t(int64_t(int64_t(r11) + 408));
  // lwz r10,128(r11)
  r10 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_count_b_address());
  // lwz r8,132(r11)
  r8 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_start_b_address());
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r8,r8,r10
  r8 = r8 + r10;
  // addi r8,r8,-1
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  // stw r10,128(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11) - 408).queued_count_b_address(), uint32_t(r10));
  // rlwinm r8,r8,2,25,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0x7C;
  // stwx r9,r8,r11
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r11), uint32_t(r9));
  // lwz r11,16(r30)
  r11 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).picture_address());
  // lwz r3,20048(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r4,r11,22741
  r4 = uint64_t(int64_t(int64_t(r11) + 22741));
  // bl 0x8222c270
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::LoadPicture(base, LIFTED_ARGS(0x8222C7D0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8222C7D0:
  // lwz r10,4(r30)
  r10 = guest::LoadU32(base, schema::LocationScreenEntry20(base, uint32_t(r30)).next_address());
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8222c7f0
  if (cr0.eq) goto loc_8222C7F0;
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r9,872(r11)
  r9 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11)).entry_offset_address());
  // add r10,r9,r10
  r10 = r9 + r10;
  // stw r10,872(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11)).entry_offset_address(), uint32_t(r10));
  // b 0x8222c718
  goto loc_8222C718;
loc_8222C7F0:
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // mr r30,r24
  r30 = r24;
  // stw r24,876(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11)).entry_table_address(), uint32_t(r24));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // stw r24,872(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11)).entry_offset_address(), uint32_t(r24));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r28,11844(r11)
  r28 = guest::LoadU32(base, uint32_t(r11) + 11844);
  // stw r27,5052(r28)
  guest::StoreU32(base, schema::CarScene(base, uint32_t(r28)).lighting_fixed_address(), uint32_t(r27));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r11,808(r11)
  r11 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11)).opponent_count_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8222c8e4
  if (!cr6.gt) goto loc_8222C8E4;
  // li r29,824
  r29 = uint64_t(int64_t(824));
loc_8222C824:
  // stw r30,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r30));
  // bl 0x821434e8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceEvent::Current(base, LIFTED_ARGS(0x8222C82C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,804
  r4 = uint64_t(int64_t(int64_t(r11) + 804));
  // bl 0x821431c0
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceEvent::GroupItem(base, LIFTED_ARGS(0x8222C83C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,4176
  r3 = uint64_t(int64_t(int64_t(r1) + 4176));
  // li r5,3008
  r5 = uint64_t(int64_t(3008));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8222C84C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,7184
  r3 = uint64_t(int64_t(int64_t(r1) + 7184));
  // bl 0x820db240
  // (left out: dead in C++)
  { const lifted::Result o = game::SceneModelRecord::Ctor(base, LIFTED_ARGS(0x8222C854, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,7184
  r3 = uint64_t(int64_t(int64_t(r1) + 7184));
  // bl 0x820d9810
  // (left out: dead in C++)
  { const lifted::Result o = game::SceneModelRecord::Reset(base, LIFTED_ARGS(0x8222C85C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,272
  r3 = uint64_t(int64_t(int64_t(r1) + 272));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x8222C864, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,4336
  r4 = uint64_t(int64_t(int64_t(r1) + 4336));
  // addi r3,r1,272
  r3 = uint64_t(int64_t(int64_t(r1) + 272));
  // bl 0x820cdce0
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::FromBuild(base, LIFTED_ARGS(0x8222C870, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4160(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 4160);
  // addi r3,r1,272
  r3 = uint64_t(int64_t(int64_t(r1) + 272));
  // ori r11,r11,2
  r11 = r11 | 2;
  // stw r11,4160(r1)
  guest::StoreU32(base, uint32_t(r1) + 4160, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(base, LIFTED_ARGS(0x8222C884, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,272
  r4 = uint64_t(int64_t(int64_t(r1) + 272));
  // addi r3,r1,7184
  r3 = uint64_t(int64_t(int64_t(r1) + 7184));
  // lha r5,272(r1)
  r5 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r1) + 272))));
  // bl 0x8215b878
  // (left out: dead in C++)
  { const lifted::Result o = game::SceneModelRecord::FromCar(base, LIFTED_ARGS(0x8222C894, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r28
  r3 = r28;
  // bl 0x820c5ab0
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::ClaimB(base, LIFTED_ARGS(0x8222C8A4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,20048(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // mr r11,r3
  r11 = r3;
  // addi r6,r1,7184
  r6 = uint64_t(int64_t(int64_t(r1) + 7184));
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r28
  r3 = r28;
  // stwx r11,r29,r10
  guest::StoreU32(base, uint32_t(r29) + uint32_t(r10), uint32_t(r11));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwzx r5,r29,r11
  r5 = guest::LoadU32(base, uint32_t(r29) + uint32_t(r11));
  // bl 0x820c5b28
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::SetUpB(base, LIFTED_ARGS(0x8222C8C8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r27,4296(r3)
  schema::SceneModel(base, uint32_t(r3)).set_draw_enabled_bits(uint32_t(r27));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // lwz r11,808(r11)
  r11 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11)).opponent_count_address());
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8222c824
  if (cr6.lt) goto loc_8222C824;
loc_8222C8E4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(base, LIFTED_ARGS(0x8222C8EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,20048(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // mr r11,r3
  r11 = r3;
  // addi r9,r22,12
  r9 = uint64_t(int64_t(int64_t(r22) + 12));
  // lwz r10,852(r10)
  r10 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r10)).location_index_address());
  // lwz r4,6104(r11)
  r4 = guest::LoadU32(base, schema::SaveData(base, uint32_t(r11)).mode_address());
  // rlwinm r10,r10,4,0,27
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 4) & 0xFFFFFFF0;
  // lwzx r3,r10,r9
  r3 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // bl 0x8228f0a0
  // (left out: dead in C++)
  { const lifted::Result o = game::LocationScreen::SceneryConfig(base, LIFTED_ARGS(0x8222C90C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // stw r24,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r24));
  // rlwinm r10,r3,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 3) & 0xFFFFFFF8;
  // stw r24,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r24));
  // addi r11,r11,11936
  r11 = uint64_t(int64_t(int64_t(r11) + 11936));
  // stw r24,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r24));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r24,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r24));
  // addi r9,r11,4
  r9 = uint64_t(int64_t(int64_t(r11) + 4));
  // stw r24,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r24));
  // stw r24,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r24));
  // stw r24,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r24));
  // lwzx r11,r10,r11
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  // stw r24,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r24));
  // stw r11,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r11));
  // lwzx r11,r10,r9
  r11 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r9));
  // stw r11,124(r1)
  guest::StoreU32(base, uint32_t(r1) + 124, uint32_t(r11));
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r3,552(r11)
  r3 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11)).scenery_address());
  // bl 0x820f3020
  // (left out: dead in C++)
  { const lifted::Result o = game::Scenery::Setup(base, LIFTED_ARGS(0x8222C95C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // lwz r3,552(r11)
  r3 = guest::LoadU32(base, schema::LocationScreen(base, uint32_t(r11)).scenery_address());
  // bl 0x820f1e00
  // (left out: dead in C++)
  { const lifted::Result o = game::Scenery::StartLoad(base, LIFTED_ARGS(0x8222C968, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,22563
  r4 = uint64_t(int64_t(22563));
  // li r3,21
  r3 = uint64_t(int64_t(21));
  // bl 0x8211e808
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Open(base, LIFTED_ARGS(0x8222C974, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,22573
  r4 = uint64_t(int64_t(22573));
  // li r3,30
  r3 = uint64_t(int64_t(30));
  // bl 0x8211e808
  // (left out: dead in C++)
  { const lifted::Result o = game::UiMovies::Open(base, LIFTED_ARGS(0x8222C980, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,7148(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 7148);
  // bl 0x8209d038
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenFades::FadeIn(base, LIFTED_ARGS(0x8222C99C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20048(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20048);
  // stw r24,896(r11)
  guest::StoreU32(base, schema::LocationScreen(base, uint32_t(r11)).load_faded_address(), uint32_t(r24));
  // addi r1,r1,11536
  r1 = uint64_t(int64_t(int64_t(r1) + 11536));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_823EACC8 game::ImageCodec::QuantizeColorKey: lifted from generated/ by lift.py.
lifted::Result f823EACC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_823EACC8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r10,-32156
  r10 = uint64_t(int64_t(-2107375616));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // addi r10,r10,-14488
  r10 = uint64_t(int64_t(int64_t(r10) + -14488));
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r8,120(r31)
  r8 = schema::ImageCodec(base, uint32_t(r31)).bytes_per_pixel_bits();
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r11,8(r31)
  r11 = schema::ImageCodec(base, uint32_t(r31)).kind_bits();
  // lwz r29,104(r31)
  r29 = schema::ImageCodec(base, uint32_t(r31)).width_bits();
  // lwz r28,116(r31)
  r28 = schema::ImageCodec(base, uint32_t(r31)).row_bytes_bits();
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // lwz r27,32(r31)
  r27 = schema::ImageCodec(base, uint32_t(r31)).surface_bits();
  // lwz r26,52(r31)
  r26 = schema::ImageCodec(base, uint32_t(r31)).rounding_bits();
  // lwz r25,24(r31)
  r25 = schema::ImageCodec(base, uint32_t(r31)).color_key_bits();
  // stw r9,104(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_width_bits(uint32_t(r9));
  // stw r8,116(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_row_bytes_bits(uint32_t(r8));
  // stw r7,32(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_surface_bits(uint32_t(r7));
  // stw r10,52(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_rounding_bits(uint32_t(r10));
  // stw r6,24(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_color_key_bits(uint32_t(r6));
  // beq cr6,0x823ead4c
  if (cr6.eq) goto loc_823EAD4C;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // beq cr6,0x823ead4c
  if (cr6.eq) goto loc_823EAD4C;
  // addi r4,r31,36
  r4 = uint64_t(int64_t(int64_t(r31) + 36));
  // lwz r5,84(r31)
  r5 = schema::ImageCodec(base, uint32_t(r31)).convert_from_bits();
  // lwz r30,88(r31)
  r30 = schema::ImageCodec(base, uint32_t(r31)).row_bits();
  // stw r9,84(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_convert_from_bits(uint32_t(r9));
  // stw r4,88(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_row_bits(uint32_t(r4));
  // bl 0x823ea0c0
  // (left out: dead in C++)
  r3 = game::ImageCodec::ConvertRange(base, r3, r4);
  // stw r5,84(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_convert_from_bits(uint32_t(r5));
  // stw r30,88(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_row_bits(uint32_t(r30));
loc_823EAD4C:
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // addi r30,r31,36
  r30 = uint64_t(int64_t(int64_t(r31) + 36));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // mr r6,r30
  r6 = r30;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).encode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x823EAD70, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = lifted::VtableWord(base, uint32_t(r31));
  // mr r6,r30
  r6 = r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::ImageCodecVtable(base, uint32_t(r11)).decode_row_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x823EAD90, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,104(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_width_bits(uint32_t(r29));
  // stw r28,116(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_row_bytes_bits(uint32_t(r28));
  // stw r27,32(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_surface_bits(uint32_t(r27));
  // stw r26,52(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_rounding_bits(uint32_t(r26));
  // stw r25,24(r31)
  schema::ImageCodec(base, uint32_t(r31)).set_color_key_bits(uint32_t(r25));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_823F2730 game::X8R8G8B8Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F2730(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F2730 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f27e4
  if (!cr6.lt) goto loc_823F27E4;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f13,2608(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f13 = double(temp.f32);
  // lfs f0,8704(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8704);
  f0 = double(temp.f32);
loc_823F277C:
  // lbz r9,1(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 1);
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f12,80(r1)
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lbz r9,2(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 2);
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,4(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lbz r9,3(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 3);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f12,96(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,8(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f277c
  if (cr6.lt) goto loc_823F277C;
loc_823F27E4:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2800
  if (cr6.eq) goto loc_823F2800;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F2800:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f281c
  if (cr6.eq) goto loc_823F281C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F281C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F2830 game::R5G6B5Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F2830(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F2830 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f2900
  if (!cr6.lt) goto loc_823F2900;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r8,-32252
  r8 = uint64_t(int64_t(-2113667072));
  // lis r9,-32252
  r9 = uint64_t(int64_t(-2113667072));
  // lfs f12,2608(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 2608);
  f12 = double(temp.f32);
  // lfs f13,-18516(r8)
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + -18516);
  f13 = double(temp.f32);
  // lfs f0,-18520(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + -18520);
  f0 = double(temp.f32);
loc_823F2884:
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,21,11,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 21) & 0x1FFFFF;
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f11,80(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r6)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,27,26,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x3F;
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f13
  f11 = double(float(f11 * f13));
  // stfs f11,4(r6)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // stfs f12,12(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // clrlwi r9,r9,27
  r9 = uint32_t(r9) & 0x1F;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f11,96(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,8(r6)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f2884
  if (cr6.lt) goto loc_823F2884;
loc_823F2900:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f291c
  if (cr6.eq) goto loc_823F291C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F291C:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2938
  if (cr6.eq) goto loc_823F2938;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F2938:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F2948 game::X1R5G5B5Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F2948(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F2948 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f2a10
  if (!cr6.lt) goto loc_823F2A10;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32252
  r9 = uint64_t(int64_t(-2113667072));
  // lfs f13,2608(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f13 = double(temp.f32);
  // lfs f0,-18520(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + -18520);
  f0 = double(temp.f32);
loc_823F2994:
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,22,27,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 22) & 0x1F;
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f12,80(r1)
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,27,27,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1F;
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,4(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // clrlwi r9,r9,27
  r9 = uint32_t(r9) & 0x1F;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f12,96(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,8(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f2994
  if (cr6.lt) goto loc_823F2994;
loc_823F2A10:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2a2c
  if (cr6.eq) goto loc_823F2A2C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F2A2C:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2a48
  if (cr6.eq) goto loc_823F2A48;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F2A48:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F2A58 game::A1R5G5B5Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F2A58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F2A58 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f2b34
  if (!cr6.lt) goto loc_823F2B34;
  // lis r9,-32252
  r9 = uint64_t(int64_t(-2113667072));
  // lfs f0,-18520(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + -18520);
  f0 = double(temp.f32);
loc_823F2A9C:
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,22,27,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 22) & 0x1F;
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f13,80(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,27,27,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1F;
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,4(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // clrlwi r9,r9,27
  r9 = uint32_t(r9) & 0x1F;
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f13,96(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,8(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // rlwinm r9,r9,17,15,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 17) & 0x1FFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // std r9,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r9);
  // lfd f13,104(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f2a9c
  if (cr6.lt) goto loc_823F2A9C;
loc_823F2B34:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2b50
  if (cr6.eq) goto loc_823F2B50;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F2B50:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2b6c
  if (cr6.eq) goto loc_823F2B6C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F2B6C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F2B80 game::A4R4G4B4Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F2B80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F2B80 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f2c5c
  if (!cr6.lt) goto loc_823F2C5C;
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // lfs f0,-32672(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + -32672);
  f0 = double(temp.f32);
loc_823F2BC4:
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // clrlwi r9,r9,28
  r9 = uint32_t(r9) & 0xF;
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f13,80(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,28,28,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 28) & 0xF;
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,4(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // clrlwi r9,r9,28
  r9 = uint32_t(r9) & 0xF;
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f13,96(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,8(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // rlwinm r9,r9,20,12,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 20) & 0xFFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // std r9,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r9);
  // lfd f13,104(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f2bc4
  if (cr6.lt) goto loc_823F2BC4;
loc_823F2C5C:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2c78
  if (cr6.eq) goto loc_823F2C78;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F2C78:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2c94
  if (cr6.eq) goto loc_823F2C94;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F2C94:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F2CA8 game::A2B10G10R10Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F2CA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F2CA8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f2d9c
  if (!cr6.lt) goto loc_823F2D9C;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // lfs f13,10808(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 10808);
  f13 = double(temp.f32);
  // lfs f0,17360(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 17360);
  f0 = double(temp.f32);
loc_823F2CF4:
  // lis r12,-64
  r12 = uint64_t(int64_t(-4194304));
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // ori r12,r12,0
  r12 = r12 | 0;
  // rldicr r12,r12,10,63
  r12 = __builtin_rotateleft64(r12, 10) & 0xFFFFFFFFFFFFFFFF;
  // and r9,r9,r12
  r9 = r9 & r12;
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f12,80(r1)
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,22,22,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 22) & 0x3FF;
  // clrldi r9,r9,32
  r9 = r9 & 0xFFFFFFFF;
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,4(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,12,22,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 12) & 0x3FF;
  // clrldi r9,r9,32
  r9 = r9 & 0xFFFFFFFF;
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f12,96(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,8(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // rlwinm r9,r9,2,30,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x3;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // clrldi r9,r9,32
  r9 = r9 & 0xFFFFFFFF;
  // std r9,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r9);
  // lfd f12,104(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfs f12,12(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f2cf4
  if (cr6.lt) goto loc_823F2CF4;
loc_823F2D9C:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2db8
  if (cr6.eq) goto loc_823F2DB8;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F2DB8:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2dd4
  if (cr6.eq) goto loc_823F2DD4;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F2DD4:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F2DE8 game::A8B8G8R8Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F2DE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F2DE8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f2eac
  if (!cr6.lt) goto loc_823F2EAC;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f0,8704(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8704);
  f0 = double(temp.f32);
loc_823F2E2C:
  // lbz r9,3(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 3);
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f13,80(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lbz r9,2(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 2);
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,4(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lbz r9,1(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 1);
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f13,96(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,8(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r9);
  // lfd f13,104(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f2e2c
  if (cr6.lt) goto loc_823F2E2C;
loc_823F2EAC:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2ec8
  if (cr6.eq) goto loc_823F2EC8;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F2EC8:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2ee4
  if (cr6.eq) goto loc_823F2EE4;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F2EE4:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F2EF8 game::X8B8G8R8Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F2EF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F2EF8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f2fac
  if (!cr6.lt) goto loc_823F2FAC;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f13,2608(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f13 = double(temp.f32);
  // lfs f0,8704(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8704);
  f0 = double(temp.f32);
loc_823F2F44:
  // lbz r9,3(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 3);
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f12,80(r1)
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lbz r9,2(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 2);
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,4(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lbz r9,1(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 1);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f12,96(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,8(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f2f44
  if (cr6.lt) goto loc_823F2F44;
loc_823F2FAC:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2fc8
  if (cr6.eq) goto loc_823F2FC8;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F2FC8:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f2fe4
  if (cr6.eq) goto loc_823F2FE4;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F2FE4:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F2FF8 game::G16R16Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F2FF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F2FF8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f309c
  if (!cr6.lt) goto loc_823F309C;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32253
  r9 = uint64_t(int64_t(-2113732608));
  // lfs f13,2608(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f13 = double(temp.f32);
  // lfs f0,29924(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 29924);
  f0 = double(temp.f32);
loc_823F3044:
  // lhz r9,2(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 2);
  // extsw r9,r9
  r9 = uint64_t(int64_t(int32_t(r9)));
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f12,80(r1)
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stfs f13,8(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // extsw r9,r9
  r9 = uint64_t(int64_t(int32_t(r9)));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,4(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f3044
  if (cr6.lt) goto loc_823F3044;
loc_823F309C:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f30b8
  if (cr6.eq) goto loc_823F30B8;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F30B8:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f30d4
  if (cr6.eq) goto loc_823F30D4;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F30D4:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F30E8 game::A2R10G10B10Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F30E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F30E8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f31dc
  if (!cr6.lt) goto loc_823F31DC;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // lfs f13,10808(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 10808);
  f13 = double(temp.f32);
  // lfs f0,17360(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 17360);
  f0 = double(temp.f32);
loc_823F3134:
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lis r12,-64
  r12 = uint64_t(int64_t(-4194304));
  // rlwinm r9,r9,12,22,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 12) & 0x3FF;
  // ori r12,r12,0
  r12 = r12 | 0;
  // clrldi r9,r9,32
  r9 = r9 & 0xFFFFFFFF;
  // rldicr r12,r12,10,63
  r12 = __builtin_rotateleft64(r12, 10) & 0xFFFFFFFFFFFFFFFF;
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f12,80(r1)
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,22,22,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 22) & 0x3FF;
  // clrldi r9,r9,32
  r9 = r9 & 0xFFFFFFFF;
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,4(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // and r9,r9,r12
  r9 = r9 & r12;
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f12,96(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,8(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // rlwinm r9,r9,2,30,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x3;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // clrldi r9,r9,32
  r9 = r9 & 0xFFFFFFFF;
  // std r9,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r9);
  // lfd f12,104(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfs f12,12(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f3134
  if (cr6.lt) goto loc_823F3134;
loc_823F31DC:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f31f8
  if (cr6.eq) goto loc_823F31F8;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F31F8:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3214
  if (cr6.eq) goto loc_823F3214;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F3214:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F3228 game::A16B16G16R16Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F3228(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F3228 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f3300
  if (!cr6.lt) goto loc_823F3300;
  // lis r9,-32253
  r9 = uint64_t(int64_t(-2113732608));
  // lfs f0,29924(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 29924);
  f0 = double(temp.f32);
loc_823F326C:
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // mr r8,r11
  r8 = r11;
  // clrldi r9,r9,48
  r9 = r9 & 0xFFFF;
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f13,80(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // rldicl r9,r9,48,48
  r9 = __builtin_rotateleft64(r9, 48) & 0xFFFF;
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,4(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // ld r9,0(r11)
  r9 = guest::LoadU64(base, uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // rldicl r9,r9,32,48
  r9 = __builtin_rotateleft64(r9, 32) & 0xFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f13,96(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,8(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // lhz r9,0(r8)
  r9 = guest::LoadU16(base, uint32_t(r8) + 0);
  // extsw r9,r9
  r9 = uint64_t(int64_t(int32_t(r9)));
  // std r9,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r9);
  // lfd f13,104(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f326c
  if (cr6.lt) goto loc_823F326C;
loc_823F3300:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f331c
  if (cr6.eq) goto loc_823F331C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F331C:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3338
  if (cr6.eq) goto loc_823F3338;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F3338:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F3348 game::A8Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F3348(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F3348 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f33c8
  if (!cr6.lt) goto loc_823F33C8;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f13,8704(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 8704);
  f13 = double(temp.f32);
  // lfs f0,2612(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2612);
  f0 = double(temp.f32);
loc_823F3390:
  // stfs f0,0(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // stfs f0,4(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // stfs f0,8(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfs f12,12(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f3390
  if (cr6.lt) goto loc_823F3390;
loc_823F33C8:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f33e4
  if (cr6.eq) goto loc_823F33E4;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F33E4:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3400
  if (cr6.eq) goto loc_823F3400;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F3400:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F3410 game::X4R4G4B4Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F3410(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F3410 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f34d4
  if (!cr6.lt) goto loc_823F34D4;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // lfs f13,2608(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f13 = double(temp.f32);
  // lfs f0,-32672(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + -32672);
  f0 = double(temp.f32);
loc_823F345C:
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // clrlwi r9,r9,28
  r9 = uint32_t(r9) & 0xF;
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f12,80(r1)
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // rlwinm r9,r9,28,28,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 28) & 0xF;
  // clrlwi r9,r9,16
  r9 = uint32_t(r9) & 0xFFFF;
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,4(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // clrlwi r9,r9,28
  r9 = uint32_t(r9) & 0xF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r9);
  // lfd f12,96(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,8(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f345c
  if (cr6.lt) goto loc_823F345C;
loc_823F34D4:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f34f0
  if (cr6.eq) goto loc_823F34F0;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F34F0:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f350c
  if (cr6.eq) goto loc_823F350C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F350C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F3520 game::L8Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F3520(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F3520 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f35a0
  if (!cr6.lt) goto loc_823F35A0;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f12,2608(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f12 = double(temp.f32);
  // lfs f13,8704(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8704);
  f13 = double(temp.f32);
loc_823F3568:
  // lbz r9,0(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stfs f12,12(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,8(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // stfs f0,4(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f3568
  if (cr6.lt) goto loc_823F3568;
loc_823F35A0:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f35bc
  if (cr6.eq) goto loc_823F35BC;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F35BC:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f35d8
  if (cr6.eq) goto loc_823F35D8;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F35D8:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F35E8 game::A8L8Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F35E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F35E8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f3680
  if (!cr6.lt) goto loc_823F3680;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f13,8704(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 8704);
  f13 = double(temp.f32);
loc_823F362C:
  // lbz r9,1(r11)
  r9 = guest::LoadU8(base, uint32_t(r11) + 1);
  // mr r8,r11
  r8 = r11;
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,8(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // stfs f0,4(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lbz r9,0(r8)
  r9 = guest::LoadU8(base, uint32_t(r8) + 0);
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f0,88(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,12(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f362c
  if (cr6.lt) goto loc_823F362C;
loc_823F3680:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f369c
  if (cr6.eq) goto loc_823F369C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F369C:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f36b8
  if (cr6.eq) goto loc_823F36B8;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F36B8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F36C8 game::V8U8Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F36C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F36C8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r8,r10,r11
  r8 = r10 + r11;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bge cr6,0x823f379c
  if (!cr6.lt) goto loc_823F379C;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32252
  r10 = uint64_t(int64_t(-2113667072));
  // li r4,-128
  r4 = uint64_t(int64_t(-128));
  // lfs f13,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f13 = double(temp.f32);
  // lfs f0,-18492(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -18492);
  f0 = double(temp.f32);
loc_823F371C:
  // lhz r10,0(r11)
  r10 = guest::LoadU16(base, uint32_t(r11) + 0);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // stfs f13,8(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // rlwinm r9,r10,24,8,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFFFFFF;
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // extsb r9,r9
  r9 = uint64_t(int64_t(int8_t(r9)));
  // subf r7,r4,r10
  r7 = r10 - r4;
  // subf r5,r4,r9
  r5 = r9 - r4;
  // cntlzw r7,r7
  r7 = uint32_t(r7) == 0 ? 32 : __builtin_clz(uint32_t(r7));
  // cntlzw r31,r5
  r31 = uint32_t(r5) == 0 ? 32 : __builtin_clz(uint32_t(r5));
  // rlwinm r5,r7,27,31,31
  r5 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 27) & 0x1;
  // rlwinm r7,r31,27,31,31
  r7 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 27) & 0x1;
  // add r10,r5,r10
  r10 = r5 + r10;
  // add r9,r7,r9
  r9 = r7 + r9;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // extsb r9,r9
  r9 = uint64_t(int64_t(int8_t(r9)));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // fmuls f12,f11,f0
  f12 = double(float(f11 * f0));
  // stfs f12,4(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f371c
  if (cr6.lt) goto loc_823F371C;
loc_823F379C:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f37b8
  if (cr6.eq) goto loc_823F37B8;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F37B8:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f37d4
  if (cr6.eq) goto loc_823F37D4;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F37D4:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_823F37E8 game::L6V5U5Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F37E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F37E8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r8,r10,r11
  r8 = r10 + r11;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bge cr6,0x823f38fc
  if (!cr6.lt) goto loc_823F38FC;
  // lis r7,-32252
  r7 = uint64_t(int64_t(-2113667072));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // li r4,-16
  r4 = uint64_t(int64_t(-16));
  // lfs f12,-18516(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + -18516);
  f12 = double(temp.f32);
  // lfs f13,2608(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f13 = double(temp.f32);
  // lfs f0,-32672(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -32672);
  f0 = double(temp.f32);
loc_823F3844:
  // lhz r10,0(r11)
  r10 = guest::LoadU16(base, uint32_t(r11) + 0);
  // stfs f13,8(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // extsh r9,r10
  r9 = uint64_t(int64_t(int16_t(r10)));
  // rlwinm r10,r10,30,2,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFF8;
  // rlwinm r9,r9,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // extsb r7,r10
  r7 = uint64_t(int64_t(int8_t(r10)));
  // extsb r10,r9
  r10 = uint64_t(int64_t(int8_t(r9)));
  // srawi r10,r10,3
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x7) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 3));
  // srawi r9,r7,3
  ca = (int32_t(r7) < 0) & ((uint32_t(r7) & 0x7) != 0);
  r9 = uint64_t(int64_t(int32_t(r7) >> 3));
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // extsb r9,r9
  r9 = uint64_t(int64_t(int8_t(r9)));
  // subf r7,r4,r10
  r7 = r10 - r4;
  // subf r5,r4,r9
  r5 = r9 - r4;
  // cntlzw r7,r7
  r7 = uint32_t(r7) == 0 ? 32 : __builtin_clz(uint32_t(r7));
  // cntlzw r31,r5
  r31 = uint32_t(r5) == 0 ? 32 : __builtin_clz(uint32_t(r5));
  // rlwinm r5,r7,27,31,31
  r5 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 27) & 0x1;
  // rlwinm r7,r31,27,31,31
  r7 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 27) & 0x1;
  // add r10,r5,r10
  r10 = r5 + r10;
  // add r9,r7,r9
  r9 = r7 + r9;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // extsb r9,r9
  r9 = uint64_t(int64_t(int8_t(r9)));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f11,80(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // lfd f10,88(r1)
  f10 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f11,f11
  f11 = double(float(f11));
  // frsp f10,f10
  f10 = double(float(f10));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r6)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // fmuls f11,f10,f0
  f11 = double(float(f10 * f0));
  // stfs f11,4(r6)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lhz r10,0(r11)
  r10 = guest::LoadU16(base, uint32_t(r11) + 0);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // rlwinm r10,r10,22,10,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 22) & 0x3FFFFF;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // std r10,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r10);
  // lfd f11,96(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f12
  f11 = double(float(f11 * f12));
  // stfs f11,12(r6)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f3844
  if (cr6.lt) goto loc_823F3844;
loc_823F38FC:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3918
  if (cr6.eq) goto loc_823F3918;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F3918:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3934
  if (cr6.eq) goto loc_823F3934;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F3934:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_823F3948 game::X8L8V8U8Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F3948(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F3948 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r8,r10,r11
  r8 = r10 + r11;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bge cr6,0x823f3a3c
  if (!cr6.lt) goto loc_823F3A3C;
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32252
  r10 = uint64_t(int64_t(-2113667072));
  // li r4,-128
  r4 = uint64_t(int64_t(-128));
  // lfs f12,8704(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 8704);
  f12 = double(temp.f32);
  // lfs f13,2608(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f13 = double(temp.f32);
  // lfs f0,-18492(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -18492);
  f0 = double(temp.f32);
loc_823F39A4:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stfs f13,8(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // rlwinm r9,r10,24,8,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFFFFFF;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // extsb r9,r9
  r9 = uint64_t(int64_t(int8_t(r9)));
  // subf r7,r4,r10
  r7 = r10 - r4;
  // subf r5,r4,r9
  r5 = r9 - r4;
  // cntlzw r7,r7
  r7 = uint32_t(r7) == 0 ? 32 : __builtin_clz(uint32_t(r7));
  // cntlzw r31,r5
  r31 = uint32_t(r5) == 0 ? 32 : __builtin_clz(uint32_t(r5));
  // rlwinm r5,r7,27,31,31
  r5 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 27) & 0x1;
  // rlwinm r7,r31,27,31,31
  r7 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 27) & 0x1;
  // add r10,r5,r10
  r10 = r5 + r10;
  // add r9,r7,r9
  r9 = r7 + r9;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // extsb r9,r9
  r9 = uint64_t(int64_t(int8_t(r9)));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f11,80(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // lfd f10,88(r1)
  f10 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f11,f11
  f11 = double(float(f11));
  // frsp f10,f10
  f10 = double(float(f10));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // stfs f11,0(r6)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // fmuls f11,f10,f0
  f11 = double(float(f10 * f0));
  // stfs f11,4(r6)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // lbz r10,1(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 1);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // std r10,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r10);
  // lfd f11,96(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f11,f11,f12
  f11 = double(float(f11 * f12));
  // stfs f11,12(r6)
  temp.f32 = float(f11);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f39a4
  if (cr6.lt) goto loc_823F39A4;
loc_823F3A3C:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3a58
  if (cr6.eq) goto loc_823F3A58;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F3A58:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3a74
  if (cr6.eq) goto loc_823F3A74;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F3A74:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_823F3A88 game::Q8W8V8U8Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F3A88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F3A88 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r4,r10,r11
  r4 = r10 + r11;
  // cmplw cr6,r11,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r4), so);
  // bge cr6,0x823f3bb0
  if (!cr6.lt) goto loc_823F3BB0;
  // lis r10,-32252
  r10 = uint64_t(int64_t(-2113667072));
  // li r9,-128
  r9 = uint64_t(int64_t(-128));
  // lfs f0,-18492(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -18492);
  f0 = double(temp.f32);
loc_823F3AD0:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // rlwinm r8,r10,24,8,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFFFFFF;
  // rlwinm r7,r10,16,16,31
  r7 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 16) & 0xFFFF;
  // rlwinm r5,r10,8,24,31
  r5 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFF;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // extsb r8,r8
  r8 = uint64_t(int64_t(int8_t(r8)));
  // extsb r5,r5
  r5 = uint64_t(int64_t(int8_t(r5)));
  // subf r31,r9,r10
  r31 = r10 - r9;
  // extsb r7,r7
  r7 = uint64_t(int64_t(int8_t(r7)));
  // subf r28,r9,r5
  r28 = r5 - r9;
  // subf r30,r9,r8
  r30 = r8 - r9;
  // cntlzw r31,r31
  r31 = uint32_t(r31) == 0 ? 32 : __builtin_clz(uint32_t(r31));
  // subf r29,r9,r7
  r29 = r7 - r9;
  // cntlzw r30,r30
  r30 = uint32_t(r30) == 0 ? 32 : __builtin_clz(uint32_t(r30));
  // cntlzw r26,r28
  r26 = uint32_t(r28) == 0 ? 32 : __builtin_clz(uint32_t(r28));
  // rlwinm r28,r31,27,31,31
  r28 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 27) & 0x1;
  // cntlzw r27,r29
  r27 = uint32_t(r29) == 0 ? 32 : __builtin_clz(uint32_t(r29));
  // rlwinm r29,r30,27,31,31
  r29 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 27) & 0x1;
  // add r10,r28,r10
  r10 = r28 + r10;
  // rlwinm r30,r27,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 27) & 0x1;
  // add r8,r29,r8
  r8 = r29 + r8;
  // rlwinm r31,r26,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 27) & 0x1;
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // add r7,r30,r7
  r7 = r30 + r7;
  // extsb r8,r8
  r8 = uint64_t(int64_t(int8_t(r8)));
  // add r5,r31,r5
  r5 = r31 + r5;
  // extsb r7,r7
  r7 = uint64_t(int64_t(int8_t(r7)));
  // extsb r5,r5
  r5 = uint64_t(int64_t(int8_t(r5)));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // cmplw cr6,r11,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r4), so);
  // std r8,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r8);
  // std r7,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r7);
  // std r5,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r5);
  // lfd f13,80(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,96(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // lfd f10,104(r1)
  f10 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f13,f13
  f13 = double(float(f13));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // frsp f10,f10
  f10 = double(float(f10));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // fmuls f13,f12,f0
  f13 = double(float(f12 * f0));
  // stfs f13,4(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // fmuls f13,f11,f0
  f13 = double(float(f11 * f0));
  // stfs f13,8(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // fmuls f13,f10,f0
  f13 = double(float(f10 * f0));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f3ad0
  if (cr6.lt) goto loc_823F3AD0;
loc_823F3BB0:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3bcc
  if (cr6.eq) goto loc_823F3BCC;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F3BCC:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3be8
  if (cr6.eq) goto loc_823F3BE8;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F3BE8:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_823F3BF0 game::V16U16Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F3BF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F3BF0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r8,r10,r11
  r8 = r10 + r11;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bge cr6,0x823f3cc4
  if (!cr6.lt) goto loc_823F3CC4;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r4,-32768
  r4 = uint64_t(int64_t(-32768));
  // lfs f13,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f13 = double(temp.f32);
  // lfs f0,11220(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 11220);
  f0 = double(temp.f32);
loc_823F3C44:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stfs f13,8(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // rlwinm r9,r10,16,16,31
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 16) & 0xFFFF;
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // subf r7,r4,r10
  r7 = r10 - r4;
  // subf r5,r4,r9
  r5 = r9 - r4;
  // cntlzw r7,r7
  r7 = uint32_t(r7) == 0 ? 32 : __builtin_clz(uint32_t(r7));
  // cntlzw r31,r5
  r31 = uint32_t(r5) == 0 ? 32 : __builtin_clz(uint32_t(r5));
  // rlwinm r5,r7,27,31,31
  r5 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 27) & 0x1;
  // rlwinm r7,r31,27,31,31
  r7 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 27) & 0x1;
  // add r10,r5,r10
  r10 = r5 + r10;
  // add r9,r7,r9
  r9 = r7 + r9;
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // lfd f12,80(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // fmuls f12,f11,f0
  f12 = double(float(f11 * f0));
  // stfs f12,4(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f3c44
  if (cr6.lt) goto loc_823F3C44;
loc_823F3CC4:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3ce0
  if (cr6.eq) goto loc_823F3CE0;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F3CE0:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3cfc
  if (cr6.eq) goto loc_823F3CFC;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F3CFC:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_823F3D10 game::A2W10V10U10Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F3D10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F3D10 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r5,r10,r11
  r5 = r10 + r11;
  // cmplw cr6,r11,r5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r5), so);
  // bge cr6,0x823f3e4c
  if (!cr6.lt) goto loc_823F3E4C;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32252
  r10 = uint64_t(int64_t(-2113667072));
  // li r7,-512
  r7 = uint64_t(int64_t(-512));
  // lfs f13,10808(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 10808);
  f13 = double(temp.f32);
  // lfs f0,-18484(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -18484);
  f0 = double(temp.f32);
loc_823F3D60:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm r9,r10,6,0,25
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 6) & 0xFFFFFFC0;
  // rlwinm r8,r10,28,4,25
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 28) & 0xFFFFFC0;
  // rlwinm r10,r10,18,14,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 18) & 0x3FFC0;
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // extsh r8,r8
  r8 = uint64_t(int64_t(int16_t(r8)));
  // extsh r4,r10
  r4 = uint64_t(int64_t(int16_t(r10)));
  // srawi r10,r9,6
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3F) != 0);
  r10 = uint64_t(int64_t(int32_t(r9) >> 6));
  // srawi r9,r8,6
  ca = (int32_t(r8) < 0) & ((uint32_t(r8) & 0x3F) != 0);
  r9 = uint64_t(int64_t(int32_t(r8) >> 6));
  // srawi r8,r4,6
  ca = (int32_t(r4) < 0) & ((uint32_t(r4) & 0x3F) != 0);
  r8 = uint64_t(int64_t(int32_t(r4) >> 6));
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // extsh r8,r8
  r8 = uint64_t(int64_t(int16_t(r8)));
  // subf r4,r7,r10
  r4 = r10 - r7;
  // subf r30,r7,r8
  r30 = r8 - r7;
  // subf r31,r7,r9
  r31 = r9 - r7;
  // cntlzw r4,r4
  r4 = uint32_t(r4) == 0 ? 32 : __builtin_clz(uint32_t(r4));
  // cntlzw r29,r30
  r29 = uint32_t(r30) == 0 ? 32 : __builtin_clz(uint32_t(r30));
  // cntlzw r31,r31
  r31 = uint32_t(r31) == 0 ? 32 : __builtin_clz(uint32_t(r31));
  // rlwinm r30,r4,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 27) & 0x1;
  // rlwinm r31,r31,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 27) & 0x1;
  // add r10,r30,r10
  r10 = r30 + r10;
  // rlwinm r4,r29,27,31,31
  r4 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 27) & 0x1;
  // add r9,r31,r9
  r9 = r31 + r9;
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // add r8,r4,r8
  r8 = r4 + r8;
  // extsh r9,r9
  r9 = uint64_t(int64_t(int16_t(r9)));
  // extsh r8,r8
  r8 = uint64_t(int64_t(int16_t(r8)));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // std r9,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r9);
  // std r8,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r8);
  // lfd f12,80(r1)
  lifted::ScalarFloatMode();
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,88(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // lfd f10,96(r1)
  f10 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // frsp f10,f10
  f10 = double(float(f10));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // stfs f12,0(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // fmuls f12,f11,f0
  f12 = double(float(f11 * f0));
  // stfs f12,4(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // fmuls f12,f10,f0
  f12 = double(float(f10 * f0));
  // stfs f12,8(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // rlwinm r10,r10,2,30,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0x3;
  // cmplw cr6,r11,r5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r5), so);
  // clrldi r10,r10,32
  r10 = r10 & 0xFFFFFFFF;
  // std r10,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r10);
  // lfd f12,104(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f12,f12,f13
  f12 = double(float(f12 * f13));
  // stfs f12,12(r6)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f3d60
  if (cr6.lt) goto loc_823F3D60;
loc_823F3E4C:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3e68
  if (cr6.eq) goto loc_823F3E68;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F3E68:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3e84
  if (cr6.eq) goto loc_823F3E84;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F3E84:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_823F3E90 game::Q16W16V16U16Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F3E90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F3E90 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r4,r10,r11
  r4 = r10 + r11;
  // cmplw cr6,r11,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r4), so);
  // bge cr6,0x823f3fb8
  if (!cr6.lt) goto loc_823F3FB8;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // li r9,-32768
  r9 = uint64_t(int64_t(-32768));
  // lfs f0,11220(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 11220);
  f0 = double(temp.f32);
loc_823F3ED8:
  // ld r10,0(r11)
  r10 = guest::LoadU64(base, uint32_t(r11) + 0);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // rldicl r8,r10,48,16
  r8 = __builtin_rotateleft64(r10, 48) & 0xFFFFFFFFFFFF;
  // rldicl r7,r10,32,32
  r7 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFF;
  // rldicl r5,r10,16,48
  r5 = __builtin_rotateleft64(r10, 16) & 0xFFFF;
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // extsh r8,r8
  r8 = uint64_t(int64_t(int16_t(r8)));
  // extsh r5,r5
  r5 = uint64_t(int64_t(int16_t(r5)));
  // subf r31,r9,r10
  r31 = r10 - r9;
  // extsh r7,r7
  r7 = uint64_t(int64_t(int16_t(r7)));
  // subf r28,r9,r5
  r28 = r5 - r9;
  // subf r30,r9,r8
  r30 = r8 - r9;
  // cntlzw r31,r31
  r31 = uint32_t(r31) == 0 ? 32 : __builtin_clz(uint32_t(r31));
  // subf r29,r9,r7
  r29 = r7 - r9;
  // cntlzw r30,r30
  r30 = uint32_t(r30) == 0 ? 32 : __builtin_clz(uint32_t(r30));
  // cntlzw r26,r28
  r26 = uint32_t(r28) == 0 ? 32 : __builtin_clz(uint32_t(r28));
  // rlwinm r28,r31,27,31,31
  r28 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 27) & 0x1;
  // cntlzw r27,r29
  r27 = uint32_t(r29) == 0 ? 32 : __builtin_clz(uint32_t(r29));
  // rlwinm r29,r30,27,31,31
  r29 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 27) & 0x1;
  // add r10,r28,r10
  r10 = r28 + r10;
  // rlwinm r30,r27,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 27) & 0x1;
  // add r8,r29,r8
  r8 = r29 + r8;
  // rlwinm r31,r26,27,31,31
  r31 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 27) & 0x1;
  // extsh r10,r10
  r10 = uint64_t(int64_t(int16_t(r10)));
  // add r7,r30,r7
  r7 = r30 + r7;
  // extsh r8,r8
  r8 = uint64_t(int64_t(int16_t(r8)));
  // add r5,r31,r5
  r5 = r31 + r5;
  // extsh r7,r7
  r7 = uint64_t(int64_t(int16_t(r7)));
  // extsh r5,r5
  r5 = uint64_t(int64_t(int16_t(r5)));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // cmplw cr6,r11,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r4), so);
  // std r8,88(r1)
  guest::StoreU64(base, uint32_t(r1) + 88, r8);
  // std r7,96(r1)
  guest::StoreU64(base, uint32_t(r1) + 96, r7);
  // std r5,104(r1)
  guest::StoreU64(base, uint32_t(r1) + 104, r5);
  // lfd f13,80(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f12,88(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 88)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // lfd f11,96(r1)
  f11 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 96)));
  // lfd f10,104(r1)
  f10 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 104)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // fcfid f10,f10
  f10 = double(std::bit_cast<int64_t>(f10));
  // frsp f13,f13
  f13 = double(float(f13));
  // frsp f12,f12
  f12 = double(float(f12));
  // frsp f11,f11
  f11 = double(float(f11));
  // frsp f10,f10
  f10 = double(float(f10));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // fmuls f13,f12,f0
  f13 = double(float(f12 * f0));
  // stfs f13,4(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // fmuls f13,f11,f0
  f13 = double(float(f11 * f0));
  // stfs f13,8(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // fmuls f13,f10,f0
  f13 = double(float(f10 * f0));
  // stfs f13,12(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f3ed8
  if (cr6.lt) goto loc_823F3ED8;
loc_823F3FB8:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3fd4
  if (cr6.eq) goto loc_823F3FD4;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F3FD4:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f3ff0
  if (cr6.eq) goto loc_823F3FF0;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F3FF0:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_823F3FF8 game::L16Codec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F3FF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F3FF8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,1,0,30
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f4080
  if (!cr6.lt) goto loc_823F4080;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32253
  r9 = uint64_t(int64_t(-2113732608));
  // lfs f12,2608(r8)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + 2608);
  f12 = double(temp.f32);
  // lfs f13,29924(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 29924);
  f13 = double(temp.f32);
loc_823F4044:
  // lhz r9,0(r11)
  r9 = guest::LoadU16(base, uint32_t(r11) + 0);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // stfs f12,12(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // extsw r9,r9
  r9 = uint64_t(int64_t(int32_t(r9)));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // std r9,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r9);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,8(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // stfs f0,4(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // stfs f0,0(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f4044
  if (cr6.lt) goto loc_823F4044;
loc_823F4080:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f409c
  if (cr6.eq) goto loc_823F409C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F409C:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f40b8
  if (cr6.eq) goto loc_823F40B8;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F40B8:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F40C8 game::R16FCodec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F40C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F40C8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0;
  lifted::Vec v0{}, v1{}, v2{}, v3{}, v4{}, v5{}, v6{}, v7{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{}, v28{}, v29{}, v30{}, v31{};
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  lifted::Vec vTemp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-480(r1)
  ea = -480 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r10,100(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r8,104(r3)
  r8 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // mullw r10,r10,r5
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r5))));
  // lwz r9,32(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // stw r3,500(r1)
  guest::StoreU32(base, uint32_t(r1) + 500, uint32_t(r3));
  // stw r6,524(r1)
  guest::StoreU32(base, uint32_t(r1) + 524, uint32_t(r6));
  // add r10,r11,r10
  r10 = r11 + r10;
  // rlwinm r11,r8,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 1) & 0xFFFFFFFE;
  // add r4,r10,r9
  r4 = r10 + r9;
  // add r31,r11,r4
  r31 = r11 + r4;
  // cmplw cr6,r4,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r31), so);
  // stw r4,224(r1)
  guest::StoreU32(base, uint32_t(r1) + 224, uint32_t(r4));
  // bge cr6,0x823f46f4
  if (!cr6.lt) goto loc_823F46F4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
loc_823F4118:
  // addi r11,r6,127
  r11 = uint64_t(int64_t(int64_t(r6) + 127));
  // addi r5,r6,4
  r5 = uint64_t(int64_t(int64_t(r6) + 4));
  // rlwinm r9,r11,0,0,24
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  // mr r10,r4
  r10 = r4;
  // mr r11,r6
  r11 = r6;
  // cmplw cr6,r5,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r9), so);
  // stw r5,232(r1)
  guest::StoreU32(base, uint32_t(r1) + 232, uint32_t(r5));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // bge cr6,0x823f4144
  if (!cr6.lt) goto loc_823F4144;
  // mr r9,r5
  r9 = r5;
loc_823F4144:
  // subf r8,r9,r5
  r8 = r5 - r9;
  // stw r9,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r9));
  // mr r3,r5
  r3 = r5;
  // rlwinm r8,r8,0,0,22
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xFFFFFE00;
  // add r8,r8,r9
  r8 = r8 + r9;
  // cmplw cr6,r5,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r8), so);
  // blt cr6,0x823f4164
  if (cr6.lt) goto loc_823F4164;
  // mr r3,r8
  r3 = r8;
loc_823F4164:
  // subf r8,r6,r9
  r8 = r9 - r6;
  // stw r3,228(r1)
  guest::StoreU32(base, uint32_t(r1) + 228, uint32_t(r3));
  // clrlwi r8,r8,27
  r8 = uint32_t(r8) & 0x1F;
  // add r8,r8,r6
  r8 = r8 + r6;
  // cmplw cr6,r5,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r8), so);
  // bge cr6,0x823f4180
  if (!cr6.lt) goto loc_823F4180;
  // mr r8,r5
  r8 = r5;
loc_823F4180:
  // subf r7,r3,r5
  r7 = r5 - r3;
  // rlwinm r7,r7,0,0,26
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFFE0;
  // add r7,r7,r3
  r7 = r7 + r3;
  // cmplw cr6,r5,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r7), so);
  // bge cr6,0x823f4198
  if (!cr6.lt) goto loc_823F4198;
  // mr r7,r5
  r7 = r5;
loc_823F4198:
  // stw r7,236(r1)
  guest::StoreU32(base, uint32_t(r1) + 236, uint32_t(r7));
  // li r30,128
  r30 = uint64_t(int64_t(128));
  // li r30,256
  r30 = uint64_t(int64_t(256));
  // li r30,384
  r30 = uint64_t(int64_t(384));
  // cmplw cr6,r6,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r8), so);
  // bge cr6,0x823f428c
  if (!cr6.lt) goto loc_823F428C;
loc_823F41C0:
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vsplth v0,v0,0
  simde_mm_store_si128((simde__m128i*)v0.u16, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u16), simde_mm_set1_epi16(short(0xF0E))));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vupkd3d128 v0,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[0] = vTemp.u32[0];
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stvewx v0,r0,r11
  ea = (uint32_t(r11)) & ~0x3;
  guest::StoreU32(base, ea, v0.u32[3 - ((ea & 0xF) >> 2)]);
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // lwz r10,80(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 80);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // blt cr6,0x823f41c0
  if (cr6.lt) goto loc_823F41C0;
  // lwz r6,524(r1)
  r6 = guest::LoadU32(base, uint32_t(r1) + 524);
  // lwz r4,224(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 224);
  // lwz r5,232(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 232);
  // lwz r9,240(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 240);
  // lwz r3,228(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 228);
  // lwz r7,236(r1)
  r7 = guest::LoadU32(base, uint32_t(r1) + 236);
  // b 0x823f428c
  goto loc_823F428C;
loc_823F4224:
  // addi r30,r1,96
  r30 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // stvx v0,r0,r30
  ea = (uint32_t(r30)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvrx v13,r10,r8
  temp.u32 = uint32_t(r10) + uint32_t(r8);
  simde_mm_store_si128((simde__m128i*)v13.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r8,r1,256
  r8 = uint64_t(int64_t(int64_t(r1) + 256));
  // vor v0,v0,v13
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v13.u8)));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // vsldoi v13,v0,v0,8
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v0.u8), 8));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // vupkd3d128 v0,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[0] = vTemp.u32[0];
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vupkd3d128 v13,v13,20
  temp.u32 = v13.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[3] = vTemp.u32[0];
  temp.u32 = v13.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[2] = vTemp.u32[0];
  temp.u32 = v13.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[1] = vTemp.u32[0];
  temp.u32 = v13.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[0] = vTemp.u32[0];
  // addi r8,r1,256
  r8 = uint64_t(int64_t(int64_t(r1) + 256));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_823F428C:
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x823f4224
  if (cr6.lt) goto loc_823F4224;
  // b 0x823f45f4
  goto loc_823F45F4;
loc_823F4298:
  // li r9,512
  r9 = uint64_t(int64_t(512));
  // li r9,640
  r9 = uint64_t(int64_t(640));
  // li r9,768
  r9 = uint64_t(int64_t(768));
  // li r9,896
  r9 = uint64_t(int64_t(896));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,384(r1)
  guest::StoreU32(base, uint32_t(r1) + 384, uint32_t(r9));
loc_823F42C0:
  // addi r23,r1,96
  r23 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // li r30,16
  r30 = uint64_t(int64_t(16));
  // li r29,16
  r29 = uint64_t(int64_t(16));
  // li r28,16
  r28 = uint64_t(int64_t(16));
  // stvx v0,r0,r23
  ea = (uint32_t(r23)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // li r27,16
  r27 = uint64_t(int64_t(16));
  // lvrx v13,r10,r8
  temp.u32 = uint32_t(r10) + uint32_t(r8);
  simde_mm_store_si128((simde__m128i*)v13.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r8,r1,256
  r8 = uint64_t(int64_t(int64_t(r1) + 256));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // vor v0,v0,v13
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v13.u8)));
  // li r26,16
  r26 = uint64_t(int64_t(16));
  // li r25,16
  r25 = uint64_t(int64_t(16));
  // li r24,16
  r24 = uint64_t(int64_t(16));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v12,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,272
  r8 = uint64_t(int64_t(int64_t(r1) + 272));
  // lvrx v13,r10,r30
  temp.u32 = uint32_t(r10) + uint32_t(r30);
  simde_mm_store_si128((simde__m128i*)v13.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v11,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // vor v13,v12,v13
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)v13.u8)));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,288
  r8 = uint64_t(int64_t(int64_t(r1) + 288));
  // lvrx v12,r10,r29
  temp.u32 = uint32_t(r10) + uint32_t(r29);
  simde_mm_store_si128((simde__m128i*)v12.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v10,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // vor v12,v11,v12
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v12.u8)));
  // stvx v10,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,304
  r8 = uint64_t(int64_t(int64_t(r1) + 304));
  // lvrx v11,r10,r28
  temp.u32 = uint32_t(r10) + uint32_t(r28);
  simde_mm_store_si128((simde__m128i*)v11.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,160
  r8 = uint64_t(int64_t(int64_t(r1) + 160));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v9,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // stvx v9,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,320
  r8 = uint64_t(int64_t(int64_t(r1) + 320));
  // lvrx v8,r10,r27
  temp.u32 = uint32_t(r10) + uint32_t(r27);
  simde_mm_store_si128((simde__m128i*)v8.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v8,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,176
  r8 = uint64_t(int64_t(int64_t(r1) + 176));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v7,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v7.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // stvx v7,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,336
  r8 = uint64_t(int64_t(int64_t(r1) + 336));
  // lvrx v6,r10,r26
  temp.u32 = uint32_t(r10) + uint32_t(r26);
  simde_mm_store_si128((simde__m128i*)v6.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v6,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v6.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,192
  r8 = uint64_t(int64_t(int64_t(r1) + 192));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v5,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v5.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // stvx v5,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v5.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,352
  r8 = uint64_t(int64_t(int64_t(r1) + 352));
  // lvrx v4,r10,r25
  temp.u32 = uint32_t(r10) + uint32_t(r25);
  simde_mm_store_si128((simde__m128i*)v4.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v4,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v4.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,208
  r8 = uint64_t(int64_t(int64_t(r1) + 208));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v3,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v3.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // stvx v3,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v3.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvrx v2,r10,r24
  temp.u32 = uint32_t(r10) + uint32_t(r24);
  simde_mm_store_si128((simde__m128i*)v2.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,368
  r8 = uint64_t(int64_t(int64_t(r1) + 368));
  // stvx v2,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v2.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // vor v11,v10,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)v11.u8)));
  // vor v10,v9,v8
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)v8.u8)));
  // vor v9,v7,v6
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)v6.u8)));
  // vupkd3d128 v6,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v6.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v6.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v6.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v6.u32[0] = vTemp.u32[0];
  // vor v8,v5,v4
  simde_mm_store_si128((simde__m128i*)v8.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v5.u8), simde_mm_load_si128((simde__m128i*)v4.u8)));
  // vupkd3d128 v5,v13,20
  temp.u32 = v13.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v5.u32[3] = vTemp.u32[0];
  temp.u32 = v13.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v5.u32[2] = vTemp.u32[0];
  temp.u32 = v13.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v5.u32[1] = vTemp.u32[0];
  temp.u32 = v13.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v5.u32[0] = vTemp.u32[0];
  // vor v7,v3,v2
  simde_mm_store_si128((simde__m128i*)v7.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v3.u8), simde_mm_load_si128((simde__m128i*)v2.u8)));
  // vupkd3d128 v4,v12,20
  temp.u32 = v12.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v4.u32[3] = vTemp.u32[0];
  temp.u32 = v12.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v4.u32[2] = vTemp.u32[0];
  temp.u32 = v12.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v4.u32[1] = vTemp.u32[0];
  temp.u32 = v12.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v4.u32[0] = vTemp.u32[0];
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,160
  r8 = uint64_t(int64_t(int64_t(r1) + 160));
  // vsldoi v0,v0,v0,8
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v0.u8), 8));
  // vsldoi v13,v13,v13,8
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)v13.u8), 8));
  // vsldoi v12,v12,v12,8
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)v12.u8), 8));
  // vupkd3d128 v3,v11,20
  temp.u32 = v11.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v3.u32[3] = vTemp.u32[0];
  temp.u32 = v11.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v3.u32[2] = vTemp.u32[0];
  temp.u32 = v11.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v3.u32[1] = vTemp.u32[0];
  temp.u32 = v11.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v3.u32[0] = vTemp.u32[0];
  // stvx v10,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,176
  r8 = uint64_t(int64_t(int64_t(r1) + 176));
  // vupkd3d128 v2,v10,20
  temp.u32 = v10.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v2.u32[3] = vTemp.u32[0];
  temp.u32 = v10.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v2.u32[2] = vTemp.u32[0];
  temp.u32 = v10.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v2.u32[1] = vTemp.u32[0];
  temp.u32 = v10.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v2.u32[0] = vTemp.u32[0];
  // vupkd3d128 v1,v9,20
  temp.u32 = v9.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v1.u32[3] = vTemp.u32[0];
  temp.u32 = v9.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v1.u32[2] = vTemp.u32[0];
  temp.u32 = v9.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v1.u32[1] = vTemp.u32[0];
  temp.u32 = v9.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v1.u32[0] = vTemp.u32[0];
  // vupkd3d128 v31,v8,20
  temp.u32 = v8.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v31.u32[3] = vTemp.u32[0];
  temp.u32 = v8.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v31.u32[2] = vTemp.u32[0];
  temp.u32 = v8.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v31.u32[1] = vTemp.u32[0];
  temp.u32 = v8.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v31.u32[0] = vTemp.u32[0];
  // vupkd3d128 v30,v7,20
  temp.u32 = v7.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v30.u32[3] = vTemp.u32[0];
  temp.u32 = v7.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v30.u32[2] = vTemp.u32[0];
  temp.u32 = v7.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v30.u32[1] = vTemp.u32[0];
  temp.u32 = v7.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v30.u32[0] = vTemp.u32[0];
  // stvx v9,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,192
  r8 = uint64_t(int64_t(int64_t(r1) + 192));
  // vsldoi v29,v11,v11,8
  simde_mm_store_si128((simde__m128i*)v29.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v11.u8), 8));
  // vsldoi v28,v10,v10,8
  simde_mm_store_si128((simde__m128i*)v28.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)v10.u8), 8));
  // vsldoi v11,v9,v9,8
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)v9.u8), 8));
  // vsldoi v10,v8,v8,8
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)v8.u8), 8));
  // stvx v8,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,208
  r8 = uint64_t(int64_t(int64_t(r1) + 208));
  // vsldoi v9,v7,v7,8
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)v7.u8), 8));
  // vupkd3d128 v8,v29,20
  temp.u32 = v29.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v8.u32[3] = vTemp.u32[0];
  temp.u32 = v29.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v8.u32[2] = vTemp.u32[0];
  temp.u32 = v29.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v8.u32[1] = vTemp.u32[0];
  temp.u32 = v29.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v8.u32[0] = vTemp.u32[0];
  // stvx v7,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,256
  r8 = uint64_t(int64_t(int64_t(r1) + 256));
  // vupkd3d128 v7,v28,20
  temp.u32 = v28.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v7.u32[3] = vTemp.u32[0];
  temp.u32 = v28.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v7.u32[2] = vTemp.u32[0];
  temp.u32 = v28.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v7.u32[1] = vTemp.u32[0];
  temp.u32 = v28.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v7.u32[0] = vTemp.u32[0];
  // stvx v6,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v6.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,272
  r8 = uint64_t(int64_t(int64_t(r1) + 272));
  // stvx v5,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v5.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,288
  r8 = uint64_t(int64_t(int64_t(r1) + 288));
  // stvx v4,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v4.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // vupkd3d128 v0,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[0] = vTemp.u32[0];
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // vupkd3d128 v13,v13,20
  temp.u32 = v13.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[3] = vTemp.u32[0];
  temp.u32 = v13.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[2] = vTemp.u32[0];
  temp.u32 = v13.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[1] = vTemp.u32[0];
  temp.u32 = v13.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[0] = vTemp.u32[0];
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,304
  r8 = uint64_t(int64_t(int64_t(r1) + 304));
  // vupkd3d128 v12,v12,20
  temp.u32 = v12.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v12.u32[3] = vTemp.u32[0];
  temp.u32 = v12.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v12.u32[2] = vTemp.u32[0];
  temp.u32 = v12.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v12.u32[1] = vTemp.u32[0];
  temp.u32 = v12.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v12.u32[0] = vTemp.u32[0];
  // stvx v6,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v6.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v3,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v3.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,320
  r8 = uint64_t(int64_t(int64_t(r1) + 320));
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v2,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v2.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,336
  r8 = uint64_t(int64_t(int64_t(r1) + 336));
  // stvx v5,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v5.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v1,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v1.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,352
  r8 = uint64_t(int64_t(int64_t(r1) + 352));
  // stvx v12,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v31,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v31.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,368
  r8 = uint64_t(int64_t(int64_t(r1) + 368));
  // stvx v4,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v4.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v30,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v30.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // stvx v29,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v29.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,160
  r8 = uint64_t(int64_t(int64_t(r1) + 160));
  // vupkd3d128 v29,v9,20
  temp.u32 = v9.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v29.u32[3] = vTemp.u32[0];
  temp.u32 = v9.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v29.u32[2] = vTemp.u32[0];
  temp.u32 = v9.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v29.u32[1] = vTemp.u32[0];
  temp.u32 = v9.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v29.u32[0] = vTemp.u32[0];
  // stvx v28,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v28.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,176
  r8 = uint64_t(int64_t(int64_t(r1) + 176));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,192
  r8 = uint64_t(int64_t(int64_t(r1) + 192));
  // vupkd3d128 v11,v11,20
  temp.u32 = v11.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v11.u32[3] = vTemp.u32[0];
  temp.u32 = v11.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v11.u32[2] = vTemp.u32[0];
  temp.u32 = v11.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v11.u32[1] = vTemp.u32[0];
  temp.u32 = v11.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v11.u32[0] = vTemp.u32[0];
  // stvx v10,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vupkd3d128 v10,v10,20
  temp.u32 = v10.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v10.u32[3] = vTemp.u32[0];
  temp.u32 = v10.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v10.u32[2] = vTemp.u32[0];
  temp.u32 = v10.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v10.u32[1] = vTemp.u32[0];
  temp.u32 = v10.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v10.u32[0] = vTemp.u32[0];
  // stvx v8,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // addi r8,r1,208
  r8 = uint64_t(int64_t(int64_t(r1) + 208));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplwi cr6,r9,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 4, so);
  // stvx v3,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v3.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v9,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r9,384(r1)
  guest::StoreU32(base, uint32_t(r1) + 384, uint32_t(r9));
  // stvx v7,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // stvx v2,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v2.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stvx v11,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // stvx v1,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v1.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v8,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,160
  r8 = uint64_t(int64_t(int64_t(r1) + 160));
  // stvx v10,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v7,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,176
  r8 = uint64_t(int64_t(int64_t(r1) + 176));
  // stvx v31,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v31.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,192
  r8 = uint64_t(int64_t(int64_t(r1) + 192));
  // stvx v29,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v29.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v10,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,208
  r8 = uint64_t(int64_t(int64_t(r1) + 208));
  // stvx v30,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v30.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v29,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v29.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // blt cr6,0x823f42c0
  if (cr6.lt) goto loc_823F42C0;
loc_823F45F4:
  // cmplw cr6,r11,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r3), so);
  // blt cr6,0x823f4298
  if (cr6.lt) goto loc_823F4298;
  // b 0x823f4668
  goto loc_823F4668;
loc_823F4600:
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvrx v13,r10,r9
  temp.u32 = uint32_t(r10) + uint32_t(r9);
  simde_mm_store_si128((simde__m128i*)v13.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r9,r1,256
  r9 = uint64_t(int64_t(int64_t(r1) + 256));
  // vor v0,v0,v13
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v13.u8)));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v13,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // vsldoi v13,v0,v0,8
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v0.u8), 8));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stvx v0,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // vupkd3d128 v0,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[0] = vTemp.u32[0];
  // stvx v13,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vupkd3d128 v13,v13,20
  temp.u32 = v13.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[3] = vTemp.u32[0];
  temp.u32 = v13.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[2] = vTemp.u32[0];
  temp.u32 = v13.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[1] = vTemp.u32[0];
  temp.u32 = v13.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[0] = vTemp.u32[0];
  // addi r9,r1,256
  r9 = uint64_t(int64_t(int64_t(r1) + 256));
  // stvx v0,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v13,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_823F4668:
  // cmplw cr6,r11,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r7), so);
  // blt cr6,0x823f4600
  if (cr6.lt) goto loc_823F4600;
  // cmplw cr6,r11,r5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r5), so);
  // bge cr6,0x823f46cc
  if (!cr6.lt) goto loc_823F46CC;
loc_823F4678:
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vsplth v0,v0,0
  simde_mm_store_si128((simde__m128i*)v0.u16, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u16), simde_mm_set1_epi16(short(0xF0E))));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vupkd3d128 v0,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[0] = vTemp.u32[0];
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stvewx v0,r0,r11
  ea = (uint32_t(r11)) & ~0x3;
  guest::StoreU32(base, ea, v0.u32[3 - ((ea & 0xF) >> 2)]);
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // lwz r10,80(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 80);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // lwz r9,232(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 232);
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // blt cr6,0x823f4678
  if (cr6.lt) goto loc_823F4678;
  // lwz r6,524(r1)
  r6 = guest::LoadU32(base, uint32_t(r1) + 524);
  // lwz r4,224(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 224);
loc_823F46CC:
  // stfs f0,12(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 12, temp.u32);
  // addi r4,r4,2
  r4 = uint64_t(int64_t(int64_t(r4) + 2));
  // stfs f0,8(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 8, temp.u32);
  // stfs f0,4(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 4, temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // cmplw cr6,r4,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r31), so);
  // stw r4,224(r1)
  guest::StoreU32(base, uint32_t(r1) + 224, uint32_t(r4));
  // stw r6,524(r1)
  guest::StoreU32(base, uint32_t(r1) + 524, uint32_t(r6));
  // blt cr6,0x823f4118
  if (cr6.lt) goto loc_823F4118;
  // lwz r3,500(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 500);
loc_823F46F4:
  // lwz r11,24(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 24);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f4710
  if (cr6.eq) goto loc_823F4710;
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 104);
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F4710:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f472c
  if (cr6.eq) goto loc_823F472C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F472C:
  // addi r1,r1,480
  r1 = uint64_t(int64_t(int64_t(r1) + 480));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return r3;
}

// sub_823F4738 game::G16R16FCodec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F4738(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F4738 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0;
  lifted::Vec v0{}, v1{}, v2{}, v3{}, v4{}, v5{}, v6{}, v7{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{}, v28{}, v29{}, v30{}, v31{};
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  lifted::Vec vTemp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-480(r1)
  ea = -480 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r10,100(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r8,104(r3)
  r8 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // mullw r10,r10,r5
  r10 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r5))));
  // lwz r9,32(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // stw r3,500(r1)
  guest::StoreU32(base, uint32_t(r1) + 500, uint32_t(r3));
  // stw r6,524(r1)
  guest::StoreU32(base, uint32_t(r1) + 524, uint32_t(r6));
  // add r10,r11,r10
  r10 = r11 + r10;
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // add r4,r10,r9
  r4 = r10 + r9;
  // add r31,r11,r4
  r31 = r11 + r4;
  // cmplw cr6,r4,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r31), so);
  // stw r4,224(r1)
  guest::StoreU32(base, uint32_t(r1) + 224, uint32_t(r4));
  // bge cr6,0x823f4d60
  if (!cr6.lt) goto loc_823F4D60;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
loc_823F4788:
  // addi r11,r6,127
  r11 = uint64_t(int64_t(int64_t(r6) + 127));
  // addi r5,r6,8
  r5 = uint64_t(int64_t(int64_t(r6) + 8));
  // rlwinm r9,r11,0,0,24
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  // mr r10,r4
  r10 = r4;
  // mr r11,r6
  r11 = r6;
  // cmplw cr6,r5,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r9), so);
  // stw r5,232(r1)
  guest::StoreU32(base, uint32_t(r1) + 232, uint32_t(r5));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // bge cr6,0x823f47b4
  if (!cr6.lt) goto loc_823F47B4;
  // mr r9,r5
  r9 = r5;
loc_823F47B4:
  // subf r8,r9,r5
  r8 = r5 - r9;
  // stw r9,240(r1)
  guest::StoreU32(base, uint32_t(r1) + 240, uint32_t(r9));
  // mr r3,r5
  r3 = r5;
  // rlwinm r8,r8,0,0,22
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0xFFFFFE00;
  // add r8,r8,r9
  r8 = r8 + r9;
  // cmplw cr6,r5,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r8), so);
  // blt cr6,0x823f47d4
  if (cr6.lt) goto loc_823F47D4;
  // mr r3,r8
  r3 = r8;
loc_823F47D4:
  // subf r8,r6,r9
  r8 = r9 - r6;
  // stw r3,228(r1)
  guest::StoreU32(base, uint32_t(r1) + 228, uint32_t(r3));
  // clrlwi r8,r8,27
  r8 = uint32_t(r8) & 0x1F;
  // add r8,r8,r6
  r8 = r8 + r6;
  // cmplw cr6,r5,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r8), so);
  // bge cr6,0x823f47f0
  if (!cr6.lt) goto loc_823F47F0;
  // mr r8,r5
  r8 = r5;
loc_823F47F0:
  // subf r7,r3,r5
  r7 = r5 - r3;
  // rlwinm r7,r7,0,0,26
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 0) & 0xFFFFFFE0;
  // add r7,r7,r3
  r7 = r7 + r3;
  // cmplw cr6,r5,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), uint32_t(r7), so);
  // bge cr6,0x823f4808
  if (!cr6.lt) goto loc_823F4808;
  // mr r7,r5
  r7 = r5;
loc_823F4808:
  // stw r7,236(r1)
  guest::StoreU32(base, uint32_t(r1) + 236, uint32_t(r7));
  // li r30,128
  r30 = uint64_t(int64_t(128));
  // li r30,256
  r30 = uint64_t(int64_t(256));
  // li r30,384
  r30 = uint64_t(int64_t(384));
  // cmplw cr6,r6,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r8), so);
  // bge cr6,0x823f48fc
  if (!cr6.lt) goto loc_823F48FC;
loc_823F4830:
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vsplth v0,v0,0
  simde_mm_store_si128((simde__m128i*)v0.u16, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u16), simde_mm_set1_epi16(short(0xF0E))));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vupkd3d128 v0,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[0] = vTemp.u32[0];
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stvewx v0,r0,r11
  ea = (uint32_t(r11)) & ~0x3;
  guest::StoreU32(base, ea, v0.u32[3 - ((ea & 0xF) >> 2)]);
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // lwz r10,80(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 80);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // blt cr6,0x823f4830
  if (cr6.lt) goto loc_823F4830;
  // lwz r6,524(r1)
  r6 = guest::LoadU32(base, uint32_t(r1) + 524);
  // lwz r4,224(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 224);
  // lwz r5,232(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 232);
  // lwz r9,240(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 240);
  // lwz r3,228(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 228);
  // lwz r7,236(r1)
  r7 = guest::LoadU32(base, uint32_t(r1) + 236);
  // b 0x823f48fc
  goto loc_823F48FC;
loc_823F4894:
  // addi r30,r1,96
  r30 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // stvx v0,r0,r30
  ea = (uint32_t(r30)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvrx v13,r10,r8
  temp.u32 = uint32_t(r10) + uint32_t(r8);
  simde_mm_store_si128((simde__m128i*)v13.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r8,r1,256
  r8 = uint64_t(int64_t(int64_t(r1) + 256));
  // vor v0,v0,v13
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v13.u8)));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // vsldoi v13,v0,v0,8
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v0.u8), 8));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // vupkd3d128 v0,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[0] = vTemp.u32[0];
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vupkd3d128 v13,v13,20
  temp.u32 = v13.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[3] = vTemp.u32[0];
  temp.u32 = v13.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[2] = vTemp.u32[0];
  temp.u32 = v13.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[1] = vTemp.u32[0];
  temp.u32 = v13.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[0] = vTemp.u32[0];
  // addi r8,r1,256
  r8 = uint64_t(int64_t(int64_t(r1) + 256));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_823F48FC:
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // blt cr6,0x823f4894
  if (cr6.lt) goto loc_823F4894;
  // b 0x823f4c64
  goto loc_823F4C64;
loc_823F4908:
  // li r9,512
  r9 = uint64_t(int64_t(512));
  // li r9,640
  r9 = uint64_t(int64_t(640));
  // li r9,768
  r9 = uint64_t(int64_t(768));
  // li r9,896
  r9 = uint64_t(int64_t(896));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,384(r1)
  guest::StoreU32(base, uint32_t(r1) + 384, uint32_t(r9));
loc_823F4930:
  // addi r23,r1,96
  r23 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // li r30,16
  r30 = uint64_t(int64_t(16));
  // li r29,16
  r29 = uint64_t(int64_t(16));
  // li r28,16
  r28 = uint64_t(int64_t(16));
  // stvx v0,r0,r23
  ea = (uint32_t(r23)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // li r27,16
  r27 = uint64_t(int64_t(16));
  // lvrx v13,r10,r8
  temp.u32 = uint32_t(r10) + uint32_t(r8);
  simde_mm_store_si128((simde__m128i*)v13.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r8,r1,256
  r8 = uint64_t(int64_t(int64_t(r1) + 256));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // vor v0,v0,v13
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v13.u8)));
  // li r26,16
  r26 = uint64_t(int64_t(16));
  // li r25,16
  r25 = uint64_t(int64_t(16));
  // li r24,16
  r24 = uint64_t(int64_t(16));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v12,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,272
  r8 = uint64_t(int64_t(int64_t(r1) + 272));
  // lvrx v13,r10,r30
  temp.u32 = uint32_t(r10) + uint32_t(r30);
  simde_mm_store_si128((simde__m128i*)v13.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v11,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // vor v13,v12,v13
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)v13.u8)));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,288
  r8 = uint64_t(int64_t(int64_t(r1) + 288));
  // lvrx v12,r10,r29
  temp.u32 = uint32_t(r10) + uint32_t(r29);
  simde_mm_store_si128((simde__m128i*)v12.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v10,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // vor v12,v11,v12
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v12.u8)));
  // stvx v10,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,304
  r8 = uint64_t(int64_t(int64_t(r1) + 304));
  // lvrx v11,r10,r28
  temp.u32 = uint32_t(r10) + uint32_t(r28);
  simde_mm_store_si128((simde__m128i*)v11.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,160
  r8 = uint64_t(int64_t(int64_t(r1) + 160));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v9,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // stvx v9,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,320
  r8 = uint64_t(int64_t(int64_t(r1) + 320));
  // lvrx v8,r10,r27
  temp.u32 = uint32_t(r10) + uint32_t(r27);
  simde_mm_store_si128((simde__m128i*)v8.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v8,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,176
  r8 = uint64_t(int64_t(int64_t(r1) + 176));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v7,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v7.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // stvx v7,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,336
  r8 = uint64_t(int64_t(int64_t(r1) + 336));
  // lvrx v6,r10,r26
  temp.u32 = uint32_t(r10) + uint32_t(r26);
  simde_mm_store_si128((simde__m128i*)v6.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v6,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v6.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,192
  r8 = uint64_t(int64_t(int64_t(r1) + 192));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v5,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v5.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // stvx v5,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v5.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,352
  r8 = uint64_t(int64_t(int64_t(r1) + 352));
  // lvrx v4,r10,r25
  temp.u32 = uint32_t(r10) + uint32_t(r25);
  simde_mm_store_si128((simde__m128i*)v4.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v4,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v4.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,208
  r8 = uint64_t(int64_t(int64_t(r1) + 208));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // lvlx v3,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v3.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // stvx v3,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v3.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvrx v2,r10,r24
  temp.u32 = uint32_t(r10) + uint32_t(r24);
  simde_mm_store_si128((simde__m128i*)v2.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,368
  r8 = uint64_t(int64_t(int64_t(r1) + 368));
  // stvx v2,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v2.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // vor v11,v10,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)v11.u8)));
  // vor v10,v9,v8
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)v8.u8)));
  // vor v9,v7,v6
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)v6.u8)));
  // vupkd3d128 v6,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v6.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v6.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v6.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v6.u32[0] = vTemp.u32[0];
  // vor v8,v5,v4
  simde_mm_store_si128((simde__m128i*)v8.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v5.u8), simde_mm_load_si128((simde__m128i*)v4.u8)));
  // vupkd3d128 v5,v13,20
  temp.u32 = v13.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v5.u32[3] = vTemp.u32[0];
  temp.u32 = v13.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v5.u32[2] = vTemp.u32[0];
  temp.u32 = v13.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v5.u32[1] = vTemp.u32[0];
  temp.u32 = v13.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v5.u32[0] = vTemp.u32[0];
  // vor v7,v3,v2
  simde_mm_store_si128((simde__m128i*)v7.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v3.u8), simde_mm_load_si128((simde__m128i*)v2.u8)));
  // vupkd3d128 v4,v12,20
  temp.u32 = v12.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v4.u32[3] = vTemp.u32[0];
  temp.u32 = v12.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v4.u32[2] = vTemp.u32[0];
  temp.u32 = v12.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v4.u32[1] = vTemp.u32[0];
  temp.u32 = v12.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v4.u32[0] = vTemp.u32[0];
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,160
  r8 = uint64_t(int64_t(int64_t(r1) + 160));
  // vsldoi v0,v0,v0,8
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v0.u8), 8));
  // vsldoi v13,v13,v13,8
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)v13.u8), 8));
  // vsldoi v12,v12,v12,8
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)v12.u8), 8));
  // vupkd3d128 v3,v11,20
  temp.u32 = v11.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v3.u32[3] = vTemp.u32[0];
  temp.u32 = v11.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v3.u32[2] = vTemp.u32[0];
  temp.u32 = v11.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v3.u32[1] = vTemp.u32[0];
  temp.u32 = v11.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v3.u32[0] = vTemp.u32[0];
  // stvx v10,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,176
  r8 = uint64_t(int64_t(int64_t(r1) + 176));
  // vupkd3d128 v2,v10,20
  temp.u32 = v10.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v2.u32[3] = vTemp.u32[0];
  temp.u32 = v10.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v2.u32[2] = vTemp.u32[0];
  temp.u32 = v10.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v2.u32[1] = vTemp.u32[0];
  temp.u32 = v10.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v2.u32[0] = vTemp.u32[0];
  // vupkd3d128 v1,v9,20
  temp.u32 = v9.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v1.u32[3] = vTemp.u32[0];
  temp.u32 = v9.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v1.u32[2] = vTemp.u32[0];
  temp.u32 = v9.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v1.u32[1] = vTemp.u32[0];
  temp.u32 = v9.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v1.u32[0] = vTemp.u32[0];
  // vupkd3d128 v31,v8,20
  temp.u32 = v8.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v31.u32[3] = vTemp.u32[0];
  temp.u32 = v8.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v31.u32[2] = vTemp.u32[0];
  temp.u32 = v8.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v31.u32[1] = vTemp.u32[0];
  temp.u32 = v8.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v31.u32[0] = vTemp.u32[0];
  // vupkd3d128 v30,v7,20
  temp.u32 = v7.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v30.u32[3] = vTemp.u32[0];
  temp.u32 = v7.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v30.u32[2] = vTemp.u32[0];
  temp.u32 = v7.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v30.u32[1] = vTemp.u32[0];
  temp.u32 = v7.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v30.u32[0] = vTemp.u32[0];
  // stvx v9,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,192
  r8 = uint64_t(int64_t(int64_t(r1) + 192));
  // vsldoi v29,v11,v11,8
  simde_mm_store_si128((simde__m128i*)v29.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v11.u8), 8));
  // vsldoi v28,v10,v10,8
  simde_mm_store_si128((simde__m128i*)v28.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)v10.u8), 8));
  // vsldoi v11,v9,v9,8
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)v9.u8), 8));
  // vsldoi v10,v8,v8,8
  simde_mm_store_si128((simde__m128i*)v10.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)v8.u8), 8));
  // stvx v8,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,208
  r8 = uint64_t(int64_t(int64_t(r1) + 208));
  // vsldoi v9,v7,v7,8
  simde_mm_store_si128((simde__m128i*)v9.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)v7.u8), 8));
  // vupkd3d128 v8,v29,20
  temp.u32 = v29.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v8.u32[3] = vTemp.u32[0];
  temp.u32 = v29.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v8.u32[2] = vTemp.u32[0];
  temp.u32 = v29.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v8.u32[1] = vTemp.u32[0];
  temp.u32 = v29.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v8.u32[0] = vTemp.u32[0];
  // stvx v7,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,256
  r8 = uint64_t(int64_t(int64_t(r1) + 256));
  // vupkd3d128 v7,v28,20
  temp.u32 = v28.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v7.u32[3] = vTemp.u32[0];
  temp.u32 = v28.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v7.u32[2] = vTemp.u32[0];
  temp.u32 = v28.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v7.u32[1] = vTemp.u32[0];
  temp.u32 = v28.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v7.u32[0] = vTemp.u32[0];
  // stvx v6,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v6.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,272
  r8 = uint64_t(int64_t(int64_t(r1) + 272));
  // stvx v5,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v5.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,288
  r8 = uint64_t(int64_t(int64_t(r1) + 288));
  // stvx v4,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v4.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // vupkd3d128 v0,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[0] = vTemp.u32[0];
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // vupkd3d128 v13,v13,20
  temp.u32 = v13.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[3] = vTemp.u32[0];
  temp.u32 = v13.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[2] = vTemp.u32[0];
  temp.u32 = v13.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[1] = vTemp.u32[0];
  temp.u32 = v13.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[0] = vTemp.u32[0];
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,304
  r8 = uint64_t(int64_t(int64_t(r1) + 304));
  // vupkd3d128 v12,v12,20
  temp.u32 = v12.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v12.u32[3] = vTemp.u32[0];
  temp.u32 = v12.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v12.u32[2] = vTemp.u32[0];
  temp.u32 = v12.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v12.u32[1] = vTemp.u32[0];
  temp.u32 = v12.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v12.u32[0] = vTemp.u32[0];
  // stvx v6,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v6.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v3,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v3.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,320
  r8 = uint64_t(int64_t(int64_t(r1) + 320));
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v2,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v2.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,336
  r8 = uint64_t(int64_t(int64_t(r1) + 336));
  // stvx v5,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v5.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v1,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v1.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,352
  r8 = uint64_t(int64_t(int64_t(r1) + 352));
  // stvx v12,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v31,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v31.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,368
  r8 = uint64_t(int64_t(int64_t(r1) + 368));
  // stvx v4,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v4.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v30,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v30.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // stvx v29,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v29.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,160
  r8 = uint64_t(int64_t(int64_t(r1) + 160));
  // vupkd3d128 v29,v9,20
  temp.u32 = v9.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v29.u32[3] = vTemp.u32[0];
  temp.u32 = v9.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v29.u32[2] = vTemp.u32[0];
  temp.u32 = v9.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v29.u32[1] = vTemp.u32[0];
  temp.u32 = v9.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v29.u32[0] = vTemp.u32[0];
  // stvx v28,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v28.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,176
  r8 = uint64_t(int64_t(int64_t(r1) + 176));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,192
  r8 = uint64_t(int64_t(int64_t(r1) + 192));
  // vupkd3d128 v11,v11,20
  temp.u32 = v11.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v11.u32[3] = vTemp.u32[0];
  temp.u32 = v11.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v11.u32[2] = vTemp.u32[0];
  temp.u32 = v11.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v11.u32[1] = vTemp.u32[0];
  temp.u32 = v11.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v11.u32[0] = vTemp.u32[0];
  // stvx v10,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vupkd3d128 v10,v10,20
  temp.u32 = v10.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v10.u32[3] = vTemp.u32[0];
  temp.u32 = v10.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v10.u32[2] = vTemp.u32[0];
  temp.u32 = v10.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v10.u32[1] = vTemp.u32[0];
  temp.u32 = v10.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v10.u32[0] = vTemp.u32[0];
  // stvx v8,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // addi r8,r1,208
  r8 = uint64_t(int64_t(int64_t(r1) + 208));
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmplwi cr6,r9,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 4, so);
  // stvx v3,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v3.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v9,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v9.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r9,384(r1)
  guest::StoreU32(base, uint32_t(r1) + 384, uint32_t(r9));
  // stvx v7,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,112
  r8 = uint64_t(int64_t(int64_t(r1) + 112));
  // stvx v2,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v2.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stvx v11,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v12,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v12.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // stvx v1,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v1.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v8,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v8.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,160
  r8 = uint64_t(int64_t(int64_t(r1) + 160));
  // stvx v10,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v7,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v7.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,176
  r8 = uint64_t(int64_t(int64_t(r1) + 176));
  // stvx v31,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v31.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v11,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,192
  r8 = uint64_t(int64_t(int64_t(r1) + 192));
  // stvx v29,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v29.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v10,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v10.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,208
  r8 = uint64_t(int64_t(int64_t(r1) + 208));
  // stvx v30,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v30.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v29,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v29.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // blt cr6,0x823f4930
  if (cr6.lt) goto loc_823F4930;
loc_823F4C64:
  // cmplw cr6,r11,r3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r3), so);
  // blt cr6,0x823f4908
  if (cr6.lt) goto loc_823F4908;
  // b 0x823f4cd8
  goto loc_823F4CD8;
loc_823F4C70:
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvrx v13,r10,r9
  temp.u32 = uint32_t(r10) + uint32_t(r9);
  simde_mm_store_si128((simde__m128i*)v13.u8, temp.u32 & 0xF ? simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskR[(temp.u32 & 0xF) * 16])) : simde_mm_setzero_si128());
  // addi r9,r1,256
  r9 = uint64_t(int64_t(int64_t(r1) + 256));
  // vor v0,v0,v13
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_or_si128(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v13.u8)));
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stvx v13,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // vsldoi v13,v0,v0,8
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_alignr_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)v0.u8), 8));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stvx v0,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // vupkd3d128 v0,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[0] = vTemp.u32[0];
  // stvx v13,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vupkd3d128 v13,v13,20
  temp.u32 = v13.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[3] = vTemp.u32[0];
  temp.u32 = v13.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[2] = vTemp.u32[0];
  temp.u32 = v13.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[1] = vTemp.u32[0];
  temp.u32 = v13.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v13.u32[0] = vTemp.u32[0];
  // addi r9,r1,256
  r9 = uint64_t(int64_t(int64_t(r1) + 256));
  // stvx v0,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stvx v13,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
loc_823F4CD8:
  // cmplw cr6,r11,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r7), so);
  // blt cr6,0x823f4c70
  if (cr6.lt) goto loc_823F4C70;
  // cmplw cr6,r11,r5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r5), so);
  // bge cr6,0x823f4d3c
  if (!cr6.lt) goto loc_823F4D3C;
loc_823F4CE8:
  // lvlx v0,0,r10
  temp.u32 = uint32_t(r10);
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)guest::Pointer(base, temp.u32 & ~0xF)), simde_mm_load_si128((simde__m128i*)&VectorMaskL[(temp.u32 & 0xF) * 16])));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vsplth v0,v0,0
  simde_mm_store_si128((simde__m128i*)v0.u16, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u16), simde_mm_set1_epi16(short(0xF0E))));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vupkd3d128 v0,v0,20
  temp.u32 = v0.u16[3];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[3] = vTemp.u32[0];
  temp.u32 = v0.u16[2];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[2] = vTemp.u32[0];
  temp.u32 = v0.u16[1];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[1] = vTemp.u32[0];
  temp.u32 = v0.u16[0];
  vTemp.u32[0] = ((temp.u32 & 0x8000) << 16) | (((temp.u32 & 0x7C00) + 0x1C000) << 13) | ((temp.u32 & 0x03FF) << 13);
  if ((temp.u32 & 0x7C00) == 0) vTemp.u32[0] = (temp.u32 & 0x8000) << 16;
  v0.u32[0] = vTemp.u32[0];
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)guest::Pointer(base, ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stvewx v0,r0,r11
  ea = (uint32_t(r11)) & ~0x3;
  guest::StoreU32(base, ea, v0.u32[3 - ((ea & 0xF) >> 2)]);
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // lwz r10,80(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 80);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // lwz r9,232(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 232);
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // blt cr6,0x823f4ce8
  if (cr6.lt) goto loc_823F4CE8;
  // lwz r6,524(r1)
  r6 = guest::LoadU32(base, uint32_t(r1) + 524);
  // lwz r4,224(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 224);
loc_823F4D3C:
  // stfs f0,12(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 12, temp.u32);
  // addi r4,r4,4
  r4 = uint64_t(int64_t(int64_t(r4) + 4));
  // stfs f0,8(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 8, temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // cmplw cr6,r4,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r31), so);
  // stw r4,224(r1)
  guest::StoreU32(base, uint32_t(r1) + 224, uint32_t(r4));
  // stw r6,524(r1)
  guest::StoreU32(base, uint32_t(r1) + 524, uint32_t(r6));
  // blt cr6,0x823f4788
  if (cr6.lt) goto loc_823F4788;
  // lwz r3,500(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 500);
loc_823F4D60:
  // lwz r11,24(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 24);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f4d7c
  if (cr6.eq) goto loc_823F4D7C;
  // lwz r11,104(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 104);
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F4D7C:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f4d98
  if (cr6.eq) goto loc_823F4D98;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F4D98:
  // addi r1,r1,480
  r1 = uint64_t(int64_t(int64_t(r1) + 480));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return r3;
}

// sub_823F53B8 game::R32FCodec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F53B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F53B8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f5420
  if (!cr6.lt) goto loc_823F5420;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f0 = double(temp.f32);
loc_823F53FC:
  // lfs f13,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stfs f13,0(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // stfs f0,12(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // stfs f0,8(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // stfs f0,4(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f53fc
  if (cr6.lt) goto loc_823F53FC;
loc_823F5420:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f543c
  if (cr6.eq) goto loc_823F543C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F543C:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f5458
  if (cr6.eq) goto loc_823F5458;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F5458:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_823F5468 game::G32R32FCodec::DecodeRow: lifted from generated/ by lift.py.
uint64_t f823F5468(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_823F5468 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r11,96(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).row_pitch_bits();
  // lwz r9,100(r3)
  r9 = schema::ImageCodec(base, uint32_t(r3)).slice_pitch_bits();
  // mullw r11,r11,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r4))));
  // lwz r10,32(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).surface_bits();
  // mullw r9,r9,r5
  r9 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r5))));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r10,104(r3)
  r10 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r10,r10,r11
  r10 = r10 + r11;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x823f54d4
  if (!cr6.lt) goto loc_823F54D4;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 2608);
  f0 = double(temp.f32);
loc_823F54AC:
  // lfs f13,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // stfs f13,0(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).red_address(), temp.u32);
  // lfs f13,4(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4);
  f13 = double(temp.f32);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // stfs f13,4(r6)
  temp.f32 = float(f13);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).green_address(), temp.u32);
  // stfs f0,12(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).alpha_address(), temp.u32);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // stfs f0,8(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::TcFloat4(base, uint32_t(r6)).blue_address(), temp.u32);
  // addi r6,r6,16
  r6 = uint64_t(int64_t(int64_t(r6) + 16));
  // blt cr6,0x823f54ac
  if (cr6.lt) goto loc_823F54AC;
loc_823F54D4:
  // lwz r11,24(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).color_key_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f54f0
  if (cr6.eq) goto loc_823F54F0;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eadb0
  // (left out: dead in C++)
  game::ImageCodec::ApplyColorKey(base, r3, r4);
loc_823F54F0:
  // lwz r11,16(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).srgb_bits();
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x823f550c
  if (cr6.eq) goto loc_823F550C;
  // lwz r11,104(r3)
  r11 = schema::ImageCodec(base, uint32_t(r3)).width_bits();
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // subf r4,r11,r6
  r4 = r6 - r11;
  // bl 0x823eb180
  // (left out: dead in C++)
  r3 = game::ImageCodec::SrgbToLinear(base, r1, r3, r4);
loc_823F550C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_824227F0 game::Dxt23::EncodeBlockStraight: lifted from generated/ by lift.py.
lifted::Result f824227F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_824227F0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x82422000
  // (left out: dead in C++)
  r3 = game::Dxt3A::EncodeBlock(base, r1, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8242282c
  if (cr0.lt) goto loc_8242282C;
  // mr r6,r5
  r6 = r5;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // bl 0x82420f50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82420F50, LIFTED_ARGS(0x8242282C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8242282C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8246B480: lifted from generated/ by lift.py.
lifted::Result f8246B480(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8246B480 (lifted)");
  uint64_t r11 = 0, r12 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e5c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r24,r3
  r24 = r3;
  // li r19,0
  r19 = uint64_t(int64_t(0));
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // mr r26,r19
  r26 = r19;
  // lbz r11,56(r24)
  r11 = guest::LoadU8(base, uint32_t(r24) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246b4ec
  if (cr6.eq) goto loc_8246B4EC;
  // mr r31,r19
  r31 = r19;
loc_8246B4AC:
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // bl 0x8245ca90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CA90(base, LIFTED_ARGS(0x8246B4B8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x8246b550
  if (cr6.eq) goto loc_8246B550;
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // bl 0x8245cae0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CAE0(base, LIFTED_ARGS(0x8246B4CC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x8246b4d8
  if (!cr6.eq) goto loc_8246B4D8;
  // mr r27,r19
  r27 = r19;
loc_8246B4D8:
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // lbz r10,56(r24)
  r10 = guest::LoadU8(base, uint32_t(r24) + 56);
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8246b4ac
  if (cr6.lt) goto loc_8246B4AC;
loc_8246B4EC:
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeRaiseIrqlToDpcLevel>(base, LIFTED_ARGS(0x8246B4F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r30,r3
  r30 = r3;
  // addi r23,r11,20240
  r23 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r31,r13
  r31 = r13;
  // lwz r11,4(r23)
  r11 = guest::LoadU32(base, uint32_t(r23) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b518
  if (cr6.eq) goto loc_8246B518;
  // lwz r10,8(r23)
  r10 = guest::LoadU32(base, uint32_t(r23) + 8);
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // beq cr6,0x8246b52c
  if (cr6.eq) goto loc_8246B52C;
loc_8246B518:
  // mr r3,r23
  r3 = r23;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeAcquireSpinLockAtRaisedIrql>(base, LIFTED_ARGS(0x8246B520, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r31,8(r23)
  guest::StoreU32(base, uint32_t(r23) + 8, uint32_t(r31));
  // stb r30,12(r23)
  guest::StoreU8(base, uint32_t(r23) + 12, uint8_t(r30));
  // lwz r11,4(r23)
  r11 = guest::LoadU32(base, uint32_t(r23) + 4);
loc_8246B52C:
  // addi r9,r11,1
  r9 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r18,r24,20
  r18 = uint64_t(int64_t(int64_t(r24) + 20));
  // stw r9,4(r23)
  guest::StoreU32(base, uint32_t(r23) + 4, uint32_t(r9));
  // lwz r11,0(r18)
  r11 = guest::LoadU32(base, uint32_t(r18) + 0);
  // cmplw cr6,r18,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), uint32_t(r11), so);
  // beq cr6,0x8246b55c
  if (cr6.eq) goto loc_8246B55C;
  // lwz r10,8(r18)
  r10 = guest::LoadU32(base, uint32_t(r18) + 8);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // b 0x8246b560
  goto loc_8246B560;
loc_8246B550:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6eac
  // (left out: dead in C++)
  return {r3, f1};
loc_8246B55C:
  // mr r11,r19
  r11 = r19;
loc_8246B560:
  // mr r29,r11
  r29 = r11;
  // li r17,4
  r17 = uint64_t(int64_t(4));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246b788
  if (cr6.eq) goto loc_8246B788;
loc_8246B570:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 0);
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // beq cr6,0x8246b788
  if (cr6.eq) goto loc_8246B788;
  // lwz r11,8(r18)
  r11 = guest::LoadU32(base, uint32_t(r18) + 8);
  // lwzx r10,r11,r29
  r10 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r29));
  // cmplw cr6,r18,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), uint32_t(r10), so);
  // subf r28,r11,r10
  r28 = r10 - r11;
  // bne cr6,0x8246b594
  if (!cr6.eq) goto loc_8246B594;
  // mr r28,r19
  r28 = r19;
loc_8246B594:
  // lbz r11,116(r29)
  r11 = guest::LoadU8(base, uint32_t(r29) + 116);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246b61c
  if (cr6.eq) goto loc_8246B61C;
  // lbz r11,56(r24)
  r11 = guest::LoadU8(base, uint32_t(r24) + 56);
  // mr r30,r19
  r30 = r19;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246b604
  if (cr6.eq) goto loc_8246B604;
  // mr r31,r19
  r31 = r19;
loc_8246B5B4:
  // mr r4,r31
  r4 = r31;
  // lwz r5,8(r29)
  r5 = guest::LoadU32(base, uint32_t(r29) + 8);
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // bl 0x8245ca30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CA30(base, LIFTED_ARGS(0x8246B5C4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x8246b5e4
  if (cr6.eq) goto loc_8246B5E4;
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // bl 0x8245cf60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CF60(base, LIFTED_ARGS(0x8246B5D8, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x8;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246b5fc
  if (cr6.eq) goto loc_8246B5FC;
loc_8246B5E4:
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // lbz r10,56(r24)
  r10 = guest::LoadU8(base, uint32_t(r24) + 56);
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8246b5b4
  if (cr6.lt) goto loc_8246B5B4;
  // b 0x8246b600
  goto loc_8246B600;
loc_8246B5FC:
  // li r30,1
  r30 = uint64_t(int64_t(1));
loc_8246B600:
  // lwz r9,4(r23)
  r9 = guest::LoadU32(base, uint32_t(r23) + 4);
loc_8246B604:
  // clrlwi r11,r30,24
  r11 = uint32_t(r30) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8246b620
  if (!cr6.eq) goto loc_8246B620;
  // stb r17,116(r29)
  guest::StoreU8(base, uint32_t(r29) + 116, uint8_t(r17));
  // lwz r9,4(r23)
  r9 = guest::LoadU32(base, uint32_t(r23) + 4);
  // b 0x8246b620
  goto loc_8246B620;
loc_8246B61C:
  // li r26,1
  r26 = uint64_t(int64_t(1));
loc_8246B620:
  // lbz r11,116(r29)
  r11 = guest::LoadU8(base, uint32_t(r29) + 116);
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bne cr6,0x8246b77c
  if (!cr6.eq) goto loc_8246B77C;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x8246b650
  if (!cr6.eq) goto loc_8246B650;
  // lbz r11,172(r24)
  r11 = guest::LoadU8(base, uint32_t(r24) + 172);
  // clrlwi r10,r11,31
  r10 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8246b650
  if (cr6.eq) goto loc_8246B650;
  // rlwinm r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246b77c
  if (cr6.eq) goto loc_8246B77C;
loc_8246B650:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x8246b698
  if (cr6.eq) goto loc_8246B698;
  // lwz r10,8(r23)
  r10 = guest::LoadU32(base, uint32_t(r23) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8246b698
  if (!cr6.eq) goto loc_8246B698;
  // addi r11,r9,-1
  r11 = uint64_t(int64_t(int64_t(r9) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r23)
  guest::StoreU32(base, uint32_t(r23) + 4, uint32_t(r11));
  // bne cr6,0x8246b698
  if (!cr6.eq) goto loc_8246B698;
  // mr r11,r19
  r11 = r19;
  // lbz r31,12(r23)
  r31 = guest::LoadU8(base, uint32_t(r23) + 12);
  // mr r3,r23
  r3 = r23;
  // stb r11,12(r23)
  guest::StoreU8(base, uint32_t(r23) + 12, uint8_t(r11));
  // stw r11,8(r23)
  guest::StoreU32(base, uint32_t(r23) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeReleaseSpinLockFromRaisedIrql>(base, LIFTED_ARGS(0x8246B690, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KfLowerIrql>(base, LIFTED_ARGS(0x8246B698, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246B698:
  // addi r31,r24,268
  r31 = uint64_t(int64_t(int64_t(r24) + 268));
  // mr r10,r13
  r10 = r13;
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b6e0
  if (cr6.eq) goto loc_8246B6E0;
  // lwz r9,8(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x8246b6e0
  if (!cr6.eq) goto loc_8246B6E0;
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bne 0x8246b6e0
  if (!cr0.eq) goto loc_8246B6E0;
  // mr r3,r31
  r3 = r31;
  // lbz r30,12(r31)
  r30 = guest::LoadU8(base, uint32_t(r31) + 12);
  // stw r19,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r19));
  // stb r19,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r19));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeReleaseSpinLockFromRaisedIrql>(base, LIFTED_ARGS(0x8246B6D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KfLowerIrql>(base, LIFTED_ARGS(0x8246B6E0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246B6E0:
  // addi r3,r24,16
  r3 = uint64_t(int64_t(int64_t(r24) + 16));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r29
  r4 = r29;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,92(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 92);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8246B6FC, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeRaiseIrqlToDpcLevel>(base, LIFTED_ARGS(0x8246B700, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // mr r29,r3
  r29 = r3;
  // mr r30,r13
  r30 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b720
  if (cr6.eq) goto loc_8246B720;
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // beq cr6,0x8246b730
  if (cr6.eq) goto loc_8246B730;
loc_8246B720:
  // mr r3,r31
  r3 = r31;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeAcquireSpinLockAtRaisedIrql>(base, LIFTED_ARGS(0x8246B728, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r30,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r30));
  // stb r29,12(r31)
  guest::StoreU8(base, uint32_t(r31) + 12, uint8_t(r29));
loc_8246B730:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeRaiseIrqlToDpcLevel>(base, LIFTED_ARGS(0x8246B740, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r23)
  r11 = guest::LoadU32(base, uint32_t(r23) + 4);
  // mr r30,r3
  r30 = r3;
  // mr r31,r13
  r31 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b760
  if (cr6.eq) goto loc_8246B760;
  // lwz r10,8(r23)
  r10 = guest::LoadU32(base, uint32_t(r23) + 8);
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // beq cr6,0x8246b774
  if (cr6.eq) goto loc_8246B774;
loc_8246B760:
  // mr r3,r23
  r3 = r23;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeAcquireSpinLockAtRaisedIrql>(base, LIFTED_ARGS(0x8246B768, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r31,8(r23)
  guest::StoreU32(base, uint32_t(r23) + 8, uint32_t(r31));
  // stb r30,12(r23)
  guest::StoreU8(base, uint32_t(r23) + 12, uint8_t(r30));
  // lwz r11,4(r23)
  r11 = guest::LoadU32(base, uint32_t(r23) + 4);
loc_8246B774:
  // addi r9,r11,1
  r9 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r9,4(r23)
  guest::StoreU32(base, uint32_t(r23) + 4, uint32_t(r9));
loc_8246B77C:
  // mr r29,r28
  r29 = r28;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x8246b570
  if (!cr6.eq) goto loc_8246B570;
loc_8246B788:
  // lbz r11,266(r24)
  r11 = guest::LoadU8(base, uint32_t(r24) + 266);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x8246b7a4
  if (!cr6.eq) goto loc_8246B7A4;
  // clrlwi r11,r27,24
  r11 = uint32_t(r27) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8246b7a4
  if (!cr6.eq) goto loc_8246B7A4;
  // mr r26,r19
  r26 = r19;
loc_8246B7A4:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x8246b7ec
  if (cr6.eq) goto loc_8246B7EC;
  // lwz r10,8(r23)
  r10 = guest::LoadU32(base, uint32_t(r23) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8246b7ec
  if (!cr6.eq) goto loc_8246B7EC;
  // addi r11,r9,-1
  r11 = uint64_t(int64_t(int64_t(r9) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r23)
  guest::StoreU32(base, uint32_t(r23) + 4, uint32_t(r11));
  // bne cr6,0x8246b7ec
  if (!cr6.eq) goto loc_8246B7EC;
  // mr r11,r19
  r11 = r19;
  // lbz r31,12(r23)
  r31 = guest::LoadU8(base, uint32_t(r23) + 12);
  // mr r3,r23
  r3 = r23;
  // stb r11,12(r23)
  guest::StoreU8(base, uint32_t(r23) + 12, uint8_t(r11));
  // stw r11,8(r23)
  guest::StoreU32(base, uint32_t(r23) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeReleaseSpinLockFromRaisedIrql>(base, LIFTED_ARGS(0x8246B7E4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KfLowerIrql>(base, LIFTED_ARGS(0x8246B7EC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246B7EC:
  // clrlwi r11,r26,24
  r11 = uint32_t(r26) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246badc
  if (cr6.eq) goto loc_8246BADC;
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeRaiseIrqlToDpcLevel>(base, LIFTED_ARGS(0x8246B7FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r23)
  r11 = guest::LoadU32(base, uint32_t(r23) + 4);
  // mr r30,r3
  r30 = r3;
  // mr r31,r13
  r31 = r13;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8246b81c
  if (cr6.eq) goto loc_8246B81C;
  // lwz r9,8(r23)
  r9 = guest::LoadU32(base, uint32_t(r23) + 8);
  // cmplw cr6,r31,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r9), so);
  // beq cr6,0x8246b834
  if (cr6.eq) goto loc_8246B834;
loc_8246B81C:
  // mr r3,r23
  r3 = r23;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeAcquireSpinLockAtRaisedIrql>(base, LIFTED_ARGS(0x8246B824, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r9,r31
  r9 = r31;
  // stw r9,8(r23)
  guest::StoreU32(base, uint32_t(r23) + 8, uint32_t(r9));
  // stb r30,12(r23)
  guest::StoreU8(base, uint32_t(r23) + 12, uint8_t(r30));
  // lwz r11,4(r23)
  r11 = guest::LoadU32(base, uint32_t(r23) + 4);
loc_8246B834:
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r10,4(r23)
  guest::StoreU32(base, uint32_t(r23) + 4, uint32_t(r10));
  // lwz r11,0(r18)
  r11 = guest::LoadU32(base, uint32_t(r18) + 0);
  // cmplw cr6,r18,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), uint32_t(r11), so);
  // beq cr6,0x8246b854
  if (cr6.eq) goto loc_8246B854;
  // lwz r8,8(r18)
  r8 = guest::LoadU32(base, uint32_t(r18) + 8);
  // subf r27,r8,r11
  r27 = r11 - r8;
  // b 0x8246b858
  goto loc_8246B858;
loc_8246B854:
  // mr r27,r19
  r27 = r19;
loc_8246B858:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8246ba98
  if (cr6.eq) goto loc_8246BA98;
  // li r20,3
  r20 = uint64_t(int64_t(3));
loc_8246B864:
  // lwz r11,0(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 0);
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // beq cr6,0x8246ba90
  if (cr6.eq) goto loc_8246BA90;
  // lwz r11,8(r18)
  r11 = guest::LoadU32(base, uint32_t(r18) + 8);
  // lwzx r10,r11,r27
  r10 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r27));
  // cmplw cr6,r18,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), uint32_t(r10), so);
  // subf r21,r11,r10
  r21 = r10 - r11;
  // bne cr6,0x8246b888
  if (!cr6.eq) goto loc_8246B888;
  // mr r21,r19
  r21 = r19;
loc_8246B888:
  // lbz r11,116(r27)
  r11 = guest::LoadU8(base, uint32_t(r27) + 116);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8246ba84
  if (!cr6.eq) goto loc_8246BA84;
  // lbz r11,56(r24)
  r11 = guest::LoadU8(base, uint32_t(r24) + 56);
  // li r22,1
  r22 = uint64_t(int64_t(1));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246ba74
  if (cr6.eq) goto loc_8246BA74;
  // mr r31,r19
  r31 = r19;
loc_8246B8A8:
  // lbz r11,266(r24)
  r11 = guest::LoadU8(base, uint32_t(r24) + 266);
  // mr r30,r19
  r30 = r19;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x8246b8c0
  if (!cr6.eq) goto loc_8246B8C0;
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x8246b8d8
  goto loc_8246B8D8;
loc_8246B8C0:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8246b8d8
  if (cr6.eq) goto loc_8246B8D8;
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // bl 0x8245cae0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CAE0(base, LIFTED_ARGS(0x8246B8D4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_8246B8D8:
  // mr r4,r31
  r4 = r31;
  // lwz r6,12(r27)
  r6 = guest::LoadU32(base, uint32_t(r27) + 12);
  // lwz r5,8(r27)
  r5 = guest::LoadU32(base, uint32_t(r27) + 8);
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // bl 0x8245c958
  // (left out: dead in C++)
  { const lifted::Result o = game::XmaObject::SetInputBuffer(base, LIFTED_ARGS(0x8246B8EC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x8246b938
  if (cr6.eq) goto loc_8246B938;
  // lbz r11,266(r24)
  r11 = guest::LoadU8(base, uint32_t(r24) + 266);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x8246b920
  if (!cr6.eq) goto loc_8246B920;
  // mr r5,r31
  r5 = r31;
  // lwz r6,28(r27)
  r6 = guest::LoadU32(base, uint32_t(r27) + 28);
  // lwz r4,12(r27)
  r4 = guest::LoadU32(base, uint32_t(r27) + 12);
  // lwz r3,8(r27)
  r3 = guest::LoadU32(base, uint32_t(r27) + 8);
  // bl 0x8246a5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8246A5F8(base, LIFTED_ARGS(0x8246B914, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // rlwinm r6,r6,25,30,31
  r6 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 25) & 0x3;
  // b 0x8246b92c
  goto loc_8246B92C;
loc_8246B920:
  // rlwinm r11,r31,14,0,17
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 14) & 0xFFFFC000;
  // mr r6,r19
  r6 = r19;
  // addi r5,r11,32
  r5 = uint64_t(int64_t(int64_t(r11) + 32));
loc_8246B92C:
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // bl 0x8245d0e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D0E0(base, LIFTED_ARGS(0x8246B938, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246B938:
  // lwz r10,16(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 16);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8246ba3c
  if (cr6.eq) goto loc_8246BA3C;
  // addi r11,r1,84
  r11 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r19,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r19));
  // lbz r9,266(r24)
  r9 = guest::LoadU8(base, uint32_t(r24) + 266);
  // cmplwi cr6,r9,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 2, so);
  // stw r19,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r19));
  // stw r19,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r19));
  // stb r10,90(r1)
  guest::StoreU8(base, uint32_t(r1) + 90, uint8_t(r10));
  // bne cr6,0x8246b9ec
  if (!cr6.eq) goto loc_8246B9EC;
  // lwz r11,24(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 24);
  // mr r5,r31
  r5 = r31;
  // lwz r30,12(r27)
  r30 = guest::LoadU32(base, uint32_t(r27) + 12);
  // lwz r29,8(r27)
  r29 = guest::LoadU32(base, uint32_t(r27) + 8);
  // addi r26,r11,-128
  r26 = uint64_t(int64_t(int64_t(r11) + -128));
  // mr r4,r30
  r4 = r30;
  // mr r6,r26
  r6 = r26;
  // mr r3,r29
  r3 = r29;
  // bl 0x8246a5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8246A5F8(base, LIFTED_ARGS(0x8246B988, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r6,20(r27)
  r6 = guest::LoadU32(base, uint32_t(r27) + 20);
  // mr r25,r3
  r25 = r3;
  // rlwinm r28,r6,25,30,31
  r28 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 25) & 0x3;
  // mr r5,r31
  r5 = r31;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // bne cr6,0x8246b9d0
  if (!cr6.eq) goto loc_8246B9D0;
  // cmplwi cr6,r6,512
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 512, so);
  // blt cr6,0x8246b9c0
  if (cr6.lt) goto loc_8246B9C0;
  // addi r6,r6,-512
  r6 = uint64_t(int64_t(int64_t(r6) + -512));
  // bl 0x8246a5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8246A5F8(base, LIFTED_ARGS(0x8246B9B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r17
  r28 = r17;
  // b 0x8246b9d4
  goto loc_8246B9D4;
loc_8246B9C0:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // bl 0x8246a5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8246A5F8(base, LIFTED_ARGS(0x8246B9C8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r19
  r28 = r19;
  // b 0x8246b9d4
  goto loc_8246B9D4;
loc_8246B9D0:
  // bl 0x8246a5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8246A5F8(base, LIFTED_ARGS(0x8246B9D4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246B9D4:
  // rlwinm r11,r26,25,30,31
  r11 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 25) & 0x3;
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // stw r25,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r25));
  // stb r28,89(r1)
  guest::StoreU8(base, uint32_t(r1) + 89, uint8_t(r28));
  // stb r11,88(r1)
  guest::StoreU8(base, uint32_t(r1) + 88, uint8_t(r11));
  // b 0x8246ba2c
  goto loc_8246BA2C;
loc_8246B9EC:
  // rlwinm r11,r31,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 1) & 0xFFFFFFFE;
  // addi r10,r31,4
  r10 = uint64_t(int64_t(int64_t(r31) + 4));
  // add r11,r31,r11
  r11 = r31 + r11;
  // rlwinm r9,r10,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r9
  r10 = r10 + r9;
  // add r11,r11,r27
  r11 = r11 + r27;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwz r9,40(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 40);
  // lbzx r10,r10,r27
  r10 = guest::LoadU8(base, uint32_t(r10) + uint32_t(r27));
  // stw r9,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r9));
  // lwz r9,44(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 44);
  // lbz r11,49(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 49);
  // stb r10,88(r1)
  guest::StoreU8(base, uint32_t(r1) + 88, uint8_t(r10));
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // stb r11,89(r1)
  guest::StoreU8(base, uint32_t(r1) + 89, uint8_t(r11));
loc_8246BA2C:
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // mr r4,r31
  r4 = r31;
  // bl 0x8245cf98
  // (left out: dead in C++)
  { const lifted::Result o = game::XmaObject::SetLoopData(base, LIFTED_ARGS(0x8246BA3C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246BA3C:
  // lbz r11,266(r24)
  r11 = guest::LoadU8(base, uint32_t(r24) + 266);
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x8246ba5c
  if (cr6.eq) goto loc_8246BA5C;
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // bl 0x8245ca90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CA90(base, LIFTED_ARGS(0x8246BA54, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x8246ba60
  if (!cr6.eq) goto loc_8246BA60;
loc_8246BA5C:
  // mr r22,r19
  r22 = r19;
loc_8246BA60:
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // lbz r10,56(r24)
  r10 = guest::LoadU8(base, uint32_t(r24) + 56);
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8246b8a8
  if (cr6.lt) goto loc_8246B8A8;
loc_8246BA74:
  // clrlwi r11,r22,24
  r11 = uint32_t(r22) & 0xFF;
  // stb r20,116(r27)
  guest::StoreU8(base, uint32_t(r27) + 116, uint8_t(r20));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246ba90
  if (cr6.eq) goto loc_8246BA90;
loc_8246BA84:
  // mr r27,r21
  r27 = r21;
  // cmplwi cr6,r21,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r21), 0, so);
  // bne cr6,0x8246b864
  if (!cr6.eq) goto loc_8246B864;
loc_8246BA90:
  // lwz r9,8(r23)
  r9 = guest::LoadU32(base, uint32_t(r23) + 8);
  // lwz r10,4(r23)
  r10 = guest::LoadU32(base, uint32_t(r23) + 4);
loc_8246BA98:
  // mr r11,r13
  r11 = r13;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8246badc
  if (cr6.eq) goto loc_8246BADC;
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x8246badc
  if (!cr6.eq) goto loc_8246BADC;
  // addi r11,r10,-1
  r11 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r23)
  guest::StoreU32(base, uint32_t(r23) + 4, uint32_t(r11));
  // bne cr6,0x8246badc
  if (!cr6.eq) goto loc_8246BADC;
  // mr r11,r19
  r11 = r19;
  // lbz r31,12(r23)
  r31 = guest::LoadU8(base, uint32_t(r23) + 12);
  // mr r3,r23
  r3 = r23;
  // stb r11,12(r23)
  guest::StoreU8(base, uint32_t(r23) + 12, uint8_t(r11));
  // stw r11,8(r23)
  guest::StoreU32(base, uint32_t(r23) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeReleaseSpinLockFromRaisedIrql>(base, LIFTED_ARGS(0x8246BAD4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KfLowerIrql>(base, LIFTED_ARGS(0x8246BADC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246BADC:
  // lwz r11,120(r24)
  r11 = guest::LoadU32(base, uint32_t(r24) + 120);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bb38
  if (cr6.eq) goto loc_8246BB38;
  // lbz r11,56(r24)
  r11 = guest::LoadU8(base, uint32_t(r24) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bb34
  if (cr6.eq) goto loc_8246BB34;
  // mr r31,r19
  r31 = r19;
loc_8246BAF8:
  // rlwinm r11,r31,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 3) & 0xFFFFFFF8;
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // addi r10,r31,15
  r10 = uint64_t(int64_t(int64_t(r31) + 15));
  // add r11,r11,r24
  r11 = r11 + r24;
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // mr r4,r31
  r4 = r31;
  // lhz r11,124(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 124);
  // lwzx r5,r10,r24
  r5 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r24));
  // rlwinm r6,r11,25,7,31
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 25) & 0x1FFFFFF;
  // bl 0x8245d0e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D0E0(base, LIFTED_ARGS(0x8246BB20, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // lbz r10,56(r24)
  r10 = guest::LoadU8(base, uint32_t(r24) + 56);
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8246baf8
  if (cr6.lt) goto loc_8246BAF8;
loc_8246BB34:
  // stw r19,120(r24)
  guest::StoreU32(base, uint32_t(r24) + 120, uint32_t(r19));
loc_8246BB38:
  // lbz r11,56(r24)
  r11 = guest::LoadU8(base, uint32_t(r24) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bb88
  if (cr6.eq) goto loc_8246BB88;
  // mr r31,r19
  r31 = r19;
loc_8246BB48:
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // bl 0x8245cae0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CAE0(base, LIFTED_ARGS(0x8246BB54, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x8246bb74
  if (!cr6.eq) goto loc_8246BB74;
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r24)
  r3 = guest::LoadU32(base, uint32_t(r24) + 184);
  // bl 0x8245cf60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CF60(base, LIFTED_ARGS(0x8246BB68, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 0) & 0x8;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246b550
  if (cr6.eq) goto loc_8246B550;
loc_8246BB74:
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // lbz r10,56(r24)
  r10 = guest::LoadU8(base, uint32_t(r24) + 56);
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8246bb48
  if (cr6.lt) goto loc_8246BB48;
loc_8246BB88:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6eac
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8246BB98: lifted from generated/ by lift.py.
lifted::Result f8246BB98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8246BB98 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r3
  r28 = r3;
  // li r23,1
  r23 = uint64_t(int64_t(1));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // mr r29,r23
  r29 = r23;
  // lbz r11,56(r28)
  r11 = guest::LoadU8(base, uint32_t(r28) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bbf4
  if (cr6.eq) goto loc_8246BBF4;
  // mr r31,r24
  r31 = r24;
loc_8246BBC4:
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r28)
  r3 = guest::LoadU32(base, uint32_t(r28) + 184);
  // bl 0x8245cae0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CAE0(base, LIFTED_ARGS(0x8246BBD0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x8246bbf0
  if (cr6.eq) goto loc_8246BBF0;
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // lbz r10,56(r28)
  r10 = guest::LoadU8(base, uint32_t(r28) + 56);
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8246bbc4
  if (cr6.lt) goto loc_8246BBC4;
  // b 0x8246bbf4
  goto loc_8246BBF4;
loc_8246BBF0:
  // mr r29,r24
  r29 = r24;
loc_8246BBF4:
  // bl 0x825dbe44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeRaiseIrqlToDpcLevel>(base, LIFTED_ARGS(0x8246BBF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // mr r30,r3
  r30 = r3;
  // addi r26,r11,20240
  r26 = uint64_t(int64_t(int64_t(r11) + 20240));
  // mr r31,r13
  r31 = r13;
  // lwz r10,4(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8246bc20
  if (cr6.eq) goto loc_8246BC20;
  // lwz r11,8(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 8);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // beq cr6,0x8246bc34
  if (cr6.eq) goto loc_8246BC34;
loc_8246BC20:
  // mr r3,r26
  r3 = r26;
  // bl 0x825db9a4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeAcquireSpinLockAtRaisedIrql>(base, LIFTED_ARGS(0x8246BC28, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r31,8(r26)
  guest::StoreU32(base, uint32_t(r26) + 8, uint32_t(r31));
  // stb r30,12(r26)
  guest::StoreU8(base, uint32_t(r26) + 12, uint8_t(r30));
  // lwz r10,4(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 4);
loc_8246BC34:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r28,20
  r11 = uint64_t(int64_t(int64_t(r28) + 20));
  // stw r10,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r10));
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // beq cr6,0x8246bdcc
  if (cr6.eq) goto loc_8246BDCC;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // subf r27,r11,r9
  r27 = r9 - r11;
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8246bdcc
  if (cr6.eq) goto loc_8246BDCC;
  // lbz r11,116(r27)
  r11 = guest::LoadU8(base, uint32_t(r27) + 116);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bdcc
  if (cr6.eq) goto loc_8246BDCC;
  // lwz r11,32(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 32);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bdcc
  if (cr6.eq) goto loc_8246BDCC;
  // lbz r11,118(r27)
  r11 = guest::LoadU8(base, uint32_t(r27) + 118);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bcd4
  if (cr6.eq) goto loc_8246BCD4;
  // clrlwi r11,r29,24
  r11 = uint32_t(r29) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bdcc
  if (cr6.eq) goto loc_8246BDCC;
  // lbz r11,119(r27)
  r11 = guest::LoadU8(base, uint32_t(r27) + 119);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8246bdcc
  if (!cr6.eq) goto loc_8246BDCC;
  // lbz r11,56(r28)
  r11 = guest::LoadU8(base, uint32_t(r28) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bccc
  if (cr6.eq) goto loc_8246BCCC;
  // mr r31,r24
  r31 = r24;
loc_8246BCA8:
  // li r5,128
  r5 = uint64_t(int64_t(128));
  // lwz r3,184(r28)
  r3 = guest::LoadU32(base, uint32_t(r28) + 184);
  // mr r4,r31
  r4 = r31;
  // bl 0x8245ce10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245CE10(base, LIFTED_ARGS(0x8246BCB8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // lbz r10,56(r28)
  r10 = guest::LoadU8(base, uint32_t(r28) + 56);
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8246bca8
  if (cr6.lt) goto loc_8246BCA8;
loc_8246BCCC:
  // stb r23,119(r27)
  guest::StoreU8(base, uint32_t(r27) + 119, uint8_t(r23));
  // b 0x8246bdc8
  goto loc_8246BDC8;
loc_8246BCD4:
  // lwz r11,16(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + 16);
  // mr r30,r23
  r30 = r23;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bd28
  if (cr6.eq) goto loc_8246BD28;
  // lbz r11,56(r28)
  r11 = guest::LoadU8(base, uint32_t(r28) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bd28
  if (cr6.eq) goto loc_8246BD28;
  // mr r31,r24
  r31 = r24;
loc_8246BCF4:
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r28)
  r3 = guest::LoadU32(base, uint32_t(r28) + 184);
  // bl 0x8245d010
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8245D010(base, LIFTED_ARGS(0x8246BD00, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne cr6,0x8246bd20
  if (!cr6.eq) goto loc_8246BD20;
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // lbz r10,56(r28)
  r10 = guest::LoadU8(base, uint32_t(r28) + 56);
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8246bcf4
  if (cr6.lt) goto loc_8246BCF4;
  // b 0x8246bd24
  goto loc_8246BD24;
loc_8246BD20:
  // mr r30,r24
  r30 = r24;
loc_8246BD24:
  // lwz r10,4(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 4);
loc_8246BD28:
  // clrlwi r11,r30,24
  r11 = uint32_t(r30) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bdcc
  if (cr6.eq) goto loc_8246BDCC;
  // lbz r11,56(r28)
  r11 = guest::LoadU8(base, uint32_t(r28) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246bdc4
  if (cr6.eq) goto loc_8246BDC4;
  // mr r31,r24
  r31 = r24;
  // li r25,3
  r25 = uint64_t(int64_t(3));
loc_8246BD48:
  // addi r11,r1,84
  r11 = uint64_t(int64_t(int64_t(r1) + 84));
  // lwz r10,32(r27)
  r10 = guest::LoadU32(base, uint32_t(r27) + 32);
  // lwz r30,12(r27)
  r30 = guest::LoadU32(base, uint32_t(r27) + 12);
  // mr r5,r31
  r5 = r31;
  // lwz r29,8(r27)
  r29 = guest::LoadU32(base, uint32_t(r27) + 8);
  // addi r6,r10,-128
  r6 = uint64_t(int64_t(int64_t(r10) + -128));
  // stw r24,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r24));
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // stw r24,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r24));
  // stw r24,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r24));
  // stb r23,90(r1)
  guest::StoreU8(base, uint32_t(r1) + 90, uint8_t(r23));
  // bl 0x8246a5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8246A5F8(base, LIFTED_ARGS(0x8246BD7C, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r6,25,30,31
  r11 = __builtin_rotateleft64(uint32_t(r6) | (r6 << 32), 25) & 0x3;
  // stw r3,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r3));
  // mr r5,r31
  r5 = r31;
  // mr r4,r30
  r4 = r30;
  // mr r3,r29
  r3 = r29;
  // stb r11,88(r1)
  guest::StoreU8(base, uint32_t(r1) + 88, uint8_t(r11));
  // bl 0x8246a698
  // (left out: dead in C++)
  r3 = f8246A698(base, r3, r4, r5);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // mr r4,r31
  r4 = r31;
  // lwz r3,184(r28)
  r3 = guest::LoadU32(base, uint32_t(r28) + 184);
  // stb r25,89(r1)
  guest::StoreU8(base, uint32_t(r1) + 89, uint8_t(r25));
  // bl 0x8245cf98
  // (left out: dead in C++)
  { const lifted::Result o = game::XmaObject::SetLoopData(base, LIFTED_ARGS(0x8246BDB0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // lbz r10,56(r28)
  r10 = guest::LoadU8(base, uint32_t(r28) + 56);
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8246bd48
  if (cr6.lt) goto loc_8246BD48;
loc_8246BDC4:
  // stb r23,118(r27)
  guest::StoreU8(base, uint32_t(r27) + 118, uint8_t(r23));
loc_8246BDC8:
  // lwz r10,4(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 4);
loc_8246BDCC:
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // mr r11,r13
  r11 = r13;
  // beq cr6,0x8246be14
  if (cr6.eq) goto loc_8246BE14;
  // lwz r9,8(r26)
  r9 = guest::LoadU32(base, uint32_t(r26) + 8);
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // bne cr6,0x8246be14
  if (!cr6.eq) goto loc_8246BE14;
  // addi r11,r10,-1
  r11 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r11,4(r26)
  guest::StoreU32(base, uint32_t(r26) + 4, uint32_t(r11));
  // bne cr6,0x8246be14
  if (!cr6.eq) goto loc_8246BE14;
  // mr r11,r24
  r11 = r24;
  // lbz r31,12(r26)
  r31 = guest::LoadU8(base, uint32_t(r26) + 12);
  // mr r3,r26
  r3 = r26;
  // stb r11,12(r26)
  guest::StoreU8(base, uint32_t(r26) + 12, uint8_t(r11));
  // stw r11,8(r26)
  guest::StoreU32(base, uint32_t(r26) + 8, uint32_t(r11));
  // bl 0x825db994
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KeReleaseSpinLockFromRaisedIrql>(base, LIFTED_ARGS(0x8246BE0C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825dbe54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::ImportAt<&lifted::import_records::KfLowerIrql>(base, LIFTED_ARGS(0x8246BE14, r31)); r3 = o.r3; f1 = o.f1; }
loc_8246BE14:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82578168: lifted from generated/ by lift.py.
uint64_t f82578168(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82578168 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r4
  r7 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82578118
  // (left out: dead in C++)
  r3 = f82578118(base, r3, r4, r5);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x8257822c
  if (cr0.lt) goto loc_8257822C;
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // beq cr6,0x8257821c
  if (cr6.eq) goto loc_8257821C;
  // lwz r11,0(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x8257820c
  if (cr6.eq) goto loc_8257820C;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // beq cr6,0x825781f4
  if (cr6.eq) goto loc_825781F4;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // beq cr6,0x825781e4
  if (cr6.eq) goto loc_825781E4;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // beq cr6,0x825781c4
  if (cr6.eq) goto loc_825781C4;
  // lis r31,-32761
  r31 = uint64_t(int64_t(-2147024896));
  // ori r31,r31,13
  r31 = r31 | 13;
  // b 0x8257822c
  goto loc_8257822C;
loc_825781C4:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // addi r10,r11,4
  r10 = uint64_t(int64_t(int64_t(r11) + 4));
  // lwbrx r11,0,r11
  r11 = __builtin_bswap32(guest::LoadU32(base, uint32_t(r11)));
  // lwbrx r10,0,r10
  r10 = __builtin_bswap32(guest::LoadU32(base, uint32_t(r10)));
  // rldicr r10,r10,32,63
  r10 = __builtin_rotateleft64(r10, 32) & 0xFFFFFFFFFFFFFFFF;
  // or r11,r10,r11
  r11 = r10 | r11;
  // std r11,0(r7)
  guest::StoreU64(base, uint32_t(r7) + 0, r11);
  // b 0x8257822c
  goto loc_8257822C;
loc_825781E4:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwbrx r11,0,r11
  r11 = __builtin_bswap32(guest::LoadU32(base, uint32_t(r11)));
  // stw r11,0(r7)
  guest::StoreU32(base, uint32_t(r7) + 0, uint32_t(r11));
  // b 0x8257822c
  goto loc_8257822C;
loc_825781F4:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lhz r11,0(r11)
  r11 = guest::LoadU16(base, uint32_t(r11) + 0);
  // rlwinm r10,r11,24,24,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 24) & 0xFF;
  // rlwimi r10,r11,8,16,23
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFF00) | (r10 & 0xFFFFFFFFFFFF00FF);
  // sth r10,0(r7)
  guest::StoreU16(base, uint32_t(r7) + 0, uint16_t(r10));
  // b 0x8257822c
  goto loc_8257822C;
loc_8257820C:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lbz r11,0(r11)
  r11 = guest::LoadU8(base, uint32_t(r11) + 0);
  // stb r11,0(r7)
  guest::StoreU8(base, uint32_t(r7) + 0, uint8_t(r11));
  // b 0x8257822c
  goto loc_8257822C;
loc_8257821C:
  // mr r3,r7
  r3 = r7;
  // lwz r4,80(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r5,0(r5)
  r5 = guest::LoadU32(base, uint32_t(r5) + 0);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x8257822C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8257822C:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825784A8: lifted from generated/ by lift.py.
uint64_t f825784A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825784A8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // mr r7,r4
  r7 = r4;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82578118
  // (left out: dead in C++)
  r3 = f82578118(base, r3, r4, r5);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x825784e8
  if (cr0.lt) goto loc_825784E8;
  // mr r3,r7
  r3 = r7;
  // lwz r5,80(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r4,84(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x825784E8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825784E8:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82578530: lifted from generated/ by lift.py.
uint64_t f82578530(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82578530 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r4
  r7 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82578118
  // (left out: dead in C++)
  r3 = f82578118(base, r3, r4, r5);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82578564
  if (cr0.lt) goto loc_82578564;
  // mr r3,r7
  r3 = r7;
  // lwz r4,80(r1)
  r4 = guest::LoadU32(base, uint32_t(r1) + 80);
  // lwz r5,0(r5)
  r5 = guest::LoadU32(base, uint32_t(r5) + 0);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82578564, r31)); r3 = o.r3; f1 = o.f1; }
loc_82578564:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82583688: lifted from generated/ by lift.py.
uint64_t f82583688(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82583688 (lifted)");
  uint64_t r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r23,r3
  r23 = r3;
  // mr r27,r4
  r27 = r4;
  // mr r24,r9
  r24 = r9;
  // mr r22,r10
  r22 = r10;
  // lfs f4,-28916(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -28916);
  f4 = double(temp.f32);
  // mr r3,r7
  r3 = r7;
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // ble cr6,0x82583700
  if (!cr6.gt) goto loc_82583700;
  // rlwinm r11,r5,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // lis r4,32639
  r4 = uint64_t(int64_t(2139029504));
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // ori r4,r4,65518
  r4 = r4 | 65518;
  // rlwinm. r10,r11,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825836e0
  if (cr0.eq) goto loc_825836E0;
  // mtctr r10
  ctr = r10;
loc_825836D4:
  // stw r4,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r4));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bdnz 0x825836d4
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825836D4;
loc_825836E0:
  // rlwinm. r11,r11,30,2,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r10,r1,88
  r10 = uint64_t(int64_t(int64_t(r1) + 88));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // beq 0x82583700
  if (cr0.eq) goto loc_82583700;
  // mtctr r11
  ctr = r11;
loc_825836F4:
  // stw r9,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r9));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x825836f4
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_825836F4;
loc_82583700:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // ble cr6,0x825837ac
  if (!cr6.gt) goto loc_825837AC;
  // rlwinm r11,r5,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // lfs f12,0(r27)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r27) + 0);
  f12 = double(temp.f32);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f11,4(r27)
  temp.u32 = guest::LoadU32(base, uint32_t(r27) + 4);
  f11 = double(temp.f32);
  // add r4,r11,r10
  r4 = r11 + r10;
loc_82583720:
  // lfs f0,0(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 0);
  f0 = double(temp.f32);
  // fsubs f0,f12,f0
  f0 = double(float(f12 - f0));
  // lfs f10,4(r3)
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 4);
  f10 = double(temp.f32);
  // lfs f9,-4(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + -4);
  f9 = double(temp.f32);
  // addi r3,r3,8
  r3 = uint64_t(int64_t(int64_t(r3) + 8));
  // fmuls f13,f0,f0
  f13 = double(float(f0 * f0));
  // fsubs f0,f11,f10
  f0 = double(float(f11 - f10));
  // fmadds f0,f0,f0,f13
  f0 = double(float(std::fma(f0, f0, f13)));
  // fcmpu cr6,f0,f9
  cr6 = lifted::CompareFloat(f0, f9);
  // bge cr6,0x825837a0
  if (!cr6.lt) goto loc_825837A0;
  // addic. r11,r5,-1
  ca = uint32_t(r5) > 0;
  r11 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x8258378c
  if (!cr0.gt) goto loc_8258378C;
  // rlwinm r10,r11,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
loc_82583758:
  // addi r31,r1,80
  r31 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfsx f13,r10,r31
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r31));
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bgt cr6,0x8258378c
  if (cr6.gt) goto loc_8258378C;
  // addi r31,r1,88
  r31 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r30,r1,84
  r30 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r29,r1,92
  r29 = uint64_t(int64_t(int64_t(r1) + 92));
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwzx r31,r10,r31
  r31 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r31));
  // stfsx f13,r10,r30
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r30), temp.u32);
  // stwx r31,r10,r29
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r29), uint32_t(r31));
  // addi r10,r10,-4
  r10 = uint64_t(int64_t(int64_t(r10) + -4));
  // bgt 0x82583758
  if (cr0.gt) goto loc_82583758;
loc_8258378C:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r31,r1,88
  r31 = uint64_t(int64_t(int64_t(r1) + 88));
  // stfsx f0,r11,r10
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r10), temp.u32);
  // stwx r9,r11,r31
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r31), uint32_t(r9));
loc_825837A0:
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // cmpw cr6,r9,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r6), so);
  // blt cr6,0x82583720
  if (cr6.lt) goto loc_82583720;
loc_825837AC:
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // ble cr6,0x825838f0
  if (!cr6.gt) goto loc_825838F0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r28,284(r1)
  r28 = guest::LoadU32(base, uint32_t(r1) + 284);
  // addi r26,r1,88
  r26 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r25,r5
  r25 = r5;
  // lfs f5,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f5 = double(temp.f32);
loc_825837C8:
  // lwz r29,0(r26)
  r29 = guest::LoadU32(base, uint32_t(r26) + 0);
  // mr r11,r29
  r11 = r29;
  // cmplw cr6,r29,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r6), so);
  // blt cr6,0x825837dc
  if (cr6.lt) goto loc_825837DC;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825837DC:
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // lfs f8,4(r27)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r27) + 4);
  f8 = double(temp.f32);
  // lfs f9,0(r27)
  temp.u32 = guest::LoadU32(base, uint32_t(r27) + 0);
  f9 = double(temp.f32);
  // rlwinm r10,r29,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 3) & 0xFFFFFFF8;
  // add r11,r11,r7
  r11 = r11 + r7;
  // add r31,r10,r8
  r31 = r10 + r8;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // lfs f6,4(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4);
  f6 = double(temp.f32);
  // lfs f7,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f7 = double(temp.f32);
  // fsubs f13,f8,f6
  f13 = double(float(f8 - f6));
  // fsubs f0,f9,f7
  f0 = double(float(f9 - f7));
  // lfs f12,4(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // lfs f11,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f11 = double(temp.f32);
  // fmuls f10,f12,f13
  f10 = double(float(f12 * f13));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // fmadds f0,f11,f0,f10
  f0 = double(float(std::fma(f11, f0, f10)));
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 96, temp.u32);
  // fmsubs f0,f11,f13,f12
  f0 = double(float(std::fma(f11, f13, -f12)));
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 100, temp.u32);
  // ble cr6,0x82583850
  if (!cr6.gt) goto loc_82583850;
  // lwz r11,292(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 292);
loc_82583834:
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // cmpw cr6,r9,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r29), so);
  // beq cr6,0x82583908
  if (cr6.eq) goto loc_82583908;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmpw cr6,r10,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r28), so);
  // blt cr6,0x82583834
  if (cr6.lt) goto loc_82583834;
loc_82583850:
  // mr r30,r22
  r30 = r22;
loc_82583854:
  // mr r5,r24
  r5 = r24;
  // mr r4,r30
  r4 = r30;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82583610
  // (left out: dead in C++)
  r3 = f82583610(base, r1, r3, r4, r5);
  // mr r11,r3
  r11 = r3;
  // cmplw cr6,r3,r24
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r24), so);
  // blt cr6,0x82583874
  if (cr6.lt) goto loc_82583874;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82583874:
  // lfs f13,4(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 4);
  f13 = double(temp.f32);
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // fmuls f10,f13,f13
  f10 = double(float(f13 * f13));
  // lfs f0,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // add r11,r11,r30
  r11 = r11 + r30;
  // lfs f11,4(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4);
  f11 = double(temp.f32);
  // fmuls f3,f13,f11
  f3 = double(float(f13 * f11));
  // lfs f12,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // fmuls f11,f0,f11
  f11 = double(float(f0 * f11));
  // fmadds f10,f0,f0,f10
  f10 = double(float(std::fma(f0, f0, f10)));
  // fmsubs f3,f0,f12,f3
  f3 = double(float(std::fma(f0, f12, -f3)));
  // fmadds f12,f13,f12,f11
  f12 = double(float(std::fma(f13, f12, f11)));
  // fdivs f0,f5,f10
  f0 = double(float(f5 / f10));
  // fmuls f13,f3,f0
  f13 = double(float(f3 * f0));
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // fadds f13,f13,f7
  f13 = double(float(f13 + f7));
  // fadds f12,f12,f6
  f12 = double(float(f12 + f6));
  // fsubs f0,f9,f13
  f0 = double(float(f9 - f13));
  // fmuls f11,f0,f0
  f11 = double(float(f0 * f0));
  // fsubs f0,f8,f12
  f0 = double(float(f8 - f12));
  // fmadds f0,f0,f0,f11
  f0 = double(float(std::fma(f0, f0, f11)));
  // fcmpu cr6,f0,f4
  cr6 = lifted::CompareFloat(f0, f4);
  // bge cr6,0x825838e4
  if (!cr6.lt) goto loc_825838E4;
  // stfs f13,80(r1)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // fmr f4,f0
  f4 = f0;
  // stfs f12,84(r1)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r1) + 84, temp.u32);
  // stw r29,0(r23)
  guest::StoreU32(base, uint32_t(r23) + 0, uint32_t(r29));
  // stw r3,4(r23)
  guest::StoreU32(base, uint32_t(r23) + 4, uint32_t(r3));
loc_825838E4:
  // addic. r25,r25,-1
  ca = uint32_t(r25) > 0;
  r25 = uint64_t(int64_t(int64_t(r25) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // addi r26,r26,4
  r26 = uint64_t(int64_t(int64_t(r26) + 4));
  // bne 0x825837c8
  if (!cr0.eq) goto loc_825837C8;
loc_825838F0:
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // stfs f0,0(r27)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r27) + 0, temp.u32);
  // lfs f0,84(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 84);
  f0 = double(temp.f32);
  // stfs f0,4(r27)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r27) + 4, temp.u32);
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return r3;
loc_82583908:
  // lwz r30,276(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 276);
  // b 0x82583854
  goto loc_82583854;
}

// sub_82583910: lifted from generated/ by lift.py.
lifted::Result f82583910(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82583910 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e58
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-144(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -144, std::bit_cast<uint64_t>(f31));
  // stwu r1,-336(r1)
  ea = -336 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r10,-32154
  r10 = uint64_t(int64_t(-2107244544));
  // mr r19,r3
  r19 = r3;
  // mr r20,r5
  r20 = r5;
  // addi r23,r10,-9872
  r23 = uint64_t(int64_t(int64_t(r10) + -9872));
  // lfs f0,4208(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4208);
  f0 = double(temp.f32);
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // addi r11,r11,31072
  r11 = uint64_t(int64_t(int64_t(r11) + 31072));
  // ble cr6,0x82583958
  if (!cr6.gt) goto loc_82583958;
  // addi r10,r11,1644
  r10 = uint64_t(int64_t(int64_t(r11) + 1644));
  // mr r21,r11
  r21 = r11;
  // mr r9,r23
  r9 = r23;
  // b 0x82583964
  goto loc_82583964;
loc_82583958:
  // addi r21,r11,48
  r21 = uint64_t(int64_t(int64_t(r11) + 48));
  // addi r9,r23,24
  r9 = uint64_t(int64_t(int64_t(r23) + 24));
  // addi r10,r11,2744
  r10 = uint64_t(int64_t(int64_t(r11) + 2744));
loc_82583964:
  // stw r10,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r10));
  // addi r8,r11,4324
  r8 = uint64_t(int64_t(int64_t(r11) + 4324));
  // addi r10,r11,3600
  r10 = uint64_t(int64_t(int64_t(r11) + 3600));
  // addi r11,r11,4984
  r11 = uint64_t(int64_t(int64_t(r11) + 4984));
  // cmpwi cr6,r20,0
  cr6 = lifted::Compare<int32_t>(int32_t(r20), 0, so);
  // stw r8,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r8));
  // stw r10,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r10));
  // stw r11,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r11));
  // ble cr6,0x825839b8
  if (!cr6.gt) goto loc_825839B8;
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // mr r11,r21
  r11 = r21;
  // subf r7,r21,r10
  r7 = r10 - r21;
  // subf r8,r21,r19
  r8 = r19 - r21;
  // mr r10,r20
  r10 = r20;
loc_8258399C:
  // lfsx f0,r8,r11
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r8) + uint32_t(r11));
  f0 = double(temp.f32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lfs f13,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stfsx f0,r7,r11
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r7) + uint32_t(r11), temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x8258399c
  if (!cr0.eq) goto loc_8258399C;
loc_825839B8:
  // addi r11,r23,48
  r11 = uint64_t(int64_t(int64_t(r23) + 48));
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r30,r11,-16
  r30 = uint64_t(int64_t(int64_t(r11) + -16));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r31,r1,144
  r31 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // mr r29,r4
  r29 = r4;
  // addi r28,r1,128
  r28 = uint64_t(int64_t(int64_t(r1) + 128));
  // addi r26,r1,148
  r26 = uint64_t(int64_t(int64_t(r1) + 148));
  // subf r22,r10,r9
  r22 = r9 - r10;
loc_825839EC:
  // lwzx r11,r22,r28
  r11 = guest::LoadU32(base, uint32_t(r22) + uint32_t(r28));
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r7,r1,112
  r7 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // subf r10,r10,r11
  r10 = r11 - r10;
loc_82583A00:
  // fmr f0,f31
  lifted::ScalarFloatMode();
  f0 = f31;
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // ble cr6,0x82583a2c
  if (!cr6.gt) goto loc_82583A2C;
  // addi r11,r1,144
  r11 = uint64_t(int64_t(int64_t(r1) + 144));
  // mr r9,r24
  r9 = r24;
loc_82583A14:
  // lfsx f13,r10,r11
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + uint32_t(r11));
  f13 = double(temp.f32);
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // lfs f12,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f12 = double(temp.f32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // fmadds f0,f13,f12,f0
  f0 = double(float(std::fma(f13, f12, f0)));
  // bne 0x82583a14
  if (!cr0.eq) goto loc_82583A14;
loc_82583A2C:
  // stfs f0,0(r7)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r7) + 0, temp.u32);
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // add r10,r27,r10
  r10 = r27 + r10;
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // bne 0x82583a00
  if (!cr0.eq) goto loc_82583A00;
  // lfs f0,0(r31)
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // cmpwi cr6,r25,4
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 4, so);
  // lfs f13,0(r26)
  temp.u32 = guest::LoadU32(base, uint32_t(r26) + 0);
  f13 = double(temp.f32);
  // lfs f2,112(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 112);
  f2 = double(temp.f32);
  // lfs f1,116(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 116);
  f1 = double(temp.f32);
  // fsubs f0,f0,f2
  f0 = double(float(f0 - f2));
  // fsubs f13,f13,f1
  f13 = double(float(f13 - f1));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 0, temp.u32);
  // stfs f13,0(r26)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r26) + 0, temp.u32);
  // bge cr6,0x82583aac
  if (!cr6.lt) goto loc_82583AAC;
  // lwz r11,0(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 0);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // lwz r18,28(r11)
  r18 = guest::LoadU32(base, uint32_t(r11) + 28);
  // lwz r17,24(r11)
  r17 = guest::LoadU32(base, uint32_t(r11) + 24);
  // lwz r16,20(r11)
  r16 = guest::LoadU32(base, uint32_t(r11) + 20);
  // lwz r10,16(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 16);
  // lwz r9,12(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 12);
  // lwz r8,8(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lwz r7,4(r11)
  r7 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r6,0(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r18,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r18));
  // stw r17,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r17));
  // stw r16,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r16));
  // bl 0x82583688
  // (left out: dead in C++)
  r3 = f82583688(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10);
  // b 0x82583b00
  goto loc_82583B00;
loc_82583AAC:
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r5,0(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r4,4(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 4);
  // bl 0x82583610
  // (left out: dead in C++)
  r3 = f82583610(base, r1, r3, r4, r5);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r3,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r3));
  // mr r10,r3
  r10 = r3;
  // stw r11,4(r29)
  guest::StoreU32(base, uint32_t(r29) + 4, uint32_t(r11));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 0);
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // blt cr6,0x82583ae8
  if (cr6.lt) goto loc_82583AE8;
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_82583AE8:
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r11,r10,r11
  r11 = r10 + r11;
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // lfs f13,4(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4);
  f13 = double(temp.f32);
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 0, temp.u32);
  // stfs f13,4(r31)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r31) + 4, temp.u32);
loc_82583B00:
  // addi r11,r31,4
  r11 = uint64_t(int64_t(int64_t(r31) + 4));
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // addi r6,r23,48
  r6 = uint64_t(int64_t(int64_t(r23) + 48));
  // fadds f0,f0,f2
  f0 = double(float(f0 + f2));
  // stfs f0,0(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 0, temp.u32);
  // addi r10,r24,1
  r10 = uint64_t(int64_t(int64_t(r24) + 1));
  // addi r9,r27,4
  r9 = uint64_t(int64_t(int64_t(r27) + 4));
  // addi r8,r26,4
  r8 = uint64_t(int64_t(int64_t(r26) + 4));
  // lfs f0,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // addi r7,r29,4
  r7 = uint64_t(int64_t(int64_t(r29) + 4));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // fadds f0,f0,f1
  f0 = double(float(f0 + f1));
  // addi r6,r6,8
  r6 = uint64_t(int64_t(int64_t(r6) + 8));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 0, temp.u32);
  // addi r25,r25,1
  r25 = uint64_t(int64_t(int64_t(r25) + 1));
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // addi r24,r10,1
  r24 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r27,r9,4
  r27 = uint64_t(int64_t(int64_t(r9) + 4));
  // addi r26,r8,4
  r26 = uint64_t(int64_t(int64_t(r8) + 4));
  // addi r29,r7,4
  r29 = uint64_t(int64_t(int64_t(r7) + 4));
  // addi r31,r11,4
  r31 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmpw cr6,r30,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r6), so);
  // blt cr6,0x825839ec
  if (cr6.lt) goto loc_825839EC;
  // cmpwi cr6,r20,0
  cr6 = lifted::Compare<int32_t>(int32_t(r20), 0, so);
  // ble cr6,0x82583b98
  if (!cr6.gt) goto loc_82583B98;
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r8,r1,144
  r8 = uint64_t(int64_t(int64_t(r1) + 144));
  // subf r9,r10,r21
  r9 = r21 - r10;
  // addi r11,r1,144
  r11 = uint64_t(int64_t(int64_t(r1) + 144));
  // subf r8,r8,r19
  r8 = r19 - r8;
  // mr r10,r20
  r10 = r20;
loc_82583B7C:
  // lfsx f0,r9,r11
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r11));
  f0 = double(temp.f32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lfs f13,0(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // fadds f0,f0,f13
  f0 = double(float(f0 + f13));
  // stfsx f0,r8,r11
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r11), temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x82583b7c
  if (!cr0.eq) goto loc_82583B7C;
loc_82583B98:
  // addi r1,r1,336
  r1 = uint64_t(int64_t(int64_t(r1) + 336));
  // lfd f31,-144(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -144)));
  // b 0x825a6ea8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82587238: lifted from generated/ by lift.py.
lifted::Result f82587238(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5) {
  ITC_NATIVE_RUNNING("sub_82587238 (lifted)");
  uint64_t r7 = 0, r8 = 0, r11 = 0, r12 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f10 = 0.0, f12 = 0.0, f13 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e64
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-112
  r12 = uint64_t(int64_t(int64_t(r1) + -112));
  // bl 0x825a790c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-304(r1)
  ea = -304 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // fmr f29,f1
  lifted::ScalarFloatMode();
  f29 = f1;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r25,r10
  r25 = r10;
  // fmr f26,f3
  f26 = f3;
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // fmr f25,f4
  f25 = f4;
  // mr r28,r3
  r28 = r3;
  // fmr f28,f5
  f28 = f5;
  // mr r20,r4
  r20 = r4;
  // lfs f30,7140(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7140);
  f30 = double(temp.f32);
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // mr r21,r5
  r21 = r5;
  // mr r19,r6
  r19 = r6;
  // mr r26,r9
  r26 = r9;
  // fmuls f13,f29,f30
  f13 = double(float(f29 * f30));
  // lfs f0,30700(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 30700);
  f0 = double(temp.f32);
  // fdivs f31,f0,f29
  f31 = double(float(f0 / f29));
  // fctiwz f0,f13
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f13) ? int64_t(0x80000000U) : (f13 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f13)))));
  // stfiwx f0,0,r10
  guest::StoreU32(base, uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r31,112(r1)
  r31 = guest::LoadU32(base, uint32_t(r1) + 112);
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // extsw r11,r31
  r11 = uint64_t(int64_t(int32_t(r31)));
  // std r11,112(r1)
  guest::StoreU64(base, uint32_t(r1) + 112, r11);
  // lfd f0,112(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 112)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmadds f0,f0,f31,f30
  f0 = double(float(std::fma(f0, f31, f30)));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r10
  guest::StoreU32(base, uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,112(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 112);
  // cmpwi cr6,r11,256
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 256, so);
  // blt cr6,0x825872d0
  if (cr6.lt) goto loc_825872D0;
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
loc_825872D0:
  // cmpwi cr6,r31,100
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 100, so);
  // ble cr6,0x825872dc
  if (!cr6.gt) goto loc_825872DC;
  // li r31,100
  r31 = uint64_t(int64_t(100));
loc_825872DC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,8108(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8108);
  f13 = double(temp.f32);
  // fmuls f0,f2,f13
  f0 = double(float(f2 * f13));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82587300
  if (!cr6.lt) goto loc_82587300;
  // li r11,100
  r11 = uint64_t(int64_t(100));
  // twllei r31,0
  if (int32_t(r31) == 0 || uint32_t(r31) < 0u) lifted::Trap(0);
  // divw r27,r11,r31
  r27 = uint32_t((int32_t(r31) && !(int32_t(r11) == INT32_MIN && int32_t(r31) == -1)) ? int32_t(r11) / int32_t(r31) : 0);
  // b 0x82587304
  goto loc_82587304;
loc_82587300:
  // li r27,1
  r27 = uint64_t(int64_t(1));
loc_82587304:
  // extsw r10,r27
  r10 = uint64_t(int64_t(int32_t(r27)));
  // fmuls f13,f0,f29
  lifted::ScalarFloatMode();
  f13 = double(float(f0 * f29));
  // addi r9,r1,112
  r9 = uint64_t(int64_t(int64_t(r1) + 112));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mullw r23,r27,r31
  r23 = uint64_t(int64_t(int64_t(int32_t(r27)) * int64_t(int32_t(r31))));
  // std r10,120(r1)
  guest::StoreU64(base, uint32_t(r1) + 120, r10);
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r24,r11,-22412
  r24 = uint64_t(int64_t(int64_t(r11) + -22412));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r11,r24,28
  r11 = uint64_t(int64_t(int64_t(r24) + 28));
  // lfs f0,-22348(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + -22348);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r9
  guest::StoreU32(base, uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r29,112(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 112);
  // mullw r22,r29,r27
  r22 = uint64_t(int64_t(int64_t(int32_t(r29)) * int64_t(int32_t(r27))));
  // lfd f0,120(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 120)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fdivs f27,f31,f0
  f27 = double(float(f31 / f0));
loc_82587354:
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // fcmpu cr6,f29,f0
  cr6 = lifted::CompareFloat(f29, f0);
  // bgt cr6,0x82587378
  if (cr6.gt) goto loc_82587378;
  // addi r10,r24,28
  r10 = uint64_t(int64_t(int64_t(r24) + 28));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // addi r10,r10,20
  r10 = uint64_t(int64_t(int64_t(r10) + 20));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // blt cr6,0x82587354
  if (cr6.lt) goto loc_82587354;
loc_82587378:
  // fmr f1,f27
  lifted::ScalarFloatMode();
  f1 = f27;
  // bl 0x82585398
  // (left out: dead in C++)
  f1 = f82585398(base, r1, f1);
  // rlwinm r6,r30,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // lwz r30,388(r1)
  r30 = guest::LoadU32(base, uint32_t(r1) + 388);
  // addi r11,r24,4
  r11 = uint64_t(int64_t(int64_t(r24) + 4));
  // fmr f4,f28
  lifted::ScalarFloatMode();
  f4 = f28;
  // mr r10,r26
  r10 = r26;
  // lwz r26,396(r1)
  r26 = guest::LoadU32(base, uint32_t(r1) + 396);
  // mr r8,r21
  r8 = r21;
  // stw r25,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r25));
  // mr r9,r26
  r9 = r26;
  // fmr f3,f27
  f3 = f27;
  // mr r7,r29
  r7 = r29;
  // stw r30,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r30));
  // lfsx f0,r6,r11
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + uint32_t(r11));
  f0 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fmadds f13,f1,f30,f0
  f13 = double(float(std::fma(f1, f30, f0)));
  // mr r6,r31
  r6 = r31;
  // fmr f2,f31
  f2 = f31;
  // lfs f0,-22364(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -22364);
  f0 = double(temp.f32);
  // fsubs f1,f13,f0
  f1 = double(float(f13 - f0));
  // bl 0x82588ff0
  // (left out: dead in C++)
  r3 = f82588FF0(base, r1, r3, r6, r7, r8, r9, r10, f1, f2, f4);
  // lfs f12,8(r28)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r28) + 8);
  f12 = double(temp.f32);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // fadds f12,f12,f31
  f12 = double(float(f12 + f31));
  // lfs f13,0(r28)
  temp.u32 = guest::LoadU32(base, uint32_t(r28) + 0);
  f13 = double(temp.f32);
  // lfs f0,-22352(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -22352);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmadds f13,f12,f0,f13
  f13 = double(float(std::fma(f12, f0, f13)));
  // lfs f0,8100(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8100);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f10,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f10 = double(temp.f32);
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // fcmpu cr6,f0,f10
  cr6 = lifted::CompareFloat(f0, f10);
  // ble cr6,0x82587410
  if (!cr6.gt) goto loc_82587410;
  // fadds f0,f0,f30
  f0 = double(float(f0 + f30));
  // b 0x82587414
  goto loc_82587414;
loc_82587410:
  // fsubs f0,f0,f30
  lifted::ScalarFloatMode();
  f0 = double(float(f0 - f30));
loc_82587414:
  // fctiwz f0,f0
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r11
  guest::StoreU32(base, uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwa r10,112(r1)
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r1) + 112))));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r7,r26
  r7 = r26;
  // stfs f31,8(r28)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r28) + 8, temp.u32);
  // mr r6,r30
  r6 = r30;
  // fmr f2,f29
  f2 = f29;
  // mr r4,r31
  r4 = r31;
  // std r10,120(r1)
  guest::StoreU64(base, uint32_t(r1) + 120, r10);
  // lfs f0,8104(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8104);
  f0 = double(temp.f32);
  // lfd f12,120(r1)
  f12 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 120)));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f12,f12
  f12 = double(float(f12));
  // fnmsubs f1,f12,f0,f13
  f1 = double(float(-std::fma(f12, f0, -f13)));
  // stfs f1,0(r28)
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r28) + 0, temp.u32);
  // bl 0x82587188
  // (left out: dead in C++)
  f82587188(base, r1, r4, r6, r7, f1, f2);
  // addi r11,r31,-1
  r11 = uint64_t(int64_t(int64_t(r31) + -1));
  // addi r10,r23,-1
  r10 = uint64_t(int64_t(int64_t(r23) + -1));
  // cmpw cr6,r11,r29
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r29), so);
  // blt cr6,0x825874cc
  if (cr6.lt) goto loc_825874CC;
  // subf r9,r29,r11
  r9 = r11 - r29;
  // rlwinm r8,r11,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r7,r9,1
  r7 = uint64_t(int64_t(int64_t(r9) + 1));
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r4,r27,2,0,29
  r4 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r30
  r9 = r9 + r30;
  // add r6,r8,r30
  r6 = r8 + r30;
  // subf r3,r30,r26
  r3 = r26 - r30;
  // subf r11,r7,r11
  r11 = r11 - r7;
loc_8258748C:
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // ble cr6,0x825874c0
  if (!cr6.gt) goto loc_825874C0;
  // mr r8,r9
  r8 = r9;
  // mr r5,r27
  r5 = r27;
  // subf r10,r27,r10
  r10 = r10 - r27;
  // subf r9,r4,r9
  r9 = r9 - r4;
loc_825874A4:
  // lfsx f0,r6,r3
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + uint32_t(r3));
  f0 = double(temp.f32);
  // addic. r5,r5,-1
  ca = uint32_t(r5) > 0;
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // stfsx f0,r3,r8
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r8), temp.u32);
  // lfs f0,0(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 0);
  f0 = double(temp.f32);
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r8) + 0, temp.u32);
  // addi r8,r8,-4
  r8 = uint64_t(int64_t(int64_t(r8) + -4));
  // bne 0x825874a4
  if (!cr0.eq) goto loc_825874A4;
loc_825874C0:
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // addi r6,r6,-4
  r6 = uint64_t(int64_t(int64_t(r6) + -4));
  // bne 0x8258748c
  if (!cr0.eq) goto loc_8258748C;
loc_825874CC:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt cr6,0x82587530
  if (cr6.lt) goto loc_82587530;
  // rlwinm r9,r11,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // subf r6,r30,r26
  r6 = r26 - r30;
  // add r7,r9,r30
  r7 = r9 + r30;
loc_825874E0:
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lfsx f0,r7,r6
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + uint32_t(r6));
  f0 = double(temp.f32);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r27,1
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 1, so);
  // stfsx f0,r9,r26
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r26), temp.u32);
  // lfs f0,0(r7)
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 0);
  f0 = double(temp.f32);
  // stfsx f0,r9,r30
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + uint32_t(r30), temp.u32);
  // ble cr6,0x82587524
  if (!cr6.gt) goto loc_82587524;
  // rlwinm r8,r10,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r27,-1
  r9 = uint64_t(int64_t(int64_t(r27) + -1));
  // add r8,r8,r30
  r8 = r8 + r30;
  // subf r10,r9,r10
  r10 = r10 - r9;
loc_82587510:
  // stfsx f10,r8,r6
  lifted::ScalarFloatMode();
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r8) + uint32_t(r6), temp.u32);
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stfs f10,0(r8)
  temp.f32 = float(f10);
  guest::StoreU32(base, uint32_t(r8) + 0, temp.u32);
  // addi r8,r8,-4
  r8 = uint64_t(int64_t(int64_t(r8) + -4));
  // bne 0x82587510
  if (!cr0.eq) goto loc_82587510;
loc_82587524:
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // addi r7,r7,-4
  r7 = uint64_t(int64_t(int64_t(r7) + -4));
  // bge 0x825874e0
  if (!cr0.lt) goto loc_825874E0;
loc_82587530:
  // cmpw cr6,r22,r23
  cr6 = lifted::Compare<int32_t>(int32_t(r22), int32_t(r23), so);
  // bge cr6,0x825875b4
  if (!cr6.lt) goto loc_825875B4;
  // rlwinm r10,r22,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r22) | (r22 << 32), 2) & 0xFFFFFFFC;
  // lwz r8,428(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 428);
  // lfs f0,0(r24)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r24) + 0);
  f0 = double(temp.f32);
  // subf r11,r22,r23
  r11 = r23 - r22;
  // add r10,r10,r30
  r10 = r10 + r30;
loc_8258754C:
  // lwz r9,0(r8)
  r9 = guest::LoadU32(base, uint32_t(r8) + 0);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // clrlwi r7,r9,16
  r7 = uint32_t(r9) & 0xFFFF;
  // srawi r9,r9,16
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0xFFFF) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 16));
  // mulli r7,r7,16807
  r7 = uint64_t(int64_t(static_cast<int64_t>(r7 * static_cast<uint64_t>(16807))));
  // mulli r9,r9,16807
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(16807))));
  // rlwinm r5,r9,16,1,15
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 16) & 0x7FFF0000;
  // rlwinm r6,r9,17,15,31
  r6 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 17) & 0x1FFFF;
  // add r9,r7,r5
  r9 = r7 + r5;
  // rlwinm r5,r9,1,31,31
  r5 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0x1;
  // clrlwi r7,r9,1
  r7 = uint32_t(r9) & 0x7FFFFFFF;
  // add r9,r6,r5
  r9 = r6 + r5;
  // add r9,r9,r7
  r9 = r9 + r7;
  // rlwinm r7,r9,1,31,31
  r7 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0x1;
  // add r9,r7,r9
  r9 = r7 + r9;
  // clrlwi r9,r9,1
  r9 = uint32_t(r9) & 0x7FFFFFFF;
  // extsw r7,r9
  r7 = uint64_t(int64_t(int32_t(r9)));
  // stw r9,0(r8)
  guest::StoreU32(base, uint32_t(r8) + 0, uint32_t(r9));
  // std r7,120(r1)
  guest::StoreU64(base, uint32_t(r1) + 120, r7);
  // lfd f13,120(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 120)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,0(r10)
  temp.f32 = float(f13);
  guest::StoreU32(base, uint32_t(r10) + 0, temp.u32);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bne 0x8258754c
  if (!cr0.eq) goto loc_8258754C;
loc_825875B4:
  // mr r9,r26
  r9 = r26;
  // mtctr r23
  ctr = r23;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // cmpwi cr6,r23,0
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // lfs f13,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // ble cr6,0x82587618
  if (!cr6.gt) goto loc_82587618;
  // mr r10,r26
  r10 = r26;
  // mr r11,r23
  r11 = r23;
loc_825875D4:
  // lfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // fadds f10,f0,f10
  f10 = double(float(f0 + f10));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bne 0x825875d4
  if (!cr0.eq) goto loc_825875D4;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,32600(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 32600);
  f0 = double(temp.f32);
  // fcmpu cr6,f10,f0
  cr6 = lifted::CompareFloat(f10, f0);
  // ble cr6,0x825875fc
  if (!cr6.gt) goto loc_825875FC;
  // fdivs f13,f0,f10
  f13 = double(float(f0 / f10));
loc_825875FC:
  // cmpwi cr6,r23,0
  cr6 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // ble cr6,0x82587618
  if (!cr6.gt) goto loc_82587618;
loc_82587604:
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // stfs f0,0(r9)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r9) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bdnz 0x82587604
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_82587604;
loc_82587618:
  // mr r10,r26
  r10 = r26;
  // fmr f3,f25
  lifted::ScalarFloatMode();
  f3 = f25;
  // mr r9,r30
  r9 = r30;
  // fmr f2,f26
  f2 = f26;
  // mr r8,r19
  r8 = r19;
  // fmr f1,f27
  f1 = f27;
  // mr r7,r21
  r7 = r21;
  // mr r6,r20
  r6 = r20;
  // li r5,160
  r5 = uint64_t(int64_t(160));
  // mr r4,r23
  r4 = r23;
  // bl 0x82583180
  // (left out: dead in C++)
  { const lifted::Result o = f82583180(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,304
  r1 = uint64_t(int64_t(int64_t(r1) + 304));
  // addi r12,r1,-112
  r12 = uint64_t(int64_t(int64_t(r1) + -112));
  // bl 0x825a7958
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6eb4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82594190: lifted from generated/ by lift.py.
lifted::Result f82594190(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82594190 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825940b0
  // (left out: dead in C++)
  { const lifted::Result o = game::PoolNode::Dtor(base, LIFTED_ARGS(0x825941A8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r4,31
  r11 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825941bc
  if (cr6.eq) goto loc_825941BC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x825941B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825941BC:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D3778: lifted from generated/ by lift.py.
uint64_t f825D3778(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_825D3778 (lifted)");
  uint64_t r11 = 0, r12 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // bl 0x825d36f0
  // (left out: dead in C++)
  r3 = game::XACTTransitionTable::Entry(base, r3, r4, r5);
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r6)
  guest::StoreU32(base, uint32_t(r6) + 0, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

lifted::Result e820B6170(uint8_t* base, const lifted::Args& a) {
  return f820B6170(base, a.r1, a.r13, a.r[0], a.r[3], a.r[5], a.r[6], a.f[0], a.f[1], a.f[2]);
}

lifted::Result e820F6C88(uint8_t* base, const lifted::Args& a) {
  return {f820F6C88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e820F7E30(uint8_t* base, const lifted::Args& a) {
  return {f820F7E30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e820FB2B0(uint8_t* base, const lifted::Args& a) {
  return {f820FB2B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e8210B958(uint8_t* base, const lifted::Args& a) {
  return f8210B958(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8210B9C0(uint8_t* base, const lifted::Args& a) {
  return f8210B9C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e82138FA8(uint8_t* base, const lifted::Args& a) {
  return {f82138FA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82139A08(uint8_t* base, const lifted::Args& a) {
  return {f82139A08(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e821C2388(uint8_t* base, const lifted::Args& a) {
  return f821C2388(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821C3E48(uint8_t* base, const lifted::Args& a) {
  return f821C3E48(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821C41D0(uint8_t* base, const lifted::Args& a) {
  return f821C41D0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821C43D0(uint8_t* base, const lifted::Args& a) {
  return f821C43D0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821C47A0(uint8_t* base, const lifted::Args& a) {
  return f821C47A0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821C4A58(uint8_t* base, const lifted::Args& a) {
  return f821C4A58(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821C5F80(uint8_t* base, const lifted::Args& a) {
  return f821C5F80(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821C60D0(uint8_t* base, const lifted::Args& a) {
  return f821C60D0(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821D7C90(uint8_t* base, const lifted::Args& a) {
  return f821D7C90(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8222C300(uint8_t* base, const lifted::Args& a) {
  return f8222C300(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e823EACC8(uint8_t* base, const lifted::Args& a) {
  return f823EACC8(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e823F2730(uint8_t* base, const lifted::Args& a) {
  return {f823F2730(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F2830(uint8_t* base, const lifted::Args& a) {
  return {f823F2830(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F2948(uint8_t* base, const lifted::Args& a) {
  return {f823F2948(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F2A58(uint8_t* base, const lifted::Args& a) {
  return {f823F2A58(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F2B80(uint8_t* base, const lifted::Args& a) {
  return {f823F2B80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F2CA8(uint8_t* base, const lifted::Args& a) {
  return {f823F2CA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F2DE8(uint8_t* base, const lifted::Args& a) {
  return {f823F2DE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F2EF8(uint8_t* base, const lifted::Args& a) {
  return {f823F2EF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F2FF8(uint8_t* base, const lifted::Args& a) {
  return {f823F2FF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F30E8(uint8_t* base, const lifted::Args& a) {
  return {f823F30E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F3228(uint8_t* base, const lifted::Args& a) {
  return {f823F3228(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F3348(uint8_t* base, const lifted::Args& a) {
  return {f823F3348(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F3410(uint8_t* base, const lifted::Args& a) {
  return {f823F3410(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F3520(uint8_t* base, const lifted::Args& a) {
  return {f823F3520(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F35E8(uint8_t* base, const lifted::Args& a) {
  return {f823F35E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F36C8(uint8_t* base, const lifted::Args& a) {
  return {f823F36C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F37E8(uint8_t* base, const lifted::Args& a) {
  return {f823F37E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F3948(uint8_t* base, const lifted::Args& a) {
  return {f823F3948(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F3A88(uint8_t* base, const lifted::Args& a) {
  return {f823F3A88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F3BF0(uint8_t* base, const lifted::Args& a) {
  return {f823F3BF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F3D10(uint8_t* base, const lifted::Args& a) {
  return {f823F3D10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F3E90(uint8_t* base, const lifted::Args& a) {
  return {f823F3E90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F3FF8(uint8_t* base, const lifted::Args& a) {
  return {f823F3FF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F40C8(uint8_t* base, const lifted::Args& a) {
  return {f823F40C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F4738(uint8_t* base, const lifted::Args& a) {
  return {f823F4738(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F53B8(uint8_t* base, const lifted::Args& a) {
  return {f823F53B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e823F5468(uint8_t* base, const lifted::Args& a) {
  return {f823F5468(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e824227F0(uint8_t* base, const lifted::Args& a) {
  return f824227F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e8246B480(uint8_t* base, const lifted::Args& a) {
  return f8246B480(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8246BB98(uint8_t* base, const lifted::Args& a) {
  return f8246BB98(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82578168(uint8_t* base, const lifted::Args& a) {
  return {f82578168(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e825784A8(uint8_t* base, const lifted::Args& a) {
  return {f825784A8(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82578530(uint8_t* base, const lifted::Args& a) {
  return {f82578530(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82583688(uint8_t* base, const lifted::Args& a) {
  return {f82583688(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]), a.f[0]};
}

lifted::Result e82583910(uint8_t* base, const lifted::Args& a) {
  return f82583910(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e82587238(uint8_t* base, const lifted::Args& a) {
  return f82587238(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4]);
}

lifted::Result e82594190(uint8_t* base, const lifted::Args& a) {
  return f82594190(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D3778(uint8_t* base, const lifted::Args& a) {
  return {f825D3778(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

namespace {

const Registration kEntries[] = {
    {0x820B6170, e820B6170},
    {0x820F6C88, e820F6C88},
    {0x820F7E30, e820F7E30},
    {0x820FB2B0, e820FB2B0},
    {0x8210B958, e8210B958},
    {0x8210B9C0, e8210B9C0},
    {0x82138FA8, e82138FA8},
    {0x82139A08, e82139A08},
    {0x821C2388, e821C2388},
    {0x821C3E48, e821C3E48},
    {0x821C41D0, e821C41D0},
    {0x821C43D0, e821C43D0},
    {0x821C47A0, e821C47A0},
    {0x821C4A58, e821C4A58},
    {0x821C5F80, e821C5F80},
    {0x821C60D0, e821C60D0},
    {0x821D7C90, e821D7C90},
    {0x8222C300, e8222C300},
    {0x823EACC8, e823EACC8},
    {0x823F2730, e823F2730},
    {0x823F2830, e823F2830},
    {0x823F2948, e823F2948},
    {0x823F2A58, e823F2A58},
    {0x823F2B80, e823F2B80},
    {0x823F2CA8, e823F2CA8},
    {0x823F2DE8, e823F2DE8},
    {0x823F2EF8, e823F2EF8},
    {0x823F2FF8, e823F2FF8},
    {0x823F30E8, e823F30E8},
    {0x823F3228, e823F3228},
    {0x823F3348, e823F3348},
    {0x823F3410, e823F3410},
    {0x823F3520, e823F3520},
    {0x823F35E8, e823F35E8},
    {0x823F36C8, e823F36C8},
    {0x823F37E8, e823F37E8},
    {0x823F3948, e823F3948},
    {0x823F3A88, e823F3A88},
    {0x823F3BF0, e823F3BF0},
    {0x823F3D10, e823F3D10},
    {0x823F3E90, e823F3E90},
    {0x823F3FF8, e823F3FF8},
    {0x823F40C8, e823F40C8},
    {0x823F4738, e823F4738},
    {0x823F53B8, e823F53B8},
    {0x823F5468, e823F5468},
    {0x824227F0, e824227F0},
    {0x8246B480, e8246B480},
    {0x8246BB98, e8246BB98},
    {0x82578168, e82578168},
    {0x825784A8, e825784A8},
    {0x82578530, e82578530},
    {0x82583688, e82583688},
    {0x82583910, e82583910},
    {0x82587238, e82587238},
    {0x82594190, e82594190},
    {0x825D3778, e825D3778},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_031)), true);

}  // namespace

}  // namespace lifted
