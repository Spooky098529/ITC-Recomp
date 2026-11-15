// Lift sweep batch 032 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_032, true, "ITC",
              "Run lift sweep batch 032's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register.
#define LIFTED_ARGS \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }

namespace lifted {

// sub_82114128: lifted from generated/ by lift.py.
uint64_t f82114128(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82114128 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e70
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r12,-2
  r12 = uint64_t(int64_t(-131072));
  // ori r12,r12,32160
  r12 = r12 | 32160;
  // bl 0x825a9934
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwux r1,r1,r12
  ea = uint32_t(r1) + uint32_t(r12);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lis r5,1
  r5 = uint64_t(int64_t(65536));
  // addi r29,r31,6120
  r29 = uint64_t(int64_t(int64_t(r31) + 6120));
  // ori r5,r5,33076
  r5 = r5 | 33076;
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // mr r4,r29
  r4 = r29;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r28,r31,2
  r28 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r28,r28,-26320
  r28 = uint64_t(int64_t(int64_t(r28) + -26320));
  // li r5,60
  r5 = uint64_t(int64_t(60));
  // mr r4,r28
  r4 = r28;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r27,r31,2
  r27 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r27,r27,-26200
  r27 = uint64_t(int64_t(int64_t(r27) + -26200));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // mr r4,r27
  r4 = r27;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r26,r31,2
  r26 = uint64_t(int64_t(int64_t(r31) + 131072));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r26,r26,-26172
  r26 = uint64_t(int64_t(int64_t(r26) + -26172));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // mr r4,r26
  r4 = r26;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r31,2
  r30 = uint64_t(int64_t(int64_t(r31) + 131072));
  // mr r3,r31
  r3 = r31;
  // lwz r25,6116(r31)
  r25 = REX_LOAD_U32(uint32_t(r31) + 6116);
  // addi r30,r30,-26332
  r30 = uint64_t(int64_t(int64_t(r30) + -26332));
  // lwz r24,0(r30)
  r24 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r23,4(r30)
  r23 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r22,8(r30)
  r22 = REX_LOAD_U32(uint32_t(r30) + 8);
  // bl 0x82113bd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82113BD8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
  // bl 0x82113e60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82113E60, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lis r5,1
  r5 = uint64_t(int64_t(65536));
  // addi r4,r1,208
  r4 = uint64_t(int64_t(int64_t(r1) + 208));
  // ori r5,r5,33076
  r5 = r5 | 33076;
  // mr r3,r29
  r3 = r29;
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,144
  r4 = uint64_t(int64_t(int64_t(r1) + 144));
  // li r5,60
  r5 = uint64_t(int64_t(60));
  // mr r3,r28
  r3 = r28;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // mr r3,r27
  r3 = r27;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // mr r3,r26
  r3 = r26;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r11,r31,2
  r11 = uint64_t(int64_t(int64_t(r31) + 131072));
  // stw r25,6116(r31)
  REX_STORE_U32(uint32_t(r31) + 6116, uint32_t(r25));
  // addi r11,r11,-21224
  r11 = uint64_t(int64_t(int64_t(r11) + -21224));
  // stw r24,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r24));
  // stw r23,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r23));
  // stw r22,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r22));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ori r10,r10,1
  r10 = r10 | 1;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // ori r10,r10,2
  r10 = r10 | 2;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // ori r10,r10,4
  r10 = r10 | 4;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // rotlwi r10,r10,0
  r10 = __builtin_rotateleft32(uint32_t(r10), 0);
  // ori r10,r10,8
  r10 = r10 | 8;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r1,0(r1)
  r1 = REX_LOAD_U32(uint32_t(r1) + 0);
  // b 0x825a6ec0
  // (left out: dead in C++)
  return r3;
}

// sub_82129A08: lifted from generated/ by lift.py.
lifted::Result f82129A08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82129A08 (lifted)");
  uint64_t r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Vec v0{}, v13{};
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-168(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -168, std::bit_cast<uint64_t>(f30));
  // stfd f31,-160(r1)
  REX_STORE_U64(uint32_t(r1) + -160, std::bit_cast<uint64_t>(f31));
  // lis r12,-3
  r12 = uint64_t(int64_t(-196608));
  // ori r12,r12,59952
  r12 = r12 | 59952;
  // bl 0x825a9934
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwux r1,r1,r12
  ea = uint32_t(r1) + uint32_t(r12);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // mr r19,r4
  r19 = r4;
  // addis r17,r27,1
  r17 = uint64_t(int64_t(int64_t(r27) + 65536));
  // addi r17,r17,-1400
  r17 = uint64_t(int64_t(int64_t(r17) + -1400));
  // lwz r11,0(r17)
  r11 = REX_LOAD_U32(uint32_t(r17) + 0);
  // stw r17,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r17));
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // blt cr6,0x8212a12c
  if (cr6.lt) goto loc_8212A12C;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // mulli r9,r19,592
  r9 = uint64_t(int64_t(static_cast<int64_t>(r19 * static_cast<uint64_t>(592))));
  // ori r10,r11,64128
  r10 = r11 | 64128;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r1,288
  r3 = uint64_t(int64_t(int64_t(r1) + 288));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // lwzx r10,r27,r10
  r10 = REX_LOAD_U32(uint32_t(r27) + uint32_t(r10));
  // add r22,r9,r11
  r22 = r9 + r11;
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,7,0,24
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // add r31,r10,r27
  r31 = r10 + r27;
  // bl 0x821298c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821298C0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2848
  r12 = r12 | 2848;
  // add r3,r1,r12
  r3 = r1 + r12;
  // bl 0x821298c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821298C0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lwz r11,108(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 108);
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // lwz r8,0(r17)
  r8 = REX_LOAD_U32(uint32_t(r17) + 0);
  // mr r9,r25
  r9 = r25;
  // mr r23,r25
  r23 = r25;
  // lfs f30,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f30 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stfs f30,128(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 128, temp.u32);
  // mr r24,r25
  r24 = r25;
  // stfs f30,132(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 132, temp.u32);
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // stfs f30,136(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 136, temp.u32);
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // stfs f30,96(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // lfs f31,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f31 = double(temp.f32);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r26,356(r1)
  REX_STORE_U32(uint32_t(r1) + 356, uint32_t(r26));
  // stfs f31,140(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 140, temp.u32);
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2832
  r12 = r12 | 2832;
  // stwx r25,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r25));
  // stfs f30,100(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // stfs f30,104(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2836
  r12 = r12 | 2836;
  // stwx r10,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r10));
  // stfs f30,112(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 112, temp.u32);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stfs f30,116(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // stfs f30,120(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 120, temp.u32);
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2916
  r12 = r12 | 2916;
  // stwx r26,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r26));
  // stfs f31,124(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // lis r12,2
  r12 = uint64_t(int64_t(131072));
  // ori r12,r12,5392
  r12 = r12 | 5392;
  // stwx r25,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r25));
  // stfs f30,144(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 144, temp.u32);
  // stfs f30,148(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 148, temp.u32);
  // stfs f30,152(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 152, temp.u32);
  // lis r12,2
  r12 = uint64_t(int64_t(131072));
  // ori r12,r12,5396
  r12 = r12 | 5396;
  // stwx r11,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r11));
  // stfs f31,156(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 156, temp.u32);
  // ble cr6,0x8212a0d4
  if (!cr6.gt) goto loc_8212A0D4;
  // lis r12,2
  r12 = uint64_t(int64_t(131072));
  // ori r12,r12,3392
  r12 = r12 | 3392;
  // add r21,r1,r12
  r21 = r1 + r12;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,832
  r12 = r12 | 832;
  // add r20,r1,r12
  r20 = r1 + r12;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2936
  r12 = r12 | 2936;
  // add r30,r1,r12
  r30 = r1 + r12;
  // addi r29,r1,376
  r29 = uint64_t(int64_t(int64_t(r1) + 376));
  // li r18,4
  r18 = uint64_t(int64_t(4));
loc_82129B74:
  // lwz r11,104(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 104);
  // mr r28,r31
  r28 = r31;
  // rlwinm r11,r11,7,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 7) & 0xFFFFFF80;
  // add r11,r11,r27
  r11 = r11 + r27;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x82129b94
  if (!cr6.eq) goto loc_82129B94;
  // addi r31,r27,128
  r31 = uint64_t(int64_t(int64_t(r27) + 128));
  // b 0x82129b98
  goto loc_82129B98;
loc_82129B94:
  // addi r31,r31,128
  r31 = uint64_t(int64_t(int64_t(r31) + 128));
loc_82129B98:
  // lwz r11,60(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 60);
  // lwz r10,60(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 60);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8212a0bc
  if (!cr6.eq) goto loc_8212A0BC;
  // lwz r11,112(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 112);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8212a0bc
  if (cr6.eq) goto loc_8212A0BC;
  // lfs f0,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f0 = double(temp.f32);
  // addi r11,r22,464
  r11 = uint64_t(int64_t(int64_t(r22) + 464));
  // lfs f11,400(r22)
  temp.u32 = REX_LOAD_U32(uint32_t(r22) + 400);
  f11 = double(temp.f32);
  // fsubs f0,f0,f11
  f0 = double(float(f0 - f11));
  // stfs f0,176(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 176, temp.u32);
  // lfs f13,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f13 = double(temp.f32);
  // lfs f12,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f12 = double(temp.f32);
  // lfs f10,404(r22)
  temp.u32 = REX_LOAD_U32(uint32_t(r22) + 404);
  f10 = double(temp.f32);
  // lfs f0,408(r22)
  temp.u32 = REX_LOAD_U32(uint32_t(r22) + 408);
  f0 = double(temp.f32);
  // fsubs f13,f13,f10
  f13 = double(float(f13 - f10));
  // fsubs f0,f12,f0
  f0 = double(float(f12 - f0));
  // lvx128 v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // stfs f0,184(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 184, temp.u32);
  // stfs f31,188(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 188, temp.u32);
  // stfs f13,180(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 180, temp.u32);
  // addi r11,r1,176
  r11 = uint64_t(int64_t(int64_t(r1) + 176));
  // lvx128 v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,256
  r11 = uint64_t(int64_t(int64_t(r1) + 256));
  // vmsum3fp128 v0,v13,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v0.f32), 0xEF));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // mr r11,r26
  r11 = r26;
  // lfs f0,256(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 256);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // blt cr6,0x82129c1c
  if (cr6.lt) goto loc_82129C1C;
  // mr r11,r25
  r11 = r25;
loc_82129C1C:
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8212a0c0
  if (!cr0.eq) goto loc_8212A0C0;
  // lwz r11,48(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 48);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8212a0c0
  if (cr6.eq) goto loc_8212A0C0;
  // addi r11,r28,64
  r11 = uint64_t(int64_t(int64_t(r28) + 64));
  // addi r10,r31,64
  r10 = uint64_t(int64_t(int64_t(r31) + 64));
  // addi r8,r1,272
  r8 = uint64_t(int64_t(int64_t(r1) + 272));
  // lvx128 v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvx128 v13,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmsum3fp128 v0,v13,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v0.f32), 0xEF));
  // stvx v0,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f0,272(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 272);
  f0 = double(temp.f32);
  // fcmpu cr6,f0,f30
  cr6 = lifted::CompareFloat(f0, f30);
  // blt cr6,0x8212a0c0
  if (cr6.lt) goto loc_8212A0C0;
  // ld r5,0(r11)
  r5 = REX_LOAD_U64(uint32_t(r11) + 0);
  // ld r6,8(r11)
  r6 = REX_LOAD_U64(uint32_t(r11) + 8);
  // ld r3,0(r10)
  r3 = REX_LOAD_U64(uint32_t(r10) + 0);
  // ld r4,8(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 8);
  // bl 0x82129940
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82129940, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), -1, so);
  // beq cr6,0x82129db8
  if (cr6.eq) goto loc_82129DB8;
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // beq cr6,0x82129ce4
  if (cr6.eq) goto loc_82129CE4;
  // addi r10,r31,80
  r10 = uint64_t(int64_t(int64_t(r31) + 80));
  // addi r9,r28,80
  r9 = uint64_t(int64_t(int64_t(r28) + 80));
  // addi r8,r31,96
  r8 = uint64_t(int64_t(int64_t(r31) + 96));
  // addi r7,r28,96
  r7 = uint64_t(int64_t(int64_t(r28) + 96));
  // addi r6,r1,128
  r6 = uint64_t(int64_t(int64_t(r1) + 128));
  // ld r17,0(r10)
  r17 = REX_LOAD_U64(uint32_t(r10) + 0);
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // ld r16,0(r9)
  r16 = REX_LOAD_U64(uint32_t(r9) + 0);
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // ld r15,0(r8)
  r15 = REX_LOAD_U64(uint32_t(r8) + 0);
  // addi r3,r1,144
  r3 = uint64_t(int64_t(int64_t(r1) + 144));
  // ld r14,0(r7)
  r14 = REX_LOAD_U64(uint32_t(r7) + 0);
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // ld r10,8(r10)
  r10 = REX_LOAD_U64(uint32_t(r10) + 8);
  // ld r9,8(r9)
  r9 = REX_LOAD_U64(uint32_t(r9) + 8);
  // ld r8,8(r8)
  r8 = REX_LOAD_U64(uint32_t(r8) + 8);
  // ld r7,8(r7)
  r7 = REX_LOAD_U64(uint32_t(r7) + 8);
  // std r17,0(r6)
  REX_STORE_U64(uint32_t(r6) + 0, r17);
  // std r16,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r16);
  // std r15,0(r4)
  REX_STORE_U64(uint32_t(r4) + 0, r15);
  // std r14,0(r3)
  REX_STORE_U64(uint32_t(r3) + 0, r14);
  // std r10,8(r6)
  REX_STORE_U64(uint32_t(r6) + 8, r10);
  // std r9,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r9);
  // std r8,8(r4)
  REX_STORE_U64(uint32_t(r4) + 8, r8);
  // std r7,8(r3)
  REX_STORE_U64(uint32_t(r3) + 8, r7);
  // b 0x82129e88
  goto loc_82129E88;
