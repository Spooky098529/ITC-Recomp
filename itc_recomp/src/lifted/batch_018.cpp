// Lift sweep batch 018 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_018, true, "ITC",
              "Run lift sweep batch 018's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address.
#define LIFTED_ARGS(lr) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr) }

namespace lifted {

// sub_825761D8: lifted from generated/ by lift.py.
lifted::Result f825761D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825761D8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // mr r27,r6
  r27 = r6;
  // mr r30,r7
  r30 = r7;
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x825761FC)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x82576274
  if (!cr0.eq) goto loc_82576274;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,20
  r7 = uint64_t(int64_t(20));
  // stw r29,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r29));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r28,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r28));
  // mr r5,r30
  r5 = r30;
  // stw r27,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r27));
  // ori r4,r4,18
  r4 = r4 | 18;
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x8257623C)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x8257624c
  if (!cr0.lt) goto loc_8257624C;
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // b 0x8257626c
  goto loc_8257626C;
loc_8257624C:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x82576268
  if (!cr6.eq) goto loc_82576268;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x82576258)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // b 0x8257626c
  goto loc_8257626C;
loc_82576268:
  // li r31,997
  r31 = uint64_t(int64_t(997));
loc_8257626C:
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825db884
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDereferenceObject, LIFTED_ARGS(0x82576274)); r3 = o.r3; f1 = o.f1; }
loc_82576274:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82576328: lifted from generated/ by lift.py.
lifted::Result f82576328(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82576328 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
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
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r28,r5
  r28 = r5;
  // mr r30,r6
  r30 = r6;
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x82576348)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x825763c0
  if (!cr0.eq) goto loc_825763C0;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,20
  r7 = uint64_t(int64_t(20));
  // stw r29,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r29));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r28,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r28));
  // mr r5,r30
  r5 = r30;
  // ori r4,r4,19
  r4 = r4 | 19;
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x82576388)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82576398
  if (!cr0.lt) goto loc_82576398;
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // b 0x825763b8
  goto loc_825763B8;
loc_82576398:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x825763b4
  if (!cr6.eq) goto loc_825763B4;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x825763A4)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // b 0x825763b8
  goto loc_825763B8;
loc_825763B4:
  // li r31,997
  r31 = uint64_t(int64_t(997));
loc_825763B8:
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825db884
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDereferenceObject, LIFTED_ARGS(0x825763C0)); r3 = o.r3; f1 = o.f1; }
loc_825763C0:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825763D0: lifted from generated/ by lift.py.
lifted::Result f825763D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825763D0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r7
  r31 = r7;
  // mr r27,r4
  r27 = r4;
  // mr r26,r5
  r26 = r5;
  // mr r30,r6
  r30 = r6;
  // mr r28,r8
  r28 = r8;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82576490
  if (cr6.eq) goto loc_82576490;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi cr6,r11,3592
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3592, so);
  // blt cr6,0x82576490
  if (cr6.lt) goto loc_82576490;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x8257640C)); r3 = o.r3; f1 = o.f1; }
  // mr. r29,r3
  r29 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne 0x8257649c
  if (!cr0.eq) goto loc_8257649C;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,32
  r7 = uint64_t(int64_t(32));
  // stw r27,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r27));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // std r26,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r26);
  // mr r5,r28
  r5 = r28;
  // stw r31,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r31));
  // ori r4,r4,26
  r4 = r4 | 26;
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,300
  r11 = uint64_t(int64_t(300));
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x82576454)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82576464
  if (!cr0.lt) goto loc_82576464;
  // li r29,1627
  r29 = uint64_t(int64_t(1627));
  // b 0x82576484
  goto loc_82576484;
loc_82576464:
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // bne cr6,0x82576480
  if (!cr6.eq) goto loc_82576480;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x82576470)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r29,r11,1627
  r29 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // b 0x82576484
  goto loc_82576484;
loc_82576480:
  // li r29,997
  r29 = uint64_t(int64_t(997));
loc_82576484:
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825db884
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDereferenceObject, LIFTED_ARGS(0x8257648C)); r3 = o.r3; f1 = o.f1; }
  // b 0x8257649c
  goto loc_8257649C;
loc_82576490:
  // li r11,3592
  r11 = uint64_t(int64_t(3592));
  // li r29,122
  r29 = uint64_t(int64_t(122));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_8257649C:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825764A8: lifted from generated/ by lift.py.
lifted::Result f825764A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_825764A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r30,r5
  r30 = r5;
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x825764C4)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x82576534
  if (!cr0.eq) goto loc_82576534;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,16
  r7 = uint64_t(int64_t(16));
  // stw r29,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r29));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r5,r30
  r5 = r30;
  // ori r4,r4,20
  r4 = r4 | 20;
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // std r11,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r11);
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x825764FC)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x8257650c
  if (!cr0.lt) goto loc_8257650C;
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // b 0x8257652c
  goto loc_8257652C;
loc_8257650C:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x82576528
  if (!cr6.eq) goto loc_82576528;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x82576518)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // b 0x8257652c
  goto loc_8257652C;
loc_82576528:
  // li r31,997
  r31 = uint64_t(int64_t(997));
loc_8257652C:
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825db884
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDereferenceObject, LIFTED_ARGS(0x82576534)); r3 = o.r3; f1 = o.f1; }
loc_82576534:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82576540: lifted from generated/ by lift.py.
lifted::Result f82576540(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82576540 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x82576560)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x825765d0
  if (!cr0.eq) goto loc_825765D0;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,16
  r7 = uint64_t(int64_t(16));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r5,r30
  r5 = r30;
  // ori r4,r4,21
  r4 = r4 | 21;
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // std r11,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r11);
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x82576598)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x825765a8
  if (!cr0.lt) goto loc_825765A8;
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // b 0x825765c8
  goto loc_825765C8;
loc_825765A8:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x825765c4
  if (!cr6.eq) goto loc_825765C4;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x825765B4)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // b 0x825765c8
  goto loc_825765C8;
loc_825765C4:
  // li r31,997
  r31 = uint64_t(int64_t(997));
loc_825765C8:
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825db884
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDereferenceObject, LIFTED_ARGS(0x825765D0)); r3 = o.r3; f1 = o.f1; }
loc_825765D0:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
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

// sub_825765F0: lifted from generated/ by lift.py.
lifted::Result f825765F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825765F0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r30,292(r1)
  r30 = REX_LOAD_U32(uint32_t(r1) + 292);
  // mr r31,r5
  r31 = r5;
  // mr r25,r7
  r25 = r7;
  // mulli r11,r31,1326
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(1326))));
  // lwz r7,0(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r23,r11,8
  r23 = uint64_t(int64_t(int64_t(r11) + 8));
  // mr r28,r3
  r28 = r3;
  // mr r27,r4
  r27 = r4;
  // mr r26,r6
  r26 = r6;
  // cmplw cr6,r7,r23
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r23), so);
  // blt cr6,0x825766d0
  if (cr6.lt) goto loc_825766D0;
  // lwz r11,300(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 300);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x825766d0
  if (cr6.eq) goto loc_825766D0;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // lwz r24,308(r1)
  r24 = REX_LOAD_U32(uint32_t(r1) + 308);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,36
  r7 = uint64_t(int64_t(36));
  // stw r28,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r28));
  // addi r6,r1,80
  r6 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r27,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r27));
  // mr r5,r24
  r5 = r24;
  // stw r31,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r31));
  // stw r29,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r29));
  // ori r4,r4,28
  r4 = r4 | 28;
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r26,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r26));
  // sth r25,92(r1)
  REX_STORE_U16(uint32_t(r1) + 92, uint16_t(r25));
  // sth r8,94(r1)
  REX_STORE_U16(uint32_t(r1) + 94, uint16_t(r8));
  // stw r9,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r9));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // stw r11,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x8257668C)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x825766ac
  if (!cr0.lt) goto loc_825766AC;
  // lis r11,-32747
  r11 = uint64_t(int64_t(-2146107392));
  // ori r11,r11,20999
  r11 = r11 | 20999;
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // beq cr6,0x825766d0
  if (cr6.eq) goto loc_825766D0;
  // li r3,1627
  r3 = uint64_t(int64_t(1627));
  // b 0x825766d8
  goto loc_825766D8;
loc_825766AC:
  // cmplwi cr6,r24,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r24), 0, so);
  // bne cr6,0x825766c8
  if (!cr6.eq) goto loc_825766C8;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x825766B8)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r3,r11,1627
  r3 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // b 0x825766d8
  goto loc_825766D8;
loc_825766C8:
  // li r3,997
  r3 = uint64_t(int64_t(997));
  // b 0x825766d8
  goto loc_825766D8;
loc_825766D0:
  // stw r23,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r23));
  // li r3,122
  r3 = uint64_t(int64_t(122));
loc_825766D8:
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825766E0: lifted from generated/ by lift.py.
lifted::Result f825766E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825766E0 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r4
  r30 = r4;
  // mr r28,r5
  r28 = r5;
  // rldicl r11,r30,16,48
  r11 = __builtin_rotateleft64(r30, 16) & 0xFFFF;
  // mr r27,r6
  r27 = r6;
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // mr r29,r7
  r29 = r7;
  // clrlwi r10,r11,28
  r10 = uint32_t(r11) & 0xF;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // cmplwi cr6,r10,9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 9, so);
  // bne cr6,0x8257671c
  if (!cr6.eq) goto loc_8257671C;
  // rlwinm. r11,r11,0,24,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC0;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82576794
  if (!cr0.eq) goto loc_82576794;
loc_8257671C:
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825dbd74
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamSessionRefObjByHandle, LIFTED_ARGS(0x82576724)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne 0x82576794
  if (!cr0.eq) goto loc_82576794;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lis r4,11
  r4 = uint64_t(int64_t(720896));
  // li r7,24
  r7 = uint64_t(int64_t(24));
  // std r30,104(r1)
  REX_STORE_U64(uint32_t(r1) + 104, r30);
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r28,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r28));
  // mr r5,r29
  r5 = r29;
  // stw r27,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r27));
  // ori r4,r4,37
  r4 = r4 | 37;
  // li r3,251
  r3 = uint64_t(int64_t(251));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x8257675C)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x8257676c
  if (!cr0.lt) goto loc_8257676C;
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // b 0x8257678c
  goto loc_8257678C;
loc_8257676C:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x82576788
  if (!cr6.eq) goto loc_82576788;
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x82576778)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // b 0x8257678c
  goto loc_8257678C;
loc_82576788:
  // li r31,997
  r31 = uint64_t(int64_t(997));
loc_8257678C:
  // lwz r3,80(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 80);
  // bl 0x825db884
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__ObDereferenceObject, LIFTED_ARGS(0x82576794)); r3 = o.r3; f1 = o.f1; }
loc_82576794:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825767A0: lifted from generated/ by lift.py.
uint64_t f825767A0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825767A0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // mr r11,r3
  r11 = r3;
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // extsw r8,r4
  r8 = uint64_t(int64_t(int32_t(r4)));
  // lwz r10,512(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 512);
  // rlwinm r10,r10,4,0,27
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 4) & 0xFFFFFFF0;
  // add r10,r10,r11
  r10 = r10 + r11;
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // std r8,8(r10)
  REX_STORE_U64(uint32_t(r10) + 8, r8);
  // lwz r3,512(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 512);
  // addi r10,r3,1
  r10 = uint64_t(int64_t(int64_t(r3) + 1));
  // stw r10,512(r11)
  REX_STORE_U32(uint32_t(r11) + 512, uint32_t(r10));
  // blr
  return r3;
}

