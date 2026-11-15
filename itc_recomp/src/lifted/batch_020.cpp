// Lift sweep batch 020 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_020, true, "ITC",
              "Run lift sweep batch 020's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address.
#define LIFTED_ARGS(lr) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr) }

namespace lifted {

// sub_82588DF0: lifted from generated/ by lift.py.
uint64_t f82588DF0(uint8_t* base, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_82588DF0 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lbz r11,20(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 20);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82588e28
  if (!cr6.eq) goto loc_82588E28;
  // extsh r11,r4
  r11 = uint64_t(int64_t(int16_t(r4)));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82588e28
  if (!cr6.eq) goto loc_82588E28;
  // lbz r11,21(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 21);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82588e28
  if (!cr6.eq) goto loc_82588E28;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lfs f0,11540(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11540);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bgelr cr6
  if (!cr6.lt) return r3;
loc_82588E28:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82588E30: lifted from generated/ by lift.py.
lifted::Result f82588E30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_82588E30 (lifted)");
  uint64_t r4 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-48
  r12 = uint64_t(int64_t(int64_t(r1) + -48));
  // bl 0x825a7918
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-224(r1)
  ea = -224 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r5
  r31 = r5;
  // fmr f28,f1
  lifted::ScalarFloatMode();
  f28 = f1;
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r5,r1,88
  r5 = uint64_t(int64_t(int64_t(r1) + 88));
  // li r4,64
  r4 = uint64_t(int64_t(64));
  // mr r27,r3
  r27 = r3;
  // bl 0x82584ed8
  // (left out: dead in C++)
  f82584ED8(base, r1, r3, r4, r5, r6);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r10,r31,4
  r10 = uint64_t(int64_t(int64_t(r31) + 4));
  // lfs f30,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f30 = double(temp.f32);
  // li r11,11
  r11 = uint64_t(int64_t(11));
  // fmr f1,f30
  f1 = f30;
loc_82588E74:
  // lfs f0,0(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f0 = double(temp.f32);
  // addic. r11,r11,-1
  ca = uint32_t(r11) > 0;
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // fnmsubs f0,f0,f0,f30
  f0 = double(float(-std::fma(f0, f0, -f30)));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // fmuls f1,f0,f1
  f1 = double(float(f0 * f1));
  // bne 0x82588e74
  if (!cr0.eq) goto loc_82588E74;
  // bl 0x82585398
  // (left out: dead in C++)
  f1 = f82585398(base, r1, f1);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // addi r29,r1,112
  r29 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // fmadds f30,f0,f0,f30
  f30 = double(float(std::fma(f0, f0, f30)));
  // li r30,5
  r30 = uint64_t(int64_t(5));
  // lis r28,-32154
  r28 = uint64_t(int64_t(-2107244544));
  // lfs f31,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f31 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // fnmsubs f28,f1,f31,f28
  f28 = double(float(-std::fma(f1, f31, -f28)));
  // lfs f13,10416(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10416);
  f13 = double(temp.f32);
  // fmuls f29,f0,f13
  f29 = double(float(f0 * f13));
loc_82588EC0:
  // lwz r11,-9752(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -9752);
  // lfsx f0,r31,r11
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  f0 = double(temp.f32);
  // addi r31,r31,128
  r31 = uint64_t(int64_t(int64_t(r31) + 128));
  // fmadds f1,f0,f29,f30
  f1 = double(float(std::fma(f0, f29, f30)));
  // bl 0x82585398
  // (left out: dead in C++)
  f1 = f82585398(base, r1, f1);
  // fnmsubs f0,f1,f31,f28
  lifted::ScalarFloatMode();
  f0 = double(float(-std::fma(f1, f31, -f28)));
  // stfs f0,0(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // bne 0x82588ec0
  if (!cr0.eq) goto loc_82588EC0;
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r10,-16560
  r10 = uint64_t(int64_t(int64_t(r10) + -16560));
  // addi r9,r27,4
  r9 = uint64_t(int64_t(int64_t(r27) + 4));
  // lfs f12,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f12 = double(temp.f32);
  // lfs f10,4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 4);
  f10 = double(temp.f32);
  // lfs f11,12(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 12);
  f11 = double(temp.f32);
loc_82588F04:
  // clrlwi r8,r11,28
  r8 = uint32_t(r11) & 0xF;
  // lfs f13,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f13 = double(temp.f32);
  // srawi r10,r11,4
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0xF) != 0);
  r10 = uint64_t(int64_t(int32_t(r11) >> 4));
  // addi r7,r1,112
  r7 = uint64_t(int64_t(int64_t(r1) + 112));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r6,r1,116
  r6 = uint64_t(int64_t(int64_t(r1) + 116));
  // std r8,96(r1)
  REX_STORE_U64(uint32_t(r1) + 96, r8);
  // lfsx f8,r10,r6
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r6));
  f8 = double(temp.f32);
  // lfd f0,96(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 96)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f9,f0,f11
  f9 = double(float(f0 * f11));
  // lfsx f0,r10,r7
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r7));
  f0 = double(temp.f32);
  // fsubs f8,f8,f0
  f8 = double(float(f8 - f0));
  // fmadds f0,f8,f9,f0
  f0 = double(float(std::fma(f8, f9, f0)));
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fcmpu cr6,f0,f12
  cr6 = lifted::CompareFloat(f0, f12);
  // ble cr6,0x82588f54
  if (!cr6.gt) goto loc_82588F54;
  // fmr f0,f12
  f0 = f12;
loc_82588F54:
  // fcmpu cr6,f0,f10
  lifted::ScalarFloatMode();
  cr6 = lifted::CompareFloat(f0, f10);
  // bgt cr6,0x82588f60
  if (cr6.gt) goto loc_82588F60;
  // fmr f0,f10
  f0 = f10;
loc_82588F60:
  // cmpwi cr6,r11,16
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 16, so);
  // bge cr6,0x82588f84
  if (!cr6.lt) goto loc_82588F84;
  // extsw r10,r11
  r10 = uint64_t(int64_t(int32_t(r11)));
  // std r10,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r10);
  // lfd f9,104(r1)
  lifted::ScalarFloatMode();
  f9 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 104)));
  // fcfid f9,f9
  f9 = double(std::bit_cast<int64_t>(f9));
  // frsp f9,f9
  f9 = double(float(f9));
  // fmuls f9,f9,f11
  f9 = double(float(f9 * f11));
  // fmuls f0,f9,f0
  f0 = double(float(f9 * f0));
loc_82588F84:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // fadds f0,f13,f0
  lifted::ScalarFloatMode();
  f0 = double(float(f13 + f0));
  // stfs f0,0(r9)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmpwi cr6,r11,64
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 64, so);
  // blt cr6,0x82588f04
  if (cr6.lt) goto loc_82588F04;
  // addi r6,r1,84
  r6 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // li r4,64
  r4 = uint64_t(int64_t(64));
  // mr r3,r27
  r3 = r27;
  // bl 0x82584ed8
  // (left out: dead in C++)
  f82584ED8(base, r1, r3, r4, r5, r6);
  // lfs f0,80(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 80);
  f0 = double(temp.f32);
  // lfs f13,84(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 84);
  f13 = double(temp.f32);
  // mr r11,r27
  r11 = r27;
  // fsubs f0,f0,f13
  f0 = double(float(f0 - f13));
  // li r10,64
  r10 = uint64_t(int64_t(64));
loc_82588FC4:
  // lfs f13,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // fadds f13,f13,f0
  f13 = double(float(f13 + f0));
  // stfs f13,0(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x82588fc4
  if (!cr0.eq) goto loc_82588FC4;
  // addi r1,r1,224
  r1 = uint64_t(int64_t(int64_t(r1) + 224));
  // addi r12,r1,-48
  r12 = uint64_t(int64_t(int64_t(r1) + -48));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82589480: lifted from generated/ by lift.py.
lifted::Result f82589480(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82589480 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2908
  r11 = uint64_t(int64_t(int64_t(r11) + 2908));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x825894ac
  if (cr0.eq) goto loc_825894AC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x825894AC)); r3 = o.r3; f1 = o.f1; }
loc_825894AC:
  // mr r3,r31
  r3 = r31;
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

// sub_82589650: lifted from generated/ by lift.py.
lifted::Result f82589650(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82589650 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r31,r11,-18548
  r31 = uint64_t(int64_t(int64_t(r11) + -18548));
  // lwz r3,28(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 28);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82589678
  if (cr6.eq) goto loc_82589678;
  // bl 0x8258e410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E410, LIFTED_ARGS(0x82589678)); r3 = o.r3; f1 = o.f1; }
loc_82589678:
  // lwz r3,32(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 32);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82589688
  if (cr6.eq) goto loc_82589688;
  // bl 0x8258e410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E410, LIFTED_ARGS(0x82589688)); r3 = o.r3; f1 = o.f1; }
loc_82589688:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82589698
  if (cr6.eq) goto loc_82589698;
  // bl 0x8258e410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E410, LIFTED_ARGS(0x82589698)); r3 = o.r3; f1 = o.f1; }
loc_82589698:
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x825896a8
  if (cr6.eq) goto loc_825896A8;
  // bl 0x8258e410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E410, LIFTED_ARGS(0x825896A8)); r3 = o.r3; f1 = o.f1; }
loc_825896A8:
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

// sub_82589AB0: lifted from generated/ by lift.py.
lifted::Result f82589AB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82589AB0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // mr r31,r3
  r31 = r3;
  // addi r4,r11,-18128
  r4 = uint64_t(int64_t(int64_t(r11) + -18128));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258e580
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E580, LIFTED_ARGS(0x82589AD4)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r11,r11,-18548
  r11 = uint64_t(int64_t(int64_t(r11) + -18548));
  // addi r7,r11,-8
  r7 = uint64_t(int64_t(int64_t(r11) + -8));
  // lwz r10,-12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -12);
  // lwz r6,8(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 8);
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r9,r10,r7
  r9 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r7));
  // add r8,r9,r31
  r8 = r9 + r31;
  // cmplw cr6,r8,r6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r6), so);
  // blt cr6,0x82589b28
  if (cr6.lt) goto loc_82589B28;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r11,-16452
  r3 = uint64_t(int64_t(int64_t(r11) + -16452));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258e5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E5B8, LIFTED_ARGS(0x82589B10)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
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
loc_82589B28:
  // lwzx r11,r10,r11
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // rlwinm r9,r9,6,0,25
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 6) & 0xFFFFFFC0;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stwx r8,r10,r7
  REX_STORE_U32(uint32_t(r10) + uint32_t(r7), uint32_t(r8));
  // add r31,r11,r9
  r31 = r11 + r9;
  // bl 0x8258e5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E5B8, LIFTED_ARGS(0x82589B40)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
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

// sub_8258BCD8: lifted from generated/ by lift.py.
uint64_t f8258BCD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258BCD8 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,40(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 40);
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bne cr6,0x8258bd0c
  if (!cr6.eq) goto loc_8258BD0C;
  // lwz r11,44(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 44);
  // cmplwi cr6,r11,6
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 6, so);
  // beq cr6,0x8258bd08
  if (cr6.eq) goto loc_8258BD08;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // beq cr6,0x8258bd04
  if (cr6.eq) goto loc_8258BD04;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // b 0x8258af80
  r3 = f8258AF80(base, r1, r3);
  return r3;
loc_8258BD04:
  // b 0x8258ada0
  r3 = f8258ADA0(base, r1, r3);
  return r3;
loc_8258BD08:
  // b 0x8258ac58
  r3 = f8258AC58(base, r3);
  return r3;
loc_8258BD0C:
  // cmplwi cr6,r11,9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 9, so);
  // bne cr6,0x8258bd18
  if (!cr6.eq) goto loc_8258BD18;
  // b 0x8258b1c0
  r3 = f8258B1C0(base, r3);
  return r3;
loc_8258BD18:
  // cmplwi cr6,r11,10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 10, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // b 0x8258b368
  r3 = f8258B368(base, r3);
  return r3;
}

