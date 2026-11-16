// Lift sweep batch 042 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_042, true, "ITC",
              "Run lift sweep batch 042's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_825C2AA0: lifted from generated/ by lift.py.
lifted::Result f825C2AA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C2AA0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r4
  r28 = r4;
  // mr r30,r3
  r30 = r3;
  // rlwinm r11,r28,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c2b1c
  if (cr6.eq) goto loc_825C2B1C;
  // addi r29,r30,-4
  r29 = uint64_t(int64_t(int64_t(r30) + -4));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mulli r10,r11,44
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(44))));
  // addi r31,r11,-1
  r31 = uint64_t(int64_t(int64_t(r11) + -1));
  // add r30,r10,r30
  r30 = r10 + r30;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x825c2af4
  if (cr6.lt) goto loc_825C2AF4;
loc_825C2ADC:
  // addi r30,r30,-44
  r30 = uint64_t(int64_t(int64_t(r30) + -44));
  // mr r3,r30
  r3 = r30;
  // bl 0x825c9660
  // (left out: dead in C++)
  r3 = f825C9660(base, r1, r13, r3);
  // addi r31,r31,-1
  r31 = uint64_t(int64_t(int64_t(r31) + -1));
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge cr6,0x825c2adc
  if (!cr6.lt) goto loc_825C2ADC;
loc_825C2AF4:
  // clrlwi r11,r28,31
  r11 = uint32_t(r28) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c2b10
  if (cr6.eq) goto loc_825C2B10;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // mr r3,r29
  r3 = r29;
  // ori r4,r4,83
  r4 = r4 | 83;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C2B10, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C2B10:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825C2B1C:
  // mr r3,r30
  r3 = r30;
  // bl 0x825c9660
  // (left out: dead in C++)
  r3 = f825C9660(base, r1, r13, r3);
  // clrlwi r11,r28,31
  r11 = uint32_t(r28) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c2b40
  if (cr6.eq) goto loc_825C2B40;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // mr r3,r30
  r3 = r30;
  // ori r4,r4,32808
  r4 = r4 | 32808;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825C2B40, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C2B40:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C35C0: lifted from generated/ by lift.py.
void f825C35C0(uint8_t* base, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_825C35C0 (lifted)");
  uint64_t r0 = 0, r11 = 0, r12 = 0, ctr = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // clrlwi r11,r4,16
  r11 = uint32_t(r4) & 0xFFFF;
  // cmplwi cr6,r11,21
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 21, so);
  // bgtlr cr6
  if (cr6.gt) return;
  // lis r12,-32164
  r12 = uint64_t(int64_t(-2107899904));
  // addi r12,r12,13796
  r12 = uint64_t(int64_t(int64_t(r12) + 13796));
  // rlwinm r0,r11,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r0,r12,r0
  r0 = REX_LOAD_U32(uint32_t(r12) + uint32_t(r0));
  // mtctr r0
  ctr = r0;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_825C363C;
  case 1:
  goto loc_825C364C;
  case 2:
  goto loc_825C365C;
  case 3:
  goto loc_825C366C;
  case 4:
  goto loc_825C367C;
  case 5:
  goto loc_825C368C;
  case 6:
  goto loc_825C369C;
  case 7:
  goto loc_825C36AC;
  case 8:
  goto loc_825C36BC;
  case 9:
  goto loc_825C36CC;
  case 10:
  goto loc_825C36DC;
  case 11:
  goto loc_825C36EC;
  case 12:
  goto loc_825C36FC;
  case 13:
  goto loc_825C370C;
  case 14:
  goto loc_825C3714;
  case 15:
  goto loc_825C371C;
  case 16:
  goto loc_825C3724;
  case 17:
  goto loc_825C372C;
  case 18:
  goto loc_825C3734;
  case 19:
  goto loc_825C373C;
  case 20:
  goto loc_825C3744;
  case 21:
  goto loc_825C374C;
  default:
  std::abort();
  }
loc_825C363C:
  // addi r11,r3,12
  r11 = uint64_t(int64_t(int64_t(r3) + 12));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C364C:
  // addi r11,r3,16
  r11 = uint64_t(int64_t(int64_t(r3) + 16));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C365C:
  // addi r11,r3,20
  r11 = uint64_t(int64_t(int64_t(r3) + 20));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C366C:
  // addi r11,r3,24
  r11 = uint64_t(int64_t(int64_t(r3) + 24));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C367C:
  // addi r11,r3,28
  r11 = uint64_t(int64_t(int64_t(r3) + 28));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C368C:
  // addi r11,r3,32
  r11 = uint64_t(int64_t(int64_t(r3) + 32));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C369C:
  // addi r11,r3,36
  r11 = uint64_t(int64_t(int64_t(r3) + 36));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C36AC:
  // addi r11,r3,40
  r11 = uint64_t(int64_t(int64_t(r3) + 40));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C36BC:
  // addi r11,r3,44
  r11 = uint64_t(int64_t(int64_t(r3) + 44));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C36CC:
  // addi r11,r3,48
  r11 = uint64_t(int64_t(int64_t(r3) + 48));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C36DC:
  // addi r11,r3,52
  r11 = uint64_t(int64_t(int64_t(r3) + 52));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C36EC:
  // addi r11,r3,56
  r11 = uint64_t(int64_t(int64_t(r3) + 56));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C36FC:
  // addi r11,r3,60
  r11 = uint64_t(int64_t(int64_t(r3) + 60));
  // fctidz f0,f1
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f1) ? int64_t(0x8000000000000000ULL) : (f1 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f1)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // blr
  return;
loc_825C370C:
  // stfs f1,64(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 64, temp.u32);
  // blr
  return;
loc_825C3714:
  // stfs f1,68(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
  // blr
  return;
loc_825C371C:
  // stfs f1,72(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 72, temp.u32);
  // blr
  return;
loc_825C3724:
  // stfs f1,76(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 76, temp.u32);
  // blr
  return;
loc_825C372C:
  // stfs f1,80(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 80, temp.u32);
  // blr
  return;
loc_825C3734:
  // stfs f1,84(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 84, temp.u32);
  // blr
  return;
loc_825C373C:
  // stfs f1,88(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 88, temp.u32);
  // blr
  return;
loc_825C3744:
  // stfs f1,92(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 92, temp.u32);
  // blr
  return;
loc_825C374C:
  // stfs f1,96(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 96, temp.u32);
  // blr
  return;
}