// sub_82576BE0: lifted from generated/ by lift.py.
lifted::Result f82576BE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82576BE0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-784(r1)
  ea = -784 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // stw r3,804(r1)
  REX_STORE_U32(uint32_t(r1) + 804, uint32_t(r3));
  // mr r28,r4
  r28 = r4;
  // stw r5,820(r1)
  REX_STORE_U32(uint32_t(r1) + 820, uint32_t(r5));
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r3,3329
  r3 = uint64_t(int64_t(3329));
  // mr r27,r6
  r27 = r6;
  // mr r29,r7
  r29 = r7;
  // stw r31,720(r1)
  REX_STORE_U32(uint32_t(r1) + 720, uint32_t(r31));
  // stw r31,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r31));
  // stw r31,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r31));
  // bl 0x82575b68
  // (left out: dead in C++)
  { const lifted::Result o = f82575B68(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576e1c
  if (cr0.lt) goto loc_82576E1C;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r3,128(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 128);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r7,r1,132
  r7 = uint64_t(int64_t(int64_t(r1) + 132));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82575bf0
  // (left out: dead in C++)
  r3 = f82575BF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576e1c
  if (cr0.lt) goto loc_82576E1C;
  // lwz r11,132(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 132);
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r3,3586
  r3 = uint64_t(int64_t(3586));
  // addi r30,r11,48
  r30 = uint64_t(int64_t(int64_t(r11) + 48));
  // bl 0x82575b68
  // (left out: dead in C++)
  { const lifted::Result o = f82575B68(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576e1c
  if (cr0.lt) goto loc_82576E1C;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r3,128(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 128);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r7,r1,132
  r7 = uint64_t(int64_t(int64_t(r1) + 132));
  // li r6,256
  r6 = uint64_t(int64_t(256));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x82575bf0
  // (left out: dead in C++)
  r3 = f82575BF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576e1c
  if (cr0.lt) goto loc_82576E1C;
  // lwz r11,132(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 132);
  // addi r4,r1,128
  r4 = uint64_t(int64_t(int64_t(r1) + 128));
  // li r3,3603
  r3 = uint64_t(int64_t(3603));
  // add r30,r30,r11
  r30 = r30 + r11;
  // bl 0x82575b68
  // (left out: dead in C++)
  { const lifted::Result o = f82575B68(base, r1, r13, r3, r4, r8, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576e1c
  if (cr0.lt) goto loc_82576E1C;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r3,128(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 128);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r7,r1,132
  r7 = uint64_t(int64_t(int64_t(r1) + 132));
  // mr r6,r30
  r6 = r30;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x82575bf0
  // (left out: dead in C++)
  r3 = f82575BF0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576e1c
  if (cr0.lt) goto loc_82576E1C;
  // addi r5,r1,136
  r5 = uint64_t(int64_t(int64_t(r1) + 136));
  // lwz r4,132(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 132);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x825dbd64
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamAlloc, LIFTED_ARGS(0x82576CE4)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576e1c
  if (cr0.lt) goto loc_82576E1C;
  // mr r3,r28
  r3 = r28;
  // bl 0x825a9958
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A9958, LIFTED_ARGS(0x82576CF4)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // addi r6,r1,144
  r6 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r7,136(r1)
  r7 = REX_LOAD_U32(uint32_t(r1) + 136);
  // addi r5,r1,152
  r5 = uint64_t(int64_t(int64_t(r1) + 152));
  // lwz r4,128(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 128);
  // addi r8,r1,140
  r8 = uint64_t(int64_t(int64_t(r1) + 140));
  // stw r29,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r29));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r31,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r31));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r27,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r27));
  // stw r6,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r6));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r5,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r5));
  // mr r6,r30
  r6 = r30;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // stw r8,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r8));
  // li r3,3603
  r3 = uint64_t(int64_t(3603));
  // lwz r8,132(r1)
  r8 = REX_LOAD_U32(uint32_t(r1) + 132);
  // stw r11,156(r1)
  REX_STORE_U32(uint32_t(r1) + 156, uint32_t(r11));
  // bl 0x82575c90
  // (left out: dead in C++)
  { const lifted::Result o = f82575C90(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576e1c
  if (cr0.lt) goto loc_82576E1C;
  // addi r4,r1,804
  r4 = uint64_t(int64_t(int64_t(r1) + 804));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // mr r4,r28
  r4 = r28;
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // addi r4,r1,820
  r4 = uint64_t(int64_t(int64_t(r1) + 820));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // mr r4,r27
  r4 = r27;
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // addi r4,r1,156
  r4 = uint64_t(int64_t(int64_t(r1) + 156));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // addi r4,r1,148
  r4 = uint64_t(int64_t(int64_t(r1) + 148));
  // addi r3,r1,208
  r3 = uint64_t(int64_t(int64_t(r1) + 208));
  // bl 0x825767a0
  // (left out: dead in C++)
  r3 = f825767A0(base, r3, r4);
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r8,r1,208
  r8 = uint64_t(int64_t(int64_t(r1) + 208));
  // lwz r9,152(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 152);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r5,128(r1)
  r5 = REX_LOAD_U32(uint32_t(r1) + 128);
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82575d78
  // (left out: dead in C++)
  { const lifted::Result o = f82575D78(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576e1c
  if (cr0.lt) goto loc_82576E1C;
  // lwz r10,144(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 144);
  // addi r11,r1,160
  r11 = uint64_t(int64_t(int64_t(r1) + 160));
  // lwz r9,140(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 140);
  // li r7,40
  r7 = uint64_t(int64_t(40));
  // addi r6,r1,160
  r6 = uint64_t(int64_t(int64_t(r1) + 160));
  // mr r5,r29
  r5 = r29;
  // li r3,252
  r3 = uint64_t(int64_t(252));
  // stw r10,28(r9)
  REX_STORE_U32(uint32_t(r9) + 28, uint32_t(r10));
  // std r31,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r31);
  // std r31,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r31);
  // lwz r10,128(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 128);
  // std r31,16(r11)
  REX_STORE_U64(uint32_t(r11) + 16, r31);
  // std r31,24(r11)
  REX_STORE_U64(uint32_t(r11) + 24, r31);
  // clrlwi r4,r10,16
  r4 = uint32_t(r10) & 0xFFFF;
  // std r31,32(r11)
  REX_STORE_U64(uint32_t(r11) + 32, r31);
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // lwz r11,140(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 140);
  // rlwimi r4,r10,16,0,15
  r4 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 16) & 0xFFFF0000) | (r4 & 0xFFFFFFFF0000FFFF);
  // stw r11,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r11));
  // bl 0x825db4b4
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgStartIORequest, LIFTED_ARGS(0x82576E14)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82576e48
  if (!cr0.lt) goto loc_82576E48;
loc_82576E1C:
  // li r31,1627
  r31 = uint64_t(int64_t(1627));
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x82576e30
  if (cr6.eq) goto loc_82576E30;
  // stw r3,24(r29)
  REX_STORE_U32(uint32_t(r29) + 24, uint32_t(r3));
  // b 0x82576e34
  goto loc_82576E34;
loc_82576E30:
  // bl 0x82296e70
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296E70, LIFTED_ARGS(0x82576E34)); r3 = o.r3; f1 = o.f1; }
loc_82576E34:
  // lwz r3,136(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 136);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82576e68
  if (cr6.eq) goto loc_82576E68;
  // bl 0x825dbd54
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XamFree, LIFTED_ARGS(0x82576E44)); r3 = o.r3; f1 = o.f1; }
  // b 0x82576e68
  goto loc_82576E68;
loc_82576E48:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x82576e58
  if (cr6.eq) goto loc_82576E58;
  // li r31,997
  r31 = uint64_t(int64_t(997));
  // b 0x82576e68
  goto loc_82576E68;
loc_82576E58:
  // bl 0x82296ec0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296EC0, LIFTED_ARGS(0x82576E5C)); r3 = o.r3; f1 = o.f1; }
  // subfic r11,r3,0
  ca = uint32_t(r3) <= 0;
  r11 = static_cast<uint64_t>(0) - r3;
  // subfe r11,r11,r11
  temp.u8 = (~uint32_t(r11) + uint32_t(r11) < ~uint32_t(r11)) | (~uint32_t(r11) + uint32_t(r11) + ca < ca);
  r11 = ~r11 + r11 + ca;
  ca = temp.u8;
  // andi. r31,r11,1627
  r31 = r11 & 1627;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
loc_82576E68:
  // mr r3,r31
  r3 = r31;
  // addi r1,r1,784
  r1 = uint64_t(int64_t(int64_t(r1) + 784));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82576F30: lifted from generated/ by lift.py.
lifted::Result f82576F30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82576F30 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r3
  r11 = r3;
  // stw r7,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r7));
  // mr r10,r4
  r10 = r4;
  // stw r8,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r8));
  // mr r31,r5
  r31 = r5;
  // stw r9,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r9));
  // mr r30,r6
  r30 = r6;
  // lis r4,5
  r4 = uint64_t(int64_t(327680));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r31,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r31));
  // ori r4,r4,32821
  r4 = r4 | 32821;
  // stw r30,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r30));
  // li r3,252
  r3 = uint64_t(int64_t(252));
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // bl 0x825dbd44
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__XMsgInProcessCall, LIFTED_ARGS(0x82576F90)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82576fa0
  if (cr0.lt) goto loc_82576FA0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x82576fd0
  goto loc_82576FD0;
loc_82576FA0:
  // lis r11,-32761
  r11 = uint64_t(int64_t(-2147024896));
  // ori r11,r11,87
  r11 = r11 | 87;
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // bne cr6,0x82576fb8
  if (!cr6.eq) goto loc_82576FB8;
  // li r3,87
  r3 = uint64_t(int64_t(87));
  // b 0x82576fd0
  goto loc_82576FD0;
loc_82576FB8:
  // lis r11,-32761
  r11 = uint64_t(int64_t(-2147024896));
  // ori r11,r11,122
  r11 = r11 | 122;
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // li r3,122
  r3 = uint64_t(int64_t(122));
  // beq cr6,0x82576fd0
  if (cr6.eq) goto loc_82576FD0;
  // li r3,1627
  r3 = uint64_t(int64_t(1627));
loc_82576FD0:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
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

// sub_82577088: lifted from generated/ by lift.py.
uint64_t f82577088(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82577088 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_825770B8: lifted from generated/ by lift.py.
uint64_t f825770B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825770B8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0;
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
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x825770f8
  if (cr0.lt) goto loc_825770F8;
  // addi r4,r30,2
  r4 = uint64_t(int64_t(int64_t(r30) + 2));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
loc_825770F8:
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

// sub_82577110: lifted from generated/ by lift.py.
uint64_t f82577110(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82577110 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82577140: lifted from generated/ by lift.py.
uint64_t f82577140(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82577140 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0;
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
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // mr r29,r5
  r29 = r5;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577188
  if (cr0.lt) goto loc_82577188;
  // lbz r11,0(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 0);
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82577188
  if (cr0.eq) goto loc_82577188;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
loc_82577188:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_82577190: lifted from generated/ by lift.py.
uint64_t f82577190(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82577190 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-176(r1)
  ea = -176 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r26,r4
  r26 = r4;
  // mr r31,r3
  r31 = r3;
  // cmplwi cr6,r26,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), 16, so);
  // bge cr6,0x8257730c
  if (!cr6.lt) goto loc_8257730C;
  // li r27,1
  r27 = uint64_t(int64_t(1));
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // addi r4,r1,81
  r4 = uint64_t(int64_t(int64_t(r1) + 81));
  // stw r27,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r27));
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm r11,r11,30,30,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x8257730c
  if (cr6.lt) goto loc_8257730C;
  // beq cr6,0x825771e4
  if (cr6.eq) goto loc_825771E4;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x8257730c
  if (!cr6.lt) goto loc_8257730C;
loc_825771E4:
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82577220
  if (cr0.eq) goto loc_82577220;
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // stw r27,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r27));
  // addi r4,r1,83
  r4 = uint64_t(int64_t(int64_t(r1) + 83));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
loc_82577220:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82577240
  if (cr0.eq) goto loc_82577240;
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
loc_82577240:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // li r28,2
  r28 = uint64_t(int64_t(2));
  // rlwinm. r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8257728c
  if (cr0.eq) goto loc_8257728C;
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // stw r28,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r28));
  // addi r4,r1,86
  r4 = uint64_t(int64_t(int64_t(r1) + 86));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // lbz r11,86(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 86);
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8257728c
  if (cr0.eq) goto loc_8257728C;
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
loc_8257728C:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x825772bc
  if (!cr6.eq) goto loc_825772BC;
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r3,r31
  r3 = r31;
  // bl 0x82577140
  // (left out: dead in C++)
  r3 = f82577140(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // b 0x825772c0
  goto loc_825772C0;
loc_825772BC:
  // lhz r29,88(r1)
  r29 = REX_LOAD_U16(uint32_t(r1) + 88);
loc_825772C0:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // mr r3,r31
  r3 = r31;
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x825773a4
  if (!cr6.eq) goto loc_825773A4;
  // lbz r11,89(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 89);
  // clrlwi r10,r29,16
  r10 = uint32_t(r29) & 0xFFFF;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // blt cr6,0x8257731c
  if (cr6.lt) goto loc_8257731C;
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // stw r27,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r27));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 12, so);
loc_82577308:
  // beq cr6,0x825773f8
  if (cr6.eq) goto loc_825773F8;
loc_8257730C:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
loc_82577314:
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
loc_8257731C:
  // addi r4,r1,82
  r4 = uint64_t(int64_t(int64_t(r1) + 82));
  // bl 0x825784a8
  // (left out: dead in C++)
  r3 = f825784A8(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // lbz r11,82(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 82);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82577360
  if (cr0.eq) goto loc_82577360;
loc_8257733C:
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // lbz r11,82(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 82);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x8257733c
  if (cr6.lt) goto loc_8257733C;
loc_82577360:
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // lhz r11,84(r1)
  r11 = REX_LOAD_U16(uint32_t(r1) + 84);
  // addi r5,r1,96
  r5 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r4,r1,100
  r4 = uint64_t(int64_t(int64_t(r1) + 100));
  // mr r3,r31
  r3 = r31;
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x82578580
  // (left out: dead in C++)
  r3 = f82578580(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // lwz r11,100(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 100);
  // lbz r11,0(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 0);
  // stb r11,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r11));
  // b 0x825773f8
  goto loc_825773F8;
loc_825773A4:
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // lwz r30,12(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 12);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r27,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r27));
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // lbz r10,80(r1)
  r10 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm r11,r10,30,30,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 30) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82577428
  if (cr6.lt) goto loc_82577428;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82577308
  if (!cr6.lt) goto loc_82577308;
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825780f0
  // (left out: dead in C++)
  r3 = f825780F0(base, r3, r4);
  // addi r4,r26,1
  r4 = uint64_t(int64_t(int64_t(r26) + 1));
  // mr r3,r31
  r3 = r31;
  // bl 0x82577190
  // (left out: dead in C++)
  r3 = f82577190(base, r1, r13, r3, r4);
loc_825773F0:
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
loc_825773F8:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x82577414
  if (!cr6.eq) goto loc_82577414;
  // clrlwi r11,r29,16
  r11 = uint32_t(r29) & 0xFFFF;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r29,r11,16
  r29 = uint32_t(r11) & 0xFFFF;
loc_82577414:
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 12, so);
  // bne cr6,0x825772c0
  if (!cr6.eq) goto loc_825772C0;
  // b 0x82577314
  goto loc_82577314;
loc_82577428:
  // rlwinm. r11,r10,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8257744c
  if (cr0.eq) goto loc_8257744C;
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // stw r27,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r27));
  // addi r4,r1,83
  r4 = uint64_t(int64_t(int64_t(r1) + 83));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
loc_8257744C:
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8257746c
  if (cr0.eq) goto loc_8257746C;
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
loc_8257746C:
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm. r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825774b4
  if (cr0.eq) goto loc_825774B4;
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // stw r28,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r28));
  // addi r4,r1,86
  r4 = uint64_t(int64_t(int64_t(r1) + 86));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578530
  // (left out: dead in C++)
  r3 = f82578530(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
  // lbz r11,86(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 86);
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825774b4
  if (cr0.eq) goto loc_825774B4;
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82577314
  if (cr0.lt) goto loc_82577314;
loc_825774B4:
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm. r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825773f8
  if (cr0.eq) goto loc_825773F8;
  // addi r4,r1,104
  r4 = uint64_t(int64_t(int64_t(r1) + 104));
  // mr r3,r31
  r3 = r31;
  // bl 0x825770b8
  // (left out: dead in C++)
  r3 = f825770B8(base, r1, r13, r3, r4);
  // b 0x825773f0
  goto loc_825773F0;
}

// sub_825774D0: lifted from generated/ by lift.py.
uint64_t f825774D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825774D0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r11,r3
  r11 = r3;
  // mr r3,r5
  r3 = r5;
  // lhz r10,40(r11)
  r10 = REX_LOAD_U16(uint32_t(r11) + 40);
  // cmplw cr6,r4,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r10), so);
  // blt cr6,0x825774fc
  if (cr6.lt) goto loc_825774FC;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // b 0x82577518
  goto loc_82577518;
loc_825774FC:
  // lhz r9,42(r11)
  r9 = REX_LOAD_U16(uint32_t(r11) + 42);
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // lwz r10,48(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mullw r11,r9,r4
  r11 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r4))));
  // add r4,r11,r10
  r4 = r11 + r10;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x82577514)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_82577518:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82577528: lifted from generated/ by lift.py.
uint64_t f82577528(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82577528 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lhz r10,24(r3)
  r10 = REX_LOAD_U16(uint32_t(r3) + 24);
  // clrlwi r11,r4,16
  r11 = uint32_t(r4) & 0xFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x82577544
  if (!cr6.gt) goto loc_82577544;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // blr
  return r3;
loc_82577544:
  // lwz r10,64(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 64);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // stw r11,0(r5)
  REX_STORE_U32(uint32_t(r5) + 0, uint32_t(r11));
  // blr
  return r3;
}

// sub_82577630: lifted from generated/ by lift.py.
uint64_t f82577630(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82577630 (lifted)");
  uint64_t r7 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r4
  r31 = r4;
  // mr r28,r3
  r28 = r3;
  // mr r4,r5
  r4 = r5;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // mr r29,r6
  r29 = r6;
  // stw r31,20(r28)
  REX_STORE_U32(uint32_t(r28) + 20, uint32_t(r31));
  // bl 0x825774d0
  // (left out: dead in C++)
  r3 = f825774D0(base, r1, r13, r3, r4, r5);
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt 0x8257769c
  if (cr0.lt) goto loc_8257769C;
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq cr6,0x8257767c
  if (cr6.eq) goto loc_8257767C;
  // lhz r6,80(r1)
  r6 = REX_LOAD_U16(uint32_t(r1) + 80);
  // lwz r10,84(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 84);
  // b 0x82577684
  goto loc_82577684;
loc_8257767C:
  // lhz r6,82(r1)
  r6 = REX_LOAD_U16(uint32_t(r1) + 82);
  // lwz r10,88(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 88);
loc_82577684:
  // add r4,r11,r10
  r4 = r11 + r10;
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // mr r5,r6
  r5 = r6;
  // mr r3,r28
  r3 = r28;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // bl 0x825780d0
  // (left out: dead in C++)
  f825780D0(base, r3, r4, r5, r6, r7);
loc_8257769C:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_825776A8: lifted from generated/ by lift.py.
lifted::Result f825776A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825776A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r30,r7
  r30 = r7;
  // mr r31,r3
  r31 = r3;
  // mr r29,r4
  r29 = r4;
  // mr r28,r5
  r28 = r5;
  // mr r27,r6
  r27 = r6;
  // mr r26,r8
  r26 = r8;
  // mr r25,r9
  r25 = r9;
  // mr r24,r10
  r24 = r10;
  // rlwinm. r11,r30,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825776ec
  if (cr0.eq) goto loc_825776EC;
  // bl 0x825db934
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Import(base, __imp__KeGetCurrentProcessType, LIFTED_ARGS(0x825776E0)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x825776ec
  if (!cr6.eq) goto loc_825776EC;
  // ori r30,r30,16
  r30 = r30 | 16;
loc_825776EC:
  // lwz r11,252(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 252);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r3,284(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 284);
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // lwz r9,260(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 260);
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // lwz r8,244(r1)
  r8 = REX_LOAD_U32(uint32_t(r1) + 244);
  // lwz r5,276(r1)
  r5 = REX_LOAD_U32(uint32_t(r1) + 276);
  // stw r11,24(r31)
  REX_STORE_U32(uint32_t(r31) + 24, uint32_t(r11));
  // lwz r11,292(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 292);
  // stw r3,44(r31)
  REX_STORE_U32(uint32_t(r31) + 44, uint32_t(r3));
  // addi r3,r31,52
  r3 = uint64_t(int64_t(int64_t(r31) + 52));
  // lwz r4,268(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 268);
  // stw r29,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r29));
  // stw r28,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r28));
  // stw r27,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r27));
  // stw r30,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r30));
  // stw r26,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r26));
  // stw r25,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r25));
  // stw r10,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r10));
  // stw r9,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r9));
  // stw r24,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r24));
  // stw r8,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r8));
  // stw r11,48(r31)
  REX_STORE_U32(uint32_t(r31) + 48, uint32_t(r11));
  // bl 0x825780d0
  // (left out: dead in C++)
  f825780D0(base, r3, r4, r5, r6, r7);
  // lwz r11,300(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 300);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r11,72(r31)
  REX_STORE_U32(uint32_t(r31) + 72, uint32_t(r11));
  // beq cr6,0x8257777c
  if (cr6.eq) goto loc_8257777C;
  // lwz r3,12(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82577770
  if (cr0.eq) goto loc_82577770;
  // bl 0x825ba168
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA168, LIFTED_ARGS(0x82577770)); r3 = o.r3; f1 = o.f1; }
loc_82577770:
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // li r10,997
  r10 = uint64_t(int64_t(997));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_8257777C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82577788: lifted from generated/ by lift.py.
uint64_t f82577788(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82577788 (lifted)");
  uint64_t r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e7c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // mr r28,r6
  r28 = r6;
  // addi r30,r31,28
  r30 = uint64_t(int64_t(int64_t(r31) + 28));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // mr r27,r8
  r27 = r8;
  // stw r4,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r4));
  // mr r29,r9
  r29 = r9;
  // stw r7,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r7));
  // mr r26,r10
  r26 = r10;
  // bl 0x82577630
  // (left out: dead in C++)
  r3 = f82577630(base, r1, r13, r3, r4, r5, r6);
  // mr. r25,r3
  r25 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // bge 0x825777fc
  if (!cr0.lt) goto loc_825777FC;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
  // stw r11,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r11));
  // stw r11,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r11));
  // stw r11,12(r30)
  REX_STORE_U32(uint32_t(r30) + 12, uint32_t(r11));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // stw r11,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r11));
  // stw r11,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r11));
  // stw r11,1080(r31)
  REX_STORE_U32(uint32_t(r31) + 1080, uint32_t(r11));
  // b 0x82577840
  goto loc_82577840;
loc_825777FC:
  // mr r7,r26
  r7 = r26;
  // stw r28,1080(r31)
  REX_STORE_U32(uint32_t(r31) + 1080, uint32_t(r28));
  // mr r6,r29
  r6 = r29;
  // mr r5,r29
  r5 = r29;
  // mr r4,r27
  r4 = r27;
  // addi r3,r31,8
  r3 = uint64_t(int64_t(int64_t(r31) + 8));
  // bl 0x825780d0
  // (left out: dead in C++)
  f825780D0(base, r3, r4, r5, r6, r7);
  // addi r10,r31,56
  r10 = uint64_t(int64_t(int64_t(r31) + 56));
  // li r9,64
  r9 = uint64_t(int64_t(64));
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82577824:
  // addi r8,r10,8
  r8 = uint64_t(int64_t(int64_t(r10) + 8));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // addi r10,r10,16
  r10 = uint64_t(int64_t(int64_t(r10) + 16));
  // stw r11,-4(r8)
  REX_STORE_U32(uint32_t(r8) + -4, uint32_t(r11));
  // std r11,0(r8)
  REX_STORE_U64(uint32_t(r8) + 0, r11);
  // bne 0x82577824
  if (!cr0.eq) goto loc_82577824;
loc_82577840:
  // mr r3,r25
  r3 = r25;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return r3;
}

// sub_82577850: lifted from generated/ by lift.py.
uint64_t f82577850(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82577850 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r29,r5
  r29 = r5;
  // mr r30,r6
  r30 = r6;
  // cmplwi cr6,r4,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 1, so);
  // bgt cr6,0x825778c8
  if (cr6.gt) goto loc_825778C8;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r31,28
  r3 = uint64_t(int64_t(int64_t(r31) + 28));
  // bl 0x82577088
  // (left out: dead in C++)
  r3 = f82577088(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x825778d0
  if (cr0.lt) goto loc_825778D0;
  // lbz r10,80(r1)
  r10 = REX_LOAD_U8(uint32_t(r1) + 80);
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x82577898
  if (cr6.eq) goto loc_82577898;
  // rlwinm r11,r10,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 27) & 0x1;
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_82577898:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // clrlwi r10,r10,27
  r10 = uint32_t(r10) & 0x1F;
  // lwz r9,512(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 512);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bge cr6,0x825778b8
  if (!cr6.lt) goto loc_825778B8;
  // rlwinm r10,r10,4,0,27
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 4) & 0xFFFFFFF0;
  // add r11,r10,r11
  r11 = r10 + r11;
  // b 0x825778bc
  goto loc_825778BC;
loc_825778B8:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825778BC:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
  // bne cr6,0x825778d0
  if (!cr6.eq) goto loc_825778D0;
loc_825778C8:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
loc_825778D0:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_825778D8: lifted from generated/ by lift.py.
lifted::Result f825778D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, double f1) {
  ITC_NATIVE_RUNNING("sub_825778D8 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-304(r1)
  ea = -304 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r4
  r28 = r4;
  // li r14,0
  r14 = uint64_t(int64_t(0));
  // mr r29,r5
  r29 = r5;
  // mr r26,r3
  r26 = r3;
  // cmplwi cr6,r28,16
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 16, so);
  // stw r28,332(r1)
  REX_STORE_U32(uint32_t(r1) + 332, uint32_t(r28));
  // sth r14,90(r1)
  REX_STORE_U16(uint32_t(r1) + 90, uint16_t(r14));
  // stw r29,340(r1)
  REX_STORE_U32(uint32_t(r1) + 340, uint32_t(r29));
  // stw r14,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r14));
  // blt cr6,0x82577918
  if (cr6.lt) goto loc_82577918;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // b 0x82577b64
  goto loc_82577B64;
loc_82577918:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq cr6,0x82577928
  if (cr6.eq) goto loc_82577928;
  // lwz r20,0(r29)
  r20 = REX_LOAD_U32(uint32_t(r29) + 0);
  // b 0x8257792c
  goto loc_8257792C;
loc_82577928:
  // mr r20,r14
  r20 = r14;
loc_8257792C:
  // lwz r11,40(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 40);
  // addi r23,r26,28
  r23 = uint64_t(int64_t(int64_t(r26) + 28));
  // addi r4,r1,81
  r4 = uint64_t(int64_t(int64_t(r1) + 81));
  // mr r3,r23
  r3 = r23;
  // stw r11,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r11));
  // bl 0x82577088
  // (left out: dead in C++)
  r3 = f82577088(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm r11,r11,30,30,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82577b58
  if (cr6.lt) goto loc_82577B58;
  // beq cr6,0x82577970
  if (cr6.eq) goto loc_82577970;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82577b58
  if (!cr6.lt) goto loc_82577B58;
  // addi r4,r1,90
  r4 = uint64_t(int64_t(int64_t(r1) + 90));
  // b 0x82577974
  goto loc_82577974;
loc_82577970:
  // addi r4,r1,86
  r4 = uint64_t(int64_t(int64_t(r1) + 86));
loc_82577974:
  // mr r3,r23
  r3 = r23;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // li r15,4
  r15 = uint64_t(int64_t(4));
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825779d0
  if (cr0.eq) goto loc_825779D0;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // addi r5,r1,120
  r5 = uint64_t(int64_t(int64_t(r1) + 120));
  // mr r4,r28
  r4 = r28;
  // mr r3,r26
  r3 = r26;
  // bl 0x82577850
  // (left out: dead in C++)
  r3 = f82577850(base, r1, r13, r3, r4, r5, r6);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lwz r10,20(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 20);
  // lwz r9,8(r26)
  r9 = REX_LOAD_U32(uint32_t(r26) + 8);
  // lwz r11,120(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 120);
  // add r10,r10,r9
  r10 = r10 + r9;
  // clrldi r10,r10,32
  r10 = r10 & 0xFFFFFFFF;
  // ld r20,8(r11)
  r20 = REX_LOAD_U64(uint32_t(r11) + 8);
  // stw r15,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r15));
  // std r10,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r10);
loc_825779D0:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x825779f0
  if (cr0.eq) goto loc_825779F0;
  // addi r4,r1,98
  r4 = uint64_t(int64_t(int64_t(r1) + 98));
  // mr r3,r23
  r3 = r23;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
loc_825779F0:
  // lbz r4,81(r1)
  r4 = REX_LOAD_U8(uint32_t(r1) + 81);
  // li r18,1
  r18 = uint64_t(int64_t(1));
  // rlwinm. r11,r4,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82577a28
  if (cr0.eq) goto loc_82577A28;
  // lwz r11,1080(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 1080);
  // addi r7,r1,116
  r7 = uint64_t(int64_t(int64_t(r1) + 116));
  // addi r6,r1,96
  r6 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r5,r23
  r5 = r23;
  // addi r3,r26,56
  r3 = uint64_t(int64_t(int64_t(r26) + 56));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // bl 0x825786b8
  // (left out: dead in C++)
  r3 = f825786B8(base, r1, r13, r3, r4, r5, r6, r7);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // b 0x82577a2c
  goto loc_82577A2C;
loc_82577A28:
  // stw r18,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r18));
loc_82577A2C:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // mr r17,r14
  r17 = r14;
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x82577ac8
  if (!cr6.eq) goto loc_82577AC8;
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r23
  r3 = r23;
  // bl 0x82577140
  // (left out: dead in C++)
  r3 = f82577140(base, r1, r13, r3, r4, r5);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r11,84(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 84);
  // addi r30,r26,56
  r30 = uint64_t(int64_t(int64_t(r26) + 56));
  // addi r5,r1,128
  r5 = uint64_t(int64_t(int64_t(r1) + 128));
  // clrlwi r4,r11,26
  r4 = uint32_t(r11) & 0x3F;
  // mr r3,r30
  r3 = r30;
  // bl 0x82578608
  // (left out: dead in C++)
  r3 = f82578608(base, r3, r4, r5);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r10,84(r1)
  r10 = REX_LOAD_U8(uint32_t(r1) + 84);
  // lwz r11,128(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 128);
  // ld r16,8(r11)
  r16 = REX_LOAD_U64(uint32_t(r11) + 8);
  // rlwinm. r10,r10,0,25,25
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82577aac
  if (cr0.eq) goto loc_82577AAC;
  // addi r5,r1,108
  r5 = uint64_t(int64_t(int64_t(r1) + 108));
  // lhz r4,92(r1)
  r4 = REX_LOAD_U16(uint32_t(r1) + 92);
  // lwz r3,48(r26)
  r3 = REX_LOAD_U32(uint32_t(r26) + 48);
  // bl 0x82577528
  // (left out: dead in C++)
  r3 = f82577528(base, r3, r4, r5);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lwz r11,108(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 108);
  // and r16,r11,r16
  r16 = r11 & r16;
loc_82577AAC:
  // lbz r11,84(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 84);
  // mr r3,r30
  r3 = r30;
  // clrlwi r4,r11,26
  r4 = uint32_t(r11) & 0x3F;
  // bl 0x82578658
  // (left out: dead in C++)
  r3 = f82578658(base, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bge 0x82577acc
  if (!cr0.lt) goto loc_82577ACC;
  // b 0x82577b60
  goto loc_82577B60;
loc_82577AC8:
  // lwz r16,132(r1)
  r16 = REX_LOAD_U32(uint32_t(r1) + 132);
loc_82577ACC:
  // lwz r11,116(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 116);
  // lwz r19,40(r26)
  r19 = REX_LOAD_U32(uint32_t(r26) + 40);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82578040
  if (cr6.eq) goto loc_82578040;
  // ld r24,136(r1)
  r24 = REX_LOAD_U64(uint32_t(r1) + 136);
loc_82577AE0:
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // mr r22,r14
  r22 = r14;
  // mr r21,r14
  r21 = r14;
  // andi. r11,r11,243
  r11 = r11 & 243;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stb r11,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r11));
  // b 0x82577afc
  goto loc_82577AFC;
loc_82577AF8:
  // lwz r28,332(r1)
  r28 = REX_LOAD_U32(uint32_t(r1) + 332);
loc_82577AFC:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x82577c1c
  if (!cr6.eq) goto loc_82577C1C;
  // lbz r11,85(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 85);
  // clrlwi r10,r22,16
  r10 = uint32_t(r22) & 0xFFFF;
  // mr r29,r14
  r29 = r14;
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // mr r3,r23
  r3 = r23;
  // blt cr6,0x82577b6c
  if (cr6.lt) goto loc_82577B6C;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x82577088
  // (left out: dead in C++)
  r3 = f82577088(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 12, so);
  // bne cr6,0x82577b58
  if (!cr6.eq) goto loc_82577B58;
  // cmpwi cr6,r21,0
  cr6 = lifted::Compare<int32_t>(int32_t(r21), 0, so);
  // bne cr6,0x82577fe0
  if (!cr6.eq) goto loc_82577FE0;
  // lbz r11,84(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 84);
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82577fe0
  if (cr0.eq) goto loc_82577FE0;
loc_82577B58:
  // lis r31,-32768
  r31 = uint64_t(int64_t(-2147483648));
  // ori r31,r31,16389
  r31 = r31 | 16389;
loc_82577B60:
  // mr r3,r31
  r3 = r31;
loc_82577B64:
  // addi r1,r1,304
  r1 = uint64_t(int64_t(int64_t(r1) + 304));
  // b 0x825a6ea0
  // (left out: dead in C++)
  return {r3, f1};
loc_82577B6C:
  // addi r4,r1,82
  r4 = uint64_t(int64_t(int64_t(r1) + 82));
  // bl 0x825784a8
  // (left out: dead in C++)
  r3 = f825784A8(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r11,82(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 82);
  // mr r30,r14
  r30 = r14;
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82577bdc
  if (cr0.eq) goto loc_82577BDC;
loc_82577B8C:
  // addi r4,r1,86
  r4 = uint64_t(int64_t(int64_t(r1) + 86));
  // mr r3,r23
  r3 = r23;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // addi r5,r1,108
  r5 = uint64_t(int64_t(int64_t(r1) + 108));
  // lhz r4,86(r1)
  r4 = REX_LOAD_U16(uint32_t(r1) + 86);
  // lwz r3,48(r26)
  r3 = REX_LOAD_U32(uint32_t(r26) + 48);
  // bl 0x82577528
  // (left out: dead in C++)
  r3 = f82577528(base, r3, r4, r5);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lwz r11,108(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 108);
  // cmplw cr6,r16,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r16), uint32_t(r11), so);
  // bne cr6,0x82577bcc
  if (!cr6.eq) goto loc_82577BCC;
  // mr r29,r18
  r29 = r18;
  // mr r21,r18
  r21 = r18;
loc_82577BCC:
  // lbz r11,82(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 82);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplw cr6,r30,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r11), so);
  // blt cr6,0x82577b8c
  if (cr6.lt) goto loc_82577B8C;
loc_82577BDC:
  // addi r4,r1,86
  r4 = uint64_t(int64_t(int64_t(r1) + 86));
  // mr r3,r23
  r3 = r23;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // bne cr6,0x82577c1c
  if (!cr6.eq) goto loc_82577C1C;
  // lhz r11,86(r1)
  r11 = REX_LOAD_U16(uint32_t(r1) + 86);
  // addi r5,r1,108
  r5 = uint64_t(int64_t(int64_t(r1) + 108));
  // addi r4,r1,136
  r4 = uint64_t(int64_t(int64_t(r1) + 136));
  // mr r3,r23
  r3 = r23;
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // bl 0x82578580
  // (left out: dead in C++)
  r3 = f82578580(base, r1, r13, r3, r4, r5);
loc_82577C10:
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // b 0x82577fe0
  goto loc_82577FE0;
loc_82577C1C:
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r30,40(r26)
  r30 = REX_LOAD_U32(uint32_t(r26) + 40);
  // mr r3,r23
  r3 = r23;
  // bl 0x82577088
  // (left out: dead in C++)
  r3 = f82577088(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm r10,r11,30,30,31
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 30) & 0x3;
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x82577cc0
  if (cr6.lt) goto loc_82577CC0;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // blt cr6,0x82577c54
  if (cr6.lt) goto loc_82577C54;
  // bne cr6,0x82577b58
  if (!cr6.eq) goto loc_82577B58;
  // b 0x82577fe0
  goto loc_82577FE0;
loc_82577C54:
  // mr r4,r30
  r4 = r30;
  // mr r3,r23
  r3 = r23;
  // bl 0x825780f0
  // (left out: dead in C++)
  r3 = f825780F0(base, r3, r4);
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm. r10,r11,0,26,26
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x82577cb8
  if (cr0.eq) goto loc_82577CB8;
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82577cb8
  if (!cr0.eq) goto loc_82577CB8;
  // cmplwi cr6,r20,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r20), 0, so);
  // beq cr6,0x82577cb0
  if (cr6.eq) goto loc_82577CB0;
  // lwz r11,0(r20)
  r11 = REX_LOAD_U32(uint32_t(r20) + 0);
  // stw r11,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r11));
loc_82577C84:
  // addi r5,r1,104
  r5 = uint64_t(int64_t(int64_t(r1) + 104));
  // addi r4,r28,1
  r4 = uint64_t(int64_t(int64_t(r28) + 1));
  // mr r3,r26
  r3 = r26;
  // bl 0x825778d8
  // (left out: dead in C++)
  { const lifted::Result o = f825778D8(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm. r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82577fc0
  if (cr0.eq) goto loc_82577FC0;
  // addi r20,r20,4
  r20 = uint64_t(int64_t(int64_t(r20) + 4));
  // b 0x82577fe0
  goto loc_82577FE0;
loc_82577CB0:
  // stw r14,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r14));
  // b 0x82577c84
  goto loc_82577C84;
loc_82577CB8:
  // stw r20,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r20));
  // b 0x82577c84
  goto loc_82577C84;
loc_82577CC0:
  // mr r27,r14
  r27 = r14;
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // mr r25,r14
  r25 = r14;
  // stw r27,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r27));
  // beq 0x82577d14
  if (cr0.eq) goto loc_82577D14;
  // addi r6,r1,92
  r6 = uint64_t(int64_t(int64_t(r1) + 92));
  // addi r5,r1,120
  r5 = uint64_t(int64_t(int64_t(r1) + 120));
  // mr r4,r28
  r4 = r28;
  // mr r3,r26
  r3 = r26;
  // bl 0x82577850
  // (left out: dead in C++)
  r3 = f82577850(base, r1, r13, r3, r4, r5, r6);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lwz r10,20(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 20);
  // lwz r9,8(r26)
  r9 = REX_LOAD_U32(uint32_t(r26) + 8);
  // lwz r11,120(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 120);
  // add r10,r10,r9
  r10 = r10 + r9;
  // lwz r27,92(r1)
  r27 = REX_LOAD_U32(uint32_t(r1) + 92);
  // clrldi r10,r10,32
  r10 = r10 & 0xFFFFFFFF;
  // ld r20,8(r11)
  r20 = REX_LOAD_U64(uint32_t(r11) + 8);
  // stw r15,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r15));
  // std r10,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r10);
loc_82577D14:
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82577d34
  if (cr0.eq) goto loc_82577D34;
  // addi r4,r1,100
  r4 = uint64_t(int64_t(int64_t(r1) + 100));
  // mr r3,r23
  r3 = r23;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
loc_82577D34:
  // lbz r4,80(r1)
  r4 = REX_LOAD_U8(uint32_t(r1) + 80);
  // mr r11,r4
  r11 = r4;
  // clrlwi r10,r11,30
  r10 = uint32_t(r11) & 0x3;
  // rlwinm. r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // slw r28,r18,r10
  r28 = uint8_t(r10) & 0x20 ? 0 : (uint32_t(r18) << (uint8_t(r10) & 0x3F));
  // beq 0x82577e10
  if (cr0.eq) goto loc_82577E10;
  // lwz r11,1080(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 1080);
  // addi r30,r26,56
  r30 = uint64_t(int64_t(int64_t(r26) + 56));
  // addi r7,r1,112
  r7 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r6,r1,88
  r6 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r5,r23
  r5 = r23;
  // mr r3,r30
  r3 = r30;
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
  // bl 0x825786b8
  // (left out: dead in C++)
  r3 = f825786B8(base, r1, r13, r3, r4, r5, r6, r7);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r11,88(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 88);
  // rlwinm. r10,r11,0,0,24
  r10 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82577da0
  if (!cr0.eq) goto loc_82577DA0;
  // lbz r10,80(r1)
  r10 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm. r10,r10,0,0,24
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x82577da0
  if (!cr0.eq) goto loc_82577DA0;
  // mr r29,r18
  r29 = r18;
  // cmplwi cr6,r20,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r20), 0, so);
  // beq cr6,0x82577e00
  if (cr6.eq) goto loc_82577E00;
  // lwz r3,0(r20)
  r3 = REX_LOAD_U32(uint32_t(r20) + 0);
  // b 0x82577da8
  goto loc_82577DA8;
loc_82577DA0:
  // mr r29,r14
  r29 = r14;
  // mr r3,r20
  r3 = r20;
loc_82577DA8:
  // stw r3,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r3));
  // rlwinm. r11,r11,0,25,25
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82577dd0
  if (cr0.eq) goto loc_82577DD0;
  // lwz r11,112(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 112);
  // mr r25,r18
  r25 = r18;
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82577dd0
  if (!cr6.eq) goto loc_82577DD0;
  // bl 0x825a9958
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A9958, LIFTED_ARGS(0x82577DC8)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r3,1
  r11 = uint64_t(int64_t(int64_t(r3) + 1));
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
loc_82577DD0:
  // lbz r11,88(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 88);
  // mr r3,r30
  r3 = r30;
  // clrlwi r4,r11,26
  r4 = uint32_t(r11) & 0x3F;
  // bl 0x82578658
  // (left out: dead in C++)
  r3 = f82578658(base, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lwz r4,104(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 104);
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // lwz r5,112(r1)
  r5 = REX_LOAD_U32(uint32_t(r1) + 112);
  // beq cr6,0x82577e08
  if (cr6.eq) goto loc_82577E08;
  // mr r28,r15
  r28 = r15;
  // b 0x82577e20
  goto loc_82577E20;
loc_82577E00:
  // mr r3,r14
  r3 = r14;
  // b 0x82577da8
  goto loc_82577DA8;
loc_82577E08:
  // mullw r28,r5,r28
  r28 = uint64_t(int64_t(int64_t(int32_t(r5)) * int64_t(int32_t(r28))));
  // b 0x82577e20
  goto loc_82577E20;
loc_82577E10:
  // mr r5,r18
  r5 = r18;
  // mr r4,r20
  r4 = r20;
  // stw r5,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r5));
  // stw r4,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r4));
loc_82577E20:
  // cmpwi cr6,r25,0
  cr6 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // beq cr6,0x82577e98
  if (cr6.eq) goto loc_82577E98;
  // addi r31,r26,8
  r31 = uint64_t(int64_t(int64_t(r26) + 8));
  // lis r3,0
  r3 = uint64_t(int64_t(0));
  // mr r6,r5
  r6 = r5;
  // mr r5,r4
  r5 = r4;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // lwz r8,0(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r30,8(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 8);
  // rotlwi r29,r11,0
  r29 = __builtin_rotateleft32(uint32_t(r11), 0);
  // add r7,r11,r8
  r7 = r11 + r8;
  // ori r3,r3,65001
  r3 = r3 | 65001;
  // subf r8,r29,r30
  r8 = r30 - r29;
  // bl 0x82296720
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82296720, LIFTED_ARGS(0x82577E64)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // bl 0x82578470
  // (left out: dead in C++)
  r3 = f82578470(base, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r11,88(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 88);
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // clrldi r5,r30,32
  r5 = r30 & 0xFFFFFFFF;
  // clrlwi r4,r11,26
  r4 = uint32_t(r11) & 0x3F;
  // addi r3,r26,56
  r3 = uint64_t(int64_t(int64_t(r26) + 56));
  // bl 0x825787d0
  // (left out: dead in C++)
  r3 = f825787D0(base, r1, r13, r3, r4, r5, r6);
  // b 0x82577f20
  goto loc_82577F20;
loc_82577E98:
  // mr r31,r14
  r31 = r14;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x82577f30
  if (cr6.eq) goto loc_82577F30;
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // clrlwi r11,r11,30
  r11 = uint32_t(r11) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82577f0c
  if (cr6.lt) goto loc_82577F0C;
  // beq cr6,0x82577ef4
  if (cr6.eq) goto loc_82577EF4;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82577edc
  if (cr6.lt) goto loc_82577EDC;
  // bne cr6,0x82577f30
  if (!cr6.eq) goto loc_82577F30;
  // ld r24,0(r4)
  r24 = REX_LOAD_U64(uint32_t(r4) + 0);
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bne cr6,0x82577f30
  if (!cr6.eq) goto loc_82577F30;
  // addi r3,r26,8
  r3 = uint64_t(int64_t(int64_t(r26) + 8));
  // bl 0x825783c8
  // (left out: dead in C++)
  r3 = f825783C8(base, r1, r3, r4, r5);
  // b 0x82577f20
  goto loc_82577F20;
loc_82577EDC:
  // lwz r24,0(r4)
  r24 = REX_LOAD_U32(uint32_t(r4) + 0);
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bne cr6,0x82577f30
  if (!cr6.eq) goto loc_82577F30;
  // addi r3,r26,8
  r3 = uint64_t(int64_t(int64_t(r26) + 8));
  // bl 0x82578340
  // (left out: dead in C++)
  r3 = f82578340(base, r3, r4, r5);
  // b 0x82577f20
  goto loc_82577F20;
loc_82577EF4:
  // lhz r24,0(r4)
  r24 = REX_LOAD_U16(uint32_t(r4) + 0);
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bne cr6,0x82577f30
  if (!cr6.eq) goto loc_82577F30;
  // addi r3,r26,8
  r3 = uint64_t(int64_t(int64_t(r26) + 8));
  // bl 0x825782c0
  // (left out: dead in C++)
  r3 = f825782C0(base, r3, r4, r5);
  // b 0x82577f20
  goto loc_82577F20;
loc_82577F0C:
  // lbz r24,0(r4)
  r24 = REX_LOAD_U8(uint32_t(r4) + 0);
  // cmpwi cr6,r27,0
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // bne cr6,0x82577f30
  if (!cr6.eq) goto loc_82577F30;
  // addi r3,r26,8
  r3 = uint64_t(int64_t(int64_t(r26) + 8));
  // bl 0x82578248
  // (left out: dead in C++)
  r3 = f82578248(base, r1, r13, r3, r4, r5);
loc_82577F20:
  // mr r31,r3
  r31 = r3;
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt cr6,0x82577b60
  if (cr6.lt) goto loc_82577B60;
  // lwz r4,104(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 104);
loc_82577F30:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x82577f44
  if (!cr6.eq) goto loc_82577F44;
  // lhz r28,90(r1)
  r28 = REX_LOAD_U16(uint32_t(r1) + 90);
loc_82577F44:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x82577f50
  if (cr6.eq) goto loc_82577F50;
  // add r20,r28,r20
  r20 = r28 + r20;
loc_82577F50:
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm. r11,r11,0,27,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x10;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82577fe0
  if (cr0.eq) goto loc_82577FE0;
  // addi r4,r1,124
  r4 = uint64_t(int64_t(int64_t(r1) + 124));
  // mr r3,r23
  r3 = r23;
  // bl 0x825770b8
  // (left out: dead in C++)
  r3 = f825770B8(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // addi r5,r1,108
  r5 = uint64_t(int64_t(int64_t(r1) + 108));
  // lhz r4,126(r1)
  r4 = REX_LOAD_U16(uint32_t(r1) + 126);
  // lwz r3,48(r26)
  r3 = REX_LOAD_U32(uint32_t(r26) + 48);
  // bl 0x82577528
  // (left out: dead in C++)
  r3 = f82577528(base, r3, r4, r5);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // lbz r11,124(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 124);
  // addi r8,r1,124
  r8 = uint64_t(int64_t(int64_t(r1) + 124));
  // lwz r9,20(r26)
  r9 = REX_LOAD_U32(uint32_t(r26) + 20);
  // mr r6,r24
  r6 = r24;
  // rlwinm r5,r11,26,6,31
  r5 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 26) & 0x3FFFFFF;
  // lwz r10,8(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 8);
  // clrlwi r4,r11,26
  r4 = uint32_t(r11) & 0x3F;
  // lwz r7,108(r1)
  r7 = REX_LOAD_U32(uint32_t(r1) + 108);
  // addi r3,r26,56
  r3 = uint64_t(int64_t(int64_t(r26) + 56));
  // slw r11,r18,r5
  r11 = uint8_t(r5) & 0x20 ? 0 : (uint32_t(r18) << (uint8_t(r5) & 0x3F));
  // subf r11,r11,r9
  r11 = r9 - r11;
  // add r9,r11,r10
  r9 = r11 + r10;
  // bl 0x82578588
  // (left out: dead in C++)
  r3 = f82578588(base, r3, r4, r5, r6, r8, r9);
  // b 0x82577c10
  goto loc_82577C10;
loc_82577FC0:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x82577fdc
  if (!cr6.eq) goto loc_82577FDC;
  // lhz r11,90(r1)
  r11 = REX_LOAD_U16(uint32_t(r1) + 90);
  // add r20,r11,r20
  r20 = r11 + r20;
  // b 0x82577fe0
  goto loc_82577FE0;
loc_82577FDC:
  // lwz r20,104(r1)
  r20 = REX_LOAD_U32(uint32_t(r1) + 104);
loc_82577FE0:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 8, so);
  // bne cr6,0x82577ffc
  if (!cr6.eq) goto loc_82577FFC;
  // clrlwi r11,r22,16
  r11 = uint32_t(r22) & 0xFFFF;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r22,r11,16
  r22 = uint32_t(r11) & 0xFFFF;
loc_82577FFC:
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // rlwinm r11,r11,0,28,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xC;
  // cmplwi cr6,r11,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 12, so);
  // bne cr6,0x82577af8
  if (!cr6.eq) goto loc_82577AF8;
  // lwz r11,116(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 116);
  // addi r17,r17,1
  r17 = uint64_t(int64_t(int64_t(r17) + 1));
  // cmplw cr6,r17,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r17), uint32_t(r11), so);
  // bge cr6,0x8257803c
  if (!cr6.lt) goto loc_8257803C;
  // mr r4,r19
  r4 = r19;
  // mr r3,r23
  r3 = r23;
  // bl 0x825780f0
  // (left out: dead in C++)
  r3 = f825780F0(base, r3, r4);
  // lwz r11,116(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 116);
  // cmplw cr6,r17,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r17), uint32_t(r11), so);
  // bge cr6,0x8257803c
  if (!cr6.lt) goto loc_8257803C;
  // lwz r28,332(r1)
  r28 = REX_LOAD_U32(uint32_t(r1) + 332);
  // b 0x82577ae0
  goto loc_82577AE0;
loc_8257803C:
  // lwz r29,340(r1)
  r29 = REX_LOAD_U32(uint32_t(r1) + 340);
loc_82578040:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // rlwinm. r11,r11,0,26,26
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x20;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x82578064
  if (cr0.eq) goto loc_82578064;
  // lbz r11,96(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 96);
  // addi r3,r26,56
  r3 = uint64_t(int64_t(int64_t(r26) + 56));
  // clrlwi r4,r11,26
  r4 = uint32_t(r11) & 0x3F;
  // bl 0x82578658
  // (left out: dead in C++)
  r3 = f82578658(base, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
loc_82578064:
  // lwz r11,116(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 116);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82578098
  if (!cr6.eq) goto loc_82578098;
  // mr r3,r23
  r3 = r23;
  // lwz r4,132(r1)
  r4 = REX_LOAD_U32(uint32_t(r1) + 132);
  // bl 0x825780f0
  // (left out: dead in C++)
  r3 = f825780F0(base, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r23
  r3 = r23;
  // bl 0x82577190
  // (left out: dead in C++)
  r3 = f82577190(base, r1, r13, r3, r4);
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // blt 0x82577b60
  if (cr0.lt) goto loc_82577B60;
loc_82578098:
  // stw r20,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r20));
  // b 0x82577b60
  goto loc_82577B60;
}

// sub_825780A0: lifted from generated/ by lift.py.
lifted::Result f825780A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_825780A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x825778d8
  // (left out: dead in C++)
  { const lifted::Result o = f825778D8(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_825780D0: lifted from generated/ by lift.py.
void f825780D0(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825780D0 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r4,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r4));
  // stw r5,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r5));
  // stw r6,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r6));
  // stw r7,16(r3)
  REX_STORE_U32(uint32_t(r3) + 16, uint32_t(r7));
  // stw r11,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r11));
  // blr
  return;
}

// sub_825780F0: lifted from generated/ by lift.py.
uint64_t f825780F0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825780F0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplw cr6,r4,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r10), so);
  // ble cr6,0x8257810c
  if (!cr6.gt) goto loc_8257810C;
  // lis r3,-32744
  r3 = uint64_t(int64_t(-2145910784));
  // ori r3,r3,8193
  r3 = r3 | 8193;
  // blr
  return r3;
loc_8257810C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r4,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r4));
  // blr
  return r3;
}

// sub_82578118: lifted from generated/ by lift.py.
uint64_t f82578118(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82578118 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // lwz r8,0(r5)
  r8 = REX_LOAD_U32(uint32_t(r5) + 0);
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // bge cr6,0x82578144
  if (!cr6.lt) goto loc_82578144;
  // lis r3,-32744
  r3 = uint64_t(int64_t(-2145910784));
  // stw r9,0(r5)
  REX_STORE_U32(uint32_t(r5) + 0, uint32_t(r9));
  // ori r3,r3,8193
  r3 = r3 | 8193;
  // blr
  return r3;
loc_82578144:
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r10,r10,r9
  r10 = r10 + r9;
  // stw r10,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r10));
  // lwz r10,0(r5)
  r10 = REX_LOAD_U32(uint32_t(r5) + 0);
  // lwz r9,12(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 12);
  // add r10,r10,r9
  r10 = r10 + r9;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // blr
  return r3;
}

// sub_82578248: lifted from generated/ by lift.py.
uint64_t f82578248(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82578248 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r5
  r30 = r5;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // cmplw cr6,r10,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r30), so);
  // bge cr6,0x82578284
  if (!cr6.lt) goto loc_82578284;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,122
  r3 = r3 | 122;
  // b 0x825782a4
  goto loc_825782A4;
loc_82578284:
  // lwz r10,0(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r5,r30
  r5 = r30;
  // add r3,r11,r10
  r3 = r11 + r10;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x82578294)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // add r11,r30,r11
  r11 = r30 + r11;
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
loc_825782A4:
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

// sub_825782C0: lifted from generated/ by lift.py.
uint64_t f825782C0(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825782C0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // rlwinm r9,r5,1,0,30
  r9 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 1) & 0xFFFFFFFE;
  // lwz r11,12(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 12);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bge cr6,0x825782e4
  if (!cr6.lt) goto loc_825782E4;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,122
  r3 = r3 | 122;
  // blr
  return r3;
loc_825782E4:
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // add r10,r11,r10
  r10 = r11 + r10;
  // beq cr6,0x82578334
  if (cr6.eq) goto loc_82578334;
loc_825782F4:
  // lwz r11,16(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 16);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82578310
  if (cr6.eq) goto loc_82578310;
  // lhz r9,0(r4)
  r9 = REX_LOAD_U16(uint32_t(r4) + 0);
  // rlwinm r11,r9,24,24,31
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 24) & 0xFF;
  // rlwimi r11,r9,8,16,23
  r11 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 8) & 0xFF00) | (r11 & 0xFFFFFFFFFFFF00FF);
  // b 0x82578314
  goto loc_82578314;