// sub_8258BE58: lifted from generated/ by lift.py.
void f8258BE58(uint8_t* base, uint64_t r1, uint64_t r7, double f1, double f2) {
  ITC_NATIVE_RUNNING("sub_8258BE58 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // rlwinm r9,r7,28,28,31
  r9 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 28) & 0xF;
  // clrlwi r8,r7,28
  r8 = uint32_t(r7) & 0xF;
  // clrlwi r7,r9,24
  r7 = uint32_t(r9) & 0xFF;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // std r8,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r8);
  // std r7,-8(r1)
  REX_STORE_U64(uint32_t(r1) + -8, r7);
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r10,r11,-17184
  r10 = uint64_t(int64_t(int64_t(r11) + -17184));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f13,32(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 32);
  f13 = double(temp.f32);
  // lbz r6,16(r10)
  r6 = REX_LOAD_U8(uint32_t(r10) + 16);
  // fdivs f9,f0,f13
  f9 = double(float(f0 / f13));
  // lfs f13,36(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 36);
  f13 = double(temp.f32);
  // fdivs f8,f0,f13
  f8 = double(float(f0 / f13));
  // lfs f13,3124(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 3124);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fadds f11,f1,f13
  f11 = double(float(f1 + f13));
  // lbz r5,17(r10)
  r5 = REX_LOAD_U8(uint32_t(r10) + 17);
  // lbz r9,18(r10)
  r9 = REX_LOAD_U8(uint32_t(r10) + 18);
  // lfs f12,8684(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8684);
  f12 = double(temp.f32);
  // lbz r11,19(r10)
  r11 = REX_LOAD_U8(uint32_t(r10) + 19);
  // fadds f10,f2,f12
  f10 = double(float(f2 + f12));
  // rlwimi r6,r11,8,16,23
  r6 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFF00) | (r6 & 0xFFFFFFFFFFFF00FF);
  // lwz r11,28(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 28);
  // fmuls f7,f9,f13
  f7 = double(float(f9 * f13));
  // fmuls f6,f8,f12
  f6 = double(float(f8 * f12));
  // clrlwi r8,r6,16
  r8 = uint32_t(r6) & 0xFFFF;
  // rlwimi r5,r8,8,0,23
  r5 = (__builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 8) & 0xFFFFFF00) | (r5 & 0xFFFFFFFF000000FF);
  // stfs f1,0(r11)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // rlwimi r9,r5,8,0,23
  r9 = (__builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 8) & 0xFFFFFF00) | (r9 & 0xFFFFFFFF000000FF);
  // stfs f2,4(r11)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // lfd f5,-16(r1)
  f5 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -16)));
  // lfd f4,-8(r1)
  f4 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -8)));
  // fcfid f5,f5
  f5 = double(std::bit_cast<int64_t>(f5));
  // fcfid f4,f4
  f4 = double(std::bit_cast<int64_t>(f4));
  // frsp f5,f5
  f5 = double(float(f5));
  // frsp f4,f4
  f4 = double(float(f4));
  // fmuls f13,f5,f13
  f13 = double(float(f5 * f13));
  // fmuls f12,f4,f12
  f12 = double(float(f4 * f12));
  // fmuls f13,f13,f9
  f13 = double(float(f13 * f9));
  // stfs f13,16(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // fmuls f12,f12,f8
  f12 = double(float(f12 * f8));
  // stfs f12,20(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // stfs f11,24(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // stw r9,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r9));
  // stfs f2,28(r11)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f0,32(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // stfs f12,44(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // fadds f9,f7,f13
  f9 = double(float(f7 + f13));
  // stfs f9,40(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 40, temp.u32);
  // fadds f8,f6,f12
  f8 = double(float(f6 + f12));
  // stfs f1,48(r11)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r11) + 48, temp.u32);
  // stfs f10,52(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 52, temp.u32);
  // stw r9,60(r11)
  REX_STORE_U32(uint32_t(r11) + 60, uint32_t(r9));
  // stfs f0,56(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 56, temp.u32);
  // stfs f13,64(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 64, temp.u32);
  // stfs f8,68(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 68, temp.u32);
  // stw r9,84(r11)
  REX_STORE_U32(uint32_t(r11) + 84, uint32_t(r9));
  // stfs f1,72(r11)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r11) + 72, temp.u32);
  // stfs f10,76(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 76, temp.u32);
  // stfs f0,80(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 80, temp.u32);
  // stfs f13,88(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 88, temp.u32);
  // stfs f8,92(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 92, temp.u32);
  // stw r9,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r9));
  // stfs f11,96(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 96, temp.u32);
  // stfs f2,100(r11)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r11) + 100, temp.u32);
  // stfs f0,104(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 104, temp.u32);
  // stfs f9,112(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 112, temp.u32);
  // stfs f12,116(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 116, temp.u32);
  // stw r9,132(r11)
  REX_STORE_U32(uint32_t(r11) + 132, uint32_t(r9));
  // stfs f11,120(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 120, temp.u32);
  // stfs f10,124(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 124, temp.u32);
  // stfs f0,128(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 128, temp.u32);
  // stfs f9,136(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 136, temp.u32);
  // stfs f8,140(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 140, temp.u32);
  // addi r11,r11,144
  r11 = uint64_t(int64_t(int64_t(r11) + 144));
  // stw r11,28(r10)
  REX_STORE_U32(uint32_t(r10) + 28, uint32_t(r11));
  // blr
  return;
}

// sub_8258BFA0: lifted from generated/ by lift.py.
lifted::Result f8258BFA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8258BFA0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // mr r31,r3
  r31 = r3;
  // addi r30,r11,-17144
  r30 = uint64_t(int64_t(int64_t(r11) + -17144));
  // mr r28,r4
  r28 = r4;
  // lwz r11,-12(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -12);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258c058
  if (cr6.eq) goto loc_8258C058;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258BFD0)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,15
  r10 = uint64_t(int64_t(15));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r29,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r29));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258BFF8)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C004)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,36
  r10 = uint64_t(int64_t(36));
  // mr r3,r31
  r3 = r31;
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r29,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r29));
  // bl 0x8258ca50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CA50, LIFTED_ARGS(0x8258C020)); r3 = o.r3; f1 = o.f1; }
  // lwz r4,-16(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + -16);
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x8258c04c
  if (cr6.eq) goto loc_8258C04C;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,0(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r5,0(r30)
  r5 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r28
  r3 = r28;
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8258C04C)); r3 = o.r3; f1 = o.f1; }
loc_8258C04C:
  // mr r11,r29
  r11 = r29;
  // stw r29,-12(r30)
  REX_STORE_U32(uint32_t(r30) + -12, uint32_t(r29));
  // stw r11,-16(r30)
  REX_STORE_U32(uint32_t(r30) + -16, uint32_t(r11));
loc_8258C058:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258C060: lifted from generated/ by lift.py.
lifted::Result f8258C060(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258C060 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32105
  r30 = uint64_t(int64_t(-2104033280));
  // lwz r31,-17164(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + -17164);
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8258c09c
  if (cr6.eq) goto loc_8258C09C;
  // mr r3,r31
  r3 = r31;
  // bl 0x82597148
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82597148, LIFTED_ARGS(0x8258C08C)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8258C094)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,-17164(r30)
  REX_STORE_U32(uint32_t(r30) + -17164, uint32_t(r11));
loc_8258C09C:
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

// sub_8258C0B8: lifted from generated/ by lift.py.
lifted::Result f8258C0B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_8258C0B8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // mr r11,r5
  r11 = r5;
  // addi r25,r10,-17164
  r25 = uint64_t(int64_t(int64_t(r10) + -17164));
  // mr r10,r30
  r10 = r30;
  // mr r31,r3
  r31 = r3;
  // mr r9,r30
  r9 = r30;
  // lbz r8,0(r11)
  r8 = REX_LOAD_U8(uint32_t(r11) + 0);
  // stw r10,8(r25)
  REX_STORE_U32(uint32_t(r25) + 8, uint32_t(r10));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x8258c17c
  if (cr6.eq) goto loc_8258C17C;
loc_8258C0F0:
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // cmpwi cr6,r10,32
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 32, so);
  // beq cr6,0x8258c16c
  if (cr6.eq) goto loc_8258C16C;
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x8258c16c
  if (cr6.eq) goto loc_8258C16C;
  // cmpwi cr6,r10,126
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 126, so);
  // bne cr6,0x8258c168
  if (!cr6.eq) goto loc_8258C168;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8258c17c
  if (cr6.eq) goto loc_8258C17C;
  // cmpwi cr6,r10,99
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 99, so);
  // beq cr6,0x8258c134
  if (cr6.eq) goto loc_8258C134;
  // cmpwi cr6,r10,67
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 67, so);
  // bne cr6,0x8258c16c
  if (!cr6.eq) goto loc_8258C16C;
loc_8258C134:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8258c17c
  if (cr6.eq) goto loc_8258C17C;
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8258c17c
  if (cr6.eq) goto loc_8258C17C;
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8258c17c
  if (cr6.eq) goto loc_8258C17C;
  // b 0x8258c16c
  goto loc_8258C16C;
loc_8258C168:
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
loc_8258C16C:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x8258c0f0
  if (!cr6.eq) goto loc_8258C0F0;
loc_8258C17C:
  // rlwinm r11,r9,1,0,30
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 1) & 0xFFFFFFFE;
  // add r11,r9,r11
  r11 = r9 + r11;
  // rlwinm r26,r11,1,0,30
  r26 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // cmpwi cr6,r26,0
  cr6 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // bne cr6,0x8258c19c
  if (!cr6.eq) goto loc_8258C19C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
loc_8258C19C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8258c9d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258C9D0, LIFTED_ARGS(0x8258C1A4)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C1B0)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r24,5
  r24 = uint64_t(int64_t(5));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r11,4(r25)
  REX_STORE_U32(uint32_t(r25) + 4, uint32_t(r11));
  // sth r24,2(r11)
  REX_STORE_U16(uint32_t(r11) + 2, uint16_t(r24));
  // stw r30,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C1D0)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r27,4
  r27 = uint64_t(int64_t(4));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // stw r27,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r27));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C1F4)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // lwz r3,0(r25)
  r3 = REX_LOAD_U32(uint32_t(r25) + 0);
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r4,r11,-16400
  r4 = uint64_t(int64_t(int64_t(r11) + -16400));
  // stw r30,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r30));
  // bl 0x82596e88
  // (left out: dead in C++)
  r3 = f82596E88(base, r1, r13, r3, r4);
  // mr r11,r3
  r11 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r11,4(r28)
  REX_STORE_U32(uint32_t(r28) + 4, uint32_t(r11));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C220)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r29,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r27,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r27));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C23C)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,60
  r10 = uint64_t(int64_t(60));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C258)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,72
  r10 = uint64_t(int64_t(72));
  // li r9,6
  r9 = uint64_t(int64_t(6));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C278)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,76
  r10 = uint64_t(int64_t(76));
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C298)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,56
  r10 = uint64_t(int64_t(56));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r30));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C2B4)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // li r28,2
  r28 = uint64_t(int64_t(2));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r28,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r28));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C2D8)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,20
  r10 = uint64_t(int64_t(20));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // mr r3,r31
  r3 = r31;
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C2F8)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,16
  r10 = uint64_t(int64_t(16));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C318)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,24
  r10 = uint64_t(int64_t(24));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r28,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r28));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C338)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r10,13
  r10 = uint64_t(int64_t(13));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C358)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r27,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r27));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // stw r28,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r28));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CAA0, LIFTED_ARGS(0x8258C374)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r26
  r4 = r26;
  // mr r3,r31
  r3 = r31;
  // stw r24,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r24));
  // stw r26,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r26));
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // bl 0x8258cb08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258CB08, LIFTED_ARGS(0x8258C394)); r3 = o.r3; f1 = o.f1; }
  // stw r3,8(r25)
  REX_STORE_U32(uint32_t(r25) + 8, uint32_t(r3));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258C3A8: lifted from generated/ by lift.py.
lifted::Result f8258C3A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258C3A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // std r6,40(r1)
  REX_STORE_U64(uint32_t(r1) + 40, r6);
  // std r7,48(r1)
  REX_STORE_U64(uint32_t(r1) + 48, r7);
  // std r8,56(r1)
  REX_STORE_U64(uint32_t(r1) + 56, r8);
  // std r9,64(r1)
  REX_STORE_U64(uint32_t(r1) + 64, r9);
  // std r10,72(r1)
  REX_STORE_U64(uint32_t(r1) + 72, r10);
  // stwu r1,-2208(r1)
  ea = -2208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32154
  r11 = uint64_t(int64_t(-2107244544));
  // mr r29,r3
  r29 = r3;
  // mr r28,r4
  r28 = r4;
  // mr r31,r5
  r31 = r5;
  // lbz r11,-4800(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + -4800);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258c5a4
  if (cr6.eq) goto loc_8258C5A4;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r27,r11,-17140
  r27 = uint64_t(int64_t(int64_t(r11) + -17140));
  // mr r3,r27
  r3 = r27;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = f825A6D28(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x8258C3F4); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r10,r1,2248
  r10 = uint64_t(int64_t(int64_t(r1) + 2248));
  // mr r4,r31
  r4 = r31;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r5,80(r1)
  r5 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825a71b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71B0, LIFTED_ARGS(0x8258C410)); r3 = o.r3; f1 = o.f1; }
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // addi r30,r1,96
  r30 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x8258c0b8
  // (left out: dead in C++)
  { const lifted::Result o = f8258C0B8(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lbz r11,96(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 96);
  // mr r26,r3
  r26 = r3;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258c584
  if (cr6.eq) goto loc_8258C584;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r31,r11,-17184
  r31 = uint64_t(int64_t(int64_t(r11) + -17184));
  // lwz r5,4(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r6,0(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 0);
loc_8258C444:
  // lbz r7,0(r30)
  r7 = REX_LOAD_U8(uint32_t(r30) + 0);
  // extsb r11,r7
  r11 = uint64_t(int64_t(int8_t(r7)));
  // cmpwi cr6,r11,32
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 32, so);
  // bne cr6,0x8258c460
  if (!cr6.eq) goto loc_8258C460;
  // addi r6,r6,8
  r6 = uint64_t(int64_t(int64_t(r6) + 8));
  // stw r6,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r6));
  // b 0x8258c574
  goto loc_8258C574;
loc_8258C460:
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bne cr6,0x8258c47c
  if (!cr6.eq) goto loc_8258C47C;
  // lwz r6,8(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 8);
  // addi r5,r5,16
  r5 = uint64_t(int64_t(int64_t(r5) + 16));
  // stw r6,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r6));
  // stw r5,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r5));
  // b 0x8258c574
  goto loc_8258C574;
loc_8258C47C:
  // cmpwi cr6,r11,126
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 126, so);
  // bne cr6,0x8258c530
  if (!cr6.eq) goto loc_8258C530;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // extsb r11,r11
  r11 = uint64_t(int64_t(int8_t(r11)));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8258c584
  if (cr6.eq) goto loc_8258C584;
  // cmpwi cr6,r11,99
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 99, so);
  // beq cr6,0x8258c4a8
  if (cr6.eq) goto loc_8258C4A8;
  // cmpwi cr6,r11,67
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 67, so);
  // bne cr6,0x8258c574
  if (!cr6.eq) goto loc_8258C574;
loc_8258C4A8:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // lbz r10,0(r11)
  r10 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // extsb r10,r10
  r10 = uint64_t(int64_t(int8_t(r10)));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8258c584
  if (cr6.eq) goto loc_8258C584;
  // lbz r9,0(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 0);
  // addi r30,r11,1
  r30 = uint64_t(int64_t(int64_t(r11) + 1));
  // extsb r9,r9
  r9 = uint64_t(int64_t(int8_t(r9)));
  // cmpwi cr6,r9,0
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq cr6,0x8258c584
  if (cr6.eq) goto loc_8258C584;
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // extsb r8,r11
  r8 = uint64_t(int64_t(int8_t(r11)));
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x8258c584
  if (cr6.eq) goto loc_8258C584;
  // addi r11,r10,-48
  r11 = uint64_t(int64_t(int64_t(r10) + -48));
  // rlwinm r11,r11,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 5) & 0xFFFFFFE0;
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // ble cr6,0x8258c4f8
  if (!cr6.gt) goto loc_8258C4F8;
  // li r11,255
  r11 = uint64_t(int64_t(255));
loc_8258C4F8:
  // addi r10,r9,-48
  r10 = uint64_t(int64_t(int64_t(r9) + -48));
  // stb r11,16(r31)
  REX_STORE_U8(uint32_t(r31) + 16, uint8_t(r11));
  // rlwinm r11,r10,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // ble cr6,0x8258c510
  if (!cr6.gt) goto loc_8258C510;
  // li r11,255
  r11 = uint64_t(int64_t(255));
