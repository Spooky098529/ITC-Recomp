// Lift sweep batch 033 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"
#include "schema/app_start_up.h"
#include "schema/command_buffer.h"
#include "schema/device_state.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_033, true, "ITC",
              "Run lift sweep batch 033's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82121760: lifted from generated/ by lift.py.
uint64_t f82121760(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82121760 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
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
  // std r6,184(r1)
  guest::StoreU64(base, uint32_t(r1) + 184, r6);
  // addi r6,r3,224
  r6 = uint64_t(int64_t(int64_t(r3) + 224));
  // std r5,176(r1)
  guest::StoreU64(base, uint32_t(r1) + 176, r5);
  // addi r5,r1,176
  r5 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfs f0,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // lwz r11,208(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 208);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // stfs f0,48(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 48, temp.u32);
  // lfs f0,4(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 4);
  f0 = double(temp.f32);
  // ld r28,0(r5)
  r28 = guest::LoadU64(base, uint32_t(r5) + 0);
  // stfs f0,52(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 52, temp.u32);
  // ld r5,8(r5)
  r5 = guest::LoadU64(base, uint32_t(r5) + 8);
  // lfs f0,8(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 8);
  f0 = double(temp.f32);
  // mr r31,r30
  r31 = r30;
  // stfs f0,56(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 56, temp.u32);
  // lfs f0,12(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 12);
  f0 = double(temp.f32);
  // stfs f0,60(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 60, temp.u32);
  // std r28,384(r3)
  guest::StoreU64(base, uint32_t(r3) + 384, r28);
  // std r5,392(r3)
  guest::StoreU64(base, uint32_t(r3) + 392, r5);
  // stw r9,404(r3)
  guest::StoreU32(base, uint32_t(r3) + 404, uint32_t(r9));
  // stw r10,408(r3)
  guest::StoreU32(base, uint32_t(r3) + 408, uint32_t(r10));
  // lwz r10,96(r6)
  r10 = guest::LoadU32(base, uint32_t(r6) + 96);
  // stw r7,112(r6)
  guest::StoreU32(base, uint32_t(r6) + 112, uint32_t(r7));
  // stw r8,120(r6)
  guest::StoreU32(base, uint32_t(r6) + 120, uint32_t(r8));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // stw r29,128(r6)
  guest::StoreU32(base, uint32_t(r6) + 128, uint32_t(r29));
  // beq cr6,0x82121cb4
  if (cr6.eq) goto loc_82121CB4;
  // lwz r10,404(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 404);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x821217ec
  if (!cr6.eq) goto loc_821217EC;
  // mr r31,r29
  r31 = r29;
loc_821217EC:
  // lwz r8,0(r11)
  r8 = guest::LoadU32(base, uint32_t(r11) + 0);
  // addi r10,r3,424
  r10 = uint64_t(int64_t(int64_t(r3) + 424));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // stw r8,108(r6)
  guest::StoreU32(base, uint32_t(r6) + 108, uint32_t(r8));
loc_821217FC:
  // lfs f0,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // stfs f0,-8(r10)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + -8, temp.u32);
  // lfs f0,4(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 4);
  f0 = double(temp.f32);
  // stfs f0,-4(r10)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + -4, temp.u32);
  // lfs f0,8(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 8);
  f0 = double(temp.f32);
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 0, temp.u32);
  // lfs f0,12(r4)
  temp.u32 = guest::LoadU32(base, uint32_t(r4) + 12);
  f0 = double(temp.f32);
  // stfs f0,4(r10)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r10) + 4, temp.u32);
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // bne 0x821217fc
  if (!cr0.eq) goto loc_821217FC;
  // lwz r10,112(r6)
  r10 = guest::LoadU32(base, uint32_t(r6) + 112);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmplwi cr6,r10,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 7, so);
  // bgt cr6,0x82121c74
  if (cr6.gt) goto loc_82121C74;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-31568
  r12 = uint64_t(int64_t(int64_t(r12) + -31568));
  // rlwinm r0,r10,1,0,30
  r0 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 1) & 0xFFFFFFFE;
  // lhzx r0,r12,r0
  r0 = guest::LoadU16(base, uint32_t(r12) + uint32_t(r0));
  // lis r12,-32238
  r12 = uint64_t(int64_t(-2112749568));
  // addi r12,r12,6240
  r12 = uint64_t(int64_t(int64_t(r12) + 6240));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r10)) {
  case 0:
  goto loc_frag_82121860;
  return r3;
  case 1:
  goto loc_82121A44;
  case 2:
  goto loc_82121B44;
  case 3:
  goto loc_frag_82121860;
  return r3;
  case 4:
  goto loc_82121B88;
  case 5:
  goto loc_82121B88;
  case 6:
  goto loc_82121C0C;
  case 7:
  goto loc_82121C44;
  default:
  std::abort();
  }
loc_82121920:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31516(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31516);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30;
loc_82121940:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,9576(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9576);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30;
loc_82121960:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8152(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8152);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30;
loc_82121980:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,3396(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 3396);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30;
loc_821219A0:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31520(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31520);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30;
loc_821219C0:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31524(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31524);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30;
loc_821219E0:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31528(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31528);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30;
loc_82121A00:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31532(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31532);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30;
loc_82121A20:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31536(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31536);
  f0 = double(temp.f32);
loc_82121A30:
  // fadds f0,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f13 + f0));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // li r12,108
  r12 = uint64_t(int64_t(108));
  // stfiwx f0,r6,r12
  guest::StoreU32(base, uint32_t(r6) + uint32_t(r12), uint32_t(std::bit_cast<uint64_t>(f0)));
  // b 0x82121cb0
  goto loc_82121CB0;
loc_82121A44:
  // lis r10,-32159
  r10 = uint64_t(int64_t(-2107572224));
  // lfs f0,24(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 24);
  f0 = double(temp.f32);
  // stfs f0,104(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 104, temp.u32);
  // addi r9,r6,92
  r9 = uint64_t(int64_t(int64_t(r6) + 92));
  // addi r10,r10,28080
  r10 = uint64_t(int64_t(int64_t(r10) + 28080));
  // lfs f0,20(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 20);
  f0 = double(temp.f32);
  // stfs f0,100(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 100, temp.u32);
  // stw r29,124(r6)
  guest::StoreU32(base, uint32_t(r6) + 124, uint32_t(r29));
  // ld r11,0(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 0);
  // std r11,80(r6)
  guest::StoreU64(base, uint32_t(r6) + 80, r11);
  // ld r11,8(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r11,88(r6)
  guest::StoreU64(base, uint32_t(r6) + 88, r11);
  // lwz r11,120(r6)
  r11 = guest::LoadU32(base, uint32_t(r6) + 120);
  // lfs f0,104(r6)
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 104);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r9
  guest::StoreU32(base, uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f0)));
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bgt cr6,0x82121cb0
  if (cr6.gt) goto loc_82121CB0;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-31600
  r12 = uint64_t(int64_t(int64_t(r12) + -31600));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32238
  r12 = uint64_t(int64_t(-2112749568));
  // addi r12,r12,6836
  r12 = uint64_t(int64_t(int64_t(r12) + 6836));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_82121AB4;
  case 1:
  goto loc_82121AC4;
  case 2:
  goto loc_82121AD4;
  case 3:
  goto loc_82121AE4;
  case 4:
  goto loc_82121AF4;
  case 5:
  goto loc_82121B04;
  case 6:
  goto loc_82121B14;
  case 7:
  goto loc_82121B24;
  case 8:
  goto loc_82121B34;
  default:
  std::abort();
  }
loc_82121AB4:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // b 0x82121920
  goto loc_82121920;
loc_82121AC4:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // b 0x82121940
  goto loc_82121940;
loc_82121AD4:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // b 0x821219a0
  goto loc_821219A0;
loc_82121AE4:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // b 0x82121980
  goto loc_82121980;
loc_82121AF4:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // b 0x82121960
  goto loc_82121960;
loc_82121B04:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // b 0x821219c0
  goto loc_821219C0;
loc_82121B14:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // b 0x821219e0
  goto loc_821219E0;
loc_82121B24:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // b 0x82121a00
  goto loc_82121A00;
loc_82121B34:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // b 0x82121a20
  goto loc_82121A20;
loc_82121B44:
  // lis r10,-32159
  r10 = uint64_t(int64_t(-2107572224));
  // lfs f0,32(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 32);
  f0 = double(temp.f32);
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stfs f0,104(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 104, temp.u32);
  // addi r10,r10,28208
  r10 = uint64_t(int64_t(int64_t(r10) + 28208));
  // lfs f0,28(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 28);
  f0 = double(temp.f32);
  // stfs f0,100(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 100, temp.u32);
  // addi r8,r6,92
  r8 = uint64_t(int64_t(int64_t(r6) + 92));
  // stw r9,124(r6)
  guest::StoreU32(base, uint32_t(r6) + 124, uint32_t(r9));
  // ld r11,0(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 0);
  // std r11,80(r6)
  guest::StoreU64(base, uint32_t(r6) + 80, r11);
  // ld r11,8(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r11,88(r6)
  guest::StoreU64(base, uint32_t(r6) + 88, r11);
loc_82121B78:
  // lfs f0,104(r6)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 104);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r8
  guest::StoreU32(base, uint32_t(r8), uint32_t(std::bit_cast<uint64_t>(f0)));
  // b 0x82121cb0
  goto loc_82121CB0;
loc_82121B88:
  // lfs f0,40(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 40);
  f0 = double(temp.f32);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // stfs f0,104(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 104, temp.u32);
  // lfs f0,36(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 36);
  f0 = double(temp.f32);
  // stw r30,124(r6)
  guest::StoreU32(base, uint32_t(r6) + 124, uint32_t(r30));
  // stfs f0,100(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 100, temp.u32);
  // beq cr6,0x82121bd8
  if (cr6.eq) goto loc_82121BD8;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r9,r11,23840
  r9 = uint64_t(int64_t(int64_t(r11) + 23840));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r8,0(r9)
  r8 = guest::LoadU64(base, uint32_t(r9) + 0);
  // ld r9,8(r9)
  r9 = guest::LoadU64(base, uint32_t(r9) + 8);
  // std r8,0(r11)
  guest::StoreU64(base, uint32_t(r11) + 0, r8);
  // std r9,8(r11)
  guest::StoreU64(base, uint32_t(r11) + 8, r9);
  // ld r11,0(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 0);
  // ld r10,8(r10)
  r10 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r11,80(r6)
  guest::StoreU64(base, uint32_t(r6) + 80, r11);
  // std r10,88(r6)
  guest::StoreU64(base, uint32_t(r6) + 88, r10);
  // b 0x82121bf8
  goto loc_82121BF8;