loc_82578310:
  // lhz r11,0(r4)
  r11 = REX_LOAD_U16(uint32_t(r4) + 0);
loc_82578314:
  // sth r11,0(r10)
  REX_STORE_U16(uint32_t(r10) + 0, uint16_t(r11));
  // addi r4,r4,2
  r4 = uint64_t(int64_t(int64_t(r4) + 2));
  // lwz r11,12(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 12);
  // addic. r5,r5,-1
  ca = uint32_t(r5) > 0;
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // addi r10,r10,2
  r10 = uint64_t(int64_t(int64_t(r10) + 2));
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // stw r11,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r11));
  // bne 0x825782f4
  if (!cr0.eq) goto loc_825782F4;
loc_82578334:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82578340: lifted from generated/ by lift.py.
uint64_t f82578340(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82578340 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // rlwinm r9,r5,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,12(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 12);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bge cr6,0x82578364
  if (!cr6.lt) goto loc_82578364;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,122
  r3 = r3 | 122;
  // blr
  return r3;
loc_82578364:
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // add r10,r11,r10
  r10 = r11 + r10;
  // beq cr6,0x825783bc
  if (cr6.eq) goto loc_825783BC;
loc_82578374:
  // lwz r11,16(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 16);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82578398
  if (cr6.eq) goto loc_82578398;
  // lwz r9,0(r4)
  r9 = REX_LOAD_U32(uint32_t(r4) + 0);
  // rlwinm r11,r9,8,24,31
  r11 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 8) & 0xFF;
  // rlwimi r11,r9,24,16,23
  r11 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 24) & 0xFF00) | (r11 & 0xFFFFFFFFFFFF00FF);
  // rlwimi r11,r9,8,8,15
  r11 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 8) & 0xFF0000) | (r11 & 0xFFFFFFFFFF00FFFF);
  // rlwimi r11,r9,24,0,7
  r11 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 24) & 0xFF000000) | (r11 & 0xFFFFFFFF00FFFFFF);
  // b 0x8257839c
  goto loc_8257839C;
loc_82578398:
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
loc_8257839C:
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // addi r4,r4,4
  r4 = uint64_t(int64_t(int64_t(r4) + 4));
  // lwz r11,12(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 12);
  // addic. r5,r5,-1
  ca = uint32_t(r5) > 0;
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // stw r11,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r11));
  // bne 0x82578374
  if (!cr0.eq) goto loc_82578374;
loc_825783BC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_825783C8: lifted from generated/ by lift.py.
uint64_t f825783C8(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_825783C8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // rlwinm r9,r5,3,0,28
  r9 = __builtin_rotateleft64(uint32_t(r5) | (r5 << 32), 3) & 0xFFFFFFF8;
  // lwz r11,12(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 12);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bge cr6,0x825783ec
  if (!cr6.lt) goto loc_825783EC;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,122
  r3 = r3 | 122;
  // blr
  return r3;
loc_825783EC:
  // lwz r10,0(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r5,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 0, so);
  // add r11,r11,r10
  r11 = r11 + r10;
  // beq cr6,0x82578464
  if (cr6.eq) goto loc_82578464;
loc_825783FC:
  // lwz r10,16(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 16);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ld r10,0(r4)
  r10 = REX_LOAD_U64(uint32_t(r4) + 0);
  // addi r4,r4,8
  r4 = uint64_t(int64_t(int64_t(r4) + 8));
  // beq cr6,0x82578448
  if (cr6.eq) goto loc_82578448;
  // std r10,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r10);
  // lwz r10,-12(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + -12);
  // lwz r9,-16(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + -16);
  // rlwinm r8,r10,8,24,31
  r8 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFF;
  // rlwinm r7,r9,8,24,31
  r7 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 8) & 0xFF;
  // rlwimi r8,r10,24,16,23
  r8 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFF00) | (r8 & 0xFFFFFFFFFFFF00FF);
  // rlwimi r7,r9,24,16,23
  r7 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 24) & 0xFF00) | (r7 & 0xFFFFFFFFFFFF00FF);
  // rlwimi r8,r10,8,8,15
  r8 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 8) & 0xFF0000) | (r8 & 0xFFFFFFFFFF00FFFF);
  // rlwimi r7,r9,8,8,15
  r7 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 8) & 0xFF0000) | (r7 & 0xFFFFFFFFFF00FFFF);
  // rlwimi r8,r10,24,0,7
  r8 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 24) & 0xFF000000) | (r8 & 0xFFFFFFFF00FFFFFF);
  // rlwimi r7,r9,24,0,7
  r7 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 24) & 0xFF000000) | (r7 & 0xFFFFFFFF00FFFFFF);
  // rldicr r10,r8,32,31
  r10 = __builtin_rotateleft64(r8, 32) & 0xFFFFFFFF00000000;
  // clrldi r9,r7,32
  r9 = r7 & 0xFFFFFFFF;
  // or r10,r10,r9
  r10 = r10 | r9;
loc_82578448:
  // std r10,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r10);
  // addic. r5,r5,-1
  ca = uint32_t(r5) > 0;
  r5 = uint64_t(int64_t(int64_t(r5) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // lwz r10,12(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 12);
  // addi r11,r11,8
  r11 = uint64_t(int64_t(int64_t(r11) + 8));
  // addi r10,r10,8
  r10 = uint64_t(int64_t(int64_t(r10) + 8));
  // stw r10,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r10));
  // bne 0x825783fc
  if (!cr0.eq) goto loc_825783FC;
loc_82578464:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82578500: lifted from generated/ by lift.py.
uint64_t f82578500(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82578500 (lifted)");
  uint64_t r5 = 0, r6 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // lwz r6,16(r3)
  r6 = REX_LOAD_U32(uint32_t(r3) + 16);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x82578168
  // (left out: dead in C++)
  r3 = f82578168(base, r1, r13, r3, r4, r5, r6);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_82578580: lifted from generated/ by lift.py.
uint64_t f82578580(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82578580 (lifted)");
  double f1 = 0.0;
  // b 0x82578118
  r3 = f82578118(base, r3, r4, r5);
  return r3;
}

// sub_82578588: lifted from generated/ by lift.py.
uint64_t f82578588(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_82578588 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // cmplwi cr6,r4,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 64, so);
  // bge cr6,0x825785f8
  if (!cr6.lt) goto loc_825785F8;
  // cmplwi cr6,r5,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r5), 3, so);
  // bgt cr6,0x825785f8
  if (cr6.gt) goto loc_825785F8;
  // rlwinm r11,r4,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 4) & 0xFFFFFFF0;
  // add r11,r11,r3
  r11 = r11 + r3;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm. r10,r10,0,1,1
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x40000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x825785b8
  if (cr0.eq) goto loc_825785B8;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // blr
  return r3;
loc_825785B8:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x825785e0
  if (cr6.eq) goto loc_825785E0;
  // lbz r8,1(r8)
  r8 = REX_LOAD_U8(uint32_t(r8) + 1);
  // clrlwi. r8,r8,31
  r8 = uint32_t(r8) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x825785e0
  if (cr0.eq) goto loc_825785E0;
  // slw r8,r10,r5
  r8 = uint8_t(r5) & 0x20 ? 0 : (uint32_t(r10) << (uint8_t(r5) & 0x3F));
  // extsw r8,r8
  r8 = uint64_t(int64_t(int32_t(r8)));
  // divdu r6,r6,r8
  r6 = r8 ? r6 / r8 : 0;
  // tdllei r8,0
  if (int64_t(r8) == 0ll || r8 < 0ull) lifted::Trap(0);
loc_825785E0:
  // rlwimi r5,r10,31,0,29
  r5 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 31) & 0xFFFFFFFC) | (r5 & 0xFFFFFFFF00000003);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // std r6,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r6);
  // stw r5,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r5));
  // blr
  return r3;
loc_825785F8:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // blr
  return r3;
}

// sub_82578608: lifted from generated/ by lift.py.
uint64_t f82578608(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82578608 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // cmplwi cr6,r4,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 64, so);
  // blt cr6,0x8257861c
  if (cr6.lt) goto loc_8257861C;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // blr
  return r3;
loc_8257861C:
  // rlwinm r11,r4,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 4) & 0xFFFFFFF0;
  // add r11,r11,r3
  r11 = r11 + r3;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm. r9,r10,0,0,0
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x80000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x8257864c
  if (cr0.eq) goto loc_8257864C;
  // rlwinm. r9,r10,0,1,1
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 0) & 0x40000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x8257864c
  if (!cr0.eq) goto loc_8257864C;
  // oris r10,r10,16384
  r10 = r10 | 1073741824;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r11,0(r5)
  REX_STORE_U32(uint32_t(r5) + 0, uint32_t(r11));
  // blr
  return r3;
loc_8257864C:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // blr
  return r3;
}

// sub_82578658: lifted from generated/ by lift.py.
uint64_t f82578658(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82578658 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // mr r9,r3
  r9 = r3;
  // cmplwi cr6,r4,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 64, so);
  // blt cr6,0x82578670
  if (cr6.lt) goto loc_82578670;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // blr
  return r3;
loc_82578670:
  // cmplwi cr6,r4,63
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 63, so);
  // bne cr6,0x82578680
  if (!cr6.eq) goto loc_82578680;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_82578680:
  // rlwinm r10,r4,4,0,27
  r10 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 4) & 0xFFFFFFF0;
  // lwzx r11,r10,r9
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r9));
  // rlwinm. r8,r11,0,0,0
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x80000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x825786a8
  if (cr0.eq) goto loc_825786A8;
  // rlwinm. r8,r11,0,1,1
  r8 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0x40000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x825786a8
  if (cr0.eq) goto loc_825786A8;
  // rlwinm r11,r11,0,2,0
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFFFFBFFFFFFF;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stwx r11,r10,r9
  REX_STORE_U32(uint32_t(r10) + uint32_t(r9), uint32_t(r11));
  // blr
  return r3;
loc_825786A8:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // blr
  return r3;
}

// sub_825786B8: lifted from generated/ by lift.py.
uint64_t f825786B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7) {
  ITC_NATIVE_RUNNING("sub_825786B8 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r31,r5
  r31 = r5;
  // mr r29,r6
  r29 = r6;
  // mr r28,r3
  r28 = r3;
  // mr r30,r4
  r30 = r4;
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // mr r27,r7
  r27 = r7;
  // bl 0x82577110
  // (left out: dead in C++)
  r3 = f82577110(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x825787c4
  if (cr0.lt) goto loc_825787C4;
  // lbz r11,0(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 0);
  // clrlwi r10,r11,26
  r10 = uint32_t(r11) & 0x3F;
  // cmplwi cr6,r10,63
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 63, so);
  // beq cr6,0x825787c4
  if (cr6.eq) goto loc_825787C4;
  // rlwinm. r11,r11,0,0,24
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xFFFFFF80;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8257879c
  if (cr0.eq) goto loc_8257879C;
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // bl 0x82578500
  // (left out: dead in C++)
  r3 = f82578500(base, r1, r13, r3, r4);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x825787c4
  if (cr0.lt) goto loc_825787C4;
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // lhz r4,80(r1)
  r4 = REX_LOAD_U16(uint32_t(r1) + 80);
  // lwz r3,20(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 20);
  // bl 0x82577528
  // (left out: dead in C++)
  r3 = f82577528(base, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x825787c4
  if (cr0.lt) goto loc_825787C4;
  // lbz r11,0(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 0);
  // clrlwi r10,r30,30
  r10 = uint32_t(r30) & 0x3;
  // clrlwi r11,r11,26
  r11 = uint32_t(r11) & 0x3F;
  // cmplwi cr6,r11,64
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 64, so);
  // bge cr6,0x8257878c
  if (!cr6.lt) goto loc_8257878C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bgt cr6,0x8257878c
  if (cr6.gt) goto loc_8257878C;
  // rlwinm r11,r11,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 4) & 0xFFFFFFF0;
  // add r11,r11,r28
  r11 = r11 + r28;
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // rlwinm. r9,r9,0,1,1
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 0) & 0x40000000;
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // beq 0x8257876c
  if (cr0.eq) goto loc_8257876C;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // b 0x82578794
  goto loc_82578794;
loc_8257876C:
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // rlwimi r10,r9,31,0,29
  r10 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 31) & 0xFFFFFFFC) | (r10 & 0xFFFFFFFF00000003);
  // lwz r9,84(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 84);
  // stw r3,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r3));
  // std r9,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r9);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x82578794
  goto loc_82578794;
loc_8257878C:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
loc_82578794:
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt cr6,0x825787c4
  if (cr6.lt) goto loc_825787C4;
loc_8257879C:
  // lbz r11,0(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 0);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // clrlwi r4,r11,26
  r4 = uint32_t(r11) & 0x3F;
  // bl 0x82578608
  // (left out: dead in C++)
  r3 = f82578608(base, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x825787c4
  if (cr0.lt) goto loc_825787C4;
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
loc_825787C4:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_825787D0: lifted from generated/ by lift.py.
uint64_t f825787D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_825787D0 (lifted)");
  uint64_t r7 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // rlwinm r11,r4,4,0,27
  r11 = __builtin_rotateleft64(uint32_t(r4) | (r4 << 32), 4) & 0xFFFFFFF0;
  // std r5,176(r1)
  REX_STORE_U64(uint32_t(r1) + 176, r5);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // add r31,r11,r3
  r31 = r11 + r3;
  // mr r7,r6
  r7 = r6;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // clrlwi r11,r11,30
  r11 = uint32_t(r11) & 0x3;
  // slw r6,r10,r11
  r6 = uint8_t(r11) & 0x20 ? 0 : (uint32_t(r10) << (uint8_t(r11) & 0x3F));
  // mr r5,r6
  r5 = r6;
  // stw r6,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r6));
  // bl 0x825780d0
  // (left out: dead in C++)
  f825780D0(base, r3, r4, r5, r6, r7);
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // clrlwi r11,r11,30
  r11 = uint32_t(r11) & 0x3;
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x82578888
  if (cr6.lt) goto loc_82578888;
  // beq cr6,0x8257886c
  if (cr6.eq) goto loc_8257886C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x82578850
  if (cr6.lt) goto loc_82578850;
  // bne cr6,0x825788a4
  if (!cr6.eq) goto loc_825788A4;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r1,176
  r4 = uint64_t(int64_t(int64_t(r1) + 176));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x825783c8
  // (left out: dead in C++)
  r3 = f825783C8(base, r1, r3, r4, r5);
  // b 0x825788a0
  goto loc_825788A0;
loc_82578850:
  // ld r11,176(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 176);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x82578340
  // (left out: dead in C++)
  r3 = f82578340(base, r3, r4, r5);
  // b 0x825788a0
  goto loc_825788A0;
loc_8257886C:
  // ld r11,176(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 176);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r1,82
  r4 = uint64_t(int64_t(int64_t(r1) + 82));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // sth r11,82(r1)
  REX_STORE_U16(uint32_t(r1) + 82, uint16_t(r11));
  // bl 0x825782c0
  // (left out: dead in C++)
  r3 = f825782C0(base, r3, r4, r5);
  // b 0x825788a0
  goto loc_825788A0;
loc_82578888:
  // ld r11,176(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 176);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // stb r11,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r11));
  // bl 0x82578248
  // (left out: dead in C++)
  r3 = f82578248(base, r1, r13, r3, r4, r5);
loc_825788A0:
  // mr r30,r3
  r30 = r3;
loc_825788A4:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
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

// sub_825788C0: lifted from generated/ by lift.py.
lifted::Result f825788C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825788C0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r27,r3
  r27 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825788fc
  if (cr6.eq) goto loc_825788FC;
  // lbz r10,0(r27)
  r10 = REX_LOAD_U8(uint32_t(r27) + 0);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x825788fc
  if (cr0.eq) goto loc_825788FC;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_825788EC:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x825788ec
  if (cr6.lt) goto loc_825788EC;
loc_825788FC:
  // lis r11,-32154
  r11 = uint64_t(int64_t(-2107244544));
  // addi r28,r11,-11204
  r28 = uint64_t(int64_t(int64_t(r11) + -11204));
  // lbz r4,0(r28)
  r4 = REX_LOAD_U8(uint32_t(r28) + 0);
  // cmplwi r4,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq 0x82578988
  if (cr0.eq) goto loc_82578988;
  // lwz r5,4(r28)
  r5 = REX_LOAD_U32(uint32_t(r28) + 4);
  // li r8,0
  r8 = uint64_t(int64_t(0));
loc_82578918:
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8257896c
  if (cr6.eq) goto loc_8257896C;
  // lbz r7,0(r27)
  r7 = REX_LOAD_U8(uint32_t(r27) + 0);
  // cmplwi r7,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r7), 0, so);
  // beq 0x8257896c
  if (cr0.eq) goto loc_8257896C;
  // mulli r9,r8,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r8 * static_cast<uint64_t>(12))));
  // lwz r10,4(r27)
  r10 = REX_LOAD_U32(uint32_t(r27) + 4);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lbzx r9,r9,r5
  r9 = REX_LOAD_U8(uint32_t(r9) + uint32_t(r5));
loc_82578940:
  // mulli r3,r11,12
  r3 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // lbzx r3,r3,r10
  r3 = REX_LOAD_U8(uint32_t(r3) + uint32_t(r10));
  // cmplw cr6,r3,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r9), so);
  // bne cr6,0x82578954
  if (!cr6.eq) goto loc_82578954;
  // li r6,1
  r6 = uint64_t(int64_t(1));
loc_82578954:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r11,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r7), so);
  // blt cr6,0x82578940
  if (cr6.lt) goto loc_82578940;
  // cmpwi cr6,r6,0
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 0, so);
  // bne cr6,0x82578978
  if (!cr6.eq) goto loc_82578978;
