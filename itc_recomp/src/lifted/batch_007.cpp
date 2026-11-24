// Lift sweep batch 007 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/ai_drivers.h"
#include "schema/camera_modes.h"
#include "schema/car_physics.h"
#include "schema/game_modes.h"
#include "schema/post_effects.h"
#include "schema/race_event.h"
#include "schema/race_hud_machine.h"
#include "schema/race_loading.h"
#include "schema/race_overlays.h"
#include "schema/race_scene.h"
#include "schema/race_setup.h"
#include "schema/race_states.h"
#include "schema/race_update.h"
#include "schema/racer_controllers.h"
#include "schema/system_services.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_007, true, "ITC",
              "Run lift sweep batch 007's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82136DC8: lifted from generated/ by lift.py.
lifted::Result f82136DC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82136DC8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-30564
  r11 = uint64_t(int64_t(int64_t(r11) + -30564));
  // addi r3,r31,8064
  r3 = uint64_t(int64_t(int64_t(r31) + 8064));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::PostEffect(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // bl 0x82133c60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82133C60, LIFTED_ARGS(0x82136DF8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r11,r11,-30664
  r11 = uint64_t(int64_t(int64_t(r11) + -30664));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::PostEffect(base, uint32_t(r31)).vtable_address(), uint32_t(r11));
  // clrlwi. r10,r30,31
  r10 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82136e14
  if (cr0.eq) goto loc_82136E14;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82136E14, r31)); r3 = o.r3; f1 = o.f1; }
loc_82136E14:
  // mr r3,r31
  r3 = r31;
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

// sub_82136F50: lifted from generated/ by lift.py.
double f82136F50(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136F50 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8436(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8436);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82136F58: lifted from generated/ by lift.py.
double f82136F58(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136F58 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8440(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8440);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82136F60: lifted from generated/ by lift.py.
double f82136F60(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136F60 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8444(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8444);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82136F68: lifted from generated/ by lift.py.
double f82136F68(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136F68 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8448(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8448);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82136F70: lifted from generated/ by lift.py.
double f82136F70(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136F70 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8452(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8452);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82136F78: lifted from generated/ by lift.py.
double f82136F78(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136F78 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8456(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8456);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82136F80: lifted from generated/ by lift.py.
double f82136F80(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136F80 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8460(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8460);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82136F88: lifted from generated/ by lift.py.
double f82136F88(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136F88 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8464(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8464);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82136F90: lifted from generated/ by lift.py.
double f82136F90(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136F90 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8468(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8468);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82136F98: lifted from generated/ by lift.py.
double f82136F98(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136F98 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8472(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8472);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82136FA0: lifted from generated/ by lift.py.
double f82136FA0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82136FA0 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,8476(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r3) + 8476);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82138520: lifted from generated/ by lift.py.
lifted::Result f82138520(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_82138520 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x82134618
  // (left out: dead in C++)
  { const lifted::Result o = f82134618(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82138550
  if (cr0.eq) goto loc_82138550;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82138550, r31)); r3 = o.r3; f1 = o.f1; }
loc_82138550:
  // mr r3,r31
  r3 = r31;
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

// sub_821392D8: lifted from generated/ by lift.py.
void f821392D8(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_821392D8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,100(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).speed_kmh_address());
  f0 = double(temp.f32);
  // lfs f13,10764(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10764);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82139314
  if (!cr6.lt) goto loc_82139314;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,9576(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9576);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bltlr cr6
  if (cr6.lt) return;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f12,6088(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).target_speed_address());
  f12 = double(temp.f32);
  // lfs f13,4208(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4208);
  f13 = double(temp.f32);
  // fmuls f13,f12,f13
  f13 = double(float(f12 * f13));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bltlr cr6
  if (cr6.lt) return;
loc_82139314:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,16(r4)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r4)).direction_address());
  f0 = double(temp.f32);
  // lfs f12,6136(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).steer_request_address());
  f12 = double(temp.f32);
  // fneg f13,f0
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // lfs f8,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f8 = double(temp.f32);
  // fcmpu cr6,f12,f8
  cr6 = lifted::CompareFloat(f12, f8);
  // blt cr6,0x82139338
  if (cr6.lt) goto loc_82139338;
  // lfs f0,108(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).steer_limit_a_address());
  f0 = double(temp.f32);
  // b 0x8213933c
  goto loc_8213933C;
loc_82139338:
  // lfs f0,112(r3)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).steer_limit_b_address());
  f0 = double(temp.f32);
loc_8213933C:
  // fdivs f0,f12,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f12 / f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f10,8712(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8712);
  f10 = double(temp.f32);
  // fmuls f11,f0,f13
  f11 = double(float(f0 * f13));
  // fcmpu cr6,f11,f10
  cr6 = lifted::CompareFloat(f11, f10);
  // blelr cr6
  if (!cr6.gt) return;
  // lfs f0,104(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).yaw_rate_address());
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f9,f0,f13
  f9 = double(float(f0 * f13));
  // lfs f0,10768(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10768);
  f0 = double(temp.f32);
  // fcmpu cr6,f9,f0
  cr6 = lifted::CompareFloat(f9, f0);
  // bge cr6,0x82139380
  if (!cr6.lt) goto loc_82139380;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f8,6140(r3)
  temp.f32 = float(f8);
  guest::StoreU32(base, schema::AiDriver(base, uint32_t(r3)).throttle_request_address(), temp.u32);
  // lfs f0,8680(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8680);
  f0 = double(temp.f32);
  // fmuls f0,f12,f0
  f0 = double(float(f12 * f0));
  // stfs f0,6136(r3)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AiDriver(base, uint32_t(r3)).steer_request_address(), temp.u32);
loc_82139380:
  // lwz r11,0(r5)
  r11 = guest::LoadU32(base, uint32_t(r5) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10708(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10708);
  f0 = double(temp.f32);
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // blelr cr6
  if (!cr6.gt) return;
  // fcmpu cr6,f9,f10
  cr6 = lifted::CompareFloat(f9, f10);
  // bltlr cr6
  if (cr6.lt) return;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,132(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).corner_grip_address());
  f13 = double(temp.f32);
  // lfs f0,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x821393bc
  if (!cr6.lt) goto loc_821393BC;
  // fmr f0,f13
  f0 = f13;
loc_821393BC:
  // fcmpu cr6,f9,f0
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f9, f0);
  // bgelr cr6
  if (!cr6.lt) return;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stfs f8,6140(r3)
  temp.f32 = float(f8);
  guest::StoreU32(base, schema::AiDriver(base, uint32_t(r3)).throttle_request_address(), temp.u32);
  // stb r11,6132(r3)
  guest::StoreU8(base, schema::AiDriver(base, uint32_t(r3)).handbrake_address(), uint8_t(r11));
  // blr
  return;
}

// sub_821398C0 game::AiDriver::UpdateTrack: lifted from generated/ by lift.py.
lifted::Result f821398C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821398C0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // bl 0x82144ef8
  // (left out: dead in C++)
  { const lifted::Result o = game::AiDriver::UpdateLane(base, LIFTED_ARGS(0x821398D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lha r11,5648(r31)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r31)).path_entry_count_address()))));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // ble cr6,0x82139938
  if (!cr6.gt) goto loc_82139938;
  // addi r11,r31,512
  r11 = uint64_t(int64_t(int64_t(r31) + 512));
  // lha r10,5136(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r11) - 512).path_entry_count_address()))));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // ble cr6,0x8213991c
  if (!cr6.gt) goto loc_8213991C;
  // lha r10,5138(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r11) - 512).path_first_address()))));
  // li r9,80
  r9 = uint64_t(int64_t(80));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // divw r9,r10,r9
  r9 = uint32_t((int32_t(r9) && !(int32_t(r10) == INT32_MIN && int32_t(r9) == -1)) ? int32_t(r10) / int32_t(r9) : 0);
  // mulli r9,r9,80
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(80))));
  // subf r10,r9,r10
  r10 = r10 - r9;
  // rlwinm r10,r10,6,0,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 6) & 0xFFFFFFC0;
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // b 0x82139920
  goto loc_82139920;
loc_8213991C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82139920:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82139938
  if (cr6.eq) goto loc_82139938;
  // mr r3,r31
  r3 = r31;
  // lwz r4,52(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + 52);
  // bl 0x821467f0
  // (left out: dead in C++)
  { const lifted::Result o = game::AiDriver::ReadVehicle(base, LIFTED_ARGS(0x82139934, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82139940
  goto loc_82139940;
loc_82139938:
  // mr r3,r31
  r3 = r31;
  // bl 0x821469d0
  // (left out: dead in C++)
  { const lifted::Result o = game::AiDriver::ReadVehicleOwnSlice(base, LIFTED_ARGS(0x82139940, r31)); r3 = o.r3; f1 = o.f1; }
loc_82139940:
  // mr r4,r31
  r4 = r31;
  // addi r3,r31,6048
  r3 = uint64_t(int64_t(int64_t(r31) + 6048));
  // bl 0x8213fcf8
  // (left out: dead in C++)
  { const lifted::Result o = game::AiSkill::Update(base, LIFTED_ARGS(0x8213994C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82139974
  if (cr0.eq) goto loc_82139974;
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).vehicle_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82139974
  if (cr0.eq) goto loc_82139974;
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f13,6068(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).skill_percentage_address());
  f13 = double(temp.f32);
  // lfs f0,8748(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 8748);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,2432(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::Vehicle(base, uint32_t(r11)).brake_efficiency_address(), temp.u32);
loc_82139974:
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).vehicle_address());
  // lwz r11,6148(r11)
  r11 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r11)).car_flags_address());
  // andi. r11,r11,129
  r11 = r11 & 129;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82139990
  if (cr0.eq) goto loc_82139990;
  // addi r3,r31,512
  r3 = uint64_t(int64_t(int64_t(r31) + 512));
  // bl 0x8213c248
  // (left out: dead in C++)
  r3 = game::AiPath::NoteFixedSections(base, r1, r13, r3);
loc_82139990:
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

// sub_8213A1B8 game::AiDriver::UpdateStarting: lifted from generated/ by lift.py.
lifted::Result f8213A1B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8213A1B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // bl 0x82144f50
  // (left out: dead in C++)
  { const lifted::Result o = game::AiDriver::ReadCar(base, LIFTED_ARGS(0x8213A1D0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x8213a1ec
  if (cr0.eq) goto loc_8213A1EC;
  // mr r3,r31
  r3 = r31;
  // bl 0x821398c0
  // (left out: dead in C++)
  { const lifted::Result o = game::AiDriver::UpdateTrack(base, r1, r13, r3); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32236
  r11 = uint64_t(int64_t(-2112618496));
  // addi r11,r11,-26432
  r11 = uint64_t(int64_t(int64_t(r11) + -26432));
  // stw r11,6148(r31)
  guest::StoreU32(base, schema::AiDriver(base, uint32_t(r31)).update_address(), uint32_t(r11));
loc_8213A1EC:
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

// sub_8213A200: lifted from generated/ by lift.py.
lifted::Result f8213A200(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_8213A200 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
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
  // stfd f30,-40(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -40, std::bit_cast<uint64_t>(f30));
  // stfd f31,-32(r1)
  guest::StoreU64(base, uint32_t(r1) + -32, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r9,0(r5)
  r9 = guest::LoadU32(base, uint32_t(r5) + 0);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r4
  r30 = r4;
  // lfs f31,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // fmr f30,f31
  f30 = f31;
  // lha r11,5654(r31)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r31)).path_look_ahead_address()))));
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // bge cr6,0x8213a244
  if (!cr6.lt) goto loc_8213A244;
  // mr r9,r11
  r9 = r11;
loc_8213A244:
  // lha r11,5648(r31)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r31)).path_entry_count_address()))));
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // bge cr6,0x8213a400
  if (!cr6.lt) goto loc_8213A400;
  // addi r11,r31,512
  r11 = uint64_t(int64_t(int64_t(r31) + 512));
  // li r8,80
  r8 = uint64_t(int64_t(80));
  // lha r10,5136(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r11) - 512).path_entry_count_address()))));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // ble cr6,0x8213a288
  if (!cr6.gt) goto loc_8213A288;
  // lha r10,5138(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r11) - 512).path_first_address()))));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // divw r7,r10,r8
  r7 = uint32_t((int32_t(r8) && !(int32_t(r10) == INT32_MIN && int32_t(r8) == -1)) ? int32_t(r10) / int32_t(r8) : 0);
  // mulli r7,r7,80
  r7 = uint64_t(int64_t(static_cast<int64_t>(r7 * static_cast<uint64_t>(80))));
  // subf r10,r7,r10
  r10 = r10 - r7;
  // rlwinm r10,r10,6,0,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 6) & 0xFFFFFFC0;
  // add r10,r10,r11
  r10 = r10 + r11;
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // b 0x8213a28c
  goto loc_8213A28C;
loc_8213A288:
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_8213A28C:
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8213a2c4
  if (cr6.eq) goto loc_8213A2C4;
  // lwz r7,52(r10)
  r7 = guest::LoadU32(base, uint32_t(r10) + 52);
  // cmplwi cr6,r7,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq cr6,0x8213a2c4
  if (cr6.eq) goto loc_8213A2C4;
  // lfs f0,16(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r30)).direction_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x8213a2b8
  if (cr6.lt) goto loc_8213A2B8;
  // lfs f0,44(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 44);
  f0 = double(temp.f32);
  // lfs f13,144(r31)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).road_offset_address());
  f13 = double(temp.f32);
  // b 0x8213a2c0
  goto loc_8213A2C0;
loc_8213A2B8:
  // lfs f0,144(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).road_offset_address());
  f0 = double(temp.f32);
  // lfs f13,40(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 40);
  f13 = double(temp.f32);
loc_8213A2C0:
  // fsubs f30,f0,f13
  lifted::ScalarFloatMode();
  f30 = double(float(f0 - f13));
loc_8213A2C4:
  // lfs f0,116(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).half_width_address());
  f0 = double(temp.f32);
  // fcmpu cr6,f30,f0
  cr6 = lifted::CompareFloat(f30, f0);
  // blt cr6,0x8213a3e8
  if (cr6.lt) goto loc_8213A3E8;
  // lha r10,5136(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r11) - 512).path_entry_count_address()))));
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // ble cr6,0x8213a300
  if (!cr6.gt) goto loc_8213A300;
  // lha r10,5138(r11)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiDriver(base, uint32_t(r11) - 512).path_first_address()))));
  // add r10,r10,r9
  r10 = r10 + r9;
  // divw r9,r10,r8
  r9 = uint32_t((int32_t(r8) && !(int32_t(r10) == INT32_MIN && int32_t(r8) == -1)) ? int32_t(r10) / int32_t(r8) : 0);
  // mulli r9,r9,80
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(80))));
  // subf r10,r9,r10
  r10 = r10 - r9;
  // rlwinm r10,r10,6,0,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 6) & 0xFFFFFFC0;
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // b 0x8213a304
  goto loc_8213A304;
loc_8213A300:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8213A304:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8213a400
  if (cr6.eq) goto loc_8213A400;
  // lfs f0,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 0);
  f0 = double(temp.f32);
  // lfs f13,80(r31)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).point_x_address());
  f13 = double(temp.f32);
  // fsubs f1,f0,f13
  f1 = double(float(f0 - f13));
  // lfs f0,8(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8);
  f0 = double(temp.f32);
  // lfs f13,88(r31)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).point_z_address());
  f13 = double(temp.f32);
  // fsubs f2,f0,f13
  f2 = double(float(f0 - f13));
  // fcmpu cr6,f1,f31
  cr6 = lifted::CompareFloat(f1, f31);
  // bne cr6,0x8213a334
  if (!cr6.eq) goto loc_8213A334;
  // fcmpu cr6,f2,f31
  cr6 = lifted::CompareFloat(f2, f31);
  // beq cr6,0x8213a400
  if (cr6.eq) goto loc_8213A400;
loc_8213A334:
  // bl 0x825a8770
  // (left out: dead in C++)
  { const lifted::Result o = game::atan2(base, LIFTED_ARGS(0x8213A338, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lfs f0,96(r31)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).heading_address());
  f0 = double(temp.f32);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // fneg f0,f0
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f0) ^ 0x8000000000000000));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // bl 0x82145408
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82145408, LIFTED_ARGS(0x8213A354, r31)); r3 = o.r3; f1 = o.f1; }
  // lfs f0,6116(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).target_angle_address());
  f0 = double(temp.f32);
  // lfs f13,80(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f13 = double(temp.f32);
  // fmuls f12,f0,f13
  f12 = double(float(f0 * f13));
  // fcmpu cr6,f12,f31
  cr6 = lifted::CompareFloat(f12, f31);
  // beq cr6,0x8213a374
  if (cr6.eq) goto loc_8213A374;
  // lfs f12,6136(r31)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).steer_request_address());
  f12 = double(temp.f32);
  // fadds f0,f12,f0
  f0 = double(float(f12 + f0));
  // stfs f0,6136(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AiDriver(base, uint32_t(r31)).steer_request_address(), temp.u32);
loc_8213A374:
  // lfs f12,6136(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).steer_request_address());
  f12 = double(temp.f32);
  // fcmpu cr6,f30,f31
  cr6 = lifted::CompareFloat(f30, f31);
  // lfs f0,6112(r31)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).slide_angle_address());
  f0 = double(temp.f32);
  // fadds f12,f0,f12
  f12 = double(float(f0 + f12));
  // stfs f12,6136(r31)
  temp.f32 = float(f12);
  guest::StoreU32(base, schema::AiDriver(base, uint32_t(r31)).steer_request_address(), temp.u32);
  // blt cr6,0x8213a3cc
  if (cr6.lt) goto loc_8213A3CC;
  // lfs f0,128(r31)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).car_radius_address());
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fdivs f0,f30,f0
  f0 = double(float(f30 / f0));
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // lfs f13,-30368(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -30368);
  f13 = double(temp.f32);
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x8213a3bc
  if (cr6.lt) goto loc_8213A3BC;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,8132(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8132);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x8213a3c4
  if (!cr6.gt) goto loc_8213A3C4;
loc_8213A3BC:
  // fmr f0,f13
  lifted::ScalarFloatMode();
  f0 = f13;
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r1) + 80, temp.u32);
loc_8213A3C4:
  // fadds f0,f12,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f12 + f0));
  // stfs f0,6136(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AiDriver(base, uint32_t(r31)).steer_request_address(), temp.u32);
loc_8213A3CC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x821392d8
  // (left out: dead in C++)
  f821392D8(base, r3, r4, r5);
  // b 0x8213a408
  goto loc_8213A408;
loc_8213A3E8:
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // stw r11,40(r30)
  guest::StoreU32(base, schema::AiCorner(base, uint32_t(r30)).kind_address(), uint32_t(r11));
  // bl 0x82139a08
  // (left out: dead in C++)
  r3 = f82139A08(base, r1, r13, r3, r4);
  // b 0x8213a408
  goto loc_8213A408;
loc_8213A400:
  // mr r3,r31
  r3 = r31;
  // bl 0x82138f00
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82138F00, LIFTED_ARGS(0x8213A408, r31)); r3 = o.r3; f1 = o.f1; }
loc_8213A408:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f30,-40(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -40)));
  // lfd f31,-32(r1)
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -32)));
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8213AB40: lifted from generated/ by lift.py.
lifted::Result f8213AB40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_8213AB40 (lifted)");
  uint64_t r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  lifted::Cr cr0;
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
  // lwz r11,0(r4)
  r11 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r4)).data_address());
  // lha r10,10(r4)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiCorner(base, uint32_t(r4)).entry_offset_address()))));
  // lha r9,52(r11)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::RouteCorner(base, uint32_t(r11)).apex_address()))));
  // add. r9,r9,r10
  r9 = r9 + r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // blt 0x8213ab78
  if (cr0.lt) goto loc_8213AB78;
  // lha r11,50(r11)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::RouteCorner(base, uint32_t(r11)).turn_in_address()))));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8213a200
  // (left out: dead in C++)
  { const lifted::Result o = f8213A200(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8213ab84
  goto loc_8213AB84;
loc_8213AB78:
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,40(r4)
  guest::StoreU32(base, uint32_t(r4) + 40, uint32_t(r11));
  // bl 0x82139a08
  // (left out: dead in C++)
  r3 = f82139A08(base, r1, r13, r3, r4);
loc_8213AB84:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8213BDF0: lifted from generated/ by lift.py.
void f8213BDF0(uint8_t* base, uint64_t r1, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_8213BDF0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,0(r5)
  r11 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r5)).data_address());
  // lwz r8,0(r4)
  r8 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r4)).data_address());
  // lwz r10,0(r6)
  r10 = guest::LoadU32(base, uint32_t(r6) + 0);
  // lha r11,0(r11)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::RouteCorner(base, uint32_t(r11)).entry_count_address()))));
  // lha r7,50(r8)
  r7 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::RouteCorner(base, uint32_t(r8)).turn_in_address()))));
  // addi r9,r11,-1
  r9 = uint64_t(int64_t(int64_t(r11) + -1));
  // mr r11,r10
  r11 = r10;
  // cmpw cr6,r11,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r7), so);
  // ble cr6,0x8213be20
  if (!cr6.gt) goto loc_8213BE20;
  // mr r11,r7
  r11 = r7;
  // subf r7,r10,r11
  r7 = r11 - r10;
  // add r9,r7,r9
  r9 = r7 + r9;
loc_8213BE20:
  // lha r8,50(r8)
  r8 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::RouteCorner(base, uint32_t(r8)).turn_in_address()))));
  // rlwinm r7,r9,3,0,28
  r7 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 3) & 0xFFFFFFF8;
  // lwz r6,4(r5)
  r6 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r5)).table_address());
  // rlwinm r9,r11,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // mr r5,r8
  r5 = r8;
  // lwz r8,4(r4)
  r8 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r4)).table_address());
  // add r7,r7,r6
  r7 = r7 + r6;
  // subf r10,r10,r5
  r10 = r5 - r10;
  // add r9,r9,r8
  r9 = r9 + r8;
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lfs f0,4(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 4);
  f0 = double(temp.f32);
  // lfs f13,4(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 4);
  f13 = double(temp.f32);
  // sth r11,14(r4)
  guest::StoreU16(base, schema::AiCorner(base, uint32_t(r4)).position_address(), uint16_t(r11));
  // std r10,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r10);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // stb r8,36(r4)
  guest::StoreU8(base, schema::AiCorner(base, uint32_t(r4)).joined_before_address(), uint8_t(r8));
  // lfd f13,-16(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fdivs f0,f0,f13
  f0 = double(float(f0 / f13));
  // stfs f0,28(r4)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AiCorner(base, uint32_t(r4)).slope_before_address(), temp.u32);
  // blr
  return;
}

// sub_8213BE80: lifted from generated/ by lift.py.
uint64_t f8213BE80(uint8_t* base, uint64_t r1, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_8213BE80 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r10,0(r5)
  r10 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r5)).data_address());
  // lwz r8,0(r4)
  r8 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r4)).data_address());
  // lwz r9,0(r6)
  r9 = guest::LoadU32(base, uint32_t(r6) + 0);
  // addi r7,r10,48
  r7 = uint64_t(int64_t(int64_t(r10) + 48));
  // mr r11,r9
  r11 = r9;
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::RouteCorner(base, uint32_t(r10)).entry_count_address()))));
  // lha r8,50(r8)
  r8 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::RouteCorner(base, uint32_t(r8)).turn_in_address()))));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpw cr6,r11,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r8), so);
  // ble cr6,0x8213beb4
  if (!cr6.gt) goto loc_8213BEB4;
  // mr r11,r8
  r11 = r8;
  // subf r9,r9,r11
  r9 = r11 - r9;
  // add r10,r9,r10
  r10 = r9 + r10;
loc_8213BEB4:
  // lwz r8,4(r5)
  r8 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r5)).table_address());
  // rlwinm r9,r10,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // lwz r3,4(r4)
  r3 = guest::LoadU32(base, schema::AiCorner(base, uint32_t(r4)).table_address());
  // rlwinm r6,r11,3,0,28
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // add r9,r9,r8
  r9 = r9 + r8;
  // add r6,r6,r3
  r6 = r6 + r3;
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lfs f13,4(r9)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + 4);
  f13 = double(temp.f32);
  // lfs f0,4(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 4);
  f0 = double(temp.f32);
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // lha r9,4(r7)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::RouteCorner(base, uint32_t(r7) - 48).apex_address()))));
  // stb r8,37(r5)
  guest::StoreU8(base, schema::AiCorner(base, uint32_t(r5)).joined_after_address(), uint8_t(r8));
  // subf r10,r9,r10
  r10 = r10 - r9;
  // lha r8,14(r5)
  r8 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiCorner(base, uint32_t(r5)).position_address()))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // mr r9,r8
  r9 = r8;
  // std r10,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r10);
  // lfd f13,-16(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fdivs f0,f0,f13
  f0 = double(float(f0 / f13));
  // stfs f0,32(r5)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AiCorner(base, uint32_t(r5)).slope_after_address(), temp.u32);
  // lha r10,4(r7)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::RouteCorner(base, uint32_t(r7) - 48).apex_address()))));
  // cmpw cr6,r9,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r10), so);
  // ble cr6,0x8213bf20
  if (!cr6.gt) goto loc_8213BF20;
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // sth r10,14(r5)
  guest::StoreU16(base, schema::AiCorner(base, uint32_t(r5)).position_address(), uint16_t(r10));
loc_8213BF20:
  // sth r11,14(r4)
  guest::StoreU16(base, schema::AiCorner(base, uint32_t(r4)).position_address(), uint16_t(r11));
  // blr
  return r3;
}

// sub_8213C248 game::AiPath::NoteFixedSections: lifted from generated/ by lift.py.
uint64_t f8213C248(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8213C248 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r30,r3
  r30 = r3;
  // bl 0x821434e8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceEvent::Current(base, LIFTED_ARGS(0x8213C264, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::AiPathAt512(base, uint32_t(r30)).unknown_0_address());
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // lwz r3,160(r11)
  r3 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r11)).vehicle_address());
  // beq 0x8213c340
  if (cr0.eq) goto loc_8213C340;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8213c340
  if (cr6.eq) goto loc_8213C340;
  // lwz r11,6168(r3)
  r11 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r3)).status_address() + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8213c340
  if (!cr6.eq) goto loc_8213C340;
  // bl 0x820c0168
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::Identity(base, LIFTED_ARGS(0x8213C28C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r10,r3,4
  r10 = uint64_t(int64_t(int64_t(r3) + 4));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r9,8(r31)
  schema::RaceEvent(base, uint32_t(r31)).set_on_section_14_bits(uint32_t(r9));
  // lha r11,0(r10)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r10) + 0))));
  // cmpwi cr6,r11,13
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 13, so);
  // beq cr6,0x8213c2d4
  if (cr6.eq) goto loc_8213C2D4;
  // cmpwi cr6,r11,14
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 14, so);
  // bne cr6,0x8213c2cc
  if (!cr6.eq) goto loc_8213C2CC;
  // lha r11,2(r10)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r10) + 2))));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x8213c2cc
  if (cr0.lt) goto loc_8213C2CC;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bge cr6,0x8213c2cc
  if (!cr6.lt) goto loc_8213C2CC;
  // stw r8,8(r31)
  schema::RaceEvent(base, uint32_t(r31)).set_on_section_14_bits(uint32_t(r8));
  // b 0x8213c2ec
  goto loc_8213C2EC;