loc_82121BD8:
  // lwz r5,404(r3)
  r5 = guest::LoadU32(base, uint32_t(r3) + 404);
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82121590
  // (left out: dead in C++)
  f82121590(base, r3, r5);
  // ld r11,0(r3)
  r11 = guest::LoadU64(base, uint32_t(r3) + 0);
  // std r11,80(r6)
  guest::StoreU64(base, uint32_t(r6) + 80, r11);
  // ld r11,8(r3)
  r11 = guest::LoadU64(base, uint32_t(r3) + 8);
  // std r11,88(r6)
  guest::StoreU64(base, uint32_t(r6) + 88, r11);
loc_82121BF8:
  // addi r11,r6,92
  r11 = uint64_t(int64_t(int64_t(r6) + 92));
  // lfs f0,104(r6)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 104);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r11
  guest::StoreU32(base, uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // b 0x82121cb0
  goto loc_82121CB0;
loc_82121C0C:
  // lis r10,-32159
  r10 = uint64_t(int64_t(-2107572224));
  // lfs f0,48(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 48);
  f0 = double(temp.f32);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stfs f0,104(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 104, temp.u32);
  // addi r10,r10,28080
  r10 = uint64_t(int64_t(int64_t(r10) + 28080));
  // lfs f0,44(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 44);
  f0 = double(temp.f32);
  // stfs f0,100(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 100, temp.u32);
  // addi r8,r6,92
  r8 = uint64_t(int64_t(int64_t(r6) + 92));
  // stw r9,124(r6)
  guest::StoreU32(base, uint32_t(r6) + 124, uint32_t(r9));
  // ld r11,0(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 0);
  // std r11,80(r6)
  guest::StoreU64(base, uint32_t(r6) + 80, r11);
  // ld r11,8(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r11,88(r6)
  guest::StoreU64(base, uint32_t(r6) + 88, r11);
  // b 0x82121b78
  goto loc_82121B78;
loc_82121C44:
  // lis r10,-32159
  r10 = uint64_t(int64_t(-2107572224));
  // lfs f0,56(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 56);
  f0 = double(temp.f32);
  // stfs f0,104(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 104, temp.u32);
  // addi r10,r10,28080
  r10 = uint64_t(int64_t(int64_t(r10) + 28080));
  // lfs f0,52(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 52);
  f0 = double(temp.f32);
  // stfs f0,100(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 100, temp.u32);
  // stw r30,124(r6)
  guest::StoreU32(base, uint32_t(r6) + 124, uint32_t(r30));
  // ld r11,0(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 0);
  // std r11,80(r6)
  guest::StoreU64(base, uint32_t(r6) + 80, r11);
  // ld r11,8(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r11,88(r6)
  guest::StoreU64(base, uint32_t(r6) + 88, r11);
  // b 0x82121ca0
  goto loc_82121CA0;
loc_82121C74:
  // lis r10,-32159
  r10 = uint64_t(int64_t(-2107572224));
  // lfs f0,8(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8);
  f0 = double(temp.f32);
  // stfs f0,104(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 104, temp.u32);
  // addi r10,r10,28080
  r10 = uint64_t(int64_t(int64_t(r10) + 28080));
  // lfs f0,4(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 4);
  f0 = double(temp.f32);
  // stfs f0,100(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 100, temp.u32);
  // stw r30,124(r6)
  guest::StoreU32(base, uint32_t(r6) + 124, uint32_t(r30));
  // ld r11,0(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 0);
  // std r11,80(r6)
  guest::StoreU64(base, uint32_t(r6) + 80, r11);
  // ld r11,8(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r11,88(r6)
  guest::StoreU64(base, uint32_t(r6) + 88, r11);
loc_82121CA0:
  // addi r9,r6,92
  r9 = uint64_t(int64_t(int64_t(r6) + 92));
  // lfs f0,104(r6)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 104);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r9
  guest::StoreU32(base, uint32_t(r9), uint32_t(std::bit_cast<uint64_t>(f0)));
loc_82121CB0:
  // stw r29,96(r6)
  guest::StoreU32(base, uint32_t(r6) + 96, uint32_t(r29));
loc_82121CB4:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
loc_frag_82121914:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31516(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31516);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30_82121914;
loc_82121A30_82121914:
  // fadds f0,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f13 + f0));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // li r12,108
  r12 = uint64_t(int64_t(108));
  // stfiwx f0,r6,r12
  guest::StoreU32(base, uint32_t(r6) + uint32_t(r12), uint32_t(std::bit_cast<uint64_t>(f0)));
  // b 0x82121cb0
  goto loc_82121CB0_82121914;
loc_82121CB0_82121914:
  // stw r29,96(r6)
  guest::StoreU32(base, uint32_t(r6) + 96, uint32_t(r29));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
loc_frag_82121860:
  // lfs f0,16(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 16);
  f0 = double(temp.f32);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // stfs f0,104(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 104, temp.u32);
  // lfs f0,12(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 12);
  f0 = double(temp.f32);
  // stw r30,124(r6)
  guest::StoreU32(base, uint32_t(r6) + 124, uint32_t(r30));
  // stfs f0,100(r6)
  temp.f32 = float(f0);
  guest::StoreU32(base, uint32_t(r6) + 100, temp.u32);
  // beq cr6,0x821218b0
  if (cr6.eq) goto loc_821218B0_82121860;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r9,r11,23840
  r9 = uint64_t(int64_t(int64_t(r11) + 23840));
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r8,0(r9)
  r8 = guest::LoadU64(base, uint32_t(r9) + 0);
  // ld r9,8(r9)
  r9 = guest::LoadU64(base, uint32_t(r9) + 8);
  // std r8,0(r11)
  guest::StoreU64(base, uint32_t(r11) + 0, r8);
  // std r9,8(r11)
  guest::StoreU64(base, uint32_t(r11) + 8, r9);
  // ld r11,0(r10)
  r11 = guest::LoadU64(base, uint32_t(r10) + 0);
  // ld r10,8(r10)
  r10 = guest::LoadU64(base, uint32_t(r10) + 8);
  // std r11,80(r6)
  guest::StoreU64(base, uint32_t(r6) + 80, r11);
  // std r10,88(r6)
  guest::StoreU64(base, uint32_t(r6) + 88, r10);
  // b 0x821218d0
  goto loc_821218D0_82121860;
loc_821218B0_82121860:
  // lwz r5,404(r3)
  r5 = guest::LoadU32(base, uint32_t(r3) + 404);
  // mr r4,r3
  r4 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82121590
  // (left out: dead in C++)
  f82121590(base, r3, r5);
  // ld r11,0(r3)
  r11 = guest::LoadU64(base, uint32_t(r3) + 0);
  // std r11,80(r6)
  guest::StoreU64(base, uint32_t(r6) + 80, r11);
  // ld r11,8(r3)
  r11 = guest::LoadU64(base, uint32_t(r3) + 8);
  // std r11,88(r6)
  guest::StoreU64(base, uint32_t(r6) + 88, r11);
loc_821218D0_82121860:
  // addi r10,r6,92
  r10 = uint64_t(int64_t(int64_t(r6) + 92));
  // lwz r11,120(r6)
  r11 = guest::LoadU32(base, uint32_t(r6) + 120);
  // lfs f0,104(r6)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r6) + 104);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // stfiwx f0,0,r10
  guest::StoreU32(base, uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // bgt cr6,0x82121cb0
  if (cr6.gt) goto loc_82121CB0_82121860;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-31584
  r12 = uint64_t(int64_t(int64_t(r12) + -31584));
  // lbzx r0,r12,r11
  r0 = guest::LoadU8(base, uint32_t(r12) + uint32_t(r11));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32238
  r12 = uint64_t(int64_t(-2112749568));
  // addi r12,r12,6420
  r12 = uint64_t(int64_t(int64_t(r12) + 6420));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_82121914;
  return r3;
  case 1:
  goto loc_82121934_82121860;
  case 2:
  goto loc_82121954_82121860;
  case 3:
  goto loc_82121974_82121860;
  case 4:
  goto loc_82121994_82121860;
  case 5:
  goto loc_821219B4_82121860;
  case 6:
  goto loc_821219D4_82121860;
  case 7:
  goto loc_821219F4_82121860;
  case 8:
  goto loc_82121A14_82121860;
  default:
  std::abort();
  }
loc_82121934_82121860:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,9576(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 9576);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30_82121860;
loc_82121954_82121860:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8152(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 8152);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30_82121860;
loc_82121974_82121860:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,3396(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 3396);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30_82121860;
loc_82121994_82121860:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31520(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31520);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30_82121860;
loc_821219B4_82121860:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31524(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31524);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30_82121860;
loc_821219D4_82121860:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31528(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31528);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30_82121860;
loc_821219F4_82121860:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31532(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31532);
  f0 = double(temp.f32);
  // b 0x82121a30
  goto loc_82121A30_82121860;
loc_82121A14_82121860:
  // lwa r11,108(r6)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(base, uint32_t(r6) + 108))));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-31536(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + -31536);
  f0 = double(temp.f32);
loc_82121A30_82121860:
  // fadds f0,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f13 + f0));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // li r12,108
  r12 = uint64_t(int64_t(108));
  // stfiwx f0,r6,r12
  guest::StoreU32(base, uint32_t(r6) + uint32_t(r12), uint32_t(std::bit_cast<uint64_t>(f0)));
  // b 0x82121cb0
  goto loc_82121CB0_82121860;
loc_82121CB0_82121860:
  // stw r29,96(r6)
  guest::StoreU32(base, uint32_t(r6) + 96, uint32_t(r29));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_822997B0: lifted from generated/ by lift.py.
lifted::Result f822997B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_822997B0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e6c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r31,r1,-272
  r31 = uint64_t(int64_t(int64_t(r1) + -272));
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r24,r3
  r24 = r3;
  // stw r24,292(r31)
  guest::StoreU32(base, uint32_t(r31) + 292, uint32_t(r24));
  // mr r25,r4
  r25 = r4;
  // stw r25,300(r31)
  guest::StoreU32(base, uint32_t(r31) + 300, uint32_t(r25));
  // stw r5,308(r31)
  guest::StoreU32(base, uint32_t(r31) + 308, uint32_t(r5));
  // stw r6,316(r31)
  guest::StoreU32(base, uint32_t(r31) + 316, uint32_t(r6));
  // mr r21,r7
  r21 = r7;
  // stw r21,324(r31)
  guest::StoreU32(base, uint32_t(r31) + 324, uint32_t(r21));
  // li r22,0
  r22 = uint64_t(int64_t(0));
  // stw r22,80(r31)
  guest::StoreU32(base, uint32_t(r31) + 80, uint32_t(r22));
  // mr r30,r22
  r30 = r22;
  // addi r10,r31,128
  r10 = uint64_t(int64_t(int64_t(r31) + 128));
  // mr r9,r22
  r9 = r22;
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // mtctr r11
  ctr = r11;
loc_822997FC:
  // std r9,0(r10)
  guest::StoreU64(base, uint32_t(r10) + 0, r9);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // bdnz 0x822997fc
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_822997FC;
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x82299874
  if (cr6.eq) goto loc_82299874;
  // mr r8,r8
  // (left out: dead in C++)
  // mr r8,r8
  // (left out: dead in C++)
  // lwz r11,0(r8)
  r11 = guest::LoadU32(base, uint32_t(r8) + 0);
  // cmplwi cr6,r11,48
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 48, so);
  // bne cr6,0x82299838
  if (!cr6.eq) goto loc_82299838;
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // mr r4,r8
  r4 = r8;
  // addi r3,r31,128
  r3 = uint64_t(int64_t(int64_t(r31) + 128));
  // bl 0x825aa840
  // (left out: dead in C++)
  { const lifted::Result o = game::memmove(base, LIFTED_ARGS(0x82299834, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r5,308(r31)
  r5 = guest::LoadU32(base, uint32_t(r31) + 308);
loc_82299838:
  // mr r8,r8
  // (left out: dead in C++)
  // mr r8,r8
  // (left out: dead in C++)
  // mr r8,r8
  // (left out: dead in C++)
  // b 0x82299860
  goto loc_82299860;
loc_82299860:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge cr6,0x82299870
  if (!cr6.lt) goto loc_82299870;
loc_82299868:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82299d40
  goto loc_82299D40;
loc_82299870:
  // lwz r6,316(r31)
  r6 = guest::LoadU32(base, uint32_t(r31) + 316);
loc_82299874:
  // lis r23,-32105
  r23 = uint64_t(int64_t(-2104033280));
  // lwz r11,5272(r23)
  r11 = guest::LoadU32(base, uint32_t(r23) + 5272);
  // rlwinm. r11,r11,0,10,10
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x200000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82299888
  if (cr0.eq) goto loc_82299888;
  // ori r24,r24,128
  r24 = r24 | 128;
loc_82299888:
  // lwz r11,132(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 132);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822998a0
  if (!cr6.eq) goto loc_822998A0;
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // lwz r11,-30488(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -30488);
  // stw r11,132(r31)
  guest::StoreU32(base, uint32_t(r31) + 132, uint32_t(r11));
loc_822998A0:
  // lwz r11,136(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 136);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822998b8
  if (!cr6.eq) goto loc_822998B8;
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // lwz r11,-30484(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -30484);
  // stw r11,136(r31)
  guest::StoreU32(base, uint32_t(r31) + 136, uint32_t(r11));
loc_822998B8:
  // lwz r11,140(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 140);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822998d0
  if (!cr6.eq) goto loc_822998D0;
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // lwz r11,-30476(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -30476);
  // stw r11,140(r31)
  guest::StoreU32(base, uint32_t(r31) + 140, uint32_t(r11));
loc_822998D0:
  // lwz r11,144(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 144);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822998e8
  if (!cr6.eq) goto loc_822998E8;
  // lis r11,-32156
  r11 = uint64_t(int64_t(-2107375616));
  // lwz r11,-30480(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + -30480);
  // stw r11,144(r31)
  guest::StoreU32(base, uint32_t(r31) + 144, uint32_t(r11));
loc_822998E8:
  // lwz r11,148(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 148);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82299900
  if (!cr6.eq) goto loc_82299900;
  // lis r11,32764
  r11 = uint64_t(int64_t(2147221504));
  // ori r11,r11,65535
  r11 = r11 | 65535;
  // stw r11,148(r31)
  guest::StoreU32(base, uint32_t(r31) + 148, uint32_t(r11));
loc_82299900:
  // lwz r11,152(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 152);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82299918
  if (cr6.eq) goto loc_82299918;
  // lis r10,15
  r10 = uint64_t(int64_t(983040));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82299920
  if (!cr6.gt) goto loc_82299920;
loc_82299918:
  // lis r11,15
  r11 = uint64_t(int64_t(983040));
  // stw r11,152(r31)
  guest::StoreU32(base, uint32_t(r31) + 152, uint32_t(r11));
loc_82299920:
  // cmplwi cr6,r6,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // lis r30,1
  r30 = uint64_t(int64_t(65536));
  // bne cr6,0x8229997c
  if (!cr6.eq) goto loc_8229997C;
  // stw r30,316(r31)
  guest::StoreU32(base, uint32_t(r31) + 316, uint32_t(r30));
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // bne cr6,0x8229994c
  if (!cr6.eq) goto loc_8229994C;
  // lis r11,64
  r11 = uint64_t(int64_t(4194304));
  // stw r11,308(r31)
  guest::StoreU32(base, uint32_t(r31) + 308, uint32_t(r11));
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r27,r11,65535
  r27 = r11 | 65535;
  // b 0x82299960
  goto loc_82299960;
loc_8229994C:
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r27,r11,65535
  r27 = r11 | 65535;
loc_82299954:
  // add r11,r5,r27
  r11 = r5 + r27;
  // rlwinm r11,r11,0,0,15
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFF0000;
loc_8229995C:
  // stw r11,308(r31)
  guest::StoreU32(base, uint32_t(r31) + 308, uint32_t(r11));
loc_82299960:
  // clrlwi. r11,r24,31
  r11 = uint32_t(r24) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r29,1424
  r29 = uint64_t(int64_t(1424));
  // cmplwi cr6,r21,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r21), 0, so);
  // bne 0x822999b4
  if (!cr0.eq) goto loc_822999B4;
  // beq cr6,0x822999a8
  if (cr6.eq) goto loc_822999A8;
  // oris r24,r24,32768
  r24 = r24 | 2147483648;
  // b 0x822999b8
  goto loc_822999B8;
loc_8229997C:
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r27,r11,65535
  r27 = r11 | 65535;
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // add r11,r6,r27
  r11 = r6 + r27;
  // rlwinm r11,r11,0,0,15
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFF0000;
  // stw r11,316(r31)
  guest::StoreU32(base, uint32_t(r31) + 316, uint32_t(r11));
  // bne cr6,0x82299954
  if (!cr6.eq) goto loc_82299954;
  // addis r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 1048576));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // rlwinm r11,r11,0,0,11
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFF00000;
  // b 0x8229995c
  goto loc_8229995C;
loc_822999A8:
  // li r29,1452
  r29 = uint64_t(int64_t(1452));
  // li r21,-1
  r21 = uint64_t(int64_t(-1));
  // b 0x822999b8
  goto loc_822999B8;
loc_822999B4:
  // bne cr6,0x82299868
  if (!cr6.eq) goto loc_82299868;
loc_822999B8:
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // lwz r11,164(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 164);
  // beq cr6,0x82299ac0
  if (cr6.eq) goto loc_82299AC0;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82299a14
  if (cr6.eq) goto loc_82299A14;
  // lwz r10,156(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 156);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x82299868
  if (cr6.eq) goto loc_82299868;
  // lwz r11,160(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 160);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82299868
  if (cr6.eq) goto loc_82299868;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bgt cr6,0x82299868
  if (cr6.gt) goto loc_82299868;
  // rlwinm. r9,r24,0,30,30
  r9 = __builtin_rotateleft64(uint32_t(r24) | (r24 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x82299868
  if (!cr0.eq) goto loc_82299868;
  // lis r5,1
  r5 = uint64_t(int64_t(65536));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r25
  r3 = r25;
  // stw r25,84(r31)
  guest::StoreU32(base, uint32_t(r31) + 84, uint32_t(r25));
  // add r28,r10,r25
  r28 = r10 + r25;
  // stw r11,308(r31)
  guest::StoreU32(base, uint32_t(r31) + 308, uint32_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82299A10, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82299ab0
  goto loc_82299AB0;
loc_82299A14:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r31,96
  r4 = uint64_t(int64_t(int64_t(r31) + 96));
  // mr r3,r25
  r3 = r25;
  // bl 0x825db694
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtQueryVirtualMemory, LIFTED_ARGS(0x82299A24, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82299868
  if (cr0.lt) goto loc_82299868;
  // lwz r3,96(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 96);
  // cmplw cr6,r3,r25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r25), so);
  // bne cr6,0x82299868
  if (!cr6.eq) goto loc_82299868;
  // lwz r11,112(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 112);
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // beq cr6,0x82299868
  if (cr6.eq) goto loc_82299868;
  // stw r3,84(r31)
  guest::StoreU32(base, uint32_t(r31) + 84, uint32_t(r3));
  // cmplwi cr6,r11,4096
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4096, so);
  // bne cr6,0x82299aa8
  if (!cr6.eq) goto loc_82299AA8;
  // lis r5,1
  r5 = uint64_t(int64_t(65536));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = game::memset(base, LIFTED_ARGS(0x82299A5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,108(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 108);
  // lwz r10,84(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 84);
  // add r28,r11,r10
  r28 = r11 + r10;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r31,96
  r4 = uint64_t(int64_t(int64_t(r31) + 96));
  // stw r11,316(r31)
  guest::StoreU32(base, uint32_t(r31) + 316, uint32_t(r11));
  // mr r3,r28
  r3 = r28;
  // bl 0x825db694
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtQueryVirtualMemory, LIFTED_ARGS(0x82299A7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,316(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 316);
  // stw r11,308(r31)
  guest::StoreU32(base, uint32_t(r31) + 308, uint32_t(r11));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82299ab0
  if (cr0.lt) goto loc_82299AB0;
  // lwz r10,112(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 112);
  // cmplwi cr6,r10,8192
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 8192, so);
  // bne cr6,0x82299ab0
  if (!cr6.eq) goto loc_82299AB0;
  // lwz r10,108(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 108);
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r11,308(r31)
  guest::StoreU32(base, uint32_t(r31) + 308, uint32_t(r11));
  // b 0x82299ab0
  goto loc_82299AB0;
loc_82299AA8:
  // stw r30,316(r31)
  guest::StoreU32(base, uint32_t(r31) + 316, uint32_t(r30));
  // mr r28,r3
  r28 = r3;
loc_82299AB0:
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // mr r10,r25
  r10 = r25;
  // stw r10,80(r31)
  guest::StoreU32(base, uint32_t(r31) + 80, uint32_t(r10));
  // b 0x82299b0c
  goto loc_82299B0C;
loc_82299AC0:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82299868
  if (!cr6.eq) goto loc_82299868;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // lis r5,24576
  r5 = uint64_t(int64_t(1610612736));
  // ori r5,r5,8192
  r5 = r5 | 8192;
  // addi r4,r31,308
  r4 = uint64_t(int64_t(int64_t(r31) + 308));
  // addi r3,r31,80
  r3 = uint64_t(int64_t(int64_t(r31) + 80));
  // bl 0x825db6d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtAllocateVirtualMemory, LIFTED_ARGS(0x82299AE4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82299868
  if (cr0.lt) goto loc_82299868;
  // mr r26,r22
  r26 = r22;
  // lwz r11,316(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 316);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82299b00
  if (!cr6.eq) goto loc_82299B00;
  // stw r30,316(r31)
  guest::StoreU32(base, uint32_t(r31) + 316, uint32_t(r30));
loc_82299B00:
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r10,84(r31)
  guest::StoreU32(base, uint32_t(r31) + 84, uint32_t(r10));
  // mr r28,r10
  r28 = r10;
loc_82299B0C:
  // lwz r11,84(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 84);
  // cmplw cr6,r11,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r28), so);
  // bne cr6,0x82299b6c
  if (!cr6.eq) goto loc_82299B6C;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,4
  r6 = uint64_t(int64_t(4));
  // lis r5,24576
  r5 = uint64_t(int64_t(1610612736));
  // ori r5,r5,4096
  r5 = r5 | 4096;
  // addi r4,r31,316
  r4 = uint64_t(int64_t(int64_t(r31) + 316));
  // addi r3,r31,84
  r3 = uint64_t(int64_t(int64_t(r31) + 84));
  // bl 0x825db6d4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtAllocateVirtualMemory, LIFTED_ARGS(0x82299B34, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82299b60
  if (!cr0.lt) goto loc_82299B60;
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // bne cr6,0x82299868
  if (!cr6.eq) goto loc_82299868;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lis r5,0
  r5 = uint64_t(int64_t(0));
  // ori r5,r5,32768
  r5 = r5 | 32768;
  // addi r4,r31,308
  r4 = uint64_t(int64_t(int64_t(r31) + 308));
  // addi r3,r31,80
  r3 = uint64_t(int64_t(int64_t(r31) + 80));
  // bl 0x825db6c4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__NtFreeVirtualMemory, LIFTED_ARGS(0x82299B5C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x82299868
  goto loc_82299868;
loc_82299B60:
  // lwz r11,316(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 316);
  // add r28,r28,r11
  r28 = r28 + r11;
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
loc_82299B6C:
  // addi r11,r10,1431
  r11 = uint64_t(int64_t(int64_t(r10) + 1431));
  // li r8,8
  r8 = uint64_t(int64_t(8));
  // rlwinm r11,r11,0,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFF8;
  // addi r9,r29,128
  r9 = uint64_t(int64_t(int64_t(r29) + 128));
  // addi r10,r10,76
  r10 = uint64_t(int64_t(int64_t(r10) + 76));
loc_82299B80:
  // addic. r8,r8,-1
  ca = uint32_t(r8) > 0;
  r8 = uint64_t(int64_t(int64_t(r8) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // stw r11,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r11));
  // mr r10,r11
  r10 = r11;
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // bne 0x82299b80
  if (!cr0.eq) goto loc_82299B80;
  // mr r29,r11
  r29 = r11;
  // stw r22,0(r10)
  guest::StoreU32(base, uint32_t(r10) + 0, uint32_t(r22));
  // lwz r10,5272(r23)
  r10 = guest::LoadU32(base, uint32_t(r23) + 5272);
  // rlwinm. r10,r10,0,20,20
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x800;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82299bc8
  if (cr0.eq) goto loc_82299BC8;
  // addi r11,r11,7
  r11 = uint64_t(int64_t(int64_t(r11) + 7));
  // addi r9,r9,1548
  r9 = uint64_t(int64_t(int64_t(r9) + 1548));
  // rlwinm r11,r11,0,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFF8;
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,380(r10)
  guest::StoreU32(base, uint32_t(r10) + 380, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // lwz r11,380(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 380);
  // addi r29,r11,1548
  r29 = uint64_t(int64_t(int64_t(r11) + 1548));
loc_82299BC8:
  // addi r11,r9,15
  r11 = uint64_t(int64_t(int64_t(r9) + 15));
  // rlwinm r30,r11,0,0,27
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFF0;
  // lis r11,-4353
  r11 = uint64_t(int64_t(-285278208));
  // ori r11,r11,61183
  r11 = r11 | 61183;
  // lis r12,24577
  r12 = uint64_t(int64_t(1610678272));
  // ori r12,r12,125
  r12 = r12 | 125;
  // and r10,r24,r12
  r10 = r24 & r12;
  // rlwinm r9,r30,28,4,31
  r9 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 28) & 0xFFFFFFF;
  // lwz r8,80(r31)
  r8 = guest::LoadU32(base, uint32_t(r31) + 80);
  // sth r9,0(r8)
  guest::StoreU16(base, uint32_t(r8) + 0, uint16_t(r9));
  // lwz r9,80(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 80);
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stb r8,5(r9)
  guest::StoreU8(base, uint32_t(r9) + 5, uint8_t(r8));
  // lwz r9,80(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,16(r9)
  guest::StoreU32(base, uint32_t(r9) + 16, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r24,20(r11)
  guest::StoreU32(base, uint32_t(r11) + 20, uint32_t(r24));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r10,24(r11)
  guest::StoreU32(base, uint32_t(r11) + 24, uint32_t(r10));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // sth r27,368(r11)
  guest::StoreU16(base, uint32_t(r11) + 368, uint16_t(r27));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // subf r10,r11,r29
  r10 = r29 - r11;
  // sth r10,58(r11)
  guest::StoreU16(base, uint32_t(r11) + 58, uint16_t(r10));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r22,60(r11)
  guest::StoreU32(base, uint32_t(r11) + 60, uint32_t(r22));
  // bl 0x825db934
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeGetCurrentProcessType, LIFTED_ARGS(0x82299C34, r31)); r3 = o.r3; f1 = o.f1; }
  // li r10,128
  r10 = uint64_t(int64_t(128));
  // lwz r9,80(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stb r3,379(r9)
  guest::StoreU8(base, uint32_t(r9) + 379, uint8_t(r3));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // addi r11,r11,384
  r11 = uint64_t(int64_t(int64_t(r11) + 384));
loc_82299C48:
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r11));
  // stw r11,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r11));
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // bne 0x82299c48
  if (!cr0.eq) goto loc_82299C48;
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // addi r11,r11,88
  r11 = uint64_t(int64_t(int64_t(r11) + 88));
  // cmpwi cr6,r21,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r21), -1, so);
  // stw r11,0(r11)
  guest::StoreU32(base, uint32_t(r11) + 0, uint32_t(r11));
  // stw r11,4(r11)
  guest::StoreU32(base, uint32_t(r11) + 4, uint32_t(r11));
  // bne cr6,0x82299c80
  if (!cr6.eq) goto loc_82299C80;
  // mr r3,r29
  r3 = r29;
  // mr r21,r29
  r21 = r29;
  // bl 0x825db954
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlInitializeCriticalSection, LIFTED_ARGS(0x82299C80, r31)); r3 = o.r3; f1 = o.f1; }
loc_82299C80:
  // mr r8,r28
  r8 = r28;
  // mr r6,r26
  r6 = r26;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r21,1408(r11)
  guest::StoreU32(base, uint32_t(r11) + 1408, uint32_t(r21));
  // lwz r7,84(r31)
  r7 = guest::LoadU32(base, uint32_t(r31) + 84);
  // lwz r11,308(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 308);
  // add r9,r7,r11
  r9 = r7 + r11;
  // lwz r3,80(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 80);
  // add r4,r30,r3
  r4 = r30 + r3;
  // bl 0x822990a0
  // (left out: dead in C++)
  { const lifted::Result o = f822990A0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82299868
  if (cr0.eq) goto loc_82299868;
  // rlwinm. r11,r24,0,15,15
  r11 = __builtin_rotateleft64(uint32_t(r24) | (r24 << 32), 0) & 0x10000;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // sth r22,56(r11)
  guest::StoreU16(base, uint32_t(r11) + 56, uint16_t(r22));
  // lwz r11,132(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 132);
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,32(r10)
  guest::StoreU32(base, uint32_t(r10) + 32, uint32_t(r11));
  // lwz r11,136(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 136);
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,36(r10)
  guest::StoreU32(base, uint32_t(r10) + 36, uint32_t(r11));
  // lwz r11,140(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 140);
  // rlwinm r11,r11,28,4,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0xFFFFFFF;
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,40(r10)
  guest::StoreU32(base, uint32_t(r10) + 40, uint32_t(r11));
  // lwz r11,144(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 144);
  // rlwinm r11,r11,28,4,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0xFFFFFFF;
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,44(r10)
  guest::StoreU32(base, uint32_t(r10) + 44, uint32_t(r11));
  // lwz r11,148(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 148);
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,52(r10)
  guest::StoreU32(base, uint32_t(r10) + 52, uint32_t(r11));
  // lwz r11,152(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 152);
  // addi r11,r11,15
  r11 = uint64_t(int64_t(int64_t(r11) + 15));
  // rlwinm r11,r11,28,4,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0xFFFFFFF;
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,28(r10)
  guest::StoreU32(base, uint32_t(r10) + 28, uint32_t(r11));
  // lwz r11,164(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 164);
  // lwz r10,80(r31)
  r10 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,1412(r10)
  guest::StoreU32(base, uint32_t(r10) + 1412, uint32_t(r11));
  // li r11,31
  r11 = uint64_t(int64_t(31));
  // li r10,-16
  r10 = uint64_t(int64_t(-16));
  // lwz r9,80(r31)
  r9 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r11,80(r9)
  guest::StoreU32(base, uint32_t(r9) + 80, uint32_t(r11));
  // lwz r11,80(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 80);
  // stw r10,84(r11)
  guest::StoreU32(base, uint32_t(r11) + 84, uint32_t(r10));
  // lwz r3,80(r31)
  r3 = guest::LoadU32(base, uint32_t(r31) + 80);
loc_82299D40:
  // addi r1,r31,272
  r1 = uint64_t(int64_t(int64_t(r31) + 272));
  // b 0x825a6ebc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8229D6F0: lifted from generated/ by lift.py.
void f8229D6F0(uint8_t* base, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8229D6F0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // stw r4,0(r3)
  guest::StoreU32(base, uint32_t(r3) + 0, uint32_t(r4));
  // stw r5,4(r3)
  guest::StoreU32(base, uint32_t(r3) + 4, uint32_t(r5));
  // lwz r11,256(r13)
  r11 = guest::LoadU32(base, uint32_t(r13) + 256);
  // lwz r10,10768(r4)
  r10 = guest::LoadU32(base, uint32_t(r4) + 10768);
  // lwz r11,88(r11)
  r11 = guest::LoadU32(base, uint32_t(r11) + 88);
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 0);
  // stw r11,12(r3)
  guest::StoreU32(base, uint32_t(r3) + 12, uint32_t(r11));
  // stw r10,8(r3)
  guest::StoreU32(base, uint32_t(r3) + 8, uint32_t(r10));
  // stw r11,16(r3)
  guest::StoreU32(base, uint32_t(r3) + 16, uint32_t(r11));
  // mftb r11
  r11 = helpers::PerformanceCounter();
  // stw r11,20(r3)
  guest::StoreU32(base, uint32_t(r3) + 20, uint32_t(r11));
  // blr
  return;
}

// sub_8229D720: lifted from generated/ by lift.py.
lifted::Result f8229D720(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8229D720 (lifted)");
  uint64_t r4 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lwz r11,4(r3)
  r11 = guest::LoadU32(base, uint32_t(r3) + 4);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8229d7d0
  if (cr6.eq) goto loc_8229D7D0;
  // mftb r11
  r11 = helpers::PerformanceCounter();
  // lwz r10,20(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 20);
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r9,16(r3)
  r9 = guest::LoadU32(base, uint32_t(r3) + 16);
  // lwz r8,4(r3)
  r8 = guest::LoadU32(base, uint32_t(r3) + 4);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // lwz r10,256(r13)
  r10 = guest::LoadU32(base, uint32_t(r13) + 256);
  // cmpwi cr6,r8,3
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 3, so);
  // lwz r10,88(r10)
  r10 = guest::LoadU32(base, uint32_t(r10) + 88);
  // subf r6,r9,r10
  r6 = r10 - r9;
  // lwz r10,0(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 0);
  // clrldi r9,r11,32
  r9 = r11 & 0xFFFFFFFF;
  // bne cr6,0x8229d77c
  if (!cr6.eq) goto loc_8229D77C;
  // ld r8,20056(r10)
  r8 = guest::LoadU64(base, uint32_t(r10) + 20056);
  // add r9,r9,r8
  r9 = r9 + r8;
  // std r9,20056(r10)
  guest::StoreU64(base, uint32_t(r10) + 20056, r9);
  // b 0x8229d788
  goto loc_8229D788;
loc_8229D77C:
  // ld r8,20048(r10)
  r8 = guest::LoadU64(base, uint32_t(r10) + 20048);
  // add r9,r9,r8
  r9 = r9 + r8;
  // std r9,20048(r10)
  guest::StoreU64(base, uint32_t(r10) + 20048, r9);
loc_8229D788:
  // lwz r10,0(r3)
  r10 = guest::LoadU32(base, uint32_t(r3) + 0);
  // lwz r9,13084(r10)
  r9 = guest::LoadU32(base, uint32_t(r10) + 13084);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8229d7d0
  if (cr0.eq) goto loc_8229D7D0;
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // lfs f0,20044(r10)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(base, uint32_t(r10) + 20044);
  f0 = double(temp.f32);
  // lwz r4,4(r3)
  r4 = guest::LoadU32(base, uint32_t(r3) + 4);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // std r11,80(r1)
  guest::StoreU64(base, uint32_t(r1) + 80, r11);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(base, uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f13,f0,f13
  f13 = double(float(f0 * f13));
  // lfs f0,7132(r11)
  temp.u32 = guest::LoadU32(base, uint32_t(r11) + 7132);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // mtctr r9
  ctr = r9;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8229D7D0, r31)); r3 = o.r3; f1 = o.f1; }
loc_8229D7D0:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = guest::LoadU32(base, uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_822AED18 game::Device::LoadShaders: lifted from generated/ by lift.py.
lifted::Result f822AED18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r9, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_822AED18 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r10 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, reserved = 0, msr = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e68
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  guest::StoreU32(base, ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r21,r4
  r21 = r4;
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // lwz r30,12436(r31)
  r30 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).vertex_shader_address());
  // lwz r25,12432(r31)
  r25 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).pixel_shader_address());
  // lwz r29,11552(r31)
  r29 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).vertex_declaration_address());
  // cmplwi r25,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // lwz r11,612(r30)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r30)).render_state_getters_address() + 92);
  // add r11,r11,r30
  r11 = r11 + r30;
  // addi r8,r11,592
  r8 = uint64_t(int64_t(int64_t(r11) + 592));
  // beq 0x822aed5c
  if (cr0.eq) goto loc_822AED5C;
  // lwz r11,76(r25)
  r11 = guest::LoadU32(base, uint32_t(r25) + 76);
  // add r11,r11,r25
  r11 = r11 + r25;
  // addi r24,r11,56
  r24 = uint64_t(int64_t(int64_t(r11) + 56));
