// Lift sweep batch 003 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

ITC_FLAG_BOOL(native_lifted_003, true, "ITC",
              "Run lift sweep batch 003's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register.
#define LIFTED_ARGS \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }

namespace lifted {

// sub_820A70A8: lifted from generated/ by lift.py.
void f820A70A8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A70A8 (lifted)");
  // stw r4,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r4));
  // blr
  return;
}

// sub_820A70B0: lifted from generated/ by lift.py.
lifted::Result f820A70B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820A70B0 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,9416
  r11 = uint64_t(int64_t(int64_t(r11) + 9416));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x820a70dc
  if (cr0.eq) goto loc_820A70DC;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A70DC:
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

// sub_820A70F8: lifted from generated/ by lift.py.
lifted::Result f820A70F8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820A70F8 (lifted)");
  uint64_t r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,9496
  r11 = uint64_t(int64_t(int64_t(r11) + 9496));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // li r3,12
  r3 = uint64_t(int64_t(12));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r10));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // stb r10,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r10));
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
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

// sub_820A7178: lifted from generated/ by lift.py.
uint64_t f820A7178(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820A7178 (lifted)");
  uint64_t r3 = 0;
  // li r3,10
  r3 = uint64_t(int64_t(10));
  // blr
  return r3;
}

// sub_820A7180: lifted from generated/ by lift.py.
lifted::Result f820A7180(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A7180 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,9496
  r11 = uint64_t(int64_t(int64_t(r11) + 9496));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,9416
  r11 = uint64_t(int64_t(int64_t(r11) + 9416));
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
  return {r3, f1};
}

// sub_820A71C8: lifted from generated/ by lift.py.
lifted::Result f820A71C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A71C8 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r30,r4
  r30 = r4;
  // bl 0x820a7180
  // (left out: dead in C++)
  { const lifted::Result o = f820A7180(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a71f8
  if (cr0.eq) goto loc_820A71F8;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A71F8:
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

// sub_820A7608: lifted from generated/ by lift.py.
void f820A7608(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A7608 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x820a7618
  if (cr6.eq) goto loc_820A7618;
  // cmplwi cr6,r4,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 1, so);
  // bnelr cr6
  if (!cr6.eq) return;
loc_820A7618:
  // stw r4,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r4));
  // blr
  return;
}

// sub_820A7620: lifted from generated/ by lift.py.
void f820A7620(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A7620 (lifted)");
  // stw r4,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r4));
  // blr
  return;
}

// sub_820A7628: lifted from generated/ by lift.py.
void f820A7628(uint8_t* base, uint64_t r1, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A7628 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // addi r11,r1,-32
  r11 = uint64_t(int64_t(int64_t(r1) + -32));
  // lfs f13,2608(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2608);
  f13 = double(temp.f32);
  // stfs f0,-32(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -32, temp.u32);
  // stfs f0,-28(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -28, temp.u32);
  // stfs f0,-24(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -24, temp.u32);
  // stfs f13,-20(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + -20, temp.u32);
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // stfs f0,-16(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -16, temp.u32);
  // stfs f0,-12(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -12, temp.u32);
  // stfs f0,-8(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -8, temp.u32);
  // stfs f13,-4(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + -4, temp.u32);
  // ld r8,0(r10)
  r8 = REX_LOAD_U64(uint32_t(r10) + 0);
  // ld r10,8(r10)
  r10 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r9,272(r4)
  REX_STORE_U64(uint32_t(r4) + 272, r9);
  // std r11,280(r4)
  REX_STORE_U64(uint32_t(r4) + 280, r11);
  // std r8,288(r4)
  REX_STORE_U64(uint32_t(r4) + 288, r8);
  // std r10,296(r4)
  REX_STORE_U64(uint32_t(r4) + 296, r10);
  // blr
  return;
}

// sub_820A7688: lifted from generated/ by lift.py.
void f820A7688(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A7688 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f0 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // addi r10,r1,-16
  r10 = uint64_t(int64_t(int64_t(r1) + -16));
  // lfs f13,2608(r9)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 2608);
  f13 = double(temp.f32);
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // addi r11,r1,-32
  r11 = uint64_t(int64_t(int64_t(r1) + -32));
  // addi r9,r9,9688
  r9 = uint64_t(int64_t(int64_t(r9) + 9688));
  // stw r8,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r8));
  // lis r8,-32256
  r8 = uint64_t(int64_t(-2113929216));
  // stfs f13,-20(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + -20, temp.u32);
  // stfs f13,-4(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + -4, temp.u32);
  // stfs f0,-32(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -32, temp.u32);
  // stfs f0,-28(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -28, temp.u32);
  // stw r9,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r9));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // stfs f13,28(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 28, temp.u32);
  // stfs f13,44(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 44, temp.u32);
  // stw r4,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r4));
  // stfs f0,-24(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -24, temp.u32);
  // stfs f0,-16(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -16, temp.u32);
  // lfs f13,9576(r9)
  temp.u32 = REX_LOAD_U32(uint32_t(r9) + 9576);
  f13 = double(temp.f32);
  // ld r9,0(r11)
  r9 = REX_LOAD_U64(uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // stfs f0,-12(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -12, temp.u32);
  // stfs f0,-8(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -8, temp.u32);
  // lfs f0,9596(r8)
  temp.u32 = REX_LOAD_U32(uint32_t(r8) + 9596);
  f0 = double(temp.f32);
  // ld r8,0(r10)
  r8 = REX_LOAD_U64(uint32_t(r10) + 0);
  // ld r10,8(r10)
  r10 = REX_LOAD_U64(uint32_t(r10) + 8);
  // std r9,16(r3)
  REX_STORE_U64(uint32_t(r3) + 16, r9);
  // std r11,24(r3)
  REX_STORE_U64(uint32_t(r3) + 24, r11);
  // std r8,32(r3)
  REX_STORE_U64(uint32_t(r3) + 32, r8);
  // std r10,40(r3)
  REX_STORE_U64(uint32_t(r3) + 40, r10);
  // stfs f0,48(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 48, temp.u32);
  // stfs f13,52(r3)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r3) + 52, temp.u32);
  // blr
  return;
}

// sub_820A7AF8: lifted from generated/ by lift.py.
lifted::Result f820A7AF8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A7AF8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,9768
  r11 = uint64_t(int64_t(int64_t(r11) + 9768));
  // li r29,0
  r29 = uint64_t(int64_t(0));
  // li r3,20
  r3 = uint64_t(int64_t(20));
  // mr r30,r4
  r30 = r4;
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r29,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r29));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
  // mr r11,r3
  r11 = r3;
  // mr r3,r31
  r3 = r31;
  // lfs f0,2612(r10)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 2612);
  f0 = double(temp.f32);
  // li r10,1
  r10 = uint64_t(int64_t(1));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // stb r10,0(r11)
  REX_STORE_U8(uint32_t(r11) + 0, uint8_t(r10));
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stfs f0,4(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 4, temp.u32);
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stfs f0,8(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 8, temp.u32);
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stb r29,12(r11)
  REX_STORE_U8(uint32_t(r11) + 12, uint8_t(r29));
  // lwz r11,12(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stfs f0,16(r11)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r11) + 16, temp.u32);
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820A7B70: lifted from generated/ by lift.py.
uint64_t f820A7B70(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_820A7B70 (lifted)");
  uint64_t r3 = 0;
  // li r3,11
  r3 = uint64_t(int64_t(11));
  // blr
  return r3;
}

// sub_820A7B78: lifted from generated/ by lift.py.
lifted::Result f820A7B78(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A7B78 (lifted)");
  uint64_t r4 = 0, r5 = 0, r11 = 0, r12 = 0, r31 = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,9768
  r11 = uint64_t(int64_t(int64_t(r11) + 9768));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,9416
  r11 = uint64_t(int64_t(int64_t(r11) + 9416));
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
  return {r3, f1};
}

// sub_820A7BC0: lifted from generated/ by lift.py.
uint64_t f820A7BC0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A7BC0 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,12(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 12);
  // lbz r3,12(r11)
  r3 = REX_LOAD_U8(uint32_t(r11) + 12);
  // blr
  return r3;
}