loc_8213C2CC:
  // stw r9,4(r31)
  schema::RaceEvent(base, uint32_t(r31)).set_on_section_13_bits(uint32_t(r9));
  // b 0x8213c2ec
  goto loc_8213C2EC;
loc_8213C2D4:
  // lha r11,2(r10)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r10) + 2))));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x8213c2ec
  if (cr0.lt) goto loc_8213C2EC;
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bge cr6,0x8213c2ec
  if (!cr6.lt) goto loc_8213C2EC;
  // stw r8,4(r31)
  schema::RaceEvent(base, uint32_t(r31)).set_on_section_13_bits(uint32_t(r8));
loc_8213C2EC:
  // stw r9,16(r31)
  schema::RaceEvent(base, uint32_t(r31)).set_on_section_5_bits(uint32_t(r9));
  // lha r11,0(r10)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r10) + 0))));
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x8213c328
  if (cr6.eq) goto loc_8213C328;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bne cr6,0x8213c320
  if (!cr6.eq) goto loc_8213C320;
  // lha r11,2(r10)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r10) + 2))));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x8213c320
  if (cr0.lt) goto loc_8213C320;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bge cr6,0x8213c320
  if (!cr6.lt) goto loc_8213C320;
  // stw r8,16(r31)
  schema::RaceEvent(base, uint32_t(r31)).set_on_section_5_bits(uint32_t(r8));
  // b 0x8213c340
  goto loc_8213C340;
loc_8213C320:
  // stw r9,12(r31)
  schema::RaceEvent(base, uint32_t(r31)).set_on_section_4_bits(uint32_t(r9));
  // b 0x8213c340
  goto loc_8213C340;
loc_8213C328:
  // lha r11,2(r10)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r10) + 2))));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x8213c340
  if (cr0.lt) goto loc_8213C340;
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bge cr6,0x8213c340
  if (!cr6.lt) goto loc_8213C340;
  // stw r8,12(r31)
  schema::RaceEvent(base, uint32_t(r31)).set_on_section_4_bits(uint32_t(r8));
loc_8213C340:
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
  return r3;
}

// sub_8213D898: lifted from generated/ by lift.py.
uint64_t f8213D898(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8213D898 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lwz r10,0(r3)
  r10 = guest::LoadU32(base, schema::AiPathAt512(base, uint32_t(r3)).unknown_0_address());
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,4(r4)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiOtherSpan(base, uint32_t(r4)).other_offset_address());
  f0 = double(temp.f32);
  // lfs f13,12(r4)
  temp.u32 = guest::LoadU32(base, schema::AiOtherSpan(base, uint32_t(r4)).widths_address());
  f13 = double(temp.f32);
  // lfs f12,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f12 = double(temp.f32);
  // lfs f11,144(r10)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r10)).road_offset_address());
  f11 = double(temp.f32);
  // fadds f13,f13,f12
  f13 = double(float(f13 + f12));
  // fsubs f0,f11,f0
  f0 = double(float(f11 - f0));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x8213d9c4
  if (!cr6.lt) goto loc_8213D9C4;
  // fneg f13,f13
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) ^ 0x8000000000000000));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x8213d9c4
  if (!cr6.gt) goto loc_8213D9C4;
  // lha r11,5136(r3)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiPathAt512(base, uint32_t(r3)).unknown_5136_address()))));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // ble cr6,0x8213d9c4
  if (!cr6.gt) goto loc_8213D9C4;
  // lha r11,5138(r3)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiPathAt512(base, uint32_t(r3)).unknown_5138_address()))));
  // li r10,80
  r10 = uint64_t(int64_t(80));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // divw r10,r11,r10
  r10 = uint32_t((int32_t(r10) && !(int32_t(r11) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r11) / int32_t(r10) : 0);
  // mulli r10,r10,80
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(80))));
  // subf r11,r10,r11
  r11 = r11 - r10;
  // rlwinm r11,r11,6,0,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 6) & 0xFFFFFFC0;
  // add r11,r11,r3
  r11 = r11 + r3;
  // addic. r9,r11,16
  ca = uint32_t(r11) > 4294967279;
  r9 = uint64_t(int64_t(int64_t(r11) + 16));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x8213d9c4
  if (cr0.eq) goto loc_8213D9C4;
  // lwz r7,52(r9)
  r7 = guest::LoadU32(base, uint32_t(r9) + 52);
  // cmplwi r7,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq 0x8213d9c4
  if (cr0.eq) goto loc_8213D9C4;
  // lbz r10,56(r7)
  r10 = guest::LoadU8(base, uint32_t(r7) + 56);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // clrlwi r8,r10,28
  r8 = uint32_t(r10) & 0xF;
  // cmplwi cr6,r8,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 1, so);
  // ble cr6,0x8213d960
  if (!cr6.gt) goto loc_8213D960;
  // lwz r10,20(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 20);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble 0x8213d95c
  if (!cr0.gt) goto loc_8213D95C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x8213d954
  if (cr6.lt) goto loc_8213D954;
  // addi r11,r10,1
  r11 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // blt cr6,0x8213d960
  if (cr6.lt) goto loc_8213D960;
loc_8213D954:
  // addi r11,r10,-1
  r11 = uint64_t(int64_t(int64_t(r10) + -1));
  // b 0x8213d960
  goto loc_8213D960;
loc_8213D95C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_8213D960:
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f11,12(r7)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r7) + 12);
  f11 = double(temp.f32);
  // lfs f13,7140(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 7140);
  f13 = double(temp.f32);
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // lbz r10,56(r7)
  r10 = guest::LoadU8(base, uint32_t(r7) + 56);
  // clrlwi r10,r10,28
  r10 = uint32_t(r10) & 0xF;
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // std r10,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r10);
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f10,f0
  f10 = double(float(f0));
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fsubs f12,f0,f12
  f12 = double(float(f0 - f12));
  // fnmsubs f13,f12,f13,f10
  f13 = double(float(-std::fma(f12, f13, -f10)));
  // fdivs f0,f13,f0
  f0 = double(float(f13 / f0));
  // fmuls f0,f0,f11
  f0 = double(float(f0 * f11));
  // stfs f0,16(r4)
  temp.f32 = float(f0);
  guest::StoreU32(base, schema::AiOtherSpan(base, uint32_t(r4)).side_offset_address(), temp.u32);
  // lwz r10,20(r9)
  r10 = guest::LoadU32(base, uint32_t(r9) + 20);
  // cmpw cr6,r11,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r10), so);
  // ble cr6,0x8213d9c0
  if (!cr6.gt) goto loc_8213D9C0;
  // bl 0x8213cb30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8213CB30, LIFTED_ARGS(0x8213D9BC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8213d9c4
  goto loc_8213D9C4;
loc_8213D9C0:
  // bl 0x8213cd50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8213CD50, LIFTED_ARGS(0x8213D9C4, r31)); r3 = o.r3; f1 = o.f1; }
loc_8213D9C4:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8213F868: lifted from generated/ by lift.py.
double f8213F868(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8213F868 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r9,500(r3)
  r9 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).profile_address());
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // lwz r11,-32024(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -32024);
  // addi r11,r11,2184
  r11 = uint64_t(int64_t(int64_t(r11) + 2184));
  // beq 0x8213f8b4
  if (cr0.eq) goto loc_8213F8B4;
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, schema::RaceProgressPart(base, uint32_t(r11) - 2184).unknown_2184_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // ble 0x8213f8b4
  if (!cr0.gt) goto loc_8213F8B4;
  // lha r7,0(r9)
  r7 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::AiProfile(base, uint32_t(r9)).key_address()))));
  // addi r9,r11,8
  r9 = uint64_t(int64_t(int64_t(r11) + 8));
loc_8213F898:
  // lwz r6,0(r9)
  r6 = guest::LoadU32(base, schema::RaceProgressPart(base, uint32_t(r9) - 2192).unknown_2192_address());
  // cmpw cr6,r6,r7
  cr6 = lifted::Compare<int32_t>(int32_t(r6), int32_t(r7), so);
  // beq cr6,0x8213f8e4
  if (cr6.eq) goto loc_8213F8E4;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r9,r9,972
  r9 = uint64_t(int64_t(int64_t(r9) + 972));
  // cmpw cr6,r10,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r8), so);
  // blt cr6,0x8213f898
  if (cr6.lt) goto loc_8213F898;
loc_8213F8B4:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_8213F8B8:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8213f8f4
  if (cr6.eq) goto loc_8213F8F4;
  // lwa r11,784(r11)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r11) + 784))));
  // std r11,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r11);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfd f0,-16(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -16)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-32676(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -32676);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // blr
  return f1;
loc_8213F8E4:
  // mulli r10,r10,972
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(972))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // b 0x8213f8b8
  goto loc_8213F8B8;
loc_8213F8F4:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_82141668 game::EventCondition::Power: lifted from generated/ by lift.py.
lifted::Result f82141668(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_82141668 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  guest::StoreU64(base, uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  guest::StoreU64(base, uint32_t(r1) + -16, r31);
  // ld r12,-4096(r1)
  r12 = guest::LoadU64(base, uint32_t(r1) + -4096);
  // stwu r1,-5088(r1)
  ea = -5088 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = game::SaveData::Of(base, LIFTED_ARGS(0x8214168C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // ori r10,r11,39200
  r10 = r11 | 39200;
  // addi r11,r3,6120
  r11 = uint64_t(int64_t(int64_t(r3) + 6120));
  // lwzx r10,r3,r10
  r10 = guest::LoadU32(base, uint32_t(r3) + uint32_t(r10));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lha r9,204(r11)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::SaveData(base, uint32_t(r11) - 6120).garage_head_address()))));
  // b 0x821416b0
  goto loc_821416B0;
loc_821416A8:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_821416B0:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x821416a8
  if (!cr0.eq) goto loc_821416A8;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 2))));
  // addi r3,r1,1152
  r3 = uint64_t(int64_t(int64_t(r1) + 1152));
  // mulli r10,r10,1968
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(1968))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r31,r11,212
  r31 = uint64_t(int64_t(int64_t(r11) + 212));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Init(base, LIFTED_ARGS(0x821416D4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,1152
  r3 = uint64_t(int64_t(int64_t(r1) + 1152));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // mr r4,r31
  r4 = r31;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x821416E4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,1152
  r3 = uint64_t(int64_t(int64_t(r1) + 1152));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = game::Car::Refresh(base, LIFTED_ARGS(0x821416EC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,1152
  r3 = uint64_t(int64_t(int64_t(r1) + 1152));
  // bl 0x820c4550
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820C4550(base, LIFTED_ARGS(0x821416F4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,992
  r4 = uint64_t(int64_t(int64_t(r11) + 992));
  // bl 0x820ce7f0
  // (left out: dead in C++)
  f820CE7F0(base, r1, r13, r3, r4);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820ce1d8
  // (left out: dead in C++)
  { const lifted::Result o = game::PowerCurve::ShiftPeak(base, LIFTED_ARGS(0x82141710, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,1136
  r4 = uint64_t(int64_t(int64_t(r1) + 1136));
  // mr r3,r30
  r3 = r30;
  // bl 0x82140400
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82140400, LIFTED_ARGS(0x8214171C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,5088
  r1 = uint64_t(int64_t(int64_t(r1) + 5088));
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

// sub_821462A0: lifted from generated/ by lift.py.
uint64_t f821462A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821462A0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x821451b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821451B8, LIFTED_ARGS(0x821462C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,480(r31)
  r11 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).watched_address());
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blt 0x82146370
  if (cr0.lt) goto loc_82146370;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82139728
  // (left out: dead in C++)
  { const lifted::Result o = game::AiDriver::Of(base, LIFTED_ARGS(0x821462D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,32(r3)
  r11 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).state_address());
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x82146370
  if (!cr6.eq) goto loc_82146370;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,480(r31)
  r10 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).watched_address());
  // lfs f13,124(r31)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).car_half_length_address());
  f13 = double(temp.f32);
  // lfs f11,124(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).car_half_length_address());
  f11 = double(temp.f32);
  // lfs f12,100(r3)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r3)).speed_kmh_address());
  f12 = double(temp.f32);
  // lfs f0,8680(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8680);
  f0 = double(temp.f32);
  // mulli r11,r10,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // add r11,r11,r31
  r11 = r11 + r31;
  // lfs f10,172(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 172);
  f10 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fneg f10,f10
  f10 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f10) ^ 0x8000000000000000));
  // fnmsubs f0,f13,f0,f10
  f0 = double(float(-std::fma(f13, f0, -f10)));
  // fsubs f11,f0,f11
  f11 = double(float(f0 - f11));
  // lfs f0,2612(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f11,f0
  cr6 = lifted::CompareFloat(f11, f0);
  // blt cr6,0x82146354
  if (cr6.lt) goto loc_82146354;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,100(r31)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).speed_kmh_address());
  f0 = double(temp.f32);
  // lfs f10,136(r31)
  temp.u32 = guest::LoadU32(base, schema::AiDriver(base, uint32_t(r31)).distance_factor_address());
  f10 = double(temp.f32);
  // lfs f13,10764(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10764);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x82146344
  if (!cr6.gt) goto loc_82146344;
  // fdivs f0,f13,f0
  f0 = double(float(f13 / f0));
  // b 0x8214634c
  goto loc_8214634C;
loc_82146344:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f0 = double(temp.f32);
loc_8214634C:
  // fmuls f0,f0,f10
  lifted::ScalarFloatMode();
  f0 = double(float(f0 * f10));
  // fmadds f12,f0,f11,f12
  f12 = double(float(std::fma(f0, f11, f12)));
loc_82146354:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r30)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r30) + 0);
  f13 = double(temp.f32);
  // lfs f0,10812(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 10812);
  f0 = double(temp.f32);
  // fmuls f0,f12,f0
  f0 = double(float(f12 * f0));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x82146370
  if (!cr6.gt) goto loc_82146370;
  // stfs f0,0(r30)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r30) + 0, temp.u32);
loc_82146370:
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
  return r3;
}

