// Lift sweep batch 009 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_009, true, "ITC",
              "Run lift sweep batch 009's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_821A0E50: lifted from generated/ by lift.py.
lifted::Result f821A0E50(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821A0E50 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r24,r3
  r24 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821A0E78, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r24)
  r11 = REX_LOAD_U32(uint32_t(r24) + 0);
  // lwz r26,0(r3)
  r26 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821a0ff0
  if (!cr6.eq) goto loc_821A0FF0;
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31676(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31676);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC20(base, LIFTED_ARGS(0x821A0E94, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31676(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31676);
  // mr r10,r3
  r10 = r3;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r3,-31680(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + -31680);
  // addis r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 65536));
  // addi r11,r11,-14799
  r11 = uint64_t(int64_t(int64_t(r11) + -14799));
  // rlwinm r28,r11,2,0,29
  r28 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // lwzx r11,r28,r11
  r11 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r11));
  // lwz r27,0(r11)
  r27 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC20(base, LIFTED_ARGS(0x821A0ECC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r26
  r3 = r26;
  // addi r4,r11,61
  r4 = uint64_t(int64_t(int64_t(r11) + 61));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821A0EDC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r27
  r3 = r27;
  // bl 0x821a0d38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A0D38, LIFTED_ARGS(0x821A0EE4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ori r9,r11,9792
  r9 = r11 | 9792;
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r8,r11,9796
  r8 = r11 | 9796;
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // stwx r10,r11,r9
  REX_STORE_U32(uint32_t(r11) + uint32_t(r9), uint32_t(r10));
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // stwx r10,r11,r8
  REX_STORE_U32(uint32_t(r11) + uint32_t(r8), uint32_t(r10));
  // bl 0x8219bfe8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219BFE8, LIFTED_ARGS(0x821A0F0C, r31)); r3 = o.r3; f1 = o.f1; }
  // bl 0x8219d3e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D3E8, LIFTED_ARGS(0x821A0F10, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // lwzx r3,r28,r11
  r3 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r11));
  // bl 0x8219cf08
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219CF08, LIFTED_ARGS(0x821A0F1C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31680(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + -31680);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC20(base, LIFTED_ARGS(0x821A0F24, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // lwz r11,-31680(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31680);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r27,r28,61
  r27 = uint64_t(int64_t(int64_t(r28) + 61));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r25,4(r11)
  r25 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mr r3,r26
  r3 = r26;
  // mr r4,r27
  r4 = r27;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A0F50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r27
  r4 = r27;
  // mr r3,r26
  r3 = r26;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821A0F5C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31676(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31676);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC20(base, LIFTED_ARGS(0x821A0F64, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // add. r10,r25,r28
  r10 = r25 + r28;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ori r9,r11,9784
  r9 = r11 | 9784;
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r8,r11,9788
  r8 = r11 | 9788;
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // stwx r3,r11,r9
  REX_STORE_U32(uint32_t(r11) + uint32_t(r9), uint32_t(r3));
  // lwz r11,-31676(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31676);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // stwx r9,r11,r8
  REX_STORE_U32(uint32_t(r11) + uint32_t(r8), uint32_t(r9));
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // addis r11,r11,3
  r11 = uint64_t(int64_t(int64_t(r11) + 196608));
  // addi r11,r11,6568
  r11 = uint64_t(int64_t(int64_t(r11) + 6568));
  // lha r9,1604(r11)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 1604))));
  // b 0x821a0fac
  goto loc_821A0FAC;
loc_821A0FA4:
  // lha r9,0(r9)
  r9 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
loc_821A0FAC:
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // add r9,r9,r11
  r9 = r9 + r11;
  // bne 0x821a0fa4
  if (!cr0.eq) goto loc_821A0FA4;
  // lha r10,2(r9)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r9) + 2))));
  // addi r10,r10,403
  r10 = uint64_t(int64_t(int64_t(r10) + 403));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwzx r3,r10,r11
  r3 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r11));
  // bl 0x8219d440
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D440, LIFTED_ARGS(0x821A0FCC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r10,r11,8176
  r10 = r11 | 8176;
  // lwz r11,-31680(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31680);
  // lwz r3,4(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r11,-31672(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31672);
  // lhax r4,r11,r10
  r4 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + uint32_t(r10)))));
  // bl 0x8219d4b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8219D4B8, LIFTED_ARGS(0x821A0FE8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r24)
  REX_STORE_U32(uint32_t(r24) + 0, uint32_t(r11));
loc_821A0FF0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821A0FFC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821A1258: lifted from generated/ by lift.py.
lifted::Result f821A1258(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821A1258 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821A1280, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821a1324
  if (!cr6.eq) goto loc_821A1324;
  // li r4,96
  r4 = uint64_t(int64_t(96));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821A12A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,97
  r4 = uint64_t(int64_t(97));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821A12AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,108
  r4 = uint64_t(int64_t(108));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821A12B8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,93
  r4 = uint64_t(int64_t(93));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821A12C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,105
  r4 = uint64_t(int64_t(105));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A12E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31680(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + -31680);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC20(base, LIFTED_ARGS(0x821A12EC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r10,r11,9796
  r10 = r11 | 9796;
  // lwz r11,-31680(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31680);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r8,r11,9792
  r8 = r11 | 9792;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31672(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31672);
  // lwzx r10,r11,r10
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // subf r10,r10,r9
  r10 = r9 - r10;
  // add r10,r10,r3
  r10 = r10 + r3;
  // stwx r10,r11,r8
  REX_STORE_U32(uint32_t(r11) + uint32_t(r8), uint32_t(r10));
  // bl 0x821a1008
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821A1008(base, LIFTED_ARGS(0x821A1320, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r28));
loc_821A1324:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821A1330, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821a1374
  if (!cr0.eq) goto loc_821A1374;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31684(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31684);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821a1350
  if (cr6.eq) goto loc_821A1350;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A1350:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31684(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31684);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
loc_821A1374:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821A1380: lifted from generated/ by lift.py.
lifted::Result f821A1380(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_821A1380 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e5c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -4096);
  // stwu r1,-4224(r1)
  ea = -4224 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,-23512
  r6 = uint64_t(int64_t(int64_t(r11) + -23512));
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82106540(base, LIFTED_ARGS(0x821A13A8, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821A13C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lis r10,-32230
  r10 = uint64_t(int64_t(-2112225280));
  // lis r28,-32230
  r28 = uint64_t(int64_t(-2112225280));
  // addi r10,r10,-136
  r10 = uint64_t(int64_t(int64_t(r10) + -136));
  // lis r17,-32230
  r17 = uint64_t(int64_t(-2112225280));
  // lwz r11,-31684(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31684);
  // lis r18,-32230
  r18 = uint64_t(int64_t(-2112225280));
  // lis r19,-32230
  r19 = uint64_t(int64_t(-2112225280));
  // lis r20,-32230
  r20 = uint64_t(int64_t(-2112225280));
  // lis r21,-32230
  r21 = uint64_t(int64_t(-2112225280));
  // stw r10,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r10));
  // lis r22,-32230
  r22 = uint64_t(int64_t(-2112225280));
  // lis r23,-32230
  r23 = uint64_t(int64_t(-2112225280));
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // lis r24,-32230
  r24 = uint64_t(int64_t(-2112225280));
  // lis r25,-32230
  r25 = uint64_t(int64_t(-2112225280));
  // lis r26,-32230
  r26 = uint64_t(int64_t(-2112225280));
  // lis r27,-32230
  r27 = uint64_t(int64_t(-2112225280));
  // lis r29,-32230
  r29 = uint64_t(int64_t(-2112225280));
  // lis r30,-32230
  r30 = uint64_t(int64_t(-2112225280));
  // lis r4,-32230
  r4 = uint64_t(int64_t(-2112225280));
  // lis r5,-32230
  r5 = uint64_t(int64_t(-2112225280));
  // lis r6,-32230
  r6 = uint64_t(int64_t(-2112225280));
  // lis r7,-32230
  r7 = uint64_t(int64_t(-2112225280));
  // lis r8,-32230
  r8 = uint64_t(int64_t(-2112225280));
  // lis r9,-32230
  r9 = uint64_t(int64_t(-2112225280));
  // addi r28,r28,-496
  r28 = uint64_t(int64_t(int64_t(r28) + -496));
  // addi r17,r17,2264
  r17 = uint64_t(int64_t(int64_t(r17) + 2264));
  // addi r18,r18,-15064
  r18 = uint64_t(int64_t(int64_t(r18) + -15064));
  // addi r19,r19,-14640
  r19 = uint64_t(int64_t(int64_t(r19) + -14640));
  // addi r20,r20,-1272
  r20 = uint64_t(int64_t(int64_t(r20) + -1272));
  // addi r21,r21,-928
  r21 = uint64_t(int64_t(int64_t(r21) + -928));
  // stw r28,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r28));
  // addi r22,r22,-14256
  r22 = uint64_t(int64_t(int64_t(r22) + -14256));
  // stw r17,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r17));
  // addi r23,r23,4408
  r23 = uint64_t(int64_t(int64_t(r23) + 4408));
  // stw r18,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r18));
  // addi r24,r24,-14088
  r24 = uint64_t(int64_t(int64_t(r24) + -14088));
  // stw r19,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r19));
  // addi r25,r25,-3168
  r25 = uint64_t(int64_t(int64_t(r25) + -3168));
  // stw r20,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r20));
  // addi r26,r26,3664
  r26 = uint64_t(int64_t(int64_t(r26) + 3664));
  // stw r21,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r21));
  // addi r27,r27,2616
  r27 = uint64_t(int64_t(int64_t(r27) + 2616));
  // stw r22,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r22));
  // addi r29,r29,-13816
  r29 = uint64_t(int64_t(int64_t(r29) + -13816));
  // stw r23,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r23));
  // addi r30,r30,-13544
  r30 = uint64_t(int64_t(int64_t(r30) + -13544));
  // stw r24,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r24));
  // addi r4,r4,-2784
  r4 = uint64_t(int64_t(int64_t(r4) + -2784));
  // stw r25,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r25));
  // addi r5,r5,2872
  r5 = uint64_t(int64_t(int64_t(r5) + 2872));
  // stw r26,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r26));
  // addi r6,r6,4696
  r6 = uint64_t(int64_t(int64_t(r6) + 4696));
  // stw r27,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r27));
  // addi r7,r7,-312
  r7 = uint64_t(int64_t(int64_t(r7) + -312));
  // stw r29,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r29));
  // addi r8,r8,-13352
  r8 = uint64_t(int64_t(int64_t(r8) + -13352));
  // stw r30,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r30));
  // addi r9,r9,-2416
  r9 = uint64_t(int64_t(int64_t(r9) + -2416));
  // stw r4,152(r1)
  REX_STORE_U32(uint32_t(r1) + 152, uint32_t(r4));
  // stw r5,156(r1)
  REX_STORE_U32(uint32_t(r1) + 156, uint32_t(r5));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // stw r6,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r6));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // stw r7,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r7));
  // stw r8,168(r1)
  REX_STORE_U32(uint32_t(r1) + 168, uint32_t(r8));
  // stw r9,172(r1)
  REX_STORE_U32(uint32_t(r1) + 172, uint32_t(r9));
  // beq cr6,0x821a1558
  if (cr6.eq) goto loc_821A1558;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x821a1538
  if (cr6.lt) goto loc_821A1538;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,96
  r9 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwzx r11,r10,r9
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r9));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x821A14F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31684(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31684);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821a156c
  if (cr0.eq) goto loc_821A156C;
  // lwz r10,16(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x821a151c
  if (!cr6.eq) goto loc_821A151C;
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821a152c
  goto loc_821A152C;
loc_821A151C:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
loc_821A1520:
  // lwz r10,16(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 16);
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r28,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r28));
loc_821A152C:
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821A1530:
  // stw r28,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r28));
  // b 0x821a156c
  goto loc_821A156C;
loc_821A1538:
  // lwz r10,16(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 16);
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x821a1520
  if (!cr6.eq) goto loc_821A1520;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821a1530
  goto loc_821A1530;
loc_821A1558:
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821a156c
  if (cr6.eq) goto loc_821A156C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821A156C, r31)); r3 = o.r3; f1 = o.f1; }
loc_821A156C:
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r10,r11,9820
  r10 = r11 | 9820;
  // lwz r11,-31672(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31672);
  // lwzx r10,r11,r10
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821a1698
  if (cr6.eq) goto loc_821A1698;
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31776(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82179be0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82179BE0(base, LIFTED_ARGS(0x821A1598, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // ori r31,r11,9832
  r31 = r11 | 9832;
  // lwz r11,-31672(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31672);
  // lwzx r11,r11,r31
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r31));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821a15cc
  if (cr0.eq) goto loc_821A15CC;
  // bne cr6,0x821a15dc
  if (!cr6.eq) goto loc_821A15DC;
  // bl 0x8219ed10
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8219ED10(base, LIFTED_ARGS(0x821A15BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31672(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31672);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stwx r10,r11,r31
  REX_STORE_U32(uint32_t(r11) + uint32_t(r31), uint32_t(r10));
  // b 0x821a15dc
  goto loc_821A15DC;
loc_821A15CC:
  // beq cr6,0x821a15dc
  if (cr6.eq) goto loc_821A15DC;
  // bl 0x8219edb0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8219EDB0(base, LIFTED_ARGS(0x821A15D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31672(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31672);
  // stwx r28,r11,r31
  REX_STORE_U32(uint32_t(r11) + uint32_t(r31), uint32_t(r28));
loc_821A15DC:
  // lis r11,3
  r11 = uint64_t(int64_t(196608));
  // ori r31,r11,9824
  r31 = r11 | 9824;
  // lwz r11,-31672(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31672);
  // lwzx r11,r11,r31
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r31));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lwz r11,-31776(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bne cr6,0x821a1688
  if (!cr6.eq) goto loc_821A1688;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // bl 0x8217f880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8217F880(base, LIFTED_ARGS(0x821A1604, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31672(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31672);
  // stwx r3,r11,r31
  REX_STORE_U32(uint32_t(r11) + uint32_t(r31), uint32_t(r3));
  // lwz r11,-31672(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31672);
  // lwzx r10,r11,r31
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r31));
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821a16b8
  if (cr6.eq) goto loc_821A16B8;
  // lis r10,3
  r10 = uint64_t(int64_t(196608));
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // ori r10,r10,9828
  r10 = r10 | 9828;
  // lwzx r10,r11,r10
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // mulli r10,r10,52
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(52))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addis r31,r11,3
  r31 = uint64_t(int64_t(int64_t(r11) + 196608));
  // addi r31,r31,-14460
  r31 = uint64_t(int64_t(int64_t(r31) + -14460));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820CD5D8(base, LIFTED_ARGS(0x821A1640, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // lwz r4,48(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 48);
  // bl 0x820cdce0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820CDCE0(base, LIFTED_ARGS(0x821A164C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4080(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 4080);
  // addi r3,r1,192
  r3 = uint64_t(int64_t(int64_t(r1) + 192));
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,4080(r1)
  REX_STORE_U32(uint32_t(r1) + 4080, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e820C4998(base, LIFTED_ARGS(0x821A1660, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31776(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31776);
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // addi r4,r1,192
  r4 = uint64_t(int64_t(int64_t(r1) + 192));
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x8217fd58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8217FD58(base, LIFTED_ARGS(0x821A1674, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31776(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31776);
  // li r4,54
  r4 = uint64_t(int64_t(54));
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82180108(base, LIFTED_ARGS(0x821A1684, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821a16b8
  goto loc_821A16B8;
loc_821A1688:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8217fe60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8217FE60(base, LIFTED_ARGS(0x821A1694, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821a16b8
  goto loc_821A16B8;
loc_821A1698:
  // lis r10,3
  r10 = uint64_t(int64_t(196608));
  // ori r31,r10,9832
  r31 = r10 | 9832;
  // lwzx r11,r11,r31
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r31));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821a16b8
  if (cr6.eq) goto loc_821A16B8;
  // bl 0x8219ee30
  // (left out: dead in C++)
  { const lifted::Result o = f8219EE30(base, r1, r13, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31672(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31672);
  // stwx r28,r11,r31
  REX_STORE_U32(uint32_t(r11) + uint32_t(r31), uint32_t(r28));
loc_821A16B8:
  // addi r1,r1,4224
  r1 = uint64_t(int64_t(int64_t(r1) + 4224));
  // b 0x825a6eac
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821A1E18: lifted from generated/ by lift.py.
lifted::Result f821A1E18(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821A1E18 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r30,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r30));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r29,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r29));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // beq cr6,0x821a1e60
  if (cr6.eq) goto loc_821A1E60;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A1E60:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // li r8,12
  r8 = uint64_t(int64_t(12));
  // li r6,158
  r6 = uint64_t(int64_t(158));
  // li r5,159
  r5 = uint64_t(int64_t(159));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // li r7,154
  r7 = uint64_t(int64_t(154));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // li r10,156
  r10 = uint64_t(int64_t(156));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // li r8,153
  r8 = uint64_t(int64_t(153));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r9,155
  r9 = uint64_t(int64_t(155));
  // stw r6,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r6));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r5,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r5));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stw r30,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r30));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31664(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31664);
  // bl 0x8220bac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BAC0(base, LIFTED_ARGS(0x821A1EBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r30,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r30));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r29,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r29));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821A1ED8: lifted from generated/ by lift.py.
lifted::Result f821A1ED8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821A1ED8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r8,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r8));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x821a1f2c
  if (cr6.eq) goto loc_821A1F2C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A1F2C:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // li r7,13
  r7 = uint64_t(int64_t(13));
  // li r6,158
  r6 = uint64_t(int64_t(158));
  // li r5,159
  r5 = uint64_t(int64_t(159));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // li r10,156
  r10 = uint64_t(int64_t(156));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // li r7,154
  r7 = uint64_t(int64_t(154));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // li r9,155
  r9 = uint64_t(int64_t(155));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r6,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r6));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r5,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r5));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,153
  r8 = uint64_t(int64_t(153));
  // lwz r3,-31664(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31664);
  // bl 0x8220bac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BAC0(base, LIFTED_ARGS(0x821A1F88, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r30,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r30));
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

// sub_821A1FA8: lifted from generated/ by lift.py.
lifted::Result f821A1FA8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821A1FA8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,12
  r7 = uint64_t(int64_t(12));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r8,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r8));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r7,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r7));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r8,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r8));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // beq cr6,0x821a1ff4
  if (cr6.eq) goto loc_821A1FF4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A1FF4:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // li r6,158
  r6 = uint64_t(int64_t(158));
  // li r5,159
  r5 = uint64_t(int64_t(159));
  // li r7,154
  r7 = uint64_t(int64_t(154));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // li r10,156
  r10 = uint64_t(int64_t(156));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // li r9,155
  r9 = uint64_t(int64_t(155));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r6,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r6));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r5,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r5));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,153
  r8 = uint64_t(int64_t(153));
  // lwz r3,-31664(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31664);
  // bl 0x8220bac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BAC0(base, LIFTED_ARGS(0x821A204C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r10));
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

// sub_821A2070: lifted from generated/ by lift.py.
lifted::Result f821A2070(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821A2070 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r8,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r8));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r9,22
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 22, so);
  // beq cr6,0x821a20bc
  if (cr6.eq) goto loc_821A20BC;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821A20BC:
  // li r9,22
  r9 = uint64_t(int64_t(22));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // li r6,158
  r6 = uint64_t(int64_t(158));
  // li r5,159
  r5 = uint64_t(int64_t(159));
  // li r10,156
  r10 = uint64_t(int64_t(156));
  // li r7,154
  r7 = uint64_t(int64_t(154));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r9,155
  r9 = uint64_t(int64_t(155));
  // stw r6,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r6));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r5,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r5));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r8,44(r11)
  REX_STORE_U32(uint32_t(r11) + 44, uint32_t(r8));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,153
  r8 = uint64_t(int64_t(153));
  // lwz r3,-31664(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31664);
  // bl 0x8220bac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BAC0(base, LIFTED_ARGS(0x821A2114, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r10));
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

// sub_821A2208: lifted from generated/ by lift.py.
lifted::Result f821A2208(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821A2208 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,31072(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 31072);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821a2320
  if (!cr6.eq) goto loc_821A2320;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,31076(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 31076);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821a2320
  if (!cr6.eq) goto loc_821A2320;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228f9e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228F9E8(base, LIFTED_ARGS(0x821A2248, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821a22a0
  if (!cr0.eq) goto loc_821A22A0;
  // lwz r10,44(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 44);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a2294
  if (cr6.lt) goto loc_821A2294;
  // bne cr6,0x821a2320
  if (!cr6.eq) goto loc_821A2320;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,21
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 21, so);
  // beq cr6,0x821a227c
  if (cr6.eq) goto loc_821A227C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A227C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,21
  r9 = uint64_t(int64_t(21));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // b 0x821a2320
  goto loc_821A2320;
loc_821A2294:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,44(r11)
  REX_STORE_U32(uint32_t(r11) + 44, uint32_t(r10));
  // b 0x821a2320
  goto loc_821A2320;
loc_821A22A0:
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a22c8
  if (cr6.lt) goto loc_821A22C8;
  // bne cr6,0x821a22d4
  if (!cr6.eq) goto loc_821A22D4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31664(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31664);
  // stw r10,60(r11)
  REX_STORE_U32(uint32_t(r11) + 60, uint32_t(r10));
  // b 0x821a22d4
  goto loc_821A22D4;
loc_821A22C8:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31664(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31664);
  // stw r9,60(r11)
  REX_STORE_U32(uint32_t(r11) + 60, uint32_t(r9));
loc_821A22D4:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a22fc
  if (cr6.eq) goto loc_821A22FC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A22FC:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r7,18
  r7 = uint64_t(int64_t(18));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r9,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r9));
loc_821A2320:
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

// sub_821A2338: lifted from generated/ by lift.py.
void f821A2338(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821A2338 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // blr
  return;
}

// sub_821A2350: lifted from generated/ by lift.py.
lifted::Result f821A2350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821A2350 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r10,44(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 44);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a23b4
  if (cr6.lt) goto loc_821A23B4;
  // bne cr6,0x821a2498
  if (!cr6.eq) goto loc_821A2498;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,21
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 21, so);
  // beq cr6,0x821a2394
  if (cr6.eq) goto loc_821A2394;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A2394:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,21
  r8 = uint64_t(int64_t(21));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821a2498
  goto loc_821A2498;
loc_821A23B4:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r30,44(r11)
  REX_STORE_U32(uint32_t(r11) + 44, uint32_t(r30));
  // bl 0x8228f9e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228F9E8(base, LIFTED_ARGS(0x821A23C4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821a2404
  if (!cr0.eq) goto loc_821A2404;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,21
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 21, so);
  // beq cr6,0x821a23e4
  if (cr6.eq) goto loc_821A23E4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A23E4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,21
  r8 = uint64_t(int64_t(21));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821a248c
  goto loc_821A248C;
loc_821A2404:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821a2448
  if (cr6.eq) goto loc_821A2448;
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a242c
  if (cr6.eq) goto loc_821A242C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A242C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a2468
  goto loc_821A2468;