loc_822AED5C:
  // lwz r23,0(r8)
  r23 = guest::LoadU32(base, uint32_t(r8) + 0);
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // lwz r20,4(r8)
  r20 = guest::LoadU32(base, uint32_t(r8) + 4);
  // stw r23,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r23));
  // beq cr6,0x822aed84
  if (cr6.eq) goto loc_822AED84;
  // lwz r11,16(r24)
  r11 = guest::LoadU32(base, uint32_t(r24) + 16);
  // lwz r10,16(r8)
  r10 = guest::LoadU32(base, uint32_t(r8) + 16);
  // xor r11,r11,r10
  r11 = r11 ^ r10;
  // clrlwi r11,r11,13
  r11 = uint32_t(r11) & 0x7FFFF;
  // b 0x822aed8c
  goto loc_822AED8C;
loc_822AED84:
  // lwz r11,16(r8)
  r11 = guest::LoadU32(base, uint32_t(r8) + 16);
  // clrlwi r11,r11,27
  r11 = uint32_t(r11) & 0x1F;
loc_822AED8C:
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // lis r10,-16383
  r10 = uint64_t(int64_t(-1073676288));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // ori r26,r10,9984
  r26 = r10 | 9984;
  // xori r4,r11,1
  r4 = r11 ^ 1;
  // lbz r11,10810(r31)
  r11 = guest::LoadU8(base, schema::Device(base, uint32_t(r31)).flags2_address());
  // li r22,-1
  r22 = uint64_t(int64_t(-1));
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822aedf4
  if (!cr0.eq) goto loc_822AEDF4;
  // cmpwi cr6,r4,0
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bne cr6,0x822aedf4
  if (!cr6.eq) goto loc_822AEDF4;
  // li r12,1
  r12 = uint64_t(int64_t(1));
  // rldicr r12,r12,51,63
  r12 = __builtin_rotateleft64(r12, 51) & 0xFFFFFFFFFFFFFFFF;
  // and r11,r21,r12
  r11 = r21 & r12;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x822aee10
  if (cr6.eq) goto loc_822AEE10;
  // lwz r27,48(r30)
  r27 = guest::LoadU32(base, schema::Device(base, uint32_t(r30)).soft_end_address());
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x822aef0c
  if (cr6.eq) goto loc_822AEF0C;
  // lwz r11,592(r30)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r30)).render_state_getters_address() + 72);
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822aef0c
  if (!cr0.eq) goto loc_822AEF0C;
  // lbz r11,10808(r31)
  r11 = guest::LoadU8(base, schema::Device(base, uint32_t(r31)).tiling_flags_address());
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822aee2c
  if (cr0.eq) goto loc_822AEE2C;