// sub_82148920: lifted from generated/ by lift.py.
lifted::Result f82148920(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82148920 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,27316(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 27316);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x82148938, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,27320(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 27320);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x82148944, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821489D8: lifted from generated/ by lift.py.
lifted::Result f821489D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821489D8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82148a3c
  if (cr6.eq) goto loc_82148A3C;
  // lis r31,-32158
  r31 = uint64_t(int64_t(-2107506688));
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // lwz r3,-29856(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -29856);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x82148a20
  if (cr6.eq) goto loc_82148A20;
  // bl 0x82459c38
  // (left out: dead in C++)
  { const lifted::Result o = game::closesocket(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, 0x82148A18); r3 = o.r3; f1 = o.f1; }
  // mr r11,r30
  r11 = r30;
  // stw r11,-29856(r31)
  guest::StoreU32(base, uint32_t(r31) + -29856, uint32_t(r11));
loc_82148A20:
  // lis r31,-32158
  r31 = uint64_t(int64_t(-2107506688));
  // lwz r3,-29860(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -29860);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x82148a58
  if (cr6.eq) goto loc_82148A58;
  // bl 0x82459c38
  // (left out: dead in C++)
  { const lifted::Result o = game::closesocket(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, 0x82148A34); r3 = o.r3; f1 = o.f1; }
  // mr r11,r30
  r11 = r30;
  // b 0x82148a54
  goto loc_82148A54;
loc_82148A3C:
  // lis r31,-32158
  r31 = uint64_t(int64_t(-2107506688));
  // lwz r3,-29860(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -29860);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x82148a58
  if (cr6.eq) goto loc_82148A58;
  // bl 0x82459c38
  // (left out: dead in C++)
  { const lifted::Result o = game::closesocket(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, 0x82148A50); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
loc_82148A54:
  // stw r11,-29860(r31)
  guest::StoreU32(base, uint32_t(r31) + -29860, uint32_t(r11));
loc_82148A58:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,27300(r10)
  guest::StoreU32(base, uint32_t(r10) + 27300, uint32_t(r11));
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

// sub_82148A80: lifted from generated/ by lift.py.
lifted::Result f82148A80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82148A80 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
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
  // lis r31,-32158
  r31 = uint64_t(int64_t(-2107506688));
  // lwz r3,-29860(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -29860);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x82148aa4
  if (cr6.eq) goto loc_82148AA4;
  // bl 0x82459c38
  // (left out: dead in C++)
  { const lifted::Result o = game::closesocket(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, 0x82148AA4); r3 = o.r3; f1 = o.f1; }
loc_82148AA4:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // stw r11,27300(r10)
  guest::StoreU32(base, uint32_t(r10) + 27300, uint32_t(r11));
  // bl 0x82459c20
  // (left out: dead in C++)
  { const lifted::Result o = game::socket(base, r1, r13, r3, r4, r5, r7, r8, r9, r10, 0x82148AC0); r3 = o.r3; f1 = o.f1; }
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r3,-29860(r31)
  guest::StoreU32(base, uint32_t(r31) + -29860, uint32_t(r3));
  // li r5,16
  r5 = uint64_t(int64_t(16));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // sth r11,96(r1)
  guest::StoreU16(base, uint32_t(r1) + 96, uint16_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // li r11,1006
  r11 = uint64_t(int64_t(1006));
  // sth r11,98(r1)
  guest::StoreU16(base, uint32_t(r1) + 98, uint16_t(r11));
  // bl 0x82459c60
  // (left out: dead in C++)
  { const lifted::Result o = game::bind(base, r1, r13, r3, r4, r5, r7, r8, r9, r10, 0x82148AE8); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r3,-29860(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + -29860);
  // lis r4,-32764
  r4 = uint64_t(int64_t(-2147221504));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // ori r4,r4,26238
  r4 = r4 | 26238;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82459c48
  // (left out: dead in C++)
  { const lifted::Result o = game::ioctlsocket(base, r1, r13, r3, r4, r5, r7, r8, r9, r10, 0x82148B04); r3 = o.r3; f1 = o.f1; }
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

// sub_82148B60: lifted from generated/ by lift.py.
lifted::Result f82148B60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_82148B60 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lwz r11,28180(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 28180);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r31,r11,30752
  r31 = uint64_t(int64_t(int64_t(r11) + 30752));
  // bne cr6,0x82148ba8
  if (!cr6.eq) goto loc_82148BA8;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // lis r4,8192
  r4 = uint64_t(int64_t(536870912));
  // addi r6,r11,29376
  r6 = uint64_t(int64_t(int64_t(r11) + 29376));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // ori r4,r4,13
  r4 = r4 | 13;
  // bl 0x82295018
  // (left out: dead in C++)
  { const lifted::Result o = f82295018(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
loc_82148BA8:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // lis r4,4096
  r4 = uint64_t(int64_t(268435456));
  // addi r6,r11,29316
  r6 = uint64_t(int64_t(int64_t(r11) + 29316));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // ori r4,r4,7
  r4 = r4 | 7;
  // bl 0x82295018
  // (left out: dead in C++)
  { const lifted::Result o = f82295018(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r4,4096
  r4 = uint64_t(int64_t(268435456));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // addi r6,r11,29312
  r6 = uint64_t(int64_t(int64_t(r11) + 29312));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // ori r4,r4,10
  r4 = r4 | 10;
  // bl 0x82295018
  // (left out: dead in C++)
  { const lifted::Result o = f82295018(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r4,4096
  r4 = uint64_t(int64_t(268435456));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // addi r6,r11,29320
  r6 = uint64_t(int64_t(int64_t(r11) + 29320));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // ori r4,r4,2
  r4 = r4 | 2;
  // bl 0x82295018
  // (left out: dead in C++)
  { const lifted::Result o = f82295018(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r4,8192
  r4 = uint64_t(int64_t(536870912));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // addi r6,r11,29344
  r6 = uint64_t(int64_t(int64_t(r11) + 29344));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // ori r4,r4,8
  r4 = r4 | 8;
  // bl 0x82295018
  // (left out: dead in C++)
  { const lifted::Result o = f82295018(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r4,8192
  r4 = uint64_t(int64_t(536870912));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // addi r6,r11,29328
  r6 = uint64_t(int64_t(int64_t(r11) + 29328));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // ori r4,r4,5
  r4 = r4 | 5;
  // bl 0x82295018
  // (left out: dead in C++)
  { const lifted::Result o = f82295018(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r4,8192
  r4 = uint64_t(int64_t(536870912));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // addi r6,r11,29336
  r6 = uint64_t(int64_t(int64_t(r11) + 29336));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // ori r4,r4,4
  r4 = r4 | 4;
  // bl 0x82295018
  // (left out: dead in C++)
  { const lifted::Result o = f82295018(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r4,8192
  r4 = uint64_t(int64_t(536870912));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // addi r6,r11,29352
  r6 = uint64_t(int64_t(int64_t(r11) + 29352));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // ori r4,r4,16
  r4 = r4 | 16;
  // bl 0x82295018
  // (left out: dead in C++)
  { const lifted::Result o = f82295018(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r4,8192
  r4 = uint64_t(int64_t(536870912));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // addi r6,r11,29360
  r6 = uint64_t(int64_t(int64_t(r11) + 29360));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // ori r4,r4,17
  r4 = r4 | 17;
  // bl 0x82295018
  // (left out: dead in C++)
  { const lifted::Result o = f82295018(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r4,8192
  r4 = uint64_t(int64_t(536870912));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // addi r6,r11,29368
  r6 = uint64_t(int64_t(int64_t(r11) + 29368));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // ori r4,r4,18
  r4 = r4 | 18;
  // bl 0x82295018
  // (left out: dead in C++)
  { const lifted::Result o = f82295018(base, r1, r13, r3, r4, r5, r6); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // ori r4,r4,32779
  r4 = r4 | 32779;
  // lwz r5,29304(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 29304);
  // bl 0x82294f70
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserSetContextEx(base, LIFTED_ARGS(0x82148CBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwz r5,29308(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 29308);
  // bl 0x82294f70
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserSetContextEx(base, LIFTED_ARGS(0x82148CD0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r5,28180(r30)
  r5 = guest::LoadU32(base, uint32_t(r30) + 28180);
  // lwz r3,84(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 84);
  // ori r4,r4,32778
  r4 = r4 | 32778;
  // bl 0x82294f70
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserSetContextEx(base, LIFTED_ARGS(0x82148CE4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_82148D00: lifted from generated/ by lift.py.
lifted::Result f82148D00(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_82148D00 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r26,-32148
  r26 = uint64_t(int64_t(-2106851328));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r29,2
  r29 = uint64_t(int64_t(2));
  // lis r25,-32148
  r25 = uint64_t(int64_t(-2106851328));
  // lwz r11,28180(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 28180);
  // stw r31,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r31));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82148d68
  if (!cr6.eq) goto loc_82148D68;
  // lwz r11,29324(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 29324);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82148d60
  if (cr6.lt) goto loc_82148D60;
  // beq cr6,0x82148d58
  if (cr6.eq) goto loc_82148D58;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82148d50
  if (cr6.lt) goto loc_82148D50;
  // beq cr6,0x82148d60
  if (cr6.eq) goto loc_82148D60;
  // lwz r27,112(r1)
  r27 = guest::LoadU32(base, uint32_t(r1) + 112);
  // b 0x82148d6c
  goto loc_82148D6C;
loc_82148D50:
  // mr r27,r29
  r27 = r29;
  // b 0x82148d6c
  goto loc_82148D6C;
loc_82148D58:
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // b 0x82148d6c
  goto loc_82148D6C;
loc_82148D60:
  // li r27,3
  r27 = uint64_t(int64_t(3));
  // b 0x82148d6c
  goto loc_82148D6C;
loc_82148D68:
  // mr r27,r31
  r27 = r31;
loc_82148D6C:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // addi r30,r11,24704
  r30 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82148D84, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r31,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r31));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r30,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r30));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // addi r28,r11,30752
  r28 = uint64_t(int64_t(int64_t(r11) + 30752));
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // mr r3,r27
  r3 = r27;
  // lwz r4,84(r28)
  r4 = guest::LoadU32(base, uint32_t(r28) + 84);
  // bl 0x825765f0
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionSearchEx(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,112(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 112);
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x82148DC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r7,r31
  r7 = r31;
  // addi r9,r10,30936
  r9 = uint64_t(int64_t(int64_t(r10) + 30936));
  // stw r3,27312(r11)
  guest::StoreU32(base, uint32_t(r11) + 27312, uint32_t(r3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,29312(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29312);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82148e08
  if (cr6.eq) goto loc_82148E08;
  // lis r10,4096
  r10 = uint64_t(int64_t(268435456));
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // ori r10,r10,10
  r10 = r10 | 10;
  // stw r10,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r10));
  // stw r11,16(r9)
  guest::StoreU32(base, uint32_t(r9) + 16, uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,8(r9)
  guest::StoreU8(base, uint32_t(r9) + 8, uint8_t(r11));
loc_82148E08:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r8,29316(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 29316);
  // cmplwi cr6,r8,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 2, so);
  // beq cr6,0x82148e48
  if (cr6.eq) goto loc_82148E48;
  // lis r11,4096
  r11 = uint64_t(int64_t(268435456));
  // clrlwi r10,r7,16
  r10 = uint32_t(r7) & 0xFFFF;
  // ori r6,r11,7
  r6 = r11 | 7;
  // mulli r11,r10,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(24))));
  // stwx r6,r11,r9
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r9), uint32_t(r6));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r5,r9,16
  r5 = uint64_t(int64_t(int64_t(r9) + 16));
  // clrlwi r7,r10,16
  r7 = uint32_t(r10) & 0xFFFF;
  // addi r4,r9,8
  r4 = uint64_t(int64_t(int64_t(r9) + 8));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stwx r8,r11,r5
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r5), uint32_t(r8));
  // stbx r10,r11,r4
  guest::StoreU8(base, uint32_t(r11) + uint32_t(r4), uint8_t(r10));
loc_82148E48:
  // lwz r6,28180(r26)
  r6 = guest::LoadU32(base, uint32_t(r26) + 28180);
  // cmplwi cr6,r6,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // bne cr6,0x82148f00
  if (!cr6.eq) goto loc_82148F00;
  // lwz r11,29324(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 29324);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82148e9c
  if (cr6.eq) goto loc_82148E9C;
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bgt cr6,0x82148f00
  if (cr6.gt) goto loc_82148F00;
  // lis r11,8192
  r11 = uint64_t(int64_t(536870912));
  // clrlwi r10,r7,16
  r10 = uint32_t(r7) & 0xFFFF;
  // ori r5,r11,14
  r5 = r11 | 14;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r9,16
  r4 = uint64_t(int64_t(int64_t(r9) + 16));
  // addi r31,r9,8
  r31 = uint64_t(int64_t(int64_t(r9) + 8));
  // ld r8,29376(r11)
  r8 = guest::LoadU64(base, uint32_t(r11) + 29376);
  // mulli r11,r10,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(24))));
  // stwx r5,r11,r9
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r9), uint32_t(r5));
  // stdx r8,r11,r4
  guest::StoreU64(base, uint32_t(r11) + uint32_t(r4), r8);
  // stbx r29,r11,r31
  guest::StoreU8(base, uint32_t(r11) + uint32_t(r31), uint8_t(r29));
  // b 0x82148ef8
  goto loc_82148EF8;
loc_82148E9C:
  // lis r10,8192
  r10 = uint64_t(int64_t(536870912));
  // clrlwi r11,r7,16
  r11 = uint32_t(r7) & 0xFFFF;
  // ori r5,r10,15
  r5 = r10 | 15;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r7,r11,1
  r7 = uint64_t(int64_t(int64_t(r11) + 1));
  // mulli r11,r11,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(24))));
  // ld r10,29384(r10)
  r10 = guest::LoadU64(base, uint32_t(r10) + 29384);
  // stwx r5,r11,r9
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r9), uint32_t(r5));
  // lis r8,8192
  r8 = uint64_t(int64_t(536870912));
  // addi r4,r9,16
  r4 = uint64_t(int64_t(int64_t(r9) + 16));
  // addi r31,r9,8
  r31 = uint64_t(int64_t(int64_t(r9) + 8));
  // ori r26,r8,14
  r26 = r8 | 14;
  // lis r8,-32148
  r8 = uint64_t(int64_t(-2106851328));
  // addi r25,r9,16
  r25 = uint64_t(int64_t(int64_t(r9) + 16));
  // stdx r10,r11,r4
  guest::StoreU64(base, uint32_t(r11) + uint32_t(r4), r10);
  // clrlwi r10,r7,16
  r10 = uint32_t(r7) & 0xFFFF;
  // stbx r29,r11,r31
  guest::StoreU8(base, uint32_t(r11) + uint32_t(r31), uint8_t(r29));
  // addi r24,r9,8
  r24 = uint64_t(int64_t(int64_t(r9) + 8));
  // mulli r11,r10,24
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(24))));
  // ld r8,29392(r8)
  r8 = guest::LoadU64(base, uint32_t(r8) + 29392);
  // stwx r26,r11,r9
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r9), uint32_t(r26));
  // stdx r8,r11,r25
  guest::StoreU64(base, uint32_t(r11) + uint32_t(r25), r8);
  // stbx r29,r11,r24
  guest::StoreU8(base, uint32_t(r11) + uint32_t(r24), uint8_t(r29));
loc_82148EF8:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // clrlwi r7,r10,16
  r7 = uint32_t(r10) & 0xFFFF;
loc_82148F00:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r8,r29
  r8 = r29;
  // addi r10,r11,30904
  r10 = uint64_t(int64_t(int64_t(r11) + 30904));
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r11,r11,32778
  r11 = r11 | 32778;
  // stw r11,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r11));
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // stw r6,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r6));
  // ori r11,r11,32779
  r11 = r11 | 32779;
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,29304(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29304);
  // stw r11,12(r10)
  guest::StoreU32(base, uint32_t(r10) + 12, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,29308(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 29308);
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // beq cr6,0x82148f50
  if (cr6.eq) goto loc_82148F50;
  // stw r8,16(r10)
  guest::StoreU32(base, uint32_t(r10) + 16, uint32_t(r8));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // stw r11,20(r10)
  guest::StoreU32(base, uint32_t(r10) + 20, uint32_t(r11));
loc_82148F50:
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // stw r3,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r3));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r4,84(r28)
  r4 = guest::LoadU32(base, uint32_t(r28) + 84);
  // li r5,10
  r5 = uint64_t(int64_t(10));
  // stw r30,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r30));
  // mr r3,r27
  r3 = r27;
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x825765f0
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionSearchEx(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82149178: lifted from generated/ by lift.py.
lifted::Result f82149178(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82149178 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r24,r3
  r24 = r3;
  // addi r25,r11,24704
  r25 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r25
  r3 = r25;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x821491A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r26,-32148
  r26 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r29,r11,26256
  r29 = uint64_t(int64_t(int64_t(r11) + 26256));
  // lwz r3,27344(r26)
  r3 = guest::LoadU32(base, uint32_t(r26) + 27344);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x821491e4
  if (cr6.eq) goto loc_821491E4;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x821491BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r30,26
  r30 = uint64_t(int64_t(26));
  // addi r31,r29,36
  r31 = uint64_t(int64_t(int64_t(r29) + 36));
loc_821491C8:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplwi cr6,r28,23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 23, so);
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x821491D4, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // addi r31,r31,40
  r31 = uint64_t(int64_t(int64_t(r31) + 40));
  // bne 0x821491c8
  if (!cr0.eq) goto loc_821491C8;
loc_821491E4:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r31,r11,27340
  r31 = uint64_t(int64_t(int64_t(r11) + 27340));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r30,r11,-29024
  r30 = uint64_t(int64_t(int64_t(r11) + -29024));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // mr r7,r31
  r7 = r31;
  // mr r6,r30
  r6 = r30;
  // li r5,26
  r5 = uint64_t(int64_t(26));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82576e78
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserReadProfileSettings(base, LIFTED_ARGS(0x8214921C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x82149224, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r5,0(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 0);
  // stw r3,27344(r26)
  guest::StoreU32(base, uint32_t(r26) + 27344, uint32_t(r3));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82149234, r31)); r3 = o.r3; f1 = o.f1; }
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // li r28,26
  r28 = uint64_t(int64_t(26));
  // addi r29,r29,32
  r29 = uint64_t(int64_t(int64_t(r29) + 32));
loc_82149240:
  // cmplwi cr6,r27,23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 23, so);
  // blt cr6,0x8214926c
  if (cr6.lt) goto loc_8214926C;
  // li r3,1000
  r3 = uint64_t(int64_t(1000));
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x82149250, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1000
  r11 = uint64_t(int64_t(1000));
  // li r5,1000
  r5 = uint64_t(int64_t(1000));
  // stw r3,4(r29)
  guest::StoreU32(base, uint32_t(r29) + 4, uint32_t(r3));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82149268, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82149280
  goto loc_82149280;
loc_8214926C:
  // li r3,512
  r3 = uint64_t(int64_t(512));
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x82149274, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,256
  r11 = uint64_t(int64_t(256));
  // stw r3,4(r29)
  guest::StoreU32(base, uint32_t(r29) + 4, uint32_t(r3));
  // stw r11,0(r29)
  guest::StoreU32(base, uint32_t(r29) + 0, uint32_t(r11));
loc_82149280:
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // addi r29,r29,40
  r29 = uint64_t(int64_t(int64_t(r29) + 40));
  // bne 0x82149240
  if (!cr0.eq) goto loc_82149240;
  // mr r9,r25
  r9 = r25;
  // lwz r8,27344(r26)
  r8 = guest::LoadU32(base, uint32_t(r26) + 27344);
  // mr r7,r31
  r7 = r31;
  // mr r6,r30
  r6 = r30;
  // li r5,26
  r5 = uint64_t(int64_t(26));
  // mr r4,r24
  r4 = r24;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82576e78
  // (left out: dead in C++)
  { const lifted::Result o = game::XUserReadProfileSettings(base, LIFTED_ARGS(0x821492B0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821495E8: lifted from generated/ by lift.py.
lifted::Result f821495E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821495E8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r3,27348(r29)
  r3 = guest::LoadU32(base, uint32_t(r29) + 27348);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82149618
  if (cr6.eq) goto loc_82149618;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x82149610, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,27348(r29)
  guest::StoreU32(base, uint32_t(r29) + 27348, uint32_t(r11));
loc_82149618:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,27360(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 27360);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r7,r11,25616
  r7 = uint64_t(int64_t(int64_t(r11) + 25616));
  // li r11,18
  r11 = uint64_t(int64_t(18));
  // stw r11,0(r7)
  guest::StoreU32(base, uint32_t(r7) + 0, uint32_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,4(r7)
  guest::StoreU32(base, uint32_t(r7) + 4, uint32_t(r11));
  // beq cr6,0x82149658
  if (cr6.eq) goto loc_82149658;
  // li r11,63
  r11 = uint64_t(int64_t(63));
  // sth r11,8(r7)
  guest::StoreU16(base, uint32_t(r7) + 8, uint16_t(r11));
  // li r11,64
  r11 = uint64_t(int64_t(64));
  // sth r11,10(r7)
  guest::StoreU16(base, uint32_t(r7) + 10, uint16_t(r11));
  // li r11,62
  r11 = uint64_t(int64_t(62));
  // b 0x8214966c
  goto loc_8214966C;
loc_82149658:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // sth r11,8(r7)
  guest::StoreU16(base, uint32_t(r7) + 8, uint16_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // sth r11,10(r7)
  guest::StoreU16(base, uint32_t(r7) + 10, uint16_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
loc_8214966C:
  // sth r11,12(r7)
  guest::StoreU16(base, uint32_t(r7) + 12, uint16_t(r11));
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // addi r9,r1,84
  r9 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,100
  r5 = uint64_t(int64_t(100));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // beq cr6,0x82149698
  if (cr6.eq) goto loc_82149698;
  // mr r4,r31
  r4 = r31;
  // bl 0x822951a8
  // (left out: dead in C++)
  { const lifted::Result o = f822951A8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821496a0
  goto loc_821496A0;
loc_82149698:
  // mr r4,r30
  r4 = r30;
  // bl 0x82295208
  // (left out: dead in C++)
  { const lifted::Result o = f82295208(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_821496A0:
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x821496A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r31,r11,24704
  r31 = uint64_t(int64_t(int64_t(r11) + 24704));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r30,27348(r29)
  guest::StoreU32(base, uint32_t(r29) + 27348, uint32_t(r30));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x821496C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r7,r31
  r7 = r31;
  // lwz r5,80(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 80);
  // mr r4,r30
  r4 = r30;
  // lwz r3,84(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 84);
  // bl 0x82295d30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295D30, LIFTED_ARGS(0x821496E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27364(r10)
  guest::StoreU32(base, uint32_t(r10) + 27364, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821497A8: lifted from generated/ by lift.py.
void f821497A8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821497A8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).container_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x821497b8
  if (!cr6.eq) goto loc_821497B8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_821497B8:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address());
  // lbz r11,25(r10)
  r11 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).is_nil_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x821497d0
  if (cr6.eq) goto loc_821497D0;
  // twi 31,r0,22
  lifted::Trap(22);
  // blr
  return;
loc_821497D0:
  // lwz r11,8(r10)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).right_address());
  // lbz r9,25(r11)
  r9 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).is_nil_address());
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x821497ec
  if (cr6.eq) goto loc_821497EC;
  // lwz r11,4(r10)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address());
  // b 0x8214981c
  goto loc_8214981C;
loc_821497E8:
  // mr r11,r10
  r11 = r10;
loc_821497EC:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address());
  // lbz r9,25(r10)
  r9 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).is_nil_address());
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x821497e8
  if (cr6.eq) goto loc_821497E8;
  // b 0x82149828
  goto loc_82149828;
loc_82149800:
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address());
  // lwz r9,8(r11)
  r9 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address());
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x82149828
  if (!cr6.eq) goto loc_82149828;
  // stw r11,4(r3)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address(), uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
loc_8214981C:
  // lbz r10,25(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 25);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82149800
  if (cr6.eq) goto loc_82149800;
loc_82149828:
  // stw r11,4(r3)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r3)).node_address(), uint32_t(r11));
  // blr
  return;
}

// sub_82149888: lifted from generated/ by lift.py.
void f82149888(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82149888 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,8(r4)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r4)).right_address());
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address());
  // stw r10,8(r4)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r4)).right_address(), uint32_t(r10));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address());
  // lbz r9,25(r10)
  r9 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).is_nil_address());
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x821498a8
  if (!cr6.eq) goto loc_821498A8;
  // stw r4,4(r10)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address(), uint32_t(r4));
loc_821498A8:
  // lwz r10,4(r4)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r4)).parent_address());
  // stw r10,4(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address(), uint32_t(r10));
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r3)).head_address());
  // lwz r9,4(r10)
  r9 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address());
  // cmplw cr6,r4,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r9), so);
  // bne cr6,0x821498c8
  if (!cr6.eq) goto loc_821498C8;
  // stw r11,4(r10)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address(), uint32_t(r11));
  // b 0x821498e4
  goto loc_821498E4;
loc_821498C8:
  // lwz r10,4(r4)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r4)).parent_address());
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).left_address());
  // cmplw cr6,r4,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r9), so);
  // bne cr6,0x821498e0
  if (!cr6.eq) goto loc_821498E0;
  // stw r11,0(r10)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).left_address(), uint32_t(r11));
  // b 0x821498e4
  goto loc_821498E4;
loc_821498E0:
  // stw r11,8(r10)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).right_address(), uint32_t(r11));
loc_821498E4:
  // stw r4,0(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address(), uint32_t(r4));
  // stw r11,4(r4)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r4)).parent_address(), uint32_t(r11));
  // blr
  return;
}

// sub_821498F0: lifted from generated/ by lift.py.
void f821498F0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821498F0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,0(r4)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r4)).left_address());
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address());
  // stw r10,0(r4)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r4)).left_address(), uint32_t(r10));
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address());
  // lbz r10,25(r10)
  r10 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).is_nil_address());
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x82149914
  if (!cr6.eq) goto loc_82149914;
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address());
  // stw r4,4(r10)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address(), uint32_t(r4));
loc_82149914:
  // lwz r10,4(r4)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r4)).parent_address());
  // stw r10,4(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address(), uint32_t(r10));
  // lwz r10,4(r3)
  r10 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r3)).head_address());
  // lwz r9,4(r10)
  r9 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address());
  // cmplw cr6,r4,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r9), so);
  // bne cr6,0x82149934
  if (!cr6.eq) goto loc_82149934;
  // stw r11,4(r10)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address(), uint32_t(r11));
  // b 0x82149950
  goto loc_82149950;
loc_82149934:
  // lwz r10,4(r4)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r4)).parent_address());
  // lwz r9,8(r10)
  r9 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).right_address());
  // cmplw cr6,r4,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r9), so);
  // bne cr6,0x8214994c
  if (!cr6.eq) goto loc_8214994C;
  // stw r11,8(r10)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).right_address(), uint32_t(r11));
  // b 0x82149950
  goto loc_82149950;
loc_8214994C:
  // stw r11,0(r10)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).left_address(), uint32_t(r11));
loc_82149950:
  // stw r4,8(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address(), uint32_t(r4));
  // stw r11,4(r4)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r4)).parent_address(), uint32_t(r11));
  // blr
  return;
}

// sub_82149A30: lifted from generated/ by lift.py.
lifted::Result f82149A30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82149A30 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82149a88
  if (cr6.eq) goto loc_82149A88;
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // twllei r31,0
  if (int32_t(r31) == 0 || uint32_t(r31) < 0u) lifted::Trap(0);
  // divwu r11,r11,r31
  r11 = uint32_t(uint32_t(r31) ? uint32_t(r11) / uint32_t(r31) : 0);
  // cmplwi cr6,r11,32
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 32, so);
  // bge cr6,0x82149a88
  if (!cr6.lt) goto loc_82149A88;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,3276
  r11 = uint64_t(int64_t(int64_t(r11) + 3276));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r10));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82091200
  // (left out: dead in C++)
  { const lifted::Result o = f82091200(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3264
  r11 = uint64_t(int64_t(int64_t(r11) + 3264));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
loc_82149A88:
  // rlwinm r3,r31,5,0,26
  r3 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 5) & 0xFFFFFFE0;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x82149A90, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_82149AA8: lifted from generated/ by lift.py.
lifted::Result f82149AA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82149AA8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // b 0x82149ae0
  goto loc_82149AE0;
loc_82149AC8:
  // mr r3,r30
  r3 = r30;
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r31)).right_address());
  // bl 0x82149aa8
  // (left out: dead in C++)
  { const lifted::Result o = f82149AA8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // lwz r31,0(r31)
  r31 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r31)).left_address());
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x82149AE0, r31)); r3 = o.r3; f1 = o.f1; }
loc_82149AE0:
  // lbz r11,25(r31)
  r11 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r31)).is_nil_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82149ac8
  if (cr6.eq) goto loc_82149AC8;
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

// sub_82149B08: lifted from generated/ by lift.py.
uint64_t f82149B08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82149B08 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // li r10,384
  r10 = uint64_t(int64_t(384));
  // subf r11,r3,r30
  r11 = r30 - r3;
  // mr r31,r3
  r31 = r3;
  // divw r11,r11,r10
  r11 = uint32_t((int32_t(r10) && !(int32_t(r11) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r11) / int32_t(r10) : 0);
  // cmplw cr6,r3,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r30), so);
  // mulli r11,r11,384
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(384))));
  // add r28,r11,r5
  r28 = r11 + r5;
  // beq cr6,0x82149b58
  if (cr6.eq) goto loc_82149B58;
  // subf r29,r3,r5
  r29 = r5 - r3;
loc_82149B3C:
  // add r3,r29,r31
  r3 = r29 + r31;
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // mr r4,r31
  r4 = r31;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = game::memcpy_825A7410(base, LIFTED_ARGS(0x82149B4C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,384
  r31 = uint64_t(int64_t(int64_t(r31) + 384));
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x82149b3c
  if (!cr6.eq) goto loc_82149B3C;
loc_82149B58:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82149C48: lifted from generated/ by lift.py.
lifted::Result f82149C48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82149C48 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r31)).head_address());
  // lwz r4,4(r11)
  r4 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address());
  // bl 0x82149aa8
  // (left out: dead in C++)
  { const lifted::Result o = f82149AA8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r31)).head_address());
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r11,4(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address(), uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r31)).head_address());
  // stw r10,8(r31)
  guest::StoreU32(base, schema::SysLinkIdSet(base, uint32_t(r31)).size_address(), uint32_t(r10));
  // stw r11,0(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address(), uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r31)).head_address());
  // stw r11,8(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address(), uint32_t(r11));
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

// sub_82149D00: lifted from generated/ by lift.py.
lifted::Result f82149D00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82149D00 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x82149a30
  // (left out: dead in C++)
  { const lifted::Result o = f82149A30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82149d28
  if (cr0.eq) goto loc_82149D28;
  // stw r10,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r10));
loc_82149D28:
  // addic. r11,r3,4
  ca = uint32_t(r3) > 4294967291;
  r11 = uint64_t(int64_t(int64_t(r3) + 4));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82149d34
  if (cr0.eq) goto loc_82149D34;
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
loc_82149D34:
  // addic. r11,r3,8
  ca = uint32_t(r3) > 4294967287;
  r11 = uint64_t(int64_t(int64_t(r3) + 8));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82149d40
  if (cr0.eq) goto loc_82149D40;
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
loc_82149D40:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stb r11,24(r3)
  guest::StoreU8(base, uint32_t(r3) + 24, uint8_t(r11));
  // stb r10,25(r3)
  guest::StoreU8(base, uint32_t(r3) + 25, uint8_t(r10));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8214AA18: lifted from generated/ by lift.py.
lifted::Result f8214AA18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214AA18 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // bl 0x82149d00
  // (left out: dead in C++)
  { const lifted::Result o = f82149D00(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r3,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r3));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stb r11,25(r3)
  guest::StoreU8(base, uint32_t(r3) + 25, uint8_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r11,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r11,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r11,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r11));
  // stw r10,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r10));
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

// sub_8214AFE8: lifted from generated/ by lift.py.
lifted::Result f8214AFE8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_8214AFE8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-1392(r1)
  ea = -1392 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stb r11,82(r1)
  guest::StoreU8(base, uint32_t(r1) + 82, uint8_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r30,r11,29448
  r30 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // lwz r10,8(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214b020
  if (!cr6.gt) goto loc_8214B020;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B020:
  // mr r31,r11
  r31 = r11;
loc_8214B024:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x8214b030
  if (!cr6.gt) goto loc_8214B030;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B030:
  // subf r11,r31,r10
  r11 = r10 - r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214b07c
  if (cr0.eq) goto loc_8214B07C;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8214b054
  if (cr6.lt) goto loc_8214B054;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B054:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 8);
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(base, LIFTED_ARGS(0x8214B060, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,8(r30)
  r10 = guest::LoadU32(base, uint32_t(r30) + 8);
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // blt cr6,0x8214b070
  if (cr6.lt) goto loc_8214B070;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B070:
  // lwz r11,4(r30)
  r11 = guest::LoadU32(base, uint32_t(r30) + 4);
  // addi r31,r31,384
  r31 = uint64_t(int64_t(int64_t(r31) + 384));
  // b 0x8214b024
  goto loc_8214B024;
loc_8214B07C:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,-29028(r11)
  r4 = guest::LoadU32(base, uint32_t(r11) + -29028);
  // bl 0x8214ab48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8214AB48(base, LIFTED_ARGS(0x8214B08C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,1392
  r1 = uint64_t(int64_t(int64_t(r1) + 1392));
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

// sub_8214B188: lifted from generated/ by lift.py.
uint64_t f8214B188(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_8214B188 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r5
  r29 = r5;
  // std r6,152(r1)
  guest::StoreU64(base, uint32_t(r1) + 152, r6);
  // mr r30,r3
  r30 = r3;
  // mr r31,r4
  r31 = r4;
  // std r29,144(r1)
  guest::StoreU64(base, uint32_t(r1) + 144, r29);
  // lwz r11,144(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 144);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214b1c0
  if (cr6.eq) goto loc_8214B1C0;
  // lwz r10,152(r1)
  r10 = guest::LoadU32(base, uint32_t(r1) + 152);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // beq cr6,0x8214b1c4
  if (cr6.eq) goto loc_8214B1C4;
loc_8214B1C0:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B1C4:
  // lwz r5,148(r1)
  r5 = guest::LoadU32(base, uint32_t(r1) + 148);
  // lwz r3,156(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 156);
  // subf r11,r5,r3
  r11 = r3 - r5;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214b1f0
  if (cr0.eq) goto loc_8214B1F0;
  // lwz r4,8(r31)
  r4 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r31)).size_address());
  // bl 0x82149b08
  // (left out: dead in C++)
  r3 = f82149B08(base, r1, r13, r3, r4, r5);
  // stw r3,8(r31)
  guest::StoreU32(base, schema::SysLinkIdSet(base, uint32_t(r31)).size_address(), uint32_t(r3));
loc_8214B1F0:
  // mr r3,r30
  r3 = r30;
  // std r29,0(r30)
  guest::StoreU64(base, schema::SysLinkId(base, uint32_t(r30)).id_address(), r29);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_8214B330: lifted from generated/ by lift.py.
lifted::Result f8214B330(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214B330 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r28,-32148
  r28 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,27304(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 27304);
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x8214b398
  if (cr6.eq) goto loc_8214B398;
  // cmpwi cr6,r11,11
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 11, so);
  // beq cr6,0x8214b378
  if (cr6.eq) goto loc_8214B378;
  // cmpwi cr6,r11,4108
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4108, so);
  // bne cr6,0x8214b3b4
  if (!cr6.eq) goto loc_8214B3B4;
  // bl 0x821489d8
  // (left out: dead in C++)
  { const lifted::Result o = f821489D8(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r31
  r11 = r31;
  // stw r11,27300(r10)
  guest::StoreU32(base, uint32_t(r10) + 27300, uint32_t(r11));
  // b 0x8214b3b4
  goto loc_8214B3B4;
loc_8214B378:
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lwz r3,27348(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 27348);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8214b3b4
  if (cr6.eq) goto loc_8214B3B4;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x8214B38C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r31
  r11 = r31;
  // stw r11,27348(r30)
  guest::StoreU32(base, uint32_t(r30) + 27348, uint32_t(r11));
  // b 0x8214b3b4
  goto loc_8214B3B4;
loc_8214B398:
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lwz r3,27312(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 27312);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8214b3b4
  if (cr6.eq) goto loc_8214B3B4;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x8214B3AC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r31
  r11 = r31;
  // stw r11,27312(r30)
  guest::StoreU32(base, uint32_t(r30) + 27312, uint32_t(r11));
loc_8214B3B4:
  // cmpwi cr6,r29,11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 11, so);
  // bgt cr6,0x8214b5e4
  if (cr6.gt) goto loc_8214B5E4;
  // beq cr6,0x8214b5c0
  if (cr6.eq) goto loc_8214B5C0;
  // cmpwi cr6,r29,2
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 2, so);
  // beq cr6,0x8214b5a0
  if (cr6.eq) goto loc_8214B5A0;
  // cmpwi cr6,r29,4
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4, so);
  // beq cr6,0x8214b554
  if (cr6.eq) goto loc_8214B554;
  // cmpwi cr6,r29,5
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 5, so);
  // beq cr6,0x8214b488
  if (cr6.eq) goto loc_8214B488;
  // cmpwi cr6,r29,7
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 7, so);
  // beq cr6,0x8214b454
  if (cr6.eq) goto loc_8214B454;
  // cmpwi cr6,r29,8
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 8, so);
  // beq cr6,0x8214b44c
  if (cr6.eq) goto loc_8214B44C;
  // cmpwi cr6,r29,9
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 9, so);
  // beq cr6,0x8214b410
  if (cr6.eq) goto loc_8214B410;
  // cmpwi cr6,r29,10
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 10, so);
  // bne cr6,0x8214b860
  if (!cr6.eq) goto loc_8214B860;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,29448
  r3 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r31,27368(r11)
  guest::StoreU32(base, uint32_t(r11) + 27368, uint32_t(r31));
  // bl 0x8214b2c8
  // (left out: dead in C++)
  r3 = f8214B2C8(base, r1, r13, r3);
  // b 0x8214b860
  goto loc_8214B860;