loc_821A2448:
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a2454
  if (cr6.eq) goto loc_821A2454;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A2454:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821A2468:
  // li r8,18
  r8 = uint64_t(int64_t(18));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r30,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r30));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r30,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r30));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
loc_821A248C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31664(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31664);
  // stw r30,60(r11)
  REX_STORE_U32(uint32_t(r11) + 60, uint32_t(r30));
loc_821A2498:
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

// sub_821A2FB0: lifted from generated/ by lift.py.
lifted::Result f821A2FB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821A2FB0 (lifted)");
  uint64_t r0 = 0, r3 = 0, r4 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // bl 0x8220c1e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220C1E0(base, LIFTED_ARGS(0x821A2FC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // bl 0x8220bd50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BD50(base, LIFTED_ARGS(0x821A2FD0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r28,5
  r28 = uint64_t(int64_t(5));
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // beq 0x821a3274
  if (cr0.eq) goto loc_821A3274;
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC20(base, LIFTED_ARGS(0x821A2FF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821A2FFC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31660(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // lwz r10,12(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 12);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmplwi cr6,r10,15
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 15, so);
  // bgt cr6,0x821a3274
  if (cr6.gt) goto loc_821A3274;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-23408
  r12 = uint64_t(int64_t(int64_t(r12) + -23408));
  // lbzx r0,r12,r10
  r0 = REX_LOAD_U8(uint32_t(r12) + uint32_t(r10));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32230
  r12 = uint64_t(int64_t(-2112225280));
  // addi r12,r12,12348
  r12 = uint64_t(int64_t(int64_t(r12) + 12348));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r10)) {
  case 0:
  goto loc_frag_821A303C;
  return {r3, f1};
  case 1:
  goto loc_821A3050;
  case 2:
  goto loc_821A3074;
  case 3:
  goto loc_821A3274;
  case 4:
  goto loc_821A3274;
  case 5:
  goto loc_821A3274;
  case 6:
  goto loc_821A3274;
  case 7:
  goto loc_821A30B0;
  case 8:
  goto loc_821A3274;
  case 9:
  goto loc_821A3274;
  case 10:
  goto loc_821A3274;
  case 11:
  goto loc_821A3130;
  case 12:
  goto loc_821A317C;
  case 13:
  goto loc_821A31A0;
  case 14:
  goto loc_821A31C4;
  case 15:
  goto loc_821A31E8;
  default:
  std::abort();
  }
loc_821A3050:
  // lwz r10,-31668(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // beq cr6,0x821a31f8
  if (cr6.eq) goto loc_821A31F8;
  // cmpwi cr6,r9,5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 5, so);
  // beq cr6,0x821a306c
  if (cr6.eq) goto loc_821A306C;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821A306C:
  // li r7,12
  r7 = uint64_t(int64_t(12));
  // b 0x821a3248
  goto loc_821A3248;
loc_821A3074:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne cr6,0x821a3100
  if (!cr6.eq) goto loc_821A3100;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // bl 0x822952c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822952C0(base, LIFTED_ARGS(0x821A3088, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31668(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x821a309c
  if (cr6.eq) goto loc_821A309C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A309C:
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // stw r30,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r30));
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821a3274
  goto loc_821A3274;
loc_821A30B0:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne cr6,0x821a3100
  if (!cr6.eq) goto loc_821A3100;
  // lwz r10,-31668(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // beq cr6,0x821a30cc
  if (cr6.eq) goto loc_821A30CC;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821A30CC:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r30,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r30));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // li r7,7
  r7 = uint64_t(int64_t(7));
  // li r6,9
  r6 = uint64_t(int64_t(9));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r8,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r8));
  // stw r7,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r7));
  // lwz r10,-31660(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r6,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r6));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // b 0x821a3274
  goto loc_821A3274;
loc_821A3100:
  // lwz r11,-31668(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x821a3114
  if (cr6.eq) goto loc_821A3114;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A3114:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // b 0x821a3274
  goto loc_821A3274;
loc_821A3130:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq cr6,0x821a3100
  if (cr6.eq) goto loc_821A3100;
  // lwz r10,-31668(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r9,5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 5, so);
  // beq cr6,0x821a314c
  if (cr6.eq) goto loc_821A314C;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821A314C:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // stw r28,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r28));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // stw r30,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r30));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r8,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r8));
  // lwz r10,-31660(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r7,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r7));
  // b 0x821a3274
  goto loc_821A3274;
loc_821A317C:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq cr6,0x821a3100
  if (cr6.eq) goto loc_821A3100;
  // lwz r10,-31668(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r9,5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 5, so);
  // beq cr6,0x821a3198
  if (cr6.eq) goto loc_821A3198;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821A3198:
  // li r7,15
  r7 = uint64_t(int64_t(15));
  // b 0x821a3248
  goto loc_821A3248;
loc_821A31A0:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // beq cr6,0x821a3100
  if (cr6.eq) goto loc_821A3100;
  // lwz r10,-31668(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r9,5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 5, so);
  // beq cr6,0x821a31bc
  if (cr6.eq) goto loc_821A31BC;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821A31BC:
  // li r7,16
  r7 = uint64_t(int64_t(16));
  // b 0x821a3248
  goto loc_821A3248;
loc_821A31C4:
  // lwz r10,-31668(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // beq cr6,0x821a31f8
  if (cr6.eq) goto loc_821A31F8;
  // cmpwi cr6,r9,5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 5, so);
  // beq cr6,0x821a31e0
  if (cr6.eq) goto loc_821A31E0;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821A31E0:
  // li r7,13
  r7 = uint64_t(int64_t(13));
  // b 0x821a3248
  goto loc_821A3248;
loc_821A31E8:
  // lwz r10,-31668(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // bne cr6,0x821a3238
  if (!cr6.eq) goto loc_821A3238;
loc_821A31F8:
  // cmpwi cr6,r9,6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 6, so);
  // beq cr6,0x821a3204
  if (cr6.eq) goto loc_821A3204;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821A3204:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // li r7,6
  r7 = uint64_t(int64_t(6));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r8,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r8));
  // stw r7,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r7));
  // stw r30,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r30));
  // lwz r10,-31660(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r6,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r6));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r26,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r26));
  // b 0x821a3274
  goto loc_821A3274;
loc_821A3238:
  // cmpwi cr6,r9,5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 5, so);
  // beq cr6,0x821a3244
  if (cr6.eq) goto loc_821A3244;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821A3244:
  // li r7,14
  r7 = uint64_t(int64_t(14));
loc_821A3248:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r8,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r8));
  // stw r28,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r28));
  // stw r30,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r30));
  // lwz r10,-31660(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r7,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r7));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r6,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r6));
loc_821A3274:
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // bl 0x8220bde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BDE0(base, LIFTED_ARGS(0x821A327C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31664(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821a32b8
  if (cr6.eq) goto loc_821A32B8;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821A3294, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31668(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x821a32a8
  if (cr6.eq) goto loc_821A32A8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A32A8:
  // stw r26,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r26));
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
loc_821A32B8:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
loc_frag_821A303C:
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // bne cr6,0x821a3100
  if (!cr6.eq) goto loc_821A3100_821A303C;
  // lwz r10,-31668(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // b 0x821a31f8
  goto loc_821A31F8_821A303C;
loc_821A3100_821A303C:
  // lwz r11,-31668(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x821a3114
  if (cr6.eq) goto loc_821A3114_821A303C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A3114_821A303C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // b 0x821a3274
  goto loc_821A3274_821A303C;
loc_821A31F8_821A303C:
  // cmpwi cr6,r9,6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 6, so);
  // beq cr6,0x821a3204
  if (cr6.eq) goto loc_821A3204_821A303C;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821A3204_821A303C:
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // li r7,6
  r7 = uint64_t(int64_t(6));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r8,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r8));
  // stw r7,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r7));
  // stw r30,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r30));
  // lwz r10,-31660(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r6,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r6));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r26,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r26));
  // b 0x821a3274
  goto loc_821A3274_821A303C;
loc_821A3274_821A303C:
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // bl 0x8220bde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BDE0(base, LIFTED_ARGS(0x821A327C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31664(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821a32b8
  if (cr6.eq) goto loc_821A32B8_821A303C;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821A3294, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31668(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x821a32a8
  if (cr6.eq) goto loc_821A32A8_821A303C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A32A8_821A303C:
  // stw r26,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r26));
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // stw r28,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r28));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
loc_821A32B8_821A303C:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821A3868: lifted from generated/ by lift.py.
lifted::Result f821A3868(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821A3868 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,31072(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 31072);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821a3940
  if (!cr6.eq) goto loc_821A3940;
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r11,31076(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 31076);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821a3940
  if (!cr6.eq) goto loc_821A3940;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8228f9e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228F9E8(base, LIFTED_ARGS(0x821A389C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bne 0x821a38f8
  if (!cr0.eq) goto loc_821A38F8;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x821a38bc
  if (cr6.eq) goto loc_821A38BC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A38BC:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31660(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r6,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r6));
  // b 0x821a3940
  goto loc_821A3940;
loc_821A38F8:
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x821a3904
  if (cr6.eq) goto loc_821A3904;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A3904:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31660(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r6,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r6));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r5,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r5));
loc_821A3940:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821A3950: lifted from generated/ by lift.py.
lifted::Result f821A3950(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_821A3950 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
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
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8228fd50
  // (left out: dead in C++)
  { const lifted::Result o = f8228FD50(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,3
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 3, so);
  // beq cr6,0x821a399c
  if (cr6.eq) goto loc_821A399C;
  // cmpwi cr6,r3,7
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 7, so);
  // beq cr6,0x821a39a4
  if (cr6.eq) goto loc_821A39A4;
  // cmpwi cr6,r3,8
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 8, so);
  // bne cr6,0x821a39f8
  if (!cr6.eq) goto loc_821A39F8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x821a3994
  if (cr6.eq) goto loc_821A3994;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A3994:
  // li r6,7
  r6 = uint64_t(int64_t(7));
  // b 0x821a39c0
  goto loc_821A39C0;
loc_821A399C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8228fa58
  // (left out: dead in C++)
  { const lifted::Result o = f8228FA58(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
loc_821A39A4:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x821a39bc
  if (cr6.eq) goto loc_821A39BC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A39BC:
  // li r6,5
  r6 = uint64_t(int64_t(5));
loc_821A39C0:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31660(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r6,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r6));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r5,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r5));
loc_821A39F8:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821A3ED8: lifted from generated/ by lift.py.
lifted::Result f821A3ED8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821A3ED8 (lifted)");
  uint64_t r3 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31664(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -31664);
  // bl 0x8220c1e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220C1E0(base, LIFTED_ARGS(0x821A3EF0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31664(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -31664);
  // bl 0x8220bd50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BD50(base, LIFTED_ARGS(0x821A3EF8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r29,-1
  r29 = uint64_t(int64_t(-1));
  // li r26,1
  r26 = uint64_t(int64_t(1));
  // li r27,17
  r27 = uint64_t(int64_t(17));
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // beq 0x821a41dc
  if (cr0.eq) goto loc_821A41DC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821A3F18, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31664(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -31664);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC20(base, LIFTED_ARGS(0x821A3F20, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31660(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,12(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 12);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821a41a0
  if (cr6.eq) goto loc_821A41A0;
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821a40fc
  if (cr6.eq) goto loc_821A40FC;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x821a409c
  if (cr6.eq) goto loc_821A409C;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x821a4064
  if (cr6.eq) goto loc_821A4064;
  // cmpwi cr6,r11,8
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 8, so);
  // beq cr6,0x821a3fe0
  if (cr6.eq) goto loc_821A3FE0;
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // bne cr6,0x821a41dc
  if (!cr6.eq) goto loc_821A41DC;
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x821a41a8
  if (cr6.eq) goto loc_821A41A8;
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
loc_821A3F68:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lwz r11,-31668(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821a3f9c
  if (cr6.eq) goto loc_821A3F9C;
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a3f84
  if (cr6.eq) goto loc_821A3F84;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A3F84:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,18
  r8 = uint64_t(int64_t(18));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // b 0x821a3fb8
  goto loc_821A3FB8;
loc_821A3F9C:
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a3fa8
  if (cr6.eq) goto loc_821A3FA8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A3FA8:
  // li r9,18
  r9 = uint64_t(int64_t(18));
  // stw r29,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r29));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
loc_821A3FB8:
  // stw r29,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r26,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r26));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r26,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r26));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // b 0x821a41dc
  goto loc_821A41DC;
loc_821A3FE0:
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x821a41a8
  if (cr6.eq) goto loc_821A41A8;
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lwz r11,-31668(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821a401c
  if (cr6.eq) goto loc_821A401C;
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x821a400c
  if (cr6.eq) goto loc_821A400C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A400C:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // b 0x821a4034
  goto loc_821A4034;
loc_821A401C:
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x821a4028
  if (cr6.eq) goto loc_821A4028;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4028:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r29,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r29));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821A4034:
  // li r10,3
  r10 = uint64_t(int64_t(3));
loc_821A4038:
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // stw r29,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r8,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r8));
  // b 0x821a41dc
  goto loc_821A41DC;
loc_821A4064:
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x821a41a8
  if (cr6.eq) goto loc_821A41A8;
loc_821A406C:
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lwz r11,-31668(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821a4164
  if (cr6.eq) goto loc_821A4164;
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a408c
  if (cr6.eq) goto loc_821A408C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A408C:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // b 0x821a4178
  goto loc_821A4178;
loc_821A409C:
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq cr6,0x821a3f68
  if (cr6.eq) goto loc_821A3F68;
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lwz r11,-31668(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821a40d4
  if (cr6.eq) goto loc_821A40D4;
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a40c4
  if (cr6.eq) goto loc_821A40C4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A40C4:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // b 0x821a40e8
  goto loc_821A40E8;
loc_821A40D4:
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a40e0
  if (cr6.eq) goto loc_821A40E0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A40E0:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r29,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r29));
loc_821A40E8:
  // stw r29,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r27,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r27));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // b 0x821a4188
  goto loc_821A4188;
loc_821A40FC:
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x821a406c
  if (!cr6.eq) goto loc_821A406C;
  // lwz r3,44(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + 44);
  // bl 0x8228fbc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228FBC0(base, LIFTED_ARGS(0x821A410C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lwz r11,-31668(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821a4144
  if (cr6.eq) goto loc_821A4144;
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x821a4134
  if (cr6.eq) goto loc_821A4134;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4134:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // b 0x821a415c
  goto loc_821A415C;
loc_821A4144:
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x821a4150
  if (cr6.eq) goto loc_821A4150;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4150:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r29,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r29));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
loc_821A415C:
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // b 0x821a4038
  goto loc_821A4038;
loc_821A4164:
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a4170
  if (cr6.eq) goto loc_821A4170;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4170:
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r29,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r29));
loc_821A4178:
  // stw r29,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r29));
  // stw r27,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r27));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // li r10,8
  r10 = uint64_t(int64_t(8));
loc_821A4188:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // b 0x821a41dc
  goto loc_821A41DC;
loc_821A41A0:
  // cmpwi cr6,r3,0
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne cr6,0x821a41b0
  if (!cr6.eq) goto loc_821A41B0;
loc_821A41A8:
  // bl 0x821a2350
  // (left out: dead in C++)
  { const lifted::Result o = f821A2350(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821a41dc
  goto loc_821A41DC;
loc_821A41B0:
  // lwz r3,44(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + 44);
  // bl 0x8228f9b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228F9B0(base, LIFTED_ARGS(0x821A41B8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31668(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,20
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 20, so);
  // beq cr6,0x821a41cc
  if (cr6.eq) goto loc_821A41CC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A41CC:
  // li r10,20
  r10 = uint64_t(int64_t(20));
  // stw r29,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r29));
  // stw r29,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r29));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821A41DC:
  // lwz r3,-31664(r28)
  r3 = REX_LOAD_U32(uint32_t(r28) + -31664);
  // bl 0x8220bde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BDE0(base, LIFTED_ARGS(0x821A41E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31664(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31664);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821a4220
  if (cr6.eq) goto loc_821A4220;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821A41FC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31668(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a4210
  if (cr6.eq) goto loc_821A4210;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4210:
  // stw r26,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r26));
  // stw r29,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r29));
  // stw r27,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r27));
  // stw r29,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r29));
loc_821A4220:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821A4228: lifted from generated/ by lift.py.
lifted::Result f821A4228(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821A4228 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r10,24(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 24);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r10,24(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 24);
  // cmpwi cr6,r10,45
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 45, so);
  // blt cr6,0x821a4290
  if (cr6.lt) goto loc_821A4290;
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821a4290
  if (!cr6.eq) goto loc_821A4290;
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
loc_821A426C:
  // bl 0x8228f9b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228F9B0(base, LIFTED_ARGS(0x821A4270, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31668(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,20
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 20, so);
  // beq cr6,0x821a4288
  if (cr6.eq) goto loc_821A4288;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821A4288:
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // b 0x821a442c
  goto loc_821A442C;
loc_821A4290:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31664(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + -31664);
  // bl 0x8220bd50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BD50(base, LIFTED_ARGS(0x821A429C, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821a443c
  if (cr0.eq) goto loc_821A443C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821A42AC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,-31660(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,12(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 12);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a4404
  if (cr6.lt) goto loc_821A4404;
  // beq cr6,0x821a43fc
  if (cr6.eq) goto loc_821A43FC;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // beq cr6,0x821a4368
  if (cr6.eq) goto loc_821A4368;
  // cmplwi cr6,r11,11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 11, so);
  // beq cr6,0x821a4360
  if (cr6.eq) goto loc_821A4360;
  // cmplwi cr6,r11,12
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 12, so);
  // beq cr6,0x821a42e0
  if (cr6.eq) goto loc_821A42E0;
  // cmplwi cr6,r11,13
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 13, so);
  // bne cr6,0x821a443c
  if (!cr6.eq) goto loc_821A443C;
loc_821A42E0:
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // beq cr6,0x821a4328
  if (cr6.eq) goto loc_821A4328;
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a4304
  if (cr6.eq) goto loc_821A4304;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4304:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a4358
  goto loc_821A4358;
loc_821A4328:
  // lwz r10,-31668(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,17
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 17, so);
  // beq cr6,0x821a433c
  if (cr6.eq) goto loc_821A433C;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821A433C:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
loc_821A4358:
  // li r10,8
  r10 = uint64_t(int64_t(8));
  // b 0x821a43e4
  goto loc_821A43E4;