loc_82129CE4:
  // lfs f11,96(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 96);
  f11 = double(temp.f32);
  // addi r10,r1,192
  r10 = uint64_t(int64_t(int64_t(r1) + 192));
  // lfs f10,100(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f10 = double(temp.f32);
  // addi r9,r1,224
  r9 = uint64_t(int64_t(int64_t(r1) + 224));
  // lfs f0,96(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 96);
  f0 = double(temp.f32);
  // ld r4,96(r31)
  r4 = REX_LOAD_U64(uint32_t(r31) + 96);
  // lfs f13,100(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 100);
  f13 = double(temp.f32);
  // fsubs f0,f11,f0
  f0 = double(float(f11 - f0));
  // fsubs f13,f10,f13
  f13 = double(float(f10 - f13));
  // lfs f11,104(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 104);
  f11 = double(temp.f32);
  // lfs f12,104(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 104);
  f12 = double(temp.f32);
  // ld r17,80(r31)
  r17 = REX_LOAD_U64(uint32_t(r31) + 80);
  // fsubs f12,f11,f12
  f12 = double(float(f11 - f12));
  // lfs f10,96(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 96);
  f10 = double(temp.f32);
  // lfs f11,100(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f11 = double(temp.f32);
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stfs f31,204(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 204, temp.u32);
  // addi r7,r1,96
  r7 = uint64_t(int64_t(int64_t(r1) + 96));
  // stfs f31,236(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 236, temp.u32);
  // addi r6,r1,112
  r6 = uint64_t(int64_t(int64_t(r1) + 112));
  // ld r15,104(r31)
  r15 = REX_LOAD_U64(uint32_t(r31) + 104);
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // ld r14,88(r31)
  r14 = REX_LOAD_U64(uint32_t(r31) + 88);
  // mr r11,r26
  r11 = r26;
  // std r4,0(r8)
  REX_STORE_U64(uint32_t(r8) + 0, r4);
  // fsubs f10,f10,f0
  f10 = double(float(f10 - f0));
  // stfs f10,192(r1)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r1) + 192, temp.u32);
  // fsubs f11,f11,f13
  f11 = double(float(f11 - f13));
  // stfs f11,196(r1)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r1) + 196, temp.u32);
  // lfs f10,104(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 104);
  f10 = double(temp.f32);
  // std r17,0(r6)
  REX_STORE_U64(uint32_t(r6) + 0, r17);
  // lfs f11,80(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 80);
  f11 = double(temp.f32);
  // fsubs f10,f10,f12
  f10 = double(float(f10 - f12));
  // fsubs f0,f11,f0
  f0 = double(float(f11 - f0));
  // stfs f10,200(r1)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r1) + 200, temp.u32);
  // stfs f0,224(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 224, temp.u32);
  // ld r3,0(r10)
  r3 = REX_LOAD_U64(uint32_t(r10) + 0);
  // lfs f10,84(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 84);
  f10 = double(temp.f32);
  // ld r10,8(r10)
  r10 = REX_LOAD_U64(uint32_t(r10) + 8);
  // lfs f0,88(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 88);
  f0 = double(temp.f32);
  // fsubs f13,f10,f13
  f13 = double(float(f10 - f13));
  // fsubs f0,f0,f12
  f0 = double(float(f0 - f12));
  // stfs f13,228(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 228, temp.u32);
  // stfs f0,232(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 232, temp.u32);
  // ld r16,0(r9)
  r16 = REX_LOAD_U64(uint32_t(r9) + 0);
  // ld r9,8(r9)
  r9 = REX_LOAD_U64(uint32_t(r9) + 8);
  // std r3,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r3);
  // std r15,8(r8)
  REX_STORE_U64(uint32_t(r8) + 8, r15);
  // std r10,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r10);
  // std r16,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r16);
  // std r14,8(r6)
  REX_STORE_U64(uint32_t(r6) + 8, r14);
  // std r9,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r9);
  // b 0x82129e88
  goto loc_82129E88;
loc_82129DB8:
  // lfs f11,80(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 80);
  f11 = double(temp.f32);
  // addi r10,r1,208
  r10 = uint64_t(int64_t(int64_t(r1) + 208));
  // lfs f10,84(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 84);
  f10 = double(temp.f32);
  // addi r9,r1,240
  r9 = uint64_t(int64_t(int64_t(r1) + 240));
  // lfs f0,80(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 80);
  f0 = double(temp.f32);
  // ld r4,80(r31)
  r4 = REX_LOAD_U64(uint32_t(r31) + 80);
  // lfs f13,84(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 84);
  f13 = double(temp.f32);
  // fsubs f0,f11,f0
  f0 = double(float(f11 - f0));
  // fsubs f13,f10,f13
  f13 = double(float(f10 - f13));
  // lfs f11,88(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 88);
  f11 = double(temp.f32);
  // lfs f12,88(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 88);
  f12 = double(temp.f32);
  // ld r17,96(r31)
  r17 = REX_LOAD_U64(uint32_t(r31) + 96);
  // fsubs f12,f11,f12
  f12 = double(float(f11 - f12));
  // lfs f10,80(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 80);
  f10 = double(temp.f32);
  // lfs f11,84(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 84);
  f11 = double(temp.f32);
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // stfs f31,220(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 220, temp.u32);
  // addi r7,r1,96
  r7 = uint64_t(int64_t(int64_t(r1) + 96));
  // stfs f31,252(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 252, temp.u32);
  // addi r6,r1,112
  r6 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r5,r1,144
  r5 = uint64_t(int64_t(int64_t(r1) + 144));
  // ld r15,88(r31)
  r15 = REX_LOAD_U64(uint32_t(r31) + 88);
  // ld r14,104(r31)
  r14 = REX_LOAD_U64(uint32_t(r31) + 104);
  // mr r11,r25
  r11 = r25;
  // std r4,0(r8)
  REX_STORE_U64(uint32_t(r8) + 0, r4);
  // fsubs f10,f10,f0
  f10 = double(float(f10 - f0));
  // stfs f10,208(r1)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r1) + 208, temp.u32);
  // fsubs f11,f11,f13
  f11 = double(float(f11 - f13));
  // stfs f11,212(r1)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r1) + 212, temp.u32);
  // lfs f10,88(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 88);
  f10 = double(temp.f32);
  // std r17,0(r6)
  REX_STORE_U64(uint32_t(r6) + 0, r17);
  // lfs f11,96(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 96);
  f11 = double(temp.f32);
  // fsubs f10,f10,f12
  f10 = double(float(f10 - f12));
  // fsubs f0,f11,f0
  f0 = double(float(f11 - f0));
  // stfs f0,240(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 240, temp.u32);
  // stfs f10,216(r1)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r1) + 216, temp.u32);
  // ld r3,0(r10)
  r3 = REX_LOAD_U64(uint32_t(r10) + 0);
  // lfs f10,100(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f10 = double(temp.f32);
  // ld r10,8(r10)
  r10 = REX_LOAD_U64(uint32_t(r10) + 8);
  // lfs f0,104(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 104);
  f0 = double(temp.f32);
  // fsubs f13,f10,f13
  f13 = double(float(f10 - f13));
  // fsubs f0,f0,f12
  f0 = double(float(f0 - f12));
  // stfs f13,244(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 244, temp.u32);
  // stfs f0,248(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 248, temp.u32);
  // ld r16,0(r9)
  r16 = REX_LOAD_U64(uint32_t(r9) + 0);
  // ld r9,8(r9)
  r9 = REX_LOAD_U64(uint32_t(r9) + 8);
  // std r3,0(r7)
  REX_STORE_U64(uint32_t(r7) + 0, r3);
  // std r15,8(r8)
  REX_STORE_U64(uint32_t(r8) + 8, r15);
  // std r10,8(r7)
  REX_STORE_U64(uint32_t(r7) + 8, r10);
  // std r16,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r16);
  // std r14,8(r6)
  REX_STORE_U64(uint32_t(r6) + 8, r14);
  // std r9,8(r5)
  REX_STORE_U64(uint32_t(r5) + 8, r9);