loc_822AEDF0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_822AEDF4:
  // mr r9,r24
  r9 = r24;
  // mr r7,r29
  r7 = r29;
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r5,r30
  r5 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x822aebf0
  // (left out: dead in C++)
  { const lifted::Result o = f822AEBF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lwz r23,80(r1)
  r23 = guest::LoadU32(base, uint32_t(r1) + 80);
loc_822AEE10:
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // bne cr6,0x822aefcc
  if (!cr6.eq) goto loc_822AEFCC;
  // lwz r11,10452(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).rb_modecontrol_address());
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // ori r21,r21,8
  r21 = r21 | 8;
  // rlwimi r11,r10,0,29,31
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x7) | (r11 & 0xFFFFFFFFFFFFFFF8);
  // b 0x822af0e8
  goto loc_822AF0E8;
loc_822AEE2C:
  // lwz r11,48(r29)
  r11 = guest::LoadU32(base, schema::VertexDeclaration(base, uint32_t(r29)).unknown_48_address());
  // lwz r10,24(r30)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r30)).render_dirty_high_address());
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x822aee70
  if (!cr6.eq) goto loc_822AEE70;
  // ld r10,40(r30)
  r10 = guest::LoadU64(base, schema::Device(base, uint32_t(r30)).write_position_and_end_address());
  // ld r11,12280(r31)
  r11 = guest::LoadU64(base, schema::Device(base, uint32_t(r31)).stream_strides_8_15_address());
  // ld r9,12272(r31)
  r9 = guest::LoadU64(base, schema::Device(base, uint32_t(r31)).stream_strides_0_7_address());
  // ld r7,32(r30)
  r7 = guest::LoadU64(base, schema::Device(base, uint32_t(r30)).other_dirty_address());
  // xor r11,r11,r10
  r11 = r11 ^ r10;
  // ld r10,40(r29)
  r10 = guest::LoadU64(base, schema::VertexDeclaration(base, uint32_t(r29)).stream_map_address() + 8);
  // ld r6,32(r29)
  r6 = guest::LoadU64(base, schema::VertexDeclaration(base, uint32_t(r29)).stream_map_address());
  // xor r9,r9,r7
  r9 = r9 ^ r7;
  // and r11,r11,r10
  r11 = r11 & r10;
  // and r10,r9,r6
  r10 = r9 & r6;
  // or r11,r11,r10
  r11 = r11 | r10;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x822aef0c
  if (cr6.eq) goto loc_822AEF0C;