loc_8258C510:
  // addi r10,r8,-48
  r10 = uint64_t(int64_t(int64_t(r8) + -48));
  // stb r11,17(r31)
  REX_STORE_U8(uint32_t(r31) + 17, uint8_t(r11));
  // rlwinm r11,r10,5,0,26
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 5) & 0xFFFFFFE0;
  // cmpwi cr6,r11,255
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 255, so);
  // ble cr6,0x8258c528
  if (!cr6.gt) goto loc_8258C528;
  // li r11,255
  r11 = uint64_t(int64_t(255));
loc_8258C528:
  // stb r11,18(r31)
  REX_STORE_U8(uint32_t(r31) + 18, uint8_t(r11));
  // b 0x8258c574
  goto loc_8258C574;
loc_8258C530:
  // extsw r10,r6
  r10 = uint64_t(int64_t(int32_t(r6)));
  // extsw r11,r5
  r11 = uint64_t(int64_t(int32_t(r5)));
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f2,f0
  f2 = double(float(f0));
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f1,f13
  f1 = double(float(f13));
  // bl 0x8258be58
  // (left out: dead in C++)
  f8258BE58(base, r1, r7, f1, f2);
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // addi r6,r11,8
  r6 = uint64_t(int64_t(int64_t(r11) + 8));
  // stw r6,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r6));
  // lwz r5,4(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 4);
loc_8258C574:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8258c444
  if (!cr6.eq) goto loc_8258C444;
loc_8258C584:
  // clrlwi r11,r26,24
  r11 = uint32_t(r26) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258c59c
  if (cr6.eq) goto loc_8258C59C;
  // mr r4,r28
  r4 = r28;
  // mr r3,r29
  r3 = r29;
  // bl 0x8258bfa0
  // (left out: dead in C++)
  { const lifted::Result o = f8258BFA0(base, r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
loc_8258C59C:
  // mr r3,r27
  r3 = r27;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = f8258E578(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x8258C5A4); r3 = o.r3; f1 = o.f1; }
loc_8258C5A4:
  // addi r1,r1,2208
  r1 = uint64_t(int64_t(int64_t(r1) + 2208));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258C5B0: lifted from generated/ by lift.py.
lifted::Result f8258C5B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8258C5B0 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-16392
  r11 = uint64_t(int64_t(int64_t(r11) + -16392));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x8258c5e4
  if (cr6.eq) goto loc_8258C5E4;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8258C5E0)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_8258C5E4:
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

// sub_8258C608: lifted from generated/ by lift.py.
lifted::Result f8258C608(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8258C608 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r3,r4
  r3 = r4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x8258c66c
  if (!cr6.eq) goto loc_8258C66C;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,128
  r8 = uint64_t(int64_t(128));
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // lis r4,16384
  r4 = uint64_t(int64_t(1073741824));
  // bl 0x82295508
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295508, LIFTED_ARGS(0x8258C648)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // stw r3,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r3));
  // beq cr6,0x8258c66c
  if (cr6.eq) goto loc_8258C66C;
  // li r3,1
  r3 = uint64_t(int64_t(1));
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
loc_8258C66C:
  // bl 0x8209ac78
  // (left out: dead in C++)
  { const lifted::Result o = f8209AC78(base, r1, r13, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
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

// sub_8258C898: lifted from generated/ by lift.py.
lifted::Result f8258C898(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8258C898 (lifted)");
  uint64_t r6 = 0, r7 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r29,r5
  r29 = r5;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r30,r4
  r30 = r4;
  // lwz r5,0(r29)
  r5 = REX_LOAD_U32(uint32_t(r29) + 0);
  // stw r28,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r28));
  // bl 0x82295d50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295D50, LIFTED_ARGS(0x8258C8CC)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x8258c8ec
  if (cr6.eq) goto loc_8258C8EC;
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // add r11,r11,r3
  r11 = r11 + r3;
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_8258C8EC:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // li r5,155
  r5 = uint64_t(int64_t(155));
  // addi r4,r11,-16316
  r4 = uint64_t(int64_t(int64_t(r11) + -16316));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r11,-16324
  r3 = uint64_t(int64_t(int64_t(r11) + -16324));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r7,r1,80
  r7 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r6,0(r29)
  r6 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addi r3,r11,-16368
  r3 = uint64_t(int64_t(int64_t(r11) + -16368));
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r5,r30
  r5 = r30;
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r11,r28
  r11 = r28;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r28,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r28));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258CCC8: lifted from generated/ by lift.py.
lifted::Result f8258CCC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258CCC8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r11,r11,-16256
  r11 = uint64_t(int64_t(int64_t(r11) + -16256));
  // lwz r3,20(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 20);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x8258cd04
  if (cr6.eq) goto loc_8258CD04;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71C0, LIFTED_ARGS(0x8258CD00)); r3 = o.r3; f1 = o.f1; }
  // stw r30,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r30));
loc_8258CD04:
  // stw r30,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r30));
  // stw r30,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r30));
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

// sub_8258CE00: lifted from generated/ by lift.py.
lifted::Result f8258CE00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258CE00 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r29,r4
  r29 = r4;
  // addi r11,r11,-16256
  r11 = uint64_t(int64_t(int64_t(r11) + -16256));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r3,20(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 20);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x8258ce38
  if (cr6.eq) goto loc_8258CE38;
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71C0, LIFTED_ARGS(0x8258CE34)); r3 = o.r3; f1 = o.f1; }
  // stw r30,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r30));
loc_8258CE38:
  // clrlwi r11,r29,31
  r11 = uint32_t(r29) & 0x1;
  // stw r30,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r30));
  // stw r30,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r30));
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258ce58
  if (cr6.eq) goto loc_8258CE58;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8258CE54)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_8258CE58:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258CEA0: lifted from generated/ by lift.py.
void f8258CEA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258CEA0 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,-16188
  r11 = uint64_t(int64_t(int64_t(r11) + -16188));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // b 0x82592178
  f82592178(base, r3);
  return;
}

// sub_8258CF40: lifted from generated/ by lift.py.
lifted::Result f8258CF40(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258CF40 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r11,r11,-16188
  r11 = uint64_t(int64_t(int64_t(r11) + -16188));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82592178
  // (left out: dead in C++)
  f82592178(base, r3);
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258cf84
  if (cr6.eq) goto loc_8258CF84;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8258CF80)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_8258CF84:
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

// sub_8258CFA0: lifted from generated/ by lift.py.
lifted::Result f8258CFA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_8258CFA0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r10,r11,-16180
  r10 = uint64_t(int64_t(int64_t(r11) + -16180));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // std r11,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r11);
  // stb r9,9(r31)
  REX_STORE_U8(uint32_t(r31) + 9, uint8_t(r9));
  // std r11,32(r31)
  REX_STORE_U64(uint32_t(r31) + 32, r11);
  // std r11,40(r31)
  REX_STORE_U64(uint32_t(r31) + 40, r11);
  // stb r11,8(r31)
  REX_STORE_U8(uint32_t(r31) + 8, uint8_t(r11));
  // std r11,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r11);
  // bl 0x825b9a70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825B9A70, LIFTED_ARGS(0x8258CFE8)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // ld r10,80(r1)
  r10 = REX_LOAD_U64(uint32_t(r1) + 80);
  // mr r3,r31
  r3 = r31;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // std r10,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r10);
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // stb r11,8(r31)
  REX_STORE_U8(uint32_t(r31) + 8, uint8_t(r11));
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

// sub_8258D020: lifted from generated/ by lift.py.
void f8258D020(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D020 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,-16180
  r11 = uint64_t(int64_t(int64_t(r11) + -16180));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_8258D030: lifted from generated/ by lift.py.
uint64_t f8258D030(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D030 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lbz r11,8(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258d07c
  if (cr6.eq) goto loc_8258D07C;
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // bne cr6,0x8258d068
  if (!cr6.eq) goto loc_8258D068;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS(0x8258D064)); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
loc_8258D068:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // std r11,40(r31)
  REX_STORE_U64(uint32_t(r31) + 40, r11);
  // std r11,32(r31)
  REX_STORE_U64(uint32_t(r31) + 32, r11);
  // std r10,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r10);
  // stb r10,9(r31)
  REX_STORE_U8(uint32_t(r31) + 9, uint8_t(r10));
loc_8258D07C:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_8258D090: lifted from generated/ by lift.py.
uint64_t f8258D090(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D090 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // bne cr6,0x8258d0bc
  if (!cr6.eq) goto loc_8258D0BC;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS(0x8258D0B8)); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
loc_8258D0BC:
  // std r11,48(r31)
  REX_STORE_U64(uint32_t(r31) + 48, r11);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_8258D0D8: lifted from generated/ by lift.py.
lifted::Result f8258D0D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D0D8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lbz r11,8(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8258d114
  if (!cr6.eq) goto loc_8258D114;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f1,7136(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7136);
  f1 = double(temp.f32);
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
loc_8258D114:
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // bne cr6,0x8258d12c
  if (!cr6.eq) goto loc_8258D12C;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS(0x8258D128)); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
loc_8258D12C:
  // ld r10,48(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 48);
  // ld r9,16(r31)
  r9 = REX_LOAD_U64(uint32_t(r31) + 16);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // std r9,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r9);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // fdiv f0,f13,f0
  f0 = f13 / f0;
  // frsp f1,f0
  f1 = double(float(f0));
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

// sub_8258D170: lifted from generated/ by lift.py.
uint64_t f8258D170(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D170 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lbz r11,8(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258d1cc
  if (cr6.eq) goto loc_8258D1CC;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS(0x8258D198)); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 9);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // beq cr6,0x8258d1bc
  if (cr6.eq) goto loc_8258D1BC;
  // ld r9,40(r31)
  r9 = REX_LOAD_U64(uint32_t(r31) + 40);
  // ld r10,24(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 24);
  // subf r10,r10,r9
  r10 = r9 - r10;
  // add r10,r10,r11
  r10 = r10 + r11;
  // std r10,40(r31)
  REX_STORE_U64(uint32_t(r31) + 40, r10);
loc_8258D1BC:
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // std r11,32(r31)
  REX_STORE_U64(uint32_t(r31) + 32, r11);
  // std r10,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r10);
  // stb r10,9(r31)
  REX_STORE_U8(uint32_t(r31) + 9, uint8_t(r10));
loc_8258D1CC:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_8258D1E0: lifted from generated/ by lift.py.
uint64_t f8258D1E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D1E0 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lbz r11,8(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258d234
  if (cr6.eq) goto loc_8258D234;
  // lbz r11,9(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 9);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8258d234
  if (!cr6.eq) goto loc_8258D234;
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // bne cr6,0x8258d224
  if (!cr6.eq) goto loc_8258D224;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS(0x8258D220)); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
loc_8258D224:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // std r11,24(r31)
  REX_STORE_U64(uint32_t(r31) + 24, r11);
  // std r11,32(r31)
  REX_STORE_U64(uint32_t(r31) + 32, r11);
  // stb r10,9(r31)
  REX_STORE_U8(uint32_t(r31) + 9, uint8_t(r10));
loc_8258D234:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_8258D248: lifted from generated/ by lift.py.
void f8258D248(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D248 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  // lbz r11,8(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return;
  // lis r10,26214
  r10 = uint64_t(int64_t(1717960704));
  // ld r11,16(r3)
  r11 = REX_LOAD_U64(uint32_t(r3) + 16);
  // lis r9,26214
  r9 = uint64_t(int64_t(1717960704));
  // ori r10,r10,26215
  r10 = r10 | 26215;
  // ori r9,r9,26214
  r9 = r9 | 26214;
  // rldimi r10,r9,32,0
  r10 = (__builtin_rotateleft64(r9, 32) & 0xFFFFFFFF00000000) | (r10 & 0xFFFFFFFF);
  // mulhd r11,r11,r10
  r11 = uint64_t(int64_t(static_cast<int64_t>((static_cast<__int128>(static_cast<int64_t>(int64_t(r11))) * static_cast<__int128>(static_cast<int64_t>(int64_t(r10)))) >> 64)));
  // ld r10,24(r3)
  r10 = REX_LOAD_U64(uint32_t(r3) + 24);
  // sradi r11,r11,2
  ca = (int64_t(r11) < 0) & ((r11 & 0x3) != 0);
  r11 = uint64_t(int64_t(int64_t(r11) >> 2));
  // rldicl r9,r11,1,63
  r9 = __builtin_rotateleft64(r11, 1) & 0x1;
  // add r11,r11,r9
  r11 = r11 + r9;
  // add r11,r11,r10
  r11 = r11 + r10;
  // std r11,24(r3)
  REX_STORE_U64(uint32_t(r3) + 24, r11);
  // blr
  return;
}

// sub_8258D290: lifted from generated/ by lift.py.
lifted::Result f8258D290(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D290 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lbz r11,8(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8258d2cc
  if (!cr6.eq) goto loc_8258D2CC;
  // lis r11,-32254
  r11 = uint64_t(int64_t(-2113798144));
  // lfd f1,-7536(r11)
  lifted::ScalarFloatMode();
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -7536)));
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
loc_8258D2CC:
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // bne cr6,0x8258d2e4
  if (!cr6.eq) goto loc_8258D2E4;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS(0x8258D2E0)); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
loc_8258D2E4:
  // ld r10,16(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 16);
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // std r10,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r10);
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // fdiv f1,f0,f13
  f1 = f0 / f13;
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

// sub_8258D318: lifted from generated/ by lift.py.
lifted::Result f8258D318(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D318 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lbz r11,8(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8258d354
  if (!cr6.eq) goto loc_8258D354;
  // lis r11,-32254
  r11 = uint64_t(int64_t(-2113798144));
  // lfd f1,-7536(r11)
  lifted::ScalarFloatMode();
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -7536)));
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
loc_8258D354:
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // bne cr6,0x8258d36c
  if (!cr6.eq) goto loc_8258D36C;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS(0x8258D368)); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
loc_8258D36C:
  // ld r10,40(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 40);
  // ld r9,16(r31)
  r9 = REX_LOAD_U64(uint32_t(r31) + 16);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // std r9,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r9);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // fdiv f1,f13,f0
  f1 = f13 / f0;
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