loc_82129E88:
  // lfs f0,128(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 128);
  f0 = double(temp.f32);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // stfs f0,-8(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + -8, temp.u32);
  // addi r23,r23,1
  r23 = uint64_t(int64_t(int64_t(r23) + 1));
  // lfs f0,132(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 132);
  f0 = double(temp.f32);
  // stfs f0,-4(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + -4, temp.u32);
  // lfs f0,136(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 136);
  f0 = double(temp.f32);
  // stfs f0,0(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 0, temp.u32);
  // lfs f0,140(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 140);
  f0 = double(temp.f32);
  // stfs f0,4(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 4, temp.u32);
  // lfs f0,96(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 96);
  f0 = double(temp.f32);
  // stfs f0,8(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 8, temp.u32);
  // lfs f0,100(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 100);
  f0 = double(temp.f32);
  // stfs f0,12(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 12, temp.u32);
  // lfs f0,104(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 104);
  f0 = double(temp.f32);
  // stfs f0,16(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 16, temp.u32);
  // lfs f0,108(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 108);
  f0 = double(temp.f32);
  // stfs f0,20(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 20, temp.u32);
  // lfs f0,112(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 112);
  f0 = double(temp.f32);
  // stfs f0,24(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 24, temp.u32);
  // lfs f0,116(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 116);
  f0 = double(temp.f32);
  // stfs f0,28(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 28, temp.u32);
  // lfs f0,120(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 120);
  f0 = double(temp.f32);
  // stfs f0,32(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 32, temp.u32);
  // lfs f0,124(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 124);
  f0 = double(temp.f32);
  // stfs f0,36(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 36, temp.u32);
  // lfs f0,144(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 144);
  f0 = double(temp.f32);
  // lfs f13,148(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 148);
  f13 = double(temp.f32);
  // lfs f12,152(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 152);
  f12 = double(temp.f32);
  // lfs f11,156(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 156);
  f11 = double(temp.f32);
  // stfs f0,40(r29)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r29) + 40, temp.u32);
  // stfs f13,44(r29)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r29) + 44, temp.u32);
  // stfs f12,48(r29)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r29) + 48, temp.u32);
  // stfs f11,52(r29)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r29) + 52, temp.u32);
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // stw r11,31992(r29)
  REX_STORE_U32(uint32_t(r29) + 31992, uint32_t(r11));
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r11,31996(r29)
  REX_STORE_U32(uint32_t(r29) + 31996, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // stw r11,32000(r29)
  REX_STORE_U32(uint32_t(r29) + 32000, uint32_t(r11));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // stw r11,32004(r29)
  REX_STORE_U32(uint32_t(r29) + 32004, uint32_t(r11));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // stw r11,32008(r29)
  REX_STORE_U32(uint32_t(r29) + 32008, uint32_t(r11));
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r11,32012(r29)
  REX_STORE_U32(uint32_t(r29) + 32012, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // stw r11,32016(r29)
  REX_STORE_U32(uint32_t(r29) + 32016, uint32_t(r11));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // stw r11,32020(r29)
  REX_STORE_U32(uint32_t(r29) + 32020, uint32_t(r11));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // stw r11,32024(r29)
  REX_STORE_U32(uint32_t(r29) + 32024, uint32_t(r11));
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r11,32028(r29)
  REX_STORE_U32(uint32_t(r29) + 32028, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // stw r11,32032(r29)
  REX_STORE_U32(uint32_t(r29) + 32032, uint32_t(r11));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // stw r11,32036(r29)
  REX_STORE_U32(uint32_t(r29) + 32036, uint32_t(r11));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // stw r11,32040(r29)
  REX_STORE_U32(uint32_t(r29) + 32040, uint32_t(r11));
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r11,32044(r29)
  REX_STORE_U32(uint32_t(r29) + 32044, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // stw r11,32048(r29)
  REX_STORE_U32(uint32_t(r29) + 32048, uint32_t(r11));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // stw r11,32052(r29)
  REX_STORE_U32(uint32_t(r29) + 32052, uint32_t(r11));
  // addi r29,r29,64
  r29 = uint64_t(int64_t(int64_t(r29) + 64));
  // stw r18,-2000(r20)
  REX_STORE_U32(uint32_t(r20) + -2000, uint32_t(r18));
  // lwz r11,120(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 120);
  // stw r11,0(r20)
  REX_STORE_U32(uint32_t(r20) + 0, uint32_t(r11));
  // addi r20,r20,4
  r20 = uint64_t(int64_t(int64_t(r20) + 4));
  // bge cr6,0x8212a0a0
  if (!cr6.lt) goto loc_8212A0A0;
  // stfs f0,-8(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + -8, temp.u32);
  // addi r24,r24,1
  r24 = uint64_t(int64_t(int64_t(r24) + 1));
  // stfs f13,-4(r30)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r30) + -4, temp.u32);
  // stfs f12,0(r30)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r30) + 0, temp.u32);
  // stfs f11,4(r30)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r30) + 4, temp.u32);
  // lfs f10,80(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 80);
  f10 = double(temp.f32);
  // stfs f10,8(r30)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r30) + 8, temp.u32);
  // lfs f10,84(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 84);
  f10 = double(temp.f32);
  // stfs f10,12(r30)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r30) + 12, temp.u32);
  // lfs f10,88(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 88);
  f10 = double(temp.f32);
  // stfs f10,16(r30)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r30) + 16, temp.u32);
  // lfs f10,92(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 92);
  f10 = double(temp.f32);
  // stfs f10,20(r30)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r30) + 20, temp.u32);
  // stfs f0,24(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 24, temp.u32);
  // stfs f13,28(r30)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r30) + 28, temp.u32);
  // stfs f12,32(r30)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r30) + 32, temp.u32);
  // stfs f11,36(r30)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r30) + 36, temp.u32);
  // lfs f0,96(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 96);
  f0 = double(temp.f32);
  // stfs f0,40(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 40, temp.u32);
  // lfs f0,100(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 100);
  f0 = double(temp.f32);
  // stfs f0,44(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 44, temp.u32);
  // lfs f0,104(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 104);
  f0 = double(temp.f32);
  // stfs f0,48(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 48, temp.u32);
  // lfs f0,108(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 108);
  f0 = double(temp.f32);
  // stfs f0,52(r30)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r30) + 52, temp.u32);
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // stw r11,31992(r30)
  REX_STORE_U32(uint32_t(r30) + 31992, uint32_t(r11));
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r11,31996(r30)
  REX_STORE_U32(uint32_t(r30) + 31996, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // stw r11,32000(r30)
  REX_STORE_U32(uint32_t(r30) + 32000, uint32_t(r11));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // stw r11,32004(r30)
  REX_STORE_U32(uint32_t(r30) + 32004, uint32_t(r11));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // stw r11,32008(r30)
  REX_STORE_U32(uint32_t(r30) + 32008, uint32_t(r11));
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r11,32012(r30)
  REX_STORE_U32(uint32_t(r30) + 32012, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // stw r11,32016(r30)
  REX_STORE_U32(uint32_t(r30) + 32016, uint32_t(r11));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // stw r11,32020(r30)
  REX_STORE_U32(uint32_t(r30) + 32020, uint32_t(r11));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // stw r11,32024(r30)
  REX_STORE_U32(uint32_t(r30) + 32024, uint32_t(r11));
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r11,32028(r30)
  REX_STORE_U32(uint32_t(r30) + 32028, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // stw r11,32032(r30)
  REX_STORE_U32(uint32_t(r30) + 32032, uint32_t(r11));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // stw r11,32036(r30)
  REX_STORE_U32(uint32_t(r30) + 32036, uint32_t(r11));
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // stw r11,32040(r30)
  REX_STORE_U32(uint32_t(r30) + 32040, uint32_t(r11));
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r11,32044(r30)
  REX_STORE_U32(uint32_t(r30) + 32044, uint32_t(r11));
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // stw r11,32048(r30)
  REX_STORE_U32(uint32_t(r30) + 32048, uint32_t(r11));
  // lwz r11,44(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 44);
  // stw r11,32052(r30)
  REX_STORE_U32(uint32_t(r30) + 32052, uint32_t(r11));
  // addi r30,r30,64
  r30 = uint64_t(int64_t(int64_t(r30) + 64));
  // stw r18,-2000(r21)
  REX_STORE_U32(uint32_t(r21) + -2000, uint32_t(r18));
  // lwz r11,120(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 120);
  // stw r11,0(r21)
  REX_STORE_U32(uint32_t(r21) + 0, uint32_t(r11));
  // addi r21,r21,4
  r21 = uint64_t(int64_t(int64_t(r21) + 4));
loc_8212A0A0:
  // cmplwi cr6,r23,499
  cr6 = lifted::Compare<uint32_t>(uint32_t(r23), 499, so);
  // bgt cr6,0x8212a0d4
  if (cr6.gt) goto loc_8212A0D4;
  // cmplwi cr6,r24,499
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 499, so);
  // bgt cr6,0x8212a0d4
  if (cr6.gt) goto loc_8212A0D4;
  // lwz r17,160(r1)
  r17 = REX_LOAD_U32(uint32_t(r1) + 160);
  // lwz r9,80(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 80);
  // b 0x8212a0c0
  goto loc_8212A0C0;
loc_8212A0BC:
  // stw r26,116(r31)
  REX_STORE_U32(uint32_t(r31) + 116, uint32_t(r26));
loc_8212A0C0:
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // lwz r11,0(r17)
  r11 = REX_LOAD_U32(uint32_t(r17) + 0);
  // cmpw cr6,r9,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r9), int32_t(r11), so);
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // blt cr6,0x82129b74
  if (cr6.lt) goto loc_82129B74;
loc_8212A0D4:
  // mr r4,r19
  r4 = r19;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2832
  r12 = r12 | 2832;
  // stwx r23,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r23));
  // mr r3,r27
  r3 = r27;
  // lis r12,2
  r12 = uint64_t(int64_t(131072));
  // ori r12,r12,5392
  r12 = r12 | 5392;
  // stwx r24,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r24));
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82126630, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r22
  r4 = r22;
  // addi r3,r1,288
  r3 = uint64_t(int64_t(int64_t(r1) + 288));
  // bl 0x821292b0
  // (left out: dead in C++)
  { const lifted::Result o = f821292B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr r4,r19
  r4 = r19;
  // mr r3,r27
  r3 = r27;
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82126630, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r22
  r4 = r22;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2848
  r12 = r12 | 2848;
  // add r3,r1,r12
  r3 = r1 + r12;
  // bl 0x821292b0
  // (left out: dead in C++)
  { const lifted::Result o = f821292B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_8212A12C:
  // lwz r1,0(r1)
  r1 = REX_LOAD_U32(uint32_t(r1) + 0);
  // lfd f30,-168(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -168)));
  // lfd f31,-160(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -160)));
  // b 0x825a6ea0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8212ACF0: lifted from generated/ by lift.py.
lifted::Result f8212ACF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8212ACF0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Vec v0{}, v6{}, v7{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{};
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-48
  r12 = uint64_t(int64_t(int64_t(r1) + -48));
  // bl 0x825a7914
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r12,-2
  r12 = uint64_t(int64_t(-131072));
  // ori r12,r12,62672
  r12 = r12 | 62672;
  // bl 0x825a9934
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwux r1,r1,r12
  ea = uint32_t(r1) + uint32_t(r12);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r27,r4
  r27 = r4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r9,r27,592
  r9 = uint64_t(int64_t(static_cast<int64_t>(r27 * static_cast<uint64_t>(592))));
  // lwz r10,32528(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 32528);
  // mulli r10,r10,720
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(720))));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // add r10,r10,r30
  r10 = r10 + r30;
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // addi r31,r10,128
  r31 = uint64_t(int64_t(int64_t(r10) + 128));
  // add r28,r9,r11
  r28 = r9 + r11;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // bl 0x821298c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821298C0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,108(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 108);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2756
  r12 = r12 | 2756;
  // stwx r11,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,276(r1)
  REX_STORE_U32(uint32_t(r1) + 276, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2752
  r12 = r12 | 2752;
  // stwx r11,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r11));
  // lwz r11,32536(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 32536);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x8212b174
  if (!cr6.gt) goto loc_8212B174;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f2,7140(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 7140);
  f2 = double(temp.f32);
  // lfs f0,2608(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2608);
  f0 = double(temp.f32);
  // lfs f1,2612(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f1 = double(temp.f32);
  // lfs f28,11540(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11540);
  f28 = double(temp.f32);
loc_8212ADA0:
  // lwz r11,96(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 96);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8212b140
  if (cr6.eq) goto loc_8212B140;
  // lfs f13,124(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 124);
  f13 = double(temp.f32);
  // lwz r11,32624(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 32624);
  // lfs f11,116(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 116);
  f11 = double(temp.f32);
  // fmadds f13,f11,f13,f13
  f13 = double(float(std::fma(f11, f13, f13)));
  // lfs f12,100(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f12 = double(temp.f32);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // fmuls f13,f13,f28
  f13 = double(float(f13 * f28));
  // bne cr6,0x8212ade0
  if (!cr6.eq) goto loc_8212ADE0;
  // cmpwi cr6,r27,3
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 3, so);
  // beq cr6,0x8212ade0
  if (cr6.eq) goto loc_8212ADE0;
  // fcmpu cr6,f13,f12
  cr6 = lifted::CompareFloat(f13, f12);
  // bge cr6,0x8212ade0
  if (!cr6.lt) goto loc_8212ADE0;
  // fmr f12,f13
  f12 = f13;
loc_8212ADE0:
  // addi r11,r28,464
  r11 = uint64_t(int64_t(int64_t(r28) + 464));
  // vspltisw v9,1
  simde_mm_store_si128((simde__m128i*)v9.u32, simde_mm_set1_epi32(int(0x1)));
  // vspltisw v8,0
  simde_mm_store_si128((simde__m128i*)v8.u32, simde_mm_set1_epi32(int(0x0)));
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // vcfsx v10,v9,1
  lifted::VectorFloatMode();
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // lvx128 v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmsum3fp128 v13,v0,v0
  simde_mm_store_ps(v13.f32, simde_mm_dp_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v0.f32), 0xEF));
  // vrsqrtefp v12,v13
  simde_mm_store_ps(v12.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v13.f32))));
  // vmulfp128 v7,v13,v10
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v10.f32)));
  // vcmpeqfp v11,v13,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v6,v12,v12
  simde_mm_store_ps(v6.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v10,v7,v6,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v6.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v12,v12,v10,v12
  simde_mm_store_ps(v12.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v12.f32)));
  // vmulfp128 v12,v13,v12
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v12.f32)));
  // vsel v11,v12,v13,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v12.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v13.u8))));
  // stvx v11,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f13,144(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 144);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f1
  cr6 = lifted::CompareFloat(f13, f1);
  // beq cr6,0x8212af38
  if (cr6.eq) goto loc_8212AF38;
  // stfs f1,112(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 112, temp.u32);
  // vcfsx v10,v9,1
  lifted::VectorFloatMode();
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // stfs f0,116(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // addi r9,r1,160
  r9 = uint64_t(int64_t(int64_t(r1) + 160));
  // stfs f1,120(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 120, temp.u32);
  // stfs f1,124(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // stfs f1,92(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // stfs f1,108(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // lvx128 v13,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r10,r1,192
  r10 = uint64_t(int64_t(int64_t(r1) + 192));
  // vpermwi128 v12,v13,135
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x78));
  // vpermwi128 v11,v13,99
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x9C));
  // vpermwi128 v13,v0,99
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x9C));
  // vpermwi128 v0,v0,135
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x78));
  // vmulfp128 v13,v13,v12
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v13,v0,v11,v13
  simde_mm_store_ps(v13.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v11.f32)), simde_mm_load_ps(v13.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmsum3fp128 v0,v13,v13
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v13.f32), 0xEF));
  // stvx v13,r0,r31
  ea = (uint32_t(r31)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vrsqrtefp v12,v0
  simde_mm_store_ps(v12.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v0.f32))));
  // vmulfp128 v7,v0,v10
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v10.f32)));
  // vcmpeqfp v11,v0,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v6,v12,v12
  simde_mm_store_ps(v6.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v10,v7,v6,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v6.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v12,v12,v10,v12
  simde_mm_store_ps(v12.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v12.f32)));
  // vcfsx v10,v9,1
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // vmulfp128 v12,v0,v12
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v12.f32)));
  // vsel v11,v12,v0,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v12.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v0.u8))));
  // stvx v11,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f13,192(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 192);
  f13 = double(temp.f32);
  // fdivs f13,f12,f13
  f13 = double(float(f12 / f13));
  // stfs f13,80(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f13,84(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // stfs f13,88(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lvx128 v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r10,r31,16
  r10 = uint64_t(int64_t(int64_t(r31) + 16));
  // vmulfp128 v0,v13,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v0.f32)));
  // stvx v0,r0,r31
  ea = (uint32_t(r31)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vpermwi128 v12,v0,99
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x9C));
  // lvx128 v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vpermwi128 v11,v0,135
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x78));
  // vpermwi128 v0,v13,135
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x78));
  // vpermwi128 v13,v13,99
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x9C));
  // vmulfp128 v0,v12,v0
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v0.f32)));
  // vnmsubfp v0,v11,v13,v0
  simde_mm_store_ps(v0.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v13.f32)), simde_mm_load_ps(v0.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvx128 v12,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmsum3fp128 v0,v12,v12
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32), 0xEF));
  // vrsqrtefp v13,v0
  simde_mm_store_ps(v13.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v0.f32))));
  // vcmpeqfp v11,v0,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v9,v0,v10
  simde_mm_store_ps(v9.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v10.f32)));
  // vmulfp128 v8,v13,v13
  simde_mm_store_ps(v8.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v13.f32)));
  // vnmsubfp v10,v9,v8,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v9.f32), simde_mm_load_ps(v8.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v13,v13,v10,v13
  simde_mm_store_ps(v13.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v13.f32)));
  // vmulfp128 v13,v0,v13
  simde_mm_store_ps(v13.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v13.f32)));
  // vsel v11,v13,v0,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v13.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v0.u8))));
  // stvx v11,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f13,160(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 160);
  f13 = double(temp.f32);
  // fdivs f13,f12,f13
  f13 = double(float(f12 / f13));
  // stfs f13,96(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // stfs f13,100(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // stfs f13,104(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // lvx128 v0,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmulfp128 v0,v12,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v0.f32)));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_8212AF38:
  // lwz r10,128(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 128);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble cr6,0x8212b138
  if (!cr6.gt) goto loc_8212B138;
  // lvx128 v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r10,r1,296
  r10 = uint64_t(int64_t(int64_t(r1) + 296));
  // rlwinm r11,r29,6,0,25
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 6) & 0xFFFFFFC0;
  // lfs f31,400(r28)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 400);
  f31 = double(temp.f32);
  // rlwinm r9,r29,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lfs f30,404(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 404);
  f30 = double(temp.f32);
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,752
  r12 = r12 | 752;
  // add r8,r1,r12
  r8 = r1 + r12;
  // lfs f29,408(r28)
  temp.u32 = REX_LOAD_U32(uint32_t(r28) + 408);
  f29 = double(temp.f32);
  // add r11,r11,r10
  r11 = r11 + r10;
  // stfs f0,140(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 140, temp.u32);
  // add r9,r9,r8
  r9 = r9 + r8;
  // addi r10,r31,248
  r10 = uint64_t(int64_t(int64_t(r31) + 248));
loc_8212AF80:
  // lfs f7,-8(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -8);
  f7 = double(temp.f32);
  // lfs f6,-4(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -4);
  f6 = double(temp.f32);
  // fsubs f13,f7,f31
  f13 = double(float(f7 - f31));
  // stfs f13,128(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 128, temp.u32);
  // fsubs f13,f6,f30
  f13 = double(float(f6 - f30));
  // lfs f5,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f5 = double(temp.f32);
  // stfs f13,132(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 132, temp.u32);
  // fsubs f13,f5,f29
  f13 = double(float(f5 - f29));
  // stfs f13,136(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 136, temp.u32);
  // addi r8,r1,128
  r8 = uint64_t(int64_t(int64_t(r1) + 128));
  // lvx128 v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r8,r1,176
  r8 = uint64_t(int64_t(int64_t(r1) + 176));
  // vmsum3fp128 v13,v13,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_dp_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v0.f32), 0xEF));
  // stvx v13,r0,r8
  ea = (uint32_t(r8)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lfs f13,176(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 176);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f1
  cr6 = lifted::CompareFloat(f13, f1);
  // blt cr6,0x8212afcc
  if (cr6.lt) goto loc_8212AFCC;
  // li r8,0
  r8 = uint64_t(int64_t(0));
loc_8212AFCC:
  // clrlwi. r8,r8,24
  r8 = uint32_t(r8) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x8212b124
  if (!cr0.eq) goto loc_8212B124;
  // lwz r8,32(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 32);
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x8212b124
  if (cr6.eq) goto loc_8212B124;
  // lfs f13,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // lfs f12,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // fmuls f13,f13,f2
  f13 = double(float(f13 * f2));
  // lfs f11,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f11 = double(temp.f32);
  // fmuls f12,f12,f2
  f12 = double(float(f12 * f2));
  // fmuls f11,f11,f2
  f11 = double(float(f11 * f2));
  // lfs f4,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f4 = double(temp.f32);
  // lfs f3,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f3 = double(temp.f32);
  // cmplwi cr6,r29,499
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 499, so);
  // lfs f27,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f27 = double(temp.f32);
  // fsubs f10,f7,f13
  f10 = double(float(f7 - f13));
  // fsubs f9,f6,f12
  f9 = double(float(f6 - f12));
  // fsubs f8,f5,f11
  f8 = double(float(f5 - f11));
  // fadds f13,f13,f7
  f13 = double(float(f13 + f7));
  // lfs f7,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f7 = double(temp.f32);
  // fadds f12,f12,f6
  f12 = double(float(f12 + f6));
  // lfs f6,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f6 = double(temp.f32);
  // fadds f11,f11,f5
  f11 = double(float(f11 + f5));
  // lfs f5,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f5 = double(temp.f32);
  // stfs f10,-8(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + -8, temp.u32);
  // stfs f9,-4(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // stfs f8,0(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // stfs f13,8(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // fadds f7,f7,f10
  f7 = double(float(f7 + f10));
  // stfs f12,12(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // fadds f6,f6,f9
  f6 = double(float(f6 + f9));
  // stfs f11,16(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // fadds f5,f5,f8
  f5 = double(float(f5 + f8));
  // stfs f0,20(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // fadds f4,f13,f4
  f4 = double(float(f13 + f4));
  // stfs f7,24(r11)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // fadds f3,f3,f12
  f3 = double(float(f3 + f12));
  // stfs f6,28(r11)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // fadds f10,f27,f11
  f10 = double(float(f27 + f11));
  // stfs f5,32(r11)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // stfs f0,36(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 36, temp.u32);
  // stfs f4,40(r11)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r11) + 40, temp.u32);
  // stfs f3,44(r11)
  temp.f32 = float(f3);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // stfs f10,48(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 48, temp.u32);
  // stfs f0,52(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 52, temp.u32);
  // lwz r8,80(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r8,31992(r11)
  REX_STORE_U32(uint32_t(r11) + 31992, uint32_t(r8));
  // lwz r8,84(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r8,31996(r11)
  REX_STORE_U32(uint32_t(r11) + 31996, uint32_t(r8));
  // lwz r8,88(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r8,32000(r11)
  REX_STORE_U32(uint32_t(r11) + 32000, uint32_t(r8));
  // lwz r8,92(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r8,32004(r11)
  REX_STORE_U32(uint32_t(r11) + 32004, uint32_t(r8));
  // lwz r8,80(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r8,32008(r11)
  REX_STORE_U32(uint32_t(r11) + 32008, uint32_t(r8));
  // lwz r8,84(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r8,32012(r11)
  REX_STORE_U32(uint32_t(r11) + 32012, uint32_t(r8));
  // lwz r8,88(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r8,32016(r11)
  REX_STORE_U32(uint32_t(r11) + 32016, uint32_t(r8));
  // lwz r8,92(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r8,32020(r11)
  REX_STORE_U32(uint32_t(r11) + 32020, uint32_t(r8));
  // lwz r8,80(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r8,32024(r11)
  REX_STORE_U32(uint32_t(r11) + 32024, uint32_t(r8));
  // lwz r8,84(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r8,32028(r11)
  REX_STORE_U32(uint32_t(r11) + 32028, uint32_t(r8));
  // lwz r8,88(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r8,32032(r11)
  REX_STORE_U32(uint32_t(r11) + 32032, uint32_t(r8));
  // lwz r8,92(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r8,32036(r11)
  REX_STORE_U32(uint32_t(r11) + 32036, uint32_t(r8));
  // lwz r8,80(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r8,32040(r11)
  REX_STORE_U32(uint32_t(r11) + 32040, uint32_t(r8));
  // lwz r8,84(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r8,32044(r11)
  REX_STORE_U32(uint32_t(r11) + 32044, uint32_t(r8));
  // lwz r8,88(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r8,32048(r11)
  REX_STORE_U32(uint32_t(r11) + 32048, uint32_t(r8));
  // lwz r8,92(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r8,32052(r11)
  REX_STORE_U32(uint32_t(r11) + 32052, uint32_t(r8));
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // lwz r8,28(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 28);
  // stw r8,-2000(r9)
  REX_STORE_U32(uint32_t(r9) + -2000, uint32_t(r8));
  // lwz r8,24(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 24);
  // stw r8,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r8));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // bgt cr6,0x8212b174
  if (cr6.gt) goto loc_8212B174;