loc_822AEE70:
  // lwz r9,8(r30)
  r9 = guest::LoadU32(base, schema::Device(base, uint32_t(r30)).pixel_float_dirty_high_address());
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x822aee98
  if (cr0.eq) goto loc_822AEE98;
  // lwz r10,10768(r31)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).progress_block_address());
  // lwz r11,10780(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).fence_address());
  // subf r9,r9,r11
  r9 = r11 - r9;
  // lwz r10,0(r10)
  r10 = guest::LoadU32(base, schema::CommandProgressBlock(base, uint32_t(r10)).fence_done_address());
  // subf r11,r10,r11
  r11 = r11 - r10;
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // blt cr6,0x822aedf0
  if (cr6.lt) goto loc_822AEDF0;
loc_822AEE98:
  // addi r28,r31,12272
  r28 = uint64_t(int64_t(int64_t(r31) + 12272));
  // mr r5,r29
  r5 = r29;
  // mr r4,r27
  r4 = r27;
  // mr r3,r30
  r3 = r30;
  // mr r6,r28
  r6 = r28;
  // bl 0x822ae340
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::PatchVertexShader(base, LIFTED_ARGS(0x822AEEB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,48(r29)
  r11 = guest::LoadU32(base, schema::VertexDeclaration(base, uint32_t(r29)).unknown_48_address());
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x822aeef4
  if (!cr6.eq) goto loc_822AEEF4;
  // lis r11,-32107
  r11 = uint64_t(int64_t(-2104164352));
  // addi r10,r11,14320
  r10 = uint64_t(int64_t(int64_t(r11) + 14320));
loc_822AEEC4:
  // mr r8,r10
  r8 = r10;
loc_822AEEC8:
  // mfmsr r9
  std::atomic_thread_fence(std::memory_order_seq_cst);
  r9 = lifted::CheckGlobalLock();
  // mtmsrd r13,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r13) & 0x8020) | (msr & ~0x8020);
  lifted::EnterGlobalLock();
  // lwarx r11,0,r8
  ea = uint32_t(r8);
  reserved = (reserved & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(*(uint32_t*)guest::Pointer(base, ea)));
  r11 = __builtin_bswap32(uint32_t(reserved));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stwcx. r11,0,r8
  ea = uint32_t(r8);
  cr0.lt = 0;
  cr0.gt = 0;
  cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(guest::Pointer(base, ea)), int32_t(reserved), __builtin_bswap32(int32_t(r11)));
  cr0.so = so;
  // mtmsrd r9,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r9) & 0x8020) | (msr & ~0x8020);
  lifted::LeaveGlobalLock();
  // bne 0x822aeec8
  if (!cr0.eq) goto loc_822AEEC8;
  // addi r9,r11,1
  r9 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r9,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 2, so);
  // blt cr6,0x822aeec4
  if (cr6.lt) goto loc_822AEEC4;
  // stw r11,48(r29)
  guest::StoreU32(base, schema::VertexDeclaration(base, uint32_t(r29)).unknown_48_address(), uint32_t(r11));