loc_8214B410:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r31
  r11 = r31;
  // stw r11,29304(r10)
  guest::StoreU32(base, uint32_t(r10) + 29304, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r11,29308(r10)
  guest::StoreU32(base, uint32_t(r10) + 29308, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,29316(r10)
  guest::StoreU32(base, uint32_t(r10) + 29316, uint32_t(r11));
  // mr r11,r31
  r11 = r31;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,29312(r10)
  guest::StoreU32(base, uint32_t(r10) + 29312, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // stw r31,88(r11)
  guest::StoreU32(base, uint32_t(r11) + 88, uint32_t(r31));
loc_8214B44C:
  // bl 0x82148d00
  // (left out: dead in C++)
  { const lifted::Result o = f82148D00(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214b860
  goto loc_8214B860;
loc_8214B454:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r31
  r11 = r31;
  // stw r11,29304(r10)
  guest::StoreU32(base, uint32_t(r10) + 29304, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r11,29308(r10)
  guest::StoreU32(base, uint32_t(r10) + 29308, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,29316(r10)
  guest::StoreU32(base, uint32_t(r10) + 29316, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r31
  r11 = r31;
  // stw r11,29312(r10)
  guest::StoreU32(base, uint32_t(r10) + 29312, uint32_t(r11));
  // b 0x8214b590
  goto loc_8214B590;
loc_8214B488:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // addi r3,r11,29464
  r3 = uint64_t(int64_t(int64_t(r11) + 29464));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214B49C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,29448
  r3 = uint64_t(int64_t(int64_t(r11) + 29448));
  // bl 0x8214b2c8
  // (left out: dead in C++)
  r3 = f8214B2C8(base, r1, r13, r3);
  // bl 0x82148b60
  // (left out: dead in C++)
  { const lifted::Result o = f82148B60(base, r1, r13, r8); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r11,30752
  r3 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r5,29416(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 29416);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r11));
  // bl 0x8214ed30
  // (left out: dead in C++)
  { const lifted::Result o = f8214ED30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,27356(r10)
  guest::StoreU32(base, uint32_t(r10) + 27356, uint32_t(r11));
  // mr r11,r31
  r11 = r31;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,27384(r10)
  guest::StoreU32(base, uint32_t(r10) + 27384, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,27388(r10)
  guest::StoreU32(base, uint32_t(r10) + 27388, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,27392(r10)
  guest::StoreU32(base, uint32_t(r10) + 27392, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,27396(r10)
  guest::StoreU32(base, uint32_t(r10) + 27396, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,27400(r10)
  guest::StoreU32(base, uint32_t(r10) + 27400, uint32_t(r11));
  // bl 0x82148a80
  // (left out: dead in C++)
  { const lifted::Result o = f82148A80(base, r1, r13, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32158
  r11 = uint64_t(int64_t(-2107506688));
  // lis r10,-32158
  r10 = uint64_t(int64_t(-2107506688));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // lwz r3,-29860(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -29860);
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,-29856(r10)
  guest::StoreU32(base, uint32_t(r10) + -29856, uint32_t(r11));
  // bl 0x82459c90
  // (left out: dead in C++)
  { const lifted::Result o = game::listen(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, 0x8214B524); r3 = o.r3; f1 = o.f1; }
  // mr r11,r31
  r11 = r31;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,25344(r10)
  guest::StoreU32(base, uint32_t(r10) + 25344, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,24736(r10)
  guest::StoreU32(base, uint32_t(r10) + 24736, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,25008(r10)
  guest::StoreU32(base, uint32_t(r10) + 25008, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,25752(r10)
  guest::StoreU32(base, uint32_t(r10) + 25752, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r31,27368(r11)
  guest::StoreU32(base, uint32_t(r11) + 27368, uint32_t(r31));
  // b 0x8214b860
  goto loc_8214B860;
loc_8214B554:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r31
  r11 = r31;
  // stw r11,29304(r10)
  guest::StoreU32(base, uint32_t(r10) + 29304, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r11,29308(r10)
  guest::StoreU32(base, uint32_t(r10) + 29308, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r31
  r11 = r31;
  // stw r11,29316(r10)
  guest::StoreU32(base, uint32_t(r10) + 29316, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,29312(r10)
  guest::StoreU32(base, uint32_t(r10) + 29312, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r31
  r11 = r31;
  // stw r11,29416(r10)
  guest::StoreU32(base, uint32_t(r10) + 29416, uint32_t(r11));
loc_8214B590:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // stw r31,88(r11)
  guest::StoreU32(base, uint32_t(r11) + 88, uint32_t(r31));
  // b 0x8214b860
  goto loc_8214B860;
loc_8214B5A0:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,28180(r10)
  guest::StoreU32(base, uint32_t(r10) + 28180, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r10,46
  r10 = uint64_t(int64_t(46));
  // addi r11,r11,30752
  r11 = uint64_t(int64_t(int64_t(r11) + 30752));
  // stw r10,80(r11)
  guest::StoreU32(base, uint32_t(r11) + 80, uint32_t(r10));
  // b 0x8214b860
  goto loc_8214B860;
loc_8214B5C0:
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r11,r31
  r11 = r31;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,27360(r10)
  guest::StoreU32(base, uint32_t(r10) + 27360, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r31,27364(r11)
  guest::StoreU32(base, uint32_t(r11) + 27364, uint32_t(r31));
  // bl 0x821495e8
  // (left out: dead in C++)
  { const lifted::Result o = f821495E8(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214b860
  goto loc_8214B860;
loc_8214B5E4:
  // cmpwi cr6,r29,4096
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4096, so);
  // beq cr6,0x8214b854
  if (cr6.eq) goto loc_8214B854;
  // cmpwi cr6,r29,4101
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4101, so);
  // beq cr6,0x8214b7ec
  if (cr6.eq) goto loc_8214B7EC;
  // cmpwi cr6,r29,4102
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4102, so);
  // beq cr6,0x8214b6d0
  if (cr6.eq) goto loc_8214B6D0;
  // cmpwi cr6,r29,4104
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4104, so);
  // beq cr6,0x8214b688
  if (cr6.eq) goto loc_8214B688;
  // cmpwi cr6,r29,4106
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4106, so);
  // beq cr6,0x8214b680
  if (cr6.eq) goto loc_8214B680;
  // cmpwi cr6,r29,4108
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4108, so);
  // beq cr6,0x8214b644
  if (cr6.eq) goto loc_8214B644;
  // cmpwi cr6,r29,4110
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4110, so);
  // bne cr6,0x8214b860
  if (!cr6.eq) goto loc_8214B860;
  // mr r11,r31
  r11 = r31;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,29400(r10)
  guest::StoreU32(base, uint32_t(r10) + 29400, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,29404(r10)
  guest::StoreU32(base, uint32_t(r10) + 29404, uint32_t(r11));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,29408(r10)
  guest::StoreU32(base, uint32_t(r10) + 29408, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r31,29412(r11)
  guest::StoreU32(base, uint32_t(r11) + 29412, uint32_t(r31));
  // b 0x8214b860
  goto loc_8214B860;
loc_8214B644:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r5,384
  r5 = uint64_t(int64_t(384));
  // addi r3,r11,29464
  r3 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r31,27356(r11)
  guest::StoreU32(base, uint32_t(r11) + 27356, uint32_t(r31));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214B660, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,29448
  r3 = uint64_t(int64_t(int64_t(r11) + 29448));
  // bl 0x8214b2c8
  // (left out: dead in C++)
  r3 = f8214B2C8(base, r1, r13, r3);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,29856
  r3 = uint64_t(int64_t(int64_t(r11) + 29856));
  // bl 0x8214f790
  // (left out: dead in C++)
  { const lifted::Result o = f8214F790(base, r1, r13, r3, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // b 0x8214b7d8
  goto loc_8214B7D8;
loc_8214B680:
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // b 0x8214b7d8
  goto loc_8214B7D8;
loc_8214B688:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,28180(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 28180);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // beq cr6,0x8214b680
  if (cr6.eq) goto loc_8214B680;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,24704
  r3 = uint64_t(int64_t(int64_t(r11) + 24704));
  // mr r11,r31
  r11 = r31;
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stb r11,27410(r10)
  guest::StoreU8(base, uint32_t(r10) + 27410, uint8_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214B6B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r31
  r11 = r31;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,27372(r10)
  guest::StoreU32(base, uint32_t(r10) + 27372, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r31,27308(r11)
  guest::StoreU32(base, uint32_t(r11) + 27308, uint32_t(r31));
  // b 0x8214b860
  goto loc_8214B860;
loc_8214B6D0:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r6,-32148
  r6 = uint64_t(int64_t(-2106851328));
  // addi r8,r11,29464
  r8 = uint64_t(int64_t(int64_t(r11) + 29464));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r6,25292
  r3 = uint64_t(int64_t(int64_t(r6) + 25292));
  // addi r7,r11,30320
  r7 = uint64_t(int64_t(int64_t(r11) + 30320));
  // lis r5,-32148
  r5 = uint64_t(int64_t(-2106851328));
  // li r6,-2
  r6 = uint64_t(int64_t(-2));
  // std r31,256(r8)
  guest::StoreU64(base, uint32_t(r8) + 256, r31);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // std r31,264(r8)
  guest::StoreU64(base, uint32_t(r8) + 264, r31);
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r9,-32148
  r9 = uint64_t(int64_t(-2106851328));
  // std r31,256(r7)
  guest::StoreU64(base, uint32_t(r7) + 256, r31);
  // addi r11,r11,25268
  r11 = uint64_t(int64_t(int64_t(r11) + 25268));
  // std r31,264(r7)
  guest::StoreU64(base, uint32_t(r7) + 264, r31);
  // addi r10,r10,25604
  r10 = uint64_t(int64_t(int64_t(r10) + 25604));
  // stw r6,26012(r5)
  guest::StoreU32(base, uint32_t(r5) + 26012, uint32_t(r6));
  // addi r9,r9,24996
  r9 = uint64_t(int64_t(int64_t(r9) + 24996));
  // lis r5,-32148
  r5 = uint64_t(int64_t(-2106851328));
  // li r6,-1
  r6 = uint64_t(int64_t(-1));
  // stw r31,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r31));
  // stw r31,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r31));
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r31));
  // stw r31,4(r9)
  guest::StoreU32(base, uint32_t(r9) + 4, uint32_t(r31));
  // stw r31,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r31));
  // stw r31,8(r9)
  guest::StoreU32(base, uint32_t(r9) + 8, uint32_t(r31));
  // stw r31,12(r11)
  guest::StoreU32(base, uint32_t(r11) + 12, uint32_t(r31));
  // stw r6,24732(r5)
  guest::StoreU32(base, uint32_t(r5) + 24732, uint32_t(r6));
  // bl 0x820c96e8
  // (left out: dead in C++)
  { const lifted::Result o = game::CarInput::Clear(base, LIFTED_ARGS(0x8214B754, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // li r11,255
  r11 = uint64_t(int64_t(255));
  // stb r11,27404(r10)
  guest::StoreU8(base, uint32_t(r10) + 27404, uint8_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29448
  r11 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // ble cr6,0x8214b77c
  if (!cr6.gt) goto loc_8214B77C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B77C:
  // mr r8,r9
  r8 = r9;
loc_8214B780:
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // ble cr6,0x8214b78c
  if (!cr6.gt) goto loc_8214B78C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B78C:
  // subf r9,r8,r10
  r9 = r10 - r8;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  // cntlzw r9,r9
  r9 = uint32_t(r9) == 0 ? 32 : __builtin_clz(uint32_t(r9));
  // rlwinm. r9,r9,27,31,31
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x8214b7d4
  if (cr0.eq) goto loc_8214B7D4;
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // blt cr6,0x8214b7b0
  if (cr6.lt) goto loc_8214B7B0;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B7B0:
  // std r31,256(r8)
  guest::StoreU64(base, uint32_t(r8) + 256, r31);
  // std r31,264(r8)
  guest::StoreU64(base, uint32_t(r8) + 264, r31);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // blt cr6,0x8214b7c8
  if (cr6.lt) goto loc_8214B7C8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214B7C8:
  // lwz r9,4(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 4);
  // addi r8,r8,384
  r8 = uint64_t(int64_t(int64_t(r8) + 384));
  // b 0x8214b780
  goto loc_8214B780;
loc_8214B7D4:
  // li r4,6
  r4 = uint64_t(int64_t(6));
loc_8214B7D8:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,30752
  r3 = uint64_t(int64_t(int64_t(r11) + 30752));
loc_8214B7E0:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // bl 0x8214ed30
  // (left out: dead in C++)
  { const lifted::Result o = f8214ED30(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214b860
  goto loc_8214B860;
loc_8214B7EC:
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,30752
  r3 = uint64_t(int64_t(int64_t(r11) + 30752));
  // lwz r11,4(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8214b860
  if (cr6.eq) goto loc_8214B860;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r11,r11,29448
  r11 = uint64_t(int64_t(int64_t(r11) + 29448));
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8214b824
  if (cr6.eq) goto loc_8214B824;
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // li r10,384
  r10 = uint64_t(int64_t(384));
  // divw r31,r11,r10
  r31 = uint32_t((int32_t(r10) && !(int32_t(r11) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r11) / int32_t(r10) : 0);
loc_8214B824:
  // cntlzw r11,r31
  r11 = uint32_t(r31) == 0 ? 32 : __builtin_clz(uint32_t(r31));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214b84c
  if (cr0.eq) goto loc_8214B84C;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // addi r11,r11,-28712
  r11 = uint64_t(int64_t(int64_t(r11) + -28712));
  // stw r11,88(r3)
  guest::StoreU32(base, uint32_t(r3) + 88, uint32_t(r11));
  // li r11,4096
  r11 = uint64_t(int64_t(4096));
  // stw r11,27304(r28)
  guest::StoreU32(base, uint32_t(r28) + 27304, uint32_t(r11));
  // b 0x8214b7e0
  goto loc_8214B7E0;
loc_8214B84C:
  // bl 0x8214afe8
  // (left out: dead in C++)
  { const lifted::Result o = f8214AFE8(base, r1, r13); r3 = o.r3; f1 = o.f1; }
  // b 0x8214b860
  goto loc_8214B860;
loc_8214B854:
  // bl 0x822956f0
  // (left out: dead in C++)
  { const lifted::Result o = game::GetTickCount(base, LIFTED_ARGS(0x8214B858, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r3,29424(r11)
  guest::StoreU32(base, uint32_t(r11) + 29424, uint32_t(r3));
loc_8214B860:
  // stw r29,27304(r28)
  guest::StoreU32(base, uint32_t(r28) + 27304, uint32_t(r29));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214BF20: lifted from generated/ by lift.py.
lifted::Result f8214BF20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214BF20 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // std r5,256(r1)
  guest::StoreU64(base, uint32_t(r1) + 256, r5);
  // mr r25,r3
  r25 = r3;
  // lwz r31,260(r1)
  r31 = guest::LoadU32(base, uint32_t(r1) + 260);
  // mr r27,r4
  r27 = r4;
  // lbz r11,25(r31)
  r11 = guest::LoadU8(base, uint32_t(r31) + 25);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214bf8c
  if (cr6.eq) goto loc_8214BF8C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,3324
  r4 = uint64_t(int64_t(int64_t(r11) + 3324));
  // bl 0x825931d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825931D0(base, LIFTED_ARGS(0x8214BF58, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x820919b8
  // (left out: dead in C++)
  { const lifted::Result o = f820919B8(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x82091200
  // (left out: dead in C++)
  { const lifted::Result o = f82091200(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r11,r11,3300
  r11 = uint64_t(int64_t(int64_t(r11) + 3300));
  // stw r11,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r11));
  // bl 0x820915f8
  // (left out: dead in C++)
  { const lifted::Result o = f820915F8(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82091328
  // (left out: dead in C++)
  { const lifted::Result o = game::Std::string::_Tidy(base, LIFTED_ARGS(0x8214BF8C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214BF8C:
  // addi r3,r1,256
  r3 = uint64_t(int64_t(int64_t(r1) + 256));
  // mr r26,r31
  r26 = r31;
  // bl 0x821497a8
  // (left out: dead in C++)
  f821497A8(base, r3);
  // lwz r11,0(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + 0);
  // lbz r10,25(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 25);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8214bfb0
  if (cr6.eq) goto loc_8214BFB0;
  // lwz r28,8(r26)
  r28 = guest::LoadU32(base, uint32_t(r26) + 8);
  // b 0x8214bfd8
  goto loc_8214BFD8;
loc_8214BFB0:
  // lwz r10,8(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 8);
  // lbz r10,25(r10)
  r10 = guest::LoadU8(base, uint32_t(r10) + 25);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8214bfc8
  if (cr6.eq) goto loc_8214BFC8;
  // mr r28,r11
  r28 = r11;
  // b 0x8214bfd8
  goto loc_8214BFD8;
loc_8214BFC8:
  // lwz r11,260(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 260);
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // lwz r28,8(r11)
  r28 = guest::LoadU32(base, uint32_t(r11) + 8);
  // bne cr6,0x8214c0b4
  if (!cr6.eq) goto loc_8214C0B4;
loc_8214BFD8:
  // lbz r11,25(r28)
  r11 = guest::LoadU8(base, uint32_t(r28) + 25);
  // lwz r31,4(r26)
  r31 = guest::LoadU32(base, uint32_t(r26) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8214bfec
  if (!cr6.eq) goto loc_8214BFEC;
  // stw r31,4(r28)
  guest::StoreU32(base, uint32_t(r28) + 4, uint32_t(r31));
loc_8214BFEC:
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r27)).head_address());
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address());
  // cmplw cr6,r10,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r26), so);
  // bne cr6,0x8214c004
  if (!cr6.eq) goto loc_8214C004;
  // stw r28,4(r11)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address(), uint32_t(r28));
  // b 0x8214c01c
  goto loc_8214C01C;
loc_8214C004:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // bne cr6,0x8214c018
  if (!cr6.eq) goto loc_8214C018;
  // stw r28,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r28));
  // b 0x8214c01c
  goto loc_8214C01C;
loc_8214C018:
  // stw r28,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r28));
loc_8214C01C:
  // lwz r9,4(r27)
  r9 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r27)).head_address());
  // lwz r11,0(r9)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r9)).left_address());
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // bne cr6,0x8214c064
  if (!cr6.eq) goto loc_8214C064;
  // lbz r11,25(r28)
  r11 = guest::LoadU8(base, uint32_t(r28) + 25);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214c040
  if (cr6.eq) goto loc_8214C040;
  // mr r11,r31
  r11 = r31;
  // b 0x8214c060
  goto loc_8214C060;
loc_8214C040:
  // lwz r10,0(r28)
  r10 = guest::LoadU32(base, uint32_t(r28) + 0);
  // mr r11,r28
  r11 = r28;
  // b 0x8214c054
  goto loc_8214C054;
loc_8214C04C:
  // mr r11,r10
  r11 = r10;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
loc_8214C054:
  // lbz r8,25(r10)
  r8 = guest::LoadU8(base, uint32_t(r10) + 25);
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x8214c04c
  if (cr6.eq) goto loc_8214C04C;
loc_8214C060:
  // stw r11,0(r9)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r9)).left_address(), uint32_t(r11));
loc_8214C064:
  // lwz r8,4(r27)
  r8 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r27)).head_address());
  // lwz r11,8(r8)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r8)).right_address());
  // cmplw cr6,r11,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r26), so);
  // bne cr6,0x8214c14c
  if (!cr6.eq) goto loc_8214C14C;
  // lbz r11,25(r28)
  r11 = guest::LoadU8(base, uint32_t(r28) + 25);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214c088
  if (cr6.eq) goto loc_8214C088;
  // mr r11,r31
  r11 = r31;
  // b 0x8214c0ac
  goto loc_8214C0AC;
loc_8214C088:
  // addi r10,r28,8
  r10 = uint64_t(int64_t(int64_t(r28) + 8));
  // mr r11,r28
  r11 = r28;
  // b 0x8214c09c
  goto loc_8214C09C;
loc_8214C094:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // addi r10,r11,8
  r10 = uint64_t(int64_t(int64_t(r11) + 8));
loc_8214C09C:
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 0);
  // lbz r9,25(r9)
  r9 = guest::LoadU8(base, uint32_t(r9) + 25);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8214c094
  if (cr6.eq) goto loc_8214C094;
loc_8214C0AC:
  // stw r11,8(r8)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r8)).right_address(), uint32_t(r11));
  // b 0x8214c14c
  goto loc_8214C14C;
loc_8214C0B4:
  // lwz r10,0(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 0);
  // stw r11,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r11));
  // lwz r10,0(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 0);
  // stw r10,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r10));
  // lwz r10,8(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8214c0d8
  if (!cr6.eq) goto loc_8214C0D8;
  // mr r31,r11
  r31 = r11;
  // b 0x8214c100
  goto loc_8214C100;
loc_8214C0D8:
  // lbz r10,25(r28)
  r10 = guest::LoadU8(base, uint32_t(r28) + 25);
  // lwz r31,4(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 4);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8214c0ec
  if (!cr6.eq) goto loc_8214C0EC;
  // stw r31,4(r28)
  guest::StoreU32(base, uint32_t(r28) + 4, uint32_t(r31));
loc_8214C0EC:
  // stw r28,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r28));
  // lwz r10,8(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 8);
  // stw r10,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r10));
  // lwz r10,8(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 8);
  // stw r11,4(r10)
  guest::StoreU32(base, uint32_t(r10) + 4, uint32_t(r11));
loc_8214C100:
  // lwz r10,4(r27)
  r10 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r27)).head_address());
  // lwz r9,4(r10)
  r9 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address());
  // cmplw cr6,r9,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r26), so);
  // bne cr6,0x8214c118
  if (!cr6.eq) goto loc_8214C118;
  // stw r11,4(r10)
  guest::StoreU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).parent_address(), uint32_t(r11));
  // b 0x8214c134
  goto loc_8214C134;
loc_8214C118:
  // lwz r10,4(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 4);
  // lwz r9,0(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 0);
  // cmplw cr6,r9,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r26), so);
  // bne cr6,0x8214c130
  if (!cr6.eq) goto loc_8214C130;
  // stw r11,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r11));
  // b 0x8214c134
  goto loc_8214C134;
loc_8214C130:
  // stw r11,8(r10)
  guest::StoreU32(base, uint32_t(r10) + 8, uint32_t(r11));
loc_8214C134:
  // lwz r10,4(r26)
  r10 = guest::LoadU32(base, uint32_t(r26) + 4);
  // stw r10,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r10));
  // lbz r9,24(r26)
  r9 = guest::LoadU8(base, uint32_t(r26) + 24);
  // lbz r10,24(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 24);
  // stb r9,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r9));
  // stb r10,24(r26)
  guest::StoreU8(base, uint32_t(r26) + 24, uint8_t(r10));
loc_8214C14C:
  // lbz r11,24(r26)
  r11 = guest::LoadU8(base, uint32_t(r26) + 24);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x8214c2ec
  if (!cr6.eq) goto loc_8214C2EC;
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r27)).head_address());
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address());
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // beq cr6,0x8214c2e8
  if (cr6.eq) goto loc_8214C2E8;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_8214C170:
  // lbz r11,24(r28)
  r11 = guest::LoadU8(base, uint32_t(r28) + 24);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x8214c2e8
  if (!cr6.eq) goto loc_8214C2E8;
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // bne cr6,0x8214c22c
  if (!cr6.eq) goto loc_8214C22C;
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
  // lbz r10,24(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 24);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8214c1b0
  if (!cr6.eq) goto loc_8214C1B0;
  // mr r4,r31
  r4 = r31;
  // stb r30,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r30));
  // mr r3,r27
  r3 = r27;
  // stb r29,24(r31)
  guest::StoreU8(base, uint32_t(r31) + 24, uint8_t(r29));
  // bl 0x82149888
  // (left out: dead in C++)
  f82149888(base, r3, r4);
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
loc_8214C1B0:
  // lbz r10,25(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 25);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8214c280
  if (!cr6.eq) goto loc_8214C280;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lbz r10,24(r10)
  r10 = guest::LoadU8(base, uint32_t(r10) + 24);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8214c1dc
  if (!cr6.eq) goto loc_8214C1DC;
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lbz r10,24(r10)
  r10 = guest::LoadU8(base, uint32_t(r10) + 24);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // beq cr6,0x8214c27c
  if (cr6.eq) goto loc_8214C27C;
loc_8214C1DC:
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lbz r10,24(r10)
  r10 = guest::LoadU8(base, uint32_t(r10) + 24);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8214c208
  if (!cr6.eq) goto loc_8214C208;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // mr r4,r11
  r4 = r11;
  // mr r3,r27
  r3 = r27;
  // stb r30,24(r10)
  guest::StoreU8(base, uint32_t(r10) + 24, uint8_t(r30));
  // stb r29,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r29));
  // bl 0x821498f0
  // (left out: dead in C++)
  f821498F0(base, r3, r4);
  // lwz r11,8(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 8);
loc_8214C208:
  // lbz r10,24(r31)
  r10 = guest::LoadU8(base, uint32_t(r31) + 24);
  // mr r4,r31
  r4 = r31;
  // mr r3,r27
  r3 = r27;
  // stb r10,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r10));
  // stb r30,24(r31)
  guest::StoreU8(base, uint32_t(r31) + 24, uint8_t(r30));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 8);
  // stb r30,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r30));
  // bl 0x82149888
  // (left out: dead in C++)
  f82149888(base, r3, r4);
  // b 0x8214c2e8
  goto loc_8214C2E8;
loc_8214C22C:
  // lbz r10,24(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 24);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8214c250
  if (!cr6.eq) goto loc_8214C250;
  // mr r4,r31
  r4 = r31;
  // stb r30,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r30));
  // mr r3,r27
  r3 = r27;
  // stb r29,24(r31)
  guest::StoreU8(base, uint32_t(r31) + 24, uint8_t(r29));
  // bl 0x821498f0
  // (left out: dead in C++)
  f821498F0(base, r3, r4);
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
loc_8214C250:
  // lbz r10,25(r11)
  r10 = guest::LoadU8(base, uint32_t(r11) + 25);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8214c280
  if (!cr6.eq) goto loc_8214C280;
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 8);
  // lbz r10,24(r10)
  r10 = guest::LoadU8(base, uint32_t(r10) + 24);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8214c29c
  if (!cr6.eq) goto loc_8214C29C;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 0);
  // lbz r10,24(r10)
  r10 = guest::LoadU8(base, uint32_t(r10) + 24);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8214c29c
  if (!cr6.eq) goto loc_8214C29C;
loc_8214C27C:
  // stb r29,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r29));
loc_8214C280:
  // lwz r11,4(r27)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r27)).head_address());
  // mr r28,r31
  r28 = r31;
  // lwz r31,4(r31)
  r31 = guest::LoadU32(base, uint32_t(r31) + 4);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).parent_address());
  // cmplw cr6,r28,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), uint32_t(r11), so);
  // bne cr6,0x8214c170
  if (!cr6.eq) goto loc_8214C170;
  // b 0x8214c2e8
  goto loc_8214C2E8;
loc_8214C29C:
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address());
  // lbz r10,24(r10)
  r10 = guest::LoadU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).color_address());
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // bne cr6,0x8214c2c8
  if (!cr6.eq) goto loc_8214C2C8;
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).right_address());
  // mr r4,r11
  r4 = r11;
  // mr r3,r27
  r3 = r27;
  // stb r30,24(r10)
  guest::StoreU8(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).color_address(), uint8_t(r30));
  // stb r29,24(r11)
  guest::StoreU8(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).color_address(), uint8_t(r29));
  // bl 0x82149888
  // (left out: dead in C++)
  f82149888(base, r3, r4);
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 0);
loc_8214C2C8:
  // lbz r10,24(r31)
  r10 = guest::LoadU8(base, uint32_t(r31) + 24);
  // mr r4,r31
  r4 = r31;
  // mr r3,r27
  r3 = r27;
  // stb r10,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r10));
  // stb r30,24(r31)
  guest::StoreU8(base, uint32_t(r31) + 24, uint8_t(r30));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stb r30,24(r11)
  guest::StoreU8(base, uint32_t(r11) + 24, uint8_t(r30));
  // bl 0x821498f0
  // (left out: dead in C++)
  f821498F0(base, r3, r4);
loc_8214C2E8:
  // stb r30,24(r28)
  guest::StoreU8(base, uint32_t(r28) + 24, uint8_t(r30));
loc_8214C2EC:
  // mr r3,r26
  r3 = r26;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8214C2F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r27)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r27)).size_address());
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8214c308
  if (cr0.eq) goto loc_8214C308;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,8(r27)
  guest::StoreU32(base, schema::SysLinkIdSet(base, uint32_t(r27)).size_address(), uint32_t(r11));
loc_8214C308:
  // ld r11,256(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 256);
  // mr r3,r25
  r3 = r25;
  // std r11,0(r25)
  guest::StoreU64(base, schema::SysLinkId(base, uint32_t(r25)).id_address(), r11);
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214C828: lifted from generated/ by lift.py.
lifted::Result f8214C828(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8214C828 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // std r5,176(r1)
  guest::StoreU64(base, uint32_t(r1) + 176, r5);
  // lwz r9,176(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 176);
  // mr r30,r3
  r30 = r3;
  // std r6,184(r1)
  guest::StoreU64(base, uint32_t(r1) + 184, r6);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r31)).head_address());
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r10)).left_address());
  // beq cr6,0x8214c860
  if (cr6.eq) goto loc_8214C860;
  // cmplw cr6,r9,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r31), so);
  // beq cr6,0x8214c864
  if (cr6.eq) goto loc_8214C864;