loc_8212B124:
  // lwz r8,128(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 128);
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // addi r10,r10,48
  r10 = uint64_t(int64_t(int64_t(r10) + 48));
  // cmpw cr6,r7,r8
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r8), so);
  // blt cr6,0x8212af80
  if (cr6.lt) goto loc_8212AF80;
loc_8212B138:
  // cmplwi cr6,r29,499
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 499, so);
  // bgt cr6,0x8212b174
  if (cr6.gt) goto loc_8212B174;
loc_8212B140:
  // lwz r11,104(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 104);
  // mulli r11,r11,720
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(720))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // addi r11,r11,-592
  r11 = uint64_t(int64_t(int64_t(r11) + -592));
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x8212b160
  if (!cr6.eq) goto loc_8212B160;
  // addi r31,r30,128
  r31 = uint64_t(int64_t(int64_t(r30) + 128));
  // b 0x8212b164
  goto loc_8212B164;
loc_8212B160:
  // addi r31,r31,720
  r31 = uint64_t(int64_t(int64_t(r31) + 720));
loc_8212B164:
  // lwz r11,32536(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 32536);
  // addi r6,r6,1
  r6 = uint64_t(int64_t(int64_t(r6) + 1));
  // cmpw cr6,r6,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r6), int32_t(r11), so);
  // blt cr6,0x8212ada0
  if (cr6.lt) goto loc_8212ADA0;
loc_8212B174:
  // mr r4,r27
  r4 = r27;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2752
  r12 = r12 | 2752;
  // stwx r29,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r29));
  // mr r3,r30
  r3 = r30;
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82126630, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r28
  r4 = r28;
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x821292b0
  // (left out: dead in C++)
  { const lifted::Result o = f821292B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r1,0(r1)
  r1 = REX_LOAD_U32(uint32_t(r1) + 0);
  // addi r12,r1,-48
  r12 = uint64_t(int64_t(int64_t(r1) + -48));
  // bl 0x825a7960
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8212BB20: lifted from generated/ by lift.py.
lifted::Result f8212BB20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8212BB20 (lifted)");
  uint64_t r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f21 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Vec v0{}, v6{}, v7{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{};
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-48
  r12 = uint64_t(int64_t(int64_t(r1) + -48));
  // bl 0x825a78fc
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r12,-2
  r12 = uint64_t(int64_t(-131072));
  // ori r12,r12,62656
  r12 = r12 | 62656;
  // bl 0x825a9934
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwux r1,r1,r12
  ea = uint32_t(r1) + uint32_t(r12);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r28,r4
  r28 = r4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r9,r28,592
  r9 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(592))));
  // lwz r10,2768(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 2768);
  // mulli r10,r10,176
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(176))));
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // add r10,r10,r30
  r10 = r10 + r30;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r31,r10,128
  r31 = uint64_t(int64_t(int64_t(r10) + 128));
  // add r27,r9,r11
  r27 = r9 + r11;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // bl 0x821298c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821298C0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,108(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 108);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2724
  r12 = r12 | 2724;
  // stwx r11,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,244(r1)
  REX_STORE_U32(uint32_t(r1) + 244, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2720
  r12 = r12 | 2720;
  // stwx r11,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r11));
  // lwz r11,2776(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 2776);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x8212bf4c
  if (!cr6.gt) goto loc_8212BF4C;
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // vspltisw v9,1
  simde_mm_store_si128((simde__m128i*)v9.u32, simde_mm_set1_epi32(int(0x1)));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // vspltisw v8,0
  simde_mm_store_si128((simde__m128i*)v8.u32, simde_mm_set1_epi32(int(0x0)));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r27,464
  r3 = uint64_t(int64_t(int64_t(r27) + 464));
  // lfs f0,7140(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 7140);
  f0 = double(temp.f32);
  // lfs f13,2608(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2608);
  f13 = double(temp.f32);
  // lfs f30,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f30 = double(temp.f32);
loc_8212BBD4:
  // lvx128 v0,r0,r3
  ea = (uint32_t(r3)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vcfsx v10,v9,1
  lifted::VectorFloatMode();
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // vmsum3fp128 v13,v0,v0
  simde_mm_store_ps(v13.f32, simde_mm_dp_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v0.f32), 0xEF));
  // addi r11,r1,128
  r11 = uint64_t(int64_t(int64_t(r1) + 128));
  // vrsqrtefp v12,v13
  simde_mm_store_ps(v12.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v13.f32))));
  // vmulfp128 v7,v13,v10
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v10.f32)));
  // vcmpeqfp v11,v13,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v6,v12,v12
  simde_mm_store_ps(v6.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v10,v7,v6,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v6.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v12,v12,v10,v12
  simde_mm_store_ps(v12.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v12.f32)));
  // vmulfp128 v12,v13,v12
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v12.f32)));
  // vsel v11,v12,v13,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v12.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v13.u8))));
  // stvx v11,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f12,128(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 128);
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f30
  cr6 = lifted::CompareFloat(f12, f30);
  // beq cr6,0x8212bd28
  if (cr6.eq) goto loc_8212BD28;
  // stfs f30,96(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // vcfsx v10,v9,1
  lifted::VectorFloatMode();
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // stfs f13,100(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // stfs f30,104(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // stfs f30,108(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f12,100(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f12 = double(temp.f32);
  // stfs f30,92(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // stfs f30,124(r1)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // lvx128 v13,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r1,160
  r11 = uint64_t(int64_t(int64_t(r1) + 160));
  // vpermwi128 v12,v13,135
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x78));
  // vpermwi128 v11,v13,99
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x9C));
  // vpermwi128 v13,v0,99
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x9C));
  // vpermwi128 v0,v0,135
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x78));
  // vmulfp128 v13,v13,v12
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v13,v0,v11,v13
  simde_mm_store_ps(v13.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v11.f32)), simde_mm_load_ps(v13.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmsum3fp128 v0,v13,v13
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v13.f32), 0xEF));
  // stvx v13,r0,r31
  ea = (uint32_t(r31)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vrsqrtefp v12,v0
  simde_mm_store_ps(v12.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v0.f32))));
  // vmulfp128 v7,v0,v10
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v10.f32)));
  // vcmpeqfp v11,v0,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v6,v12,v12
  simde_mm_store_ps(v6.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v10,v7,v6,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v6.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v12,v12,v10,v12
  simde_mm_store_ps(v12.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v12.f32)));
  // vcfsx v10,v9,1
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // vmulfp128 v12,v0,v12
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v12.f32)));
  // vsel v11,v12,v0,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v12.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v0.u8))));
  // stvx v11,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f11,160(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 160);
  f11 = double(temp.f32);
  // fdivs f12,f12,f11
  f12 = double(float(f12 / f11));
  // stfs f12,80(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f12,84(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // stfs f12,88(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // lvx128 v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r11,r31,16
  r11 = uint64_t(int64_t(int64_t(r31) + 16));
  // vmulfp128 v0,v13,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v0.f32)));
  // stvx v0,r0,r31
  ea = (uint32_t(r31)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vpermwi128 v12,v0,99
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x9C));
  // lvx128 v13,r0,r3
  ea = (uint32_t(r3)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vpermwi128 v11,v0,135
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x78));
  // vpermwi128 v0,v13,135
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x78));
  // vpermwi128 v13,v13,99
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x9C));
  // vmulfp128 v0,v12,v0
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v0.f32)));
  // vnmsubfp v0,v11,v13,v0
  simde_mm_store_ps(v0.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v13.f32)), simde_mm_load_ps(v0.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvx128 v12,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmsum3fp128 v0,v12,v12
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32), 0xEF));
  // lfs f12,100(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f12 = double(temp.f32);
  // vrsqrtefp v13,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v0.f32))));
  // vmulfp128 v7,v0,v10
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v10.f32)));
  // vcmpeqfp v11,v0,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v6,v13,v13
  simde_mm_store_ps(v6.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v13.f32)));
  // vnmsubfp v10,v7,v6,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v6.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v13,v13,v10,v13
  simde_mm_store_ps(v13.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v13.f32)));
  // vmulfp128 v13,v0,v13
  simde_mm_store_ps(v13.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v13.f32)));
  // vsel v11,v13,v0,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v13.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v0.u8))));
  // stvx v11,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f11,144(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 144);
  f11 = double(temp.f32);
  // fdivs f12,f12,f11
  f12 = double(float(f12 / f11));
  // stfs f12,112(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 112, temp.u32);
  // stfs f12,116(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // stfs f12,120(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 120, temp.u32);
  // addi r10,r1,112
  r10 = uint64_t(int64_t(int64_t(r1) + 112));
  // lvx128 v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmulfp128 v0,v12,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v0.f32)));
  // stvx v0,r0,r11
  ea = (uint32_t(r11)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_8212BD28:
  // lwz r11,116(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 116);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x8212bf10
  if (!cr6.gt) goto loc_8212BF10;
  // addi r8,r1,264
  r8 = uint64_t(int64_t(int64_t(r1) + 264));
  // rlwinm r11,r29,6,0,25
  r11 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 6) & 0xFFFFFFC0;
  // rlwinm r7,r29,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r29) | (r29 << 32), 2) & 0xFFFFFFFC;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,720
  r12 = r12 | 720;
  // add r6,r1,r12
  r6 = r1 + r12;
  // add r11,r11,r8
  r11 = r11 + r8;
  // addi r9,r31,16
  r9 = uint64_t(int64_t(int64_t(r31) + 16));
  // addi r10,r31,80
  r10 = uint64_t(int64_t(int64_t(r31) + 80));
  // add r7,r7,r6
  r7 = r7 + r6;
  // addi r8,r31,136
  r8 = uint64_t(int64_t(int64_t(r31) + 136));