loc_822AEEF4:
  // lwz r11,48(r29)
  r11 = guest::LoadU32(base, schema::VertexDeclaration(base, uint32_t(r29)).unknown_48_address());
  // stw r11,24(r30)
  guest::StoreU32(base, schema::Device(base, uint32_t(r30)).render_dirty_high_address(), uint32_t(r11));
  // ld r11,0(r28)
  r11 = guest::LoadU64(base, schema::Device(base, uint32_t(r28) - 12272).stream_strides_0_7_address());
  // std r11,32(r30)
  guest::StoreU64(base, schema::Device(base, uint32_t(r30)).other_dirty_address(), r11);
  // ld r11,12280(r31)
  r11 = guest::LoadU64(base, schema::Device(base, uint32_t(r31)).stream_strides_8_15_address());
  // std r11,40(r30)
  guest::StoreU64(base, schema::Device(base, uint32_t(r30)).write_position_and_end_address(), r11);
loc_822AEF0C:
  // lwz r10,48(r31)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).soft_end_address());
  // lwz r11,40(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).write_position_address());
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822aef28
  if (!cr6.gt) goto loc_822AEF28;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::EndSegment(base, LIFTED_ARGS(0x822AEF24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
loc_822AEF28:
  // rlwinm r10,r27,12,20,31
  r10 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 12) & 0xFFF;
  // mr r8,r26
  r8 = r26;
  // addi r10,r10,512
  r10 = uint64_t(int64_t(int64_t(r10) + 512));
  // clrlwi r9,r27,3
  r9 = uint32_t(r27) & 0x1FFFFFFF;
  // rlwinm r10,r10,0,19,19
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x1000;
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // add r10,r10,r9
  r10 = r10 + r9;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,600(r30)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r30)).render_state_getters_address() + 80);
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,10780(r31)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).fence_address());
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,40(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).write_position_address(), uint32_t(r11));
  // beq 0x822aef6c
  if (cr0.eq) goto loc_822AEF6C;
  // stw r10,8(r30)
  guest::StoreU32(base, schema::Device(base, uint32_t(r30)).pixel_float_dirty_high_address(), uint32_t(r10));
  // b 0x822aefb8
  goto loc_822AEFB8;
loc_822AEF6C:
  // lwz r11,10784(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).fence_mask_address());
  // lwz r10,0(r30)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r30)).vertex_float_dirty_high_address());
  // and. r11,r11,r10
  r11 = r11 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822aefb8
  if (cr0.eq) goto loc_822AEFB8;
  // lwz r11,13172(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).fence_list_end_address());
  // lwz r3,13168(r31)
  r3 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).fence_list_address());
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // blt cr6,0x822aef94
  if (cr6.lt) goto loc_822AEF94;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ad790
  // (left out: dead in C++)
  { const lifted::Result o = f822AD790(base, r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_822AEF94:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // addi r10,r3,8
  r10 = uint64_t(int64_t(int64_t(r3) + 8));
  // stw r22,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r22));
  // rlwimi r11,r30,30,2,31
  r11 = (__builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 30) & 0x3FFFFFFF) | (r11 & 0xFFFFFFFFC0000000);
  // rlwinm r11,r11,0,2,0
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFBFFFFFFF;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // ld r11,80(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 80);
  // std r11,0(r3)
  guest::StoreU64(base, schema::DeviceFenceEntry(base, uint32_t(r3)).entry_address(), r11);
  // stw r10,13168(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).fence_list_address(), uint32_t(r10));
loc_822AEFB8:
  // ld r11,12272(r31)
  r11 = guest::LoadU64(base, schema::Device(base, uint32_t(r31)).stream_strides_0_7_address());
  // ld r10,12280(r31)
  r10 = guest::LoadU64(base, schema::Device(base, uint32_t(r31)).stream_strides_8_15_address());
  // std r11,11568(r31)
  guest::StoreU64(base, schema::Device(base, uint32_t(r31)).shader_strides_0_7_address(), r11);
  // std r10,11576(r31)
  guest::StoreU64(base, schema::Device(base, uint32_t(r31)).shader_strides_8_15_address(), r10);
  // b 0x822aee10
  goto loc_822AEE10;
loc_822AEFCC:
  // lwz r9,0(r24)
  r9 = guest::LoadU32(base, uint32_t(r24) + 0);
  // lis r10,4096
  r10 = uint64_t(int64_t(268435456));
  // lbz r11,10810(r31)
  r11 = guest::LoadU8(base, schema::Device(base, uint32_t(r31)).flags2_address());
  // li r12,1
  r12 = uint64_t(int64_t(1));
  // or r23,r9,r23
  r23 = r9 | r23;
  // lwz r7,4(r24)
  r7 = guest::LoadU32(base, uint32_t(r24) + 4);
  // rldicr r12,r12,52,63
  r12 = __builtin_rotateleft64(r12, 52) & 0xFFFFFFFFFFFFFFFF;
  // rlwinm r9,r23,0,1,3
  r9 = __builtin_rotateleft64(uint32_t(r23) | (r23 << 32), 0) & 0x70000000;
  // and r8,r21,r12
  r8 = r21 & r12;
  // subfc r10,r9,r10
  ca = uint32_t(r10) >= uint32_t(r9);
  r10 = r10 - r9;
  // or r20,r7,r20
  r20 = r7 | r20;
  // subfe r10,r10,r10
  temp.u8 = (~uint32_t(r10) + uint32_t(r10) < ~uint32_t(r10)) | (~uint32_t(r10) + uint32_t(r10) + ca < ca);
  r10 = ~r10 + r10 + ca;
  ca = temp.u8;
  // cmpldi cr6,r8,0
  cr6 = lifted::Compare<uint64_t>(r8, 0, so);
  // rlwinm r10,r10,4,27,27
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 4) & 0x10;
  // or r10,r10,r11
  r10 = r10 | r11;
  // rlwimi r11,r10,0,27,27
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x10) | (r11 & 0xFFFFFFFFFFFFFFEF);
  // stb r11,10810(r31)
  guest::StoreU8(base, schema::Device(base, uint32_t(r31)).flags2_address(), uint8_t(r11));
  // beq cr6,0x822af0ec
  if (cr6.eq) goto loc_822AF0EC;
  // lwz r10,48(r31)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).soft_end_address());
  // lwz r11,40(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).write_position_address());
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822af030
  if (!cr6.gt) goto loc_822AF030;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::EndSegment(base, LIFTED_ARGS(0x822AF02C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
loc_822AF030:
  // stwu r26,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r26));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,24(r25)
  r10 = guest::LoadU32(base, uint32_t(r25) + 24);
  // rlwinm r8,r10,12,20,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 12) & 0xFFF;
  // rlwinm r9,r10,0,3,30
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x1FFFFFFE;
  // addi r10,r8,512
  r10 = uint64_t(int64_t(int64_t(r8) + 512));
  // rlwinm r10,r10,0,19,19
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x1000;
  // add r10,r10,r9
  r10 = r10 + r9;
  // ori r10,r10,1
  r10 = r10 | 1;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,64(r25)
  r10 = guest::LoadU32(base, uint32_t(r25) + 64);
  // rlwinm r10,r10,30,2,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3FFFFFFF;
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,10780(r31)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).fence_address());
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,40(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).write_position_address(), uint32_t(r11));
  // beq 0x822af078
  if (cr0.eq) goto loc_822AF078;
  // stw r10,8(r25)
  guest::StoreU32(base, uint32_t(r25) + 8, uint32_t(r10));
  // b 0x822af0c4
  goto loc_822AF0C4;