loc_8257896C:
  // clrlwi r11,r30,24
  r11 = uint32_t(r30) & 0xFF;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r30,r11,24
  r30 = uint32_t(r11) & 0xFF;
loc_82578978:
  // addi r11,r8,1
  r11 = uint64_t(int64_t(int64_t(r8) + 1));
  // clrlwi r8,r11,24
  r8 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r8,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r4), so);
  // blt cr6,0x82578918
  if (cr6.lt) goto loc_82578918;
loc_82578988:
  // lis r11,24714
  r11 = uint64_t(int64_t(1619656704));
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // ori r31,r11,8194
  r31 = r11 | 8194;
  // mr r4,r31
  r4 = r31;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x8257899C)); r3 = o.r3; f1 = o.f1; }
  // mr. r29,r3
  r29 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq 0x82578ae0
  if (cr0.eq) goto loc_82578AE0;
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x825789b4
  if (cr6.eq) goto loc_825789B4;
  // lbz r11,0(r27)
  r11 = REX_LOAD_U8(uint32_t(r27) + 0);
  // stb r11,0(r29)
  REX_STORE_U8(uint32_t(r29) + 0, uint8_t(r11));
loc_825789B4:
  // lbz r11,0(r29)
  r11 = REX_LOAD_U8(uint32_t(r29) + 0);
  // clrlwi r10,r30,24
  r10 = uint32_t(r30) & 0xFF;
  // mr r4,r31
  r4 = r31;
  // add r11,r11,r10
  r11 = r11 + r10;
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // mr r10,r11
  r10 = r11;
  // mulli r3,r10,12
  r3 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // stb r11,0(r29)
  REX_STORE_U8(uint32_t(r29) + 0, uint8_t(r11));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825789D8)); r3 = o.r3; f1 = o.f1; }
  // stw r3,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82578ab4
  if (cr0.eq) goto loc_82578AB4;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x82578a10
  if (cr6.eq) goto loc_82578A10;
  // lbz r11,0(r27)
  r11 = REX_LOAD_U8(uint32_t(r27) + 0);
  // lwz r4,4(r27)
  r4 = REX_LOAD_U32(uint32_t(r27) + 4);
  // mr r31,r11
  r31 = r11;
  // cmplwi r4,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq 0x82578a10
  if (cr0.eq) goto loc_82578A10;
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // mulli r5,r11,12
  r5 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x82578A10)); r3 = o.r3; f1 = o.f1; }
loc_82578A10:
  // lbz r26,0(r28)
  r26 = REX_LOAD_U8(uint32_t(r28) + 0);
  // cmplwi r26,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r26), 0, so);
  // beq 0x82578ae0
  if (cr0.eq) goto loc_82578AE0;
  // lwz r28,4(r28)
  r28 = REX_LOAD_U32(uint32_t(r28) + 4);
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_82578A24:
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x82578a78
  if (cr6.eq) goto loc_82578A78;
  // lbz r8,0(r27)
  r8 = REX_LOAD_U8(uint32_t(r27) + 0);
  // cmplwi r8,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq 0x82578a78
  if (cr0.eq) goto loc_82578A78;
  // mulli r9,r30,12
  r9 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(12))));
  // lwz r10,4(r27)
  r10 = REX_LOAD_U32(uint32_t(r27) + 4);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lbzx r9,r9,r28
  r9 = REX_LOAD_U8(uint32_t(r9) + uint32_t(r28));
loc_82578A4C:
  // mulli r6,r11,12
  r6 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(12))));
  // lbzx r6,r6,r10
  r6 = REX_LOAD_U8(uint32_t(r6) + uint32_t(r10));
  // cmplw cr6,r6,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r6), uint32_t(r9), so);
  // bne cr6,0x82578a60
  if (!cr6.eq) goto loc_82578A60;
  // li r7,1
  r7 = uint64_t(int64_t(1));
loc_82578A60:
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // clrlwi r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // blt cr6,0x82578a4c
  if (cr6.lt) goto loc_82578A4C;
  // cmpwi cr6,r7,0
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // bne cr6,0x82578aa0
  if (!cr6.eq) goto loc_82578AA0;
loc_82578A78:
  // mulli r11,r30,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(12))));
  // lwz r10,4(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 4);
  // clrlwi r31,r31,24
  r31 = uint32_t(r31) & 0xFF;
  // add r4,r11,r28
  r4 = r11 + r28;
  // mulli r11,r31,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r31 * static_cast<uint64_t>(12))));
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // add r3,r11,r10
  r3 = r11 + r10;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x82578A98)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r31,1
  r11 = uint64_t(int64_t(int64_t(r31) + 1));
  // clrlwi r31,r11,24
  r31 = uint32_t(r11) & 0xFF;
loc_82578AA0:
  // addi r11,r30,1
  r11 = uint64_t(int64_t(int64_t(r30) + 1));
  // clrlwi r30,r11,24
  r30 = uint32_t(r11) & 0xFF;
  // cmplw cr6,r30,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r26), so);
  // blt cr6,0x82578a24
  if (cr6.lt) goto loc_82578A24;
  // b 0x82578ae0
  goto loc_82578AE0;
loc_82578AB4:
  // lwz r3,4(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82578ad0
  if (cr0.eq) goto loc_82578AD0;
  // mr r4,r31
  r4 = r31;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x82578AC8)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r11));
loc_82578AD0:
  // mr r4,r31
  r4 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x82578ADC)); r3 = o.r3; f1 = o.f1; }
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_82578AE0:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82578AF0: lifted from generated/ by lift.py.
lifted::Result f82578AF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82578AF0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lwz r11,0(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 0);
  // mr r30,r3
  r30 = r3;
  // lwz r3,4(r5)
  r3 = REX_LOAD_U32(uint32_t(r5) + 4);
  // mr r31,r4
  r31 = r4;
  // mr r29,r6
  r29 = r6;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,4(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 4);
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // lwz r11,8(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 8);
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // bl 0x825788c0
  // (left out: dead in C++)
  { const lifted::Result o = f825788C0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // stw r3,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r3));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x82578b40
  if (!cr0.eq) goto loc_82578B40;
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // ori r30,r30,14
  r30 = r30 | 14;
  // b 0x82578bec
  goto loc_82578BEC;
loc_82578B40:
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // bge cr6,0x82578b54
  if (!cr6.lt) goto loc_82578B54;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stb r11,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r11));
loc_82578B54:
  // lbz r11,81(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 81);
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x82578b68
  if (!cr6.lt) goto loc_82578B68;
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stb r11,81(r1)
  REX_STORE_U8(uint32_t(r1) + 81, uint8_t(r11));
loc_82578B68:
  // addi r3,r30,4
  r3 = uint64_t(int64_t(int64_t(r30) + 4));
  // b 0x82578b80
  goto loc_82578B80;
loc_82578B70:
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82578b88
  if (!cr6.eq) goto loc_82578B88;
  // addi r3,r3,4
  r3 = uint64_t(int64_t(int64_t(r3) + 4));
loc_82578B80:
  // cmplw cr6,r3,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r31), so);
  // blt cr6,0x82578b70
  if (cr6.lt) goto loc_82578B70;
loc_82578B88:
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r6,r29
  r6 = r29;
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r4,r31
  r4 = r31;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82578BA0)); r3 = o.r3; f1 = o.f1; }
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge 0x82578bec
  if (!cr0.lt) goto loc_82578BEC;
  // lwz r3,84(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 84);
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x82578bec
  if (cr6.eq) goto loc_82578BEC;
  // addi r31,r3,4
  r31 = uint64_t(int64_t(int64_t(r3) + 4));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82578be0
  if (cr0.eq) goto loc_82578BE0;
  // lis r4,24714
  r4 = uint64_t(int64_t(1619656704));
  // mr r3,r11
  r3 = r11;
  // ori r4,r4,8194
  r4 = r4 | 8194;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x82578BD4)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // lwz r3,84(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 84);
loc_82578BE0:
  // lis r4,24714
  r4 = uint64_t(int64_t(1619656704));
  // ori r4,r4,8194
  r4 = r4 | 8194;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x82578BEC)); r3 = o.r3; f1 = o.f1; }
loc_82578BEC:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82578C88: lifted from generated/ by lift.py.
void f82578C88(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82578C88 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // stw r11,4(r4)
  REX_STORE_U32(uint32_t(r4) + 4, uint32_t(r11));
  // lwz r11,36(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 36);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82578ca8
  if (!cr6.eq) goto loc_82578CA8;
  // lis r11,-32154
  r11 = uint64_t(int64_t(-2107244544));
  // addi r11,r11,-11056
  r11 = uint64_t(int64_t(int64_t(r11) + -11056));
  // b 0x82578cb0
  goto loc_82578CB0;
loc_82578CA8:
  // lis r11,-32154
  r11 = uint64_t(int64_t(-2107244544));
  // addi r11,r11,-11088
  r11 = uint64_t(int64_t(int64_t(r11) + -11088));
loc_82578CB0:
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_82578CB8: lifted from generated/ by lift.py.
void f82578CB8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82578CB8 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blr
  return;
}

// sub_82578CD0: lifted from generated/ by lift.py.
uint64_t f82578CD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82578CD0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r31,r4
  r31 = r4;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r30,r3
  r30 = r3;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r5,4(r11)
  r5 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r3,8(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 8);
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x82578D00)); r3 = o.r3; f1 = o.f1; }
  // lwz r5,0(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r4,12(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 12);
  // lwz r3,8(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 8);
  // bl 0x8257c160
  // (left out: dead in C++)
  r3 = f8257C160(base, r1, r13, r3, r4, r5);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82578d30
  if (!cr0.lt) goto loc_82578D30;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r9,12(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 12);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
loc_82578D30:
  // addi r11,r30,44
  r11 = uint64_t(int64_t(int64_t(r30) + 44));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x82578d4c
  if (cr0.eq) goto loc_82578D4C;
  // stw r31,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r31));
  // b 0x82578d50
  goto loc_82578D50;
loc_82578D4C:
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
loc_82578D50:
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
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

// sub_82578D70: lifted from generated/ by lift.py.
uint64_t f82578D70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82578D70 (lifted)");
  uint64_t r4 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // addi r30,r31,52
  r30 = uint64_t(int64_t(int64_t(r31) + 52));
loc_82578D88:
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r4,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq 0x82578dd8
  if (cr0.eq) goto loc_82578DD8;
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,259
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 259, so);
  // beq cr6,0x82578dd8
  if (cr6.eq) goto loc_82578DD8;
  // rotlwi r11,r4,0
  r11 = __builtin_rotateleft32(uint32_t(r4), 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82578dcc
  if (cr0.eq) goto loc_82578DCC;
  // lwz r10,4(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x82578dc0
  if (!cr6.eq) goto loc_82578DC0;
  // stw r28,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r28));
loc_82578DC0:
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r10,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r10));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
loc_82578DCC:
  // mr r3,r31
  r3 = r31;
  // bl 0x82578cd0
  // (left out: dead in C++)
  r3 = f82578CD0(base, r1, r13, r3, r4);
  // b 0x82578d88
  goto loc_82578D88;
loc_82578DD8:
  // addi r29,r31,44
  r29 = uint64_t(int64_t(int64_t(r31) + 44));
loc_82578DDC:
  // lwz r4,0(r29)
  r4 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplwi r4,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq 0x82578ef0
  if (cr0.eq) goto loc_82578EF0;
  // lwz r9,0(r4)
  r9 = REX_LOAD_U32(uint32_t(r4) + 0);
  // lwz r11,0(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 0);
  // cmpwi cr6,r11,259
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 259, so);
  // beq cr6,0x82578ef0
  if (cr6.eq) goto loc_82578EF0;
  // rotlwi r11,r4,0
  r11 = __builtin_rotateleft32(uint32_t(r4), 0);
  // lwz r10,8(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + 8);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82578e24
  if (cr0.eq) goto loc_82578E24;
  // lwz r8,4(r29)
  r8 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bne cr6,0x82578e18
  if (!cr6.eq) goto loc_82578E18;
  // stw r28,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r28));
loc_82578E18:
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r8,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r8));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
loc_82578E24:
  // lwz r11,0(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82578ee4
  if (!cr6.eq) goto loc_82578EE4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lbz r9,60(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 60);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lbz r11,140(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 140);
  // mr r8,r11
  r8 = r11;
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // beq cr6,0x82578e50
  if (cr6.eq) goto loc_82578E50;
  // stb r11,60(r31)
  REX_STORE_U8(uint32_t(r31) + 60, uint8_t(r11));
loc_82578E50:
  // lhz r11,62(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 62);
  // cmplwi cr6,r11,4096
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4096, so);
  // ble cr6,0x82578e60
  if (!cr6.gt) goto loc_82578E60;
  // sth r28,62(r31)
  REX_STORE_U16(uint32_t(r31) + 62, uint16_t(r28));
loc_82578E60:
  // lhz r11,-2(r10)
  r11 = REX_LOAD_U16(uint32_t(r10) + -2);
  // li r8,259
  r8 = uint64_t(int64_t(259));
  // lbz r9,60(r31)
  r9 = REX_LOAD_U8(uint32_t(r31) + 60);
  // rlwimi r11,r9,12,0,19
  r11 = (__builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 12) & 0xFFFFF000) | (r11 & 0xFFFFFFFF00000FFF);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // sth r11,-2(r10)
  REX_STORE_U16(uint32_t(r10) + -2, uint16_t(r11));
  // lhz r9,62(r31)
  r9 = REX_LOAD_U16(uint32_t(r31) + 62);
  // rlwimi r9,r11,0,16,19
  r9 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xF000) | (r9 & 0xFFFFFFFFFFFF0FFF);
  // sth r9,-2(r10)
  REX_STORE_U16(uint32_t(r10) + -2, uint16_t(r9));
  // lhz r11,62(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 62);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // sth r11,62(r31)
  REX_STORE_U16(uint32_t(r31) + 62, uint16_t(r11));
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // stw r28,4(r4)
  REX_STORE_U32(uint32_t(r4) + 4, uint32_t(r28));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82578eb8
  if (cr0.eq) goto loc_82578EB8;
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // b 0x82578ebc
  goto loc_82578EBC;
loc_82578EB8:
  // stw r4,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r4));
loc_82578EBC:
  // stw r4,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r4));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r9,4(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // lwz r9,0(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 0);
  // slw r10,r10,r9
  r10 = uint8_t(r9) & 0x20 ? 0 : (uint32_t(r10) << (uint8_t(r9) & 0x3F));
  // lwz r9,1268(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 1268);
  // or r10,r10,r9
  r10 = r10 | r9;
  // stw r10,1268(r11)
  REX_STORE_U32(uint32_t(r11) + 1268, uint32_t(r10));
  // b 0x82578ddc
  goto loc_82578DDC;
loc_82578EE4:
  // mr r3,r31
  r3 = r31;
  // bl 0x82578cd0
  // (left out: dead in C++)
  r3 = f82578CD0(base, r1, r13, r3, r4);
  // b 0x82578ddc
  goto loc_82578DDC;
loc_82578EF0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_82578F00: lifted from generated/ by lift.py.
lifted::Result f82578F00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82578F00 (lifted)");
  uint64_t r11 = 0, r12 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e74
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // li r25,0
  r25 = uint64_t(int64_t(0));
  // mr r23,r25
  r23 = r25;
  // mr r26,r25
  r26 = r25;
  // lwz r3,68(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 68);
  // lwz r24,64(r29)
  r24 = REX_LOAD_U32(uint32_t(r29) + 64);
  // bl 0x8257d020
  // (left out: dead in C++)
  r3 = f8257D020(base, r1, r13, r3);
  // addi r27,r29,44
  r27 = uint64_t(int64_t(int64_t(r29) + 44));
  // lwz r31,0(r27)
  r31 = REX_LOAD_U32(uint32_t(r27) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne 0x82578f40
  if (!cr0.eq) goto loc_82578F40;
  // mr r30,r25
  r30 = r25;
  // b 0x82578f58
  goto loc_82578F58;
loc_82578F40:
  // lwz r30,0(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r30,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq 0x82578f58
  if (cr0.eq) goto loc_82578F58;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,259
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 259, so);
  // bne cr6,0x82578f74
  if (!cr6.eq) goto loc_82578F74;
loc_82578F58:
  // lwz r11,64(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 64);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82578f74
  if (!cr6.eq) goto loc_82578F74;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r3,60(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 60);
  // stw r11,64(r29)
  REX_STORE_U32(uint32_t(r29) + 64, uint32_t(r11));
  // bl 0x8245a208
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A208, LIFTED_ARGS(0x82578F74)); r3 = o.r3; f1 = o.f1; }
loc_82578F74:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82579058
  if (cr6.eq) goto loc_82579058;
loc_82578F7C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,259
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 259, so);
  // beq cr6,0x82579058
  if (cr6.eq) goto loc_82579058;
  // lwz r11,20(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 20);
  // cmplw cr6,r26,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r11), so);
  // bge cr6,0x82579058
  if (!cr6.lt) goto loc_82579058;
  // addi r28,r29,52
  r28 = uint64_t(int64_t(int64_t(r29) + 52));
  // mr r3,r28
  r3 = r28;
  // bl 0x8257c570
  // (left out: dead in C++)
  r3 = f8257C570(base, r3);
  // lwz r11,20(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 20);
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // bge cr6,0x82579058
  if (!cr6.lt) goto loc_82579058;
  // li r11,259
  r11 = uint64_t(int64_t(259));
  // li r5,88
  r5 = uint64_t(int64_t(88));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x82578FC4)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r3,60(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 60);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lwz r11,12(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 12);
  // stw r31,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r31));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // bl 0x8245a050
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A050, LIFTED_ARGS(0x82578FE8)); r3 = o.r3; f1 = o.f1; }
  // mr. r23,r3
  r23 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r23), 0, so);
  // blt 0x82579050
  if (cr0.lt) goto loc_82579050;
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82579018
  if (cr0.eq) goto loc_82579018;
  // lwz r10,4(r27)
  r10 = REX_LOAD_U32(uint32_t(r27) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8257900c
  if (!cr6.eq) goto loc_8257900C;
  // stw r25,4(r27)
  REX_STORE_U32(uint32_t(r27) + 4, uint32_t(r25));
loc_8257900C:
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r10,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r10));
  // stw r25,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r25));
loc_82579018:
  // stw r25,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r25));
  // lwz r11,4(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82579030
  if (cr0.eq) goto loc_82579030;
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // b 0x82579034
  goto loc_82579034;
loc_82579030:
  // stw r31,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r31));
loc_82579034:
  // stw r31,4(r28)
  REX_STORE_U32(uint32_t(r28) + 4, uint32_t(r31));
  // addi r26,r26,1
  r26 = uint64_t(int64_t(int64_t(r26) + 1));
  // lwz r31,0(r27)
  r31 = REX_LOAD_U32(uint32_t(r27) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x82579058
  if (cr0.eq) goto loc_82579058;
  // lwz r30,0(r31)
  r30 = REX_LOAD_U32(uint32_t(r31) + 0);
  // b 0x82578f7c
  goto loc_82578F7C;
loc_82579050:
  // mr r23,r25
  r23 = r25;
  // stw r25,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r25));
loc_82579058:
  // lwz r11,64(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 64);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8257908c
  if (!cr6.eq) goto loc_8257908C;
  // cmpwi cr6,r24,1
  cr6 = lifted::Compare<int32_t>(int32_t(r24), 1, so);
  // bne cr6,0x8257908c
  if (!cr6.eq) goto loc_8257908C;
  // addi r3,r29,52
  r3 = uint64_t(int64_t(int64_t(r29) + 52));
  // bl 0x8257c570
  // (left out: dead in C++)
  r3 = f8257C570(base, r3);
  // cmplwi cr6,r3,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 2, so);
  // blt cr6,0x8257908c
  if (cr6.lt) goto loc_8257908C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,60(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 60);
  // bl 0x8245a168
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A168, LIFTED_ARGS(0x82579088)); r3 = o.r3; f1 = o.f1; }
  // stw r25,64(r29)
  REX_STORE_U32(uint32_t(r29) + 64, uint32_t(r25));
loc_8257908C:
  // addi r31,r29,52
  r31 = uint64_t(int64_t(int64_t(r29) + 52));
  // b 0x825790d4
  goto loc_825790D4;
loc_82579094:
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,259
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 259, so);
  // beq cr6,0x825790e0
  if (cr6.eq) goto loc_825790E0;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x825790cc
  if (cr0.eq) goto loc_825790CC;
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x825790c0
  if (!cr6.eq) goto loc_825790C0;
  // stw r25,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r25));
loc_825790C0:
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
  // stw r25,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r25));
loc_825790CC:
  // mr r3,r29
  r3 = r29;
  // bl 0x82578cd0
  // (left out: dead in C++)
  r3 = f82578CD0(base, r1, r13, r3, r4);
loc_825790D4:
  // lwz r4,0(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r4,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // bne 0x82579094
  if (!cr0.eq) goto loc_82579094;
loc_825790E0:
  // mr r3,r23
  r3 = r23;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6ec4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_825790F0: lifted from generated/ by lift.py.
lifted::Result f825790F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825790F0 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,36(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 36);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82579100
  if (!cr6.eq) goto loc_82579100;
  // b 0x82578f00
  { const lifted::Result o = f82578F00(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82579100:
  // b 0x82578d70
  r3 = f82578D70(base, r1, r13, r3);
  return {r3, f1};
}

// sub_82579108: lifted from generated/ by lift.py.
lifted::Result f82579108(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82579108 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // bl 0x8257b9d0
  // (left out: dead in C++)
  r3 = f8257B9D0(base, r3);
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt 0x82579194
  if (cr0.lt) goto loc_82579194;
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x8257916c
  if (!cr6.eq) goto loc_8257916C;
  // lwz r11,52(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 52);
  // b 0x8257915c
  goto loc_8257915C;
loc_82579140:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r9,12(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 12);
  // stw r9,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r9));
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
loc_8257915C:
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82579140
  if (!cr0.eq) goto loc_82579140;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8257d6f8
  // (left out: dead in C++)
  f8257D6F8(base, r3);
loc_8257916C:
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82579194
  if (!cr6.eq) goto loc_82579194;
  // lwz r3,60(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 60);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82579194
  if (cr0.eq) goto loc_82579194;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // stw r11,64(r31)
  REX_STORE_U32(uint32_t(r31) + 64, uint32_t(r11));
  // bl 0x8245a1b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A1B0, LIFTED_ARGS(0x82579190)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
loc_82579194:
  // mr r3,r30
  r3 = r30;
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

// sub_825791B0: lifted from generated/ by lift.py.
lifted::Result f825791B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_825791B0 (lifted)");
  uint64_t r4 = 0, r7 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r31,r3
  r31 = r3;
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // addi r28,r31,16
  r28 = uint64_t(int64_t(int64_t(r31) + 16));
  // mr r7,r28
  r7 = r28;
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // stw r6,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r6));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r5,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r5));
  // lwz r10,24(r5)
  r10 = REX_LOAD_U32(uint32_t(r5) + 24);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // lwz r5,20(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 20);
  // rlwinm r11,r11,28,30,30
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 28) & 0x2;
  // xori r6,r11,2
  r6 = r11 ^ 2;
  // lwz r11,28(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 28);
  // add r3,r11,r6
  r3 = r11 + r6;
  // bl 0x8257c858
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8257C858, LIFTED_ARGS(0x825791FC)); r3 = o.r3; f1 = o.f1; }
  // mr. r27,r3
  r27 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // blt 0x82579274
  if (cr0.lt) goto loc_82579274;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x82579258
  if (!cr6.gt) goto loc_82579258;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_82579218:
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r9,40(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 40);
  // lwz r8,8(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mullw r11,r8,r29
  r11 = uint64_t(int64_t(int64_t(int32_t(r8)) * int64_t(int32_t(r29))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // stwx r11,r30,r9
  REX_STORE_U32(uint32_t(r30) + uint32_t(r9), uint32_t(r11));
  // lwz r11,40(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 40);
  // add r4,r30,r11
  r4 = r30 + r11;
  // bl 0x82578cd0
  // (left out: dead in C++)
  r3 = f82578CD0(base, r1, r13, r3, r4);
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r30,r30,8
  r30 = uint64_t(int64_t(int64_t(r30) + 8));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x82579218
  if (cr6.lt) goto loc_82579218;
loc_82579258:
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82579274
  if (!cr6.eq) goto loc_82579274;
  // addi r4,r31,68
  r4 = uint64_t(int64_t(int64_t(r31) + 68));
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // bl 0x8257d518
  // (left out: dead in C++)
  { const lifted::Result o = f8257D518(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
loc_82579274:
  // mr r3,r27
  r3 = r27;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82579280: lifted from generated/ by lift.py.
lifted::Result f82579280(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82579280 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82579300
  if (cr6.eq) goto loc_82579300;
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x825792cc
  if (!cr6.eq) goto loc_825792CC;
  // lwz r3,60(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 60);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x825792c4
  if (cr0.eq) goto loc_825792C4;
  // bl 0x82459e18
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82459E18, LIFTED_ARGS(0x825792C0)); r3 = o.r3; f1 = o.f1; }
  // stw r30,60(r31)
  REX_STORE_U32(uint32_t(r31) + 60, uint32_t(r30));
loc_825792C4:
  // lwz r3,68(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 68);
  // bl 0x8257cfa8
  // (left out: dead in C++)
  { const lifted::Result o = f8257CFA8(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825792CC:
  // lwz r3,40(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 40);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x825792e8
  if (cr0.eq) goto loc_825792E8;
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // ori r4,r4,3
  r4 = r4 | 3;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x825792E4)); r3 = o.r3; f1 = o.f1; }
  // stw r30,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r30));
loc_825792E8:
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // bl 0x8257c710
  // (left out: dead in C++)
  { const lifted::Result o = f8257C710(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // mr r3,r31
  r3 = r31;
  // ori r4,r4,32785
  r4 = r4 | 32785;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x82579300)); r3 = o.r3; f1 = o.f1; }
loc_82579300:
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

// sub_82579318: lifted from generated/ by lift.py.
uint64_t f82579318(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82579318 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r24,r3
  r24 = r3;
  // mr r28,r6
  r28 = r6;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // mr r27,r4
  r27 = r4;
  // mr r25,r5
  r25 = r5;
  // ori r3,r3,10
  r3 = r3 | 10;
  // mr r30,r26
  r30 = r26;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x82579350
  if (cr6.eq) goto loc_82579350;
  // stw r26,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r26));
loc_82579350:
  // lwz r29,52(r24)
  r29 = REX_LOAD_U32(uint32_t(r24) + 52);
  // cmplwi r29,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // beq 0x82579404
  if (cr0.eq) goto loc_82579404;
loc_8257935C:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x825793f8
  if (cr6.eq) goto loc_825793F8;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r9,12(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r8,0(r25)
  r8 = REX_LOAD_U32(uint32_t(r25) + 0);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // subf r8,r30,r8
  r8 = r8 - r30;
  // addi r31,r9,2
  r31 = uint64_t(int64_t(int64_t(r9) + 2));
  // cmplw cr6,r31,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r8), so);
  // bgt cr6,0x82579430
  if (cr6.gt) goto loc_82579430;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mr r5,r31
  r5 = r31;
  // add r3,r30,r27
  r3 = r30 + r27;
  // add r11,r11,r10
  r11 = r11 + r10;
  // addi r4,r11,-2
  r4 = uint64_t(int64_t(int64_t(r11) + -2));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x825793A4)); r3 = o.r3; f1 = o.f1; }
  // add r30,r31,r30
  r30 = r31 + r30;
  // mr r3,r26
  r3 = r26;
  // cmplwi cr6,r28,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // beq cr6,0x825793c0
  if (cr6.eq) goto loc_825793C0;
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_825793C0:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addi r9,r31,-2
  r9 = uint64_t(int64_t(int64_t(r31) + -2));
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // add r9,r8,r9
  r9 = r8 + r9;
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // bge cr6,0x825793e0
  if (!cr6.lt) goto loc_825793E0;
  // mr r10,r9
  r10 = r9;