loc_8212BD64:
  // lfs f29,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f29 = double(temp.f32);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // lfs f12,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f12 = double(temp.f32);
  // fmr f6,f29
  f6 = f29;
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // lfs f28,4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 4);
  f28 = double(temp.f32);
  // lfs f11,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f11 = double(temp.f32);
  // fmr f2,f28
  f2 = f28;
  // lfs f9,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f9 = double(temp.f32);
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // lfs f27,8(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 8);
  f27 = double(temp.f32);
  // fmuls f9,f9,f0
  f9 = double(float(f9 * f0));
  // lfs f10,-8(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -8);
  f10 = double(temp.f32);
  // fmr f1,f27
  f1 = f27;
  // fmr f26,f29
  f26 = f29;
  // lfs f8,-4(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + -4);
  f8 = double(temp.f32);
  // fmr f25,f28
  f25 = f28;
  // lfs f7,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f7 = double(temp.f32);
  // fmr f24,f27
  f24 = f27;
  // stfs f13,4(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // fmr f23,f29
  f23 = f29;
  // cmplwi cr6,r29,499
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 499, so);
  // fmuls f6,f6,f0
  f6 = double(float(f6 * f0));
  // fsubs f5,f10,f12
  f5 = double(float(f10 - f12));
  // fadds f31,f12,f10
  f31 = double(float(f12 + f10));
  // fsubs f4,f8,f11
  f4 = double(float(f8 - f11));
  // fsubs f3,f7,f9
  f3 = double(float(f7 - f9));
  // fmuls f2,f2,f0
  f2 = double(float(f2 * f0));
  // fadds f7,f9,f7
  f7 = double(float(f9 + f7));
  // fmuls f1,f1,f0
  f1 = double(float(f1 * f0));
  // fadds f8,f11,f8
  f8 = double(float(f11 + f8));
  // fmuls f9,f26,f0
  f9 = double(float(f26 * f0));
  // fmr f22,f28
  f22 = f28;
  // fsubs f12,f5,f6
  f12 = double(float(f5 - f6));
  // stfs f12,-8(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + -8, temp.u32);
  // fmuls f6,f25,f0
  f6 = double(float(f25 * f0));
  // fmuls f5,f24,f0
  f5 = double(float(f24 * f0));
  // fmr f21,f27
  f21 = f27;
  // fsubs f11,f4,f2
  f11 = double(float(f4 - f2));
  // stfs f11,-4(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // fsubs f10,f3,f1
  f10 = double(float(f3 - f1));
  // stfs f10,0(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // stfs f13,20(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // fsubs f9,f31,f9
  f9 = double(float(f31 - f9));
  // stfs f9,8(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // fadds f12,f23,f12
  f12 = double(float(f23 + f12));
  // fsubs f8,f8,f6
  f8 = double(float(f8 - f6));
  // stfs f8,12(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // fsubs f7,f7,f5
  f7 = double(float(f7 - f5));
  // stfs f7,16(r11)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // stfs f12,24(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // fadds f11,f22,f11
  f11 = double(float(f22 + f11));
  // stfs f11,28(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f13,36(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 36, temp.u32);
  // fadds f10,f21,f10
  f10 = double(float(f21 + f10));
  // stfs f10,32(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // stfs f13,52(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 52, temp.u32);
  // fadds f9,f29,f9
  f9 = double(float(f29 + f9));
  // stfs f9,40(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 40, temp.u32);
  // fadds f12,f28,f8
  f12 = double(float(f28 + f8));
  // stfs f12,44(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // fadds f11,f27,f7
  f11 = double(float(f27 + f7));
  // stfs f11,48(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 48, temp.u32);
  // lwz r6,0(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r6,31992(r11)
  REX_STORE_U32(uint32_t(r11) + 31992, uint32_t(r6));
  // lwz r6,4(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r6,31996(r11)
  REX_STORE_U32(uint32_t(r11) + 31996, uint32_t(r6));
  // lwz r6,8(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r6,32000(r11)
  REX_STORE_U32(uint32_t(r11) + 32000, uint32_t(r6));
  // lwz r6,12(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 12);
  // stw r6,32004(r11)
  REX_STORE_U32(uint32_t(r11) + 32004, uint32_t(r6));
  // lwz r6,0(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r6,32008(r11)
  REX_STORE_U32(uint32_t(r11) + 32008, uint32_t(r6));
  // lwz r6,4(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r6,32012(r11)
  REX_STORE_U32(uint32_t(r11) + 32012, uint32_t(r6));
  // lwz r6,8(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r6,32016(r11)
  REX_STORE_U32(uint32_t(r11) + 32016, uint32_t(r6));
  // lwz r6,12(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 12);
  // stw r6,32020(r11)
  REX_STORE_U32(uint32_t(r11) + 32020, uint32_t(r6));
  // lwz r6,0(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r6,32024(r11)
  REX_STORE_U32(uint32_t(r11) + 32024, uint32_t(r6));
  // lwz r6,4(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r6,32028(r11)
  REX_STORE_U32(uint32_t(r11) + 32028, uint32_t(r6));
  // lwz r6,8(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r6,32032(r11)
  REX_STORE_U32(uint32_t(r11) + 32032, uint32_t(r6));
  // lwz r6,12(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 12);
  // stw r6,32036(r11)
  REX_STORE_U32(uint32_t(r11) + 32036, uint32_t(r6));
  // lwz r6,0(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r6,32040(r11)
  REX_STORE_U32(uint32_t(r11) + 32040, uint32_t(r6));
  // lwz r6,4(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r6,32044(r11)
  REX_STORE_U32(uint32_t(r11) + 32044, uint32_t(r6));
  // lwz r6,8(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r6,32048(r11)
  REX_STORE_U32(uint32_t(r11) + 32048, uint32_t(r6));
  // lwz r6,12(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 12);
  // stw r6,32052(r11)
  REX_STORE_U32(uint32_t(r11) + 32052, uint32_t(r6));
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // lwz r6,28(r8)
  r6 = REX_LOAD_U32(uint32_t(r8) + 28);
  // stw r6,-2000(r7)
  REX_STORE_U32(uint32_t(r7) + -2000, uint32_t(r6));
  // lwz r6,24(r8)
  r6 = REX_LOAD_U32(uint32_t(r8) + 24);
  // stw r6,0(r7)
  REX_STORE_U32(uint32_t(r7) + 0, uint32_t(r6));
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // bgt cr6,0x8212bf4c
  if (cr6.gt) goto loc_8212BF4C;
  // lwz r6,116(r31)
  r6 = REX_LOAD_U32(uint32_t(r31) + 116);
  // addi r5,r5,1
  r5 = uint64_t(int64_t(int64_t(r5) + 1));
  // addi r8,r8,48
  r8 = uint64_t(int64_t(int64_t(r8) + 48));
  // cmpw cr6,r5,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r5), int32_t(r6), so);
  // blt cr6,0x8212bd64
  if (cr6.lt) goto loc_8212BD64;
loc_8212BF10:
  // cmplwi cr6,r29,499
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 499, so);
  // bgt cr6,0x8212bf4c
  if (cr6.gt) goto loc_8212BF4C;
  // lwz r11,104(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 104);
  // mulli r11,r11,176
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(176))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // addi r11,r11,-48
  r11 = uint64_t(int64_t(int64_t(r11) + -48));
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x8212bf38
  if (!cr6.eq) goto loc_8212BF38;
  // addi r31,r30,128
  r31 = uint64_t(int64_t(int64_t(r30) + 128));
  // b 0x8212bf3c
  goto loc_8212BF3C;
loc_8212BF38:
  // addi r31,r31,176
  r31 = uint64_t(int64_t(int64_t(r31) + 176));
loc_8212BF3C:
  // lwz r11,2776(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 2776);
  // addi r4,r4,1
  r4 = uint64_t(int64_t(int64_t(r4) + 1));
  // cmpw cr6,r4,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r4), int32_t(r11), so);
  // blt cr6,0x8212bbd4
  if (cr6.lt) goto loc_8212BBD4;
loc_8212BF4C:
  // mr r4,r28
  r4 = r28;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2720
  r12 = r12 | 2720;
  // stwx r29,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r29));
  // mr r3,r30
  r3 = r30;
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82126630, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r27
  r4 = r27;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x821292b0
  // (left out: dead in C++)
  { const lifted::Result o = f821292B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r1,0(r1)
  r1 = REX_LOAD_U32(uint32_t(r1) + 0);
  // addi r12,r1,-48
  r12 = uint64_t(int64_t(int64_t(r1) + -48));
  // bl 0x825a7948
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8212C6D0: lifted from generated/ by lift.py.
lifted::Result f8212C6D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8212C6D0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Vec v0{}, v6{}, v7{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{};
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-56
  r12 = uint64_t(int64_t(int64_t(r1) + -56));
  // bl 0x825a7918
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r12,-2
  r12 = uint64_t(int64_t(-131072));
  // ori r12,r12,62704
  r12 = r12 | 62704;
  // bl 0x825a9934
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwux r1,r1,r12
  ea = uint32_t(r1) + uint32_t(r12);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r28,r4
  r28 = r4;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mulli r9,r28,592
  r9 = uint64_t(int64_t(static_cast<int64_t>(r28 * static_cast<uint64_t>(592))));
  // lwz r10,2688(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 2688);
  // rlwinm r10,r10,7,0,24
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // addi r11,r11,-21504
  r11 = uint64_t(int64_t(int64_t(r11) + -21504));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // add r10,r10,r30
  r10 = r10 + r30;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // add r26,r9,r11
  r26 = r9 + r11;
  // addi r31,r10,112
  r31 = uint64_t(int64_t(int64_t(r10) + 112));
  // mr r29,r27
  r29 = r27;
  // bl 0x821298c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821298C0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 104);
  // mr r6,r27
  r6 = r27;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2720
  r12 = r12 | 2720;
  // stwx r27,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r27));
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2724
  r12 = r12 | 2724;
  // stwx r11,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,244(r1)
  REX_STORE_U32(uint32_t(r1) + 244, uint32_t(r11));
  // lwz r11,2696(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 2696);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x8212cac0
  if (!cr6.gt) goto loc_8212CAC0;
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,720
  r12 = r12 | 720;
  // add r7,r1,r12
  r7 = r1 + r12;
  // addi r11,r1,264
  r11 = uint64_t(int64_t(int64_t(r1) + 264));
  // lfs f0,7140(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 7140);
  f0 = double(temp.f32);
  // lfs f13,2608(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2608);
  f13 = double(temp.f32);
  // lfs f31,2612(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f31 = double(temp.f32);
loc_8212C788:
  // lwz r10,96(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 96);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8212ca8c
  if (cr6.eq) goto loc_8212CA8C;
  // addi r9,r26,464
  r9 = uint64_t(int64_t(int64_t(r26) + 464));
  // vspltisw v9,1
  simde_mm_store_si128((simde__m128i*)v9.u32, simde_mm_set1_epi32(int(0x1)));
  // vspltisw v8,0
  simde_mm_store_si128((simde__m128i*)v8.u32, simde_mm_set1_epi32(int(0x0)));
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // vcfsx v10,v9,1
  lifted::VectorFloatMode();
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // lvx128 v0,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmsum3fp128 v13,v0,v0
  simde_mm_store_ps(v13.f32, simde_mm_dp_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v0.f32), 0xEF));
  // vrsqrtefp v12,v13
  simde_mm_store_ps(v12.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v13.f32))));
  // vmulfp128 v7,v13,v10
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v10.f32)));
  // vcmpeqfp v11,v13,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v6,v12,v12
  simde_mm_store_ps(v6.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v10,v7,v6,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v6.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v12,v12,v10,v12
  simde_mm_store_ps(v12.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v12.f32)));
  // vmulfp128 v12,v13,v12
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v12.f32)));
  // vsel v11,v12,v13,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v12.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v13.u8))));
  // stvx v11,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f12,128(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 128);
  f12 = double(temp.f32);
  // fcmpu cr6,f12,f31
  cr6 = lifted::CompareFloat(f12, f31);
  // beq cr6,0x8212c8f4
  if (cr6.eq) goto loc_8212C8F4;
  // stfs f31,96(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // vcfsx v10,v9,1
  lifted::VectorFloatMode();
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // stfs f13,100(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // stfs f31,104(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // stfs f31,108(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f12,100(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f12 = double(temp.f32);
  // stfs f31,92(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // stfs f31,124(r1)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // lvx128 v13,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r10,r1,160
  r10 = uint64_t(int64_t(int64_t(r1) + 160));
  // vpermwi128 v12,v13,135
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x78));
  // vpermwi128 v11,v13,99
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x9C));
  // vpermwi128 v13,v0,99
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x9C));
  // vpermwi128 v0,v0,135
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x78));
  // vmulfp128 v13,v13,v12
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v13,v0,v11,v13
  simde_mm_store_ps(v13.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v11.f32)), simde_mm_load_ps(v13.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmsum3fp128 v0,v13,v13
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v13.f32), 0xEF));
  // stvx v13,r0,r31
  ea = (uint32_t(r31)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vrsqrtefp v12,v0
  simde_mm_store_ps(v12.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v0.f32))));
  // vmulfp128 v7,v0,v10
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v10.f32)));
  // vcmpeqfp v11,v0,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v6,v12,v12
  simde_mm_store_ps(v6.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v10,v7,v6,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v6.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v12,v12,v10,v12
  simde_mm_store_ps(v12.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v12.f32)));
  // vcfsx v10,v9,1
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // vmulfp128 v12,v0,v12
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v12.f32)));
  // vsel v11,v12,v0,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v12.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v0.u8))));
  // stvx v11,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f11,160(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 160);
  f11 = double(temp.f32);
  // fdivs f12,f12,f11
  f12 = double(float(f12 / f11));
  // stfs f12,80(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f12,84(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // stfs f12,88(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lvx128 v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r10,r31,16
  r10 = uint64_t(int64_t(int64_t(r31) + 16));
  // vmulfp128 v0,v13,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v0.f32)));
  // stvx v0,r0,r31
  ea = (uint32_t(r31)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vpermwi128 v12,v0,99
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x9C));
  // lvx128 v13,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vpermwi128 v11,v0,135
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x78));
  // vpermwi128 v0,v13,135
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x78));
  // addi r9,r1,144
  r9 = uint64_t(int64_t(int64_t(r1) + 144));
  // vpermwi128 v13,v13,99
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x9C));
  // vmulfp128 v0,v12,v0
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v0.f32)));
  // vnmsubfp v0,v11,v13,v0
  simde_mm_store_ps(v0.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v13.f32)), simde_mm_load_ps(v0.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvx128 v12,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmsum3fp128 v0,v12,v12
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32), 0xEF));
  // lfs f12,100(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f12 = double(temp.f32);
  // vrsqrtefp v13,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v0.f32))));
  // vcmpeqfp v11,v0,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v9,v0,v10
  simde_mm_store_ps(v9.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v10.f32)));
  // vmulfp128 v8,v13,v13
  simde_mm_store_ps(v8.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v13.f32)));
  // vnmsubfp v10,v9,v8,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v9.f32), simde_mm_load_ps(v8.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v13,v13,v10,v13
  simde_mm_store_ps(v13.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v13.f32)));
  // vmulfp128 v13,v0,v13
  simde_mm_store_ps(v13.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v13.f32)));
  // vsel v11,v13,v0,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v13.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v0.u8))));
  // stvx v11,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f11,144(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 144);
  f11 = double(temp.f32);
  // fdivs f12,f12,f11
  f12 = double(float(f12 / f11));
  // stfs f12,112(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 112, temp.u32);
  // stfs f12,116(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // stfs f12,120(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + 120, temp.u32);
  // addi r9,r1,112
  r9 = uint64_t(int64_t(int64_t(r1) + 112));
  // lvx128 v0,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmulfp128 v0,v12,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v0.f32)));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_8212C8F4:
  // lfs f12,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f12 = double(temp.f32);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // lfs f11,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f11 = double(temp.f32);
  // fmuls f12,f12,f0
  f12 = double(float(f12 * f0));
  // fmuls f11,f11,f0
  f11 = double(float(f11 * f0));
  // lfs f10,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f10 = double(temp.f32);
  // lfs f9,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f9 = double(temp.f32);
  // fmuls f10,f10,f0
  f10 = double(float(f10 * f0));
  // lfs f8,48(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f8 = double(temp.f32);
  // fmuls f9,f9,f0
  f9 = double(float(f9 * f0));
  // lfs f7,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f7 = double(temp.f32);
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // lfs f5,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f5 = double(temp.f32);
  // cmplwi cr6,r29,499
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 499, so);
  // fmuls f5,f5,f0
  f5 = double(float(f5 * f0));
  // lfs f3,48(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f3 = double(temp.f32);
  // lfs f6,56(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 56);
  f6 = double(temp.f32);
  // lfs f4,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f4 = double(temp.f32);
  // lfs f30,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f30 = double(temp.f32);
  // fmuls f4,f4,f0
  f4 = double(float(f4 * f0));
  // fsubs f8,f8,f12
  f8 = double(float(f8 - f12));
  // lfs f2,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f2 = double(temp.f32);
  // fsubs f7,f7,f11
  f7 = double(float(f7 - f11));
  // lfs f29,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f29 = double(temp.f32);
  // fadds f3,f3,f12
  f3 = double(float(f3 + f12));
  // lfs f12,56(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 56);
  f12 = double(temp.f32);
  // fadds f1,f12,f10
  f1 = double(float(f12 + f10));
  // lfs f28,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f28 = double(temp.f32);
  // fsubs f6,f6,f10
  f6 = double(float(f6 - f10));
  // stfs f13,4(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // fadds f2,f2,f11
  f2 = double(float(f2 + f11));
  // fsubs f12,f8,f9
  f12 = double(float(f8 - f9));
  // stfs f12,-8(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + -8, temp.u32);
  // fmuls f9,f30,f0
  f9 = double(float(f30 * f0));
  // fsubs f11,f7,f5
  f11 = double(float(f7 - f5));
  // stfs f11,-4(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // fmuls f8,f29,f0
  f8 = double(float(f29 * f0));
  // fmuls f7,f28,f0
  f7 = double(float(f28 * f0));
  // fmr f5,f30
  f5 = f30;
  // fsubs f10,f6,f4
  f10 = double(float(f6 - f4));
  // stfs f10,0(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fmr f4,f28
  f4 = f28;
  // stfs f13,20(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // fmr f6,f29
  f6 = f29;
  // fsubs f9,f3,f9
  f9 = double(float(f3 - f9));
  // stfs f9,8(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // fmr f3,f28
  f3 = f28;
  // fsubs f8,f2,f8
  f8 = double(float(f2 - f8));
  // stfs f8,12(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // fsubs f7,f1,f7
  f7 = double(float(f1 - f7));
  // stfs f7,16(r11)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // fadds f12,f12,f5
  f12 = double(float(f12 + f5));
  // stfs f12,24(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // stfs f13,36(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 36, temp.u32);
  // fadds f12,f4,f10
  f12 = double(float(f4 + f10));
  // stfs f12,32(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // fadds f11,f6,f11
  f11 = double(float(f6 + f11));
  // stfs f11,28(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f13,52(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 52, temp.u32);
  // fadds f10,f30,f9
  f10 = double(float(f30 + f9));
  // stfs f10,40(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 40, temp.u32);
  // fadds f9,f29,f8
  f9 = double(float(f29 + f8));
  // stfs f9,44(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // fadds f8,f3,f7
  f8 = double(float(f3 + f7));
  // stfs f8,48(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 48, temp.u32);
  // lwz r9,80(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r9,31992(r11)
  REX_STORE_U32(uint32_t(r11) + 31992, uint32_t(r9));
  // lwz r9,84(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r9,31996(r11)
  REX_STORE_U32(uint32_t(r11) + 31996, uint32_t(r9));
  // lwz r9,88(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r9,32000(r11)
  REX_STORE_U32(uint32_t(r11) + 32000, uint32_t(r9));
  // lwz r9,92(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r9,32004(r11)
  REX_STORE_U32(uint32_t(r11) + 32004, uint32_t(r9));
  // lwz r9,80(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r9,32008(r11)
  REX_STORE_U32(uint32_t(r11) + 32008, uint32_t(r9));
  // lwz r9,84(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r9,32012(r11)
  REX_STORE_U32(uint32_t(r11) + 32012, uint32_t(r9));
  // lwz r9,88(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r9,32016(r11)
  REX_STORE_U32(uint32_t(r11) + 32016, uint32_t(r9));
  // lwz r9,92(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r9,32020(r11)
  REX_STORE_U32(uint32_t(r11) + 32020, uint32_t(r9));
  // lwz r9,80(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r9,32024(r11)
  REX_STORE_U32(uint32_t(r11) + 32024, uint32_t(r9));
  // lwz r9,84(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r9,32028(r11)
  REX_STORE_U32(uint32_t(r11) + 32028, uint32_t(r9));
  // lwz r9,88(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r9,32032(r11)
  REX_STORE_U32(uint32_t(r11) + 32032, uint32_t(r9));
  // lwz r9,92(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r9,32036(r11)
  REX_STORE_U32(uint32_t(r11) + 32036, uint32_t(r9));
  // lwz r9,80(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r9,32040(r11)
  REX_STORE_U32(uint32_t(r11) + 32040, uint32_t(r9));
  // lwz r9,84(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r9,32044(r11)
  REX_STORE_U32(uint32_t(r11) + 32044, uint32_t(r9));
  // lwz r9,88(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r9,32048(r11)
  REX_STORE_U32(uint32_t(r11) + 32048, uint32_t(r9));
  // lwz r9,92(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r9,32052(r11)
  REX_STORE_U32(uint32_t(r11) + 32052, uint32_t(r9));
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // stw r10,-2000(r7)
  REX_STORE_U32(uint32_t(r7) + -2000, uint32_t(r10));
  // stw r27,0(r7)
  REX_STORE_U32(uint32_t(r7) + 0, uint32_t(r27));
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // bgt cr6,0x8212cac0
  if (cr6.gt) goto loc_8212CAC0;