// sub_8258D3A8: lifted from generated/ by lift.py.
lifted::Result f8258D3A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D3A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lbz r11,8(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8258d3e4
  if (!cr6.eq) goto loc_8258D3E4;
  // lis r11,-32254
  r11 = uint64_t(int64_t(-2113798144));
  // lfd f1,-7536(r11)
  lifted::ScalarFloatMode();
  f1 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + -7536)));
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
loc_8258D3E4:
  // ld r11,24(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 24);
  // cmpdi cr6,r11,0
  cr6 = lifted::Compare<int64_t>(int64_t(r11), 0, so);
  // bne cr6,0x8258d3fc
  if (!cr6.eq) goto loc_8258D3FC;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS(0x8258D3F8)); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
loc_8258D3FC:
  // ld r10,16(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 16);
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // ld r10,32(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 32);
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // std r11,32(r31)
  REX_STORE_U64(uint32_t(r31) + 32, r11);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f13,80(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // fdiv f1,f13,f0
  f1 = f13 / f0;
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

// sub_8258D440: lifted from generated/ by lift.py.
uint64_t f8258D440(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D440 (lifted)");
  // lbz r3,9(r3)
  r3 = REX_LOAD_U8(uint32_t(r3) + 9);
  // blr
  return r3;
}

// sub_8258D448: lifted from generated/ by lift.py.
lifted::Result f8258D448(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8258D448 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-16180
  r11 = uint64_t(int64_t(int64_t(r11) + -16180));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x8258d47c
  if (cr6.eq) goto loc_8258D47C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8258D478)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_8258D47C:
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

// sub_8258D490: lifted from generated/ by lift.py.
lifted::Result f8258D490(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8258D490 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r10,-32249
  r10 = uint64_t(int64_t(-2113470464));
  // mr r11,r3
  r11 = r3;
  // addi r10,r10,-13576
  r10 = uint64_t(int64_t(int64_t(r10) + -13576));
  // addi r3,r11,224
  r3 = uint64_t(int64_t(int64_t(r11) + 224));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x82592638
  { const lifted::Result o = f82592638(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8258D4A8: lifted from generated/ by lift.py.
uint64_t f8258D4A8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D4A8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,268(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 268);
  // addi r11,r11,68
  r11 = uint64_t(int64_t(int64_t(r11) + 68));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r3
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r3));
  // blr
  return r3;
}

// sub_8258D4C0: lifted from generated/ by lift.py.
void f8258D4C0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258D4C0 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,268(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 268);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // stw r11,268(r3)
  REX_STORE_U32(uint32_t(r3) + 268, uint32_t(r11));
  // blr
  return;
}

// sub_8258D4D8: lifted from generated/ by lift.py.
lifted::Result f8258D4D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8258D4D8 (lifted)");
  uint64_t r3 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // addi r3,r11,-17100
  r3 = uint64_t(int64_t(int64_t(r11) + -17100));
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lwz r9,-17108(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + -17108);
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // stw r9,-17108(r11)
  REX_STORE_U32(uint32_t(r11) + -17108, uint32_t(r9));
  // lwz r11,-17104(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -17104);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,-17104(r10)
  REX_STORE_U32(uint32_t(r10) + -17104, uint32_t(r11));
  // b 0x8258dd78
  { const lifted::Result o = f8258DD78(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8258D508: lifted from generated/ by lift.py.
lifted::Result f8258D508(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258D508 (lifted)");
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
  // addi r11,r11,-13576
  r11 = uint64_t(int64_t(int64_t(r11) + -13576));
  // addi r3,r31,224
  r3 = uint64_t(int64_t(int64_t(r31) + 224));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82592638
  // (left out: dead in C++)
  { const lifted::Result o = f82592638(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258d550
  if (cr6.eq) goto loc_8258D550;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8258D54C)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_8258D550:
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

// sub_8258DBA8: lifted from generated/ by lift.py.
lifted::Result f8258DBA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258DBA8 (lifted)");
  uint64_t r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r31,224
  r3 = uint64_t(int64_t(int64_t(r31) + 224));
  // bl 0x82592670
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82592670, LIFTED_ARGS(0x8258DBC8)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,140(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 140);
  // bl 0x8229d9d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8229D9D0, LIFTED_ARGS(0x8258DBD0)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // lwz r30,140(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 140);
  // bl 0x8258d9f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258D9F8, LIFTED_ARGS(0x8258DBDC)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x822a4340
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A4340, LIFTED_ARGS(0x8258DBEC)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x822a4340
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A4340, LIFTED_ARGS(0x8258DBFC)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x822a4340
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A4340, LIFTED_ARGS(0x8258DC0C)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x822a4340
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A4340, LIFTED_ARGS(0x8258DC1C)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x822a4060
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A4060, LIFTED_ARGS(0x8258DC28)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,272(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 272);
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A67B0, LIFTED_ARGS(0x8258DC30)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,276(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 276);
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A67B0, LIFTED_ARGS(0x8258DC38)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8229d978
  // (left out: dead in C++)
  { const lifted::Result o = f8229D978(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
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

// sub_8258DC58: lifted from generated/ by lift.py.
lifted::Result f8258DC58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258DC58 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,140(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 140);
  // bl 0x822a8a60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A8A60, LIFTED_ARGS(0x8258DC80)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8258daa8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258DAA8, LIFTED_ARGS(0x8258DC88)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,268(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 268);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,140(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 140);
  // addi r11,r11,68
  r11 = uint64_t(int64_t(int64_t(r11) + 68));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r4,r11,r31
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r31));
  // bl 0x822a08a0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A08A0, LIFTED_ARGS(0x8258DCA4)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,268(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 268);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // stw r11,268(r31)
  REX_STORE_U32(uint32_t(r31) + 268, uint32_t(r11));
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

// sub_8258DCC8: lifted from generated/ by lift.py.
lifted::Result f8258DCC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8258DCC8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r30,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r30));
  // bl 0x82295e78
  // (left out: dead in C++)
  { const lifted::Result o = f82295E78(base, r1, r13, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r3));
  // beq cr6,0x8258dd10
  if (cr6.eq) goto loc_8258DD10;
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // bne cr6,0x8258dd40
  if (!cr6.eq) goto loc_8258DD40;
loc_8258DD10:
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // stw r30,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r30));
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // addi r4,r11,-13544
  r4 = uint64_t(int64_t(int64_t(r11) + -13544));
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r11,-16324
  r3 = uint64_t(int64_t(int64_t(r11) + -16324));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r3,r11,-13572
  r3 = uint64_t(int64_t(int64_t(r11) + -13572));
  // bl 0x825a1318
  // (left out: dead in C++)
  { const lifted::Result o = f825A1318(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r11));
loc_8258DD40:
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
  return {r3, f1};
}

// sub_8258DD60: lifted from generated/ by lift.py.
lifted::Result f8258DD60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8258DD60 (lifted)");
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x82294e20
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8258DD78: lifted from generated/ by lift.py.
lifted::Result f8258DD78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258DD78 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beqlr cr6
  if (cr6.eq) return {r3, f1};
  // b 0x825ba1e0
  { const lifted::Result o = f825BA1E0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8258DD90: lifted from generated/ by lift.py.
lifted::Result f8258DD90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258DD90 (lifted)");
  uint64_t r12 = 0;
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
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8258ddbc
  if (cr6.eq) goto loc_8258DDBC;
  // lwz r4,0(r4)
  r4 = REX_LOAD_U32(uint32_t(r4) + 0);
  // bl 0x822970a0
  // (left out: dead in C++)
  { const lifted::Result o = f822970A0(base, r1, r13, r3, r4, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // beq cr6,0x8258ddc0
  if (cr6.eq) goto loc_8258DDC0;
loc_8258DDBC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8258DDC0:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8258DDD0: lifted from generated/ by lift.py.
void f8258DDD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258DDD0 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,-13520
  r11 = uint64_t(int64_t(int64_t(r11) + -13520));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // b 0x8258e530
  f8258E530(base, r3);
  return;
}

// sub_8258DDE0: lifted from generated/ by lift.py.
lifted::Result f8258DDE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258DDE0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x8258de20
  if (cr6.eq) goto loc_8258DE20;
  // lwz r4,0(r4)
  r4 = REX_LOAD_U32(uint32_t(r4) + 0);
  // bl 0x822970a0
  // (left out: dead in C++)
  { const lifted::Result o = f822970A0(base, r1, r13, r3, r4, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne cr6,0x8258de38
  if (!cr6.eq) goto loc_8258DE38;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x82294e20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82294E20, LIFTED_ARGS(0x8258DE18)); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
loc_8258DE20:
  // li r3,1
  r3 = uint64_t(int64_t(1));
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
loc_8258DE38:
  // li r3,0
  r3 = uint64_t(int64_t(0));
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

// sub_8258DE50: lifted from generated/ by lift.py.
lifted::Result f8258DE50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8258DE50 (lifted)");
  uint64_t r11 = 0, r12 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r3,8(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8258DE70)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8258DEB8: lifted from generated/ by lift.py.
lifted::Result f8258DEB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258DEB8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r11,r11,-13520
  r11 = uint64_t(int64_t(int64_t(r11) + -13520));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8258e530
  // (left out: dead in C++)
  f8258E530(base, r3);
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258defc
  if (cr6.eq) goto loc_8258DEFC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8258DEF8)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_8258DEFC:
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

// sub_8258E530: lifted from generated/ by lift.py.
void f8258E530(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258E530 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,3796
  r11 = uint64_t(int64_t(int64_t(r11) + 3796));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_8258E540: lifted from generated/ by lift.py.
lifted::Result f8258E540(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8258E540 (lifted)");
  uint64_t r3 = 0;
  double f1 = 0.0;
  // lwz r3,0(r4)
  r3 = REX_LOAD_U32(uint32_t(r4) + 0);
  // b 0x82295500
  { const lifted::Result o = f82295500(base, r1, r13, r3, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8258E548: lifted from generated/ by lift.py.
lifted::Result f8258E548(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_8258E548 (lifted)");
  uint64_t r12 = 0, r31 = 0;
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
  // bl 0x825db954
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__RtlInitializeCriticalSection, LIFTED_ARGS(0x8258E560)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
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

// sub_8258E578: lifted from generated/ by lift.py.
lifted::Result f8258E578(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8258E578 (lifted)");
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // b 0x825db8d4
  { const lifted::Result o = lifted::Import(base, __imp__RtlLeaveCriticalSection, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8258E5C8: lifted from generated/ by lift.py.
lifted::Result f8258E5C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8258E5C8 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // addi r3,r11,4
  r3 = uint64_t(int64_t(int64_t(r11) + 4));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8258E698: lifted from generated/ by lift.py.
lifted::Result f8258E698(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8258E698 (lifted)");
  uint64_t r3 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // addi r31,r4,16
  r31 = uint64_t(int64_t(int64_t(r4) + 16));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmpwi cr6,r11,36
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 36, so);
  // beq cr6,0x8258e6f8
  if (cr6.eq) goto loc_8258E6F8;
  // lis r10,-32154
  r10 = uint64_t(int64_t(-2107244544));
  // addi r30,r10,-4640
  r30 = uint64_t(int64_t(int64_t(r10) + -4640));
loc_8258E6C4:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r5,8(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // lwzx r11,r11,r30
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r30));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8258E6E0)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,1
  r11 = uint64_t(int64_t(int64_t(r3) + 1));
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r31,r11,r31
  r31 = r11 + r31;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmpwi cr6,r11,36
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 36, so);
  // bne cr6,0x8258e6c4
  if (!cr6.eq) goto loc_8258E6C4;
loc_8258E6F8:
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
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

// sub_8258E718: lifted from generated/ by lift.py.
lifted::Result f8258E718(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_8258E718 (lifted)");
  uint64_t r3 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r4
  r29 = r4;
  // addi r31,r29,16
  r31 = uint64_t(int64_t(int64_t(r29) + 16));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmpwi cr6,r11,36
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 36, so);
  // beq cr6,0x8258e774
  if (cr6.eq) goto loc_8258E774;
  // lis r10,-32154
  r10 = uint64_t(int64_t(-2107244544));
  // addi r30,r10,-4640
  r30 = uint64_t(int64_t(int64_t(r10) + -4640));
loc_8258E740:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r5,8(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r3,r31,16
  r3 = uint64_t(int64_t(int64_t(r31) + 16));
  // lwzx r11,r11,r30
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r30));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8258E75C)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,1
  r11 = uint64_t(int64_t(int64_t(r3) + 1));
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r31,r11,r31
  r31 = r11 + r31;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmpwi cr6,r11,36
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 36, so);
  // bne cr6,0x8258e740
  if (!cr6.eq) goto loc_8258E740;
loc_8258E774:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258E918: lifted from generated/ by lift.py.
uint64_t f8258E918(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258E918 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  uint8_t ca = 0;
  lifted::Temp temp{};
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // addi r8,r3,16
  r8 = uint64_t(int64_t(int64_t(r3) + 16));
  // lwz r9,4(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r11,3004(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 3004);
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // subfic r8,r10,0
  ca = uint32_t(r10) <= 0;
  r8 = static_cast<uint64_t>(0) - r10;
  // stw r9,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r9));
  // subfe r8,r8,r8
  temp.u8 = (~uint32_t(r8) + uint32_t(r8) < ~uint32_t(r8)) | (~uint32_t(r8) + uint32_t(r8) + ca < ca);
  r8 = ~r8 + r8 + ca;
  ca = temp.u8;
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // rlwinm r8,r8,0,30,30
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0x2;
  // addi r8,r8,2
  r8 = uint64_t(int64_t(int64_t(r8) + 2));
  // mullw r9,r8,r9
  r9 = uint64_t(int64_t(int64_t(int32_t(r8)) * int64_t(int32_t(r9))));
  // addi r11,r9,15
  r11 = uint64_t(int64_t(int64_t(r9) + 15));
  // rlwinm r3,r11,28,4,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0xFFFFFFF;
  // blr
  return r3;
}

// sub_8258EF60: lifted from generated/ by lift.py.
lifted::Result f8258EF60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258EF60 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r28,r3
  r28 = r3;
  // lwz r30,4(r28)
  r30 = REX_LOAD_U32(uint32_t(r28) + 4);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x8258efb0
  if (cr6.eq) goto loc_8258EFB0;
  // addi r31,r28,16
  r31 = uint64_t(int64_t(int64_t(r28) + 16));
  // lis r29,-32105
  r29 = uint64_t(int64_t(-2104033280));
loc_8258EF84:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r3,-17008(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + -17008);
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // add r11,r3,r11
  r11 = r3 + r11;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8258EFA0)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x8258ef84
  if (!cr6.eq) goto loc_8258EF84;
loc_8258EFB0:
  // lwz r3,4(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258EFC0: lifted from generated/ by lift.py.
lifted::Result f8258EFC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8258EFC0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f01c
  if (!cr6.gt) goto loc_8258F01C;
  // addi r31,r29,20
  r31 = uint64_t(int64_t(int64_t(r29) + 20));
  // lis r28,-32105
  r28 = uint64_t(int64_t(-2104033280));
loc_8258EFE8:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r3,-17008(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -17008);
  // lwz r5,4(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 4);
  // add r11,r3,r11
  r11 = r3 + r11;
  // lwz r4,-4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + -4);
  // lwz r11,440(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 440);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8258F008)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258efe8
  if (cr6.lt) goto loc_8258EFE8;
loc_8258F01C:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258F028: lifted from generated/ by lift.py.
uint64_t f8258F028(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258F028 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f074
  if (!cr6.gt) goto loc_8258F074;
  // addi r31,r29,16
  r31 = uint64_t(int64_t(int64_t(r29) + 16));
  // lis r28,-32105
  r28 = uint64_t(int64_t(-2104033280));
loc_8258F050:
  // lwz r5,4(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r3,-17008(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -17008);
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x822a3b30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A3B30, LIFTED_ARGS(0x8258F060)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258f050
  if (cr6.lt) goto loc_8258F050;
loc_8258F074:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_8258F080: lifted from generated/ by lift.py.
lifted::Result f8258F080(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8258F080 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r28,r3
  r28 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f0e0
  if (!cr6.gt) goto loc_8258F0E0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r31,r28,16
  r31 = uint64_t(int64_t(int64_t(r28) + 16));
  // lis r27,-32105
  r27 = uint64_t(int64_t(-2104033280));
  // rldicr r29,r11,63,63
  r29 = __builtin_rotateleft64(r11, 63) & 0xFFFFFFFFFFFFFFFF;
loc_8258F0B0:
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r3,-17008(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -17008);
  // addi r11,r4,20
  r11 = uint64_t(int64_t(int64_t(r4) + 20));
  // lwz r5,4(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 4);
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // srd r6,r29,r11
  r6 = uint8_t(r11) & 0x40 ? 0 : (r29 >> (uint8_t(r11) & 0x7F));
  // bl 0x822a6008
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A6008, LIFTED_ARGS(0x8258F0CC)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258f0b0
  if (cr6.lt) goto loc_8258F0B0;
loc_8258F0E0:
  // lwz r3,4(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258F3E0: lifted from generated/ by lift.py.
lifted::Result f8258F3E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258F3E0 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f428
  if (!cr6.gt) goto loc_8258F428;
  // addi r31,r29,16
  r31 = uint64_t(int64_t(int64_t(r29) + 16));
  // lis r28,-32105
  r28 = uint64_t(int64_t(-2104033280));
loc_8258F408:
  // lwz r3,-17008(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -17008);
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x822a8958
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A8958, LIFTED_ARGS(0x8258F414)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258f408
  if (cr6.lt) goto loc_8258F408;
loc_8258F428:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258F438: lifted from generated/ by lift.py.
uint64_t f8258F438(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258F438 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f480
  if (!cr6.gt) goto loc_8258F480;
  // addi r31,r29,16
  r31 = uint64_t(int64_t(int64_t(r29) + 16));
  // lis r28,-32105
  r28 = uint64_t(int64_t(-2104033280));
loc_8258F460:
  // lwz r3,-17008(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -17008);
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x822a8618
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A8618, LIFTED_ARGS(0x8258F46C)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258f460
  if (cr6.lt) goto loc_8258F460;
loc_8258F480:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_8258F490: lifted from generated/ by lift.py.
uint64_t f8258F490(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8258F490 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f4d8
  if (!cr6.gt) goto loc_8258F4D8;
  // addi r31,r29,16
  r31 = uint64_t(int64_t(int64_t(r29) + 16));
  // lis r28,-32105
  r28 = uint64_t(int64_t(-2104033280));
loc_8258F4B8:
  // lwz r3,-17008(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -17008);
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x822a8960
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A8960, LIFTED_ARGS(0x8258F4C4)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258f4b8
  if (cr6.lt) goto loc_8258F4B8;
loc_8258F4D8:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_8258F4E8: lifted from generated/ by lift.py.
lifted::Result f8258F4E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_8258F4E8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
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
  // mr r28,r3
  r28 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f55c
  if (!cr6.gt) goto loc_8258F55C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r31,r28,24
  r31 = uint64_t(int64_t(int64_t(r28) + 24));
  // lis r27,-32105
  r27 = uint64_t(int64_t(-2104033280));
  // rldicr r29,r11,63,63
  r29 = __builtin_rotateleft64(r11, 63) & 0xFFFFFFFFFFFFFFFF;
loc_8258F518:
  // lwz r4,-8(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + -8);
  // lwz r3,-17008(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -17008);
  // subfic r11,r4,95
  ca = uint32_t(r4) <= 95;
  r11 = static_cast<uint64_t>(95) - r4;
  // lwz r7,4(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r6,0(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mulli r11,r11,21846
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(21846))));
  // lwz r5,-4(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + -4);
  // rlwinm r11,r11,16,16,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 16) & 0xFFFF;
  // addi r11,r11,20
  r11 = uint64_t(int64_t(int64_t(r11) + 20));
  // clrldi r11,r11,32
  r11 = r11 & 0xFFFFFFFF;
  // srd r8,r29,r11
  r8 = uint8_t(r11) & 0x40 ? 0 : (r29 >> (uint8_t(r11) & 0x7F));
  // bl 0x822a3758
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A3758, LIFTED_ARGS(0x8258F548)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258f518
  if (cr6.lt) goto loc_8258F518;
loc_8258F55C:
  // lwz r3,4(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258F568: lifted from generated/ by lift.py.
lifted::Result f8258F568(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8258F568 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f5b0
  if (!cr6.gt) goto loc_8258F5B0;
  // addi r31,r29,16
  r31 = uint64_t(int64_t(int64_t(r29) + 16));
  // lis r28,-32105
  r28 = uint64_t(int64_t(-2104033280));
loc_8258F590:
  // lwz r3,-17008(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -17008);
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x822a3908
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A3908, LIFTED_ARGS(0x8258F59C)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258f590
  if (cr6.lt) goto loc_8258F590;
loc_8258F5B0:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258F6A0: lifted from generated/ by lift.py.
lifted::Result f8258F6A0(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_8258F6A0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // addi r31,r11,3000
  r31 = uint64_t(int64_t(int64_t(r11) + 3000));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8258f764
  if (cr6.eq) goto loc_8258F764;
  // lwz r3,-20008(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -20008);
  // bl 0x822a2470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A2470, LIFTED_ARGS(0x8258F6CC)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r30,r3
  r30 = r3;
  // lwz r3,-20008(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -20008);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x8258f6ec
  if (cr6.eq) goto loc_8258F6EC;
  // li r4,1
  r4 = uint64_t(int64_t(1));
loc_8258F6EC:
  // bl 0x822a2430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A2430, LIFTED_ARGS(0x8258F6F0)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r4,-20008(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + -20008);
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r10,28(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lwz r9,24(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 24);
  // lwz r8,20(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r7,16(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 16);
  // lwz r6,12(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r5,0(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,16(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8258F724)); r3 = o.r3; f1 = o.f1; }
  // li r8,4096
  r8 = uint64_t(int64_t(4096));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r3,-20008(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -20008);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x822a3758
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A3758, LIFTED_ARGS(0x8258F740)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,-20008(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -20008);
  // rldicr r6,r6,43,63
  r6 = __builtin_rotateleft64(r6, 43) & 0xFFFFFFFFFFFFFFFF;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x822a6008
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A6008, LIFTED_ARGS(0x8258F758)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // lwz r3,-20008(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -20008);
  // bl 0x822a2430
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A2430, LIFTED_ARGS(0x8258F764)); r3 = o.r3; f1 = o.f1; }
loc_8258F764:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258F770: lifted from generated/ by lift.py.
lifted::Result f8258F770(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258F770 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32105
  r10 = uint64_t(int64_t(-2104033280));
  // lwz r4,4(r3)
  r4 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r9,12(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 12);
  // lwz r5,8(r3)
  r5 = REX_LOAD_U32(uint32_t(r3) + 8);
  // addi r11,r11,2384
  r11 = uint64_t(int64_t(int64_t(r11) + 2384));
  // lwz r3,-17008(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + -17008);
  // rlwinm r10,r4,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 3) & 0xFFFFFFF8;
  // addi r8,r11,4
  r8 = uint64_t(int64_t(int64_t(r11) + 4));
  // lwzx r11,r10,r11
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // lwzx r10,r10,r8
  r10 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r8));
  // mullw r11,r11,r9
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r9))));
  // add r6,r11,r10
  r6 = r11 + r10;
  // bl 0x825be300
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BE300, LIFTED_ARGS(0x8258F7B4)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8258F7C8: lifted from generated/ by lift.py.
lifted::Result f8258F7C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258F7C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  uint8_t ca = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r9,-32105
  r9 = uint64_t(int64_t(-2104033280));
  // lwz r4,4(r3)
  r4 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // lwz r8,12(r3)
  r8 = REX_LOAD_U32(uint32_t(r3) + 12);
  // addi r11,r11,2384
  r11 = uint64_t(int64_t(int64_t(r11) + 2384));
  // clrlwi r6,r10,16
  r6 = uint32_t(r10) & 0xFFFF;
  // lwz r3,-17008(r9)
  r3 = REX_LOAD_U32(uint32_t(r9) + -17008);
  // rlwinm r9,r4,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 3) & 0xFFFFFFF8;
  // addi r7,r11,4
  r7 = uint64_t(int64_t(int64_t(r11) + 4));
  // srawi r5,r10,16
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0xFFFF) != 0);
  r5 = uint64_t(int64_t(int32_t(r10) >> 16));
  // lwzx r11,r9,r11
  r11 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r11));
  // lwzx r10,r9,r7
  r10 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r7));
  // mullw r11,r11,r8
  r11 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r8))));
  // add r7,r11,r10
  r7 = r11 + r10;
  // bl 0x825be708
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BE708, LIFTED_ARGS(0x8258F814)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_8258F828: lifted from generated/ by lift.py.
lifted::Result f8258F828(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258F828 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r30,-32105
  r30 = uint64_t(int64_t(-2104033280));
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r3,-17008(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -17008);
  // lhz r6,10(r31)
  r6 = REX_LOAD_U16(uint32_t(r31) + 10);
  // lhz r5,8(r31)
  r5 = REX_LOAD_U16(uint32_t(r31) + 8);
  // bl 0x825bd8f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BD8F0, LIFTED_ARGS(0x8258F858)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8258f87c
  if (cr6.eq) goto loc_8258F87C;
  // lhz r11,10(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 10);
  // lhz r10,8(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 8);
  // lwz r4,12(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 12);
  // mullw r5,r11,r10
  r5 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r10))));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x8258F874)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-17008(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -17008);
  // bl 0x825bdda0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BDDA0, LIFTED_ARGS(0x8258F87C)); r3 = o.r3; f1 = o.f1; }