loc_825793E0:
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r9,12(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // bne cr6,0x825793fc
  if (!cr6.eq) goto loc_825793FC;
  // stw r26,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r26));
loc_825793F8:
  // lwz r29,4(r29)
  r29 = REX_LOAD_U32(uint32_t(r29) + 4);
loc_825793FC:
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x8257935c
  if (!cr6.eq) goto loc_8257935C;
loc_82579404:
  // lwz r9,4(r24)
  r9 = REX_LOAD_U32(uint32_t(r24) + 4);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,32(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 32);
  // lwz r9,0(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 0);
  // slw r10,r10,r9
  r10 = uint8_t(r9) & 0x20 ? 0 : (uint32_t(r10) << (uint8_t(r9) & 0x3F));
  // lwz r9,1268(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 1268);
  // andc r10,r9,r10
  r10 = r9 & ~r10;
  // stw r10,1268(r11)
  REX_STORE_U32(uint32_t(r11) + 1268, uint32_t(r10));
loc_82579424:
  // stw r30,0(r25)
  REX_STORE_U32(uint32_t(r25) + 0, uint32_t(r30));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return r3;
loc_82579430:
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // bne cr6,0x82579424
  if (!cr6.eq) goto loc_82579424;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // mr r30,r31
  r30 = r31;
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x82579424
  goto loc_82579424;
}

// sub_82579448: lifted from generated/ by lift.py.
lifted::Result f82579448(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82579448 (lifted)");
  uint64_t r5 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-272(r1)
  ea = -272 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // lis r10,-32768
  r10 = uint64_t(int64_t(-2147483648));
  // mr r27,r4
  r27 = r4;
  // ori r26,r10,16385
  r26 = r10 | 16385;
  // lwz r11,36(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 36);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x82579580
  if (cr6.eq) goto loc_82579580;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x82579480)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,84(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + 84, temp.u32);
  // stb r31,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r31));
  // stb r31,81(r1)
  REX_STORE_U8(uint32_t(r1) + 81, uint8_t(r31));
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x825794A8)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,80
  r11 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,104
  r3 = uint64_t(int64_t(int64_t(r1) + 104));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // stb r30,96(r1)
  REX_STORE_U8(uint32_t(r1) + 96, uint8_t(r30));
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x825794C8)); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r27,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r27));
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x825794E4)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r29,60
  r28 = uint64_t(int64_t(int64_t(r29) + 60));
  // stb r30,88(r1)
  REX_STORE_U8(uint32_t(r1) + 88, uint8_t(r30));
  // addi r11,r1,104
  r11 = uint64_t(int64_t(int64_t(r1) + 104));
  // lwz r3,0(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 0);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x82579514
  if (cr0.eq) goto loc_82579514;
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8257957c
  if (cr6.eq) goto loc_8257957C;
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x82459ef0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82459EF0, LIFTED_ARGS(0x82579510)); r3 = o.r3; f1 = o.f1; }
  // b 0x82579578
  goto loc_82579578;
loc_82579514:
  // li r5,92
  r5 = uint64_t(int64_t(92));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x82579524)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,20(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 20);
  // lis r11,-32168
  r11 = uint64_t(int64_t(-2108162048));
  // stb r30,112(r1)
  REX_STORE_U8(uint32_t(r1) + 112, uint8_t(r30));
  // li r9,16000
  r9 = uint64_t(int64_t(16000));
  // stb r30,116(r1)
  REX_STORE_U8(uint32_t(r1) + 116, uint8_t(r30));
  // addi r11,r11,-29512
  r11 = uint64_t(int64_t(int64_t(r11) + -29512));
  // stb r30,169(r1)
  REX_STORE_U8(uint32_t(r1) + 169, uint8_t(r30));
  // mr r4,r28
  r4 = r28;
  // stb r30,170(r1)
  REX_STORE_U8(uint32_t(r1) + 170, uint8_t(r30));
  // stb r10,171(r1)
  REX_STORE_U8(uint32_t(r1) + 171, uint8_t(r10));
  // addi r10,r1,88
  r10 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // stb r31,176(r1)
  REX_STORE_U8(uint32_t(r1) + 176, uint8_t(r31));
  // stw r9,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r9));
  // stw r31,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r31));
  // stw r31,188(r1)
  REX_STORE_U32(uint32_t(r1) + 188, uint32_t(r31));
  // stw r10,184(r1)
  REX_STORE_U32(uint32_t(r1) + 184, uint32_t(r10));
  // stw r11,192(r1)
  REX_STORE_U32(uint32_t(r1) + 192, uint32_t(r11));
  // stw r31,196(r1)
  REX_STORE_U32(uint32_t(r1) + 196, uint32_t(r31));
  // stw r29,200(r1)
  REX_STORE_U32(uint32_t(r1) + 200, uint32_t(r29));
  // bl 0x8245a768
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8245A768, LIFTED_ARGS(0x82579578)); r3 = o.r3; f1 = o.f1; }
loc_82579578:
  // mr r26,r3
  r26 = r3;
loc_8257957C:
  // stw r30,64(r29)
  REX_STORE_U32(uint32_t(r29) + 64, uint32_t(r30));
loc_82579580:
  // mr r3,r26
  r3 = r26;
  // addi r1,r1,272
  r1 = uint64_t(int64_t(int64_t(r1) + 272));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82579590: lifted from generated/ by lift.py.
uint64_t f82579590(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_82579590 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r26,r3
  r26 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // mr r30,r5
  r30 = r5;
  // mr r3,r29
  r3 = r29;
  // lwz r11,36(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 36);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x825795c4
  if (!cr6.eq) goto loc_825795C4;
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16385
  r3 = r3 | 16385;
  // b 0x82579628
  goto loc_82579628;
loc_825795C4:
  // lwz r11,8(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 8);
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // stw r29,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r29));
  // addi r31,r11,2
  r31 = uint64_t(int64_t(int64_t(r11) + 2));
  // divwu. r27,r10,r31
  r27 = uint32_t(uint32_t(r31) ? uint32_t(r10) / uint32_t(r31) : 0);
  cr0 = lifted::Compare<int32_t>(int32_t(r27), 0, so);
  // twllei r31,0
  if (int32_t(r31) == 0 || uint32_t(r31) < 0u) lifted::Trap(0);
  // beq 0x82579628
  if (cr0.eq) goto loc_82579628;
  // mr r28,r4
  r28 = r4;
loc_825795EC:
  // mr r5,r31
  r5 = r31;
  // lwz r3,68(r26)
  r3 = REX_LOAD_U32(uint32_t(r26) + 68);
  // mr r4,r28
  r4 = r28;
  // bl 0x8257d298
  // (left out: dead in C++)
  r3 = f8257D298(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x82579628
  if (cr0.lt) goto loc_82579628;
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // beq cr6,0x82579628
  if (cr6.eq) goto loc_82579628;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // add r28,r28,r31
  r28 = r28 + r31;
  // add r11,r11,r31
  r11 = r11 + r31;
  // cmplw cr6,r29,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r27), so);
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
  // blt cr6,0x825795ec
  if (cr6.lt) goto loc_825795EC;
loc_82579628:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_82579630: lifted from generated/ by lift.py.
lifted::Result f82579630(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82579630 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
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
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82579668
  if (!cr6.eq) goto loc_82579668;
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82579668
  if (!cr6.eq) goto loc_82579668;
  // lwz r3,68(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 68);
  // bl 0x8257d460
  // (left out: dead in C++)
  f8257D460(base, r3);
loc_82579668:
  // lwz r11,24(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 24);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82579688
  if (!cr6.eq) goto loc_82579688;
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82579688
  if (!cr6.eq) goto loc_82579688;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8257d6d8
  // (left out: dead in C++)
  f8257D6D8(base, r3);
loc_82579688:
  // mr r3,r31
  r3 = r31;
  // bl 0x8257b9a8
  // (left out: dead in C++)
  r3 = f8257B9A8(base, r3);
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge 0x825796a0
  if (!cr0.lt) goto loc_825796A0;
  // mr r3,r31
  r3 = r31;
  // bl 0x82579108
  // (left out: dead in C++)
  { const lifted::Result o = f82579108(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_825796A0:
  // mr r3,r30
  r3 = r30;
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

// sub_825796C0: lifted from generated/ by lift.py.
lifted::Result f825796C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_825796C0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r28,r4
  r28 = r4;
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // mr r29,r3
  r29 = r3;
  // ori r4,r4,32785
  r4 = r4 | 32785;
  // li r3,72
  r3 = uint64_t(int64_t(72));
  // mr r30,r5
  r30 = r5;
  // mr r26,r6
  r26 = r6;
  // li r27,0
  r27 = uint64_t(int64_t(0));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x825796F0)); r3 = o.r3; f1 = o.f1; }
  // mr. r31,r3
  r31 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq 0x82579770
  if (cr0.eq) goto loc_82579770;
  // lis r11,-32154
  r11 = uint64_t(int64_t(-2107244544));
  // stw r29,32(r31)
  REX_STORE_U32(uint32_t(r31) + 32, uint32_t(r29));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r30,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r30));
  // addi r11,r11,-11196
  r11 = uint64_t(int64_t(int64_t(r11) + -11196));
  // stw r28,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r28));
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // cmpwi cr6,r30,1
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 1, so);
  // stw r10,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r10));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r9,20(r31)
  REX_STORE_U32(uint32_t(r31) + 20, uint32_t(r9));
  // bne cr6,0x82579748
  if (!cr6.eq) goto loc_82579748;
  // lwz r4,68(r28)
  r4 = REX_LOAD_U32(uint32_t(r28) + 68);
  // cmplwi r4,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq 0x82579750
  if (cr0.eq) goto loc_82579750;
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mr r3,r31
  r3 = r31;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82579744)); r3 = o.r3; f1 = o.f1; }
  // b 0x82579750
  goto loc_82579750;
loc_82579748:
  // li r11,255
  r11 = uint64_t(int64_t(255));
  // stb r11,60(r31)
  REX_STORE_U8(uint32_t(r31) + 60, uint8_t(r11));
loc_82579750:
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // ori r4,r4,3
  r4 = r4 | 3;
  // rlwinm r3,r11,3,0,28
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x82579764)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,40(r31)
  REX_STORE_U32(uint32_t(r31) + 40, uint32_t(r3));
  // bne 0x82579784
  if (!cr0.eq) goto loc_82579784;
loc_82579770:
  // lis r27,-32761
  r27 = uint64_t(int64_t(-2147024896));
  // mr r3,r31
  r3 = r31;
  // ori r27,r27,14
  r27 = r27 | 14;
  // bl 0x82579280
  // (left out: dead in C++)
  { const lifted::Result o = f82579280(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_82579784:
  // mr r3,r27
  r3 = r27;
  // stw r31,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r31));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82579798: lifted from generated/ by lift.py.
lifted::Result f82579798(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82579798 (lifted)");
  lifted::Cr cr0;
  uint8_t so = 0;
  // clrlwi. r5,r5,24
  r5 = uint32_t(r5) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r5), 0, so);
  // bne 0x825797a8
  if (!cr0.eq) goto loc_825797A8;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // b 0x825796c0
  { const lifted::Result o = f825796C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_825797A8:
  // b 0x825796c0
  { const lifted::Result o = f825796C0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_825797B0: lifted from generated/ by lift.py.
uint64_t f825797B0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_825797B0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32168
  r11 = uint64_t(int64_t(-2108162048));
  // addi r3,r11,-26728
  r3 = uint64_t(int64_t(int64_t(r11) + -26728));
  // blr
  return r3;
}

// sub_825797C0: lifted from generated/ by lift.py.
uint64_t f825797C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_825797C0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r31,r4
  r31 = r4;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r30,r3
  r30 = r3;
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r5,4(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x825797EC)); r3 = o.r3; f1 = o.f1; }
  // mr r5,r31
  r5 = r31;
  // lwz r4,12(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 12);
  // lwz r3,8(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 8);
  // bl 0x8257c160
  // (left out: dead in C++)
  r3 = f8257C160(base, r1, r13, r3, r4, r5);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bge 0x82579814
  if (!cr0.lt) goto loc_82579814;
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r10,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r10));
loc_82579814:
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

// sub_82579830: lifted from generated/ by lift.py.
lifted::Result f82579830(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82579830 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
  lifted::Cr cr0;
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
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // mr r31,r3
  r31 = r3;
  // lwz r11,29992(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 29992);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8257b9d0
  // (left out: dead in C++)
  r3 = f8257B9D0(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8257986c
  if (cr0.lt) goto loc_8257986C;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8257eff8
  // (left out: dead in C++)
  { const lifted::Result o = f8257EFF8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8257986C:
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

// sub_82579880: lifted from generated/ by lift.py.
void f82579880(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82579880 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32154
  r11 = uint64_t(int64_t(-2107244544));
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // addi r11,r11,-11088
  r11 = uint64_t(int64_t(int64_t(r11) + -11088));
  // stw r10,4(r4)
  REX_STORE_U32(uint32_t(r4) + 4, uint32_t(r10));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
  // blr
  return;
}

// sub_82579898: lifted from generated/ by lift.py.
lifted::Result f82579898(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82579898 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lis r11,-32250
  r11 = uint64_t(int64_t(-2113536000));
  // mr r31,r3
  r31 = r3;
  // lwz r11,29992(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 29992);
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8257b9a8
  // (left out: dead in C++)
  r3 = f8257B9A8(base, r3);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8257995c
  if (cr0.lt) goto loc_8257995C;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x825798D0)); r3 = o.r3; f1 = o.f1; }
  // lis r12,-1057
  r12 = uint64_t(int64_t(-69271552));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // ori r12,r12,221
  r12 = r12 | 221;
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // rldicr r12,r12,32,31
  r12 = __builtin_rotateleft64(r12, 32) & 0xFFFFFFFF00000000;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // oris r12,r12,60123
  r12 = r12 | 3940220928;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // ori r12,r12,61168
  r12 = r12 | 61168;
  // or r4,r11,r12
  r4 = r11 | r12;
  // bl 0x8257f070
  // (left out: dead in C++)
  { const lifted::Result o = f8257F070(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8257995c
  if (cr0.lt) goto loc_8257995C;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
loc_8257991C:
  // subf r11,r30,r29
  r11 = r29 - r30;
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // mr r4,r30
  r4 = r30;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // addi r5,r11,-1
  r5 = uint64_t(int64_t(int64_t(r11) + -1));
  // bl 0x8257f1e8
  // (left out: dead in C++)
  { const lifted::Result o = f8257F1E8(base, r1, r13, r3, r4, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // blt 0x8257995c
  if (cr0.lt) goto loc_8257995C;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmplwi cr6,r30,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 4, so);
  // blt cr6,0x8257991c
  if (cr6.lt) goto loc_8257991C;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8257ef80
  // (left out: dead in C++)
  { const lifted::Result o = f8257EF80(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_8257995C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82579968: lifted from generated/ by lift.py.
lifted::Result f82579968(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_82579968 (lifted)");
  uint64_t r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // lwz r9,36(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r8,8(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mullw r11,r8,r9
  r11 = uint64_t(int64_t(int64_t(int32_t(r8)) * int64_t(int32_t(r9))));
  // add r30,r11,r10
  r30 = r11 + r10;
  // b 0x82579a70
  goto loc_82579A70;
loc_82579998:
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // cmplw cr6,r29,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r10), so);
  // bge cr6,0x82579a7c
  if (!cr6.lt) goto loc_82579A7C;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82579a2c
  if (!cr6.eq) goto loc_82579A2C;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r10,8(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 8);
  // addi r4,r10,-2
  r4 = uint64_t(int64_t(int64_t(r10) + -2));
  // lbz r10,40(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 40);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lbz r11,140(r11)
  r11 = REX_LOAD_U8(uint32_t(r11) + 140);
  // mr r9,r11
  r9 = r11;
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // beq cr6,0x825799d4
  if (cr6.eq) goto loc_825799D4;
  // stb r11,40(r31)
  REX_STORE_U8(uint32_t(r31) + 40, uint8_t(r11));
loc_825799D4:
  // lhz r11,42(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 42);
  // cmplwi cr6,r11,4096
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4096, so);
  // ble cr6,0x825799e8
  if (!cr6.gt) goto loc_825799E8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // sth r11,42(r31)
  REX_STORE_U16(uint32_t(r31) + 42, uint16_t(r11));
loc_825799E8:
  // lhz r11,0(r4)
  r11 = REX_LOAD_U16(uint32_t(r4) + 0);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lbz r10,40(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 40);
  // rlwimi r11,r10,12,0,19
  r11 = (__builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 12) & 0xFFFFF000) | (r11 & 0xFFFFFFFF00000FFF);
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // sth r11,0(r4)
  REX_STORE_U16(uint32_t(r4) + 0, uint16_t(r11));
  // lhz r10,42(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 42);
  // rlwimi r10,r11,0,16,19
  r10 = (__builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 0) & 0xF000) | (r10 & 0xFFFFFFFFFFFF0FFF);
  // sth r10,0(r4)
  REX_STORE_U16(uint32_t(r4) + 0, uint16_t(r10));
  // lhz r11,42(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 42);
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // sth r11,42(r31)
  REX_STORE_U16(uint32_t(r31) + 42, uint16_t(r11));
  // lwz r11,12(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 12);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8257f198
  // (left out: dead in C++)
  { const lifted::Result o = f8257F198(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82579A2C:
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825797c0
  // (left out: dead in C++)
  r3 = f825797C0(base, r1, r13, r3, r4);
  // lwz r11,36(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 36);
  // lwz r10,20(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // addi r9,r11,1
  r9 = uint64_t(int64_t(int64_t(r11) + 1));
  // lwz r11,16(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 16);
  // twllei r10,0
  if (int32_t(r10) == 0 || uint32_t(r10) < 0u) lifted::Trap(0);
  // divwu r8,r9,r10
  r8 = uint32_t(uint32_t(r10) ? uint32_t(r9) / uint32_t(r10) : 0);
  // mullw r10,r8,r10
  r10 = uint64_t(int64_t(int64_t(int32_t(r8)) * int64_t(int32_t(r10))));
  // subf r10,r10,r9
  r10 = r9 - r10;
  // stw r10,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r10));
  // lwz r8,8(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mullw r11,r8,r10
  r11 = uint64_t(int64_t(int64_t(int32_t(r8)) * int64_t(int32_t(r10))));
  // add r30,r11,r9
  r30 = r11 + r9;
loc_82579A70:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,259
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 259, so);
  // bne cr6,0x82579998
  if (!cr6.eq) goto loc_82579998;
loc_82579A7C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82579A88: lifted from generated/ by lift.py.
lifted::Result f82579A88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_82579A88 (lifted)");
  uint64_t r7 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32250
  r10 = uint64_t(int64_t(-2113536000));
  // mr r31,r3
  r31 = r3;
  // mr r11,r6
  r11 = r6;
  // addi r26,r31,16
  r26 = uint64_t(int64_t(int64_t(r31) + 16));
  // mr r28,r4
  r28 = r4;
  // lwz r10,29992(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 29992);
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // stw r5,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r5));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // mr r7,r26
  r7 = r26;
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // lwz r11,24(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 24);
  // lwz r5,20(r31)
  r5 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // addi r3,r11,2
  r3 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8257c858
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8257C858, LIFTED_ARGS(0x82579AD8)); r3 = o.r3; f1 = o.f1; }
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt 0x82579bdc
  if (cr0.lt) goto loc_82579BDC;
  // addi r29,r31,44
  r29 = uint64_t(int64_t(int64_t(r31) + 44));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r28
  r3 = r28;
  // mr r8,r29
  r8 = r29;
  // bl 0x8257f388
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8257F388, LIFTED_ARGS(0x82579B00)); r3 = o.r3; f1 = o.f1; }
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt 0x82579bdc
  if (cr0.lt) goto loc_82579BDC;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x82579B18)); r3 = o.r3; f1 = o.f1; }
  // lis r12,-1057
  r12 = uint64_t(int64_t(-69271552));
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // ori r12,r12,221
  r12 = r12 | 221;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // rldicr r12,r12,32,31
  r12 = __builtin_rotateleft64(r12, 32) & 0xFFFFFFFF00000000;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // oris r12,r12,60123
  r12 = r12 | 3940220928;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // ori r12,r12,61168
  r12 = r12 | 61168;
  // or r4,r11,r12
  r4 = r11 | r12;
  // bl 0x8257f070
  // (left out: dead in C++)
  { const lifted::Result o = f8257F070(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt 0x82579bdc
  if (cr0.lt) goto loc_82579BDC;
  // li r27,0
  r27 = uint64_t(int64_t(0));
loc_82579B5C:
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mr r4,r27
  r4 = r27;
  // bl 0x8257f1e8
  // (left out: dead in C++)
  { const lifted::Result o = f8257F1E8(base, r1, r13, r3, r4, r5, r6, f1); r3 = o.r3; f1 = o.f1; }
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt 0x82579bdc
  if (cr0.lt) goto loc_82579BDC;
  // addi r27,r27,1
  r27 = uint64_t(int64_t(int64_t(r27) + 1));
  // cmplwi cr6,r27,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 4, so);
  // blt cr6,0x82579b5c
  if (cr6.lt) goto loc_82579B5C;
  // mr r3,r28
  r3 = r28;
  // lwz r4,0(r29)
  r4 = REX_LOAD_U32(uint32_t(r29) + 0);
  // bl 0x8257a3d0
  // (left out: dead in C++)
  f8257A3D0(base, r3, r4);
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // ble cr6,0x82579bd4
  if (!cr6.gt) goto loc_82579BD4;
loc_82579B9C:
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r4,12(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 12);
  // lwz r3,8(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 8);
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mullw r11,r9,r29
  r11 = uint64_t(int64_t(int64_t(int32_t(r9)) * int64_t(int32_t(r29))));
  // add r5,r11,r10
  r5 = r11 + r10;
  // bl 0x8257c160
  // (left out: dead in C++)
  r3 = f8257C160(base, r1, r13, r3, r4, r5);
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // blt 0x82579bd4
  if (cr0.lt) goto loc_82579BD4;
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x82579b9c
  if (cr6.lt) goto loc_82579B9C;
loc_82579BD4:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r11));
loc_82579BDC:
  // mr r3,r30
  r3 = r30;
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_82579BE8: lifted from generated/ by lift.py.
lifted::Result f82579BE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82579BE8 (lifted)");
  uint64_t r4 = 0, r12 = 0, r31 = 0;
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
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x82579c30
  if (cr6.eq) goto loc_82579C30;
  // lwz r3,32(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 32);
  // lwz r4,44(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 44);
  // bl 0x8257a3f0
  // (left out: dead in C++)
  r3 = f8257A3F0(base, r1, r13, r3, r4);
  // lwz r3,44(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 44);
  // bl 0x8257ede8
  // (left out: dead in C++)
  { const lifted::Result o = f8257EDE8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // bl 0x8257c710
  // (left out: dead in C++)
  { const lifted::Result o = f8257C710(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // mr r3,r31
  r3 = r31;
  // ori r4,r4,32782
  r4 = r4 | 32782;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x82579C30)); r3 = o.r3; f1 = o.f1; }
loc_82579C30:
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

// sub_82579C48: lifted from generated/ by lift.py.
lifted::Result f82579C48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82579C48 (lifted)");
  uint64_t r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r30,r4
  r30 = r4;
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // mr r31,r3
  r31 = r3;
  // ori r4,r4,32782
  r4 = r4 | 32782;
  // li r3,48
  r3 = uint64_t(int64_t(48));
  // mr r28,r5
  r28 = r5;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(0x82579C74)); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82579ca0
  if (!cr0.eq) goto loc_82579CA0;
  // lis r29,-32761
  r29 = uint64_t(int64_t(-2147024896));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // ori r29,r29,14
  r29 = r29 | 14;
  // bl 0x82579be8
  // (left out: dead in C++)
  { const lifted::Result o = f82579BE8(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_82579C90:
  // mr r3,r29
  r3 = r29;
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
loc_82579CA0:
  // lis r10,-32154
  r10 = uint64_t(int64_t(-2107244544));
  // stw r30,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r30));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // addi r10,r10,-11156
  r10 = uint64_t(int64_t(int64_t(r10) + -11156));
  // stw r31,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r31));
  // li r8,10
  r8 = uint64_t(int64_t(10));
  // li r7,255
  r7 = uint64_t(int64_t(255));
  // stw r9,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stb r7,40(r11)
  REX_STORE_U8(uint32_t(r11) + 40, uint8_t(r7));
  // b 0x82579c90
  goto loc_82579C90;
}

// sub_82579CD0: lifted from generated/ by lift.py.
lifted::Result f82579CD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82579CD0 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  // clrlwi. r11,r5,24
  r11 = uint32_t(r5) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82579ce0
  if (!cr0.eq) goto loc_82579CE0;
  // mr r5,r6
  r5 = r6;
  // b 0x82579c48
  { const lifted::Result o = f82579C48(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
loc_82579CE0:
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,87
  r3 = r3 | 87;
  // blr
  return {r3, f1};
}

// sub_82579CF0: lifted from generated/ by lift.py.
uint64_t f82579CF0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82579CF0 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  // lis r11,-32168
  r11 = uint64_t(int64_t(-2108162048));
  // addi r3,r11,-25392
  r3 = uint64_t(int64_t(int64_t(r11) + -25392));
  // blr
  return r3;
}

// sub_82579D00: lifted from generated/ by lift.py.
uint64_t f82579D00(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82579D00 (lifted)");
  uint64_t r3 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x82579d10
  if (cr6.eq) goto loc_82579D10;
  // li r11,6172
  r11 = uint64_t(int64_t(6172));
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
loc_82579D10:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82579D40: lifted from generated/ by lift.py.
uint64_t f82579D40(uint8_t* base, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82579D40 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x82579d58
  if (cr6.eq) goto loc_82579D58;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stb r11,0(r4)
  REX_STORE_U8(uint32_t(r4) + 0, uint8_t(r11));
  // sth r10,2(r4)
  REX_STORE_U16(uint32_t(r4) + 2, uint16_t(r10));
loc_82579D58:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82579D60: lifted from generated/ by lift.py.
uint64_t f82579D60(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_82579D60 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  // lis r11,-32761
  r11 = uint64_t(int64_t(-2147024896));
  // clrlwi. r10,r4,24
  r10 = uint32_t(r4) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ori r11,r11,87
  r11 = r11 | 87;
  // bne 0x82579d7c
  if (!cr0.eq) goto loc_82579D7C;
  // lwz r10,0(r6)
  r10 = REX_LOAD_U32(uint32_t(r6) + 0);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r10,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r10));
loc_82579D7C:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_82579D88: lifted from generated/ by lift.py.
uint64_t f82579D88(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82579D88 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x82579d98
  if (cr6.eq) goto loc_82579D98;
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // stw r11,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r11));
loc_82579D98:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_82579DA0: lifted from generated/ by lift.py.
lifted::Result f82579DA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_82579DA0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
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
  // mr r3,r4
  r3 = r4;
  // mr r29,r5
  r29 = r5;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r4,6172
  r4 = uint64_t(int64_t(6172));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82579DD0)); r3 = o.r3; f1 = o.f1; }
  // mr. r11,r3
  r11 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82579df4
  if (!cr0.eq) goto loc_82579DF4;
  // lis r30,-32761
  r30 = uint64_t(int64_t(-2147024896));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // ori r30,r30,14
  r30 = r30 | 14;