loc_8212CA8C:
  // lwz r10,2700(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 2700);
  // rlwinm r10,r10,7,0,24
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 7) & 0xFFFFFF80;
  // add r10,r10,r30
  r10 = r10 + r30;
  // addi r10,r10,-16
  r10 = uint64_t(int64_t(int64_t(r10) + -16));
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // bne cr6,0x8212caac
  if (!cr6.eq) goto loc_8212CAAC;
  // addi r31,r30,112
  r31 = uint64_t(int64_t(int64_t(r30) + 112));
  // b 0x8212cab0
  goto loc_8212CAB0;
loc_8212CAAC:
  // addi r31,r31,128
  r31 = uint64_t(int64_t(int64_t(r31) + 128));
loc_8212CAB0:
  // lwz r10,2696(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 2696);
  // addi r6,r6,1
  r6 = uint64_t(int64_t(int64_t(r6) + 1));
  // cmpw cr6,r6,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r6), int32_t(r10), so);
  // blt cr6,0x8212c788
  if (cr6.lt) goto loc_8212C788;
loc_8212CAC0:
  // mr r4,r28
  r4 = r28;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2720
  r12 = r12 | 2720;
  // stwx r29,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r29));
  // mr r3,r30
  r3 = r30;
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82126630, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r26
  r4 = r26;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x821292b0
  // (left out: dead in C++)
  { const lifted::Result o = f821292B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r1,0(r1)
  r1 = REX_LOAD_U32(uint32_t(r1) + 0);
  // addi r12,r1,-56
  r12 = uint64_t(int64_t(int64_t(r1) + -56));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8212DFB8: lifted from generated/ by lift.py.
lifted::Result f8212DFB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_8212DFB8 (lifted)");
  uint64_t r5 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f22 = 0.0, f23 = 0.0, f24 = 0.0, f25 = 0.0, f26 = 0.0, f27 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Vec v0{}, v6{}, v7{}, v8{}, v9{}, v10{}, v11{}, v12{}, v13{};
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-72
  r12 = uint64_t(int64_t(int64_t(r1) + -72));
  // bl 0x825a7900
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r12,-3
  r12 = uint64_t(int64_t(-196608));
  // ori r12,r12,60080
  r12 = r12 | 60080;
  // bl 0x825a9934
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwux r1,r1,r12
  ea = uint32_t(r1) + uint32_t(r12);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r26,r4
  r26 = r4;
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mulli r9,r26,592
  r9 = uint64_t(int64_t(static_cast<int64_t>(r26 * static_cast<uint64_t>(592))));
  // lwz r11,28932(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 28932);
  // mulli r11,r11,144
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(144))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // addi r10,r10,-21504
  r10 = uint64_t(int64_t(int64_t(r10) + -21504));
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // addi r29,r11,128
  r29 = uint64_t(int64_t(int64_t(r11) + 128));
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // add r24,r9,r10
  r24 = r9 + r10;
  // mr r28,r27
  r28 = r27;
  // mr r31,r29
  r31 = r29;
  // bl 0x821298c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821298C0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,28940(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 28940);
  // mr r7,r27
  r7 = r27;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // li r25,4
  r25 = uint64_t(int64_t(4));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmr f13,f0
  f13 = f0;
  // fmr f12,f0
  f12 = f0;
  // fmr f11,f0
  f11 = f0;
  // fmr f10,f0
  f10 = f0;
  // lfs f30,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f30 = double(temp.f32);
  // lwz r11,108(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 108);
  // fmr f29,f30
  f29 = f30;
  // fmr f9,f0
  f9 = f0;
  // fmr f8,f0
  f8 = f0;
  // fmr f7,f0
  f7 = f0;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // fmr f6,f0
  f6 = f0;
  // fmr f5,f0
  f5 = f0;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2720
  r12 = r12 | 2720;
  // stwx r27,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r27));
  // fmr f28,f30
  f28 = f30;
  // fmr f4,f0
  f4 = f0;
  // fmr f3,f0
  f3 = f0;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2724
  r12 = r12 | 2724;
  // stwx r11,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // fmr f2,f0
  f2 = f0;
  // stw r11,244(r1)
  REX_STORE_U32(uint32_t(r1) + 244, uint32_t(r11));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,7140(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7140);
  f31 = double(temp.f32);
  // ble cr6,0x8212e46c
  if (!cr6.gt) goto loc_8212E46C;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,720
  r12 = r12 | 720;
  // add r8,r1,r12
  r8 = r1 + r12;
  // addi r11,r1,264
  r11 = uint64_t(int64_t(int64_t(r1) + 264));
loc_8212E0B0:
  // lwz r10,96(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 96);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x8212e438
  if (cr6.eq) goto loc_8212E438;
  // addi r9,r24,464
  r9 = uint64_t(int64_t(int64_t(r24) + 464));
  // vspltisw v9,1
  simde_mm_store_si128((simde__m128i*)v9.u32, simde_mm_set1_epi32(int(0x1)));
  // vspltisw v8,0
  simde_mm_store_si128((simde__m128i*)v8.u32, simde_mm_set1_epi32(int(0x0)));
  // addi r10,r1,128
  r10 = uint64_t(int64_t(int64_t(r1) + 128));
  // vcfsx v10,v9,1
  lifted::VectorFloatMode();
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // lvx128 v0,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmsum3fp128 v13,v0,v0
  simde_mm_store_ps(v13.f32, simde_mm_dp_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v0.f32), 0xEF));
  // vrsqrtefp v12,v13
  simde_mm_store_ps(v12.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v13.f32))));
  // vmulfp128 v7,v13,v10
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v10.f32)));
  // vcmpeqfp v11,v13,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v6,v12,v12
  simde_mm_store_ps(v6.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v10,v7,v6,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v6.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v12,v12,v10,v12
  simde_mm_store_ps(v12.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v12.f32)));
  // vmulfp128 v12,v13,v12
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v12.f32)));
  // vsel v11,v12,v13,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v12.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v13.u8))));
  // stvx v11,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f1,128(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 128);
  f1 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // beq cr6,0x8212e21c
  if (cr6.eq) goto loc_8212E21C;
  // stfs f0,96(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 96, temp.u32);
  // vcfsx v10,v9,1
  lifted::VectorFloatMode();
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // stfs f30,100(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r1) + 100, temp.u32);
  // stfs f0,104(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 104, temp.u32);
  // stfs f0,108(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 108, temp.u32);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f1,100(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f1 = double(temp.f32);
  // stfs f0,92(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 92, temp.u32);
  // stfs f0,124(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 124, temp.u32);
  // lvx128 v13,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r10,r1,160
  r10 = uint64_t(int64_t(int64_t(r1) + 160));
  // vpermwi128 v12,v13,135
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x78));
  // vpermwi128 v11,v13,99
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x9C));
  // vpermwi128 v13,v0,99
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x9C));
  // vpermwi128 v0,v0,135
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x78));
  // vmulfp128 v13,v13,v12
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v13,v0,v11,v13
  simde_mm_store_ps(v13.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v11.f32)), simde_mm_load_ps(v13.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmsum3fp128 v0,v13,v13
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v13.f32), 0xEF));
  // stvx v13,r0,r31
  ea = (uint32_t(r31)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v13.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vrsqrtefp v12,v0
  simde_mm_store_ps(v12.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v0.f32))));
  // vmulfp128 v7,v0,v10
  simde_mm_store_ps(v7.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v10.f32)));
  // vcmpeqfp v11,v0,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v6,v12,v12
  simde_mm_store_ps(v6.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32)));
  // vnmsubfp v10,v7,v6,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v7.f32), simde_mm_load_ps(v6.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v12,v12,v10,v12
  simde_mm_store_ps(v12.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v12.f32)));
  // vcfsx v10,v9,1
  simde_mm_store_ps(v10.f32, simde_mm_mul_ps(simde_mm_cvtepi32_ps(simde_mm_load_si128((simde__m128i*)v9.u32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x3F000000)))));
  // vmulfp128 v12,v0,v12
  simde_mm_store_ps(v12.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v12.f32)));
  // vsel v11,v12,v0,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v12.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v0.u8))));
  // stvx v11,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f27,160(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 160);
  f27 = double(temp.f32);
  // fdivs f1,f1,f27
  f1 = double(float(f1 / f27));
  // stfs f1,80(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 80, temp.u32);
  // stfs f1,84(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // stfs f1,88(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 88, temp.u32);
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // lvx128 v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // addi r10,r31,16
  r10 = uint64_t(int64_t(int64_t(r31) + 16));
  // vmulfp128 v0,v13,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v0.f32)));
  // stvx v0,r0,r31
  ea = (uint32_t(r31)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vpermwi128 v12,v0,99
  simde_mm_store_si128((simde__m128i*)v12.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x9C));
  // lvx128 v13,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v13.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vpermwi128 v11,v0,135
  simde_mm_store_si128((simde__m128i*)v11.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v0.u32), 0x78));
  // vpermwi128 v0,v13,135
  simde_mm_store_si128((simde__m128i*)v0.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x78));
  // addi r9,r1,144
  r9 = uint64_t(int64_t(int64_t(r1) + 144));
  // vpermwi128 v13,v13,99
  simde_mm_store_si128((simde__m128i*)v13.u32, simde_mm_shuffle_epi32(simde_mm_load_si128((simde__m128i*)v13.u32), 0x9C));
  // vmulfp128 v0,v12,v0
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v0.f32)));
  // vnmsubfp v0,v11,v13,v0
  simde_mm_store_ps(v0.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v11.f32), simde_mm_load_ps(v13.f32)), simde_mm_load_ps(v0.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lvx128 v12,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v12.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmsum3fp128 v0,v12,v12
  simde_mm_store_ps(v0.f32, simde_mm_dp_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v12.f32), 0xEF));
  // lfs f1,100(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 100);
  f1 = double(temp.f32);
  // vrsqrtefp v13,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v13.f32, simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(simde_mm_load_ps(v0.f32))));
  // vcmpeqfp v11,v0,v8
  simde_mm_store_ps(v11.f32, simde_mm_cmpeq_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v8.f32)));
  // vmulfp128 v9,v0,v10
  simde_mm_store_ps(v9.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v10.f32)));
  // vmulfp128 v8,v13,v13
  simde_mm_store_ps(v8.f32, simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v13.f32)));
  // vnmsubfp v10,v9,v8,v10
  simde_mm_store_ps(v10.f32, simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(simde_mm_load_ps(v9.f32), simde_mm_load_ps(v8.f32)), simde_mm_load_ps(v10.f32)), simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)))));
  // vmaddfp v13,v13,v10,v13
  simde_mm_store_ps(v13.f32, simde_mm_add_ps(simde_mm_mul_ps(simde_mm_load_ps(v13.f32), simde_mm_load_ps(v10.f32)), simde_mm_load_ps(v13.f32)));
  // vmulfp128 v13,v0,v13
  simde_mm_store_ps(v13.f32, simde_mm_mul_ps(simde_mm_load_ps(v0.f32), simde_mm_load_ps(v13.f32)));
  // vsel v11,v13,v0,v11
  simde_mm_store_si128((simde__m128i*)v11.u8, simde_mm_or_si128(simde_mm_andnot_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v13.u8)), simde_mm_and_si128(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)v0.u8))));
  // stvx v11,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v11.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // lfs f27,144(r1)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + 144);
  f27 = double(temp.f32);
  // fdivs f1,f1,f27
  f1 = double(float(f1 / f27));
  // stfs f1,112(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 112, temp.u32);
  // stfs f1,116(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 116, temp.u32);
  // stfs f1,120(r1)
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r1) + 120, temp.u32);
  // addi r9,r1,112
  r9 = uint64_t(int64_t(int64_t(r1) + 112));
  // lvx128 v0,r0,r9
  ea = (uint32_t(r9)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)v0.u8, simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)REX_RAW_ADDR(ea)), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
  // vmulfp128 v0,v12,v0
  lifted::VectorFloatMode();
  simde_mm_store_ps(v0.f32, simde_mm_mul_ps(simde_mm_load_ps(v12.f32), simde_mm_load_ps(v0.f32)));
  // stvx v0,r0,r10
  ea = (uint32_t(r10)) & ~0xF;
  simde_mm_store_si128((simde__m128i*)REX_RAW_ADDR(ea), simde_mm_shuffle_epi8(simde_mm_load_si128((simde__m128i*)v0.u8), simde_mm_load_si128((simde__m128i*)VectorMaskL)));