loc_8258F87C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
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

// sub_8258F898: lifted from generated/ by lift.py.
lifted::Result f8258F898(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258F898 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f8e0
  if (!cr6.gt) goto loc_8258F8E0;
  // addi r31,r29,16
  r31 = uint64_t(int64_t(int64_t(r29) + 16));
loc_8258F8BC:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x8258f8cc
  if (cr6.eq) goto loc_8258F8CC;
  // bl 0x822a67b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A67B0, LIFTED_ARGS(0x8258F8CC)); r3 = o.r3; f1 = o.f1; }
loc_8258F8CC:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258f8bc
  if (cr6.lt) goto loc_8258F8BC;
loc_8258F8E0:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258F8F0: lifted from generated/ by lift.py.
lifted::Result f8258F8F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258F8F0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f93c
  if (!cr6.gt) goto loc_8258F93C;
  // addi r31,r29,20
  r31 = uint64_t(int64_t(int64_t(r29) + 20));
  // lis r28,-32105
  r28 = uint64_t(int64_t(-2104033280));
loc_8258F918:
  // lwz r3,-17008(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -17008);
  // lwz r5,4(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x822a4340
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A4340, LIFTED_ARGS(0x8258F928)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258f918
  if (cr6.lt) goto loc_8258F918;
loc_8258F93C:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258F948: lifted from generated/ by lift.py.
lifted::Result f8258F948(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258F948 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8258f990
  if (!cr6.gt) goto loc_8258F990;
  // addi r31,r29,20
  r31 = uint64_t(int64_t(int64_t(r29) + 20));
  // lis r28,-32105
  r28 = uint64_t(int64_t(-2104033280));
loc_8258F970:
  // lwz r3,-17008(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -17008);
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // bl 0x822a4060
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822A4060, LIFTED_ARGS(0x8258F97C)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8258f970
  if (cr6.lt) goto loc_8258F970;
loc_8258F990:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258F9A0: lifted from generated/ by lift.py.
lifted::Result f8258F9A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9) {
  ITC_NATIVE_RUNNING("sub_8258F9A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r11,r31,16
  r11 = uint64_t(int64_t(int64_t(r31) + 16));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lbz r3,45(r11)
  r3 = REX_LOAD_U8(uint32_t(r11) + 45);
  // lbz r9,44(r11)
  r9 = REX_LOAD_U8(uint32_t(r11) + 44);
  // lbz r29,46(r11)
  r29 = REX_LOAD_U8(uint32_t(r11) + 46);
  // lbz r28,47(r11)
  r28 = REX_LOAD_U8(uint32_t(r11) + 47);
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // std r3,120(r1)
  REX_STORE_U64(uint32_t(r1) + 120, r3);
  // std r9,112(r1)
  REX_STORE_U64(uint32_t(r1) + 112, r9);
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // std r29,128(r1)
  REX_STORE_U64(uint32_t(r1) + 128, r29);
  // clrlwi r4,r8,31
  r4 = uint32_t(r8) & 0x1;
  // std r28,144(r1)
  REX_STORE_U64(uint32_t(r1) + 144, r28);
  // lfd f11,144(r1)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 144)));
  // fcfid f11,f11
  f11 = double(std::bit_cast<int64_t>(f11));
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // frsp f11,f11
  f11 = double(float(f11));
  // lfd f13,120(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 120)));
  // lfd f0,112(r1)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 112)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // lfd f12,128(r1)
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 128)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // fcfid f12,f12
  f12 = double(std::bit_cast<int64_t>(f12));
  // frsp f13,f13
  f13 = double(float(f13));
  // frsp f10,f0
  f10 = double(float(f0));
  // lfs f0,8704(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 8704);
  f0 = double(temp.f32);
  // frsp f12,f12
  f12 = double(float(f12));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // stfs f13,148(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 148, temp.u32);
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // stfs f10,144(r1)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r1) + 144, temp.u32);
  // fmuls f13,f12,f0
  f13 = double(float(f12 * f0));
  // stfs f13,152(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 152, temp.u32);
  // fmuls f0,f11,f0
  f0 = double(float(f11 * f0));
  // stfs f0,156(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 156, temp.u32);
  // beq cr6,0x8258fa54
  if (cr6.eq) goto loc_8258FA54;
  // addi r5,r11,8
  r5 = uint64_t(int64_t(int64_t(r11) + 8));
loc_8258FA54:
  // rlwinm r9,r8,0,30,30
  r9 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0x2;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8258fa64
  if (cr6.eq) goto loc_8258FA64;
  // lwz r6,24(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 24);
loc_8258FA64:
  // rlwinm r9,r8,0,29,29
  r9 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0x4;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8258fa74
  if (cr6.eq) goto loc_8258FA74;
  // addi r7,r11,28
  r7 = uint64_t(int64_t(int64_t(r11) + 28));
loc_8258FA74:
  // rlwinm r9,r8,0,28,28
  r9 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0x8;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8258fa84
  if (cr6.eq) goto loc_8258FA84;
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
loc_8258FA84:
  // rlwinm r9,r8,0,27,27
  r9 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 0) & 0x10;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8258fa94
  if (cr6.eq) goto loc_8258FA94;
  // addi r30,r11,56
  r30 = uint64_t(int64_t(int64_t(r11) + 56));
loc_8258FA94:
  // lis r4,-32105
  r4 = uint64_t(int64_t(-2104033280));
  // lwz r29,52(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 52);
  // lfs f1,48(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 48);
  f1 = double(temp.f32);
  // lwz r9,40(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 40);
  // lwz r8,36(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 36);
  // stw r30,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r30));
  // lwz r3,-17008(r4)
  r3 = REX_LOAD_U32(uint32_t(r4) + -17008);
  // lwz r4,4(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r29,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r29));
  // bl 0x822afa58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822AFA58, LIFTED_ARGS(0x8258FABC)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8258FAC8: lifted from generated/ by lift.py.
lifted::Result f8258FAC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258FAC8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // lis r11,-32105
  r11 = uint64_t(int64_t(-2104033280));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r9,48(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 48);
  // lfs f1,44(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 44);
  f1 = double(temp.f32);
  // lwz r3,-17008(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -17008);
  // lwz r7,40(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lwz r6,20(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 20);
  // bl 0x822a04f8
  // (left out: dead in C++)
  { const lifted::Result o = f822A04F8(base, r1, r13, r3, r4, r5, r6, r7, r9, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13); r3 = o.r3; f1 = o.f1; }
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
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

// sub_8258FB20: lifted from generated/ by lift.py.
lifted::Result f8258FB20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8258FB20 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // addi r9,r1,100
  r9 = uint64_t(int64_t(int64_t(r1) + 100));
  // addi r31,r30,16
  r31 = uint64_t(int64_t(int64_t(r30) + 16));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r29,-32105
  r29 = uint64_t(int64_t(-2104033280));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r9,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r9));
  // lwz r5,4(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r3,-17008(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + -17008);
  // lwz r9,20(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r8,16(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r7,12(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lwz r6,8(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 8);
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // bl 0x825bddc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BDDC0, LIFTED_ARGS(0x8258FB70)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x8258fbc0
  if (!cr6.eq) goto loc_8258FBC0;
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // beq cr6,0x8258fb8c
  if (cr6.eq) goto loc_8258FB8C;
  // li r11,4
  r11 = uint64_t(int64_t(4));
