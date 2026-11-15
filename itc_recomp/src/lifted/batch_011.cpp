// Lift sweep batch 011 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_011, true, "ITC",
              "Run lift sweep batch 011's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register.
#define LIFTED_ARGS \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }

namespace lifted {

// sub_821B5BF0: lifted from generated/ by lift.py.
lifted::Result f821B5BF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B5BF0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5c84
  if (!cr6.eq) goto loc_821B5C84;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821b5c50
  if (cr6.eq) goto loc_821B5C50;
  // addi r4,r11,149
  r4 = uint64_t(int64_t(int64_t(r11) + 149));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b5c60
  goto loc_821B5C60;
loc_821B5C50:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B5C60:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,208
  r4 = uint64_t(int64_t(208));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4298
  // (left out: dead in C++)
  { const lifted::Result o = f821B4298(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r29));
loc_821B5C84:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b5cd8
  if (!cr0.eq) goto loc_821B5CD8;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821b5cb0
  if (cr6.eq) goto loc_821B5CB0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B5CB0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
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
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
  // bl 0x821ad530
  // (left out: dead in C++)
  r3 = f821AD530(base, r1, r13);
loc_821B5CD8:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B5CE8: lifted from generated/ by lift.py.
lifted::Result f821B5CE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B5CE8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5d84
  if (!cr6.eq) goto loc_821B5D84;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821b5d4c
  if (cr6.eq) goto loc_821B5D4C;
  // addi r4,r11,159
  r4 = uint64_t(int64_t(int64_t(r11) + 159));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b5d5c
  goto loc_821B5D5C;
loc_821B5D4C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B5D5C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,214
  r4 = uint64_t(int64_t(214));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4548
  // (left out: dead in C++)
  { const lifted::Result o = f821B4548(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B5D84:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b5dd8
  if (!cr0.eq) goto loc_821B5DD8;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821b5db0
  if (cr6.eq) goto loc_821B5DB0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B5DB0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,11
  r8 = uint64_t(int64_t(11));
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r6,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r6));
loc_821B5DD8:
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

// sub_821B5DF8: lifted from generated/ by lift.py.
lifted::Result f821B5DF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B5DF8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5e94
  if (!cr6.eq) goto loc_821B5E94;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821b5e5c
  if (cr6.eq) goto loc_821B5E5C;
  // addi r4,r11,159
  r4 = uint64_t(int64_t(int64_t(r11) + 159));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b5e6c
  goto loc_821B5E6C;
loc_821B5E5C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B5E6C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,216
  r4 = uint64_t(int64_t(216));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4548
  // (left out: dead in C++)
  { const lifted::Result o = f821B4548(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B5E94:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b5ee8
  if (!cr0.eq) goto loc_821B5EE8;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821b5ec0
  if (cr6.eq) goto loc_821B5EC0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B5EC0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,11
  r8 = uint64_t(int64_t(11));
  // li r7,2
  r7 = uint64_t(int64_t(2));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r6,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r6));
loc_821B5EE8:
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

// sub_821B5F08: lifted from generated/ by lift.py.
lifted::Result f821B5F08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B5F08 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r29,3
  r29 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r29,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r29));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b5f9c
  if (!cr6.eq) goto loc_821B5F9C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821b5f64
  if (cr6.eq) goto loc_821B5F64;
  // addi r4,r11,174
  r4 = uint64_t(int64_t(int64_t(r11) + 174));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b5f74
  goto loc_821B5F74;
loc_821B5F64:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B5F74:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,222
  r4 = uint64_t(int64_t(222));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b46d8
  // (left out: dead in C++)
  { const lifted::Result o = f821B46D8(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821B5F9C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b5fec
  if (!cr0.eq) goto loc_821B5FEC;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821b5fc8
  if (cr6.eq) goto loc_821B5FC8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B5FC8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // li r8,15
  r8 = uint64_t(int64_t(15));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
loc_821B5FEC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B5FF8: lifted from generated/ by lift.py.
lifted::Result f821B5FF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B5FF8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r29,3
  r29 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r29,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r29));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b608c
  if (!cr6.eq) goto loc_821B608C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821b6054
  if (cr6.eq) goto loc_821B6054;
  // addi r4,r11,174
  r4 = uint64_t(int64_t(int64_t(r11) + 174));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b6064
  goto loc_821B6064;
loc_821B6054:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B6064:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,224
  r4 = uint64_t(int64_t(224));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b46d8
  // (left out: dead in C++)
  { const lifted::Result o = f821B46D8(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821B608C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b60dc
  if (!cr0.eq) goto loc_821B60DC;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821b60b8
  if (cr6.eq) goto loc_821B60B8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B60B8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // li r8,15
  r8 = uint64_t(int64_t(15));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
loc_821B60DC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B60E8: lifted from generated/ by lift.py.
lifted::Result f821B60E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B60E8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r29,3
  r29 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r29,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r29));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b617c
  if (!cr6.eq) goto loc_821B617C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821b6144
  if (cr6.eq) goto loc_821B6144;
  // addi r4,r11,174
  r4 = uint64_t(int64_t(int64_t(r11) + 174));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b6154
  goto loc_821B6154;
loc_821B6144:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B6154:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,226
  r4 = uint64_t(int64_t(226));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b46d8
  // (left out: dead in C++)
  { const lifted::Result o = f821B46D8(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821B617C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b61cc
  if (!cr0.eq) goto loc_821B61CC;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821b61a8
  if (cr6.eq) goto loc_821B61A8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B61A8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // li r8,15
  r8 = uint64_t(int64_t(15));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
loc_821B61CC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B61D8: lifted from generated/ by lift.py.
lifted::Result f821B61D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B61D8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r29,3
  r29 = uint64_t(int64_t(3));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r29,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r29));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b626c
  if (!cr6.eq) goto loc_821B626C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821b6234
  if (cr6.eq) goto loc_821B6234;
  // addi r4,r11,159
  r4 = uint64_t(int64_t(int64_t(r11) + 159));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b6244
  goto loc_821B6244;
loc_821B6234:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B6244:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,228
  r4 = uint64_t(int64_t(228));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b46d8
  // (left out: dead in C++)
  { const lifted::Result o = f821B46D8(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821B626C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b62bc
  if (!cr0.eq) goto loc_821B62BC;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821b6298
  if (cr6.eq) goto loc_821B6298;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B6298:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
  // li r8,15
  r8 = uint64_t(int64_t(15));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
loc_821B62BC:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B62C8: lifted from generated/ by lift.py.
lifted::Result f821B62C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B62C8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b6364
  if (!cr6.eq) goto loc_821B6364;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821b632c
  if (cr6.eq) goto loc_821B632C;
  // addi r4,r11,186
  r4 = uint64_t(int64_t(int64_t(r11) + 186));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b633c
  goto loc_821B633C;
loc_821B632C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B633C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,236
  r4 = uint64_t(int64_t(236));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4848
  // (left out: dead in C++)
  { const lifted::Result o = f821B4848(base, r1, r13, r6, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B6364:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b63bc
  if (!cr0.eq) goto loc_821B63BC;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821b6390
  if (cr6.eq) goto loc_821B6390;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B6390:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // li r7,4
  r7 = uint64_t(int64_t(4));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r6,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r6));
  // bl 0x821ae640
  // (left out: dead in C++)
  r3 = f821AE640(base, r1, r13);
loc_821B63BC:
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

// sub_821B63D8: lifted from generated/ by lift.py.
lifted::Result f821B63D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B63D8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b6474
  if (!cr6.eq) goto loc_821B6474;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821b643c
  if (cr6.eq) goto loc_821B643C;
  // addi r4,r11,186
  r4 = uint64_t(int64_t(int64_t(r11) + 186));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b644c
  goto loc_821B644C;
loc_821B643C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B644C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,238
  r4 = uint64_t(int64_t(238));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4848
  // (left out: dead in C++)
  { const lifted::Result o = f821B4848(base, r1, r13, r6, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B6474:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b64cc
  if (!cr0.eq) goto loc_821B64CC;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821b64a0
  if (cr6.eq) goto loc_821B64A0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B64A0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // li r7,4
  r7 = uint64_t(int64_t(4));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r6,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r6));
  // bl 0x821ae640
  // (left out: dead in C++)
  r3 = f821AE640(base, r1, r13);
loc_821B64CC:
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

// sub_821B64E8: lifted from generated/ by lift.py.
lifted::Result f821B64E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B64E8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b6584
  if (!cr6.eq) goto loc_821B6584;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821b654c
  if (cr6.eq) goto loc_821B654C;
  // addi r4,r11,186
  r4 = uint64_t(int64_t(int64_t(r11) + 186));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b655c
  goto loc_821B655C;
loc_821B654C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B655C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,240
  r4 = uint64_t(int64_t(240));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4848
  // (left out: dead in C++)
  { const lifted::Result o = f821B4848(base, r1, r13, r6, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B6584:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b65dc
  if (!cr0.eq) goto loc_821B65DC;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821b65b0
  if (cr6.eq) goto loc_821B65B0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B65B0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // li r7,4
  r7 = uint64_t(int64_t(4));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r6,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r6));
  // bl 0x821ae640
  // (left out: dead in C++)
  r3 = f821AE640(base, r1, r13);
loc_821B65DC:
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

// sub_821B65F8: lifted from generated/ by lift.py.
lifted::Result f821B65F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B65F8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b6694
  if (!cr6.eq) goto loc_821B6694;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // beq cr6,0x821b665c
  if (cr6.eq) goto loc_821B665C;
  // addi r4,r11,174
  r4 = uint64_t(int64_t(int64_t(r11) + 174));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b666c
  goto loc_821B666C;
loc_821B665C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B666C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,242
  r4 = uint64_t(int64_t(242));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4848
  // (left out: dead in C++)
  { const lifted::Result o = f821B4848(base, r1, r13, r6, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B6694:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b66ec
  if (!cr0.eq) goto loc_821B66EC;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821b66c0
  if (cr6.eq) goto loc_821B66C0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B66C0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // li r7,4
  r7 = uint64_t(int64_t(4));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r6,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r6));
  // bl 0x821ae640
  // (left out: dead in C++)
  r3 = f821AE640(base, r1, r13);
loc_821B66EC:
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

// sub_821B6708: lifted from generated/ by lift.py.
lifted::Result f821B6708(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B6708 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b67a4
  if (!cr6.eq) goto loc_821B67A4;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31652(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31652);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821b676c
  if (cr6.eq) goto loc_821B676C;
  // addi r4,r11,194
  r4 = uint64_t(int64_t(int64_t(r11) + 194));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b677c
  goto loc_821B677C;
loc_821B676C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31648);
  // lwz r3,732(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 732);
  // bl 0x821ad3b0
  // (left out: dead in C++)
  { const lifted::Result o = f821AD3B0(base, r1, r13, r3, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_821B677C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,244
  r4 = uint64_t(int64_t(244));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821b4848
  // (left out: dead in C++)
  { const lifted::Result o = f821B4848(base, r1, r13, r6, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821B67A4:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b67fc
  if (!cr0.eq) goto loc_821B67FC;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // beq cr6,0x821b67d0
  if (cr6.eq) goto loc_821B67D0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B67D0:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // li r7,4
  r7 = uint64_t(int64_t(4));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r7,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r7));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // lwz r11,-31656(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31656);
  // stw r6,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r6));
  // bl 0x821ae640
  // (left out: dead in C++)
  r3 = f821AE640(base, r1, r13);
loc_821B67FC:
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

// sub_821B6818: lifted from generated/ by lift.py.
lifted::Result f821B6818(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_821B6818 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r14 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e50
  // (left out: dead in C++)
  // (left out: dead in C++)
  // ld r12,-4096(r1)
  r12 = REX_LOAD_U64(uint32_t(r1) + -4096);
  // stwu r1,-6592(r1)
  ea = -6592 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // stw r3,6612(r1)
  REX_STORE_U32(uint32_t(r1) + 6612, uint32_t(r3));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // addi r6,r11,-23244
  r6 = uint64_t(int64_t(int64_t(r11) + -23244));
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // lis r14,-32229
  r14 = uint64_t(int64_t(-2112159744));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // addi r14,r14,-19176
  r14 = uint64_t(int64_t(int64_t(r14) + -19176));
  // lis r15,-32229
  r15 = uint64_t(int64_t(-2112159744));
  // stw r10,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r10));
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // lis r16,-32229
  r16 = uint64_t(int64_t(-2112159744));
  // lwz r11,-31656(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31656);
  // lis r17,-32229
  r17 = uint64_t(int64_t(-2112159744));
  // stw r14,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r14));
  // lis r18,-32229
  r18 = uint64_t(int64_t(-2112159744));
  // lwz r14,80(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lis r19,-32229
  r19 = uint64_t(int64_t(-2112159744));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // addi r14,r14,19688
  r14 = uint64_t(int64_t(int64_t(r14) + 19688));
  // lis r20,-32229
  r20 = uint64_t(int64_t(-2112159744));
  // lis r21,-32229
  r21 = uint64_t(int64_t(-2112159744));
  // lis r22,-32229
  r22 = uint64_t(int64_t(-2112159744));
  // stw r10,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r10));
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // stw r14,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r14));
  // lis r23,-32229
  r23 = uint64_t(int64_t(-2112159744));
  // lis r24,-32229
  r24 = uint64_t(int64_t(-2112159744));
  // lis r25,-32229
  r25 = uint64_t(int64_t(-2112159744));
  // lis r26,-32229
  r26 = uint64_t(int64_t(-2112159744));
  // stw r10,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r10));
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // lis r27,-32229
  r27 = uint64_t(int64_t(-2112159744));
  // lis r28,-32229
  r28 = uint64_t(int64_t(-2112159744));
  // lis r29,-32229
  r29 = uint64_t(int64_t(-2112159744));
  // lis r30,-32229
  r30 = uint64_t(int64_t(-2112159744));
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // lis r4,-32229
  r4 = uint64_t(int64_t(-2112159744));
  // lis r5,-32229
  r5 = uint64_t(int64_t(-2112159744));
  // lis r6,-32229
  r6 = uint64_t(int64_t(-2112159744));
  // lis r7,-32229
  r7 = uint64_t(int64_t(-2112159744));
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // lis r8,-32229
  r8 = uint64_t(int64_t(-2112159744));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // stw r10,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r10));
  // lis r10,-32229
  r10 = uint64_t(int64_t(-2112159744));
  // lwz r14,100(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 100);
  // addi r14,r14,-19016
  r14 = uint64_t(int64_t(int64_t(r14) + -19016));
  // stw r14,168(r1)
  REX_STORE_U32(uint32_t(r1) + 168, uint32_t(r14));
  // lwz r14,92(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 92);
  // addi r14,r14,-18792
  r14 = uint64_t(int64_t(int64_t(r14) + -18792));
  // stw r14,172(r1)
  REX_STORE_U32(uint32_t(r1) + 172, uint32_t(r14));
  // lwz r14,108(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 108);
  // addi r14,r14,-18280
  r14 = uint64_t(int64_t(int64_t(r14) + -18280));
  // stw r14,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r14));
  // lwz r14,84(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 84);
  // addi r14,r14,-18448
  r14 = uint64_t(int64_t(int64_t(r14) + -18448));
  // stw r14,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r14));
  // lwz r14,88(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 88);
  // addi r14,r14,19984
  r14 = uint64_t(int64_t(int64_t(r14) + 19984));
  // stw r14,184(r1)
  REX_STORE_U32(uint32_t(r1) + 184, uint32_t(r14));
  // lwz r14,96(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 96);
  // addi r14,r14,-17408
  r14 = uint64_t(int64_t(int64_t(r14) + -17408));
  // stw r14,188(r1)
  REX_STORE_U32(uint32_t(r1) + 188, uint32_t(r14));
  // lwz r14,104(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 104);
  // addi r9,r9,9816
  r9 = uint64_t(int64_t(int64_t(r9) + 9816));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // addi r14,r14,5752
  r14 = uint64_t(int64_t(int64_t(r14) + 5752));
  // addi r10,r10,-17104
  r10 = uint64_t(int64_t(int64_t(r10) + -17104));
  // addi r15,r15,20440
  r15 = uint64_t(int64_t(int64_t(r15) + 20440));
  // addi r16,r16,-17104
  r16 = uint64_t(int64_t(int64_t(r16) + -17104));
  // stw r9,288(r1)
  REX_STORE_U32(uint32_t(r1) + 288, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r14,192(r1)
  REX_STORE_U32(uint32_t(r1) + 192, uint32_t(r14));
  // addi r17,r17,6496
  r17 = uint64_t(int64_t(int64_t(r17) + 6496));
  // lwz r14,112(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 112);
  // addi r18,r18,20712
  r18 = uint64_t(int64_t(int64_t(r18) + 20712));
  // stw r3,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r3));
  // addi r3,r20,6976
  r3 = uint64_t(int64_t(int64_t(r20) + 6976));
  // addi r14,r14,20224
  r14 = uint64_t(int64_t(int64_t(r14) + 20224));
  // stw r10,220(r1)
  REX_STORE_U32(uint32_t(r1) + 220, uint32_t(r10));
  // stw r9,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // addi r19,r19,20928
  r19 = uint64_t(int64_t(int64_t(r19) + 20928));
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // addi r4,r4,9136
  r4 = uint64_t(int64_t(int64_t(r4) + 9136));
  // stw r15,200(r1)
  REX_STORE_U32(uint32_t(r1) + 200, uint32_t(r15));
  // stw r3,224(r1)
  REX_STORE_U32(uint32_t(r1) + 224, uint32_t(r3));
  // addi r3,r21,21200
  r3 = uint64_t(int64_t(int64_t(r21) + 21200));
  // addi r5,r5,-16424
  r5 = uint64_t(int64_t(int64_t(r5) + -16424));
  // stw r14,196(r1)
  REX_STORE_U32(uint32_t(r1) + 196, uint32_t(r14));
  // stw r9,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // addi r6,r6,9344
  r6 = uint64_t(int64_t(int64_t(r6) + 9344));
  // stw r16,204(r1)
  REX_STORE_U32(uint32_t(r1) + 204, uint32_t(r16));
  // addi r7,r7,9672
  r7 = uint64_t(int64_t(int64_t(r7) + 9672));
  // stw r17,208(r1)
  REX_STORE_U32(uint32_t(r1) + 208, uint32_t(r17));
  // stw r3,228(r1)
  REX_STORE_U32(uint32_t(r1) + 228, uint32_t(r3));
  // addi r3,r22,21416
  r3 = uint64_t(int64_t(int64_t(r22) + 21416));
  // addi r8,r8,23040
  r8 = uint64_t(int64_t(int64_t(r8) + 23040));
  // stw r18,212(r1)
  REX_STORE_U32(uint32_t(r1) + 212, uint32_t(r18));
  // stw r9,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r19,216(r1)
  REX_STORE_U32(uint32_t(r1) + 216, uint32_t(r19));
  // lis r14,-32229
  r14 = uint64_t(int64_t(-2112159744));
  // stw r4,268(r1)
  REX_STORE_U32(uint32_t(r1) + 268, uint32_t(r4));
  // stw r3,232(r1)
  REX_STORE_U32(uint32_t(r1) + 232, uint32_t(r3));
  // addi r3,r23,-16960
  r3 = uint64_t(int64_t(int64_t(r23) + -16960));
  // stw r5,272(r1)
  REX_STORE_U32(uint32_t(r1) + 272, uint32_t(r5));
  // stw r9,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r6,276(r1)
  REX_STORE_U32(uint32_t(r1) + 276, uint32_t(r6));
  // stw r7,280(r1)
  REX_STORE_U32(uint32_t(r1) + 280, uint32_t(r7));
  // stw r3,236(r1)
  REX_STORE_U32(uint32_t(r1) + 236, uint32_t(r3));
  // addi r3,r24,7584
  r3 = uint64_t(int64_t(int64_t(r24) + 7584));
  // stw r8,284(r1)
  REX_STORE_U32(uint32_t(r1) + 284, uint32_t(r8));
  // stw r9,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r3,240(r1)
  REX_STORE_U32(uint32_t(r1) + 240, uint32_t(r3));
  // addi r3,r25,21656
  r3 = uint64_t(int64_t(int64_t(r25) + 21656));
  // stw r9,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r3,244(r1)
  REX_STORE_U32(uint32_t(r1) + 244, uint32_t(r3));
  // addi r3,r26,21872
  r3 = uint64_t(int64_t(int64_t(r26) + 21872));
  // stw r9,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r3,248(r1)
  REX_STORE_U32(uint32_t(r1) + 248, uint32_t(r3));
  // addi r3,r27,-16648
  r3 = uint64_t(int64_t(int64_t(r27) + -16648));
  // stw r9,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r3,252(r1)
  REX_STORE_U32(uint32_t(r1) + 252, uint32_t(r3));
  // addi r3,r28,8448
  r3 = uint64_t(int64_t(int64_t(r28) + 8448));
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r3,256(r1)
  REX_STORE_U32(uint32_t(r1) + 256, uint32_t(r3));
  // addi r3,r29,22216
  r3 = uint64_t(int64_t(int64_t(r29) + 22216));
  // stw r9,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r3,260(r1)
  REX_STORE_U32(uint32_t(r1) + 260, uint32_t(r3));
  // addi r3,r30,22552
  r3 = uint64_t(int64_t(int64_t(r30) + 22552));
  // stw r9,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r3,264(r1)
  REX_STORE_U32(uint32_t(r1) + 264, uint32_t(r3));
  // stw r9,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r9,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r9));
  // addi r14,r14,-16280
  r14 = uint64_t(int64_t(int64_t(r14) + -16280));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // lis r15,-32229
  r15 = uint64_t(int64_t(-2112159744));
  // lis r16,-32229
  r16 = uint64_t(int64_t(-2112159744));
  // addi r15,r15,12064
  r15 = uint64_t(int64_t(int64_t(r15) + 12064));
  // stw r14,292(r1)
  REX_STORE_U32(uint32_t(r1) + 292, uint32_t(r14));
  // lis r17,-32229
  r17 = uint64_t(int64_t(-2112159744));
  // lwz r14,112(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 112);
  // lis r18,-32229
  r18 = uint64_t(int64_t(-2112159744));
  // stw r9,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // addi r14,r14,10024
  r14 = uint64_t(int64_t(int64_t(r14) + 10024));
  // lis r19,-32229
  r19 = uint64_t(int64_t(-2112159744));
  // stw r15,360(r1)
  REX_STORE_U32(uint32_t(r1) + 360, uint32_t(r15));
  // lis r20,-32229
  r20 = uint64_t(int64_t(-2112159744));
  // lis r21,-32229
  r21 = uint64_t(int64_t(-2112159744));
  // stw r9,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r9));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // stw r14,296(r1)
  REX_STORE_U32(uint32_t(r1) + 296, uint32_t(r14));
  // lis r22,-32229
  r22 = uint64_t(int64_t(-2112159744));
  // lwz r14,104(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 104);
  // lis r23,-32229
  r23 = uint64_t(int64_t(-2112159744));
  // lis r24,-32229
  r24 = uint64_t(int64_t(-2112159744));
  // addi r14,r14,10352
  r14 = uint64_t(int64_t(int64_t(r14) + 10352));
  // stw r9,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r9));
  // lis r25,-32229
  r25 = uint64_t(int64_t(-2112159744));
  // lis r26,-32229
  r26 = uint64_t(int64_t(-2112159744));
  // lis r27,-32229
  r27 = uint64_t(int64_t(-2112159744));
  // lis r28,-32229
  r28 = uint64_t(int64_t(-2112159744));
  // stw r14,300(r1)
  REX_STORE_U32(uint32_t(r1) + 300, uint32_t(r14));
  // lis r29,-32229
  r29 = uint64_t(int64_t(-2112159744));
  // lwz r14,96(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 96);
  // lis r30,-32229
  r30 = uint64_t(int64_t(-2112159744));
  // lis r3,-32229
  r3 = uint64_t(int64_t(-2112159744));
  // addi r14,r14,23288
  r14 = uint64_t(int64_t(int64_t(r14) + 23288));
  // lis r4,-32229
  r4 = uint64_t(int64_t(-2112159744));
  // lis r5,-32229
  r5 = uint64_t(int64_t(-2112159744));
  // lis r6,-32229
  r6 = uint64_t(int64_t(-2112159744));
  // lis r7,-32229
  r7 = uint64_t(int64_t(-2112159744));
  // stw r14,304(r1)
  REX_STORE_U32(uint32_t(r1) + 304, uint32_t(r14));
  // lis r8,-32229
  r8 = uint64_t(int64_t(-2112159744));
  // lwz r14,88(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 88);
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // addi r14,r14,10496
  r14 = uint64_t(int64_t(int64_t(r14) + 10496));
  // stw r14,308(r1)
  REX_STORE_U32(uint32_t(r1) + 308, uint32_t(r14));
  // lwz r14,84(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 84);
  // addi r14,r14,-16280
  r14 = uint64_t(int64_t(int64_t(r14) + -16280));
  // stw r14,312(r1)
  REX_STORE_U32(uint32_t(r1) + 312, uint32_t(r14));
  // lwz r14,108(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 108);
  // addi r14,r14,10024
  r14 = uint64_t(int64_t(int64_t(r14) + 10024));
  // stw r14,316(r1)
  REX_STORE_U32(uint32_t(r1) + 316, uint32_t(r14));
  // lwz r14,92(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 92);
  // addi r14,r14,10704
  r14 = uint64_t(int64_t(int64_t(r14) + 10704));
  // stw r14,320(r1)
  REX_STORE_U32(uint32_t(r1) + 320, uint32_t(r14));
  // lwz r14,100(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 100);
  // addi r14,r14,23536
  r14 = uint64_t(int64_t(int64_t(r14) + 23536));
  // stw r14,324(r1)
  REX_STORE_U32(uint32_t(r1) + 324, uint32_t(r14));
  // lwz r14,80(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 80);
  // addi r14,r14,10848
  r14 = uint64_t(int64_t(int64_t(r14) + 10848));
  // stw r14,328(r1)
  REX_STORE_U32(uint32_t(r1) + 328, uint32_t(r14));
  // lwz r14,116(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 116);
  // addi r14,r14,-16136
  r14 = uint64_t(int64_t(int64_t(r14) + -16136));
  // stw r14,332(r1)
  REX_STORE_U32(uint32_t(r1) + 332, uint32_t(r14));
  // lwz r14,144(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 144);
  // addi r14,r14,11056
  r14 = uint64_t(int64_t(int64_t(r14) + 11056));
  // stw r14,336(r1)
  REX_STORE_U32(uint32_t(r1) + 336, uint32_t(r14));
  // lwz r14,128(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 128);
  // addi r14,r14,11384
  r14 = uint64_t(int64_t(int64_t(r14) + 11384));
  // stw r14,340(r1)
  REX_STORE_U32(uint32_t(r1) + 340, uint32_t(r14));
  // lwz r14,140(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 140);
  // addi r14,r14,23784
  r14 = uint64_t(int64_t(int64_t(r14) + 23784));
  // stw r14,344(r1)
  REX_STORE_U32(uint32_t(r1) + 344, uint32_t(r14));
  // lwz r14,136(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 136);
  // addi r14,r14,11528
  r14 = uint64_t(int64_t(int64_t(r14) + 11528));
  // stw r14,348(r1)
  REX_STORE_U32(uint32_t(r1) + 348, uint32_t(r14));
  // lwz r14,120(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 120);
  // addi r14,r14,-16136
  r14 = uint64_t(int64_t(int64_t(r14) + -16136));
  // stw r14,352(r1)
  REX_STORE_U32(uint32_t(r1) + 352, uint32_t(r14));
  // lwz r14,124(r1)
  r14 = REX_LOAD_U32(uint32_t(r1) + 124);
  // addi r14,r14,11736
  r14 = uint64_t(int64_t(int64_t(r14) + 11736));
  // stw r14,356(r1)
  REX_STORE_U32(uint32_t(r1) + 356, uint32_t(r14));
  // addi r3,r3,24808
  r3 = uint64_t(int64_t(int64_t(r3) + 24808));
  // addi r16,r16,24056
  r16 = uint64_t(int64_t(int64_t(r16) + 24056));
  // lis r15,-32229
  r15 = uint64_t(int64_t(-2112159744));
  // addi r17,r17,12208
  r17 = uint64_t(int64_t(int64_t(r17) + 12208));
  // addi r18,r18,-15992
  r18 = uint64_t(int64_t(int64_t(r18) + -15992));
  // stw r3,424(r1)
  REX_STORE_U32(uint32_t(r1) + 424, uint32_t(r3));
  // lis r3,-32229
  r3 = uint64_t(int64_t(-2112159744));
  // stw r16,364(r1)
  REX_STORE_U32(uint32_t(r1) + 364, uint32_t(r16));
  // lis r16,-32229
  r16 = uint64_t(int64_t(-2112159744));
  // addi r3,r3,14496
  r3 = uint64_t(int64_t(int64_t(r3) + 14496));
  // stw r17,368(r1)
  REX_STORE_U32(uint32_t(r1) + 368, uint32_t(r17));
  // lis r17,-32229
  r17 = uint64_t(int64_t(-2112159744));
  // stw r18,372(r1)
  REX_STORE_U32(uint32_t(r1) + 372, uint32_t(r18));
  // addi r19,r19,15464
  r19 = uint64_t(int64_t(int64_t(r19) + 15464));
  // lis r18,-32229
  r18 = uint64_t(int64_t(-2112159744));
  // lis r14,-32229
  r14 = uint64_t(int64_t(-2112159744));
  // stw r3,456(r1)
  REX_STORE_U32(uint32_t(r1) + 456, uint32_t(r3));
  // addi r3,r15,14144
  r3 = uint64_t(int64_t(int64_t(r15) + 14144));
  // addi r20,r20,12416
  r20 = uint64_t(int64_t(int64_t(r20) + 12416));
  // addi r21,r21,24328
  r21 = uint64_t(int64_t(int64_t(r21) + 24328));
  // stw r19,376(r1)
  REX_STORE_U32(uint32_t(r1) + 376, uint32_t(r19));
  // addi r22,r22,12560
  r22 = uint64_t(int64_t(int64_t(r22) + 12560));
  // addi r23,r23,-15992
  r23 = uint64_t(int64_t(int64_t(r23) + -15992));
  // stw r3,460(r1)
  REX_STORE_U32(uint32_t(r1) + 460, uint32_t(r3));
  // addi r3,r16,25288
  r3 = uint64_t(int64_t(int64_t(r16) + 25288));
  // addi r24,r24,15464
  r24 = uint64_t(int64_t(int64_t(r24) + 15464));
  // stw r20,380(r1)
  REX_STORE_U32(uint32_t(r1) + 380, uint32_t(r20));
  // addi r25,r25,12768
  r25 = uint64_t(int64_t(int64_t(r25) + 12768));
  // stw r21,384(r1)
  REX_STORE_U32(uint32_t(r1) + 384, uint32_t(r21));
  // addi r26,r26,24568
  r26 = uint64_t(int64_t(int64_t(r26) + 24568));
  // stw r22,388(r1)
  REX_STORE_U32(uint32_t(r1) + 388, uint32_t(r22));
  // addi r27,r27,12912
  r27 = uint64_t(int64_t(int64_t(r27) + 12912));
  // stw r23,392(r1)
  REX_STORE_U32(uint32_t(r1) + 392, uint32_t(r23));
  // stw r3,464(r1)
  REX_STORE_U32(uint32_t(r1) + 464, uint32_t(r3));
  // addi r3,r17,14288
  r3 = uint64_t(int64_t(int64_t(r17) + 14288));
  // addi r28,r28,-15992
  r28 = uint64_t(int64_t(int64_t(r28) + -15992));
  // stw r24,396(r1)
  REX_STORE_U32(uint32_t(r1) + 396, uint32_t(r24));
  // addi r29,r29,15464
  r29 = uint64_t(int64_t(int64_t(r29) + 15464));
  // stw r25,400(r1)
  REX_STORE_U32(uint32_t(r1) + 400, uint32_t(r25));
  // addi r30,r30,13120
  r30 = uint64_t(int64_t(int64_t(r30) + 13120));
  // stw r26,404(r1)
  REX_STORE_U32(uint32_t(r1) + 404, uint32_t(r26));
  // addi r4,r4,13264
  r4 = uint64_t(int64_t(int64_t(r4) + 13264));
  // stw r27,408(r1)
  REX_STORE_U32(uint32_t(r1) + 408, uint32_t(r27));
  // stw r3,468(r1)
  REX_STORE_U32(uint32_t(r1) + 468, uint32_t(r3));
  // addi r5,r5,-16136
  r5 = uint64_t(int64_t(int64_t(r5) + -16136));
  // addi r6,r6,13472
  r6 = uint64_t(int64_t(int64_t(r6) + 13472));
  // stw r28,412(r1)
  REX_STORE_U32(uint32_t(r1) + 412, uint32_t(r28));
  // addi r7,r7,13792
  r7 = uint64_t(int64_t(int64_t(r7) + 13792));
  // stw r29,416(r1)
  REX_STORE_U32(uint32_t(r1) + 416, uint32_t(r29));
  // addi r8,r8,25048
  r8 = uint64_t(int64_t(int64_t(r8) + 25048));
  // stw r30,420(r1)
  REX_STORE_U32(uint32_t(r1) + 420, uint32_t(r30));
  // addi r9,r9,13936
  r9 = uint64_t(int64_t(int64_t(r9) + 13936));
  // stw r4,428(r1)
  REX_STORE_U32(uint32_t(r1) + 428, uint32_t(r4));
  // addi r3,r18,-15848
  r3 = uint64_t(int64_t(int64_t(r18) + -15848));
  // stw r5,432(r1)
  REX_STORE_U32(uint32_t(r1) + 432, uint32_t(r5));
  // addi r14,r14,-15848
  r14 = uint64_t(int64_t(int64_t(r14) + -15848));
  // stw r6,436(r1)
  REX_STORE_U32(uint32_t(r1) + 436, uint32_t(r6));
  // lis r19,-32229
  r19 = uint64_t(int64_t(-2112159744));
  // stw r7,440(r1)
  REX_STORE_U32(uint32_t(r1) + 440, uint32_t(r7));
  // stw r8,444(r1)
  REX_STORE_U32(uint32_t(r1) + 444, uint32_t(r8));
  // lis r20,-32229
  r20 = uint64_t(int64_t(-2112159744));
  // stw r9,448(r1)
  REX_STORE_U32(uint32_t(r1) + 448, uint32_t(r9));
  // lis r21,-32229
  r21 = uint64_t(int64_t(-2112159744));
  // stw r3,472(r1)
  REX_STORE_U32(uint32_t(r1) + 472, uint32_t(r3));
  // lis r22,-32229
  r22 = uint64_t(int64_t(-2112159744));
  // lis r23,-32229
  r23 = uint64_t(int64_t(-2112159744));
  // stw r14,452(r1)
  REX_STORE_U32(uint32_t(r1) + 452, uint32_t(r14));
  // lis r24,-32229
  r24 = uint64_t(int64_t(-2112159744));
  // lis r25,-32229
  r25 = uint64_t(int64_t(-2112159744));
  // lis r26,-32229
  r26 = uint64_t(int64_t(-2112159744));
  // lis r27,-32229
  r27 = uint64_t(int64_t(-2112159744));
  // lis r28,-32229
  r28 = uint64_t(int64_t(-2112159744));
  // lis r29,-32229
  r29 = uint64_t(int64_t(-2112159744));
  // lis r30,-32229
  r30 = uint64_t(int64_t(-2112159744));
  // lis r4,-32229
  r4 = uint64_t(int64_t(-2112159744));
  // lis r5,-32229
  r5 = uint64_t(int64_t(-2112159744));
  // lis r6,-32229
  r6 = uint64_t(int64_t(-2112159744));
  // lis r7,-32229
  r7 = uint64_t(int64_t(-2112159744));
  // lis r8,-32229
  r8 = uint64_t(int64_t(-2112159744));
  // lis r9,-32229
  r9 = uint64_t(int64_t(-2112159744));
  // addi r3,r19,14496
  r3 = uint64_t(int64_t(int64_t(r19) + 14496));
  // stw r3,476(r1)
  REX_STORE_U32(uint32_t(r1) + 476, uint32_t(r3));
  // addi r3,r20,14760
  r3 = uint64_t(int64_t(int64_t(r20) + 14760));
  // addi r4,r4,26104
  r4 = uint64_t(int64_t(int64_t(r4) + 26104));
  // addi r5,r5,15872
  r5 = uint64_t(int64_t(int64_t(r5) + 15872));
  // addi r6,r6,-15704
  r6 = uint64_t(int64_t(int64_t(r6) + -15704));
  // addi r7,r7,16080
  r7 = uint64_t(int64_t(int64_t(r7) + 16080));
  // stw r3,480(r1)
  REX_STORE_U32(uint32_t(r1) + 480, uint32_t(r3));
  // addi r3,r21,25560
  r3 = uint64_t(int64_t(int64_t(r21) + 25560));
  // addi r8,r8,16344
  r8 = uint64_t(int64_t(int64_t(r8) + 16344));
  // stw r4,524(r1)
  REX_STORE_U32(uint32_t(r1) + 524, uint32_t(r4));
  // addi r9,r9,26376
  r9 = uint64_t(int64_t(int64_t(r9) + 26376));
  // stw r5,528(r1)
  REX_STORE_U32(uint32_t(r1) + 528, uint32_t(r5));
  // stw r6,532(r1)
  REX_STORE_U32(uint32_t(r1) + 532, uint32_t(r6));
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // stw r7,536(r1)
  REX_STORE_U32(uint32_t(r1) + 536, uint32_t(r7));
  // stw r3,484(r1)
  REX_STORE_U32(uint32_t(r1) + 484, uint32_t(r3));
  // addi r3,r22,14904
  r3 = uint64_t(int64_t(int64_t(r22) + 14904));
  // stw r8,540(r1)
  REX_STORE_U32(uint32_t(r1) + 540, uint32_t(r8));
  // stw r9,544(r1)
  REX_STORE_U32(uint32_t(r1) + 544, uint32_t(r9));
  // stw r3,488(r1)
  REX_STORE_U32(uint32_t(r1) + 488, uint32_t(r3));
  // addi r3,r23,-15848
  r3 = uint64_t(int64_t(int64_t(r23) + -15848));
  // stw r3,492(r1)
  REX_STORE_U32(uint32_t(r1) + 492, uint32_t(r3));
  // addi r3,r24,14496
  r3 = uint64_t(int64_t(int64_t(r24) + 14496));
  // stw r3,496(r1)
  REX_STORE_U32(uint32_t(r1) + 496, uint32_t(r3));
  // addi r3,r25,15112
  r3 = uint64_t(int64_t(int64_t(r25) + 15112));
  // stw r3,500(r1)
  REX_STORE_U32(uint32_t(r1) + 500, uint32_t(r3));
  // addi r3,r26,25832
  r3 = uint64_t(int64_t(int64_t(r26) + 25832));
  // stw r3,504(r1)
  REX_STORE_U32(uint32_t(r1) + 504, uint32_t(r3));
  // addi r3,r27,15256
  r3 = uint64_t(int64_t(int64_t(r27) + 15256));
  // stw r3,508(r1)
  REX_STORE_U32(uint32_t(r1) + 508, uint32_t(r3));
  // addi r3,r28,-15992
  r3 = uint64_t(int64_t(int64_t(r28) + -15992));
  // stw r3,512(r1)
  REX_STORE_U32(uint32_t(r1) + 512, uint32_t(r3));
  // addi r3,r29,15464
  r3 = uint64_t(int64_t(int64_t(r29) + 15464));
  // stw r3,516(r1)
  REX_STORE_U32(uint32_t(r1) + 516, uint32_t(r3));
  // addi r3,r30,15728
  r3 = uint64_t(int64_t(int64_t(r30) + 15728));
  // stw r3,520(r1)
  REX_STORE_U32(uint32_t(r1) + 520, uint32_t(r3));
  // beq cr6,0x821b6e54
  if (cr6.eq) goto loc_821B6E54;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x821b6e30
  if (cr6.lt) goto loc_821B6E30;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,160
  r9 = uint64_t(int64_t(int64_t(r1) + 160));
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwzx r11,r10,r9
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r9));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31656(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31656);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b6e6c
  if (cr0.eq) goto loc_821B6E6C;
  // lwz r9,16(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x821b6e14
  if (!cr6.eq) goto loc_821B6E14;
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // b 0x821b6e24
  goto loc_821B6E24;
loc_821B6E14:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_821B6E18:
  // lwz r9,16(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 16);
  // stw r8,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r8));
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
loc_821B6E24:
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
loc_821B6E28:
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // b 0x821b6e6c
  goto loc_821B6E6C;
loc_821B6E30:
  // lwz r9,16(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x821b6e18
  if (!cr6.eq) goto loc_821B6E18;
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b6e28
  goto loc_821B6E28;
loc_821B6E54:
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x821b6e6c
  if (cr6.eq) goto loc_821B6E6C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,132(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 132);
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B6E6C:
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r10,560(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 560);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821b6f98
  if (cr6.eq) goto loc_821B6F98;
  // lwz r10,744(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 744);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821b6f58
  if (!cr6.eq) goto loc_821B6F58;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // ori r10,r11,39200
  r10 = r11 | 39200;
  // addi r11,r3,6120
  r11 = uint64_t(int64_t(int64_t(r3) + 6120));
  // lwzx r9,r3,r10
  r9 = REX_LOAD_U32(uint32_t(r3) + uint32_t(r10));
  // cmplwi r9,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r9), 0, so);
  // lha r10,204(r11)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r11) + 204))));
  // b 0x821b6eb8
  goto loc_821B6EB8;
loc_821B6EB0:
  // lha r10,0(r10)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 0))));
  // addic. r9,r9,-1
  ca = uint32_t(r9) > 0;
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
loc_821B6EB8:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r11
  r10 = r10 + r11;
  // bne 0x821b6eb0
  if (!cr0.eq) goto loc_821B6EB0;
  // lha r10,2(r10)
  r10 = uint64_t(int64_t(int16_t(REX_LOAD_U16(uint32_t(r10) + 2))));
  // addi r3,r1,560
  r3 = uint64_t(int64_t(int64_t(r1) + 560));
  // li r5,1968
  r5 = uint64_t(int64_t(1968));
  // mulli r10,r10,1968
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(1968))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // addi r4,r11,212
  r4 = uint64_t(int64_t(int64_t(r11) + 212));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,572(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 572);
  // bl 0x8215aad0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8215AAD0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // mr r4,r3
  r4 = r3;
  // lwz r11,-31776(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x8217f880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8217F880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r3,744(r11)
  REX_STORE_U32(uint32_t(r11) + 744, uint32_t(r3));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // lwz r10,744(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 744);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821b6f98
  if (cr6.eq) goto loc_821B6F98;
  // addi r3,r1,2528
  r3 = uint64_t(int64_t(int64_t(r1) + 2528));
  // bl 0x820cd5d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820CD5D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,2528
  r3 = uint64_t(int64_t(int64_t(r1) + 2528));
  // addi r4,r1,560
  r4 = uint64_t(int64_t(int64_t(r1) + 560));
  // li r5,1884
  r5 = uint64_t(int64_t(1884));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,6416(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 6416);
  // addi r3,r1,2528
  r3 = uint64_t(int64_t(int64_t(r1) + 2528));
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,6416(r1)
  REX_STORE_U32(uint32_t(r1) + 6416, uint32_t(r11));
  // bl 0x820c4998
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820C4998, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31776(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31776);
  // li r5,-1
  r5 = uint64_t(int64_t(-1));
  // addi r4,r1,2528
  r4 = uint64_t(int64_t(int64_t(r1) + 2528));
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x8217fd58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8217FD58, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821b6f94
  goto loc_821B6F94;
loc_821B6F58:
  // lwz r11,740(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 740);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // addi r31,r11,-31776
  r31 = uint64_t(int64_t(int64_t(r11) + -31776));
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bne cr6,0x821b6f7c
  if (!cr6.eq) goto loc_821B6F7C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_821B6F7C:
  // bl 0x82178b40
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82178B40, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bl 0x8217fe60
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8217FE60, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B6F94:
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
loc_821B6F98:
  // lwz r11,760(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 760);
  // cmpwi cr6,r11,21
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 21, so);
  // beq cr6,0x821b6fb8
  if (cr6.eq) goto loc_821B6FB8;
  // lwz r9,6612(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + 6612);
  // li r10,21
  r10 = uint64_t(int64_t(21));
  // stw r11,9604(r9)
  REX_STORE_U32(uint32_t(r9) + 9604, uint32_t(r11));
  // lwz r11,-31648(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31648);
  // stw r10,760(r11)
  REX_STORE_U32(uint32_t(r11) + 760, uint32_t(r10));
loc_821B6FB8:
  // addi r1,r1,6592
  r1 = uint64_t(int64_t(int64_t(r1) + 6592));
  // b 0x825a6ea0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B8238: lifted from generated/ by lift.py.
void f821B8238(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821B8238 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  // lwz r11,9608(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 9608);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821b8280
  if (cr6.eq) goto loc_821B8280;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31644(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31644);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821b825c
  if (cr0.eq) goto loc_821B825C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821B825C:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r8,9604(r3)
  REX_STORE_U32(uint32_t(r3) + 9604, uint32_t(r8));
  // blr
  return;
loc_821B8280:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9604(r3)
  REX_STORE_U32(uint32_t(r3) + 9604, uint32_t(r11));
  // blr
  return;
}

// sub_821B83A8: lifted from generated/ by lift.py.
lifted::Result f821B83A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B83A8 (lifted)");
  uint64_t r4 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lwz r3,-31640(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31640);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31636(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31636);
  // lwz r11,96(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 96);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821b8404
  if (cr6.lt) goto loc_821B8404;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // bge cr6,0x821b84b0
  if (!cr6.lt) goto loc_821B84B0;
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31640(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31640);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b842c
  if (!cr0.eq) goto loc_821B842C;
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // b 0x821b841c
  goto loc_821B841C;
loc_821B8404:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31640(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31640);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821b842c
  if (!cr0.eq) goto loc_821B842C;
  // li r4,163
  r4 = uint64_t(int64_t(163));
loc_821B841C:
  // lwz r3,-31640(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31640);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b84b0
  if (cr0.eq) goto loc_821B84B0;
loc_821B842C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31644(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31644);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821b8444
  if (cr6.eq) goto loc_821B8444;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821B8444:
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // addi r11,r30,204
  r11 = uint64_t(int64_t(int64_t(r30) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r7,25
  r7 = uint64_t(int64_t(25));
  // stw r8,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r8));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // stw r9,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r9));
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // lwz r10,600(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 600);
  // lwz r9,604(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 604);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // add r9,r9,r10
  r9 = r9 + r10;
  // addi r9,r9,-1
  r9 = uint64_t(int64_t(int64_t(r9) + -1));
  // stw r10,600(r11)
  REX_STORE_U32(uint32_t(r11) + 600, uint32_t(r10));
  // divwu r5,r9,r5
  r5 = uint32_t(uint32_t(r5) ? uint32_t(r9) / uint32_t(r5) : 0);
  // mulli r5,r5,50
  r5 = uint64_t(int64_t(static_cast<int64_t>(r5 * static_cast<uint64_t>(50))));
  // subf r10,r5,r9
  r10 = r9 - r5;
  // mulli r10,r10,12
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(12))));
  // add r11,r10,r11
  r11 = r10 + r11;
  // stw r7,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r7));
  // stw r6,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r6));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B84B0:
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

// sub_821B84C8: lifted from generated/ by lift.py.
void f821B84C8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821B84C8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // li r6,9
  r6 = uint64_t(int64_t(9));
  // addi r11,r3,204
  r11 = uint64_t(int64_t(int64_t(r3) + 204));
  // li r5,50
  r5 = uint64_t(int64_t(50));
  // li r9,26
  r9 = uint64_t(int64_t(26));
  // lwz r10,-31636(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31636);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r6,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r6));
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
  // blr
  return;
}

// sub_821B8588: lifted from generated/ by lift.py.
lifted::Result f821B8588(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B8588 (lifted)");
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
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b8600
  if (!cr6.eq) goto loc_821B8600;
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lwz r11,-31636(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31636);
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r9));
loc_821B8600:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
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

// sub_821B8630: lifted from generated/ by lift.py.
uint64_t f821B8630(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821B8630 (lifted)");
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r31,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r31));
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

// sub_821B8688: lifted from generated/ by lift.py.
uint64_t f821B8688(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821B8688 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r27,r3
  r27 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r28,0(r3)
  r28 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // lwz r11,104(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 104);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble cr6,0x821b8720
  if (!cr6.gt) goto loc_821B8720;
loc_821B86D8:
  // lwz r11,100(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 100);
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r28
  r3 = r28;
  // lwz r29,0(r27)
  r29 = REX_LOAD_U32(uint32_t(r27) + 0);
  // add r5,r11,r30
  r5 = r11 + r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,10
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 10, so);
  // beq cr6,0x821b8704
  if (cr6.eq) goto loc_821B8704;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r29,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r29));
  // b 0x821b8708
  goto loc_821B8708;
loc_821B8704:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821B8708:
  // stw r11,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r11));
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // lwz r11,104(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 104);
  // addi r27,r27,4
  r27 = uint64_t(int64_t(int64_t(r27) + 4));
  // cmpw cr6,r30,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r30), int32_t(r11), so);
  // blt cr6,0x821b86d8
  if (cr6.lt) goto loc_821B86D8;
loc_821B8720:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_821B8728: lifted from generated/ by lift.py.
uint64_t f821B8728(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821B8728 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821B876C:
  // lwz r11,100(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 100);
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r3,r31
  r3 = r31;
  // lwz r28,0(r29)
  r28 = REX_LOAD_U32(uint32_t(r29) + 0);
  // add r5,r30,r11
  r5 = r30 + r11;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r28,37
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 37, so);
  // bge cr6,0x821b8798
  if (!cr6.lt) goto loc_821B8798;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r28,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r28));
  // b 0x821b879c
  goto loc_821B879C;
loc_821B8798:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821B879C:
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // stw r11,80(r3)
  REX_STORE_U32(uint32_t(r3) + 80, uint32_t(r11));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // cmpwi cr6,r30,7
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 7, so);
  // blt cr6,0x821b876c
  if (cr6.lt) goto loc_821B876C;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return r3;
}

// sub_821B87B8: lifted from generated/ by lift.py.
uint64_t f821B87B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821B87B8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r9,r3,2
  r9 = uint64_t(int64_t(int64_t(r3) + 2));
loc_821B87D0:
  // lbzx r8,r9,r11
  r8 = REX_LOAD_U8(uint32_t(r9) + uint32_t(r11));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // extsb r8,r8
  r8 = uint64_t(int64_t(int8_t(r8)));
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // stw r8,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r8));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // blt cr6,0x821b87d0
  if (cr6.lt) goto loc_821B87D0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r9,r3,5
  r9 = uint64_t(int64_t(int64_t(r3) + 5));
loc_821B87F8:
  // lbzx r8,r9,r11
  r8 = REX_LOAD_U8(uint32_t(r9) + uint32_t(r11));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // extsb r8,r8
  r8 = uint64_t(int64_t(int8_t(r8)));
  // cmpwi cr6,r11,7
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 7, so);
  // stw r8,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r8));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // blt cr6,0x821b87f8
  if (cr6.lt) goto loc_821B87F8;
  // lbz r11,1(r3)
  r11 = REX_LOAD_U8(uint32_t(r3) + 1);
  // extsb r3,r11
  r3 = uint64_t(int64_t(int8_t(r11)));
  // bl 0x821b8630
  // (left out: dead in C++)
  r3 = f821B8630(base, r1, r13, r3);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x821b8688
  // (left out: dead in C++)
  r3 = f821B8688(base, r1, r13, r3);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // bl 0x821b8728
  // (left out: dead in C++)
  r3 = f821B8728(base, r1, r13, r3);
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return r3;
}

// sub_821B8840: lifted from generated/ by lift.py.
lifted::Result f821B8840(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B8840 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, ctr = 0;
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
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r9,-32228
  r9 = uint64_t(int64_t(-2112094208));
  // lis r10,-32228
  r10 = uint64_t(int64_t(-2112094208));
  // addi r9,r9,-31832
  r9 = uint64_t(int64_t(int64_t(r9) + -31832));
  // addi r10,r10,-31544
  r10 = uint64_t(int64_t(int64_t(r10) + -31544));
  // lwz r11,-31644(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31644);
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821b8890
  if (cr6.eq) goto loc_821B8890;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r4
  r3 = r4;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B8890:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821B8ED0: lifted from generated/ by lift.py.
lifted::Result f821B8ED0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821B8ED0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r29,1
  r29 = uint64_t(int64_t(1));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821b8f34
  if (!cr6.eq) goto loc_821B8F34;
  // bl 0x821b8988
  // (left out: dead in C++)
  { const lifted::Result o = f821B8988(base, r1, r13, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r29,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r29));
loc_821B8F34:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm. r3,r11,27,31,31
  r3 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b8f58
  if (cr0.eq) goto loc_821B8F58;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31636(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31636);
  // stw r29,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r29));
loc_821B8F58:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821B8F60: lifted from generated/ by lift.py.
lifted::Result f821B8F60(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821B8F60 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // mr r30,r3
  r30 = r3;
  // addi r6,r11,-23200
  r6 = uint64_t(int64_t(int64_t(r11) + -23200));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // li r3,100
  r3 = uint64_t(int64_t(100));
  // bl 0x82106540
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82106540, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lis r10,-32228
  r10 = uint64_t(int64_t(-2112094208));
  // lis r9,-32228
  r9 = uint64_t(int64_t(-2112094208));
  // addi r10,r10,-31352
  r10 = uint64_t(int64_t(int64_t(r10) + -31352));
  // addi r9,r9,-28976
  r9 = uint64_t(int64_t(int64_t(r9) + -28976));
  // lwz r11,-31644(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31644);
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // stw r9,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r9));
  // lwz r10,12(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 12);
  // cmpwi cr6,r10,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), -1, so);
  // beq cr6,0x821b9060
  if (cr6.eq) goto loc_821B9060;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // blt cr6,0x821b903c
  if (cr6.lt) goto loc_821B903C;
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,80
  r9 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,24
  r3 = uint64_t(int64_t(int64_t(r11) + 24));
  // lwzx r11,r10,r9
  r11 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r9));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31644(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31644);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821b9068
  if (cr0.eq) goto loc_821B9068;
  // lwz r9,16(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x821b9020
  if (!cr6.eq) goto loc_821B9020;
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // b 0x821b9030
  goto loc_821B9030;
loc_821B9020:
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
loc_821B9024:
  // lwz r9,16(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 16);
  // stw r8,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r8));
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
loc_821B9030:
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
loc_821B9034:
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
  // b 0x821b9068
  goto loc_821B9068;
loc_821B903C:
  // lwz r9,16(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 16);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,-1
  r8 = uint64_t(int64_t(-1));
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // bne cr6,0x821b9024
  if (!cr6.eq) goto loc_821B9024;
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821b9034
  goto loc_821B9034;
loc_821B9060:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B9068:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31636(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31636);
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r9,21
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 21, so);
  // beq cr6,0x821b9090
  if (cr6.eq) goto loc_821B9090;
  // stw r9,9604(r30)
  REX_STORE_U32(uint32_t(r30) + 9604, uint32_t(r9));
  // li r8,21
  r8 = uint64_t(int64_t(21));
  // lwz r11,-31636(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31636);
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // lwz r11,-31636(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31636);
loc_821B9090:
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821b90c0
  if (cr6.eq) goto loc_821B90C0;
  // addi r6,r11,112
  r6 = uint64_t(int64_t(int64_t(r11) + 112));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31776);
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f2,-23204(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -23204);
  f2 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f1,-23208(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -23208);
  f1 = double(temp.f32);
  // bl 0x8217c708
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8217C708, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821B90C0:
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

// sub_821B9F98: lifted from generated/ by lift.py.
void f821B9F98(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821B9F98 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // stw r10,708(r11)
  REX_STORE_U32(uint32_t(r11) + 708, uint32_t(r10));
  // blr
  return;
}

// sub_821BA010: lifted from generated/ by lift.py.
lifted::Result f821BA010(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821BA010 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // lwz r11,700(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 700);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ba070
  if (!cr0.eq) goto loc_821BA070;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r1,84
  r4 = uint64_t(int64_t(int64_t(r1) + 84));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23680(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 23680);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r10,700(r11)
  REX_STORE_U32(uint32_t(r11) + 700, uint32_t(r10));
  // b 0x821ba0d8
  goto loc_821BA0D8;
loc_821BA070:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821ba0d8
  if (!cr6.eq) goto loc_821BA0D8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r31,2
  r31 = uint64_t(int64_t(2));
  // addi r5,r1,88
  r5 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r4,r1,92
  r4 = uint64_t(int64_t(int64_t(r1) + 92));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // stw r31,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r31));
  // lwz r3,23680(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 23680);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ba0d8
  if (!cr0.eq) goto loc_821BA0D8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31632(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31632);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821ba0c8
  if (cr6.eq) goto loc_821BA0C8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821BA0C8:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
loc_821BA0D8:
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

// sub_821BA0F0: lifted from generated/ by lift.py.
lifted::Result f821BA0F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_821BA0F0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e64
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-320(r1)
  ea = -320 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // lwz r11,700(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 700);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ba14c
  if (!cr0.eq) goto loc_821BA14C;
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // addi r5,r1,84
  r5 = uint64_t(int64_t(int64_t(r1) + 84));
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // stw r11,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r11));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23680(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 23680);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // li r10,1
  r10 = uint64_t(int64_t(1));
loc_821BA144:
  // stw r10,700(r11)
  REX_STORE_U32(uint32_t(r11) + 700, uint32_t(r10));
  // b 0x821ba374
  goto loc_821BA374;
loc_821BA14C:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // li r30,2
  r30 = uint64_t(int64_t(2));
  // bne cr6,0x821ba1d0
  if (!cr6.eq) goto loc_821BA1D0;
  // lis r29,-32148
  r29 = uint64_t(int64_t(-2106851328));
  // stw r30,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r30));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r5,r1,92
  r5 = uint64_t(int64_t(int64_t(r1) + 92));
  // addi r4,r1,96
  r4 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r3,23680(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 23680);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ba374
  if (!cr0.eq) goto loc_821BA374;
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // lwz r10,696(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 696);
  // lwz r9,692(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 692);
  // cmpw cr6,r10,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r9), so);
  // bne cr6,0x821ba1a8
  if (!cr6.eq) goto loc_821BA1A8;
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // b 0x821ba144
  goto loc_821BA144;
loc_821BA1A8:
  // lwz r3,23680(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 23680);
  // addi r4,r1,100
  r4 = uint64_t(int64_t(int64_t(r1) + 100));
  // stw r30,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r30));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // stw r30,700(r11)
  REX_STORE_U32(uint32_t(r11) + 700, uint32_t(r30));
  // b 0x821ba374
  goto loc_821BA374;
loc_821BA1D0:
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // lwz r3,23680(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 23680);
  // bne cr6,0x821ba314
  if (!cr6.eq) goto loc_821BA314;
  // stw r30,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r30));
  // addi r4,r1,104
  r4 = uint64_t(int64_t(int64_t(r1) + 104));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ba374
  if (cr0.eq) goto loc_821BA374;
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // stw r30,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r30));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r4,r5,27280
  r4 = uint64_t(int64_t(int64_t(r5) + 27280));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // addi r30,r5,27268
  r30 = uint64_t(int64_t(int64_t(r5) + 27268));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // stb r11,80(r1)
  REX_STORE_U8(uint32_t(r1) + 80, uint8_t(r11));
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r29,r5,27256
  r29 = uint64_t(int64_t(int64_t(r5) + 27256));
  // stw r4,152(r1)
  REX_STORE_U32(uint32_t(r1) + 152, uint32_t(r4));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // stw r30,156(r1)
  REX_STORE_U32(uint32_t(r1) + 156, uint32_t(r30));
  // addi r28,r5,27364
  r28 = uint64_t(int64_t(int64_t(r5) + 27364));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // lwz r11,692(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 692);
  // lis r7,-32256
  r7 = uint64_t(int64_t(-2113929216));
  // stw r29,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r29));
  // addi r27,r5,27244
  r27 = uint64_t(int64_t(int64_t(r5) + 27244));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // rlwinm r19,r11,2,0,29
  r19 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stw r28,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r28));
  // addi r26,r5,27232
  r26 = uint64_t(int64_t(int64_t(r5) + 27232));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stw r27,168(r1)
  REX_STORE_U32(uint32_t(r1) + 168, uint32_t(r27));
  // addi r25,r5,27220
  r25 = uint64_t(int64_t(int64_t(r5) + 27220));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // stw r26,172(r1)
  REX_STORE_U32(uint32_t(r1) + 172, uint32_t(r26));
  // addi r24,r5,27208
  r24 = uint64_t(int64_t(int64_t(r5) + 27208));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,27352
  r11 = uint64_t(int64_t(int64_t(r11) + 27352));
  // stw r25,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r25));
  // addi r23,r5,27196
  r23 = uint64_t(int64_t(int64_t(r5) + 27196));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // addi r10,r10,27340
  r10 = uint64_t(int64_t(int64_t(r10) + 27340));
  // stw r24,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r24));
  // addi r22,r5,27184
  r22 = uint64_t(int64_t(int64_t(r5) + 27184));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // stw r11,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r11));
  // addi r9,r9,27328
  r9 = uint64_t(int64_t(int64_t(r9) + 27328));
  // stw r23,184(r1)
  REX_STORE_U32(uint32_t(r1) + 184, uint32_t(r23));
  // addi r21,r5,27172
  r21 = uint64_t(int64_t(int64_t(r5) + 27172));
  // addi r8,r8,27316
  r8 = uint64_t(int64_t(int64_t(r8) + 27316));
  // stw r10,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r10));
  // addi r7,r7,27304
  r7 = uint64_t(int64_t(int64_t(r7) + 27304));
  // stw r22,188(r1)
  REX_STORE_U32(uint32_t(r1) + 188, uint32_t(r22));
  // addi r6,r6,27292
  r6 = uint64_t(int64_t(int64_t(r6) + 27292));
  // addi r20,r1,128
  r20 = uint64_t(int64_t(int64_t(r1) + 128));
  // stw r9,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r9));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r21,192(r1)
  REX_STORE_U32(uint32_t(r1) + 192, uint32_t(r21));
  // addi r3,r1,108
  r3 = uint64_t(int64_t(int64_t(r1) + 108));
  // stw r8,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r8));
  // stw r7,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r7));
  // stw r6,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r6));
  // lwzx r4,r19,r20
  r4 = REX_LOAD_U32(uint32_t(r19) + uint32_t(r20));
  // bl 0x821063b0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821063B0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // lwz r10,692(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 692);
  // stw r10,696(r11)
  REX_STORE_U32(uint32_t(r11) + 696, uint32_t(r10));
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // stw r9,700(r11)
  REX_STORE_U32(uint32_t(r11) + 700, uint32_t(r9));
  // b 0x821ba374
  goto loc_821BA374;
loc_821BA314:
  // stw r30,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r30));
  // addi r4,r1,112
  r4 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ba374
  if (!cr0.eq) goto loc_821BA374;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r30,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r30));
  // addi r4,r1,108
  r4 = uint64_t(int64_t(int64_t(r1) + 108));
  // addi r3,r1,116
  r3 = uint64_t(int64_t(int64_t(r1) + 116));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // bl 0x82175850
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82175850, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31632(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31632);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821ba364
  if (cr6.eq) goto loc_821BA364;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821BA364:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
loc_821BA374:
  // addi r1,r1,320
  r1 = uint64_t(int64_t(int64_t(r1) + 320));
  // b 0x825a6eb4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821BA5F0: lifted from generated/ by lift.py.
uint64_t f821BA5F0(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821BA5F0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // addi r10,r1,-48
  r10 = uint64_t(int64_t(int64_t(r1) + -48));
  // stw r11,-48(r1)
  REX_STORE_U32(uint32_t(r1) + -48, uint32_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,-44(r1)
  REX_STORE_U32(uint32_t(r1) + -44, uint32_t(r11));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,-40(r1)
  REX_STORE_U32(uint32_t(r1) + -40, uint32_t(r11));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r11,-36(r1)
  REX_STORE_U32(uint32_t(r1) + -36, uint32_t(r11));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r11,-32(r1)
  REX_STORE_U32(uint32_t(r1) + -32, uint32_t(r11));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,-28(r1)
  REX_STORE_U32(uint32_t(r1) + -28, uint32_t(r11));
  // li r11,12
  r11 = uint64_t(int64_t(12));
  // stw r11,-24(r1)
  REX_STORE_U32(uint32_t(r1) + -24, uint32_t(r11));
  // li r11,13
  r11 = uint64_t(int64_t(13));
  // stw r11,-20(r1)
  REX_STORE_U32(uint32_t(r1) + -20, uint32_t(r11));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // stw r11,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r11));
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821BA64C:
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpw cr6,r8,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r3), so);
  // beq cr6,0x821ba66c
  if (cr6.eq) goto loc_821BA66C;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // ble cr6,0x821ba64c
  if (!cr6.gt) goto loc_821BA64C;
  // b 0x821ba670
  goto loc_821BA670;
loc_821BA66C:
  // mr r9,r11
  r9 = r11;
loc_821BA670:
  // add r11,r9,r4
  r11 = r9 + r4;
  // cmpwi cr6,r11,9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 9, so);
  // ble cr6,0x821ba680
  if (!cr6.gt) goto loc_821BA680;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821BA680:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x821ba68c
  if (!cr6.lt) goto loc_821BA68C;
  // li r11,9
  r11 = uint64_t(int64_t(9));
loc_821BA68C:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,-48
  r10 = uint64_t(int64_t(int64_t(r1) + -48));
  // lwzx r3,r11,r10
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // blr
  return r3;
}

// sub_821BA6A0: lifted from generated/ by lift.py.
uint64_t f821BA6A0(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821BA6A0 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // stw r11,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r11));
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821BA6BC:
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpw cr6,r8,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r3), so);
  // beq cr6,0x821ba6dc
  if (cr6.eq) goto loc_821BA6DC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // ble cr6,0x821ba6bc
  if (!cr6.gt) goto loc_821BA6BC;
  // b 0x821ba6e0
  goto loc_821BA6E0;
loc_821BA6DC:
  // mr r9,r11
  r9 = r11;
loc_821BA6E0:
  // add r11,r9,r4
  r11 = r9 + r4;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // ble cr6,0x821ba6f0
  if (!cr6.gt) goto loc_821BA6F0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821BA6F0:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x821ba6fc
  if (!cr6.lt) goto loc_821BA6FC;
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821BA6FC:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // lwzx r3,r11,r10
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // blr
  return r3;
}

// sub_821BA710: lifted from generated/ by lift.py.
uint64_t f821BA710(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821BA710 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // addi r10,r1,-64
  r10 = uint64_t(int64_t(int64_t(r1) + -64));
  // stw r11,-64(r1)
  REX_STORE_U32(uint32_t(r1) + -64, uint32_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,-60(r1)
  REX_STORE_U32(uint32_t(r1) + -60, uint32_t(r11));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,-56(r1)
  REX_STORE_U32(uint32_t(r1) + -56, uint32_t(r11));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r11,-52(r1)
  REX_STORE_U32(uint32_t(r1) + -52, uint32_t(r11));
  // li r11,6
  r11 = uint64_t(int64_t(6));
  // stw r11,-48(r1)
  REX_STORE_U32(uint32_t(r1) + -48, uint32_t(r11));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,-44(r1)
  REX_STORE_U32(uint32_t(r1) + -44, uint32_t(r11));
  // li r11,12
  r11 = uint64_t(int64_t(12));
  // stw r11,-40(r1)
  REX_STORE_U32(uint32_t(r1) + -40, uint32_t(r11));
  // li r11,13
  r11 = uint64_t(int64_t(13));
  // stw r11,-36(r1)
  REX_STORE_U32(uint32_t(r1) + -36, uint32_t(r11));
  // li r11,15
  r11 = uint64_t(int64_t(15));
  // stw r11,-32(r1)
  REX_STORE_U32(uint32_t(r1) + -32, uint32_t(r11));
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r11,-28(r1)
  REX_STORE_U32(uint32_t(r1) + -28, uint32_t(r11));
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // stw r11,-24(r1)
  REX_STORE_U32(uint32_t(r1) + -24, uint32_t(r11));
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r11,-20(r1)
  REX_STORE_U32(uint32_t(r1) + -20, uint32_t(r11));
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // stw r11,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r11));
  // li r11,11
  r11 = uint64_t(int64_t(11));
  // stw r11,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821BA78C:
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpw cr6,r8,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r3), so);
  // beq cr6,0x821ba7ac
  if (cr6.eq) goto loc_821BA7AC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpwi cr6,r11,13
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 13, so);
  // ble cr6,0x821ba78c
  if (!cr6.gt) goto loc_821BA78C;
  // b 0x821ba7b0
  goto loc_821BA7B0;
loc_821BA7AC:
  // mr r9,r11
  r9 = r11;
loc_821BA7B0:
  // add r11,r9,r4
  r11 = r9 + r4;
  // cmpwi cr6,r11,13
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 13, so);
  // ble cr6,0x821ba7c0
  if (!cr6.gt) goto loc_821BA7C0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821BA7C0:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x821ba7cc
  if (!cr6.lt) goto loc_821BA7CC;
  // li r11,13
  r11 = uint64_t(int64_t(13));
loc_821BA7CC:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,-64
  r10 = uint64_t(int64_t(int64_t(r1) + -64));
  // lwzx r3,r11,r10
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // blr
  return r3;
}

// sub_821BAD80: lifted from generated/ by lift.py.
uint64_t f821BAD80(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_821BAD80 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // li r11,17
  r11 = uint64_t(int64_t(17));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // addi r10,r1,-32
  r10 = uint64_t(int64_t(int64_t(r1) + -32));
  // stw r11,-32(r1)
  REX_STORE_U32(uint32_t(r1) + -32, uint32_t(r11));
  // li r11,18
  r11 = uint64_t(int64_t(18));
  // stw r11,-28(r1)
  REX_STORE_U32(uint32_t(r1) + -28, uint32_t(r11));
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,-24(r1)
  REX_STORE_U32(uint32_t(r1) + -24, uint32_t(r11));
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // stw r11,-20(r1)
  REX_STORE_U32(uint32_t(r1) + -20, uint32_t(r11));
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // stw r11,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r11));
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // stw r11,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821BADBC:
  // lwz r8,0(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpw cr6,r8,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r3), so);
  // beq cr6,0x821baddc
  if (cr6.eq) goto loc_821BADDC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // ble cr6,0x821badbc
  if (!cr6.gt) goto loc_821BADBC;
  // b 0x821bade0
  goto loc_821BADE0;
loc_821BADDC:
  // mr r9,r11
  r9 = r11;
loc_821BADE0:
  // add r11,r9,r4
  r11 = r9 + r4;
  // cmpwi cr6,r11,5
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 5, so);
  // ble cr6,0x821badf0
  if (!cr6.gt) goto loc_821BADF0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821BADF0:
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x821badfc
  if (!cr6.lt) goto loc_821BADFC;
  // li r11,5
  r11 = uint64_t(int64_t(5));
loc_821BADFC:
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,-32
  r10 = uint64_t(int64_t(int64_t(r1) + -32));
  // lwzx r3,r11,r10
  r3 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // blr
  return r3;
}

// sub_821BB420: lifted from generated/ by lift.py.
uint64_t f821BB420(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BB420 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // lwz r8,340(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 340);
  // lwz r10,332(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 332);
  // lwz r9,336(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 336);
  // stw r8,-40(r1)
  REX_STORE_U32(uint32_t(r1) + -40, uint32_t(r8));
  // lwz r8,344(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 344);
  // stw r10,-48(r1)
  REX_STORE_U32(uint32_t(r1) + -48, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r9,-44(r1)
  REX_STORE_U32(uint32_t(r1) + -44, uint32_t(r9));
  // addi r9,r1,-48
  r9 = uint64_t(int64_t(int64_t(r1) + -48));
  // stw r8,-36(r1)
  REX_STORE_U32(uint32_t(r1) + -36, uint32_t(r8));
  // lwz r8,348(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 348);
  // stw r8,-32(r1)
  REX_STORE_U32(uint32_t(r1) + -32, uint32_t(r8));
  // lwz r8,352(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 352);
  // stw r8,-28(r1)
  REX_STORE_U32(uint32_t(r1) + -28, uint32_t(r8));
  // lwz r8,368(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 368);
  // stw r8,-24(r1)
  REX_STORE_U32(uint32_t(r1) + -24, uint32_t(r8));
  // lwz r8,360(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 360);
  // stw r8,-20(r1)
  REX_STORE_U32(uint32_t(r1) + -20, uint32_t(r8));
  // lwz r8,364(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 364);
  // stw r8,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r8));
  // lwz r8,356(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 356);
  // stw r8,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r8));
  // lwz r8,372(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 372);
  // lwz r11,376(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 376);
  // stw r8,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r8));
  // stw r11,-4(r1)
  REX_STORE_U32(uint32_t(r1) + -4, uint32_t(r11));
loc_821BB490:
  // cmpw cr6,r10,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r3), so);
  // beq cr6,0x821bb4b0
  if (cr6.eq) goto loc_821BB4B0;
  // rlwinm r11,r3,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // lwz r7,0(r9)
  r7 = REX_LOAD_U32(uint32_t(r9) + 0);
  // addi r8,r1,-48
  r8 = uint64_t(int64_t(int64_t(r1) + -48));
  // lwzx r11,r11,r8
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r8));
  // cmpw cr6,r7,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r11), so);
  // beq cr6,0x821bb4c8
  if (cr6.eq) goto loc_821BB4C8;
loc_821BB4B0:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // blt cr6,0x821bb490
  if (cr6.lt) goto loc_821BB490;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_821BB4C8:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_821BB4D0: lifted from generated/ by lift.py.
uint64_t f821BB4D0(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821BB4D0 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // lwz r8,340(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 340);
  // lwz r10,332(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 332);
  // stw r8,-40(r1)
  REX_STORE_U32(uint32_t(r1) + -40, uint32_t(r8));
  // lwz r8,344(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 344);
  // stw r10,-48(r1)
  REX_STORE_U32(uint32_t(r1) + -48, uint32_t(r10));
  // lwz r10,336(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 336);
  // stw r8,-36(r1)
  REX_STORE_U32(uint32_t(r1) + -36, uint32_t(r8));
  // lwz r8,348(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 348);
  // stw r10,-44(r1)
  REX_STORE_U32(uint32_t(r1) + -44, uint32_t(r10));
  // addi r10,r1,-48
  r10 = uint64_t(int64_t(int64_t(r1) + -48));
  // stw r8,-32(r1)
  REX_STORE_U32(uint32_t(r1) + -32, uint32_t(r8));
  // lwz r8,352(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 352);
  // stw r8,-28(r1)
  REX_STORE_U32(uint32_t(r1) + -28, uint32_t(r8));
  // lwz r8,368(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 368);
  // stw r8,-24(r1)
  REX_STORE_U32(uint32_t(r1) + -24, uint32_t(r8));
  // lwz r8,360(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 360);
  // stw r8,-20(r1)
  REX_STORE_U32(uint32_t(r1) + -20, uint32_t(r8));
  // lwz r8,364(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 364);
  // stw r8,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r8));
  // lwz r8,356(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 356);
  // stw r8,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r8));
  // lwz r8,372(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 372);
  // lwz r11,376(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 376);
  // stw r8,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r8));
  // stw r11,-4(r1)
  REX_STORE_U32(uint32_t(r1) + -4, uint32_t(r11));
loc_821BB540:
  // mr r8,r9
  r8 = r9;
  // cmpwi cr6,r9,12
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 12, so);
  // bge cr6,0x821bb570
  if (!cr6.lt) goto loc_821BB570;
  // lwz r7,0(r10)
  r7 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r10,4
  r11 = uint64_t(int64_t(int64_t(r10) + 4));
loc_821BB554:
  // lwz r6,0(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r7,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r6), so);
  // beq cr6,0x821bb588
  if (cr6.eq) goto loc_821BB588;
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmpwi cr6,r8,12
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 12, so);
  // blt cr6,0x821bb554
  if (cr6.lt) goto loc_821BB554;
loc_821BB570:
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpwi cr6,r9,12
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 12, so);
  // blt cr6,0x821bb540
  if (cr6.lt) goto loc_821BB540;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_821BB588:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_821BB5F8: lifted from generated/ by lift.py.
uint64_t f821BB5F8(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BB5F8 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // lwz r8,412(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 412);
  // lwz r10,404(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 404);
  // lwz r9,408(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 408);
  // stw r8,-24(r1)
  REX_STORE_U32(uint32_t(r1) + -24, uint32_t(r8));
  // lwz r8,420(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 420);
  // stw r10,-32(r1)
  REX_STORE_U32(uint32_t(r1) + -32, uint32_t(r10));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r9,-28(r1)
  REX_STORE_U32(uint32_t(r1) + -28, uint32_t(r9));
  // addi r9,r1,-32
  r9 = uint64_t(int64_t(int64_t(r1) + -32));
  // stw r8,-20(r1)
  REX_STORE_U32(uint32_t(r1) + -20, uint32_t(r8));
  // lwz r8,416(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 416);
  // lwz r11,424(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 424);
  // stw r8,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r8));
  // stw r11,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r11));
loc_821BB638:
  // cmpw cr6,r10,r3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r3), so);
  // beq cr6,0x821bb658
  if (cr6.eq) goto loc_821BB658;
  // rlwinm r11,r3,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r3) | (r3 << 32), 2) & 0xFFFFFFFC;
  // lwz r7,0(r9)
  r7 = REX_LOAD_U32(uint32_t(r9) + 0);
  // addi r8,r1,-32
  r8 = uint64_t(int64_t(int64_t(r1) + -32));
  // lwzx r11,r11,r8
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r8));
  // cmpw cr6,r7,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r11), so);
  // beq cr6,0x821bb670
  if (cr6.eq) goto loc_821BB670;
loc_821BB658:
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmpwi cr6,r10,6
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 6, so);
  // blt cr6,0x821bb638
  if (cr6.lt) goto loc_821BB638;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_821BB670:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_821BB678: lifted from generated/ by lift.py.
uint64_t f821BB678(uint8_t* base, uint64_t r1) {
  ITC_NATIVE_RUNNING("sub_821BB678 (lifted)");
  uint64_t r3 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // lwz r8,412(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 412);
  // lwz r10,404(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 404);
  // stw r8,-24(r1)
  REX_STORE_U32(uint32_t(r1) + -24, uint32_t(r8));
  // lwz r8,420(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 420);
  // stw r10,-32(r1)
  REX_STORE_U32(uint32_t(r1) + -32, uint32_t(r10));
  // lwz r10,408(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 408);
  // stw r8,-20(r1)
  REX_STORE_U32(uint32_t(r1) + -20, uint32_t(r8));
  // lwz r8,416(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 416);
  // lwz r11,424(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 424);
  // stw r10,-28(r1)
  REX_STORE_U32(uint32_t(r1) + -28, uint32_t(r10));
  // addi r10,r1,-32
  r10 = uint64_t(int64_t(int64_t(r1) + -32));
  // stw r8,-16(r1)
  REX_STORE_U32(uint32_t(r1) + -16, uint32_t(r8));
  // stw r11,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r11));
loc_821BB6B8:
  // mr r8,r9
  r8 = r9;
  // cmpwi cr6,r9,6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 6, so);
  // bge cr6,0x821bb6e8
  if (!cr6.lt) goto loc_821BB6E8;
  // lwz r7,0(r10)
  r7 = REX_LOAD_U32(uint32_t(r10) + 0);
  // addi r11,r10,4
  r11 = uint64_t(int64_t(int64_t(r10) + 4));
loc_821BB6CC:
  // lwz r6,0(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r7,r6
  cr6 = lifted::Compare<int32_t>(int32_t(r7), int32_t(r6), so);
  // beq cr6,0x821bb700
  if (cr6.eq) goto loc_821BB700;
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmpwi cr6,r8,6
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 6, so);
  // blt cr6,0x821bb6cc
  if (cr6.lt) goto loc_821BB6CC;
loc_821BB6E8:
  // addi r9,r9,1
  r9 = uint64_t(int64_t(int64_t(r9) + 1));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // cmpwi cr6,r9,6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 6, so);
  // blt cr6,0x821bb6b8
  if (cr6.lt) goto loc_821BB6B8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_821BB700:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // blr
  return r3;
}

// sub_821BD680: lifted from generated/ by lift.py.
lifted::Result f821BD680(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821BD680 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821bd6e8
  if (!cr6.eq) goto loc_821BD6E8;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dff8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DFF8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // li r3,15
  r3 = uint64_t(int64_t(15));
  // bl 0x8209d0c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8209D0C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821BD6E8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_821BDA38: lifted from generated/ by lift.py.
lifted::Result f821BDA38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821BDA38 (lifted)");
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821bdaa8
  if (!cr6.eq) goto loc_821BDAA8;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,341
  r4 = uint64_t(int64_t(341));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,341
  r4 = uint64_t(int64_t(341));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821BDAA8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_821BDC18: lifted from generated/ by lift.py.
lifted::Result f821BDC18(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821BDC18 (lifted)");
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821bdc88
  if (!cr6.eq) goto loc_821BDC88;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,510
  r4 = uint64_t(int64_t(510));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,510
  r4 = uint64_t(int64_t(510));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821BDC88:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_821BDCB0: lifted from generated/ by lift.py.
lifted::Result f821BDCB0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821BDCB0 (lifted)");
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
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
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
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r10,459
  r10 = uint64_t(int64_t(459));
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // li r10,463
  r10 = uint64_t(int64_t(463));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // li r10,467
  r10 = uint64_t(int64_t(467));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // li r10,471
  r10 = uint64_t(int64_t(471));
  // stw r10,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r10));
  // li r10,475
  r10 = uint64_t(int64_t(475));
  // stw r10,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r10));
  // li r10,479
  r10 = uint64_t(int64_t(479));
  // stw r10,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r10));
  // li r10,483
  r10 = uint64_t(int64_t(483));
  // stw r10,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r10));
  // li r10,487
  r10 = uint64_t(int64_t(487));
  // stw r10,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r10));
  // li r10,491
  r10 = uint64_t(int64_t(491));
  // stw r10,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r10));
  // li r10,495
  r10 = uint64_t(int64_t(495));
  // stw r10,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r10));
  // li r10,499
  r10 = uint64_t(int64_t(499));
  // stw r10,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r10));
  // li r10,503
  r10 = uint64_t(int64_t(503));
  // stw r10,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r10));
  // li r10,507
  r10 = uint64_t(int64_t(507));
  // stw r10,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r10));
  // bne cr6,0x821bde70
  if (!cr6.eq) goto loc_821BDE70;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31628(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31628);
  // lwz r30,4(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r28,0(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r30,12
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 12, so);
  // bge cr6,0x821bdd7c
  if (!cr6.lt) goto loc_821BDD7C;
  // addi r4,r30,510
  r4 = uint64_t(int64_t(int64_t(r30) + 510));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821bddcc
  goto loc_821BDDCC;
loc_821BDD7C:
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r4,528
  r4 = uint64_t(int64_t(528));
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821bdd94
  if (cr6.eq) goto loc_821BDD94;
  // li r4,527
  r4 = uint64_t(int64_t(527));
loc_821BDD94:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,526
  r4 = uint64_t(int64_t(526));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,526
  r4 = uint64_t(int64_t(526));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,660(r11)
  REX_STORE_U32(uint32_t(r11) + 660, uint32_t(r10));
loc_821BDDCC:
  // cmpwi cr6,r28,12
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 12, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bge cr6,0x821bddfc
  if (!cr6.lt) goto loc_821BDDFC;
  // addi r30,r28,510
  r30 = uint64_t(int64_t(int64_t(r28) + 510));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // b 0x821bde60
  goto loc_821BDE60;
loc_821BDDFC:
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwzx r30,r11,r10
  r30 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821bde54
  if (!cr6.eq) goto loc_821BDE54;
  // li r4,528
  r4 = uint64_t(int64_t(528));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,528
  r4 = uint64_t(int64_t(528));
  // b 0x821bde60
  goto loc_821BDE60;
loc_821BDE54:
  // li r4,527
  r4 = uint64_t(int64_t(527));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,527
  r4 = uint64_t(int64_t(527));
loc_821BDE60:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
loc_821BDE70:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821BDFA8: lifted from generated/ by lift.py.
lifted::Result f821BDFA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821BDFA8 (lifted)");
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821be018
  if (!cr6.eq) goto loc_821BE018;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,749
  r4 = uint64_t(int64_t(749));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,749
  r4 = uint64_t(int64_t(749));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821BE018:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_821BE040: lifted from generated/ by lift.py.
lifted::Result f821BE040(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821BE040 (lifted)");
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
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
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
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r10,698
  r10 = uint64_t(int64_t(698));
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // li r10,702
  r10 = uint64_t(int64_t(702));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // li r10,706
  r10 = uint64_t(int64_t(706));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // li r10,710
  r10 = uint64_t(int64_t(710));
  // stw r10,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r10));
  // li r10,714
  r10 = uint64_t(int64_t(714));
  // stw r10,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r10));
  // li r10,718
  r10 = uint64_t(int64_t(718));
  // stw r10,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r10));
  // li r10,722
  r10 = uint64_t(int64_t(722));
  // stw r10,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r10));
  // li r10,726
  r10 = uint64_t(int64_t(726));
  // stw r10,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r10));
  // li r10,730
  r10 = uint64_t(int64_t(730));
  // stw r10,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r10));
  // li r10,734
  r10 = uint64_t(int64_t(734));
  // stw r10,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r10));
  // li r10,738
  r10 = uint64_t(int64_t(738));
  // stw r10,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r10));
  // li r10,742
  r10 = uint64_t(int64_t(742));
  // stw r10,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r10));
  // li r10,746
  r10 = uint64_t(int64_t(746));
  // stw r10,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r10));
  // bne cr6,0x821be200
  if (!cr6.eq) goto loc_821BE200;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31628(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31628);
  // lwz r30,4(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r28,0(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r30,6
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 6, so);
  // bge cr6,0x821be10c
  if (!cr6.lt) goto loc_821BE10C;
  // addi r4,r30,749
  r4 = uint64_t(int64_t(int64_t(r30) + 749));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821be15c
  goto loc_821BE15C;
loc_821BE10C:
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r4,767
  r4 = uint64_t(int64_t(767));
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821be124
  if (cr6.eq) goto loc_821BE124;
  // li r4,766
  r4 = uint64_t(int64_t(766));
loc_821BE124:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,765
  r4 = uint64_t(int64_t(765));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,765
  r4 = uint64_t(int64_t(765));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,660(r11)
  REX_STORE_U32(uint32_t(r11) + 660, uint32_t(r10));
loc_821BE15C:
  // cmpwi cr6,r28,6
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 6, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bge cr6,0x821be18c
  if (!cr6.lt) goto loc_821BE18C;
  // addi r30,r28,749
  r30 = uint64_t(int64_t(int64_t(r28) + 749));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // b 0x821be1f0
  goto loc_821BE1F0;
loc_821BE18C:
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwzx r30,r11,r10
  r30 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821be1e4
  if (!cr6.eq) goto loc_821BE1E4;
  // li r4,767
  r4 = uint64_t(int64_t(767));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,767
  r4 = uint64_t(int64_t(767));
  // b 0x821be1f0
  goto loc_821BE1F0;
loc_821BE1E4:
  // li r4,766
  r4 = uint64_t(int64_t(766));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,766
  r4 = uint64_t(int64_t(766));
loc_821BE1F0:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
loc_821BE200:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821BE4A0: lifted from generated/ by lift.py.
uint64_t f821BE4A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BE4A0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stfd f31,-24(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // extsw r11,r31
  r11 = uint64_t(int64_t(int32_t(r31)));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_821BE7A8: lifted from generated/ by lift.py.
uint64_t f821BE7A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BE7A8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stfd f31,-24(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // extsw r11,r31
  r11 = uint64_t(int64_t(int32_t(r31)));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_821BEDC8: lifted from generated/ by lift.py.
uint64_t f821BEDC8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BEDC8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stfd f31,-24(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // extsw r11,r31
  r11 = uint64_t(int64_t(int32_t(r31)));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_821BEE48: lifted from generated/ by lift.py.
lifted::Result f821BEE48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821BEE48 (lifted)");
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // lwz r10,260(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 260);
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lwz r10,264(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 264);
  // lwz r11,268(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 268);
  // stw r10,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r10));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,88
  r30 = uint64_t(int64_t(int64_t(r1) + 88));
loc_821BEEA8:
  // addi r4,r31,4
  r4 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821be988
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BE988, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,3
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 3, so);
  // blt cr6,0x821beea8
  if (cr6.lt) goto loc_821BEEA8;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821BEED8: lifted from generated/ by lift.py.
uint64_t f821BEED8(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821BEED8 (lifted)");
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // lwz r10,260(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 260);
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lwz r10,264(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 264);
  // lwz r11,268(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 268);
  // stw r10,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r10));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,88
  r30 = uint64_t(int64_t(int64_t(r1) + 88));
loc_821BEF38:
  // addi r4,r31,4
  r4 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821bea48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BEA48, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,3
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 3, so);
  // blt cr6,0x821bef38
  if (cr6.lt) goto loc_821BEF38;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_821BEF68: lifted from generated/ by lift.py.
uint64_t f821BEF68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BEF68 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // std r31,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r31);
  // stfd f31,-24(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -24, std::bit_cast<uint64_t>(f31));
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // extsw r11,r31
  r11 = uint64_t(int64_t(int32_t(r31)));
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stfs f31,16(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r3) + 16, temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // lfd f31,-24(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -24)));
  // ld r31,-16(r1)
  r31 = REX_LOAD_U64(uint32_t(r1) + -16);
  // blr
  return r3;
}

// sub_821BF2B8: lifted from generated/ by lift.py.
lifted::Result f821BF2B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821BF2B8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r28,0(r3)
  r28 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r26,r3
  r26 = r3;
  // li r31,348
  r31 = uint64_t(int64_t(348));
  // addi r29,r1,96
  r29 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r30,12
  r30 = uint64_t(int64_t(12));
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // lwz r10,284(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 284);
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // lwz r10,288(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 288);
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // lwz r10,292(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 292);
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // lwz r10,296(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 296);
  // stw r10,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r10));
  // lwz r10,300(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 300);
  // stw r10,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r10));
  // lwz r10,304(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 304);
  // stw r10,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r10));
  // lwz r10,320(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 320);
  // stw r10,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r10));
  // lwz r10,312(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 312);
  // stw r10,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r10));
  // lwz r10,316(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 316);
  // stw r10,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r10));
  // lwz r10,308(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 308);
  // stw r10,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r10));
  // lwz r10,324(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 324);
  // lwz r11,328(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 328);
  // stw r10,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r10));
  // stw r11,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r11));
loc_821BF368:
  // addi r4,r31,-347
  r4 = uint64_t(int64_t(int64_t(r31) + -347));
  // mr r3,r26
  r3 = r26;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r27,r3
  r27 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r31
  r5 = r31;
  // mr r3,r28
  r3 = r28;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r27
  r3 = r27;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // stw r11,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r11));
  // bne 0x821bf368
  if (!cr0.eq) goto loc_821BF368;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821BF3C0: lifted from generated/ by lift.py.
lifted::Result f821BF3C0(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_821BF3C0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,460
  r11 = uint64_t(int64_t(460));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // stw r11,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r11));
  // li r11,464
  r11 = uint64_t(int64_t(464));
  // stw r11,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r11));
  // li r11,468
  r11 = uint64_t(int64_t(468));
  // stw r11,168(r1)
  REX_STORE_U32(uint32_t(r1) + 168, uint32_t(r11));
  // li r11,472
  r11 = uint64_t(int64_t(472));
  // stw r11,172(r1)
  REX_STORE_U32(uint32_t(r1) + 172, uint32_t(r11));
  // li r11,476
  r11 = uint64_t(int64_t(476));
  // stw r11,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r11));
  // li r11,480
  r11 = uint64_t(int64_t(480));
  // stw r11,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r11));
  // li r11,484
  r11 = uint64_t(int64_t(484));
  // stw r11,184(r1)
  REX_STORE_U32(uint32_t(r1) + 184, uint32_t(r11));
  // li r11,488
  r11 = uint64_t(int64_t(488));
  // stw r11,188(r1)
  REX_STORE_U32(uint32_t(r1) + 188, uint32_t(r11));
  // li r11,492
  r11 = uint64_t(int64_t(492));
  // stw r11,192(r1)
  REX_STORE_U32(uint32_t(r1) + 192, uint32_t(r11));
  // li r11,496
  r11 = uint64_t(int64_t(496));
  // stw r11,196(r1)
  REX_STORE_U32(uint32_t(r1) + 196, uint32_t(r11));
  // li r11,500
  r11 = uint64_t(int64_t(500));
  // stw r11,200(r1)
  REX_STORE_U32(uint32_t(r1) + 200, uint32_t(r11));
  // li r11,504
  r11 = uint64_t(int64_t(504));
  // stw r11,204(r1)
  REX_STORE_U32(uint32_t(r1) + 204, uint32_t(r11));
  // li r11,508
  r11 = uint64_t(int64_t(508));
  // stw r11,208(r1)
  REX_STORE_U32(uint32_t(r1) + 208, uint32_t(r11));
  // li r11,461
  r11 = uint64_t(int64_t(461));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,465
  r11 = uint64_t(int64_t(465));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // li r11,469
  r11 = uint64_t(int64_t(469));
  // stw r11,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r11));
  // li r11,473
  r11 = uint64_t(int64_t(473));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // li r11,477
  r11 = uint64_t(int64_t(477));
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
  // li r11,481
  r11 = uint64_t(int64_t(481));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // li r11,485
  r11 = uint64_t(int64_t(485));
  // stw r11,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r11));
  // li r11,489
  r11 = uint64_t(int64_t(489));
  // stw r11,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r11));
  // li r11,493
  r11 = uint64_t(int64_t(493));
  // stw r11,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r11));
  // li r11,497
  r11 = uint64_t(int64_t(497));
  // stw r11,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r11));
  // li r11,501
  r11 = uint64_t(int64_t(501));
  // stw r11,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r11));
  // li r11,505
  r11 = uint64_t(int64_t(505));
  // stw r11,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r11));
  // li r11,509
  r11 = uint64_t(int64_t(509));
  // stw r11,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r11));
loc_821BF4C0:
  // mr r3,r28
  r3 = r28;
  // bl 0x821ba7e0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BA7E0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // beq cr6,0x821bf4ec
  if (cr6.eq) goto loc_821BF4EC;
  // addi r11,r1,160
  r11 = uint64_t(int64_t(int64_t(r1) + 160));
loc_821BF4EC:
  // lwzx r29,r31,r11
  r29 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // mr r4,r29
  r4 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // cmpwi cr6,r31,48
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 48, so);
  // blt cr6,0x821bf4c0
  if (cr6.lt) goto loc_821BF4C0;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821BF7F8: lifted from generated/ by lift.py.
lifted::Result f821BF7F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821BF7F8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r27,r3
  r27 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // lwz r10,380(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 380);
  // stw r10,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r10));
  // lwz r10,384(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 384);
  // stw r10,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r10));
  // lwz r10,388(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 388);
  // stw r10,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r10));
  // li r10,351
  r10 = uint64_t(int64_t(351));
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // lwz r10,396(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 396);
  // stw r10,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r10));
  // li r10,352
  r10 = uint64_t(int64_t(352));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // lwz r10,392(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 392);
  // lwz r11,400(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 400);
  // stw r10,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r10));
  // li r10,353
  r10 = uint64_t(int64_t(353));
  // stw r11,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r11));
  // li r11,354
  r11 = uint64_t(int64_t(354));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // li r11,355
  r11 = uint64_t(int64_t(355));
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
  // li r11,359
  r11 = uint64_t(int64_t(359));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821BF8A4:
  // addi r29,r11,1
  r29 = uint64_t(int64_t(int64_t(r11) + 1));
  // mr r3,r27
  r3 = r27;
  // mr r4,r29
  r4 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r28,r3
  r28 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // lwzx r5,r31,r11
  r5 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r28,r1,128
  r28 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwzx r10,r31,r28
  r10 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r28));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r11,r29
  r11 = r29;
  // cmpwi cr6,r31,24
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 24, so);
  // stw r10,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r10));
  // blt cr6,0x821bf8a4
  if (cr6.lt) goto loc_821BF8A4;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821BF908: lifted from generated/ by lift.py.
lifted::Result f821BF908(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_821BF908 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,699
  r11 = uint64_t(int64_t(699));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // stw r11,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r11));
  // li r11,703
  r11 = uint64_t(int64_t(703));
  // stw r11,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r11));
  // li r11,707
  r11 = uint64_t(int64_t(707));
  // stw r11,168(r1)
  REX_STORE_U32(uint32_t(r1) + 168, uint32_t(r11));
  // li r11,711
  r11 = uint64_t(int64_t(711));
  // stw r11,172(r1)
  REX_STORE_U32(uint32_t(r1) + 172, uint32_t(r11));
  // li r11,715
  r11 = uint64_t(int64_t(715));
  // stw r11,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r11));
  // li r11,719
  r11 = uint64_t(int64_t(719));
  // stw r11,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r11));
  // li r11,723
  r11 = uint64_t(int64_t(723));
  // stw r11,184(r1)
  REX_STORE_U32(uint32_t(r1) + 184, uint32_t(r11));
  // li r11,727
  r11 = uint64_t(int64_t(727));
  // stw r11,188(r1)
  REX_STORE_U32(uint32_t(r1) + 188, uint32_t(r11));
  // li r11,731
  r11 = uint64_t(int64_t(731));
  // stw r11,192(r1)
  REX_STORE_U32(uint32_t(r1) + 192, uint32_t(r11));
  // li r11,735
  r11 = uint64_t(int64_t(735));
  // stw r11,196(r1)
  REX_STORE_U32(uint32_t(r1) + 196, uint32_t(r11));
  // li r11,739
  r11 = uint64_t(int64_t(739));
  // stw r11,200(r1)
  REX_STORE_U32(uint32_t(r1) + 200, uint32_t(r11));
  // li r11,743
  r11 = uint64_t(int64_t(743));
  // stw r11,204(r1)
  REX_STORE_U32(uint32_t(r1) + 204, uint32_t(r11));
  // li r11,747
  r11 = uint64_t(int64_t(747));
  // stw r11,208(r1)
  REX_STORE_U32(uint32_t(r1) + 208, uint32_t(r11));
  // li r11,700
  r11 = uint64_t(int64_t(700));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,704
  r11 = uint64_t(int64_t(704));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // li r11,708
  r11 = uint64_t(int64_t(708));
  // stw r11,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r11));
  // li r11,712
  r11 = uint64_t(int64_t(712));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // li r11,716
  r11 = uint64_t(int64_t(716));
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
  // li r11,720
  r11 = uint64_t(int64_t(720));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // li r11,724
  r11 = uint64_t(int64_t(724));
  // stw r11,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r11));
  // li r11,728
  r11 = uint64_t(int64_t(728));
  // stw r11,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r11));
  // li r11,732
  r11 = uint64_t(int64_t(732));
  // stw r11,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r11));
  // li r11,736
  r11 = uint64_t(int64_t(736));
  // stw r11,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r11));
  // li r11,740
  r11 = uint64_t(int64_t(740));
  // stw r11,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r11));
  // li r11,744
  r11 = uint64_t(int64_t(744));
  // stw r11,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r11));
  // li r11,748
  r11 = uint64_t(int64_t(748));
  // stw r11,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r11));
loc_821BFA08:
  // mr r3,r28
  r3 = r28;
  // bl 0x821bab58
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BAB58, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // beq cr6,0x821bfa34
  if (cr6.eq) goto loc_821BFA34;
  // addi r11,r1,160
  r11 = uint64_t(int64_t(int64_t(r1) + 160));
loc_821BFA34:
  // lwzx r29,r31,r11
  r29 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // mr r4,r29
  r4 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // cmpwi cr6,r31,24
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 24, so);
  // blt cr6,0x821bfa08
  if (cr6.lt) goto loc_821BFA08;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821BFBD8: lifted from generated/ by lift.py.
uint64_t f821BFBD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BFBD8 (lifted)");
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821bfc48
  if (!cr6.eq) goto loc_821BFC48;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,346
  r4 = uint64_t(int64_t(346));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,346
  r4 = uint64_t(int64_t(346));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821BFC48:
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
  return r3;
}

// sub_821BFC68: lifted from generated/ by lift.py.
lifted::Result f821BFC68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821BFC68 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821bfcf0
  if (!cr6.eq) goto loc_821BFCF0;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31600(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31600);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821bfce0
  if (cr6.lt) goto loc_821BFCE0;
  // beq cr6,0x821bfcd8
  if (cr6.eq) goto loc_821BFCD8;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821bfcd0
  if (cr6.lt) goto loc_821BFCD0;
  // bne cr6,0x821bfce8
  if (!cr6.eq) goto loc_821BFCE8;
  // li r4,350
  r4 = uint64_t(int64_t(350));
  // b 0x821bfce4
  goto loc_821BFCE4;
loc_821BFCD0:
  // li r4,348
  r4 = uint64_t(int64_t(348));
  // b 0x821bfce4
  goto loc_821BFCE4;
loc_821BFCD8:
  // li r4,347
  r4 = uint64_t(int64_t(347));
  // b 0x821bfce4
  goto loc_821BFCE4;
loc_821BFCE0:
  // li r4,346
  r4 = uint64_t(int64_t(346));
loc_821BFCE4:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821BFCE8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821BFCF0:
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_821BFD08: lifted from generated/ by lift.py.
uint64_t f821BFD08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BFD08 (lifted)");
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821bfd78
  if (!cr6.eq) goto loc_821BFD78;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,581
  r4 = uint64_t(int64_t(581));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,581
  r4 = uint64_t(int64_t(581));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821BFD78:
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
  return r3;
}

// sub_821BFD98: lifted from generated/ by lift.py.
lifted::Result f821BFD98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821BFD98 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r10,530
  r10 = uint64_t(int64_t(530));
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // li r10,534
  r10 = uint64_t(int64_t(534));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // li r10,538
  r10 = uint64_t(int64_t(538));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // li r10,542
  r10 = uint64_t(int64_t(542));
  // stw r10,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r10));
  // li r10,546
  r10 = uint64_t(int64_t(546));
  // stw r10,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r10));
  // li r10,550
  r10 = uint64_t(int64_t(550));
  // stw r10,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r10));
  // li r10,554
  r10 = uint64_t(int64_t(554));
  // stw r10,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r10));
  // li r10,558
  r10 = uint64_t(int64_t(558));
  // stw r10,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r10));
  // li r10,562
  r10 = uint64_t(int64_t(562));
  // stw r10,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r10));
  // li r10,566
  r10 = uint64_t(int64_t(566));
  // stw r10,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r10));
  // li r10,570
  r10 = uint64_t(int64_t(570));
  // stw r10,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r10));
  // li r10,574
  r10 = uint64_t(int64_t(574));
  // stw r10,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r10));
  // li r10,578
  r10 = uint64_t(int64_t(578));
  // stw r10,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r10));
  // bne cr6,0x821bff10
  if (!cr6.eq) goto loc_821BFF10;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31600(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31600);
  // lwz r30,4(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r30,12
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 12, so);
  // bge cr6,0x821bfe5c
  if (!cr6.lt) goto loc_821BFE5C;
  // addi r4,r30,581
  r4 = uint64_t(int64_t(int64_t(r30) + 581));
  // b 0x821bfe88
  goto loc_821BFE88;
loc_821BFE5C:
  // li r4,599
  r4 = uint64_t(int64_t(599));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,597
  r4 = uint64_t(int64_t(597));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,597
  r4 = uint64_t(int64_t(597));
  // mr r3,r31
  r3 = r31;
loc_821BFE88:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,12
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 12, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bge cr6,0x821bfebc
  if (!cr6.lt) goto loc_821BFEBC;
  // addi r30,r29,581
  r30 = uint64_t(int64_t(int64_t(r29) + 581));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // b 0x821bff00
  goto loc_821BFF00;
loc_821BFEBC:
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwzx r30,r11,r10
  r30 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,599
  r4 = uint64_t(int64_t(599));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,599
  r4 = uint64_t(int64_t(599));
loc_821BFF00:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_821BFF10:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821BFF20: lifted from generated/ by lift.py.
lifted::Result f821BFF20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821BFF20 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821bff84
  if (!cr6.eq) goto loc_821BFF84;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31600(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31600);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,12
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 12, so);
  // addi r4,r11,581
  r4 = uint64_t(int64_t(int64_t(r11) + 581));
  // bne cr6,0x821bff78
  if (!cr6.eq) goto loc_821BFF78;
  // li r4,599
  r4 = uint64_t(int64_t(599));
loc_821BFF78:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821BFF84:
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_821BFFA0: lifted from generated/ by lift.py.
uint64_t f821BFFA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BFFA0 (lifted)");
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c0010
  if (!cr6.eq) goto loc_821C0010;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,820
  r4 = uint64_t(int64_t(820));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,820
  r4 = uint64_t(int64_t(820));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821C0010:
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
  return r3;
}

// sub_821C0030: lifted from generated/ by lift.py.
lifted::Result f821C0030(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821C0030 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r10,769
  r10 = uint64_t(int64_t(769));
  // lwz r11,0(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // li r10,773
  r10 = uint64_t(int64_t(773));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // li r10,777
  r10 = uint64_t(int64_t(777));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // li r10,781
  r10 = uint64_t(int64_t(781));
  // stw r10,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r10));
  // li r10,785
  r10 = uint64_t(int64_t(785));
  // stw r10,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r10));
  // li r10,789
  r10 = uint64_t(int64_t(789));
  // stw r10,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r10));
  // li r10,793
  r10 = uint64_t(int64_t(793));
  // stw r10,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r10));
  // li r10,797
  r10 = uint64_t(int64_t(797));
  // stw r10,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r10));
  // li r10,801
  r10 = uint64_t(int64_t(801));
  // stw r10,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r10));
  // li r10,805
  r10 = uint64_t(int64_t(805));
  // stw r10,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r10));
  // li r10,809
  r10 = uint64_t(int64_t(809));
  // stw r10,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r10));
  // li r10,813
  r10 = uint64_t(int64_t(813));
  // stw r10,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r10));
  // li r10,817
  r10 = uint64_t(int64_t(817));
  // stw r10,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r10));
  // bne cr6,0x821c01a8
  if (!cr6.eq) goto loc_821C01A8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31600(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31600);
  // lwz r30,4(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r30,6
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 6, so);
  // bge cr6,0x821c00f4
  if (!cr6.lt) goto loc_821C00F4;
  // addi r4,r30,820
  r4 = uint64_t(int64_t(int64_t(r30) + 820));
  // b 0x821c0120
  goto loc_821C0120;
loc_821C00F4:
  // li r4,838
  r4 = uint64_t(int64_t(838));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,836
  r4 = uint64_t(int64_t(836));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,836
  r4 = uint64_t(int64_t(836));
  // mr r3,r31
  r3 = r31;
loc_821C0120:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,6
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 6, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bge cr6,0x821c0154
  if (!cr6.lt) goto loc_821C0154;
  // addi r30,r29,820
  r30 = uint64_t(int64_t(int64_t(r29) + 820));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // b 0x821c0198
  goto loc_821C0198;
loc_821C0154:
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwzx r30,r11,r10
  r30 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,838
  r4 = uint64_t(int64_t(838));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,838
  r4 = uint64_t(int64_t(838));
loc_821C0198:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r28)
  REX_STORE_U32(uint32_t(r28) + 0, uint32_t(r11));
loc_821C01A8:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C01B8: lifted from generated/ by lift.py.
lifted::Result f821C01B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821C01B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c021c
  if (!cr6.eq) goto loc_821C021C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31600(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31600);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // addi r4,r11,820
  r4 = uint64_t(int64_t(int64_t(r11) + 820));
  // bne cr6,0x821c0210
  if (!cr6.eq) goto loc_821C0210;
  // li r4,838
  r4 = uint64_t(int64_t(838));
loc_821C0210:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821C021C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
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

// sub_821C02B8: lifted from generated/ by lift.py.
lifted::Result f821C02B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821C02B8 (lifted)");
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // lwz r10,272(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 272);
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lwz r10,276(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 276);
  // lwz r11,280(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 280);
  // stw r10,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r10));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,88
  r30 = uint64_t(int64_t(int64_t(r1) + 88));
loc_821C0318:
  // addi r4,r31,4
  r4 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821be988
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BE988, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,3
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 3, so);
  // blt cr6,0x821c0318
  if (cr6.lt) goto loc_821C0318;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C0348: lifted from generated/ by lift.py.
uint64_t f821C0348(uint8_t* base, uint64_t r1, uint64_t r13) {
  ITC_NATIVE_RUNNING("sub_821C0348 (lifted)");
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // lwz r10,272(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 272);
  // stw r10,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r10));
  // lwz r10,276(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 276);
  // lwz r11,280(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 280);
  // stw r10,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r10));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r29,r3
  r29 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // addi r30,r1,88
  r30 = uint64_t(int64_t(int64_t(r1) + 88));
loc_821C03A8:
  // addi r4,r31,4
  r4 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821bea48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BEA48, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r30,r30,4
  r30 = uint64_t(int64_t(int64_t(r30) + 4));
  // cmpwi cr6,r31,3
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 3, so);
  // blt cr6,0x821c03a8
  if (cr6.lt) goto loc_821C03A8;
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6edc
  // (left out: dead in C++)
  return r3;
}

// sub_821C03D8: lifted from generated/ by lift.py.
lifted::Result f821C03D8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821C03D8 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e80
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r28,0(r3)
  r28 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r28
  r3 = r28;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r26,r3
  r26 = r3;
  // li r31,348
  r31 = uint64_t(int64_t(348));
  // addi r29,r1,96
  r29 = uint64_t(int64_t(int64_t(r1) + 96));
  // li r30,12
  r30 = uint64_t(int64_t(12));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // lwz r10,332(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 332);
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // lwz r10,336(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 336);
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // lwz r10,340(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 340);
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // lwz r10,344(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 344);
  // stw r10,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r10));
  // lwz r10,348(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 348);
  // stw r10,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r10));
  // lwz r10,352(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 352);
  // stw r10,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r10));
  // lwz r10,368(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 368);
  // stw r10,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r10));
  // lwz r10,360(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 360);
  // stw r10,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r10));
  // lwz r10,364(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 364);
  // stw r10,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r10));
  // lwz r10,356(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 356);
  // stw r10,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r10));
  // lwz r10,372(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 372);
  // lwz r11,376(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 376);
  // stw r10,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r10));
  // stw r11,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r11));
loc_821C0488:
  // addi r4,r31,-347
  r4 = uint64_t(int64_t(int64_t(r31) + -347));
  // mr r3,r26
  r3 = r26;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r27,r3
  r27 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r31
  r5 = r31;
  // mr r3,r28
  r3 = r28;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r27
  r3 = r27;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // addic. r30,r30,-1
  ca = uint32_t(r30) > 0;
  r30 = uint64_t(int64_t(int64_t(r30) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // addi r31,r31,1
  r31 = uint64_t(int64_t(int64_t(r31) + 1));
  // addi r29,r29,4
  r29 = uint64_t(int64_t(int64_t(r29) + 4));
  // stw r11,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r11));
  // bne 0x821c0488
  if (!cr0.eq) goto loc_821C0488;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C04E0: lifted from generated/ by lift.py.
lifted::Result f821C04E0(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_821C04E0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,531
  r11 = uint64_t(int64_t(531));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // stw r11,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r11));
  // li r11,535
  r11 = uint64_t(int64_t(535));
  // stw r11,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r11));
  // li r11,539
  r11 = uint64_t(int64_t(539));
  // stw r11,168(r1)
  REX_STORE_U32(uint32_t(r1) + 168, uint32_t(r11));
  // li r11,543
  r11 = uint64_t(int64_t(543));
  // stw r11,172(r1)
  REX_STORE_U32(uint32_t(r1) + 172, uint32_t(r11));
  // li r11,547
  r11 = uint64_t(int64_t(547));
  // stw r11,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r11));
  // li r11,551
  r11 = uint64_t(int64_t(551));
  // stw r11,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r11));
  // li r11,555
  r11 = uint64_t(int64_t(555));
  // stw r11,184(r1)
  REX_STORE_U32(uint32_t(r1) + 184, uint32_t(r11));
  // li r11,559
  r11 = uint64_t(int64_t(559));
  // stw r11,188(r1)
  REX_STORE_U32(uint32_t(r1) + 188, uint32_t(r11));
  // li r11,563
  r11 = uint64_t(int64_t(563));
  // stw r11,192(r1)
  REX_STORE_U32(uint32_t(r1) + 192, uint32_t(r11));
  // li r11,567
  r11 = uint64_t(int64_t(567));
  // stw r11,196(r1)
  REX_STORE_U32(uint32_t(r1) + 196, uint32_t(r11));
  // li r11,571
  r11 = uint64_t(int64_t(571));
  // stw r11,200(r1)
  REX_STORE_U32(uint32_t(r1) + 200, uint32_t(r11));
  // li r11,575
  r11 = uint64_t(int64_t(575));
  // stw r11,204(r1)
  REX_STORE_U32(uint32_t(r1) + 204, uint32_t(r11));
  // li r11,579
  r11 = uint64_t(int64_t(579));
  // stw r11,208(r1)
  REX_STORE_U32(uint32_t(r1) + 208, uint32_t(r11));
  // li r11,532
  r11 = uint64_t(int64_t(532));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,536
  r11 = uint64_t(int64_t(536));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // li r11,540
  r11 = uint64_t(int64_t(540));
  // stw r11,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r11));
  // li r11,544
  r11 = uint64_t(int64_t(544));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // li r11,548
  r11 = uint64_t(int64_t(548));
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
  // li r11,552
  r11 = uint64_t(int64_t(552));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // li r11,556
  r11 = uint64_t(int64_t(556));
  // stw r11,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r11));
  // li r11,560
  r11 = uint64_t(int64_t(560));
  // stw r11,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r11));
  // li r11,564
  r11 = uint64_t(int64_t(564));
  // stw r11,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r11));
  // li r11,568
  r11 = uint64_t(int64_t(568));
  // stw r11,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r11));
  // li r11,572
  r11 = uint64_t(int64_t(572));
  // stw r11,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r11));
  // li r11,576
  r11 = uint64_t(int64_t(576));
  // stw r11,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r11));
  // li r11,580
  r11 = uint64_t(int64_t(580));
  // stw r11,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r11));
loc_821C05E0:
  // mr r3,r28
  r3 = r28;
  // bl 0x821bb420
  // (left out: dead in C++)
  r3 = f821BB420(base, r1, r3);
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // beq cr6,0x821c060c
  if (cr6.eq) goto loc_821C060C;
  // addi r11,r1,160
  r11 = uint64_t(int64_t(int64_t(r1) + 160));
loc_821C060C:
  // lwzx r29,r31,r11
  r29 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // mr r4,r29
  r4 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // cmpwi cr6,r31,48
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 48, so);
  // blt cr6,0x821c05e0
  if (cr6.lt) goto loc_821C05E0;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C0640: lifted from generated/ by lift.py.
lifted::Result f821C0640(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821C0640 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-208(r1)
  ea = -208 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r27,r3
  r27 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // lwz r10,404(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 404);
  // stw r10,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r10));
  // lwz r10,408(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 408);
  // stw r10,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r10));
  // lwz r10,412(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 412);
  // stw r10,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r10));
  // li r10,351
  r10 = uint64_t(int64_t(351));
  // stw r10,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r10));
  // lwz r10,420(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 420);
  // stw r10,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r10));
  // li r10,352
  r10 = uint64_t(int64_t(352));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // lwz r10,416(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 416);
  // lwz r11,424(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 424);
  // stw r10,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r10));
  // li r10,353
  r10 = uint64_t(int64_t(353));
  // stw r11,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r11));
  // li r11,354
  r11 = uint64_t(int64_t(354));
  // stw r10,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r10));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // li r11,355
  r11 = uint64_t(int64_t(355));
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
  // li r11,359
  r11 = uint64_t(int64_t(359));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821C06EC:
  // addi r29,r11,1
  r29 = uint64_t(int64_t(int64_t(r11) + 1));
  // mr r3,r27
  r3 = r27;
  // mr r4,r29
  r4 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r28,r3
  r28 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // mr r4,r3
  r4 = r3;
  // mr r3,r30
  r3 = r30;
  // lwzx r5,r31,r11
  r5 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // addi r28,r1,128
  r28 = uint64_t(int64_t(int64_t(r1) + 128));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwzx r10,r31,r28
  r10 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r28));
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // mr r11,r29
  r11 = r29;
  // cmpwi cr6,r31,24
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 24, so);
  // stw r10,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r10));
  // blt cr6,0x821c06ec
  if (cr6.lt) goto loc_821C06EC;
  // addi r1,r1,208
  r1 = uint64_t(int64_t(int64_t(r1) + 208));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C0750: lifted from generated/ by lift.py.
lifted::Result f821C0750(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_821C0750 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-256(r1)
  ea = -256 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,770
  r11 = uint64_t(int64_t(770));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r28,0
  r28 = uint64_t(int64_t(0));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // stw r11,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r11));
  // li r11,774
  r11 = uint64_t(int64_t(774));
  // stw r11,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r11));
  // li r11,778
  r11 = uint64_t(int64_t(778));
  // stw r11,168(r1)
  REX_STORE_U32(uint32_t(r1) + 168, uint32_t(r11));
  // li r11,782
  r11 = uint64_t(int64_t(782));
  // stw r11,172(r1)
  REX_STORE_U32(uint32_t(r1) + 172, uint32_t(r11));
  // li r11,786
  r11 = uint64_t(int64_t(786));
  // stw r11,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r11));
  // li r11,790
  r11 = uint64_t(int64_t(790));
  // stw r11,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r11));
  // li r11,794
  r11 = uint64_t(int64_t(794));
  // stw r11,184(r1)
  REX_STORE_U32(uint32_t(r1) + 184, uint32_t(r11));
  // li r11,798
  r11 = uint64_t(int64_t(798));
  // stw r11,188(r1)
  REX_STORE_U32(uint32_t(r1) + 188, uint32_t(r11));
  // li r11,802
  r11 = uint64_t(int64_t(802));
  // stw r11,192(r1)
  REX_STORE_U32(uint32_t(r1) + 192, uint32_t(r11));
  // li r11,806
  r11 = uint64_t(int64_t(806));
  // stw r11,196(r1)
  REX_STORE_U32(uint32_t(r1) + 196, uint32_t(r11));
  // li r11,810
  r11 = uint64_t(int64_t(810));
  // stw r11,200(r1)
  REX_STORE_U32(uint32_t(r1) + 200, uint32_t(r11));
  // li r11,814
  r11 = uint64_t(int64_t(814));
  // stw r11,204(r1)
  REX_STORE_U32(uint32_t(r1) + 204, uint32_t(r11));
  // li r11,818
  r11 = uint64_t(int64_t(818));
  // stw r11,208(r1)
  REX_STORE_U32(uint32_t(r1) + 208, uint32_t(r11));
  // li r11,771
  r11 = uint64_t(int64_t(771));
  // stw r11,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r11));
  // li r11,775
  r11 = uint64_t(int64_t(775));
  // stw r11,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r11));
  // li r11,779
  r11 = uint64_t(int64_t(779));
  // stw r11,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r11));
  // li r11,783
  r11 = uint64_t(int64_t(783));
  // stw r11,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r11));
  // li r11,787
  r11 = uint64_t(int64_t(787));
  // stw r11,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
  // li r11,791
  r11 = uint64_t(int64_t(791));
  // stw r11,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r11));
  // li r11,795
  r11 = uint64_t(int64_t(795));
  // stw r11,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r11));
  // li r11,799
  r11 = uint64_t(int64_t(799));
  // stw r11,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r11));
  // li r11,803
  r11 = uint64_t(int64_t(803));
  // stw r11,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r11));
  // li r11,807
  r11 = uint64_t(int64_t(807));
  // stw r11,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r11));
  // li r11,811
  r11 = uint64_t(int64_t(811));
  // stw r11,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r11));
  // li r11,815
  r11 = uint64_t(int64_t(815));
  // stw r11,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r11));
  // li r11,819
  r11 = uint64_t(int64_t(819));
  // stw r11,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r11));
loc_821C0850:
  // mr r3,r28
  r3 = r28;
  // bl 0x821bb5f8
  // (left out: dead in C++)
  r3 = f821BB5F8(base, r1, r3);
  // cmpwi cr6,r3,1
  cr6 = lifted::Compare<int32_t>(int32_t(r3), 1, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r30
  r3 = r30;
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // beq cr6,0x821c087c
  if (cr6.eq) goto loc_821C087C;
  // addi r11,r1,160
  r11 = uint64_t(int64_t(int64_t(r1) + 160));
loc_821C087C:
  // lwzx r29,r31,r11
  r29 = REX_LOAD_U32(uint32_t(r31) + uint32_t(r11));
  // mr r4,r29
  r4 = r29;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r31,r31,4
  r31 = uint64_t(int64_t(int64_t(r31) + 4));
  // addi r28,r28,1
  r28 = uint64_t(int64_t(int64_t(r28) + 1));
  // cmpwi cr6,r31,24
  cr6 = lifted::Compare<int32_t>(int32_t(r31), 24, so);
  // blt cr6,0x821c0850
  if (cr6.lt) goto loc_821C0850;
  // addi r1,r1,256
  r1 = uint64_t(int64_t(int64_t(r1) + 256));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C0EF0: lifted from generated/ by lift.py.
lifted::Result f821C0EF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821C0EF0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // li r28,7
  r28 = uint64_t(int64_t(7));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // stw r28,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r28));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // mr r11,r3
  r11 = r3;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // lwz r10,-31624(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // lwz r30,0(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r29,r10,12
  r29 = uint64_t(int64_t(int64_t(r10) + 12));
  // bl 0x82105620
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82105620, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,6100(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 6100);
  // mr r8,r29
  r8 = r29;
  // li r11,5
  r11 = uint64_t(int64_t(5));
  // addi r9,r3,8
  r9 = uint64_t(int64_t(int64_t(r3) + 8));
  // mtctr r11
  ctr = r11;
loc_821C0F44:
  // lwz r11,0(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + 0);
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // stw r11,0(r8)
  REX_STORE_U32(uint32_t(r8) + 0, uint32_t(r11));
  // addi r8,r8,4
  r8 = uint64_t(int64_t(int64_t(r8) + 4));
  // bdnz 0x821c0f44
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_821C0F44;
  // lwz r11,6116(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 6116);
  // divw r9,r10,r28
  r9 = uint32_t((int32_t(r28) && !(int32_t(r10) == INT32_MIN && int32_t(r28) == -1)) ? int32_t(r10) / int32_t(r28) : 0);
  // cmpwi cr6,r10,999
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 999, so);
  // mulli r9,r9,7
  r9 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(7))));
  // stw r11,20(r29)
  REX_STORE_U32(uint32_t(r29) + 20, uint32_t(r11));
  // subf r11,r9,r10
  r11 = r10 - r9;
  // stw r11,24(r29)
  REX_STORE_U32(uint32_t(r29) + 24, uint32_t(r11));
  // ble cr6,0x821c0f7c
  if (!cr6.gt) goto loc_821C0F7C;
  // li r10,999
  r10 = uint64_t(int64_t(999));
loc_821C0F7C:
  // stw r10,28(r29)
  REX_STORE_U32(uint32_t(r29) + 28, uint32_t(r10));
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // mr r3,r30
  r3 = r30;
  // addi r6,r11,12
  r6 = uint64_t(int64_t(int64_t(r11) + 12));
  // bl 0x8211d930
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D930, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // mr r3,r30
  r3 = r30;
  // lwz r6,32(r11)
  r6 = REX_LOAD_U32(uint32_t(r11) + 32);
  // bl 0x8211c740
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C740, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // li r6,8
  r6 = uint64_t(int64_t(8));
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // mr r3,r30
  r3 = r30;
  // lwz r7,40(r11)
  r7 = REX_LOAD_U32(uint32_t(r11) + 40);
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,11
  r5 = uint64_t(int64_t(11));
  // li r4,16
  r4 = uint64_t(int64_t(16));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // lwz r11,36(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 36);
  // stw r11,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r11));
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C1AA0: lifted from generated/ by lift.py.
lifted::Result f821C1AA0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821C1AA0 (lifted)");
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
  // lwz r3,-31628(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31628);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31628(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31628);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821c1b04
  if (cr6.eq) goto loc_821C1B04;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C1AD4:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821c1aec
  if (cr6.eq) goto loc_821C1AEC;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821C1AEC:
  // li r9,22
  r9 = uint64_t(int64_t(22));
loc_821C1AF0:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // b 0x821c1ce8
  goto loc_821C1CE8;
loc_821C1B04:
  // bl 0x821beac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BEAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c1b38
  if (cr0.eq) goto loc_821C1B38;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821c1b30
  if (cr6.eq) goto loc_821C1B30;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821C1B30:
  // li r9,23
  r9 = uint64_t(int64_t(23));
  // b 0x821c1af0
  goto loc_821C1AF0;
loc_821C1B38:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31628(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31628);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c1c20
  if (cr0.eq) goto loc_821C1C20;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31628(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31628);
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r10,-31624(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + -31624);
  // cmpwi cr6,r8,3
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 3, so);
  // bne cr6,0x821c1c00
  if (!cr6.eq) goto loc_821C1C00;
  // lwz r7,660(r10)
  r7 = REX_LOAD_U32(uint32_t(r10) + 660);
  // cmpwi cr6,r7,0
  cr6 = lifted::Compare<int32_t>(int32_t(r7), 0, so);
  // bne cr6,0x821c1bc4
  if (!cr6.eq) goto loc_821C1BC4;
  // addi r11,r10,260
  r11 = uint64_t(int64_t(int64_t(r10) + 260));
  // addi r10,r10,68
  r10 = uint64_t(int64_t(int64_t(r10) + 68));
  // lwz r8,0(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r8,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r8));
  // lwz r8,4(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
  // lwz r11,-31624(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31624);
  // addi r10,r11,272
  r10 = uint64_t(int64_t(int64_t(r11) + 272));
  // addi r11,r11,80
  r11 = uint64_t(int64_t(int64_t(r11) + 80));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // lwz r10,8(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // bl 0x821beed8
  // (left out: dead in C++)
  r3 = f821BEED8(base, r1, r13);
  // b 0x821c1c8c
  goto loc_821C1C8C;
loc_821C1BC4:
  // cmpwi cr6,r8,3
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 3, so);
  // bne cr6,0x821c1c00
  if (!cr6.eq) goto loc_821C1C00;
  // lwz r8,660(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 660);
  // cmpwi cr6,r8,1
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 1, so);
  // bne cr6,0x821c1c00
  if (!cr6.eq) goto loc_821C1C00;
  // addi r11,r10,452
  r11 = uint64_t(int64_t(int64_t(r10) + 452));
  // addi r10,r10,260
  r10 = uint64_t(int64_t(int64_t(r10) + 260));
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // stw r9,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r9));
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
  // bl 0x821bee48
  // (left out: dead in C++)
  { const lifted::Result o = f821BEE48(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821c1cf0
  goto loc_821C1CF0;
loc_821C1C00:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // lwz r11,-31624(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31624);
  // stw r7,660(r11)
  REX_STORE_U32(uint32_t(r11) + 660, uint32_t(r7));
  // b 0x821c1ad4
  goto loc_821C1AD4;
loc_821C1C20:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31628(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31628);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c1cf0
  if (cr0.eq) goto loc_821C1CF0;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31624(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31624);
  // addi r9,r11,68
  r9 = uint64_t(int64_t(int64_t(r11) + 68));
  // addi r11,r11,260
  r11 = uint64_t(int64_t(int64_t(r11) + 260));
  // lwz r8,0(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 0);
  // stw r8,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r8));
  // lwz r8,4(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 4);
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // lwz r9,8(r9)
  r9 = REX_LOAD_U32(uint32_t(r9) + 8);
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
  // lwz r11,-31624(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31624);
  // addi r10,r11,80
  r10 = uint64_t(int64_t(int64_t(r11) + 80));
  // addi r11,r11,272
  r11 = uint64_t(int64_t(int64_t(r11) + 272));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // lwz r10,8(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // bl 0x821bee48
  // (left out: dead in C++)
  { const lifted::Result o = f821BEE48(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821C1C8C:
  // bl 0x821c0348
  // (left out: dead in C++)
  r3 = f821C0348(base, r1, r13);
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31604(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821c1ca8
  if (cr6.eq) goto loc_821C1CA8;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821C1CA8:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31632);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // beq cr6,0x821c1cd8
  if (cr6.eq) goto loc_821C1CD8;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821C1CD8:
  // li r8,24
  r8 = uint64_t(int64_t(24));
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r8,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r8));
loc_821C1CE8:
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
loc_821C1CF0:
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

// sub_821C1ED8: lifted from generated/ by lift.py.
lifted::Result f821C1ED8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821C1ED8 (lifted)");
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
  // lwz r3,-31628(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31628);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31628(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31628);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821c1f2c
  if (cr6.eq) goto loc_821C1F2C;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x821c1f24
  if (cr6.eq) goto loc_821C1F24;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821C1F24:
  // li r9,30
  r9 = uint64_t(int64_t(30));
  // b 0x821c1f5c
  goto loc_821C1F5C;
loc_821C1F2C:
  // bl 0x821ba550
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BA550, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c1f78
  if (cr0.eq) goto loc_821C1F78;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x821c1f58
  if (cr6.eq) goto loc_821C1F58;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821C1F58:
  // li r9,31
  r9 = uint64_t(int64_t(31));
loc_821C1F5C:
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
loc_821C1F70:
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // b 0x821c2194
  goto loc_821C2194;
loc_821C1F78:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31628(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31628);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c20dc
  if (cr0.eq) goto loc_821C20DC;
  // lwz r11,-31628(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31628);
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r9,-31624(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // bne cr6,0x821c2090
  if (!cr6.eq) goto loc_821C2090;
  // lwz r8,660(r9)
  r8 = REX_LOAD_U32(uint32_t(r9) + 660);
  // cmpwi cr6,r8,0
  cr6 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne cr6,0x821c205c
  if (!cr6.eq) goto loc_821C205C;
  // bl 0x821ba890
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BA890, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821c2054
  if (!cr0.eq) goto loc_821C2054;
  // bl 0x821bb4d0
  // (left out: dead in C++)
  r3 = f821BB4D0(base, r1);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821c2054
  if (!cr0.eq) goto loc_821C2054;
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // addi r4,r11,284
  r4 = uint64_t(int64_t(int64_t(r11) + 284));
  // addi r3,r11,92
  r3 = uint64_t(int64_t(int64_t(r11) + 92));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // addi r4,r11,332
  r4 = uint64_t(int64_t(int64_t(r11) + 332));
  // addi r3,r11,140
  r3 = uint64_t(int64_t(int64_t(r11) + 140));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821c200c
  if (cr6.eq) goto loc_821C200C;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821C200C:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r8,32
  r8 = uint64_t(int64_t(32));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // stw r8,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r8));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31604(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31604);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // beq cr6,0x821c2040
  if (cr6.eq) goto loc_821C2040;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821C2040:
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // b 0x821c1f70
  goto loc_821C1F70;
loc_821C2054:
  // li r3,3
  r3 = uint64_t(int64_t(3));
  // b 0x821c2190
  goto loc_821C2190;
loc_821C205C:
  // cmpwi cr6,r10,12
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 12, so);
  // bne cr6,0x821c2090
  if (!cr6.eq) goto loc_821C2090;
  // lwz r10,660(r9)
  r10 = REX_LOAD_U32(uint32_t(r9) + 660);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821c2090
  if (!cr6.eq) goto loc_821C2090;
  // addi r4,r9,476
  r4 = uint64_t(int64_t(int64_t(r9) + 476));
  // addi r3,r9,284
  r3 = uint64_t(int64_t(int64_t(r9) + 284));
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf2b8
  // (left out: dead in C++)
  { const lifted::Result o = f821BF2B8(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf3c0
  // (left out: dead in C++)
  { const lifted::Result o = f821BF3C0(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x821c2190
  goto loc_821C2190;
loc_821C2090:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // stw r8,660(r11)
  REX_STORE_U32(uint32_t(r11) + 660, uint32_t(r8));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x821c20c4
  if (cr6.eq) goto loc_821C20C4;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821C20C4:
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // li r9,30
  r9 = uint64_t(int64_t(30));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r8,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r8));
  // b 0x821c2184
  goto loc_821C2184;
loc_821C20DC:
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r3,-31628(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31628);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c2194
  if (cr0.eq) goto loc_821C2194;
  // lis r31,-32146
  r31 = uint64_t(int64_t(-2106720256));
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // addi r4,r11,92
  r4 = uint64_t(int64_t(int64_t(r11) + 92));
  // addi r3,r11,284
  r3 = uint64_t(int64_t(int64_t(r11) + 284));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31624);
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // addi r4,r11,140
  r4 = uint64_t(int64_t(int64_t(r11) + 140));
  // addi r3,r11,332
  r3 = uint64_t(int64_t(int64_t(r11) + 332));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf2b8
  // (left out: dead in C++)
  { const lifted::Result o = f821BF2B8(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf3c0
  // (left out: dead in C++)
  { const lifted::Result o = f821BF3C0(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c03d8
  // (left out: dead in C++)
  { const lifted::Result o = f821C03D8(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c04e0
  // (left out: dead in C++)
  { const lifted::Result o = f821C04E0(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31632(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + -31632);
  // lwz r11,0(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821c2144
  if (cr6.eq) goto loc_821C2144;
  // stw r11,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r11));
loc_821C2144:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // li r8,32
  r8 = uint64_t(int64_t(32));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // stw r8,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r8));
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r10,-31604(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31604);
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r9,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), -1, so);
  // beq cr6,0x821c2178
  if (cr6.eq) goto loc_821C2178;
  // stw r9,8(r10)
  REX_STORE_U32(uint32_t(r10) + 8, uint32_t(r9));
loc_821C2178:
  // li r9,7
  r9 = uint64_t(int64_t(7));
  // stw r11,4(r10)
  REX_STORE_U32(uint32_t(r10) + 4, uint32_t(r11));
  // li r3,2
  r3 = uint64_t(int64_t(2));
loc_821C2184:
  // stw r11,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r11));
  // stw r11,16(r10)
  REX_STORE_U32(uint32_t(r10) + 16, uint32_t(r11));
  // stw r9,12(r10)
  REX_STORE_U32(uint32_t(r10) + 12, uint32_t(r9));
loc_821C2190:
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C2194:
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

// sub_821C36F0: lifted from generated/ by lift.py.
lifted::Result f821C36F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C36F0 (lifted)");
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
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
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
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c38f8
  if (!cr6.eq) goto loc_821C38F8;
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // lis r30,-32146
  r30 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31628(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31628);
  // lwz r28,4(r11)
  r28 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r29,0(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r28,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 1, so);
  // blt cr6,0x821c37b4
  if (cr6.lt) goto loc_821C37B4;
  // beq cr6,0x821c37ac
  if (cr6.eq) goto loc_821C37AC;
  // cmplwi cr6,r28,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r28), 3, so);
  // blt cr6,0x821c37a4
  if (cr6.lt) goto loc_821C37A4;
  // bne cr6,0x821c37c0
  if (!cr6.eq) goto loc_821C37C0;
  // lwz r11,-31624(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31624);
  // mr r3,r31
  r3 = r31;
  // li r4,344
  r4 = uint64_t(int64_t(344));
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821c3770
  if (cr6.eq) goto loc_821C3770;
  // li r4,345
  r4 = uint64_t(int64_t(345));
loc_821C3770:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31624);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // cmpwi cr6,r29,0
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // stw r10,660(r11)
  REX_STORE_U32(uint32_t(r11) + 660, uint32_t(r10));
  // bne cr6,0x821c3790
  if (!cr6.eq) goto loc_821C3790;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x821c379c
  goto loc_821C379C;
loc_821C3790:
  // cmpwi cr6,r29,2
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 2, so);
  // bne cr6,0x821c37c0
  if (!cr6.eq) goto loc_821C37C0;
  // li r3,80
  r3 = uint64_t(int64_t(80));
loc_821C379C:
  // bl 0x821bef68
  // (left out: dead in C++)
  r3 = f821BEF68(base, r1, r13, r3);
  // b 0x821c37c0
  goto loc_821C37C0;
loc_821C37A4:
  // li r4,343
  r4 = uint64_t(int64_t(343));
  // b 0x821c37b8
  goto loc_821C37B8;
loc_821C37AC:
  // li r4,342
  r4 = uint64_t(int64_t(342));
  // b 0x821c37b8
  goto loc_821C37B8;
loc_821C37B4:
  // li r4,341
  r4 = uint64_t(int64_t(341));
loc_821C37B8:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C37C0:
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // blt cr6,0x821c3864
  if (cr6.lt) goto loc_821C3864;
  // beq cr6,0x821c3840
  if (cr6.eq) goto loc_821C3840;
  // cmplwi cr6,r29,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 3, so);
  // blt cr6,0x821c381c
  if (cr6.lt) goto loc_821C381C;
  // bne cr6,0x821c388c
  if (!cr6.eq) goto loc_821C388C;
  // lwz r11,-31624(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + -31624);
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
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c380c
  if (!cr6.eq) goto loc_821C380C;
  // li r4,344
  r4 = uint64_t(int64_t(344));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,344
  r4 = uint64_t(int64_t(344));
  // b 0x821c3884
  goto loc_821C3884;
loc_821C380C:
  // li r4,345
  r4 = uint64_t(int64_t(345));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,345
  r4 = uint64_t(int64_t(345));
  // b 0x821c3884
  goto loc_821C3884;
loc_821C381C:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,343
  r4 = uint64_t(int64_t(343));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,343
  r4 = uint64_t(int64_t(343));
  // b 0x821c3884
  goto loc_821C3884;
loc_821C3840:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,342
  r4 = uint64_t(int64_t(342));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,342
  r4 = uint64_t(int64_t(342));
  // b 0x821c3884
  goto loc_821C3884;
loc_821C3864:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,341
  r4 = uint64_t(int64_t(341));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,341
  r4 = uint64_t(int64_t(341));
loc_821C3884:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C388C:
  // li r4,243
  r4 = uint64_t(int64_t(243));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,244
  r4 = uint64_t(int64_t(244));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r29,3
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 3, so);
  // beq cr6,0x821c38f0
  if (cr6.eq) goto loc_821C38F0;
  // cmpwi cr6,r28,3
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 3, so);
  // beq cr6,0x821c38f0
  if (cr6.eq) goto loc_821C38F0;
  // lwz r11,-31628(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31628);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821c38cc
  if (!cr6.eq) goto loc_821C38CC;
  // li r4,244
  r4 = uint64_t(int64_t(244));
  // b 0x821c38d8
  goto loc_821C38D8;
loc_821C38CC:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x821c38f0
  if (!cr6.eq) goto loc_821C38F0;
  // li r4,243
  r4 = uint64_t(int64_t(243));
loc_821C38D8:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C38F0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r11));
loc_821C38F8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C3910: lifted from generated/ by lift.py.
lifted::Result f821C3910(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821C3910 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c39e8
  if (!cr6.eq) goto loc_821C39E8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31628(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31628);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821c39dc
  if (!cr6.eq) goto loc_821C39DC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c39a8
  if (!cr6.eq) goto loc_821C39A8;
  // li r4,345
  r4 = uint64_t(int64_t(345));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,344
  r4 = uint64_t(int64_t(344));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,344
  r4 = uint64_t(int64_t(344));
  // b 0x821c39d0
  goto loc_821C39D0;
loc_821C39A8:
  // li r4,344
  r4 = uint64_t(int64_t(344));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,345
  r4 = uint64_t(int64_t(345));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,345
  r4 = uint64_t(int64_t(345));
loc_821C39D0:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c39e0
  goto loc_821C39E0;
loc_821C39DC:
  // bl 0x821bee48
  // (left out: dead in C++)
  { const lifted::Result o = f821BEE48(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
loc_821C39E0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821C39E8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_821C3A10: lifted from generated/ by lift.py.
lifted::Result f821C3A10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C3A10 (lifted)");
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
  // stwu r1,-416(r1)
  ea = -416 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
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
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,423
  r11 = uint64_t(int64_t(423));
  // lwz r10,0(r26)
  r10 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // stw r11,144(r1)
  REX_STORE_U32(uint32_t(r1) + 144, uint32_t(r11));
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // lwz r9,284(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 284);
  // stw r9,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r9));
  // li r9,425
  r9 = uint64_t(int64_t(425));
  // stw r9,148(r1)
  REX_STORE_U32(uint32_t(r1) + 148, uint32_t(r9));
  // lwz r9,288(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 288);
  // stw r9,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r9));
  // li r9,427
  r9 = uint64_t(int64_t(427));
  // stw r9,152(r1)
  REX_STORE_U32(uint32_t(r1) + 152, uint32_t(r9));
  // lwz r9,292(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 292);
  // stw r9,104(r1)
  REX_STORE_U32(uint32_t(r1) + 104, uint32_t(r9));
  // li r9,429
  r9 = uint64_t(int64_t(429));
  // stw r9,156(r1)
  REX_STORE_U32(uint32_t(r1) + 156, uint32_t(r9));
  // lwz r9,296(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 296);
  // stw r9,108(r1)
  REX_STORE_U32(uint32_t(r1) + 108, uint32_t(r9));
  // li r9,431
  r9 = uint64_t(int64_t(431));
  // stw r9,160(r1)
  REX_STORE_U32(uint32_t(r1) + 160, uint32_t(r9));
  // lwz r9,300(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 300);
  // stw r9,112(r1)
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r9));
  // li r9,433
  r9 = uint64_t(int64_t(433));
  // stw r9,164(r1)
  REX_STORE_U32(uint32_t(r1) + 164, uint32_t(r9));
  // lwz r9,304(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 304);
  // stw r9,116(r1)
  REX_STORE_U32(uint32_t(r1) + 116, uint32_t(r9));
  // li r9,435
  r9 = uint64_t(int64_t(435));
  // stw r9,168(r1)
  REX_STORE_U32(uint32_t(r1) + 168, uint32_t(r9));
  // lwz r9,320(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 320);
  // stw r9,120(r1)
  REX_STORE_U32(uint32_t(r1) + 120, uint32_t(r9));
  // li r9,437
  r9 = uint64_t(int64_t(437));
  // stw r9,172(r1)
  REX_STORE_U32(uint32_t(r1) + 172, uint32_t(r9));
  // lwz r9,312(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 312);
  // stw r9,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r9));
  // li r9,439
  r9 = uint64_t(int64_t(439));
  // stw r9,176(r1)
  REX_STORE_U32(uint32_t(r1) + 176, uint32_t(r9));
  // lwz r9,316(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 316);
  // stw r9,128(r1)
  REX_STORE_U32(uint32_t(r1) + 128, uint32_t(r9));
  // li r9,441
  r9 = uint64_t(int64_t(441));
  // stw r9,180(r1)
  REX_STORE_U32(uint32_t(r1) + 180, uint32_t(r9));
  // lwz r9,308(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 308);
  // stw r9,132(r1)
  REX_STORE_U32(uint32_t(r1) + 132, uint32_t(r9));
  // li r9,443
  r9 = uint64_t(int64_t(443));
  // stw r9,184(r1)
  REX_STORE_U32(uint32_t(r1) + 184, uint32_t(r9));
  // lwz r9,324(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 324);
  // stw r9,136(r1)
  REX_STORE_U32(uint32_t(r1) + 136, uint32_t(r9));
  // li r9,445
  r9 = uint64_t(int64_t(445));
  // stw r9,188(r1)
  REX_STORE_U32(uint32_t(r1) + 188, uint32_t(r9));
  // lwz r9,328(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 328);
  // stw r9,140(r1)
  REX_STORE_U32(uint32_t(r1) + 140, uint32_t(r9));
  // li r9,447
  r9 = uint64_t(int64_t(447));
  // stw r9,192(r1)
  REX_STORE_U32(uint32_t(r1) + 192, uint32_t(r9));
  // li r9,449
  r9 = uint64_t(int64_t(449));
  // stw r9,196(r1)
  REX_STORE_U32(uint32_t(r1) + 196, uint32_t(r9));
  // li r9,451
  r9 = uint64_t(int64_t(451));
  // stw r9,200(r1)
  REX_STORE_U32(uint32_t(r1) + 200, uint32_t(r9));
  // li r9,453
  r9 = uint64_t(int64_t(453));
  // stw r9,204(r1)
  REX_STORE_U32(uint32_t(r1) + 204, uint32_t(r9));
  // li r9,455
  r9 = uint64_t(int64_t(455));
  // stw r9,208(r1)
  REX_STORE_U32(uint32_t(r1) + 208, uint32_t(r9));
  // li r9,422
  r9 = uint64_t(int64_t(422));
  // stw r9,224(r1)
  REX_STORE_U32(uint32_t(r1) + 224, uint32_t(r9));
  // li r9,424
  r9 = uint64_t(int64_t(424));
  // stw r9,228(r1)
  REX_STORE_U32(uint32_t(r1) + 228, uint32_t(r9));
  // li r9,426
  r9 = uint64_t(int64_t(426));
  // stw r9,232(r1)
  REX_STORE_U32(uint32_t(r1) + 232, uint32_t(r9));
  // li r9,428
  r9 = uint64_t(int64_t(428));
  // stw r9,236(r1)
  REX_STORE_U32(uint32_t(r1) + 236, uint32_t(r9));
  // li r9,430
  r9 = uint64_t(int64_t(430));
  // stw r9,240(r1)
  REX_STORE_U32(uint32_t(r1) + 240, uint32_t(r9));
  // li r9,432
  r9 = uint64_t(int64_t(432));
  // stw r9,244(r1)
  REX_STORE_U32(uint32_t(r1) + 244, uint32_t(r9));
  // li r9,434
  r9 = uint64_t(int64_t(434));
  // stw r9,248(r1)
  REX_STORE_U32(uint32_t(r1) + 248, uint32_t(r9));
  // li r9,436
  r9 = uint64_t(int64_t(436));
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r9,252(r1)
  REX_STORE_U32(uint32_t(r1) + 252, uint32_t(r9));
  // li r9,438
  r9 = uint64_t(int64_t(438));
  // stw r9,256(r1)
  REX_STORE_U32(uint32_t(r1) + 256, uint32_t(r9));
  // li r9,440
  r9 = uint64_t(int64_t(440));
  // stw r9,260(r1)
  REX_STORE_U32(uint32_t(r1) + 260, uint32_t(r9));
  // li r9,442
  r9 = uint64_t(int64_t(442));
  // stw r9,264(r1)
  REX_STORE_U32(uint32_t(r1) + 264, uint32_t(r9));
  // li r9,444
  r9 = uint64_t(int64_t(444));
  // stw r9,268(r1)
  REX_STORE_U32(uint32_t(r1) + 268, uint32_t(r9));
  // li r9,446
  r9 = uint64_t(int64_t(446));
  // stw r9,272(r1)
  REX_STORE_U32(uint32_t(r1) + 272, uint32_t(r9));
  // li r9,448
  r9 = uint64_t(int64_t(448));
  // stw r9,276(r1)
  REX_STORE_U32(uint32_t(r1) + 276, uint32_t(r9));
  // li r9,450
  r9 = uint64_t(int64_t(450));
  // stw r9,280(r1)
  REX_STORE_U32(uint32_t(r1) + 280, uint32_t(r9));
  // li r9,452
  r9 = uint64_t(int64_t(452));
  // stw r9,284(r1)
  REX_STORE_U32(uint32_t(r1) + 284, uint32_t(r9));
  // li r9,454
  r9 = uint64_t(int64_t(454));
  // stw r9,288(r1)
  REX_STORE_U32(uint32_t(r1) + 288, uint32_t(r9));
  // li r9,356
  r9 = uint64_t(int64_t(356));
  // stw r9,304(r1)
  REX_STORE_U32(uint32_t(r1) + 304, uint32_t(r9));
  // li r9,360
  r9 = uint64_t(int64_t(360));
  // stw r9,308(r1)
  REX_STORE_U32(uint32_t(r1) + 308, uint32_t(r9));
  // li r9,364
  r9 = uint64_t(int64_t(364));
  // stw r9,312(r1)
  REX_STORE_U32(uint32_t(r1) + 312, uint32_t(r9));
  // li r9,368
  r9 = uint64_t(int64_t(368));
  // stw r9,316(r1)
  REX_STORE_U32(uint32_t(r1) + 316, uint32_t(r9));
  // li r9,372
  r9 = uint64_t(int64_t(372));
  // stw r9,320(r1)
  REX_STORE_U32(uint32_t(r1) + 320, uint32_t(r9));
  // li r9,376
  r9 = uint64_t(int64_t(376));
  // stw r9,324(r1)
  REX_STORE_U32(uint32_t(r1) + 324, uint32_t(r9));
  // li r9,380
  r9 = uint64_t(int64_t(380));
  // stw r9,328(r1)
  REX_STORE_U32(uint32_t(r1) + 328, uint32_t(r9));
  // li r9,384
  r9 = uint64_t(int64_t(384));
  // stw r9,332(r1)
  REX_STORE_U32(uint32_t(r1) + 332, uint32_t(r9));
  // li r9,388
  r9 = uint64_t(int64_t(388));
  // stw r9,336(r1)
  REX_STORE_U32(uint32_t(r1) + 336, uint32_t(r9));
  // li r9,392
  r9 = uint64_t(int64_t(392));
  // stw r9,340(r1)
  REX_STORE_U32(uint32_t(r1) + 340, uint32_t(r9));
  // li r9,396
  r9 = uint64_t(int64_t(396));
  // stw r9,344(r1)
  REX_STORE_U32(uint32_t(r1) + 344, uint32_t(r9));
  // li r9,400
  r9 = uint64_t(int64_t(400));
  // stw r9,348(r1)
  REX_STORE_U32(uint32_t(r1) + 348, uint32_t(r9));
  // li r9,404
  r9 = uint64_t(int64_t(404));
  // stw r9,352(r1)
  REX_STORE_U32(uint32_t(r1) + 352, uint32_t(r9));
  // bne cr6,0x821c3e2c
  if (!cr6.eq) goto loc_821C3E2C;
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r10,-31628(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -31628);
  // lwz r28,4(r10)
  r28 = REX_LOAD_U32(uint32_t(r10) + 4);
  // lwz r27,0(r10)
  r27 = REX_LOAD_U32(uint32_t(r10) + 0);
  // cmpwi cr6,r28,12
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 12, so);
  // bge cr6,0x821c3c50
  if (!cr6.lt) goto loc_821C3C50;
  // addi r4,r28,407
  r4 = uint64_t(int64_t(int64_t(r28) + 407));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c3c9c
  goto loc_821C3C9C;
loc_821C3C50:
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // li r4,354
  r4 = uint64_t(int64_t(354));
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821c3c64
  if (cr6.eq) goto loc_821C3C64;
  // li r4,353
  r4 = uint64_t(int64_t(353));
loc_821C3C64:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,352
  r4 = uint64_t(int64_t(352));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,352
  r4 = uint64_t(int64_t(352));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // stw r10,660(r11)
  REX_STORE_U32(uint32_t(r11) + 660, uint32_t(r10));
loc_821C3C9C:
  // cmpwi cr6,r27,12
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 12, so);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bge cr6,0x821c3ccc
  if (!cr6.lt) goto loc_821C3CCC;
  // addi r30,r27,407
  r30 = uint64_t(int64_t(int64_t(r27) + 407));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // b 0x821c3d30
  goto loc_821C3D30;
loc_821C3CCC:
  // rlwinm r11,r28,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,304
  r10 = uint64_t(int64_t(int64_t(r1) + 304));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // lwzx r30,r11,r10
  r30 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // mr r4,r30
  r4 = r30;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821c3d24
  if (!cr6.eq) goto loc_821C3D24;
  // li r4,354
  r4 = uint64_t(int64_t(354));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,354
  r4 = uint64_t(int64_t(354));
  // b 0x821c3d30
  goto loc_821C3D30;
loc_821C3D24:
  // li r4,353
  r4 = uint64_t(int64_t(353));
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,353
  r4 = uint64_t(int64_t(353));
loc_821C3D30:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r27,12
  cr6 = lifted::Compare<int32_t>(int32_t(r27), 12, so);
  // addi r10,r1,96
  r10 = uint64_t(int64_t(int64_t(r1) + 96));
  // beq cr6,0x821c3da4
  if (cr6.eq) goto loc_821C3DA4;
  // rlwinm r11,r27,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // cmpwi cr6,r28,12
  cr6 = lifted::Compare<int32_t>(int32_t(r28), 12, so);
  // lwzx r30,r11,r10
  r30 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // beq cr6,0x821c3dc8
  if (cr6.eq) goto loc_821C3DC8;
  // rlwinm r9,r28,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r1,96
  r8 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwzx r11,r9,r8
  r11 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r8));
  // cmpw cr6,r11,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r30), so);
  // beq cr6,0x821c3e24
  if (cr6.eq) goto loc_821C3E24;
  // rlwinm r29,r11,2,0,29
  r29 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // addi r11,r1,224
  r11 = uint64_t(int64_t(int64_t(r1) + 224));
  // mr r3,r31
  r3 = r31;
  // lwzx r4,r29,r11
  r4 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,144
  r11 = uint64_t(int64_t(int64_t(r1) + 144));
  // mr r3,r31
  r3 = r31;
  // lwzx r4,r29,r11
  r4 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r10,r1,144
  r10 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r9,r1,224
  r9 = uint64_t(int64_t(int64_t(r1) + 224));
  // lwzx r29,r11,r10
  r29 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // lwzx r4,r11,r9
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r9));
  // b 0x821c3ddc
  goto loc_821C3DDC;
loc_821C3DA4:
  // rlwinm r11,r28,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r28) | (r28 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,144
  r9 = uint64_t(int64_t(int64_t(r1) + 144));
  // mr r3,r31
  r3 = r31;
  // lwzx r11,r11,r10
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r10));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r4,r11,r9
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r9));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // b 0x821c3e20
  goto loc_821C3E20;
loc_821C3DC8:
  // rlwinm r11,r30,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // addi r9,r1,144
  r9 = uint64_t(int64_t(int64_t(r1) + 144));
  // addi r4,r1,224
  r4 = uint64_t(int64_t(int64_t(r1) + 224));
  // lwzx r29,r11,r9
  r29 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r9));
  // lwzx r4,r11,r4
  r4 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r4));
loc_821C3DDC:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
loc_821C3E20:
  // bl 0x821bf0f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BF0F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C3E24:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r11));
loc_821C3E2C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,416
  r1 = uint64_t(int64_t(int64_t(r1) + 416));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C5350: lifted from generated/ by lift.py.
lifted::Result f821C5350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C5350 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
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
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r10,r3
  r10 = r3;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r30,0(r10)
  r30 = REX_LOAD_U32(uint32_t(r10) + 0);
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // lwz r11,-31628(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31628);
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r31,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r31));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r31));
  // stw r31,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r31));
  // stw r31,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r31));
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // addi r10,r11,68
  r10 = uint64_t(int64_t(int64_t(r11) + 68));
  // addi r11,r11,260
  r11 = uint64_t(int64_t(int64_t(r11) + 260));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // lwz r10,8(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // addi r10,r11,80
  r10 = uint64_t(int64_t(int64_t(r11) + 80));
  // addi r11,r11,272
  r11 = uint64_t(int64_t(int64_t(r11) + 272));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // lwz r10,8(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // bl 0x821bfa68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BFA68, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // stw r31,660(r11)
  REX_STORE_U32(uint32_t(r11) + 660, uint32_t(r31));
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r29,345
  r29 = uint64_t(int64_t(345));
  // li r28,3
  r28 = uint64_t(int64_t(3));
loc_821C544C:
  // addi r4,r29,-340
  r4 = uint64_t(int64_t(int64_t(r29) + -340));
  // mr r3,r27
  r3 = r27;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r29
  r5 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // bne 0x821c544c
  if (!cr0.eq) goto loc_821C544C;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // li r29,345
  r29 = uint64_t(int64_t(345));
  // li r28,3
  r28 = uint64_t(int64_t(3));
loc_821C5498:
  // addi r4,r29,-340
  r4 = uint64_t(int64_t(int64_t(r29) + -340));
  // mr r3,r27
  r3 = r27;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // mr r5,r29
  r5 = r29;
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // bne 0x821c5498
  if (!cr0.eq) goto loc_821C5498;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r29,r3
  r29 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,8
  r4 = uint64_t(int64_t(8));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r29,r3
  r29 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r29
  r3 = r29;
  // stw r31,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r29
  r3 = r29;
  // stw r31,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r29
  r3 = r29;
  // stw r31,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bee48
  // (left out: dead in C++)
  { const lifted::Result o = f821BEE48(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821beed8
  // (left out: dead in C++)
  r3 = f821BEED8(base, r1, r13);
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C56B0: lifted from generated/ by lift.py.
lifted::Result f821C56B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C56B0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r10,12
  r10 = uint64_t(int64_t(12));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31628(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31628);
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // stw r31,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r31));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r31));
  // stw r31,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r31));
  // stw r31,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r31));
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // addi r4,r11,92
  r4 = uint64_t(int64_t(int64_t(r11) + 92));
  // addi r3,r11,284
  r3 = uint64_t(int64_t(int64_t(r11) + 284));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // stw r31,660(r11)
  REX_STORE_U32(uint32_t(r11) + 660, uint32_t(r31));
  // bl 0x821bfa68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BFA68, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf2b8
  // (left out: dead in C++)
  { const lifted::Result o = f821BF2B8(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r31,r3
  r31 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,999
  r6 = uint64_t(int64_t(999));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C570, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf3c0
  // (left out: dead in C++)
  { const lifted::Result o = f821BF3C0(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C5900: lifted from generated/ by lift.py.
lifted::Result f821C5900(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_821C5900 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r30,0(r3)
  r30 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r10,6
  r10 = uint64_t(int64_t(6));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // lis r29,-32146
  r29 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31628(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31628);
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // stw r31,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r31));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r31));
  // stw r31,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r31));
  // stw r31,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r31));
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // addi r4,r11,188
  r4 = uint64_t(int64_t(int64_t(r11) + 188));
  // addi r3,r11,380
  r3 = uint64_t(int64_t(int64_t(r11) + 380));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + -31624);
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // li r3,-1
  r3 = uint64_t(int64_t(-1));
  // stw r31,660(r11)
  REX_STORE_U32(uint32_t(r11) + 660, uint32_t(r31));
  // bl 0x821bfa68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BFA68, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf7f8
  // (left out: dead in C++)
  { const lifted::Result o = f821BF7F8(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r31,r3
  r31 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r31
  r3 = r31;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,999
  r6 = uint64_t(int64_t(999));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c1f8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C1F8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r30
  r3 = r30;
  // bl 0x8211c570
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C570, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821bf908
  // (left out: dead in C++)
  { const lifted::Result o = f821BF908(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C5A20: lifted from generated/ by lift.py.
lifted::Result f821C5A20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821C5A20 (lifted)");
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
  // lwz r3,-31600(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821c5a6c
  if (cr6.eq) goto loc_821C5A6C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31604(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821c5a64
  if (cr0.eq) goto loc_821C5A64;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C5A64:
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // b 0x821c5a94
  goto loc_821C5A94;
loc_821C5A6C:
  // bl 0x821bb290
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BB290, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c5ab8
  if (cr0.eq) goto loc_821C5AB8;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31604(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821c5a90
  if (cr0.eq) goto loc_821C5A90;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C5A90:
  // li r9,2
  r9 = uint64_t(int64_t(2));
loc_821C5A94:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c5b58
  goto loc_821C5B58;
loc_821C5AB8:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31600(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c5b58
  if (cr0.eq) goto loc_821C5B58;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // bne cr6,0x821c5b14
  if (!cr6.eq) goto loc_821C5B14;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // addi r10,r11,464
  r10 = uint64_t(int64_t(int64_t(r11) + 464));
  // addi r11,r11,272
  r11 = uint64_t(int64_t(int64_t(r11) + 272));
  // lwz r9,0(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 0);
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r9,4(r10)
  r9 = REX_LOAD_U32(uint32_t(r10) + 4);
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // lwz r10,8(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 8);
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // bl 0x821c02b8
  // (left out: dead in C++)
  { const lifted::Result o = f821C02B8(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821c5b58
  goto loc_821C5B58;
loc_821C5B14:
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31604(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821c5b3c
  if (cr0.eq) goto loc_821C5B3C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C5B3C:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
loc_821C5B58:
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

// sub_821C5B70: lifted from generated/ by lift.py.
lifted::Result f821C5B70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821C5B70 (lifted)");
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
  // lwz r3,-31600(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821c5bc4
  if (cr6.eq) goto loc_821C5BC4;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31604(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821c5bbc
  if (cr6.eq) goto loc_821C5BBC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C5BBC:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // b 0x821c5bf4
  goto loc_821C5BF4;
loc_821C5BC4:
  // bl 0x821bb3b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BB3B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c5c10
  if (cr0.eq) goto loc_821C5C10;
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31604(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821c5bf0
  if (cr6.eq) goto loc_821C5BF0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C5BF0:
  // li r9,6
  r9 = uint64_t(int64_t(6));
loc_821C5BF4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821c5c5c
  goto loc_821C5C5C;
loc_821C5C10:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31600(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c5c5c
  if (cr0.eq) goto loc_821C5C5C;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,12
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 12, so);
  // bne cr6,0x821c5c5c
  if (!cr6.eq) goto loc_821C5C5C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // addi r4,r11,524
  r4 = uint64_t(int64_t(int64_t(r11) + 524));
  // addi r3,r11,332
  r3 = uint64_t(int64_t(int64_t(r11) + 332));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c03d8
  // (left out: dead in C++)
  { const lifted::Result o = f821C03D8(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c04e0
  // (left out: dead in C++)
  { const lifted::Result o = f821C04E0(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
loc_821C5C5C:
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

// sub_821C5C70: lifted from generated/ by lift.py.
lifted::Result f821C5C70(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821C5C70 (lifted)");
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
  // lwz r3,-31600(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // bl 0x82186568
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82186568, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821c5cbc
  if (cr6.eq) goto loc_821C5CBC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31604(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821c5cb4
  if (cr6.eq) goto loc_821C5CB4;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C5CB4:
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // b 0x821c5ce4
  goto loc_821C5CE4;
loc_821C5CBC:
  // bl 0x821bb590
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821BB590, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c5d08
  if (cr0.eq) goto loc_821C5D08;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31604(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821c5ce0
  if (cr6.eq) goto loc_821C5CE0;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C5CE0:
  // li r9,10
  r9 = uint64_t(int64_t(10));
loc_821C5CE4:
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c5d54
  goto loc_821C5D54;
loc_821C5D08:
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r3,-31600(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // bl 0x821864c8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821864C8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821c5d54
  if (cr0.eq) goto loc_821C5D54;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31600(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + -31600);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x821c5d54
  if (!cr6.eq) goto loc_821C5D54;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // lwz r11,-31596(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31596);
  // addi r4,r11,596
  r4 = uint64_t(int64_t(int64_t(r11) + 596));
  // addi r3,r11,404
  r3 = uint64_t(int64_t(int64_t(r11) + 404));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c0640
  // (left out: dead in C++)
  { const lifted::Result o = f821C0640(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c0750
  // (left out: dead in C++)
  { const lifted::Result o = f821C0750(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
loc_821C5D54:
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

// sub_821C5D68: lifted from generated/ by lift.py.
lifted::Result f821C5D68(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821C5D68 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c5f2c
  if (!cr6.eq) goto loc_821C5F2C;
  // lis r28,-32146
  r28 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31600(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31600);
  // lwz r29,4(r11)
  r29 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r30,0(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r29,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 1, so);
  // blt cr6,0x821c5e08
  if (cr6.lt) goto loc_821C5E08;
  // beq cr6,0x821c5e00
  if (cr6.eq) goto loc_821C5E00;
  // cmplwi cr6,r29,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 3, so);
  // blt cr6,0x821c5df8
  if (cr6.lt) goto loc_821C5DF8;
  // bne cr6,0x821c5e14
  if (!cr6.eq) goto loc_821C5E14;
  // li r4,350
  r4 = uint64_t(int64_t(350));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,0
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne cr6,0x821c5de4
  if (!cr6.eq) goto loc_821C5DE4;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x821c5df0
  goto loc_821C5DF0;
loc_821C5DE4:
  // cmpwi cr6,r30,2
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 2, so);
  // bne cr6,0x821c5e14
  if (!cr6.eq) goto loc_821C5E14;
  // li r3,80
  r3 = uint64_t(int64_t(80));
loc_821C5DF0:
  // bl 0x821c0238
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C0238, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c5e14
  goto loc_821C5E14;
loc_821C5DF8:
  // li r4,348
  r4 = uint64_t(int64_t(348));
  // b 0x821c5e0c
  goto loc_821C5E0C;
loc_821C5E00:
  // li r4,347
  r4 = uint64_t(int64_t(347));
  // b 0x821c5e0c
  goto loc_821C5E0C;
loc_821C5E08:
  // li r4,346
  r4 = uint64_t(int64_t(346));
loc_821C5E0C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C5E14:
  // cmplwi cr6,r30,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 1, so);
  // blt cr6,0x821c5e98
  if (cr6.lt) goto loc_821C5E98;
  // beq cr6,0x821c5e74
  if (cr6.eq) goto loc_821C5E74;
  // cmplwi cr6,r30,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 3, so);
  // blt cr6,0x821c5e50
  if (cr6.lt) goto loc_821C5E50;
  // bne cr6,0x821c5ec0
  if (!cr6.eq) goto loc_821C5EC0;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,350
  r4 = uint64_t(int64_t(350));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,350
  r4 = uint64_t(int64_t(350));
  // b 0x821c5eb8
  goto loc_821C5EB8;
loc_821C5E50:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,348
  r4 = uint64_t(int64_t(348));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,348
  r4 = uint64_t(int64_t(348));
  // b 0x821c5eb8
  goto loc_821C5EB8;
loc_821C5E74:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,347
  r4 = uint64_t(int64_t(347));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,347
  r4 = uint64_t(int64_t(347));
  // b 0x821c5eb8
  goto loc_821C5EB8;
loc_821C5E98:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,346
  r4 = uint64_t(int64_t(346));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,346
  r4 = uint64_t(int64_t(346));
loc_821C5EB8:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C5EC0:
  // li r4,297
  r4 = uint64_t(int64_t(297));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,298
  r4 = uint64_t(int64_t(298));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi cr6,r30,3
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 3, so);
  // beq cr6,0x821c5f24
  if (cr6.eq) goto loc_821C5F24;
  // cmpwi cr6,r29,3
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 3, so);
  // beq cr6,0x821c5f24
  if (cr6.eq) goto loc_821C5F24;
  // lwz r11,-31600(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + -31600);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821c5f00
  if (!cr6.eq) goto loc_821C5F00;
  // li r4,298
  r4 = uint64_t(int64_t(298));
  // b 0x821c5f0c
  goto loc_821C5F0C;
loc_821C5F00:
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x821c5f24
  if (!cr6.eq) goto loc_821C5F24;
  // li r4,297
  r4 = uint64_t(int64_t(297));
loc_821C5F0C:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C5F24:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r27)
  REX_STORE_U32(uint32_t(r27) + 0, uint32_t(r11));
loc_821C5F2C:
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C5F38: lifted from generated/ by lift.py.
lifted::Result f821C5F38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821C5F38 (lifted)");
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
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c5f64
  if (!cr6.eq) goto loc_821C5F64;
  // bl 0x821c02b8
  // (left out: dead in C++)
  { const lifted::Result o = f821C02B8(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_821C5F64:
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
}

// sub_821C61B0: lifted from generated/ by lift.py.
lifted::Result f821C61B0(uint8_t* base, uint64_t r1, uint64_t r13, double f1) {
  ITC_NATIVE_RUNNING("sub_821C61B0 (lifted)");
  uint64_t r3 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r8,1
  r8 = uint64_t(int64_t(1));
  // li r7,3
  r7 = uint64_t(int64_t(3));
  // lwz r11,-31600(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31600);
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
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
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // stw r7,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r7));
  // stw r8,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r8));
  // lwz r11,-31600(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31600);
  // stw r8,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r8));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // stw r11,-31596(r10)
  REX_STORE_U32(uint32_t(r10) + -31596, uint32_t(r11));
  // bl 0x821c02b8
  // (left out: dead in C++)
  { const lifted::Result o = f821C02B8(base, r1, r13, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c0348
  // (left out: dead in C++)
  r3 = f821C0348(base, r1, r13);
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_821C6238: lifted from generated/ by lift.py.
lifted::Result f821C6238(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821C6238 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,12
  r8 = uint64_t(int64_t(12));
  // li r5,48
  r5 = uint64_t(int64_t(48));
  // lwz r11,-31600(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31600);
  // stw r31,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r31));
  // stw r8,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r8));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r31));
  // stw r31,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r31));
  // stw r31,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r31));
  // lwz r11,-31600(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31600);
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // stw r9,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r9));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // addi r4,r11,140
  r4 = uint64_t(int64_t(int64_t(r11) + 140));
  // addi r3,r11,332
  r3 = uint64_t(int64_t(int64_t(r11) + 332));
  // stw r11,-31596(r10)
  REX_STORE_U32(uint32_t(r10) + -31596, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c03d8
  // (left out: dead in C++)
  { const lifted::Result o = f821C03D8(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,10
  r4 = uint64_t(int64_t(10));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r30,r3
  r30 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // stw r31,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // stw r31,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // stw r31,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c04e0
  // (left out: dead in C++)
  { const lifted::Result o = f821C04E0(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C6350: lifted from generated/ by lift.py.
lifted::Result f821C6350(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_821C6350 (lifted)");
  uint64_t r3 = 0, r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r29,0(r3)
  r29 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // li r5,24
  r5 = uint64_t(int64_t(24));
  // lwz r11,-31600(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31600);
  // stw r31,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r31));
  // stw r8,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r8));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r31,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r31));
  // stw r31,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r31));
  // stw r31,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r31));
  // stw r31,40(r11)
  REX_STORE_U32(uint32_t(r11) + 40, uint32_t(r31));
  // stw r31,48(r11)
  REX_STORE_U32(uint32_t(r11) + 48, uint32_t(r31));
  // stw r31,52(r11)
  REX_STORE_U32(uint32_t(r11) + 52, uint32_t(r31));
  // stw r31,36(r11)
  REX_STORE_U32(uint32_t(r11) + 36, uint32_t(r31));
  // stw r31,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r31));
  // stw r31,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r31));
  // stw r31,28(r11)
  REX_STORE_U32(uint32_t(r11) + 28, uint32_t(r31));
  // stw r31,32(r11)
  REX_STORE_U32(uint32_t(r11) + 32, uint32_t(r31));
  // lwz r11,-31600(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31600);
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // stw r9,64(r11)
  REX_STORE_U32(uint32_t(r11) + 64, uint32_t(r9));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // addi r4,r11,212
  r4 = uint64_t(int64_t(int64_t(r11) + 212));
  // addi r3,r11,404
  r3 = uint64_t(int64_t(int64_t(r11) + 404));
  // stw r11,-31596(r10)
  REX_STORE_U32(uint32_t(r10) + -31596, uint32_t(r11));
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c0640
  // (left out: dead in C++)
  { const lifted::Result o = f821C0640(base, r1, r13, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // li r5,6
  r5 = uint64_t(int64_t(6));
  // li r4,12
  r4 = uint64_t(int64_t(12));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r30,r3
  r30 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r3,r30
  r3 = r30;
  // stw r31,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,7
  r4 = uint64_t(int64_t(7));
  // mr r3,r30
  r3 = r30;
  // stw r31,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // mr r3,r30
  r3 = r30;
  // stw r31,80(r11)
  REX_STORE_U32(uint32_t(r11) + 80, uint32_t(r31));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r3
  r4 = r3;
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // mr r3,r29
  r3 = r29;
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c0750
  // (left out: dead in C++)
  { const lifted::Result o = f821C0750(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C7188: lifted from generated/ by lift.py.
lifted::Result f821C7188(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821C7188 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
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
  // li r25,7
  r25 = uint64_t(int64_t(7));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r11,23756
  r3 = uint64_t(int64_t(int64_t(r11) + 23756));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r25,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r25));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lis r27,-32146
  r27 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // li r30,-1
  r30 = uint64_t(int64_t(-1));
  // addi r28,r11,-23176
  r28 = uint64_t(int64_t(int64_t(r11) + -23176));
  // li r24,1
  r24 = uint64_t(int64_t(1));
  // bne cr6,0x821c7318
  if (!cr6.eq) goto loc_821C7318;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,21
  r4 = uint64_t(int64_t(21));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31628(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31628);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // addi r5,r11,2
  r5 = uint64_t(int64_t(int64_t(r11) + 2));
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C470, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,-31624(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31624);
  // mr r29,r30
  r29 = r30;
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r10,664(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 664);
  // mulli r11,r9,7
  r11 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(7))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r28
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r28));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821c72ec
  if (cr6.lt) goto loc_821C72EC;
  // beq cr6,0x821c72e0
  if (cr6.eq) goto loc_821C72E0;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821c72bc
  if (cr6.lt) goto loc_821C72BC;
  // beq cr6,0x821c7298
  if (cr6.eq) goto loc_821C7298;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // blt cr6,0x821c7274
  if (cr6.lt) goto loc_821C7274;
  // beq cr6,0x821c7268
  if (cr6.eq) goto loc_821C7268;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bge cr6,0x821c72f4
  if (!cr6.lt) goto loc_821C72F4;
  // li r29,23
  r29 = uint64_t(int64_t(23));
  // bl 0x821c6e50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C6E50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c72f4
  goto loc_821C72F4;
loc_821C7268:
  // li r29,21
  r29 = uint64_t(int64_t(21));
  // bl 0x821c6d48
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C6D48, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c72f4
  goto loc_821C72F4;
loc_821C7274:
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // bne cr6,0x821c728c
  if (!cr6.eq) goto loc_821C728C;
  // li r29,25
  r29 = uint64_t(int64_t(25));
  // bl 0x821c5900
  // (left out: dead in C++)
  { const lifted::Result o = f821C5900(base, r1, r13, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c6350
  // (left out: dead in C++)
  { const lifted::Result o = f821C6350(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821c72f4
  goto loc_821C72F4;
loc_821C728C:
  // li r29,24
  r29 = uint64_t(int64_t(24));
  // bl 0x821c57d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C57D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c72f4
  goto loc_821C72F4;
loc_821C7298:
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // bne cr6,0x821c72b0
  if (!cr6.eq) goto loc_821C72B0;
  // li r29,19
  r29 = uint64_t(int64_t(19));
  // bl 0x821c56b0
  // (left out: dead in C++)
  { const lifted::Result o = f821C56B0(base, r1, r13, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c6238
  // (left out: dead in C++)
  { const lifted::Result o = f821C6238(base, r1, r13, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821c72f4
  goto loc_821C72F4;
loc_821C72B0:
  // li r29,18
  r29 = uint64_t(int64_t(18));
  // bl 0x821c5580
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C5580, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c72f4
  goto loc_821C72F4;
loc_821C72BC:
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // bne cr6,0x821c72d4
  if (!cr6.eq) goto loc_821C72D4;
  // li r29,17
  r29 = uint64_t(int64_t(17));
  // bl 0x821c5350
  // (left out: dead in C++)
  { const lifted::Result o = f821C5350(base, r1, r13, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c61b0
  // (left out: dead in C++)
  { const lifted::Result o = f821C61B0(base, r1, r13, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x821c72f4
  goto loc_821C72F4;
loc_821C72D4:
  // li r29,16
  r29 = uint64_t(int64_t(16));
  // bl 0x821c51d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C51D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c72f4
  goto loc_821C72F4;
loc_821C72E0:
  // li r29,15
  r29 = uint64_t(int64_t(15));
  // bl 0x821c4fd0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C4FD0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821c72f4
  goto loc_821C72F4;
loc_821C72EC:
  // li r29,14
  r29 = uint64_t(int64_t(14));
  // bl 0x821c4db0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C4DB0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C72F4:
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r5,r29
  r5 = r29;
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r24,0(r26)
  REX_STORE_U32(uint32_t(r26) + 0, uint32_t(r24));
loc_821C7318:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821c75d0
  if (!cr0.eq) goto loc_821C75D0;
  // lwz r11,-31624(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + -31624);
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r10,664(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 664);
  // mulli r11,r9,7
  r11 = uint64_t(int64_t(static_cast<int64_t>(r9 * static_cast<uint64_t>(7))));
  // add r11,r11,r10
  r11 = r11 + r10;
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r28
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r28));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821c7598
  if (cr6.lt) goto loc_821C7598;
  // beq cr6,0x821c7558
  if (cr6.eq) goto loc_821C7558;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821c74dc
  if (cr6.lt) goto loc_821C74DC;
  // beq cr6,0x821c7450
  if (cr6.eq) goto loc_821C7450;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // blt cr6,0x821c73c4
  if (cr6.lt) goto loc_821C73C4;
  // beq cr6,0x821c73a4
  if (cr6.eq) goto loc_821C73A4;
  // cmplwi cr6,r11,7
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 7, so);
  // bge cr6,0x821c75d0
  if (!cr6.lt) goto loc_821C75D0;
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31632(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31632);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,10
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 10, so);
  // beq cr6,0x821c738c
  if (cr6.eq) goto loc_821C738C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C738C:
  // li r10,10
  r10 = uint64_t(int64_t(10));
loc_821C7390:
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r30,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r30));
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // lwz r11,-31632(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31632);
  // b 0x821c75c8
  goto loc_821C75C8;
loc_821C73A4:
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31632(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31632);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // beq cr6,0x821c73bc
  if (cr6.eq) goto loc_821C73BC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C73BC:
  // li r10,9
  r10 = uint64_t(int64_t(9));
  // b 0x821c7390
  goto loc_821C7390;
loc_821C73C4:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // lwz r11,-31632(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31632);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bne cr6,0x821c7438
  if (!cr6.eq) goto loc_821C7438;
  // cmpwi cr6,r9,8
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 8, so);
  // beq cr6,0x821c73e4
  if (cr6.eq) goto loc_821C73E4;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821C73E4:
  // li r8,37
  r8 = uint64_t(int64_t(37));
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // li r9,8
  r9 = uint64_t(int64_t(8));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r8,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r8));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // lwz r11,-31632(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31632);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31604(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821c7420
  if (cr6.eq) goto loc_821C7420;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C7420:
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
loc_821C7428:
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
loc_821C742C:
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // b 0x821c75d0
  goto loc_821C75D0;
loc_821C7438:
  // cmpwi cr6,r9,7
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 7, so);
  // beq cr6,0x821c7444
  if (cr6.eq) goto loc_821C7444;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821C7444:
  // li r9,33
  r9 = uint64_t(int64_t(33));
  // stw r25,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r25));
  // b 0x821c75b8
  goto loc_821C75B8;
loc_821C7450:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // lwz r11,-31632(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31632);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bne cr6,0x821c74bc
  if (!cr6.eq) goto loc_821C74BC;
  // cmpwi cr6,r9,6
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 6, so);
  // beq cr6,0x821c7470
  if (cr6.eq) goto loc_821C7470;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821C7470:
  // li r9,29
  r9 = uint64_t(int64_t(29));
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // li r8,6
  r8 = uint64_t(int64_t(6));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // lwz r11,-31632(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31632);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31604(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821c74ac
  if (cr6.eq) goto loc_821C74AC;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C74AC:
  // li r10,4
  r10 = uint64_t(int64_t(4));
  // stw r24,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r24));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // b 0x821c742c
  goto loc_821C742C;
loc_821C74BC:
  // cmpwi cr6,r9,5
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 5, so);
  // beq cr6,0x821c74c8
  if (cr6.eq) goto loc_821C74C8;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821C74C8:
  // li r9,5
  r9 = uint64_t(int64_t(5));
  // li r7,25
  r7 = uint64_t(int64_t(25));
loc_821C74D0:
  // stw r7,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r7));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // b 0x821c75bc
  goto loc_821C75BC;
loc_821C74DC:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // lwz r11,-31632(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31632);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // bne cr6,0x821c7540
  if (!cr6.eq) goto loc_821C7540;
  // cmpwi cr6,r9,4
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 4, so);
  // beq cr6,0x821c74fc
  if (cr6.eq) goto loc_821C74FC;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821C74FC:
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // li r7,21
  r7 = uint64_t(int64_t(21));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r7,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r7));
  // lwz r11,-31632(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31632);
  // stw r9,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r9));
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31604(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31604);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821c7538
  if (cr0.eq) goto loc_821C7538;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C7538:
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // b 0x821c7428
  goto loc_821C7428;
loc_821C7540:
  // cmpwi cr6,r9,3
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 3, so);
  // beq cr6,0x821c754c
  if (cr6.eq) goto loc_821C754C;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821C754C:
  // li r9,3
  r9 = uint64_t(int64_t(3));
  // li r7,17
  r7 = uint64_t(int64_t(17));
  // b 0x821c74d0
  goto loc_821C74D0;
loc_821C7558:
  // lis r9,-32146
  r9 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31632(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31632);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // beq cr6,0x821c7570
  if (cr6.eq) goto loc_821C7570;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C7570:
  // li r10,13
  r10 = uint64_t(int64_t(13));
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // li r8,2
  r8 = uint64_t(int64_t(2));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // lwz r11,-31632(r9)
  r11 = REX_LOAD_U32(uint32_t(r9) + -31632);
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
  // b 0x821c75d0
  goto loc_821C75D0;
loc_821C7598:
  // lis r10,-32146
  r10 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31632(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31632);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r9,1
  cr6 = lifted::Compare<int32_t>(int32_t(r9), 1, so);
  // beq cr6,0x821c75b0
  if (cr6.eq) goto loc_821C75B0;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821C75B0:
  // li r9,9
  r9 = uint64_t(int64_t(9));
  // stw r24,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r24));
loc_821C75B8:
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
loc_821C75BC:
  // stw r30,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r30));
  // stw r30,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r30));
  // lwz r11,-31632(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + -31632);
loc_821C75C8:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // stw r8,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r8));
loc_821C75D0:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // b 0x825a6ec8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821C79B8: lifted from generated/ by lift.py.
lifted::Result f821C79B8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821C79B8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c7a9c
  if (!cr6.eq) goto loc_821C7A9C;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31628(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31628);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821c7a54
  if (cr6.lt) goto loc_821C7A54;
  // beq cr6,0x821c7a4c
  if (cr6.eq) goto loc_821C7A4C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821c7a44
  if (cr6.lt) goto loc_821C7A44;
  // bne cr6,0x821c7a60
  if (!cr6.eq) goto loc_821C7A60;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // mr r3,r31
  r3 = r31;
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c7a3c
  if (!cr6.eq) goto loc_821C7A3C;
  // li r4,344
  r4 = uint64_t(int64_t(344));
  // b 0x821c7a5c
  goto loc_821C7A5C;
loc_821C7A3C:
  // li r4,345
  r4 = uint64_t(int64_t(345));
  // b 0x821c7a5c
  goto loc_821C7A5C;
loc_821C7A44:
  // li r4,343
  r4 = uint64_t(int64_t(343));
  // b 0x821c7a58
  goto loc_821C7A58;
loc_821C7A4C:
  // li r4,342
  r4 = uint64_t(int64_t(342));
  // b 0x821c7a58
  goto loc_821C7A58;
loc_821C7A54:
  // li r4,341
  r4 = uint64_t(int64_t(341));
loc_821C7A58:
  // mr r3,r31
  r3 = r31;
loc_821C7A5C:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821C7A60:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C470, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,7
  r5 = uint64_t(int64_t(7));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c4be0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C4BE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821C7A9C:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821c7aec
  if (!cr0.eq) goto loc_821C7AEC;
  // lis r8,-32146
  r8 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31632(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31632);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821c7ac8
  if (cr0.eq) goto loc_821C7AC8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C7AC8:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r7,5
  r7 = uint64_t(int64_t(5));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r7,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r7));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,-31632(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31632);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
loc_821C7AEC:
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

// sub_821C7BF0: lifted from generated/ by lift.py.
lifted::Result f821C7BF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821C7BF0 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c7cac
  if (!cr6.eq) goto loc_821C7CAC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31628(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31628);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,12
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 12, so);
  // beq cr6,0x821c7c4c
  if (cr6.eq) goto loc_821C7C4C;
  // addi r4,r11,510
  r4 = uint64_t(int64_t(int64_t(r11) + 510));
  // b 0x821c7c68
  goto loc_821C7C68;
loc_821C7C4C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,527
  r4 = uint64_t(int64_t(527));
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821c7c68
  if (cr6.eq) goto loc_821C7C68;
  // li r4,528
  r4 = uint64_t(int64_t(528));
loc_821C7C68:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C470, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,9
  r5 = uint64_t(int64_t(9));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c4be0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C4BE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821C7CAC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821c7cfc
  if (!cr0.eq) goto loc_821C7CFC;
  // lis r8,-32146
  r8 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31632(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31632);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821c7cd8
  if (cr0.eq) goto loc_821C7CD8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C7CD8:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r7,5
  r7 = uint64_t(int64_t(5));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r7,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r7));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,-31632(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31632);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
loc_821C7CFC:
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

// sub_821C7E00: lifted from generated/ by lift.py.
lifted::Result f821C7E00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_821C7E00 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821c7ebc
  if (!cr6.eq) goto loc_821C7EBC;
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31628(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31628);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // beq cr6,0x821c7e5c
  if (cr6.eq) goto loc_821C7E5C;
  // addi r4,r11,749
  r4 = uint64_t(int64_t(int64_t(r11) + 749));
  // b 0x821c7e78
  goto loc_821C7E78;
loc_821C7E5C:
  // lis r11,-32146
  r11 = uint64_t(int64_t(-2106720256));
  // li r4,766
  r4 = uint64_t(int64_t(766));
  // lwz r11,-31624(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + -31624);
  // lwz r11,660(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 660);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821c7e78
  if (cr6.eq) goto loc_821C7E78;
  // li r4,767
  r4 = uint64_t(int64_t(767));
loc_821C7E78:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211c470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C470, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,-1
  r4 = uint64_t(int64_t(-1));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x821c4be0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821C4BE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,0(r30)
  REX_STORE_U32(uint32_t(r30) + 0, uint32_t(r11));
loc_821C7EBC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211e1b8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211E1B8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // bne 0x821c7f0c
  if (!cr0.eq) goto loc_821C7F0C;
  // lis r8,-32146
  r8 = uint64_t(int64_t(-2106720256));
  // lwz r11,-31632(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31632);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpwi r10,0
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x821c7ee8
  if (cr0.eq) goto loc_821C7EE8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821C7EE8:
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r7,5
  r7 = uint64_t(int64_t(5));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r7,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r7));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // lwz r11,-31632(r8)
  r11 = REX_LOAD_U32(uint32_t(r8) + -31632);
  // stw r10,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r10));
loc_821C7F0C:
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

// sub_821D4540: lifted from generated/ by lift.py.
lifted::Result f821D4540(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821D4540 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r29,r10,r11
  r29 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,10040(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 10040);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 8);
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // bne cr6,0x821d48b4
  if (!cr6.eq) goto loc_821D48B4;
  // lwz r11,9656(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9656);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821d4680
  if (!cr6.eq) goto loc_821D4680;
  // lbz r9,9660(r30)
  r9 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi cr6,r9,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 3, so);
  // bne cr6,0x821d4680
  if (!cr6.eq) goto loc_821D4680;
  // lwz r11,40(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 40);
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821d4654
  if (cr6.eq) goto loc_821D4654;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r10,9663(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r11,r3
  r11 = r3;
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8112(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8112);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // lbz r11,9679(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9679);
loc_821D460C:
  // addi r4,r11,733
  r4 = uint64_t(int64_t(int64_t(r11) + 733));
loc_821D4610:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,734
  r4 = uint64_t(int64_t(int64_t(r11) + 734));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,734
  r4 = uint64_t(int64_t(int64_t(r11) + 734));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // addi r4,r11,734
  r4 = uint64_t(int64_t(int64_t(r11) + 734));
  // b 0x821d4b90
  goto loc_821D4B90;
loc_821D4654:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // addi r4,r11,735
  r4 = uint64_t(int64_t(int64_t(r11) + 735));
  // b 0x821d4610
  goto loc_821D4610;
loc_821D4680:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821d4810
  if (!cr6.eq) goto loc_821D4810;
  // lbz r9,9660(r30)
  r9 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi cr6,r9,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 4, so);
  // bne cr6,0x821d4810
  if (!cr6.eq) goto loc_821D4810;
  // lwz r11,40(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 40);
  // li r4,642
  r4 = uint64_t(int64_t(642));
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821d4794
  if (cr6.eq) goto loc_821D4794;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,643
  r4 = uint64_t(int64_t(643));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r10,9663(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8112(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8112);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // lbz r11,9679(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9679);
  // addi r4,r11,631
  r4 = uint64_t(int64_t(int64_t(r11) + 631));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9679(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9679);
loc_821D474C:
  // addi r4,r11,570
  r4 = uint64_t(int64_t(int64_t(r11) + 570));
loc_821D4750:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,571
  r4 = uint64_t(int64_t(int64_t(r11) + 571));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,571
  r4 = uint64_t(int64_t(int64_t(r11) + 571));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // addi r4,r11,571
  r4 = uint64_t(int64_t(int64_t(r11) + 571));
  // b 0x821d4b90
  goto loc_821D4B90;
loc_821D4794:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,642
  r4 = uint64_t(int64_t(642));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,633
  r4 = uint64_t(int64_t(int64_t(r11) + 633));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // addi r4,r11,572
  r4 = uint64_t(int64_t(int64_t(r11) + 572));
  // b 0x821d4750
  goto loc_821D4750;
loc_821D4810:
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x821d501c
  if (!cr6.eq) goto loc_821D501C;
  // lbz r11,9660(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x821d501c
  if (!cr0.eq) goto loc_821D501C;
  // lwz r11,40(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 40);
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821d4888
  if (cr6.eq) goto loc_821D4888;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r10,9663(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r11,r3
  r11 = r3;
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8112(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8112);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // lbz r11,9679(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9679);
  // b 0x821d4b4c
  goto loc_821D4B4C;
loc_821D4888:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // addi r4,r11,772
  r4 = uint64_t(int64_t(int64_t(r11) + 772));
  // b 0x821d4b50
  goto loc_821D4B50;
loc_821D48B4:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821d4b9c
  if (!cr6.eq) goto loc_821D4B9C;
  // lwz r11,9656(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9656);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821d4964
  if (!cr6.eq) goto loc_821D4964;
  // lbz r9,9660(r30)
  r9 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi cr6,r9,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 3, so);
  // bne cr6,0x821d4964
  if (!cr6.eq) goto loc_821D4964;
  // lwz r11,40(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 40);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821d4938
  if (cr6.eq) goto loc_821D4938;
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r10,9663(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r11,r3
  r11 = r3;
  // li r4,734
  r4 = uint64_t(int64_t(734));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8112(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8112);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // b 0x821d4610
  goto loc_821D4610;
loc_821D4938:
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // b 0x821d460c
  goto loc_821D460C;
loc_821D4964:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821d4aac
  if (!cr6.eq) goto loc_821D4AAC;
  // lbz r9,9660(r30)
  r9 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi cr6,r9,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), 4, so);
  // bne cr6,0x821d4aac
  if (!cr6.eq) goto loc_821D4AAC;
  // lwz r11,40(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 40);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821d4a30
  if (cr6.eq) goto loc_821D4A30;
  // li r4,642
  r4 = uint64_t(int64_t(642));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,643
  r4 = uint64_t(int64_t(643));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,30
  r4 = uint64_t(int64_t(30));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r10,9663(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r11,r3
  r11 = r3;
  // li r4,632
  r4 = uint64_t(int64_t(632));
  // mr r3,r31
  r3 = r31;
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8112(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8112);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,571
  r4 = uint64_t(int64_t(571));
  // b 0x821d4750
  goto loc_821D4750;
loc_821D4A30:
  // li r4,643
  r4 = uint64_t(int64_t(643));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,643
  r4 = uint64_t(int64_t(643));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,631
  r4 = uint64_t(int64_t(int64_t(r11) + 631));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,632
  r4 = uint64_t(int64_t(int64_t(r11) + 632));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // b 0x821d474c
  goto loc_821D474C;
loc_821D4AAC:
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x821d501c
  if (!cr6.eq) goto loc_821D501C;
  // lbz r11,9660(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x821d501c
  if (!cr0.eq) goto loc_821D501C;
  // lwz r11,40(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 40);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821d4b24
  if (cr6.eq) goto loc_821D4B24;
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r10,9663(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r11,r3
  r11 = r3;
  // li r4,771
  r4 = uint64_t(int64_t(771));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,8112(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8112);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // b 0x821d4b50
  goto loc_821D4B50;
loc_821D4B24:
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
loc_821D4B4C:
  // addi r4,r11,770
  r4 = uint64_t(int64_t(int64_t(r11) + 770));
loc_821D4B50:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,771
  r4 = uint64_t(int64_t(int64_t(r11) + 771));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,771
  r4 = uint64_t(int64_t(int64_t(r11) + 771));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9663(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9663);
  // addi r4,r11,771
  r4 = uint64_t(int64_t(int64_t(r11) + 771));
loc_821D4B90:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821d501c
  goto loc_821D501C;
loc_821D4B9C:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821d4ca0
  if (cr6.eq) goto loc_821D4CA0;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821d4ca0
  if (cr0.eq) goto loc_821D4CA0;
  // lwz r11,9656(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9656);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821d4c00
  if (!cr6.eq) goto loc_821D4C00;
  // lbz r10,9660(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bne cr6,0x821d4c00
  if (!cr6.eq) goto loc_821D4C00;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821D4BE4:
  // addi r4,r30,734
  r4 = uint64_t(int64_t(int64_t(r30) + 734));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821d4be4
  if (cr6.lt) goto loc_821D4BE4;
  // b 0x821d4c84
  goto loc_821D4C84;
loc_821D4C00:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821d4c54
  if (!cr6.eq) goto loc_821D4C54;
  // lbz r10,9660(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // bne cr6,0x821d4c54
  if (!cr6.eq) goto loc_821D4C54;
  // li r30,571
  r30 = uint64_t(int64_t(571));
  // li r29,10
  r29 = uint64_t(int64_t(10));
loc_821D4C1C:
  // addi r4,r30,61
  r4 = uint64_t(int64_t(int64_t(r30) + 61));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x821d4c1c
  if (!cr0.eq) goto loc_821D4C1C;
  // li r4,642
  r4 = uint64_t(int64_t(642));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,643
  r4 = uint64_t(int64_t(643));
  // b 0x821d4c94
  goto loc_821D4C94;
loc_821D4C54:
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x821d501c
  if (!cr6.eq) goto loc_821D501C;
  // lbz r11,9660(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x821d501c
  if (!cr0.eq) goto loc_821D501C;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821D4C6C:
  // addi r4,r30,771
  r4 = uint64_t(int64_t(int64_t(r30) + 771));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821d4c6c
  if (cr6.lt) goto loc_821D4C6C;
loc_821D4C84:
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
loc_821D4C94:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821d501c
  goto loc_821D501C;
loc_821D4CA0:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821d501c
  if (cr6.eq) goto loc_821D501C;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821d501c
  if (cr0.eq) goto loc_821D501C;
  // lwz r11,9656(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9656);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821d4ddc
  if (!cr6.eq) goto loc_821D4DDC;
  // lbz r10,9660(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi cr6,r10,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 3, so);
  // bne cr6,0x821d4ddc
  if (!cr6.eq) goto loc_821D4DDC;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_821D4CE8:
  // addi r4,r29,734
  r4 = uint64_t(int64_t(int64_t(r29) + 734));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r29,10
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 10, so);
  // blt cr6,0x821d4ce8
  if (cr6.lt) goto loc_821D4CE8;
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9661(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9661);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,515
  r4 = uint64_t(int64_t(int64_t(r11) + 515));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9661(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9661);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,515
  r4 = uint64_t(int64_t(int64_t(r11) + 515));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9661(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9661);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,466
  r4 = uint64_t(int64_t(int64_t(r11) + 466));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9661(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9661);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,466
  r4 = uint64_t(int64_t(int64_t(r11) + 466));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x821cd868
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CD868, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9661(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9661);
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x821d4da4
  if (!cr6.eq) goto loc_821D4DA4;
  // addi r4,r11,476
  r4 = uint64_t(int64_t(int64_t(r11) + 476));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9661(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9661);
  // addi r4,r11,476
  r4 = uint64_t(int64_t(int64_t(r11) + 476));
  // b 0x821d4db0
  goto loc_821D4DB0;
loc_821D4DA4:
  // li r4,486
  r4 = uint64_t(int64_t(486));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,486
  r4 = uint64_t(int64_t(486));
loc_821D4DB0:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,992
  r4 = uint64_t(int64_t(992));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,992
  r4 = uint64_t(int64_t(992));
  // b 0x821d5004
  goto loc_821D5004;
loc_821D4DDC:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821d4efc
  if (!cr6.eq) goto loc_821D4EFC;
  // lbz r10,9660(r30)
  r10 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi cr6,r10,4
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 4, so);
  // bne cr6,0x821d4efc
  if (!cr6.eq) goto loc_821D4EFC;
  // li r29,571
  r29 = uint64_t(int64_t(571));
  // li r28,10
  r28 = uint64_t(int64_t(10));
loc_821D4DF8:
  // addi r4,r29,61
  r4 = uint64_t(int64_t(int64_t(r29) + 61));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r29
  r4 = r29;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r28,r28,-1
  ca = uint32_t(r28) > 0;
  r28 = uint64_t(int64_t(int64_t(r28) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r28), 0, so);
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // bne 0x821d4df8
  if (!cr0.eq) goto loc_821D4DF8;
  // li r4,642
  r4 = uint64_t(int64_t(642));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,643
  r4 = uint64_t(int64_t(643));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,515
  r4 = uint64_t(int64_t(int64_t(r11) + 515));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,515
  r4 = uint64_t(int64_t(int64_t(r11) + 515));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,466
  r4 = uint64_t(int64_t(int64_t(r11) + 466));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,466
  r4 = uint64_t(int64_t(int64_t(r11) + 466));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,0
  r10 = uint64_t(int64_t(0));
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // mr r3,r31
  r3 = r31;
  // ori r10,r10,36368
  r10 = r10 | 36368;
  // lwzx r10,r30,r10
  r10 = REX_LOAD_U32(uint32_t(r30) + uint32_t(r10));
  // cmpw cr6,r10,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r10), int32_t(r11), so);
  // bne cr6,0x821d4ec4
  if (!cr6.eq) goto loc_821D4EC4;
  // addi r4,r11,476
  r4 = uint64_t(int64_t(int64_t(r11) + 476));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // addi r4,r11,476
  r4 = uint64_t(int64_t(int64_t(r11) + 476));
  // b 0x821d4ed0
  goto loc_821D4ED0;
loc_821D4EC4:
  // li r4,486
  r4 = uint64_t(int64_t(486));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,486
  r4 = uint64_t(int64_t(486));
loc_821D4ED0:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,996
  r4 = uint64_t(int64_t(996));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,996
  r4 = uint64_t(int64_t(996));
  // b 0x821d5004
  goto loc_821D5004;
loc_821D4EFC:
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x821d501c
  if (!cr6.eq) goto loc_821D501C;
  // lbz r11,9660(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9660);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x821d501c
  if (!cr0.eq) goto loc_821D501C;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_821D4F14:
  // addi r4,r29,771
  r4 = uint64_t(int64_t(int64_t(r29) + 771));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r29,10
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 10, so);
  // blt cr6,0x821d4f14
  if (cr6.lt) goto loc_821D4F14;
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,515
  r4 = uint64_t(int64_t(int64_t(r11) + 515));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,515
  r4 = uint64_t(int64_t(int64_t(r11) + 515));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,466
  r4 = uint64_t(int64_t(int64_t(r11) + 466));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,466
  r4 = uint64_t(int64_t(int64_t(r11) + 466));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x821cd868
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CD868, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x821d4fd0
  if (!cr6.eq) goto loc_821D4FD0;
  // addi r4,r11,476
  r4 = uint64_t(int64_t(int64_t(r11) + 476));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lbz r11,9662(r30)
  r11 = REX_LOAD_U8(uint32_t(r30) + 9662);
  // addi r4,r11,476
  r4 = uint64_t(int64_t(int64_t(r11) + 476));
  // b 0x821d4fdc
  goto loc_821D4FDC;
loc_821D4FD0:
  // li r4,486
  r4 = uint64_t(int64_t(486));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,486
  r4 = uint64_t(int64_t(486));
loc_821D4FDC:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1000
  r4 = uint64_t(int64_t(1000));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1000
  r4 = uint64_t(int64_t(1000));
loc_821D5004:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821D501C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821E9920: lifted from generated/ by lift.py.
lifted::Result f821E9920(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821E9920 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r29,r10,r11
  r29 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e9990
  if (cr6.eq) goto loc_821E9990;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821e9bc8
  if (!cr0.eq) goto loc_821E9BC8;
loc_821E9990:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e99bc
  if (cr6.eq) goto loc_821E99BC;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821e9bc8
  if (!cr0.eq) goto loc_821E9BC8;
loc_821E99BC:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e99e8
  if (cr6.eq) goto loc_821E99E8;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821e9b2c
  if (!cr0.eq) goto loc_821E9B2C;
loc_821E99E8:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e9a14
  if (cr6.eq) goto loc_821E9A14;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821e9b2c
  if (!cr0.eq) goto loc_821E9B2C;
loc_821E9A14:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e9a78
  if (cr6.eq) goto loc_821E9A78;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821e9a78
  if (cr0.eq) goto loc_821E9A78;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821E9A44:
  // addi r4,r30,887
  r4 = uint64_t(int64_t(int64_t(r30) + 887));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821e9a44
  if (cr6.lt) goto loc_821E9A44;
  // li r4,897
  r4 = uint64_t(int64_t(897));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,898
  r4 = uint64_t(int64_t(898));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821e9c94
  goto loc_821E9C94;
loc_821E9A78:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e9c94
  if (cr6.eq) goto loc_821E9C94;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821e9c94
  if (cr0.eq) goto loc_821E9C94;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821E9AA8:
  // addi r4,r30,887
  r4 = uint64_t(int64_t(int64_t(r30) + 887));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821e9aa8
  if (cr6.lt) goto loc_821E9AA8;
  // li r4,897
  r4 = uint64_t(int64_t(897));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,898
  r4 = uint64_t(int64_t(898));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,982
  r4 = uint64_t(int64_t(982));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,982
  r4 = uint64_t(int64_t(982));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1010
  r4 = uint64_t(int64_t(1010));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1010
  r4 = uint64_t(int64_t(1010));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821e9c94
  goto loc_821E9C94;
loc_821E9B2C:
  // lwz r11,9708(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821e9c94
  if (cr6.eq) goto loc_821E9C94;
  // lwz r11,30496(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30496);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821e9b98
  if (cr6.eq) goto loc_821E9B98;
  // li r4,897
  r4 = uint64_t(int64_t(897));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,898
  r4 = uint64_t(int64_t(898));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,36
  r4 = uint64_t(int64_t(36));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,30492(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 30492);
  // li r4,890
  r4 = uint64_t(int64_t(890));
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
loc_821E9B84:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // mr r11,r3
  r11 = r3;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // b 0x821e9c4c
  goto loc_821E9C4C;
loc_821E9B98:
  // li r4,898
  r4 = uint64_t(int64_t(898));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,898
  r4 = uint64_t(int64_t(898));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30492(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30492);
  // addi r4,r11,886
  r4 = uint64_t(int64_t(int64_t(r11) + 886));
  // b 0x821e9c4c
  goto loc_821E9C4C;
loc_821E9BC8:
  // lwz r11,9708(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821e9c94
  if (cr6.eq) goto loc_821E9C94;
  // lwz r11,30496(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30496);
  // li r4,897
  r4 = uint64_t(int64_t(897));
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821e9c24
  if (cr6.eq) goto loc_821E9C24;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,898
  r4 = uint64_t(int64_t(898));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,36
  r4 = uint64_t(int64_t(36));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,30492(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 30492);
  // li r4,887
  r4 = uint64_t(int64_t(887));
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // b 0x821e9b84
  goto loc_821E9B84;
loc_821E9C24:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,897
  r4 = uint64_t(int64_t(897));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30492(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30492);
  // addi r4,r11,888
  r4 = uint64_t(int64_t(int64_t(r11) + 888));
loc_821E9C4C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30492(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30492);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,887
  r4 = uint64_t(int64_t(int64_t(r11) + 887));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30492(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30492);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,887
  r4 = uint64_t(int64_t(int64_t(r11) + 887));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30492(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30492);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,887
  r4 = uint64_t(int64_t(int64_t(r11) + 887));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821E9C94:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821E9DD8: lifted from generated/ by lift.py.
lifted::Result f821E9DD8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821E9DD8 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r29,r10,r11
  r29 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e9e48
  if (cr6.eq) goto loc_821E9E48;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ea058
  if (!cr0.eq) goto loc_821EA058;
loc_821E9E48:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e9e74
  if (cr6.eq) goto loc_821E9E74;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ea058
  if (!cr0.eq) goto loc_821EA058;
loc_821E9E74:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e9ea0
  if (cr6.eq) goto loc_821E9EA0;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821e9fbc
  if (!cr0.eq) goto loc_821E9FBC;
loc_821E9EA0:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e9ecc
  if (cr6.eq) goto loc_821E9ECC;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821e9fbc
  if (!cr0.eq) goto loc_821E9FBC;
loc_821E9ECC:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821e9f30
  if (cr6.eq) goto loc_821E9F30;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821e9f30
  if (cr0.eq) goto loc_821E9F30;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821E9EFC:
  // addi r4,r30,926
  r4 = uint64_t(int64_t(int64_t(r30) + 926));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821e9efc
  if (cr6.lt) goto loc_821E9EFC;
  // li r4,936
  r4 = uint64_t(int64_t(936));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,937
  r4 = uint64_t(int64_t(937));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ea124
  goto loc_821EA124;
loc_821E9F30:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ea124
  if (cr6.eq) goto loc_821EA124;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ea124
  if (cr0.eq) goto loc_821EA124;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821E9F60:
  // addi r4,r30,926
  r4 = uint64_t(int64_t(int64_t(r30) + 926));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821e9f60
  if (cr6.lt) goto loc_821E9F60;
  // li r4,936
  r4 = uint64_t(int64_t(936));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,937
  r4 = uint64_t(int64_t(937));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,984
  r4 = uint64_t(int64_t(984));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,984
  r4 = uint64_t(int64_t(984));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ea124
  goto loc_821EA124;
loc_821E9FBC:
  // lwz r11,9708(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ea124
  if (cr6.eq) goto loc_821EA124;
  // lwz r11,30504(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30504);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ea028
  if (cr6.eq) goto loc_821EA028;
  // li r4,936
  r4 = uint64_t(int64_t(936));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,937
  r4 = uint64_t(int64_t(937));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,37
  r4 = uint64_t(int64_t(37));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,30500(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 30500);
  // li r4,928
  r4 = uint64_t(int64_t(928));
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
loc_821EA014:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // mr r11,r3
  r11 = r3;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // b 0x821ea0dc
  goto loc_821EA0DC;
loc_821EA028:
  // li r4,937
  r4 = uint64_t(int64_t(937));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,937
  r4 = uint64_t(int64_t(937));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30500(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30500);
  // addi r4,r11,925
  r4 = uint64_t(int64_t(int64_t(r11) + 925));
  // b 0x821ea0dc
  goto loc_821EA0DC;
loc_821EA058:
  // lwz r11,9708(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ea124
  if (cr6.eq) goto loc_821EA124;
  // lwz r11,30504(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30504);
  // li r4,936
  r4 = uint64_t(int64_t(936));
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ea0b4
  if (cr6.eq) goto loc_821EA0B4;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,937
  r4 = uint64_t(int64_t(937));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,37
  r4 = uint64_t(int64_t(37));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,30500(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 30500);
  // li r4,926
  r4 = uint64_t(int64_t(926));
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // b 0x821ea014
  goto loc_821EA014;
loc_821EA0B4:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,936
  r4 = uint64_t(int64_t(936));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30500(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30500);
  // addi r4,r11,927
  r4 = uint64_t(int64_t(int64_t(r11) + 927));
loc_821EA0DC:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30500(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30500);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,926
  r4 = uint64_t(int64_t(int64_t(r11) + 926));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30500(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30500);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,926
  r4 = uint64_t(int64_t(int64_t(r11) + 926));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30500(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30500);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,926
  r4 = uint64_t(int64_t(int64_t(r11) + 926));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821EA124:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821EA898: lifted from generated/ by lift.py.
lifted::Result f821EA898(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821EA898 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f30,-48(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -48, std::bit_cast<uint64_t>(f30));
  // stfd f31,-40(r1)
  REX_STORE_U64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-144(r1)
  ea = -144 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r30,r3
  r30 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r29,r10,r11
  r29 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ea910
  if (cr6.eq) goto loc_821EA910;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eaa18
  if (!cr0.eq) goto loc_821EAA18;
loc_821EA910:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ea93c
  if (cr6.eq) goto loc_821EA93C;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eaa18
  if (!cr0.eq) goto loc_821EAA18;
loc_821EA93C:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ea968
  if (cr6.eq) goto loc_821EA968;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eaa18
  if (!cr0.eq) goto loc_821EAA18;
loc_821EA968:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ea994
  if (cr6.eq) goto loc_821EA994;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eaa18
  if (!cr0.eq) goto loc_821EAA18;
loc_821EA994:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ea9c0
  if (cr6.eq) goto loc_821EA9C0;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ea9ec
  if (!cr0.eq) goto loc_821EA9EC;
loc_821EA9C0:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eaad8
  if (cr6.eq) goto loc_821EAAD8;
  // lwz r3,0(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821eaad8
  if (cr0.eq) goto loc_821EAAD8;
loc_821EA9EC:
  // lwz r11,9652(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9652);
  // cmpwi cr6,r11,21
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 21, so);
  // beq cr6,0x821eaad8
  if (cr6.eq) goto loc_821EAAD8;
  // li r29,0
  r29 = uint64_t(int64_t(0));
loc_821EA9FC:
  // addi r4,r29,219
  r4 = uint64_t(int64_t(int64_t(r29) + 219));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r29,r29,1
  r29 = uint64_t(int64_t(int64_t(r29) + 1));
  // cmpwi cr6,r29,2
  cr6 = lifted::Compare<int32_t>(int32_t(r29), 2, so);
  // blt cr6,0x821ea9fc
  if (cr6.lt) goto loc_821EA9FC;
  // b 0x821eaad8
  goto loc_821EAAD8;
loc_821EAA18:
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // lwa r10,30512(r30)
  r10 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r30) + 30512))));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r10,-32255
  r10 = uint64_t(int64_t(-2113863680));
  // lfs f31,-18996(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + -18996);
  f31 = double(temp.f32);
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // frsp f0,f0
  f0 = double(float(f0));
  // lfs f30,8700(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 8700);
  f30 = double(temp.f32);
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // lwa r10,30516(r30)
  r10 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r30) + 30516))));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // stfs f0,12(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 12, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwa r11,30520(r30)
  r11 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r30) + 30520))));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fmuls f0,f0,f31
  f0 = double(float(f0 * f31));
  // fmuls f0,f0,f30
  f0 = double(float(f0 * f30));
  // stfs f0,12(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
loc_821EAAD8:
  // lwz r11,9708(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9708);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821eae50
  if (!cr6.eq) goto loc_821EAE50;
  // lwz r11,9652(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 9652);
  // cmpwi cr6,r11,21
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 21, so);
  // bne cr6,0x821eae50
  if (!cr6.eq) goto loc_821EAE50;
  // lwz r11,30532(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30532);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821eab48
  if (cr6.lt) goto loc_821EAB48;
  // beq cr6,0x821eab40
  if (cr6.eq) goto loc_821EAB40;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821eab38
  if (cr6.lt) goto loc_821EAB38;
  // beq cr6,0x821eab20
  if (cr6.eq) goto loc_821EAB20;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821eab54
  if (!cr6.lt) goto loc_821EAB54;
  // lwz r11,30540(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30540);
  // addi r4,r11,219
  r4 = uint64_t(int64_t(int64_t(r11) + 219));
  // b 0x821eab4c
  goto loc_821EAB4C;
loc_821EAB20:
  // li r4,307
  r4 = uint64_t(int64_t(307));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30536(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30536);
  // addi r4,r11,294
  r4 = uint64_t(int64_t(int64_t(r11) + 294));
  // b 0x821eab4c
  goto loc_821EAB4C;
loc_821EAB38:
  // li r4,306
  r4 = uint64_t(int64_t(306));
  // b 0x821eab4c
  goto loc_821EAB4C;
loc_821EAB40:
  // li r4,305
  r4 = uint64_t(int64_t(305));
  // b 0x821eab4c
  goto loc_821EAB4C;
loc_821EAB48:
  // li r4,304
  r4 = uint64_t(int64_t(304));
loc_821EAB4C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821EAB54:
  // lwz r11,30508(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30508);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821eadf4
  if (cr6.lt) goto loc_821EADF4;
  // beq cr6,0x821ead9c
  if (cr6.eq) goto loc_821EAD9C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821ead44
  if (cr6.lt) goto loc_821EAD44;
  // beq cr6,0x821eacac
  if (cr6.eq) goto loc_821EACAC;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821eae50
  if (!cr6.lt) goto loc_821EAE50;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // stfs f31,68(r10)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 68, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 68, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r29,r3
  r29 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r29
  r3 = r29;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 68, temp.u32);
  // lwz r11,30524(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30524);
  // addi r4,r11,4
  r4 = uint64_t(int64_t(int64_t(r11) + 4));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10812(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10812);
  f0 = double(temp.f32);
  // stfs f0,68(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
  // lwz r11,30528(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30528);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,219
  r4 = uint64_t(int64_t(int64_t(r11) + 219));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30528(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30528);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,219
  r4 = uint64_t(int64_t(int64_t(r11) + 219));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30528(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30528);
  // addi r4,r11,219
  r4 = uint64_t(int64_t(int64_t(r11) + 219));
  // b 0x821eae48
  goto loc_821EAE48;
loc_821EACAC:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,307
  r4 = uint64_t(int64_t(307));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,307
  r4 = uint64_t(int64_t(307));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,307
  r4 = uint64_t(int64_t(307));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30524(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30524);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,294
  r4 = uint64_t(int64_t(int64_t(r11) + 294));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30524(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30524);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,294
  r4 = uint64_t(int64_t(int64_t(r11) + 294));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30524(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 30524);
  // addi r4,r11,294
  r4 = uint64_t(int64_t(int64_t(r11) + 294));
  // b 0x821eae48
  goto loc_821EAE48;
loc_821EAD44:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,306
  r4 = uint64_t(int64_t(306));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,306
  r4 = uint64_t(int64_t(306));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,306
  r4 = uint64_t(int64_t(306));
  // b 0x821eae48
  goto loc_821EAE48;
loc_821EAD9C:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,305
  r4 = uint64_t(int64_t(305));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,305
  r4 = uint64_t(int64_t(305));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,305
  r4 = uint64_t(int64_t(305));
  // b 0x821eae48
  goto loc_821EAE48;
loc_821EADF4:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,23
  r4 = uint64_t(int64_t(23));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,304
  r4 = uint64_t(int64_t(304));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,304
  r4 = uint64_t(int64_t(304));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,304
  r4 = uint64_t(int64_t(304));
loc_821EAE48:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821EAE50:
  // mr r3,r30
  r3 = r30;
  // bl 0x821ea130
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821EA130, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // lfd f30,-48(r1)
  lifted::ScalarFloatMode();
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -48)));
  // lfd f31,-40(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821EB2A0: lifted from generated/ by lift.py.
lifted::Result f821EB2A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821EB2A0 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e8c
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f31,-40(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -40, std::bit_cast<uint64_t>(f31));
  // stwu r1,-128(r1)
  ea = -128 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eb314
  if (cr6.eq) goto loc_821EB314;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eb41c
  if (!cr0.eq) goto loc_821EB41C;
loc_821EB314:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eb340
  if (cr6.eq) goto loc_821EB340;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eb41c
  if (!cr0.eq) goto loc_821EB41C;
loc_821EB340:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eb36c
  if (cr6.eq) goto loc_821EB36C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eb41c
  if (!cr0.eq) goto loc_821EB41C;
loc_821EB36C:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eb398
  if (cr6.eq) goto loc_821EB398;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eb41c
  if (!cr0.eq) goto loc_821EB41C;
loc_821EB398:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eb3c4
  if (cr6.eq) goto loc_821EB3C4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eb3f0
  if (!cr0.eq) goto loc_821EB3F0;
loc_821EB3C4:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eb5ac
  if (cr6.eq) goto loc_821EB5AC;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821eb5ac
  if (cr0.eq) goto loc_821EB5AC;
loc_821EB3F0:
  // lwz r11,9652(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9652);
  // cmpwi cr6,r11,22
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 22, so);
  // beq cr6,0x821eb5ac
  if (cr6.eq) goto loc_821EB5AC;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821EB400:
  // addi r4,r30,309
  r4 = uint64_t(int64_t(int64_t(r30) + 309));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,2
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 2, so);
  // blt cr6,0x821eb400
  if (cr6.lt) goto loc_821EB400;
  // b 0x821eb5ac
  goto loc_821EB5AC;
loc_821EB41C:
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwa r11,30552(r29)
  r11 = uint64_t(int64_t(int32_t(REX_LOAD_U32(uint32_t(r29) + 30552))));
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // frsp f13,f0
  f13 = double(float(f0));
  // lfs f0,-18996(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + -18996);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmuls f13,f13,f0
  f13 = double(float(f13 * f0));
  // lfs f0,8700(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 8700);
  f0 = double(temp.f32);
  // fmuls f0,f13,f0
  f0 = double(float(f13 * f0));
  // stfs f0,12(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 12, temp.u32);
  // lwz r11,30544(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30544);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x821eb5ac
  if (!cr6.eq) goto loc_821EB5AC;
  // li r5,14
  r5 = uint64_t(int64_t(14));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30548(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30548);
  // mr r4,r3
  r4 = r3;
  // li r5,450
  r5 = uint64_t(int64_t(450));
  // mr r3,r31
  r3 = r31;
  // addi r6,r11,1
  r6 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,17
  r5 = uint64_t(int64_t(17));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30568(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30568);
  // mr r4,r3
  r4 = r3;
  // li r5,450
  r5 = uint64_t(int64_t(450));
  // mr r3,r31
  r3 = r31;
  // addi r6,r11,1
  r6 = uint64_t(int64_t(int64_t(r11) + 1));
  // bl 0x8211c7e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211C7E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eb4ec
  if (cr6.eq) goto loc_821EB4EC;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eb584
  if (!cr0.eq) goto loc_821EB584;
loc_821EB4EC:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eb518
  if (cr6.eq) goto loc_821EB518;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eb584
  if (!cr0.eq) goto loc_821EB584;
loc_821EB518:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eb544
  if (cr6.eq) goto loc_821EB544;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821eb570
  if (!cr0.eq) goto loc_821EB570;
loc_821EB544:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eb5ac
  if (cr6.eq) goto loc_821EB5AC;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821eb5ac
  if (cr0.eq) goto loc_821EB5AC;
loc_821EB570:
  // li r4,382
  r4 = uint64_t(int64_t(382));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,382
  r4 = uint64_t(int64_t(382));
  // b 0x821eb594
  goto loc_821EB594;
loc_821EB584:
  // li r4,383
  r4 = uint64_t(int64_t(383));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,383
  r4 = uint64_t(int64_t(383));
loc_821EB594:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821EB5AC:
  // lwz r11,9708(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9708);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821eb8d0
  if (!cr6.eq) goto loc_821EB8D0;
  // lwz r11,9652(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9652);
  // cmpwi cr6,r11,22
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 22, so);
  // bne cr6,0x821eb8d0
  if (!cr6.eq) goto loc_821EB8D0;
  // lwz r11,30564(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30564);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821eb60c
  if (cr6.lt) goto loc_821EB60C;
  // beq cr6,0x821eb604
  if (cr6.eq) goto loc_821EB604;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821eb5ec
  if (cr6.lt) goto loc_821EB5EC;
  // bne cr6,0x821eb618
  if (!cr6.eq) goto loc_821EB618;
  // lwz r11,30576(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30576);
  // addi r4,r11,309
  r4 = uint64_t(int64_t(int64_t(r11) + 309));
  // b 0x821eb610
  goto loc_821EB610;
loc_821EB5EC:
  // li r4,386
  r4 = uint64_t(int64_t(386));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30572(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30572);
  // addi r4,r11,366
  r4 = uint64_t(int64_t(int64_t(r11) + 366));
  // b 0x821eb610
  goto loc_821EB610;
loc_821EB604:
  // li r4,385
  r4 = uint64_t(int64_t(385));
  // b 0x821eb610
  goto loc_821EB610;
loc_821EB60C:
  // li r4,384
  r4 = uint64_t(int64_t(384));
loc_821EB610:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821EB618:
  // lwz r11,30544(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30544);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821eb874
  if (cr6.lt) goto loc_821EB874;
  // beq cr6,0x821eb81c
  if (cr6.eq) goto loc_821EB81C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821eb784
  if (cr6.lt) goto loc_821EB784;
  // bne cr6,0x821eb8d0
  if (!cr6.eq) goto loc_821EB8D0;
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r5,8
  r5 = uint64_t(int64_t(8));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r5,15
  r5 = uint64_t(int64_t(15));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // lfs f31,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // stfs f31,68(r10)
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,18
  r5 = uint64_t(int64_t(18));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 68, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,5
  r5 = uint64_t(int64_t(5));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 68, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 68, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,4
  r4 = uint64_t(int64_t(4));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // li r5,4
  r5 = uint64_t(int64_t(4));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 68, temp.u32);
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // mr r30,r3
  r30 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r3,r30
  r3 = r30;
  // stfs f31,68(r11)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  REX_STORE_U32(uint32_t(r11) + 68, temp.u32);
  // lwz r11,30556(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30556);
  // addi r4,r11,4
  r4 = uint64_t(int64_t(int64_t(r11) + 4));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,3
  r4 = uint64_t(int64_t(3));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // mr r3,r31
  r3 = r31;
  // lfs f0,10812(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10812);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // addi r4,r11,309
  r4 = uint64_t(int64_t(int64_t(r11) + 309));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,309
  r4 = uint64_t(int64_t(int64_t(r11) + 309));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30560(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30560);
  // addi r4,r11,309
  r4 = uint64_t(int64_t(int64_t(r11) + 309));
  // b 0x821eb8c8
  goto loc_821EB8C8;
loc_821EB784:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,386
  r4 = uint64_t(int64_t(386));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,386
  r4 = uint64_t(int64_t(386));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,386
  r4 = uint64_t(int64_t(386));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30556(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30556);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,366
  r4 = uint64_t(int64_t(int64_t(r11) + 366));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30556(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30556);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,366
  r4 = uint64_t(int64_t(int64_t(r11) + 366));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30556(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 30556);
  // addi r4,r11,366
  r4 = uint64_t(int64_t(int64_t(r11) + 366));
  // b 0x821eb8c8
  goto loc_821EB8C8;
loc_821EB81C:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,385
  r4 = uint64_t(int64_t(385));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,385
  r4 = uint64_t(int64_t(385));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,385
  r4 = uint64_t(int64_t(385));
  // b 0x821eb8c8
  goto loc_821EB8C8;
loc_821EB874:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,24
  r4 = uint64_t(int64_t(24));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,384
  r4 = uint64_t(int64_t(384));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,68(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 68, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,384
  r4 = uint64_t(int64_t(384));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,384
  r4 = uint64_t(int64_t(384));
loc_821EB8C8:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821EB8D0:
  // mr r3,r29
  r3 = r29;
  // bl 0x821eae68
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821EAE68, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // lfd f31,-40(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -40)));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821EBA00: lifted from generated/ by lift.py.
lifted::Result f821EBA00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821EBA00 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eba70
  if (cr6.eq) goto loc_821EBA70;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ebcbc
  if (!cr0.eq) goto loc_821EBCBC;
loc_821EBA70:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eba9c
  if (cr6.eq) goto loc_821EBA9C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ebcbc
  if (!cr0.eq) goto loc_821EBCBC;
loc_821EBA9C:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ebac8
  if (cr6.eq) goto loc_821EBAC8;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ebbfc
  if (!cr0.eq) goto loc_821EBBFC;
loc_821EBAC8:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ebaf4
  if (cr6.eq) goto loc_821EBAF4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ebbfc
  if (!cr0.eq) goto loc_821EBBFC;
loc_821EBAF4:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ebb64
  if (cr6.eq) goto loc_821EBB64;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ebb64
  if (cr0.eq) goto loc_821EBB64;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821EBB24:
  // addi r5,r30,2
  r5 = uint64_t(int64_t(int64_t(r30) + 2));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821ebb24
  if (cr6.lt) goto loc_821EBB24;
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ebdbc
  goto loc_821EBDBC;
loc_821EBB64:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ebdbc
  if (cr6.eq) goto loc_821EBDBC;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ebdbc
  if (cr0.eq) goto loc_821EBDBC;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821EBB94:
  // addi r5,r30,2
  r5 = uint64_t(int64_t(int64_t(r30) + 2));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821ebb94
  if (cr6.lt) goto loc_821EBB94;
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1004
  r4 = uint64_t(int64_t(1004));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1004
  r4 = uint64_t(int64_t(1004));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ebdbc
  goto loc_821EBDBC;
loc_821EBBFC:
  // lwz r11,9708(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ebdbc
  if (cr6.eq) goto loc_821EBDBC;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // ori r11,r11,33544
  r11 = r11 | 33544;
  // mr r3,r31
  r3 = r31;
  // lwzx r11,r29,r11
  r11 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ebc80
  if (cr6.eq) goto loc_821EBC80;
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // li r5,3
  r5 = uint64_t(int64_t(3));
  // addi r30,r30,-31996
  r30 = uint64_t(int64_t(int64_t(r30) + -31996));
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
loc_821EBC6C:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // mr r11,r3
  r11 = r3;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // b 0x821ebd64
  goto loc_821EBD64;
loc_821EBC80:
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // addi r30,r30,-31996
  r30 = uint64_t(int64_t(int64_t(r30) + -31996));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r5,r11,1
  r5 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x821ebd64
  goto loc_821EBD64;
loc_821EBCBC:
  // lwz r11,9708(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ebdbc
  if (cr6.eq) goto loc_821EBDBC;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // ori r11,r11,33544
  r11 = r11 | 33544;
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // lwzx r11,r29,r11
  r11 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ebd30
  if (cr6.eq) goto loc_821EBD30;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,13
  r5 = uint64_t(int64_t(13));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // li r5,2
  r5 = uint64_t(int64_t(2));
  // addi r30,r30,-31996
  r30 = uint64_t(int64_t(int64_t(r30) + -31996));
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // b 0x821ebc6c
  goto loc_821EBC6C;
loc_821EBD30:
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // li r5,12
  r5 = uint64_t(int64_t(12));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // addi r30,r30,-31996
  r30 = uint64_t(int64_t(int64_t(r30) + -31996));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r5,r11,3
  r5 = uint64_t(int64_t(int64_t(r11) + 3));
loc_821EBD64:
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // addi r5,r11,2
  r5 = uint64_t(int64_t(int64_t(r11) + 2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211de38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DE38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,1
  r6 = uint64_t(int64_t(1));
  // addi r5,r11,2
  r5 = uint64_t(int64_t(int64_t(r11) + 2));
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211d978
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211D978, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,27
  r4 = uint64_t(int64_t(27));
  // addi r5,r11,2
  r5 = uint64_t(int64_t(int64_t(r11) + 2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211ddd8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDD8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821EBDBC:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821EC038: lifted from generated/ by lift.py.
lifted::Result f821EC038(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821EC038 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r28,r3
  r28 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec0a8
  if (cr6.eq) goto loc_821EC0A8;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ec524
  if (!cr0.eq) goto loc_821EC524;
loc_821EC0A8:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec0d4
  if (cr6.eq) goto loc_821EC0D4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ec524
  if (!cr0.eq) goto loc_821EC524;
loc_821EC0D4:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec100
  if (cr6.eq) goto loc_821EC100;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ec324
  if (!cr0.eq) goto loc_821EC324;
loc_821EC100:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec12c
  if (cr6.eq) goto loc_821EC12C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ec324
  if (!cr0.eq) goto loc_821EC324;
loc_821EC12C:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec288
  if (cr6.eq) goto loc_821EC288;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ec288
  if (cr0.eq) goto loc_821EC288;
  // li r30,428
  r30 = uint64_t(int64_t(428));
  // li r29,10
  r29 = uint64_t(int64_t(10));
loc_821EC160:
  // addi r4,r30,61
  r4 = uint64_t(int64_t(int64_t(r30) + 61));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x821ec160
  if (!cr0.eq) goto loc_821EC160;
  // li r4,499
  r4 = uint64_t(int64_t(499));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,500
  r4 = uint64_t(int64_t(500));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r28,1
  r30 = uint64_t(int64_t(int64_t(r28) + 65536));
  // mr r3,r31
  r3 = r31;
  // addi r30,r30,-31988
  r30 = uint64_t(int64_t(int64_t(r30) + -31988));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821ec1c4
  if (!cr6.eq) goto loc_821EC1C4;
  // li r4,503
  r4 = uint64_t(int64_t(503));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,503
  r4 = uint64_t(int64_t(503));
  // b 0x821ec1d4
  goto loc_821EC1D4;
loc_821EC1C4:
  // addi r4,r11,501
  r4 = uint64_t(int64_t(int64_t(r11) + 501));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,501
  r4 = uint64_t(int64_t(int64_t(r11) + 501));
loc_821EC1D4:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821ec20c
  if (!cr6.eq) goto loc_821EC20C;
  // li r4,446
  r4 = uint64_t(int64_t(446));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,446
  r4 = uint64_t(int64_t(446));
  // b 0x821ec21c
  goto loc_821EC21C;
loc_821EC20C:
  // addi r4,r11,444
  r4 = uint64_t(int64_t(int64_t(r11) + 444));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,444
  r4 = uint64_t(int64_t(int64_t(r11) + 444));
loc_821EC21C:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x821cd868
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CD868, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x821ec278
  if (!cr6.eq) goto loc_821EC278;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821ec264
  if (!cr6.eq) goto loc_821EC264;
  // li r4,457
  r4 = uint64_t(int64_t(457));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,457
  r4 = uint64_t(int64_t(457));
  // b 0x821ec308
  goto loc_821EC308;
loc_821EC264:
  // addi r4,r11,455
  r4 = uint64_t(int64_t(int64_t(r11) + 455));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,455
  r4 = uint64_t(int64_t(int64_t(r11) + 455));
  // b 0x821ec308
  goto loc_821EC308;
loc_821EC278:
  // li r4,465
  r4 = uint64_t(int64_t(465));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,465
  r4 = uint64_t(int64_t(465));
  // b 0x821ec308
  goto loc_821EC308;
loc_821EC288:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec694
  if (cr6.eq) goto loc_821EC694;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ec694
  if (cr0.eq) goto loc_821EC694;
  // li r30,428
  r30 = uint64_t(int64_t(428));
  // li r29,10
  r29 = uint64_t(int64_t(10));
loc_821EC2BC:
  // addi r4,r30,61
  r4 = uint64_t(int64_t(int64_t(r30) + 61));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x821ec2bc
  if (!cr0.eq) goto loc_821EC2BC;
  // li r4,499
  r4 = uint64_t(int64_t(499));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,500
  r4 = uint64_t(int64_t(500));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1003
  r4 = uint64_t(int64_t(1003));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1003
  r4 = uint64_t(int64_t(1003));
loc_821EC308:
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ec694
  goto loc_821EC694;
loc_821EC324:
  // lwz r11,9708(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ec694
  if (cr6.eq) goto loc_821EC694;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // ori r11,r11,33552
  r11 = r11 | 33552;
  // lwzx r11,r28,r11
  r11 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ec3f4
  if (cr6.eq) goto loc_821EC3F4;
  // li r4,499
  r4 = uint64_t(int64_t(499));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,500
  r4 = uint64_t(int64_t(500));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r28,1
  r30 = uint64_t(int64_t(int64_t(r28) + 65536));
  // mr r11,r3
  r11 = r3;
  // addi r30,r30,-31988
  r30 = uint64_t(int64_t(int64_t(r30) + -31988));
  // li r4,491
  r4 = uint64_t(int64_t(491));
  // mr r3,r31
  r3 = r31;
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,489
  r4 = uint64_t(int64_t(int64_t(r11) + 489));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,489
  r4 = uint64_t(int64_t(int64_t(r11) + 489));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,489
  r4 = uint64_t(int64_t(int64_t(r11) + 489));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,430
  r4 = uint64_t(int64_t(430));
  // b 0x821ec64c
  goto loc_821EC64C;
loc_821EC3F4:
  // addis r30,r28,1
  r30 = uint64_t(int64_t(int64_t(r28) + 65536));
  // li r4,500
  r4 = uint64_t(int64_t(500));
  // addi r30,r30,-31988
  r30 = uint64_t(int64_t(int64_t(r30) + -31988));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821ec4a8
  if (!cr6.eq) goto loc_821EC4A8;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,500
  r4 = uint64_t(int64_t(500));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,490
  r4 = uint64_t(int64_t(490));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,491
  r4 = uint64_t(int64_t(491));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,491
  r4 = uint64_t(int64_t(491));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,491
  r4 = uint64_t(int64_t(491));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,429
  r4 = uint64_t(int64_t(429));
loc_821EC470:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,430
  r4 = uint64_t(int64_t(430));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,430
  r4 = uint64_t(int64_t(430));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,430
  r4 = uint64_t(int64_t(430));
  // b 0x821ec68c
  goto loc_821EC68C;
loc_821EC4A8:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,500
  r4 = uint64_t(int64_t(500));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,488
  r4 = uint64_t(int64_t(int64_t(r11) + 488));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,489
  r4 = uint64_t(int64_t(int64_t(r11) + 489));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,489
  r4 = uint64_t(int64_t(int64_t(r11) + 489));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,489
  r4 = uint64_t(int64_t(int64_t(r11) + 489));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,427
  r4 = uint64_t(int64_t(int64_t(r11) + 427));
  // b 0x821ec64c
  goto loc_821EC64C;
loc_821EC524:
  // lwz r11,9708(r28)
  r11 = REX_LOAD_U32(uint32_t(r28) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ec694
  if (cr6.eq) goto loc_821EC694;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r4,499
  r4 = uint64_t(int64_t(499));
  // ori r11,r11,33552
  r11 = r11 | 33552;
  // mr r3,r31
  r3 = r31;
  // lwzx r11,r28,r11
  r11 = REX_LOAD_U32(uint32_t(r28) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ec5cc
  if (cr6.eq) goto loc_821EC5CC;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,500
  r4 = uint64_t(int64_t(500));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,28
  r4 = uint64_t(int64_t(28));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,489
  r4 = uint64_t(int64_t(489));
  // mr r3,r31
  r3 = r31;
  // lfs f0,7820(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 7820);
  f0 = double(temp.f32);
  // stfs f0,16(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 16, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,491
  r4 = uint64_t(int64_t(491));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,491
  r4 = uint64_t(int64_t(491));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,491
  r4 = uint64_t(int64_t(491));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,428
  r4 = uint64_t(int64_t(428));
  // b 0x821ec470
  goto loc_821EC470;
loc_821EC5CC:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,499
  r4 = uint64_t(int64_t(499));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r28,1
  r30 = uint64_t(int64_t(int64_t(r28) + 65536));
  // mr r3,r31
  r3 = r31;
  // addi r30,r30,-31988
  r30 = uint64_t(int64_t(int64_t(r30) + -31988));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,490
  r4 = uint64_t(int64_t(int64_t(r11) + 490));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,489
  r4 = uint64_t(int64_t(int64_t(r11) + 489));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,489
  r4 = uint64_t(int64_t(int64_t(r11) + 489));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,489
  r4 = uint64_t(int64_t(int64_t(r11) + 489));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,429
  r4 = uint64_t(int64_t(int64_t(r11) + 429));
loc_821EC64C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,428
  r4 = uint64_t(int64_t(int64_t(r11) + 428));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,428
  r4 = uint64_t(int64_t(int64_t(r11) + 428));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,428
  r4 = uint64_t(int64_t(int64_t(r11) + 428));
loc_821EC68C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821EC694:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6ed8
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821EC7E0: lifted from generated/ by lift.py.
lifted::Result f821EC7E0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821EC7E0 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec850
  if (cr6.eq) goto loc_821EC850;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ecb78
  if (!cr0.eq) goto loc_821ECB78;
loc_821EC850:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec87c
  if (cr6.eq) goto loc_821EC87C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ecb78
  if (!cr0.eq) goto loc_821ECB78;
loc_821EC87C:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec8a8
  if (cr6.eq) goto loc_821EC8A8;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ecac4
  if (!cr0.eq) goto loc_821ECAC4;
loc_821EC8A8:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec8d4
  if (cr6.eq) goto loc_821EC8D4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ecac4
  if (!cr0.eq) goto loc_821ECAC4;
loc_821EC8D4:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ec938
  if (cr6.eq) goto loc_821EC938;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ec938
  if (cr0.eq) goto loc_821EC938;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821EC904:
  // addi r4,r30,771
  r4 = uint64_t(int64_t(int64_t(r30) + 771));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821ec904
  if (cr6.lt) goto loc_821EC904;
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ecc5c
  goto loc_821ECC5C;
loc_821EC938:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ecc5c
  if (cr6.eq) goto loc_821ECC5C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ecc5c
  if (cr0.eq) goto loc_821ECC5C;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821EC968:
  // addi r4,r30,771
  r4 = uint64_t(int64_t(int64_t(r30) + 771));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821ec968
  if (cr6.lt) goto loc_821EC968;
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // mr r3,r31
  r3 = r31;
  // addi r30,r30,-31988
  r30 = uint64_t(int64_t(int64_t(r30) + -31988));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821ec9c0
  if (!cr6.eq) goto loc_821EC9C0;
  // li r4,517
  r4 = uint64_t(int64_t(517));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,517
  r4 = uint64_t(int64_t(517));
  // b 0x821ec9d0
  goto loc_821EC9D0;
loc_821EC9C0:
  // addi r4,r11,515
  r4 = uint64_t(int64_t(int64_t(r11) + 515));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,515
  r4 = uint64_t(int64_t(int64_t(r11) + 515));
loc_821EC9D0:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821eca08
  if (!cr6.eq) goto loc_821ECA08;
  // li r4,468
  r4 = uint64_t(int64_t(468));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,468
  r4 = uint64_t(int64_t(468));
  // b 0x821eca18
  goto loc_821ECA18;
loc_821ECA08:
  // addi r4,r11,466
  r4 = uint64_t(int64_t(int64_t(r11) + 466));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,466
  r4 = uint64_t(int64_t(int64_t(r11) + 466));
loc_821ECA18:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r29
  r3 = r29;
  // bl 0x821cd868
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CD868, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpw cr6,r3,r11
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r11), so);
  // mr r3,r31
  r3 = r31;
  // bne cr6,0x821eca74
  if (!cr6.eq) goto loc_821ECA74;
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821eca60
  if (!cr6.eq) goto loc_821ECA60;
  // li r4,478
  r4 = uint64_t(int64_t(478));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,478
  r4 = uint64_t(int64_t(478));
  // b 0x821eca80
  goto loc_821ECA80;
loc_821ECA60:
  // addi r4,r11,476
  r4 = uint64_t(int64_t(int64_t(r11) + 476));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,476
  r4 = uint64_t(int64_t(int64_t(r11) + 476));
  // b 0x821eca80
  goto loc_821ECA80;
loc_821ECA74:
  // li r4,486
  r4 = uint64_t(int64_t(486));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,486
  r4 = uint64_t(int64_t(486));
loc_821ECA80:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1000
  r4 = uint64_t(int64_t(1000));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1000
  r4 = uint64_t(int64_t(1000));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ecc5c
  goto loc_821ECC5C;
loc_821ECAC4:
  // lwz r11,9708(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ecc5c
  if (cr6.eq) goto loc_821ECC5C;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // ori r11,r11,33560
  r11 = r11 | 33560;
  // lwzx r11,r29,r11
  r11 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ecb40
  if (cr6.eq) goto loc_821ECB40;
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // li r4,776
  r4 = uint64_t(int64_t(776));
  // addi r30,r30,-31980
  r30 = uint64_t(int64_t(int64_t(r30) + -31980));
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
loc_821ECB2C:
  // fcfid f0,f0
  lifted::ScalarFloatMode();
  f0 = double(std::bit_cast<int64_t>(f0));
  // mr r11,r3
  r11 = r3;
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // b 0x821ecc14
  goto loc_821ECC14;
loc_821ECB40:
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // addi r30,r30,-31980
  r30 = uint64_t(int64_t(int64_t(r30) + -31980));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,770
  r4 = uint64_t(int64_t(int64_t(r11) + 770));
  // b 0x821ecc14
  goto loc_821ECC14;
loc_821ECB78:
  // lwz r11,9708(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ecc5c
  if (cr6.eq) goto loc_821ECC5C;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // ori r11,r11,33560
  r11 = r11 | 33560;
  // mr r3,r31
  r3 = r31;
  // lwzx r11,r29,r11
  r11 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ecbe4
  if (cr6.eq) goto loc_821ECBE4;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,780
  r4 = uint64_t(int64_t(780));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,32
  r4 = uint64_t(int64_t(32));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // li r4,771
  r4 = uint64_t(int64_t(771));
  // addi r30,r30,-31980
  r30 = uint64_t(int64_t(int64_t(r30) + -31980));
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // b 0x821ecb2c
  goto loc_821ECB2C;
loc_821ECBE4:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,779
  r4 = uint64_t(int64_t(779));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // addi r30,r30,-31980
  r30 = uint64_t(int64_t(int64_t(r30) + -31980));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,772
  r4 = uint64_t(int64_t(int64_t(r11) + 772));
loc_821ECC14:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,771
  r4 = uint64_t(int64_t(int64_t(r11) + 771));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,771
  r4 = uint64_t(int64_t(int64_t(r11) + 771));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,771
  r4 = uint64_t(int64_t(int64_t(r11) + 771));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821ECC5C:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821ED108: lifted from generated/ by lift.py.
lifted::Result f821ED108(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821ED108 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r26,r3
  r26 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ed178
  if (cr6.eq) goto loc_821ED178;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ed8fc
  if (!cr0.eq) goto loc_821ED8FC;
loc_821ED178:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ed1a4
  if (cr6.eq) goto loc_821ED1A4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ed8fc
  if (!cr0.eq) goto loc_821ED8FC;
loc_821ED1A4:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ed1d0
  if (cr6.eq) goto loc_821ED1D0;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ed6bc
  if (!cr0.eq) goto loc_821ED6BC;
loc_821ED1D0:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ed1fc
  if (cr6.eq) goto loc_821ED1FC;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ed6bc
  if (!cr0.eq) goto loc_821ED6BC;
loc_821ED1FC:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ed228
  if (cr6.eq) goto loc_821ED228;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ed614
  if (!cr0.eq) goto loc_821ED614;
loc_821ED228:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ed254
  if (cr6.eq) goto loc_821ED254;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ed614
  if (!cr0.eq) goto loc_821ED614;
loc_821ED254:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ed280
  if (cr6.eq) goto loc_821ED280;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ed468
  if (!cr0.eq) goto loc_821ED468;
loc_821ED280:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ed2ac
  if (cr6.eq) goto loc_821ED2AC;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ed468
  if (!cr0.eq) goto loc_821ED468;
loc_821ED2AC:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ed380
  if (cr6.eq) goto loc_821ED380;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ed380
  if (cr0.eq) goto loc_821ED380;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821ED2DC:
  // addi r4,r30,74
  r4 = uint64_t(int64_t(int64_t(r30) + 74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821ed2dc
  if (cr6.lt) goto loc_821ED2DC;
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r11,r11,33572
  r11 = r11 | 33572;
  // lwzx r11,r26,r11
  r11 = REX_LOAD_U32(uint32_t(r26) + uint32_t(r11));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821ed370
  if (cr6.lt) goto loc_821ED370;
  // beq cr6,0x821ed368
  if (cr6.eq) goto loc_821ED368;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821ed360
  if (cr6.lt) goto loc_821ED360;
  // beq cr6,0x821ed358
  if (cr6.eq) goto loc_821ED358;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821edb68
  if (!cr6.lt) goto loc_821EDB68;
  // li r4,136
  r4 = uint64_t(int64_t(136));
  // b 0x821ed374
  goto loc_821ED374;
loc_821ED358:
  // li r4,131
  r4 = uint64_t(int64_t(131));
  // b 0x821ed374
  goto loc_821ED374;
loc_821ED360:
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // b 0x821ed374
  goto loc_821ED374;
loc_821ED368:
  // li r4,121
  r4 = uint64_t(int64_t(121));
  // b 0x821ed374
  goto loc_821ED374;
loc_821ED370:
  // li r4,116
  r4 = uint64_t(int64_t(116));
loc_821ED374:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821edb68
  goto loc_821EDB68;
loc_821ED380:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821edb68
  if (cr6.eq) goto loc_821EDB68;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821edb68
  if (cr0.eq) goto loc_821EDB68;
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821ED3B0:
  // addi r4,r30,74
  r4 = uint64_t(int64_t(int64_t(r30) + 74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,10
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 10, so);
  // blt cr6,0x821ed3b0
  if (cr6.lt) goto loc_821ED3B0;
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r11,r11,33572
  r11 = r11 | 33572;
  // lwzx r11,r26,r11
  r11 = REX_LOAD_U32(uint32_t(r26) + uint32_t(r11));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821ed444
  if (cr6.lt) goto loc_821ED444;
  // beq cr6,0x821ed43c
  if (cr6.eq) goto loc_821ED43C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821ed434
  if (cr6.lt) goto loc_821ED434;
  // beq cr6,0x821ed42c
  if (cr6.eq) goto loc_821ED42C;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821ed450
  if (!cr6.lt) goto loc_821ED450;
  // li r4,136
  r4 = uint64_t(int64_t(136));
  // b 0x821ed448
  goto loc_821ED448;
loc_821ED42C:
  // li r4,131
  r4 = uint64_t(int64_t(131));
  // b 0x821ed448
  goto loc_821ED448;
loc_821ED434:
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // b 0x821ed448
  goto loc_821ED448;
loc_821ED43C:
  // li r4,121
  r4 = uint64_t(int64_t(121));
  // b 0x821ed448
  goto loc_821ED448;
loc_821ED444:
  // li r4,116
  r4 = uint64_t(int64_t(116));
loc_821ED448:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821ED450:
  // li r4,1006
  r4 = uint64_t(int64_t(1006));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1006
  r4 = uint64_t(int64_t(1006));
  // b 0x821edb54
  goto loc_821EDB54;
loc_821ED468:
  // lwz r11,9708(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821edb68
  if (cr6.eq) goto loc_821EDB68;
  // addis r30,r26,1
  r30 = uint64_t(int64_t(int64_t(r26) + 65536));
  // mr r3,r31
  r3 = r31;
  // addi r30,r30,-31964
  r30 = uint64_t(int64_t(int64_t(r30) + -31964));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ed49c
  if (!cr0.eq) goto loc_821ED49C;
  // li r4,150
  r4 = uint64_t(int64_t(150));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,150
  r4 = uint64_t(int64_t(150));
  // b 0x821ed4ac
  goto loc_821ED4AC;
loc_821ED49C:
  // addi r4,r11,145
  r4 = uint64_t(int64_t(int64_t(r11) + 145));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,145
  r4 = uint64_t(int64_t(int64_t(r11) + 145));
loc_821ED4AC:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821ed5d8
  if (cr6.lt) goto loc_821ED5D8;
  // beq cr6,0x821ed59c
  if (cr6.eq) goto loc_821ED59C;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821ed560
  if (cr6.lt) goto loc_821ED560;
  // beq cr6,0x821ed524
  if (cr6.eq) goto loc_821ED524;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821edb68
  if (!cr6.lt) goto loc_821EDB68;
  // li r4,131
  r4 = uint64_t(int64_t(131));
loc_821ED4EC:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,136
  r4 = uint64_t(int64_t(136));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,136
  r4 = uint64_t(int64_t(136));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,136
  r4 = uint64_t(int64_t(136));
  // b 0x821ed9ac
  goto loc_821ED9AC;
loc_821ED524:
  // li r4,126
  r4 = uint64_t(int64_t(126));
loc_821ED528:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,131
  r4 = uint64_t(int64_t(131));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,131
  r4 = uint64_t(int64_t(131));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,131
  r4 = uint64_t(int64_t(131));
  // b 0x821ed9ac
  goto loc_821ED9AC;
loc_821ED560:
  // li r4,121
  r4 = uint64_t(int64_t(121));
loc_821ED564:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // b 0x821ed9ac
  goto loc_821ED9AC;
loc_821ED59C:
  // li r4,116
  r4 = uint64_t(int64_t(116));
loc_821ED5A0:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,121
  r4 = uint64_t(int64_t(121));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,121
  r4 = uint64_t(int64_t(121));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,121
  r4 = uint64_t(int64_t(121));
  // b 0x821ed9ac
  goto loc_821ED9AC;
loc_821ED5D8:
  // li r4,136
  r4 = uint64_t(int64_t(136));
loc_821ED5DC:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,116
  r4 = uint64_t(int64_t(116));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,116
  r4 = uint64_t(int64_t(116));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,116
  r4 = uint64_t(int64_t(116));
  // b 0x821ed9ac
  goto loc_821ED9AC;
loc_821ED614:
  // lwz r11,9708(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821edb68
  if (cr6.eq) goto loc_821EDB68;
  // addis r30,r26,1
  r30 = uint64_t(int64_t(int64_t(r26) + 65536));
  // mr r3,r31
  r3 = r31;
  // addi r30,r30,-31964
  r30 = uint64_t(int64_t(int64_t(r30) + -31964));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821ed648
  if (!cr6.eq) goto loc_821ED648;
  // li r4,151
  r4 = uint64_t(int64_t(151));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,151
  r4 = uint64_t(int64_t(151));
  // b 0x821ed658
  goto loc_821ED658;
loc_821ED648:
  // subfic r4,r11,155
  ca = uint32_t(r11) <= 155;
  r4 = static_cast<uint64_t>(155) - r11;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // subfic r4,r11,155
  ca = uint32_t(r11) <= 155;
  r4 = static_cast<uint64_t>(155) - r11;
loc_821ED658:
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
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // blt cr6,0x821ed6b4
  if (cr6.lt) goto loc_821ED6B4;
  // beq cr6,0x821ed6ac
  if (cr6.eq) goto loc_821ED6AC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821ed6a4
  if (cr6.lt) goto loc_821ED6A4;
  // beq cr6,0x821ed69c
  if (cr6.eq) goto loc_821ED69C;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821edb68
  if (!cr6.lt) goto loc_821EDB68;
  // li r4,116
  r4 = uint64_t(int64_t(116));
  // b 0x821ed4ec
  goto loc_821ED4EC;
loc_821ED69C:
  // li r4,136
  r4 = uint64_t(int64_t(136));
  // b 0x821ed528
  goto loc_821ED528;
loc_821ED6A4:
  // li r4,131
  r4 = uint64_t(int64_t(131));
  // b 0x821ed564
  goto loc_821ED564;
loc_821ED6AC:
  // li r4,126
  r4 = uint64_t(int64_t(126));
  // b 0x821ed5a0
  goto loc_821ED5A0;
loc_821ED6B4:
  // li r4,121
  r4 = uint64_t(int64_t(121));
  // b 0x821ed5dc
  goto loc_821ED5DC;
loc_821ED6BC:
  // lwz r11,9708(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821edb68
  if (cr6.eq) goto loc_821EDB68;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // ori r11,r11,33568
  r11 = r11 | 33568;
  // lwzx r11,r26,r11
  r11 = REX_LOAD_U32(uint32_t(r26) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ed7ec
  if (cr6.eq) goto loc_821ED7EC;
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,79
  r4 = uint64_t(int64_t(79));
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // stfs f0,16(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 16, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,74
  r4 = uint64_t(int64_t(74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,74
  r4 = uint64_t(int64_t(74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,74
  r4 = uint64_t(int64_t(74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r27,r26,1
  r27 = uint64_t(int64_t(int64_t(r26) + 65536));
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r27,r27,-31972
  r27 = uint64_t(int64_t(int64_t(r27) + -31972));
loc_821ED780:
  // addi r5,r30,9
  r5 = uint64_t(int64_t(int64_t(r30) + 9));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r29,r3
  r29 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r28,r3
  r28 = r3;
  // mr r4,r30
  r4 = r30;
  // mr r3,r26
  r3 = r26;
  // bl 0x821cdc38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CDC38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // bne 0x821ed7bc
  if (!cr0.eq) goto loc_821ED7BC;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821ED7BC:
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // stw r11,80(r28)
  REX_STORE_U32(uint32_t(r28) + 80, uint32_t(r11));
  // mr r3,r29
  r3 = r29;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r27)
  r11 = REX_LOAD_U32(uint32_t(r27) + 0);
  // mulli r11,r11,5
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(5))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,5
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 5, so);
  // stw r11,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r11));
  // blt cr6,0x821ed780
  if (cr6.lt) goto loc_821ED780;
  // b 0x821edb68
  goto loc_821EDB68;
loc_821ED7EC:
  // addis r30,r26,1
  r30 = uint64_t(int64_t(int64_t(r26) + 65536));
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // addi r30,r30,-31972
  r30 = uint64_t(int64_t(int64_t(r30) + -31972));
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821ed894
  if (!cr6.eq) goto loc_821ED894;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,77
  r4 = uint64_t(int64_t(77));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,79
  r4 = uint64_t(int64_t(79));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,79
  r4 = uint64_t(int64_t(79));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,79
  r4 = uint64_t(int64_t(79));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,145
  r4 = uint64_t(int64_t(145));
  // b 0x821ed374
  goto loc_821ED374;
loc_821ED894:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,73
  r4 = uint64_t(int64_t(int64_t(r11) + 73));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // b 0x821edb34
  goto loc_821EDB34;
loc_821ED8FC:
  // lwz r11,9708(r26)
  r11 = REX_LOAD_U32(uint32_t(r26) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821edb68
  if (cr6.eq) goto loc_821EDB68;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // ori r11,r11,33568
  r11 = r11 | 33568;
  // mr r3,r31
  r3 = r31;
  // lwzx r11,r26,r11
  r11 = REX_LOAD_U32(uint32_t(r26) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ed9b8
  if (cr6.eq) goto loc_821ED9B8;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r10,r3
  r10 = r3;
  // li r4,74
  r4 = uint64_t(int64_t(74));
  // mr r3,r31
  r3 = r31;
  // lfs f0,10148(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10148);
  f0 = double(temp.f32);
  // stfs f0,16(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 16, temp.u32);
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,79
  r4 = uint64_t(int64_t(79));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,79
  r4 = uint64_t(int64_t(79));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,79
  r4 = uint64_t(int64_t(79));
loc_821ED9AC:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821edb68
  goto loc_821EDB68;
loc_821ED9B8:
  // addis r29,r26,1
  r29 = uint64_t(int64_t(int64_t(r26) + 65536));
  // addi r29,r29,-31972
  r29 = uint64_t(int64_t(int64_t(r29) + -31972));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821edad0
  if (!cr6.eq) goto loc_821EDAD0;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,79
  r4 = uint64_t(int64_t(79));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,77
  r4 = uint64_t(int64_t(77));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,77
  r4 = uint64_t(int64_t(77));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,77
  r4 = uint64_t(int64_t(77));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,145
  r4 = uint64_t(int64_t(145));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r30,0
  r30 = uint64_t(int64_t(0));
loc_821EDA64:
  // addi r5,r30,9
  r5 = uint64_t(int64_t(int64_t(r30) + 9));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // mr r28,r3
  r28 = r3;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r27,r3
  r27 = r3;
  // mr r4,r30
  r4 = r30;
  // mr r3,r26
  r3 = r26;
  // bl 0x821cdc38
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CDC38, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // bne 0x821edaa0
  if (!cr0.eq) goto loc_821EDAA0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821EDAA0:
  // li r4,6
  r4 = uint64_t(int64_t(6));
  // stw r11,80(r27)
  REX_STORE_U32(uint32_t(r27) + 80, uint32_t(r11));
  // mr r3,r28
  r3 = r28;
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mulli r11,r11,5
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(5))));
  // add r11,r11,r30
  r11 = r11 + r30;
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // cmpwi cr6,r30,5
  cr6 = lifted::Compare<int32_t>(int32_t(r30), 5, so);
  // stw r11,108(r3)
  REX_STORE_U32(uint32_t(r3) + 108, uint32_t(r11));
  // blt cr6,0x821eda64
  if (cr6.lt) goto loc_821EDA64;
  // b 0x821edb68
  goto loc_821EDB68;
loc_821EDAD0:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,75
  r4 = uint64_t(int64_t(int64_t(r11) + 75));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
loc_821EDB34:
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,145
  r4 = uint64_t(int64_t(145));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,145
  r4 = uint64_t(int64_t(145));
  // li r5,1
  r5 = uint64_t(int64_t(1));
loc_821EDB54:
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821EDB68:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821EDCA8: lifted from generated/ by lift.py.
lifted::Result f821EDCA8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821EDCA8 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
  lifted::Temp temp{};
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // mr r29,r3
  r29 = r3;
  // addi r3,r10,23756
  r3 = uint64_t(int64_t(int64_t(r10) + 23756));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // li r11,2
  r11 = uint64_t(int64_t(2));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8211b3f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211B3F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r31,0(r3)
  r31 = REX_LOAD_U32(uint32_t(r3) + 0);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821edd18
  if (cr6.eq) goto loc_821EDD18;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ee148
  if (!cr0.eq) goto loc_821EE148;
loc_821EDD18:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821edd44
  if (cr6.eq) goto loc_821EDD44;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ee148
  if (!cr0.eq) goto loc_821EE148;
loc_821EDD44:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821edd70
  if (cr6.eq) goto loc_821EDD70;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821edf4c
  if (!cr0.eq) goto loc_821EDF4C;
loc_821EDD70:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821edd9c
  if (cr6.eq) goto loc_821EDD9C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821edf4c
  if (!cr0.eq) goto loc_821EDF4C;
loc_821EDD9C:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ede88
  if (cr6.eq) goto loc_821EDE88;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ede88
  if (cr0.eq) goto loc_821EDE88;
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // mr r3,r29
  r3 = r29;
  // addi r30,r30,-31960
  r30 = uint64_t(int64_t(int64_t(r30) + -31960));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ce188
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CE188, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // beq 0x821ee384
  if (cr0.eq) goto loc_821EE384;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // ori r11,r11,33564
  r11 = r11 | 33564;
  // lwzx r11,r29,r11
  r11 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // beq cr6,0x821ede28
  if (cr6.eq) goto loc_821EDE28;
  // li r30,111
  r30 = uint64_t(int64_t(111));
  // li r29,10
  r29 = uint64_t(int64_t(10));
loc_821EDE00:
  // addi r4,r30,-37
  r4 = uint64_t(int64_t(int64_t(r30) + -37));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x821ede00
  if (!cr0.eq) goto loc_821EDE00;
  // b 0x821edee0
  goto loc_821EDEE0;
loc_821EDE28:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r11,r11,8450
  r11 = uint64_t(int64_t(int64_t(r11) + 8450));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r29
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r29));
  // cmpwi cr6,r11,-1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), -1, so);
  // beq cr6,0x821ee384
  if (cr6.eq) goto loc_821EE384;
  // li r30,111
  r30 = uint64_t(int64_t(111));
  // li r29,10
  r29 = uint64_t(int64_t(10));
loc_821EDE48:
  // addi r4,r30,-37
  r4 = uint64_t(int64_t(int64_t(r30) + -37));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x821ede48
  if (!cr0.eq) goto loc_821EDE48;
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ee384
  goto loc_821EE384;
loc_821EDE88:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ee384
  if (cr6.eq) goto loc_821EE384;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ee384
  if (cr0.eq) goto loc_821EE384;
  // li r30,111
  r30 = uint64_t(int64_t(111));
  // li r29,10
  r29 = uint64_t(int64_t(10));
loc_821EDEBC:
  // addi r4,r30,-37
  r4 = uint64_t(int64_t(int64_t(r30) + -37));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addic. r29,r29,-1
  ca = uint32_t(r29) > 0;
  r29 = uint64_t(int64_t(int64_t(r29) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r29), 0, so);
  // addi r30,r30,1
  r30 = uint64_t(int64_t(int64_t(r30) + 1));
  // bne 0x821edebc
  if (!cr0.eq) goto loc_821EDEBC;
loc_821EDEE0:
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,144
  r4 = uint64_t(int64_t(144));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,144
  r4 = uint64_t(int64_t(144));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1011
  r4 = uint64_t(int64_t(1011));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,1011
  r4 = uint64_t(int64_t(1011));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ee384
  goto loc_821EE384;
loc_821EDF4C:
  // lwz r11,9708(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ee384
  if (cr6.eq) goto loc_821EE384;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // ori r11,r11,33580
  r11 = r11 | 33580;
  // lwzx r11,r29,r11
  r11 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ee06c
  if (cr6.eq) goto loc_821EE06C;
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // mr r11,r3
  r11 = r3;
  // addi r30,r30,-31960
  r30 = uint64_t(int64_t(int64_t(r30) + -31960));
  // mr r3,r29
  r3 = r29;
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ce188
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CE188, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r4,78
  r4 = uint64_t(int64_t(78));
  // mr r3,r31
  r3 = r31;
  // beq 0x821ee02c
  if (cr0.eq) goto loc_821EE02C;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ee064
  goto loc_821EE064;
loc_821EE02C:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,9
  r4 = uint64_t(int64_t(9));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10812(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10812);
  f0 = double(temp.f32);
  // stfs f0,68(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
loc_821EE064:
  // li r4,115
  r4 = uint64_t(int64_t(115));
  // b 0x821ee33c
  goto loc_821EE33C;
loc_821EE06C:
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // mr r3,r29
  r3 = r29;
  // addi r30,r30,-31960
  r30 = uint64_t(int64_t(int64_t(r30) + -31960));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ce188
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CE188, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,73
  r4 = uint64_t(int64_t(int64_t(r11) + 73));
  // beq 0x821ee100
  if (cr0.eq) goto loc_821EE100;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ee13c
  goto loc_821EE13C;
loc_821EE100:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,4
  r4 = uint64_t(int64_t(int64_t(r11) + 4));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10812(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10812);
  f0 = double(temp.f32);
  // stfs f0,68(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
loc_821EE13C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,110
  r4 = uint64_t(int64_t(int64_t(r11) + 110));
  // b 0x821ee33c
  goto loc_821EE33C;
loc_821EE148:
  // lwz r11,9708(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 9708);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ee384
  if (cr6.eq) goto loc_821EE384;
  // lis r11,0
  r11 = uint64_t(int64_t(0));
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // ori r11,r11,33580
  r11 = r11 | 33580;
  // mr r3,r31
  r3 = r31;
  // lwzx r11,r29,r11
  r11 = REX_LOAD_U32(uint32_t(r29) + uint32_t(r11));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ee268
  if (cr6.eq) goto loc_821EE268;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,85
  r4 = uint64_t(int64_t(85));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // mr r11,r3
  r11 = r3;
  // addi r30,r30,-31960
  r30 = uint64_t(int64_t(int64_t(r30) + -31960));
  // mr r3,r29
  r3 = r29;
  // lwz r10,0(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mulli r10,r10,40
  r10 = uint64_t(int64_t(static_cast<int64_t>(r10 * static_cast<uint64_t>(40))));
  // extsw r10,r10
  r10 = uint64_t(int64_t(int32_t(r10)));
  // std r10,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r10);
  // lfd f0,80(r1)
  lifted::ScalarFloatMode();
  f0 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ce188
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CE188, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // li r4,74
  r4 = uint64_t(int64_t(74));
  // mr r3,r31
  r3 = r31;
  // beq 0x821ee228
  if (cr0.eq) goto loc_821EE228;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ee260
  goto loc_821EE260;
loc_821EE228:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,5
  r4 = uint64_t(int64_t(5));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10812(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10812);
  f0 = double(temp.f32);
  // stfs f0,68(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
loc_821EE260:
  // li r4,111
  r4 = uint64_t(int64_t(111));
  // b 0x821ee33c
  goto loc_821EE33C;
loc_821EE268:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // li r4,84
  r4 = uint64_t(int64_t(84));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r30,r29,1
  r30 = uint64_t(int64_t(int64_t(r29) + 65536));
  // mr r3,r29
  r3 = r29;
  // addi r30,r30,-31960
  r30 = uint64_t(int64_t(int64_t(r30) + -31960));
  // lwz r4,0(r30)
  r4 = REX_LOAD_U32(uint32_t(r30) + 0);
  // bl 0x821ce188
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821CE188, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // cmpwi r3,0
  cr0 = lifted::Compare<int32_t>(int32_t(r3), 0, so);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,75
  r4 = uint64_t(int64_t(int64_t(r11) + 75));
  // beq 0x821ee2f8
  if (cr0.eq) goto loc_821EE2F8;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821ee334
  goto loc_821EE334;
loc_821EE2F8:
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,74
  r4 = uint64_t(int64_t(int64_t(r11) + 74));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r5,0
  r5 = uint64_t(int64_t(0));
  // li r4,2
  r4 = uint64_t(int64_t(2));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211a578
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211A578, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,6
  r4 = uint64_t(int64_t(int64_t(r11) + 6));
  // bl 0x8259d658
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8259D658, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,10812(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10812);
  f0 = double(temp.f32);
  // stfs f0,68(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 68, temp.u32);
loc_821EE334:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r11,112
  r4 = uint64_t(int64_t(int64_t(r11) + 112));
loc_821EE33C:
  // mr r3,r31
  r3 = r31;
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,111
  r4 = uint64_t(int64_t(int64_t(r11) + 111));
  // bl 0x8211df20
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DF20, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r6,0
  r6 = uint64_t(int64_t(0));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // addi r4,r11,111
  r4 = uint64_t(int64_t(int64_t(r11) + 111));
  // mr r3,r31
  r3 = r31;
  // bl 0x8211dac8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DAC8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r31
  r3 = r31;
  // addi r4,r11,111
  r4 = uint64_t(int64_t(int64_t(r11) + 111));
  // bl 0x8211dde0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x8211DDE0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821EE384:
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_821EF320: lifted from generated/ by lift.py.
lifted::Result f821EF320(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821EF320 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef37c
  if (cr6.eq) goto loc_821EF37C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ef594
  if (!cr0.eq) goto loc_821EF594;
loc_821EF37C:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef3a8
  if (cr6.eq) goto loc_821EF3A8;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ef594
  if (!cr0.eq) goto loc_821EF594;
loc_821EF3A8:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef3d4
  if (cr6.eq) goto loc_821EF3D4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ef564
  if (!cr0.eq) goto loc_821EF564;
loc_821EF3D4:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef400
  if (cr6.eq) goto loc_821EF400;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ef564
  if (!cr0.eq) goto loc_821EF564;
loc_821EF400:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef4b0
  if (cr6.eq) goto loc_821EF4B0;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ef4b0
  if (cr0.eq) goto loc_821EF4B0;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // li r8,19
  r8 = uint64_t(int64_t(19));
  // li r10,20
  r10 = uint64_t(int64_t(20));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // stw r11,9716(r31)
  REX_STORE_U32(uint32_t(r31) + 9716, uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10008);
  // stw r7,30500(r31)
  REX_STORE_U32(uint32_t(r31) + 30500, uint32_t(r7));
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r8,r11,r31
  REX_STORE_U32(uint32_t(r11) + uint32_t(r31), uint32_t(r8));
  // lbz r8,10008(r31)
  r8 = REX_LOAD_U8(uint32_t(r31) + 10008);
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // addi r8,r8,1
  r8 = uint64_t(int64_t(int64_t(r8) + 1));
  // stw r10,9652(r31)
  REX_STORE_U32(uint32_t(r31) + 9652, uint32_t(r10));
  // stb r8,10008(r31)
  REX_STORE_U8(uint32_t(r31) + 10008, uint8_t(r8));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // lwz r8,9652(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 9652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // beq cr6,0x821ef4a8
  if (cr6.eq) goto loc_821EF4A8;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821EF4A8:
  // li r10,12
  r10 = uint64_t(int64_t(12));
  // b 0x821ef550
  goto loc_821EF550;
loc_821EF4B0:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef5c4
  if (cr6.eq) goto loc_821EF5C4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ef5c4
  if (cr0.eq) goto loc_821EF5C4;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r9,-1
  r9 = uint64_t(int64_t(-1));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10008);
  // addi r10,r11,255
  r10 = uint64_t(int64_t(int64_t(r11) + 255));
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // clrlwi r10,r10,24
  r10 = uint32_t(r10) & 0xFF;
  // mr r8,r10
  r8 = r10;
  // addi r8,r8,2439
  r8 = uint64_t(int64_t(int64_t(r8) + 2439));
  // stb r10,10008(r31)
  REX_STORE_U8(uint32_t(r31) + 10008, uint8_t(r10));
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r10,r8,r31
  r10 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r31));
  // stw r10,9652(r31)
  REX_STORE_U32(uint32_t(r31) + 9652, uint32_t(r10));
  // stw r9,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r9));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
  // stw r10,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r10));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // lwz r8,9652(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 9652);
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r8,r10
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r10), so);
  // beq cr6,0x821ef54c
  if (cr6.eq) goto loc_821EF54C;
  // stw r10,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r10));
loc_821EF54C:
  // li r10,2
  r10 = uint64_t(int64_t(2));
loc_821EF550:
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r9,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r9));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // b 0x821ef5c4
  goto loc_821EF5C4;
loc_821EF564:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,30492(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30492);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // stw r10,30492(r31)
  REX_STORE_U32(uint32_t(r31) + 30492, uint32_t(r10));
  // ble cr6,0x821ef5bc
  if (!cr6.gt) goto loc_821EF5BC;
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_821EF58C:
  // stw r10,30492(r31)
  REX_STORE_U32(uint32_t(r31) + 30492, uint32_t(r10));
  // b 0x821ef5c0
  goto loc_821EF5C0;
loc_821EF594:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,30492(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30492);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // stw r10,30492(r31)
  REX_STORE_U32(uint32_t(r31) + 30492, uint32_t(r10));
  // bge 0x821ef5bc
  if (!cr0.lt) goto loc_821EF5BC;
  // li r10,3
  r10 = uint64_t(int64_t(3));
  // b 0x821ef58c
  goto loc_821EF58C;
loc_821EF5BC:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821EF5C0:
  // stw r11,30496(r31)
  REX_STORE_U32(uint32_t(r31) + 30496, uint32_t(r11));
loc_821EF5C4:
  // mr r3,r31
  r3 = r31;
  // bl 0x821e9920
  // (left out: dead in C++)
  { const lifted::Result o = f821E9920(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_821EF5E8: lifted from generated/ by lift.py.
lifted::Result f821EF5E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821EF5E8 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef644
  if (cr6.eq) goto loc_821EF644;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821efa24
  if (!cr0.eq) goto loc_821EFA24;
loc_821EF644:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef670
  if (cr6.eq) goto loc_821EF670;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821efa24
  if (!cr0.eq) goto loc_821EFA24;
loc_821EF670:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef69c
  if (cr6.eq) goto loc_821EF69C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ef9f4
  if (!cr0.eq) goto loc_821EF9F4;
loc_821EF69C:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef6c8
  if (cr6.eq) goto loc_821EF6C8;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821ef9f4
  if (!cr0.eq) goto loc_821EF9F4;
loc_821EF6C8:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821ef940
  if (cr6.eq) goto loc_821EF940;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821ef940
  if (cr0.eq) goto loc_821EF940;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,20
  r10 = uint64_t(int64_t(20));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // stw r11,9716(r31)
  REX_STORE_U32(uint32_t(r31) + 9716, uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10008);
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r10,r11,r31
  REX_STORE_U32(uint32_t(r11) + uint32_t(r31), uint32_t(r10));
  // lbz r10,10008(r31)
  r10 = REX_LOAD_U8(uint32_t(r31) + 10008);
  // lwz r11,30500(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30500);
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi r11,0
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // stb r10,10008(r31)
  REX_STORE_U8(uint32_t(r31) + 10008, uint8_t(r10));
  // bne 0x821ef810
  if (!cr0.eq) goto loc_821EF810;
  // lwz r10,30492(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30492);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821ef78c
  if (cr6.eq) goto loc_821EF78C;
  // addi r11,r10,7645
  r11 = uint64_t(int64_t(int64_t(r10) + 7645));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r31
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r31));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ef78c
  if (cr6.eq) goto loc_821EF78C;
  // addi r9,r10,7649
  r9 = uint64_t(int64_t(int64_t(r10) + 7649));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r10,7653
  r8 = uint64_t(int64_t(int64_t(r10) + 7653));
  // addi r7,r10,7657
  r7 = uint64_t(int64_t(int64_t(r10) + 7657));
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // stw r11,30508(r31)
  REX_STORE_U32(uint32_t(r31) + 30508, uint32_t(r11));
  // rlwinm r7,r7,2,0,29
  r7 = __builtin_rotateleft64(uint32_t(r7) | (r7 << 32), 2) & 0xFFFFFFFC;
  // lwzx r9,r9,r31
  r9 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r31));
  // stw r9,30512(r31)
  REX_STORE_U32(uint32_t(r31) + 30512, uint32_t(r9));
  // lwzx r9,r8,r31
  r9 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r31));
  // stw r9,30516(r31)
  REX_STORE_U32(uint32_t(r31) + 30516, uint32_t(r9));
  // lwzx r9,r7,r31
  r9 = REX_LOAD_U32(uint32_t(r7) + uint32_t(r31));
  // b 0x821ef7a0
  goto loc_821EF7A0;
loc_821EF78C:
  // li r9,180
  r9 = uint64_t(int64_t(180));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r9,30512(r31)
  REX_STORE_U32(uint32_t(r31) + 30512, uint32_t(r9));
  // stw r11,30508(r31)
  REX_STORE_U32(uint32_t(r31) + 30508, uint32_t(r11));
  // stw r9,30516(r31)
  REX_STORE_U32(uint32_t(r31) + 30516, uint32_t(r9));
loc_821EF7A0:
  // addi r10,r10,7661
  r10 = uint64_t(int64_t(int64_t(r10) + 7661));
  // stw r11,30528(r31)
  REX_STORE_U32(uint32_t(r31) + 30528, uint32_t(r11));
  // stw r9,30520(r31)
  REX_STORE_U32(uint32_t(r31) + 30520, uint32_t(r9));
  // li r8,21
  r8 = uint64_t(int64_t(21));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwz r9,10036(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // lwzx r7,r10,r31
  r7 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r31));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r11,30532(r31)
  REX_STORE_U32(uint32_t(r31) + 30532, uint32_t(r11));
  // stw r11,30540(r31)
  REX_STORE_U32(uint32_t(r31) + 30540, uint32_t(r11));
  // stw r8,9652(r31)
  REX_STORE_U32(uint32_t(r31) + 9652, uint32_t(r8));
  // stw r7,30524(r31)
  REX_STORE_U32(uint32_t(r31) + 30524, uint32_t(r7));
  // rotlwi r7,r7,0
  r7 = __builtin_rotateleft32(uint32_t(r7), 0);
  // stw r7,30536(r31)
  REX_STORE_U32(uint32_t(r31) + 30536, uint32_t(r7));
  // stw r8,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r8));
  // stw r8,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r8));
  // stw r8,8(r9)
  REX_STORE_U32(uint32_t(r9) + 8, uint32_t(r8));
  // stw r10,20(r9)
  REX_STORE_U32(uint32_t(r9) + 20, uint32_t(r10));
  // stw r10,12(r9)
  REX_STORE_U32(uint32_t(r9) + 12, uint32_t(r10));
  // stw r11,24(r9)
  REX_STORE_U32(uint32_t(r9) + 24, uint32_t(r11));
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // lwz r8,9652(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 9652);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821ef808
  if (cr6.eq) goto loc_821EF808;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821EF808:
  // li r9,13
  r9 = uint64_t(int64_t(13));
  // b 0x821ef9e0
  goto loc_821EF9E0;
loc_821EF810:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x821ef8e8
  if (!cr6.eq) goto loc_821EF8E8;
  // lwz r10,30492(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30492);
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // beq cr6,0x821ef860
  if (cr6.eq) goto loc_821EF860;
  // addi r11,r10,7665
  r11 = uint64_t(int64_t(int64_t(r10) + 7665));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwzx r11,r11,r31
  r11 = REX_LOAD_U32(uint32_t(r11) + uint32_t(r31));
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq cr6,0x821ef860
  if (cr6.eq) goto loc_821EF860;
  // addi r9,r10,7669
  r9 = uint64_t(int64_t(int64_t(r10) + 7669));
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // rlwinm r9,r9,2,0,29
  r9 = __builtin_rotateleft64(uint32_t(r9) | (r9 << 32), 2) & 0xFFFFFFFC;
  // addi r8,r10,7673
  r8 = uint64_t(int64_t(int64_t(r10) + 7673));
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // stw r11,30544(r31)
  REX_STORE_U32(uint32_t(r31) + 30544, uint32_t(r11));
  // lwzx r9,r9,r31
  r9 = REX_LOAD_U32(uint32_t(r9) + uint32_t(r31));
  // stw r9,30548(r31)
  REX_STORE_U32(uint32_t(r31) + 30548, uint32_t(r9));
  // lwzx r9,r8,r31
  r9 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r31));
  // b 0x821ef870
  goto loc_821EF870;
loc_821EF860:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r9,180
  r9 = uint64_t(int64_t(180));
  // stw r11,30544(r31)
  REX_STORE_U32(uint32_t(r31) + 30544, uint32_t(r11));
  // stw r11,30548(r31)
  REX_STORE_U32(uint32_t(r31) + 30548, uint32_t(r11));
loc_821EF870:
  // addi r10,r10,7677
  r10 = uint64_t(int64_t(int64_t(r10) + 7677));
  // stw r9,30552(r31)
  REX_STORE_U32(uint32_t(r31) + 30552, uint32_t(r9));
  // stw r11,30560(r31)
  REX_STORE_U32(uint32_t(r31) + 30560, uint32_t(r11));
  // li r8,22
  r8 = uint64_t(int64_t(22));
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // lwz r9,30548(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 30548);
  // lwzx r7,r10,r31
  r7 = REX_LOAD_U32(uint32_t(r10) + uint32_t(r31));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r9,30568(r31)
  REX_STORE_U32(uint32_t(r31) + 30568, uint32_t(r9));
  // lwz r9,10036(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // stw r11,30564(r31)
  REX_STORE_U32(uint32_t(r31) + 30564, uint32_t(r11));
  // stw r11,30576(r31)
  REX_STORE_U32(uint32_t(r31) + 30576, uint32_t(r11));
  // stw r7,30556(r31)
  REX_STORE_U32(uint32_t(r31) + 30556, uint32_t(r7));
  // rotlwi r7,r7,0
  r7 = __builtin_rotateleft32(uint32_t(r7), 0);
  // stw r8,9652(r31)
  REX_STORE_U32(uint32_t(r31) + 9652, uint32_t(r8));
  // stw r7,30572(r31)
  REX_STORE_U32(uint32_t(r31) + 30572, uint32_t(r7));
  // stw r8,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r8));
  // stw r8,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r8));
  // stw r8,8(r9)
  REX_STORE_U32(uint32_t(r9) + 8, uint32_t(r8));
  // stw r10,20(r9)
  REX_STORE_U32(uint32_t(r9) + 20, uint32_t(r10));
  // stw r10,12(r9)
  REX_STORE_U32(uint32_t(r9) + 12, uint32_t(r10));
  // stw r11,24(r9)
  REX_STORE_U32(uint32_t(r9) + 24, uint32_t(r11));
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // lwz r8,9652(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 9652);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821ef8e0
  if (cr6.eq) goto loc_821EF8E0;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821EF8E0:
  // li r9,14
  r9 = uint64_t(int64_t(14));
  // b 0x821ef9e0
  goto loc_821EF9E0;
loc_821EF8E8:
  // cmpwi cr6,r11,2
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 2, so);
  // bne cr6,0x821efa54
  if (!cr6.eq) goto loc_821EFA54;
  // li r8,14
  r8 = uint64_t(int64_t(14));
  // lwz r9,10036(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r8,9652(r31)
  REX_STORE_U32(uint32_t(r31) + 9652, uint32_t(r8));
  // stw r11,9672(r31)
  REX_STORE_U32(uint32_t(r31) + 9672, uint32_t(r11));
  // stw r8,4(r9)
  REX_STORE_U32(uint32_t(r9) + 4, uint32_t(r8));
  // stw r8,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r8));
  // stw r8,8(r9)
  REX_STORE_U32(uint32_t(r9) + 8, uint32_t(r8));
  // stw r10,20(r9)
  REX_STORE_U32(uint32_t(r9) + 20, uint32_t(r10));
  // stw r10,12(r9)
  REX_STORE_U32(uint32_t(r9) + 12, uint32_t(r10));
  // stw r11,24(r9)
  REX_STORE_U32(uint32_t(r9) + 24, uint32_t(r11));
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // lwz r8,9652(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 9652);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821ef938
  if (cr6.eq) goto loc_821EF938;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821EF938:
  // li r9,63
  r9 = uint64_t(int64_t(63));
  // b 0x821ef9e0
  goto loc_821EF9E0;
loc_821EF940:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efa54
  if (cr6.eq) goto loc_821EFA54;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821efa54
  if (cr0.eq) goto loc_821EFA54;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10008);
  // addi r9,r11,255
  r9 = uint64_t(int64_t(int64_t(r11) + 255));
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // clrlwi r9,r9,24
  r9 = uint32_t(r9) & 0xFF;
  // mr r8,r9
  r8 = r9;
  // addi r8,r8,2439
  r8 = uint64_t(int64_t(int64_t(r8) + 2439));
  // stb r9,10008(r31)
  REX_STORE_U8(uint32_t(r31) + 10008, uint8_t(r9));
  // rlwinm r8,r8,2,0,29
  r8 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 2) & 0xFFFFFFFC;
  // lwzx r9,r8,r31
  r9 = REX_LOAD_U32(uint32_t(r8) + uint32_t(r31));
  // stw r9,9652(r31)
  REX_STORE_U32(uint32_t(r31) + 9652, uint32_t(r9));
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r7,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r7));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // lwz r8,9652(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 9652);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821ef9dc
  if (cr6.eq) goto loc_821EF9DC;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821EF9DC:
  // li r9,11
  r9 = uint64_t(int64_t(11));
loc_821EF9E0:
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821efa54
  goto loc_821EFA54;
loc_821EF9F4:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,30500(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30500);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpwi cr6,r10,2
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 2, so);
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // stw r10,30500(r31)
  REX_STORE_U32(uint32_t(r31) + 30500, uint32_t(r10));
  // ble cr6,0x821efa4c
  if (!cr6.gt) goto loc_821EFA4C;
  // li r10,0
  r10 = uint64_t(int64_t(0));
loc_821EFA1C:
  // stw r10,30500(r31)
  REX_STORE_U32(uint32_t(r31) + 30500, uint32_t(r10));
  // b 0x821efa50
  goto loc_821EFA50;
loc_821EFA24:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r10,30500(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30500);
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // addic. r10,r10,-1
  ca = uint32_t(r10) > 0;
  r10 = uint64_t(int64_t(int64_t(r10) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // stw r10,30500(r31)
  REX_STORE_U32(uint32_t(r31) + 30500, uint32_t(r10));
  // bge 0x821efa4c
  if (!cr0.lt) goto loc_821EFA4C;
  // li r10,2
  r10 = uint64_t(int64_t(2));
  // b 0x821efa1c
  goto loc_821EFA1C;
loc_821EFA4C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821EFA50:
  // stw r11,30504(r31)
  REX_STORE_U32(uint32_t(r31) + 30504, uint32_t(r11));
loc_821EFA54:
  // mr r3,r31
  r3 = r31;
  // bl 0x821e9dd8
  // (left out: dead in C++)
  { const lifted::Result o = f821E9DD8(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_821EFA78: lifted from generated/ by lift.py.
lifted::Result f821EFA78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821EFA78 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efad4
  if (cr6.eq) goto loc_821EFAD4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f02b8
  if (!cr0.eq) goto loc_821F02B8;
loc_821EFAD4:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efb00
  if (cr6.eq) goto loc_821EFB00;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f02b8
  if (!cr0.eq) goto loc_821F02B8;
loc_821EFB00:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efb2c
  if (cr6.eq) goto loc_821EFB2C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0288
  if (!cr0.eq) goto loc_821F0288;
loc_821EFB2C:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efb58
  if (cr6.eq) goto loc_821EFB58;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0288
  if (!cr0.eq) goto loc_821F0288;
loc_821EFB58:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efb84
  if (cr6.eq) goto loc_821EFB84;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0030
  if (!cr0.eq) goto loc_821F0030;
loc_821EFB84:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efbb0
  if (cr6.eq) goto loc_821EFBB0;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0030
  if (!cr0.eq) goto loc_821F0030;
loc_821EFBB0:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efbdc
  if (cr6.eq) goto loc_821EFBDC;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821efdb0
  if (!cr0.eq) goto loc_821EFDB0;
loc_821EFBDC:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efc08
  if (cr6.eq) goto loc_821EFC08;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821efdb0
  if (!cr0.eq) goto loc_821EFDB0;
loc_821EFC08:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efd38
  if (cr6.eq) goto loc_821EFD38;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821efd38
  if (cr0.eq) goto loc_821EFD38;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30508(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30508);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821efd14
  if (!cr6.eq) goto loc_821EFD14;
  // lwz r10,30528(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30528);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821efc64
  if (!cr6.eq) goto loc_821EFC64;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // stb r8,9667(r31)
  REX_STORE_U8(uint32_t(r31) + 9667, uint8_t(r8));
  // b 0x821efc84
  goto loc_821EFC84;
loc_821EFC64:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821efd14
  if (!cr6.eq) goto loc_821EFD14;
  // lwz r10,30528(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30528);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821efd00
  if (!cr6.eq) goto loc_821EFD00;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r9,13
  r9 = uint64_t(int64_t(13));
  // stb r8,9668(r31)
  REX_STORE_U8(uint32_t(r31) + 9668, uint8_t(r8));
loc_821EFC84:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r7,21
  r7 = uint64_t(int64_t(21));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10008);
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r31
  REX_STORE_U32(uint32_t(r11) + uint32_t(r31), uint32_t(r7));
  // lbz r7,10008(r31)
  r7 = REX_LOAD_U8(uint32_t(r31) + 10008);
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // stw r9,9652(r31)
  REX_STORE_U32(uint32_t(r31) + 9652, uint32_t(r9));
  // stb r7,10008(r31)
  REX_STORE_U8(uint32_t(r31) + 10008, uint8_t(r7));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r8));
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // lwz r8,9652(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 9652);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821efce8
  if (cr6.eq) goto loc_821EFCE8;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821EFCE8:
  // li r9,63
  r9 = uint64_t(int64_t(63));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821f02f8
  goto loc_821F02F8;
loc_821EFD00:
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821efd14
  if (!cr6.eq) goto loc_821EFD14;
  // lwz r10,30528(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30528);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821f02f8
  if (cr6.eq) goto loc_821F02F8;
loc_821EFD14:
  // lwz r10,30524(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30524);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r11,30532(r31)
  REX_STORE_U32(uint32_t(r31) + 30532, uint32_t(r11));
  // lwz r11,30528(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30528);
  // stw r10,30536(r31)
  REX_STORE_U32(uint32_t(r31) + 30536, uint32_t(r10));
  // stw r9,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r9));
  // stw r7,30528(r31)
  REX_STORE_U32(uint32_t(r31) + 30528, uint32_t(r7));
  // b 0x821efda0
  goto loc_821EFDA0;
loc_821EFD38:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f02f8
  if (cr6.eq) goto loc_821F02F8;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f02f8
  if (cr0.eq) goto loc_821F02F8;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30508(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30508);
  // cmpwi cr6,r11,4
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 4, so);
  // bne cr6,0x821efd84
  if (!cr6.eq) goto loc_821EFD84;
  // lwz r10,30528(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30528);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821f02f8
  if (cr6.eq) goto loc_821F02F8;
loc_821EFD84:
  // lwz r9,30524(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 30524);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,30532(r31)
  REX_STORE_U32(uint32_t(r31) + 30532, uint32_t(r11));
  // lwz r11,30528(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30528);
  // stw r9,30536(r31)
  REX_STORE_U32(uint32_t(r31) + 30536, uint32_t(r9));
  // stw r10,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r10));
  // stw r10,30528(r31)
  REX_STORE_U32(uint32_t(r31) + 30528, uint32_t(r10));
loc_821EFDA0:
  // li r8,4
  r8 = uint64_t(int64_t(4));
  // stw r11,30540(r31)
  REX_STORE_U32(uint32_t(r31) + 30540, uint32_t(r11));
  // stw r8,30508(r31)
  REX_STORE_U32(uint32_t(r31) + 30508, uint32_t(r8));
  // b 0x821f02f8
  goto loc_821F02F8;
loc_821EFDB0:
  // lwz r11,30508(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30508);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // stw r11,30532(r31)
  REX_STORE_U32(uint32_t(r31) + 30532, uint32_t(r11));
  // blt cr6,0x821eff78
  if (cr6.lt) goto loc_821EFF78;
  // beq cr6,0x821efecc
  if (cr6.eq) goto loc_821EFECC;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821efe20
  if (cr6.lt) goto loc_821EFE20;
  // beq cr6,0x821efdfc
  if (cr6.eq) goto loc_821EFDFC;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821f02f8
  if (!cr6.lt) goto loc_821F02F8;
  // lwz r11,30528(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30528);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // stw r11,30540(r31)
  REX_STORE_U32(uint32_t(r31) + 30540, uint32_t(r11));
  // stw r10,30528(r31)
  REX_STORE_U32(uint32_t(r31) + 30528, uint32_t(r10));
  // ble cr6,0x821f02e8
  if (!cr6.gt) goto loc_821F02E8;
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821EFDF4:
  // stw r11,30528(r31)
  REX_STORE_U32(uint32_t(r31) + 30528, uint32_t(r11));
  // b 0x821f02f8
  goto loc_821F02F8;
loc_821EFDFC:
  // lwz r11,30524(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30524);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // stw r11,30536(r31)
  REX_STORE_U32(uint32_t(r31) + 30536, uint32_t(r11));
  // stw r10,30524(r31)
  REX_STORE_U32(uint32_t(r31) + 30524, uint32_t(r10));
  // ble cr6,0x821f02e8
  if (!cr6.gt) goto loc_821F02E8;
  // li r11,7
  r11 = uint64_t(int64_t(7));
loc_821EFE18:
  // stw r11,30524(r31)
  REX_STORE_U32(uint32_t(r31) + 30524, uint32_t(r11));
  // b 0x821f02f8
  goto loc_821F02F8;
loc_821EFE20:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efe5c
  if (cr6.eq) goto loc_821EFE5C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821efe68
  if (!cr6.lt) goto loc_821EFE68;
loc_821EFE5C:
  // lwz r11,30520(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30520);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x821efeb0
  goto loc_821EFEB0;
loc_821EFE68:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efe9c
  if (cr6.eq) goto loc_821EFE9C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821efea8
  if (!cr6.lt) goto loc_821EFEA8;
loc_821EFE9C:
  // lwz r11,30520(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30520);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // b 0x821efeb0
  goto loc_821EFEB0;
loc_821EFEA8:
  // lwz r11,30520(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30520);
  // addi r11,r11,5
  r11 = uint64_t(int64_t(int64_t(r11) + 5));
loc_821EFEB0:
  // stw r11,30520(r31)
  REX_STORE_U32(uint32_t(r31) + 30520, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,360
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 360, so);
  // ble cr6,0x821f0024
  if (!cr6.gt) goto loc_821F0024;
  // li r11,360
  r11 = uint64_t(int64_t(360));
loc_821EFEC4:
  // stw r11,30520(r31)
  REX_STORE_U32(uint32_t(r31) + 30520, uint32_t(r11));
  // b 0x821f02f8
  goto loc_821F02F8;
loc_821EFECC:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eff08
  if (cr6.eq) goto loc_821EFF08;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821eff14
  if (!cr6.lt) goto loc_821EFF14;
loc_821EFF08:
  // lwz r11,30516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30516);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x821eff5c
  goto loc_821EFF5C;
loc_821EFF14:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821eff48
  if (cr6.eq) goto loc_821EFF48;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821eff54
  if (!cr6.lt) goto loc_821EFF54;
loc_821EFF48:
  // lwz r11,30516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30516);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // b 0x821eff5c
  goto loc_821EFF5C;
loc_821EFF54:
  // lwz r11,30516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30516);
  // addi r11,r11,5
  r11 = uint64_t(int64_t(int64_t(r11) + 5));
loc_821EFF5C:
  // stw r11,30516(r31)
  REX_STORE_U32(uint32_t(r31) + 30516, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,360
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 360, so);
  // ble cr6,0x821f0024
  if (!cr6.gt) goto loc_821F0024;
  // li r11,360
  r11 = uint64_t(int64_t(360));
loc_821EFF70:
  // stw r11,30516(r31)
  REX_STORE_U32(uint32_t(r31) + 30516, uint32_t(r11));
  // b 0x821f02f8
  goto loc_821F02F8;
loc_821EFF78:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821effb4
  if (cr6.eq) goto loc_821EFFB4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821effc0
  if (!cr6.lt) goto loc_821EFFC0;
loc_821EFFB4:
  // lwz r11,30512(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30512);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x821f0008
  goto loc_821F0008;
loc_821EFFC0:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821efff4
  if (cr6.eq) goto loc_821EFFF4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f0000
  if (!cr6.lt) goto loc_821F0000;
loc_821EFFF4:
  // lwz r11,30512(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30512);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // b 0x821f0008
  goto loc_821F0008;
loc_821F0000:
  // lwz r11,30512(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30512);
  // addi r11,r11,5
  r11 = uint64_t(int64_t(int64_t(r11) + 5));
loc_821F0008:
  // stw r11,30512(r31)
  REX_STORE_U32(uint32_t(r31) + 30512, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,360
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 360, so);
  // ble cr6,0x821f0024
  if (!cr6.gt) goto loc_821F0024;
  // li r11,360
  r11 = uint64_t(int64_t(360));
loc_821F001C:
  // stw r11,30512(r31)
  REX_STORE_U32(uint32_t(r31) + 30512, uint32_t(r11));
  // b 0x821f02f8
  goto loc_821F02F8;
loc_821F0024:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821f02f8
  goto loc_821F02F8;
loc_821F0030:
  // lwz r11,30508(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30508);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // stw r11,30532(r31)
  REX_STORE_U32(uint32_t(r31) + 30532, uint32_t(r11));
  // blt cr6,0x821f01e0
  if (cr6.lt) goto loc_821F01E0;
  // beq cr6,0x821f0138
  if (cr6.eq) goto loc_821F0138;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821f0090
  if (cr6.lt) goto loc_821F0090;
  // beq cr6,0x821f0074
  if (cr6.eq) goto loc_821F0074;
  // cmplwi cr6,r11,5
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 5, so);
  // bge cr6,0x821f02f8
  if (!cr6.lt) goto loc_821F02F8;
  // lwz r11,30528(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30528);
  // addic. r10,r11,-1
  ca = uint32_t(r11) > 0;
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,30540(r31)
  REX_STORE_U32(uint32_t(r31) + 30540, uint32_t(r11));
  // stw r10,30528(r31)
  REX_STORE_U32(uint32_t(r31) + 30528, uint32_t(r10));
  // bge 0x821f02e8
  if (!cr0.lt) goto loc_821F02E8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821efdf4
  goto loc_821EFDF4;
loc_821F0074:
  // lwz r11,30524(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30524);
  // addic. r10,r11,-1
  ca = uint32_t(r11) > 0;
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,30536(r31)
  REX_STORE_U32(uint32_t(r31) + 30536, uint32_t(r11));
  // stw r10,30524(r31)
  REX_STORE_U32(uint32_t(r31) + 30524, uint32_t(r10));
  // bge 0x821f02e8
  if (!cr0.lt) goto loc_821F02E8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821efe18
  goto loc_821EFE18;
loc_821F0090:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f00cc
  if (cr6.eq) goto loc_821F00CC;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f00d8
  if (!cr6.lt) goto loc_821F00D8;
loc_821F00CC:
  // lwz r11,30520(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30520);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x821f0120
  goto loc_821F0120;
loc_821F00D8:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f010c
  if (cr6.eq) goto loc_821F010C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f0118
  if (!cr6.lt) goto loc_821F0118;
loc_821F010C:
  // lwz r11,30520(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30520);
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // b 0x821f0120
  goto loc_821F0120;
loc_821F0118:
  // lwz r11,30520(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30520);
  // addi r11,r11,-5
  r11 = uint64_t(int64_t(int64_t(r11) + -5));
loc_821F0120:
  // stw r11,30520(r31)
  REX_STORE_U32(uint32_t(r31) + 30520, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x821f0024
  if (!cr6.lt) goto loc_821F0024;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821efec4
  goto loc_821EFEC4;
loc_821F0138:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0174
  if (cr6.eq) goto loc_821F0174;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f0180
  if (!cr6.lt) goto loc_821F0180;
loc_821F0174:
  // lwz r11,30516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30516);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x821f01c8
  goto loc_821F01C8;
loc_821F0180:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f01b4
  if (cr6.eq) goto loc_821F01B4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f01c0
  if (!cr6.lt) goto loc_821F01C0;
loc_821F01B4:
  // lwz r11,30516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30516);
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // b 0x821f01c8
  goto loc_821F01C8;
loc_821F01C0:
  // lwz r11,30516(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30516);
  // addi r11,r11,-5
  r11 = uint64_t(int64_t(int64_t(r11) + -5));
loc_821F01C8:
  // stw r11,30516(r31)
  REX_STORE_U32(uint32_t(r31) + 30516, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x821f0024
  if (!cr6.lt) goto loc_821F0024;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821eff70
  goto loc_821EFF70;
loc_821F01E0:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f021c
  if (cr6.eq) goto loc_821F021C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f0228
  if (!cr6.lt) goto loc_821F0228;
loc_821F021C:
  // lwz r11,30512(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30512);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x821f0270
  goto loc_821F0270;
loc_821F0228:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f025c
  if (cr6.eq) goto loc_821F025C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f0268
  if (!cr6.lt) goto loc_821F0268;
loc_821F025C:
  // lwz r11,30512(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30512);
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // b 0x821f0270
  goto loc_821F0270;
loc_821F0268:
  // lwz r11,30512(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30512);
  // addi r11,r11,-5
  r11 = uint64_t(int64_t(int64_t(r11) + -5));
loc_821F0270:
  // stw r11,30512(r31)
  REX_STORE_U32(uint32_t(r31) + 30512, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x821f0024
  if (!cr6.lt) goto loc_821F0024;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821f001c
  goto loc_821F001C;
loc_821F0288:
  // lwz r11,30524(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30524);
  // lwz r10,30528(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30528);
  // stw r11,30536(r31)
  REX_STORE_U32(uint32_t(r31) + 30536, uint32_t(r11));
  // lwz r11,30508(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30508);
  // stw r10,30540(r31)
  REX_STORE_U32(uint32_t(r31) + 30540, uint32_t(r10));
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,4
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 4, so);
  // stw r11,30532(r31)
  REX_STORE_U32(uint32_t(r31) + 30532, uint32_t(r11));
  // stw r10,30508(r31)
  REX_STORE_U32(uint32_t(r31) + 30508, uint32_t(r10));
  // ble cr6,0x821f02e8
  if (!cr6.gt) goto loc_821F02E8;
  // li r11,4
  r11 = uint64_t(int64_t(4));
  // b 0x821f02e0
  goto loc_821F02E0;
loc_821F02B8:
  // lwz r11,30524(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30524);
  // lwz r10,30528(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30528);
  // stw r11,30536(r31)
  REX_STORE_U32(uint32_t(r31) + 30536, uint32_t(r11));
  // lwz r11,30508(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30508);
  // stw r10,30540(r31)
  REX_STORE_U32(uint32_t(r31) + 30540, uint32_t(r10));
  // addic. r10,r11,-1
  ca = uint32_t(r11) > 0;
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,30532(r31)
  REX_STORE_U32(uint32_t(r31) + 30532, uint32_t(r11));
  // stw r10,30508(r31)
  REX_STORE_U32(uint32_t(r31) + 30508, uint32_t(r10));
  // bge 0x821f02e8
  if (!cr0.lt) goto loc_821F02E8;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821F02E0:
  // stw r11,30508(r31)
  REX_STORE_U32(uint32_t(r31) + 30508, uint32_t(r11));
  // b 0x821f02f8
  goto loc_821F02F8;
loc_821F02E8:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
loc_821F02F8:
  // mr r3,r31
  r3 = r31;
  // bl 0x821ea898
  // (left out: dead in C++)
  { const lifted::Result o = f821EA898(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_821F0318: lifted from generated/ by lift.py.
lifted::Result f821F0318(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_821F0318 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r10,-32148
  r10 = uint64_t(int64_t(-2106851328));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-21632
  r11 = uint64_t(int64_t(int64_t(r11) + -21632));
  // lwz r10,-21596(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + -21596);
  // rlwinm r10,r10,3,0,28
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 3) & 0xFFFFFFF8;
  // add r30,r10,r11
  r30 = r10 + r11;
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0374
  if (cr6.eq) goto loc_821F0374;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,153
  r4 = uint64_t(int64_t(153));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0934
  if (!cr0.eq) goto loc_821F0934;
loc_821F0374:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f03a0
  if (cr6.eq) goto loc_821F03A0;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,167
  r4 = uint64_t(int64_t(167));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0934
  if (!cr0.eq) goto loc_821F0934;
loc_821F03A0:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f03cc
  if (cr6.eq) goto loc_821F03CC;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,154
  r4 = uint64_t(int64_t(154));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0904
  if (!cr0.eq) goto loc_821F0904;
loc_821F03CC:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f03f8
  if (cr6.eq) goto loc_821F03F8;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,168
  r4 = uint64_t(int64_t(168));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0904
  if (!cr0.eq) goto loc_821F0904;
loc_821F03F8:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0424
  if (cr6.eq) goto loc_821F0424;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,155
  r4 = uint64_t(int64_t(155));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f07b8
  if (!cr0.eq) goto loc_821F07B8;
loc_821F0424:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0450
  if (cr6.eq) goto loc_821F0450;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f07b8
  if (!cr0.eq) goto loc_821F07B8;
loc_821F0450:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f047c
  if (cr6.eq) goto loc_821F047C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,156
  r4 = uint64_t(int64_t(156));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0650
  if (!cr0.eq) goto loc_821F0650;
loc_821F047C:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f04a8
  if (cr6.eq) goto loc_821F04A8;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821f0650
  if (!cr0.eq) goto loc_821F0650;
loc_821F04A8:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f05d8
  if (cr6.eq) goto loc_821F05D8;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,159
  r4 = uint64_t(int64_t(159));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f05d8
  if (cr0.eq) goto loc_821F05D8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30544(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30544);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821f05b4
  if (!cr6.eq) goto loc_821F05B4;
  // lwz r10,30560(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30560);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bne cr6,0x821f0504
  if (!cr6.eq) goto loc_821F0504;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r9,12
  r9 = uint64_t(int64_t(12));
  // stb r8,9667(r31)
  REX_STORE_U8(uint32_t(r31) + 9667, uint8_t(r8));
  // b 0x821f0524
  goto loc_821F0524;
loc_821F0504:
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821f05b4
  if (!cr6.eq) goto loc_821F05B4;
  // lwz r10,30560(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30560);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // bne cr6,0x821f05a0
  if (!cr6.eq) goto loc_821F05A0;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // li r9,13
  r9 = uint64_t(int64_t(13));
  // stb r8,9668(r31)
  REX_STORE_U8(uint32_t(r31) + 9668, uint8_t(r8));
loc_821F0524:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r7,22
  r7 = uint64_t(int64_t(22));
  // li r10,-1
  r10 = uint64_t(int64_t(-1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lbz r11,10008(r31)
  r11 = REX_LOAD_U8(uint32_t(r31) + 10008);
  // addi r11,r11,2439
  r11 = uint64_t(int64_t(int64_t(r11) + 2439));
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // stwx r7,r11,r31
  REX_STORE_U32(uint32_t(r11) + uint32_t(r31), uint32_t(r7));
  // lbz r7,10008(r31)
  r7 = REX_LOAD_U8(uint32_t(r31) + 10008);
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // addi r7,r7,1
  r7 = uint64_t(int64_t(int64_t(r7) + 1));
  // stw r9,9652(r31)
  REX_STORE_U32(uint32_t(r31) + 9652, uint32_t(r9));
  // stb r7,10008(r31)
  REX_STORE_U8(uint32_t(r31) + 10008, uint8_t(r7));
  // stw r9,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r9));
  // stw r9,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r9));
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
  // stw r10,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r10));
  // stw r10,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r10));
  // stw r8,24(r11)
  REX_STORE_U32(uint32_t(r11) + 24, uint32_t(r8));
  // lwz r11,10036(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 10036);
  // lwz r8,9652(r31)
  r8 = REX_LOAD_U32(uint32_t(r31) + 9652);
  // lwz r9,0(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmpw cr6,r8,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r8), int32_t(r9), so);
  // beq cr6,0x821f0588
  if (cr6.eq) goto loc_821F0588;
  // stw r9,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r9));
loc_821F0588:
  // li r9,63
  r9 = uint64_t(int64_t(63));
  // stw r9,12(r11)
  REX_STORE_U32(uint32_t(r11) + 12, uint32_t(r9));
  // stw r10,16(r11)
  REX_STORE_U32(uint32_t(r11) + 16, uint32_t(r10));
  // stw r8,4(r11)
  REX_STORE_U32(uint32_t(r11) + 4, uint32_t(r8));
  // stw r10,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r10));
  // b 0x821f0974
  goto loc_821F0974;
loc_821F05A0:
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821f05b4
  if (!cr6.eq) goto loc_821F05B4;
  // lwz r10,30560(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30560);
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq cr6,0x821f0974
  if (cr6.eq) goto loc_821F0974;
loc_821F05B4:
  // lwz r10,30556(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30556);
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // li r7,0
  r7 = uint64_t(int64_t(0));
  // stw r11,30564(r31)
  REX_STORE_U32(uint32_t(r31) + 30564, uint32_t(r11));
  // lwz r11,30560(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30560);
  // stw r10,30572(r31)
  REX_STORE_U32(uint32_t(r31) + 30572, uint32_t(r10));
  // stw r9,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r9));
  // stw r7,30560(r31)
  REX_STORE_U32(uint32_t(r31) + 30560, uint32_t(r7));
  // b 0x821f0640
  goto loc_821F0640;
loc_821F05D8:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0974
  if (cr6.eq) goto loc_821F0974;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,158
  r4 = uint64_t(int64_t(158));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x821f0974
  if (cr0.eq) goto loc_821F0974;
  // li r3,2
  r3 = uint64_t(int64_t(2));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30544(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30544);
  // cmpwi cr6,r11,3
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 3, so);
  // bne cr6,0x821f0624
  if (!cr6.eq) goto loc_821F0624;
  // lwz r10,30560(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30560);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // beq cr6,0x821f0974
  if (cr6.eq) goto loc_821F0974;
loc_821F0624:
  // lwz r9,30556(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 30556);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,30564(r31)
  REX_STORE_U32(uint32_t(r31) + 30564, uint32_t(r11));
  // lwz r11,30560(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30560);
  // stw r9,30572(r31)
  REX_STORE_U32(uint32_t(r31) + 30572, uint32_t(r9));
  // stw r10,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r10));
  // stw r10,30560(r31)
  REX_STORE_U32(uint32_t(r31) + 30560, uint32_t(r10));
loc_821F0640:
  // li r8,3
  r8 = uint64_t(int64_t(3));
  // stw r11,30576(r31)
  REX_STORE_U32(uint32_t(r31) + 30576, uint32_t(r11));
  // stw r8,30544(r31)
  REX_STORE_U32(uint32_t(r31) + 30544, uint32_t(r8));
  // b 0x821f0974
  goto loc_821F0974;
loc_821F0650:
  // lwz r11,30544(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30544);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // stw r11,30564(r31)
  REX_STORE_U32(uint32_t(r31) + 30564, uint32_t(r11));
  // blt cr6,0x821f0770
  if (cr6.lt) goto loc_821F0770;
  // beq cr6,0x821f06b8
  if (cr6.eq) goto loc_821F06B8;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821f0694
  if (cr6.lt) goto loc_821F0694;
  // bne cr6,0x821f0974
  if (!cr6.eq) goto loc_821F0974;
  // lwz r11,30560(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30560);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // stw r11,30576(r31)
  REX_STORE_U32(uint32_t(r31) + 30576, uint32_t(r11));
  // stw r10,30560(r31)
  REX_STORE_U32(uint32_t(r31) + 30560, uint32_t(r10));
  // ble cr6,0x821f0964
  if (!cr6.gt) goto loc_821F0964;
  // li r11,1
  r11 = uint64_t(int64_t(1));
loc_821F068C:
  // stw r11,30560(r31)
  REX_STORE_U32(uint32_t(r31) + 30560, uint32_t(r11));
  // b 0x821f0974
  goto loc_821F0974;
loc_821F0694:
  // lwz r11,30556(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30556);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
  // stw r11,30572(r31)
  REX_STORE_U32(uint32_t(r31) + 30572, uint32_t(r11));
  // stw r10,30556(r31)
  REX_STORE_U32(uint32_t(r31) + 30556, uint32_t(r10));
  // ble cr6,0x821f0964
  if (!cr6.gt) goto loc_821F0964;
  // li r11,7
  r11 = uint64_t(int64_t(7));
loc_821F06B0:
  // stw r11,30556(r31)
  REX_STORE_U32(uint32_t(r31) + 30556, uint32_t(r11));
  // b 0x821f0974
  goto loc_821F0974;
loc_821F06B8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f06f4
  if (cr6.eq) goto loc_821F06F4;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f0700
  if (!cr6.lt) goto loc_821F0700;
loc_821F06F4:
  // lwz r11,30552(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30552);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x821f0748
  goto loc_821F0748;
loc_821F0700:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f0734
  if (cr6.eq) goto loc_821F0734;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,170
  r4 = uint64_t(int64_t(170));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f0740
  if (!cr6.lt) goto loc_821F0740;
loc_821F0734:
  // lwz r11,30552(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30552);
  // addi r11,r11,2
  r11 = uint64_t(int64_t(int64_t(r11) + 2));
  // b 0x821f0748
  goto loc_821F0748;
loc_821F0740:
  // lwz r11,30552(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30552);
  // addi r11,r11,5
  r11 = uint64_t(int64_t(int64_t(r11) + 5));
loc_821F0748:
  // stw r11,30552(r31)
  REX_STORE_U32(uint32_t(r31) + 30552, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,360
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 360, so);
  // ble cr6,0x821f0764
  if (!cr6.gt) goto loc_821F0764;
  // li r11,360
  r11 = uint64_t(int64_t(360));
loc_821F075C:
  // stw r11,30552(r31)
  REX_STORE_U32(uint32_t(r31) + 30552, uint32_t(r11));
  // b 0x821f0974
  goto loc_821F0974;
loc_821F0764:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // b 0x821f0974
  goto loc_821F0974;
loc_821F0770:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r10,30492(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30492);
  // cmpwi cr6,r10,1
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 1, so);
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lwz r11,30548(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30548);
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,30568(r31)
  REX_STORE_U32(uint32_t(r31) + 30568, uint32_t(r11));
  // stw r10,30548(r31)
  REX_STORE_U32(uint32_t(r31) + 30548, uint32_t(r10));
  // bne cr6,0x821f07a4
  if (!cr6.eq) goto loc_821F07A4;
  // cmpwi cr6,r10,9
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 9, so);
  // b 0x821f07a8
  goto loc_821F07A8;
loc_821F07A4:
  // cmpwi cr6,r10,7
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 7, so);
loc_821F07A8:
  // ble cr6,0x821f0974
  if (!cr6.gt) goto loc_821F0974;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821F07B0:
  // stw r11,30548(r31)
  REX_STORE_U32(uint32_t(r31) + 30548, uint32_t(r11));
  // b 0x821f0974
  goto loc_821F0974;
loc_821F07B8:
  // lwz r11,30544(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30544);
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // stw r11,30564(r31)
  REX_STORE_U32(uint32_t(r31) + 30564, uint32_t(r11));
  // blt cr6,0x821f08b8
  if (cr6.lt) goto loc_821F08B8;
  // beq cr6,0x821f0810
  if (cr6.eq) goto loc_821F0810;
  // cmplwi cr6,r11,3
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 3, so);
  // blt cr6,0x821f07f4
  if (cr6.lt) goto loc_821F07F4;
  // bne cr6,0x821f0974
  if (!cr6.eq) goto loc_821F0974;
  // lwz r11,30560(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30560);
  // addic. r10,r11,-1
  ca = uint32_t(r11) > 0;
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,30576(r31)
  REX_STORE_U32(uint32_t(r31) + 30576, uint32_t(r11));
  // stw r10,30560(r31)
  REX_STORE_U32(uint32_t(r31) + 30560, uint32_t(r10));
  // bge 0x821f0964
  if (!cr0.lt) goto loc_821F0964;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821f068c
  goto loc_821F068C;
loc_821F07F4:
  // lwz r11,30556(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30556);
  // addic. r10,r11,-1
  ca = uint32_t(r11) > 0;
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,30572(r31)
  REX_STORE_U32(uint32_t(r31) + 30572, uint32_t(r11));
  // stw r10,30556(r31)
  REX_STORE_U32(uint32_t(r31) + 30556, uint32_t(r10));
  // bge 0x821f0964
  if (!cr0.lt) goto loc_821F0964;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821f06b0
  goto loc_821F06B0;
loc_821F0810:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f084c
  if (cr6.eq) goto loc_821F084C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11420(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11420);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f0858
  if (!cr6.lt) goto loc_821F0858;
loc_821F084C:
  // lwz r11,30552(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30552);
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // b 0x821f08a0
  goto loc_821F08A0;
loc_821F0858:
  // lwz r11,4(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // beq cr6,0x821f088c
  if (cr6.eq) goto loc_821F088C;
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,169
  r4 = uint64_t(int64_t(169));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f0,11284(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 11284);
  f0 = double(temp.f32);
  // fcmpu cr6,f1,f0
  cr6 = lifted::CompareFloat(f1, f0);
  // bge cr6,0x821f0898
  if (!cr6.lt) goto loc_821F0898;
loc_821F088C:
  // lwz r11,30552(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30552);
  // addi r11,r11,-2
  r11 = uint64_t(int64_t(int64_t(r11) + -2));
  // b 0x821f08a0
  goto loc_821F08A0;
loc_821F0898:
  // lwz r11,30552(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30552);
  // addi r11,r11,-5
  r11 = uint64_t(int64_t(int64_t(r11) + -5));
loc_821F08A0:
  // stw r11,30552(r31)
  REX_STORE_U32(uint32_t(r31) + 30552, uint32_t(r11));
  // rotlwi r11,r11,0
  r11 = __builtin_rotateleft32(uint32_t(r11), 0);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bge cr6,0x821f0764
  if (!cr6.lt) goto loc_821F0764;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x821f075c
  goto loc_821F075C;
loc_821F08B8:
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,30492(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30492);
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // lwz r11,30548(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30548);
  // addi r10,r11,-1
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,30568(r31)
  REX_STORE_U32(uint32_t(r31) + 30568, uint32_t(r11));
  // stw r10,30548(r31)
  REX_STORE_U32(uint32_t(r31) + 30548, uint32_t(r10));
  // bne cr6,0x821f08f4
  if (!cr6.eq) goto loc_821F08F4;
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge cr6,0x821f0974
  if (!cr6.lt) goto loc_821F0974;
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // b 0x821f07b0
  goto loc_821F07B0;
loc_821F08F4:
  // cmpwi cr6,r10,0
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // bge cr6,0x821f0974
  if (!cr6.lt) goto loc_821F0974;
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // b 0x821f07b0
  goto loc_821F07B0;
loc_821F0904:
  // lwz r11,30556(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30556);
  // lwz r10,30560(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30560);
  // stw r11,30572(r31)
  REX_STORE_U32(uint32_t(r31) + 30572, uint32_t(r11));
  // lwz r11,30544(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30544);
  // stw r10,30576(r31)
  REX_STORE_U32(uint32_t(r31) + 30576, uint32_t(r10));
  // addi r10,r11,1
  r10 = uint64_t(int64_t(int64_t(r11) + 1));
  // cmpwi cr6,r10,3
  cr6 = lifted::Compare<int32_t>(int32_t(r10), 3, so);
  // stw r11,30564(r31)
  REX_STORE_U32(uint32_t(r31) + 30564, uint32_t(r11));
  // stw r10,30544(r31)
  REX_STORE_U32(uint32_t(r31) + 30544, uint32_t(r10));
  // ble cr6,0x821f0964
  if (!cr6.gt) goto loc_821F0964;
  // li r11,3
  r11 = uint64_t(int64_t(3));
  // b 0x821f095c
  goto loc_821F095C;
loc_821F0934:
  // lwz r11,30556(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30556);
  // lwz r10,30560(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 30560);
  // stw r11,30572(r31)
  REX_STORE_U32(uint32_t(r31) + 30572, uint32_t(r11));
  // lwz r11,30544(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 30544);
  // stw r10,30576(r31)
  REX_STORE_U32(uint32_t(r31) + 30576, uint32_t(r10));
  // addic. r10,r11,-1
  ca = uint32_t(r11) > 0;
  r10 = uint64_t(int64_t(int64_t(r11) + -1));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,30564(r31)
  REX_STORE_U32(uint32_t(r31) + 30564, uint32_t(r11));
  // stw r10,30544(r31)
  REX_STORE_U32(uint32_t(r31) + 30544, uint32_t(r10));
  // bge 0x821f0964
  if (!cr0.lt) goto loc_821F0964;
  // li r11,0
  r11 = uint64_t(int64_t(0));
loc_821F095C:
  // stw r11,30544(r31)
  REX_STORE_U32(uint32_t(r31) + 30544, uint32_t(r11));
  // b 0x821f0974
  goto loc_821F0974;
loc_821F0964:
  // li r3,4
  r3 = uint64_t(int64_t(4));
  // bl 0x821103f0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x821103F0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // stw r11,9708(r31)
  REX_STORE_U32(uint32_t(r31) + 9708, uint32_t(r11));
loc_821F0974:
  // mr r3,r31
  r3 = r31;
  // bl 0x821eb2a0
  // (left out: dead in C++)
  { const lifted::Result o = f821EB2A0(base, r1, r13, r3, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
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

namespace {

lifted::Result e821B5BF0(uint8_t* base, const lifted::Args& a) {
  return f821B5BF0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B5CE8(uint8_t* base, const lifted::Args& a) {
  return f821B5CE8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B5DF8(uint8_t* base, const lifted::Args& a) {
  return f821B5DF8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B5F08(uint8_t* base, const lifted::Args& a) {
  return f821B5F08(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B5FF8(uint8_t* base, const lifted::Args& a) {
  return f821B5FF8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B60E8(uint8_t* base, const lifted::Args& a) {
  return f821B60E8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B61D8(uint8_t* base, const lifted::Args& a) {
  return f821B61D8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B62C8(uint8_t* base, const lifted::Args& a) {
  return f821B62C8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B63D8(uint8_t* base, const lifted::Args& a) {
  return f821B63D8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B64E8(uint8_t* base, const lifted::Args& a) {
  return f821B64E8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B65F8(uint8_t* base, const lifted::Args& a) {
  return f821B65F8(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B6708(uint8_t* base, const lifted::Args& a) {
  return f821B6708(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B6818(uint8_t* base, const lifted::Args& a) {
  return f821B6818(base, a.r1, a.r13, a.r[0], a.f[0]);
}

lifted::Result e821B8238(uint8_t* base, const lifted::Args& a) {
  f821B8238(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e821B83A8(uint8_t* base, const lifted::Args& a) {
  return f821B83A8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B84C8(uint8_t* base, const lifted::Args& a) {
  f821B84C8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e821B8588(uint8_t* base, const lifted::Args& a) {
  return f821B8588(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821B8630(uint8_t* base, const lifted::Args& a) {
  return {f821B8630(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821B8688(uint8_t* base, const lifted::Args& a) {
  return {f821B8688(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821B8728(uint8_t* base, const lifted::Args& a) {
  return {f821B8728(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821B87B8(uint8_t* base, const lifted::Args& a) {
  return {f821B87B8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821B8840(uint8_t* base, const lifted::Args& a) {
  return f821B8840(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B8ED0(uint8_t* base, const lifted::Args& a) {
  return f821B8ED0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821B8F60(uint8_t* base, const lifted::Args& a) {
  return f821B8F60(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821B9F98(uint8_t* base, const lifted::Args& a) {
  f821B9F98(base);
  return {a.r[0], a.f[0]};
}

lifted::Result e821BA010(uint8_t* base, const lifted::Args& a) {
  return f821BA010(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821BA0F0(uint8_t* base, const lifted::Args& a) {
  return f821BA0F0(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e821BA5F0(uint8_t* base, const lifted::Args& a) {
  return {f821BA5F0(base, a.r1, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e821BA6A0(uint8_t* base, const lifted::Args& a) {
  return {f821BA6A0(base, a.r1, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e821BA710(uint8_t* base, const lifted::Args& a) {
  return {f821BA710(base, a.r1, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e821BAD80(uint8_t* base, const lifted::Args& a) {
  return {f821BAD80(base, a.r1, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e821BB420(uint8_t* base, const lifted::Args& a) {
  return {f821BB420(base, a.r1, a.r[0]), a.f[0]};
}

lifted::Result e821BB4D0(uint8_t* base, const lifted::Args& a) {
  return {f821BB4D0(base, a.r1), a.f[0]};
}

lifted::Result e821BB5F8(uint8_t* base, const lifted::Args& a) {
  return {f821BB5F8(base, a.r1, a.r[0]), a.f[0]};
}

lifted::Result e821BB678(uint8_t* base, const lifted::Args& a) {
  return {f821BB678(base, a.r1), a.f[0]};
}

lifted::Result e821BD680(uint8_t* base, const lifted::Args& a) {
  return f821BD680(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821BDA38(uint8_t* base, const lifted::Args& a) {
  return f821BDA38(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821BDC18(uint8_t* base, const lifted::Args& a) {
  return f821BDC18(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821BDCB0(uint8_t* base, const lifted::Args& a) {
  return f821BDCB0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821BDFA8(uint8_t* base, const lifted::Args& a) {
  return f821BDFA8(base, a.r1, a.r13, a.r[0], a.r[5], a.f[0]);
}

lifted::Result e821BE040(uint8_t* base, const lifted::Args& a) {
  return f821BE040(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821BE4A0(uint8_t* base, const lifted::Args& a) {
  return {f821BE4A0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821BE7A8(uint8_t* base, const lifted::Args& a) {
  return {f821BE7A8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821BEDC8(uint8_t* base, const lifted::Args& a) {
  return {f821BEDC8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821BEE48(uint8_t* base, const lifted::Args& a) {
  return f821BEE48(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821BEED8(uint8_t* base, const lifted::Args& a) {
  return {f821BEED8(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821BEF68(uint8_t* base, const lifted::Args& a) {
  return {f821BEF68(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821BF2B8(uint8_t* base, const lifted::Args& a) {
  return f821BF2B8(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821BF3C0(uint8_t* base, const lifted::Args& a) {
  return f821BF3C0(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e821BF7F8(uint8_t* base, const lifted::Args& a) {
  return f821BF7F8(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821BF908(uint8_t* base, const lifted::Args& a) {
  return f821BF908(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e821BFBD8(uint8_t* base, const lifted::Args& a) {
  return {f821BFBD8(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821BFC68(uint8_t* base, const lifted::Args& a) {
  return f821BFC68(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e821BFD08(uint8_t* base, const lifted::Args& a) {
  return {f821BFD08(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821BFD98(uint8_t* base, const lifted::Args& a) {
  return f821BFD98(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e821BFF20(uint8_t* base, const lifted::Args& a) {
  return f821BFF20(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e821BFFA0(uint8_t* base, const lifted::Args& a) {
  return {f821BFFA0(base, a.r1, a.r13, a.r[0]), a.f[0]};
}

lifted::Result e821C0030(uint8_t* base, const lifted::Args& a) {
  return f821C0030(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e821C01B8(uint8_t* base, const lifted::Args& a) {
  return f821C01B8(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e821C02B8(uint8_t* base, const lifted::Args& a) {
  return f821C02B8(base, a.r1, a.r13, a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821C0348(uint8_t* base, const lifted::Args& a) {
  return {f821C0348(base, a.r1, a.r13), a.f[0]};
}

lifted::Result e821C03D8(uint8_t* base, const lifted::Args& a) {
  return f821C03D8(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821C04E0(uint8_t* base, const lifted::Args& a) {
  return f821C04E0(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e821C0640(uint8_t* base, const lifted::Args& a) {
  return f821C0640(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821C0750(uint8_t* base, const lifted::Args& a) {
  return f821C0750(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e821C0EF0(uint8_t* base, const lifted::Args& a) {
  return f821C0EF0(base, a.r1, a.r13, a.r[4], a.f[0]);
}

lifted::Result e821C1AA0(uint8_t* base, const lifted::Args& a) {
  return f821C1AA0(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821C1ED8(uint8_t* base, const lifted::Args& a) {
  return f821C1ED8(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821C36F0(uint8_t* base, const lifted::Args& a) {
  return f821C36F0(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821C3910(uint8_t* base, const lifted::Args& a) {
  return f821C3910(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821C3A10(uint8_t* base, const lifted::Args& a) {
  return f821C3A10(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821C5350(uint8_t* base, const lifted::Args& a) {
  return f821C5350(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821C56B0(uint8_t* base, const lifted::Args& a) {
  return f821C56B0(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821C5900(uint8_t* base, const lifted::Args& a) {
  return f821C5900(base, a.r1, a.r13, a.r[3], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e821C5A20(uint8_t* base, const lifted::Args& a) {
  return f821C5A20(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821C5B70(uint8_t* base, const lifted::Args& a) {
  return f821C5B70(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821C5C70(uint8_t* base, const lifted::Args& a) {
  return f821C5C70(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821C5D68(uint8_t* base, const lifted::Args& a) {
  return f821C5D68(base, a.r1, a.r13, a.r[0], a.r[4], a.f[0]);
}

lifted::Result e821C5F38(uint8_t* base, const lifted::Args& a) {
  return f821C5F38(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821C61B0(uint8_t* base, const lifted::Args& a) {
  return f821C61B0(base, a.r1, a.r13, a.f[0]);
}

lifted::Result e821C6238(uint8_t* base, const lifted::Args& a) {
  return f821C6238(base, a.r1, a.r13, a.r[3], a.r[4], a.f[0]);
}

lifted::Result e821C6350(uint8_t* base, const lifted::Args& a) {
  return f821C6350(base, a.r1, a.r13, a.r[3], a.r[4], a.f[0]);
}

lifted::Result e821C7188(uint8_t* base, const lifted::Args& a) {
  return f821C7188(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821C79B8(uint8_t* base, const lifted::Args& a) {
  return f821C79B8(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821C7BF0(uint8_t* base, const lifted::Args& a) {
  return f821C7BF0(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821C7E00(uint8_t* base, const lifted::Args& a) {
  return f821C7E00(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e821D4540(uint8_t* base, const lifted::Args& a) {
  return f821D4540(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821E9920(uint8_t* base, const lifted::Args& a) {
  return f821E9920(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821E9DD8(uint8_t* base, const lifted::Args& a) {
  return f821E9DD8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821EA898(uint8_t* base, const lifted::Args& a) {
  return f821EA898(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821EB2A0(uint8_t* base, const lifted::Args& a) {
  return f821EB2A0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821EBA00(uint8_t* base, const lifted::Args& a) {
  return f821EBA00(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821EC038(uint8_t* base, const lifted::Args& a) {
  return f821EC038(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821EC7E0(uint8_t* base, const lifted::Args& a) {
  return f821EC7E0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821ED108(uint8_t* base, const lifted::Args& a) {
  return f821ED108(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821EDCA8(uint8_t* base, const lifted::Args& a) {
  return f821EDCA8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821EF320(uint8_t* base, const lifted::Args& a) {
  return f821EF320(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821EF5E8(uint8_t* base, const lifted::Args& a) {
  return f821EF5E8(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821EFA78(uint8_t* base, const lifted::Args& a) {
  return f821EFA78(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e821F0318(uint8_t* base, const lifted::Args& a) {
  return f821F0318(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

const Registration kEntries[] = {
    {0x821B5BF0, e821B5BF0},
    {0x821B5CE8, e821B5CE8},
    {0x821B5DF8, e821B5DF8},
    {0x821B5F08, e821B5F08},
    {0x821B5FF8, e821B5FF8},
    {0x821B60E8, e821B60E8},
    {0x821B61D8, e821B61D8},
    {0x821B62C8, e821B62C8},
    {0x821B63D8, e821B63D8},
    {0x821B64E8, e821B64E8},
    {0x821B65F8, e821B65F8},
    {0x821B6708, e821B6708},
    {0x821B6818, e821B6818},
    {0x821B8238, e821B8238},
    {0x821B83A8, e821B83A8},
    {0x821B84C8, e821B84C8},
    {0x821B8588, e821B8588},
    {0x821B8630, e821B8630},
    {0x821B8688, e821B8688},
    {0x821B8728, e821B8728},
    {0x821B87B8, e821B87B8},
    {0x821B8840, e821B8840},
    {0x821B8ED0, e821B8ED0},
    {0x821B8F60, e821B8F60},
    {0x821B9F98, e821B9F98},
    {0x821BA010, e821BA010},
    {0x821BA0F0, e821BA0F0},
    {0x821BA5F0, e821BA5F0},
    {0x821BA6A0, e821BA6A0},
    {0x821BA710, e821BA710},
    {0x821BAD80, e821BAD80},
    {0x821BB420, e821BB420},
    {0x821BB4D0, e821BB4D0},
    {0x821BB5F8, e821BB5F8},
    {0x821BB678, e821BB678},
    {0x821BD680, e821BD680},
    {0x821BDA38, e821BDA38},
    {0x821BDC18, e821BDC18},
    {0x821BDCB0, e821BDCB0},
    {0x821BDFA8, e821BDFA8},
    {0x821BE040, e821BE040},
    {0x821BE4A0, e821BE4A0},
    {0x821BE7A8, e821BE7A8},
    {0x821BEDC8, e821BEDC8},
    {0x821BEE48, e821BEE48},
    {0x821BEED8, e821BEED8},
    {0x821BEF68, e821BEF68},
    {0x821BF2B8, e821BF2B8},
    {0x821BF3C0, e821BF3C0},
    {0x821BF7F8, e821BF7F8},
    {0x821BF908, e821BF908},
    {0x821BFBD8, e821BFBD8},
    {0x821BFC68, e821BFC68},
    {0x821BFD08, e821BFD08},
    {0x821BFD98, e821BFD98},
    {0x821BFF20, e821BFF20},
    {0x821BFFA0, e821BFFA0},
    {0x821C0030, e821C0030},
    {0x821C01B8, e821C01B8},
    {0x821C02B8, e821C02B8},
    {0x821C0348, e821C0348},
    {0x821C03D8, e821C03D8},
    {0x821C04E0, e821C04E0},
    {0x821C0640, e821C0640},
    {0x821C0750, e821C0750},
    {0x821C0EF0, e821C0EF0},
    {0x821C1AA0, e821C1AA0},
    {0x821C1ED8, e821C1ED8},
    {0x821C36F0, e821C36F0},
    {0x821C3910, e821C3910},
    {0x821C3A10, e821C3A10},
    {0x821C5350, e821C5350},
    {0x821C56B0, e821C56B0},
    {0x821C5900, e821C5900},
    {0x821C5A20, e821C5A20},
    {0x821C5B70, e821C5B70},
    {0x821C5C70, e821C5C70},
    {0x821C5D68, e821C5D68},
    {0x821C5F38, e821C5F38},
    {0x821C61B0, e821C61B0},
    {0x821C6238, e821C6238},
    {0x821C6350, e821C6350},
    {0x821C7188, e821C7188},
    {0x821C79B8, e821C79B8},
    {0x821C7BF0, e821C7BF0},
    {0x821C7E00, e821C7E00},
    {0x821D4540, e821D4540},
    {0x821E9920, e821E9920},
    {0x821E9DD8, e821E9DD8},
    {0x821EA898, e821EA898},
    {0x821EB2A0, e821EB2A0},
    {0x821EBA00, e821EBA00},
    {0x821EC038, e821EC038},
    {0x821EC7E0, e821EC7E0},
    {0x821ED108, e821ED108},
    {0x821EDCA8, e821EDCA8},
    {0x821EF320, e821EF320},
    {0x821EF5E8, e821EF5E8},
    {0x821EFA78, e821EFA78},
    {0x821F0318, e821F0318},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_011)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_821B5BF0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B5BF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B5BF0);
}

extern "C" REX_FUNC(sub_821B5CE8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B5CE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B5CE8);
}

extern "C" REX_FUNC(sub_821B5DF8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B5DF8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B5DF8);
}

extern "C" REX_FUNC(sub_821B5F08) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B5F08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B5F08);
}

extern "C" REX_FUNC(sub_821B5FF8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B5FF8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B5FF8);
}

extern "C" REX_FUNC(sub_821B60E8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B60E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B60E8);
}

extern "C" REX_FUNC(sub_821B61D8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B61D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B61D8);
}

extern "C" REX_FUNC(sub_821B62C8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B62C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B62C8);
}

extern "C" REX_FUNC(sub_821B63D8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B63D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B63D8);
}

extern "C" REX_FUNC(sub_821B64E8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B64E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B64E8);
}

extern "C" REX_FUNC(sub_821B65F8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B65F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B65F8);
}

extern "C" REX_FUNC(sub_821B6708) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B6708(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B6708);
}

extern "C" REX_FUNC(sub_821B6818) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B6818(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B6818);
}

extern "C" REX_FUNC(sub_821B8238) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B8238(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B8238);
}

extern "C" REX_FUNC(sub_821B83A8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B83A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B83A8);
}

extern "C" REX_FUNC(sub_821B84C8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B84C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B84C8);
}

extern "C" REX_FUNC(sub_821B8588) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B8588(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B8588);
}

extern "C" REX_FUNC(sub_821B8630) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B8630(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B8630);
}

extern "C" REX_FUNC(sub_821B8688) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B8688(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B8688);
}

extern "C" REX_FUNC(sub_821B8728) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B8728(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B8728);
}

extern "C" REX_FUNC(sub_821B87B8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B87B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B87B8);
}

extern "C" REX_FUNC(sub_821B8840) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B8840(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B8840);
}

extern "C" REX_FUNC(sub_821B8ED0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B8ED0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B8ED0);
}

extern "C" REX_FUNC(sub_821B8F60) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B8F60(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B8F60);
}

extern "C" REX_FUNC(sub_821B9F98) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821B9F98(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821B9F98);
}

extern "C" REX_FUNC(sub_821BA010) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BA010(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BA010);
}

extern "C" REX_FUNC(sub_821BA0F0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BA0F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BA0F0);
}

extern "C" REX_FUNC(sub_821BA5F0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BA5F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BA5F0);
}

extern "C" REX_FUNC(sub_821BA6A0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BA6A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BA6A0);
}

extern "C" REX_FUNC(sub_821BA710) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BA710(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BA710);
}

extern "C" REX_FUNC(sub_821BAD80) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BAD80(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BAD80);
}

extern "C" REX_FUNC(sub_821BB420) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BB420(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BB420);
}

extern "C" REX_FUNC(sub_821BB4D0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BB4D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BB4D0);
}

extern "C" REX_FUNC(sub_821BB5F8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BB5F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BB5F8);
}

extern "C" REX_FUNC(sub_821BB678) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BB678(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BB678);
}

extern "C" REX_FUNC(sub_821BD680) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BD680(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BD680);
}

extern "C" REX_FUNC(sub_821BDA38) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BDA38(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BDA38);
}

extern "C" REX_FUNC(sub_821BDC18) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BDC18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BDC18);
}

extern "C" REX_FUNC(sub_821BDCB0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BDCB0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BDCB0);
}

extern "C" REX_FUNC(sub_821BDFA8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BDFA8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BDFA8);
}

extern "C" REX_FUNC(sub_821BE040) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BE040(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BE040);
}

extern "C" REX_FUNC(sub_821BE4A0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BE4A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BE4A0);
}

extern "C" REX_FUNC(sub_821BE7A8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BE7A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BE7A8);
}

extern "C" REX_FUNC(sub_821BEDC8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BEDC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BEDC8);
}

extern "C" REX_FUNC(sub_821BEE48) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BEE48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BEE48);
}

extern "C" REX_FUNC(sub_821BEED8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BEED8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BEED8);
}

extern "C" REX_FUNC(sub_821BEF68) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BEF68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BEF68);
}

extern "C" REX_FUNC(sub_821BF2B8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BF2B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BF2B8);
}

extern "C" REX_FUNC(sub_821BF3C0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BF3C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BF3C0);
}

extern "C" REX_FUNC(sub_821BF7F8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BF7F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BF7F8);
}

extern "C" REX_FUNC(sub_821BF908) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BF908(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BF908);
}

extern "C" REX_FUNC(sub_821BFBD8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BFBD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BFBD8);
}

extern "C" REX_FUNC(sub_821BFC68) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BFC68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BFC68);
}

extern "C" REX_FUNC(sub_821BFD08) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BFD08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BFD08);
}

extern "C" REX_FUNC(sub_821BFD98) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BFD98(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BFD98);
}

extern "C" REX_FUNC(sub_821BFF20) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BFF20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BFF20);
}

extern "C" REX_FUNC(sub_821BFFA0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821BFFA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821BFFA0);
}

extern "C" REX_FUNC(sub_821C0030) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C0030(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C0030);
}

extern "C" REX_FUNC(sub_821C01B8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C01B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C01B8);
}

extern "C" REX_FUNC(sub_821C02B8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C02B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C02B8);
}

extern "C" REX_FUNC(sub_821C0348) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C0348(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C0348);
}

extern "C" REX_FUNC(sub_821C03D8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C03D8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C03D8);
}

extern "C" REX_FUNC(sub_821C04E0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C04E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C04E0);
}

extern "C" REX_FUNC(sub_821C0640) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C0640(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C0640);
}

extern "C" REX_FUNC(sub_821C0750) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C0750(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C0750);
}

extern "C" REX_FUNC(sub_821C0EF0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C0EF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C0EF0);
}

extern "C" REX_FUNC(sub_821C1AA0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C1AA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C1AA0);
}

extern "C" REX_FUNC(sub_821C1ED8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C1ED8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C1ED8);
}

extern "C" REX_FUNC(sub_821C36F0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C36F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C36F0);
}

extern "C" REX_FUNC(sub_821C3910) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C3910(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C3910);
}

extern "C" REX_FUNC(sub_821C3A10) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C3A10(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C3A10);
}

extern "C" REX_FUNC(sub_821C5350) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C5350(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C5350);
}

extern "C" REX_FUNC(sub_821C56B0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C56B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C56B0);
}

extern "C" REX_FUNC(sub_821C5900) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C5900(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C5900);
}

extern "C" REX_FUNC(sub_821C5A20) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C5A20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C5A20);
}

extern "C" REX_FUNC(sub_821C5B70) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C5B70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C5B70);
}

extern "C" REX_FUNC(sub_821C5C70) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C5C70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C5C70);
}

extern "C" REX_FUNC(sub_821C5D68) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C5D68(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C5D68);
}

extern "C" REX_FUNC(sub_821C5F38) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C5F38(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C5F38);
}

extern "C" REX_FUNC(sub_821C61B0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C61B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C61B0);
}

extern "C" REX_FUNC(sub_821C6238) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C6238(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C6238);
}

extern "C" REX_FUNC(sub_821C6350) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C6350(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C6350);
}

extern "C" REX_FUNC(sub_821C7188) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C7188(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C7188);
}

extern "C" REX_FUNC(sub_821C79B8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C79B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C79B8);
}

extern "C" REX_FUNC(sub_821C7BF0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C7BF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C7BF0);
}

extern "C" REX_FUNC(sub_821C7E00) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821C7E00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821C7E00);
}

extern "C" REX_FUNC(sub_821D4540) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821D4540(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821D4540);
}

extern "C" REX_FUNC(sub_821E9920) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821E9920(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821E9920);
}

extern "C" REX_FUNC(sub_821E9DD8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821E9DD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821E9DD8);
}

extern "C" REX_FUNC(sub_821EA898) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821EA898(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821EA898);
}

extern "C" REX_FUNC(sub_821EB2A0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821EB2A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821EB2A0);
}

extern "C" REX_FUNC(sub_821EBA00) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821EBA00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821EBA00);
}

extern "C" REX_FUNC(sub_821EC038) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821EC038(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821EC038);
}

extern "C" REX_FUNC(sub_821EC7E0) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821EC7E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821EC7E0);
}

extern "C" REX_FUNC(sub_821ED108) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821ED108(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821ED108);
}

extern "C" REX_FUNC(sub_821EDCA8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821EDCA8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821EDCA8);
}

extern "C" REX_FUNC(sub_821EF320) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821EF320(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821EF320);
}

extern "C" REX_FUNC(sub_821EF5E8) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821EF5E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821EF5E8);
}

extern "C" REX_FUNC(sub_821EFA78) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821EFA78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821EFA78);
}

extern "C" REX_FUNC(sub_821F0318) {
  if (!ITC_FLAG(native_lifted_011)) {
    __imp__sub_821F0318(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e821F0318);
}