loc_8214C860:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214C864:
  // lwz r8,180(r1)
  r8 = guest::LoadU32(base, uint32_t(r1) + 180);
  // lwz r28,188(r1)
  r28 = guest::LoadU32(base, uint32_t(r1) + 188);
  // subf r11,r8,r11
  r11 = r11 - r8;
  // lwz r29,184(r1)
  r29 = guest::LoadU32(base, uint32_t(r1) + 184);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214c8c0
  if (cr0.eq) goto loc_8214C8C0;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x8214c890
  if (cr6.eq) goto loc_8214C890;
  // cmplw cr6,r29,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r31), so);
  // beq cr6,0x8214c894
  if (cr6.eq) goto loc_8214C894;
loc_8214C890:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214C894:
  // subf r11,r28,r10
  r11 = r10 - r28;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214c8c0
  if (cr0.eq) goto loc_8214C8C0;
  // mr r3,r31
  r3 = r31;
  // bl 0x82149c48
  // (left out: dead in C++)
  { const lifted::Result o = f82149C48(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::SysLinkIdSet(base, uint32_t(r31)).head_address());
  // stw r31,0(r30)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r30)).container_address(), uint32_t(r31));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::SysLinkIdSetNode(base, uint32_t(r11)).left_address());
  // stw r11,4(r30)
  guest::StoreU32(base, schema::SysLinkIdSetIter(base, uint32_t(r30)).node_address(), uint32_t(r11));
  // b 0x8214c920
  goto loc_8214C920;
loc_8214C8C0:
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8214c8d0
  if (cr6.eq) goto loc_8214C8D0;
  // cmplw cr6,r9,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r29), so);
  // beq cr6,0x8214c8d4
  if (cr6.eq) goto loc_8214C8D4;
loc_8214C8D0:
  // twi 31,r0,22
  lifted::Trap(22);
loc_8214C8D4:
  // lwz r11,180(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 180);
  // subf r11,r11,r28
  r11 = r28 - r11;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm. r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8214c91c
  if (cr0.eq) goto loc_8214C91C;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // std r5,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r5);
  // bl 0x821497a8
  // (left out: dead in C++)
  f821497A8(base, r3);
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // ld r5,0(r11)
  r5 = guest::LoadU64(base, uint32_t(r11) + 0);
  // bl 0x8214bf20
  // (left out: dead in C++)
  { const lifted::Result o = f8214BF20(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // ld r5,176(r1)
  r5 = guest::LoadU64(base, uint32_t(r1) + 176);
  // lwz r9,176(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 176);
  // b 0x8214c8c0
  goto loc_8214C8C0;
loc_8214C91C:
  // std r5,0(r30)
  guest::StoreU64(base, schema::SysLinkIdSetIter(base, uint32_t(r30)).whole_address(), r5);
loc_8214C920:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8214C930: lifted from generated/ by lift.py.
lifted::Result f8214C930(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8214C930 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r9,r1,88
  r9 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // stw r31,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r31));
  // stw r31,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r31));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // ld r6,0(r10)
  r6 = guest::LoadU64(base, uint32_t(r10) + 0);
  // stw r11,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r11));
  // ld r5,0(r9)
  r5 = guest::LoadU64(base, uint32_t(r9) + 0);
  // bl 0x8214c828
  // (left out: dead in C++)
  { const lifted::Result o = f8214C828(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 4);
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8214C980, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // stw r11,8(r31)
  guest::StoreU32(base, uint32_t(r31) + 8, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_8214E240: lifted from generated/ by lift.py.
lifted::Result f8214E240(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8214E240 (lifted)");
  uint64_t r3 = 0, r12 = 0;
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
  // bl 0x82148920
  // (left out: dead in C++)
  { const lifted::Result o = f82148920(base, r1, r13, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // bl 0x8214f240
  // (left out: dead in C++)
  { const lifted::Result o = f8214F240(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8214E608: lifted from generated/ by lift.py.
uint64_t f8214E608(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8214E608 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r9,46
  r9 = uint64_t(int64_t(46));
  // li r5,60
  r5 = uint64_t(int64_t(60));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r10,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r10));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // stw r11,88(r31)
  guest::StoreU32(base, uint32_t(r31) + 88, uint32_t(r11));
  // stw r11,136(r31)
  guest::StoreU32(base, uint32_t(r31) + 136, uint32_t(r11));
  // stw r11,4(r31)
  guest::StoreU32(base, uint32_t(r31) + 4, uint32_t(r11));
  // stw r11,84(r31)
  guest::StoreU32(base, uint32_t(r31) + 84, uint32_t(r11));
  // stw r9,80(r31)
  guest::StoreU32(base, uint32_t(r31) + 80, uint32_t(r9));
  // stw r11,140(r31)
  guest::StoreU32(base, uint32_t(r31) + 140, uint32_t(r11));
  // stw r10,120(r31)
  guest::StoreU32(base, uint32_t(r31) + 120, uint32_t(r10));
  // stw r10,124(r31)
  guest::StoreU32(base, uint32_t(r31) + 124, uint32_t(r10));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214E660, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_8214E678: lifted from generated/ by lift.py.
lifted::Result f8214E678(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214E678 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8214e6b0
  if (cr6.eq) goto loc_8214E6B0;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82576080
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionDelete(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x8214E6A8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8214E6B0:
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

// sub_8214E6C8: lifted from generated/ by lift.py.
lifted::Result f8214E6C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_8214E6C8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x8214e6f8
  if (!cr6.eq) goto loc_8214E6F8;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,120(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_120_address(), uint32_t(r10));
  // b 0x8214e700
  goto loc_8214E700;
loc_8214E6F8:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r11,120(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_120_address(), uint32_t(r11));
loc_8214E700:
  // stw r10,124(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_124_address(), uint32_t(r10));
  // lwz r10,4(r31)
  r10 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_4_address());
  // stw r11,128(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_128_address(), uint32_t(r11));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,132(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_132_address(), uint32_t(r11));
  // beq cr6,0x8214e724
  if (cr6.eq) goto loc_8214E724;
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_80_address());
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,80(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_80_address(), uint32_t(r11));
loc_8214E724:
  // addi r30,r31,92
  r30 = uint64_t(int64_t(int64_t(r31) + 92));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214E738, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r10,r31
  r10 = r31;
  // mr r9,r30
  r9 = r30;
  // lwz r6,124(r31)
  r6 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_124_address());
  // addi r8,r31,8
  r8 = uint64_t(int64_t(int64_t(r31) + 8));
  // lwz r5,120(r31)
  r5 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_120_address());
  // addi r7,r31,72
  r7 = uint64_t(int64_t(int64_t(r31) + 72));
  // lwz r4,84(r31)
  r4 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_84_address());
  // lwz r3,80(r31)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_80_address());
  // bl 0x82575e50
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionCreate(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_8214EC80: lifted from generated/ by lift.py.
lifted::Result f8214EC80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214EC80 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r30,r31,92
  r30 = uint64_t(int64_t(int64_t(r31) + 92));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // stw r11,136(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_136_address(), uint32_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214ECB8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_0_address());
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // ld r5,72(r31)
  r5 = guest::LoadU64(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_72_address());
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825763d0
  // (left out: dead in C++)
  { const lifted::Result o = f825763D0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,80(r1)
  r3 = guest::LoadU32(base, uint32_t(r1) + 80);
  // bl 0x825a6940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6940(base, LIFTED_ARGS(0x8214ECDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r7,r3
  r7 = r3;
  // mr r8,r30
  r8 = r30;
  // ld r5,72(r31)
  r5 = guest::LoadU64(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_72_address());
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_0_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r7,136(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_136_address(), uint32_t(r7));
  // bl 0x825763d0
  // (left out: dead in C++)
  { const lifted::Result o = f825763D0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,997
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 997, so);
  // beq cr6,0x8214ed0c
  if (cr6.eq) goto loc_8214ED0C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295700
  // (left out: dead in C++)
  r3 = f82295700(base, r1, r13, r3, 0x8214ED0C);
loc_8214ED0C:
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,140(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_140_address(), uint32_t(r11));
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

// sub_8214ED30: lifted from generated/ by lift.py.
lifted::Result f8214ED30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214ED30 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r30,r5
  r30 = r5;
  // lwz r11,140(r31)
  r11 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_140_address());
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x8214ed78
  if (cr6.eq) goto loc_8214ED78;
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // beq cr6,0x8214ed64
  if (cr6.eq) goto loc_8214ED64;
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // bne cr6,0x8214ed90
  if (!cr6.eq) goto loc_8214ED90;
loc_8214ED64:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,128(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_128_address(), uint32_t(r11));
  // stw r11,132(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_132_address(), uint32_t(r11));
  // stw r11,88(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_88_address(), uint32_t(r11));
  // b 0x8214ed90
  goto loc_8214ED90;
loc_8214ED78:
  // lwz r3,136(r31)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_136_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8214ed90
  if (cr0.eq) goto loc_8214ED90;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A71C0(base, LIFTED_ARGS(0x8214ED88, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,136(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_136_address(), uint32_t(r11));
loc_8214ED90:
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // beq cr6,0x8214edc0
  if (cr6.eq) goto loc_8214EDC0;
  // cmpwi cr6,r29,4
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 4, so);
  // beq cr6,0x8214ee50
  if (cr6.eq) goto loc_8214EE50;
  // cmpwi cr6,r29,6
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 6, so);
  // beq cr6,0x8214ee28
  if (cr6.eq) goto loc_8214EE28;
  // cmpwi cr6,r29,8
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 8, so);
  // beq cr6,0x8214ee04
  if (cr6.eq) goto loc_8214EE04;
  // cmpwi cr6,r29,11
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 11, so);
  // beq cr6,0x8214edd8
  if (cr6.eq) goto loc_8214EDD8;
  // cmpwi cr6,r29,12
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 12, so);
  // bne cr6,0x8214edcc
  if (!cr6.eq) goto loc_8214EDCC;
loc_8214EDC0:
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8214e6c8
  // (left out: dead in C++)
  { const lifted::Result o = f8214E6C8(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
loc_8214EDCC:
  // stw r29,140(r31)
  guest::StoreU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_140_address(), uint32_t(r29));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_8214EDD8:
  // addi r30,r31,92
  r30 = uint64_t(int64_t(int64_t(r31) + 92));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214EDEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_0_address());
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8214edcc
  if (cr6.eq) goto loc_8214EDCC;
  // mr r4,r30
  r4 = r30;
  // bl 0x82576080
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionDelete(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x8214edcc
  goto loc_8214EDCC;
loc_8214EE04:
  // addi r30,r31,92
  r30 = uint64_t(int64_t(int64_t(r31) + 92));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214EE18, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_0_address());
  // bl 0x82576540
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionEnd(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x8214edcc
  goto loc_8214EDCC;
loc_8214EE28:
  // addi r30,r31,92
  r30 = uint64_t(int64_t(int64_t(r31) + 92));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x8214EE3C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r30
  r5 = r30;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::SysLiveSession(base, uint32_t(r31)).unknown_0_address());
  // bl 0x825764a8
  // (left out: dead in C++)
  { const lifted::Result o = game::XSessionStart(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // b 0x8214edcc
  goto loc_8214EDCC;
loc_8214EE50:
  // mr r3,r31
  r3 = r31;
  // bl 0x8214ec80
  // (left out: dead in C++)
  { const lifted::Result o = f8214EC80(base, r1, r13, r3, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8214edcc
  goto loc_8214EDCC;
}

// sub_8214F240: lifted from generated/ by lift.py.
lifted::Result f8214F240(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8214F240 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // lwz r3,31052(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 31052);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8214f26c
  if (cr6.eq) goto loc_8214F26C;
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8214f26c
  if (cr6.eq) goto loc_8214F26C;
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x8214F26C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8214F26C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // stw r11,31052(r31)
  guest::StoreU32(base, uint32_t(r31) + 31052, uint32_t(r11));
  // stw r11,31132(r10)
  guest::StoreU32(base, uint32_t(r10) + 31132, uint32_t(r11));
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

// sub_8214F660 game::NetworkManager::Ctor: lifted from generated/ by lift.py.
void f8214F660(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8214F660 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r11));
  // stw r11,20(r3)
  guest::StoreU32(base, uint32_t(r3) + 20, uint32_t(r11));
  // stw r11,24(r3)
  guest::StoreU32(base, uint32_t(r3) + 24, uint32_t(r11));
  // stw r11,28(r3)
  guest::StoreU32(base, uint32_t(r3) + 28, uint32_t(r11));
  // stw r11,32(r3)
  guest::StoreU32(base, uint32_t(r3) + 32, uint32_t(r11));
  // stw r11,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r11));
  // stw r11,8(r3)
  guest::StoreU32(base, uint32_t(r3) + 8, uint32_t(r11));
  // stw r11,12(r3)
  guest::StoreU32(base, uint32_t(r3) + 12, uint32_t(r11));
  // stw r11,16(r3)
  guest::StoreU32(base, uint32_t(r3) + 16, uint32_t(r11));
  // blr
  return;
}

// sub_8214F690: lifted from generated/ by lift.py.
lifted::Result f8214F690(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8214F690 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8214f6c8
  if (cr0.eq) goto loc_8214F6C8;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214F6C0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  guest::StoreU32(base, uint32_t(r31) + 0, uint32_t(r11));
loc_8214F6C8:
  // lwz r3,448(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 448);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x8214F6D0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_8214F790: lifted from generated/ by lift.py.
lifted::Result f8214F790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8214F790 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).voice_engine_address());
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r11)).slot_get_remote_talkers_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214F7C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8214f804
  if (cr6.eq) goto loc_8214F804;
  // addi r30,r1,96
  r30 = uint64_t(int64_t(int64_t(r1) + 96));
loc_8214F7D8:
  // lwz r3,0(r29)
  r3 = guest::LoadU32(base, schema::NetworkManager(base, uint32_t(r29)).voice_engine_address());
  // ld r4,0(r30)
  r4 = guest::LoadU64(base, uint32_t(r30) + 0);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,44(r11)
  r11 = guest::LoadU32(base, schema::VoiceEngine(base, uint32_t(r11)).slot_unregister_remote_talker_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8214F7F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // blt cr6,0x8214f7d8
  if (cr6.lt) goto loc_8214F7D8;
loc_8214F804:
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82150040 game::CameraAnims::SetGlobals: lifted from generated/ by lift.py.
void f82150040(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82150040 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r3,31136(r11)
  guest::StoreU32(base, uint32_t(r11) + 31136, uint32_t(r3));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stfs f0,31144(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 31144, temp.u32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r4,31140(r11)
  guest::StoreU32(base, uint32_t(r11) + 31140, uint32_t(r4));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stfs f1,31148(r11)
  temp.f32 = float(f1);
  guest::StoreU32(base, uint32_t(r11) + 31148, temp.u32);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r5,31152(r11)
  guest::StoreU32(base, uint32_t(r11) + 31152, uint32_t(r5));
  // blr
  return;
}

// sub_82150B80: lifted from generated/ by lift.py.
lifted::Result f82150B80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82150B80 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
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
  // stfd f31,-32(r1)
  lifted::ScalarFloatMode();
  guest::StoreU64(base, uint32_t(r1) + -32, std::bit_cast<uint64_t>(f31));
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,128
  r3 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r5,31136(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 31136);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r4,31152(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 31152);
  // lfs f31,2608(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 2608);
  f31 = double(temp.f32);
  // stfs f31,140(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 140, temp.u32);
  // stfs f31,156(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 156, temp.u32);
  // stfs f31,92(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 92, temp.u32);
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r1) + 108, temp.u32);
  // bl 0x82150960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82150960, LIFTED_ARGS(0x82150BCC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r4,31152(r30)
  r4 = guest::LoadU32(base, uint32_t(r30) + 31152);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r5,31140(r11)
  r5 = guest::LoadU32(base, uint32_t(r11) + 31140);
  // bl 0x82150960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82150960, LIFTED_ARGS(0x82150BE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lfs f10,128(r1)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 128);
  f10 = double(temp.f32);
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lfs f9,144(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 144);
  f9 = double(temp.f32);
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // lfs f8,132(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 132);
  f8 = double(temp.f32);
  // lfs f7,148(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 148);
  f7 = double(temp.f32);
  // lfs f0,31144(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 31144);
  f0 = double(temp.f32);
  // lfs f13,31148(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 31148);
  f13 = double(temp.f32);
  // fdivs f12,f0,f13
  f12 = double(float(f0 / f13));
  // lfs f0,-27984(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + -27984);
  f0 = double(temp.f32);
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // lfs f13,-27988(r9)
  temp.u32 = guest::LoadU32(base, uint32_t(r9) + -27988);
  f13 = double(temp.f32);
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmuls f6,f12,f13
  f6 = double(float(f12 * f13));
  // fmadds f5,f11,f0,f12
  f5 = double(float(std::fma(f11, f0, f12)));
  // fmuls f6,f6,f11
  f6 = double(float(f6 * f11));
  // fmuls f11,f6,f11
  f11 = double(float(f6 * f11));
  // fmuls f6,f5,f12
  f6 = double(float(f5 * f12));
  // fmadds f12,f6,f12,f11
  f12 = double(float(std::fma(f6, f12, f11)));
  // lfs f6,80(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 80);
  f6 = double(temp.f32);
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmuls f12,f12,f6
  f12 = double(float(f12 * f6));
  // fmadds f12,f11,f10,f12
  f12 = double(float(std::fma(f11, f10, f12)));
  // stfs f12,0(r31)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r31) + 0, temp.u32);
  // lfs f11,31148(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 31148);
  f11 = double(temp.f32);
  // lfs f12,31144(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 31144);
  f12 = double(temp.f32);
  // fdivs f12,f12,f11
  f12 = double(float(f12 / f11));
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmuls f10,f12,f13
  f10 = double(float(f12 * f13));
  // fmadds f6,f11,f0,f12
  f6 = double(float(std::fma(f11, f0, f12)));
  // fmuls f10,f10,f11
  f10 = double(float(f10 * f11));
  // fmuls f11,f10,f11
  f11 = double(float(f10 * f11));
  // fmuls f10,f6,f12
  f10 = double(float(f6 * f12));
  // fmadds f12,f10,f12,f11
  f12 = double(float(std::fma(f10, f12, f11)));
  // lfs f10,96(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 96);
  f10 = double(temp.f32);
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmuls f12,f12,f10
  f12 = double(float(f12 * f10));
  // fmadds f12,f11,f9,f12
  f12 = double(float(std::fma(f11, f9, f12)));
  // stfs f12,16(r31)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r31) + 16, temp.u32);
  // lfs f11,31148(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 31148);
  f11 = double(temp.f32);
  // lfs f12,31144(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 31144);
  f12 = double(temp.f32);
  // fdivs f12,f12,f11
  f12 = double(float(f12 / f11));
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmuls f10,f12,f13
  f10 = double(float(f12 * f13));
  // fmadds f9,f11,f0,f12
  f9 = double(float(std::fma(f11, f0, f12)));
  // fmuls f10,f10,f11
  f10 = double(float(f10 * f11));
  // fmuls f11,f10,f11
  f11 = double(float(f10 * f11));
  // fmuls f10,f9,f12
  f10 = double(float(f9 * f12));
  // fmadds f12,f10,f12,f11
  f12 = double(float(std::fma(f10, f12, f11)));
  // lfs f10,84(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 84);
  f10 = double(temp.f32);
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmuls f12,f12,f10
  f12 = double(float(f12 * f10));
  // fmadds f12,f11,f8,f12
  f12 = double(float(std::fma(f11, f8, f12)));
  // stfs f12,4(r31)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r31) + 4, temp.u32);
  // lfs f11,31148(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 31148);
  f11 = double(temp.f32);
  // lfs f12,31144(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 31144);
  f12 = double(temp.f32);
  // fdivs f12,f12,f11
  f12 = double(float(f12 / f11));
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmuls f10,f12,f13
  f10 = double(float(f12 * f13));
  // fmadds f9,f11,f0,f12
  f9 = double(float(std::fma(f11, f0, f12)));
  // fmuls f10,f10,f11
  f10 = double(float(f10 * f11));
  // fmuls f11,f10,f11
  f11 = double(float(f10 * f11));
  // fmuls f10,f9,f12
  f10 = double(float(f9 * f12));
  // lfs f8,136(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 136);
  f8 = double(temp.f32);
  // lfs f6,112(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 112);
  f6 = double(temp.f32);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lfs f5,116(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 116);
  f5 = double(temp.f32);
  // fmadds f12,f10,f12,f11
  f12 = double(float(std::fma(f10, f12, f11)));
  // lfs f10,100(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 100);
  f10 = double(temp.f32);
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmuls f12,f12,f10
  f12 = double(float(f12 * f10));
  // fmadds f12,f11,f7,f12
  f12 = double(float(std::fma(f11, f7, f12)));
  // stfs f12,20(r31)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r31) + 20, temp.u32);
  // lfs f12,31144(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 31144);
  f12 = double(temp.f32);
  // lfs f11,31148(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 31148);
  f11 = double(temp.f32);
  // fdivs f12,f12,f11
  f12 = double(float(f12 / f11));
  // lfs f7,152(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 152);
  f7 = double(temp.f32);
  // fmuls f9,f12,f13
  f9 = double(float(f12 * f13));
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmadds f10,f11,f0,f12
  f10 = double(float(std::fma(f11, f0, f12)));
  // fmuls f9,f9,f11
  f9 = double(float(f9 * f11));
  // fmuls f10,f10,f12
  f10 = double(float(f10 * f12));
  // fmuls f11,f9,f11
  f11 = double(float(f9 * f11));
  // fmadds f12,f10,f12,f11
  f12 = double(float(std::fma(f10, f12, f11)));
  // lfs f10,88(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 88);
  f10 = double(temp.f32);
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmuls f12,f12,f10
  f12 = double(float(f12 * f10));
  // fmadds f12,f11,f8,f12
  f12 = double(float(std::fma(f11, f8, f12)));
  // stfs f12,8(r31)
  temp.f32 = float(f12);
  guest::StoreU32(base, uint32_t(r31) + 8, temp.u32);
  // lfs f12,31144(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 31144);
  f12 = double(temp.f32);
  // lfs f11,31148(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 31148);
  f11 = double(temp.f32);
  // fdivs f12,f12,f11
  f12 = double(float(f12 / f11));
  // stfs f31,12(r31)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r31) + 12, temp.u32);
  // stfs f31,28(r31)
  temp.f32 = float(f31);
  guest::StoreU32(base, uint32_t(r31) + 28, temp.u32);
  // stfs f6,32(r31)
  temp.f32 = float(f6);
  guest::StoreU32(base, uint32_t(r31) + 32, temp.u32);
  // stfs f5,36(r31)
  temp.f32 = float(f5);
  guest::StoreU32(base, uint32_t(r31) + 36, temp.u32);
  // fmuls f13,f12,f13
  f13 = double(float(f12 * f13));
  // fsubs f11,f31,f12
  f11 = double(float(f31 - f12));
  // fmadds f0,f11,f0,f12
  f0 = double(float(std::fma(f11, f0, f12)));
  // fmuls f13,f13,f11
  f13 = double(float(f13 * f11));
  // fmuls f0,f0,f12
  f0 = double(float(f0 * f12));
  // fmuls f13,f13,f11
  f13 = double(float(f13 * f11));
  // fmadds f0,f0,f12,f13
  f0 = double(float(std::fma(f0, f12, f13)));
  // lfs f12,104(r1)
  temp.u32 = guest::LoadU32(base, uint32_t(r1) + 104);
  f12 = double(temp.f32);
  // fsubs f13,f31,f0
  f13 = double(float(f31 - f0));
  // fmuls f0,f0,f12
  f0 = double(float(f0 * f12));
  // fmadds f0,f13,f7,f0
  f0 = double(float(std::fma(f13, f7, f0)));
  // stfs f0,24(r31)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r31) + 24, temp.u32);
  // lfs f0,31144(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 31144);
  f0 = double(temp.f32);
  // fadds f0,f0,f31
  f0 = double(float(f0 + f31));
  // lfs f13,31148(r10)
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 31148);
  f13 = double(temp.f32);
  // stfs f0,31144(r11)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r11) + 31144, temp.u32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82150db0
  if (!cr6.lt) goto loc_82150DB0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82150DB0:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-32(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + -32)));
  // ld r30,-24(r1)
  r30 = guest::LoadU64(base, uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_821511B8: lifted from generated/ by lift.py.
lifted::Result f821511B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821511B8 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32148
  r31 = uint64_t(int64_t(-2106851328));
  // mr r28,r4
  r28 = r4;
  // mr r30,r3
  r30 = r3;
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,23340(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 23340);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addis r4,r11,6
  r4 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r4,r4,-27104
  r4 = uint64_t(int64_t(int64_t(r4) + -27104));
  // bl 0x820ffd88
  // (left out: dead in C++)
  { const lifted::Result o = game::Stages::Copy(base, LIFTED_ARGS(0x821511EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // ori r10,r11,15088
  r10 = r11 | 15088;
  // lwz r11,23340(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 23340);
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x82151214
  if (!cr6.eq) goto loc_82151214;
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x82151214
  if (cr6.eq) goto loc_82151214;
  // li r29,1
  r29 = uint64_t(int64_t(1));
loc_82151214:
  // lwz r11,10420(r30)
  r11 = guest::LoadU32(base, schema::RaceModeInterface32(base, uint32_t(r30)).course_setup_state_address());
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // bne cr6,0x821512dc
  if (!cr6.eq) goto loc_821512DC;
  // lwz r3,-31504(r27)
  r3 = guest::LoadU32(base, uint32_t(r27) + -31504);
  // bl 0x82211e90
  // (left out: dead in C++)
  r3 = f82211E90(base, r1, r13);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821512dc
  if (cr0.eq) goto loc_821512DC;
  // lis r11,2
  r11 = uint64_t(int64_t(131072));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ori r11,r11,36156
  r11 = r11 | 36156;
  // cmplwi cr6,r28,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 2, so);
  // stwx r10,r30,r11
  guest::StoreU32(base, uint32_t(r30) + uint32_t(r11), uint32_t(r10));
  // blt cr6,0x821512b0
  if (cr6.lt) goto loc_821512B0;
  // cmplwi cr6,r28,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 4, so);
  // blt cr6,0x82151298
  if (cr6.lt) goto loc_82151298;
  // beq cr6,0x8215127c
  if (cr6.eq) goto loc_8215127C;
  // cmplwi cr6,r28,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 7, so);
  // bge cr6,0x821512d4
  if (!cr6.lt) goto loc_821512D4;
  // lwz r11,23340(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 23340);
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r11) - 367332).flags_address());
  // oris r10,r10,1
  r10 = r10 | 65536;
loc_82151274:
  // stw r10,0(r11)
  guest::StoreU32(base, schema::RaceData(base, uint32_t(r11) - 367332).flags_address(), uint32_t(r10));
  // b 0x821512d4
  goto loc_821512D4;
loc_8215127C:
  // bl 0x82260800
  // (left out: dead in C++)
  { const lifted::Result o = f82260800(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,23340(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 23340);
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r11) - 367332).flags_address());
  // ori r10,r10,32768
  r10 = r10 | 32768;
  // b 0x82151274
  goto loc_82151274;
loc_82151298:
  // lwz r11,23340(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 23340);
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r11) - 367332).flags_address());
  // oris r10,r10,1
  r10 = r10 | 65536;
  // b 0x821512c4
  goto loc_821512C4;