loc_8258FB8C:
  // lwz r10,12(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lwz r3,96(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 96);
  // mullw r5,r10,r11
  r5 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r11))));
  // lwz r4,24(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 24);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x8258FBA0)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // mullw r5,r11,r10
  r5 = uint64_t(int64_t(int64_t(int32_t(r11)) * int64_t(int32_t(r10))));
  // lwz r4,28(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 28);
  // lwz r3,100(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 100);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x8258FBB8)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-17008(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + -17008);
  // bl 0x825bdda0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BDDA0, LIFTED_ARGS(0x8258FBC0)); r3 = o.r3; f1 = o.f1; }
loc_8258FBC0:
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82590C48: lifted from generated/ by lift.py.
lifted::Result f82590C48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82590C48 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r11,r11,-13432
  r11 = uint64_t(int64_t(int64_t(r11) + -13432));
  // mr r30,r4
  r30 = r4;
  // lwz r3,20(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 20);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71C0, LIFTED_ARGS(0x82590C78)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,24(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 24);
  // bl 0x825a71c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71C0, LIFTED_ARGS(0x82590C80)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82590c98
  if (cr6.eq) goto loc_82590C98;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82590C94)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_82590C98:
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

// sub_82590DE0: lifted from generated/ by lift.py.
lifted::Result f82590DE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82590DE0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // addi r11,r11,-13428
  r11 = uint64_t(int64_t(int64_t(r11) + -13428));
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x82590e24
  if (cr6.eq) goto loc_82590E24;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82590E24)); r3 = o.r3; f1 = o.f1; }
loc_82590E24:
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82590e44
  if (cr6.eq) goto loc_82590E44;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82590E44)); r3 = o.r3; f1 = o.f1; }
loc_82590E44:
  // lwz r30,4(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x82590e84
  if (cr6.eq) goto loc_82590E84;
  // addi r31,r30,68
  r31 = uint64_t(int64_t(int64_t(r30) + 68));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82590e64
  if (cr6.eq) goto loc_82590E64;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82590E64)); r3 = o.r3; f1 = o.f1; }
loc_82590E64:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r3,r30,4
  r3 = uint64_t(int64_t(int64_t(r30) + 4));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // bl 0x825a13c0
  // (left out: dead in C++)
  { const lifted::Result o = f825A13C0(base, r1, r13, r3, r6, r7, r8, r9, f1, 0x82590E7C); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82590E84)); r3 = o.r3; f1 = o.f1; }
loc_82590E84:
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

// sub_82590EA0: lifted from generated/ by lift.py.
lifted::Result f82590EA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82590EA0 (lifted)");
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
  // bl 0x82590de0
  // (left out: dead in C++)
  { const lifted::Result o = f82590DE0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82590ed8
  if (cr6.eq) goto loc_82590ED8;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82590ED4)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_82590ED8:
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

// sub_825916F0: lifted from generated/ by lift.py.
lifted::Result f825916F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825916F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x8259178c
  if (!cr6.gt) goto loc_8259178C;
  // addi r31,r29,20
  r31 = uint64_t(int64_t(int64_t(r29) + 20));
  // lis r28,-32105
  r28 = uint64_t(int64_t(-2104033280));
loc_82591718:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8259172C)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82591740)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82591778
  if (!cr6.eq) goto loc_82591778;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // bne cr6,0x82591778
  if (!cr6.eq) goto loc_82591778;
  // mr r4,r31
  r4 = r31;
  // lwz r3,-17012(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -17012);
  // bl 0x82591540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82591540, LIFTED_ARGS(0x82591764)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82591778)); r3 = o.r3; f1 = o.f1; }
loc_82591778:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r31,r31,16
  r31 = uint64_t(int64_t(int64_t(r31) + 16));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x82591718
  if (cr6.lt) goto loc_82591718;
loc_8259178C:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82591898: lifted from generated/ by lift.py.
lifted::Result f82591898(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82591898 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-13340
  r11 = uint64_t(int64_t(int64_t(r11) + -13340));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825918cc
  if (cr6.eq) goto loc_825918CC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x825918C8)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825918CC:
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

// sub_825918E0: lifted from generated/ by lift.py.
void f825918E0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_825918E0 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,-13340
  r11 = uint64_t(int64_t(int64_t(r11) + -13340));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_82591BB8: lifted from generated/ by lift.py.
uint64_t f82591BB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82591BB8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r29,r3
  r29 = r3;
  // addi r11,r11,-13324
  r11 = uint64_t(int64_t(int64_t(r11) + -13324));
  // addi r31,r29,192
  r31 = uint64_t(int64_t(int64_t(r29) + 192));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_82591BDC:
  // addi r31,r31,-12
  r31 = uint64_t(int64_t(int64_t(r31) + -12));
  // mr r3,r31
  r3 = r31;
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x82591BE8)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge cr6,0x82591bdc
  if (!cr6.lt) goto loc_82591BDC;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,-13340
  r11 = uint64_t(int64_t(int64_t(r11) + -13340));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82592058: lifted from generated/ by lift.py.
lifted::Result f82592058(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82592058 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // bl 0x82591bb8
  // (left out: dead in C++)
  r3 = f82591BB8(base, r1, r13, r3);
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82592090
  if (cr6.eq) goto loc_82592090;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x8259208C)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_82592090:
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

// sub_825920A8: lifted from generated/ by lift.py.
void f825920A8(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825920A8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lwz r11,0(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 0);
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r11,r11,r4
  r11 = r11 + r4;
  // ld r10,0(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 0);
  // std r10,0(r3)
  REX_STORE_U64(uint32_t(r3) + 0, r10);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // std r11,8(r3)
  REX_STORE_U64(uint32_t(r3) + 8, r11);
  // blr
  return;
}