loc_82579DE4:
  // mr r3,r30
  r3 = r30;
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
loc_82579DF4:
  // lis r10,-32154
  r10 = uint64_t(int64_t(-2107244544));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // addi r10,r10,-11116
  r10 = uint64_t(int64_t(int64_t(r10) + -11116));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r10,4(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // b 0x82579de4
  goto loc_82579DE4;
}

// sub_8257A3D0: lifted from generated/ by lift.py.
void f8257A3D0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8257A3D0 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // addi r11,r11,282
  r11 = uint64_t(int64_t(int64_t(r11) + 282));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r4,r11,r3
  REX_STORE_U32(uint32_t(r11) + uint32_t(r3), uint32_t(r4));
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r11));
  // blr
  return;
}

// sub_8257A3F0: lifted from generated/ by lift.py.
uint64_t f8257A3F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8257A3F0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x8257a480
  if (cr0.eq) goto loc_8257A480;
  // addi r9,r31,1128
  r9 = uint64_t(int64_t(int64_t(r31) + 1128));
loc_8257A418:
  // lwz r8,0(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 0);
  // cmplw cr6,r8,r4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r4), so);
  // beq cr6,0x8257a43c
  if (cr6.eq) goto loc_8257A43C;
  // lwz r8,8(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 8);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // blt cr6,0x8257a418
  if (cr6.lt) goto loc_8257A418;
  // b 0x8257a480
  goto loc_8257A480;
loc_8257A43C:
  // subf r10,r11,r10
  r10 = r10 - r11;
  // addi r9,r11,283
  r9 = uint64_t(int64_t(int64_t(r11) + 283));
  // addi r11,r11,282
  r11 = uint64_t(int64_t(int64_t(r11) + 282));
  // addi r8,r10,-1
  r8 = uint64_t(int64_t(int64_t(r10) + -1));
  // rlwinm r10,r9,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r5,r8,2,0,29
  r5 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // add r4,r10,r31
  r4 = r10 + r31;
  // add r3,r11,r31
  r3 = r11 + r31;
  // bl 0x825aa840
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825AA840, LIFTED_ARGS(0x8257A464)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // addi r10,r11,282
  r10 = uint64_t(int64_t(int64_t(r11) + 282));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stwx r11,r10,r31
  REX_STORE_U32(uint32_t(r10) + uint32_t(r31), uint32_t(r11));
loc_8257A480:
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

// sub_8257B9A8: lifted from generated/ by lift.py.
uint64_t f8257B9A8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8257B9A8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // mr r11,r3
  r11 = r3;
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r8,8(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 8);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r9,r10,r8
  REX_STORE_U32(uint32_t(r10) + uint32_t(r8), uint32_t(r9));
  // stw r9,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r9));
  // blr
  return r3;
}

// sub_8257B9D0: lifted from generated/ by lift.py.
uint64_t f8257B9D0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8257B9D0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // mr r11,r3
  r11 = r3;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r8,8(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 8);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r9,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r9));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stwx r9,r10,r8
  REX_STORE_U32(uint32_t(r10) + uint32_t(r8), uint32_t(r9));
  // blr
  return r3;
}

// sub_8257B9F8: lifted from generated/ by lift.py.
uint64_t f8257B9F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8257B9F8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lwz r11,24(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 24);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x8257ba74
  if (!cr6.eq) goto loc_8257BA74;
  // lwz r9,8(r3)
  r9 = REX_LOAD_U32(uint32_t(r3) + 8);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8257ba74
  if (cr0.eq) goto loc_8257BA74;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r9,16
  r11 = uint64_t(int64_t(int64_t(r9) + 16));
loc_8257BA24:
  // lwz r8,-8(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + -8);
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x8257ba3c
  if (cr6.eq) goto loc_8257BA3C;
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r8,1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 1, so);
  // beq cr6,0x8257ba64
  if (cr6.eq) goto loc_8257BA64;
loc_8257BA3C:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmplwi cr6,r10,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 2, so);
  // blt cr6,0x8257ba24
  if (cr6.lt) goto loc_8257BA24;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8257BA50:
  // addi r11,r3,0
  r11 = uint64_t(int64_t(int64_t(r3) + 0));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // b 0x8257ba78
  goto loc_8257BA78;
loc_8257BA64:
  // lwz r11,4(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 4);
  // addi r3,r11,8
  r3 = uint64_t(int64_t(int64_t(r11) + 8));
  // bl 0x8257c570
  // (left out: dead in C++)
  r3 = f8257C570(base, r3);
  // b 0x8257ba50
  goto loc_8257BA50;
loc_8257BA74:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_8257BA78:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8257BA88: lifted from generated/ by lift.py.
lifted::Result f8257BA88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8257BA88 (lifted)");
  uint64_t r5 = 0, r11 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r11,r4
  r11 = r4;
  // lis r4,25738
  r4 = uint64_t(int64_t(1686765568));
  // mullw r3,r3,r11
  r3 = uint64_t(int64_t(int64_t(int32_t(r3)) * int64_t(int32_t(r11))));
  // ori r4,r4,24
  r4 = r4 | 24;
  // b 0x822976f8
  { const lifted::Result o = lifted::Call(base, 0x822976F8, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8257BAA0: lifted from generated/ by lift.py.
uint64_t f8257BAA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8257BAA0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
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
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x8257BAB8)); r3 = o.r3; f1 = o.f1; }
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
  return r3;
}

// sub_8257BAD0: lifted from generated/ by lift.py.
uint64_t f8257BAD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8257BAD0 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r12 = 0, r31 = 0;
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
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x8257BAE8)); r3 = o.r3; f1 = o.f1; }
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
  return r3;
}

// sub_8257BB00: lifted from generated/ by lift.py.
lifted::Result f8257BB00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257BB00 (lifted)");
  uint64_t r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r3,r3,52
  r3 = uint64_t(int64_t(int64_t(r3) + 52));
  // bl 0x8257fbf8
  // (left out: dead in C++)
  { const lifted::Result o = f8257FBF8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_8257BB28: lifted from generated/ by lift.py.
lifted::Result f8257BB28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257BB28 (lifted)");
  uint64_t r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // addi r3,r3,52
  r3 = uint64_t(int64_t(int64_t(r3) + 52));
  // bl 0x82580020
  // (left out: dead in C++)
  { const lifted::Result o = f82580020(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_8257BB50: lifted from generated/ by lift.py.
lifted::Result f8257BB50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, uint64_t lr) {
  ITC_NATIVE_RUNNING("sub_8257BB50 (lifted)");
  uint64_t r4 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lis r4,25738
  r4 = uint64_t(int64_t(1686765568));
  // ori r4,r4,24
  r4 = r4 | 24;
  // b 0x82297790
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(lr)); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_8257BB60: lifted from generated/ by lift.py.
uint64_t f8257BB60(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_8257BB60 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32168
  r11 = uint64_t(int64_t(-2108162048));
  // addi r3,r11,-17784
  r3 = uint64_t(int64_t(int64_t(r11) + -17784));
  // bl 0x82580178
  // (left out: dead in C++)
  r3 = f82580178(base, r3);
  // lis r11,-32168
  r11 = uint64_t(int64_t(-2108162048));
  // addi r3,r11,-17584
  r3 = uint64_t(int64_t(int64_t(r11) + -17584));
  // bl 0x82580188
  // (left out: dead in C++)
  r3 = f82580188(base, r3);
  // lis r11,-32168
  r11 = uint64_t(int64_t(-2108162048));
  // addi r3,r11,-17760
  r3 = uint64_t(int64_t(int64_t(r11) + -17760));
  // bl 0x82580198
  // (left out: dead in C++)
  r3 = f82580198(base, r3);
  // lis r11,-32168
  r11 = uint64_t(int64_t(-2108162048));
  // addi r3,r11,-17712
  r3 = uint64_t(int64_t(int64_t(r11) + -17712));
  // bl 0x825801a8
  // (left out: dead in C++)
  r3 = f825801A8(base, r3);
  // lis r11,-32165
  r11 = uint64_t(int64_t(-2107965440));
  // addi r3,r11,-30344
  r3 = uint64_t(int64_t(int64_t(r11) + -30344));
  // bl 0x825801b8
  // (left out: dead in C++)
  r3 = f825801B8(base, r3);
  // lis r11,-32165
  r11 = uint64_t(int64_t(-2107965440));
  // addi r3,r11,-30560
  r3 = uint64_t(int64_t(int64_t(r11) + -30560));
  // bl 0x825801c8
  // (left out: dead in C++)
  r3 = f825801C8(base, r3);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_8257BBC8: lifted from generated/ by lift.py.
lifted::Result f8257BBC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257BBC8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0;
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
  // bl 0x8257bb60
  // (left out: dead in C++)
  r3 = f8257BB60(base, r1, r13);
  // addi r30,r31,52
  r30 = uint64_t(int64_t(int64_t(r31) + 52));
  // mr r3,r30
  r3 = r30;
  // bl 0x8257fd18
  // (left out: dead in C++)
  { const lifted::Result o = f8257FD18(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8257bc0c
  if (cr0.eq) goto loc_8257BC0C;
  // mr r3,r30
  r3 = r30;
  // bl 0x8257fbf8
  // (left out: dead in C++)
  { const lifted::Result o = f8257FBF8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // b 0x8257bc30
  goto loc_8257BC30;
loc_8257BC0C:
  // addi r11,r31,64
  r11 = uint64_t(int64_t(int64_t(r31) + 64));
  // li r10,160
  r10 = uint64_t(int64_t(160));
  // addi r9,r31,384
  r9 = uint64_t(int64_t(int64_t(r31) + 384));
  // li r8,9
  r8 = uint64_t(int64_t(9));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r11));
  // sth r10,32(r31)
  REX_STORE_U16(uint32_t(r31) + 32, uint16_t(r10));
  // stw r9,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r9));
  // sth r8,40(r31)
  REX_STORE_U16(uint32_t(r31) + 40, uint16_t(r8));
loc_8257BC30:
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

// sub_8257BC48: lifted from generated/ by lift.py.
lifted::Result f8257BC48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_8257BC48 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e68
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r24,0
  r24 = uint64_t(int64_t(0));
  // addi r23,r31,8
  r23 = uint64_t(int64_t(int64_t(r31) + 8));
  // mr r21,r4
  r21 = r4;
  // mr r20,r24
  r20 = r24;
  // lwz r8,4(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r22,1
  r22 = uint64_t(int64_t(1));
  // addi r11,r1,88
  r11 = uint64_t(int64_t(int64_t(r1) + 88));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // mr r10,r23
  r10 = r23;
  // lwz r25,8(r8)
  r25 = REX_LOAD_U32(uint32_t(r8) + 8);
  // addi r8,r31,28
  r8 = uint64_t(int64_t(int64_t(r31) + 28));
  // stw r24,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r24));
  // stw r8,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r8));
loc_8257BC8C:
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplwi r8,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq 0x8257bcc8
  if (cr0.eq) goto loc_8257BCC8;
  // lwz r7,8(r10)
  r7 = REX_LOAD_U32(uint32_t(r10) + 8);
  // cmpwi cr6,r7,1
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 1, so);
  // bne cr6,0x8257bcb0
  if (!cr6.eq) goto loc_8257BCB0;
  // lwz r8,8(r8)
  r8 = REX_LOAD_U32(uint32_t(r8) + 8);
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // b 0x8257bcb4
  goto loc_8257BCB4;
loc_8257BCB0:
  // stw r24,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r24));
loc_8257BCB4:
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x8257bccc
  if (cr6.eq) goto loc_8257BCCC;
  // addi r20,r20,1
  r20 = uint64_t(int64_t(int64_t(r20) + 1));
  // b 0x8257bccc
  goto loc_8257BCCC;
loc_8257BCC8:
  // stw r24,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r24));
loc_8257BCCC:
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x8257bc8c
  if (!cr0.eq) goto loc_8257BC8C;
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x8257be3c
  if (cr6.eq) goto loc_8257BE3C;
  // cmplwi cr6,r20,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r20), 0, so);
  // beq cr6,0x8257be3c
  if (cr6.eq) goto loc_8257BE3C;
  // mr r22,r24
  r22 = r24;
  // b 0x8257be34
  goto loc_8257BE34;
loc_8257BCF4:
  // addi r21,r21,-1
  r21 = uint64_t(int64_t(int64_t(r21) + -1));
  // cmplwi cr6,r25,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r25), 0, so);
  // beq cr6,0x8257be3c
  if (cr6.eq) goto loc_8257BE3C;
  // cmplwi cr6,r20,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r20), 0, so);
  // beq cr6,0x8257be3c
  if (cr6.eq) goto loc_8257BE3C;
  // lwz r11,0(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 0);
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r9,24(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 24);
  // addi r4,r31,64
  r4 = uint64_t(int64_t(int64_t(r31) + 64));
  // addi r3,r31,400
  r3 = uint64_t(int64_t(int64_t(r31) + 400));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // add r5,r10,r11
  r5 = r10 + r11;
  // lwz r11,24(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 24);
  // rlwinm r6,r11,31,1,31
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 31) & 0x7FFFFFFF;
  // bl 0x8257f5e8
  // (left out: dead in C++)
  r3 = f8257F5E8(base, r1, r3, r4, r5, r6, r7);
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // stb r24,48(r31)
  REX_STORE_U8(uint32_t(r31) + 48, uint8_t(r24));
  // addi r3,r31,52
  r3 = uint64_t(int64_t(int64_t(r31) + 52));
  // sth r24,46(r31)
  REX_STORE_U16(uint32_t(r31) + 46, uint16_t(r24));
  // bl 0x8257fa90
  // (left out: dead in C++)
  { const lifted::Result o = f8257FA90(base, r1, r13, r3, r4, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8257be3c
  if (!cr0.eq) goto loc_8257BE3C;
  // lhz r29,46(r31)
  r29 = REX_LOAD_U16(uint32_t(r31) + 46);
  // mr r22,r24
  r22 = r24;
  // addi r28,r1,88
  r28 = uint64_t(int64_t(int64_t(r1) + 88));
  // mr r27,r23
  r27 = r23;
  // li r26,2
  r26 = uint64_t(int64_t(2));
loc_8257BD68:
  // lwz r30,0(r28)
  r30 = REX_LOAD_U32(uint32_t(r28) + 0);
  // cmplwi r30,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq 0x8257bddc
  if (cr0.eq) goto loc_8257BDDC;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r5,r29
  r5 = r29;
  // addi r4,r31,384
  r4 = uint64_t(int64_t(int64_t(r31) + 384));
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // add r3,r10,r11
  r3 = r10 + r11;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x8257BD90)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // add r9,r9,r29
  r9 = r9 + r29;
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // bge cr6,0x8257bdac
  if (!cr6.lt) goto loc_8257BDAC;
  // mr r10,r9
  r10 = r9;
loc_8257BDAC:
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x8257bddc
  if (!cr6.eq) goto loc_8257BDDC;
  // mr r4,r30
  r4 = r30;
  // lwz r3,0(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + 0);
  // bl 0x8257c590
  // (left out: dead in C++)
  r3 = f8257C590(base, r3, r4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r3));
  // bne 0x8257bddc
  if (!cr0.eq) goto loc_8257BDDC;
  // addi r20,r20,-1
  r20 = uint64_t(int64_t(int64_t(r20) + -1));
loc_8257BDDC:
  // addic. r26,r26,-1
  ca = uint32_t(r26) > 0;
  r26 = uint64_t(int64_t(int64_t(r26) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r26), 0, so);
  // addi r27,r27,4
  r27 = uint64_t(int64_t(int64_t(r27) + 4));
  // addi r28,r28,4
  r28 = uint64_t(int64_t(int64_t(r28) + 4));
  // bne 0x8257bd68
  if (!cr0.eq) goto loc_8257BD68;
  // lwz r11,0(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 0);
  // lwz r10,24(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 24);
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r9,24(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 24);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // add r9,r8,r9
  r9 = r8 + r9;
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // bge cr6,0x8257be10
  if (!cr6.lt) goto loc_8257BE10;
  // mr r10,r9
  r10 = r9;
loc_8257BE10:
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,0(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x8257be34
  if (!cr6.eq) goto loc_8257BE34;
  // mr r4,r25
  r4 = r25;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8257c590
  // (left out: dead in C++)
  r3 = f8257C590(base, r3, r4);
  // mr r25,r3
  r25 = r3;
loc_8257BE34:
  // cmplwi cr6,r21,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r21), 0, so);
  // bne cr6,0x8257bcf4
  if (!cr6.eq) goto loc_8257BCF4;
loc_8257BE3C:
  // mr r3,r22
  r3 = r22;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6eb8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257BE48: lifted from generated/ by lift.py.
lifted::Result f8257BE48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257BE48 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0;
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
  // bl 0x8257bb60
  // (left out: dead in C++)
  r3 = f8257BB60(base, r1, r13);
  // addi r30,r31,52
  r30 = uint64_t(int64_t(int64_t(r31) + 52));
  // mr r3,r30
  r3 = r30;
  // bl 0x82580170
  // (left out: dead in C++)
  { const lifted::Result o = f82580170(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,16
  r11 = uint32_t(r3) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x8257be8c
  if (cr0.eq) goto loc_8257BE8C;
  // mr r3,r30
  r3 = r30;
  // bl 0x82580020
  // (left out: dead in C++)
  { const lifted::Result o = f82580020(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // b 0x8257bea8
  goto loc_8257BEA8;
loc_8257BE8C:
  // addi r11,r31,64
  r11 = uint64_t(int64_t(int64_t(r31) + 64));
  // addi r10,r31,704
  r10 = uint64_t(int64_t(int64_t(r31) + 704));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r11,28(r31)
  REX_STORE_U32(uint32_t(r31) + 28, uint32_t(r11));
  // stw r10,36(r31)
  REX_STORE_U32(uint32_t(r31) + 36, uint32_t(r10));
  // sth r9,40(r31)
  REX_STORE_U16(uint32_t(r31) + 40, uint16_t(r9));
loc_8257BEA8:
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

// sub_8257BEC0: lifted from generated/ by lift.py.
lifted::Result f8257BEC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, double f1) {
  ITC_NATIVE_RUNNING("sub_8257BEC0 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e5c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-880(r1)
  ea = -880 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // li r23,0
  r23 = uint64_t(int64_t(0));
  // addi r21,r31,28
  r21 = uint64_t(int64_t(int64_t(r31) + 28));
  // addi r20,r31,8
  r20 = uint64_t(int64_t(int64_t(r31) + 8));
  // mr r19,r4
  r19 = r4;
  // lwz r8,4(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 4);
  // mr r18,r23
  r18 = r23;
  // li r17,1
  r17 = uint64_t(int64_t(1));
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // mr r10,r20
  r10 = r20;
  // lwz r22,8(r8)
  r22 = REX_LOAD_U32(uint32_t(r8) + 8);
  // stw r21,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r21));
  // stw r23,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r23));
loc_8257BF04:
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplwi r8,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq 0x8257bf40
  if (cr0.eq) goto loc_8257BF40;
  // lwz r7,8(r10)
  r7 = REX_LOAD_U32(uint32_t(r10) + 8);
  // cmpwi cr6,r7,1
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 1, so);
  // bne cr6,0x8257bf28
  if (!cr6.eq) goto loc_8257BF28;
  // lwz r8,8(r8)
  r8 = REX_LOAD_U32(uint32_t(r8) + 8);
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // b 0x8257bf2c
  goto loc_8257BF2C;
loc_8257BF28:
  // stw r23,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r23));
loc_8257BF2C:
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r8,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq cr6,0x8257bf44
  if (cr6.eq) goto loc_8257BF44;
  // addi r18,r18,1
  r18 = uint64_t(int64_t(int64_t(r18) + 1));
  // b 0x8257bf44
  goto loc_8257BF44;
loc_8257BF40:
  // stw r23,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r23));
loc_8257BF44:
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bne 0x8257bf04
  if (!cr0.eq) goto loc_8257BF04;
  // cmplwi cr6,r22,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r22), 0, so);
  // beq cr6,0x8257c154
  if (cr6.eq) goto loc_8257C154;
  // cmplwi cr6,r18,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), 0, so);
  // beq cr6,0x8257c154
  if (cr6.eq) goto loc_8257C154;
  // mr r17,r23
  r17 = r23;
  // b 0x8257c14c
  goto loc_8257C14C;
loc_8257BF6C:
  // addi r19,r19,-1
  r19 = uint64_t(int64_t(int64_t(r19) + -1));
  // cmplwi cr6,r22,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r22), 0, so);
  // beq cr6,0x8257c154
  if (cr6.eq) goto loc_8257C154;
  // cmplwi cr6,r18,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r18), 0, so);
  // beq cr6,0x8257c154
  if (cr6.eq) goto loc_8257C154;
  // lwz r10,24(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 24);
  // addi r3,r31,704
  r3 = uint64_t(int64_t(int64_t(r31) + 704));
  // lwz r11,0(r22)
  r11 = REX_LOAD_U32(uint32_t(r22) + 0);
  // lwz r5,24(r10)
  r5 = REX_LOAD_U32(uint32_t(r10) + 24);
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // add r4,r10,r11
  r4 = r10 + r11;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x8257BFA0)); r3 = o.r3; f1 = o.f1; }
  // addi r28,r31,64
  r28 = uint64_t(int64_t(int64_t(r31) + 64));
  // addi r30,r31,52
  r30 = uint64_t(int64_t(int64_t(r31) + 52));
  // stb r23,44(r31)
  REX_STORE_U8(uint32_t(r31) + 44, uint8_t(r23));
  // sth r23,32(r31)
  REX_STORE_U16(uint32_t(r31) + 32, uint16_t(r23));
  // sth r23,42(r31)
  REX_STORE_U16(uint32_t(r31) + 42, uint16_t(r23));
  // stw r28,0(r21)
  REX_STORE_U32(uint32_t(r21) + 0, uint32_t(r28));
loc_8257BFB8:
  // addi r5,r1,88
  r5 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // mr r3,r30
  r3 = r30;
  // bl 0x8257feb8
  // (left out: dead in C++)
  { const lifted::Result o = f8257FEB8(base, r1, r13, r3, r4, r5, f1); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // clrlwi. r11,r29,16
  r11 = uint32_t(r29) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8257c004
  if (!cr0.eq) goto loc_8257C004;
  // lhz r11,32(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 32);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r10,0(r21)
  r10 = REX_LOAD_U32(uint32_t(r21) + 0);
  // rotlwi r11,r11,1
  r11 = __builtin_rotateleft32(uint32_t(r11), 1);
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // sth r23,32(r31)
  REX_STORE_U16(uint32_t(r31) + 32, uint16_t(r23));
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,0(r21)
  REX_STORE_U32(uint32_t(r21) + 0, uint32_t(r11));
  // bl 0x8257ff50
  // (left out: dead in C++)
  r3 = f8257FF50(base, r3, r4);
  // lbz r11,80(r1)
  r11 = REX_LOAD_U8(uint32_t(r1) + 80);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257bfb8
  if (cr0.eq) goto loc_8257BFB8;
loc_8257C004:
  // clrlwi. r11,r29,16
  r11 = uint32_t(r29) & 0xFFFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x8257c154
  if (!cr0.eq) goto loc_8257C154;
  // li r11,160
  r11 = uint64_t(int64_t(160));
  // lhz r24,42(r31)
  r24 = REX_LOAD_U16(uint32_t(r31) + 42);
  // li r5,320
  r5 = uint64_t(int64_t(320));
  // mr r4,r28
  r4 = r28;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // sth r11,32(r31)
  REX_STORE_U16(uint32_t(r31) + 32, uint16_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x8257C028)); r3 = o.r3; f1 = o.f1; }
  // li r7,1
  r7 = uint64_t(int64_t(1));
  // li r6,160
  r6 = uint64_t(int64_t(160));
  // addi r5,r1,112
  r5 = uint64_t(int64_t(int64_t(r1) + 112));
  // mr r4,r28
  r4 = r28;
  // addi r3,r31,720
  r3 = uint64_t(int64_t(int64_t(r31) + 720));
  // bl 0x8257f5e8
  // (left out: dead in C++)
  r3 = f8257F5E8(base, r1, r3, r4, r5, r6, r7);
  // addi r27,r1,96
  r27 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r26,r20
  r26 = r20;
  // li r25,2
  r25 = uint64_t(int64_t(2));
loc_8257C04C:
  // lwz r30,0(r27)
  r30 = REX_LOAD_U32(uint32_t(r27) + 0);
  // cmplwi r30,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq 0x8257c0fc
  if (cr0.eq) goto loc_8257C0FC;
  // lhz r11,32(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 32);
  // li r10,8000
  r10 = uint64_t(int64_t(8000));
  // mulli r11,r11,16000
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(16000))));
  // divw r11,r11,r10
  r11 = uint32_t((int32_t(r10) && !(int32_t(r11) == INT32_MIN && int32_t(r10) == -1)) ? int32_t(r11) / int32_t(r10) : 0);
  // rlwinm r29,r11,1,0,30
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 1) & 0xFFFFFFFE;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // cmplw cr6,r29,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), uint32_t(r11), so);
  // blt cr6,0x8257c094
  if (cr6.lt) goto loc_8257C094;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // subf r29,r11,r10
  r29 = r10 - r11;
loc_8257C094:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r5,r29
  r5 = r29;
  // mr r4,r28
  r4 = r28;
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // add r3,r10,r11
  r3 = r10 + r11;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS(0x8257C0B0)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // add r9,r9,r29
  r9 = r9 + r29;
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // bge cr6,0x8257c0cc
  if (!cr6.lt) goto loc_8257C0CC;
  // mr r10,r9
  r10 = r9;
loc_8257C0CC:
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x8257c0fc
  if (!cr6.eq) goto loc_8257C0FC;
  // mr r4,r30
  r4 = r30;
  // lwz r3,0(r26)
  r3 = REX_LOAD_U32(uint32_t(r26) + 0);
  // bl 0x8257c590
  // (left out: dead in C++)
  r3 = f8257C590(base, r3, r4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r3));
  // bne 0x8257c0fc
  if (!cr0.eq) goto loc_8257C0FC;
  // addi r18,r18,-1
  r18 = uint64_t(int64_t(int64_t(r18) + -1));
loc_8257C0FC:
  // addic. r25,r25,-1
  ca = uint32_t(r25) > 0;
  r25 = uint64_t(int64_t(int64_t(r25) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r25), 0, so);
  // addi r26,r26,4
  r26 = uint64_t(int64_t(int64_t(r26) + 4));
  // addi r27,r27,4
  r27 = uint64_t(int64_t(int64_t(r27) + 4));
  // bne 0x8257c04c
  if (!cr0.eq) goto loc_8257C04C;
  // lwz r11,0(r22)
  r11 = REX_LOAD_U32(uint32_t(r22) + 0);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // add r9,r9,r24
  r9 = r9 + r24;
  // cmplw cr6,r9,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r10), so);
  // bge cr6,0x8257c128
  if (!cr6.lt) goto loc_8257C128;
  // mr r10,r9
  r10 = r9;
loc_8257C128:
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // lwz r11,0(r22)
  r11 = REX_LOAD_U32(uint32_t(r22) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bne cr6,0x8257c14c
  if (!cr6.eq) goto loc_8257C14C;
  // mr r4,r22
  r4 = r22;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x8257c590
  // (left out: dead in C++)
  r3 = f8257C590(base, r3, r4);
  // mr r22,r3
  r22 = r3;
loc_8257C14C:
  // cmplwi cr6,r19,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r19), 0, so);
  // bne cr6,0x8257bf6c
  if (!cr6.eq) goto loc_8257BF6C;
loc_8257C154:
  // mr r3,r17
  r3 = r17;
  // addi r1,r1,880
  r1 = uint64_t(int64_t(int64_t(r1) + 880));
  // b 0x825a6eac
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257C160: lifted from generated/ by lift.py.
uint64_t f8257C160(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8257C160 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r4
  r11 = r4;
  // cmplwi cr6,r11,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 2, so);
  // bne cr6,0x8257c174
  if (!cr6.eq) goto loc_8257C174;
  // lwz r3,4(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 4);
  // b 0x8257c180
  goto loc_8257C180;
loc_8257C174:
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r11,r3
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r3));
loc_8257C180:
  // mr r4,r5
  r4 = r5;
  // b 0x8257c610
  r3 = f8257C610(base, r1, r13, r3, r4);
  return r3;
}