loc_821A4360:
  // bl 0x821a2350
  // (left out: dead in C++)
  { const lifted::Result o = f821A2350(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821a443c
  goto loc_821A443C;
loc_821A4368:
  // lwz r11,16(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // beq cr6,0x821a43b0
  if (cr6.eq) goto loc_821A43B0;
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a438c
  if (cr6.eq) goto loc_821A438C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A438C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a43e0
  goto loc_821A43E0;
loc_821A43B0:
  // lwz r10,-31668(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,17
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 17, so);
  // beq cr6,0x821a43c4
  if (cr6.eq) goto loc_821A43C4;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821A43C4:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
loc_821A43E0:
  // li r10,9
  r10 = uint64_t(int64_t(9));
loc_821A43E4:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // b 0x821a443c
  goto loc_821A443C;
loc_821A43FC:
  // lwz r3,44(r10)
  r3 = REX_LOAD_U32(uint32_t(r10) + 44);
  // b 0x821a426c
  goto loc_821A426C;
loc_821A4404:
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x822952c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e822952C0(base, LIFTED_ARGS(0x821A4410, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31668(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,22
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 22, so);
  // beq cr6,0x821a4428
  if (cr6.eq) goto loc_821A4428;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821A4428:
  // li r9,22
  r9 = uint64_t(int64_t(22));
loc_821A442C:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r11,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r11));
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
loc_821A443C:
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

// sub_821A4450: lifted from generated/ by lift.py.
lifted::Result f821A4450(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_821A4450 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228f9e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228F9E8(base, LIFTED_ARGS(0x821A4470, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821a45f0
  if (cr6.eq) goto loc_821A45F0;
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // bne cr6,0x821a47f8
  if (!cr6.eq) goto loc_821A47F8;
  // lwz r10,24(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 24);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r10,24(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 24);
  // cmpwi cr6,r10,90
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 90, so);
  // blt cr6,0x821a47f8
  if (cr6.lt) goto loc_821A47F8;
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // beq cr6,0x821a4614
  if (cr6.eq) goto loc_821A4614;
  // cmpwi cr6,r3,8
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 8, so);
  // beq cr6,0x821a47f8
  if (cr6.eq) goto loc_821A47F8;
  // cmpwi cr6,r3,2
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 2, so);
  // bne cr6,0x821a4508
  if (!cr6.eq) goto loc_821A4508;
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228fa20
  // (left out: dead in C++)
  { const lifted::Result o = f8228FA20(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,4
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 4, so);
  // bne cr6,0x821a44f8
  if (!cr6.eq) goto loc_821A44F8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31668(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,23
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 23, so);
  // beq cr6,0x821a44e4
  if (cr6.eq) goto loc_821A44E4;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821A44E4:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r9,23
  r9 = uint64_t(int64_t(23));
  // stw r11,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r11));
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
loc_821A44F8:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228fa58
  // (left out: dead in C++)
  { const lifted::Result o = f8228FA58(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // b 0x821a47f8
  goto loc_821A47F8;
loc_821A4508:
  // cmpwi cr6,r3,3
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 3, so);
  // bne cr6,0x821a47f8
  if (!cr6.eq) goto loc_821A47F8;
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228fac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228FAC8(base, LIFTED_ARGS(0x821A4518, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x821a47f8
  if (!cr0.eq) goto loc_821A47F8;
  // bl 0x82105678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82105678(base, LIFTED_ARGS(0x821A4524, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r10,44(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 44);
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // ori r5,r11,54572
  r5 = r11 | 54572;
  // lis r11,-32127
  r11 = uint64_t(int64_t(-2105475072));
  // mr r9,r5
  r9 = r5;
  // lwz r11,25072(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 25072);
  // lwz r4,0(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 0);
  // mullw r11,r10,r9
  r11 = uint64_t(int64_t(int64_t(int32_t(r10)) * int64_t(int32_t(r9))));
  // add r3,r11,r3
  r3 = r11 + r3;
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x821A4550, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // beq cr6,0x821a459c
  if (cr6.eq) goto loc_821A459C;
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a4578
  if (cr6.eq) goto loc_821A4578;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4578:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,18
  r8 = uint64_t(int64_t(18));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a45cc
  goto loc_821A45CC;
loc_821A459C:
  // lwz r10,-31668(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,18
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 18, so);
  // beq cr6,0x821a45b0
  if (cr6.eq) goto loc_821A45B0;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821A45B0:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,18
  r8 = uint64_t(int64_t(18));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
loc_821A45CC:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r9,11
  r9 = uint64_t(int64_t(11));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // b 0x821a47f8
  goto loc_821A47F8;
loc_821A45F0:
  // lwz r10,24(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 24);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r10,24(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 24);
  // cmpwi cr6,r10,90
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 90, so);
  // blt cr6,0x821a47f8
  if (cr6.lt) goto loc_821A47F8;
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // bne cr6,0x821a46a4
  if (!cr6.eq) goto loc_821A46A4;
loc_821A4614:
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // beq cr6,0x821a465c
  if (cr6.eq) goto loc_821A465C;
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a4638
  if (cr6.eq) goto loc_821A4638;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4638:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a468c
  goto loc_821A468C;
loc_821A465C:
  // lwz r10,-31668(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,17
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 17, so);
  // beq cr6,0x821a4670
  if (cr6.eq) goto loc_821A4670;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821A4670:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
loc_821A468C:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // b 0x821a47f8
  goto loc_821A47F8;
loc_821A46A4:
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228fc50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228FC50(base, LIFTED_ARGS(0x821A46AC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi cr6,r3,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 1, so);
  // blt cr6,0x821a46c0
  if (cr6.lt) goto loc_821A46C0;
  // beq cr6,0x821a470c
  if (cr6.eq) goto loc_821A470C;
  // cmplwi cr6,r3,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 3, so);
  // bge cr6,0x821a47f8
  if (!cr6.lt) goto loc_821A47F8;
loc_821A46C0:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // beq cr6,0x821a47b0
  if (cr6.eq) goto loc_821A47B0;
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a46e8
  if (cr6.eq) goto loc_821A46E8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A46E8:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,18
  r8 = uint64_t(int64_t(18));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a47e0
  goto loc_821A47E0;
loc_821A470C:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // beq cr6,0x821a4758
  if (cr6.eq) goto loc_821A4758;
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a4734
  if (cr6.eq) goto loc_821A4734;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4734:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a4788
  goto loc_821A4788;
loc_821A4758:
  // lwz r10,-31668(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,17
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 17, so);
  // beq cr6,0x821a476c
  if (cr6.eq) goto loc_821A476C;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821A476C:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
loc_821A4788:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228fbc0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228FBC0(base, LIFTED_ARGS(0x821A47AC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821a47f8
  goto loc_821A47F8;
loc_821A47B0:
  // lwz r10,-31668(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,18
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 18, so);
  // beq cr6,0x821a47c4
  if (cr6.eq) goto loc_821A47C4;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821A47C4:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,18
  r8 = uint64_t(int64_t(18));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
loc_821A47E0:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // li r10,7
  r10 = uint64_t(int64_t(7));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
loc_821A47F8:
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

// sub_821A4810: lifted from generated/ by lift.py.
lifted::Result f821A4810(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_821A4810 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228f9e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228F9E8(base, LIFTED_ARGS(0x821A4830, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,4
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 4, so);
  // beq cr6,0x821a495c
  if (cr6.eq) goto loc_821A495C;
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228fa90
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228FA90(base, LIFTED_ARGS(0x821A4844, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // beq 0x821a48e0
  if (cr0.eq) goto loc_821A48E0;
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821a48a0
  if (cr6.eq) goto loc_821A48A0;
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x821a4884
  if (cr6.eq) goto loc_821A4884;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4884:
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a48c4
  goto loc_821A48C4;
loc_821A48A0:
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x821a48ac
  if (cr6.eq) goto loc_821A48AC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A48AC:
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r7,19
  r7 = uint64_t(int64_t(19));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
loc_821A48C4:
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // b 0x821a495c
  goto loc_821A495C;
loc_821A48E0:
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821a4920
  if (cr6.eq) goto loc_821A4920;
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a4904
  if (cr6.eq) goto loc_821A4904;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4904:
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a4944
  goto loc_821A4944;
loc_821A4920:
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821a492c
  if (cr6.eq) goto loc_821A492C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A492C:
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r7,17
  r7 = uint64_t(int64_t(17));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
loc_821A4944:
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
loc_821A495C:
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

// sub_821A4970: lifted from generated/ by lift.py.
lifted::Result f821A4970(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_821A4970 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228fd50
  // (left out: dead in C++)
  { const lifted::Result o = f8228FD50(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,3
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 3, so);
  // beq cr6,0x821a4a58
  if (cr6.eq) goto loc_821A4A58;
  // cmpwi cr6,r3,7
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 7, so);
  // beq cr6,0x821a4a14
  if (cr6.eq) goto loc_821A4A14;
  // cmpwi cr6,r3,8
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 8, so);
  // bne cr6,0x821a4aa8
  if (!cr6.eq) goto loc_821A4AA8;
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821a49ec
  if (cr6.eq) goto loc_821A49EC;
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a49d0
  if (cr6.eq) goto loc_821A49D0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A49D0:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a4a0c
  goto loc_821A4A0C;
loc_821A49EC:
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a49f8
  if (cr6.eq) goto loc_821A49F8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A49F8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821A4A0C:
  // li r10,13
  r10 = uint64_t(int64_t(13));
  // b 0x821a4a8c
  goto loc_821A4A8C;
loc_821A4A14:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31668(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31668);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // beq cr6,0x821a4a68
  if (cr6.eq) goto loc_821A4A68;
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a4a3c
  if (cr6.eq) goto loc_821A4A3C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4A3C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821a4a88
  goto loc_821A4A88;
loc_821A4A58:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228fa58
  // (left out: dead in C++)
  { const lifted::Result o = f8228FA58(base, r1, r13, r3, r4, r5, r6, r7, r8, r9); r3 = o.r3; f1 = o.f1; }
  // b 0x821a4a14
  goto loc_821A4A14;
loc_821A4A68:
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821a4a74
  if (cr6.eq) goto loc_821A4A74;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A4A74:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
loc_821A4A88:
  // li r10,12
  r10 = uint64_t(int64_t(12));
loc_821A4A8C:
  // li r8,18
  r8 = uint64_t(int64_t(18));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
loc_821A4AA8:
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

// sub_821A4EF8: lifted from generated/ by lift.py.
lifted::Result f821A4EF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_821A4EF8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r29,r3
  r29 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x8228f9e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228F9E8(base, LIFTED_ARGS(0x821A4F10, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r31,r11,1
  r31 = r11 ^ 1;
  // bl 0x8228f9e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8228F9E8(base, LIFTED_ARGS(0x821A4F28, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r11,r11,1
  r11 = r11 ^ 1;
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // beq cr6,0x821a4f70
  if (cr6.eq) goto loc_821A4F70;
  // bl 0x82105678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82105678(base, LIFTED_ARGS(0x821A4F40, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // bl 0x82105678
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82105678(base, LIFTED_ARGS(0x821A4F48, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // cmpwi cr6,r31,0
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 0, so);
  // ori r5,r11,54572
  r5 = r11 | 54572;
  // add r3,r3,r5
  r3 = r3 + r5;
  // beq cr6,0x821a4f64
  if (cr6.eq) goto loc_821A4F64;
  // mr r4,r30
  r4 = r30;
  // b 0x821a4f6c
  goto loc_821A4F6C;
loc_821A4F64:
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
loc_821A4F6C:
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x821A4F70, r31)); r3 = o.r3; f1 = o.f1; }
loc_821A4F70:
  // addi r11,r29,204
  r11 = uint64_t(int64_t(int64_t(r29) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,31
  r9 = uint64_t(int64_t(31));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // lwz r6,604(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
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
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31660);
  // stw r4,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r4));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821A6030: lifted from generated/ by lift.py.
lifted::Result f821A6030(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821A6030 (lifted)");
  uint64_t r0 = 0, r4 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,44
  r11 = uint64_t(int64_t(44));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821A6058, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821a6318
  if (!cr6.eq) goto loc_821A6318;
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // stw r29,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r29));
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmplwi cr6,r11,13
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 13, so);
  // bgt cr6,0x821a61d4
  if (cr6.gt) goto loc_821A61D4;
  // lis r12,-32255
  r12 = uint64_t(int64_t(-2113863680));
  // addi r12,r12,-23336
  r12 = uint64_t(int64_t(int64_t(r12) + -23336));
  // lbzx r0,r12,r11
  r0 = REX_LOAD_U8(uint32_t(r12) + uint32_t(r11));
  // rlwinm r0,r0,2,0,29
  r0 = __builtin_rotateleft64(uint32_t(r0) | (r0 << 32), 2) & 0xFFFFFFFC;
  // lis r12,-32230
  r12 = uint64_t(int64_t(-2112225280));
  // addi r12,r12,24756
  r12 = uint64_t(int64_t(int64_t(r12) + 24756));
  // add r12,r12,r0
  r12 = r12 + r0;
  // mtctr r12
  ctr = r12;
  // bctr
  switch (uint32_t(r11)) {
  case 0:
  goto loc_frag_821A60B4;
  return {r3, f1};
  case 1:
  goto loc_821A60C4;
  case 2:
  goto loc_821A60FC;
  case 3:
  goto loc_821A6118;
  case 4:
  goto loc_821A6128;
  case 5:
  goto loc_821A6138;
  case 6:
  goto loc_821A6154;
  case 7:
  goto loc_821A6164;
  case 8:
  goto loc_821A60FC;
  case 9:
  goto loc_821A60FC;
  case 10:
  goto loc_821A6174;
  case 11:
  goto loc_821A61A4;
  case 12:
  goto loc_821A61B4;
  case 13:
  goto loc_821A61C4;
  default:
  std::abort();
  }
loc_821A60C4:
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // std r28,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r28);
  // std r28,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r28);
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228f7d0
  // (left out: dead in C++)
  { const lifted::Result o = f8228F7D0(base, r1, r13, r3, r4, r6, r7, r8, r9, 0x821A60E0); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r3,99
  r3 = uint64_t(int64_t(99));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A60EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,99
  r3 = uint64_t(int64_t(99));
loc_821A60F0:
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x821a52c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A52C0, LIFTED_ARGS(0x821A60F8, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821a61d4
  goto loc_821A61D4;
loc_821A60FC:
  // li r3,86
  r3 = uint64_t(int64_t(86));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A6104, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,86
  r3 = uint64_t(int64_t(86));
loc_821A6108:
  // bl 0x821a52c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A52C0, LIFTED_ARGS(0x821A610C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r29,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r29));
  // b 0x821a61d4
  goto loc_821A61D4;
loc_821A6118:
  // li r3,62
  r3 = uint64_t(int64_t(62));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A6120, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,62
  r3 = uint64_t(int64_t(62));
  // b 0x821a61d0
  goto loc_821A61D0;
loc_821A6128:
  // li r3,83
  r3 = uint64_t(int64_t(83));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A6130, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,83
  r3 = uint64_t(int64_t(83));
  // b 0x821a6108
  goto loc_821A6108;
loc_821A6138:
  // li r3,84
  r3 = uint64_t(int64_t(84));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A6140, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,84
  r3 = uint64_t(int64_t(84));
  // bl 0x821a52c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A52C0, LIFTED_ARGS(0x821A6148, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r28,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r28));
  // b 0x821a61d4
  goto loc_821A61D4;
loc_821A6154:
  // li r3,84
  r3 = uint64_t(int64_t(84));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A615C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,84
  r3 = uint64_t(int64_t(84));
  // b 0x821a6108
  goto loc_821A6108;
loc_821A6164:
  // li r3,85
  r3 = uint64_t(int64_t(85));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A616C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,85
  r3 = uint64_t(int64_t(85));
  // b 0x821a61d0
  goto loc_821A61D0;
loc_821A6174:
  // addi r11,r1,112
  r11 = uint64_t(int64_t(int64_t(r1) + 112));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // std r28,0(r11)
  REX_STORE_U64(uint32_t(r11) + 0, r28);
  // std r28,8(r11)
  REX_STORE_U64(uint32_t(r11) + 8, r28);
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,44(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 44);
  // bl 0x8228f7d0
  // (left out: dead in C++)
  { const lifted::Result o = f8228F7D0(base, r1, r13, r3, r4, r6, r7, r8, r9, 0x821A6190); r3 = o.r3; f1 = o.f1; }
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A619C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // b 0x821a60f0
  goto loc_821A60F0;
loc_821A61A4:
  // li r3,88
  r3 = uint64_t(int64_t(88));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A61AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,88
  r3 = uint64_t(int64_t(88));
  // b 0x821a61d0
  goto loc_821A61D0;
loc_821A61B4:
  // li r3,60
  r3 = uint64_t(int64_t(60));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A61BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,60
  r3 = uint64_t(int64_t(60));
  // b 0x821a61d0
  goto loc_821A61D0;
loc_821A61C4:
  // li r3,103
  r3 = uint64_t(int64_t(103));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A61CC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,103
  r3 = uint64_t(int64_t(103));
loc_821A61D0:
  // bl 0x821a52c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A52C0, LIFTED_ARGS(0x821A61D4, r31)); r3 = o.r3; f1 = o.f1; }
loc_821A61D4:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a62a8
  if (cr6.lt) goto loc_821A62A8;
  // beq cr6,0x821a6254
  if (cr6.eq) goto loc_821A6254;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a6310
  if (!cr6.lt) goto loc_821A6310;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,13
  r4 = uint64_t(int64_t(13));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A620C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821A621C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r29,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821A6234, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D658(base, LIFTED_ARGS(0x821A623C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r28));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r28,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r28));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r28,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r28));
  // b 0x821a6310
  goto loc_821A6310;
loc_821A6254:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,13
  r4 = uint64_t(int64_t(13));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A6270, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821A6280, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r28,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r28));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821A6298, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D658(base, LIFTED_ARGS(0x821A62A0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r29));
  // b 0x821a6308
  goto loc_821A6308;
loc_821A62A8:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A62C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,159
  r5 = uint64_t(int64_t(159));
  // li r11,158
  r11 = uint64_t(int64_t(158));
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // li r10,156
  r10 = uint64_t(int64_t(156));
  // li r9,155
  r9 = uint64_t(int64_t(155));
  // li r8,153
  r8 = uint64_t(int64_t(153));
  // stw r5,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r5));
  // li r7,154
  r7 = uint64_t(int64_t(154));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // bl 0x8220bac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BAC0(base, LIFTED_ARGS(0x821A62F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // lwz r4,48(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 48);
  // bl 0x8220bc30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC30(base, LIFTED_ARGS(0x821A6308, r31)); r3 = o.r3; f1 = o.f1; }
loc_821A6308:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r29,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r29));
loc_821A6310:
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821A6318, r31)); r3 = o.r3; f1 = o.f1; }
loc_821A6318:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821A6324, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r29,r11,27,31,31
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq 0x821a63a4
  if (cr0.eq) goto loc_821A63A4;
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821a636c
  if (cr0.eq) goto loc_821A636C;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821a63a4
  if (!cr6.eq) goto loc_821A63A4;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A6364, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x821a639c
  goto loc_821A639C;
loc_821A636C:
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC20(base, LIFTED_ARGS(0x821A6374, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r31,r11,2
  r31 = uint64_t(int64_t(int64_t(r11) + 2));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A6398, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
loc_821A639C:
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821A63A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_821A63A4:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
loc_frag_821A60B4:
  // li r3,101
  r3 = uint64_t(int64_t(101));
  // bl 0x821a51c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A51C8, LIFTED_ARGS(0x821A60BC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,101
  r3 = uint64_t(int64_t(101));
  // b 0x821a61d0
  goto loc_821A61D0_821A60B4;
loc_821A61D0_821A60B4:
  // bl 0x821a52c0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821A52C0, LIFTED_ARGS(0x821A61D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a62a8
  if (cr6.lt) goto loc_821A62A8_821A60B4;
  // beq cr6,0x821a6254
  if (cr6.eq) goto loc_821A6254_821A60B4;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a6310
  if (!cr6.lt) goto loc_821A6310_821A60B4;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,13
  r4 = uint64_t(int64_t(13));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A620C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821A621C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r29,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821A6234, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D658(base, LIFTED_ARGS(0x821A623C, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r28,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r28));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r28,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r28));
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r28,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r28));
  // b 0x821a6310
  goto loc_821A6310_821A60B4;
loc_821A6254_821A60B4:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,13
  r4 = uint64_t(int64_t(13));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A6270, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821A6280, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // stw r28,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r28));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821A6298, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D658(base, LIFTED_ARGS(0x821A62A0, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r29));
  // b 0x821a6308
  goto loc_821A6308_821A60B4;
loc_821A62A8_821A60B4:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A62C4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,159
  r5 = uint64_t(int64_t(159));
  // li r11,158
  r11 = uint64_t(int64_t(158));
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // li r10,156
  r10 = uint64_t(int64_t(156));
  // li r9,155
  r9 = uint64_t(int64_t(155));
  // li r8,153
  r8 = uint64_t(int64_t(153));
  // stw r5,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r5));
  // li r7,154
  r7 = uint64_t(int64_t(154));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // bl 0x8220bac0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BAC0(base, LIFTED_ARGS(0x821A62F8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // lwz r4,48(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 48);
  // bl 0x8220bc30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC30(base, LIFTED_ARGS(0x821A6308, r31)); r3 = o.r3; f1 = o.f1; }
loc_821A6308_821A60B4:
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // stw r29,56(r11)
  REX_STORE_U32(uint32_t(r11) + 56, uint32_t(r29));
loc_821A6310_821A60B4:
  // li r3,5
  r3 = uint64_t(int64_t(5));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821A6318, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821A6324, r31)); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r29,r11,27,31,31
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // beq 0x821a63a4
  if (cr0.eq) goto loc_821A63A4_821A60B4;
  // lwz r11,-31660(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31660);
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821a636c
  if (cr0.eq) goto loc_821A636C_821A60B4;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821a63a4
  if (!cr6.eq) goto loc_821A63A4_821A60B4;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A6364, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // b 0x821a639c
  goto loc_821A639C_821A60B4;
loc_821A636C_821A60B4:
  // lwz r3,-31664(r27)
  r3 = REX_LOAD_U32(uint32_t(r27) + -31664);
  // bl 0x8220bc20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8220BC20(base, LIFTED_ARGS(0x821A6374, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r31,r11,2
  r31 = uint64_t(int64_t(int64_t(r11) + 2));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821A6398, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
loc_821A639C_821A60B4:
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821A63A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_821A63A4_821A60B4:
  // mr r3,r29
  r3 = r29;
  // addi r1,r1,176
  r1 = uint64_t(int64_t(int64_t(r1) + 176));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821A8DE0: lifted from generated/ by lift.py.