// sub_82592178: lifted from generated/ by lift.py.
void f82592178(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82592178 (lifted)");
  uint64_t r11 = 0;
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // addi r11,r11,-13312
  r11 = uint64_t(int64_t(int64_t(r11) + -13312));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_82592400: lifted from generated/ by lift.py.
uint64_t f82592400(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82592400 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // addi r9,r31,128
  r9 = uint64_t(int64_t(int64_t(r31) + 128));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // addi r11,r31,64
  r11 = uint64_t(int64_t(int64_t(r31) + 64));
  // stfs f0,80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // stfs f0,88(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // stfs f0,100(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // stfs f0,108(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // stfs f0,112(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 112, temp.u32);
  // stfs f0,116(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // stfs f0,120(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 120, temp.u32);
  // stfs f0,124(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
loc_82592454:
  // addi r8,r1,80
  r8 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r7,r1,96
  r7 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r6,r11,-32
  r6 = uint64_t(int64_t(int64_t(r11) + -32));
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r4,r11
  r4 = r11;
  // ld r30,0(r8)
  r30 = REX_LOAD_U64(uint32_t(r8) + 0);
  // addi r3,r11,32
  r3 = uint64_t(int64_t(int64_t(r11) + 32));
  // ld r8,8(r8)
  r8 = REX_LOAD_U64(uint32_t(r8) + 8);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // ld r29,0(r7)
  r29 = REX_LOAD_U64(uint32_t(r7) + 0);
  // addi r11,r11,16
  r11 = uint64_t(int64_t(int64_t(r11) + 16));
  // ld r7,8(r7)
  r7 = REX_LOAD_U64(uint32_t(r7) + 8);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // ld r28,0(r5)
  r28 = REX_LOAD_U64(uint32_t(r5) + 0);
  // std r30,0(r6)
  REX_STORE_U64(uint32_t(r6) + 0, r30);
  // std r8,8(r6)
  REX_STORE_U64(uint32_t(r6) + 8, r8);
  // ld r8,8(r5)
  r8 = REX_LOAD_U64(uint32_t(r5) + 8);
  // std r29,0(r4)
  REX_STORE_U64(uint32_t(r4) + 0, r29);
  // std r7,8(r4)
  REX_STORE_U64(uint32_t(r4) + 8, r7);
  // std r28,0(r3)
  REX_STORE_U64(uint32_t(r3) + 0, r28);
  // std r8,8(r3)
  REX_STORE_U64(uint32_t(r3) + 8, r8);
  // stfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r9) + 0, temp.u32);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bne cr6,0x82592454
  if (!cr6.eq) goto loc_82592454;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r5,128
  r5 = uint64_t(int64_t(128));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r31,284
  r3 = uint64_t(int64_t(int64_t(r31) + 284));
  // stw r11,272(r31)
  REX_STORE_U32(uint32_t(r31) + 272, uint32_t(r11));
  // stw r11,264(r31)
  REX_STORE_U32(uint32_t(r31) + 264, uint32_t(r11));
  // stw r11,268(r31)
  REX_STORE_U32(uint32_t(r31) + 268, uint32_t(r11));
  // bl 0x825a71d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A71D0, LIFTED_ARGS(0x825924D4)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_825925A8: lifted from generated/ by lift.py.
lifted::Result f825925A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825925A8 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-13312
  r11 = uint64_t(int64_t(int64_t(r11) + -13312));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x825925dc
  if (cr6.eq) goto loc_825925DC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x825925D8)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_825925DC:
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

// sub_825925F0: lifted from generated/ by lift.py.
lifted::Result f825925F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825925F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // bl 0x8258dcc8
  // (left out: dead in C++)
  { const lifted::Result o = f8258DCC8(base, r1, r13, r3, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x8258e548
  // (left out: dead in C++)
  { const lifted::Result o = f8258E548(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // stw r11,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r11));
  // stw r11,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r11));
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

// sub_82592638: lifted from generated/ by lift.py.
lifted::Result f82592638(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82592638 (lifted)");
  uint64_t r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // addi r3,r31,4
  r3 = uint64_t(int64_t(int64_t(r31) + 4));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x82592654)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x8258dd60
  // (left out: dead in C++)
  { const lifted::Result o = f8258DD60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x8259265C); r3 = o.r3; f1 = o.f1; }
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

// sub_82592900: lifted from generated/ by lift.py.
lifted::Result f82592900(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82592900 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
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
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825a6cc0
  // (left out: dead in C++)
  { const lifted::Result o = f825A6CC0(base, r1, r13, r3, r4, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82592940
  if (cr0.eq) goto loc_82592940;
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x82592940
  if (!cr6.lt) goto loc_82592940;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
loc_82592940:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82592950
  if (cr6.eq) goto loc_82592950;
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_82592950:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825a6d08
  // (left out: dead in C++)
  { const lifted::Result o = f825A6D08(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, 0x82592958); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
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

// sub_82593B10: lifted from generated/ by lift.py.
lifted::Result f82593B10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82593B10 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r3,r4
  r3 = r4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x82593b94
  if (!cr6.eq) goto loc_82593B94;
  // lbz r11,0(r5)
  r11 = REX_LOAD_U8(uint32_t(r5) + 0);
  // lis r8,26624
  r8 = uint64_t(int64_t(1744830464));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // ori r8,r8,1
  r8 = r8 | 1;
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r7,r11,3
  r7 = uint64_t(int64_t(int64_t(r11) + 3));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lis r4,-32768
  r4 = uint64_t(int64_t(-2147483648));
  // bl 0x82295508
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82295508, LIFTED_ARGS(0x82593B60)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // stw r3,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r3));
  // beq cr6,0x82593b94
  if (cr6.eq) goto loc_82593B94;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x822965e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822965E0, LIFTED_ARGS(0x82593B74)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
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
loc_82593B94:
  // li r3,0
  r3 = uint64_t(int64_t(0));
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

// sub_82593BB0: lifted from generated/ by lift.py.
uint64_t f82593BB0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82593BB0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // mr r11,r3
  r11 = r3;
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // lwz r3,4(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // blr
  return r3;
}

// sub_82593BC8: lifted from generated/ by lift.py.
lifted::Result f82593BC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82593BC8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // addi r11,r11,-13300
  r11 = uint64_t(int64_t(int64_t(r11) + -13300));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x8258c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258C5F8, LIFTED_ARGS(0x82593BF4)); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82593c0c
  if (cr6.eq) goto loc_82593C0C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82593C08)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_82593C0C:
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

// sub_82593C40: lifted from generated/ by lift.py.
uint64_t f82593C40(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82593C40 (lifted)");
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
  // blr
  return r3;
}

// sub_82593C58: lifted from generated/ by lift.py.
lifted::Result f82593C58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82593C58 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-13252
  r11 = uint64_t(int64_t(int64_t(r11) + -13252));
  // clrlwi r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq cr6,0x82593c8c
  if (cr6.eq) goto loc_82593C8C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82593C88)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_82593C8C:
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

// sub_82593D78: lifted from generated/ by lift.py.
uint64_t f82593D78(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82593D78 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r3,28(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 28);
  // blr
  return r3;
}

// sub_82593D88: lifted from generated/ by lift.py.
lifted::Result f82593D88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82593D88 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r3
  r11 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r4,4(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 4);
  // bl 0x8258e580
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E580, LIFTED_ARGS(0x82593DA4)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258e5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E5B8, LIFTED_ARGS(0x82593DAC)); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_82593E30: lifted from generated/ by lift.py.
lifted::Result f82593E30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82593E30 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r3,28(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 28);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82593e64
  if (cr6.eq) goto loc_82593E64;
  // bl 0x820a7bd0
  // (left out: dead in C++)
  { const lifted::Result o = f820A7BD0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82593E58); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
loc_82593E64:
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

// sub_82593E78: lifted from generated/ by lift.py.
lifted::Result f82593E78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11) {
  ITC_NATIVE_RUNNING("sub_82593E78 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r4
  r30 = r4;
  // mr r29,r5
  r29 = r5;
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8258e580
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E580, LIFTED_ARGS(0x82593E9C)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // addi r5,r11,28
  r5 = uint64_t(int64_t(int64_t(r11) + 28));
  // bl 0x82482730
  // (left out: dead in C++)
  { const lifted::Result o = f82482730(base, r1, r13, r3, r4, r5, r6, r7, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x82593edc
  if (!cr6.eq) goto loc_82593EDC;
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lis r11,-32240
  r11 = uint64_t(int64_t(-2112880640));
  // mr r6,r31
  r6 = r31;
  // addi r5,r11,25920
  r5 = uint64_t(int64_t(int64_t(r11) + 25920));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r3,28(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + 28);
  // bl 0x82481b88
  // (left out: dead in C++)
  { const lifted::Result o = f82481B88(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82593ED8); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82593EDC:
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258e5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8258E5B8, LIFTED_ARGS(0x82593EE4)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82593EF0: lifted from generated/ by lift.py.
lifted::Result f82593EF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82593EF0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32249
  r11 = uint64_t(int64_t(-2113470464));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-13168
  r11 = uint64_t(int64_t(int64_t(r11) + -13168));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r3,28(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 28);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82593f30
  if (cr6.eq) goto loc_82593F30;
  // bl 0x820a7bd0
  // (left out: dead in C++)
  { const lifted::Result o = f820A7BD0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82593F24); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
loc_82593F30:
  // lwz r31,4(r31)
  r31 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82593f4c
  if (cr6.eq) goto loc_82593F4C;
  // mr r3,r31
  r3 = r31;
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS(0x82593F44)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82593F4C)); r3 = o.r3; f1 = o.f1; }
loc_82593F4C:
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

// sub_82593F60: lifted from generated/ by lift.py.
lifted::Result f82593F60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82593F60 (lifted)");
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
  // bl 0x82593ef0
  // (left out: dead in C++)
  { const lifted::Result o = f82593EF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  // mr r3,r31
  r3 = r31;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82593f98
  if (cr6.eq) goto loc_82593F98;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS(0x82593F94)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_82593F98:
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

// sub_82594290: lifted from generated/ by lift.py.
uint64_t f82594290(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82594290 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x825942a4
  if (!cr6.eq) goto loc_825942A4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_825942A4:
  // lwz r10,16(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 16);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // srawi r3,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r3 = uint64_t(int64_t(int32_t(r11) >> 2));
  // blr
  return r3;
}

namespace {

lifted::Result e82588DF0(uint8_t* base, const lifted::Args& a) {
  return {f82588DF0(base, a.r[0], a.r[1], a.f[0]), a.f[0]};
}

lifted::Result e82588E30(uint8_t* base, const lifted::Args& a) {
  return f82588E30(base, a.r1, a.r13, a.r[0], a.r[2], a.f[0]);
}

lifted::Result e82589480(uint8_t* base, const lifted::Args& a) {
  return f82589480(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82589650(uint8_t* base, const lifted::Args& a) {
  return f82589650(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82589AB0(uint8_t* base, const lifted::Args& a) {
  return f82589AB0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8258BCD8(uint8_t* base, const lifted::Args& a) {
  return {f8258BCD8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8258BE58(uint8_t* base, const lifted::Args& a) {
  f8258BE58(base, a.r1, a.r[4], a.f[0], a.f[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8258BFA0(uint8_t* base, const lifted::Args& a) {
  return f8258BFA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8258C060(uint8_t* base, const lifted::Args& a) {
  return f8258C060(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258C0B8(uint8_t* base, const lifted::Args& a) {
  return f8258C0B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e8258C3A8(uint8_t* base, const lifted::Args& a) {
  return f8258C3A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258C5B0(uint8_t* base, const lifted::Args& a) {
  return f8258C5B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8258C608(uint8_t* base, const lifted::Args& a) {
  return f8258C608(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8258C898(uint8_t* base, const lifted::Args& a) {
  return f8258C898(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8258CCC8(uint8_t* base, const lifted::Args& a) {
  return f8258CCC8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258CE00(uint8_t* base, const lifted::Args& a) {
  return f8258CE00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258CEA0(uint8_t* base, const lifted::Args& a) {
  f8258CEA0(base, a.r1, a.r13, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8258CF40(uint8_t* base, const lifted::Args& a) {
  return f8258CF40(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258CFA0(uint8_t* base, const lifted::Args& a) {
  return f8258CFA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e8258D020(uint8_t* base, const lifted::Args& a) {
  f8258D020(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8258D030(uint8_t* base, const lifted::Args& a) {
  return {f8258D030(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8258D090(uint8_t* base, const lifted::Args& a) {
  return {f8258D090(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8258D0D8(uint8_t* base, const lifted::Args& a) {
  return f8258D0D8(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e8258D170(uint8_t* base, const lifted::Args& a) {
  return {f8258D170(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8258D1E0(uint8_t* base, const lifted::Args& a) {
  return {f8258D1E0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8258D248(uint8_t* base, const lifted::Args& a) {
  f8258D248(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8258D290(uint8_t* base, const lifted::Args& a) {
  return f8258D290(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e8258D318(uint8_t* base, const lifted::Args& a) {
  return f8258D318(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e8258D3A8(uint8_t* base, const lifted::Args& a) {
  return f8258D3A8(base, a.r1, a.r13, a.r[0]);
}

lifted::Result e8258D440(uint8_t* base, const lifted::Args& a) {
  return {f8258D440(base, a.r[0]), a.f[0]};
}

lifted::Result e8258D448(uint8_t* base, const lifted::Args& a) {
  return f8258D448(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8258D490(uint8_t* base, const lifted::Args& a) {
  return f8258D490(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8258D4A8(uint8_t* base, const lifted::Args& a) {
  return {f8258D4A8(base, a.r[0]), a.f[0]};
}

lifted::Result e8258D4C0(uint8_t* base, const lifted::Args& a) {
  f8258D4C0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8258D4D8(uint8_t* base, const lifted::Args& a) {
  return f8258D4D8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8258D508(uint8_t* base, const lifted::Args& a) {
  return f8258D508(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258DBA8(uint8_t* base, const lifted::Args& a) {
  return f8258DBA8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8258DC58(uint8_t* base, const lifted::Args& a) {
  return f8258DC58(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8258DCC8(uint8_t* base, const lifted::Args& a) {
  return f8258DCC8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8258DD60(uint8_t* base, const lifted::Args& a) {
  return f8258DD60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8258DD78(uint8_t* base, const lifted::Args& a) {
  return f8258DD78(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258DD90(uint8_t* base, const lifted::Args& a) {
  return f8258DD90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258DDD0(uint8_t* base, const lifted::Args& a) {
  f8258DDD0(base, a.r1, a.r13, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8258DDE0(uint8_t* base, const lifted::Args& a) {
  return f8258DDE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258DE50(uint8_t* base, const lifted::Args& a) {
  return f8258DE50(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8258DEB8(uint8_t* base, const lifted::Args& a) {
  return f8258DEB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258E530(uint8_t* base, const lifted::Args& a) {
  f8258E530(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8258E540(uint8_t* base, const lifted::Args& a) {
  return f8258E540(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8258E548(uint8_t* base, const lifted::Args& a) {
  return f8258E548(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8258E578(uint8_t* base, const lifted::Args& a) {
  return f8258E578(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8258E5C8(uint8_t* base, const lifted::Args& a) {
  return f8258E5C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.lr);
}

lifted::Result e8258E698(uint8_t* base, const lifted::Args& a) {
  return f8258E698(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8258E718(uint8_t* base, const lifted::Args& a) {
  return f8258E718(base, a.r1, a.r13, a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e8258E918(uint8_t* base, const lifted::Args& a) {
  return {f8258E918(base, a.r[0]), a.f[0]};
}

lifted::Result e8258EF60(uint8_t* base, const lifted::Args& a) {
  return f8258EF60(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258EFC0(uint8_t* base, const lifted::Args& a) {
  return f8258EFC0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8258F028(uint8_t* base, const lifted::Args& a) {
  return {f8258F028(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8258F080(uint8_t* base, const lifted::Args& a) {
  return f8258F080(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8258F3E0(uint8_t* base, const lifted::Args& a) {
  return f8258F3E0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8258F438(uint8_t* base, const lifted::Args& a) {
  return {f8258F438(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8258F490(uint8_t* base, const lifted::Args& a) {
  return {f8258F490(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8258F4E8(uint8_t* base, const lifted::Args& a) {
  return f8258F4E8(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e8258F568(uint8_t* base, const lifted::Args& a) {
  return f8258F568(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8258F6A0(uint8_t* base, const lifted::Args& a) {
  return f8258F6A0(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e8258F770(uint8_t* base, const lifted::Args& a) {
  return f8258F770(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8258F7C8(uint8_t* base, const lifted::Args& a) {
  return f8258F7C8(base, a.r1, a.r13, a.r[0], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8258F828(uint8_t* base, const lifted::Args& a) {
  return f8258F828(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8258F898(uint8_t* base, const lifted::Args& a) {
  return f8258F898(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8258F8F0(uint8_t* base, const lifted::Args& a) {
  return f8258F8F0(base, a.r1, a.r13, a.r[0], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8258F948(uint8_t* base, const lifted::Args& a) {
  return f8258F948(base, a.r1, a.r13, a.r[0], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8258F9A0(uint8_t* base, const lifted::Args& a) {
  return f8258F9A0(base, a.r1, a.r13, a.r[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8]);
}

lifted::Result e8258FAC8(uint8_t* base, const lifted::Args& a) {
  return f8258FAC8(base, a.r1, a.r13, a.r[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e8258FB20(uint8_t* base, const lifted::Args& a) {
  return f8258FB20(base, a.r1, a.r13, a.r[0], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e82590C48(uint8_t* base, const lifted::Args& a) {
  return f82590C48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82590DE0(uint8_t* base, const lifted::Args& a) {
  return f82590DE0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82590EA0(uint8_t* base, const lifted::Args& a) {
  return f82590EA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825916F0(uint8_t* base, const lifted::Args& a) {
  return f825916F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82591898(uint8_t* base, const lifted::Args& a) {
  return f82591898(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825918E0(uint8_t* base, const lifted::Args& a) {
  f825918E0(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82591BB8(uint8_t* base, const lifted::Args& a) {
  return {f82591BB8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82592058(uint8_t* base, const lifted::Args& a) {
  return f82592058(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825920A8(uint8_t* base, const lifted::Args& a) {
  f825920A8(base, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82592178(uint8_t* base, const lifted::Args& a) {
  f82592178(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82592400(uint8_t* base, const lifted::Args& a) {
  return {f82592400(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e825925A8(uint8_t* base, const lifted::Args& a) {
  return f825925A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825925F0(uint8_t* base, const lifted::Args& a) {
  return f825925F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82592638(uint8_t* base, const lifted::Args& a) {
  return f82592638(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82592900(uint8_t* base, const lifted::Args& a) {
  return f82592900(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e82593B10(uint8_t* base, const lifted::Args& a) {
  return f82593B10(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[7], a.f[0]);
}

lifted::Result e82593BB0(uint8_t* base, const lifted::Args& a) {
  return {f82593BB0(base, a.r[0]), a.f[0]};
}

lifted::Result e82593BC8(uint8_t* base, const lifted::Args& a) {
  return f82593BC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82593C40(uint8_t* base, const lifted::Args& a) {
  return {f82593C40(base, a.r[0]), a.f[0]};
}

lifted::Result e82593C58(uint8_t* base, const lifted::Args& a) {
  return f82593C58(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82593D78(uint8_t* base, const lifted::Args& a) {
  return {f82593D78(base, a.r[0]), a.f[0]};
}

lifted::Result e82593D88(uint8_t* base, const lifted::Args& a) {
  return f82593D88(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82593E30(uint8_t* base, const lifted::Args& a) {
  return f82593E30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82593E78(uint8_t* base, const lifted::Args& a) {
  return f82593E78(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10]);
}

lifted::Result e82593EF0(uint8_t* base, const lifted::Args& a) {
  return f82593EF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82593F60(uint8_t* base, const lifted::Args& a) {
  return f82593F60(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82594290(uint8_t* base, const lifted::Args& a) {
  return {f82594290(base, a.r[0]), a.f[0]};
}

const Registration kEntries[] = {
    {0x82588DF0, e82588DF0},
    {0x82588E30, e82588E30},
    {0x82589480, e82589480},
    {0x82589650, e82589650},
    {0x82589AB0, e82589AB0},
    {0x8258BCD8, e8258BCD8},
    {0x8258BE58, e8258BE58},
    {0x8258BFA0, e8258BFA0},
    {0x8258C060, e8258C060},
    {0x8258C0B8, e8258C0B8},
    {0x8258C3A8, e8258C3A8},
    {0x8258C5B0, e8258C5B0},
    {0x8258C608, e8258C608},
    {0x8258C898, e8258C898},
    {0x8258CCC8, e8258CCC8},
    {0x8258CE00, e8258CE00},
    {0x8258CEA0, e8258CEA0},
    {0x8258CF40, e8258CF40},
    {0x8258CFA0, e8258CFA0},
    {0x8258D020, e8258D020},
    {0x8258D030, e8258D030},
    {0x8258D090, e8258D090},
    {0x8258D0D8, e8258D0D8},
    {0x8258D170, e8258D170},
    {0x8258D1E0, e8258D1E0},
    {0x8258D248, e8258D248},
    {0x8258D290, e8258D290},
    {0x8258D318, e8258D318},
    {0x8258D3A8, e8258D3A8},
    {0x8258D440, e8258D440},
    {0x8258D448, e8258D448},
    {0x8258D490, e8258D490},
    {0x8258D4A8, e8258D4A8},
    {0x8258D4C0, e8258D4C0},
    {0x8258D4D8, e8258D4D8},
    {0x8258D508, e8258D508},
    {0x8258DBA8, e8258DBA8},
    {0x8258DC58, e8258DC58},
    {0x8258DCC8, e8258DCC8},
    {0x8258DD60, e8258DD60},
    {0x8258DD78, e8258DD78},
    {0x8258DD90, e8258DD90},
    {0x8258DDD0, e8258DDD0},
    {0x8258DDE0, e8258DDE0},
    {0x8258DE50, e8258DE50},
    {0x8258DEB8, e8258DEB8},
    {0x8258E530, e8258E530},
    {0x8258E540, e8258E540},
    {0x8258E548, e8258E548},
    {0x8258E578, e8258E578},
    {0x8258E5C8, e8258E5C8},
    {0x8258E698, e8258E698},
    {0x8258E718, e8258E718},
    {0x8258E918, e8258E918},
    {0x8258EF60, e8258EF60},
    {0x8258EFC0, e8258EFC0},
    {0x8258F028, e8258F028},
    {0x8258F080, e8258F080},
    {0x8258F3E0, e8258F3E0},
    {0x8258F438, e8258F438},
    {0x8258F490, e8258F490},
    {0x8258F4E8, e8258F4E8},
    {0x8258F568, e8258F568},
    {0x8258F6A0, e8258F6A0},
    {0x8258F770, e8258F770},
    {0x8258F7C8, e8258F7C8},
    {0x8258F828, e8258F828},
    {0x8258F898, e8258F898},
    {0x8258F8F0, e8258F8F0},
    {0x8258F948, e8258F948},
    {0x8258F9A0, e8258F9A0},
    {0x8258FAC8, e8258FAC8},
    {0x8258FB20, e8258FB20},
    {0x82590C48, e82590C48},
    {0x82590DE0, e82590DE0},
    {0x82590EA0, e82590EA0},
    {0x825916F0, e825916F0},
    {0x82591898, e82591898},
    {0x825918E0, e825918E0},
    {0x82591BB8, e82591BB8},
    {0x82592058, e82592058},
    {0x825920A8, e825920A8},
    {0x82592178, e82592178},
    {0x82592400, e82592400},
    {0x825925A8, e825925A8},
    {0x825925F0, e825925F0},
    {0x82592638, e82592638},
    {0x82592900, e82592900},
    {0x82593B10, e82593B10},
    {0x82593BB0, e82593BB0},
    {0x82593BC8, e82593BC8},
    {0x82593C40, e82593C40},
    {0x82593C58, e82593C58},
    {0x82593D78, e82593D78},
    {0x82593D88, e82593D88},
    {0x82593E30, e82593E30},
    {0x82593E78, e82593E78},
    {0x82593EF0, e82593EF0},
    {0x82593F60, e82593F60},
    {0x82594290, e82594290},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_020)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_82588DF0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82588DF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82588DF0);
}

extern "C" REX_FUNC(sub_82588E30) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82588E30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82588E30);
}

extern "C" REX_FUNC(sub_82589480) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82589480(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82589480);
}

extern "C" REX_FUNC(sub_82589650) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82589650(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82589650);
}

extern "C" REX_FUNC(sub_82589AB0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82589AB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82589AB0);
}

extern "C" REX_FUNC(sub_8258BCD8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258BCD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258BCD8);
}

extern "C" REX_FUNC(sub_8258BE58) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258BE58(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258BE58);
}

extern "C" REX_FUNC(sub_8258BFA0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258BFA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258BFA0);
}

extern "C" REX_FUNC(sub_8258C060) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258C060(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258C060);
}

extern "C" REX_FUNC(sub_8258C0B8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258C0B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258C0B8);
}

extern "C" REX_FUNC(sub_8258C3A8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258C3A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258C3A8);
}

extern "C" REX_FUNC(sub_8258C5B0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258C5B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258C5B0);
}

extern "C" REX_FUNC(sub_8258C608) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258C608(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258C608);
}

extern "C" REX_FUNC(sub_8258C898) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258C898(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258C898);
}

extern "C" REX_FUNC(sub_8258CCC8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258CCC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258CCC8);
}

extern "C" REX_FUNC(sub_8258CE00) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258CE00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258CE00);
}

extern "C" REX_FUNC(sub_8258CEA0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258CEA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258CEA0);
}

extern "C" REX_FUNC(sub_8258CF40) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258CF40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258CF40);
}

extern "C" REX_FUNC(sub_8258CFA0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258CFA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258CFA0);
}

extern "C" REX_FUNC(sub_8258D020) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D020(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D020);
}

extern "C" REX_FUNC(sub_8258D030) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D030(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D030);
}

extern "C" REX_FUNC(sub_8258D090) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D090(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D090);
}

extern "C" REX_FUNC(sub_8258D0D8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D0D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D0D8);
}

extern "C" REX_FUNC(sub_8258D170) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D170(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D170);
}

extern "C" REX_FUNC(sub_8258D1E0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D1E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D1E0);
}

extern "C" REX_FUNC(sub_8258D248) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D248(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D248);
}

extern "C" REX_FUNC(sub_8258D290) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D290(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D290);
}

extern "C" REX_FUNC(sub_8258D318) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D318(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D318);
}

extern "C" REX_FUNC(sub_8258D3A8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D3A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D3A8);
}

extern "C" REX_FUNC(sub_8258D440) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D440(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D440);
}

extern "C" REX_FUNC(sub_8258D448) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D448(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D448);
}

extern "C" REX_FUNC(sub_8258D490) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D490(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D490);
}

extern "C" REX_FUNC(sub_8258D4A8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D4A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D4A8);
}

extern "C" REX_FUNC(sub_8258D4C0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D4C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D4C0);
}

extern "C" REX_FUNC(sub_8258D4D8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D4D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D4D8);
}

extern "C" REX_FUNC(sub_8258D508) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258D508(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258D508);
}

extern "C" REX_FUNC(sub_8258DBA8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258DBA8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258DBA8);
}

extern "C" REX_FUNC(sub_8258DC58) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258DC58(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258DC58);
}

extern "C" REX_FUNC(sub_8258DCC8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258DCC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258DCC8);
}

extern "C" REX_FUNC(sub_8258DD60) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258DD60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258DD60);
}

extern "C" REX_FUNC(sub_8258DD78) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258DD78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258DD78);
}

extern "C" REX_FUNC(sub_8258DD90) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258DD90(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258DD90);
}

extern "C" REX_FUNC(sub_8258DDD0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258DDD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258DDD0);
}

extern "C" REX_FUNC(sub_8258DDE0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258DDE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258DDE0);
}

extern "C" REX_FUNC(sub_8258DE50) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258DE50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258DE50);
}

extern "C" REX_FUNC(sub_8258DEB8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258DEB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258DEB8);
}

extern "C" REX_FUNC(sub_8258E530) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258E530(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258E530);
}

extern "C" REX_FUNC(sub_8258E540) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258E540(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258E540);
}

extern "C" REX_FUNC(sub_8258E548) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258E548(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258E548);
}

extern "C" REX_FUNC(sub_8258E578) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258E578(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258E578);
}

extern "C" REX_FUNC(sub_8258E5C8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258E5C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258E5C8);
}

extern "C" REX_FUNC(sub_8258E698) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258E698(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258E698);
}

extern "C" REX_FUNC(sub_8258E718) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258E718(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258E718);
}

extern "C" REX_FUNC(sub_8258E918) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258E918(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258E918);
}

extern "C" REX_FUNC(sub_8258EF60) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258EF60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258EF60);
}

extern "C" REX_FUNC(sub_8258EFC0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258EFC0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258EFC0);
}

extern "C" REX_FUNC(sub_8258F028) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F028(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F028);
}

extern "C" REX_FUNC(sub_8258F080) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F080(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F080);
}

extern "C" REX_FUNC(sub_8258F3E0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F3E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F3E0);
}

extern "C" REX_FUNC(sub_8258F438) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F438(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F438);
}

extern "C" REX_FUNC(sub_8258F490) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F490(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F490);
}

extern "C" REX_FUNC(sub_8258F4E8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F4E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F4E8);
}

extern "C" REX_FUNC(sub_8258F568) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F568(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F568);
}

extern "C" REX_FUNC(sub_8258F6A0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F6A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F6A0);
}

extern "C" REX_FUNC(sub_8258F770) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F770(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F770);
}

extern "C" REX_FUNC(sub_8258F7C8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F7C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F7C8);
}

extern "C" REX_FUNC(sub_8258F828) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F828(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F828);
}

extern "C" REX_FUNC(sub_8258F898) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F898(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F898);
}

extern "C" REX_FUNC(sub_8258F8F0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F8F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F8F0);
}

extern "C" REX_FUNC(sub_8258F948) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F948(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F948);
}

extern "C" REX_FUNC(sub_8258F9A0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258F9A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258F9A0);
}

extern "C" REX_FUNC(sub_8258FAC8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258FAC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258FAC8);
}

extern "C" REX_FUNC(sub_8258FB20) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_8258FB20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8258FB20);
}

extern "C" REX_FUNC(sub_82590C48) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82590C48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82590C48);
}

extern "C" REX_FUNC(sub_82590DE0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82590DE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82590DE0);
}

extern "C" REX_FUNC(sub_82590EA0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82590EA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82590EA0);
}

extern "C" REX_FUNC(sub_825916F0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_825916F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825916F0);
}

extern "C" REX_FUNC(sub_82591898) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82591898(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82591898);
}

extern "C" REX_FUNC(sub_825918E0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_825918E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825918E0);
}

extern "C" REX_FUNC(sub_82591BB8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82591BB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82591BB8);
}

extern "C" REX_FUNC(sub_82592058) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82592058(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82592058);
}

extern "C" REX_FUNC(sub_825920A8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_825920A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825920A8);
}

extern "C" REX_FUNC(sub_82592178) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82592178(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82592178);
}

extern "C" REX_FUNC(sub_82592400) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82592400(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82592400);
}

extern "C" REX_FUNC(sub_825925A8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_825925A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825925A8);
}

extern "C" REX_FUNC(sub_825925F0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_825925F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825925F0);
}

extern "C" REX_FUNC(sub_82592638) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82592638(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82592638);
}

extern "C" REX_FUNC(sub_82592900) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82592900(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82592900);
}

extern "C" REX_FUNC(sub_82593B10) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593B10(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593B10);
}

extern "C" REX_FUNC(sub_82593BB0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593BB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593BB0);
}

extern "C" REX_FUNC(sub_82593BC8) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593BC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593BC8);
}

extern "C" REX_FUNC(sub_82593C40) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593C40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593C40);
}

extern "C" REX_FUNC(sub_82593C58) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593C58(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593C58);
}

extern "C" REX_FUNC(sub_82593D78) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593D78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593D78);
}

extern "C" REX_FUNC(sub_82593D88) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593D88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593D88);
}

extern "C" REX_FUNC(sub_82593E30) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593E30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593E30);
}

extern "C" REX_FUNC(sub_82593E78) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593E78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593E78);
}

extern "C" REX_FUNC(sub_82593EF0) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593EF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593EF0);
}

extern "C" REX_FUNC(sub_82593F60) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82593F60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82593F60);
}

extern "C" REX_FUNC(sub_82594290) {
  if (!ITC_FLAG(native_lifted_020)) {
    __imp__sub_82594290(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82594290);
}