loc_822AF078:
  // lwz r11,10784(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).fence_mask_address());
  // lwz r10,0(r25)
  r10 = guest::LoadU32(base, uint32_t(r25) + 0);
  // and. r11,r11,r10
  r11 = r11 & r10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822af0c4
  if (cr0.eq) goto loc_822AF0C4;
  // lwz r11,13172(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).fence_list_end_address());
  // lwz r3,13168(r31)
  r3 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).fence_list_address());
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // blt cr6,0x822af0a0
  if (cr6.lt) goto loc_822AF0A0;
  // mr r3,r31
  r3 = r31;
  // bl 0x822ad790
  // (left out: dead in C++)
  { const lifted::Result o = f822AD790(base, r1, r13, r3, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_822AF0A0:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // addi r10,r3,8
  r10 = uint64_t(int64_t(int64_t(r3) + 8));
  // stw r22,84(r1)
  guest::StoreU32(base, uint32_t(r1) + 84, uint32_t(r22));
  // rlwimi r11,r25,30,2,31
  r11 = (__builtin_rotateleft64(uint32_t(r25) | (r25 << 32), 30) & 0x3FFFFFFF) | (r11 & 0xFFFFFFFFC0000000);
  // rlwinm r11,r11,0,2,0
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFBFFFFFFF;
  // stw r11,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r11));
  // ld r11,80(r1)
  r11 = guest::LoadU64(base, uint32_t(r1) + 80);
  // std r11,0(r3)
  guest::StoreU64(base, schema::DeviceFenceEntry(base, uint32_t(r3)).entry_address(), r11);
  // stw r10,13168(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).fence_list_address(), uint32_t(r10));
loc_822AF0C4:
  // lwz r11,24(r24)
  r11 = guest::LoadU32(base, uint32_t(r24) + 24);
  // lis r12,32
  r12 = uint64_t(int64_t(2097152));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // ori r12,r12,7
  r12 = r12 | 7;
  // rldicr r12,r12,46,63
  r12 = __builtin_rotateleft64(r12, 46) & 0xFFFFFFFFFFFFFFFF;
  // stw r11,10408(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).sq_interpolator_cntl_address(), uint32_t(r11));
  // lwz r11,10452(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).rb_modecontrol_address());
  // or r21,r21,r12
  r21 = r21 | r12;
  // rlwimi r11,r10,2,29,31
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0x7) | (r11 & 0xFFFFFFFFFFFFFFF8);
loc_822AF0E8:
  // stw r11,10452(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).rb_modecontrol_address(), uint32_t(r11));
loc_822AF0EC:
  // li r12,3
  r12 = uint64_t(int64_t(3));
  // stw r23,10400(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).sq_program_cntl_address(), uint32_t(r23));
  // stw r20,10404(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).sq_context_misc_address(), uint32_t(r20));
  // rldicr r12,r12,47,16
  r12 = __builtin_rotateleft64(r12, 47) & 0xFFFF800000000000;
  // or r28,r21,r12
  r28 = r21 | r12;
  // li r12,1
  r12 = uint64_t(int64_t(1));
  // rldicr r12,r12,49,63
  r12 = __builtin_rotateleft64(r12, 49) & 0xFFFFFFFFFFFFFFFF;
  // and r11,r28,r12
  r11 = r28 & r12;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x822af1cc
  if (cr6.eq) goto loc_822AF1CC;
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // beq cr6,0x822af128
  if (cr6.eq) goto loc_822AF128;
  // lwz r11,0(r24)
  r11 = guest::LoadU32(base, uint32_t(r24) + 0);
  // rlwinm r4,r11,5,31,31
  r4 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0x1;
  // b 0x822af12c
  goto loc_822AF12C;
loc_822AF128:
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_822AF12C:
  // lbz r11,10810(r31)
  r11 = guest::LoadU8(base, schema::Device(base, uint32_t(r31)).flags2_address());
  // mr r3,r31
  r3 = r31;
  // lwz r10,10420(r31)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).rb_depthcontrol_address());
  // rlwinm r11,r11,26,6,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 26) & 0x3FFFFFF;
  // rlwinm r10,r10,31,1,31
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 31) & 0x7FFFFFFF;
  // and r30,r11,r10
  r30 = r11 & r10;
  // lwz r11,10432(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).rb_hizcontrol_address());
  // rlwimi r30,r11,0,0,30
  r30 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFE) | (r30 & 0xFFFFFFFF00000001);
  // stw r30,10432(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).rb_hizcontrol_address(), uint32_t(r30));
  // bl 0x822ada98
  // (left out: dead in C++)
  r3 = f822ADA98(base, r3, r4);
  // rlwimi r30,r3,1,30,30
  r30 = (__builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 1) & 0x2) | (r30 & 0xFFFFFFFFFFFFFFFD);
  // stw r30,10432(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).rb_hizcontrol_address(), uint32_t(r30));
  // rlwinm. r11,r30,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x2;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822af1ac
  if (cr0.eq) goto loc_822AF1AC;
  // lbz r11,10809(r31)
  r11 = guest::LoadU8(base, schema::Device(base, uint32_t(r31)).flags_address());
  // rlwinm. r10,r11,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x822af1c8
  if (cr0.eq) goto loc_822AF1C8;
  // andi. r11,r11,251
  r11 = r11 & 251;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r10,48(r31)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).soft_end_address());
  // lwz r3,40(r31)
  r3 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).write_position_address());
  // cmplw cr6,r3,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r10), so);
  // stb r11,10809(r31)
  guest::StoreU8(base, schema::Device(base, uint32_t(r31)).flags_address(), uint8_t(r11));
  // ble cr6,0x822af190
  if (!cr6.gt) goto loc_822AF190;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::EndSegment(base, LIFTED_ARGS(0x822AF190, r31)); r3 = o.r3; f1 = o.f1; }
loc_822AF190:
  // lis r11,-16384
  r11 = uint64_t(int64_t(-1073741824));
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // ori r11,r11,17920
  r11 = r11 | 17920;
  // stwu r11,4(r3)
  ea = 4 + uint32_t(r3);
  guest::StoreU32(base, ea, uint32_t(r11));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r10,4(r3)
  ea = 4 + uint32_t(r3);
  guest::StoreU32(base, ea, uint32_t(r10));
  r3 = (r3 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stw r3,40(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).write_position_address(), uint32_t(r3));
  // b 0x822af1c8
  goto loc_822AF1C8;
loc_822AF1AC:
  // mr r3,r31
  r3 = r31;
  // bl 0x822ada28
  // (left out: dead in C++)
  r3 = f822ADA28(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x822af1c8
  if (cr0.eq) goto loc_822AF1C8;
  // lbz r11,10809(r31)
  r11 = guest::LoadU8(base, schema::Device(base, uint32_t(r31)).flags_address());
  // ori r11,r11,4
  r11 = r11 | 4;
  // stb r11,10809(r31)
  guest::StoreU8(base, schema::Device(base, uint32_t(r31)).flags_address(), uint8_t(r11));
loc_822AF1C8:
  // ori r28,r28,256
  r28 = r28 | 256;
loc_822AF1CC:
  // lbz r11,10808(r31)
  r11 = guest::LoadU8(base, schema::Device(base, uint32_t(r31)).tiling_flags_address());
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822af2cc
  if (cr0.eq) goto loc_822AF2CC;
  // lwz r29,10452(r31)
  r29 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).rb_modecontrol_address());
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // clrlwi r30,r29,29
  r30 = uint32_t(r29) & 0x7;
  // beq cr6,0x822af208
  if (cr6.eq) goto loc_822AF208;
  // lwz r11,10428(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).rb_colorcontrol_address());
  // rlwinm. r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822af20c
  if (!cr0.eq) goto loc_822AF20C;
  // lwz r11,28(r24)
  r11 = guest::LoadU32(base, uint32_t(r24) + 28);
  // rlwinm. r10,r11,0,27,27
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x822af20c
  if (!cr0.eq) goto loc_822AF20C;
  // rlwinm. r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x822af20c
  if (!cr0.eq) goto loc_822AF20C;
loc_822AF208:
  // li r30,5
  r30 = uint64_t(int64_t(5));
loc_822AF20C:
  // lwz r11,12440(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).shader_word_address());
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // bne cr6,0x822af224
  if (!cr6.eq) goto loc_822AF224;
  // rlwinm r11,r28,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0x8;
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x822af2cc
  if (cr6.eq) goto loc_822AF2CC;