uint64_t f821A8DE0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821A8DE0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,572(r11)
  REX_STORE_U32(uint32_t(r11) + 572, uint32_t(r10));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82105620(base, LIFTED_ARGS(0x821A8E04, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // ori r11,r11,63600
  r11 = r11 | 63600;
  // stwx r10,r3,r11
  REX_STORE_U32(uint32_t(r3) + uint32_t(r11), uint32_t(r10));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_821A8E28: lifted from generated/ by lift.py.
uint64_t f821A8E28(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821A8E28 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,572(r11)
  REX_STORE_U32(uint32_t(r11) + 572, uint32_t(r10));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82105620(base, LIFTED_ARGS(0x821A8E4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // ori r11,r11,63600
  r11 = r11 | 63600;
  // stwx r10,r3,r11
  REX_STORE_U32(uint32_t(r3) + uint32_t(r11), uint32_t(r10));
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_821A8E70: lifted from generated/ by lift.py.
void f821A8E70(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821A8E70 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,572(r11)
  REX_STORE_U32(uint32_t(r11) + 572, uint32_t(r10));
  // blr
  return;
}

// sub_821A8E88: lifted from generated/ by lift.py.
void f821A8E88(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821A8E88 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,572(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 572);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bne cr6,0x821a8ec8
  if (!cr6.eq) goto loc_821A8EC8;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x821a8eb4
  if (cr6.eq) goto loc_821A8EB4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A8EB4:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // b 0x821a8eec
  goto loc_821A8EEC;
loc_821A8EC8:
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // beq cr6,0x821a8ed4
  if (cr6.eq) goto loc_821A8ED4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821A8ED4:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,7
  r8 = uint64_t(int64_t(7));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
loc_821A8EEC:
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // blr
  return;
}

// sub_821A8EF8: lifted from generated/ by lift.py.
uint64_t f821A8EF8(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821A8EF8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31652(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // addi r7,r11,604
  r7 = uint64_t(int64_t(int64_t(r11) + 604));
  // addi r6,r11,616
  r6 = uint64_t(int64_t(int64_t(r11) + 616));
  // addi r5,r11,628
  r5 = uint64_t(int64_t(int64_t(r11) + 628));
  // lwz r8,36(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 36);
  // addi r4,r11,640
  r4 = uint64_t(int64_t(int64_t(r11) + 640));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r31,r11,652
  r31 = uint64_t(int64_t(int64_t(r11) + 652));
  // stw r11,-160(r1)
  REX_STORE_U32(uint32_t(r1) + -160, uint32_t(r11));
  // lwz r3,0(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 0);
  // addi r30,r11,664
  r30 = uint64_t(int64_t(int64_t(r11) + 664));
  // addi r29,r11,676
  r29 = uint64_t(int64_t(int64_t(r11) + 676));
  // cmpwi cr6,r8,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), -1, so);
  // stw r3,-128(r1)
  REX_STORE_U32(uint32_t(r1) + -128, uint32_t(r3));
  // lwz r3,4(r7)
  r3 = REX_LOAD_U32(uint32_t(r7) + 4);
  // lwz r7,8(r7)
  r7 = REX_LOAD_U32(uint32_t(r7) + 8);
  // stw r3,-124(r1)
  REX_STORE_U32(uint32_t(r1) + -124, uint32_t(r3));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r7,-120(r1)
  REX_STORE_U32(uint32_t(r1) + -120, uint32_t(r7));
  // lwz r7,0(r6)
  r7 = REX_LOAD_U32(uint32_t(r6) + 0);
  // stw r7,-116(r1)
  REX_STORE_U32(uint32_t(r1) + -116, uint32_t(r7));
  // lwz r7,4(r6)
  r7 = REX_LOAD_U32(uint32_t(r6) + 4);
  // stw r7,-112(r1)
  REX_STORE_U32(uint32_t(r1) + -112, uint32_t(r7));
  // addi r7,r11,4
  r7 = uint64_t(int64_t(int64_t(r11) + 4));
  // stw r7,-156(r1)
  REX_STORE_U32(uint32_t(r1) + -156, uint32_t(r7));
  // lwz r7,8(r6)
  r7 = REX_LOAD_U32(uint32_t(r6) + 8);
  // addi r6,r11,24
  r6 = uint64_t(int64_t(int64_t(r11) + 24));
  // stw r7,-108(r1)
  REX_STORE_U32(uint32_t(r1) + -108, uint32_t(r7));
  // addi r7,r11,8
  r7 = uint64_t(int64_t(int64_t(r11) + 8));
  // stw r6,-136(r1)
  REX_STORE_U32(uint32_t(r1) + -136, uint32_t(r6));
  // stw r7,-152(r1)
  REX_STORE_U32(uint32_t(r1) + -152, uint32_t(r7));
  // lwz r7,0(r5)
  r7 = REX_LOAD_U32(uint32_t(r5) + 0);
  // stw r7,-104(r1)
  REX_STORE_U32(uint32_t(r1) + -104, uint32_t(r7));
  // addi r7,r11,12
  r7 = uint64_t(int64_t(int64_t(r11) + 12));
  // stw r7,-148(r1)
  REX_STORE_U32(uint32_t(r1) + -148, uint32_t(r7));
  // lwz r7,4(r5)
  r7 = REX_LOAD_U32(uint32_t(r5) + 4);
  // stw r7,-100(r1)
  REX_STORE_U32(uint32_t(r1) + -100, uint32_t(r7));
  // addi r7,r11,16
  r7 = uint64_t(int64_t(int64_t(r11) + 16));
  // stw r7,-144(r1)
  REX_STORE_U32(uint32_t(r1) + -144, uint32_t(r7));
  // lwz r7,8(r5)
  r7 = REX_LOAD_U32(uint32_t(r5) + 8);
  // stw r7,-96(r1)
  REX_STORE_U32(uint32_t(r1) + -96, uint32_t(r7));
  // addi r7,r11,20
  r7 = uint64_t(int64_t(int64_t(r11) + 20));
  // stw r7,-140(r1)
  REX_STORE_U32(uint32_t(r1) + -140, uint32_t(r7));
  // lwz r7,0(r4)
  r7 = REX_LOAD_U32(uint32_t(r4) + 0);
  // stw r7,-92(r1)
  REX_STORE_U32(uint32_t(r1) + -92, uint32_t(r7));
  // lwz r7,4(r4)
  r7 = REX_LOAD_U32(uint32_t(r4) + 4);
  // stw r7,-88(r1)
  REX_STORE_U32(uint32_t(r1) + -88, uint32_t(r7));
  // lwz r7,8(r4)
  r7 = REX_LOAD_U32(uint32_t(r4) + 8);
  // stw r7,-84(r1)
  REX_STORE_U32(uint32_t(r1) + -84, uint32_t(r7));
  // lwz r7,0(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r7,-80(r1)
  REX_STORE_U32(uint32_t(r1) + -80, uint32_t(r7));
  // lwz r7,4(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 4);
  // stw r7,-76(r1)
  REX_STORE_U32(uint32_t(r1) + -76, uint32_t(r7));
  // lwz r7,8(r31)
  r7 = REX_LOAD_U32(uint32_t(r31) + 8);
  // stw r7,-72(r1)
  REX_STORE_U32(uint32_t(r1) + -72, uint32_t(r7));
  // lwz r7,0(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 0);
  // stw r7,-68(r1)
  REX_STORE_U32(uint32_t(r1) + -68, uint32_t(r7));
  // lwz r7,4(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 4);
  // stw r7,-64(r1)
  REX_STORE_U32(uint32_t(r1) + -64, uint32_t(r7));
  // lwz r7,8(r30)
  r7 = REX_LOAD_U32(uint32_t(r30) + 8);
  // stw r7,-60(r1)
  REX_STORE_U32(uint32_t(r1) + -60, uint32_t(r7));
  // lwz r7,0(r29)
  r7 = REX_LOAD_U32(uint32_t(r29) + 0);
  // stw r7,-56(r1)
  REX_STORE_U32(uint32_t(r1) + -56, uint32_t(r7));
  // lwz r7,4(r29)
  r7 = REX_LOAD_U32(uint32_t(r29) + 4);
  // stw r7,-52(r1)
  REX_STORE_U32(uint32_t(r1) + -52, uint32_t(r7));
  // lwz r7,8(r29)
  r7 = REX_LOAD_U32(uint32_t(r29) + 8);
  // stw r7,-48(r1)
  REX_STORE_U32(uint32_t(r1) + -48, uint32_t(r7));
  // bne cr6,0x821a9124
  if (!cr6.eq) goto loc_821A9124;
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // beq cr6,0x821a90ec
  if (cr6.eq) goto loc_821A90EC;
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r1,-160
  r8 = uint64_t(int64_t(int64_t(r1) + -160));
  // mulli r11,r10,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r8,r9,r8
  r8 = r9 + r8;
  // addi r9,r1,-128
  r9 = uint64_t(int64_t(int64_t(r1) + -128));
  // add r9,r11,r9
  r9 = r11 + r9;
  // lwz r11,0(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + 0);
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x821a9054
  if (!cr6.gt) goto loc_821A9054;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9054:
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // lfs f0,8748(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 8748);
  f0 = double(temp.f32);
  // fsubs f0,f13,f0
  f0 = double(float(f13 - f0));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // beq cr6,0x821a90d8
  if (cr6.eq) goto loc_821A90D8;
  // mr r7,r10
  r7 = r10;
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bgt cr6,0x821a9240
  if (cr6.gt) goto loc_821A9240;
  // mr r11,r8
  r11 = r8;
loc_821A9080:
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // lfs f13,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x821a9098
  if (!cr6.lt) goto loc_821A9098;
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
loc_821A9098:
  // cmpwi cr6,r7,5
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 5, so);
  // beq cr6,0x821a9240
  if (cr6.eq) goto loc_821A9240;
  // cmpwi cr6,r7,6
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 6, so);
  // beq cr6,0x821a9240
  if (cr6.eq) goto loc_821A9240;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lfs f0,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // lfs f13,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x821a9240
  if (!cr6.lt) goto loc_821A9240;
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addi r9,r9,12
  r9 = uint64_t(int64_t(int64_t(r9) + 12));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmpwi cr6,r7,5
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 5, so);
  // ble cr6,0x821a9080
  if (!cr6.gt) goto loc_821A9080;
  // b 0x821a9240
  goto loc_821A9240;
loc_821A90D8:
  // lfs f13,0(r6)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f13 = double(temp.f32);
  // lfs f0,-56(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -56);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x821a9240
  if (!cr6.lt) goto loc_821A9240;
  // b 0x821a91f4
  goto loc_821A91F4;
loc_821A90EC:
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r10,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r10));
  // lwz r10,-31648(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a9220
  if (cr6.lt) goto loc_821A9220;
  // beq cr6,0x821a911c
  if (cr6.eq) goto loc_821A911C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a9238
  if (!cr6.lt) goto loc_821A9238;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821a9238
  goto loc_821A9238;
loc_821A911C:
  // stw r3,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r3));
  // b 0x821a9238
  goto loc_821A9238;
loc_821A9124:
  // cmpwi cr6,r8,1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 1, so);
  // bne cr6,0x821a9240
  if (!cr6.eq) goto loc_821A9240;
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // beq cr6,0x821a91fc
  if (cr6.eq) goto loc_821A91FC;
  // rlwinm r9,r10,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r1,-160
  r8 = uint64_t(int64_t(int64_t(r1) + -160));
  // mulli r11,r10,12
  r11 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r8,r9,r8
  r8 = r9 + r8;
  // addi r9,r1,-124
  r9 = uint64_t(int64_t(int64_t(r1) + -124));
  // add r9,r11,r9
  r9 = r11 + r9;
  // lwz r11,0(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + 0);
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // lfs f13,0(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bge cr6,0x821a9164
  if (!cr6.lt) goto loc_821A9164;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9164:
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // lfs f13,0(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 0);
  f13 = double(temp.f32);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // lfs f0,8748(r7)
  temp.u32 = REX_LOAD_U32(uint32_t(r7) + 8748);
  f0 = double(temp.f32);
  // fadds f0,f13,f0
  f0 = double(float(f13 + f0));
  // stfs f0,0(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 0, temp.u32);
  // beq cr6,0x821a91e4
  if (cr6.eq) goto loc_821A91E4;
  // mr r7,r10
  r7 = r10;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x821a9240
  if (cr6.lt) goto loc_821A9240;
  // mr r11,r8
  r11 = r8;
loc_821A9190:
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lfs f0,0(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 0);
  f0 = double(temp.f32);
  // lfs f13,0(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x821a91a8
  if (!cr6.gt) goto loc_821A91A8;
  // stfs f0,0(r8)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r8) + 0, temp.u32);
loc_821A91A8:
  // cmpwi cr6,r7,0
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // beq cr6,0x821a9240
  if (cr6.eq) goto loc_821A9240;
  // cmpwi cr6,r7,6
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 6, so);
  // beq cr6,0x821a9240
  if (cr6.eq) goto loc_821A9240;
  // lwz r10,-4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -4);
  // lfs f0,0(r8)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 0);
  f0 = double(temp.f32);
  // lfs f13,0(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // ble cr6,0x821a9240
  if (!cr6.gt) goto loc_821A9240;
  // stfs f0,0(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 0, temp.u32);
  // addic. r7,r7,-1
  ca = uint32_t(r7) > 0;
  r7 = uint64_t(int64_t(int64_t(r7) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // addi r9,r9,-12
  r9 = uint64_t(int64_t(int64_t(r9) + -12));
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // bge 0x821a9190
  if (!cr0.lt) goto loc_821A9190;
  // b 0x821a9240
  goto loc_821A9240;
loc_821A91E4:
  // lfs f13,0(r6)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 0);
  f13 = double(temp.f32);
  // lfs f0,-52(r1)
  temp.u32 = REX_LOAD_U32(uint32_t(r1) + -52);
  f0 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // ble cr6,0x821a9240
  if (!cr6.gt) goto loc_821A9240;
loc_821A91F4:
  // stfs f0,0(r6)
  lifted::ScalarFloatMode();
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r6) + 0, temp.u32);
  // b 0x821a9240
  goto loc_821A9240;
loc_821A91FC:
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r10,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r10));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a9230
  if (cr6.lt) goto loc_821A9230;
  // beq cr6,0x821a9228
  if (cr6.eq) goto loc_821A9228;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a9238
  if (!cr6.lt) goto loc_821A9238;
loc_821A9220:
  // mr r11,r3
  r11 = r3;
  // b 0x821a923c
  goto loc_821A923C;
loc_821A9228:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821a9234
  goto loc_821A9234;
loc_821A9230:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821A9234:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821A9238:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821A923C:
  // mr r3,r11
  r3 = r11;
loc_821A9240:
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_821A9248: lifted from generated/ by lift.py.
uint64_t f821A9248(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821A9248 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lis r8,-32146
  r8 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r7,r5
  r7 = r5;
  // lwz r11,-31648(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31648);
  // mr r3,r5
  r3 = r5;
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r4,r11,28
  r4 = uint64_t(int64_t(int64_t(r11) + 28));
  // lwz r9,688(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 688);
  // lwz r6,36(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 36);
  // stw r4,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r4));
  // addi r4,r11,32
  r4 = uint64_t(int64_t(int64_t(r11) + 32));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplwi cr6,r9,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 1, so);
  // stw r4,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // blt cr6,0x821a92a8
  if (cr6.lt) goto loc_821A92A8;
  // cmplwi cr6,r9,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 4, so);
  // blt cr6,0x821a92a0
  if (cr6.lt) goto loc_821A92A0;
  // bne cr6,0x821a92ac
  if (!cr6.eq) goto loc_821A92AC;
  // mr r7,r10
  r7 = r10;
  // b 0x821a92ac
  goto loc_821A92AC;
loc_821A92A0:
  // mr r7,r4
  r7 = r4;
  // b 0x821a92ac
  goto loc_821A92AC;
loc_821A92A8:
  // mr r7,r5
  r7 = r5;
loc_821A92AC:
  // cmpwi cr6,r6,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r6), -1, so);
  // bne cr6,0x821a932c
  if (!cr6.eq) goto loc_821A932C;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821a92f0
  if (cr6.eq) goto loc_821A92F0;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r7,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble cr6,0x821a92d8
  if (!cr6.gt) goto loc_821A92D8;
  // mr r3,r4
  r3 = r4;
loc_821A92D8:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bgelr
  if (!cr0.lt) return r3;
  // stw r5,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r5));
  // blr
  return r3;
loc_821A92F0:
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r10,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r10));
  // lwz r11,-31648(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a939c
  if (cr6.lt) goto loc_821A939C;
  // beq cr6,0x821a9324
  if (cr6.eq) goto loc_821A9324;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a9318
  if (!cr6.lt) goto loc_821A9318;
loc_821A9314:
  // stw r4,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r4));
loc_821A9318:
  // mr r11,r4
  r11 = r4;
loc_821A931C:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
loc_821A9324:
  // stw r5,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r5));
  // b 0x821a9318
  goto loc_821A9318;
loc_821A932C:
  // cmpwi cr6,r6,1
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821a9378
  if (cr6.eq) goto loc_821A9378;
  // rlwinm r11,r7,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // lwzx r10,r11,r10
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bge cr6,0x821a9358
  if (!cr6.lt) goto loc_821A9358;
  // mr r3,r4
  r3 = r4;
loc_821A9358:
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // blr
  return r3;
loc_821A9378:
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r10,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r10));
  // lwz r11,-31648(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a9314
  if (cr6.lt) goto loc_821A9314;
  // beq cr6,0x821a93a4
  if (cr6.eq) goto loc_821A93A4;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a9318
  if (!cr6.lt) goto loc_821A9318;
loc_821A939C:
  // mr r11,r5
  r11 = r5;
  // b 0x821a931c
  goto loc_821A931C;
loc_821A93A4:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // b 0x821a9318
  goto loc_821A9318;
}

// sub_821A93B0: lifted from generated/ by lift.py.
uint64_t f821A93B0(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821A93B0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lis r8,-32146
  r8 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r7,r5
  r7 = r5;
  // lwz r11,-31648(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31648);
  // mr r3,r5
  r3 = r5;
  // lwz r10,-31652(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31652);
  // addi r4,r11,36
  r4 = uint64_t(int64_t(int64_t(r11) + 36));
  // lwz r9,688(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 688);
  // lwz r6,36(r10)
  r6 = REX_LOAD_U32(uint32_t(r10) + 36);
  // stw r4,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r4));
  // addi r4,r11,40
  r4 = uint64_t(int64_t(int64_t(r11) + 40));
  // lwz r10,0(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmplwi cr6,r9,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 1, so);
  // stw r4,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // blt cr6,0x821a9410
  if (cr6.lt) goto loc_821A9410;
  // cmplwi cr6,r9,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 4, so);
  // blt cr6,0x821a9408
  if (cr6.lt) goto loc_821A9408;
  // bne cr6,0x821a9414
  if (!cr6.eq) goto loc_821A9414;
  // mr r7,r10
  r7 = r10;
  // b 0x821a9414
  goto loc_821A9414;
loc_821A9408:
  // mr r7,r4
  r7 = r4;
  // b 0x821a9414
  goto loc_821A9414;
loc_821A9410:
  // mr r7,r5
  r7 = r5;
loc_821A9414:
  // cmpwi cr6,r6,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r6), -1, so);
  // bne cr6,0x821a9494
  if (!cr6.eq) goto loc_821A9494;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821a9458
  if (cr6.eq) goto loc_821A9458;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r7,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble cr6,0x821a9440
  if (!cr6.gt) goto loc_821A9440;
  // mr r3,r4
  r3 = r4;
loc_821A9440:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bgelr
  if (!cr0.lt) return r3;
  // stw r5,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r5));
  // blr
  return r3;
loc_821A9458:
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r10,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r10));
  // lwz r11,-31648(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a9504
  if (cr6.lt) goto loc_821A9504;
  // beq cr6,0x821a948c
  if (cr6.eq) goto loc_821A948C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a9480
  if (!cr6.lt) goto loc_821A9480;
loc_821A947C:
  // stw r4,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r4));
loc_821A9480:
  // mr r11,r4
  r11 = r4;
loc_821A9484:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
loc_821A948C:
  // stw r5,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r5));
  // b 0x821a9480
  goto loc_821A9480;
loc_821A9494:
  // cmpwi cr6,r6,1
  cr6 = lifted::Compare<int32_t>(int32_t(r6), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821a94e0
  if (cr6.eq) goto loc_821A94E0;
  // rlwinm r11,r7,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // lwzx r10,r11,r10
  r10 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bge cr6,0x821a94c0
  if (!cr6.lt) goto loc_821A94C0;
  // mr r3,r4
  r3 = r4;
loc_821A94C0:
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // blr
  return r3;
loc_821A94E0:
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r10,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r10));
  // lwz r11,-31648(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a947c
  if (cr6.lt) goto loc_821A947C;
  // beq cr6,0x821a950c
  if (cr6.eq) goto loc_821A950C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a9480
  if (!cr6.lt) goto loc_821A9480;
loc_821A9504:
  // mr r11,r5
  r11 = r5;
  // b 0x821a9484
  goto loc_821A9484;
loc_821A950C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // b 0x821a9480
  goto loc_821A9480;
}