// sub_820A7BD0: lifted from generated/ by lift.py.
lifted::Result f820A7BD0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A7BD0 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820A7BE0: lifted from generated/ by lift.py.
lifted::Result f820A7BE0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9) {
  ITC_NATIVE_RUNNING("sub_820A7BE0 (lifted)");
  uint64_t r3 = 0, r10 = 0, r11 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,0(r4)
  r10 = REX_LOAD_U32(uint32_t(r4) + 0);
  // mr r3,r4
  r3 = r4;
  // lfs f1,9572(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 9572);
  f1 = double(temp.f32);
  // lwz r11,4(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 4);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820A7C00: lifted from generated/ by lift.py.
lifted::Result f820A7C00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A7C00 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r30,r4
  r30 = r4;
  // bl 0x820a7b78
  // (left out: dead in C++)
  { const lifted::Result o = f820A7B78(base, r1, r13, r3, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a7c30
  if (cr0.eq) goto loc_820A7C30;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A7C30:
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

// sub_820A8050: lifted from generated/ by lift.py.
void f820A8050(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8050 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // slw r11,r11,r4
  r11 = uint8_t(r4) & 0x20 ? 0 : (uint32_t(r11) << (uint8_t(r4) & 0x3F));
  // or r11,r11,r10
  r11 = r11 | r10;
  // stw r11,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r11));
  // blr
  return;
}

// sub_820A8068: lifted from generated/ by lift.py.
uint64_t f820A8068(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8068 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // li r11,1
  r11 = uint64_t(int64_t(1));
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // slw r11,r11,r4
  r11 = uint8_t(r4) & 0x20 ? 0 : (uint32_t(r11) << (uint8_t(r4) & 0x3F));
  // and r11,r11,r10
  r11 = r11 & r10;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // blr
  return r3;
}

// sub_820A8088: lifted from generated/ by lift.py.
void f820A8088(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8088 (lifted)");
  uint64_t r11 = 0;
  // clrlwi r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  // stw r11,324(r3)
  REX_STORE_U32(uint32_t(r3) + 324, uint32_t(r11));
  // blr
  return;
}

// sub_820A8098: lifted from generated/ by lift.py.
void f820A8098(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8098 (lifted)");
  uint64_t r11 = 0;
  // clrlwi r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  // stw r11,328(r3)
  REX_STORE_U32(uint32_t(r3) + 328, uint32_t(r11));
  // blr
  return;
}

// sub_820A80A8: lifted from generated/ by lift.py.
lifted::Result f820A80A8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A80A8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r3,28(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 28);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,44(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_820A80F8: lifted from generated/ by lift.py.
void f820A80F8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A80F8 (lifted)");
  uint64_t r11 = 0;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,340(r3)
  REX_STORE_U32(uint32_t(r3) + 340, uint32_t(r11));
  // blr
  return;
}

// sub_820A8108: lifted from generated/ by lift.py.
lifted::Result f820A8108(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820A8108 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,20(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 20);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x820a8190
  if (cr6.eq) goto loc_820A8190;
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,28(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 28);
  // lfs f0,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stfs f0,536(r10)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r10) + 536, temp.u32);
  // lwz r10,28(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 28);
  // stw r11,540(r10)
  REX_STORE_U32(uint32_t(r10) + 540, uint32_t(r11));
  // lwz r10,28(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 28);
  // stw r11,544(r10)
  REX_STORE_U32(uint32_t(r10) + 544, uint32_t(r11));
  // lwz r3,20(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 20);
  // lwz r4,28(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 28);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,168(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 168);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,172(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 172);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A8190:
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

// sub_820A81A8: lifted from generated/ by lift.py.
void f820A81A8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A81A8 (lifted)");
  lifted::Cr cr6;
  uint8_t so = 0;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x820a81b8
  if (cr6.eq) goto loc_820A81B8;
  // cmplwi cr6,r4,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 1, so);
  // bnelr cr6
  if (!cr6.eq) return;
loc_820A81B8:
  // stw r4,316(r3)
  REX_STORE_U32(uint32_t(r3) + 316, uint32_t(r4));
  // blr
  return;
}

// sub_820A81C0: lifted from generated/ by lift.py.
lifted::Result f820A81C0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A81C0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // cmpwi cr6,r4,4
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 4, so);
  // beq cr6,0x820a8298
  if (cr6.eq) goto loc_820A8298;
  // cmpwi cr6,r4,5
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 5, so);
  // beq cr6,0x820a8270
  if (cr6.eq) goto loc_820A8270;
  // cmpwi cr6,r4,9
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 9, so);
  // beq cr6,0x820a824c
  if (cr6.eq) goto loc_820A824C;
  // cmpwi cr6,r4,10
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 10, so);
  // beq cr6,0x820a8224
  if (cr6.eq) goto loc_820A8224;
  // cmpwi cr6,r4,11
  cr6 = lifted::Compare<int32_t>(int32_t(r4), 11, so);
  // bne cr6,0x820a8268
  if (!cr6.eq) goto loc_820A8268;
  // li r3,16
  r3 = uint64_t(int64_t(16));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a8268
  if (cr0.eq) goto loc_820A8268;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // addi r4,r31,12
  r4 = uint64_t(int64_t(int64_t(r31) + 12));
  // bne cr6,0x820a821c
  if (!cr6.eq) goto loc_820A821C;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_820A821C:
  // bl 0x820a7af8
  // (left out: dead in C++)
  { const lifted::Result o = f820A7AF8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x820a82d4
  goto loc_820A82D4;
loc_820A8224:
  // li r3,16
  r3 = uint64_t(int64_t(16));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a8268
  if (cr0.eq) goto loc_820A8268;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // addi r4,r31,8
  r4 = uint64_t(int64_t(int64_t(r31) + 8));
  // bne cr6,0x820a8244
  if (!cr6.eq) goto loc_820A8244;
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_820A8244:
  // bl 0x820a70f8
  // (left out: dead in C++)
  { const lifted::Result o = f820A70F8(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x820a82d4
  goto loc_820A82D4;
loc_820A824C:
  // li r3,64
  r3 = uint64_t(int64_t(64));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a8268
  if (cr0.eq) goto loc_820A8268;
  // mr r4,r31
  r4 = r31;
  // bl 0x820a7688
  // (left out: dead in C++)
  f820A7688(base, r1, r3, r4);
  // b 0x820a82d4
  goto loc_820A82D4;
loc_820A8268:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x820a82d4
  goto loc_820A82D4;
loc_820A8270:
  // li r3,16
  r3 = uint64_t(int64_t(16));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a828c
  if (cr0.eq) goto loc_820A828C;
  // bl 0x820a75e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A75E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // b 0x820a8290
  goto loc_820A8290;
loc_820A828C:
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_820A8290:
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // b 0x820a82bc
  goto loc_820A82BC;
loc_820A8298:
  // li r3,16
  r3 = uint64_t(int64_t(16));
  // bl 0x825a6880
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A6880, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a82b4
  if (cr0.eq) goto loc_820A82B4;
  // bl 0x820a75e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A75E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r31,r3
  r31 = r3;
  // b 0x820a82b8
  goto loc_820A82B8;
loc_820A82B4:
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_820A82B8:
  // li r4,0
  r4 = uint64_t(int64_t(0));
loc_820A82BC:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,80(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 80);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r3,r31
  r3 = r31;
loc_820A82D4:
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

// sub_820A82E8: lifted from generated/ by lift.py.
lifted::Result f820A82E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A82E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,160(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 160);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,32(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 32);
  // mr r4,r3
  r4 = r3;
  // mr r3,r11
  r3 = r11;
  // lwz r10,0(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 0);
  // lwz r11,4(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_820A8340: lifted from generated/ by lift.py.
lifted::Result f820A8340(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A8340 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a8394
  if (cr0.eq) goto loc_820A8394;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cmpw cr6,r3,r30
  cr6 = lifted::Compare<int32_t>(int32_t(r3), int32_t(r30), so);
  // bne cr6,0x820a8394
  if (!cr6.eq) goto loc_820A8394;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // b 0x820a83ac
  goto loc_820A83AC;
loc_820A8394:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,48(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 48);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A83AC:
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

// sub_820A83C8: lifted from generated/ by lift.py.
lifted::Result f820A83C8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A83C8 (lifted)");
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
  // lwz r3,32(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 32);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // cntlzw r11,r3
  r11 = uint32_t(r3) == 0 ? 32 : __builtin_clz(uint32_t(r3));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820A8408: lifted from generated/ by lift.py.
lifted::Result f820A8408(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A8408 (lifted)");
  uint64_t r11 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,32(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 32);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820A8420: lifted from generated/ by lift.py.
void f820A8420(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6) {
  ITC_NATIVE_RUNNING("sub_820A8420 (lifted)");
  uint64_t r11 = 0;
  // clrlwi r11,r4,24
  r11 = uint32_t(r4) & 0xFF;
  // stw r5,332(r3)
  REX_STORE_U32(uint32_t(r3) + 332, uint32_t(r5));
  // stw r6,336(r3)
  REX_STORE_U32(uint32_t(r3) + 336, uint32_t(r6));
  // stw r11,320(r3)
  REX_STORE_U32(uint32_t(r3) + 320, uint32_t(r11));
  // blr
  return;
}

// sub_820A8438: lifted from generated/ by lift.py.
double f820A8438(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A8438 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,340(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 340);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x820a8450
  if (cr6.eq) goto loc_820A8450;
  // lwz r11,28(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lfs f1,696(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 696);
  f1 = double(temp.f32);
  // blr
  return f1;
loc_820A8450:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_820A8460: lifted from generated/ by lift.py.
uint64_t f820A8460(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A8460 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // lwz r11,340(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 340);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x820a8484
  if (cr6.eq) goto loc_820A8484;
  // lwz r11,28(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lwz r11,736(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 736);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // blr
  return r3;
loc_820A8484:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_820A8490: lifted from generated/ by lift.py.
void f820A8490(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8490 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,340(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 340);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beqlr cr6
  if (cr6.eq) return;
  // lwz r11,28(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lfs f0,720(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 720);
  f0 = double(temp.f32);
  // stfs f0,0(r4)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 0, temp.u32);
  // lfs f0,724(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 724);
  f0 = double(temp.f32);
  // stfs f0,4(r4)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 4, temp.u32);
  // lfs f0,728(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 728);
  f0 = double(temp.f32);
  // stfs f0,8(r4)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r4) + 8, temp.u32);
  // blr
  return;
}

// sub_820A84C0: lifted from generated/ by lift.py.
double f820A84C0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A84C0 (lifted)");
  uint64_t r11 = 0;
  double f1 = 0.0;
  lifted::Cr cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  // lwz r11,340(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 340);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x820a84d8
  if (cr6.eq) goto loc_820A84D8;
  // lwz r11,28(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 28);
  // lfs f1,732(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 732);
  f1 = double(temp.f32);
  // blr
  return f1;
loc_820A84D8:
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f1,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_820A84E8: lifted from generated/ by lift.py.
uint64_t f820A84E8(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A84E8 (lifted)");
  uint64_t r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // mr r11,r3
  r11 = r3;
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x820a8500
  if (cr6.eq) goto loc_820A8500;
  // li r3,1
  r3 = uint64_t(int64_t(1));
  // stw r4,20(r11)
  REX_STORE_U32(uint32_t(r11) + 20, uint32_t(r4));
  // blr
  return r3;
loc_820A8500:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
}

// sub_820A8508: lifted from generated/ by lift.py.
lifted::Result f820A8508(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A8508 (lifted)");
  uint64_t r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // mr r31,r4
  r31 = r4;
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // lwz r30,28(r29)
  r30 = REX_LOAD_U32(uint32_t(r29) + 28);
  // beq cr6,0x820a865c
  if (cr6.eq) goto loc_820A865C;
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r11,132(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 132);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a865c
  if (cr0.eq) goto loc_820A865C;
  // lwz r3,16(r29)
  r3 = REX_LOAD_U32(uint32_t(r29) + 16);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r3
  r4 = r3;
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r4,r30
  r4 = r30;
  // mr r3,r31
  r3 = r31;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,52(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 52);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a8614
  if (cr0.eq) goto loc_820A8614;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,68(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 68);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A8614:
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r31
  r3 = r31;
  // lwz r11,64(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 64);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a865c
  if (cr0.eq) goto loc_820A865C;
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // mr r3,r29
  r3 = r29;
  // lwz r11,56(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 56);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A865C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820A8690: lifted from generated/ by lift.py.
lifted::Result f820A8690(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A8690 (lifted)");
  uint64_t r4 = 0, r11 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r11,r3
  r11 = r3;
  // lwz r3,20(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 20);
  // lwz r4,28(r11)
  r4 = REX_LOAD_U32(uint32_t(r11) + 28);
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820A86B0: lifted from generated/ by lift.py.
lifted::Result f820A86B0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A86B0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // lwz r11,60(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 60);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr. r4,r3
  r4 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r4), 0, so);
  // beq 0x820a86f0
  if (cr0.eq) goto loc_820A86F0;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // mr r3,r31
  r3 = r31;
  // lwz r11,172(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 172);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A86F0:
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

// sub_820A8708: lifted from generated/ by lift.py.
lifted::Result f820A8708(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A8708 (lifted)");
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
  // lwz r11,0(r5)
  r11 = REX_LOAD_U32(uint32_t(r5) + 0);
  // mr r31,r4
  r31 = r4;
  // cmpwi cr6,r11,6
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 6, so);
  // bne cr6,0x820a8784
  if (!cr6.eq) goto loc_820A8784;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,32(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 32);
  // stw r30,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r30));
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r30,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r30));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,28(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 28);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // stw r30,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r30));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r31
  r3 = r31;
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A8784:
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

// sub_820A87A0: lifted from generated/ by lift.py.
lifted::Result f820A87A0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A87A0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a87dc
  if (cr0.eq) goto loc_820A87DC;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,16(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_820A87DC:
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

// sub_820A87F0: lifted from generated/ by lift.py.
lifted::Result f820A87F0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A87F0 (lifted)");
  uint64_t r4 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // mr r31,r3
  r31 = r3;
  // lwz r3,0(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a882c
  if (cr0.eq) goto loc_820A882C;
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
loc_820A882C:
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

// sub_820A8840: lifted from generated/ by lift.py.
lifted::Result f820A8840(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A8840 (lifted)");
  uint64_t r11 = 0, r12 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r12,0(r3)
  r12 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,184(r12)
  r11 = REX_LOAD_U32(uint32_t(r12) + 184);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820A8850: lifted from generated/ by lift.py.
lifted::Result f820A8850(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A8850 (lifted)");
  uint64_t r11 = 0, r12 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r12,0(r3)
  r12 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,188(r12)
  r11 = REX_LOAD_U32(uint32_t(r12) + 188);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820A8860: lifted from generated/ by lift.py.
lifted::Result f820A8860(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A8860 (lifted)");
  uint64_t r11 = 0, r12 = 0, ctr = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r12,0(r3)
  r12 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,180(r12)
  r11 = REX_LOAD_U32(uint32_t(r12) + 180);
  // mtctr r11
  ctr = r11;
  // bctr
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820A8870: lifted from generated/ by lift.py.
lifted::Result f820A8870(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8) {
  ITC_NATIVE_RUNNING("sub_820A8870 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0, ctr = 0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r30,r31,32
  r30 = uint64_t(int64_t(int64_t(r31) + 32));
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // lis r9,-32256
  r9 = uint64_t(int64_t(-2113929216));
  // addi r11,r11,9912
  r11 = uint64_t(int64_t(int64_t(r11) + 9912));
  // addi r10,r10,9896
  r10 = uint64_t(int64_t(int64_t(r10) + 9896));
  // addi r9,r9,9884
  r9 = uint64_t(int64_t(int64_t(r9) + 9884));
  // lwz r3,0(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 0);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // stw r10,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r10));
  // stw r9,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r9));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // addi r3,r31,28
  r3 = uint64_t(int64_t(int64_t(r31) + 28));
  // stw r11,340(r31)
  REX_STORE_U32(uint32_t(r31) + 340, uint32_t(r11));
  // bl 0x820a87a0
  // (left out: dead in C++)
  { const lifted::Result o = f820A87A0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x820a87f0
  // (left out: dead in C++)
  { const lifted::Result o = f820A87F0(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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

// sub_820A88F8: lifted from generated/ by lift.py.
uint64_t f820A88F8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A88F8 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,340(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 340);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // blr
  return r3;
}

// sub_820A8910: lifted from generated/ by lift.py.
void f820A8910(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8910 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // ori r11,r11,4
  r11 = r11 | 4;
  // stw r11,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r11));
  // lfs f0,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // stfs f0,80(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 80, temp.u32);
  // blr
  return;
}

// sub_820A8928: lifted from generated/ by lift.py.
void f820A8928(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8928 (lifted)");
  uint64_t r11 = 0;
  double f0 = 0.0;
  lifted::Temp temp{};
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // ori r11,r11,8
  r11 = r11 | 8;
  // stw r11,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r11));
  // lfs f0,0(r4)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r4) + 0);
  f0 = double(temp.f32);
  // stfs f0,84(r3)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r3) + 84, temp.u32);
  // blr
  return;
}

// sub_820A8940: lifted from generated/ by lift.py.
void f820A8940(uint8_t* base, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_820A8940 (lifted)");
  lifted::Temp temp{};
  // stfs f1,228(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 228, temp.u32);
  // blr
  return;
}

// sub_820A8948: lifted from generated/ by lift.py.
void f820A8948(uint8_t* base, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_820A8948 (lifted)");
  lifted::Temp temp{};
  // stfs f1,304(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 304, temp.u32);
  // blr
  return;
}

// sub_820A8950: lifted from generated/ by lift.py.
void f820A8950(uint8_t* base, uint64_t r3, double f1) {
  ITC_NATIVE_RUNNING("sub_820A8950 (lifted)");
  lifted::Temp temp{};
  // stfs f1,308(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f1);
  REX_STORE_U32(uint32_t(r3) + 308, temp.u32);
  // blr
  return;
}

// sub_820A8960: lifted from generated/ by lift.py.
uint64_t f820A8960(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A8960 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,324(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 324);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // blr
  return r3;
}

// sub_820A8978: lifted from generated/ by lift.py.
uint64_t f820A8978(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A8978 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,328(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 328);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // xori r3,r11,1
  r3 = r11 ^ 1;
  // blr
  return r3;
}

// sub_820A8990: lifted from generated/ by lift.py.
double f820A8990(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A8990 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,80(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 80);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_820A8998: lifted from generated/ by lift.py.
double f820A8998(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A8998 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,84(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 84);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_820A89A0: lifted from generated/ by lift.py.
double f820A89A0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A89A0 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,216(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 216);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_820A89A8: lifted from generated/ by lift.py.
double f820A89A8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A89A8 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,220(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 220);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_820A89B0: lifted from generated/ by lift.py.
double f820A89B0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A89B0 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,292(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 292);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_820A89B8: lifted from generated/ by lift.py.
double f820A89B8(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A89B8 (lifted)");
  double f1 = 0.0;
  lifted::Temp temp{};
  // lfs f1,296(r3)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r3) + 296);
  f1 = double(temp.f32);
  // blr
  return f1;
}

// sub_820A89C0: lifted from generated/ by lift.py.
void f820A89C0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A89C0 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // ori r11,r11,1
  r11 = r11 | 1;
  // stw r11,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r11));
  // ld r11,0(r4)
  r11 = REX_LOAD_U64(uint32_t(r4) + 0);
  // std r11,48(r3)
  REX_STORE_U64(uint32_t(r3) + 48, r11);
  // ld r11,8(r4)
  r11 = REX_LOAD_U64(uint32_t(r4) + 8);
  // std r11,56(r3)
  REX_STORE_U64(uint32_t(r3) + 56, r11);
  // blr
  return;
}

// sub_820A89E0: lifted from generated/ by lift.py.
void f820A89E0(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A89E0 (lifted)");
  uint64_t r11 = 0;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // ori r11,r11,2
  r11 = r11 | 2;
  // stw r11,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r11));
  // ld r11,0(r4)
  r11 = REX_LOAD_U64(uint32_t(r4) + 0);
  // std r11,64(r3)
  REX_STORE_U64(uint32_t(r3) + 64, r11);
  // ld r11,8(r4)
  r11 = REX_LOAD_U64(uint32_t(r4) + 8);
  // std r11,72(r3)
  REX_STORE_U64(uint32_t(r3) + 72, r11);
  // blr
  return;
}

// sub_820A8A00: lifted from generated/ by lift.py.
uint64_t f820A8A00(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8A00 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,96
  r3 = uint64_t(int64_t(int64_t(r3) + 96));
  // b 0x820a0ac0
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820A8A08: lifted from generated/ by lift.py.
uint64_t f820A8A08(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8A08 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,160
  r3 = uint64_t(int64_t(int64_t(r3) + 160));
  // b 0x820a0ac0
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820A8A10: lifted from generated/ by lift.py.
uint64_t f820A8A10(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8A10 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,240
  r3 = uint64_t(int64_t(int64_t(r3) + 240));
  // b 0x820a0ac0
  { const lifted::Result o = lifted::Call(base, 0x820A0AC0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820A8A18: lifted from generated/ by lift.py.
void f820A8A18(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8A18 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // addi r11,r4,48
  r11 = uint64_t(int64_t(int64_t(r4) + 48));
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

// sub_820A8A30: lifted from generated/ by lift.py.
void f820A8A30(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8A30 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // addi r11,r4,64
  r11 = uint64_t(int64_t(int64_t(r4) + 64));
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

// sub_820A8A48: lifted from generated/ by lift.py.
uint64_t f820A8A48(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8A48 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r4,r4,88
  r4 = uint64_t(int64_t(int64_t(r4) + 88));
  // li r5,64
  r5 = uint64_t(int64_t(64));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,12(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 12, temp.u32);
  // stfs f0,28(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 28, temp.u32);
  // stfs f0,44(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 44, temp.u32);
  // stfs f0,60(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 60, temp.u32);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_820A8A98: lifted from generated/ by lift.py.
uint64_t f820A8A98(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8A98 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r4,r4,152
  r4 = uint64_t(int64_t(int64_t(r4) + 152));
  // li r5,64
  r5 = uint64_t(int64_t(64));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,12(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 12, temp.u32);
  // stfs f0,28(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 28, temp.u32);
  // stfs f0,44(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 44, temp.u32);
  // stfs f0,60(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 60, temp.u32);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_820A8AE8: lifted from generated/ by lift.py.
uint64_t f820A8AE8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A8AE8 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0;
  double f0 = 0.0, f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r4,r4,228
  r4 = uint64_t(int64_t(int64_t(r4) + 228));
  // li r5,64
  r5 = uint64_t(int64_t(64));
  // lfs f0,2608(r11)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f0 = double(temp.f32);
  // stfs f0,12(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 12, temp.u32);
  // stfs f0,28(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 28, temp.u32);
  // stfs f0,44(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 44, temp.u32);
  // stfs f0,60(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 60, temp.u32);
  // bl 0x825a7410
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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

// sub_820A8B38: lifted from generated/ by lift.py.
lifted::Result f820A8B38(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10) {
  ITC_NATIVE_RUNNING("sub_820A8B38 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // bl 0x820a8870
  // (left out: dead in C++)
  { const lifted::Result o = f820A8870(base, r1, r13, r3, r4, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a8b68
  if (cr0.eq) goto loc_820A8B68;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A8B68:
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

// sub_820A8EA0: lifted from generated/ by lift.py.
uint64_t f820A8EA0(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A8EA0 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne 0x820a8eb8
  if (!cr0.eq) goto loc_820A8EB8;
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // blr
  return r3;
loc_820A8EB8:
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // srawi r3,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r3 = uint64_t(int64_t(int32_t(r11) >> 2));
  // blr
  return r3;
}

// sub_820A8EC8: lifted from generated/ by lift.py.
void f820A8EC8(uint8_t* base, uint64_t r1, uint64_t r3, uint64_t r4, uint64_t r5) {
  ITC_NATIVE_RUNNING("sub_820A8EC8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  lifted::Cr cr6;
  uint8_t so = 0;
  // ld r11,0(r4)
  r11 = REX_LOAD_U64(uint32_t(r4) + 0);
  // std r11,-16(r1)
  REX_STORE_U64(uint32_t(r1) + -16, r11);
  // neg r11,r5
  r11 = uint64_t(int64_t(static_cast<int64_t>(-r5)));
  // lwz r10,-16(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + -16);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x820a8ee4
  if (!cr6.eq) goto loc_820A8EE4;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A8EE4:
  // lwz r9,-12(r1)
  r9 = REX_LOAD_U32(uint32_t(r1) + -12);
  // rlwinm r11,r11,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // lwz r8,8(r10)
  r8 = REX_LOAD_U32(uint32_t(r10) + 8);
  // add r11,r11,r9
  r11 = r11 + r9;
  // cmplw cr6,r11,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r8), so);
  // bgt cr6,0x820a8f08
  if (cr6.gt) goto loc_820A8F08;
  // lwz r10,4(r10)
  r10 = REX_LOAD_U32(uint32_t(r10) + 4);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bge cr6,0x820a8f0c
  if (!cr6.lt) goto loc_820A8F0C;
loc_820A8F08:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A8F0C:
  // stw r11,-12(r1)
  REX_STORE_U32(uint32_t(r1) + -12, uint32_t(r11));
  // ld r11,-16(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + -16);
  // std r11,0(r3)
  REX_STORE_U64(uint32_t(r3) + 0, r11);
  // blr
  return;
}

// sub_820A8F20: lifted from generated/ by lift.py.
lifted::Result f820A8F20(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A8F20 (lifted)");
  uint64_t r6 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r5
  r29 = r5;
  // mr r31,r4
  r31 = r4;
  // mr r30,r3
  r30 = r3;
  // std r29,144(r1)
  REX_STORE_U64(uint32_t(r1) + 144, r29);
  // lwz r3,148(r1)
  r3 = REX_LOAD_U32(uint32_t(r1) + 148);
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // addi r5,r3,4
  r5 = uint64_t(int64_t(int64_t(r3) + 4));
  // subf r11,r5,r11
  r11 = r11 - r5;
  // srawi. r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820a8f60
  if (!cr0.gt) goto loc_820A8F60;
  // rlwinm r6,r11,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r4,r6
  r4 = r6;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7348, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A8F60:
  // lwz r11,8(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 8);
  // mr r3,r30
  r3 = r30;
  // std r29,0(r30)
  REX_STORE_U64(uint32_t(r30) + 0, r29);
  // addi r11,r11,-4
  r11 = uint64_t(int64_t(int64_t(r11) + -4));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820A9010: lifted from generated/ by lift.py.
lifted::Result f820A9010(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A9010 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // mr r30,r4
  r30 = r4;
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a903c
  if (cr0.eq) goto loc_820A903C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A903C:
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // clrlwi. r10,r30,31
  r10 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,4(r31)
  REX_STORE_U32(uint32_t(r31) + 4, uint32_t(r11));
  // stw r11,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r11));
  // stw r11,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r11));
  // beq 0x820a905c
  if (cr0.eq) goto loc_820A905C;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A905C:
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

// sub_820A9078: lifted from generated/ by lift.py.
lifted::Result f820A9078(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_820A9078 (lifted)");
  uint64_t r4 = 0, r5 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x820a917c
  if (cr0.eq) goto loc_820A917C;
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // subf r10,r10,r9
  r10 = r9 - r10;
  // srawi. r10,r10,2
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x3) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 2));
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // beq 0x820a917c
  if (cr0.eq) goto loc_820A917C;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq 0x820a90c4
  if (cr0.eq) goto loc_820A90C4;
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // subf r10,r10,r9
  r10 = r9 - r10;
  // srawi r10,r10,2
  ca = (int32_t(r10) < 0) & ((uint32_t(r10) & 0x3) != 0);
  r10 = uint64_t(int64_t(int32_t(r10) >> 2));
loc_820A90C4:
  // lwz r9,4(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 4);
  // lwz r8,8(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // ble cr6,0x820a90d8
  if (!cr6.gt) goto loc_820A90D8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A90D8:
  // rlwinm r10,r10,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r10) | (r10 << 32), 2) & 0xFFFFFFFC;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // stw r9,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r9));
  // add r10,r10,r9
  r10 = r10 + r9;
  // ld r9,80(r1)
  r9 = REX_LOAD_U64(uint32_t(r1) + 80);
  // lwz r8,8(r11)
  r8 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplw cr6,r10,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r8), so);
  // std r9,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r9);
  // bgt cr6,0x820a9108
  if (cr6.gt) goto loc_820A9108;
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x820a910c
  if (!cr6.lt) goto loc_820A910C;
loc_820A9108:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A910C:
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // addi r4,r1,88
  r4 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // std r11,88(r1)
  REX_STORE_U64(uint32_t(r1) + 88, r11);
  // bl 0x820a8ec8
  // (left out: dead in C++)
  f820A8EC8(base, r1, r3, r4, r5);
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x820a9138
  if (!cr6.eq) goto loc_820A9138;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A9138:
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r11,84(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 84);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // blt cr6,0x820a914c
  if (cr6.lt) goto loc_820A914C;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A914C:
  // lwz r3,0(r11)
  r3 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x820a916c
  if (cr0.eq) goto loc_820A916C;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A916C:
  // addi r3,r1,88
  r3 = uint64_t(int64_t(int64_t(r1) + 88));
  // ld r5,80(r1)
  r5 = REX_LOAD_U64(uint32_t(r1) + 80);
  // lwz r4,4(r31)
  r4 = REX_LOAD_U32(uint32_t(r31) + 4);
  // bl 0x820a8f20
  // (left out: dead in C++)
  { const lifted::Result o = f820A8F20(base, r1, r13, r3, r4, r5, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_820A917C:
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
}

// sub_820A9200: lifted from generated/ by lift.py.
lifted::Result f820A9200(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, double f1) {
  ITC_NATIVE_RUNNING("sub_820A9200 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r9 = 0, r10 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // lwz r3,4(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 4);
  // b 0x820a9198
  { const lifted::Result o = lifted::Call(base, 0x820A9198, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820A9208: lifted from generated/ by lift.py.
lifted::Result f820A9208(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A9208 (lifted)");
  uint64_t r5 = 0, r11 = 0, r12 = 0, r30 = 0, r31 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r30,r4
  r30 = r4;
  // addi r11,r11,10112
  r11 = uint64_t(int64_t(int64_t(r11) + 10112));
  // lwz r3,4(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x820a9244
  if (cr0.eq) goto loc_820A9244;
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x820a9010
  // (left out: dead in C++)
  { const lifted::Result o = f820A9010(base, r1, r13, r3, r4, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_820A9244:
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a9254
  if (cr0.eq) goto loc_820A9254;
  // mr r3,r31
  r3 = r31;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A9254:
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

// sub_820A9270: lifted from generated/ by lift.py.
lifted::Result f820A9270(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A9270 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,8716
  r4 = uint64_t(int64_t(int64_t(r11) + 8716));
  // bl 0x825931d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825931D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
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
  { const lifted::Result o = lifted::Call(base, 0x82091328, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820A92D0: lifted from generated/ by lift.py.
lifted::Result f820A92D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A92D0 (lifted)");
  uint64_t r3 = 0, r4 = 0, r11 = 0, r12 = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-160(r1)
  ea = -160 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r4,r11,3416
  r4 = uint64_t(int64_t(int64_t(r11) + 3416));
  // bl 0x825931d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825931D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
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
  REX_STORE_U32(uint32_t(r1) + 112, uint32_t(r11));
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
  { const lifted::Result o = lifted::Call(base, 0x82091328, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r1,r1,160
  r1 = uint64_t(int64_t(int64_t(r1) + 160));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820A9330: lifted from generated/ by lift.py.
lifted::Result f820A9330(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_820A9330 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r31,r3
  r31 = r3;
  // mr r30,r4
  r30 = r4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x820a936c
  if (cr0.eq) goto loc_820A936C;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  // cmplw cr6,r11,r30
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r30), so);
  // bgt cr6,0x820a9370
  if (cr6.gt) goto loc_820A9370;
loc_820A936C:
  // bl 0x820a9270
  // (left out: dead in C++)
  { const lifted::Result o = f820A9270(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_820A9370:
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x820a9384
  if (!cr6.gt) goto loc_820A9384;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A9384:
  // rlwinm r10,r30,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // stw r31,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r31));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // add r3,r10,r11
  r3 = r10 + r11;
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r3,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r9), so);
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // bgt cr6,0x820a93b4
  if (cr6.gt) goto loc_820A93B4;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // bge cr6,0x820a93b8
  if (!cr6.lt) goto loc_820A93B8;
loc_820A93B4:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A93B8:
  // lwz r11,80(r1)
  r11 = REX_LOAD_U32(uint32_t(r1) + 80);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplw cr6,r3,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), uint32_t(r11), so);
  // blt cr6,0x820a93cc
  if (cr6.lt) goto loc_820A93CC;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A93CC:
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

// sub_820A93E8: lifted from generated/ by lift.py.
lifted::Result f820A93E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_820A93E8 (lifted)");
  uint64_t r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Cr cr0, cr6;
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
  // mr r30,r3
  r30 = r3;
  // lwz r25,0(r6)
  r25 = REX_LOAD_U32(uint32_t(r6) + 0);
  // std r4,168(r1)
  REX_STORE_U64(uint32_t(r1) + 168, r4);
  // mr r27,r5
  r27 = r5;
  // lwz r10,4(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne 0x820a9418
  if (!cr0.eq) goto loc_820A9418;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // b 0x820a9424
  goto loc_820A9424;
loc_820A9418:
  // lwz r11,12(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 12);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // srawi r8,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r8 = uint64_t(int64_t(int32_t(r11) >> 2));
loc_820A9424:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x820a96c4
  if (cr6.eq) goto loc_820A96C4;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x820a943c
  if (!cr6.eq) goto loc_820A943C;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x820a9448
  goto loc_820A9448;
loc_820A943C:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
loc_820A9448:
  // lis r9,16383
  r9 = uint64_t(int64_t(1073676288));
  // ori r9,r9,65535
  r9 = r9 | 65535;
  // subf r11,r11,r9
  r11 = r9 - r11;
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // bge cr6,0x820a9464
  if (!cr6.lt) goto loc_820A9464;
  // bl 0x820a92d0
  // (left out: dead in C++)
  { const lifted::Result o = f820A92D0(base, r1, r13, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // b 0x820a96c4
  goto loc_820A96C4;
loc_820A9464:
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x820a9474
  if (!cr6.eq) goto loc_820A9474;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x820a9480
  goto loc_820A9480;
loc_820A9474:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
loc_820A9480:
  // add r11,r11,r27
  r11 = r11 + r27;
  // cmplw cr6,r8,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r11), so);
  // bge cr6,0x820a95bc
  if (!cr6.lt) goto loc_820A95BC;
  // rlwinm r11,r8,31,1,31
  r11 = __builtin_rotateleft64(uint32_t(r8) | (r8 << 32), 31) & 0x7FFFFFFF;
  // li r26,0
  r26 = uint64_t(int64_t(0));
  // subf r9,r11,r9
  r9 = r9 - r11;
  // cmplw cr6,r9,r8
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r8), so);
  // blt cr6,0x820a94a4
  if (cr6.lt) goto loc_820A94A4;
  // add r26,r11,r8
  r26 = r11 + r8;
loc_820A94A4:
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x820a94b4
  if (!cr6.eq) goto loc_820A94B4;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x820a94c0
  goto loc_820A94C0;
loc_820A94B4:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
loc_820A94C0:
  // add r11,r11,r27
  r11 = r11 + r27;
  // cmplw cr6,r26,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r26), uint32_t(r11), so);
  // bge cr6,0x820a94ec
  if (!cr6.lt) goto loc_820A94EC;
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne cr6,0x820a94dc
  if (!cr6.eq) goto loc_820A94DC;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x820a94e8
  goto loc_820A94E8;
loc_820A94DC:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // subf r11,r10,r11
  r11 = r11 - r10;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
loc_820A94E8:
  // add r26,r11,r27
  r26 = r11 + r27;
loc_820A94EC:
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r26
  r3 = r26;
  // bl 0x82092b78
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x82092B78, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r5,4(r30)
  r5 = REX_LOAD_U32(uint32_t(r30) + 4);
  // lwz r31,172(r1)
  r31 = REX_LOAD_U32(uint32_t(r1) + 172);
  // mr r29,r3
  r29 = r3;
  // subf r11,r5,r31
  r11 = r31 - r5;
  // srawi. r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // rlwinm r6,r11,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r28,r6,r29
  r28 = r6 + r29;
  // beq 0x820a9520
  if (cr0.eq) goto loc_820A9520;
  // mr r4,r6
  r4 = r6;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7348, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A9520:
  // cmplwi cr6,r27,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq cr6,0x820a9544
  if (cr6.eq) goto loc_820A9544;
  // mr r11,r28
  r11 = r28;
  // cmplwi r27,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r27), 0, so);
  // beq 0x820a9544
  if (cr0.eq) goto loc_820A9544;
  // mtctr r27
  ctr = r27;
loc_820A9538:
  // stw r25,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r25));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // bdnz 0x820a9538
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_820A9538;
loc_820A9544:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // subf r11,r31,r11
  r11 = r11 - r31;
  // srawi. r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a956c
  if (cr0.eq) goto loc_820A956C;
  // rlwinm r6,r11,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r27,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // mr r5,r31
  r5 = r31;
  // add r3,r11,r28
  r3 = r11 + r28;
  // mr r4,r6
  r4 = r6;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7348, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A956C:
  // lwz r3,4(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 4);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // bne 0x820a9580
  if (!cr0.eq) goto loc_820A9580;
  // li r11,0
  r11 = uint64_t(int64_t(0));
  // b 0x820a958c
  goto loc_820A958C;
loc_820A9580:
  // lwz r11,8(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 8);
  // subf r11,r3,r11
  r11 = r11 - r3;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
loc_820A958C:
  // add r31,r11,r27
  r31 = r11 + r27;
  // cmplwi cr6,r3,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq cr6,0x820a959c
  if (cr6.eq) goto loc_820A959C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A959C:
  // rlwinm r10,r26,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r26) | (r26 << 32), 2) & 0xFFFFFFFC;
  // stw r29,4(r30)
  REX_STORE_U32(uint32_t(r30) + 4, uint32_t(r29));
  // rlwinm r11,r31,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r31) | (r31 << 32), 2) & 0xFFFFFFFC;
  // add r10,r10,r29
  r10 = r10 + r29;
  // add r11,r11,r29
  r11 = r11 + r29;
  // stw r10,12(r30)
  REX_STORE_U32(uint32_t(r30) + 12, uint32_t(r10));
  // stw r11,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r11));
  // b 0x820a96c4
  goto loc_820A96C4;
loc_820A95BC:
  // lwz r28,8(r30)
  r28 = REX_LOAD_U32(uint32_t(r30) + 8);
  // lwz r31,172(r1)
  r31 = REX_LOAD_U32(uint32_t(r1) + 172);
  // subf r11,r31,r28
  r11 = r28 - r31;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  // cmplw cr6,r11,r27
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r27), so);
  // bge cr6,0x820a9654
  if (!cr6.lt) goto loc_820A9654;
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x820a95f4
  if (cr0.eq) goto loc_820A95F4;
  // rlwinm r6,r11,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // rlwinm r11,r27,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // mr r5,r31
  r5 = r31;
  // add r3,r11,r31
  r3 = r11 + r31;
  // mr r4,r6
  r4 = r6;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7348, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A95F4:
  // lwz r10,8(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 8);
  // subf r11,r31,r10
  r11 = r10 - r31;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  // subf. r11,r11,r27
  r11 = r27 - r11;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a9620
  if (cr0.eq) goto loc_820A9620;
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x820a9620
  if (cr0.eq) goto loc_820A9620;
  // mtctr r11
  ctr = r11;
loc_820A9614:
  // stw r25,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r25));
  // addi r10,r10,4
  r10 = uint64_t(int64_t(int64_t(r10) + 4));
  // bdnz 0x820a9614
  --ctr;
  if (uint32_t(ctr) != 0) goto loc_820A9614;
loc_820A9620:
  // lwz r10,8(r30)
  r10 = REX_LOAD_U32(uint32_t(r30) + 8);
  // rlwinm r11,r27,2,0,29
  r11 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // mr r9,r31
  r9 = r31;
  // add r10,r11,r10
  r10 = r11 + r10;
  // subf r11,r11,r10
  r11 = r10 - r11;
  // cmplw cr6,r31,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r11), so);
  // stw r10,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r10));
  // beq cr6,0x820a96c4
  if (cr6.eq) goto loc_820A96C4;
loc_820A9640:
  // stw r25,0(r9)
  REX_STORE_U32(uint32_t(r9) + 0, uint32_t(r25));
  // addi r9,r9,4
  r9 = uint64_t(int64_t(int64_t(r9) + 4));
  // cmplw cr6,r9,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r9), uint32_t(r11), so);
  // bne cr6,0x820a9640
  if (!cr6.eq) goto loc_820A9640;
  // b 0x820a96c4
  goto loc_820A96C4;
loc_820A9654:
  // rlwinm r26,r27,2,0,29
  r26 = __builtin_rotateleft64(uint32_t(r27) | (r27 << 32), 2) & 0xFFFFFFFC;
  // lwz r3,8(r30)
  r3 = REX_LOAD_U32(uint32_t(r30) + 8);
  // subf r29,r26,r28
  r29 = r28 - r26;
  // subf r11,r29,r28
  r11 = r28 - r29;
  // srawi. r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // rlwinm r6,r11,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // add r27,r6,r3
  r27 = r6 + r3;
  // beq 0x820a9680
  if (cr0.eq) goto loc_820A9680;
  // mr r5,r29
  r5 = r29;
  // mr r4,r6
  r4 = r6;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7348, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A9680:
  // subf r11,r31,r29
  r11 = r29 - r31;
  // stw r27,8(r30)
  REX_STORE_U32(uint32_t(r30) + 8, uint32_t(r27));
  // srawi. r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // ble 0x820a96a4
  if (!cr0.gt) goto loc_820A96A4;
  // rlwinm r6,r11,2,0,29
  r6 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 2) & 0xFFFFFFFC;
  // mr r5,r31
  r5 = r31;
  // mr r4,r6
  r4 = r6;
  // subf r3,r6,r28
  r3 = r28 - r6;
  // bl 0x825a7348
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A7348, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820A96A4:
  // add r10,r26,r31
  r10 = r26 + r31;
  // mr r11,r31
  r11 = r31;
  // cmplw cr6,r31,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), uint32_t(r10), so);
  // beq cr6,0x820a96c4
  if (cr6.eq) goto loc_820A96C4;
loc_820A96B4:
  // stw r25,0(r11)
  REX_STORE_U32(uint32_t(r11) + 0, uint32_t(r25));
  // addi r11,r11,4
  r11 = uint64_t(int64_t(int64_t(r11) + 4));
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x820a96b4
  if (!cr6.eq) goto loc_820A96B4;
loc_820A96C4:
  // addi r1,r1,144
  r1 = uint64_t(int64_t(int64_t(r1) + 144));
  // b 0x825a6ecc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820A96D0: lifted from generated/ by lift.py.
lifted::Result f820A96D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820A96D0 (lifted)");
  uint64_t r4 = 0, r10 = 0, r11 = 0, r12 = 0;
  lifted::Cr cr0;
  uint8_t ca = 0, so = 0;
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // stw r12,-8(r1)
  REX_STORE_U32(uint32_t(r1) + -8, uint32_t(r12));
  // stwu r1,-96(r1)
  ea = -96 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lwz r3,4(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 4);
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x820a9724
  if (cr0.eq) goto loc_820A9724;
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // srawi. r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x820a9724
  if (cr0.eq) goto loc_820A9724;
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x820a9714
  if (cr0.eq) goto loc_820A9714;
  // lwz r10,8(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 8);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // srawi r11,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r11 = uint64_t(int64_t(int32_t(r11) >> 2));
loc_820A9714:
  // addi r4,r11,-1
  r4 = uint64_t(int64_t(int64_t(r11) + -1));
  // bl 0x820a9330
  // (left out: dead in C++)
  { const lifted::Result o = f820A9330(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r3,0(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 0);
  // b 0x820a9728
  goto loc_820A9728;
loc_820A9724:
  // li r3,0
  r3 = uint64_t(int64_t(0));
loc_820A9728:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820A9738: lifted from generated/ by lift.py.
lifted::Result f820A9738(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_820A9738 (lifted)");
  uint64_t r9 = 0, r10 = 0, r11 = 0, r12 = 0, r29 = 0, r30 = 0, r31 = 0;
  lifted::Cr cr0, cr6;
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
  // mr r31,r4
  r31 = r4;
  // mr r4,r5
  r4 = r5;
  // mr r29,r3
  r29 = r3;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // std r4,160(r1)
  REX_STORE_U64(uint32_t(r1) + 160, r4);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x820a9770
  if (cr0.eq) goto loc_820A9770;
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // subf r9,r11,r10
  r9 = r10 - r11;
  // srawi. r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  cr0 = lifted::Compare<int32_t>(int32_t(r9), 0, so);
  // bne 0x820a9778
  if (!cr0.eq) goto loc_820A9778;
loc_820A9770:
  // li r30,0
  r30 = uint64_t(int64_t(0));
  // b 0x820a97a8
  goto loc_820A97A8;
loc_820A9778:
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x820a9784
  if (!cr6.gt) goto loc_820A9784;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A9784:
  // lwz r10,160(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 160);
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x820a9798
  if (cr6.eq) goto loc_820A9798;
  // cmplw cr6,r10,r31
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r31), so);
  // beq cr6,0x820a979c
  if (cr6.eq) goto loc_820A979C;
loc_820A9798:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A979C:
  // lwz r10,164(r1)
  r10 = REX_LOAD_U32(uint32_t(r1) + 164);
  // subf r11,r11,r10
  r11 = r10 - r11;
  // srawi r30,r11,2
  ca = (int32_t(r11) < 0) & ((uint32_t(r11) & 0x3) != 0);
  r30 = uint64_t(int64_t(int32_t(r11) >> 2));
loc_820A97A8:
  // li r5,1
  r5 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // bl 0x820a93e8
  // (left out: dead in C++)
  { const lifted::Result o = f820A93E8(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // lwz r10,8(r31)
  r10 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // ble cr6,0x820a97c8
  if (!cr6.gt) goto loc_820A97C8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A97C8:
  // rlwinm r10,r30,2,0,29
  r10 = __builtin_rotateleft64(uint32_t(r30) | (r30 << 32), 2) & 0xFFFFFFFC;
  // stw r31,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r31));
  // stw r11,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r11));
  // add r10,r10,r11
  r10 = r10 + r11;
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // lwz r9,8(r31)
  r9 = REX_LOAD_U32(uint32_t(r31) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // std r11,80(r1)
  REX_STORE_U64(uint32_t(r1) + 80, r11);
  // bgt cr6,0x820a97f8
  if (cr6.gt) goto loc_820A97F8;
  // lwz r11,4(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 4);
  // cmplw cr6,r10,r11
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r11), so);
  // bge cr6,0x820a97fc
  if (!cr6.lt) goto loc_820A97FC;
loc_820A97F8:
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A97FC:
  // stw r10,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r10));
  // mr r3,r29
  r3 = r29;
  // ld r11,80(r1)
  r11 = REX_LOAD_U64(uint32_t(r1) + 80);
  // std r11,0(r29)
  REX_STORE_U64(uint32_t(r29) + 0, r11);
  // addi r1,r1,128
  r1 = uint64_t(int64_t(int64_t(r1) + 128));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820A9818: lifted from generated/ by lift.py.
lifted::Result f820A9818(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_820A9818 (lifted)");
  uint64_t r5 = 0, r6 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0;
  lifted::Cr cr0, cr6;
  uint8_t ca = 0, so = 0;
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
  // mr r6,r4
  r6 = r4;
  // lwz r10,4(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 4);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne 0x820a9840
  if (!cr0.eq) goto loc_820A9840;
  // li r8,0
  r8 = uint64_t(int64_t(0));
  // b 0x820a984c
  goto loc_820A984C;
loc_820A9840:
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // srawi r8,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r8 = uint64_t(int64_t(int32_t(r9) >> 2));
loc_820A984C:
  // cmplwi cr6,r10,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // beq cr6,0x820a9880
  if (cr6.eq) goto loc_820A9880;
  // lwz r9,12(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 12);
  // subf r9,r10,r9
  r9 = r9 - r10;
  // srawi r9,r9,2
  ca = (int32_t(r9) < 0) & ((uint32_t(r9) & 0x3) != 0);
  r9 = uint64_t(int64_t(int32_t(r9) >> 2));
  // cmplw cr6,r8,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r8), uint32_t(r9), so);
  // bge cr6,0x820a9880
  if (!cr6.lt) goto loc_820A9880;
  // lwz r10,8(r11)
  r10 = REX_LOAD_U32(uint32_t(r11) + 8);
  // lwz r9,0(r6)
  r9 = REX_LOAD_U32(uint32_t(r6) + 0);
  // addi r8,r10,4
  r8 = uint64_t(int64_t(int64_t(r10) + 4));
  // stw r9,0(r10)
  REX_STORE_U32(uint32_t(r10) + 0, uint32_t(r9));
  // stw r8,8(r11)
  REX_STORE_U32(uint32_t(r11) + 8, uint32_t(r8));
  // b 0x820a98ac
  goto loc_820A98AC;
loc_820A9880:
  // lwz r9,8(r11)
  r9 = REX_LOAD_U32(uint32_t(r11) + 8);
  // cmplw cr6,r10,r9
  cr6 = lifted::Compare<uint32_t>(uint32_t(r10), uint32_t(r9), so);
  // ble cr6,0x820a9890
  if (!cr6.gt) goto loc_820A9890;
  // twi 31,r0,22
  lifted::Trap(22);
loc_820A9890:
  // addi r10,r1,80
  r10 = uint64_t(int64_t(int64_t(r1) + 80));
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // mr r4,r11
  r4 = r11;
  // stw r9,84(r1)
  REX_STORE_U32(uint32_t(r1) + 84, uint32_t(r9));
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // ld r5,0(r10)
  r5 = REX_LOAD_U64(uint32_t(r10) + 0);
  // bl 0x820a9738
  // (left out: dead in C++)
  { const lifted::Result o = f820A9738(base, r1, r13, r3, r4, r5, r6, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_820A98AC:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820A9928: lifted from generated/ by lift.py.
lifted::Result f820A9928(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r7, double f1) {
  ITC_NATIVE_RUNNING("sub_820A9928 (lifted)");
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
  // stw r4,124(r1)
  REX_STORE_U32(uint32_t(r1) + 124, uint32_t(r4));
  // cmplwi cr6,r4,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r4), 0, so);
  // beq cr6,0x820a994c
  if (cr6.eq) goto loc_820A994C;
  // addi r4,r1,124
  r4 = uint64_t(int64_t(int64_t(r1) + 124));
  // lwz r3,4(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 4);
  // bl 0x820a9818
  // (left out: dead in C++)
  { const lifted::Result o = f820A9818(base, r1, r13, r3, r4, r7, f1); r3 = o.r3; f1 = o.f1; }
loc_820A994C:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

// sub_820A9960: lifted from generated/ by lift.py.
uint64_t f820A9960(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A9960 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // addi r3,r3,672
  r3 = uint64_t(int64_t(int64_t(r3) + 672));
  // li r5,128
  r5 = uint64_t(int64_t(128));
  // b 0x825a7410
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820A9970: lifted from generated/ by lift.py.
uint64_t f820A9970(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820A9970 (lifted)");
  uint64_t r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  // mr r11,r3
  r11 = r3;
  // mr r3,r4
  r3 = r4;
  // addi r4,r11,672
  r4 = uint64_t(int64_t(int64_t(r11) + 672));
  // li r5,128
  r5 = uint64_t(int64_t(128));
  // b 0x825a7410
  { const lifted::Result o = lifted::Call(base, 0x825A7410, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return r3;
}

// sub_820A9BD8: lifted from generated/ by lift.py.
void f820A9BD8(uint8_t* base, uint64_t r1, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820A9BD8 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f0 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0;
  lifted::Temp temp{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e88
  // (left out: dead in C++)
  // (left out: dead in C++)
  // lis r6,-32256
  r6 = uint64_t(int64_t(-2113929216));
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r10,r1,-128
  r10 = uint64_t(int64_t(int64_t(r1) + -128));
  // addi r9,r1,-112
  r9 = uint64_t(int64_t(int64_t(r1) + -112));
  // lfs f12,9576(r6)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r6) + 9576);
  f12 = double(temp.f32);
  // addi r8,r1,-96
  r8 = uint64_t(int64_t(int64_t(r1) + -96));
  // stfs f12,-104(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + -104, temp.u32);
  // addi r7,r1,-80
  r7 = uint64_t(int64_t(int64_t(r1) + -80));
  // lfs f12,10108(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 10108);
  f12 = double(temp.f32);
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // lfs f0,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f0 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // stfs f12,-88(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + -88, temp.u32);
  // addi r6,r1,-64
  r6 = uint64_t(int64_t(int64_t(r1) + -64));
  // stfs f12,-72(r1)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r1) + -72, temp.u32);
  // stfs f0,-144(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -144, temp.u32);
  // lfs f11,9572(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 9572);
  f11 = double(temp.f32);
  // lis r5,-32256
  r5 = uint64_t(int64_t(-2113929216));
  // lfs f13,2608(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2608);
  f13 = double(temp.f32);
  // addi r11,r1,-144
  r11 = uint64_t(int64_t(int64_t(r1) + -144));
  // stfs f0,-140(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -140, temp.u32);
  // stfs f0,-136(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -136, temp.u32);
  // stfs f13,-132(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + -132, temp.u32);
  // lfs f12,8152(r5)
  temp.u32 = REX_LOAD_U32(uint32_t(r5) + 8152);
  f12 = double(temp.f32);
  // ld r5,0(r11)
  r5 = REX_LOAD_U64(uint32_t(r11) + 0);
  // stfs f0,-128(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -128, temp.u32);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // stfs f0,-124(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -124, temp.u32);
  // stfs f0,-120(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -120, temp.u32);
  // stfs f13,-116(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + -116, temp.u32);
  // ld r4,0(r10)
  r4 = REX_LOAD_U64(uint32_t(r10) + 0);
  // ld r10,8(r10)
  r10 = REX_LOAD_U64(uint32_t(r10) + 8);
  // stfs f0,-112(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -112, temp.u32);
  // stfs f0,-108(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -108, temp.u32);
  // std r5,480(r3)
  REX_STORE_U64(uint32_t(r3) + 480, r5);
  // stfs f13,-100(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + -100, temp.u32);
  // ld r31,0(r9)
  r31 = REX_LOAD_U64(uint32_t(r9) + 0);
  // std r11,488(r3)
  REX_STORE_U64(uint32_t(r3) + 488, r11);
  // ld r9,8(r9)
  r9 = REX_LOAD_U64(uint32_t(r9) + 8);
  // stfs f0,-96(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -96, temp.u32);
  // std r4,496(r3)
  REX_STORE_U64(uint32_t(r3) + 496, r4);
  // stfs f0,-92(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -92, temp.u32);
  // stfs f13,-84(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + -84, temp.u32);
  // std r10,504(r3)
  REX_STORE_U64(uint32_t(r3) + 504, r10);
  // ld r30,0(r8)
  r30 = REX_LOAD_U64(uint32_t(r8) + 0);
  // ld r8,8(r8)
  r8 = REX_LOAD_U64(uint32_t(r8) + 8);
  // stfs f0,-80(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -80, temp.u32);
  // std r31,512(r3)
  REX_STORE_U64(uint32_t(r3) + 512, r31);
  // stfs f0,-76(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -76, temp.u32);
  // stfs f13,-68(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + -68, temp.u32);
  // std r9,520(r3)
  REX_STORE_U64(uint32_t(r3) + 520, r9);
  // ld r29,0(r7)
  r29 = REX_LOAD_U64(uint32_t(r7) + 0);
  // ld r7,8(r7)
  r7 = REX_LOAD_U64(uint32_t(r7) + 8);
  // stfs f11,528(r3)
  temp.f32 = float(f11);
  REX_STORE_U32(uint32_t(r3) + 528, temp.u32);
  // stfs f12,532(r3)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 532, temp.u32);
  // stfs f12,548(r3)
  temp.f32 = float(f12);
  REX_STORE_U32(uint32_t(r3) + 548, temp.u32);
  // std r30,624(r3)
  REX_STORE_U64(uint32_t(r3) + 624, r30);
  // stfs f0,-64(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -64, temp.u32);
  // std r8,632(r3)
  REX_STORE_U64(uint32_t(r3) + 632, r8);
  // stfs f0,-60(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -60, temp.u32);
  // stfs f0,-56(r1)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r1) + -56, temp.u32);
  // std r29,640(r3)
  REX_STORE_U64(uint32_t(r3) + 640, r29);
  // stfs f13,-52(r1)
  temp.f32 = float(f13);
  REX_STORE_U32(uint32_t(r1) + -52, temp.u32);
  // ld r28,0(r6)
  r28 = REX_LOAD_U64(uint32_t(r6) + 0);
  // ld r6,8(r6)
  r6 = REX_LOAD_U64(uint32_t(r6) + 8);
  // std r7,648(r3)
  REX_STORE_U64(uint32_t(r3) + 648, r7);
  // std r28,656(r3)
  REX_STORE_U64(uint32_t(r3) + 656, r28);
  // std r6,664(r3)
  REX_STORE_U64(uint32_t(r3) + 664, r6);
  // b 0x825a6ed8
  // (left out: dead in C++)
  return;
}

// sub_820AA010: lifted from generated/ by lift.py.
uint64_t f820AA010(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820AA010 (lifted)");
  // addi r3,r3,560
  r3 = uint64_t(int64_t(int64_t(r3) + 560));
  // blr
  return r3;
}

// sub_820AA018: lifted from generated/ by lift.py.
void f820AA018(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820AA018 (lifted)");
  // stw r4,800(r3)
  REX_STORE_U32(uint32_t(r3) + 800, uint32_t(r4));
  // blr
  return;
}

// sub_820AA020: lifted from generated/ by lift.py.
uint64_t f820AA020(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_820AA020 (lifted)");
  // lwz r3,800(r3)
  r3 = REX_LOAD_U32(uint32_t(r3) + 800);
  // blr
  return r3;
}

// sub_820AA028: lifted from generated/ by lift.py.
void f820AA028(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820AA028 (lifted)");
  uint64_t r11 = 0;
  // ld r11,0(r4)
  r11 = REX_LOAD_U64(uint32_t(r4) + 0);
  // std r11,400(r3)
  REX_STORE_U64(uint32_t(r3) + 400, r11);
  // ld r11,8(r4)
  r11 = REX_LOAD_U64(uint32_t(r4) + 8);
  // std r11,408(r3)
  REX_STORE_U64(uint32_t(r3) + 408, r11);
  // blr
  return;
}

// sub_820AA040: lifted from generated/ by lift.py.
void f820AA040(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820AA040 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // addi r11,r4,400
  r11 = uint64_t(int64_t(int64_t(r4) + 400));
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

// sub_820AA058: lifted from generated/ by lift.py.
void f820AA058(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820AA058 (lifted)");
  uint64_t r11 = 0;
  // ld r11,0(r4)
  r11 = REX_LOAD_U64(uint32_t(r4) + 0);
  // std r11,416(r3)
  REX_STORE_U64(uint32_t(r3) + 416, r11);
  // ld r11,8(r4)
  r11 = REX_LOAD_U64(uint32_t(r4) + 8);
  // std r11,424(r3)
  REX_STORE_U64(uint32_t(r3) + 424, r11);
  // blr
  return;
}

// sub_820AA070: lifted from generated/ by lift.py.
void f820AA070(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_820AA070 (lifted)");
  uint64_t r10 = 0, r11 = 0;
  // addi r11,r4,416
  r11 = uint64_t(int64_t(int64_t(r4) + 416));
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

// sub_820AA088: lifted from generated/ by lift.py.
lifted::Result f820AA088(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820AA088 (lifted)");
  uint64_t r4 = 0, r5 = 0, r6 = 0, r11 = 0, r12 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0;
  double f1 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e84
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stfd f29,-72(r1)
  lifted::ScalarFloatMode();
  REX_STORE_U64(uint32_t(r1) + -72, std::bit_cast<uint64_t>(f29));
  // stfd f30,-64(r1)
  REX_STORE_U64(uint32_t(r1) + -64, std::bit_cast<uint64_t>(f30));
  // stfd f31,-56(r1)
  REX_STORE_U64(uint32_t(r1) + -56, std::bit_cast<uint64_t>(f31));
  // stwu r1,-192(r1)
  ea = -192 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r28,r31,432
  r28 = uint64_t(int64_t(int64_t(r31) + 432));
  // addi r30,r31,272
  r30 = uint64_t(int64_t(int64_t(r31) + 272));
  // lfs f5,10148(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10148);
  f5 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r5,r31,400
  r5 = uint64_t(int64_t(int64_t(r31) + 400));
  // mr r6,r28
  r6 = r28;
  // mr r4,r30
  r4 = r30;
  // lfs f4,3396(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 3396);
  f4 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lfs f31,2612(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 2612);
  f31 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmr f3,f31
  f3 = f31;
  // lfs f30,10152(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10152);
  f30 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fmr f2,f30
  f2 = f30;
  // lfs f29,10156(r11)
  temp.u32 = REX_LOAD_U32(uint32_t(r11) + 10156);
  f29 = double(temp.f32);
  // fmr f1,f29
  f1 = f29;
  // bl 0x820a6470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A6470, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,-32256
  r10 = uint64_t(int64_t(-2113929216));
  // fmr f3,f31
  lifted::ScalarFloatMode();
  f3 = f31;
  // mr r11,r3
  r11 = r3;
  // fmr f2,f30
  f2 = f30;
  // addi r27,r31,448
  r27 = uint64_t(int64_t(int64_t(r31) + 448));
  // fmr f1,f29
  f1 = f29;
  // addi r29,r31,464
  r29 = uint64_t(int64_t(int64_t(r31) + 464));
  // addi r5,r31,416
  r5 = uint64_t(int64_t(int64_t(r31) + 416));
  // lfs f5,9596(r10)
  temp.u32 = REX_LOAD_U32(uint32_t(r10) + 9596);
  f5 = double(temp.f32);
  // addi r3,r1,96
  r3 = uint64_t(int64_t(int64_t(r1) + 96));
  // ld r10,0(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 0);
  // mr r6,r27
  r6 = r27;
  // mr r4,r29
  r4 = r29;
  // fmr f4,f5
  f4 = f5;
  // std r10,0(r30)
  REX_STORE_U64(uint32_t(r30) + 0, r10);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // std r11,8(r30)
  REX_STORE_U64(uint32_t(r30) + 8, r11);
  // bl 0x820a6470
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A6470, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r11,r3
  r11 = r3;
  // mr r5,r29
  r5 = r29;
  // mr r4,r30
  r4 = r30;
  // addi r3,r31,288
  r3 = uint64_t(int64_t(int64_t(r31) + 288));
  // ld r10,0(r11)
  r10 = REX_LOAD_U64(uint32_t(r11) + 0);
  // std r10,0(r29)
  REX_STORE_U64(uint32_t(r29) + 0, r10);
  // ld r11,8(r11)
  r11 = REX_LOAD_U64(uint32_t(r11) + 8);
  // std r11,8(r29)
  REX_STORE_U64(uint32_t(r29) + 8, r11);
  // bl 0x820a9cf8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x820A9CF8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // ld r11,0(r30)
  r11 = REX_LOAD_U64(uint32_t(r30) + 0);
  // std r11,0(r28)
  REX_STORE_U64(uint32_t(r28) + 0, r11);
  // ld r11,8(r30)
  r11 = REX_LOAD_U64(uint32_t(r30) + 8);
  // std r11,8(r28)
  REX_STORE_U64(uint32_t(r28) + 8, r11);
  // ld r11,0(r29)
  r11 = REX_LOAD_U64(uint32_t(r29) + 0);
  // std r11,0(r27)
  REX_STORE_U64(uint32_t(r27) + 0, r11);
  // ld r11,8(r29)
  r11 = REX_LOAD_U64(uint32_t(r29) + 8);
  // std r11,8(r27)
  REX_STORE_U64(uint32_t(r27) + 8, r11);
  // addi r1,r1,192
  r1 = uint64_t(int64_t(int64_t(r1) + 192));
  // lfd f29,-72(r1)
  lifted::ScalarFloatMode();
  f29 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -72)));
  // lfd f30,-64(r1)
  f30 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // lfd f31,-56(r1)
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -56)));
  // b 0x825a6ed4
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_820AA198: lifted from generated/ by lift.py.
lifted::Result f820AA198(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1, double f2, double f3, double f4, double f5, double f6, double f7, double f8, double f9, double f10, double f11, double f12, double f13) {
  ITC_NATIVE_RUNNING("sub_820AA198 (lifted)");
  uint64_t r4 = 0, r5 = 0;
  // addi r5,r3,416
  r5 = uint64_t(int64_t(int64_t(r3) + 416));
  // addi r4,r3,272
  r4 = uint64_t(int64_t(int64_t(r3) + 272));
  // addi r3,r3,288
  r3 = uint64_t(int64_t(int64_t(r3) + 288));
  // b 0x820a9cf8
  { const lifted::Result o = lifted::Call(base, 0x820A9CF8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  return {r3, f1};
}

// sub_820AC950: lifted from generated/ by lift.py.
lifted::Result f820AC950(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820AC950 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,2740
  r11 = uint64_t(int64_t(int64_t(r11) + 2740));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x820ac97c
  if (cr0.eq) goto loc_820AC97C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820AC97C:
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

// sub_820ACCF0: lifted from generated/ by lift.py.
lifted::Result f820ACCF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, double f1) {
  ITC_NATIVE_RUNNING("sub_820ACCF0 (lifted)");
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
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,10368
  r11 = uint64_t(int64_t(int64_t(r11) + 10368));
  // clrlwi. r10,r4,31
  r10 = uint32_t(r4) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r10), 0, so);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // beq 0x820acd1c
  if (cr0.eq) goto loc_820ACD1C;
  // bl 0x825a70e8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, 0x825A70E8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_820ACD1C:
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

namespace {

lifted::Result e820A70A8(uint8_t* base, const lifted::Args& a) {
  f820A70A8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A70B0(uint8_t* base, const lifted::Args& a) {
  return f820A70B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820A70F8(uint8_t* base, const lifted::Args& a) {
  return f820A70F8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820A7178(uint8_t* base, const lifted::Args& a) {
  return {f820A7178(base), a.f[0]};
}

lifted::Result e820A7180(uint8_t* base, const lifted::Args& a) {
  return f820A7180(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A71C8(uint8_t* base, const lifted::Args& a) {
  return f820A71C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A7608(uint8_t* base, const lifted::Args& a) {
  f820A7608(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A7620(uint8_t* base, const lifted::Args& a) {
  f820A7620(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A7628(uint8_t* base, const lifted::Args& a) {
  f820A7628(base, a.r1, a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A7688(uint8_t* base, const lifted::Args& a) {
  f820A7688(base, a.r1, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A7AF8(uint8_t* base, const lifted::Args& a) {
  return f820A7AF8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A7B70(uint8_t* base, const lifted::Args& a) {
  return {f820A7B70(base), a.f[0]};
}

lifted::Result e820A7B78(uint8_t* base, const lifted::Args& a) {
  return f820A7B78(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A7BC0(uint8_t* base, const lifted::Args& a) {
  return {f820A7BC0(base, a.r[0]), a.f[0]};
}

lifted::Result e820A7BD0(uint8_t* base, const lifted::Args& a) {
  return f820A7BD0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A7BE0(uint8_t* base, const lifted::Args& a) {
  return f820A7BE0(base, a.r1, a.r13, a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6]);
}

lifted::Result e820A7C00(uint8_t* base, const lifted::Args& a) {
  return f820A7C00(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A8050(uint8_t* base, const lifted::Args& a) {
  f820A8050(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8068(uint8_t* base, const lifted::Args& a) {
  return {f820A8068(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A8088(uint8_t* base, const lifted::Args& a) {
  f820A8088(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8098(uint8_t* base, const lifted::Args& a) {
  f820A8098(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A80A8(uint8_t* base, const lifted::Args& a) {
  return f820A80A8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A80F8(uint8_t* base, const lifted::Args& a) {
  f820A80F8(base, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8108(uint8_t* base, const lifted::Args& a) {
  return f820A8108(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820A81A8(uint8_t* base, const lifted::Args& a) {
  f820A81A8(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A81C0(uint8_t* base, const lifted::Args& a) {
  return f820A81C0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A82E8(uint8_t* base, const lifted::Args& a) {
  return f820A82E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A8340(uint8_t* base, const lifted::Args& a) {
  return f820A8340(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A83C8(uint8_t* base, const lifted::Args& a) {
  return f820A83C8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A8408(uint8_t* base, const lifted::Args& a) {
  return f820A8408(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A8420(uint8_t* base, const lifted::Args& a) {
  f820A8420(base, a.r[0], a.r[1], a.r[2], a.r[3]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8438(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820A8438(base, a.r[0])};
}

lifted::Result e820A8460(uint8_t* base, const lifted::Args& a) {
  return {f820A8460(base, a.r[0]), a.f[0]};
}

lifted::Result e820A8490(uint8_t* base, const lifted::Args& a) {
  f820A8490(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A84C0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820A84C0(base, a.r[0])};
}

lifted::Result e820A84E8(uint8_t* base, const lifted::Args& a) {
  return {f820A84E8(base, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A8508(uint8_t* base, const lifted::Args& a) {
  return f820A8508(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A8690(uint8_t* base, const lifted::Args& a) {
  return f820A8690(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A86B0(uint8_t* base, const lifted::Args& a) {
  return f820A86B0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A8708(uint8_t* base, const lifted::Args& a) {
  return f820A8708(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A87A0(uint8_t* base, const lifted::Args& a) {
  return f820A87A0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A87F0(uint8_t* base, const lifted::Args& a) {
  return f820A87F0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A8840(uint8_t* base, const lifted::Args& a) {
  return f820A8840(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A8850(uint8_t* base, const lifted::Args& a) {
  return f820A8850(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A8860(uint8_t* base, const lifted::Args& a) {
  return f820A8860(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A8870(uint8_t* base, const lifted::Args& a) {
  return f820A8870(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5]);
}

lifted::Result e820A88F8(uint8_t* base, const lifted::Args& a) {
  return {f820A88F8(base, a.r[0]), a.f[0]};
}

lifted::Result e820A8910(uint8_t* base, const lifted::Args& a) {
  f820A8910(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8928(uint8_t* base, const lifted::Args& a) {
  f820A8928(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8940(uint8_t* base, const lifted::Args& a) {
  f820A8940(base, a.r[0], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8948(uint8_t* base, const lifted::Args& a) {
  f820A8948(base, a.r[0], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8950(uint8_t* base, const lifted::Args& a) {
  f820A8950(base, a.r[0], a.f[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8960(uint8_t* base, const lifted::Args& a) {
  return {f820A8960(base, a.r[0]), a.f[0]};
}

lifted::Result e820A8978(uint8_t* base, const lifted::Args& a) {
  return {f820A8978(base, a.r[0]), a.f[0]};
}

lifted::Result e820A8990(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820A8990(base, a.r[0])};
}

lifted::Result e820A8998(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820A8998(base, a.r[0])};
}

lifted::Result e820A89A0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820A89A0(base, a.r[0])};
}

lifted::Result e820A89A8(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820A89A8(base, a.r[0])};
}

lifted::Result e820A89B0(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820A89B0(base, a.r[0])};
}

lifted::Result e820A89B8(uint8_t* base, const lifted::Args& a) {
  return {a.r[0], f820A89B8(base, a.r[0])};
}

lifted::Result e820A89C0(uint8_t* base, const lifted::Args& a) {
  f820A89C0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A89E0(uint8_t* base, const lifted::Args& a) {
  f820A89E0(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8A00(uint8_t* base, const lifted::Args& a) {
  return {f820A8A00(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A8A08(uint8_t* base, const lifted::Args& a) {
  return {f820A8A08(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A8A10(uint8_t* base, const lifted::Args& a) {
  return {f820A8A10(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A8A18(uint8_t* base, const lifted::Args& a) {
  f820A8A18(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8A30(uint8_t* base, const lifted::Args& a) {
  f820A8A30(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8A48(uint8_t* base, const lifted::Args& a) {
  return {f820A8A48(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A8A98(uint8_t* base, const lifted::Args& a) {
  return {f820A8A98(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A8AE8(uint8_t* base, const lifted::Args& a) {
  return {f820A8AE8(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A8B38(uint8_t* base, const lifted::Args& a) {
  return f820A8B38(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7]);
}

lifted::Result e820A8EA0(uint8_t* base, const lifted::Args& a) {
  return {f820A8EA0(base, a.r[0]), a.f[0]};
}

lifted::Result e820A8EC8(uint8_t* base, const lifted::Args& a) {
  f820A8EC8(base, a.r1, a.r[0], a.r[1], a.r[2]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820A8F20(uint8_t* base, const lifted::Args& a) {
  return f820A8F20(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A9010(uint8_t* base, const lifted::Args& a) {
  return f820A9010(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A9078(uint8_t* base, const lifted::Args& a) {
  return f820A9078(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e820A9200(uint8_t* base, const lifted::Args& a) {
  return f820A9200(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.f[0]);
}

lifted::Result e820A9208(uint8_t* base, const lifted::Args& a) {
  return f820A9208(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A9270(uint8_t* base, const lifted::Args& a) {
  return f820A9270(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A92D0(uint8_t* base, const lifted::Args& a) {
  return f820A92D0(base, a.r1, a.r13, a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A9330(uint8_t* base, const lifted::Args& a) {
  return f820A9330(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e820A93E8(uint8_t* base, const lifted::Args& a) {
  return f820A93E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e820A96D0(uint8_t* base, const lifted::Args& a) {
  return f820A96D0(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820A9738(uint8_t* base, const lifted::Args& a) {
  return f820A9738(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.f[0]);
}

lifted::Result e820A9818(uint8_t* base, const lifted::Args& a) {
  return f820A9818(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e820A9928(uint8_t* base, const lifted::Args& a) {
  return f820A9928(base, a.r1, a.r13, a.r[0], a.r[1], a.r[4], a.f[0]);
}

lifted::Result e820A9960(uint8_t* base, const lifted::Args& a) {
  return {f820A9960(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A9970(uint8_t* base, const lifted::Args& a) {
  return {f820A9970(base, a.r1, a.r13, a.r[0], a.r[1]), a.f[0]};
}

lifted::Result e820A9BD8(uint8_t* base, const lifted::Args& a) {
  f820A9BD8(base, a.r1, a.r[0]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820AA010(uint8_t* base, const lifted::Args& a) {
  return {f820AA010(base, a.r[0]), a.f[0]};
}

lifted::Result e820AA018(uint8_t* base, const lifted::Args& a) {
  f820AA018(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820AA020(uint8_t* base, const lifted::Args& a) {
  return {f820AA020(base, a.r[0]), a.f[0]};
}

lifted::Result e820AA028(uint8_t* base, const lifted::Args& a) {
  f820AA028(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820AA040(uint8_t* base, const lifted::Args& a) {
  f820AA040(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820AA058(uint8_t* base, const lifted::Args& a) {
  f820AA058(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820AA070(uint8_t* base, const lifted::Args& a) {
  f820AA070(base, a.r[0], a.r[1]);
  return {a.r[0], a.f[0]};
}

lifted::Result e820AA088(uint8_t* base, const lifted::Args& a) {
  return f820AA088(base, a.r1, a.r13, a.r[0], a.r[4], a.r[5], a.r[6], a.r[7], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820AA198(uint8_t* base, const lifted::Args& a) {
  return f820AA198(base, a.r1, a.r13, a.r[0], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0], a.f[1], a.f[2], a.f[3], a.f[4], a.f[5], a.f[6], a.f[7], a.f[8], a.f[9], a.f[10], a.f[11], a.f[12]);
}

lifted::Result e820AC950(uint8_t* base, const lifted::Args& a) {
  return f820AC950(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

lifted::Result e820ACCF0(uint8_t* base, const lifted::Args& a) {
  return f820ACCF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.f[0]);
}

const Registration kEntries[] = {
    {0x820A70A8, e820A70A8},
    {0x820A70B0, e820A70B0},
    {0x820A70F8, e820A70F8},
    {0x820A7178, e820A7178},
    {0x820A7180, e820A7180},
    {0x820A71C8, e820A71C8},
    {0x820A7608, e820A7608},
    {0x820A7620, e820A7620},
    {0x820A7628, e820A7628},
    {0x820A7688, e820A7688},
    {0x820A7AF8, e820A7AF8},
    {0x820A7B70, e820A7B70},
    {0x820A7B78, e820A7B78},
    {0x820A7BC0, e820A7BC0},
    {0x820A7BD0, e820A7BD0},
    {0x820A7BE0, e820A7BE0},
    {0x820A7C00, e820A7C00},
    {0x820A8050, e820A8050},
    {0x820A8068, e820A8068},
    {0x820A8088, e820A8088},
    {0x820A8098, e820A8098},
    {0x820A80A8, e820A80A8},
    {0x820A80F8, e820A80F8},
    {0x820A8108, e820A8108},
    {0x820A81A8, e820A81A8},
    {0x820A81C0, e820A81C0},
    {0x820A82E8, e820A82E8},
    {0x820A8340, e820A8340},
    {0x820A83C8, e820A83C8},
    {0x820A8408, e820A8408},
    {0x820A8420, e820A8420},
    {0x820A8438, e820A8438},
    {0x820A8460, e820A8460},
    {0x820A8490, e820A8490},
    {0x820A84C0, e820A84C0},
    {0x820A84E8, e820A84E8},
    {0x820A8508, e820A8508},
    {0x820A8690, e820A8690},
    {0x820A86B0, e820A86B0},
    {0x820A8708, e820A8708},
    {0x820A87A0, e820A87A0},
    {0x820A87F0, e820A87F0},
    {0x820A8840, e820A8840},
    {0x820A8850, e820A8850},
    {0x820A8860, e820A8860},
    {0x820A8870, e820A8870},
    {0x820A88F8, e820A88F8},
    {0x820A8910, e820A8910},
    {0x820A8928, e820A8928},
    {0x820A8940, e820A8940},
    {0x820A8948, e820A8948},
    {0x820A8950, e820A8950},
    {0x820A8960, e820A8960},
    {0x820A8978, e820A8978},
    {0x820A8990, e820A8990},
    {0x820A8998, e820A8998},
    {0x820A89A0, e820A89A0},
    {0x820A89A8, e820A89A8},
    {0x820A89B0, e820A89B0},
    {0x820A89B8, e820A89B8},
    {0x820A89C0, e820A89C0},
    {0x820A89E0, e820A89E0},
    {0x820A8A00, e820A8A00},
    {0x820A8A08, e820A8A08},
    {0x820A8A10, e820A8A10},
    {0x820A8A18, e820A8A18},
    {0x820A8A30, e820A8A30},
    {0x820A8A48, e820A8A48},
    {0x820A8A98, e820A8A98},
    {0x820A8AE8, e820A8AE8},
    {0x820A8B38, e820A8B38},
    {0x820A8EA0, e820A8EA0},
    {0x820A8EC8, e820A8EC8},
    {0x820A8F20, e820A8F20},
    {0x820A9010, e820A9010},
    {0x820A9078, e820A9078},
    {0x820A9200, e820A9200},
    {0x820A9208, e820A9208},
    {0x820A9270, e820A9270},
    {0x820A92D0, e820A92D0},
    {0x820A9330, e820A9330},
    {0x820A93E8, e820A93E8},
    {0x820A96D0, e820A96D0},
    {0x820A9738, e820A9738},
    {0x820A9818, e820A9818},
    {0x820A9928, e820A9928},
    {0x820A9960, e820A9960},
    {0x820A9970, e820A9970},
    {0x820A9BD8, e820A9BD8},
    {0x820AA010, e820AA010},
    {0x820AA018, e820AA018},
    {0x820AA020, e820AA020},
    {0x820AA028, e820AA028},
    {0x820AA040, e820AA040},
    {0x820AA058, e820AA058},
    {0x820AA070, e820AA070},
    {0x820AA088, e820AA088},
    {0x820AA198, e820AA198},
    {0x820AC950, e820AC950},
    {0x820ACCF0, e820ACCF0},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_003)), true);

}  // namespace

}  // namespace lifted

// The entries for the SDK and the code not native yet (the function table
// reaches these): to go once nothing recompiled calls them.
extern "C" REX_FUNC(sub_820A70A8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A70A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A70A8);
}

extern "C" REX_FUNC(sub_820A70B0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A70B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A70B0);
}

extern "C" REX_FUNC(sub_820A70F8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A70F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A70F8);
}

extern "C" REX_FUNC(sub_820A7178) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7178(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7178);
}

extern "C" REX_FUNC(sub_820A7180) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7180(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7180);
}

extern "C" REX_FUNC(sub_820A71C8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A71C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A71C8);
}

extern "C" REX_FUNC(sub_820A7608) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7608(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7608);
}

extern "C" REX_FUNC(sub_820A7620) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7620(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7620);
}

extern "C" REX_FUNC(sub_820A7628) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7628(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7628);
}

extern "C" REX_FUNC(sub_820A7688) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7688(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7688);
}

extern "C" REX_FUNC(sub_820A7AF8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7AF8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7AF8);
}

extern "C" REX_FUNC(sub_820A7B70) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7B70(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7B70);
}

extern "C" REX_FUNC(sub_820A7B78) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7B78(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7B78);
}

extern "C" REX_FUNC(sub_820A7BC0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7BC0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7BC0);
}

extern "C" REX_FUNC(sub_820A7BD0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7BD0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7BD0);
}

extern "C" REX_FUNC(sub_820A7BE0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7BE0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7BE0);
}

extern "C" REX_FUNC(sub_820A7C00) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A7C00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A7C00);
}

extern "C" REX_FUNC(sub_820A8050) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8050(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8050);
}

extern "C" REX_FUNC(sub_820A8068) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8068(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8068);
}

extern "C" REX_FUNC(sub_820A8088) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8088(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8088);
}

extern "C" REX_FUNC(sub_820A8098) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8098(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8098);
}

extern "C" REX_FUNC(sub_820A80A8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A80A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A80A8);
}

extern "C" REX_FUNC(sub_820A80F8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A80F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A80F8);
}

extern "C" REX_FUNC(sub_820A8108) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8108(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8108);
}

extern "C" REX_FUNC(sub_820A81A8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A81A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A81A8);
}

extern "C" REX_FUNC(sub_820A81C0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A81C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A81C0);
}

extern "C" REX_FUNC(sub_820A82E8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A82E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A82E8);
}

extern "C" REX_FUNC(sub_820A8340) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8340(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8340);
}

extern "C" REX_FUNC(sub_820A83C8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A83C8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A83C8);
}

extern "C" REX_FUNC(sub_820A8408) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8408(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8408);
}

extern "C" REX_FUNC(sub_820A8420) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8420(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8420);
}

extern "C" REX_FUNC(sub_820A8438) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8438(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8438);
}

extern "C" REX_FUNC(sub_820A8460) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8460(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8460);
}

extern "C" REX_FUNC(sub_820A8490) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8490(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8490);
}

extern "C" REX_FUNC(sub_820A84C0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A84C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A84C0);
}

extern "C" REX_FUNC(sub_820A84E8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A84E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A84E8);
}

extern "C" REX_FUNC(sub_820A8508) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8508(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8508);
}

extern "C" REX_FUNC(sub_820A8690) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8690(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8690);
}

extern "C" REX_FUNC(sub_820A86B0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A86B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A86B0);
}

extern "C" REX_FUNC(sub_820A8708) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8708(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8708);
}

extern "C" REX_FUNC(sub_820A87A0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A87A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A87A0);
}

extern "C" REX_FUNC(sub_820A87F0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A87F0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A87F0);
}

extern "C" REX_FUNC(sub_820A8840) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8840(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8840);
}

extern "C" REX_FUNC(sub_820A8850) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8850(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8850);
}

extern "C" REX_FUNC(sub_820A8860) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8860(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8860);
}

extern "C" REX_FUNC(sub_820A8870) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8870(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8870);
}

extern "C" REX_FUNC(sub_820A88F8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A88F8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A88F8);
}

extern "C" REX_FUNC(sub_820A8910) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8910(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8910);
}

extern "C" REX_FUNC(sub_820A8928) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8928(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8928);
}

extern "C" REX_FUNC(sub_820A8940) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8940(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8940);
}

extern "C" REX_FUNC(sub_820A8948) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8948(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8948);
}

extern "C" REX_FUNC(sub_820A8950) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8950(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8950);
}

extern "C" REX_FUNC(sub_820A8960) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8960(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8960);
}

extern "C" REX_FUNC(sub_820A8978) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8978(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8978);
}

extern "C" REX_FUNC(sub_820A8990) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8990(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8990);
}

extern "C" REX_FUNC(sub_820A8998) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8998(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8998);
}

extern "C" REX_FUNC(sub_820A89A0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A89A0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A89A0);
}

extern "C" REX_FUNC(sub_820A89A8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A89A8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A89A8);
}

extern "C" REX_FUNC(sub_820A89B0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A89B0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A89B0);
}

extern "C" REX_FUNC(sub_820A89B8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A89B8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A89B8);
}

extern "C" REX_FUNC(sub_820A89C0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A89C0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A89C0);
}

extern "C" REX_FUNC(sub_820A89E0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A89E0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A89E0);
}

extern "C" REX_FUNC(sub_820A8A00) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8A00(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8A00);
}

extern "C" REX_FUNC(sub_820A8A08) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8A08(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8A08);
}

extern "C" REX_FUNC(sub_820A8A10) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8A10(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8A10);
}

extern "C" REX_FUNC(sub_820A8A18) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8A18(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8A18);
}

extern "C" REX_FUNC(sub_820A8A30) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8A30(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8A30);
}

extern "C" REX_FUNC(sub_820A8A48) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8A48(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8A48);
}

extern "C" REX_FUNC(sub_820A8A98) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8A98(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8A98);
}

extern "C" REX_FUNC(sub_820A8AE8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8AE8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8AE8);
}

extern "C" REX_FUNC(sub_820A8B38) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8B38(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8B38);
}

extern "C" REX_FUNC(sub_820A8EA0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8EA0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8EA0);
}

extern "C" REX_FUNC(sub_820A8EC8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8EC8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8EC8);
}

extern "C" REX_FUNC(sub_820A8F20) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A8F20(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A8F20);
}

extern "C" REX_FUNC(sub_820A9010) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9010(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9010);
}

extern "C" REX_FUNC(sub_820A9078) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9078(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9078);
}

extern "C" REX_FUNC(sub_820A9200) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9200(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9200);
}

extern "C" REX_FUNC(sub_820A9208) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9208(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9208);
}

extern "C" REX_FUNC(sub_820A9270) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9270(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9270);
}

extern "C" REX_FUNC(sub_820A92D0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A92D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A92D0);
}

extern "C" REX_FUNC(sub_820A9330) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9330(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9330);
}

extern "C" REX_FUNC(sub_820A93E8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A93E8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A93E8);
}

extern "C" REX_FUNC(sub_820A96D0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A96D0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A96D0);
}

extern "C" REX_FUNC(sub_820A9738) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9738(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9738);
}

extern "C" REX_FUNC(sub_820A9818) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9818(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9818);
}

extern "C" REX_FUNC(sub_820A9928) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9928(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9928);
}

extern "C" REX_FUNC(sub_820A9960) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9960(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9960);
}

extern "C" REX_FUNC(sub_820A9970) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9970(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9970);
}

extern "C" REX_FUNC(sub_820A9BD8) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820A9BD8(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820A9BD8);
}

extern "C" REX_FUNC(sub_820AA010) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820AA010(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820AA010);
}

extern "C" REX_FUNC(sub_820AA018) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820AA018(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820AA018);
}

extern "C" REX_FUNC(sub_820AA020) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820AA020(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820AA020);
}

extern "C" REX_FUNC(sub_820AA028) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820AA028(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820AA028);
}

extern "C" REX_FUNC(sub_820AA040) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820AA040(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820AA040);
}

extern "C" REX_FUNC(sub_820AA058) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820AA058(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820AA058);
}

extern "C" REX_FUNC(sub_820AA070) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820AA070(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820AA070);
}

extern "C" REX_FUNC(sub_820AA088) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820AA088(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820AA088);
}

extern "C" REX_FUNC(sub_820AA198) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820AA198(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820AA198);
}

extern "C" REX_FUNC(sub_820AC950) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820AC950(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820AC950);
}

extern "C" REX_FUNC(sub_820ACCF0) {
  if (!ITC_FLAG(native_lifted_003)) {
    __imp__sub_820ACCF0(ctx, base);
    return;
  }
  lifted::Enter(ctx, base, lifted::e820ACCF0);
}