// sub_825C3758: lifted from generated/ by lift.py.
lifted::Result f825C3758(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825C3758 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r10,r3
  r10 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,8(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c3798
  if (cr6.eq) goto loc_825C3798;
  // mr r3,r10
  r3 = r10;
  // bl 0x825c35c0
  // (left out: dead in C++)
  f825C35C0(base, r3, r4, f1);
  // lwz r3,8(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + 8);
  // addi r4,r10,12
  r4 = uint64_t(int64_t(int64_t(r10) + 12));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C3798, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C3798:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825C7358: lifted from generated/ by lift.py.
lifted::Result f825C7358(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C7358 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r4
  r28 = r4;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // mr r30,r3
  r30 = r3;
  // ori r4,r4,32773
  r4 = r4 | 32773;
  // li r3,352
  r3 = uint64_t(int64_t(352));
  // mr r27,r5
  r27 = r5;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825C7380, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x825c7414
  if (cr6.eq) goto loc_825C7414;
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // lwz r4,24(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 24);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r30
  r5 = r30;
  // bl 0x825c5e58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C5E58, LIFTED_ARGS(0x825C73A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // lis r9,-32249
  r9 = uint64_t(int64_t(-2113470464));
  // lis r8,-32249
  r8 = uint64_t(int64_t(-2113470464));
  // lis r7,-32249
  r7 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,22792
  r11 = uint64_t(int64_t(int64_t(r11) + 22792));
  // addi r10,r10,22636
  r10 = uint64_t(int64_t(int64_t(r10) + 22636));
  // addi r9,r9,24748
  r9 = uint64_t(int64_t(int64_t(r9) + 24748));
  // addi r8,r8,22788
  r8 = uint64_t(int64_t(int64_t(r8) + 22788));
  // addi r7,r7,24732
  r7 = uint64_t(int64_t(int64_t(r7) + 24732));
  // addi r29,r31,12
  r29 = uint64_t(int64_t(int64_t(r31) + 12));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // mr r4,r28
  r4 = r28;
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // mr r3,r31
  r3 = r31;
  // stw r9,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r9));
  // stw r7,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r7));
  // stw r8,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r8));
  // bl 0x825ccfa0
  // (left out: dead in C++)
  { const lifted::Result o = f825CCFA0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825c7424
  if (cr6.lt) goto loc_825C7424;
  // mr r4,r31
  r4 = r31;
  // stw r31,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r31));
  // lwz r3,24(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 24);
  // bl 0x825c1518
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C1518, LIFTED_ARGS(0x825C7408, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C7414:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_825C7424:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r29
  r3 = r29;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C743C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C7B10: lifted from generated/ by lift.py.
lifted::Result f825C7B10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825C7B10 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7B38, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C7B50, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 30);
  // add r3,r11,r30
  r3 = r11 + r30;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825C8EB8: lifted from generated/ by lift.py.
double f825C8EB8(uint8_t* base, uint64_t r1, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825C8EB8 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f1 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // extsh r11,r4
  r11 = uint64_t(int64_t(int16_t(r4)));
  // cmpwi cr6,r11,2400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2400, so);
  // blt cr6,0x825c8ed0
  if (cr6.lt) goto loc_825C8ED0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f1,8680(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f1 = double(temp.f32);
  // blr
  return f1;
loc_825C8ED0:
  // cmpwi cr6,r11,-2400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -2400, so);
  // bgt cr6,0x825c8ee4
  if (cr6.gt) goto loc_825C8EE4;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f1,10416(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10416);
  f1 = double(temp.f32);
  // blr
  return f1;
loc_825C8EE4:
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfd f0,-16(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,23436(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 23436);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // blr
  return f1;
}

// sub_825C8F08: lifted from generated/ by lift.py.
lifted::Result f825C8F08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_825C8F08 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r7,20(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // addi r9,r7,5
  r9 = uint64_t(int64_t(int64_t(r7) + 5));
  // lbz r11,2(r7)
  r11 = REX_LOAD_U8(uint32_t(r7) + 2);
  // stfs f0,36(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
  // lwz r10,0(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + 0);
  // addi r8,r11,-1
  r8 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // lfs f0,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x825c8f5c
  if (cr6.gt) goto loc_825C8F5C;
  // lwz r11,4(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 4);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lfs f0,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // b 0x825c90cc
  goto loc_825C90CC;
loc_825C8F5C:
  // lbz r11,2(r7)
  r11 = REX_LOAD_U8(uint32_t(r7) + 2);
  // rotlwi r10,r11,3
  r10 = __builtin_rotateleft32(uint32_t(r11), 3);
  // add r11,r11,r10
  r11 = r11 + r10;
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r10,-9(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -9);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x825c8f9c
  if (cr6.lt) goto loc_825C8F9C;
  // lwz r11,-5(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -5);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lbz r11,40(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 40);
  // ori r11,r11,64
  r11 = r11 | 64;
  // stb r11,40(r31)
  REX_STORE_U8(uint32_t(r31) + 40, uint8_t(r11));
  // lfs f0,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // b 0x825c90cc
  goto loc_825C90CC;
loc_825C8F9C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // ble cr6,0x825c90d0
  if (!cr6.gt) goto loc_825C90D0;
  // mr r10,r9
  r10 = r9;
loc_825C8FAC:
  // lwz r6,0(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r6,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r6));
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x825c8fd4
  if (cr6.lt) goto loc_825C8FD4;
  // lwz r6,9(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 9);
  // stw r6,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r6));
  // lfs f0,88(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // ble cr6,0x825c8fe8
  if (!cr6.gt) goto loc_825C8FE8;
loc_825C8FD4:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,9
  r10 = uint64_t(int64_t(int64_t(r10) + 9));
  // cmpw cr6,r11,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r8), so);
  // blt cr6,0x825c8fac
  if (cr6.lt) goto loc_825C8FAC;
  // b 0x825c90d0
  goto loc_825C90D0;
loc_825C8FE8:
  // rlwinm r10,r11,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // add r11,r11,r10
  r11 = r11 + r10;
  // add r11,r11,r9
  r11 = r11 + r9;
  // lwz r8,9(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 9);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lbz r10,8(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 8);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // stw r8,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r8));
  // stw r9,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r9));
  // lfs f0,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f0 = double(temp.f32);
  // fsubs f12,f1,f0
  f12 = double(float(f1 - f0));
  // lfs f13,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f13 = double(temp.f32);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // fdivs f0,f12,f0
  f0 = double(float(f12 / f0));
  // bne cr6,0x825c9070
  if (!cr6.eq) goto loc_825C9070;
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r10,13(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 13);
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f13,88(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f13 = double(temp.f32);
  // lfs f12,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f12 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // lfs f13,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f13 = double(temp.f32);
  // blt cr6,0x825c905c
  if (cr6.lt) goto loc_825C905C;
  // fnmsubs f0,f0,f0,f13
  f0 = double(float(-std::fma(f0, f0, -f13)));
  // fsqrts f0,f0
  f0 = double(float(sqrt(f0)));
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // b 0x825c90ac
  goto loc_825C90AC;
loc_825C905C:
  // fsubs f0,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f13 - f0));
  // fnmsubs f0,f0,f0,f13
  f0 = double(float(-std::fma(f0, f0, -f13)));
  // fsqrts f0,f0
  f0 = double(float(sqrt(f0)));
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // b 0x825c90ac
  goto loc_825C90AC;
loc_825C9070:
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // bne cr6,0x825c90ac
  if (!cr6.eq) goto loc_825C90AC;
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r10,13(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 13);
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lfs f13,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f13 = double(temp.f32);
  // lfs f12,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f12 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // lfs f13,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f13 = double(temp.f32);
  // blt cr6,0x825c90a4
  if (cr6.lt) goto loc_825C90A4;
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
loc_825C90A4:
  // fnmsubs f0,f0,f0,f13
  lifted::ScalarFloatMode();
  f0 = double(float(-std::fma(f0, f0, -f13)));
  // fsqrts f0,f0
  f0 = double(float(sqrt(f0)));
loc_825C90AC:
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,13(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 13);
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lfs f13,88(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 88);
  f13 = double(temp.f32);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lfs f12,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f12 = double(temp.f32);
  // fsubs f12,f12,f13
  f12 = double(float(f12 - f13));
  // fmadds f0,f12,f0,f13
  f0 = double(float(std::fma(f12, f0, f13)));
loc_825C90CC:
  // stfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
loc_825C90D0:
  // lhz r11,3(r7)
  r11 = REX_LOAD_U16(uint32_t(r7) + 3);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825c9178
  if (cr6.lt) goto loc_825C9178;
  // beq cr6,0x825c9118
  if (cr6.eq) goto loc_825C9118;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x825c91c8
  if (!cr6.lt) goto loc_825C91C8;
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // lfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,88(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 88);
  // cmpwi cr6,r11,6400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6400, so);
  // ble cr6,0x825c9108
  if (!cr6.gt) goto loc_825C9108;
  // li r11,6400
  r11 = uint64_t(int64_t(6400));
loc_825C9108:
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // b 0x825c91a4
  goto loc_825C91A4;
loc_825C9118:
  // lfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfd f0,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, std::bit_cast<uint64_t>(f0));
  // lhz r11,94(r1)
  r11 = REX_LOAD_U16(uint32_t(r1) + 94);
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // cmpwi cr6,r11,2400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2400, so);
  // blt cr6,0x825c9140
  if (cr6.lt) goto loc_825C9140;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f0 = double(temp.f32);
  // b 0x825c91c4
  goto loc_825C91C4;
loc_825C9140:
  // cmpwi cr6,r11,-2400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -2400, so);
  // bgt cr6,0x825c9154
  if (cr6.gt) goto loc_825C9154;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f0,10416(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10416);
  f0 = double(temp.f32);
  // b 0x825c91c4
  goto loc_825C91C4;
loc_825C9154:
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,23436(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 23436);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // b 0x825c91c4
  goto loc_825C91C4;
loc_825C9178:
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // lfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,88(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 88);
  // cmpwi cr6,r11,6400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6400, so);
  // ble cr6,0x825c9198
  if (!cr6.gt) goto loc_825C9198;
  // li r11,6400
  r11 = uint64_t(int64_t(6400));
loc_825C9198:
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
loc_825C91A4:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fcfid f13,f0
  lifted::ScalarFloatMode();
  f13 = double(std::bit_cast<int64_t>(f0));
  // lfd f1,-2264(r11)
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -2264)));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfd f0,-18128(r11)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -18128)));
  // fmul f2,f13,f0
  f2 = f13 * f0;
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS(0x825C91C0, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
loc_825C91C4:
  // stfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
loc_825C91C8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825C91E0: lifted from generated/ by lift.py.
lifted::Result f825C91E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825C91E0 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r5,r4
  r5 = r4;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r3,124(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 124);
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c92bc
  if (cr6.eq) goto loc_825C92BC;
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lhz r10,3(r10)
  r10 = REX_LOAD_U16(uint32_t(r10) + 3);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x825c9294
  if (cr6.lt) goto loc_825C9294;
  // beq cr6,0x825c926c
  if (cr6.eq) goto loc_825C926C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x825c9390
  if (!cr6.lt) goto loc_825C9390;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C9238, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,21(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 21);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c9390
  if (cr6.eq) goto loc_825C9390;
  // lwz r3,32(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 32);
  // lfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f0 = double(temp.f32);
  // lfs f13,116(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 116);
  f13 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
  // bl 0x825cab40
  // (left out: dead in C++)
  { const lifted::Result o = f825CAB40(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_825C926C:
  // lfs f0,120(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 120);
  f0 = double(temp.f32);
  // mr r3,r11
  r3 = r11;
  // lfs f13,36(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f13 = double(temp.f32);
  // fadds f1,f0,f13
  f1 = double(float(f0 + f13));
  // bl 0x825cabb0
  // (left out: dead in C++)
  { const lifted::Result o = f825CABB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_825C9294:
  // lfs f0,112(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 112);
  f0 = double(temp.f32);
  // mr r3,r11
  r3 = r11;
  // lfs f13,36(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f13 = double(temp.f32);
  // fmuls f1,f0,f13
  f1 = double(float(f0 * f13));
  // bl 0x825cab10
  // (left out: dead in C++)
  { const lifted::Result o = f825CAB10(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_825C92BC:
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c9374
  if (cr6.eq) goto loc_825C9374;
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lhz r10,3(r10)
  r10 = REX_LOAD_U16(uint32_t(r10) + 3);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x825c9304
  if (cr6.lt) goto loc_825C9304;
  // bne cr6,0x825c9390
  if (!cr6.eq) goto loc_825C9390;
  // lfs f0,60(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 60);
  f0 = double(temp.f32);
  // mr r3,r11
  r3 = r11;
  // lfs f13,36(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f13 = double(temp.f32);
  // fadds f1,f0,f13
  f1 = double(float(f0 + f13));
  // bl 0x825d06a0
  // (left out: dead in C++)
  { const lifted::Result o = f825D06A0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_825C9304:
  // lfs f0,56(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 56);
  f0 = double(temp.f32);
  // mr r3,r11
  r3 = r11;
  // lfs f13,36(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f13 = double(temp.f32);
  // fmuls f1,f0,f13
  f1 = double(float(f0 * f13));
  // bl 0x825d0670
  // (left out: dead in C++)
  { const lifted::Result o = f825D0670(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825C9318); r3 = o.r3; f1 = o.f1; }
  // lbz r11,40(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 40);
  // rlwinm r10,r11,0,25,25
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825c9390
  if (cr6.eq) goto loc_825C9390;
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lhz r10,0(r10)
  r10 = REX_LOAD_U16(uint32_t(r10) + 0);
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // bne cr6,0x825c9358
  if (!cr6.eq) goto loc_825C9358;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,28(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 28);
  // bl 0x825d01d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D01D8, LIFTED_ARGS(0x825C9344, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_825C9358:
  // clrlwi r11,r11,25
  r11 = uint32_t(r11) & 0x7F;
  // stb r11,40(r31)
  REX_STORE_U8(uint32_t(r31) + 40, uint8_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_825C9374:
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lfs f1,36(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 36);
  f1 = double(temp.f32);
  // lhz r11,3(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 3);
  // addis r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 65536));
  // addi r11,r11,-3
  r11 = uint64_t(int64_t(int64_t(r11) + -3));
  // clrlwi r4,r11,16
  r4 = uint32_t(r11) & 0xFFFF;
  // bl 0x825c3758
  // (left out: dead in C++)
  { const lifted::Result o = f825C3758(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_825C9390:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825C95F8: lifted from generated/ by lift.py.
void f825C95F8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C95F8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r11,r3,4
  r11 = uint64_t(int64_t(int64_t(r3) + 4));
  // li r6,128
  r6 = uint64_t(int64_t(128));
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // stw r11,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r11));
  // addi r9,r10,21316
  r9 = uint64_t(int64_t(int64_t(r10) + 21316));
  // stw r11,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r11));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r8,r10,23444
  r8 = uint64_t(int64_t(int64_t(r10) + 23444));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stfs f0,36(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 36, temp.u32);
  // addi r7,r10,23440
  r7 = uint64_t(int64_t(int64_t(r10) + 23440));
  // stb r6,40(r3)
  REX_STORE_U8(uint32_t(r3) + 40, uint8_t(r6));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r8,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r10,16(r3)
  REX_STORE_U32(uint32_t(r3) + 16, uint32_t(r10));
  // stw r10,20(r3)
  REX_STORE_U32(uint32_t(r3) + 20, uint32_t(r10));
  // stw r10,24(r3)
  REX_STORE_U32(uint32_t(r3) + 24, uint32_t(r10));
  // stw r10,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r10));
  // stw r10,32(r3)
  REX_STORE_U32(uint32_t(r3) + 32, uint32_t(r10));
  // blr
  return;
}

// sub_825C9658: lifted from generated/ by lift.py.
lifted::Result f825C9658(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C9658 (lifted)");
  // addi r3,r3,-4
  r3 = uint64_t(int64_t(int64_t(r3) + -4));
  // b 0x825c2aa0
  { const lifted::Result o = f825C2AA0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825C9660: lifted from generated/ by lift.py.
uint64_t f825C9660(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825C9660 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,23444
  r11 = uint64_t(int64_t(int64_t(r11) + 23444));
  // addi r10,r10,23440
  r10 = uint64_t(int64_t(int64_t(r10) + 23440));
  // lwz r9,24(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // beq cr6,0x825c96d0
  if (cr6.eq) goto loc_825C96D0;
  // lwz r4,228(r9)
  r4 = REX_LOAD_U32(uint32_t(r9) + 228);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825c96d0
  if (cr6.eq) goto loc_825C96D0;
loc_825C96A4:
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // cmplw cr6,r11,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r31), so);
  // beq cr6,0x825c96c0
  if (cr6.eq) goto loc_825C96C0;
  // lwz r4,4(r4)
  r4 = REX_LOAD_U32(uint32_t(r4) + 4);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x825c96a4
  if (!cr6.eq) goto loc_825C96A4;
  // b 0x825c96d0
  goto loc_825C96D0;
loc_825C96C0:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825c96d0
  if (cr6.eq) goto loc_825C96D0;
  // addi r3,r9,24
  r3 = uint64_t(int64_t(int64_t(r9) + 24));
  // bl 0x825c2c60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C2C60, LIFTED_ARGS(0x825C96D0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C96D0:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825C96F0: lifted from generated/ by lift.py.
lifted::Result f825C96F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C96F0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825c8f08
  // (left out: dead in C++)
  { const lifted::Result o = f825C8F08(base, r1, r13, r3, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,32(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 32);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c9738
  if (cr6.eq) goto loc_825C9738;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lhz r11,3(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 3);
  // clrlwi r4,r11,24
  r4 = uint32_t(r11) & 0xFF;
  // bl 0x825cb818
  // (left out: dead in C++)
  { const lifted::Result o = f825CB818(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_825C9738:
  // lwz r3,28(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825c9768
  if (cr6.eq) goto loc_825C9768;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lhz r11,3(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 3);
  // clrlwi r4,r11,24
  r4 = uint32_t(r11) & 0xFF;
  // bl 0x825d0868
  // (left out: dead in C++)
  { const lifted::Result o = f825D0868(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_825C9768:
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x825c91e0
  // (left out: dead in C++)
  { const lifted::Result o = f825C91E0(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825C9788: lifted from generated/ by lift.py.
lifted::Result f825C9788(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C9788 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r3,r4
  r3 = r4;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // stw r4,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r4));
  // stw r6,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r6));
  // stw r5,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r5));
  // lhz r4,0(r6)
  r4 = REX_LOAD_U16(uint32_t(r6) + 0);
  // bl 0x825c0748
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C0748, LIFTED_ARGS(0x825C97B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 0);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c981c
  if (cr6.eq) goto loc_825C981C;
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // addi r30,r31,24
  r30 = uint64_t(int64_t(int64_t(r31) + 24));
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r30
  r5 = r30;
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lhz r4,0(r10)
  r4 = REX_LOAD_U16(uint32_t(r10) + 0);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // bl 0x825c5960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C5960, LIFTED_ARGS(0x825C97E8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825c9868
  if (cr6.lt) goto loc_825C9868;
loc_825C97F4:
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lhz r11,3(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 3);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825c984c
  if (cr6.lt) goto loc_825C984C;
  // beq cr6,0x825c9810
  if (cr6.eq) goto loc_825C9810;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x825c9858
  if (!cr6.lt) goto loc_825C9858;
loc_825C9810:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // b 0x825c9854
  goto loc_825C9854;
loc_825C981C:
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lhz r4,0(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 0);
  // bl 0x825c0698
  // (left out: dead in C++)
  { const lifted::Result o = f825C0698(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,24
  r30 = uint64_t(int64_t(int64_t(r31) + 24));
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r3));
  // bne cr6,0x825c97f4
  if (!cr6.eq) goto loc_825C97F4;
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,10
  r3 = r3 | 10;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C984C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
loc_825C9854:
  // stfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
loc_825C9858:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r4,r31
  r4 = r31;
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // bl 0x825c60c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C60C0, LIFTED_ARGS(0x825C9868, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C9868:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C9878: lifted from generated/ by lift.py.
lifted::Result f825C9878(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C9878 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r3,r4
  r3 = r4;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // stw r4,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r4));
  // stw r5,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r5));
  // lhz r4,0(r5)
  r4 = REX_LOAD_U16(uint32_t(r5) + 0);
  // bl 0x825c0748
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C0748, LIFTED_ARGS(0x825C98A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 0);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825c98e0
  if (!cr6.eq) goto loc_825C98E0;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lhz r4,0(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 0);
  // bl 0x825c0698
  // (left out: dead in C++)
  { const lifted::Result o = f825C0698(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r3));
  // bne cr6,0x825c98e0
  if (!cr6.eq) goto loc_825C98E0;
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,10
  r3 = r3 | 10;
  // b 0x825c9944
  goto loc_825C9944;
loc_825C98E0:
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lhz r11,3(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 3);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825c9908
  if (cr6.lt) goto loc_825C9908;
  // beq cr6,0x825c98fc
  if (cr6.eq) goto loc_825C98FC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x825c9914
  if (!cr6.lt) goto loc_825C9914;
loc_825C98FC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // b 0x825c9910
  goto loc_825C9910;
loc_825C9908:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
loc_825C9910:
  // stfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
loc_825C9914:
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // mr r4,r31
  r4 = r31;
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // bl 0x825c60c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C60C0, LIFTED_ARGS(0x825C9924, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r10,24(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 24);
  // mr r3,r31
  r3 = r31;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lfs f1,20(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 20);
  f1 = double(temp.f32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825C9940, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
loc_825C9944:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825C9960: lifted from generated/ by lift.py.
lifted::Result f825C9960(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C9960 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // addi r30,r31,24
  r30 = uint64_t(int64_t(int64_t(r31) + 24));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c99bc
  if (cr6.eq) goto loc_825C99BC;
  // lwz r4,228(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 228);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825c99bc
  if (cr6.eq) goto loc_825C99BC;
loc_825C9990:
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // cmplw cr6,r10,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r31), so);
  // beq cr6,0x825c99ac
  if (cr6.eq) goto loc_825C99AC;
  // lwz r4,4(r4)
  r4 = REX_LOAD_U32(uint32_t(r4) + 4);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne cr6,0x825c9990
  if (!cr6.eq) goto loc_825C9990;
  // b 0x825c99bc
  goto loc_825C99BC;
loc_825C99AC:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825c99bc
  if (cr6.eq) goto loc_825C99BC;
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // bl 0x825c2c60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C2C60, LIFTED_ARGS(0x825C99BC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C99BC:
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lhz r4,0(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 0);
  // bl 0x825c0748
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C0748, LIFTED_ARGS(0x825C99CC, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,0(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 0);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c9a50
  if (cr6.eq) goto loc_825C9A50;
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c9a28
  if (cr6.eq) goto loc_825C9A28;
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mr r5,r30
  r5 = r30;
  // lhz r4,0(r10)
  r4 = REX_LOAD_U16(uint32_t(r10) + 0);
loc_825C99FC:
  // bl 0x825c5960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C5960, LIFTED_ARGS(0x825C9A00, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825c9a1c
  if (cr6.lt) goto loc_825C9A1C;
loc_825C9A0C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r4,r31
  r4 = r31;
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // bl 0x825c60c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C60C0, LIFTED_ARGS(0x825C9A1C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825C9A1C:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825C9A28:
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825c9a0c
  if (cr6.eq) goto loc_825C9A0C;
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mr r5,r30
  r5 = r30;
  // lhz r4,0(r10)
  r4 = REX_LOAD_U16(uint32_t(r10) + 0);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // b 0x825c99fc
  goto loc_825C99FC;
loc_825C9A50:
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lhz r4,0(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 0);
  // bl 0x825c0698
  // (left out: dead in C++)
  { const lifted::Result o = f825C0698(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r3));
  // bne cr6,0x825c9a0c
  if (!cr6.eq) goto loc_825C9A0C;
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,10
  r3 = r3 | 10;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825C9A80: lifted from generated/ by lift.py.
lifted::Result f825C9A80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825C9A80 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // stw r4,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r4));
  // stw r5,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r5));
  // stw r6,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r6));
  // bl 0x825c9960
  // (left out: dead in C++)
  { const lifted::Result o = f825C9960(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825c9ae0
  if (cr6.lt) goto loc_825C9AE0;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lhz r11,3(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 3);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825c9ad4
  if (cr6.lt) goto loc_825C9AD4;
  // beq cr6,0x825c9ac8
  if (cr6.eq) goto loc_825C9AC8;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x825c9ae0
  if (!cr6.lt) goto loc_825C9AE0;
loc_825C9AC8:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // b 0x825c9adc
  goto loc_825C9ADC;
loc_825C9AD4:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
loc_825C9ADC:
  // stfs f0,36(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 36, temp.u32);
loc_825C9AE0:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825CA8B0: lifted from generated/ by lift.py.
lifted::Result f825CA8B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CA8B0 (lifted)");
  uint64_t r12 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r31,40(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 40);
  // addi r30,r3,36
  r30 = uint64_t(int64_t(int64_t(r3) + 36));
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // beq cr6,0x825ca8e8
  if (cr6.eq) goto loc_825CA8E8;
loc_825CA8D4:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d04a8
  // (left out: dead in C++)
  { const lifted::Result o = f825D04A8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r30), so);
  // bne cr6,0x825ca8d4
  if (!cr6.eq) goto loc_825CA8D4;
loc_825CA8E8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825CAB10: lifted from generated/ by lift.py.
lifted::Result f825CAB10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CAB10 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x825cab28
  if (cr6.lt) goto loc_825CAB28;
  // stfs f1,112(r3)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 112, temp.u32);
  // b 0x825cab2c
  goto loc_825CAB2C;
loc_825CAB28:
  // stfs f0,112(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 112, temp.u32);
loc_825CAB2C:
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825ca860
  { const lifted::Result o = f825CA860(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CAB40: lifted from generated/ by lift.py.
lifted::Result f825CAB40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CAB40 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // lwz r11,28(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 28);
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // rlwinm r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // bne cr6,0x825caba8
  if (!cr6.eq) goto loc_825CABA8;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x825cab7c
  if (cr6.lt) goto loc_825CAB7C;
  // stfs f1,116(r30)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r30) + 116, temp.u32);
  // b 0x825cab80
  goto loc_825CAB80;
loc_825CAB7C:
  // stfs f0,116(r30)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 116, temp.u32);
loc_825CAB80:
  // lwz r31,40(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 40);
  // addi r29,r30,36
  r29 = uint64_t(int64_t(int64_t(r30) + 36));
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // beq cr6,0x825caba8
  if (cr6.eq) goto loc_825CABA8;
loc_825CAB90:
  // mr r3,r31
  r3 = r31;
  // lfs f1,116(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 116);
  f1 = double(temp.f32);
  // bl 0x825d0490
  // (left out: dead in C++)
  { const lifted::Result o = f825D0490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825CAB9C); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // bne cr6,0x825cab90
  if (!cr6.eq) goto loc_825CAB90;
loc_825CABA8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CABB0: lifted from generated/ by lift.py.
lifted::Result f825CABB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825CABB0 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // stfs f1,120(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 120, temp.u32);
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825ca8b0
  { const lifted::Result o = f825CA8B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825CABC8: lifted from generated/ by lift.py.
lifted::Result f825CABC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CABC8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CABEC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // mr r29,r3
  r29 = r3;
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // rlwinm r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x825cac28
  if (!cr6.eq) goto loc_825CAC28;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CAC18, r31)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,2(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 2);
  // rotlwi r11,r11,2
  r11 = __builtin_rotateleft32(uint32_t(r11), 2);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x825cac2c
  goto loc_825CAC2C;
loc_825CAC28:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825CAC2C:
  // rlwinm r10,r30,2,22,29
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0x3FC;
  // add r11,r10,r11
  r11 = r10 + r11;
  // add r11,r11,r29
  r11 = r11 + r29;
  // lwz r3,3(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 3);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CB3D8: lifted from generated/ by lift.py.
lifted::Result f825CB3D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CB3D8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CB400, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 28);
  // addi r31,r3,2
  r31 = uint64_t(int64_t(int64_t(r3) + 2));
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x825cb42c
  if (!cr6.eq) goto loc_825CB42C;
  // mr r3,r30
  r3 = r30;
  // bl 0x825cae08
  // (left out: dead in C++)
  { const lifted::Result o = f825CAE08(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r3,2,22,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0x3FC;
  // add r11,r11,r31
  r11 = r11 + r31;
  // addi r31,r11,1
  r31 = uint64_t(int64_t(int64_t(r11) + 1));
loc_825CB42C:
  // clrlwi r9,r29,24
  r9 = uint32_t(r29) & 0xFF;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x825cb45c
  if (cr6.eq) goto loc_825CB45C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825CB43C:
  // lbz r10,0(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // rotlwi r10,r10,2
  r10 = __builtin_rotateleft32(uint32_t(r10), 2);
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // add r10,r10,r31
  r10 = r10 + r31;
  // cmplw cr6,r11,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r9), so);
  // addi r31,r10,1
  r31 = uint64_t(int64_t(int64_t(r10) + 1));
  // blt cr6,0x825cb43c
  if (cr6.lt) goto loc_825CB43C;
loc_825CB45C:
  // rlwinm r11,r28,2,22,29
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0x3FC;
  // add r11,r11,r31
  r11 = r11 + r31;
  // lwz r3,1(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 1);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CB818: lifted from generated/ by lift.py.
lifted::Result f825CB818(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CB818 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // clrlwi r29,r4,24
  r29 = uint32_t(r4) & 0xFF;
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825cb844
  if (!cr6.eq) goto loc_825CB844;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,112(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 112, temp.u32);
  // b 0x825cb870
  goto loc_825CB870;
loc_825CB844:
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // bne cr6,0x825cb85c
  if (!cr6.eq) goto loc_825CB85C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,120(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 120, temp.u32);
  // b 0x825cb870
  goto loc_825CB870;
loc_825CB85C:
  // cmplwi cr6,r29,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 2, so);
  // bne cr6,0x825cb870
  if (!cr6.eq) goto loc_825CB870;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,116(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 116, temp.u32);
loc_825CB870:
  // mr r3,r30
  r3 = r30;
  // bl 0x825cae08
  // (left out: dead in C++)
  { const lifted::Result o = f825CAE08(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // clrlwi r28,r3,24
  r28 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x825cb8c0
  if (cr6.eq) goto loc_825CB8C0;
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_825CB888:
  // lwz r10,96(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 96);
  // mulli r11,r31,44
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(44))));
  // add r3,r11,r10
  r3 = r11 + r10;
  // lwz r11,20(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lhz r11,3(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 3);
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bne cr6,0x825cb8b0
  if (!cr6.eq) goto loc_825CB8B0;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825c91e0
  // (left out: dead in C++)
  { const lifted::Result o = f825C91E0(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_825CB8B0:
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r28), so);
  // blt cr6,0x825cb888
  if (cr6.lt) goto loc_825CB888;
loc_825CB8C0:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825cb8f4
  if (!cr6.eq) goto loc_825CB8F4;
  // lwz r31,40(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 40);
  // addi r29,r30,36
  r29 = uint64_t(int64_t(int64_t(r30) + 36));
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // beq cr6,0x825cb950
  if (cr6.eq) goto loc_825CB950;
loc_825CB8D8:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d0428
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0428, LIFTED_ARGS(0x825CB8E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // bne cr6,0x825cb8d8
  if (!cr6.eq) goto loc_825CB8D8;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CB8F4:
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // bne cr6,0x825cb90c
  if (!cr6.eq) goto loc_825CB90C;
  // mr r3,r30
  r3 = r30;
  // bl 0x825ca8b0
  // (left out: dead in C++)
  { const lifted::Result o = f825CA8B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825CB90C:
  // cmplwi cr6,r29,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 2, so);
  // bne cr6,0x825cb950
  if (!cr6.eq) goto loc_825CB950;
  // lwz r11,28(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 28);
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // rlwinm r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  // cmplwi cr6,r11,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 16, so);
  // bne cr6,0x825cb950
  if (!cr6.eq) goto loc_825CB950;
  // lwz r31,40(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 40);
  // addi r29,r30,36
  r29 = uint64_t(int64_t(int64_t(r30) + 36));
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // beq cr6,0x825cb950
  if (cr6.eq) goto loc_825CB950;
loc_825CB938:
  // mr r3,r31
  r3 = r31;
  // lfs f1,116(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 116);
  f1 = double(temp.f32);
  // bl 0x825d0490
  // (left out: dead in C++)
  { const lifted::Result o = f825D0490(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825CB944); r3 = o.r3; f1 = o.f1; }
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplw cr6,r31,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r29), so);
  // bne cr6,0x825cb938
  if (!cr6.eq) goto loc_825CB938;
loc_825CB950:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825CC148: lifted from generated/ by lift.py.
lifted::Result f825CC148(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_825CC148 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x825cc194
  if (cr6.eq) goto loc_825CC194;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x825cc178
  if (cr6.eq) goto loc_825CC178;
  // lfs f0,20(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f0 = double(temp.f32);
  // b 0x825cc1b4
  goto loc_825CC1B4;
loc_825CC178:
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfs f13,20(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f13 = double(temp.f32);
  // lfd f0,-6072(r11)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -6072)));
  // fmul f1,f13,f0
  f1 = f13 * f0;
  // bl 0x825a88a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A88A0, LIFTED_ARGS(0x825CC18C, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f0,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f1));
  // b 0x825cc1b4
  goto loc_825CC1B4;
loc_825CC194:
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f2,20(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f2 = double(temp.f32);
  // lfd f1,-2264(r11)
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -2264)));
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS(0x825CC1A4, r31)); r3 = o.r3; f1 = o.f1; }
  // frsp f13,f1
  lifted::ScalarFloatMode();
  f13 = double(float(f1));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,8124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8124);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
loc_825CC1B4:
  // stfs f0,4(r31)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825CC858: lifted from generated/ by lift.py.
lifted::Result f825CC858(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825CC858 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CC880, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 28);
  // mr r30,r3
  r30 = r3;
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // rlwinm r10,r11,0,30,30
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // beq cr6,0x825cc8b0
  if (cr6.eq) goto loc_825CC8B0;
  // rlwinm r10,r11,0,29,29
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // beq cr6,0x825cc8b0
  if (cr6.eq) goto loc_825CC8B0;
  // rlwinm r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x825cc8cc
  if (!cr6.eq) goto loc_825CC8CC;
loc_825CC8B0:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CC8C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,0(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 0);
  // b 0x825cc8d0
  goto loc_825CC8D0;
loc_825CC8CC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825CC8D0:
  // lhzx r3,r11,r30
  r3 = REX_LOAD_U16(uint32_t(r11) + uint32_t(r30));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825CCFA0: lifted from generated/ by lift.py.
lifted::Result f825CCFA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825CCFA0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // mr r30,r4
  r30 = r4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // lwz r11,84(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 84);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825CCFD4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825cd004
  if (cr6.lt) goto loc_825CD004;
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825c5c80
  // (left out: dead in C++)
  { const lifted::Result o = f825C5C80(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825cd004
  if (cr6.lt) goto loc_825CD004;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r11));
loc_825CD004:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D0490: lifted from generated/ by lift.py.
lifted::Result f825D0490(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D0490 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,24(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 24);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825d5698
  { const lifted::Result o = f825D5698(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D04A8: lifted from generated/ by lift.py.
lifted::Result f825D04A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D04A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stfd f30,-32(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -32, std::bit_cast<uint64_t>(f30));
  // stfd f31,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d04f0
  if (cr6.eq) goto loc_825D04F0;
  // lfs f31,44(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 44);
  f31 = double(temp.f32);
  // lwz r3,20(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lfs f30,60(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 60);
  f30 = double(temp.f32);
  // bl 0x825cac48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CAC48, LIFTED_ARGS(0x825D04E0, r31)); r3 = o.r3; f1 = o.f1; }
  // fadds f0,f31,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f31 + f1));
  // lwz r3,24(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 24);
  // fadds f1,f0,f30
  f1 = double(float(f0 + f30));
  // bl 0x825d56b8
  // (left out: dead in C++)
  { const lifted::Result o = f825D56B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825D04F0); r3 = o.r3; f1 = o.f1; }
loc_825D04F0:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f30,-32(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D0540: lifted from generated/ by lift.py.
lifted::Result f825D0540(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D0540 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,24(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 24);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825d5718
  { const lifted::Result o = f825D5718(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, lr); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D0670: lifted from generated/ by lift.py.
lifted::Result f825D0670(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D0670 (lifted)");
  uint64_t r11 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // blt cr6,0x825d0688
  if (cr6.lt) goto loc_825D0688;
  // stfs f1,56(r3)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 56, temp.u32);
  // b 0x825d068c
  goto loc_825D068C;
loc_825D0688:
  // stfs f0,56(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 56, temp.u32);
loc_825D068C:
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825d0428
  { const lifted::Result o = lifted::Call(base, 0x825D0428, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D06A0: lifted from generated/ by lift.py.
lifted::Result f825D06A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D06A0 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // stfs f1,60(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 60, temp.u32);
  // cmpwi cr6,r5,0
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825d04a8
  { const lifted::Result o = f825D04A8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D0868: lifted from generated/ by lift.py.
lifted::Result f825D0868(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D0868 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-56(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -56, std::bit_cast<uint64_t>(f30));
  // stfd f31,-48(r1)
  REX_STORE_U64(uint32_t(r1) + -48, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // clrlwi r29,r4,24
  r29 = uint32_t(r4) & 0xFF;
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825d089c
  if (!cr6.eq) goto loc_825D089C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,56(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 56, temp.u32);
  // b 0x825d08a8
  goto loc_825D08A8;
loc_825D089C:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,60(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 60, temp.u32);
loc_825D08A8:
  // lbz r4,48(r30)
  r4 = REX_LOAD_U8(uint32_t(r30) + 48);
  // lwz r3,20(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 20);
  // bl 0x825cb348
  // (left out: dead in C++)
  { const lifted::Result o = f825CB348(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // clrlwi r28,r3,24
  r28 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x825d08fc
  if (cr6.eq) goto loc_825D08FC;
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_825D08C4:
  // lwz r10,52(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 52);
  // mulli r11,r31,44
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(44))));
  // add r3,r11,r10
  r3 = r11 + r10;
  // lwz r11,20(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 20);
  // lhz r11,3(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 3);
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r11,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r29), so);
  // bne cr6,0x825d08ec
  if (!cr6.eq) goto loc_825D08EC;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x825c91e0
  // (left out: dead in C++)
  { const lifted::Result o = f825C91E0(base, r1, r13, r3, r4, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_825D08EC:
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r31,r28
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r28), so);
  // blt cr6,0x825d08c4
  if (cr6.lt) goto loc_825D08C4;
loc_825D08FC:
  // lwz r11,24(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 24);
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x825d0940
  if (!cr6.eq) goto loc_825D0940;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d0968
  if (cr6.eq) goto loc_825D0968;
  // lfs f31,40(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 40);
  f31 = double(temp.f32);
  // lwz r3,20(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 20);
  // lfs f30,56(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 56);
  f30 = double(temp.f32);
  // bl 0x825cb5e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CB5E8, LIFTED_ARGS(0x825D0920, r31)); r3 = o.r3; f1 = o.f1; }
  // fmuls f0,f31,f30
  lifted::ScalarFloatMode();
  f0 = double(float(f31 * f30));
  // lwz r3,24(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 24);
  // fmuls f1,f0,f1
  f1 = double(float(f0 * f1));
  // bl 0x825d5678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D5678, LIFTED_ARGS(0x825D0930, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f30,-56(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -56)));
  // lfd f31,-48(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_825D0940:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d0968
  if (cr6.eq) goto loc_825D0968;
  // lfs f31,44(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 44);
  f31 = double(temp.f32);
  // lwz r3,20(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 20);
  // lfs f30,60(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 60);
  f30 = double(temp.f32);
  // bl 0x825cac48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CAC48, LIFTED_ARGS(0x825D0958, r31)); r3 = o.r3; f1 = o.f1; }
  // fadds f0,f31,f1
  lifted::ScalarFloatMode();
  f0 = double(float(f31 + f1));
  // lwz r3,24(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 24);
  // fadds f1,f0,f30
  f1 = double(float(f0 + f30));
  // bl 0x825d56b8
  // (left out: dead in C++)
  { const lifted::Result o = f825D56B8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x825D0968); r3 = o.r3; f1 = o.f1; }
loc_825D0968:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f30,-56(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -56)));
  // lfd f31,-48(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D0E78: lifted from generated/ by lift.py.
void f825D0E78(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D0E78 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,20(r3)
  REX_STORE_U32(uint32_t(r3) + 20, uint32_t(r11));
  // stw r11,24(r3)
  REX_STORE_U32(uint32_t(r3) + 24, uint32_t(r11));
  // stw r11,32(r3)
  REX_STORE_U32(uint32_t(r3) + 32, uint32_t(r11));
  // stw r11,28(r3)
  REX_STORE_U32(uint32_t(r3) + 28, uint32_t(r11));
  // lbz r11,40(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 40);
  // rlwinm r11,r11,0,28,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x8;
  // stb r11,40(r3)
  REX_STORE_U8(uint32_t(r3) + 40, uint8_t(r11));
  // blr
  return;
}

// sub_825D1A80: lifted from generated/ by lift.py.
uint64_t f825D1A80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_825D1A80 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r8
  r30 = r8;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1760
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1760, LIFTED_ARGS(0x825D1AA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r30,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r30));
  // addi r11,r11,27172
  r11 = uint64_t(int64_t(int64_t(r11) + 27172));
  // mr r3,r31
  r3 = r31;
  // stw r10,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D1AD8: lifted from generated/ by lift.py.
lifted::Result f825D1AD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D1AD8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,27172
  r11 = uint64_t(int64_t(int64_t(r11) + 27172));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d0ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0EF0, LIFTED_ARGS(0x825D1B04, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d1b24
  if (cr6.eq) goto loc_825D1B24;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32778
  r4 = r4 | 32778;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D1B20, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D1B24:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D1B88: lifted from generated/ by lift.py.
lifted::Result f825D1B88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D1B88 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1358
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1358, LIFTED_ARGS(0x825D1BA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825d1bc4
  if (!cr6.eq) goto loc_825D1BC4;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // addi r4,r11,7
  r4 = uint64_t(int64_t(int64_t(r11) + 7));
  // bl 0x825bf7a8
  // (left out: dead in C++)
  { const lifted::Result o = f825BF7A8(base, r1, r13, r3, r4, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x825d1bfc
  if (cr6.eq) goto loc_825D1BFC;
loc_825D1BC4:
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r10,16(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r11,28(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 28);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d1be4
  if (cr6.eq) goto loc_825D1BE4;
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825d1be8
  if (!cr6.eq) goto loc_825D1BE8;
loc_825D1BE4:
  // stw r31,36(r10)
  REX_STORE_U32(uint32_t(r10) + 36, uint32_t(r31));
loc_825D1BE8:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d13f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D13F0, LIFTED_ARGS(0x825D1BF0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d0b70
  // (left out: dead in C++)
  { const lifted::Result o = f825D0B70(base, r1, r13, r3, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_825D1BFC:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D2AD8: lifted from generated/ by lift.py.
void f825D2AD8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D2AD8 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,27240
  r11 = uint64_t(int64_t(int64_t(r11) + 27240));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_825D35A8: lifted from generated/ by lift.py.
uint64_t f825D35A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825D35A8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // bl 0x825d3428
  // (left out: dead in C++)
  r3 = f825D3428(base, r1, r13, r3);
  // clrlwi r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x825d3604
  if (cr6.eq) goto loc_825D3604;
  // rlwinm r9,r11,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r10,r10,r11
  r10 = r10 + r11;
  // lhz r10,8(r10)
  r10 = REX_LOAD_U16(uint32_t(r10) + 8);
  // sth r10,0(r30)
  REX_STORE_U16(uint32_t(r30) + 0, uint16_t(r10));
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // add r11,r10,r11
  r11 = r10 + r11;
  // lbz r11,10(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 10);
  // stb r11,0(r29)
  REX_STORE_U8(uint32_t(r29) + 0, uint8_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
loc_825D3604:
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,20
  r3 = r3 | 20;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_825D5698: lifted from generated/ by lift.py.
lifted::Result f825D5698(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D5698 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,36(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D5718: lifted from generated/ by lift.py.
lifted::Result f825D5718(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D5718 (lifted)");
  uint64_t r11 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,36(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 36);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,76(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 76);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825D5DE8: lifted from generated/ by lift.py.
void f825D5DE8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825D5DE8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // addi r11,r3,48
  r11 = uint64_t(int64_t(int64_t(r3) + 48));
  // stw r11,4(r4)
  REX_STORE_U32(uint32_t(r4) + 4, uint32_t(r11));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r10,8(r4)
  REX_STORE_U32(uint32_t(r4) + 8, uint32_t(r10));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r4,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r4));
  // stw r4,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r4));
  // blr
  return;
}

// sub_825D5F40: lifted from generated/ by lift.py.
uint64_t f825D5F40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825D5F40 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1760
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1760, LIFTED_ARGS(0x825D5F5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,27356
  r11 = uint64_t(int64_t(int64_t(r11) + 27356));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D5F80: lifted from generated/ by lift.py.
lifted::Result f825D5F80(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D5F80 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,27356
  r11 = uint64_t(int64_t(int64_t(r11) + 27356));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d0ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0EF0, LIFTED_ARGS(0x825D5FAC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d5fcc
  if (cr6.eq) goto loc_825D5FCC;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32783
  r4 = r4 | 32783;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D5FC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D5FCC:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D5FE8: lifted from generated/ by lift.py.
uint64_t f825D5FE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D5FE8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r7
  r30 = r7;
  // mr r7,r8
  r7 = r8;
  // mr r8,r9
  r8 = r9;
  // mr r9,r10
  r9 = r10;
  // mr r31,r3
  r31 = r3;
  // bl 0x825d8aa8
  // (left out: dead in C++)
  r3 = f825D8AA8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lwz r9,196(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 196);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,27380
  r11 = uint64_t(int64_t(int64_t(r11) + 27380));
  // stw r30,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r30));
  // mr r3,r31
  r3 = r31;
  // stw r10,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r9,68(r31)
  REX_STORE_U32(uint32_t(r31) + 68, uint32_t(r9));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D6050: lifted from generated/ by lift.py.
lifted::Result f825D6050(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D6050 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,27380
  r11 = uint64_t(int64_t(int64_t(r11) + 27380));
  // lwz r10,64(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 64);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d60a0
  if (cr6.eq) goto loc_825D60A0;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // mr r4,r31
  r4 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,100(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 100);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D60A0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D60A0:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d8b18
  // (left out: dead in C++)
  f825D8B18(base, r1, r13, r3, 0x825D60A8);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D60C0: lifted from generated/ by lift.py.
uint64_t f825D60C0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D60C0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lhz r11,56(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d60e0
  if (cr6.eq) goto loc_825D60E0;
  // lwz r11,52(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 52);
  // lhz r10,2(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 2);
  // lhz r11,0(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 0);
  // mullw r3,r10,r11
  r3 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // blr
  return r3;
loc_825D60E0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825D60E8: lifted from generated/ by lift.py.
lifted::Result f825D60E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D60E8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r30,r4
  r30 = r4;
  // addi r11,r11,27380
  r11 = uint64_t(int64_t(int64_t(r11) + 27380));
  // lwz r10,64(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 64);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d6140
  if (cr6.eq) goto loc_825D6140;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // mr r4,r31
  r4 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,100(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 100);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D6140, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D6140:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d8b18
  // (left out: dead in C++)
  f825D8B18(base, r1, r13, r3, 0x825D6148);
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d6168
  if (cr6.eq) goto loc_825D6168;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32779
  r4 = r4 | 32779;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D6164, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D6168:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D6180: lifted from generated/ by lift.py.
lifted::Result f825D6180(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_825D6180 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r28,r4
  r28 = r4;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r30,48(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r29,296(r30)
  r29 = REX_LOAD_U32(uint32_t(r30) + 296);
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x825d6240
  if (cr6.eq) goto loc_825D6240;
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,81
  r3 = uint64_t(int64_t(int64_t(r1) + 81));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x825D61C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r9,304(r30)
  r9 = REX_LOAD_U16(uint32_t(r30) + 304);
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // rlwinm r8,r9,24,8,31
  r8 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 24) & 0xFFFFFF;
  // lwz r10,104(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 104);
  // stb r30,98(r1)
  REX_STORE_U8(uint32_t(r1) + 98, uint8_t(r30));
  // rldicr r10,r10,8,63
  r10 = __builtin_rotateleft64(r10, 8) & 0xFFFFFFFFFFFFFFFF;
  // stb r28,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r28));
  // stb r9,90(r1)
  REX_STORE_U8(uint32_t(r1) + 90, uint8_t(r9));
  // srawi r9,r30,24
  ca = (int32_t(r30) < 0) & ((uint32_t(r30) & 0xFFFFFF) != 0);
  r9 = uint64_t(int64_t(int32_t(r30) >> 24));
  // lwz r11,7(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 7);
  // stb r8,89(r1)
  REX_STORE_U8(uint32_t(r1) + 89, uint8_t(r8));
  // stb r29,94(r1)
  REX_STORE_U8(uint32_t(r1) + 94, uint8_t(r29));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stb r9,95(r1)
  REX_STORE_U8(uint32_t(r1) + 95, uint8_t(r9));
  // rldicl r9,r30,56,8
  r9 = __builtin_rotateleft64(r30, 56) & 0xFFFFFFFFFFFFFF;
  // stb r11,102(r1)
  REX_STORE_U8(uint32_t(r1) + 102, uint8_t(r11));
  // ld r4,80(r1)
  r4 = REX_LOAD_U64(uint32_t(r1) + 80);
  // sth r9,96(r1)
  REX_STORE_U16(uint32_t(r1) + 96, uint16_t(r9));
  // lbz r9,108(r1)
  r9 = REX_LOAD_U8(uint32_t(r1) + 108);
  // or r10,r9,r10
  r10 = r9 | r10;
  // rlwinm r9,r11,8,24,31
  r9 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFF;
  // rldicr r7,r10,24,39
  r7 = __builtin_rotateleft64(r10, 24) & 0xFFFFFFFFFF000000;
  // stb r9,99(r1)
  REX_STORE_U8(uint32_t(r1) + 99, uint8_t(r9));
  // rldicl r9,r11,56,8
  r9 = __builtin_rotateleft64(r11, 56) & 0xFFFFFFFFFFFFFF;
  // srawi r11,r29,24
  ca = (int32_t(r29) < 0) & ((uint32_t(r29) & 0xFFFFFF) != 0);
  r11 = uint64_t(int64_t(int32_t(r29) >> 24));
  // sth r9,100(r1)
  REX_STORE_U16(uint32_t(r1) + 100, uint16_t(r9));
  // stb r11,91(r1)
  REX_STORE_U8(uint32_t(r1) + 91, uint8_t(r11));
  // rldicl r11,r29,56,8
  r11 = __builtin_rotateleft64(r29, 56) & 0xFFFFFFFFFFFFFF;
  // ld r6,96(r1)
  r6 = REX_LOAD_U64(uint32_t(r1) + 96);
  // sth r11,92(r1)
  REX_STORE_U16(uint32_t(r1) + 92, uint16_t(r11));
  // ld r5,88(r1)
  r5 = REX_LOAD_U64(uint32_t(r1) + 88);
  // bl 0x825c1388
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C1388, LIFTED_ARGS(0x825D6240, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D6240:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D6248: lifted from generated/ by lift.py.
lifted::Result f825D6248(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D6248 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d8b28
  // (left out: dead in C++)
  { const lifted::Result o = f825D8B28(base, r1, r13, r3, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x825d6284
  if (cr6.lt) goto loc_825D6284;
  // bne cr6,0x825d6290
  if (!cr6.eq) goto loc_825D6290;
  // lwz r11,68(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 68);
  // lbz r10,25(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 25);
  // ori r10,r10,8
  r10 = r10 | 8;
  // stb r10,25(r11)
  REX_STORE_U8(uint32_t(r11) + 25, uint8_t(r10));
  // b 0x825d6290
  goto loc_825D6290;
loc_825D6284:
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x825d6180
  // (left out: dead in C++)
  { const lifted::Result o = f825D6180(base, r1, r13, r3, r4, f1); r3 = o.r3; f1 = o.f1; }
loc_825D6290:
  // lwz r11,64(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 64);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825d62c8
  if (cr6.eq) goto loc_825D62C8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,44(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 44);
  // mr r4,r31
  r4 = r31;
  // stw r11,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r11));
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,96(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 96);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D62C8, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D62C8:
  // lhz r11,56(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 56);
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d62f0
  if (cr6.eq) goto loc_825D62F0;
  // bl 0x825d8bc8
  // (left out: dead in C++)
  { const lifted::Result o = f825D8BC8(base, r1, r13, r3, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_825D62F0:
  // bl 0x825d13f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D13F0, LIFTED_ARGS(0x825D62F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D6308: lifted from generated/ by lift.py.
uint64_t f825D6308(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_825D6308 (lifted)");
  uint64_t r9 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // mr r30,r6
  r30 = r6;
  // bl 0x825d8aa8
  // (left out: dead in C++)
  r3 = f825D8AA8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r4,r30,8
  r4 = uint64_t(int64_t(int64_t(r30) + 8));
  // addi r11,r11,27400
  r11 = uint64_t(int64_t(int64_t(r11) + 27400));
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d8d50
  // (left out: dead in C++)
  f825D8D50(base, r1, r3, r4);
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D6360: lifted from generated/ by lift.py.
lifted::Result f825D6360(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D6360 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,27400
  r11 = uint64_t(int64_t(int64_t(r11) + 27400));
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x825D6390, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825d8b18
  // (left out: dead in C++)
  f825D8B18(base, r1, r13, r3, 0x825D6398);
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d63b8
  if (cr6.eq) goto loc_825D63B8;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32786
  r4 = r4 | 32786;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D63B4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D63B8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D63D0: lifted from generated/ by lift.py.
lifted::Result f825D63D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D63D0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // bl 0x825d1358
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1358, LIFTED_ARGS(0x825D63F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lbz r11,7(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 7);
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d646c
  if (cr6.eq) goto loc_825D646C;
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // bl 0x825d8da0
  // (left out: dead in C++)
  r3 = f825D8DA0(base, r1, r13, r3);
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // lfs f0,68(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 68);
  f0 = double(temp.f32);
  // rlwinm r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x825d64e4
  if (!cr6.eq) goto loc_825D64E4;
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r8,32(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 32);
  // mr r4,r31
  r4 = r31;
  // lwz r9,24(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 24);
  // stw r10,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r10));
  // subf r10,r8,r9
  r10 = r9 - r8;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r10));
  // lbz r10,44(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 44);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stb r10,44(r11)
  REX_STORE_U8(uint32_t(r11) + 44, uint8_t(r10));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r3,20(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 20);
  // bl 0x825cb0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CB0C8, LIFTED_ARGS(0x825D6468, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825d64e4
  goto loc_825D64E4;
loc_825D646C:
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lfs f1,40(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 40);
  f1 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x825d6490
  if (cr6.gt) goto loc_825D6490;
  // li r11,-9600
  r11 = uint64_t(int64_t(-9600));
  // b 0x825d64b0
  goto loc_825D64B0;
loc_825D6490:
  // bl 0x825a9d80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A9D80, LIFTED_ARGS(0x825D6494, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfd f0,27136(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 27136)));
  // fmul f0,f1,f0
  f0 = f1 * f0;
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r10
  REX_STORE_U32(uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
loc_825D64B0:
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // addi r5,r11,8
  r5 = uint64_t(int64_t(int64_t(r11) + 8));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lfs f1,10764(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10764);
  f1 = double(temp.f32);
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // bl 0x825bfb38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFB38, LIFTED_ARGS(0x825D64E4, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D64E4:
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,88(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 88);
  // cmpwi cr6,r11,6400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6400, so);
  // ble cr6,0x825d6504
  if (!cr6.gt) goto loc_825D6504;
  // li r11,6400
  r11 = uint64_t(int64_t(6400));
loc_825D6504:
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f0
  f13 = double(std::bit_cast<int64_t>(f0));
  // lfd f1,-2264(r11)
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -2264)));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfd f0,-18128(r11)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -18128)));
  // fmul f2,f13,f0
  f2 = f13 * f0;
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS(0x825D652C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // frsp f1,f1
  lifted::ScalarFloatMode();
  f1 = double(float(f1));
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d0bf8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0BF8, LIFTED_ARGS(0x825D653C, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,56(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d6554
  if (cr6.eq) goto loc_825D6554;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d8bc8
  // (left out: dead in C++)
  { const lifted::Result o = f825D8BC8(base, r1, r13, r3, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // b 0x825d6564
  goto loc_825D6564;
loc_825D6554:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x825d6564
  if (cr6.eq) goto loc_825D6564;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d13f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D13F0, LIFTED_ARGS(0x825D6564, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D6564:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D6580: lifted from generated/ by lift.py.
lifted::Result f825D6580(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D6580 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-72(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -72, std::bit_cast<uint64_t>(f31));
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // rlwinm r29,r10,27,8,31
  r29 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0xFFFFFF;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D65B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r30
  r26 = r30;
  // cmpw cr6,r29,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r30), so);
  // bge cr6,0x825d67d4
  if (!cr6.lt) goto loc_825D67D4;
  // add r11,r3,r29
  r11 = r3 + r29;
  // cmpw cr6,r11,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r30), so);
  // blt cr6,0x825d67d4
  if (cr6.lt) goto loc_825D67D4;
  // lhz r27,56(r31)
  r27 = REX_LOAD_U16(uint32_t(r31) + 56);
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825d6638
  if (cr6.eq) goto loc_825D6638;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d8c78
  // (left out: dead in C++)
  { const lifted::Result o = f825D8C78(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // mr r25,r3
  r25 = r3;
  // lhz r10,0(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 0);
  // cmplwi cr6,r10,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 65535, so);
  // beq cr6,0x825d6614
  if (cr6.eq) goto loc_825D6614;
  // lhz r11,56(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 56);
  // clrlwi r10,r27,16
  r10 = uint32_t(r27) & 0xFFFF;
  // subf r11,r11,r10
  r11 = r10 - r11;
  // clrlwi r28,r11,16
  r28 = uint32_t(r11) & 0xFFFF;
  // b 0x825d6638
  goto loc_825D6638;
loc_825D6614:
  // subf r10,r29,r30
  r10 = r30 - r29;
  // lhz r9,2(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 2);
  // rotlwi r11,r10,1
  r11 = __builtin_rotateleft32(uint32_t(r10), 1);
  // divw r10,r10,r9
  r10 = uint32_t((int32_t(r9) && !(int32_t(r10) == INT32_MIN && int32_t(r9) == -1)) ? int32_t(r10) / int32_t(r9) : 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // twllei r9,0
  if (int32_t(r9) == 0 || uint32_t(r9) < 0u) lifted::Trap(0);
  // andc r11,r9,r11
  r11 = r9 & ~r11;
  // clrlwi r28,r10,16
  r28 = uint32_t(r10) & 0xFFFF;
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
loc_825D6638:
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lbz r11,7(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 7);
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d66e0
  if (cr6.eq) goto loc_825D66E0;
  // lis r10,-13108
  r10 = uint64_t(int64_t(-859045888));
  // subf r11,r29,r30
  r11 = r30 - r29;
  // ori r10,r10,52429
  r10 = r10 | 52429;
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // mulhwu r11,r11,r10
  r11 = (uint64_t(uint32_t(r11)) * uint64_t(uint32_t(r10))) >> 32;
  // rlwinm r30,r11,30,2,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // mr r4,r30
  r4 = r30;
  // bl 0x825d8e98
  // (left out: dead in C++)
  r3 = f825D8E98(base, r1, r3, r4);
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f0,68(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 68);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmpwi cr6,r11,6400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6400, so);
  // ble cr6,0x825d668c
  if (!cr6.gt) goto loc_825D668C;
  // li r11,6400
  r11 = uint64_t(int64_t(6400));
loc_825D668C:
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f0
  f13 = double(std::bit_cast<int64_t>(f0));
  // lfd f1,-2264(r11)
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -2264)));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfd f0,-18128(r11)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -18128)));
  // fmul f2,f13,f0
  f2 = f13 * f0;
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS(0x825D66B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // frsp f1,f1
  lifted::ScalarFloatMode();
  f1 = double(float(f1));
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d0bf8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0BF8, LIFTED_ARGS(0x825D66C4, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // mr r3,r25
  r3 = r25;
  // add r26,r30,r11
  r26 = r30 + r11;
  // stw r26,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r26));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-72(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_825D66E0:
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lfs f1,40(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 40);
  f1 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgt cr6,0x825d6704
  if (cr6.gt) goto loc_825D6704;
  // li r11,-9600
  r11 = uint64_t(int64_t(-9600));
  // b 0x825d6724
  goto loc_825D6724;
loc_825D6704:
  // bl 0x825a9d80
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A9D80, LIFTED_ARGS(0x825D6708, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfd f0,27136(r11)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 27136)));
  // fmul f0,f1,f0
  f0 = f1 * f0;
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r10
  REX_STORE_U32(uint32_t(r10), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
loc_825D6724:
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // clrlwi r29,r28,16
  r29 = uint32_t(r28) & 0xFFFF;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // beq cr6,0x825d677c
  if (cr6.eq) goto loc_825D677C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lfs f31,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f31 = double(temp.f32);
loc_825D6754:
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // addi r5,r11,8
  r5 = uint64_t(int64_t(int64_t(r11) + 8));
  // bl 0x825bfb38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFB38, LIFTED_ARGS(0x825D676C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // clrlwi r30,r11,16
  r30 = uint32_t(r11) & 0xFFFF;
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // blt cr6,0x825d6754
  if (cr6.lt) goto loc_825D6754;
loc_825D677C:
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r11
  REX_STORE_U32(uint32_t(r11), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,88(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 88);
  // cmpwi cr6,r11,6400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6400, so);
  // ble cr6,0x825d679c
  if (!cr6.gt) goto loc_825D679C;
  // li r11,6400
  r11 = uint64_t(int64_t(6400));
loc_825D679C:
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f0
  f13 = double(std::bit_cast<int64_t>(f0));
  // lfd f1,-2264(r11)
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -2264)));
  // lis r11,-32251
  r11 = uint64_t(int64_t(-2113601536));
  // lfd f0,-18128(r11)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -18128)));
  // fmul f2,f13,f0
  f2 = f13 * f0;
  // bl 0x825a8ed0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A8ED0, LIFTED_ARGS(0x825D67C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // frsp f1,f1
  lifted::ScalarFloatMode();
  f1 = double(float(f1));
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d0bf8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0BF8, LIFTED_ARGS(0x825D67D4, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D67D4:
  // mr r3,r25
  r3 = r25;
  // stw r26,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r26));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-72(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D67E8: lifted from generated/ by lift.py.
uint64_t f825D67E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_825D67E8 (lifted)");
  uint64_t r9 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // mr r30,r6
  r30 = r6;
  // bl 0x825d8aa8
  // (left out: dead in C++)
  r3 = f825D8AA8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r4,r30,8
  r4 = uint64_t(int64_t(int64_t(r30) + 8));
  // addi r11,r11,27420
  r11 = uint64_t(int64_t(int64_t(r11) + 27420));
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d8d50
  // (left out: dead in C++)
  f825D8D50(base, r1, r3, r4);
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D6840: lifted from generated/ by lift.py.
lifted::Result f825D6840(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D6840 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,27420
  r11 = uint64_t(int64_t(int64_t(r11) + 27420));
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x825D6870, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825d8b18
  // (left out: dead in C++)
  f825D8B18(base, r1, r13, r3, 0x825D6878);
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d6898
  if (cr6.eq) goto loc_825D6898;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32784
  r4 = r4 | 32784;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D6894, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D6898:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D68B0: lifted from generated/ by lift.py.
lifted::Result f825D68B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D68B0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stfd f30,-40(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f30));
  // stfd f31,-32(r1)
  REX_STORE_U64(uint32_t(r1) + -32, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // bl 0x825d1358
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1358, LIFTED_ARGS(0x825D68D8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r9,16(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lbz r11,7(r9)
  r11 = REX_LOAD_U8(uint32_t(r9) + 7);
  // lfs f30,10416(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 10416);
  f30 = double(temp.f32);
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f31 = double(temp.f32);
  // beq cr6,0x825d6964
  if (cr6.eq) goto loc_825D6964;
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // bl 0x825d8da0
  // (left out: dead in C++)
  r3 = f825D8DA0(base, r1, r13, r3);
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // lfs f0,68(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 68);
  f0 = double(temp.f32);
  // rlwinm r30,r11,27,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x825d69dc
  if (!cr6.eq) goto loc_825D69DC;
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r8,32(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 32);
  // mr r4,r31
  r4 = r31;
  // lwz r9,24(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 24);
  // stw r10,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r10));
  // subf r10,r8,r9
  r10 = r9 - r8;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r10));
  // lbz r10,44(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 44);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stb r10,44(r11)
  REX_STORE_U8(uint32_t(r11) + 44, uint8_t(r10));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r3,20(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 20);
  // bl 0x825cb0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CB0C8, LIFTED_ARGS(0x825D6960, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825d69dc
  goto loc_825D69DC;
loc_825D6964:
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lfs f0,44(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 44);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f31
  cr6 = lifted::CompareFloat(f0, f31);
  // blt cr6,0x825d6980
  if (cr6.lt) goto loc_825D6980;
  // li r11,2400
  r11 = uint64_t(int64_t(2400));
  // b 0x825d69a8
  goto loc_825D69A8;
loc_825D6980:
  // fcmpu cr6,f0,f30
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f30);
  // bgt cr6,0x825d6990
  if (cr6.gt) goto loc_825D6990;
  // li r11,-2400
  r11 = uint64_t(int64_t(-2400));
  // b 0x825d69a8
  goto loc_825D69A8;
loc_825D6990:
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // lfs f13,-12280(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -12280);
  f13 = double(temp.f32);
  // fmuls f0,f0,f13
  f0 = double(float(f0 * f13));
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfd f0,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, std::bit_cast<uint64_t>(f0));
  // lhz r11,86(r1)
  r11 = REX_LOAD_U16(uint32_t(r1) + 86);
loc_825D69A8:
  // mr r10,r11
  r10 = r11;
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r5,r9,8
  r5 = uint64_t(int64_t(int64_t(r9) + 8));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lfs f1,10764(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10764);
  f1 = double(temp.f32);
  // extsh r11,r10
  r11 = uint64_t(int64_t(int16_t(r10)));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // bl 0x825bfb38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFB38, LIFTED_ARGS(0x825D69DC, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D69DC:
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfd f0,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, std::bit_cast<uint64_t>(f0));
  // lhz r11,94(r1)
  r11 = REX_LOAD_U16(uint32_t(r1) + 94);
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // cmpwi cr6,r11,2400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2400, so);
  // blt cr6,0x825d6a00
  if (cr6.lt) goto loc_825D6A00;
  // fmr f1,f31
  f1 = f31;
  // b 0x825d6a30
  goto loc_825D6A30;
loc_825D6A00:
  // cmpwi cr6,r11,-2400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -2400, so);
  // bgt cr6,0x825d6a10
  if (cr6.gt) goto loc_825D6A10;
  // fmr f1,f30
  lifted::ScalarFloatMode();
  f1 = f30;
  // b 0x825d6a30
  goto loc_825D6A30;
loc_825D6A10:
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,23436(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 23436);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
loc_825D6A30:
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d0c78
  // (left out: dead in C++)
  { const lifted::Result o = f825D0C78(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lhz r11,56(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d6a54
  if (cr6.eq) goto loc_825D6A54;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d8bc8
  // (left out: dead in C++)
  { const lifted::Result o = f825D8BC8(base, r1, r13, r3, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // b 0x825d6a64
  goto loc_825D6A64;
loc_825D6A54:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq cr6,0x825d6a64
  if (cr6.eq) goto loc_825D6A64;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d13f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D13F0, LIFTED_ARGS(0x825D6A64, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D6A64:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f30,-40(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // lfd f31,-32(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -32)));
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D6A88: lifted from generated/ by lift.py.
uint64_t f825D6A88(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D6A88 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,16(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 16);
  // lbz r11,7(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 7);
  // clrlwi r11,r11,31
  r11 = uint32_t(r11) & 0x1;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d6ab0
  if (cr6.eq) goto loc_825D6AB0;
  // lwz r11,64(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 64);
  // lhz r11,12(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 12);
  // rotlwi r10,r11,2
  r10 = __builtin_rotateleft32(uint32_t(r11), 2);
  // add r3,r11,r10
  r3 = r11 + r10;
  // blr
  return r3;
loc_825D6AB0:
  // lhz r11,56(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 56);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d6ad0
  if (cr6.eq) goto loc_825D6AD0;
  // lwz r11,52(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 52);
  // lhz r10,2(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 2);
  // lhz r11,0(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 0);
  // mullw r3,r10,r11
  r3 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // blr
  return r3;
loc_825D6AD0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825D6AD8: lifted from generated/ by lift.py.
lifted::Result f825D6AD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D6AD8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-72(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -72, std::bit_cast<uint64_t>(f31));
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // rlwinm r29,r10,27,8,31
  r29 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0xFFFFFF;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D6B10, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r26,r30
  r26 = r30;
  // cmpw cr6,r29,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r29), int32_t(r30), so);
  // bgt cr6,0x825d6cdc
  if (cr6.gt) goto loc_825D6CDC;
  // add r11,r3,r29
  r11 = r3 + r29;
  // cmpw cr6,r11,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r30), so);
  // blt cr6,0x825d6cdc
  if (cr6.lt) goto loc_825D6CDC;
  // lhz r27,56(r31)
  r27 = REX_LOAD_U16(uint32_t(r31) + 56);
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825d6b90
  if (cr6.eq) goto loc_825D6B90;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d8c78
  // (left out: dead in C++)
  { const lifted::Result o = f825D8C78(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // mr r25,r3
  r25 = r3;
  // lhz r10,0(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 0);
  // cmplwi cr6,r10,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 65535, so);
  // beq cr6,0x825d6b6c
  if (cr6.eq) goto loc_825D6B6C;
  // lhz r11,56(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 56);
  // clrlwi r10,r27,16
  r10 = uint32_t(r27) & 0xFFFF;
  // subf r11,r11,r10
  r11 = r10 - r11;
  // clrlwi r28,r11,16
  r28 = uint32_t(r11) & 0xFFFF;
  // b 0x825d6b90
  goto loc_825D6B90;
loc_825D6B6C:
  // subf r10,r29,r30
  r10 = r30 - r29;
  // lhz r9,2(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 2);
  // rotlwi r11,r10,1
  r11 = __builtin_rotateleft32(uint32_t(r10), 1);
  // divw r10,r10,r9
  r10 = uint32_t((int32_t(r9) && !(int32_t(r10) == INT32_MIN && int32_t(r9) == -1)) ? int32_t(r10) / int32_t(r9) : 0);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // twllei r9,0
  if (int32_t(r9) == 0 || uint32_t(r9) < 0u) lifted::Trap(0);
  // andc r11,r9,r11
  r11 = r9 & ~r11;
  // clrlwi r28,r10,16
  r28 = uint32_t(r10) & 0xFFFF;
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
loc_825D6B90:
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lbz r10,7(r10)
  r10 = REX_LOAD_U8(uint32_t(r10) + 7);
  // clrlwi r10,r10,31
  r10 = uint32_t(r10) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825d6c08
  if (cr6.eq) goto loc_825D6C08;
  // lis r10,-13108
  r10 = uint64_t(int64_t(-859045888));
  // subf r11,r11,r30
  r11 = r30 - r11;
  // ori r10,r10,52429
  r10 = r10 | 52429;
  // addi r3,r31,64
  r3 = uint64_t(int64_t(int64_t(r31) + 64));
  // mulhwu r11,r11,r10
  r11 = (uint64_t(uint32_t(r11)) * uint64_t(uint32_t(r10))) >> 32;
  // rlwinm r30,r11,30,2,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3FFFFFFF;
  // mr r4,r30
  r4 = r30;
  // bl 0x825d8e98
  // (left out: dead in C++)
  r3 = f825D8E98(base, r1, r3, r4);
  // lfs f0,68(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 68);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfd f0,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, std::bit_cast<uint64_t>(f0));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lhz r4,86(r1)
  r4 = REX_LOAD_U16(uint32_t(r1) + 86);
  // bl 0x825c8eb8
  // (left out: dead in C++)
  f1 = f825C8EB8(base, r1, r4);
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d0c78
  // (left out: dead in C++)
  { const lifted::Result o = f825D0C78(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // mr r3,r25
  r3 = r25;
  // add r26,r30,r11
  r26 = r30 + r11;
  // stw r26,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r26));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-72(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
loc_825D6C08:
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lfs f1,44(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 44);
  f1 = double(temp.f32);
  // bl 0x825d0ea0
  // (left out: dead in C++)
  r3 = f825D0EA0(base, r1, f1);
  // extsh r11,r3
  r11 = uint64_t(int64_t(int16_t(r3)));
  // clrlwi r29,r28,16
  r29 = uint32_t(r28) & 0xFFFF;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // beq cr6,0x825d6c74
  if (cr6.eq) goto loc_825D6C74;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lfs f31,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f31 = double(temp.f32);
loc_825D6C4C:
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // addi r5,r11,8
  r5 = uint64_t(int64_t(int64_t(r11) + 8));
  // bl 0x825bfb38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFB38, LIFTED_ARGS(0x825D6C64, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // clrlwi r30,r11,16
  r30 = uint32_t(r11) & 0xFFFF;
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // blt cr6,0x825d6c4c
  if (cr6.lt) goto loc_825D6C4C;
loc_825D6C74:
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // fctiwz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x80000000U) : (f0 >= double(INT_MAX)) ? INT_MAX : simde_mm_cvttsd_si32(simde_mm_load_sd(&f0)))));
  // stfd f0,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, std::bit_cast<uint64_t>(f0));
  // lhz r11,94(r1)
  r11 = REX_LOAD_U16(uint32_t(r1) + 94);
  // extsh r11,r11
  r11 = uint64_t(int64_t(int16_t(r11)));
  // cmpwi cr6,r11,2400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2400, so);
  // blt cr6,0x825d6c9c
  if (cr6.lt) goto loc_825D6C9C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f1,8680(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8680);
  f1 = double(temp.f32);
  // b 0x825d6cd0
  goto loc_825D6CD0;
loc_825D6C9C:
  // cmpwi cr6,r11,-2400
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -2400, so);
  // bgt cr6,0x825d6cb0
  if (cr6.gt) goto loc_825D6CB0;
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f1,10416(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10416);
  f1 = double(temp.f32);
  // b 0x825d6cd0
  goto loc_825D6CD0;
loc_825D6CB0:
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,23436(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 23436);
  f0 = double(temp.f32);
  // fmuls f1,f13,f0
  f1 = double(float(f13 * f0));
loc_825D6CD0:
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d0c78
  // (left out: dead in C++)
  { const lifted::Result o = f825D0C78(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825D6CDC:
  // mr r3,r25
  r3 = r25;
  // stw r26,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r26));
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // lfd f31,-72(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D6CF0: lifted from generated/ by lift.py.
uint64_t f825D6CF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_825D6CF0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d8aa8
  // (left out: dead in C++)
  r3 = f825D8AA8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,27440
  r11 = uint64_t(int64_t(int64_t(r11) + 27440));
  // mr r3,r31
  r3 = r31;
  // stw r10,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D6D38: lifted from generated/ by lift.py.
lifted::Result f825D6D38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D6D38 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,27440
  r11 = uint64_t(int64_t(int64_t(r11) + 27440));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d8b18
  // (left out: dead in C++)
  f825D8B18(base, r1, r13, r3, 0x825D6D64);
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d6d84
  if (cr6.eq) goto loc_825D6D84;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32785
  r4 = r4 | 32785;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D6D80, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D6D84:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D6DA0: lifted from generated/ by lift.py.
lifted::Result f825D6DA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D6DA0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1358
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1358, LIFTED_ARGS(0x825D6DB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825d6dec
  if (cr6.eq) goto loc_825D6DEC;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lhz r4,7(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 7);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // b 0x825d6e00
  goto loc_825D6E00;
loc_825D6DEC:
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lhz r4,7(r10)
  r4 = REX_LOAD_U16(uint32_t(r10) + 7);
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
loc_825D6E00:
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D6E08, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // addi r5,r11,9
  r5 = uint64_t(int64_t(int64_t(r11) + 9));
  // lfs f1,2608(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f1 = double(temp.f32);
  // bl 0x825bfb38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFB38, LIFTED_ARGS(0x825D6E24, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // lfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f1 = double(temp.f32);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825d6e58
  if (cr6.eq) goto loc_825D6E58;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lhz r4,7(r10)
  r4 = REX_LOAD_U16(uint32_t(r10) + 7);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // b 0x825d6e6c
  goto loc_825D6E6C;
loc_825D6E58:
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lhz r4,7(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 7);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
loc_825D6E6C:
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D6E74, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,56(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 56);
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d6e9c
  if (cr6.eq) goto loc_825D6E9C;
  // bl 0x825d8bc8
  // (left out: dead in C++)
  { const lifted::Result o = f825D8BC8(base, r1, r13, r3, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
loc_825D6E9C:
  // bl 0x825d13f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D13F0, LIFTED_ARGS(0x825D6EA0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D6EB8: lifted from generated/ by lift.py.
lifted::Result f825D6EB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D6EB8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-64(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -64, std::bit_cast<uint64_t>(f31));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r28,r4
  r28 = r4;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lhz r4,7(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 7);
  // bl 0x825c0748
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C0748, LIFTED_ARGS(0x825D6EE4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne cr6,0x825d6ef4
  if (!cr6.eq) goto loc_825D6EF4;
  // lis r26,-30009
  r26 = uint64_t(int64_t(-1966669824));
  // ori r26,r26,10
  r26 = r26 | 10;
loc_825D6EF4:
  // mr r30,r28
  r30 = r28;
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // blt cr6,0x825d70b8
  if (cr6.lt) goto loc_825D70B8;
  // lbz r11,0(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 0);
  // rlwinm r11,r11,0,29,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x4;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d6f48
  if (cr6.eq) goto loc_825D6F48;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r10,44(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r9,16(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 16);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // stw r11,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r11));
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // lhz r4,7(r9)
  r4 = REX_LOAD_U16(uint32_t(r9) + 7);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // bl 0x825c5960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C5960, LIFTED_ARGS(0x825D6F3C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_825D6F48:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // stw r9,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r9));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r10,12(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 12);
  // rlwinm r27,r11,27,8,31
  r27 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0xFFFFFF;
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D6F70, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r28,r27
  cr6 = lifted::Compare<int32_t>(int32_t(r28), int32_t(r27), so);
  // blt cr6,0x825d70b4
  if (cr6.lt) goto loc_825D70B4;
  // add r11,r3,r27
  r11 = r3 + r27;
  // cmpw cr6,r11,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r28), so);
  // blt cr6,0x825d70b4
  if (cr6.lt) goto loc_825D70B4;
  // lhz r11,56(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 56);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d6fcc
  if (cr6.eq) goto loc_825D6FCC;
  // mr r4,r28
  r4 = r28;
  // mr r3,r31
  r3 = r31;
  // bl 0x825d8c78
  // (left out: dead in C++)
  { const lifted::Result o = f825D8C78(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // lwz r10,52(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 52);
  // subf r9,r27,r28
  r9 = r28 - r27;
  // mr r26,r3
  r26 = r3;
  // rotlwi r11,r9,1
  r11 = __builtin_rotateleft32(uint32_t(r9), 1);
  // lhz r10,2(r10)
  r10 = REX_LOAD_U16(uint32_t(r10) + 2);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // divw r9,r9,r10
  r9 = uint32_t((int32_t(r10) && !(int32_t(r9) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r9) / int32_t(r10) : 0);
  // andc r11,r10,r11
  r11 = r10 & ~r11;
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // clrlwi r30,r9,16
  r30 = uint32_t(r9) & 0xFFFF;
  // twlgei r11,-1
  if (int32_t(r11) == -1 || uint32_t(r11) > 4294967295u) lifted::Trap(0);
loc_825D6FCC:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,60(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 60);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // beq cr6,0x825d700c
  if (cr6.eq) goto loc_825D700C;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lhz r4,7(r10)
  r4 = REX_LOAD_U16(uint32_t(r10) + 7);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // b 0x825d7020
  goto loc_825D7020;
loc_825D700C:
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lhz r4,7(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 7);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
loc_825D7020:
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7028, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // clrlwi r29,r30,16
  r29 = uint32_t(r30) & 0xFFFF;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lfs f31,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f31 = double(temp.f32);
loc_825D7038:
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // addi r5,r11,9
  r5 = uint64_t(int64_t(int64_t(r11) + 9));
  // bl 0x825bfb38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFB38, LIFTED_ARGS(0x825D7050, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // clrlwi r30,r11,16
  r30 = uint32_t(r11) & 0xFFFF;
  // cmplw cr6,r30,r29
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r29), so);
  // ble cr6,0x825d7038
  if (!cr6.gt) goto loc_825D7038;
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // lfs f1,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f1 = double(temp.f32);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825d7094
  if (cr6.eq) goto loc_825D7094;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lhz r4,7(r10)
  r4 = REX_LOAD_U16(uint32_t(r10) + 7);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // b 0x825d70a8
  goto loc_825D70A8;
loc_825D7094:
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lhz r4,7(r11)
  r4 = REX_LOAD_U16(uint32_t(r11) + 7);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
loc_825D70A8:
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D70B0, r31)); r3 = o.r3; f1 = o.f1; }
  // subf r30,r27,r28
  r30 = r28 - r27;
loc_825D70B4:
  // stw r30,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r30));
loc_825D70B8:
  // mr r3,r26
  r3 = r26;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D70C8: lifted from generated/ by lift.py.
uint64_t f825D70C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825D70C8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1760
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1760, LIFTED_ARGS(0x825D70E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,27460
  r11 = uint64_t(int64_t(int64_t(r11) + 27460));
  // mr r3,r31
  r3 = r31;
  // stw r10,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D7110: lifted from generated/ by lift.py.
lifted::Result f825D7110(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D7110 (lifted)");
  uint64_t r4 = 0, r10 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1358
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1358, LIFTED_ARGS(0x825D7128, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825d13f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D13F0, LIFTED_ARGS(0x825D7130, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D7148: lifted from generated/ by lift.py.
lifted::Result f825D7148(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D7148 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,27460
  r11 = uint64_t(int64_t(int64_t(r11) + 27460));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d0ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0EF0, LIFTED_ARGS(0x825D7174, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d7194
  if (cr6.eq) goto loc_825D7194;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32788
  r4 = r4 | 32788;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D7190, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D7194:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D71C0: lifted from generated/ by lift.py.
uint64_t f825D71C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825D71C0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1760
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1760, LIFTED_ARGS(0x825D71DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,27608
  r11 = uint64_t(int64_t(int64_t(r11) + 27608));
  // mr r3,r31
  r3 = r31;
  // stw r10,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r10));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D7210: lifted from generated/ by lift.py.
lifted::Result f825D7210(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D7210 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1358
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1358, LIFTED_ARGS(0x825D7228, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d7250
  if (cr6.eq) goto loc_825D7250;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7248, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r11));
loc_825D7250:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d13f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D13F0, LIFTED_ARGS(0x825D7258, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D7270: lifted from generated/ by lift.py.
lifted::Result f825D7270(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D7270 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r3,52(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d72ac
  if (cr6.eq) goto loc_825D72AC;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7298, r31)); r3 = o.r3; f1 = o.f1; }
  // rlwinm r3,r3,30,31,31
  r3 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 30) & 0x1;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
loc_825D72AC:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825D7328: lifted from generated/ by lift.py.
lifted::Result f825D7328(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D7328 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r30,r4
  r30 = r4;
  // addi r11,r11,27608
  r11 = uint64_t(int64_t(int64_t(r11) + 27608));
  // lwz r3,52(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d736c
  if (cr6.eq) goto loc_825D736C;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D736C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D736C:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,27480
  r11 = uint64_t(int64_t(int64_t(r11) + 27480));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d0ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0EF0, LIFTED_ARGS(0x825D7380, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d73a0
  if (cr6.eq) goto loc_825D73A0;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32780
  r4 = r4 | 32780;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D739C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D73A0:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D73B8: lifted from generated/ by lift.py.
lifted::Result f825D73B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825D73B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r10,16(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r4,7(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + 7);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r3,296(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 296);
  // bl 0x825c7358
  // (left out: dead in C++)
  { const lifted::Result o = f825C7358(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825d7400
  if (cr6.lt) goto loc_825D7400;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r11));
loc_825D7400:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D75A8: lifted from generated/ by lift.py.
lifted::Result f825D75A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D75A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D75D0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d7608
  if (cr6.eq) goto loc_825D7608;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D75EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r4,r30
  r4 = r30;
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7600, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825D7608:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D7618: lifted from generated/ by lift.py.
lifted::Result f825D7618(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D7618 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1358
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1358, LIFTED_ARGS(0x825D7630, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7644, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d7678
  if (cr6.eq) goto loc_825D7678;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7668, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825d7678
  if (cr6.lt) goto loc_825D7678;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFE28, LIFTED_ARGS(0x825D7674, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r3));
loc_825D7678:
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFE28, LIFTED_ARGS(0x825D767C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r3));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D7698: lifted from generated/ by lift.py.
uint64_t f825D7698(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825D7698 (lifted)");
  double f1 = 0.0;
  // addi r3,r3,52
  r3 = uint64_t(int64_t(int64_t(r3) + 52));
  // b 0x825d35a8
  r3 = f825D35A8(base, r1, r13, r3, r4, r5);
  return r3;
}

// sub_825D7790: lifted from generated/ by lift.py.
lifted::Result f825D7790(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D7790 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D77B8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d7828
  if (cr6.eq) goto loc_825D7828;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D77D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r10,56(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 56);
  // lhz r30,52(r11)
  r30 = REX_LOAD_U16(uint32_t(r11) + 52);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D77F0, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // lwz r3,36(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 36);
  // mr r4,r30
  r4 = r30;
  // bl 0x825cc380
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CC380, LIFTED_ARGS(0x825D7804, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825d7828
  if (cr6.lt) goto loc_825D7828;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lbz r11,8(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 8);
  // mullw r11,r11,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r10))));
  // lwz r10,84(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 84);
  // add r3,r11,r10
  r3 = r11 + r10;
  // b 0x825d782c
  goto loc_825D782C;
loc_825D7828:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_825D782C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D7848: lifted from generated/ by lift.py.
lifted::Result f825D7848(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D7848 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x825d786c
  if (!cr6.eq) goto loc_825D786C;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFE28, LIFTED_ARGS(0x825D7868, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r3,92(r31)
  REX_STORE_U32(uint32_t(r31) + 92, uint32_t(r3));
loc_825D786C:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7880, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d78fc
  if (cr6.eq) goto loc_825D78FC;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // lbz r11,7(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 7);
  // clrlwi r29,r11,31
  r29 = uint32_t(r11) & 0x1;
  // bne cr6,0x825d78bc
  if (!cr6.eq) goto loc_825D78BC;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x825d78c0
  if (!cr6.eq) goto loc_825D78C0;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d05c0
  // (left out: dead in C++)
  { const lifted::Result o = f825D05C0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825d78c0
  if (!cr6.eq) goto loc_825D78C0;
loc_825D78BC:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_825D78C0:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D78D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D78F4, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_825D78FC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D7908: lifted from generated/ by lift.py.
lifted::Result f825D7908(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D7908 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7928, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r10,56(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 56);
  // lhz r30,52(r11)
  r30 = REX_LOAD_U16(uint32_t(r11) + 52);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7944, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,36(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 36);
  // mr r4,r30
  r4 = r30;
  // bl 0x825cc380
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CC380, LIFTED_ARGS(0x825D7958, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825d79e0
  if (cr6.lt) goto loc_825D79E0;
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r10,80(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x825d7984
  if (!cr6.lt) goto loc_825D7984;
  // subf r11,r11,r10
  r11 = r10 - r11;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // b 0x825d7988
  goto loc_825D7988;
loc_825D7984:
  // stw r29,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r29));
loc_825D7988:
  // addi r30,r31,100
  r30 = uint64_t(int64_t(int64_t(r31) + 100));
  // mr r3,r30
  r3 = r30;
  // bl 0x825d0e78
  // (left out: dead in C++)
  f825D0E78(base, r3);
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D79A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r10,8192
  r10 = uint64_t(int64_t(536870912));
  // mr r11,r3
  r11 = r3;
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // ori r10,r10,41
  r10 = r10 | 41;
  // sth r29,80(r31)
  REX_STORE_U16(uint32_t(r31) + 80, uint16_t(r29));
  // li r9,255
  r9 = uint64_t(int64_t(255));
  // stw r29,83(r31)
  REX_STORE_U32(uint32_t(r31) + 83, uint32_t(r29));
  // addi r4,r31,172
  r4 = uint64_t(int64_t(int64_t(r31) + 172));
  // stw r11,168(r31)
  REX_STORE_U32(uint32_t(r31) + 168, uint32_t(r11));
  // stw r10,76(r31)
  REX_STORE_U32(uint32_t(r31) + 76, uint32_t(r10));
  // stb r9,82(r31)
  REX_STORE_U8(uint32_t(r31) + 82, uint8_t(r9));
  // bl 0x825d5de8
  // (left out: dead in C++)
  f825D5DE8(base, r3, r4);
  // mr r3,r30
  r3 = r30;
  // bl 0x825d1918
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1918, LIFTED_ARGS(0x825D79E0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D79E0:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D79E8: lifted from generated/ by lift.py.
lifted::Result f825D79E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_825D79E8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFE28, LIFTED_ARGS(0x825D7A00, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r11,r3
  r11 = r3;
  // lwz r9,96(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 96);
  // mr r3,r31
  r3 = r31;
  // subf r31,r9,r11
  r31 = r11 - r9;
  // lwz r10,56(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 56);
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7A20, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7A30, r31)); r3 = o.r3; f1 = o.f1; }
  // subf r3,r31,r3
  r3 = r3 - r31;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D7A48: lifted from generated/ by lift.py.
lifted::Result f825D7A48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D7A48 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,104(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 104);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7A6C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825d7a88
  if (cr6.eq) goto loc_825D7A88;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r3,20(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 20);
  // bl 0x825cadf0
  // (left out: dead in C++)
  { const lifted::Result o = f825CADF0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, 0x825D7A88); r3 = o.r3; f1 = o.f1; }
loc_825D7A88:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D7AA0: lifted from generated/ by lift.py.
uint64_t f825D7AA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825D7AA0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r27,r5
  r27 = r5;
  // bl 0x825d1760
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1760, LIFTED_ARGS(0x825D7AC0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r11,r11,27744
  r11 = uint64_t(int64_t(int64_t(r11) + 27744));
  // stw r30,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r30));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // addi r5,r11,16
  r5 = uint64_t(int64_t(int64_t(r11) + 16));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi r10,r10,27
  r10 = uint32_t(r10) & 0x1F;
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x825d7af0
  if (cr6.eq) goto loc_825D7AF0;
  // addi r5,r11,9
  r5 = uint64_t(int64_t(int64_t(r11) + 9));
loc_825D7AF0:
  // mr r4,r29
  r4 = r29;
  // addi r3,r31,52
  r3 = uint64_t(int64_t(int64_t(r31) + 52));
  // bl 0x825d3360
  // (left out: dead in C++)
  f825D3360(base, r3, r4, r5);
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // addi r28,r31,100
  r28 = uint64_t(int64_t(int64_t(r31) + 100));
  // stw r30,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r30));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lbz r11,8(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 8);
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // addi r6,r31,76
  r6 = uint64_t(int64_t(int64_t(r31) + 76));
  // stw r30,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r30));
  // mr r5,r27
  r5 = r27;
  // stw r30,88(r31)
  REX_STORE_U32(uint32_t(r31) + 88, uint32_t(r30));
  // mr r4,r29
  r4 = r29;
  // stw r30,92(r31)
  REX_STORE_U32(uint32_t(r31) + 92, uint32_t(r30));
  // mr r3,r28
  r3 = r28;
  // stw r30,96(r31)
  REX_STORE_U32(uint32_t(r31) + 96, uint32_t(r30));
  // stb r11,68(r31)
  REX_STORE_U8(uint32_t(r31) + 68, uint8_t(r11));
  // bl 0x825d5fe8
  // (left out: dead in C++)
  r3 = f825D5FE8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10);
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r11,r31,172
  r11 = uint64_t(int64_t(int64_t(r31) + 172));
  // addi r10,r10,27736
  r10 = uint64_t(int64_t(int64_t(r10) + 27736));
  // mr r3,r31
  r3 = r31;
  // stw r28,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r28));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r11,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r11));
  // stw r11,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r11));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_825D7B70: lifted from generated/ by lift.py.
uint64_t f825D7B70(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D7B70 (lifted)");
  // lbz r3,68(r3)
  r3 = REX_LOAD_U8(uint32_t(r3) + 68);
  // blr
  return r3;
}

// sub_825D7B80: lifted from generated/ by lift.py.
uint64_t f825D7B80(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D7B80 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,16(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 16);
  // addi r3,r11,9
  r3 = uint64_t(int64_t(int64_t(r11) + 9));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // clrlwi r10,r10,27
  r10 = uint32_t(r10) & 0x1F;
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beqlr cr6
  if (cr6.eq) return r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825D7BA0: lifted from generated/ by lift.py.
lifted::Result f825D7BA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D7BA0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,27744
  r11 = uint64_t(int64_t(int64_t(r11) + 27744));
  // lwz r3,72(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 72);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d7be0
  if (cr6.eq) goto loc_825D7BE0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7BE0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D7BE0:
  // addi r11,r31,172
  r11 = uint64_t(int64_t(int64_t(r31) + 172));
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // addi r3,r31,100
  r3 = uint64_t(int64_t(int64_t(r31) + 100));
  // addi r10,r10,21316
  r10 = uint64_t(int64_t(int64_t(r10) + 21316));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,8(r8)
  REX_STORE_U32(uint32_t(r8) + 8, uint32_t(r9));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r8,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r8));
  // stw r11,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r11));
  // stw r11,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r11));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x825d6050
  // (left out: dead in C++)
  { const lifted::Result o = f825D6050(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,52
  r3 = uint64_t(int64_t(int64_t(r31) + 52));
  // bl 0x825d2ad8
  // (left out: dead in C++)
  f825D2AD8(base, r3);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,27480
  r11 = uint64_t(int64_t(int64_t(r11) + 27480));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d0ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0EF0, LIFTED_ARGS(0x825D7C34, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D7C48: lifted from generated/ by lift.py.
lifted::Result f825D7C48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D7C48 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // mr r29,r26
  r29 = r26;
  // mr r27,r26
  r27 = r26;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // stw r26,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r26));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r11,r11,27,8,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0xFFFFFF;
  // cmpw cr6,r11,r4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r4), so);
  // bgt cr6,0x825d7c88
  if (cr6.gt) goto loc_825D7C88;
  // subf r28,r11,r4
  r28 = r4 - r11;
  // stw r11,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r11));
  // b 0x825d7c90
  goto loc_825D7C90;
loc_825D7C88:
  // mr r28,r26
  r28 = r26;
  // stw r4,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r4));
loc_825D7C90:
  // lwz r11,84(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 84);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825d7e64
  if (!cr6.eq) goto loc_825D7E64;
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // blt cr6,0x825d7e64
  if (cr6.lt) goto loc_825D7E64;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7CB8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,82
  r4 = uint64_t(int64_t(int64_t(r1) + 82));
  // mr r3,r31
  r3 = r31;
  // lwz r11,108(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 108);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7CD4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825d7e2c
  if (cr6.lt) goto loc_825D7E2C;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lbz r4,80(r1)
  r4 = REX_LOAD_U8(uint32_t(r1) + 80);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r3,296(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 296);
  // bl 0x825c7218
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825C7218, LIFTED_ARGS(0x825D7CFC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825d7e24
  if (cr6.eq) goto loc_825D7E24;
  // lwz r11,252(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 252);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x825d7e58
  if (cr6.eq) goto loc_825D7E58;
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // lhz r4,82(r1)
  r4 = REX_LOAD_U16(uint32_t(r1) + 82);
  // mr r5,r31
  r5 = r31;
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7D30, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825d7e2c
  if (cr6.lt) goto loc_825D7E2C;
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r11,r11,0,9,9
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x400000;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d7d68
  if (cr6.eq) goto loc_825D7D68;
  // lwz r3,84(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 84);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7D64, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825d7d94
  goto loc_825D7D94;
loc_825D7D68:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7D7C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lwz r3,84(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 84);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7D94, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D7D94:
  // mr r30,r3
  r30 = r3;
  // cmpw cr6,r30,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r28), so);
  // bge cr6,0x825d7c90
  if (!cr6.lt) goto loc_825D7C90;
  // lwz r3,84(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 84);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d7dc0
  if (cr6.eq) goto loc_825D7DC0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7DC0, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D7DC0:
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // stw r26,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r26));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r11,r11,0,9,9
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x400000;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d7e1c
  if (cr6.eq) goto loc_825D7E1C;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r11,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 255, so);
  // beq cr6,0x825d7e14
  if (cr6.eq) goto loc_825D7E14;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7E0C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,255
  r11 = uint64_t(int64_t(int64_t(r3) + 255));
  // stb r11,68(r31)
  REX_STORE_U8(uint32_t(r31) + 68, uint8_t(r11));
loc_825D7E14:
  // subf r28,r30,r28
  r28 = r28 - r30;
  // b 0x825d7c90
  goto loc_825D7C90;
loc_825D7E1C:
  // li r28,-1
  r28 = uint64_t(int64_t(-1));
  // b 0x825d7c90
  goto loc_825D7C90;
loc_825D7E24:
  // lis r29,-30009
  r29 = uint64_t(int64_t(-1966669824));
  // ori r29,r29,19
  r29 = r29 | 19;
loc_825D7E2C:
  // lwz r3,84(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 84);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825d7e4c
  if (cr6.eq) goto loc_825D7E4C;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7E4C, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D7E4C:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_825D7E58:
  // lis r29,-30009
  r29 = uint64_t(int64_t(-1966669824));
  // ori r29,r29,22
  r29 = r29 | 22;
  // b 0x825d7e2c
  goto loc_825D7E2C;
loc_825D7E64:
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825d7e2c
  if (cr6.lt) goto loc_825D7E2C;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825d7e84
  if (!cr6.eq) goto loc_825D7E84;
  // lis r3,-30009
  r3 = uint64_t(int64_t(-1966669824));
  // ori r3,r3,25
  r3 = r3 | 25;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_825D7E84:
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // mr r3,r31
  r3 = r31;
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r10,48(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 48);
  // rlwinm r30,r11,10,31,31
  r30 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 10) & 0x1;
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7EA4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // lwz r3,84(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 84);
  // mr r6,r28
  r6 = r28;
  // mr r5,r30
  r5 = r30;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7EC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // blt cr6,0x825d7e2c
  if (cr6.lt) goto loc_825D7E2C;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r4,84(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 84);
  // lwz r11,104(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 104);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D7EE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,84(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 84);
  // addi r11,r27,240
  r11 = uint64_t(int64_t(int64_t(r27) + 240));
  // mr r3,r29
  r3 = r29;
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r10,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825D7F18: lifted from generated/ by lift.py.
lifted::Result f825D7F18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_825D7F18 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // addi r11,r11,21316
  r11 = uint64_t(int64_t(int64_t(r11) + 21316));
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r9,4(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r10,8(r9)
  REX_STORE_U32(uint32_t(r9) + 8, uint32_t(r10));
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r9,4(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r9,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r9));
  // stw r31,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r31));
  // stw r31,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r31));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d7f74
  if (cr6.eq) goto loc_825D7F74;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32812
  r4 = r4 | 32812;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D7F70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D7F74:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D7F88: lifted from generated/ by lift.py.
lifted::Result f825D7F88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D7F88 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x825d7ba0
  // (left out: dead in C++)
  { const lifted::Result o = f825D7BA0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d7fc8
  if (cr6.eq) goto loc_825D7FC8;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32782
  r4 = r4 | 32782;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D7FC4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D7FC8:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D8950: lifted from generated/ by lift.py.
uint64_t f825D8950(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825D8950 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1760
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1760, LIFTED_ARGS(0x825D896C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r3,r31
  r3 = r31;
  // addi r11,r11,28008
  r11 = uint64_t(int64_t(int64_t(r11) + 28008));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D8990: lifted from generated/ by lift.py.
lifted::Result f825D8990(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D8990 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,28008
  r11 = uint64_t(int64_t(int64_t(r11) + 28008));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d0ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0EF0, LIFTED_ARGS(0x825D89BC, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d89dc
  if (cr6.eq) goto loc_825D89DC;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32787
  r4 = r4 | 32787;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D89D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D89DC:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D89F8: lifted from generated/ by lift.py.
lifted::Result f825D89F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D89F8 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lbz r11,7(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 7);
  // clrlwi r30,r11,31
  r30 = uint32_t(r11) & 0x1;
  // bl 0x825d1358
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1358, LIFTED_ARGS(0x825D8A20, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lbz r11,7(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 7);
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d8a5c
  if (cr6.eq) goto loc_825D8A5C;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // mr r4,r30
  r4 = r30;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r3,48(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 48);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D8A58, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x825d8a88
  goto loc_825D8A88;
loc_825D8A5C:
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x825d8a7c
  if (!cr6.eq) goto loc_825D8A7C;
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r3,16(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 16);
  // bl 0x825d05c0
  // (left out: dead in C++)
  { const lifted::Result o = f825D05C0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x825d8a7c
  if (!cr6.eq) goto loc_825D8A7C;
  // li r30,1
  r30 = uint64_t(int64_t(1));
loc_825D8A7C:
  // mr r4,r30
  r4 = r30;
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // bl 0x825d5d18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D5D18, LIFTED_ARGS(0x825D8A88, r31)); r3 = o.r3; f1 = o.f1; }
loc_825D8A88:
  // mr r3,r31
  r3 = r31;
  // bl 0x825d13f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D13F0, LIFTED_ARGS(0x825D8A90, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D8AA8: lifted from generated/ by lift.py.
uint64_t f825D8AA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D8AA8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r8
  r30 = r8;
  // mr r8,r9
  r8 = r9;
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1760
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1760, LIFTED_ARGS(0x825D8ACC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r30,52(r31)
  REX_STORE_U32(uint32_t(r31) + 52, uint32_t(r30));
  // addi r11,r11,28028
  r11 = uint64_t(int64_t(int64_t(r11) + 28028));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // sth r10,48(r31)
  REX_STORE_U16(uint32_t(r31) + 48, uint16_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825d8af4
  if (cr6.eq) goto loc_825D8AF4;
  // lhz r11,0(r30)
  r11 = REX_LOAD_U16(uint32_t(r30) + 0);
  // b 0x825d8af8
  goto loc_825D8AF8;
loc_825D8AF4:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825D8AF8:
  // mr r3,r31
  r3 = r31;
  // sth r11,56(r31)
  REX_STORE_U16(uint32_t(r31) + 56, uint16_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D8B18: lifted from generated/ by lift.py.
void f825D8B18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_825D8B18 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,28028
  r11 = uint64_t(int64_t(int64_t(r11) + 28028));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // b 0x825d0ef0
  { const lifted::Result o = lifted::Call(base, 0x825D0EF0, LIFTED_ARGS(lr, r31)); r3 = o.r3; f1 = o.f1; }
  return;
}

// sub_825D8B28: lifted from generated/ by lift.py.
lifted::Result f825D8B28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_825D8B28 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // bl 0x825d1358
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D1358, LIFTED_ARGS(0x825D8B40, r31)); r3 = o.r3; f1 = o.f1; }
  // lhz r11,48(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 48);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // sth r11,48(r31)
  REX_STORE_U16(uint32_t(r31) + 48, uint16_t(r11));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D8B60: lifted from generated/ by lift.py.
lifted::Result f825D8B60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825D8B60 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,28028
  r11 = uint64_t(int64_t(int64_t(r11) + 28028));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825d0ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825D0EF0, LIFTED_ARGS(0x825D8B8C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825d8bac
  if (cr6.eq) goto loc_825D8BAC;
  // lis r4,8324
  r4 = uint64_t(int64_t(545521664));
  // ori r4,r4,32866
  r4 = r4 | 32866;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825D8BA8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825D8BAC:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D8BC8: lifted from generated/ by lift.py.
lifted::Result f825D8BC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_825D8BC8 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lhz r11,56(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 56);
  // cmplwi cr6,r11,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 65535, so);
  // beq cr6,0x825d8bf4
  if (cr6.eq) goto loc_825D8BF4;
  // addis r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 65536));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // sth r11,56(r31)
  REX_STORE_U16(uint32_t(r31) + 56, uint16_t(r11));
loc_825D8BF4:
  // lwz r10,52(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 52);
  // lwz r9,20(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lhz r10,2(r10)
  r10 = REX_LOAD_U16(uint32_t(r10) + 2);
  // add r10,r10,r9
  r10 = r10 + r9;
  // stw r10,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r10,r10,0,0,0
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x80000000;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x825d8c40
  if (cr6.eq) goto loc_825D8C40;
  // lhz r5,4(r11)
  r5 = REX_LOAD_U16(uint32_t(r11) + 4);
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // beq cr6,0x825d8c40
  if (cr6.eq) goto loc_825D8C40;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // bl 0x825bf650
  // (left out: dead in C++)
  r3 = f825BF650(base, r1, r13, r3, r4, r5);
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // add r11,r11,r3
  r11 = r11 + r3;
  // stw r11,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r11));
loc_825D8C40:
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // mr r4,r31
  r4 = r31;
  // lbz r10,44(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 44);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stb r10,44(r11)
  REX_STORE_U8(uint32_t(r11) + 44, uint8_t(r10));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r3,20(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 20);
  // bl 0x825cb0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825CB0C8, LIFTED_ARGS(0x825D8C64, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D8C78: lifted from generated/ by lift.py.
lifted::Result f825D8C78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825D8C78 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x825D8CA4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm r10,r11,27,8,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0xFFFFFF;
  // ble cr6,0x825d8d2c
  if (!cr6.gt) goto loc_825D8D2C;
  // lhz r9,56(r31)
  r9 = REX_LOAD_U16(uint32_t(r31) + 56);
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x825d8d2c
  if (cr6.eq) goto loc_825D8D2C;
  // cmpw cr6,r10,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r30), so);
  // bge cr6,0x825d8d2c
  if (!cr6.lt) goto loc_825D8D2C;
  // add r11,r10,r3
  r11 = r10 + r3;
  // cmpw cr6,r11,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r30), so);
  // blt cr6,0x825d8d2c
  if (cr6.lt) goto loc_825D8D2C;
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // subf r8,r10,r30
  r8 = r30 - r10;
  // cmplwi cr6,r9,65535
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 65535, so);
  // rotlwi r10,r8,1
  r10 = __builtin_rotateleft32(uint32_t(r8), 1);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // lhz r9,2(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 2);
  // divw r8,r8,r9
  r8 = uint32_t((int32_t(r9) && !(int32_t(r8) == INT32_MIN && int32_t(r9) == -1)) ? int32_t(r8) / int32_t(r9) : 0);
  // twllei r9,0
  if (int32_t(r9) == 0 || uint32_t(r9) < 0u) lifted::Trap(0);
  // andc r9,r9,r10
  r9 = r9 & ~r10;
  // clrlwi r10,r8,16
  r10 = uint32_t(r8) & 0xFFFF;
  // twlgei r9,-1
  if (int32_t(r9) == -1 || uint32_t(r9) > 4294967295u) lifted::Trap(0);
  // beq cr6,0x825d8d18
  if (cr6.eq) goto loc_825D8D18;
  // lhz r8,0(r11)
  r8 = REX_LOAD_U16(uint32_t(r11) + 0);
  // clrlwi r9,r10,16
  r9 = uint32_t(r10) & 0xFFFF;
  // subf r9,r9,r8
  r9 = r8 - r9;
  // sth r9,56(r31)
  REX_STORE_U16(uint32_t(r31) + 56, uint16_t(r9));
loc_825D8D18:
  // sth r10,48(r31)
  REX_STORE_U16(uint32_t(r31) + 48, uint16_t(r10));
  // clrlwi r9,r10,16
  r9 = uint32_t(r10) & 0xFFFF;
  // lhz r11,2(r11)
  r11 = REX_LOAD_U16(uint32_t(r11) + 2);
  // mullw r11,r11,r9
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r9))));
  // subf r30,r11,r30
  r30 = r30 - r11;
loc_825D8D2C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r30,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r30));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return {r3, f1};
}

// sub_825D8D50: lifted from generated/ by lift.py.
void f825D8D50(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825D8D50 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  // stw r4,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r4));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r10,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r10));
  // lfs f0,-16(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -16);
  f0 = double(temp.f32);
  // stfs f0,4(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 4, temp.u32);
  // lwz r10,4(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 4);
  // stw r10,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r10));
  // lfs f0,-16(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -16);
  f0 = double(temp.f32);
  // stfs f0,8(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
  // lhz r10,12(r4)
  r10 = REX_LOAD_U16(uint32_t(r4) + 12);
  // std r11,16(r3)
  REX_STORE_U64(uint32_t(r3) + 16, r11);
  // extsw r11,r10
  r11 = uint64_t(int64_t(int32_t(r10)));
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // lfd f0,-16(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,12(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // blr
  return;
}

// sub_825D8DA0: lifted from generated/ by lift.py.
uint64_t f825D8DA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825D8DA0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r30,-24(r1)
  REX_STORE_U64(uint32_t(r1) + -24, r30);
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,12(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f13 = double(temp.f32);
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x825d8dd4
  if (cr6.gt) goto loc_825D8DD4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x825d8e7c
  goto loc_825D8E7C;
loc_825D8DD4:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS(0x825D8DDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x825d8df8
  if (!cr6.eq) goto loc_825D8DF8;
  // bl 0x825bfe28
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BFE28, LIFTED_ARGS(0x825D8DEC, r31)); r3 = o.r3; f1 = o.f1; }
  // extsw r10,r3
  r10 = uint64_t(int64_t(int32_t(r3)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // b 0x825d8dfc
  goto loc_825D8DFC;
loc_825D8DF8:
  // ld r10,80(r1)
  r10 = REX_LOAD_U64(uint32_t(r1) + 80);
loc_825D8DFC:
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // beq cr6,0x825d8e74
  if (cr6.eq) goto loc_825D8E74;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // beq cr6,0x825d8e30
  if (cr6.eq) goto loc_825D8E30;
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f0
  f13 = double(std::bit_cast<int64_t>(f0));
  // lfd f0,5256(r11)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 5256)));
  // fmul f0,f13,f0
  f0 = f13 * f0;
  // b 0x825d8e3c
  goto loc_825D8E3C;
loc_825D8E30:
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // lfd f0,88(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
loc_825D8E3C:
  // frsp f0,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f0));
  // lfs f13,12(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 12);
  f13 = double(temp.f32);
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lfs f11,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f11 = double(temp.f32);
  // fsubs f12,f13,f0
  f12 = double(float(f13 - f0));
  // lfs f13,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f13 = double(temp.f32);
  // fmadds f11,f13,f0,f11
  f11 = double(float(std::fma(f13, f0, f11)));
  // stfs f11,4(r31)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r31) + 4, temp.u32);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stfs f12,12(r31)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r31) + 12, temp.u32);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lfs f12,80(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f12 = double(temp.f32);
  // fmadds f0,f12,f0,f13
  f0 = double(float(std::fma(f12, f0, f13)));
  // stfs f0,8(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 8, temp.u32);
loc_825D8E74:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
loc_825D8E7C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r30,-24(r1)
  r30 = REX_LOAD_U64(uint32_t(r1) + -24);
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_825D8E98: lifted from generated/ by lift.py.
uint64_t f825D8E98(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825D8E98 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,12(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f0 = double(temp.f32);
  // lfs f12,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f12 = double(temp.f32);
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // ble cr6,0x825d8f00
  if (!cr6.gt) goto loc_825D8F00;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // lfs f0,28048(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 28048);
  f0 = double(temp.f32);
loc_825D8EB4:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x825d8f00
  if (cr6.eq) goto loc_825D8F00;
  // lfs f13,12(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f13 = double(temp.f32);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // fsubs f11,f13,f0
  f11 = double(float(f13 - f0));
  // lfs f13,8(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 8);
  f13 = double(temp.f32);
  // lfs f10,4(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 4);
  f10 = double(temp.f32);
  // addi r4,r4,-1
  r4 = uint64_t(int64_t(int64_t(r4) + -1));
  // fmadds f10,f13,f0,f10
  f10 = double(float(std::fma(f13, f0, f10)));
  // stfs f10,4(r3)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r3) + 4, temp.u32);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stfs f11,12(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // stw r11,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r11));
  // lfs f11,-16(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -16);
  f11 = double(temp.f32);
  // fmadds f13,f11,f0,f13
  f13 = double(float(std::fma(f11, f0, f13)));
  // stfs f13,8(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 8, temp.u32);
  // lfs f13,12(r3)
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // bgt cr6,0x825d8eb4
  if (cr6.gt) goto loc_825D8EB4;
loc_825D8F00:
  // lfs f0,12(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 12);
  f0 = double(temp.f32);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // bgtlr cr6
  if (cr6.gt) return r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

namespace {

lifted::Result e825C2AA0(uint8_t* base, const lifted::Args& a) {
  return f825C2AA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C35C0(uint8_t* base, const lifted::Args& a) {
  f825C35C0(base, a.r[0], a.r[1], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825C3758(uint8_t* base, const lifted::Args& a) {
  return f825C3758(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C7358(uint8_t* base, const lifted::Args& a) {
  return f825C7358(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C7B10(uint8_t* base, const lifted::Args& a) {
  return f825C7B10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825C8EB8(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f825C8EB8(base, a.r1, a.r[1])};
}

lifted::Result e825C8F08(uint8_t* base, const lifted::Args& a) {
  return f825C8F08(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e825C91E0(uint8_t* base, const lifted::Args& a) {
  return f825C91E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825C95F8(uint8_t* base, const lifted::Args& a) {
  f825C95F8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825C9658(uint8_t* base, const lifted::Args& a) {
  return f825C9658(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C9660(uint8_t* base, const lifted::Args& a) {
  return {f825C9660(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825C96F0(uint8_t* base, const lifted::Args& a) {
  return f825C96F0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C9788(uint8_t* base, const lifted::Args& a) {
  return f825C9788(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C9878(uint8_t* base, const lifted::Args& a) {
  return f825C9878(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C9960(uint8_t* base, const lifted::Args& a) {
  return f825C9960(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825C9A80(uint8_t* base, const lifted::Args& a) {
  return f825C9A80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CA8B0(uint8_t* base, const lifted::Args& a) {
  return f825CA8B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CAB10(uint8_t* base, const lifted::Args& a) {
  return f825CAB10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CAB40(uint8_t* base, const lifted::Args& a) {
  return f825CAB40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CABB0(uint8_t* base, const lifted::Args& a) {
  return f825CABB0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825CABC8(uint8_t* base, const lifted::Args& a) {
  return f825CABC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CB3D8(uint8_t* base, const lifted::Args& a) {
  return f825CB3D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CB818(uint8_t* base, const lifted::Args& a) {
  return f825CB818(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CC148(uint8_t* base, const lifted::Args& a) {
  return f825CC148(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e825CC858(uint8_t* base, const lifted::Args& a) {
  return f825CC858(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825CCFA0(uint8_t* base, const lifted::Args& a) {
  return f825CCFA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D0490(uint8_t* base, const lifted::Args& a) {
  return f825D0490(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825D04A8(uint8_t* base, const lifted::Args& a) {
  return f825D04A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D0540(uint8_t* base, const lifted::Args& a) {
  return f825D0540(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825D0670(uint8_t* base, const lifted::Args& a) {
  return f825D0670(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825D06A0(uint8_t* base, const lifted::Args& a) {
  return f825D06A0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D0868(uint8_t* base, const lifted::Args& a) {
  return f825D0868(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D0E78(uint8_t* base, const lifted::Args& a) {
  f825D0E78(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D1A80(uint8_t* base, const lifted::Args& a) {
  return {f825D1A80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]), a.f[0]};
}

lifted::Result e825D1AD8(uint8_t* base, const lifted::Args& a) {
  return f825D1AD8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D1B88(uint8_t* base, const lifted::Args& a) {
  return f825D1B88(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D2AD8(uint8_t* base, const lifted::Args& a) {
  f825D2AD8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D35A8(uint8_t* base, const lifted::Args& a) {
  return {f825D35A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825D5698(uint8_t* base, const lifted::Args& a) {
  return f825D5698(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825D5718(uint8_t* base, const lifted::Args& a) {
  return f825D5718(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e825D5DE8(uint8_t* base, const lifted::Args& a) {
  f825D5DE8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D5F40(uint8_t* base, const lifted::Args& a) {
  return {f825D5F40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e825D5F80(uint8_t* base, const lifted::Args& a) {
  return f825D5F80(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D5FE8(uint8_t* base, const lifted::Args& a) {
  return {f825D5FE8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]), a.f[0]};
}

lifted::Result e825D6050(uint8_t* base, const lifted::Args& a) {
  return f825D6050(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D60C0(uint8_t* base, const lifted::Args& a) {
  return {f825D60C0(base, a.r[0]), a.f[0]};
}

lifted::Result e825D60E8(uint8_t* base, const lifted::Args& a) {
  return f825D60E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D6180(uint8_t* base, const lifted::Args& a) {
  return f825D6180(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e825D6248(uint8_t* base, const lifted::Args& a) {
  return f825D6248(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D6308(uint8_t* base, const lifted::Args& a) {
  return {f825D6308(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]), a.f[0]};
}

lifted::Result e825D6360(uint8_t* base, const lifted::Args& a) {
  return f825D6360(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D63D0(uint8_t* base, const lifted::Args& a) {
  return f825D63D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D6580(uint8_t* base, const lifted::Args& a) {
  return f825D6580(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D67E8(uint8_t* base, const lifted::Args& a) {
  return {f825D67E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]), a.f[0]};
}

lifted::Result e825D6840(uint8_t* base, const lifted::Args& a) {
  return f825D6840(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D68B0(uint8_t* base, const lifted::Args& a) {
  return f825D68B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D6A88(uint8_t* base, const lifted::Args& a) {
  return {f825D6A88(base, a.r[0]), a.f[0]};
}

lifted::Result e825D6AD8(uint8_t* base, const lifted::Args& a) {
  return f825D6AD8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D6CF0(uint8_t* base, const lifted::Args& a) {
  return {f825D6CF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]), a.f[0]};
}

lifted::Result e825D6D38(uint8_t* base, const lifted::Args& a) {
  return f825D6D38(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D6DA0(uint8_t* base, const lifted::Args& a) {
  return f825D6DA0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D6EB8(uint8_t* base, const lifted::Args& a) {
  return f825D6EB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D70C8(uint8_t* base, const lifted::Args& a) {
  return {f825D70C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e825D7110(uint8_t* base, const lifted::Args& a) {
  return f825D7110(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D7148(uint8_t* base, const lifted::Args& a) {
  return f825D7148(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D71C0(uint8_t* base, const lifted::Args& a) {
  return {f825D71C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e825D7210(uint8_t* base, const lifted::Args& a) {
  return f825D7210(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D7270(uint8_t* base, const lifted::Args& a) {
  return f825D7270(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D7328(uint8_t* base, const lifted::Args& a) {
  return f825D7328(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D73B8(uint8_t* base, const lifted::Args& a) {
  return f825D73B8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825D75A8(uint8_t* base, const lifted::Args& a) {
  return f825D75A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D7618(uint8_t* base, const lifted::Args& a) {
  return f825D7618(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D7698(uint8_t* base, const lifted::Args& a) {
  return {f825D7698(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825D7790(uint8_t* base, const lifted::Args& a) {
  return f825D7790(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D7848(uint8_t* base, const lifted::Args& a) {
  return f825D7848(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D7908(uint8_t* base, const lifted::Args& a) {
  return f825D7908(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D79E8(uint8_t* base, const lifted::Args& a) {
  return f825D79E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e825D7A48(uint8_t* base, const lifted::Args& a) {
  return f825D7A48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D7AA0(uint8_t* base, const lifted::Args& a) {
  return {f825D7AA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e825D7B70(uint8_t* base, const lifted::Args& a) {
  return {f825D7B70(base, a.r[0]), a.f[0]};
}

lifted::Result e825D7B80(uint8_t* base, const lifted::Args& a) {
  return {f825D7B80(base, a.r[0]), a.f[0]};
}

lifted::Result e825D7BA0(uint8_t* base, const lifted::Args& a) {
  return f825D7BA0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D7C48(uint8_t* base, const lifted::Args& a) {
  return f825D7C48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D7F18(uint8_t* base, const lifted::Args& a) {
  return f825D7F18(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e825D7F88(uint8_t* base, const lifted::Args& a) {
  return f825D7F88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D8950(uint8_t* base, const lifted::Args& a) {
  return {f825D8950(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e825D8990(uint8_t* base, const lifted::Args& a) {
  return f825D8990(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D89F8(uint8_t* base, const lifted::Args& a) {
  return f825D89F8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D8AA8(uint8_t* base, const lifted::Args& a) {
  return {f825D8AA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]), a.f[0]};
}

lifted::Result e825D8B18(uint8_t* base, const lifted::Args& a) {
  f825D8B18(base, a.r1, a.r13, a.r[0], a.lr);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D8B28(uint8_t* base, const lifted::Args& a) {
  return f825D8B28(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e825D8B60(uint8_t* base, const lifted::Args& a) {
  return f825D8B60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825D8BC8(uint8_t* base, const lifted::Args& a) {
  return f825D8BC8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e825D8C78(uint8_t* base, const lifted::Args& a) {
  return f825D8C78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825D8D50(uint8_t* base, const lifted::Args& a) {
  f825D8D50(base, a.r1, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825D8DA0(uint8_t* base, const lifted::Args& a) {
  return {f825D8DA0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825D8E98(uint8_t* base, const lifted::Args& a) {
  return {f825D8E98(base, a.r1, a.r[0], a.r[1]), a.f[0]};
}

const Registration kEntries[] = {
    {0x825C2AA0, e825C2AA0},
    {0x825C35C0, e825C35C0},
    {0x825C3758, e825C3758},
    {0x825C7358, e825C7358},
    {0x825C7B10, e825C7B10},
    {0x825C8EB8, e825C8EB8},
    {0x825C8F08, e825C8F08},
    {0x825C91E0, e825C91E0},
    {0x825C95F8, e825C95F8},
    {0x825C9658, e825C9658},
    {0x825C9660, e825C9660},
    {0x825C96F0, e825C96F0},
    {0x825C9788, e825C9788},
    {0x825C9878, e825C9878},
    {0x825C9960, e825C9960},
    {0x825C9A80, e825C9A80},
    {0x825CA8B0, e825CA8B0},
    {0x825CAB10, e825CAB10},
    {0x825CAB40, e825CAB40},
    {0x825CABB0, e825CABB0},
    {0x825CABC8, e825CABC8},
    {0x825CB3D8, e825CB3D8},
    {0x825CB818, e825CB818},
    {0x825CC148, e825CC148},
    {0x825CC858, e825CC858},
    {0x825CCFA0, e825CCFA0},
    {0x825D0490, e825D0490},
    {0x825D04A8, e825D04A8},
    {0x825D0540, e825D0540},
    {0x825D0670, e825D0670},
    {0x825D06A0, e825D06A0},
    {0x825D0868, e825D0868},
    {0x825D0E78, e825D0E78},
    {0x825D1A80, e825D1A80},
    {0x825D1AD8, e825D1AD8},
    {0x825D1B88, e825D1B88},
    {0x825D2AD8, e825D2AD8},
    {0x825D35A8, e825D35A8},
    {0x825D5698, e825D5698},
    {0x825D5718, e825D5718},
    {0x825D5DE8, e825D5DE8},
    {0x825D5F40, e825D5F40},
    {0x825D5F80, e825D5F80},
    {0x825D5FE8, e825D5FE8},
    {0x825D6050, e825D6050},
    {0x825D60C0, e825D60C0},
    {0x825D60E8, e825D60E8},
    {0x825D6180, e825D6180},
    {0x825D6248, e825D6248},
    {0x825D6308, e825D6308},
    {0x825D6360, e825D6360},
    {0x825D63D0, e825D63D0},
    {0x825D6580, e825D6580},
    {0x825D67E8, e825D67E8},
    {0x825D6840, e825D6840},
    {0x825D68B0, e825D68B0},
    {0x825D6A88, e825D6A88},
    {0x825D6AD8, e825D6AD8},
    {0x825D6CF0, e825D6CF0},
    {0x825D6D38, e825D6D38},
    {0x825D6DA0, e825D6DA0},
    {0x825D6EB8, e825D6EB8},
    {0x825D70C8, e825D70C8},
    {0x825D7110, e825D7110},
    {0x825D7148, e825D7148},
    {0x825D71C0, e825D71C0},
    {0x825D7210, e825D7210},
    {0x825D7270, e825D7270},
    {0x825D7328, e825D7328},
    {0x825D73B8, e825D73B8},
    {0x825D75A8, e825D75A8},
    {0x825D7618, e825D7618},
    {0x825D7698, e825D7698},
    {0x825D7790, e825D7790},
    {0x825D7848, e825D7848},
    {0x825D7908, e825D7908},
    {0x825D79E8, e825D79E8},
    {0x825D7A48, e825D7A48},
    {0x825D7AA0, e825D7AA0},
    {0x825D7B70, e825D7B70},
    {0x825D7B80, e825D7B80},
    {0x825D7BA0, e825D7BA0},
    {0x825D7C48, e825D7C48},
    {0x825D7F18, e825D7F18},
    {0x825D7F88, e825D7F88},
    {0x825D8950, e825D8950},
    {0x825D8990, e825D8990},
    {0x825D89F8, e825D89F8},
    {0x825D8AA8, e825D8AA8},
    {0x825D8B18, e825D8B18},
    {0x825D8B28, e825D8B28},
    {0x825D8B60, e825D8B60},
    {0x825D8BC8, e825D8BC8},
    {0x825D8C78, e825D8C78},
    {0x825D8D50, e825D8D50},
    {0x825D8DA0, e825D8DA0},
    {0x825D8E98, e825D8E98},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_042)), true);

}  // namespace

}  // namespace lifted