// sub_8257C2B0: lifted from generated/ by lift.py.
uint64_t f8257C2B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_8257C2B0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r11,r4
  r11 = r4;
  // mr r28,r3
  r28 = r3;
  // mr r27,r5
  r27 = r5;
  // mr r26,r6
  r26 = r6;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8257c2dc
  if (cr6.eq) goto loc_8257C2DC;
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
loc_8257C2DC:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8257c2e8
  if (cr6.eq) goto loc_8257C2E8;
  // stw r30,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r30));
loc_8257C2E8:
  // lwz r10,4(r28)
  r10 = REX_LOAD_U32(uint32_t(r28) + 4);
  // lis r9,-16384
  r9 = uint64_t(int64_t(-1073741824));
  // ori r31,r9,1
  r31 = r9 | 1;
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x8257c34c
  if (cr0.eq) goto loc_8257C34C;
  // lwz r4,8(r10)
  r4 = REX_LOAD_U32(uint32_t(r10) + 8);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x8257c32c
  if (cr6.eq) goto loc_8257C32C;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x8257c34c
  if (cr6.eq) goto loc_8257C34C;
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r31,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r31));
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r3,4(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 4);
  // bl 0x8257c590
  // (left out: dead in C++)
  r3 = f8257C590(base, r3, r4);
  // b 0x8257c34c
  goto loc_8257C34C;
loc_8257C32C:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x8257c34c
  if (cr6.eq) goto loc_8257C34C;
loc_8257C334:
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // lwz r3,4(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + 4);
  // bl 0x8257c590
  // (left out: dead in C++)
  r3 = f8257C590(base, r3, r4);
  // mr. r4,r3
  r4 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bne 0x8257c334
  if (!cr0.eq) goto loc_8257C334;
loc_8257C34C:
  // addi r29,r28,8
  r29 = uint64_t(int64_t(int64_t(r28) + 8));
loc_8257C350:
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257c398
  if (cr0.eq) goto loc_8257C398;
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x8257c36c
  if (cr6.eq) goto loc_8257C36C;
  // cmplw cr6,r30,r26
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), uint32_t(r26), so);
  // bne cr6,0x8257c398
  if (!cr6.eq) goto loc_8257C398;
loc_8257C36C:
  // lwz r4,8(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // bne cr6,0x8257c3ac
  if (!cr6.eq) goto loc_8257C3AC;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x8257c398
  if (cr6.eq) goto loc_8257C398;
loc_8257C380:
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // bl 0x8257c590
  // (left out: dead in C++)
  r3 = f8257C590(base, r3, r4);
  // mr. r4,r3
  r4 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // bne 0x8257c380
  if (!cr0.eq) goto loc_8257C380;
loc_8257C398:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmplwi cr6,r30,2
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 2, so);
  // blt cr6,0x8257c350
  if (cr6.lt) goto loc_8257C350;
  // b 0x8257c3d4
  goto loc_8257C3D4;
loc_8257C3AC:
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x8257c3d4
  if (cr6.eq) goto loc_8257C3D4;
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // addi r10,r30,2
  r10 = uint64_t(int64_t(int64_t(r30) + 2));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
  // lwzx r3,r10,r28
  r3 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r28));
  // bl 0x8257c590
  // (left out: dead in C++)
  r3 = f8257C590(base, r3, r4);
loc_8257C3D4:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

// sub_8257C3E0: lifted from generated/ by lift.py.
lifted::Result f8257C3E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257C3E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x8257c480
  if (cr6.eq) goto loc_8257C480;
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8257c2b0
  // (left out: dead in C++)
  r3 = f8257C2B0(base, r1, r13, r3, r4, r5, r6);
  // lwz r11,24(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 24);
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257c434
  if (cr0.eq) goto loc_8257C434;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257c430
  if (cr0.eq) goto loc_8257C430;
  // mr r3,r30
  r3 = r30;
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x8257C430)); r3 = o.r3; f1 = o.f1; }
loc_8257C430:
  // stw r28,24(r30)
  REX_STORE_U32(uint32_t(r30) + 24, uint32_t(r28));
loc_8257C434:
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257c448
  if (cr0.eq) goto loc_8257C448;
  // bl 0x8257c6b0
  // (left out: dead in C++)
  { const lifted::Result o = f8257C6B0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // stw r28,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r28));
loc_8257C448:
  // addi r31,r30,8
  r31 = uint64_t(int64_t(int64_t(r30) + 8));
  // li r29,2
  r29 = uint64_t(int64_t(2));
loc_8257C450:
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257c464
  if (cr0.eq) goto loc_8257C464;
  // bl 0x8257c6b0
  // (left out: dead in C++)
  { const lifted::Result o = f8257C6B0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // stw r28,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r28));
loc_8257C464:
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // bne 0x8257c450
  if (!cr0.eq) goto loc_8257C450;
  // lis r4,25738
  r4 = uint64_t(int64_t(1686765568));
  // mr r3,r30
  r3 = r30;
  // ori r4,r4,8
  r4 = r4 | 8;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x8257C480)); r3 = o.r3; f1 = o.f1; }
loc_8257C480:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_8257C570: lifted from generated/ by lift.py.
uint64_t f8257C570(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8257C570 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr0;
  uint8_t so = 0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x8257c584
  goto loc_8257C584;
loc_8257C57C:
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // addi r3,r3,1
  r3 = uint64_t(int64_t(int64_t(r3) + 1));
loc_8257C584:
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x8257c57c
  if (!cr0.eq) goto loc_8257C57C;
  // blr
  return r3;
}

// sub_8257C590: lifted from generated/ by lift.py.
uint64_t f8257C590(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8257C590 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,0(r4)
  r11 = REX_LOAD_U32(uint32_t(r4) + 0);
  // mr r8,r3
  r8 = r3;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r9,259
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 259, so);
  // bne cr6,0x8257c5ac
  if (!cr6.eq) goto loc_8257C5AC;
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_8257C5AC:
  // addi r11,r8,8
  r11 = uint64_t(int64_t(int64_t(r8) + 8));
  // stw r10,0(r4)
  REX_STORE_U32(uint32_t(r4) + 0, uint32_t(r10));
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplw cr6,r4,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), uint32_t(r9), so);
  // bne cr6,0x8257c5e8
  if (!cr6.eq) goto loc_8257C5E8;
  // rotlwi r9,r9,0
  r9 = __builtin_rotateleft32(uint32_t(r9), 0);
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq 0x8257c5e8
  if (cr0.eq) goto loc_8257C5E8;
  // lwz r7,4(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplw cr6,r9,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r7), so);
  // bne cr6,0x8257c5dc
  if (!cr6.eq) goto loc_8257C5DC;
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
loc_8257C5DC:
  // lwz r7,4(r9)
  r7 = REX_LOAD_U32(uint32_t(r9) + 4);
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r10,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r10));
loc_8257C5E8:
  // stw r10,4(r4)
  REX_STORE_U32(uint32_t(r4) + 4, uint32_t(r10));
  // lwz r10,4(r8)
  r10 = REX_LOAD_U32(uint32_t(r8) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x8257c600
  if (cr0.eq) goto loc_8257C600;
  // stw r4,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r4));
  // b 0x8257c604
  goto loc_8257C604;
loc_8257C600:
  // stw r4,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r4));
loc_8257C604:
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r4,4(r8)
  REX_STORE_U32(uint32_t(r8) + 4, uint32_t(r4));
  // blr
  return r3;
}

// sub_8257C610: lifted from generated/ by lift.py.
uint64_t f8257C610(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8257C610 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r30,r3
  r30 = r3;
  // mr r29,r4
  r29 = r4;
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // lwz r31,0(r30)
  r31 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r31,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq 0x8257c68c
  if (cr0.eq) goto loc_8257C68C;
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // bne cr6,0x8257c644
  if (!cr6.eq) goto loc_8257C644;
  // stw r28,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r28));
loc_8257C644:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
  // stw r28,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r28));
  // bl 0x822962b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x822962B0, LIFTED_ARGS(0x8257C660)); r3 = o.r3; f1 = o.f1; }
  // li r10,259
  r10 = uint64_t(int64_t(259));
  // stw r28,4(r29)
  REX_STORE_U32(uint32_t(r29) + 4, uint32_t(r28));
  // addi r11,r30,8
  r11 = uint64_t(int64_t(int64_t(r30) + 8));
  // stw r10,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r10));
  // stw r29,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r29));
  // stw r28,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r28));
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x8257c698
  if (cr0.eq) goto loc_8257C698;
  // stw r31,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r31));
  // b 0x8257c69c
  goto loc_8257C69C;
loc_8257C68C:
  // lis r28,-32761
  r28 = uint64_t(int64_t(-2147024896));
  // ori r28,r28,122
  r28 = r28 | 122;
  // b 0x8257c6a0
  goto loc_8257C6A0;
loc_8257C698:
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
loc_8257C69C:
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
loc_8257C6A0:
  // mr r3,r28
  r3 = r28;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_8257C6B0: lifted from generated/ by lift.py.
lifted::Result f8257C6B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257C6B0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8257c6fc
  if (cr6.eq) goto loc_8257C6FC;
  // lwz r3,16(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 16);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257c6ec
  if (cr0.eq) goto loc_8257C6EC;
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // ori r4,r4,3
  r4 = r4 | 3;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x8257C6E4)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,16(r31)
  REX_STORE_U32(uint32_t(r31) + 16, uint32_t(r11));
loc_8257C6EC:
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // mr r3,r31
  r3 = r31;
  // ori r4,r4,32773
  r4 = r4 | 32773;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x8257C6FC)); r3 = o.r3; f1 = o.f1; }
loc_8257C6FC:
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

// sub_8257C710: lifted from generated/ by lift.py.
lifted::Result f8257C710(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_8257C710 (lifted)");
  uint64_t r4 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // beq cr6,0x8257c77c
  if (cr6.eq) goto loc_8257C77C;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257c750
  if (cr0.eq) goto loc_8257C750;
  // lis r4,24714
  r4 = uint64_t(int64_t(1619656704));
  // ori r4,r4,8194
  r4 = r4 | 8194;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x8257C74C)); r3 = o.r3; f1 = o.f1; }
  // stw r30,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r30));
loc_8257C750:
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257c76c
  if (cr0.eq) goto loc_8257C76C;
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // ori r4,r4,9
  r4 = r4 | 9;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x8257C768)); r3 = o.r3; f1 = o.f1; }
  // stw r30,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r30));
loc_8257C76C:
  // lis r4,24970
  r4 = uint64_t(int64_t(1636433920));
  // mr r3,r31
  r3 = r31;
  // ori r4,r4,32774
  r4 = r4 | 32774;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82297790, LIFTED_ARGS(0x8257C77C)); r3 = o.r3; f1 = o.f1; }
loc_8257C77C:
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

// sub_8257C948: lifted from generated/ by lift.py.
uint64_t f8257C948(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8257C948 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x825ba0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825BA0C8, LIFTED_ARGS(0x8257C964)); r3 = o.r3; f1 = o.f1; }
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // ld r9,328(r31)
  r9 = REX_LOAD_U64(uint32_t(r31) + 328);
  // mulli r10,r11,1000
  r10 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(1000))));
  // rotldi r11,r10,1
  r11 = __builtin_rotateleft64(r10, 1);
  // divd r3,r10,r9
  r3 = uint64_t(int64_t((int64_t(r9) && !(int64_t(r10) == INT64_MIN && int64_t(r9) == -1)) ? int64_t(r10) / int64_t(r9) : 0));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // tdllei r9,0
  if (int64_t(r9) == 0ll || r9 < 0ull) lifted::Trap(0);
  // andc r11,r9,r11
  r11 = r9 & ~r11;
  // tdlgei r11,-1
  if (int64_t(r11) == -1ll || r11 > 18446744073709551615ull) lifted::Trap(0);
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

// sub_8257C9A0: lifted from generated/ by lift.py.
uint64_t f8257C9A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8257C9A0 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,72(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 72);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x8257c9f0
  if (cr6.eq) goto loc_8257C9F0;
  // bl 0x8257c948
  // (left out: dead in C++)
  r3 = f8257C948(base, r1, r13, r3);
  // ld r10,24(r31)
  r10 = REX_LOAD_U64(uint32_t(r31) + 24);
  // ld r9,16(r31)
  r9 = REX_LOAD_U64(uint32_t(r31) + 16);
  // subf r10,r10,r3
  r10 = r3 - r10;
  // ld r11,40(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 40);
  // subf r10,r9,r10
  r10 = r10 - r9;
  // cmpld cr6,r10,r11
  cr6 = lifted::Compare<uint64_t>(r10, r11, so);
  // blt cr6,0x8257c9f0
  if (cr6.lt) goto loc_8257C9F0;
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // std r11,56(r31)
  REX_STORE_U64(uint32_t(r31) + 56, r11);
loc_8257C9F0:
  // mr r3,r30
  r3 = r30;
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

// sub_8257CA10: lifted from generated/ by lift.py.
uint64_t f8257CA10(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_8257CA10 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // mr r9,r3
  r9 = r3;
  // li r10,20
  r10 = uint64_t(int64_t(20));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // ld r7,16(r9)
  r7 = REX_LOAD_U64(uint32_t(r9) + 16);
  // lhz r8,32(r9)
  r8 = REX_LOAD_U16(uint32_t(r9) + 32);
  // divdu r10,r7,r10
  r10 = r10 ? r7 / r10 : 0;
  // lhz r11,66(r9)
  r11 = REX_LOAD_U16(uint32_t(r9) + 66);
  // std r6,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r6);
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // add r10,r10,r8
  r10 = r10 + r8;
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // srawi r8,r10,12
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0xFFF) != 0);
  r8 = uint64_t(int64_t(int32_t(r10) >> 12));
  // addze r8,r8
  temp.s64 = int64_t(r8) + ca;
  ca = temp.u32 < uint32_t(r8);
  r8 = uint64_t(int64_t(temp.s64));
  // rlwinm r8,r8,12,0,19
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 12) & 0xFFFFF000;
  // subf r10,r8,r10
  r10 = r10 - r8;
  // clrlwi r7,r10,16
  r7 = uint32_t(r10) & 0xFFFF;
  // addi r10,r7,4096
  r10 = uint64_t(int64_t(int64_t(r7) + 4096));
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // subf r8,r11,r10
  r8 = r10 - r11;
  // add r10,r10,r11
  r10 = r10 + r11;
  // srawi r11,r8,12
  ca = (int32_t(r8) < 0) & ((uint32_t(r8) & 0xFFF) != 0);
  r11 = uint64_t(int64_t(int32_t(r8) >> 12));
  // addze r11,r11
  temp.s64 = int64_t(r11) + ca;
  ca = temp.u32 < uint32_t(r11);
  r11 = uint64_t(int64_t(temp.s64));
  // srawi r3,r10,12
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0xFFF) != 0);
  r3 = uint64_t(int64_t(int32_t(r10) >> 12));
  // rlwinm r11,r11,12,0,19
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 12) & 0xFFFFF000;
  // addze r3,r3
  temp.s64 = int64_t(r3) + ca;
  ca = temp.u32 < uint32_t(r3);
  r3 = uint64_t(int64_t(temp.s64));
  // subf r11,r11,r8
  r11 = r8 - r11;
  // rlwinm r8,r3,12,0,19
  r8 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 12) & 0xFFFFF000;
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // subf r10,r8,r10
  r10 = r10 - r8;
  // clrlwi r8,r4,16
  r8 = uint32_t(r4) & 0xFFFF;
  // clrlwi r10,r10,16
  r10 = uint32_t(r10) & 0xFFFF;
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x8257cab8
  if (!cr6.lt) goto loc_8257CAB8;
  // cmplw cr6,r8,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r11), so);
  // blt cr6,0x8257caa4
  if (cr6.lt) goto loc_8257CAA4;
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // ble cr6,0x8257caf4
  if (!cr6.gt) goto loc_8257CAF4;
loc_8257CAA4:
  // lwz r11,356(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 356);
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,356(r9)
  REX_STORE_U32(uint32_t(r9) + 356, uint32_t(r11));
  // blr
  return r3;
loc_8257CAB8:
  // cmplw cr6,r8,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r11), so);
  // bge cr6,0x8257cac8
  if (!cr6.lt) goto loc_8257CAC8;
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // bgt cr6,0x8257caa4
  if (cr6.gt) goto loc_8257CAA4;
loc_8257CAC8:
  // cmplw cr6,r7,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r10), so);
  // bge cr6,0x8257caec
  if (!cr6.lt) goto loc_8257CAEC;
  // cmplw cr6,r7,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r7), uint32_t(r8), so);
  // bgt cr6,0x8257cae0
  if (cr6.gt) goto loc_8257CAE0;
  // cmplw cr6,r8,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r10), so);
  // ble cr6,0x8257cafc
  if (!cr6.gt) goto loc_8257CAFC;
loc_8257CAE0:
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_8257CAE4:
  // stw r6,356(r9)
  REX_STORE_U32(uint32_t(r9) + 356, uint32_t(r6));
  // blr
  return r3;
loc_8257CAEC:
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bgt cr6,0x8257cafc
  if (cr6.gt) goto loc_8257CAFC;
loc_8257CAF4:
  // cmplw cr6,r8,r7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r7), so);
  // blt cr6,0x8257cae0
  if (cr6.lt) goto loc_8257CAE0;
loc_8257CAFC:
  // subf r11,r7,r8
  r11 = r8 - r7;
  // ld r10,16(r9)
  r10 = REX_LOAD_U64(uint32_t(r9) + 16);
  // mr r3,r6
  r3 = r6;
  // addi r11,r11,4096
  r11 = uint64_t(int64_t(int64_t(r11) + 4096));
  // srawi r8,r11,12
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0xFFF) != 0);
  r8 = uint64_t(int64_t(int32_t(r11) >> 12));
  // addze r8,r8
  temp.s64 = int64_t(r8) + ca;
  ca = temp.u32 < uint32_t(r8);
  r8 = uint64_t(int64_t(temp.s64));
  // rlwinm r8,r8,12,0,19
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 12) & 0xFFFFF000;
  // subf r11,r8,r11
  r11 = r11 - r8;
  // clrlwi r11,r11,16
  r11 = uint32_t(r11) & 0xFFFF;
  // mulli r11,r11,20
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(20))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // add r11,r11,r10
  r11 = r11 + r10;
  // std r11,0(r5)
  REX_STORE_U64(uint32_t(r5) + 0, r11);
  // b 0x8257cae4
  goto loc_8257CAE4;
}

// sub_8257CB38: lifted from generated/ by lift.py.
uint64_t f8257CB38(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8257CB38 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // addi r10,r3,372
  r10 = uint64_t(int64_t(int64_t(r3) + 372));
  // lwz r8,0(r4)
  r8 = REX_LOAD_U32(uint32_t(r4) + 0);
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // mr r9,r3
  r9 = r3;
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257cb98
  if (cr0.eq) goto loc_8257CB98;
  // ld r7,24(r8)
  r7 = REX_LOAD_U64(uint32_t(r8) + 24);
loc_8257CB58:
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ld r6,24(r8)
  r6 = REX_LOAD_U64(uint32_t(r8) + 24);
  // cmpld cr6,r7,r6
  cr6 = lifted::Compare<uint64_t>(r7, r6, so);
  // blt cr6,0x8257cb78
  if (cr6.lt) goto loc_8257CB78;
  // mr r9,r11
  r9 = r11;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x8257cb58
  if (!cr0.eq) goto loc_8257CB58;
loc_8257CB78:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8257cbbc
  if (!cr6.eq) goto loc_8257CBBC;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // beq cr6,0x8257cb98
  if (cr6.eq) goto loc_8257CB98;
  // lwz r11,0(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 0);
  // ld r11,24(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 24);
  // cmpld cr6,r11,r7
  cr6 = lifted::Compare<uint64_t>(r11, r7, so);
  // beq cr6,0x8257cbfc
  if (cr6.eq) goto loc_8257CBFC;
loc_8257CB98:
  // stw r3,4(r4)
  REX_STORE_U32(uint32_t(r4) + 4, uint32_t(r3));
  // lwz r11,4(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257cbb0
  if (cr0.eq) goto loc_8257CBB0;
  // stw r4,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r4));
  // b 0x8257cbb4
  goto loc_8257CBB4;
loc_8257CBB0:
  // stw r4,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r4));
loc_8257CBB4:
  // stw r4,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r4));
  // blr
  return r3;
loc_8257CBBC:
  // ld r11,24(r8)
  r11 = REX_LOAD_U64(uint32_t(r8) + 24);
  // cmpld cr6,r11,r7
  cr6 = lifted::Compare<uint64_t>(r11, r7, so);
  // beq cr6,0x8257cbfc
  if (cr6.eq) goto loc_8257CBFC;
  // cmplwi cr6,r9,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // bne cr6,0x8257cbec
  if (!cr6.eq) goto loc_8257CBEC;
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r11,4(r4)
  REX_STORE_U32(uint32_t(r4) + 4, uint32_t(r11));
  // lwz r11,4(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r4,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r4));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // b 0x8257cbb4
  goto loc_8257CBB4;
loc_8257CBEC:
  // lwz r11,4(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 4);
  // stw r11,4(r4)
  REX_STORE_U32(uint32_t(r4) + 4, uint32_t(r11));
  // stw r4,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r4));
  // blr
  return r3;
loc_8257CBFC:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16389
  r3 = r3 | 16389;
  // blr
  return r3;
}

// sub_8257CC08: lifted from generated/ by lift.py.
void f8257CC08(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_8257CC08 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // lhz r11,64(r3)
  r11 = REX_LOAD_U16(uint32_t(r3) + 64);
  // clrlwi r8,r4,16
  r8 = uint32_t(r4) & 0xFFFF;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // subf r11,r11,r8
  r11 = r8 - r11;
  // addi r10,r3,372
  r10 = uint64_t(int64_t(int64_t(r3) + 372));
  // addi r11,r11,4096
  r11 = uint64_t(int64_t(int64_t(r11) + 4096));
  // srawi r8,r11,12
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0xFFF) != 0);
  r8 = uint64_t(int64_t(int32_t(r11) >> 12));
  // stw r9,312(r3)
  REX_STORE_U32(uint32_t(r3) + 312, uint32_t(r9));
  // stw r9,304(r3)
  REX_STORE_U32(uint32_t(r3) + 304, uint32_t(r9));
  // addze r8,r8
  temp.s64 = int64_t(r8) + ca;
  ca = temp.u32 < uint32_t(r8);
  r8 = uint64_t(int64_t(temp.s64));
  // rlwinm r8,r8,12,0,19
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 12) & 0xFFFFF000;
  // subf r11,r8,r11
  r11 = r11 - r8;
  // ld r8,24(r3)
  r8 = REX_LOAD_U64(uint32_t(r3) + 24);
  // sth r11,32(r3)
  REX_STORE_U16(uint32_t(r3) + 32, uint16_t(r11));
  // ld r11,16(r3)
  r11 = REX_LOAD_U64(uint32_t(r3) + 16);
  // std r9,16(r3)
  REX_STORE_U64(uint32_t(r3) + 16, r9);
  // add r11,r11,r8
  r11 = r11 + r8;
  // std r11,24(r3)
  REX_STORE_U64(uint32_t(r3) + 24, r11);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257cc78
  if (cr0.eq) goto loc_8257CC78;
  // lwz r8,4(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 4);
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bne cr6,0x8257cc6c
  if (!cr6.eq) goto loc_8257CC6C;
  // stw r9,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r9));
loc_8257CC6C:
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r8,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r8));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
loc_8257CC78:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return;
  // addi r7,r3,364
  r7 = uint64_t(int64_t(int64_t(r3) + 364));
loc_8257CC84:
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // lwz r8,4(r7)
  r8 = REX_LOAD_U32(uint32_t(r7) + 4);
  // cmplwi r8,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r8), 0, so);
  // beq 0x8257cc9c
  if (cr0.eq) goto loc_8257CC9C;
  // stw r11,4(r8)
  REX_STORE_U32(uint32_t(r8) + 4, uint32_t(r11));
  // b 0x8257cca0
  goto loc_8257CCA0;
loc_8257CC9C:
  // stw r11,0(r7)
  REX_STORE_U32(uint32_t(r7) + 0, uint32_t(r11));
loc_8257CCA0:
  // stw r11,4(r7)
  REX_STORE_U32(uint32_t(r7) + 4, uint32_t(r11));
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257cccc
  if (cr0.eq) goto loc_8257CCCC;
  // lwz r8,4(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 4);
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bne cr6,0x8257ccc0
  if (!cr6.eq) goto loc_8257CCC0;
  // stw r9,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r9));
loc_8257CCC0:
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r8,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r8));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
loc_8257CCCC:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x8257cc84
  if (!cr6.eq) goto loc_8257CC84;
  // blr
  return;
}

// sub_8257CCD8: lifted from generated/ by lift.py.
uint64_t f8257CCD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8257CCD8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r31,r3
  r31 = r3;
  // lwz r8,312(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 312);
  // lwz r9,304(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 304);
  // lhz r11,68(r31)
  r11 = REX_LOAD_U16(uint32_t(r31) + 68);
  // lhz r10,64(r31)
  r10 = REX_LOAD_U16(uint32_t(r31) + 64);
  // std r8,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r8);
  // subf r10,r11,r10
  r10 = r10 - r11;
  // std r9,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r9);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // lfd f13,88(r1)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 88)));
  // fcfid f12,f0
  f12 = double(std::bit_cast<int64_t>(f0));
  // fcfid f0,f13
  f0 = double(std::bit_cast<int64_t>(f13));
  // lfd f13,8784(r11)
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + 8784)));
  // fdiv f11,f12,f0
  f11 = f12 / f0;
  // fcmpu cr6,f11,f13
  cr6 = lifted::CompareFloat(f11, f13);
  // bge cr6,0x8257cd30
  if (!cr6.lt) goto loc_8257CD30;
  // fdiv f12,f12,f0
  f12 = f12 / f0;
  // b 0x8257cd34
  goto loc_8257CD34;
loc_8257CD30:
  // fmr f12,f13
  lifted::ScalarFloatMode();
  f12 = f13;
loc_8257CD34:
  // lfd f11,288(r31)
  lifted::ScalarFloatMode();
  f11 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r31) + 288)));
  // fmul f0,f11,f0
  f0 = f11 * f0;
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // blt cr6,0x8257cd48
  if (cr6.lt) goto loc_8257CD48;
  // fmr f0,f13
  f0 = f13;