// sub_821A9518: lifted from generated/ by lift.py.
uint64_t f821A9518(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821A9518 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r3,r7
  r3 = r7;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r9,36(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // addi r6,r11,44
  r6 = uint64_t(int64_t(int64_t(r11) + 44));
  // stw r6,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r6));
  // addi r6,r11,48
  r6 = uint64_t(int64_t(int64_t(r11) + 48));
  // stw r6,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r6));
  // bne cr6,0x821a95cc
  if (!cr6.eq) goto loc_821A95CC;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a9594
  if (cr6.eq) goto loc_821A9594;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -10, so);
  // ble cr6,0x821a9574
  if (!cr6.gt) goto loc_821A9574;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9574:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r10,-10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -10, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bgelr cr6
  if (!cr6.lt) return r3;
  // li r10,-10
  r10 = uint64_t(int64_t(-10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blr
  return r3;
loc_821A9594:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a9634
  if (cr6.lt) goto loc_821A9634;
  // beq cr6,0x821a95c4
  if (cr6.eq) goto loc_821A95C4;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a964c
  if (!cr6.lt) goto loc_821A964C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821a964c
  goto loc_821A964C;
loc_821A95C4:
  // stw r7,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r7));
  // b 0x821a964c
  goto loc_821A964C;
loc_821A95CC:
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a9610
  if (cr6.eq) goto loc_821A9610;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge cr6,0x821a95f8
  if (!cr6.lt) goto loc_821A95F8;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A95F8:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addic. r10,r10,1
  ca = uint32_t(r10) > 4294967294;
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blelr
  if (!cr0.gt) return r3;
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // blr
  return r3;
loc_821A9610:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a9644
  if (cr6.lt) goto loc_821A9644;
  // beq cr6,0x821a963c
  if (cr6.eq) goto loc_821A963C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a964c
  if (!cr6.lt) goto loc_821A964C;
loc_821A9634:
  // mr r11,r7
  r11 = r7;
  // b 0x821a9650
  goto loc_821A9650;
loc_821A963C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821a9648
  goto loc_821A9648;
loc_821A9644:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821A9648:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821A964C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821A9650:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821A9658: lifted from generated/ by lift.py.
uint64_t f821A9658(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821A9658 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r9,36(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // addi r7,r11,52
  r7 = uint64_t(int64_t(int64_t(r11) + 52));
  // stw r7,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r7));
  // addi r7,r11,56
  r7 = uint64_t(int64_t(int64_t(r11) + 56));
  // stw r7,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r7));
  // bne cr6,0x821a9704
  if (!cr6.eq) goto loc_821A9704;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a96cc
  if (cr6.eq) goto loc_821A96CC;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // ble cr6,0x821a96b0
  if (!cr6.gt) goto loc_821A96B0;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A96B0:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bgelr cr6
  if (!cr6.lt) return r3;
  // li r10,-5
  r10 = uint64_t(int64_t(-5));
  // b 0x821a9748
  goto loc_821A9748;
loc_821A96CC:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a9774
  if (cr6.lt) goto loc_821A9774;
  // beq cr6,0x821a96fc
  if (cr6.eq) goto loc_821A96FC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a978c
  if (!cr6.lt) goto loc_821A978C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821a978c
  goto loc_821A978C;
loc_821A96FC:
  // stw r3,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r3));
  // b 0x821a978c
  goto loc_821A978C;
loc_821A9704:
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a9750
  if (cr6.eq) goto loc_821A9750;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bge cr6,0x821a9730
  if (!cr6.lt) goto loc_821A9730;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9730:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r10,5
  r10 = uint64_t(int64_t(5));
loc_821A9748:
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blr
  return r3;
loc_821A9750:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a9784
  if (cr6.lt) goto loc_821A9784;
  // beq cr6,0x821a977c
  if (cr6.eq) goto loc_821A977C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a978c
  if (!cr6.lt) goto loc_821A978C;
loc_821A9774:
  // mr r11,r3
  r11 = r3;
  // b 0x821a9790
  goto loc_821A9790;
loc_821A977C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821a9788
  goto loc_821A9788;
loc_821A9784:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821A9788:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821A978C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821A9790:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821A9798: lifted from generated/ by lift.py.
uint64_t f821A9798(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821A9798 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r9,36(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // addi r7,r11,60
  r7 = uint64_t(int64_t(int64_t(r11) + 60));
  // stw r7,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r7));
  // addi r7,r11,64
  r7 = uint64_t(int64_t(int64_t(r11) + 64));
  // stw r7,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r7));
  // bne cr6,0x821a9844
  if (!cr6.eq) goto loc_821A9844;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a980c
  if (cr6.eq) goto loc_821A980C;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // ble cr6,0x821a97f0
  if (!cr6.gt) goto loc_821A97F0;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A97F0:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bgelr cr6
  if (!cr6.lt) return r3;
  // li r10,-5
  r10 = uint64_t(int64_t(-5));
  // b 0x821a9888
  goto loc_821A9888;
loc_821A980C:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a98b4
  if (cr6.lt) goto loc_821A98B4;
  // beq cr6,0x821a983c
  if (cr6.eq) goto loc_821A983C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a98cc
  if (!cr6.lt) goto loc_821A98CC;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821a98cc
  goto loc_821A98CC;
loc_821A983C:
  // stw r3,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r3));
  // b 0x821a98cc
  goto loc_821A98CC;
loc_821A9844:
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a9890
  if (cr6.eq) goto loc_821A9890;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bge cr6,0x821a9870
  if (!cr6.lt) goto loc_821A9870;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9870:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r10,5
  r10 = uint64_t(int64_t(5));
loc_821A9888:
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blr
  return r3;
loc_821A9890:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a98c4
  if (cr6.lt) goto loc_821A98C4;
  // beq cr6,0x821a98bc
  if (cr6.eq) goto loc_821A98BC;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a98cc
  if (!cr6.lt) goto loc_821A98CC;
loc_821A98B4:
  // mr r11,r3
  r11 = r3;
  // b 0x821a98d0
  goto loc_821A98D0;
loc_821A98BC:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821a98c8
  goto loc_821A98C8;
loc_821A98C4:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821A98C8:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821A98CC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821A98D0:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821A98D8: lifted from generated/ by lift.py.
uint64_t f821A98D8(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821A98D8 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r9,36(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // addi r7,r11,68
  r7 = uint64_t(int64_t(int64_t(r11) + 68));
  // stw r7,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r7));
  // addi r7,r11,72
  r7 = uint64_t(int64_t(int64_t(r11) + 72));
  // stw r7,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r7));
  // bne cr6,0x821a9984
  if (!cr6.eq) goto loc_821A9984;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a994c
  if (cr6.eq) goto loc_821A994C;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // ble cr6,0x821a9930
  if (!cr6.gt) goto loc_821A9930;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9930:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bgelr cr6
  if (!cr6.lt) return r3;
  // li r10,-5
  r10 = uint64_t(int64_t(-5));
  // b 0x821a99c8
  goto loc_821A99C8;
loc_821A994C:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a99f4
  if (cr6.lt) goto loc_821A99F4;
  // beq cr6,0x821a997c
  if (cr6.eq) goto loc_821A997C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a9a0c
  if (!cr6.lt) goto loc_821A9A0C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821a9a0c
  goto loc_821A9A0C;
loc_821A997C:
  // stw r3,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r3));
  // b 0x821a9a0c
  goto loc_821A9A0C;
loc_821A9984:
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a99d0
  if (cr6.eq) goto loc_821A99D0;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bge cr6,0x821a99b0
  if (!cr6.lt) goto loc_821A99B0;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A99B0:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r10,5
  r10 = uint64_t(int64_t(5));
loc_821A99C8:
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blr
  return r3;
loc_821A99D0:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a9a04
  if (cr6.lt) goto loc_821A9A04;
  // beq cr6,0x821a99fc
  if (cr6.eq) goto loc_821A99FC;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a9a0c
  if (!cr6.lt) goto loc_821A9A0C;
loc_821A99F4:
  // mr r11,r3
  r11 = r3;
  // b 0x821a9a10
  goto loc_821A9A10;
loc_821A99FC:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821a9a08
  goto loc_821A9A08;
loc_821A9A04:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821A9A08:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821A9A0C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821A9A10:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821A9A18: lifted from generated/ by lift.py.
uint64_t f821A9A18(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821A9A18 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r9,36(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // addi r7,r11,76
  r7 = uint64_t(int64_t(int64_t(r11) + 76));
  // stw r7,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r7));
  // addi r7,r11,80
  r7 = uint64_t(int64_t(int64_t(r11) + 80));
  // stw r7,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r7));
  // bne cr6,0x821a9ac4
  if (!cr6.eq) goto loc_821A9AC4;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a9a8c
  if (cr6.eq) goto loc_821A9A8C;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // ble cr6,0x821a9a70
  if (!cr6.gt) goto loc_821A9A70;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9A70:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bgelr cr6
  if (!cr6.lt) return r3;
  // li r10,-5
  r10 = uint64_t(int64_t(-5));
  // b 0x821a9b08
  goto loc_821A9B08;
loc_821A9A8C:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a9b34
  if (cr6.lt) goto loc_821A9B34;
  // beq cr6,0x821a9abc
  if (cr6.eq) goto loc_821A9ABC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a9b4c
  if (!cr6.lt) goto loc_821A9B4C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821a9b4c
  goto loc_821A9B4C;
loc_821A9ABC:
  // stw r3,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r3));
  // b 0x821a9b4c
  goto loc_821A9B4C;
loc_821A9AC4:
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a9b10
  if (cr6.eq) goto loc_821A9B10;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bge cr6,0x821a9af0
  if (!cr6.lt) goto loc_821A9AF0;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9AF0:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r10,5
  r10 = uint64_t(int64_t(5));
loc_821A9B08:
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blr
  return r3;
loc_821A9B10:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a9b44
  if (cr6.lt) goto loc_821A9B44;
  // beq cr6,0x821a9b3c
  if (cr6.eq) goto loc_821A9B3C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a9b4c
  if (!cr6.lt) goto loc_821A9B4C;
loc_821A9B34:
  // mr r11,r3
  r11 = r3;
  // b 0x821a9b50
  goto loc_821A9B50;
loc_821A9B3C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821a9b48
  goto loc_821A9B48;
loc_821A9B44:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821A9B48:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821A9B4C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821A9B50:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821A9B58: lifted from generated/ by lift.py.
uint64_t f821A9B58(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821A9B58 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r9,36(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // addi r7,r11,84
  r7 = uint64_t(int64_t(int64_t(r11) + 84));
  // stw r7,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r7));
  // addi r7,r11,88
  r7 = uint64_t(int64_t(int64_t(r11) + 88));
  // stw r7,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r7));
  // bne cr6,0x821a9c04
  if (!cr6.eq) goto loc_821A9C04;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a9bcc
  if (cr6.eq) goto loc_821A9BCC;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // ble cr6,0x821a9bb0
  if (!cr6.gt) goto loc_821A9BB0;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9BB0:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r10,-5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -5, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bgelr cr6
  if (!cr6.lt) return r3;
  // li r10,-5
  r10 = uint64_t(int64_t(-5));
  // b 0x821a9c48
  goto loc_821A9C48;
loc_821A9BCC:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a9c74
  if (cr6.lt) goto loc_821A9C74;
  // beq cr6,0x821a9bfc
  if (cr6.eq) goto loc_821A9BFC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a9c8c
  if (!cr6.lt) goto loc_821A9C8C;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821a9c8c
  goto loc_821A9C8C;
loc_821A9BFC:
  // stw r3,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r3));
  // b 0x821a9c8c
  goto loc_821A9C8C;
loc_821A9C04:
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821a9c50
  if (cr6.eq) goto loc_821A9C50;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // bge cr6,0x821a9c30
  if (!cr6.lt) goto loc_821A9C30;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9C30:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r10,5
  r10 = uint64_t(int64_t(5));
loc_821A9C48:
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blr
  return r3;
loc_821A9C50:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a9c84
  if (cr6.lt) goto loc_821A9C84;
  // beq cr6,0x821a9c7c
  if (cr6.eq) goto loc_821A9C7C;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a9c8c
  if (!cr6.lt) goto loc_821A9C8C;
loc_821A9C74:
  // mr r11,r3
  r11 = r3;
  // b 0x821a9c90
  goto loc_821A9C90;
loc_821A9C7C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821a9c88
  goto loc_821A9C88;
loc_821A9C84:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821A9C88:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821A9C8C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821A9C90:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821A9C98: lifted from generated/ by lift.py.
uint64_t f821A9C98(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821A9C98 (lifted)");
  uint64_t r3 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r3,r8
  r3 = r8;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x821a9d3c
  if (!cr6.eq) goto loc_821A9D3C;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821a9cfc
  if (cr6.eq) goto loc_821A9CFC;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,92(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 92);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble cr6,0x821a9cd8
  if (!cr6.gt) goto loc_821A9CD8;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9CD8:
  // lwz r10,92(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 92);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r10,92(r11)
  REX_STORE_U32(uint32_t(r11) + 92, uint32_t(r10));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,92(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 92);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bgelr cr6
  if (!cr6.lt) return r3;
  // stw r8,92(r11)
  REX_STORE_U32(uint32_t(r11) + 92, uint32_t(r8));
  // blr
  return r3;
loc_821A9CFC:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a9db8
  if (cr6.lt) goto loc_821A9DB8;
  // beq cr6,0x821a9d34
  if (cr6.eq) goto loc_821A9D34;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a9dd0
  if (!cr6.lt) goto loc_821A9DD0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821a9dd0
  goto loc_821A9DD0;
loc_821A9D34:
  // stw r8,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r8));
  // b 0x821a9dd0
  goto loc_821A9DD0;
loc_821A9D3C:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821a9d8c
  if (cr6.eq) goto loc_821A9D8C;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,92(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 92);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // bge cr6,0x821a9d64
  if (!cr6.lt) goto loc_821A9D64;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9D64:
  // lwz r10,92(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 92);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,92(r11)
  REX_STORE_U32(uint32_t(r11) + 92, uint32_t(r10));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,92(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 92);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // stw r10,92(r11)
  REX_STORE_U32(uint32_t(r11) + 92, uint32_t(r10));
  // blr
  return r3;
loc_821A9D8C:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a9dc8
  if (cr6.lt) goto loc_821A9DC8;
  // beq cr6,0x821a9dc0
  if (cr6.eq) goto loc_821A9DC0;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a9dd0
  if (!cr6.lt) goto loc_821A9DD0;
loc_821A9DB8:
  // mr r11,r8
  r11 = r8;
  // b 0x821a9dd4
  goto loc_821A9DD4;
loc_821A9DC0:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821a9dcc
  goto loc_821A9DCC;
loc_821A9DC8:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821A9DCC:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821A9DD0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821A9DD4:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821A9DE0: lifted from generated/ by lift.py.
uint64_t f821A9DE0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821A9DE0 (lifted)");
  uint64_t r3 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r3,r8
  r3 = r8;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x821a9e84
  if (!cr6.eq) goto loc_821A9E84;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821a9e44
  if (cr6.eq) goto loc_821A9E44;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,112(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 112);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble cr6,0x821a9e20
  if (!cr6.gt) goto loc_821A9E20;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9E20:
  // lwz r10,112(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 112);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r10,112(r11)
  REX_STORE_U32(uint32_t(r11) + 112, uint32_t(r10));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,112(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 112);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bgelr cr6
  if (!cr6.lt) return r3;
  // stw r8,112(r11)
  REX_STORE_U32(uint32_t(r11) + 112, uint32_t(r8));
  // blr
  return r3;
loc_821A9E44:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821a9f00
  if (cr6.lt) goto loc_821A9F00;
  // beq cr6,0x821a9e7c
  if (cr6.eq) goto loc_821A9E7C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821a9f18
  if (!cr6.lt) goto loc_821A9F18;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821a9f18
  goto loc_821A9F18;
loc_821A9E7C:
  // stw r8,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r8));
  // b 0x821a9f18
  goto loc_821A9F18;
loc_821A9E84:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821a9ed4
  if (cr6.eq) goto loc_821A9ED4;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,112(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 112);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // bge cr6,0x821a9eac
  if (!cr6.lt) goto loc_821A9EAC;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9EAC:
  // lwz r10,112(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 112);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,112(r11)
  REX_STORE_U32(uint32_t(r11) + 112, uint32_t(r10));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,112(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 112);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // stw r10,112(r11)
  REX_STORE_U32(uint32_t(r11) + 112, uint32_t(r10));
  // blr
  return r3;
loc_821A9ED4:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821a9f10
  if (cr6.lt) goto loc_821A9F10;
  // beq cr6,0x821a9f08
  if (cr6.eq) goto loc_821A9F08;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821a9f18
  if (!cr6.lt) goto loc_821A9F18;
loc_821A9F00:
  // mr r11,r8
  r11 = r8;
  // b 0x821a9f1c
  goto loc_821A9F1C;
loc_821A9F08:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821a9f14
  goto loc_821A9F14;
loc_821A9F10:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821A9F14:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821A9F18:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821A9F1C:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821A9F28: lifted from generated/ by lift.py.
uint64_t f821A9F28(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821A9F28 (lifted)");
  uint64_t r3 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r3,r8
  r3 = r8;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x821a9fcc
  if (!cr6.eq) goto loc_821A9FCC;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821a9f8c
  if (cr6.eq) goto loc_821A9F8C;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,108(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 108);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble cr6,0x821a9f68
  if (!cr6.gt) goto loc_821A9F68;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9F68:
  // lwz r10,108(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 108);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // stw r10,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r10));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,108(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 108);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bgelr cr6
  if (!cr6.lt) return r3;
  // stw r8,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r8));
  // blr
  return r3;
loc_821A9F8C:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821aa048
  if (cr6.lt) goto loc_821AA048;
  // beq cr6,0x821a9fc4
  if (cr6.eq) goto loc_821A9FC4;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821aa060
  if (!cr6.lt) goto loc_821AA060;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821aa060
  goto loc_821AA060;
loc_821A9FC4:
  // stw r8,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r8));
  // b 0x821aa060
  goto loc_821AA060;
loc_821A9FCC:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821aa01c
  if (cr6.eq) goto loc_821AA01C;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,108(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 108);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // bge cr6,0x821a9ff4
  if (!cr6.lt) goto loc_821A9FF4;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821A9FF4:
  // lwz r10,108(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 108);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r10));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,108(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 108);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // stw r10,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r10));
  // blr
  return r3;
loc_821AA01C:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821aa058
  if (cr6.lt) goto loc_821AA058;
  // beq cr6,0x821aa050
  if (cr6.eq) goto loc_821AA050;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821aa060
  if (!cr6.lt) goto loc_821AA060;
loc_821AA048:
  // mr r11,r8
  r11 = r8;
  // b 0x821aa064
  goto loc_821AA064;
loc_821AA050:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821aa05c
  goto loc_821AA05C;
loc_821AA058:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821AA05C:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821AA060:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821AA064:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821AA070: lifted from generated/ by lift.py.
uint64_t f821AA070(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821AA070 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // mr r3,r7
  r3 = r7;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r9,36(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // addi r6,r11,100
  r6 = uint64_t(int64_t(int64_t(r11) + 100));
  // stw r6,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r6));
  // addi r6,r11,104
  r6 = uint64_t(int64_t(int64_t(r11) + 104));
  // stw r6,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r6));
  // bne cr6,0x821aa124
  if (!cr6.eq) goto loc_821AA124;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821aa0ec
  if (cr6.eq) goto loc_821AA0EC;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -10, so);
  // ble cr6,0x821aa0cc
  if (!cr6.gt) goto loc_821AA0CC;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821AA0CC:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpwi cr6,r10,-10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -10, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bgelr cr6
  if (!cr6.lt) return r3;
  // li r10,-10
  r10 = uint64_t(int64_t(-10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blr
  return r3;
loc_821AA0EC:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821aa18c
  if (cr6.lt) goto loc_821AA18C;
  // beq cr6,0x821aa11c
  if (cr6.eq) goto loc_821AA11C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821aa1a4
  if (!cr6.lt) goto loc_821AA1A4;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821aa1a4
  goto loc_821AA1A4;
loc_821AA11C:
  // stw r7,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r7));
  // b 0x821aa1a4
  goto loc_821AA1A4;
loc_821AA124:
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r8,2
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 2, so);
  // beq cr6,0x821aa168
  if (cr6.eq) goto loc_821AA168;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // rlwinm r11,r8,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge cr6,0x821aa150
  if (!cr6.lt) goto loc_821AA150;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821AA150:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addic. r10,r10,1
  ca = uint32_t(r10) > 4294967294;
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // blelr
  if (!cr0.gt) return r3;
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // blr
  return r3;
loc_821AA168:
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821aa19c
  if (cr6.lt) goto loc_821AA19C;
  // beq cr6,0x821aa194
  if (cr6.eq) goto loc_821AA194;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821aa1a4
  if (!cr6.lt) goto loc_821AA1A4;
loc_821AA18C:
  // mr r11,r7
  r11 = r7;
  // b 0x821aa1a8
  goto loc_821AA1A8;
loc_821AA194:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821aa1a0
  goto loc_821AA1A0;
loc_821AA19C:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821AA1A0:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821AA1A4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821AA1A8:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821AA1B0: lifted from generated/ by lift.py.
uint64_t f821AA1B0(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821AA1B0 (lifted)");
  uint64_t r3 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r3,r8
  r3 = r8;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // bne cr6,0x821aa254
  if (!cr6.eq) goto loc_821AA254;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821aa214
  if (cr6.eq) goto loc_821AA214;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,96(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 96);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // ble cr6,0x821aa1f0
  if (!cr6.gt) goto loc_821AA1F0;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821AA1F0:
  // lwz r10,96(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 96);
  // addi r10,r10,-5
  r10 = uint64_t(int64_t(int64_t(r10) + -5));
  // stw r10,96(r11)
  REX_STORE_U32(uint32_t(r11) + 96, uint32_t(r10));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,96(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 96);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bgelr cr6
  if (!cr6.lt) return r3;
  // stw r8,96(r11)
  REX_STORE_U32(uint32_t(r11) + 96, uint32_t(r8));
  // blr
  return r3;
loc_821AA214:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r11,732(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 732);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821aa2d0
  if (cr6.lt) goto loc_821AA2D0;
  // beq cr6,0x821aa24c
  if (cr6.eq) goto loc_821AA24C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821aa2e8
  if (!cr6.lt) goto loc_821AA2E8;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r11));
  // b 0x821aa2e8
  goto loc_821AA2E8;
loc_821AA24C:
  // stw r8,732(r10)
  REX_STORE_U32(uint32_t(r10) + 732, uint32_t(r8));
  // b 0x821aa2e8
  goto loc_821AA2E8;
loc_821AA254:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bnelr cr6
  if (!cr6.eq) return r3;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821aa2a4
  if (cr6.eq) goto loc_821AA2A4;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,96(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 96);
  // cmpwi cr6,r10,100
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 100, so);
  // bge cr6,0x821aa27c
  if (!cr6.lt) goto loc_821AA27C;
  // li r3,1
  r3 = uint64_t(int64_t(1));
loc_821AA27C:
  // lwz r10,96(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 96);
  // addi r10,r10,5
  r10 = uint64_t(int64_t(int64_t(r10) + 5));
  // stw r10,96(r11)
  REX_STORE_U32(uint32_t(r11) + 96, uint32_t(r10));
  // lwz r11,-31648(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31648);
  // lwz r10,96(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 96);
  // cmpwi cr6,r10,100
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 100, so);
  // blelr cr6
  if (!cr6.gt) return r3;
  // li r10,100
  r10 = uint64_t(int64_t(100));
  // stw r10,96(r11)
  REX_STORE_U32(uint32_t(r11) + 96, uint32_t(r10));
  // blr
  return r3;
loc_821AA2A4:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r9,732(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 732);
  // stw r9,736(r11)
  REX_STORE_U32(uint32_t(r11) + 736, uint32_t(r9));
  // lwz r11,-31648(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,732(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 732);
  // cmplwi cr6,r10,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 1, so);
  // blt cr6,0x821aa2e0
  if (cr6.lt) goto loc_821AA2E0;
  // beq cr6,0x821aa2d8
  if (cr6.eq) goto loc_821AA2D8;
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bge cr6,0x821aa2e8
  if (!cr6.lt) goto loc_821AA2E8;
loc_821AA2D0:
  // mr r11,r8
  r11 = r8;
  // b 0x821aa2ec
  goto loc_821AA2EC;
loc_821AA2D8:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821aa2e4
  goto loc_821AA2E4;
loc_821AA2E0:
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821AA2E4:
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
loc_821AA2E8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821AA2EC:
  // mr r3,r11
  r3 = r11;
  // blr
  return r3;
}

// sub_821AA500: lifted from generated/ by lift.py.
void f821AA500(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821AA500 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,580(r11)
  REX_STORE_U32(uint32_t(r11) + 580, uint32_t(r10));
  // lwz r11,9608(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // bne cr6,0x821aa524
  if (!cr6.eq) goto loc_821AA524;
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821AA524:
  // stw r11,9604(r3)
  REX_STORE_U32(uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_821AA530: lifted from generated/ by lift.py.
void f821AA530(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821AA530 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,580(r11)
  REX_STORE_U32(uint32_t(r11) + 580, uint32_t(r10));
  // lwz r11,9608(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aa558
  if (cr6.eq) goto loc_821AA558;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,9604(r3)
  REX_STORE_U32(uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
loc_821AA558:
  // stw r10,9604(r3)
  REX_STORE_U32(uint32_t(r3) + 9604, uint32_t(r10));
  // blr
  return;
}

// sub_821AA560: lifted from generated/ by lift.py.
lifted::Result f821AA560(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AA560 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82186568(base, LIFTED_ARGS(0x821AA57C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aa5b4
  if (cr6.eq) goto loc_821AA5B4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821aa5a4
  if (cr0.eq) goto loc_821AA5A4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA5A4:
  // li r9,4
  r9 = uint64_t(int64_t(4));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // b 0x821aa78c
  goto loc_821AA78C;
loc_821AA5B4:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AA5BC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aa754
  if (cr0.eq) goto loc_821AA754;
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821aa70c
  if (cr6.lt) goto loc_821AA70C;
  // beq cr6,0x821aa6d0
  if (cr6.eq) goto loc_821AA6D0;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821aa674
  if (cr6.lt) goto loc_821AA674;
  // beq cr6,0x821aa630
  if (cr6.eq) goto loc_821AA630;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821aa7a4
  if (!cr6.lt) goto loc_821AA7A4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821aa7a4
  if (!cr6.eq) goto loc_821AA7A4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AA608, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,5
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 5, so);
  // beq cr6,0x821aa620
  if (cr6.eq) goto loc_821AA620;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA620:
  // li r10,22
  r10 = uint64_t(int64_t(22));
  // li r9,23
  r9 = uint64_t(int64_t(23));
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // b 0x821aa65c
  goto loc_821AA65C;
loc_821AA630:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AA638, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821aa650
  if (cr6.eq) goto loc_821AA650;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA650:
  // li r10,18
  r10 = uint64_t(int64_t(18));
  // li r9,19
  r9 = uint64_t(int64_t(19));
  // li r8,4
  r8 = uint64_t(int64_t(4));
loc_821AA65C:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821aa7a4
  goto loc_821AA7A4;
loc_821AA674:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,592(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 592);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aa6c8
  if (cr6.eq) goto loc_821AA6C8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821aa6a0
  if (cr6.eq) goto loc_821AA6A0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA6A0:
  // li r10,14
  r10 = uint64_t(int64_t(14));
  // li r9,15
  r9 = uint64_t(int64_t(15));
  // li r8,3
  r8 = uint64_t(int64_t(3));
loc_821AA6AC:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821aa7a0
  goto loc_821AA7A0;
loc_821AA6C8:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // b 0x821aa7a0
  goto loc_821AA7A0;
loc_821AA6D0:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,592(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 592);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aa6c8
  if (cr6.eq) goto loc_821AA6C8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821aa6fc
  if (cr6.eq) goto loc_821AA6FC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA6FC:
  // li r10,10
  r10 = uint64_t(int64_t(10));
  // li r9,11
  r9 = uint64_t(int64_t(11));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // b 0x821aa6ac
  goto loc_821AA6AC;
loc_821AA70C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,584(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821aa72c
  if (!cr6.eq) goto loc_821AA72C;
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aa6c8
  if (cr6.eq) goto loc_821AA6C8;
loc_821AA72C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821aa744
  if (cr6.eq) goto loc_821AA744;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA744:
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // b 0x821aa6ac
  goto loc_821AA6AC;
loc_821AA754:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AA760, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aa7a4
  if (cr0.eq) goto loc_821AA7A4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // beq cr6,0x821aa780
  if (cr6.eq) goto loc_821AA780;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA780:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // li r3,2
  r3 = uint64_t(int64_t(2));
loc_821AA78C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821AA7A0:
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AA7A4, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AA7A4:
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

// sub_821AA7B8: lifted from generated/ by lift.py.
lifted::Result f821AA7B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AA7B8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82186568(base, LIFTED_ARGS(0x821AA7D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aa80c
  if (cr6.eq) goto loc_821AA80C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821aa7fc
  if (cr6.eq) goto loc_821AA7FC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA7FC:
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // b 0x821aa944
  goto loc_821AA944;
loc_821AA80C:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AA814, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aa90c
  if (cr0.eq) goto loc_821AA90C;
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821aa8d0
  if (cr6.lt) goto loc_821AA8D0;
  // beq cr6,0x821aa894
  if (cr6.eq) goto loc_821AA894;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821aa95c
  if (!cr6.lt) goto loc_821AA95C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aa88c
  if (cr6.eq) goto loc_821AA88C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x821aa864
  if (cr6.eq) goto loc_821AA864;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA864:
  // li r10,37
  r10 = uint64_t(int64_t(37));
  // li r9,38
  r9 = uint64_t(int64_t(38));
  // li r8,10
  r8 = uint64_t(int64_t(10));
loc_821AA870:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821aa958
  goto loc_821AA958;
loc_821AA88C:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // b 0x821aa958
  goto loc_821AA958;
loc_821AA894:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aa88c
  if (cr6.eq) goto loc_821AA88C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x821aa8c0
  if (cr6.eq) goto loc_821AA8C0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA8C0:
  // li r10,32
  r10 = uint64_t(int64_t(32));
  // li r9,33
  r9 = uint64_t(int64_t(33));
  // li r8,9
  r8 = uint64_t(int64_t(9));
  // b 0x821aa870
  goto loc_821AA870;
loc_821AA8D0:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,584(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aa88c
  if (cr6.eq) goto loc_821AA88C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,8
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 8, so);
  // beq cr6,0x821aa8fc
  if (cr6.eq) goto loc_821AA8FC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA8FC:
  // li r10,27
  r10 = uint64_t(int64_t(27));
  // li r9,28
  r9 = uint64_t(int64_t(28));
  // li r8,8
  r8 = uint64_t(int64_t(8));
  // b 0x821aa870
  goto loc_821AA870;
loc_821AA90C:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AA918, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aa95c
  if (cr0.eq) goto loc_821AA95C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821aa938
  if (cr0.eq) goto loc_821AA938;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA938:
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r3,2
  r3 = uint64_t(int64_t(2));
loc_821AA944:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821AA958:
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AA95C, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AA95C:
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

// sub_821AA970: lifted from generated/ by lift.py.
lifted::Result f821AA970(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AA970 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82186568(base, LIFTED_ARGS(0x821AA98C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aa9c4
  if (cr6.eq) goto loc_821AA9C4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821aa9b4
  if (cr6.eq) goto loc_821AA9B4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AA9B4:
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // b 0x821aaa8c
  goto loc_821AAA8C;
loc_821AA9C4:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AA9CC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aaa54
  if (cr0.eq) goto loc_821AAA54;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AA9DC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821aaa18
  if (cr6.lt) goto loc_821AAA18;
  // bne cr6,0x821aaaa4
  if (!cr6.eq) goto loc_821AAAA4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // beq cr6,0x821aaa08
  if (cr6.eq) goto loc_821AAA08;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAA08:
  // li r10,47
  r10 = uint64_t(int64_t(47));
  // li r9,48
  r9 = uint64_t(int64_t(48));
  // li r8,12
  r8 = uint64_t(int64_t(12));
  // b 0x821aaa3c
  goto loc_821AAA3C;
loc_821AAA18:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 11, so);
  // beq cr6,0x821aaa30
  if (cr6.eq) goto loc_821AAA30;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAA30:
  // li r10,42
  r10 = uint64_t(int64_t(42));
  // li r9,43
  r9 = uint64_t(int64_t(43));
  // li r8,11
  r8 = uint64_t(int64_t(11));
loc_821AAA3C:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821aaaa4
  goto loc_821AAAA4;
loc_821AAA54:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AAA60, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aaaa4
  if (cr0.eq) goto loc_821AAAA4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821aaa80
  if (cr0.eq) goto loc_821AAA80;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAA80:
  // li r9,13
  r9 = uint64_t(int64_t(13));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r3,2
  r3 = uint64_t(int64_t(2));
loc_821AAA8C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AAAA4, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AAAA4:
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

// sub_821AAAB8: lifted from generated/ by lift.py.
lifted::Result f821AAAB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AAAB8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82186568(base, LIFTED_ARGS(0x821AAAD4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aab0c
  if (cr6.eq) goto loc_821AAB0C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821aaafc
  if (cr6.eq) goto loc_821AAAFC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAAFC:
  // li r9,16
  r9 = uint64_t(int64_t(16));
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // b 0x821aac30
  goto loc_821AAC30;
loc_821AAB0C:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AAB14, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aabf8
  if (cr0.eq) goto loc_821AABF8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AAB24, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821aabbc
  if (cr6.lt) goto loc_821AABBC;
  // beq cr6,0x821aab94
  if (cr6.eq) goto loc_821AAB94;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821aab6c
  if (cr6.lt) goto loc_821AAB6C;
  // bne cr6,0x821aac48
  if (!cr6.eq) goto loc_821AAC48;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,16
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 16, so);
  // beq cr6,0x821aab5c
  if (cr6.eq) goto loc_821AAB5C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAB5C:
  // li r10,67
  r10 = uint64_t(int64_t(67));
  // li r9,68
  r9 = uint64_t(int64_t(68));
  // li r8,16
  r8 = uint64_t(int64_t(16));
  // b 0x821aabe0
  goto loc_821AABE0;
loc_821AAB6C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,15
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 15, so);
  // beq cr6,0x821aab84
  if (cr6.eq) goto loc_821AAB84;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAB84:
  // li r10,62
  r10 = uint64_t(int64_t(62));
  // li r9,63
  r9 = uint64_t(int64_t(63));
  // li r8,15
  r8 = uint64_t(int64_t(15));
  // b 0x821aabe0
  goto loc_821AABE0;
loc_821AAB94:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,14
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 14, so);
  // beq cr6,0x821aabac
  if (cr6.eq) goto loc_821AABAC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AABAC:
  // li r10,57
  r10 = uint64_t(int64_t(57));
  // li r9,58
  r9 = uint64_t(int64_t(58));
  // li r8,14
  r8 = uint64_t(int64_t(14));
  // b 0x821aabe0
  goto loc_821AABE0;
loc_821AABBC:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,13
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 13, so);
  // beq cr6,0x821aabd4
  if (cr6.eq) goto loc_821AABD4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AABD4:
  // li r10,52
  r10 = uint64_t(int64_t(52));
  // li r9,53
  r9 = uint64_t(int64_t(53));
  // li r8,13
  r8 = uint64_t(int64_t(13));
loc_821AABE0:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821aac48
  goto loc_821AAC48;
loc_821AABF8:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AAC04, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aac48
  if (cr0.eq) goto loc_821AAC48;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821aac24
  if (cr0.eq) goto loc_821AAC24;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAC24:
  // li r9,17
  r9 = uint64_t(int64_t(17));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r3,2
  r3 = uint64_t(int64_t(2));
loc_821AAC30:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AAC48, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AAC48:
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

// sub_821AAC60: lifted from generated/ by lift.py.
lifted::Result f821AAC60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AAC60 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82186568(base, LIFTED_ARGS(0x821AAC7C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aacb4
  if (cr6.eq) goto loc_821AACB4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821aaca4
  if (cr6.eq) goto loc_821AACA4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AACA4:
  // li r9,20
  r9 = uint64_t(int64_t(20));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // b 0x821aae60
  goto loc_821AAE60;
loc_821AACB4:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AACBC, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aae28
  if (cr0.eq) goto loc_821AAE28;
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821aadec
  if (cr6.lt) goto loc_821AADEC;
  // beq cr6,0x821aadac
  if (cr6.eq) goto loc_821AADAC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821aad68
  if (cr6.lt) goto loc_821AAD68;
  // beq cr6,0x821aad38
  if (cr6.eq) goto loc_821AAD38;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821aae78
  if (!cr6.lt) goto loc_821AAE78;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821aad30
  if (!cr6.eq) goto loc_821AAD30;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AAD08, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,21
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 21, so);
  // beq cr6,0x821aad20
  if (cr6.eq) goto loc_821AAD20;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAD20:
  // li r10,92
  r10 = uint64_t(int64_t(92));
  // li r9,93
  r9 = uint64_t(int64_t(93));
  // li r8,21
  r8 = uint64_t(int64_t(21));
  // b 0x821aad94
  goto loc_821AAD94;
loc_821AAD30:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // b 0x821aae74
  goto loc_821AAE74;
loc_821AAD38:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AAD40, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,20
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 20, so);
  // beq cr6,0x821aad58
  if (cr6.eq) goto loc_821AAD58;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAD58:
  // li r10,87
  r10 = uint64_t(int64_t(87));
  // li r9,88
  r9 = uint64_t(int64_t(88));
  // li r8,20
  r8 = uint64_t(int64_t(20));
  // b 0x821aad94
  goto loc_821AAD94;
loc_821AAD68:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AAD70, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,19
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 19, so);
  // beq cr6,0x821aad88
  if (cr6.eq) goto loc_821AAD88;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAD88:
  // li r10,82
  r10 = uint64_t(int64_t(82));
  // li r9,83
  r9 = uint64_t(int64_t(83));
  // li r8,19
  r8 = uint64_t(int64_t(19));
loc_821AAD94:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821aae78
  goto loc_821AAE78;
loc_821AADAC:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,18
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 18, so);
  // beq cr6,0x821aadc4
  if (cr6.eq) goto loc_821AADC4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AADC4:
  // li r10,77
  r10 = uint64_t(int64_t(77));
  // li r9,78
  r9 = uint64_t(int64_t(78));
  // li r8,18
  r8 = uint64_t(int64_t(18));
loc_821AADD0:
  // li r7,-1
  r7 = uint64_t(int64_t(-1));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // b 0x821aae74
  goto loc_821AAE74;
loc_821AADEC:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 596);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aad30
  if (cr6.eq) goto loc_821AAD30;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,17
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 17, so);
  // beq cr6,0x821aae18
  if (cr6.eq) goto loc_821AAE18;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAE18:
  // li r10,72
  r10 = uint64_t(int64_t(72));
  // li r9,73
  r9 = uint64_t(int64_t(73));
  // li r8,17
  r8 = uint64_t(int64_t(17));
  // b 0x821aadd0
  goto loc_821AADD0;
loc_821AAE28:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AAE34, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821aae78
  if (cr0.eq) goto loc_821AAE78;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821aae54
  if (cr0.eq) goto loc_821AAE54;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821AAE54:
  // li r9,21
  r9 = uint64_t(int64_t(21));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r3,2
  r3 = uint64_t(int64_t(2));
loc_821AAE60:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821AAE74:
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AAE78, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AAE78:
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

// sub_821AAE90: lifted from generated/ by lift.py.
uint64_t f821AAE90(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_821AAE90 (lifted)");
  uint64_t r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lha r11,44(r4)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r4) + 44))));
  // mr r10,r5
  r10 = r5;
  // mr r31,r3
  r31 = r3;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // add r11,r11,r4
  r11 = r11 + r4;
  // beq cr6,0x821aaed0
  if (cr6.eq) goto loc_821AAED0;
loc_821AAEBC:
  // lha r11,0(r11)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 0))));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r11,r11,r4
  r11 = r11 + r4;
  // bne 0x821aaebc
  if (!cr0.eq) goto loc_821AAEBC;
loc_821AAED0:
  // lha r11,2(r11)
  r11 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 2))));
  // li r5,28
  r5 = uint64_t(int64_t(28));
  // mr r3,r31
  r3 = r31;
  // mulli r11,r11,28
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(28))));
  // add r11,r11,r4
  r11 = r11 + r4;
  // addi r4,r11,52
  r4 = uint64_t(int64_t(int64_t(r11) + 52));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e825A7410(base, LIFTED_ARGS(0x821AAEEC, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821AAF08: lifted from generated/ by lift.py.
lifted::Result f821AAF08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AAF08 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AAF34, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DFF8(base, LIFTED_ARGS(0x821AAF3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,9616(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 9616);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821aaf58
  if (cr6.eq) goto loc_821AAF58;
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // bl 0x8211e258
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E258(base, LIFTED_ARGS(0x821AAF50, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,9608(r31)
  REX_STORE_U32(uint32_t(r31) + 9608, uint32_t(r11));
loc_821AAF58:
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

// sub_821AB038: lifted from generated/ by lift.py.
lifted::Result f821AB038(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AB038 (lifted)");
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
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // mr r30,r3
  r30 = r3;
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82186568(base, LIFTED_ARGS(0x821AB05C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // lwz r11,8(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ab09c
  if (cr6.eq) goto loc_821AB09C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31656(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // beq cr6,0x821ab084
  if (cr6.eq) goto loc_821AB084;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821AB084:
  // li r8,5
  r8 = uint64_t(int64_t(5));
  // li r9,24
  r9 = uint64_t(int64_t(24));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // b 0x821ab228
  goto loc_821AB228;
loc_821AB09C:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AB0A4, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821ab1ec
  if (cr0.eq) goto loc_821AB1EC;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AB0B4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31652(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,568(r11)
  REX_STORE_U32(uint32_t(r11) + 568, uint32_t(r10));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AB0E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AB0F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AB10C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AB120, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82105620(base, LIFTED_ARGS(0x821AB128, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // ori r11,r11,63600
  r11 = r11 | 63600;
  // lwzx r11,r3,r11
  r11 = REX_LOAD_U32(uint32_t(r3) + uint32_t(r11));
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r11,r11,40960
  r11 = r11 | 40960;
  // beq cr6,0x821ab150
  if (cr6.eq) goto loc_821AB150;
  // stwx r7,r31,r11
  REX_STORE_U32(uint32_t(r31) + uint32_t(r11), uint32_t(r7));
  // b 0x821ab158
  goto loc_821AB158;
loc_821AB150:
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stwx r10,r31,r11
  REX_STORE_U32(uint32_t(r31) + uint32_t(r11), uint32_t(r10));
loc_821AB158:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31656(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821ab170
  if (cr6.eq) goto loc_821AB170;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821AB170:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // stw r11,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r11));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lwz r11,13020(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 13020);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821ab1a0
  if (cr6.eq) goto loc_821AB1A0;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bne cr6,0x821ab238
  if (!cr6.eq) goto loc_821AB238;
  // li r9,10
  r9 = uint64_t(int64_t(10));
  // b 0x821ab1a4
  goto loc_821AB1A4;
loc_821AB1A0:
  // li r9,34
  r9 = uint64_t(int64_t(34));
loc_821AB1A4:
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // mr r8,r7
  r8 = r7;
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
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
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // b 0x821ab238
  goto loc_821AB238;
loc_821AB1EC:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31652(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31652);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821864C8(base, LIFTED_ARGS(0x821AB1F8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821ab238
  if (cr0.eq) goto loc_821AB238;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31656(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31656);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821ab218
  if (cr6.eq) goto loc_821AB218;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821AB218:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r9,25
  r9 = uint64_t(int64_t(25));
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
loc_821AB228:
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e821103F0(base, LIFTED_ARGS(0x821AB238, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AB238:
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

// sub_821AB250: lifted from generated/ by lift.py.
void f821AB250(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821AB250 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // lwz r11,13020(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 13020);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821ab270
  if (cr6.eq) goto loc_821AB270;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // bnelr cr6
  if (!cr6.eq) return;
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // b 0x821ab274
  goto loc_821AB274;
loc_821AB270:
  // li r9,17
  r9 = uint64_t(int64_t(17));
loc_821AB274:
  // addi r11,r3,204
  r11 = uint64_t(int64_t(int64_t(r3) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r6,604(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r6,r6,r10
  r6 = r6 + r10;
  // addi r6,r6,-1
  r6 = uint64_t(int64_t(int64_t(r6) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
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
  // stw r7,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r7));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // blr
  return;
}

// sub_821AB2C0: lifted from generated/ by lift.py.
lifted::Result f821AB2C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AB2C0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AB2E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // mr r3,r31
  r3 = r31;
  // bl 0x8211aba8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211ABA8(base, LIFTED_ARGS(0x821AB2F0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r6,r11,528
  r6 = uint64_t(int64_t(int64_t(r11) + 528));
  // bl 0x8211d930
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211D930(base, LIFTED_ARGS(0x821AB30C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // lwz r6,548(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 548);
  // bl 0x8211c740
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C740(base, LIFTED_ARGS(0x821AB324, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // lwz r7,556(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 556);
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AB340, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB350, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r10,-31648(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // mr r11,r3
  r11 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r31
  r3 = r31;
  // lwz r10,552(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 552);
  // stw r10,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB370, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r31
  r3 = r31;
  // stw r29,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r29));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB38C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r31
  r3 = r31;
  // stw r29,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r29));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB3A4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r31
  r3 = r31;
  // stw r28,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r28));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB3C0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r31
  r3 = r31;
  // stw r28,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r28));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB3D8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r31
  r3 = r31;
  // stw r29,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB3F0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // stw r29,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r29));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB408, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C7E8(base, LIFTED_ARGS(0x821AB418, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB428, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C7E8(base, LIFTED_ARGS(0x821AB438, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB448, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C7E8(base, LIFTED_ARGS(0x821AB458, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r10,560(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne 0x821ab488
  if (!cr0.eq) goto loc_821AB488;
  // li r10,5
  r10 = uint64_t(int64_t(5));
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stw r10,564(r11)
  REX_STORE_U32(uint32_t(r11) + 564, uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB480, r31)); r3 = o.r3; f1 = o.f1; }
  // stw r29,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r29));
  // b 0x821ab4cc
  goto loc_821AB4CC;
loc_821AB488:
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821ab4d0
  if (!cr6.eq) goto loc_821AB4D0;
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stw r10,564(r11)
  REX_STORE_U32(uint32_t(r11) + 564, uint32_t(r10));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AB4A8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,29
  r5 = uint64_t(int64_t(29));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // stw r28,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r28));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r11,752(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 752);
  // addi r6,r11,369
  r6 = uint64_t(int64_t(int64_t(r11) + 369));
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AB4CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AB4CC:
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
loc_821AB4D0:
  // stw r29,728(r11)
  REX_STORE_U32(uint32_t(r11) + 728, uint32_t(r29));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // addi r11,r11,708
  r11 = uint64_t(int64_t(int64_t(r11) + 708));
  // lwz r10,20(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 20);
  // addi r9,r10,1
  r9 = uint64_t(int64_t(int64_t(r10) + 1));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stwx r29,r10,r11
  REX_STORE_U32(uint32_t(r10) + uint32_t(r11), uint32_t(r29));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r29,756(r11)
  REX_STORE_U32(uint32_t(r11) + 756, uint32_t(r29));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r29,740(r11)
  REX_STORE_U32(uint32_t(r11) + 740, uint32_t(r29));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r28,748(r11)
  REX_STORE_U32(uint32_t(r11) + 748, uint32_t(r28));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r29,576(r11)
  REX_STORE_U32(uint32_t(r11) + 576, uint32_t(r29));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AB518: lifted from generated/ by lift.py.
lifted::Result f821AB518(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821AB518 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AB548, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ab570
  if (!cr6.eq) goto loc_821AB570;
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // stw r10,704(r11)
  REX_STORE_U32(uint32_t(r11) + 704, uint32_t(r10));
  // stw r9,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r9));
loc_821AB570:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821AB578, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // lwz r10,704(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 704);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // stw r10,704(r11)
  REX_STORE_U32(uint32_t(r11) + 704, uint32_t(r10));
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r11,704(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 704);
  // cmpwi cr6,r11,10
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 10, so);
  // bge cr6,0x821ab5a0
  if (!cr6.lt) goto loc_821AB5A0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_821AB5A0:
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

// sub_821AB5B8: lifted from generated/ by lift.py.
lifted::Result f821AB5B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821AB5B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AB5E8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ab670
  if (!cr6.eq) goto loc_821AB670;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,94
  r4 = uint64_t(int64_t(94));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AB614, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,94
  r4 = uint64_t(int64_t(94));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AB620, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,584(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821ab640
  if (!cr6.eq) goto loc_821AB640;
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ab668
  if (cr6.eq) goto loc_821AB668;
loc_821AB640:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AB65C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AB668, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AB668:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821AB670:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821AB67C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_821AB698: lifted from generated/ by lift.py.
lifted::Result f821AB698(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821AB698 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AB6C0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ab7d4
  if (!cr6.eq) goto loc_821AB7D4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ab728
  if (!cr0.eq) goto loc_821AB728;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,584(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821ab704
  if (!cr6.eq) goto loc_821AB704;
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ab77c
  if (cr6.eq) goto loc_821AB77C;
loc_821AB704:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AB720, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // b 0x821ab770
  goto loc_821AB770;
loc_821AB728:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ab738
  if (cr6.eq) goto loc_821AB738;
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821ab74c
  if (!cr6.eq) goto loc_821AB74C;
loc_821AB738:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // lwz r10,592(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 592);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821ab77c
  if (cr6.eq) goto loc_821AB77C;
loc_821AB74C:
  // addi r30,r11,17
  r30 = uint64_t(int64_t(int64_t(r11) + 17));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AB76C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
loc_821AB770:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AB778, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821ab7cc
  goto loc_821AB7CC;
loc_821AB77C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,41
  r4 = uint64_t(int64_t(41));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AB798, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,41
  r4 = uint64_t(int64_t(41));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821AB7A4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,42
  r4 = uint64_t(int64_t(42));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AB7C0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,42
  r4 = uint64_t(int64_t(42));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821AB7CC, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AB7CC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821AB7D4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821AB7E0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AB7F0: lifted from generated/ by lift.py.
lifted::Result f821AB7F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821AB7F0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AB820, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ab854
  if (!cr6.eq) goto loc_821AB854;
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
  // lwz r3,7152(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 7152);
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8209D0C8(base, LIFTED_ARGS(0x821AB84C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821AB854:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821AB860, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32150
  r11 = uint64_t(int64_t(-2106982400));
  // lis r10,-32150
  r10 = uint64_t(int64_t(-2106982400));
  // lwz r11,20708(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20708);
  // lwz r10,20700(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 20700);
  // or r11,r11,r10
  r11 = r11 | r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
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

// sub_821AB898: lifted from generated/ by lift.py.
lifted::Result f821AB898(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AB898 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
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
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r25,r3
  r25 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AB8C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r25)
  r11 = REX_LOAD_U32(uint32_t(r25) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821abbe0
  if (!cr6.eq) goto loc_821ABBE0;
  // lis r26,-32146
  r26 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31652);
  // lwz r28,4(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r30,0(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r4,r28,17
  r4 = uint64_t(int64_t(int64_t(r28) + 17));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821AB8F0, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // bne cr6,0x821ab93c
  if (!cr6.eq) goto loc_821AB93C;
  // lwz r11,-31648(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31648);
  // lwz r10,584(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821ab918
  if (!cr6.eq) goto loc_821AB918;
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ab9c8
  if (cr6.eq) goto loc_821AB9C8;
loc_821AB918:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AB934, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,17
  r4 = uint64_t(int64_t(17));
  // b 0x821ab9bc
  goto loc_821AB9BC;
loc_821AB93C:
  // cmpwi cr6,r30,1
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 1, so);
  // beq cr6,0x821ab988
  if (cr6.eq) goto loc_821AB988;
  // cmpwi cr6,r30,2
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 2, so);
  // beq cr6,0x821ab988
  if (cr6.eq) goto loc_821AB988;
  // cmpwi cr6,r30,4
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 4, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x821ab9a8
  if (!cr6.eq) goto loc_821AB9A8;
  // lwz r11,-31648(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31648);
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ab9d8
  if (!cr6.eq) goto loc_821AB9D8;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AB980, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // b 0x821ab9bc
  goto loc_821AB9BC;
loc_821AB988:
  // lwz r11,-31648(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31648);
  // lwz r11,592(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 592);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ab9c8
  if (cr6.eq) goto loc_821AB9C8;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
loc_821AB9A8:
  // addi r29,r30,17
  r29 = uint64_t(int64_t(int64_t(r30) + 17));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r29
  r4 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AB9B8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
loc_821AB9BC:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AB9C4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821aba18
  goto loc_821ABA18;
loc_821AB9C8:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
loc_821AB9D8:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,41
  r4 = uint64_t(int64_t(41));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AB9E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,41
  r4 = uint64_t(int64_t(41));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821AB9F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,42
  r4 = uint64_t(int64_t(42));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABA0C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,42
  r4 = uint64_t(int64_t(42));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABA18, r31)); r3 = o.r3; f1 = o.f1; }
loc_821ABA18:
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABA24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABA30, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABA3C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABA48, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31648);
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x821aba64
  if (!cr6.eq) goto loc_821ABA64;
  // lwz r10,564(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 564);
  // addi r10,r10,-1
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  // cmpw cr6,r10,r28
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r28), so);
  // beq cr6,0x821aba7c
  if (cr6.eq) goto loc_821ABA7C;
loc_821ABA64:
  // cmpwi cr6,r28,0
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // bne cr6,0x821abaac
  if (!cr6.eq) goto loc_821ABAAC;
  // lwz r11,564(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 564);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmpw cr6,r11,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r30), so);
  // bne cr6,0x821abaac
  if (!cr6.eq) goto loc_821ABAAC;
loc_821ABA7C:
  // mulli r11,r30,40
  r11 = uint64_t(int64_t(static_cast<int64_t>(r30 * static_cast<uint64_t>(40))));
  // extsw r11,r11
  r11 = uint64_t(int64_t(int32_t(r11)));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r31
  r3 = r31;
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ABAA4, r31)); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // b 0x821abae8
  goto loc_821ABAE8;
loc_821ABAAC:
  // lwz r11,-31652(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31652);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821abac4
  if (!cr6.eq) goto loc_821ABAC4;
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // b 0x821abad0
  goto loc_821ABAD0;
loc_821ABAC4:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x821abae8
  if (!cr6.eq) goto loc_821ABAE8;
  // li r4,27
  r4 = uint64_t(int64_t(27));
loc_821ABAD0:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABAE8, r31)); r3 = o.r3; f1 = o.f1; }
loc_821ABAE8:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ABAF8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // stw r30,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r30));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ABB10, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // stw r30,108(r11)
  REX_STORE_U32(uint32_t(r11) + 108, uint32_t(r30));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABB24, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABB40, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x821abb6c
  if (!cr6.eq) goto loc_821ABB6C;
  // lwz r11,-31648(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31648);
  // lwz r10,584(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821abbac
  if (!cr6.eq) goto loc_821ABBAC;
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821abbac
  if (!cr6.eq) goto loc_821ABBAC;
  // li r6,911
  r6 = uint64_t(int64_t(911));
  // b 0x821abbb8
  goto loc_821ABBB8;
loc_821ABB6C:
  // cmpwi cr6,r30,2
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 2, so);
  // bne cr6,0x821abb8c
  if (!cr6.eq) goto loc_821ABB8C;
  // lwz r11,-31648(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31648);
  // lwz r11,592(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 592);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821abbac
  if (!cr6.eq) goto loc_821ABBAC;
  // li r6,908
  r6 = uint64_t(int64_t(908));
  // b 0x821abbb8
  goto loc_821ABBB8;
loc_821ABB8C:
  // cmpwi cr6,r30,1
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 1, so);
  // bne cr6,0x821abbac
  if (!cr6.eq) goto loc_821ABBAC;
  // lwz r11,-31648(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31648);
  // lwz r11,592(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 592);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821abbac
  if (!cr6.eq) goto loc_821ABBAC;
  // li r6,907
  r6 = uint64_t(int64_t(907));
  // b 0x821abbb8
  goto loc_821ABBB8;
loc_821ABBAC:
  // lwz r11,-31652(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r6,r11,866
  r6 = uint64_t(int64_t(int64_t(r11) + 866));
loc_821ABBB8:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821ABBC8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821ABBD8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r25)
  REX_STORE_U32(uint32_t(r25) + 0, uint32_t(r11));
loc_821ABBE0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821ABBEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lfd f31,-72(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821ABC00: lifted from generated/ by lift.py.
lifted::Result f821ABC00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821ABC00 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ABC28, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821abd14
  if (!cr6.eq) goto loc_821ABD14;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821abc90
  if (cr6.lt) goto loc_821ABC90;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821abd0c
  if (!cr6.lt) goto loc_821ABD0C;
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r10,-31648(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31648);
  // mr r3,r31
  r3 = r31;
  // lwz r10,588(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 588);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821abcd0
  if (cr6.eq) goto loc_821ABCD0;
  // addi r30,r11,56
  r30 = uint64_t(int64_t(int64_t(r11) + 56));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABC88, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // b 0x821abcc4
  goto loc_821ABCC4;
loc_821ABC90:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // mr r3,r31
  r3 = r31;
  // lwz r11,584(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821abcd0
  if (cr6.eq) goto loc_821ABCD0;
  // li r4,56
  r4 = uint64_t(int64_t(56));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABCC0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,56
  r4 = uint64_t(int64_t(56));
loc_821ABCC4:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821ABCCC, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821abd0c
  goto loc_821ABD0C;
loc_821ABCD0:
  // li r4,80
  r4 = uint64_t(int64_t(80));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABCD8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,80
  r4 = uint64_t(int64_t(80));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABCE4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABD00, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABD0C, r31)); r3 = o.r3; f1 = o.f1; }
loc_821ABD0C:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821ABD14:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821ABD20, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821ABD30: lifted from generated/ by lift.py.
lifted::Result f821ABD30(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821ABD30 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ABD58, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821abda8
  if (!cr6.eq) goto loc_821ABDA8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r31,r11,56
  r31 = uint64_t(int64_t(int64_t(r11) + 56));
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABD94, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821ABDA0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821ABDA8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821ABDB4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821ABDC0: lifted from generated/ by lift.py.
lifted::Result f821ABDC0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821ABDC0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ABDE8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821abedc
  if (!cr6.eq) goto loc_821ABEDC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821abe44
  if (!cr0.eq) goto loc_821ABE44;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 596);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821abe74
  if (cr6.eq) goto loc_821ABE74;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,56
  r4 = uint64_t(int64_t(56));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABE3C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,56
  r4 = uint64_t(int64_t(56));
  // b 0x821abecc
  goto loc_821ABECC;
loc_821ABE44:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821abeb8
  if (!cr6.eq) goto loc_821ABEB8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821abe74
  if (!cr6.eq) goto loc_821ABE74;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,60
  r4 = uint64_t(int64_t(60));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABE6C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,60
  r4 = uint64_t(int64_t(60));
  // b 0x821abecc
  goto loc_821ABECC;
loc_821ABE74:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,80
  r4 = uint64_t(int64_t(80));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABE80, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,80
  r4 = uint64_t(int64_t(80));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABE8C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABEA8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,81
  r4 = uint64_t(int64_t(81));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABEB4, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821abed4
  goto loc_821ABED4;
loc_821ABEB8:
  // addi r30,r11,56
  r30 = uint64_t(int64_t(int64_t(r11) + 56));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABEC8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
loc_821ABECC:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821ABED4, r31)); r3 = o.r3; f1 = o.f1; }
loc_821ABED4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821ABEDC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821ABEE8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821ABEF8: lifted from generated/ by lift.py.
lifted::Result f821ABEF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821ABEF8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ABF20, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821abfbc
  if (!cr6.eq) goto loc_821ABFBC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // mr r3,r31
  r3 = r31;
  // lwz r30,0(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r29,r30,17
  r29 = uint64_t(int64_t(int64_t(r30) + 17));
  // mr r4,r29
  r4 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABF5C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821ABF68, r31)); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
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
  // beq cr6,0x821abfa0
  if (cr6.eq) goto loc_821ABFA0;
  // addi r30,r30,102
  r30 = uint64_t(int64_t(int64_t(r30) + 102));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABF90, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821ABF9C, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821abfb4
  goto loc_821ABFB4;
loc_821ABFA0:
  // li r4,106
  r4 = uint64_t(int64_t(106));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821ABFA8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,106
  r4 = uint64_t(int64_t(106));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821ABFB4, r31)); r3 = o.r3; f1 = o.f1; }
loc_821ABFB4:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_821ABFBC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821ABFC8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821ABFD8: lifted from generated/ by lift.py.
lifted::Result f821ABFD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821ABFD8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC000, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ac050
  if (!cr6.eq) goto loc_821AC050;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r31,r11,134
  r31 = uint64_t(int64_t(int64_t(r11) + 134));
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AC03C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AC048, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821AC050:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821AC05C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AC068: lifted from generated/ by lift.py.
lifted::Result f821AC068(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC068 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC090, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ac0e0
  if (!cr6.eq) goto loc_821AC0E0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r31,r11,149
  r31 = uint64_t(int64_t(int64_t(r11) + 149));
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AC0CC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AC0D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821AC0E0:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821AC0EC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AC0F8: lifted from generated/ by lift.py.
lifted::Result f821AC0F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC0F8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC120, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ac170
  if (!cr6.eq) goto loc_821AC170;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r31,r11,159
  r31 = uint64_t(int64_t(int64_t(r11) + 159));
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AC15C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AC168, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821AC170:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821AC17C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AC188: lifted from generated/ by lift.py.
lifted::Result f821AC188(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC188 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC1B0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ac200
  if (!cr6.eq) goto loc_821AC200;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r31,r11,174
  r31 = uint64_t(int64_t(int64_t(r11) + 174));
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AC1EC, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AC1F8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821AC200:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821AC20C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AC218: lifted from generated/ by lift.py.
lifted::Result f821AC218(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC218 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC240, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ac290
  if (!cr6.eq) goto loc_821AC290;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r31,r11,186
  r31 = uint64_t(int64_t(int64_t(r11) + 186));
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AC27C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AC288, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821AC290:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821AC29C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AC2A8: lifted from generated/ by lift.py.
lifted::Result f821AC2A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC2A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC2D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ac320
  if (!cr6.eq) goto loc_821AC320;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // mr r3,r30
  r3 = r30;
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r31,r11,194
  r31 = uint64_t(int64_t(int64_t(r11) + 194));
  // mr r4,r31
  r4 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AC30C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r31
  r4 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AC318, r31)); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r11));
loc_821AC320:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211E1B8(base, LIFTED_ARGS(0x821AC32C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AC338: lifted from generated/ by lift.py.
lifted::Result f821AC338(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC338 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC360, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821AC3D0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,871
  r6 = uint64_t(int64_t(871));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AC3E4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821AC3F4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821AC408: lifted from generated/ by lift.py.
lifted::Result f821AC408(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC408 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC430, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821AC4A0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,872
  r6 = uint64_t(int64_t(872));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AC4B4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821AC4C4, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821AC4D8: lifted from generated/ by lift.py.
lifted::Result f821AC4D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC4D8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC500, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821AC570, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,873
  r6 = uint64_t(int64_t(873));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AC584, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821AC594, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821AC5A8: lifted from generated/ by lift.py.
lifted::Result f821AC5A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC5A8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC5D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821AC644, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,874
  r6 = uint64_t(int64_t(874));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AC658, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821AC668, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ac68c
  if (cr6.eq) goto loc_821AC68C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,59
  r4 = uint64_t(int64_t(59));
  // lwz r11,-31776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82180108(base, LIFTED_ARGS(0x821AC68C, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AC68C:
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

// sub_821AC6A8: lifted from generated/ by lift.py.
lifted::Result f821AC6A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC6A8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC6D4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821AC744, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,875
  r6 = uint64_t(int64_t(875));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AC758, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821AC768, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ac78c
  if (cr6.eq) goto loc_821AC78C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,61
  r4 = uint64_t(int64_t(61));
  // lwz r11,-31776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82180108(base, LIFTED_ARGS(0x821AC78C, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AC78C:
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

// sub_821AC7A8: lifted from generated/ by lift.py.
lifted::Result f821AC7A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC7A8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC7D0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821AC840, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,876
  r6 = uint64_t(int64_t(876));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AC854, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821AC864, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821AC878: lifted from generated/ by lift.py.
lifted::Result f821AC878(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC878 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC8A0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821AC910, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,877
  r6 = uint64_t(int64_t(877));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AC924, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821AC934, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821AC948: lifted from generated/ by lift.py.
lifted::Result f821AC948(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821AC948 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AC970, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821AC9E0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,878
  r6 = uint64_t(int64_t(878));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821AC9F4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821ACA04, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821ACA18: lifted from generated/ by lift.py.
lifted::Result f821ACA18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821ACA18 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ACA44, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821ACAB4, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,879
  r6 = uint64_t(int64_t(879));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821ACAC8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821ACAD8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821acafc
  if (cr6.eq) goto loc_821ACAFC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,63
  r4 = uint64_t(int64_t(63));
  // lwz r11,-31776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x82180108
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e82180108(base, LIFTED_ARGS(0x821ACAFC, r31)); r3 = o.r3; f1 = o.f1; }
loc_821ACAFC:
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

// sub_821ACB18: lifted from generated/ by lift.py.
lifted::Result f821ACB18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821ACB18 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ACB40, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821ACBAC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,880
  r6 = uint64_t(int64_t(880));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821ACBC0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821ACBD0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821ACBE8: lifted from generated/ by lift.py.
lifted::Result f821ACBE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821ACBE8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ACC10, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821ACC7C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,881
  r6 = uint64_t(int64_t(881));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821ACC90, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821ACCA0, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821ACCB8: lifted from generated/ by lift.py.
lifted::Result f821ACCB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821ACCB8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ACCE0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821ACD4C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,882
  r6 = uint64_t(int64_t(882));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821ACD60, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821ACD70, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821ACD88: lifted from generated/ by lift.py.
lifted::Result f821ACD88(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821ACD88 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ACDB0, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,2
  r9 = uint64_t(int64_t(2));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821ACE20, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,883
  r6 = uint64_t(int64_t(883));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821ACE34, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821ACE44, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821ACE58: lifted from generated/ by lift.py.
lifted::Result f821ACE58(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821ACE58 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ACE80, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r10,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r10));
  // stw r10,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r10));
  // stw r10,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r10));
  // stw r10,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r10));
  // stw r10,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r10));
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // stw r10,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r10));
  // stw r10,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r10));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // stw r10,732(r11)
  REX_STORE_U32(uint32_t(r11) + 732, uint32_t(r10));
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C570(base, LIFTED_ARGS(0x821ACEEC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,884
  r6 = uint64_t(int64_t(884));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C1F8(base, LIFTED_ARGS(0x821ACF00, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c5f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C5F8(base, LIFTED_ARGS(0x821ACF10, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821ACF28: lifted from generated/ by lift.py.
lifted::Result f821ACF28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821ACF28 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ACF50, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r29
  r3 = r29;
  // lwz r28,0(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D658(base, LIFTED_ARGS(0x821ACF64, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,104(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 104);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x821acfa4
  if (!cr6.gt) goto loc_821ACFA4;
loc_821ACF78:
  // lwz r11,100(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 100);
  // mr r3,r29
  r3 = r29;
  // add r4,r11,r31
  r4 = r11 + r31;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D658(base, LIFTED_ARGS(0x821ACF88, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r3,r28
  r3 = r28;
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211CC50(base, LIFTED_ARGS(0x821ACF94, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,104(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 104);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpw cr6,r31,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r31), int32_t(r11), so);
  // blt cr6,0x821acf78
  if (cr6.lt) goto loc_821ACF78;
loc_821ACFA4:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AD2E0: lifted from generated/ by lift.py.
uint64_t f821AD2E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821AD2E0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AD310, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r31,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 1, so);
  // blt cr6,0x821ad370
  if (cr6.lt) goto loc_821AD370;
  // beq cr6,0x821ad34c
  if (cr6.eq) goto loc_821AD34C;
  // cmplwi cr6,r31,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 3, so);
  // bge cr6,0x821ad398
  if (!cr6.lt) goto loc_821AD398;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AD344, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // b 0x821ad390
  goto loc_821AD390;
loc_821AD34C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AD368, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x821ad390
  goto loc_821AD390;
loc_821AD370:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AD38C, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
loc_821AD390:
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DDE0(base, LIFTED_ARGS(0x821AD398, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AD398:
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

// sub_821AD3B0: lifted from generated/ by lift.py.
lifted::Result f821AD3B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821AD3B0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AD3E0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmplwi cr6,r31,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 1, so);
  // blt cr6,0x821ad408
  if (cr6.lt) goto loc_821AD408;
  // beq cr6,0x821ad400
  if (cr6.eq) goto loc_821AD400;
  // cmplwi cr6,r31,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 3, so);
  // bge cr6,0x821ad414
  if (!cr6.lt) goto loc_821AD414;
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // b 0x821ad40c
  goto loc_821AD40C;
loc_821AD400:
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // b 0x821ad40c
  goto loc_821AD40C;
loc_821AD408:
  // li r4,1
  r4 = uint64_t(int64_t(1));
loc_821AD40C:
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821AD414, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AD414:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DAC8(base, LIFTED_ARGS(0x821AD430, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211DF20(base, LIFTED_ARGS(0x821AD43C, r31)); r3 = o.r3; f1 = o.f1; }
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

// sub_821AD458: lifted from generated/ by lift.py.
uint64_t f821AD458(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AD458 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AD484, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r10,592(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 592);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821ad4c8
  if (!cr6.eq) goto loc_821AD4C8;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A5B8(base, LIFTED_ARGS(0x821AD4B0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A5B8(base, LIFTED_ARGS(0x821AD4C4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
loc_821AD4C8:
  // lwz r10,584(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821ad4f8
  if (!cr6.eq) goto loc_821AD4F8;
  // lwz r10,588(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821ad4f8
  if (!cr6.eq) goto loc_821AD4F8;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A5B8(base, LIFTED_ARGS(0x821AD4F4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
loc_821AD4F8:
  // lwz r11,560(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821ad518
  if (!cr6.eq) goto loc_821AD518;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,14
  r4 = uint64_t(int64_t(14));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A5B8(base, LIFTED_ARGS(0x821AD518, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AD518:
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

// sub_821AD530: lifted from generated/ by lift.py.
uint64_t f821AD530(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AD530 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AD55C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
  // lwz r10,584(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 584);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821ad58c
  if (!cr6.eq) goto loc_821AD58C;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A5B8(base, LIFTED_ARGS(0x821AD588, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31648);
loc_821AD58C:
  // lwz r11,588(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821ad5c0
  if (!cr6.eq) goto loc_821AD5C0;
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A5B8(base, LIFTED_ARGS(0x821AD5AC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,2
  r6 = uint64_t(int64_t(2));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,15
  r4 = uint64_t(int64_t(15));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a5b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A5B8(base, LIFTED_ARGS(0x821AD5C0, r31)); r3 = o.r3; f1 = o.f1; }
loc_821AD5C0:
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

// sub_821ADAF0: lifted from generated/ by lift.py.
lifted::Result f821ADAF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821ADAF0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ADB14, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r30,198
  r30 = uint64_t(int64_t(198));
  // li r28,2
  r28 = uint64_t(int64_t(2));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
loc_821ADB24:
  // addi r5,r30,-190
  r5 = uint64_t(int64_t(int64_t(r30) + -190));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADB34, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D658(base, LIFTED_ARGS(0x821ADB3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // mr r4,r3
  r4 = r3;
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821adb6c
  if (cr6.lt) goto loc_821ADB6C;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x821adb64
  if (cr6.lt) goto loc_821ADB64;
  // bne cr6,0x821adb78
  if (!cr6.eq) goto loc_821ADB78;
  // mr r5,r30
  r5 = r30;
  // b 0x821adb70
  goto loc_821ADB70;
loc_821ADB64:
  // li r5,199
  r5 = uint64_t(int64_t(199));
  // b 0x821adb70
  goto loc_821ADB70;
loc_821ADB6C:
  // li r5,198
  r5 = uint64_t(int64_t(198));
loc_821ADB70:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C7E8(base, LIFTED_ARGS(0x821ADB78, r31)); r3 = o.r3; f1 = o.f1; }
loc_821ADB78:
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x821adb24
  if (!cr0.eq) goto loc_821ADB24;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADB94, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // addi r5,r11,421
  r5 = uint64_t(int64_t(int64_t(r11) + 421));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C7E8(base, LIFTED_ARGS(0x821ADBAC, r31)); r3 = o.r3; f1 = o.f1; }
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADBBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r10,588(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 588);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821adbe4
  if (cr6.eq) goto loc_821ADBE4;
  // lwz r11,692(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 692);
  // addi r5,r11,524
  r5 = uint64_t(int64_t(int64_t(r11) + 524));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C7E8(base, LIFTED_ARGS(0x821ADBE0, r31)); r3 = o.r3; f1 = o.f1; }
  // b 0x821adbe8
  goto loc_821ADBE8;
loc_821ADBE4:
  // bl 0x8211cc50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211CC50(base, LIFTED_ARGS(0x821ADBE8, r31)); r3 = o.r3; f1 = o.f1; }
loc_821ADBE8:
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADBF8, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r30,r3
  r30 = r3;
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADC0C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x821adc30
  if (cr6.lt) goto loc_821ADC30;
  // bne cr6,0x821adc40
  if (!cr6.eq) goto loc_821ADC40;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,80(r30)
  REX_STORE_U32(uint32_t(r30) + 80, uint32_t(r11));
  // stw r11,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r11));
  // b 0x821adc40
  goto loc_821ADC40;
loc_821ADC30:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r11,80(r30)
  REX_STORE_U32(uint32_t(r30) + 80, uint32_t(r11));
  // stw r10,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r10));
loc_821ADC40:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821ADC48: lifted from generated/ by lift.py.
lifted::Result f821ADC48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821ADC48 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ADC6C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r28,0(r3)
  r28 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r10,28(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821ADC8C:
  // addi r5,r31,8
  r5 = uint64_t(int64_t(int64_t(r31) + 8));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r28
  r3 = r28;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADCA0, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821adccc
  if (cr6.lt) goto loc_821ADCCC;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x821adcc4
  if (cr6.lt) goto loc_821ADCC4;
  // bne cr6,0x821adcd0
  if (!cr6.eq) goto loc_821ADCD0;
  // mr r30,r31
  r30 = r31;
  // b 0x821adcd0
  goto loc_821ADCD0;
loc_821ADCC4:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x821adcd0
  goto loc_821ADCD0;
loc_821ADCCC:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821ADCD0:
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwzx r4,r11,r10
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821adc8c
  if (cr6.lt) goto loc_821ADC8C;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821ADD00: lifted from generated/ by lift.py.
uint64_t f821ADD00(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821ADD00 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ADD24, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r28,0(r3)
  r28 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r10,28(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821ADD44:
  // addi r5,r31,8
  r5 = uint64_t(int64_t(int64_t(r31) + 8));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r28
  r3 = r28;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADD58, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821add84
  if (cr6.lt) goto loc_821ADD84;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x821add7c
  if (cr6.lt) goto loc_821ADD7C;
  // bne cr6,0x821add88
  if (!cr6.eq) goto loc_821ADD88;
  // mr r30,r31
  r30 = r31;
  // b 0x821add88
  goto loc_821ADD88;
loc_821ADD7C:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x821add88
  goto loc_821ADD88;
loc_821ADD84:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821ADD88:
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwzx r4,r11,r10
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // bl 0x821ad090
  // (left out: dead in C++)
  r3 = f821AD090(base, r1, r13, r3, r4, r5, r6);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821add44
  if (cr6.lt) goto loc_821ADD44;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_821ADDB8: lifted from generated/ by lift.py.
lifted::Result f821ADDB8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821ADDB8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ADDDC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r28,0(r3)
  r28 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821ADDFC:
  // addi r5,r31,8
  r5 = uint64_t(int64_t(int64_t(r31) + 8));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r28
  r3 = r28;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADE10, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821ade3c
  if (cr6.lt) goto loc_821ADE3C;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x821ade34
  if (cr6.lt) goto loc_821ADE34;
  // bne cr6,0x821ade40
  if (!cr6.eq) goto loc_821ADE40;
  // mr r30,r31
  r30 = r31;
  // b 0x821ade40
  goto loc_821ADE40;
loc_821ADE34:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x821ade40
  goto loc_821ADE40;
loc_821ADE3C:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821ADE40:
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwzx r4,r11,r10
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821addfc
  if (cr6.lt) goto loc_821ADDFC;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821ADE70: lifted from generated/ by lift.py.
uint64_t f821ADE70(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821ADE70 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ADE94, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r28,0(r3)
  r28 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r10,36(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 36);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821ADEB4:
  // addi r5,r31,8
  r5 = uint64_t(int64_t(int64_t(r31) + 8));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r28
  r3 = r28;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADEC8, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31648);
  // lwz r11,688(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 688);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821adef4
  if (cr6.lt) goto loc_821ADEF4;
  // cmplwi cr6,r11,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 4, so);
  // blt cr6,0x821adeec
  if (cr6.lt) goto loc_821ADEEC;
  // bne cr6,0x821adef8
  if (!cr6.eq) goto loc_821ADEF8;
  // mr r30,r31
  r30 = r31;
  // b 0x821adef8
  goto loc_821ADEF8;
loc_821ADEEC:
  // li r30,1
  r30 = uint64_t(int64_t(1));
  // b 0x821adef8
  goto loc_821ADEF8;
loc_821ADEF4:
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821ADEF8:
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // li r6,10
  r6 = uint64_t(int64_t(10));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwzx r4,r11,r10
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // bl 0x821ad090
  // (left out: dead in C++)
  r3 = f821AD090(base, r1, r13, r3, r4, r5, r6);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821adeb4
  if (cr6.lt) goto loc_821ADEB4;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return r3;
}

// sub_821ADF28: lifted from generated/ by lift.py.
lifted::Result f821ADF28(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821ADF28 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ADF4C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,198
  r31 = uint64_t(int64_t(198));
  // li r30,2
  r30 = uint64_t(int64_t(2));
loc_821ADF58:
  // addi r5,r31,-191
  r5 = uint64_t(int64_t(int64_t(r31) + -191));
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADF68, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D658(base, LIFTED_ARGS(0x821ADF70, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r31
  r5 = r31;
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C7E8(base, LIFTED_ARGS(0x821ADF80, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // bne 0x821adf58
  if (!cr0.eq) goto loc_821ADF58;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821ADF98: lifted from generated/ by lift.py.
lifted::Result f821ADF98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821ADF98 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821ADFBC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,44(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 44);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821ADFE0:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821ADFF0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,-10
  r5 = uint64_t(int64_t(-10));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821adfe0
  if (cr6.lt) goto loc_821ADFE0;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AE018: lifted from generated/ by lift.py.
uint64_t f821AE018(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AE018 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AE03C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,44(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 44);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE060:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AE070, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,-10
  r5 = uint64_t(int64_t(-10));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ad090
  // (left out: dead in C++)
  r3 = f821AD090(base, r1, r13, r3, r4, r5, r6);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae060
  if (cr6.lt) goto loc_821AE060;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_821AE098: lifted from generated/ by lift.py.
lifted::Result f821AE098(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE098 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AE0BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,52(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 52);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE0E0:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AE0F0, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,-5
  r5 = uint64_t(int64_t(-5));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae0e0
  if (cr6.lt) goto loc_821AE0E0;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AE118: lifted from generated/ by lift.py.
uint64_t f821AE118(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AE118 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AE13C, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,52(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 52);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE160:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AE170, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,-5
  r5 = uint64_t(int64_t(-5));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ad090
  // (left out: dead in C++)
  r3 = f821AD090(base, r1, r13, r3, r4, r5, r6);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae160
  if (cr6.lt) goto loc_821AE160;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_821AE198: lifted from generated/ by lift.py.
lifted::Result f821AE198(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE198 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AE1BC, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,198
  r31 = uint64_t(int64_t(198));
  // li r29,2
  r29 = uint64_t(int64_t(2));
loc_821AE1C8:
  // addi r5,r31,-191
  r5 = uint64_t(int64_t(int64_t(r31) + -191));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AE1D8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D658(base, LIFTED_ARGS(0x821AE1E0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r31
  r5 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C7E8(base, LIFTED_ARGS(0x821AE1F0, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // bne 0x821ae1c8
  if (!cr0.eq) goto loc_821AE1C8;
  // li r31,198
  r31 = uint64_t(int64_t(198));
  // li r29,2
  r29 = uint64_t(int64_t(2));
loc_821AE204:
  // addi r5,r31,-191
  r5 = uint64_t(int64_t(int64_t(r31) + -191));
  // li r4,22
  r4 = uint64_t(int64_t(22));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AE214, r31)); r3 = o.r3; f1 = o.f1; }
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8259D658(base, LIFTED_ARGS(0x821AE21C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r31
  r5 = r31;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211C7E8(base, LIFTED_ARGS(0x821AE22C, r31)); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // bne 0x821ae204
  if (!cr0.eq) goto loc_821AE204;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AE240: lifted from generated/ by lift.py.
lifted::Result f821AE240(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE240 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AE264, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,60(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 60);
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE288:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AE298, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,-5
  r5 = uint64_t(int64_t(-5));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae288
  if (cr6.lt) goto loc_821AE288;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AE2C0: lifted from generated/ by lift.py.
uint64_t f821AE2C0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AE2C0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AE2E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,60(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 60);
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE308:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AE318, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,-5
  r5 = uint64_t(int64_t(-5));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ad090
  // (left out: dead in C++)
  r3 = f821AD090(base, r1, r13, r3, r4, r5, r6);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae308
  if (cr6.lt) goto loc_821AE308;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_821AE340: lifted from generated/ by lift.py.
lifted::Result f821AE340(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE340 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AE364, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,68(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 68);
  // lwz r11,72(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 72);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE388:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AE398, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,-5
  r5 = uint64_t(int64_t(-5));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae388
  if (cr6.lt) goto loc_821AE388;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821AE3C0: lifted from generated/ by lift.py.
uint64_t f821AE3C0(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821AE3C0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AE3E4, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,68(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 68);
  // lwz r11,72(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 72);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE408:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AE418, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,-5
  r5 = uint64_t(int64_t(-5));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ad090
  // (left out: dead in C++)
  r3 = f821AD090(base, r1, r13, r3, r4, r5, r6);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae408
  if (cr6.lt) goto loc_821AE408;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_821AE440: lifted from generated/ by lift.py.
lifted::Result f821AE440(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821AE440 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211B3F0(base, LIFTED_ARGS(0x821AE464, r31)); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,80
  r30 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r10,76(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 76);
  // lwz r11,80(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 80);
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
loc_821AE488:
  // addi r5,r31,7
  r5 = uint64_t(int64_t(int64_t(r31) + 7));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::entry::e8211A578(base, LIFTED_ARGS(0x821AE498, r31)); r3 = o.r3; f1 = o.f1; }
  // li r6,5
  r6 = uint64_t(int64_t(5));
  // li r5,-5
  r5 = uint64_t(int64_t(-5));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821acfb0
  // (left out: dead in C++)
  { const lifted::Result o = f821ACFB0(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,2
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 2, so);
  // blt cr6,0x821ae488
  if (cr6.lt) goto loc_821AE488;
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

lifted::Result e821A0E50(uint8_t* base, const lifted::Args& a) {
  return f821A0E50(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821A1258(uint8_t* base, const lifted::Args& a) {
  return f821A1258(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821A1380(uint8_t* base, const lifted::Args& a) {
  return f821A1380(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e821A1E18(uint8_t* base, const lifted::Args& a) {
  return f821A1E18(base, a.r1, a.r13);
}

lifted::Result e821A1ED8(uint8_t* base, const lifted::Args& a) {
  return f821A1ED8(base, a.r1, a.r13);
}

lifted::Result e821A1FA8(uint8_t* base, const lifted::Args& a) {
  return f821A1FA8(base, a.r1, a.r13);
}

lifted::Result e821A2070(uint8_t* base, const lifted::Args& a) {
  return f821A2070(base, a.r1, a.r13);
}

lifted::Result e821A2208(uint8_t* base, const lifted::Args& a) {
  return f821A2208(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821A2338(uint8_t* base, const lifted::Args& a) {
  f821A2338(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e821A2350(uint8_t* base, const lifted::Args& a) {
  return f821A2350(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821A2FB0(uint8_t* base, const lifted::Args& a) {
  return f821A2FB0(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821A3868(uint8_t* base, const lifted::Args& a) {
  return f821A3868(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821A3950(uint8_t* base, const lifted::Args& a) {
  return f821A3950(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e821A3ED8(uint8_t* base, const lifted::Args& a) {
  return f821A3ED8(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821A4228(uint8_t* base, const lifted::Args& a) {
  return f821A4228(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821A4450(uint8_t* base, const lifted::Args& a) {
  return f821A4450(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e821A4810(uint8_t* base, const lifted::Args& a) {
  return f821A4810(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e821A4970(uint8_t* base, const lifted::Args& a) {
  return f821A4970(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e821A4EF8(uint8_t* base, const lifted::Args& a) {
  return f821A4EF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e821A6030(uint8_t* base, const lifted::Args& a) {
  return f821A6030(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821A8DE0(uint8_t* base, const lifted::Args& a) {
  return {f821A8DE0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821A8E28(uint8_t* base, const lifted::Args& a) {
  return {f821A8E28(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821A8E70(uint8_t* base, const lifted::Args& a) {
  f821A8E70(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e821A8E88(uint8_t* base, const lifted::Args& a) {
  f821A8E88(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e821A8EF8(uint8_t* base, const lifted::Args& a) {
  return {f821A8EF8(base, a.r1), a.f[0]};
}

lifted::Result e821A9248(uint8_t* base, const lifted::Args& a) {
  return {f821A9248(base, a.r1), a.f[0]};
}

lifted::Result e821A93B0(uint8_t* base, const lifted::Args& a) {
  return {f821A93B0(base, a.r1), a.f[0]};
}

lifted::Result e821A9518(uint8_t* base, const lifted::Args& a) {
  return {f821A9518(base, a.r1), a.f[0]};
}

lifted::Result e821A9658(uint8_t* base, const lifted::Args& a) {
  return {f821A9658(base, a.r1), a.f[0]};
}

lifted::Result e821A9798(uint8_t* base, const lifted::Args& a) {
  return {f821A9798(base, a.r1), a.f[0]};
}

lifted::Result e821A98D8(uint8_t* base, const lifted::Args& a) {
  return {f821A98D8(base, a.r1), a.f[0]};
}

lifted::Result e821A9A18(uint8_t* base, const lifted::Args& a) {
  return {f821A9A18(base, a.r1), a.f[0]};
}

lifted::Result e821A9B58(uint8_t* base, const lifted::Args& a) {
  return {f821A9B58(base, a.r1), a.f[0]};
}

lifted::Result e821A9C98(uint8_t* base, const lifted::Args& a) {
  return {f821A9C98(base), a.f[0]};
}

lifted::Result e821A9DE0(uint8_t* base, const lifted::Args& a) {
  return {f821A9DE0(base), a.f[0]};
}

lifted::Result e821A9F28(uint8_t* base, const lifted::Args& a) {
  return {f821A9F28(base), a.f[0]};
}

lifted::Result e821AA070(uint8_t* base, const lifted::Args& a) {
  return {f821AA070(base, a.r1), a.f[0]};
}

lifted::Result e821AA1B0(uint8_t* base, const lifted::Args& a) {
  return {f821AA1B0(base), a.f[0]};
}

lifted::Result e821AA500(uint8_t* base, const lifted::Args& a) {
  f821AA500(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e821AA530(uint8_t* base, const lifted::Args& a) {
  f821AA530(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e821AA560(uint8_t* base, const lifted::Args& a) {
  return f821AA560(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AA7B8(uint8_t* base, const lifted::Args& a) {
  return f821AA7B8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AA970(uint8_t* base, const lifted::Args& a) {
  return f821AA970(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AAAB8(uint8_t* base, const lifted::Args& a) {
  return f821AAAB8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AAC60(uint8_t* base, const lifted::Args& a) {
  return f821AAC60(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AAE90(uint8_t* base, const lifted::Args& a) {
  return {f821AAE90(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2]), a.f[0]};
}

lifted::Result e821AAF08(uint8_t* base, const lifted::Args& a) {
  return f821AAF08(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AB038(uint8_t* base, const lifted::Args& a) {
  return f821AB038(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AB250(uint8_t* base, const lifted::Args& a) {
  f821AB250(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e821AB2C0(uint8_t* base, const lifted::Args& a) {
  return f821AB2C0(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AB518(uint8_t* base, const lifted::Args& a) {
  return f821AB518(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821AB5B8(uint8_t* base, const lifted::Args& a) {
  return f821AB5B8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821AB698(uint8_t* base, const lifted::Args& a) {
  return f821AB698(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821AB7F0(uint8_t* base, const lifted::Args& a) {
  return f821AB7F0(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821AB898(uint8_t* base, const lifted::Args& a) {
  return f821AB898(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821ABC00(uint8_t* base, const lifted::Args& a) {
  return f821ABC00(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821ABD30(uint8_t* base, const lifted::Args& a) {
  return f821ABD30(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821ABDC0(uint8_t* base, const lifted::Args& a) {
  return f821ABDC0(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821ABEF8(uint8_t* base, const lifted::Args& a) {
  return f821ABEF8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821ABFD8(uint8_t* base, const lifted::Args& a) {
  return f821ABFD8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821AC068(uint8_t* base, const lifted::Args& a) {
  return f821AC068(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821AC0F8(uint8_t* base, const lifted::Args& a) {
  return f821AC0F8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821AC188(uint8_t* base, const lifted::Args& a) {
  return f821AC188(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821AC218(uint8_t* base, const lifted::Args& a) {
  return f821AC218(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821AC2A8(uint8_t* base, const lifted::Args& a) {
  return f821AC2A8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821AC338(uint8_t* base, const lifted::Args& a) {
  return f821AC338(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821AC408(uint8_t* base, const lifted::Args& a) {
  return f821AC408(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821AC4D8(uint8_t* base, const lifted::Args& a) {
  return f821AC4D8(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821AC5A8(uint8_t* base, const lifted::Args& a) {
  return f821AC5A8(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821AC6A8(uint8_t* base, const lifted::Args& a) {
  return f821AC6A8(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821AC7A8(uint8_t* base, const lifted::Args& a) {
  return f821AC7A8(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821AC878(uint8_t* base, const lifted::Args& a) {
  return f821AC878(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821AC948(uint8_t* base, const lifted::Args& a) {
  return f821AC948(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821ACA18(uint8_t* base, const lifted::Args& a) {
  return f821ACA18(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821ACB18(uint8_t* base, const lifted::Args& a) {
  return f821ACB18(base, a.r1, a.r13, a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821ACBE8(uint8_t* base, const lifted::Args& a) {
  return f821ACBE8(base, a.r1, a.r13, a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821ACCB8(uint8_t* base, const lifted::Args& a) {
  return f821ACCB8(base, a.r1, a.r13, a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821ACD88(uint8_t* base, const lifted::Args& a) {
  return f821ACD88(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821ACE58(uint8_t* base, const lifted::Args& a) {
  return f821ACE58(base, a.r1, a.r13, a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821ACF28(uint8_t* base, const lifted::Args& a) {
  return f821ACF28(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AD2E0(uint8_t* base, const lifted::Args& a) {
  return {f821AD2E0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821AD3B0(uint8_t* base, const lifted::Args& a) {
  return f821AD3B0(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e821AD458(uint8_t* base, const lifted::Args& a) {
  return {f821AD458(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AD530(uint8_t* base, const lifted::Args& a) {
  return {f821AD530(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821ADAF0(uint8_t* base, const lifted::Args& a) {
  return f821ADAF0(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821ADC48(uint8_t* base, const lifted::Args& a) {
  return f821ADC48(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821ADD00(uint8_t* base, const lifted::Args& a) {
  return {f821ADD00(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821ADDB8(uint8_t* base, const lifted::Args& a) {
  return f821ADDB8(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821ADE70(uint8_t* base, const lifted::Args& a) {
  return {f821ADE70(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821ADF28(uint8_t* base, const lifted::Args& a) {
  return f821ADF28(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821ADF98(uint8_t* base, const lifted::Args& a) {
  return f821ADF98(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821AE018(uint8_t* base, const lifted::Args& a) {
  return {f821AE018(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AE098(uint8_t* base, const lifted::Args& a) {
  return f821AE098(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821AE118(uint8_t* base, const lifted::Args& a) {
  return {f821AE118(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AE198(uint8_t* base, const lifted::Args& a) {
  return f821AE198(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821AE240(uint8_t* base, const lifted::Args& a) {
  return f821AE240(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821AE2C0(uint8_t* base, const lifted::Args& a) {
  return {f821AE2C0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AE340(uint8_t* base, const lifted::Args& a) {
  return f821AE340(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821AE3C0(uint8_t* base, const lifted::Args& a) {
  return {f821AE3C0(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821AE440(uint8_t* base, const lifted::Args& a) {
  return f821AE440(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x821A0E50, e821A0E50},
    {0x821A1258, e821A1258},
    {0x821A1380, e821A1380},
    {0x821A1E18, e821A1E18},
    {0x821A1ED8, e821A1ED8},
    {0x821A1FA8, e821A1FA8},
    {0x821A2070, e821A2070},
    {0x821A2208, e821A2208},
    {0x821A2338, e821A2338},
    {0x821A2350, e821A2350},
    {0x821A2FB0, e821A2FB0},
    {0x821A3868, e821A3868},
    {0x821A3950, e821A3950},
    {0x821A3ED8, e821A3ED8},
    {0x821A4228, e821A4228},
    {0x821A4450, e821A4450},
    {0x821A4810, e821A4810},
    {0x821A4970, e821A4970},
    {0x821A4EF8, e821A4EF8},
    {0x821A6030, e821A6030},
    {0x821A8DE0, e821A8DE0},
    {0x821A8E28, e821A8E28},
    {0x821A8E70, e821A8E70},
    {0x821A8E88, e821A8E88},
    {0x821A8EF8, e821A8EF8},
    {0x821A9248, e821A9248},
    {0x821A93B0, e821A93B0},
    {0x821A9518, e821A9518},
    {0x821A9658, e821A9658},
    {0x821A9798, e821A9798},
    {0x821A98D8, e821A98D8},
    {0x821A9A18, e821A9A18},
    {0x821A9B58, e821A9B58},
    {0x821A9C98, e821A9C98},
    {0x821A9DE0, e821A9DE0},
    {0x821A9F28, e821A9F28},
    {0x821AA070, e821AA070},
    {0x821AA1B0, e821AA1B0},
    {0x821AA500, e821AA500},
    {0x821AA530, e821AA530},
    {0x821AA560, e821AA560},
    {0x821AA7B8, e821AA7B8},
    {0x821AA970, e821AA970},
    {0x821AAAB8, e821AAAB8},
    {0x821AAC60, e821AAC60},
    {0x821AAE90, e821AAE90},
    {0x821AAF08, e821AAF08},
    {0x821AB038, e821AB038},
    {0x821AB250, e821AB250},
    {0x821AB2C0, e821AB2C0},
    {0x821AB518, e821AB518},
    {0x821AB5B8, e821AB5B8},
    {0x821AB698, e821AB698},
    {0x821AB7F0, e821AB7F0},
    {0x821AB898, e821AB898},
    {0x821ABC00, e821ABC00},
    {0x821ABD30, e821ABD30},
    {0x821ABDC0, e821ABDC0},
    {0x821ABEF8, e821ABEF8},
    {0x821ABFD8, e821ABFD8},
    {0x821AC068, e821AC068},
    {0x821AC0F8, e821AC0F8},
    {0x821AC188, e821AC188},
    {0x821AC218, e821AC218},
    {0x821AC2A8, e821AC2A8},
    {0x821AC338, e821AC338},
    {0x821AC408, e821AC408},
    {0x821AC4D8, e821AC4D8},
    {0x821AC5A8, e821AC5A8},
    {0x821AC6A8, e821AC6A8},
    {0x821AC7A8, e821AC7A8},
    {0x821AC878, e821AC878},
    {0x821AC948, e821AC948},
    {0x821ACA18, e821ACA18},
    {0x821ACB18, e821ACB18},
    {0x821ACBE8, e821ACBE8},
    {0x821ACCB8, e821ACCB8},
    {0x821ACD88, e821ACD88},
    {0x821ACE58, e821ACE58},
    {0x821ACF28, e821ACF28},
    {0x821AD2E0, e821AD2E0},
    {0x821AD3B0, e821AD3B0},
    {0x821AD458, e821AD458},
    {0x821AD530, e821AD530},
    {0x821ADAF0, e821ADAF0},
    {0x821ADC48, e821ADC48},
    {0x821ADD00, e821ADD00},
    {0x821ADDB8, e821ADDB8},
    {0x821ADE70, e821ADE70},
    {0x821ADF28, e821ADF28},
    {0x821ADF98, e821ADF98},
    {0x821AE018, e821AE018},
    {0x821AE098, e821AE098},
    {0x821AE118, e821AE118},
    {0x821AE198, e821AE198},
    {0x821AE240, e821AE240},
    {0x821AE2C0, e821AE2C0},
    {0x821AE340, e821AE340},
    {0x821AE3C0, e821AE3C0},
    {0x821AE440, e821AE440},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_009)), true);

}  // namespace

}  // namespace lifted