loc_8212E21C:
  // lwz r10,128(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 128);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x8212e29c
  if (cr6.lt) goto loc_8212E29C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x8212e35c
  if (!cr6.lt) goto loc_8212E35C;
  // lfs f7,48(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f7 = double(temp.f32);
  // lfs f6,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f6 = double(temp.f32);
  // lfs f10,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f10 = double(temp.f32);
  // lfs f9,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f9 = double(temp.f32);
  // fadds f10,f10,f7
  f10 = double(float(f10 + f7));
  // fadds f9,f6,f9
  f9 = double(float(f6 + f9));
  // lfs f13,0(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // lfs f7,56(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 56);
  f7 = double(temp.f32);
  // lfs f6,64(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 64);
  f6 = double(temp.f32);
  // lfs f8,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f8 = double(temp.f32);
  // fadds f4,f13,f6
  f4 = double(float(f13 + f6));
  // fadds f8,f7,f8
  f8 = double(float(f7 + f8));
  // lfs f12,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // lfs f11,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f11 = double(temp.f32);
  // lfs f7,68(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 68);
  f7 = double(temp.f32);
  // lfs f6,72(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 72);
  f6 = double(temp.f32);
  // fadds f3,f7,f12
  f3 = double(float(f7 + f12));
  // fadds f2,f6,f11
  f2 = double(float(f6 + f11));
  // lfs f13,48(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f13 = double(temp.f32);
  // lfs f12,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f12 = double(temp.f32);
  // lfs f11,56(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 56);
  f11 = double(temp.f32);
  // lfs f29,60(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 60);
  f29 = double(temp.f32);
  // lfs f7,64(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 64);
  f7 = double(temp.f32);
  // lfs f6,68(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 68);
  f6 = double(temp.f32);
  // lfs f5,72(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 72);
  f5 = double(temp.f32);
  // lfs f28,76(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 76);
  f28 = double(temp.f32);
  // b 0x8212e35c
  goto loc_8212E35C;
loc_8212E29C:
  // lfs f13,0(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 0);
  f13 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // lfs f11,8(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 8);
  f11 = double(temp.f32);
  // lfs f12,4(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 4);
  f12 = double(temp.f32);
  // fmuls f11,f11,f31
  f11 = double(float(f11 * f31));
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // lfs f9,48(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f9 = double(temp.f32);
  // lfs f6,48(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 48);
  f6 = double(temp.f32);
  // lfs f8,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f8 = double(temp.f32);
  // lfs f7,56(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 56);
  f7 = double(temp.f32);
  // lfs f5,52(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 52);
  f5 = double(temp.f32);
  // lfs f10,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f10 = double(temp.f32);
  // lfs f3,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f3 = double(temp.f32);
  // fmuls f10,f10,f31
  f10 = double(float(f10 * f31));
  // lfs f1,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f1 = double(temp.f32);
  // fmuls f3,f3,f31
  f3 = double(float(f3 * f31));
  // fadds f6,f13,f6
  f6 = double(float(f13 + f6));
  // lfs f27,16(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 16);
  f27 = double(temp.f32);
  // fsubs f9,f9,f13
  f9 = double(float(f9 - f13));
  // lfs f13,56(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 56);
  f13 = double(temp.f32);
  // fadds f4,f13,f11
  f4 = double(float(f13 + f11));
  // lfs f13,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f13 = double(temp.f32);
  // fadds f5,f5,f12
  f5 = double(float(f5 + f12));
  // lfs f25,24(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 24);
  f25 = double(temp.f32);
  // fsubs f8,f8,f12
  f8 = double(float(f8 - f12));
  // lfs f12,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f12 = double(temp.f32);
  // fsubs f7,f7,f11
  f7 = double(float(f7 - f11));
  // fmuls f11,f13,f31
  f11 = double(float(f13 * f31));
  // lfs f13,20(r31)
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 20);
  f13 = double(temp.f32);
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // fmuls f2,f13,f31
  f2 = double(float(f13 * f31));
  // fmr f26,f13
  f26 = f13;
  // fmuls f1,f1,f31
  f1 = double(float(f1 * f31));
  // fsubs f13,f9,f10
  f13 = double(float(f9 - f10));
  // fsubs f10,f6,f3
  f10 = double(float(f6 - f3));
  // fmr f24,f27
  f24 = f27;
  // fmr f22,f25
  f22 = f25;
  // fsubs f11,f7,f11
  f11 = double(float(f7 - f11));
  // fsubs f12,f8,f12
  f12 = double(float(f8 - f12));
  // fsubs f9,f5,f2
  f9 = double(float(f5 - f2));
  // fmr f23,f26
  f23 = f26;
  // fsubs f8,f4,f1
  f8 = double(float(f4 - f1));
  // fadds f7,f13,f27
  f7 = double(float(f13 + f27));
  // fadds f4,f10,f24
  f4 = double(float(f10 + f24));
  // fadds f5,f25,f11
  f5 = double(float(f25 + f11));
  // fadds f6,f26,f12
  f6 = double(float(f26 + f12));
  // fadds f3,f23,f9
  f3 = double(float(f23 + f9));
  // fadds f2,f22,f8
  f2 = double(float(f22 + f8));
loc_8212E35C:
  // stfs f13,-8(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + -8, temp.u32);
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // stfs f12,-4(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // stfs f11,0(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // cmplwi cr6,r28,499
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 499, so);
  // stfs f29,4(r11)
  temp.f32 = float(f29);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // stfs f10,8(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // stfs f9,12(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f8,16(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // stfs f30,20(r11)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // stfs f7,24(r11)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // stfs f6,28(r11)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f5,32(r11)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // stfs f28,36(r11)
  temp.f32 = float(f28);
  REX_STORE_U32(uint32_t(r11) + 36, temp.u32);
  // stfs f4,40(r11)
  temp.f32 = float(f4);
  REX_STORE_U32(uint32_t(r11) + 40, temp.u32);
  // stfs f3,44(r11)
  temp.f32 = float(f3);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // stfs f2,48(r11)
  temp.f32 = float(f2);
  REX_STORE_U32(uint32_t(r11) + 48, temp.u32);
  // stfs f30,52(r11)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r11) + 52, temp.u32);
  // lwz r10,80(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r10,31992(r11)
  REX_STORE_U32(uint32_t(r11) + 31992, uint32_t(r10));
  // lwz r10,84(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r10,31996(r11)
  REX_STORE_U32(uint32_t(r11) + 31996, uint32_t(r10));
  // lwz r10,88(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r10,32000(r11)
  REX_STORE_U32(uint32_t(r11) + 32000, uint32_t(r10));
  // lwz r10,92(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r10,32004(r11)
  REX_STORE_U32(uint32_t(r11) + 32004, uint32_t(r10));
  // lwz r10,80(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r10,32008(r11)
  REX_STORE_U32(uint32_t(r11) + 32008, uint32_t(r10));
  // lwz r10,84(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r10,32012(r11)
  REX_STORE_U32(uint32_t(r11) + 32012, uint32_t(r10));
  // lwz r10,88(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r10,32016(r11)
  REX_STORE_U32(uint32_t(r11) + 32016, uint32_t(r10));
  // lwz r10,92(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r10,32020(r11)
  REX_STORE_U32(uint32_t(r11) + 32020, uint32_t(r10));
  // lwz r10,80(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r10,32024(r11)
  REX_STORE_U32(uint32_t(r11) + 32024, uint32_t(r10));
  // lwz r10,84(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r10,32028(r11)
  REX_STORE_U32(uint32_t(r11) + 32028, uint32_t(r10));
  // lwz r10,88(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r10,32032(r11)
  REX_STORE_U32(uint32_t(r11) + 32032, uint32_t(r10));
  // lwz r10,92(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r10,32036(r11)
  REX_STORE_U32(uint32_t(r11) + 32036, uint32_t(r10));
  // lwz r10,80(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 80);
  // stw r10,32040(r11)
  REX_STORE_U32(uint32_t(r11) + 32040, uint32_t(r10));
  // lwz r10,84(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 84);
  // stw r10,32044(r11)
  REX_STORE_U32(uint32_t(r11) + 32044, uint32_t(r10));
  // lwz r10,88(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 88);
  // stw r10,32048(r11)
  REX_STORE_U32(uint32_t(r11) + 32048, uint32_t(r10));
  // lwz r10,92(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 92);
  // stw r10,32052(r11)
  REX_STORE_U32(uint32_t(r11) + 32052, uint32_t(r10));
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // stw r25,-2000(r8)
  REX_STORE_U32(uint32_t(r8) + -2000, uint32_t(r25));
  // stw r27,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r27));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // bgt cr6,0x8212e46c
  if (cr6.gt) goto loc_8212E46C;
loc_8212E438:
  // lwz r10,104(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 104);
  // mulli r10,r10,144
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(144))));
  // add r10,r10,r30
  r10 = r10 + r30;
  // addi r10,r10,-16
  r10 = uint64_t(int64_t(int64_t(r10) + -16));
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // bne cr6,0x8212e458
  if (!cr6.eq) goto loc_8212E458;
  // addi r31,r30,128
  r31 = uint64_t(int64_t(int64_t(r30) + 128));
  // b 0x8212e45c
  goto loc_8212E45C;
loc_8212E458:
  // addi r31,r31,144
  r31 = uint64_t(int64_t(int64_t(r31) + 144));
loc_8212E45C:
  // lwz r10,28940(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 28940);
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // cmpw cr6,r7,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r10), so);
  // blt cr6,0x8212e0b0
  if (cr6.lt) goto loc_8212E0B0;
loc_8212E46C:
  // mr r4,r26
  r4 = r26;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2720
  r12 = r12 | 2720;
  // stwx r28,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r28));
  // mr r3,r30
  r3 = r30;
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82126630, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r24
  r4 = r24;
  // addi r3,r1,176
  r3 = uint64_t(int64_t(int64_t(r1) + 176));
  // bl 0x821292b0
  // (left out: dead in C++)
  { const lifted::Result o = f821292B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,28976(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 28976);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8212e650
  if (cr6.eq) goto loc_8212E650;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2736
  r12 = r12 | 2736;
  // add r3,r1,r12
  r3 = r1 + r12;
  // mr r31,r27
  r31 = r27;
  // bl 0x821298c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821298C0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,108(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 108);
  // lfs f4,29008(r30)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 29008);
  f4 = double(temp.f32);
  // mr r5,r27
  r5 = r27;
  // lfs f3,29012(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 29012);
  f3 = double(temp.f32);
  // lis r12,2
  r12 = uint64_t(int64_t(131072));
  // ori r12,r12,3280
  r12 = r12 | 3280;
  // add r7,r1,r12
  r7 = r1 + r12;
  // lfs f2,29016(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 29016);
  f2 = double(temp.f32);
  // addi r9,r30,29032
  r9 = uint64_t(int64_t(int64_t(r30) + 29032));
  // li r10,255
  r10 = uint64_t(int64_t(255));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r8,200
  r8 = uint64_t(int64_t(200));
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2804
  r12 = r12 | 2804;
  // stwx r25,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r25));
  // li r6,150
  r6 = uint64_t(int64_t(150));
  // lis r12,2
  r12 = uint64_t(int64_t(131072));
  // ori r12,r12,5280
  r12 = r12 | 5280;
  // stwx r27,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r27));
  // lis r12,2
  r12 = uint64_t(int64_t(131072));
  // ori r12,r12,5284
  r12 = r12 | 5284;
  // stwx r11,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r11));
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2824
  r12 = r12 | 2824;
  // add r11,r1,r12
  r11 = r1 + r12;
loc_8212E518:
  // lfs f0,0(r29)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 0);
  f0 = double(temp.f32);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // lfs f13,4(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 4);
  f13 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // lfs f12,8(r29)
  temp.u32 = REX_LOAD_U32(uint32_t(r29) + 8);
  f12 = double(temp.f32);
  // fmuls f13,f13,f31
  f13 = double(float(f13 * f31));
  // fmuls f12,f12,f31
  f12 = double(float(f12 * f31));
  // lfs f11,29008(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 29008);
  f11 = double(temp.f32);
  // lfs f10,29012(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 29012);
  f10 = double(temp.f32);
  // cmplwi cr6,r31,499
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 499, so);
  // lfs f9,29016(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 29016);
  f9 = double(temp.f32);
  // lfs f1,-8(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -8);
  f1 = double(temp.f32);
  // lfs f29,-4(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + -4);
  f29 = double(temp.f32);
  // lfs f28,0(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f28 = double(temp.f32);
  // lfs f5,29020(r30)
  temp.u32 = REX_LOAD_U32(uint32_t(r30) + 29020);
  f5 = double(temp.f32);
  // stfs f11,-8(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + -8, temp.u32);
  // fadds f11,f1,f11
  f11 = double(float(f1 + f11));
  // fadds f8,f4,f0
  f8 = double(float(f4 + f0));
  // stfs f10,-4(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + -4, temp.u32);
  // fadds f7,f3,f13
  f7 = double(float(f3 + f13));
  // stfs f9,0(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // fadds f6,f2,f12
  f6 = double(float(f2 + f12));
  // stfs f5,4(r11)
  temp.f32 = float(f5);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // stfs f30,20(r11)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r11) + 20, temp.u32);
  // fadds f10,f29,f10
  f10 = double(float(f29 + f10));
  // stfs f8,8(r11)
  temp.f32 = float(f8);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // fadds f9,f9,f28
  f9 = double(float(f9 + f28));
  // stfs f7,12(r11)
  temp.f32 = float(f7);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // stfs f6,16(r11)
  temp.f32 = float(f6);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // stfs f30,36(r11)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r11) + 36, temp.u32);
  // stfs f11,24(r11)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r11) + 24, temp.u32);
  // stfs f10,28(r11)
  temp.f32 = float(f10);
  REX_STORE_U32(uint32_t(r11) + 28, temp.u32);
  // stfs f9,32(r11)
  temp.f32 = float(f9);
  REX_STORE_U32(uint32_t(r11) + 32, temp.u32);
  // fadds f0,f0,f8
  f0 = double(float(f0 + f8));
  // fadds f13,f13,f7
  f13 = double(float(f13 + f7));
  // stfs f30,52(r11)
  temp.f32 = float(f30);
  REX_STORE_U32(uint32_t(r11) + 52, temp.u32);
  // fadds f12,f12,f6
  f12 = double(float(f12 + f6));
  // stfs f0,40(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 40, temp.u32);
  // stfs f13,44(r11)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r11) + 44, temp.u32);
  // stfs f12,48(r11)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r11) + 48, temp.u32);
  // stw r10,31992(r11)
  REX_STORE_U32(uint32_t(r11) + 31992, uint32_t(r10));
  // stw r10,31996(r11)
  REX_STORE_U32(uint32_t(r11) + 31996, uint32_t(r10));
  // stw r10,32000(r11)
  REX_STORE_U32(uint32_t(r11) + 32000, uint32_t(r10));
  // stw r10,32004(r11)
  REX_STORE_U32(uint32_t(r11) + 32004, uint32_t(r10));
  // stw r10,32008(r11)
  REX_STORE_U32(uint32_t(r11) + 32008, uint32_t(r10));
  // stw r10,32012(r11)
  REX_STORE_U32(uint32_t(r11) + 32012, uint32_t(r10));
  // stw r10,32016(r11)
  REX_STORE_U32(uint32_t(r11) + 32016, uint32_t(r10));
  // stw r10,32020(r11)
  REX_STORE_U32(uint32_t(r11) + 32020, uint32_t(r10));
  // stw r10,32024(r11)
  REX_STORE_U32(uint32_t(r11) + 32024, uint32_t(r10));
  // stw r8,32028(r11)
  REX_STORE_U32(uint32_t(r11) + 32028, uint32_t(r8));
  // stw r8,32032(r11)
  REX_STORE_U32(uint32_t(r11) + 32032, uint32_t(r8));
  // stw r6,32036(r11)
  REX_STORE_U32(uint32_t(r11) + 32036, uint32_t(r6));
  // stw r10,32040(r11)
  REX_STORE_U32(uint32_t(r11) + 32040, uint32_t(r10));
  // stw r8,32044(r11)
  REX_STORE_U32(uint32_t(r11) + 32044, uint32_t(r8));
  // stw r8,32048(r11)
  REX_STORE_U32(uint32_t(r11) + 32048, uint32_t(r8));
  // stw r6,32052(r11)
  REX_STORE_U32(uint32_t(r11) + 32052, uint32_t(r6));
  // addi r11,r11,64
  r11 = uint64_t(int64_t(int64_t(r11) + 64));
  // stw r25,-2000(r7)
  REX_STORE_U32(uint32_t(r7) + -2000, uint32_t(r25));
  // lwz r4,132(r29)
  r4 = REX_LOAD_U32(uint32_t(r29) + 132);
  // stw r4,0(r7)
  REX_STORE_U32(uint32_t(r7) + 0, uint32_t(r4));
  // bgt cr6,0x8212e620
  if (cr6.gt) goto loc_8212E620;
  // addi r5,r5,1
  r5 = uint64_t(int64_t(int64_t(r5) + 1));
  // addi r9,r9,16
  r9 = uint64_t(int64_t(int64_t(r9) + 16));
  // addi r7,r7,4
  r7 = uint64_t(int64_t(int64_t(r7) + 4));
  // cmpwi cr6,r5,50
  cr6 = lifted::Compare<int32_t>(int32_t(r5), 50, so);
  // blt cr6,0x8212e518
  if (cr6.lt) goto loc_8212E518;