loc_821512B0:
  // lwz r11,23340(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 23340);
  // addis r11,r11,6
  r11 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r11,r11,-25884
  r11 = uint64_t(int64_t(int64_t(r11) + -25884));
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r11) - 367332).flags_address());
  // ori r10,r10,32768
  r10 = r10 | 32768;
loc_821512C4:
  // stw r10,0(r11)
  guest::StoreU32(base, schema::RaceData(base, uint32_t(r11) - 367332).flags_address(), uint32_t(r10));
  // cmpwi cr6,r29,1
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 1, so);
  // bne cr6,0x821512d4
  if (!cr6.eq) goto loc_821512D4;
  // bl 0x82260800
  // (left out: dead in C++)
  { const lifted::Result o = f82260800(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_821512D4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x821512e8
  goto loc_821512E8;
loc_821512DC:
  // lwz r3,-31504(r27)
  r3 = guest::LoadU32(base, uint32_t(r27) + -31504);
  // bl 0x82211ce0
  // (left out: dead in C++)
  f82211CE0(base);
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_821512E8:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821512F0: lifted from generated/ by lift.py.
lifted::Result f821512F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_821512F0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // li r11,37
  r11 = uint64_t(int64_t(37));
  // ori r10,r10,41584
  r10 = r10 | 41584;
  // lis r9,1
  r9 = uint64_t(int64_t(65536));
  // lis r8,1
  r8 = uint64_t(int64_t(65536));
  // ori r9,r9,60848
  r9 = r9 | 60848;
  // ori r8,r8,51204
  r8 = r8 | 51204;
  // stw r11,10420(r3)
  guest::StoreU32(base, schema::RaceModeInterface32(base, uint32_t(r3)).course_setup_state_address(), uint32_t(r11));
  // stwx r11,r3,r10
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r10), uint32_t(r11));
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // lis r6,2
  r6 = uint64_t(int64_t(131072));
  // stw r11,20100(r3)
  guest::StoreU32(base, schema::RaceModeInterface32(base, uint32_t(r3)).camera_director_state_address(), uint32_t(r11));
  // ori r10,r10,31972
  r10 = r10 | 31972;
  // stwx r11,r3,r9
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r9), uint32_t(r11));
  // ori r6,r6,16784
  r6 = r6 | 16784;
  // lis r9,2
  r9 = uint64_t(int64_t(131072));
  // stwx r11,r3,r8
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r8), uint32_t(r11));
  // li r7,24
  r7 = uint64_t(int64_t(24));
  // ori r9,r9,7164
  r9 = r9 | 7164;
  // stwx r11,r3,r10
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r10), uint32_t(r11));
  // lis r10,2
  r10 = uint64_t(int64_t(131072));
  // lis r8,2
  r8 = uint64_t(int64_t(131072));
  // stwx r11,r3,r6
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r6), uint32_t(r11));
  // ori r4,r10,36156
  r4 = r10 | 36156;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // ori r8,r8,26444
  r8 = r8 | 26444;
  // stwx r7,r3,r9
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r9), uint32_t(r7));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lis r9,10
  r9 = uint64_t(int64_t(655360));
  // stwx r10,r3,r4
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r4), uint32_t(r10));
  // ori r9,r9,15052
  r9 = r9 | 15052;
  // stwx r5,r3,r8
  guest::StoreU32(base, uint32_t(r3) + uint32_t(r8), uint32_t(r5));
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 23340);
  // stwx r10,r11,r9
  guest::StoreU32(base, uint32_t(r11) + uint32_t(r9), uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31504(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -31504);
  // b 0x82211cf8
  { const lifted::Result o = f82211CF8(base, r1, r13, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82151388: lifted from generated/ by lift.py.
uint64_t f82151388(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82151388 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // ori r11,r11,39652
  r11 = r11 | 39652;
  // mr r31,r3
  r31 = r3;
  // mr r9,r11
  r9 = r11;
  // mr r8,r11
  r8 = r11;
  // lwz r11,23340(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 23340);
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r9,0(r11)
  r9 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r11) - 367332).flags_address());
  // rlwinm r9,r9,0,17,15
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0xFFFFFFFFFFFF7FFF;
  // stw r9,0(r11)
  guest::StoreU32(base, schema::RaceData(base, uint32_t(r11) - 367332).flags_address(), uint32_t(r9));
  // lwz r11,23340(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 23340);
  // add r11,r11,r8
  r11 = r11 + r8;
  // lwz r10,0(r11)
  r10 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r11) - 367332).flags_address());
  // rlwinm r10,r10,0,16,14
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFFFFFFFEFFFF;
  // stw r10,0(r11)
  guest::StoreU32(base, schema::RaceData(base, uint32_t(r11) - 367332).flags_address(), uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31504(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -31504);
  // bl 0x82211f18
  // (left out: dead in C++)
  f82211F18(base);
  // lis r11,2
  r11 = uint64_t(int64_t(131072));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // ori r11,r11,36156
  r11 = r11 | 36156;
  // stwx r10,r31,r11
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r11), uint32_t(r10));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_82151698 game::CourseSetup::Restart: lifted from generated/ by lift.py.
void f82151698(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82151698 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // addi r11,r4,204
  r11 = uint64_t(int64_t(int64_t(r4) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r9,9604(r3)
  guest::StoreU32(base, schema::CourseSetup(base, uint32_t(r3)).state_address(), uint32_t(r9));
  // lwz r10,600(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = guest::LoadU32(base, uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  guest::StoreU32(base, uint32_t(r11) + 600, uint32_t(r10));
  // divwu r5,r6,r5
  r5 = uint32_t(uint32_t(r5) ? uint32_t(r6) / uint32_t(r5) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r10,r5,r6
  r10 = r6 - r5;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  guest::StoreU32(base, uint32_t(r11) + 8, uint32_t(r7));
  // blr
  return;
}

// sub_82151D20 game::RaceMode::Init: lifted from generated/ by lift.py.
lifted::Result f82151D20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82151D20 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,2
  r11 = uint64_t(int64_t(131072));
  // mr r31,r3
  r31 = r3;
  // ori r11,r11,36188
  r11 = r11 | 36188;
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // addi r30,r31,848
  r30 = uint64_t(int64_t(int64_t(r31) + 848));
  // mr r3,r30
  r3 = r30;
  // stwx r10,r31,r11
  guest::StoreU32(base, uint32_t(r31) + uint32_t(r11), uint32_t(r10));
  // bl 0x821517d8
  // (left out: dead in C++)
  { const lifted::Result o = game::CourseSetup::Init(base, LIFTED_ARGS(0x82151D4C, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r29,r31,1
  r29 = uint64_t(int64_t(int64_t(r31) + 65536));
  // addi r29,r29,32012
  r29 = uint64_t(int64_t(int64_t(r29) + 32012));
  // mr r3,r29
  r3 = r29;
  // bl 0x82160ae0
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceFlow::Init(base, LIFTED_ARGS(0x82151D5C, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r28,r31,2
  r28 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r28,r28,-14260
  r28 = uint64_t(int64_t(int64_t(r28) + -14260));
  // mr r3,r28
  r3 = r28;
  // bl 0x82174d28
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceWorld::Init(base, LIFTED_ARGS(0x82151D6C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r27,r31,10512
  r27 = uint64_t(int64_t(int64_t(r31) + 10512));
  // mr r3,r27
  r3 = r27;
  // bl 0x82159950
  // (left out: dead in C++)
  { const lifted::Result o = game::CameraDirector::InitStates(base, LIFTED_ARGS(0x82151D78, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r26,r31,2
  r26 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r26,r26,-23904
  r26 = uint64_t(int64_t(int64_t(r26) + -23904));
  // mr r3,r26
  r3 = r26;
  // bl 0x82169920
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceStates::Init(base, LIFTED_ARGS(0x82151D88, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r25,r31,1
  r25 = uint64_t(int64_t(int64_t(r31) + 65536));
  // addi r25,r25,22400
  r25 = uint64_t(int64_t(int64_t(r25) + 22400));
  // mr r3,r25
  r3 = r25;
  // bl 0x8215d808
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceSceneMachine::Init(base, LIFTED_ARGS(0x82151D98, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r24,r31,2
  r24 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r24,r24,-2408
  r24 = uint64_t(int64_t(int64_t(r24) + -2408));
  // mr r3,r24
  r3 = r24;
  // bl 0x8216e6e8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceHudMachine::Init(base, LIFTED_ARGS(0x82151DA8, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r23,r31,2
  r23 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r23,r23,7212
  r23 = uint64_t(int64_t(int64_t(r23) + 7212));
  // mr r3,r23
  r3 = r23;
  // bl 0x82177378
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceSounds::Init(base, LIFTED_ARGS(0x82151DB8, r31)); r3 = o.r3; f1 = o.f1; }
  // addis r22,r31,2
  r22 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r22,r22,16872
  r22 = uint64_t(int64_t(int64_t(r22) + 16872));
  // mr r3,r22
  r3 = r22;
  // bl 0x8215e6c8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlaysMachine::Init(base, LIFTED_ARGS(0x82151DC8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,36
  r11 = uint64_t(int64_t(int64_t(r31) + 36));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // lwz r10,200(r11)
  r10 = schema::RaceMode(base, uint32_t(r11) - 36).handler_count_bits();
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,200(r11)
  schema::RaceMode(base, uint32_t(r11) - 36).set_handler_count_bits(uint32_t(r9));
  // stwx r30,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r30));
  // lwz r10,200(r11)
  r10 = schema::RaceMode(base, uint32_t(r11) - 36).handler_count_bits();
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,200(r11)
  schema::RaceMode(base, uint32_t(r11) - 36).set_handler_count_bits(uint32_t(r9));
  // stwx r29,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r29));
  // lwz r10,200(r11)
  r10 = schema::RaceMode(base, uint32_t(r11) - 36).handler_count_bits();
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,200(r11)
  schema::RaceMode(base, uint32_t(r11) - 36).set_handler_count_bits(uint32_t(r9));
  // addi r9,r31,10528
  r9 = uint64_t(int64_t(int64_t(r31) + 10528));
  // stwx r28,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r28));
  // bne cr6,0x82151e18
  if (!cr6.eq) goto loc_82151E18;
  // li r9,0
  r9 = uint64_t(int64_t(0));
loc_82151E18:
  // lwz r10,200(r11)
  r10 = schema::RaceMode(base, uint32_t(r11) - 36).handler_count_bits();
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r8,r10,1
  r8 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r8,200(r11)
  schema::RaceMode(base, uint32_t(r11) - 36).set_handler_count_bits(uint32_t(r8));
  // stwx r9,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r9));
  // lwz r10,200(r11)
  r10 = schema::RaceMode(base, uint32_t(r11) - 36).handler_count_bits();
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,200(r11)
  schema::RaceMode(base, uint32_t(r11) - 36).set_handler_count_bits(uint32_t(r9));
  // stwx r26,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r26));
  // lwz r10,200(r11)
  r10 = schema::RaceMode(base, uint32_t(r11) - 36).handler_count_bits();
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,200(r11)
  schema::RaceMode(base, uint32_t(r11) - 36).set_handler_count_bits(uint32_t(r9));
  // stwx r25,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r25));
  // lwz r10,200(r11)
  r10 = schema::RaceMode(base, uint32_t(r11) - 36).handler_count_bits();
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,200(r11)
  schema::RaceMode(base, uint32_t(r11) - 36).set_handler_count_bits(uint32_t(r9));
  // stwx r24,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r24));
  // lwz r10,200(r11)
  r10 = schema::RaceMode(base, uint32_t(r11) - 36).handler_count_bits();
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,200(r11)
  schema::RaceMode(base, uint32_t(r11) - 36).set_handler_count_bits(uint32_t(r9));
  // stwx r23,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r23));
  // lwz r10,200(r11)
  r10 = schema::RaceMode(base, uint32_t(r11) - 36).handler_count_bits();
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,200(r11)
  schema::RaceMode(base, uint32_t(r11) - 36).set_handler_count_bits(uint32_t(r9));
  // stwx r22,r10,r11
  guest::StoreU32(base, uint32_t(r10) + uint32_t(r11), uint32_t(r22));
  // bl 0x822478d0
  // (left out: dead in C++)
  { const lifted::Result o = game::LoadingScreen::Show(base, LIFTED_ARGS(0x82151EA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32221
  r11 = uint64_t(int64_t(-2111635456));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // addi r11,r11,23560
  r11 = uint64_t(int64_t(int64_t(r11) + 23560));
  // addi r10,r31,4
  r10 = uint64_t(int64_t(int64_t(r31) + 4));
  // stw r9,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r9));
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // addic. r11,r10,4
  ca = uint32_t(r10) > 4294967291;
  r11 = uint64_t(int64_t(int64_t(r10) + 4));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82151edc
  if (cr0.eq) goto loc_82151EDC;
  // lis r9,-32255
  r9 = uint64_t(int64_t(-2113863680));
  // ld r8,80(r1)
  r8 = guest::LoadU64(base, uint32_t(r1) + 80);
  // stw r31,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r31));
  // addi r9,r9,4000
  r9 = uint64_t(int64_t(int64_t(r9) + 4000));
  // std r8,8(r11)
  guest::StoreU64(base, uint32_t(r11) + 8, r8);
  // stw r9,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r9));
  // b 0x82151ee0
  goto loc_82151EE0;
loc_82151EDC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82151EE0:
  // stw r11,0(r10)
  schema::RaceMode(base, uint32_t(r10) - 4).set_delegate_ptr_bits(uint32_t(r11));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ec0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82153150 game::CameraDirector::SetState3: lifted from generated/ by lift.py.
void f82153150(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82153150 (lifted)");
  uint64_t r11 = 0;
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,9620(r3)
  guest::StoreU32(base, schema::CameraDirector(base, uint32_t(r3)).state_address(), uint32_t(r11));
  // blr
  return;
}

// sub_821540B0 game::CameraPath::Dtor: lifted from generated/ by lift.py.
lifted::Result f821540B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821540B0 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-27760
  r11 = uint64_t(int64_t(int64_t(r11) + -27760));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  guest::StoreU32(base, schema::CameraPathKey(base, uint32_t(r31)).unknown_0_address(), uint32_t(r11));
  // bl 0x82152028
  // (left out: dead in C++)
  { const lifted::Result o = game::Buffer28::Free(base, LIFTED_ARGS(0x821540DC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821540ec
  if (cr0.eq) goto loc_821540EC;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x821540EC, r31)); r3 = o.r3; f1 = o.f1; }
loc_821540EC:
  // mr r3,r31
  r3 = r31;
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

// sub_82155BF8: lifted from generated/ by lift.py.
void f82155BF8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82155BF8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lwz r10,360(r3)
  r10 = guest::LoadU32(base, schema::Cutscene(base, uint32_t(r3)).shot_address());
  // lha r11,1024(r3)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::Cutscene(base, uint32_t(r3)).shots_address() + 72))));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blelr cr6
  if (!cr6.gt) return;
  // addi r11,r3,952
  r11 = uint64_t(int64_t(int64_t(r3) + 952));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lha r9,68(r11)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::Cutscene(base, uint32_t(r11) - 952).shots_address() + 68))));
  // b 0x82155c20
  goto loc_82155C20;
loc_82155C18:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_82155C20:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x82155c18
  if (!cr0.eq) goto loc_82155C18;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 2))));
  // mulli r10,r10,608
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(608))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r8,r11,76
  r8 = uint64_t(int64_t(int64_t(r11) + 76));
  // lwz r11,604(r8)
  r11 = guest::LoadU32(base, uint32_t(r8) + 604);
  // rlwinm. r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beqlr
  if (cr0.eq) return;
  // lwz r10,600(r8)
  r10 = guest::LoadU32(base, uint32_t(r8) + 600);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,600(r8)
  guest::StoreU32(base, uint32_t(r8) + 600, uint32_t(r10));
  // lha r10,84(r8)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r8) + 84))));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beqlr
  if (cr0.eq) return;
  // addi r9,r8,12
  r9 = uint64_t(int64_t(int64_t(r8) + 12));
loc_82155C68:
  // lha r10,68(r9)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 68))));
  // mr r7,r11
  r7 = r11;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r9
  r10 = r10 + r9;
  // beq cr6,0x82155c94
  if (cr6.eq) goto loc_82155C94;
loc_82155C80:
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r10) + 0))));
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r9
  r10 = r10 + r9;
  // bne 0x82155c80
  if (!cr0.eq) goto loc_82155C80;
loc_82155C94:
  // lha r10,2(r10)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r10) + 2))));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // rlwinm r10,r10,5,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r7,100(r10)
  r7 = guest::LoadU32(base, uint32_t(r10) + 100);
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // stw r7,100(r10)
  guest::StoreU32(base, uint32_t(r10) + 100, uint32_t(r7));
  // lha r10,84(r8)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r8) + 84))));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x82155c68
  if (cr6.lt) goto loc_82155C68;
  // blr
  return;
}

// sub_82155CC0: lifted from generated/ by lift.py.
void f82155CC0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82155CC0 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lwz r10,360(r3)
  r10 = guest::LoadU32(base, schema::Cutscene(base, uint32_t(r3)).shot_address());
  // lha r11,1024(r3)
  r11 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::Cutscene(base, uint32_t(r3)).shots_address() + 72))));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blelr cr6
  if (!cr6.gt) return;
  // addi r11,r3,952
  r11 = uint64_t(int64_t(int64_t(r3) + 952));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // lha r9,68(r11)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, schema::Cutscene(base, uint32_t(r11) - 952).shots_address() + 68))));
  // b 0x82155ce8
  goto loc_82155CE8;
loc_82155CE0:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_82155CE8:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x82155ce0
  if (!cr0.eq) goto loc_82155CE0;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 2))));
  // mulli r10,r10,608
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(608))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r8,r11,76
  r8 = uint64_t(int64_t(int64_t(r11) + 76));
  // lwz r11,604(r8)
  r11 = guest::LoadU32(base, uint32_t(r8) + 604);
  // rlwinm. r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beqlr
  if (cr0.eq) return;
  // lwz r11,600(r8)
  r11 = guest::LoadU32(base, uint32_t(r8) + 600);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // blelr
  if (!cr0.gt) return;
  // addi r10,r11,-1
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r10,600(r8)
  guest::StoreU32(base, uint32_t(r8) + 600, uint32_t(r10));
  // lha r10,84(r8)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r8) + 84))));
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beqlr
  if (cr0.eq) return;
  // addi r9,r8,12
  r9 = uint64_t(int64_t(int64_t(r8) + 12));
loc_82155D38:
  // lha r10,68(r9)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r9) + 68))));
  // mr r7,r11
  r7 = r11;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r9
  r10 = r10 + r9;
  // beq cr6,0x82155d64
  if (cr6.eq) goto loc_82155D64;
loc_82155D50:
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r10) + 0))));
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r9
  r10 = r10 + r9;
  // bne 0x82155d50
  if (!cr0.eq) goto loc_82155D50;
loc_82155D64:
  // lha r10,2(r10)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r10) + 2))));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // rlwinm r10,r10,5,0,26
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r7,100(r10)
  r7 = guest::LoadU32(base, uint32_t(r10) + 100);
  // addi r7,r7,-1
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  // stw r7,100(r10)
  guest::StoreU32(base, uint32_t(r10) + 100, uint32_t(r7));
  // lha r10,84(r8)
  r10 = uint64_t(int64_t(int16_t(guest::LoadU16(base, uint32_t(r8) + 84))));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x82155d38
  if (cr6.lt) goto loc_82155D38;
  // blr
  return;
}