loc_8257CD48:
  // fsub f13,f13,f0
  lifted::ScalarFloatMode();
  f13 = f13 - f0;
  // addi r11,r10,10
  r11 = uint64_t(int64_t(int64_t(r10) + 10));
  // fmul f12,f0,f12
  f12 = f0 * f12;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // stfd f0,296(r31)
  REX_STORE_U64(uint32_t(r31) + 296, std::bit_cast<uint64_t>(f0));
  // lfdx f0,r11,r31
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + uint32_t(r31))));
  // fmadd f0,f13,f0,f12
  f0 = std::fma(f13, f0, f12);
  // stfdx f0,r11,r31
  REX_STORE_U64(uint32_t(r11) + uint32_t(r31), std::bit_cast<uint64_t>(f0));
  // stw r26,304(r31)
  REX_STORE_U32(uint32_t(r31) + 304, uint32_t(r26));
  // lfd f0,280(r31)
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r31) + 280)));
  // stw r26,312(r31)
  REX_STORE_U32(uint32_t(r31) + 312, uint32_t(r26));
  // lfdx f13,r11,r31
  f13 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + uint32_t(r31))));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x8257cea8
  if (!cr6.lt) goto loc_8257CEA8;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x8257cf04
  if (cr6.eq) goto loc_8257CF04;
  // addi r11,r10,9
  r11 = uint64_t(int64_t(int64_t(r10) + 9));
  // fsub f13,f13,f0
  f13 = f13 - f0;
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // lfdx f12,r11,r31
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + uint32_t(r31))));
  // fsub f12,f12,f0
  f12 = f12 - f0;
  // fabs f0,f13
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) & ~0x8000000000000000));
  // fabs f13,f12
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f12) & ~0x8000000000000000));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x8257cf04
  if (!cr6.lt) goto loc_8257CF04;
  // ld r11,40(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 40);
  // addi r30,r31,372
  r30 = uint64_t(int64_t(int64_t(r31) + 372));
  // li r10,20
  r10 = uint64_t(int64_t(20));
  // addi r11,r11,-20
  r11 = uint64_t(int64_t(int64_t(r11) + -20));
  // divdu r10,r11,r10
  r10 = r10 ? r11 / r10 : 0;
  // lwz r28,0(r30)
  r28 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r28,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r28), 0, so);
  // std r11,40(r31)
  REX_STORE_U64(uint32_t(r31) + 40, r11);
  // sth r10,64(r31)
  REX_STORE_U16(uint32_t(r31) + 64, uint16_t(r10));
  // beq 0x8257cf04
  if (cr0.eq) goto loc_8257CF04;
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // mr r3,r30
  r3 = r30;
  // lwz r27,0(r28)
  r27 = REX_LOAD_U32(uint32_t(r28) + 0);
  // addi r11,r11,20
  r11 = uint64_t(int64_t(int64_t(r11) + 20));
  // ld r29,24(r27)
  r29 = REX_LOAD_U64(uint32_t(r27) + 24);
  // std r11,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r11);
  // bl 0x8257c570
  // (left out: dead in C++)
  r3 = f8257C570(base, r3);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x8257cf04
  if (cr0.eq) goto loc_8257CF04;
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // cmpld cr6,r11,r29
  cr6 = lifted::Compare<uint64_t>(r11, r29, so);
  // ble cr6,0x8257cf04
  if (!cr6.gt) goto loc_8257CF04;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r9,r29,20
  r9 = uint64_t(int64_t(int64_t(r29) + 20));
  // mr r8,r26
  r8 = r26;
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257ce98
  if (cr0.eq) goto loc_8257CE98;
loc_8257CE1C:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // ld r10,24(r10)
  r10 = REX_LOAD_U64(uint32_t(r10) + 24);
  // cmpld cr6,r10,r9
  cr6 = lifted::Compare<uint64_t>(r10, r9, so);
  // bgt cr6,0x8257ce40
  if (cr6.gt) goto loc_8257CE40;
  // bne cr6,0x8257ce34
  if (!cr6.eq) goto loc_8257CE34;
  // li r8,1
  r8 = uint64_t(int64_t(1));
loc_8257CE34:
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x8257ce1c
  if (!cr0.eq) goto loc_8257CE1C;
loc_8257CE40:
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq cr6,0x8257ce98
  if (cr6.eq) goto loc_8257CE98;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x8257ce70
  if (cr0.eq) goto loc_8257CE70;
  // lwz r10,4(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x8257ce64
  if (!cr6.eq) goto loc_8257CE64;
  // stw r26,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r26));
loc_8257CE64:
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r10,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r10));
  // stw r26,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r26));
loc_8257CE70:
  // addi r11,r31,364
  r11 = uint64_t(int64_t(int64_t(r31) + 364));
  // stw r26,4(r28)
  REX_STORE_U32(uint32_t(r28) + 4, uint32_t(r26));
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x8257ce8c
  if (cr0.eq) goto loc_8257CE8C;
  // stw r28,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r28));
  // b 0x8257ce90
  goto loc_8257CE90;
loc_8257CE8C:
  // stw r28,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r28));
loc_8257CE90:
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // b 0x8257cf04
  goto loc_8257CF04;
loc_8257CE98:
  // ld r11,24(r27)
  r11 = REX_LOAD_U64(uint32_t(r27) + 24);
  // addi r11,r11,20
  r11 = uint64_t(int64_t(int64_t(r11) + 20));
  // std r11,24(r27)
  REX_STORE_U64(uint32_t(r27) + 24, r11);
  // b 0x8257cf04
  goto loc_8257CF04;
loc_8257CEA8:
  // addi r11,r10,1
  r11 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmplwi cr6,r11,25
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 25, so);
  // bge cr6,0x8257cf04
  if (!cr6.lt) goto loc_8257CF04;
  // addi r11,r10,11
  r11 = uint64_t(int64_t(int64_t(r10) + 11));
  // fsub f13,f13,f0
  lifted::ScalarFloatMode();
  f13 = f13 - f0;
  // rlwinm r11,r11,3,0,28
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 3) & 0xFFFFFFF8;
  // lfdx f12,r11,r31
  f12 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r11) + uint32_t(r31))));
  // fsub f12,f12,f0
  f12 = f12 - f0;
  // fabs f0,f13
  f0 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f13) & ~0x8000000000000000));
  // fabs f13,f12
  f13 = std::bit_cast<double>(uint64_t(std::bit_cast<uint64_t>(f12) & ~0x8000000000000000));
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x8257cf04
  if (!cr6.lt) goto loc_8257CF04;
  // ld r11,16(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 16);
  // cmpldi cr6,r11,0
  cr6 = lifted::Compare<uint64_t>(r11, 0, so);
  // beq cr6,0x8257cf04
  if (cr6.eq) goto loc_8257CF04;
  // addi r11,r11,-20
  r11 = uint64_t(int64_t(int64_t(r11) + -20));
  // li r10,20
  r10 = uint64_t(int64_t(20));
  // std r11,16(r31)
  REX_STORE_U64(uint32_t(r31) + 16, r11);
  // ld r11,40(r31)
  r11 = REX_LOAD_U64(uint32_t(r31) + 40);
  // addi r11,r11,20
  r11 = uint64_t(int64_t(int64_t(r11) + 20));
  // divdu r10,r11,r10
  r10 = r10 ? r11 / r10 : 0;
  // std r11,40(r31)
  REX_STORE_U64(uint32_t(r31) + 40, r11);
  // sth r10,64(r31)
  REX_STORE_U16(uint32_t(r31) + 64, uint16_t(r10));
loc_8257CF04:
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return r3;
}

namespace {

lifted::Result e825761D8(uint8_t* base, const lifted::Args& a) {
  return f825761D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82576328(uint8_t* base, const lifted::Args& a) {
  return f82576328(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825763D0(uint8_t* base, const lifted::Args& a) {
  return f825763D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825764A8(uint8_t* base, const lifted::Args& a) {
  return f825764A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e82576540(uint8_t* base, const lifted::Args& a) {
  return f82576540(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e825765F0(uint8_t* base, const lifted::Args& a) {
  return f825765F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825766E0(uint8_t* base, const lifted::Args& a) {
  return f825766E0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e825767A0(uint8_t* base, const lifted::Args& a) {
  return {f825767A0(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82576BE0(uint8_t* base, const lifted::Args& a) {
  return f82576BE0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82576F30(uint8_t* base, const lifted::Args& a) {
  return f82576F30(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e82577088(uint8_t* base, const lifted::Args& a) {
  return {f82577088(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825770B8(uint8_t* base, const lifted::Args& a) {
  return {f825770B8(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82577110(uint8_t* base, const lifted::Args& a) {
  return {f82577110(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82577140(uint8_t* base, const lifted::Args& a) {
  return {f82577140(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82577190(uint8_t* base, const lifted::Args& a) {
  return {f82577190(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825774D0(uint8_t* base, const lifted::Args& a) {
  return {f825774D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82577528(uint8_t* base, const lifted::Args& a) {
  return {f82577528(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82577630(uint8_t* base, const lifted::Args& a) {
  return {f82577630(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e825776A8(uint8_t* base, const lifted::Args& a) {
  return f825776A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82577788(uint8_t* base, const lifted::Args& a) {
  return {f82577788(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]), a.f[0]};
}

lifted::Result e82577850(uint8_t* base, const lifted::Args& a) {
  return {f82577850(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e825778D8(uint8_t* base, const lifted::Args& a) {
  return f825778D8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.f[0]);
}

lifted::Result e825780A0(uint8_t* base, const lifted::Args& a) {
  return f825780A0(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e825780D0(uint8_t* base, const lifted::Args& a) {
  f825780D0(base, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]);
  return {a.r[0], a.f[0]};
}

lifted::Result e825780F0(uint8_t* base, const lifted::Args& a) {
  return {f825780F0(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82578118(uint8_t* base, const lifted::Args& a) {
  return {f82578118(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82578248(uint8_t* base, const lifted::Args& a) {
  return {f82578248(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825782C0(uint8_t* base, const lifted::Args& a) {
  return {f825782C0(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82578340(uint8_t* base, const lifted::Args& a) {
  return {f82578340(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e825783C8(uint8_t* base, const lifted::Args& a) {
  return {f825783C8(base, a.r1, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82578500(uint8_t* base, const lifted::Args& a) {
  return {f82578500(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82578580(uint8_t* base, const lifted::Args& a) {
  return {f82578580(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82578588(uint8_t* base, const lifted::Args& a) {
  return {f82578588(base, a.r[0], a.r[1], a.r[2], a.r[3], a.r[5], a.r[6]), a.f[0]};
}

lifted::Result e82578608(uint8_t* base, const lifted::Args& a) {
  return {f82578608(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82578658(uint8_t* base, const lifted::Args& a) {
  return {f82578658(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e825786B8(uint8_t* base, const lifted::Args& a) {
  return {f825786B8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4]), a.f[0]};
}

lifted::Result e825787D0(uint8_t* base, const lifted::Args& a) {
  return {f825787D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e825788C0(uint8_t* base, const lifted::Args& a) {
  return f825788C0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82578AF0(uint8_t* base, const lifted::Args& a) {
  return f82578AF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82578C88(uint8_t* base, const lifted::Args& a) {
  f82578C88(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82578CB8(uint8_t* base, const lifted::Args& a) {
  f82578CB8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82578CD0(uint8_t* base, const lifted::Args& a) {
  return {f82578CD0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82578D70(uint8_t* base, const lifted::Args& a) {
  return {f82578D70(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e82578F00(uint8_t* base, const lifted::Args& a) {
  return f82578F00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825790F0(uint8_t* base, const lifted::Args& a) {
  return f825790F0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82579108(uint8_t* base, const lifted::Args& a) {
  return f82579108(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825791B0(uint8_t* base, const lifted::Args& a) {
  return f825791B0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82579280(uint8_t* base, const lifted::Args& a) {
  return f82579280(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82579318(uint8_t* base, const lifted::Args& a) {
  return {f82579318(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e82579448(uint8_t* base, const lifted::Args& a) {
  return f82579448(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82579590(uint8_t* base, const lifted::Args& a) {
  return {f82579590(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e82579630(uint8_t* base, const lifted::Args& a) {
  return f82579630(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825796C0(uint8_t* base, const lifted::Args& a) {
  return f825796C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82579798(uint8_t* base, const lifted::Args& a) {
  return f82579798(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e825797B0(uint8_t* base, const lifted::Args& a) {
  return {f825797B0(base), a.f[0]};
}

lifted::Result e825797C0(uint8_t* base, const lifted::Args& a) {
  return {f825797C0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82579830(uint8_t* base, const lifted::Args& a) {
  return f82579830(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82579880(uint8_t* base, const lifted::Args& a) {
  f82579880(base, a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e82579898(uint8_t* base, const lifted::Args& a) {
  return f82579898(base, a.r1, a.r13, a.r[0], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82579968(uint8_t* base, const lifted::Args& a) {
  return f82579968(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e82579A88(uint8_t* base, const lifted::Args& a) {
  return f82579A88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e82579BE8(uint8_t* base, const lifted::Args& a) {
  return f82579BE8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82579C48(uint8_t* base, const lifted::Args& a) {
  return f82579C48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82579CD0(uint8_t* base, const lifted::Args& a) {
  return f82579CD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e82579CF0(uint8_t* base, const lifted::Args& a) {
  return {f82579CF0(base), a.f[0]};
}

lifted::Result e82579D00(uint8_t* base, const lifted::Args& a) {
  return {f82579D00(base, a.r[1]), a.f[0]};
}

lifted::Result e82579D40(uint8_t* base, const lifted::Args& a) {
  return {f82579D40(base, a.r[1]), a.f[0]};
}

lifted::Result e82579D60(uint8_t* base, const lifted::Args& a) {
  return {f82579D60(base, a.r[0], a.r[1], a.r[3]), a.f[0]};
}

lifted::Result e82579D88(uint8_t* base, const lifted::Args& a) {
  return {f82579D88(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e82579DA0(uint8_t* base, const lifted::Args& a) {
  return f82579DA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e8257A3D0(uint8_t* base, const lifted::Args& a) {
  f8257A3D0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8257A3F0(uint8_t* base, const lifted::Args& a) {
  return {f8257A3F0(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8257B9A8(uint8_t* base, const lifted::Args& a) {
  return {f8257B9A8(base, a.r[0]), a.f[0]};
}

lifted::Result e8257B9D0(uint8_t* base, const lifted::Args& a) {
  return {f8257B9D0(base, a.r[0]), a.f[0]};
}

lifted::Result e8257B9F8(uint8_t* base, const lifted::Args& a) {
  return {f8257B9F8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8257BA88(uint8_t* base, const lifted::Args& a) {
  return f8257BA88(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8257BAA0(uint8_t* base, const lifted::Args& a) {
  return {f8257BAA0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8257BAD0(uint8_t* base, const lifted::Args& a) {
  return {f8257BAD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8257BB00(uint8_t* base, const lifted::Args& a) {
  return f8257BB00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257BB28(uint8_t* base, const lifted::Args& a) {
  return f8257BB28(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257BB50(uint8_t* base, const lifted::Args& a) {
  return f8257BB50(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.lr);
}

lifted::Result e8257BB60(uint8_t* base, const lifted::Args& a) {
  return {f8257BB60(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e8257BBC8(uint8_t* base, const lifted::Args& a) {
  return f8257BBC8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257BC48(uint8_t* base, const lifted::Args& a) {
  return f8257BC48(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e8257BE48(uint8_t* base, const lifted::Args& a) {
  return f8257BE48(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257BEC0(uint8_t* base, const lifted::Args& a) {
  return f8257BEC0(base, a.r1, a.r13, a.r[0], a.r[1], a.f[0]);
}

lifted::Result e8257C160(uint8_t* base, const lifted::Args& a) {
  return {f8257C160(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8257C2B0(uint8_t* base, const lifted::Args& a) {
  return {f8257C2B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3]), a.f[0]};
}

lifted::Result e8257C3E0(uint8_t* base, const lifted::Args& a) {
  return f8257C3E0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257C570(uint8_t* base, const lifted::Args& a) {
  return {f8257C570(base, a.r[0]), a.f[0]};
}

lifted::Result e8257C590(uint8_t* base, const lifted::Args& a) {
  return {f8257C590(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8257C610(uint8_t* base, const lifted::Args& a) {
  return {f8257C610(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8257C6B0(uint8_t* base, const lifted::Args& a) {
  return f8257C6B0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257C710(uint8_t* base, const lifted::Args& a) {
  return f8257C710(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e8257C948(uint8_t* base, const lifted::Args& a) {
  return {f8257C948(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8257C9A0(uint8_t* base, const lifted::Args& a) {
  return {f8257C9A0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e8257CA10(uint8_t* base, const lifted::Args& a) {
  return {f8257CA10(base, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e8257CB38(uint8_t* base, const lifted::Args& a) {
  return {f8257CB38(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e8257CC08(uint8_t* base, const lifted::Args& a) {
  f8257CC08(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e8257CCD8(uint8_t* base, const lifted::Args& a) {
  return {f8257CCD8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

const Registration kEntries[] = {
    {0x825761D8, e825761D8},
    {0x82576328, e82576328},
    {0x825763D0, e825763D0},
    {0x825764A8, e825764A8},
    {0x82576540, e82576540},
    {0x825765F0, e825765F0},
    {0x825766E0, e825766E0},
    {0x825767A0, e825767A0},
    {0x82576BE0, e82576BE0},
    {0x82576F30, e82576F30},
    {0x82577088, e82577088},
    {0x825770B8, e825770B8},
    {0x82577110, e82577110},
    {0x82577140, e82577140},
    {0x82577190, e82577190},
    {0x825774D0, e825774D0},
    {0x82577528, e82577528},
    {0x82577630, e82577630},
    {0x825776A8, e825776A8},
    {0x82577788, e82577788},
    {0x82577850, e82577850},
    {0x825778D8, e825778D8},
    {0x825780A0, e825780A0},
    {0x825780D0, e825780D0},
    {0x825780F0, e825780F0},
    {0x82578118, e82578118},
    {0x82578248, e82578248},
    {0x825782C0, e825782C0},
    {0x82578340, e82578340},
    {0x825783C8, e825783C8},
    {0x82578500, e82578500},
    {0x82578580, e82578580},
    {0x82578588, e82578588},
    {0x82578608, e82578608},
    {0x82578658, e82578658},
    {0x825786B8, e825786B8},
    {0x825787D0, e825787D0},
    {0x825788C0, e825788C0},
    {0x82578AF0, e82578AF0},
    {0x82578C88, e82578C88},
    {0x82578CB8, e82578CB8},
    {0x82578CD0, e82578CD0},
    {0x82578D70, e82578D70},
    {0x82578F00, e82578F00},
    {0x825790F0, e825790F0},
    {0x82579108, e82579108},
    {0x825791B0, e825791B0},
    {0x82579280, e82579280},
    {0x82579318, e82579318},
    {0x82579448, e82579448},
    {0x82579590, e82579590},
    {0x82579630, e82579630},
    {0x825796C0, e825796C0},
    {0x82579798, e82579798},
    {0x825797B0, e825797B0},
    {0x825797C0, e825797C0},
    {0x82579830, e82579830},
    {0x82579880, e82579880},
    {0x82579898, e82579898},
    {0x82579968, e82579968},
    {0x82579A88, e82579A88},
    {0x82579BE8, e82579BE8},
    {0x82579C48, e82579C48},
    {0x82579CD0, e82579CD0},
    {0x82579CF0, e82579CF0},
    {0x82579D00, e82579D00},
    {0x82579D40, e82579D40},
    {0x82579D60, e82579D60},
    {0x82579D88, e82579D88},
    {0x82579DA0, e82579DA0},
    {0x8257A3D0, e8257A3D0},
    {0x8257A3F0, e8257A3F0},
    {0x8257B9A8, e8257B9A8},
    {0x8257B9D0, e8257B9D0},
    {0x8257B9F8, e8257B9F8},
    {0x8257BA88, e8257BA88},
    {0x8257BAA0, e8257BAA0},
    {0x8257BAD0, e8257BAD0},
    {0x8257BB00, e8257BB00},
    {0x8257BB28, e8257BB28},
    {0x8257BB50, e8257BB50},
    {0x8257BB60, e8257BB60},
    {0x8257BBC8, e8257BBC8},
    {0x8257BC48, e8257BC48},
    {0x8257BE48, e8257BE48},
    {0x8257BEC0, e8257BEC0},
    {0x8257C160, e8257C160},
    {0x8257C2B0, e8257C2B0},
    {0x8257C3E0, e8257C3E0},
    {0x8257C570, e8257C570},
    {0x8257C590, e8257C590},
    {0x8257C610, e8257C610},
    {0x8257C6B0, e8257C6B0},
    {0x8257C710, e8257C710},
    {0x8257C948, e8257C948},
    {0x8257C9A0, e8257C9A0},
    {0x8257CA10, e8257CA10},
    {0x8257CB38, e8257CB38},
    {0x8257CC08, e8257CC08},
    {0x8257CCD8, e8257CCD8},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_018)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_825761D8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825761D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825761D8);
}

extern "C" REX_FUNC(sub_82576328) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82576328(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82576328);
}

extern "C" REX_FUNC(sub_825763D0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825763D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825763D0);
}

extern "C" REX_FUNC(sub_825764A8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825764A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825764A8);
}

extern "C" REX_FUNC(sub_82576540) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82576540(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82576540);
}

extern "C" REX_FUNC(sub_825765F0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825765F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825765F0);
}

extern "C" REX_FUNC(sub_825766E0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825766E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825766E0);
}

extern "C" REX_FUNC(sub_825767A0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825767A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825767A0);
}

extern "C" REX_FUNC(sub_82576BE0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82576BE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82576BE0);
}

extern "C" REX_FUNC(sub_82576F30) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82576F30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82576F30);
}

extern "C" REX_FUNC(sub_82577088) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82577088(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82577088);
}

extern "C" REX_FUNC(sub_825770B8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825770B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825770B8);
}

extern "C" REX_FUNC(sub_82577110) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82577110(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82577110);
}

extern "C" REX_FUNC(sub_82577140) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82577140(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82577140);
}

extern "C" REX_FUNC(sub_82577190) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82577190(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82577190);
}

extern "C" REX_FUNC(sub_825774D0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825774D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825774D0);
}

extern "C" REX_FUNC(sub_82577528) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82577528(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82577528);
}

extern "C" REX_FUNC(sub_82577630) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82577630(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82577630);
}

extern "C" REX_FUNC(sub_825776A8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825776A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825776A8);
}

extern "C" REX_FUNC(sub_82577788) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82577788(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82577788);
}

extern "C" REX_FUNC(sub_82577850) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82577850(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82577850);
}

extern "C" REX_FUNC(sub_825778D8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825778D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825778D8);
}

extern "C" REX_FUNC(sub_825780A0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825780A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825780A0);
}

extern "C" REX_FUNC(sub_825780D0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825780D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825780D0);
}

extern "C" REX_FUNC(sub_825780F0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825780F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825780F0);
}

extern "C" REX_FUNC(sub_82578118) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578118(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578118);
}

extern "C" REX_FUNC(sub_82578248) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578248(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578248);
}

extern "C" REX_FUNC(sub_825782C0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825782C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825782C0);
}

extern "C" REX_FUNC(sub_82578340) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578340(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578340);
}

extern "C" REX_FUNC(sub_825783C8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825783C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825783C8);
}

extern "C" REX_FUNC(sub_82578500) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578500(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578500);
}

extern "C" REX_FUNC(sub_82578580) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578580(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578580);
}

extern "C" REX_FUNC(sub_82578588) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578588(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578588);
}

extern "C" REX_FUNC(sub_82578608) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578608(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578608);
}

extern "C" REX_FUNC(sub_82578658) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578658(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578658);
}

extern "C" REX_FUNC(sub_825786B8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825786B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825786B8);
}

extern "C" REX_FUNC(sub_825787D0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825787D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825787D0);
}

extern "C" REX_FUNC(sub_825788C0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825788C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825788C0);
}

extern "C" REX_FUNC(sub_82578AF0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578AF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578AF0);
}

extern "C" REX_FUNC(sub_82578C88) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578C88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578C88);
}

extern "C" REX_FUNC(sub_82578CB8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578CB8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578CB8);
}

extern "C" REX_FUNC(sub_82578CD0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578CD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578CD0);
}

extern "C" REX_FUNC(sub_82578D70) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578D70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578D70);
}

extern "C" REX_FUNC(sub_82578F00) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82578F00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82578F00);
}

extern "C" REX_FUNC(sub_825790F0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825790F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825790F0);
}

extern "C" REX_FUNC(sub_82579108) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579108(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579108);
}

extern "C" REX_FUNC(sub_825791B0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825791B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825791B0);
}

extern "C" REX_FUNC(sub_82579280) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579280(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579280);
}

extern "C" REX_FUNC(sub_82579318) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579318(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579318);
}

extern "C" REX_FUNC(sub_82579448) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579448(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579448);
}

extern "C" REX_FUNC(sub_82579590) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579590(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579590);
}

extern "C" REX_FUNC(sub_82579630) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579630(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579630);
}

extern "C" REX_FUNC(sub_825796C0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825796C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825796C0);
}

extern "C" REX_FUNC(sub_82579798) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579798(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579798);
}

extern "C" REX_FUNC(sub_825797B0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825797B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825797B0);
}

extern "C" REX_FUNC(sub_825797C0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_825797C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e825797C0);
}

extern "C" REX_FUNC(sub_82579830) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579830(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579830);
}

extern "C" REX_FUNC(sub_82579880) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579880(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579880);
}

extern "C" REX_FUNC(sub_82579898) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579898(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579898);
}

extern "C" REX_FUNC(sub_82579968) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579968(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579968);
}

extern "C" REX_FUNC(sub_82579A88) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579A88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579A88);
}

extern "C" REX_FUNC(sub_82579BE8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579BE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579BE8);
}

extern "C" REX_FUNC(sub_82579C48) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579C48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579C48);
}

extern "C" REX_FUNC(sub_82579CD0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579CD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579CD0);
}

extern "C" REX_FUNC(sub_82579CF0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579CF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579CF0);
}

extern "C" REX_FUNC(sub_82579D00) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579D00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579D00);
}

extern "C" REX_FUNC(sub_82579D40) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579D40(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579D40);
}

extern "C" REX_FUNC(sub_82579D60) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579D60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579D60);
}

extern "C" REX_FUNC(sub_82579D88) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579D88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579D88);
}

extern "C" REX_FUNC(sub_82579DA0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_82579DA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e82579DA0);
}

extern "C" REX_FUNC(sub_8257A3D0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257A3D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257A3D0);
}

extern "C" REX_FUNC(sub_8257A3F0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257A3F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257A3F0);
}

extern "C" REX_FUNC(sub_8257B9A8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257B9A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257B9A8);
}

extern "C" REX_FUNC(sub_8257B9D0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257B9D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257B9D0);
}

extern "C" REX_FUNC(sub_8257B9F8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257B9F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257B9F8);
}

extern "C" REX_FUNC(sub_8257BA88) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BA88(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BA88);
}

extern "C" REX_FUNC(sub_8257BAA0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BAA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BAA0);
}

extern "C" REX_FUNC(sub_8257BAD0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BAD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BAD0);
}

extern "C" REX_FUNC(sub_8257BB00) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BB00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BB00);
}

extern "C" REX_FUNC(sub_8257BB28) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BB28(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BB28);
}

extern "C" REX_FUNC(sub_8257BB50) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BB50(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BB50);
}

extern "C" REX_FUNC(sub_8257BB60) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BB60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BB60);
}

extern "C" REX_FUNC(sub_8257BBC8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BBC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BBC8);
}

extern "C" REX_FUNC(sub_8257BC48) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BC48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BC48);
}

extern "C" REX_FUNC(sub_8257BE48) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BE48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BE48);
}

extern "C" REX_FUNC(sub_8257BEC0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257BEC0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257BEC0);
}

extern "C" REX_FUNC(sub_8257C160) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257C160(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257C160);
}

extern "C" REX_FUNC(sub_8257C2B0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257C2B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257C2B0);
}

extern "C" REX_FUNC(sub_8257C3E0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257C3E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257C3E0);
}

extern "C" REX_FUNC(sub_8257C570) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257C570(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257C570);
}

extern "C" REX_FUNC(sub_8257C590) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257C590(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257C590);
}

extern "C" REX_FUNC(sub_8257C610) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257C610(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257C610);
}

extern "C" REX_FUNC(sub_8257C6B0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257C6B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257C6B0);
}

extern "C" REX_FUNC(sub_8257C710) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257C710(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257C710);
}

extern "C" REX_FUNC(sub_8257C948) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257C948(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257C948);
}

extern "C" REX_FUNC(sub_8257C9A0) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257C9A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257C9A0);
}

extern "C" REX_FUNC(sub_8257CA10) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257CA10(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257CA10);
}

extern "C" REX_FUNC(sub_8257CB38) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257CB38(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257CB38);
}

extern "C" REX_FUNC(sub_8257CC08) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257CC08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257CC08);
}

extern "C" REX_FUNC(sub_8257CCD8) {
  if (!ITC_FLAG(native_lifted_018)) {
    __imp__sub_8257CCD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e8257CCD8);
}