loc_8212E620:
  // mr r4,r26
  r4 = r26;
  // lis r12,2
  r12 = uint64_t(int64_t(131072));
  // ori r12,r12,5280
  r12 = r12 | 5280;
  // stwx r31,r1,r12
  REX_STORE_U32(uint32_t(r1) + uint32_t(r12), uint32_t(r31));
  // mr r3,r30
  r3 = r30;
  // bl 0x82126630
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82126630, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r5,r3
  r5 = r3;
  // mr r4,r24
  r4 = r24;
  // lis r12,1
  r12 = uint64_t(int64_t(65536));
  // ori r12,r12,2736
  r12 = r12 | 2736;
  // add r3,r1,r12
  r3 = r1 + r12;
  // bl 0x821292b0
  // (left out: dead in C++)
  { const lifted::Result o = f821292B0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_8212E650:
  // lwz r1,0(r1)
  r1 = REX_LOAD_U32(uint32_t(r1) + 0);
  // addi r12,r1,-72
  r12 = uint64_t(int64_t(int64_t(r1) + -72));
  // bl 0x825a794c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8246AEE0: lifted from generated/ by lift.py.
lifted::Result f8246AEE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_8246AEE0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e78
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r31,r1,-160
  r31 = uint64_t(int64_t(int64_t(r1) + -160));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r9,r3
  r9 = r3;
  // lbz r3,56(r9)
  r3 = REX_LOAD_U8(uint32_t(r9) + 56);
  // rotlwi r11,r3,1
  r11 = __builtin_rotateleft32(uint32_t(r3), 1);
  // add r11,r3,r11
  r11 = r3 + r11;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // neg r11,r11
  r11 = uint64_t(int64_t(static_cast<int64_t>(-r11)));
  // rlwinm r12,r11,0,0,27
  r12 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFF0;
  // bl 0x825a9934
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lwz r11,0(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 0);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // li r25,1
  r25 = uint64_t(int64_t(1));
  // stwux r11,r1,r12
  ea = uint32_t(r1) + uint32_t(r12);
  REX_STORE_U32(ea, uint32_t(r11));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // beq cr6,0x8246b0a0
  if (cr6.eq) goto loc_8246B0A0;
  // lis r11,1398
  r11 = uint64_t(int64_t(91619328));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // ori r24,r11,6641
  r24 = r11 | 6641;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r9,64
  r10 = uint64_t(int64_t(int64_t(r9) + 64));
  // addi r8,r4,4
  r8 = uint64_t(int64_t(int64_t(r4) + 4));
  // addi r29,r9,254
  r29 = uint64_t(int64_t(int64_t(r9) + 254));
  // mr r30,r3
  r30 = r3;
  // ori r27,r11,48000
  r27 = r11 | 48000;
loc_8246AF4C:
  // lwz r6,-4(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + -4);
  // li r26,3968
  r26 = uint64_t(int64_t(3968));
  // lwz r11,192(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 192);
  // mullw r11,r6,r11
  r11 = uint64_t(int64_t(int64_t(int32_t(r6)) * int64_t(int32_t(r11))));
  // rlwinm r11,r11,8,0,23
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 8) & 0xFFFFFF00;
  // mulhwu r11,r11,r24
  r11 = (uint64_t(uint32_t(r11)) * uint64_t(uint32_t(r24))) >> 32;
  // rlwinm r11,r11,22,10,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 22) & 0x3FFFFF;
  // addi r6,r11,16
  r6 = uint64_t(int64_t(int64_t(r11) + 16));
  // sth r6,0(r29)
  REX_STORE_U16(uint32_t(r29) + 0, uint16_t(r6));
  // lbz r6,0(r10)
  r6 = REX_LOAD_U8(uint32_t(r10) + 0);
  // cmplwi cr6,r6,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 1, so);
  // beq cr6,0x8246af80
  if (cr6.eq) goto loc_8246AF80;
  // li r26,1920
  r26 = uint64_t(int64_t(1920));
loc_8246AF80:
  // lbz r5,1(r10)
  r5 = REX_LOAD_U8(uint32_t(r10) + 1);
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // bne cr6,0x8246af94
  if (!cr6.eq) goto loc_8246AF94;
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // b 0x8246aff8
  goto loc_8246AFF8;
loc_8246AF94:
  // addi r11,r11,127
  r11 = uint64_t(int64_t(int64_t(r11) + 127));
  // rlwinm r11,r11,25,7,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 25) & 0x1FFFFFF;
  // addi r6,r11,-1
  r6 = uint64_t(int64_t(int64_t(r11) + -1));
  // rlwinm r28,r11,7,0,24
  r28 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 7) & 0xFFFFFF80;
  // and r6,r6,r11
  r6 = r6 & r11;
  // cmplwi cr6,r6,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), 0, so);
  // beq cr6,0x8246afbc
  if (cr6.eq) goto loc_8246AFBC;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // subfic r11,r11,32
  ca = uint32_t(r11) <= 32;
  r11 = static_cast<uint64_t>(32) - r11;
  // slw r11,r25,r11
  r11 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r25) << (uint8_t(r11) & 0x3F));
loc_8246AFBC:
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bge cr6,0x8246afcc
  if (!cr6.lt) goto loc_8246AFCC;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // b 0x8246afd8
  goto loc_8246AFD8;
loc_8246AFCC:
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // ble cr6,0x8246afd8
  if (!cr6.gt) goto loc_8246AFD8;
  // li r11,8
  r11 = uint64_t(int64_t(8));
loc_8246AFD8:
  // rlwinm r6,r11,7,0,24
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 7) & 0xFFFFFF80;
  // cmplwi cr6,r5,255
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 255, so);
  // add r6,r6,r28
  r6 = r6 + r28;
  // beq cr6,0x8246aff0
  if (cr6.eq) goto loc_8246AFF0;
  // rlwinm r5,r5,7,0,24
  r5 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 7) & 0xFFFFFF80;
  // add r6,r5,r6
  r6 = r5 + r6;
loc_8246AFF0:
  // cmplw cr6,r6,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r26), so);
  // ble cr6,0x8246affc
  if (!cr6.gt) goto loc_8246AFFC;
loc_8246AFF8:
  // mr r6,r26
  r6 = r26;
loc_8246AFFC:
  // lwz r5,-4(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + -4);
  // addi r30,r30,-1
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  // addi r29,r29,2
  r29 = uint64_t(int64_t(int64_t(r29) + 2));
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // stw r5,-4(r8)
  REX_STORE_U32(uint32_t(r8) + -4, uint32_t(r5));
  // lbz r5,0(r10)
  r5 = REX_LOAD_U8(uint32_t(r10) + 0);
  // stb r11,5(r8)
  REX_STORE_U8(uint32_t(r8) + 5, uint8_t(r11));
  // stw r6,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r6));
  // stb r5,4(r8)
  REX_STORE_U8(uint32_t(r8) + 4, uint8_t(r5));
  // addi r8,r8,12
  r8 = uint64_t(int64_t(int64_t(r8) + 12));
  // lwz r11,188(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 188);
  // lbz r6,252(r9)
  r6 = REX_LOAD_U8(uint32_t(r9) + 252);
  // add r11,r7,r11
  r11 = r7 + r11;
  // stw r6,84(r11)
  REX_STORE_U32(uint32_t(r11) + 84, uint32_t(r6));
  // lfs f0,116(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 116);
  f0 = double(temp.f32);
  // lwz r6,-4(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + -4);
  // lwz r11,188(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 188);
  // lbz r5,0(r10)
  r5 = REX_LOAD_U8(uint32_t(r10) + 0);
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // add r11,r7,r11
  r11 = r7 + r11;
  // std r6,80(r31)
  REX_STORE_U64(uint32_t(r31) + 80, r6);
  // addi r28,r11,16
  r28 = uint64_t(int64_t(int64_t(r11) + 16));
  // lwz r6,80(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 80);
  // stb r25,12(r11)
  REX_STORE_U8(uint32_t(r11) + 12, uint8_t(r25));
  // ori r6,r6,1
  r6 = r6 | 1;
  // stb r5,13(r11)
  REX_STORE_U8(uint32_t(r11) + 13, uint8_t(r5));
  // stw r6,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r6));
  // lfd f13,80(r31)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r31) + 80)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f13,f13
  f13 = double(float(f13));
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // fctidz f0,f0
  f0 = std::bit_cast<double>(uint64_t(int64_t(std::isnan(f0) ? int64_t(0x8000000000000000ULL) : (f0 > double(LLONG_MAX)) ? LLONG_MAX : simde_mm_cvttsd_si64(simde_mm_load_sd(&f0)))));
  // stfiwx f0,0,r28
  REX_STORE_U32(uint32_t(r28), uint32_t(std::bit_cast<uint64_t>(f0)));
  // lwz r11,188(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 188);
  // add r11,r7,r11
  r11 = r7 + r11;
  // addi r7,r7,88
  r7 = uint64_t(int64_t(int64_t(r7) + 88));
  // lwz r6,80(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 80);
  // stw r27,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r27));
  // ori r6,r6,2
  r6 = r6 | 2;
  // stw r6,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r6));
  // bne cr6,0x8246af4c
  if (!cr6.eq) goto loc_8246AF4C;
loc_8246B0A0:
  // lbz r11,252(r9)
  r11 = REX_LOAD_U8(uint32_t(r9) + 252);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // rlwinm r11,r11,0,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x2;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8246b0b8
  if (cr6.eq) goto loc_8246B0B8;
  // mr r5,r25
  r5 = r25;
loc_8246B0B8:
  // addi r6,r9,184
  r6 = uint64_t(int64_t(int64_t(r9) + 184));
  // bl 0x8245c650
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245C650, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r31,160
  r1 = uint64_t(int64_t(int64_t(r31) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

namespace {

lifted::Result e82114128(uint8_t* base, const lifted::Args& a) {
  return {f82114128(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82129A08(uint8_t* base, const lifted::Args& a) {
  return f82129A08(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e8212ACF0(uint8_t* base, const lifted::Args& a) {
  return f8212ACF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8212BB20(uint8_t* base, const lifted::Args& a) {
  return f8212BB20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8212C6D0(uint8_t* base, const lifted::Args& a) {
  return f8212C6D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e8212DFB8(uint8_t* base, const lifted::Args& a) {
  return f8212DFB8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[5], a.f[0]);
}

lifted::Result e8246AEE0(uint8_t* base, const lifted::Args& a) {
  return f8246AEE0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

const Registration kEntries[] = {
    {0x82114128, e82114128},
    {0x82129A08, e82129A08},
    {0x8212ACF0, e8212ACF0},
    {0x8212BB20, e8212BB20},
    {0x8212C6D0, e8212C6D0},
    {0x8212DFB8, e8212DFB8},
    {0x8246AEE0, e8246AEE0},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_032)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_82114128) {
  if (!ITC_FLAG(native_lifted_032)) {
    __imp__sub_82114128(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82114128);
}

extern "C" REX_FUNC(sub_82129A08) {
  if (!ITC_FLAG(native_lifted_032)) {
    __imp__sub_82129A08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82129A08);
}

extern "C" REX_FUNC(sub_8212ACF0) {
  if (!ITC_FLAG(native_lifted_032)) {
    __imp__sub_8212ACF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8212ACF0);
}

extern "C" REX_FUNC(sub_8212BB20) {
  if (!ITC_FLAG(native_lifted_032)) {
    __imp__sub_8212BB20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8212BB20);
}

extern "C" REX_FUNC(sub_8212C6D0) {
  if (!ITC_FLAG(native_lifted_032)) {
    __imp__sub_8212C6D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8212C6D0);
}

extern "C" REX_FUNC(sub_8212DFB8) {
  if (!ITC_FLAG(native_lifted_032)) {
    __imp__sub_8212DFB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8212DFB8);
}

extern "C" REX_FUNC(sub_8246AEE0) {
  if (!ITC_FLAG(native_lifted_032)) {
    __imp__sub_8246AEE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8246AEE0);
}