loc_822AF224:
  // lwz r10,48(r31)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).soft_end_address());
  // lwz r11,40(r31)
  r11 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).write_position_address());
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x822af240
  if (!cr6.gt) goto loc_822AF240;
  // mr r3,r31
  r3 = r31;
  // bl 0x8229ef48
  // (left out: dead in C++)
  { const lifted::Result o = game::Device::EndSegment(base, LIFTED_ARGS(0x822AF23C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
loc_822AF240:
  // li r10,8712
  r10 = uint64_t(int64_t(8712));
  // lis r9,-16384
  r9 = uint64_t(int64_t(-1073741824));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // ori r8,r9,24576
  r8 = r9 | 24576;
  // lis r9,-16384
  r9 = uint64_t(int64_t(-1073741824));
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r7,r8
  r7 = r8;
  // lis r10,5461
  r10 = uint64_t(int64_t(357892096));
  // ori r9,r9,24832
  r9 = r9 | 24832;
  // ori r6,r10,21845
  r6 = r10 | 21845;
  // mr r5,r9
  r5 = r9;
  // stwu r29,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r29));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-16383
  r10 = uint64_t(int64_t(-1073676288));
  // stw r30,12440(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).shader_word_address(), uint32_t(r30));
  // lis r3,4
  r3 = uint64_t(int64_t(262144));
  // ori r27,r10,11521
  r27 = r10 | 11521;
  // ori r3,r3,520
  r3 = r3 | 520;
  // mr r10,r30
  r10 = r30;
  // stwu r7,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r7));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r12,-9
  r12 = uint64_t(int64_t(-9));
  // rlwimi r10,r29,0,0,28
  r10 = (__builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 0) & 0xFFFFFFF8) | (r10 & 0xFFFFFFFF00000007);
  // and r28,r28,r12
  r28 = r28 & r12;
  // stwu r6,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r6));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r5,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r5));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r4,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r4));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r27,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r27));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r3,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r3));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r8,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r8));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,12448(r31)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).bin_mask_address());
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stwu r9,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r9));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r10,12452(r31)
  r10 = guest::LoadU32(base, schema::Device(base, uint32_t(r31)).bin_mask_high_address());
  // stwu r10,4(r11)
  ea = 4 + uint32_t(r11);
  guest::StoreU32(base, ea, uint32_t(r10));
  r11 = (r11 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // stw r11,40(r31)
  guest::StoreU32(base, schema::Device(base, uint32_t(r31)).write_position_address(), uint32_t(r11));
loc_822AF2CC:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A6BD0: lifted from generated/ by lift.py.
lifted::Result f825A6BD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_825A6BD0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, reserved = 0, msr = 0;
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
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // mr r29,r3
  r29 = r3;
  // addi r8,r11,-19480
  r8 = uint64_t(int64_t(int64_t(r11) + -19480));
loc_825A6BE8:
  // mfmsr r9
  std::atomic_thread_fence(std::memory_order_seq_cst);
  r9 = lifted::CheckGlobalLock();
  // mtmsrd r13,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r13) & 0x8020) | (msr & ~0x8020);
  lifted::EnterGlobalLock();
  // lwarx r10,0,r8
  ea = uint32_t(r8);
  reserved = (reserved & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(*(uint32_t*)guest::Pointer(base, ea)));
  r10 = __builtin_bswap32(uint32_t(reserved));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stwcx. r10,0,r8
  ea = uint32_t(r8);
  cr0.lt = 0;
  cr0.gt = 0;
  cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(guest::Pointer(base, ea)), int32_t(reserved), __builtin_bswap32(int32_t(r10)));
  cr0.so = so;
  // mtmsrd r9,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r9) & 0x8020) | (msr & ~0x8020);
  lifted::LeaveGlobalLock();
  // bne 0x825a6be8
  if (!cr0.eq) goto loc_825A6BE8;
  // mr r11,r10
  r11 = r10;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825a6c34
  if (!cr6.eq) goto loc_825A6C34;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r30,r11,4112
  r30 = uint64_t(int64_t(int64_t(r11) + 4112));
  // mr r31,r30
  r31 = r30;
loc_825A6C1C:
  // mr r3,r31
  r3 = r31;
  // bl 0x825a6d20
  // (left out: dead in C++)
  { const lifted::Result o = f825A6D20(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x825A6C24); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,28
  r31 = uint64_t(int64_t(int64_t(r31) + 28));
  // addi r11,r30,112
  r11 = uint64_t(int64_t(int64_t(r30) + 112));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x825a6c1c
  if (cr6.lt) goto loc_825A6C1C;
loc_825A6C34:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825A6C40: lifted from generated/ by lift.py.
uint64_t f825A6C40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825A6C40 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, reserved = 0, msr = 0;
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
  // lis r11,-32153
  r11 = uint64_t(int64_t(-2107179008));
  // addi r8,r11,-19480
  r8 = uint64_t(int64_t(int64_t(r11) + -19480));
loc_825A6C5C:
  // mfmsr r9
  std::atomic_thread_fence(std::memory_order_seq_cst);
  r9 = lifted::CheckGlobalLock();
  // mtmsrd r13,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r13) & 0x8020) | (msr & ~0x8020);
  lifted::EnterGlobalLock();
  // lwarx r10,0,r8
  ea = uint32_t(r8);
  reserved = (reserved & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(*(uint32_t*)guest::Pointer(base, ea)));
  r10 = __builtin_bswap32(uint32_t(reserved));
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stwcx. r10,0,r8
  ea = uint32_t(r8);
  cr0.lt = 0;
  cr0.gt = 0;
  cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(guest::Pointer(base, ea)), int32_t(reserved), __builtin_bswap32(int32_t(r10)));
  cr0.so = so;
  // mtmsrd r9,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r9) & 0x8020) | (msr & ~0x8020);
  lifted::LeaveGlobalLock();
  // bne 0x825a6c5c
  if (!cr0.eq) goto loc_825A6C5C;
  // mr r11,r10
  r11 = r10;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x825a6ca8
  if (!cr6.lt) goto loc_825A6CA8;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r30,r11,4112
  r30 = uint64_t(int64_t(int64_t(r11) + 4112));
  // mr r31,r30
  r31 = r30;
loc_825A6C90:
  // mr r3,r31
  r3 = r31;
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x825A6C98, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,28
  r31 = uint64_t(int64_t(int64_t(r31) + 28));
  // addi r11,r30,112
  r11 = uint64_t(int64_t(int64_t(r30) + 112));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x825a6c90
  if (cr6.lt) goto loc_825A6C90;
loc_825A6CA8:
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

// sub_825BFEA0 game::XACTEngine::CheckNotInCallback: lifted from generated/ by lift.py.
uint64_t f825BFEA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825BFEA0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, reserved = 0, msr = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x825b9a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825B9A60(base, LIFTED_ARGS(0x825BFEB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,20(r31)
  r11 = guest::LoadU32(base, uint32_t(r31) + 20);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r3,80(r1)
  guest::StoreU32(base, uint32_t(r1) + 80, uint32_t(r3));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
loc_825BFEC8:
  // mfmsr r8
  std::atomic_thread_fence(std::memory_order_seq_cst);
  r8 = lifted::CheckGlobalLock();
  // mtmsrd r13,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r13) & 0x8020) | (msr & ~0x8020);
  lifted::EnterGlobalLock();
  // lwarx r9,0,r10
  ea = uint32_t(r10);
  reserved = (reserved & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(*(uint32_t*)guest::Pointer(base, ea)));
  r9 = __builtin_bswap32(uint32_t(reserved));
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // bne cr6,0x825bfeec
  if (!cr6.eq) goto loc_825BFEEC;
  // stwcx. r7,0,r10
  ea = uint32_t(r10);
  cr0.lt = 0;
  cr0.gt = 0;
  cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(guest::Pointer(base, ea)), int32_t(reserved), __builtin_bswap32(int32_t(r7)));
  cr0.so = so;
  // mtmsrd r8,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r8) & 0x8020) | (msr & ~0x8020);
  lifted::LeaveGlobalLock();
  // bne 0x825bfec8
  if (!cr0.eq) goto loc_825BFEC8;
  // b 0x825bfef4
  goto loc_825BFEF4;
loc_825BFEEC:
  // stwcx. r9,0,r10
  ea = uint32_t(r10);
  cr0.lt = 0;
  cr0.gt = 0;
  cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(guest::Pointer(base, ea)), int32_t(reserved), __builtin_bswap32(int32_t(r9)));
  cr0.so = so;
  // mtmsrd r8,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r8) & 0x8020) | (msr & ~0x8020);
  lifted::LeaveGlobalLock();
loc_825BFEF4:
  // lwz r11,80(r1)
  r11 = guest::LoadU32(base, uint32_t(r1) + 80);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x825bff1c
  if (!cr6.eq) goto loc_825BFF1C;
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,18
  r3 = r3 | 18;
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
loc_825BFF1C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
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

// sub_825D1C50 game::XACTWave::RequestNotify: lifted from generated/ by lift.py.
void f825D1C50(uint8_t* base, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D1C50 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, reserved = 0, msr = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  uint32_t ea{};
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r3,28
  r10 = uint64_t(int64_t(int64_t(r3) + 28));
loc_825D1C58:
  // mfmsr r8
  std::atomic_thread_fence(std::memory_order_seq_cst);
  r8 = lifted::CheckGlobalLock();
  // mtmsrd r13,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r13) & 0x8020) | (msr & ~0x8020);
  lifted::EnterGlobalLock();
  // lwarx r9,0,r10
  ea = uint32_t(r10);
  reserved = (reserved & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(*(uint32_t*)guest::Pointer(base, ea)));
  r9 = __builtin_bswap32(uint32_t(reserved));
  // stwcx. r11,0,r10
  ea = uint32_t(r10);
  cr0.lt = 0;
  cr0.gt = 0;
  cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(guest::Pointer(base, ea)), int32_t(reserved), __builtin_bswap32(int32_t(r11)));
  cr0.so = so;
  // mtmsrd r8,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r8) & 0x8020) | (msr & ~0x8020);
  lifted::LeaveGlobalLock();
  // bne 0x825d1c58
  if (!cr0.eq) goto loc_825D1C58;
  // blr
  return;
}

lifted::Result e82121760(uint8_t* base, const lifted::Args& a) {
  return {f82121760(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]), a.f[0]};
}

lifted::Result e822997B0(uint8_t* base, const lifted::Args& a) {
  return f822997B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8229D6F0(uint8_t* base, const lifted::Args& a) {
  f8229D6F0(base, a.r13, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8229D720(uint8_t* base, const lifted::Args& a) {
  return f8229D720(base, a.r1, a.r13, a.r[0], a.r[2], a.r[4], a.f[0]);
}

lifted::Result e822AED18(uint8_t* base, const lifted::Args& a) {
  return f822AED18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[6], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e825A6BD0(uint8_t* base, const lifted::Args& a) {
  return f825A6BD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e825A6C40(uint8_t* base, const lifted::Args& a) {
  return {f825A6C40(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825BFEA0(uint8_t* base, const lifted::Args& a) {
  return {f825BFEA0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825D1C50(uint8_t* base, const lifted::Args& a) {
  f825D1C50(base, a.r13, a.r[0]);
  return {a.r[0], a.f[0]};
}

namespace {

const Registration kEntries[] = {
    {0x82121760, e82121760},
    {0x822997B0, e822997B0},
    {0x8229D6F0, e8229D6F0},
    {0x8229D720, e8229D720},
    {0x822AED18, e822AED18},
    {0x825A6BD0, e825A6BD0},
    {0x825A6C40, e825A6C40},
    {0x825BFEA0, e825BFEA0},
    {0x825D1C50, e825D1C50},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_033)), true);

}  // namespace

}  // namespace lifted