// sub_82156248 game::Cutscene::OnPad: lifted from generated/ by lift.py.
lifted::Result f82156248(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82156248 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r10,-21596(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r31,r10,r11
  r31 = r10 + r11;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,10756(r30)
  guest::StoreU32(base, schema::Cutscene(base, uint32_t(r30)).unknown_10756_address(), uint32_t(r11));
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821562b4
  if (cr6.eq) goto loc_821562B4;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,164
  r4 = uint64_t(int64_t(164));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821562A0, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821562b4
  if (cr0.eq) goto loc_821562B4;
  // mr r3,r30
  r3 = r30;
  // bl 0x82155bf8
  // (left out: dead in C++)
  f82155BF8(base, r3);
  // stw r29,10756(r30)
  guest::StoreU32(base, schema::Cutscene(base, uint32_t(r30)).unknown_10756_address(), uint32_t(r29));
loc_821562B4:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821562ec
  if (cr6.eq) goto loc_821562EC;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 0);
  // li r4,163
  r4 = uint64_t(int64_t(163));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,48(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821562D8, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821562ec
  if (cr0.eq) goto loc_821562EC;
  // mr r3,r30
  r3 = r30;
  // bl 0x82155cc0
  // (left out: dead in C++)
  f82155CC0(base, r3);
  // stw r29,10756(r30)
  guest::StoreU32(base, schema::Cutscene(base, uint32_t(r30)).unknown_10756_address(), uint32_t(r29));
loc_821562EC:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82159948 game::CameraDirector::UpdateThunk: lifted from generated/ by lift.py.
lifted::Result f82159948(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82159948 (lifted)");
  uint64_t r4 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x821594b8
  { const lifted::Result o = game::CameraDirector::Update(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8215AA88 game::RaceScene::UnloadScenery: lifted from generated/ by lift.py.
lifted::Result f8215AA88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215AA88 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,21788(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 21788);
  // bl 0x820f2b38
  // (left out: dead in C++)
  { const lifted::Result o = game::Scenery::Unload(base, LIFTED_ARGS(0x8215AAA0, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x820f2af8
  // (left out: dead in C++)
  { const lifted::Result o = game::Scenery::Destroy(base, LIFTED_ARGS(0x8215AAA4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,49
  r5 = uint64_t(int64_t(49));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8209fa38
  // (left out: dead in C++)
  { const lifted::Result o = game::RecordClear(base, LIFTED_ARGS(0x8215AABC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8215B720 game::RaceScene::Teardown: lifted from generated/ by lift.py.
lifted::Result f8215B720(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8215B720 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x8215aa88
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceScene::UnloadScenery(base, r1, r13, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x820c4d88
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::StartTeardown(base, LIFTED_ARGS(0x8215B738, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,50
  r30 = uint64_t(int64_t(50));
loc_8215B73C:
  // bl 0x820c9608
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::Update(base, LIFTED_ARGS(0x8215B740, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne 0x8215b73c
  if (!cr0.eq) goto loc_8215B73C;
  // bl 0x82133a70
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffects::DestroyRace(base, LIFTED_ARGS(0x8215B74C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,31152
  r31 = uint64_t(int64_t(int64_t(r31) + 31152));
  // li r30,3
  r30 = uint64_t(int64_t(3));
loc_8215B754:
  // lwz r29,0(r31)
  r29 = schema::RaceScene(base, uint32_t(r31) - 31152).screen_object_bits();
  // cmplwi r29,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq 0x8215b770
  if (cr0.eq) goto loc_8215B770;
  // mr r3,r29
  r3 = r29;
  // bl 0x82132fc8
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenObject::Dtor(base, LIFTED_ARGS(0x8215B768, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A70E8(base, LIFTED_ARGS(0x8215B770, r31)); r3 = o.r3; f1 = o.f1; }
loc_8215B770:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // stw r11,0(r31)
  schema::RaceScene(base, uint32_t(r31) - 31152).set_screen_object_bits(uint32_t(r11));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x8215b754
  if (!cr0.eq) goto loc_8215B754;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8215C248 game::RaceScene::SetUpCars: lifted from generated/ by lift.py.
lifted::Result f8215C248(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8215C248 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // bl 0x820c8a50
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::Create(base, LIFTED_ARGS(0x8215C25C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r28,11844(r11)
  r28 = guest::LoadU32(base, uint32_t(r11) + 11844);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 23340);
  // addi r27,r11,48
  r27 = uint64_t(int64_t(int64_t(r11) + 48));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,0(r30)
  schema::CarAt16_2(base, uint32_t(r30)).set_unknown_0_bits(uint32_t(r11));
  // bl 0x820d0d50
  // (left out: dead in C++)
  { const lifted::Result o = game::SceneSetUp::Ctor(base, LIFTED_ARGS(0x8215C280, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,0(r30)
  r11 = schema::CarAt16_2(base, uint32_t(r30)).unknown_0_bits();
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // std r31,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r31);
  // std r31,8(r10)
  guest::StoreU64(base, uint32_t(r10) + 8, r31);
  // std r31,16(r10)
  guest::StoreU64(base, uint32_t(r10) + 16, r31);
  // std r31,24(r10)
  guest::StoreU64(base, uint32_t(r10) + 24, r31);
  // std r31,32(r10)
  guest::StoreU64(base, uint32_t(r10) + 32, r31);
  // stw r31,40(r10)
  guest::StoreU32(base, uint32_t(r10) + 40, uint32_t(r31));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // stw r11,92(r1)
  guest::StoreU32(base, uint32_t(r1) + 92, uint32_t(r11));
  // stw r11,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r11));
  // stw r10,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // stw r11,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r11));
  // stw r11,108(r1)
  guest::StoreU32(base, uint32_t(r1) + 108, uint32_t(r11));
  // stw r11,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r11));
  // bl 0x820c8de8
  // (left out: dead in C++)
  { const lifted::Result o = game::CarScene::SetUp(base, LIFTED_ARGS(0x8215C2DC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,16
  r11 = uint64_t(int64_t(int64_t(r30) + 16));
  // li r10,7
  r10 = uint64_t(int64_t(7));
loc_8215C2E4:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r31,0(r11)
  schema::CarAt16_2(base, uint32_t(r11) - 16).set_unknown_16_bits(uint32_t(r31));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r9,4272(r11)
  schema::CarAt16_2(base, uint32_t(r11) - 16).set_unknown_4288_bits(uint32_t(r9));
  // addi r11,r11,4288
  r11 = uint64_t(int64_t(int64_t(r11) + 4288));
  // bne 0x8215c2e4
  if (!cr0.eq) goto loc_8215C2E4;
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::RacerCount(base, LIFTED_ARGS(0x8215C300, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x8215c358
  if (!cr0.gt) goto loc_8215C358;
  // li r29,2308
  r29 = uint64_t(int64_t(2308));
  // lis r26,-32146
  r26 = uint64_t(int64_t(-2106720256));
loc_8215C310:
  // lwz r11,-32012(r26)
  r11 = guest::LoadU32(base, uint32_t(r26) + -32012);
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // mr r4,r28
  r4 = r28;
  // mr r3,r30
  r3 = r30;
  // lwzx r11,r29,r11
  r11 = guest::LoadU32(base, uint32_t(r29) + uint32_t(r11));
  // lwz r10,20(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 20);
  // lwz r9,24(r11)
  r9 = guest::LoadU32(base, uint32_t(r11) + 24);
  // mulli r11,r10,4288
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(4288))));
  // mulli r10,r9,3936
  r10 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(3936))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // add r6,r10,r27
  r6 = r10 + r27;
  // addi r5,r11,16
  r5 = uint64_t(int64_t(int64_t(r11) + 16));
  // bl 0x8215c0d8
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceScene::RemakeCarModel(base, LIFTED_ARGS(0x8215C344, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::RacerCount(base, LIFTED_ARGS(0x8215C350, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r31,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r3), so);
  // blt cr6,0x8215c310
  if (cr6.lt) goto loc_8215C310;
loc_8215C358:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x8215C360, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8215C770 game::RaceSceneMachine::OnDraw: lifted from generated/ by lift.py.
lifted::Result f8215C770(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8215C770 (lifted)");
  uint64_t r3 = 0, r11 = 0, r31 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-32044(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -32044);
  // b 0x8215b4f8
  { const lifted::Result o = game::Race::DrawStep(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8215D098 game::RaceSceneMachine::OnStart: lifted from generated/ by lift.py.
lifted::Result f8215D098(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8215D098 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r31,r3
  r31 = r3;
  // lwz r3,-32044(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -32044);
  // bl 0x8215b790
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceScene::LoadScenery(base, LIFTED_ARGS(0x8215D0B8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9608(r31)
  guest::StoreU32(base, schema::RaceSceneMachine(base, uint32_t(r31)).paused_address(), uint32_t(r11));
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

// sub_8215D100 game::RaceScene::SetUp: lifted from generated/ by lift.py.
lifted::Result f8215D100(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8215D100 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // bl 0x8215c248
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceScene::SetUpCars(base, r1, r13, r3, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,30064
  r3 = uint64_t(int64_t(int64_t(r31) + 30064));
  // bl 0x8215cf78
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceLights::SetUp(base, LIFTED_ARGS(0x8215D120, r31)); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // lis r11,10
  r11 = uint64_t(int64_t(655360));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // ori r10,r11,15088
  r10 = r11 | 15088;
  // stw r30,31152(r31)
  schema::RaceScene(base, uint32_t(r31)).set_screen_object_bits(uint32_t(r30));
  // stw r30,31156(r31)
  schema::RaceScene(base, uint32_t(r31)).set_screen_object_first_bits(uint32_t(r30));
  // stw r30,31160(r31)
  schema::RaceScene(base, uint32_t(r31)).set_screen_object_second_bits(uint32_t(r30));
  // lwz r11,23340(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 23340);
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x8215d214
  if (!cr6.eq) goto loc_8215D214;
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x82138a18
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffects::Create(base, LIFTED_ARGS(0x8215D158, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82138a18
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffects::Create(base, LIFTED_ARGS(0x8215D164, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82138a18
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffects::Create(base, LIFTED_ARGS(0x8215D170, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82138a18
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffects::Create(base, LIFTED_ARGS(0x8215D17C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,11
  r4 = uint64_t(int64_t(11));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82138a18
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffects::Create(base, LIFTED_ARGS(0x8215D188, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82138a18
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffects::Create(base, LIFTED_ARGS(0x8215D194, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // ori r3,r3,62464
  r3 = r3 | 62464;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8215D1A0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8215d1cc
  if (cr0.eq) goto loc_8215D1CC;
  // li r9,599
  r9 = uint64_t(int64_t(599));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,288
  r5 = uint64_t(int64_t(288));
  // li r4,1024
  r4 = uint64_t(int64_t(1024));
  // bl 0x82133000
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenObject::Ctor(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // b 0x8215d1d0
  goto loc_8215D1D0;
loc_8215D1CC:
  // mr r11,r30
  r11 = r30;
loc_8215D1D0:
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // stw r11,31156(r31)
  schema::RaceScene(base, uint32_t(r31)).set_screen_object_first_bits(uint32_t(r11));
  // ori r3,r3,62464
  r3 = r3 | 62464;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8215D1E0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8215d208
  if (cr0.eq) goto loc_8215D208;
  // li r9,601
  r9 = uint64_t(int64_t(601));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // li r7,288
  r7 = uint64_t(int64_t(288));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,288
  r5 = uint64_t(int64_t(288));
  // li r4,1024
  r4 = uint64_t(int64_t(1024));
  // bl 0x82133000
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenObject::Ctor(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8215d20c
  goto loc_8215D20C;
loc_8215D208:
  // mr r3,r30
  r3 = r30;
loc_8215D20C:
  // stw r3,31160(r31)
  schema::RaceScene(base, uint32_t(r31)).set_screen_object_second_bits(uint32_t(r3));
  // b 0x8215d270
  goto loc_8215D270;
loc_8215D214:
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x82138a18
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffects::Create(base, LIFTED_ARGS(0x8215D21C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82138a18
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffects::Create(base, LIFTED_ARGS(0x8215D228, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82138a18
  // (left out: dead in C++)
  { const lifted::Result o = game::PostEffects::Create(base, LIFTED_ARGS(0x8215D234, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r3,1
  r3 = uint64_t(int64_t(65536));
  // ori r3,r3,62464
  r3 = r3 | 62464;
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A6880(base, LIFTED_ARGS(0x8215D240, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8215d268
  if (cr0.eq) goto loc_8215D268;
  // li r9,599
  r9 = uint64_t(int64_t(599));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,576
  r5 = uint64_t(int64_t(576));
  // li r4,1024
  r4 = uint64_t(int64_t(1024));
  // bl 0x82133000
  // (left out: dead in C++)
  { const lifted::Result o = game::ScreenObject::Ctor(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x8215d26c
  goto loc_8215D26C;
loc_8215D268:
  // mr r3,r30
  r3 = r30;
loc_8215D26C:
  // stw r3,31152(r31)
  schema::RaceScene(base, uint32_t(r31)).set_screen_object_bits(uint32_t(r3));
loc_8215D270:
  // lwz r11,23340(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + 23340);
  // lwz r10,4(r11)
  r10 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r11)).route_course_address());
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x8215d29c
  if (cr6.eq) goto loc_8215D29C;
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x8215d29c
  if (cr6.eq) goto loc_8215D29C;
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x8215d29c
  if (cr6.eq) goto loc_8215D29C;
  // lwz r4,32(r11)
  r4 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r11)).mode_address());
  // mr r3,r30
  r3 = r30;
  // b 0x8215d2a4
  goto loc_8215D2A4;
loc_8215D29C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // mr r4,r30
  r4 = r30;
loc_8215D2A4:
  // bl 0x8228f5c0
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceScene::SetEffectParams(base, LIFTED_ARGS(0x8215D2A8, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8215D528 game::RaceSceneMachine::OnSetUp: lifted from generated/ by lift.py.
lifted::Result f8215D528(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8215D528 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  double f1 = 0.0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-32044(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -32044);
  // b 0x8215d100
  { const lifted::Result o = game::RaceScene::SetUp(base, r1, r13, r3, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8215D538 game::RaceSceneMachine::OnUpdate: lifted from generated/ by lift.py.
lifted::Result f8215D538(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8215D538 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-32044(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -32044);
  // b 0x8215d2b0
  { const lifted::Result o = game::RaceScene::Update(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8215D7D8 game::RaceSceneMachine::OnEnd: lifted from generated/ by lift.py.
lifted::Result f8215D7D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8215D7D8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r31 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  guest::StoreU32(base, uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-32044(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + -32044);
  // bl 0x8215b720
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceScene::Teardown(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // bl 0x8215d790
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceScene::Destroy(base, LIFTED_ARGS(0x8215D7F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8215DF20 game::RaceOverlays::RecordAll: lifted from generated/ by lift.py.
lifted::Result f8215DF20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215DF20 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // li r30,25
  r30 = uint64_t(int64_t(25));
loc_8215DF3C:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::RaceOverlaysAt9608_2(base, uint32_t(r31)).unknown_0_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8215df5c
  if (cr6.eq) goto loc_8215DF5C;
  // rotlwi r3,r11,0
  r3 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,20(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8215DF5C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8215DF5C:
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x8215df3c
  if (!cr0.eq) goto loc_8215DF3C;
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

// sub_8215DF80 game::RaceOverlays::DeleteAll: lifted from generated/ by lift.py.
lifted::Result f8215DF80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215DF80 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // li r30,25
  r30 = uint64_t(int64_t(25));
loc_8215DF9C:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlays(base, uint32_t(r31)).overlay_0_address());
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8215dfd8
  if (cr0.eq) goto loc_8215DFD8;
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, schema::OverlayVtable(base, uint32_t(r11)).deleting_destructor_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8215DFBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlays(base, uint32_t(r31)).overlay_0_address());
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // bl 0x82295330
  // (left out: dead in C++)
  { schema::CarOverlays(base, uint32_t(r3)).Delete(); r3 = 1; }  // host_free 0x8215DFD0 (CarOverlays)
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  guest::StoreU32(base, schema::RaceOverlays(base, uint32_t(r31)).overlay_0_address(), uint32_t(r11));
loc_8215DFD8:
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x8215df9c
  if (!cr0.eq) goto loc_8215DF9C;
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

// sub_8215E140 game::RaceHudMachine::Restart: lifted from generated/ by lift.py.
void f8215E140(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8215E140 (lifted)");
  uint64_t r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  guest::StoreU32(base, schema::RaceHudMachine(base, uint32_t(r3)).state_address(), uint32_t(r11));
  // blr
  return;
}

// sub_8215E150 game::RaceOverlays::OnUpdate: lifted from generated/ by lift.py.
lifted::Result f8215E150(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8215E150 (lifted)");
  uint64_t r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,9608
  r3 = uint64_t(int64_t(int64_t(r3) + 9608));
  // b 0x8215dea0
  { const lifted::Result o = game::RaceOverlays::Run(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8215E158: lifted from generated/ by lift.py.
lifted::Result f8215E158(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215E158 (lifted)");
  // addi r3,r3,9608
  r3 = uint64_t(int64_t(int64_t(r3) + 9608));
  // b 0x8215df20
  { const lifted::Result o = game::RaceOverlays::RecordAll(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8215E160 game::RaceOverlays::FreeViews: lifted from generated/ by lift.py.
lifted::Result f8215E160(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215E160 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r28,25
  r28 = uint64_t(int64_t(25));
loc_8215E174:
  // lwz r11,0(r31)
  r11 = guest::LoadU32(base, schema::RaceOverlays(base, uint32_t(r31)).overlay_0_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8215e1bc
  if (cr6.eq) goto loc_8215E1BC;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_8215E184:
  // mr r3,r29
  r3 = r29;
  // bl 0x8215e048
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::SlotOf(base, LIFTED_ARGS(0x8215E18C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlays(base, uint32_t(r31)).overlay_0_address());
  // mr r4,r30
  r4 = r30;
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x8215E19C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8215e1b0
  if (cr0.eq) goto loc_8215E1B0;
  // mr r4,r30
  r4 = r30;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlays(base, uint32_t(r31)).overlay_0_address());
  // bl 0x82126570
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::FreeView(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
loc_8215E1B0:
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r29,25
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 25, so);
  // blt cr6,0x8215e184
  if (cr6.lt) goto loc_8215E184;
loc_8215E1BC:
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x8215e174
  if (!cr0.eq) goto loc_8215E174;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8215E1D0 game::RaceOverlays::Destroy: lifted from generated/ by lift.py.
lifted::Result f8215E1D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215E1D0 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // bl 0x8215e160
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::FreeViews(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8215df80
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::DeleteAll(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_8215E208 game::RaceOverlays::Create: lifted from generated/ by lift.py.
lifted::Result f8215E208(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215E208 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // bl 0x8215e160
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::FreeViews(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8215df80
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::DeleteAll(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r11,r30
  r11 = r30;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r10,25
  r10 = uint64_t(int64_t(25));
  // mtctr r10
  ctr = r10;
loc_8215E234:
  // stw r9,0(r11)
  guest::StoreU32(base, schema::RaceOverlaysAt9608_3(base, uint32_t(r11)).unknown_0_address(), uint32_t(r9));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x8215e234
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_8215E234;
  // lis r5,128
  r5 = uint64_t(int64_t(8388608));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // ori r5,r5,4096
  r5 = r5 | 4096;
  // li r4,848
  r4 = uint64_t(int64_t(848));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { r3 = schema::CarOverlays::New(base).address; f1 = 0; }  // host_alloc 0x8215E258 (CarOverlays)
  // stw r3,0(r30)
  guest::StoreU32(base, schema::RaceOverlaysAt9608_3(base, uint32_t(r30)).unknown_0_address(), uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8215e268
  if (cr0.eq) goto loc_8215E268;
  // bl 0x82123350
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::Ctor(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
loc_8215E268:
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // ori r10,r11,38416
  r10 = r11 | 38416;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,23340(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 23340);
  // lwzx r11,r11,r10
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // ble cr6,0x8215e288
  if (!cr6.gt) goto loc_8215E288;
  // li r11,7
  r11 = uint64_t(int64_t(7));
loc_8215E288:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // ble cr6,0x8215e2cc
  if (!cr6.gt) goto loc_8215E2CC;
  // addi r31,r30,4
  r31 = uint64_t(int64_t(int64_t(r30) + 4));
  // addi r29,r11,-1
  r29 = uint64_t(int64_t(int64_t(r11) + -1));
loc_8215E298:
  // lis r5,128
  r5 = uint64_t(int64_t(8388608));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // ori r5,r5,4096
  r5 = r5 | 4096;
  // li r4,848
  r4 = uint64_t(int64_t(848));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82295398
  // (left out: dead in C++)
  { r3 = schema::CarOverlays::New(base).address; f1 = 0; }  // host_alloc 0x8215E2B0 (CarOverlays)
  // stw r3,0(r31)
  guest::StoreU32(base, schema::RaceOverlaysAt9608_3(base, uint32_t(r31) - 4).unknown_4_address(), uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8215e2c0
  if (cr0.eq) goto loc_8215E2C0;
  // bl 0x82123350
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::Ctor(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
loc_8215E2C0:
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x8215e298
  if (!cr0.eq) goto loc_8215E298;
loc_8215E2CC:
  // li r4,71
  r4 = uint64_t(int64_t(71));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_3(base, uint32_t(r30)).unknown_0_address());
  // bl 0x82125888
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::LoadTextures(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8215E2E0 game::RaceOverlays::Bind: lifted from generated/ by lift.py.
lifted::Result f8215E2E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215E2E0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x8215e160
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::FreeViews(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8215e000
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::ShareTextures(base, LIFTED_ARGS(0x8215E2FC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82126860
  // (left out: dead in C++)
  { const lifted::Result o = game::ViewDesc::Clear(base, LIFTED_ARGS(0x8215E304, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82126860
  // (left out: dead in C++)
  { const lifted::Result o = game::ViewDesc::Clear(base, LIFTED_ARGS(0x8215E30C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82126860
  // (left out: dead in C++)
  { const lifted::Result o = game::ViewDesc::Clear(base, LIFTED_ARGS(0x8215E314, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::Get(base, LIFTED_ARGS(0x8215E31C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwz r3,11844(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 11844);
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x8215E334, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r23,-32148
  r23 = uint64_t(int64_t(-2106851328));
  // lwz r9,80(r1)
  r9 = guest::LoadU32(base, uint32_t(r1) + 80);
  // mr r11,r3
  r11 = r3;
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_0_address());
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r10,11496(r23)
  r10 = guest::LoadU32(base, uint32_t(r23) + 11496);
  // stw r11,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r11));
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, schema::Viewports(base, uint32_t(r10)).first_address());
  // stw r9,96(r1)
  guest::StoreU32(base, uint32_t(r1) + 96, uint32_t(r9));
  // stw r11,100(r1)
  guest::StoreU32(base, uint32_t(r1) + 100, uint32_t(r11));
  // stw r10,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r10));
  // stw r10,104(r1)
  guest::StoreU32(base, uint32_t(r1) + 104, uint32_t(r10));
  // bl 0x82126660
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::NewView(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_0_address());
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x8215E374, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82126890
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::Set(base, LIFTED_ARGS(0x8215E37C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r25,-32148
  r25 = uint64_t(int64_t(-2106851328));
  // lis r11,5
  r11 = uint64_t(int64_t(327680));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // ori r26,r11,38416
  r26 = r11 | 38416;
  // li r24,2
  r24 = uint64_t(int64_t(2));
  // lwz r11,23340(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 23340);
  // lwzx r10,r11,r26
  r10 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r26));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // ble cr6,0x8215e510
  if (!cr6.gt) goto loc_8215E510;
  // lwz r11,11496(r23)
  r11 = guest::LoadU32(base, uint32_t(r23) + 11496);
  // lwz r10,8(r11)
  r10 = guest::LoadU32(base, schema::Viewports(base, uint32_t(r11)).split_address());
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8215e3d0
  if (cr6.eq) goto loc_8215E3D0;
  // lwz r11,4(r11)
  r11 = guest::LoadU32(base, schema::Viewports(base, uint32_t(r11)).second_address());
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // lwz r3,11844(r30)
  r3 = guest::LoadU32(base, uint32_t(r30) + 11844);
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
  // bl 0x820c5d70
  // (left out: dead in C++)
  { const lifted::Result o = game::ScenePools::FindModel(base, LIFTED_ARGS(0x8215E3C8, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r3));
  // b 0x8215e3dc
  goto loc_8215E3DC;
loc_8215E3D0:
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // stw r29,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r29));
  // stw r11,88(r1)
  guest::StoreU32(base, uint32_t(r1) + 88, uint32_t(r11));
loc_8215E3DC:
  // lwz r11,4(r31)
  r11 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_4_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8215e46c
  if (cr6.eq) goto loc_8215E46C;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::Get(base, LIFTED_ARGS(0x8215E3F0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_4_address());
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // stw r11,112(r1)
  guest::StoreU32(base, uint32_t(r1) + 112, uint32_t(r11));
  // lwz r11,84(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 84);
  // stw r11,116(r1)
  guest::StoreU32(base, uint32_t(r1) + 116, uint32_t(r11));
  // lwz r11,88(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 88);
  // stw r11,120(r1)
  guest::StoreU32(base, uint32_t(r1) + 120, uint32_t(r11));
  // bl 0x82126660
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::NewView(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_4_address());
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x8215E424, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82126890
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::Set(base, LIFTED_ARGS(0x8215E42C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_0_address());
  // bl 0x82126660
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::NewView(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_0_address());
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x8215E444, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82126890
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::Set(base, LIFTED_ARGS(0x8215E44C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_4_address());
  // bl 0x82126660
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::NewView(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_4_address());
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x8215E464, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82126890
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::Set(base, LIFTED_ARGS(0x8215E46C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8215E46C:
  // mr r28,r24
  r28 = r24;
  // addi r30,r31,8
  r30 = uint64_t(int64_t(int64_t(r31) + 8));
loc_8215E474:
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r30) - 8).unknown_8_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8215e4fc
  if (cr6.eq) goto loc_8215E4FC;
  // mr r3,r28
  r3 = r28;
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::Get(base, LIFTED_ARGS(0x8215E488, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // mr r3,r28
  r3 = r28;
  // stw r29,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r29));
  // bl 0x8215e048
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::SlotOf(base, LIFTED_ARGS(0x8215E498, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r30) - 8).unknown_8_address());
  // mr r4,r27
  r4 = r27;
  // bl 0x82126660
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::NewView(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // mr r4,r27
  r4 = r27;
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r30) - 8).unknown_8_address());
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x8215E4B4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82126890
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::Set(base, LIFTED_ARGS(0x8215E4BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r30) - 8).unknown_8_address());
  // bl 0x82126660
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::NewView(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r30) - 8).unknown_8_address());
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x8215E4D4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x82126890
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::Set(base, LIFTED_ARGS(0x8215E4DC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r30) - 8).unknown_8_address());
  // bl 0x82126660
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::NewView(base, r1, r13, r3, r4, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,0(r30)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r30) - 8).unknown_8_address());
  // bl 0x82126550
  // (left out: dead in C++)
  { const lifted::Result o = game::CarOverlays::View(base, LIFTED_ARGS(0x8215E4F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x82126890
  // (left out: dead in C++)
  { const lifted::Result o = game::OverlayView::Set(base, LIFTED_ARGS(0x8215E4FC, r31)); r3 = o.r3; f1 = o.f1; }
loc_8215E4FC:
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r28,25
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 25, so);
  // blt cr6,0x8215e474
  if (cr6.lt) goto loc_8215E474;
  // lwz r11,23340(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 23340);
loc_8215E510:
  // addi r30,r11,4
  r30 = uint64_t(int64_t(int64_t(r11) + 4));
  // stw r29,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r29));
  // stw r24,132(r1)
  guest::StoreU32(base, uint32_t(r1) + 132, uint32_t(r24));
  // stw r29,140(r1)
  guest::StoreU32(base, uint32_t(r1) + 140, uint32_t(r29));
  // stw r29,136(r1)
  guest::StoreU32(base, uint32_t(r1) + 136, uint32_t(r29));
  // lwz r10,0(r30)
  r10 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r30) - 4).route_course_address());
  // lwz r9,24(r30)
  r9 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r30) - 4).effects_word_address());
  // lwz r8,28(r30)
  r8 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r30) - 4).mode_address());
  // stw r10,144(r1)
  guest::StoreU32(base, uint32_t(r1) + 144, uint32_t(r10));
  // stw r9,148(r1)
  guest::StoreU32(base, uint32_t(r1) + 148, uint32_t(r9));
  // stw r8,152(r1)
  guest::StoreU32(base, uint32_t(r1) + 152, uint32_t(r8));
  // lwzx r11,r11,r26
  r11 = guest::LoadU32(base, uint32_t(r11) + uint32_t(r26));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // ble cr6,0x8215e660
  if (!cr6.gt) goto loc_8215E660;
  // lwz r11,11496(r23)
  r11 = guest::LoadU32(base, uint32_t(r23) + 11496);
  // stw r8,184(r1)
  guest::StoreU32(base, uint32_t(r1) + 184, uint32_t(r8));
  // stw r10,176(r1)
  guest::StoreU32(base, uint32_t(r1) + 176, uint32_t(r10));
  // stw r9,180(r1)
  guest::StoreU32(base, uint32_t(r1) + 180, uint32_t(r9));
  // stw r29,168(r1)
  guest::StoreU32(base, uint32_t(r1) + 168, uint32_t(r29));
  // lwz r11,8(r11)
  r11 = guest::LoadU32(base, schema::Viewports(base, uint32_t(r11)).split_address());
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8215e60c
  if (cr6.eq) goto loc_8215E60C;
  // li r28,3
  r28 = uint64_t(int64_t(3));
  // lwz r3,4(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_4_address());
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r24,128(r1)
  guest::StoreU32(base, uint32_t(r1) + 128, uint32_t(r24));
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // stw r28,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r28));
  // stw r11,172(r1)
  guest::StoreU32(base, uint32_t(r1) + 172, uint32_t(r11));
  // stw r28,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r28));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, schema::OverlayVtablePart(base, uint32_t(r11)).unknown_24_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8215E598, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r30)
  r11 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r30) - 4).mode_address());
  // stw r29,168(r1)
  guest::StoreU32(base, uint32_t(r1) + 168, uint32_t(r29));
  // addi r29,r31,8
  r29 = uint64_t(int64_t(int64_t(r31) + 8));
  // stw r28,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r28));
  // stw r28,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r28));
  // stw r11,184(r1)
  guest::StoreU32(base, uint32_t(r1) + 184, uint32_t(r11));
  // lwz r11,0(r30)
  r11 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r30) - 4).route_course_address());
  // stw r11,176(r1)
  guest::StoreU32(base, uint32_t(r1) + 176, uint32_t(r11));
  // lwz r11,24(r30)
  r11 = guest::LoadU32(base, schema::RaceData(base, uint32_t(r30) - 4).effects_word_address());
  // mr r30,r24
  r30 = r24;
  // stw r11,180(r1)
  guest::StoreU32(base, uint32_t(r1) + 180, uint32_t(r11));
loc_8215E5C4:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r29) - 8).unknown_8_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8215e5f8
  if (cr6.eq) goto loc_8215E5F8;
  // mr r3,r30
  r3 = r30;
  // bl 0x8215e048
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::SlotOf(base, LIFTED_ARGS(0x8215E5D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r29) - 8).unknown_8_address());
  // stw r3,172(r1)
  guest::StoreU32(base, uint32_t(r1) + 172, uint32_t(r3));
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = lifted::VtableWord(base, uint32_t(r11));
  // lwz r11,24(r10)
  r11 = guest::LoadU32(base, schema::OverlayVtable(base, uint32_t(r10)).init_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8215E5F8, r31)); r3 = o.r3; f1 = o.f1; }
loc_8215E5F8:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmpwi cr6,r30,25
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 25, so);
  // blt cr6,0x8215e5c4
  if (cr6.lt) goto loc_8215E5C4;
  // b 0x8215e660
  goto loc_8215E660;
loc_8215E60C:
  // stw r29,160(r1)
  guest::StoreU32(base, uint32_t(r1) + 160, uint32_t(r29));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // stw r24,164(r1)
  guest::StoreU32(base, uint32_t(r1) + 164, uint32_t(r24));
  // addi r29,r31,4
  r29 = uint64_t(int64_t(int64_t(r31) + 4));
loc_8215E61C:
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r29) - 4).unknown_4_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8215e650
  if (cr6.eq) goto loc_8215E650;
  // mr r3,r30
  r3 = r30;
  // bl 0x8215e048
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::SlotOf(base, LIFTED_ARGS(0x8215E630, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r29) - 4).unknown_4_address());
  // stw r3,172(r1)
  guest::StoreU32(base, uint32_t(r1) + 172, uint32_t(r3));
  // addi r4,r1,160
  r4 = uint64_t(int64_t(int64_t(r1) + 160));
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = lifted::VtableWord(base, uint32_t(r11));
  // lwz r11,24(r10)
  r11 = guest::LoadU32(base, schema::OverlayVtablePart(base, uint32_t(r10)).unknown_24_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8215E650, r31)); r3 = o.r3; f1 = o.f1; }
loc_8215E650:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmpwi cr6,r30,25
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 25, so);
  // blt cr6,0x8215e61c
  if (cr6.lt) goto loc_8215E61C;
loc_8215E660:
  // lwz r3,0(r31)
  r3 = guest::LoadU32(base, schema::RaceOverlaysAt9608_4(base, uint32_t(r31)).unknown_0_address());
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r11,0(r3)
  r11 = lifted::VtableWord(base, uint32_t(r3));
  // lwz r11,24(r11)
  r11 = guest::LoadU32(base, schema::CarOverlaysPart(base, uint32_t(r11)).unknown_24_address());
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8215E678, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8215E680: lifted from generated/ by lift.py.
lifted::Result f8215E680(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215E680 (lifted)");
  // addi r3,r3,9608
  r3 = uint64_t(int64_t(int64_t(r3) + 9608));
  // b 0x8215e208
  { const lifted::Result o = game::RaceOverlays::Create(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8215E688: lifted from generated/ by lift.py.
lifted::Result f8215E688(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215E688 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // addi r31,r3,9608
  r31 = uint64_t(int64_t(int64_t(r3) + 9608));
  // mr r3,r31
  r3 = r31;
  // bl 0x8215e160
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::FreeViews(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8215df80
  // (left out: dead in C++)
  { const lifted::Result o = game::RaceOverlays::DeleteAll(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_8215E6C0: lifted from generated/ by lift.py.
lifted::Result f8215E6C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8215E6C0 (lifted)");
  // addi r3,r3,9608
  r3 = uint64_t(int64_t(int64_t(r3) + 9608));
  // b 0x8215e2e0
  { const lifted::Result o = game::RaceOverlays::Bind(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8215F5F0: lifted from generated/ by lift.py.
uint64_t f8215F5F0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_8215F5F0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x820c96e8
  // (left out: dead in C++)
  { const lifted::Result o = game::CarInput::Clear(base, LIFTED_ARGS(0x8215F604, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::RacerCount(base, LIFTED_ARGS(0x8215F60C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x8215f66c
  if (!cr0.gt) goto loc_8215F66C;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // li r31,2308
  r31 = uint64_t(int64_t(2308));
  // addi r30,r11,23344
  r30 = uint64_t(int64_t(int64_t(r11) + 23344));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
loc_8215F624:
  // lwz r11,-32012(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + -32012);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwzx r9,r31,r11
  r9 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r11));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // lwz r9,20(r9)
  r9 = guest::LoadU32(base, uint32_t(r9) + 20);
  // rlwinm r9,r9,2,22,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0x3FC;
  // lwzx r9,r9,r30
  r9 = guest::LoadU32(base, uint32_t(r9) + uint32_t(r30));
  // mtctr r11
  ctr = r11;
loc_8215F644:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r11,0(r9)
  guest::StoreU32(base, uint32_t(r9) + 0, uint32_t(r11));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bdnz 0x8215f644
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_8215F644;
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::RacerCount(base, LIFTED_ARGS(0x8215F664, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r28,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r3), so);
  // blt cr6,0x8215f624
  if (cr6.lt) goto loc_8215F624;
loc_8215F66C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_8215F7B0 game::RaceFlow::OnAbort: lifted from generated/ by lift.py.
uint64_t f8215F7B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8215F7B0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // bl 0x821022b0
  // (left out: dead in C++)
  r3 = game::CarInputs::ClearAll(base, r1, r13);
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,9604(r31)
  guest::StoreU32(base, schema::RaceFlow(base, uint32_t(r31)).state_address(), uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = guest::LoadU64(base, uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_8215FB08 game::RaceFlow::AiUnlessFlagged: lifted from generated/ by lift.py.
lifted::Result f8215FB08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8215FB08 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::RacerCount(base, LIFTED_ARGS(0x8215FB1C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x8215fb80
  if (!cr0.gt) goto loc_8215FB80;
  // li r31,2308
  r31 = uint64_t(int64_t(2308));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
loc_8215FB2C:
  // lwz r11,-32012(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + -32012);
  // lwzx r11,r31,r11
  r11 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r11));
  // lwz r30,20(r11)
  r30 = guest::LoadU32(base, uint32_t(r11) + 20);
  // mr r3,r30
  r3 = r30;
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::Get(base, LIFTED_ARGS(0x8215FB40, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-32012(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + -32012);
  // mr r3,r30
  r3 = r30;
  // lwzx r11,r31,r11
  r11 = guest::LoadU32(base, uint32_t(r31) + uint32_t(r11));
  // lwz r11,0(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 0);
  // rlwinm. r10,r11,0,27,27
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x8215fb64
  if (cr0.eq) goto loc_8215FB64;
  // rlwinm. r11,r11,0,17,17
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // beq 0x8215fb68
  if (cr0.eq) goto loc_8215FB68;
loc_8215FB64:
  // li r4,1
  r4 = uint64_t(int64_t(1));
loc_8215FB68:
  // bl 0x8215f968
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8215F968, LIFTED_ARGS(0x8215FB6C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::RacerCount(base, LIFTED_ARGS(0x8215FB78, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r28,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r3), so);
  // blt cr6,0x8215fb2c
  if (cr6.lt) goto loc_8215FB2C;
loc_8215FB80:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8215FC30 game::RaceFlow::OnState14: lifted from generated/ by lift.py.
lifted::Result f8215FC30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8215FC30 (lifted)");
  uint64_t r3 = 0;
  // b 0x8215fb08
  { const lifted::Result o = game::RaceFlow::AiUnlessFlagged(base, r1, r13, r9, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82160880: lifted from generated/ by lift.py.
lifted::Result f82160880(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82160880 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // mr r28,r3
  r28 = r3;
  // mr r27,r4
  r27 = r4;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::RacerCount(base, LIFTED_ARGS(0x8216089C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x82160930
  if (!cr0.gt) goto loc_82160930;
  // li r30,2308
  r30 = uint64_t(int64_t(2308));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
loc_821608AC:
  // lwz r11,-32012(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + -32012);
  // lwzx r11,r30,r11
  r11 = guest::LoadU32(base, uint32_t(r30) + uint32_t(r11));
  // lwz r31,20(r11)
  r31 = guest::LoadU32(base, uint32_t(r11) + 20);
  // mr r3,r31
  r3 = r31;
  // bl 0x820c0468
  // (left out: dead in C++)
  { const lifted::Result o = game::Vehicle::Get(base, LIFTED_ARGS(0x821608C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-32012(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + -32012);
  // lwzx r10,r30,r11
  r10 = guest::LoadU32(base, uint32_t(r30) + uint32_t(r11));
  // lwz r11,6148(r3)
  r11 = guest::LoadU32(base, schema::Vehicle(base, uint32_t(r3)).car_flags_address());
  // lwz r3,24(r10)
  r3 = guest::LoadU32(base, uint32_t(r10) + 24);
  // clrlwi. r9,r11,31
  r9 = uint32_t(r11) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x821608e0
  if (cr0.eq) goto loc_821608E0;
  // lwz r5,9612(r28)
  r5 = guest::LoadU32(base, schema::RaceFlow(base, uint32_t(r28)).player_a_address());
  // b 0x821608ec
  goto loc_821608EC;
loc_821608E0:
  // rlwinm. r11,r11,25,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 25) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82160900
  if (cr0.eq) goto loc_82160900;
  // lwz r5,9616(r28)
  r5 = guest::LoadU32(base, schema::RaceFlow(base, uint32_t(r28)).player_b_address());
loc_821608EC:
  // mr r6,r27
  r6 = r27;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r4,r31
  r4 = r31;
  // bl 0x821604c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821604C8, LIFTED_ARGS(0x821608FC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x8216091c
  goto loc_8216091C;
loc_82160900:
  // lwz r11,0(r10)
  r11 = guest::LoadU32(base, uint32_t(r10) + 0);
  // mr r3,r31
  r3 = r31;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // rlwinm. r11,r11,0,17,17
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82160918
  if (!cr0.eq) goto loc_82160918;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_82160918:
  // bl 0x8215f968
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8215F968, LIFTED_ARGS(0x8216091C, r31)); r3 = o.r3; f1 = o.f1; }
loc_8216091C:
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::RacerCount(base, LIFTED_ARGS(0x82160928, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r26,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r26), int32_t(r3), so);
  // blt cr6,0x821608ac
  if (cr6.lt) goto loc_821608AC;
loc_82160930:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82163218 game::RaceStates::CarsLoaded: lifted from generated/ by lift.py.
lifted::Result f82163218(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82163218 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::RacerCount(base, LIFTED_ARGS(0x8216322C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ble 0x821632b4
  if (!cr0.gt) goto loc_821632B4;
  // li r30,2308
  r30 = uint64_t(int64_t(2308));
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // lis r28,-32148
  r28 = uint64_t(int64_t(-2106851328));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
loc_82163244:
  // lwz r11,-32012(r29)
  r11 = guest::LoadU32(base, uint32_t(r29) + -32012);
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // lwzx r31,r30,r11
  r31 = guest::LoadU32(base, uint32_t(r30) + uint32_t(r11));
  // lwz r11,23340(r28)
  r11 = guest::LoadU32(base, uint32_t(r28) + 23340);
  // addis r3,r11,6
  r3 = uint64_t(int64_t(int64_t(r11) + 393216));
  // addi r3,r3,-27104
  r3 = uint64_t(int64_t(int64_t(r3) + -27104));
  // lwz r4,24(r31)
  r4 = guest::LoadU32(base, uint32_t(r31) + 24);
  // bl 0x820ffcf8
  // (left out: dead in C++)
  { const lifted::Result o = game::Stages::HasRacer(base, LIFTED_ARGS(0x82163264, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x821632a0
  if (cr0.eq) goto loc_821632A0;
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // mulli r10,r11,4288
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(4288))));
  // lwz r11,-32044(r27)
  r11 = guest::LoadU32(base, uint32_t(r27) + -32044);
  // add r11,r10,r11
  r11 = r10 + r11;
  // lwz r10,4304(r11)
  r10 = guest::LoadU32(base, uint32_t(r11) + 4304);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // beq cr6,0x821632c0
  if (cr6.eq) goto loc_821632C0;
  // lwz r3,32(r11)
  r3 = guest::LoadU32(base, uint32_t(r11) + 32);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x821632c0
  if (cr0.eq) goto loc_821632C0;
  // bl 0x820da318
  // (left out: dead in C++)
  { const lifted::Result o = game::SceneModel::PollLoad(base, LIFTED_ARGS(0x82163298, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821632c0
  if (cr0.eq) goto loc_821632C0;
loc_821632A0:
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // bl 0x821705f8
  // (left out: dead in C++)
  { const lifted::Result o = game::Race::RacerCount(base, LIFTED_ARGS(0x821632AC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r26,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r26), int32_t(r3), so);
  // blt cr6,0x82163244
  if (cr6.lt) goto loc_82163244;
loc_821632B4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821632B8:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_821632C0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x821632b8
  goto loc_821632B8;
}

// sub_82163328 game::RaceStates::ShowLoading: lifted from generated/ by lift.py.
lifted::Result f82163328(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82163328 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r11,r3
  r11 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r5,671
  r5 = uint64_t(int64_t(671));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,9616(r11)
  guest::StoreU32(base, schema::RaceStates(base, uint32_t(r11)).ticks_address(), uint32_t(r10));
  // b 0x822478d0
  { const lifted::Result o = game::LoadingScreen::Show(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82163358 game::RaceStates::ShowLoadingStep0: lifted from generated/ by lift.py.
lifted::Result f82163358(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_82163358 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r11,r3
  r11 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r5,671
  r5 = uint64_t(int64_t(671));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,9612(r11)
  guest::StoreU32(base, schema::RaceStates(base, uint32_t(r11)).step_address(), uint32_t(r10));
  // stw r10,9616(r11)
  guest::StoreU32(base, schema::RaceStates(base, uint32_t(r11)).ticks_address(), uint32_t(r10));
  // b 0x822478d0
  { const lifted::Result o = game::LoadingScreen::Show(base, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_82163388 game::RaceStates::OnQuit2: lifted from generated/ by lift.py.
void f82163388(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82163388 (lifted)");
  uint64_t r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9608(r3)
  guest::StoreU32(base, schema::RaceStates(base, uint32_t(r3)).pause_quit_address(), uint32_t(r11));
  // blr
  return;
}

lifted::Result e82136DC8(uint8_t* base, const lifted::Args& a) {
  return f82136DC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82136F50(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136F50(base, a.r[0])};
}

lifted::Result e82136F58(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136F58(base, a.r[0])};
}

lifted::Result e82136F60(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136F60(base, a.r[0])};
}

lifted::Result e82136F68(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136F68(base, a.r[0])};
}

lifted::Result e82136F70(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136F70(base, a.r[0])};
}

lifted::Result e82136F78(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136F78(base, a.r[0])};
}

lifted::Result e82136F80(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136F80(base, a.r[0])};
}

lifted::Result e82136F88(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136F88(base, a.r[0])};
}

lifted::Result e82136F90(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136F90(base, a.r[0])};
}

lifted::Result e82136F98(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136F98(base, a.r[0])};
}

lifted::Result e82136FA0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f82136FA0(base, a.r[0])};
}

lifted::Result e82138520(uint8_t* base, const lifted::Args& a) {
  return f82138520(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e821392D8(uint8_t* base, const lifted::Args& a) {
  f821392D8(base, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e821398C0(uint8_t* base, const lifted::Args& a) {
  return f821398C0(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e8213A1B8(uint8_t* base, const lifted::Args& a) {
  return f8213A1B8(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8213A200(uint8_t* base, const lifted::Args& a) {
  return f8213A200(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e8213AB40(uint8_t* base, const lifted::Args& a) {
  return f8213AB40(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e8213BDF0(uint8_t* base, const lifted::Args& a) {
  f8213BDF0(base, a.r1, a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8213BE80(uint8_t* base, const lifted::Args& a) {
  return {f8213BE80(base, a.r1, a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e8213C248(uint8_t* base, const lifted::Args& a) {
  return {f8213C248(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8213D898(uint8_t* base, const lifted::Args& a) {
  return {f8213D898(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8213F868(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f8213F868(base, a.r1, a.r[0])};
}

lifted::Result e82141668(uint8_t* base, const lifted::Args& a) {
  return f82141668(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e821462A0(uint8_t* base, const lifted::Args& a) {
  return {f821462A0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82148920(uint8_t* base, const lifted::Args& a) {
  return f82148920(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e821489D8(uint8_t* base, const lifted::Args& a) {
  return f821489D8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82148A80(uint8_t* base, const lifted::Args& a) {
  return f82148A80(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82148B60(uint8_t* base, const lifted::Args& a) {
  return f82148B60(base, a.r1, a.r13, a.r[5]);
}

lifted::Result e82148D00(uint8_t* base, const lifted::Args& a) {
  return f82148D00(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e82149178(uint8_t* base, const lifted::Args& a) {
  return f82149178(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821495E8(uint8_t* base, const lifted::Args& a) {
  return f821495E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821497A8(uint8_t* base, const lifted::Args& a) {
  f821497A8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82149888(uint8_t* base, const lifted::Args& a) {
  f82149888(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e821498F0(uint8_t* base, const lifted::Args& a) {
  f821498F0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82149A30(uint8_t* base, const lifted::Args& a) {
  return f82149A30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82149AA8(uint8_t* base, const lifted::Args& a) {
  return f82149AA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82149B08(uint8_t* base, const lifted::Args& a) {
  return {f82149B08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82149C48(uint8_t* base, const lifted::Args& a) {
  return f82149C48(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82149D00(uint8_t* base, const lifted::Args& a) {
  return f82149D00(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214AA18(uint8_t* base, const lifted::Args& a) {
  return f8214AA18(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214AFE8(uint8_t* base, const lifted::Args& a) {
  return f8214AFE8(base, a.r1, a.r13);
}

lifted::Result e8214B188(uint8_t* base, const lifted::Args& a) {
  return {f8214B188(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e8214B330(uint8_t* base, const lifted::Args& a) {
  return f8214B330(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214BF20(uint8_t* base, const lifted::Args& a) {
  return f8214BF20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214C828(uint8_t* base, const lifted::Args& a) {
  return f8214C828(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e8214C930(uint8_t* base, const lifted::Args& a) {
  return f8214C930(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8214E240(uint8_t* base, const lifted::Args& a) {
  return f8214E240(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8214E608(uint8_t* base, const lifted::Args& a) {
  return {f8214E608(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8214E678(uint8_t* base, const lifted::Args& a) {
  return f8214E678(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214E6C8(uint8_t* base, const lifted::Args& a) {
  return f8214E6C8(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e8214EC80(uint8_t* base, const lifted::Args& a) {
  return f8214EC80(base, a.r1, a.r13, a.r[0], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214ED30(uint8_t* base, const lifted::Args& a) {
  return f8214ED30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214F240(uint8_t* base, const lifted::Args& a) {
  return f8214F240(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8214F660(uint8_t* base, const lifted::Args& a) {
  f8214F660(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8214F690(uint8_t* base, const lifted::Args& a) {
  return f8214F690(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8214F790(uint8_t* base, const lifted::Args& a) {
  return f8214F790(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82150040(uint8_t* base, const lifted::Args& a) {
  f82150040(base, a.r[0], a.r[1], a.r[2], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82150B80(uint8_t* base, const lifted::Args& a) {
  return f82150B80(base, a.r1, a.r13, a.r[0], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821511B8(uint8_t* base, const lifted::Args& a) {
  return f821511B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821512F0(uint8_t* base, const lifted::Args& a) {
  return f821512F0(base, a.r1, a.r13, a.r[0], a.f[0], a.lr);
}

lifted::Result e82151388(uint8_t* base, const lifted::Args& a) {
  return {f82151388(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82151698(uint8_t* base, const lifted::Args& a) {
  f82151698(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82151D20(uint8_t* base, const lifted::Args& a) {
  return f82151D20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82153150(uint8_t* base, const lifted::Args& a) {
  f82153150(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e821540B0(uint8_t* base, const lifted::Args& a) {
  return f821540B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82155BF8(uint8_t* base, const lifted::Args& a) {
  f82155BF8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82155CC0(uint8_t* base, const lifted::Args& a) {
  f82155CC0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82156248(uint8_t* base, const lifted::Args& a) {
  return f82156248(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82159948(uint8_t* base, const lifted::Args& a) {
  return f82159948(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.f[0], a.lr);
}

lifted::Result e8215AA88(uint8_t* base, const lifted::Args& a) {
  return f8215AA88(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215B720(uint8_t* base, const lifted::Args& a) {
  return f8215B720(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8215C248(uint8_t* base, const lifted::Args& a) {
  return f8215C248(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8215C770(uint8_t* base, const lifted::Args& a) {
  return f8215C770(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12], a.lr);
}

lifted::Result e8215D098(uint8_t* base, const lifted::Args& a) {
  return f8215D098(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8215D100(uint8_t* base, const lifted::Args& a) {
  return f8215D100(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8215D528(uint8_t* base, const lifted::Args& a) {
  return f8215D528(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8215D538(uint8_t* base, const lifted::Args& a) {
  return f8215D538(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e8215D7D8(uint8_t* base, const lifted::Args& a) {
  return f8215D7D8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8215DF20(uint8_t* base, const lifted::Args& a) {
  return f8215DF20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215DF80(uint8_t* base, const lifted::Args& a) {
  return f8215DF80(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215E140(uint8_t* base, const lifted::Args& a) {
  f8215E140(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8215E150(uint8_t* base, const lifted::Args& a) {
  return f8215E150(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8215E158(uint8_t* base, const lifted::Args& a) {
  return f8215E158(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215E160(uint8_t* base, const lifted::Args& a) {
  return f8215E160(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215E1D0(uint8_t* base, const lifted::Args& a) {
  return f8215E1D0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215E208(uint8_t* base, const lifted::Args& a) {
  return f8215E208(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215E2E0(uint8_t* base, const lifted::Args& a) {
  return f8215E2E0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215E680(uint8_t* base, const lifted::Args& a) {
  return f8215E680(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215E688(uint8_t* base, const lifted::Args& a) {
  return f8215E688(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215E6C0(uint8_t* base, const lifted::Args& a) {
  return f8215E6C0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8215F5F0(uint8_t* base, const lifted::Args& a) {
  return {f8215F5F0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e8215F7B0(uint8_t* base, const lifted::Args& a) {
  return {f8215F7B0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8215FB08(uint8_t* base, const lifted::Args& a) {
  return f8215FB08(base, a.r1, a.r13, a.r[6], a.f[0]);
}

lifted::Result e8215FC30(uint8_t* base, const lifted::Args& a) {
  return f8215FC30(base, a.r1, a.r13, a.r[6], a.f[0]);
}

lifted::Result e82160880(uint8_t* base, const lifted::Args& a) {
  return f82160880(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e82163218(uint8_t* base, const lifted::Args& a) {
  return f82163218(base, a.r1, a.r13, a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82163328(uint8_t* base, const lifted::Args& a) {
  return f82163328(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e82163358(uint8_t* base, const lifted::Args& a) {
  return f82163358(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0], a.lr);
}

lifted::Result e82163388(uint8_t* base, const lifted::Args& a) {
  f82163388(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

namespace {

const Registration kEntries[] = {
    {0x82136DC8, e82136DC8},
    {0x82136F50, e82136F50},
    {0x82136F58, e82136F58},
    {0x82136F60, e82136F60},
    {0x82136F68, e82136F68},
    {0x82136F70, e82136F70},
    {0x82136F78, e82136F78},
    {0x82136F80, e82136F80},
    {0x82136F88, e82136F88},
    {0x82136F90, e82136F90},
    {0x82136F98, e82136F98},
    {0x82136FA0, e82136FA0},
    {0x82138520, e82138520},
    {0x821392D8, e821392D8},
    {0x821398C0, e821398C0},
    {0x8213A1B8, e8213A1B8},
    {0x8213A200, e8213A200},
    {0x8213AB40, e8213AB40},
    {0x8213BDF0, e8213BDF0},
    {0x8213BE80, e8213BE80},
    {0x8213C248, e8213C248},
    {0x8213D898, e8213D898},
    {0x8213F868, e8213F868},
    {0x82141668, e82141668},
    {0x821462A0, e821462A0},
    {0x82148920, e82148920},
    {0x821489D8, e821489D8},
    {0x82148A80, e82148A80},
    {0x82148B60, e82148B60},
    {0x82148D00, e82148D00},
    {0x82149178, e82149178},
    {0x821495E8, e821495E8},
    {0x821497A8, e821497A8},
    {0x82149888, e82149888},
    {0x821498F0, e821498F0},
    {0x82149A30, e82149A30},
    {0x82149AA8, e82149AA8},
    {0x82149B08, e82149B08},
    {0x82149C48, e82149C48},
    {0x82149D00, e82149D00},
    {0x8214AA18, e8214AA18},
    {0x8214AFE8, e8214AFE8},
    {0x8214B188, e8214B188},
    {0x8214B330, e8214B330},
    {0x8214BF20, e8214BF20},
    {0x8214C828, e8214C828},
    {0x8214C930, e8214C930},
    {0x8214E240, e8214E240},
    {0x8214E608, e8214E608},
    {0x8214E678, e8214E678},
    {0x8214E6C8, e8214E6C8},
    {0x8214EC80, e8214EC80},
    {0x8214ED30, e8214ED30},
    {0x8214F240, e8214F240},
    {0x8214F660, e8214F660},
    {0x8214F690, e8214F690},
    {0x8214F790, e8214F790},
    {0x82150040, e82150040},
    {0x82150B80, e82150B80},
    {0x821511B8, e821511B8},
    {0x821512F0, e821512F0},
    {0x82151388, e82151388},
    {0x82151698, e82151698},
    {0x82151D20, e82151D20},
    {0x82153150, e82153150},
    {0x821540B0, e821540B0},
    {0x82155BF8, e82155BF8},
    {0x82155CC0, e82155CC0},
    {0x82156248, e82156248},
    {0x82159948, e82159948},
    {0x8215AA88, e8215AA88},
    {0x8215B720, e8215B720},
    {0x8215C248, e8215C248},
    {0x8215C770, e8215C770},
    {0x8215D098, e8215D098},
    {0x8215D100, e8215D100},
    {0x8215D528, e8215D528},
    {0x8215D538, e8215D538},
    {0x8215D7D8, e8215D7D8},
    {0x8215DF20, e8215DF20},
    {0x8215DF80, e8215DF80},
    {0x8215E140, e8215E140},
    {0x8215E150, e8215E150},
    {0x8215E158, e8215E158},
    {0x8215E160, e8215E160},
    {0x8215E1D0, e8215E1D0},
    {0x8215E208, e8215E208},
    {0x8215E2E0, e8215E2E0},
    {0x8215E680, e8215E680},
    {0x8215E688, e8215E688},
    {0x8215E6C0, e8215E6C0},
    {0x8215F5F0, e8215F5F0},
    {0x8215F7B0, e8215F7B0},
    {0x8215FB08, e8215FB08},
    {0x8215FC30, e8215FC30},
    {0x82160880, e82160880},
    {0x82163218, e82163218},
    {0x82163328, e82163328},
    {0x82163358, e82163358},
    {0x82163388, e82163388},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_007)), true);

}  // namespace

}  // namespace lifted
