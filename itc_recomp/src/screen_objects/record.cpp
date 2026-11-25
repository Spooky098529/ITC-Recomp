// ScreenObject::Record (sub_82130978, docs/functions/screen-objects.md) as
// plain C++: lift.py's translation of generated/ (itc_recomp/tools/lift.py,
// docs/loops/lift-sweep.md), made with the tool's own functions since the
// tool leaves out functions with an override of ours (this one's wrap in
// src/debug/frame_state_diff.cpp, which calls it with --native_screen_objects).
// Statement by statement as the recompiled code: registers as locals, guest
// memory through guest::Load and guest::Store, the command list's functions and
// the 87 calls through vtables by the dispatch table (lifted::Call: their
// overrides and wraps, AllocateSlots' interpolation among them, still run),
// the lifted callees (sub_82130618, sub_8212F600, sub_8212F538,
// sub_8212F778) called directly. r4 to r10 and f1 are the caller's, passed
// on as the original's calls through vtables pass every argument register.
// Don't edit by hand beyond this header and the names: made again with the
// tool if the translation changes.

#include "screen_objects/screen_objects.h"

#include "lifted/functions.h"
#include "lifted/sdk.h"

#include "base/log.h"

#include <bit>
#include <cstdlib>

// A call by address or an import passes every argument register.
#define LIFTED_ARGS \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13} }

namespace screen_objects {

lifted::Result Record(uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4,
                      uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9,
                      uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82130978 ScreenObject::Record");
  uint64_t r11 = 0, r12 = 0, r15 = 0, r16 = 0, r17 = 0, r18 = 0, r19 = 0, r20 = 0, r21 = 0, r22 = 0, r23 = 0, r24 = 0, r25 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f28 = 0.0, f29 = 0.0, f30 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
  uint8_t so = 0;
  lifted::Temp temp{};
  uint32_t ea{};
  // mflr r12
  r12 = 0;
  // bl 0x825a6e54
  // (left out: dead in C++)
  // (left out: dead in C++)
  // addi r12,r1,-144
  r12 = uint64_t(int64_t(int64_t(r1) + -144));
  // bl 0x825a7918
  // (left out: dead in C++)
  // (left out: dead in C++)
  // stwu r1,-288(r1)
  ea = -288 + uint32_t(r1);
  guest::StoreU32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r3
  r29 = r3;
  // li r31,0
  r31 = uint64_t(int64_t(0));
  // lbz r11,24(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 24);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82131668
  if (cr0.eq) goto loc_82131668;
  // lis r30,-32148
  r30 = uint64_t(int64_t(-2106851328));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x8258c9d0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258C9D0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r15,r3
  r15 = r3;
  // li r24,5
  r24 = uint64_t(int64_t(5));
  // lis r11,-32148
  r11 = uint64_t(int64_t(-2106851328));
  // addi r25,r11,-21504
  r25 = uint64_t(int64_t(int64_t(r11) + -21504));
  // stw r31,4(r15)
  guest::StoreU32(uint32_t(r15) + 4, uint32_t(r31));
  // sth r24,2(r15)
  guest::StoreU16(uint32_t(r15) + 2, uint16_t(r24));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,80(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 80);
  // mulli r11,r11,592
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(592))));
  // lwz r10,0(r3)
  r10 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r10,32(r10)
  r10 = guest::LoadU32(uint32_t(r10) + 32);
  // add r27,r11,r25
  r27 = r11 + r25;
  // mtctr r10
  ctr = r10;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,30
  r11 = uint64_t(int64_t(30));
  // li r28,1
  r28 = uint64_t(int64_t(1));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // stw r11,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r11));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // lwz r10,496(r27)
  r10 = guest::LoadU32(uint32_t(r27) + 496);
  // lfs f28,2612(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2612);
  f28 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // sth r10,0(r3)
  guest::StoreU16(uint32_t(r3) + 0, uint16_t(r10));
  // lfs f29,2608(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 2608);
  f29 = double(temp.f32);
  // lwz r11,500(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 500);
  // sth r11,2(r3)
  guest::StoreU16(uint32_t(r3) + 2, uint16_t(r11));
  // lwz r11,504(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 504);
  // sth r11,4(r3)
  guest::StoreU16(uint32_t(r3) + 4, uint16_t(r11));
  // lwz r11,508(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 508);
  // stfs f28,8(r3)
  temp.f32 = float(f28);
  guest::StoreU32(uint32_t(r3) + 8, temp.u32);
  // stfs f29,12(r3)
  temp.f32 = float(f29);
  guest::StoreU32(uint32_t(r3) + 12, temp.u32);
  // sth r11,6(r3)
  guest::StoreU16(uint32_t(r3) + 6, uint16_t(r11));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,25
  r11 = uint64_t(int64_t(25));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // stw r11,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r11));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // bl 0x8258cc30
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CC30, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r27,2
  r27 = uint64_t(int64_t(2));
  // li r23,3
  r23 = uint64_t(int64_t(3));
  // stw r27,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r27));
  // lwz r11,16(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 16);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x82130b4c
  if (!cr6.eq) goto loc_82130B4C;
  // lwz r11,20(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82130b50
  if (cr6.eq) goto loc_82130B50;
loc_82130B4C:
  // stw r23,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r23));
loc_82130B50:
  // stw r31,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r31));
  // lis r10,6184
  r10 = uint64_t(int64_t(405274624));
  // lis r9,6690
  r9 = uint64_t(int64_t(438435840));
  // lwz r8,4(r29)
  r8 = guest::LoadU32(uint32_t(r29) + 4);
  // li r11,255
  r11 = uint64_t(int64_t(255));
  // stfs f28,48(r3)
  lifted::ScalarFloatMode();
  temp.f32 = float(f28);
  guest::StoreU32(uint32_t(r3) + 48, temp.u32);
  // ori r10,r10,390
  r10 = r10 | 390;
  // stw r31,36(r3)
  guest::StoreU32(uint32_t(r3) + 36, uint32_t(r31));
  // ori r9,r9,407
  r9 = r9 | 407;
  // stw r31,40(r3)
  guest::StoreU32(uint32_t(r3) + 40, uint32_t(r31));
  // stw r31,52(r3)
  guest::StoreU32(uint32_t(r3) + 52, uint32_t(r31));
  // stw r8,24(r3)
  guest::StoreU32(uint32_t(r3) + 24, uint32_t(r8));
  // stb r11,47(r3)
  guest::StoreU8(uint32_t(r3) + 47, uint8_t(r11));
  // stb r31,44(r3)
  guest::StoreU8(uint32_t(r3) + 44, uint8_t(r31));
  // stb r31,45(r3)
  guest::StoreU8(uint32_t(r3) + 45, uint8_t(r31));
  // stb r31,46(r3)
  guest::StoreU8(uint32_t(r3) + 46, uint8_t(r31));
  // stw r10,56(r3)
  guest::StoreU32(uint32_t(r3) + 56, uint32_t(r10));
  // stw r31,60(r3)
  guest::StoreU32(uint32_t(r3) + 60, uint32_t(r31));
  // stw r9,64(r3)
  guest::StoreU32(uint32_t(r3) + 64, uint32_t(r9));
  // lwz r11,16(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 16);
  // stw r11,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r11));
  // lwz r11,20(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 20);
  // stw r11,12(r3)
  guest::StoreU32(uint32_t(r3) + 12, uint32_t(r11));
  // lwz r10,16(r29)
  r10 = guest::LoadU32(uint32_t(r29) + 16);
  // lwz r11,8(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 8);
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,16(r3)
  guest::StoreU32(uint32_t(r3) + 16, uint32_t(r11));
  // lwz r11,12(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 12);
  // lwz r10,20(r29)
  r10 = guest::LoadU32(uint32_t(r29) + 20);
  // add r11,r11,r10
  r11 = r11 + r10;
  // stw r11,20(r3)
  guest::StoreU32(uint32_t(r3) + 20, uint32_t(r11));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r26,4
  r26 = uint64_t(int64_t(4));
  // stw r28,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r28));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // stw r26,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r26));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r19,60
  r19 = uint64_t(int64_t(60));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // stw r19,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r19));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r20,40
  r20 = uint64_t(int64_t(40));
  // stw r31,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r31));
  // stw r20,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r20));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r21,304
  r21 = uint64_t(int64_t(304));
  // stw r31,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r31));
  // stw r21,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r21));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r22,56
  r22 = uint64_t(int64_t(56));
  // stw r31,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r31));
  // stw r22,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r22));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r23,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r23));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // stw r31,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r31));
  // addi r10,r25,496
  r10 = uint64_t(int64_t(int64_t(r25) + 496));
  // ori r11,r11,1798
  r11 = r11 | 1798;
  // stw r11,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r11));
  // lwz r11,80(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 80);
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // mulli r11,r11,592
  r11 = uint64_t(int64_t(static_cast<int64_t>(r11 * static_cast<uint64_t>(592))));
  // lwz r9,0(r3)
  r9 = guest::LoadU32(uint32_t(r3) + 0);
  // add r11,r11,r10
  r11 = r11 + r10;
  // lwz r9,32(r9)
  r9 = guest::LoadU32(uint32_t(r9) + 32);
  // lwa r10,0(r11)
  r10 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r11) + 0))));
  // lwa r11,4(r11)
  r11 = uint64_t(int64_t(int32_t(guest::LoadU32(uint32_t(r11) + 4))));
  // std r10,80(r1)
  guest::StoreU64(uint32_t(r1) + 80, r10);
  // std r11,96(r1)
  guest::StoreU64(uint32_t(r1) + 96, r11);
  // lfd f13,96(r1)
  lifted::ScalarFloatMode();
  f13 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 96)));
  // fcfid f13,f13
  f13 = double(std::bit_cast<int64_t>(f13));
  // frsp f30,f13
  f30 = double(float(f13));
  // lfd f0,80(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 80)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f31,f0
  f31 = double(float(f0));
  // mtctr r9
  ctr = r9;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lis r10,1
  r10 = uint64_t(int64_t(65536));
  // li r11,7
  r11 = uint64_t(int64_t(7));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // ori r10,r10,1
  r10 = r10 | 1;
  // stw r11,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r11));
  // stw r10,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r10));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // stfs f31,96(r1)
  lifted::ScalarFloatMode();
  temp.f32 = float(f31);
  guest::StoreU32(uint32_t(r1) + 96, temp.u32);
  // stfs f30,100(r1)
  temp.f32 = float(f30);
  guest::StoreU32(uint32_t(r1) + 100, temp.u32);
  // stfs f28,104(r1)
  temp.f32 = float(f28);
  guest::StoreU32(uint32_t(r1) + 104, temp.u32);
  // stfs f28,108(r1)
  temp.f32 = float(f28);
  guest::StoreU32(uint32_t(r1) + 108, temp.u32);
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // ld r10,0(r11)
  r10 = guest::LoadU64(uint32_t(r11) + 0);
  // ld r11,8(r11)
  r11 = guest::LoadU64(uint32_t(r11) + 8);
  // std r10,0(r3)
  guest::StoreU64(uint32_t(r3) + 0, r10);
  // std r11,8(r3)
  guest::StoreU64(uint32_t(r3) + 8, r11);
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,8
  r11 = uint64_t(int64_t(8));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // stw r11,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r11));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // bl 0x825896d8
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x825896D8, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r25)
  guest::StoreU32(uint32_t(r25) + 0, uint32_t(r3));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,9
  r11 = uint64_t(int64_t(9));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // stw r11,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r11));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // mr r25,r3
  r25 = r3;
  // li r3,8
  r3 = uint64_t(int64_t(8));
  // bl 0x82589700
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x82589700, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r3,0(r25)
  guest::StoreU32(uint32_t(r25) + 0, uint32_t(r3));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,10
  r11 = uint64_t(int64_t(10));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // stw r11,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r11));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 0);
  // stw r11,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r11));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r26,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r26));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r31,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r31));
  // lwz r11,4(r29)
  r11 = guest::LoadU32(uint32_t(r29) + 4);
  // stw r11,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r11));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r27,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r27));
  // stw r24,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r24));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r31,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r31));
  // stw r31,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r31));
  // stw r27,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r27));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r31,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r31));
  // stw r26,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r26));
  // stw r27,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r27));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r24,20
  r24 = uint64_t(int64_t(20));
  // stw r31,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r31));
  // stw r28,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r28));
  // stw r24,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r24));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,16
  r11 = uint64_t(int64_t(16));
  // stw r31,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r31));
  // stw r28,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r28));
  // stw r11,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r11));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r25,r29,1
  r25 = uint64_t(int64_t(int64_t(r29) + 65536));
  // li r11,24
  r11 = uint64_t(int64_t(24));
  // stw r31,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r31));
  // addi r25,r25,10572
  r25 = uint64_t(int64_t(int64_t(r25) + 10572));
  // stw r27,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r27));
  // li r17,19
  r17 = uint64_t(int64_t(19));
  // li r18,6
  r18 = uint64_t(int64_t(6));
  // stw r11,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r11));
  // lbz r11,0(r25)
  r11 = guest::LoadU8(uint32_t(r25) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82131238
  if (!cr0.eq) goto loc_82131238;
  // lbz r11,27(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 27);
  // lbz r10,25(r29)
  r10 = guest::LoadU8(uint32_t(r29) + 25);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // or r11,r11,r10
  r11 = r11 | r10;
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821311a4
  if (!cr0.eq) goto loc_821311A4;
  // mr r3,r29
  r3 = r29;
  // bl 0x82130618
  // (left out: dead in C++)
  { const lifted::Result o = lifted::f82130618(r1, r13, r3); r3 = o.r3; f1 = o.f1; }
  // clrlwi r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // stb r11,0(r25)
  guest::StoreU8(uint32_t(r25) + 0, uint8_t(r11));
loc_821311A4:
  // addis r27,r29,2
  r27 = uint64_t(int64_t(int64_t(r29) + 131072));
  // addi r27,r27,-3096
  r27 = uint64_t(int64_t(int64_t(r27) + -3096));
  // lwz r11,0(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82131238
  if (cr6.eq) goto loc_82131238;
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r17,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r17));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // addis r11,r29,1
  r11 = uint64_t(int64_t(int64_t(r29) + 65536));
  // stw r18,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r18));
  // lwz r10,0(r27)
  r10 = guest::LoadU32(uint32_t(r27) + 0);
  // addi r11,r11,10600
  r11 = uint64_t(int64_t(int64_t(r11) + 10600));
  // sth r24,6(r3)
  guest::StoreU16(uint32_t(r3) + 6, uint16_t(r24));
  // stw r11,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r11));
  // sth r10,4(r3)
  guest::StoreU16(uint32_t(r3) + 4, uint16_t(r10));
loc_82131238:
  // mr r3,r29
  r3 = r29;
  // bl 0x8212f600
  // (left out: dead in C++)
  { const lifted::Result o = lifted::f8212F600(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // addis r16,r29,2
  r16 = uint64_t(int64_t(int64_t(r29) + 131072));
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // addi r16,r16,-3076
  r16 = uint64_t(int64_t(int64_t(r16) + -3076));
  // lfs f0,10772(r11)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r11) + 10772);
  f0 = double(temp.f32);
  // lfs f13,0(r16)
  temp.u32 = guest::LoadU32(uint32_t(r16) + 0);
  f13 = double(temp.f32);
  // fcmpu cr6,f13,f0
  cr6 = lifted::CompareFloat(f13, f0);
  // bgt cr6,0x82131268
  if (cr6.gt) goto loc_82131268;
  // lbz r11,0(r25)
  r11 = guest::LoadU8(uint32_t(r25) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x821314bc
  if (cr0.eq) goto loc_821314BC;
loc_82131268:
  // lwz r3,88(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 88);
  // lfs f1,40(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r29) + 40);
  f1 = double(temp.f32);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,88(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 88);
  // lfs f1,44(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r29) + 44);
  f1 = double(temp.f32);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,40(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 40);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,88(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 88);
  // lfs f1,48(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r29) + 48);
  f1 = double(temp.f32);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,44(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 44);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,92(r29)
  r3 = guest::LoadU32(uint32_t(r29) + 92);
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x821312d8
  if (cr0.eq) goto loc_821312D8;
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lfs f3,72(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r29) + 72);
  f3 = double(temp.f32);
  // lfs f2,68(r29)
  temp.u32 = guest::LoadU32(uint32_t(r29) + 68);
  f2 = double(temp.f32);
  // lfs f1,64(r29)
  temp.u32 = guest::LoadU32(uint32_t(r29) + 64);
  f1 = double(temp.f32);
  // lwz r11,16(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_821312D8:
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // addis r23,r29,1
  r23 = uint64_t(int64_t(int64_t(r29) + 65536));
  // ori r11,r11,10580
  r11 = r11 | 10580;
  // addi r23,r23,10576
  r23 = uint64_t(int64_t(int64_t(r23) + 10576));
  // mr r27,r31
  r27 = r31;
  // lfsx f0,r29,r11
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r29) + uint32_t(r11));
  f0 = double(temp.f32);
  // lis r11,-32255
  r11 = uint64_t(int64_t(-2113863680));
  // lfs f12,-30756(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + -30756);
  f12 = double(temp.f32);
  // lwz r11,0(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 0);
  // fmuls f13,f0,f12
  f13 = double(float(f0 * f12));
  // std r11,96(r1)
  guest::StoreU64(uint32_t(r1) + 96, r11);
  // lfd f0,96(r1)
  f0 = std::bit_cast<double>(uint64_t(guest::LoadU64(uint32_t(r1) + 96)));
  // fcfid f0,f0
  f0 = double(std::bit_cast<int64_t>(f0));
  // frsp f0,f0
  f0 = double(float(f0));
  // fcmpu cr6,f0,f13
  cr6 = lifted::CompareFloat(f0, f13);
  // bge cr6,0x82131344
  if (!cr6.lt) goto loc_82131344;
  // lbz r11,26(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 26);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82131344
  if (!cr0.eq) goto loc_82131344;
  // fdivs f0,f0,f13
  f0 = double(float(f0 / f13));
  // lfs f13,76(r29)
  temp.u32 = guest::LoadU32(uint32_t(r29) + 76);
  f13 = double(temp.f32);
  // lis r11,-32256
  r11 = uint64_t(int64_t(-2113929216));
  // fadds f0,f0,f29
  f0 = double(float(f0 + f29));
  // fmuls f13,f0,f13
  f13 = double(float(f0 * f13));
  // lfs f0,7140(r11)
  temp.u32 = guest::LoadU32(uint32_t(r11) + 7140);
  f0 = double(temp.f32);
  // fmuls f31,f13,f0
  f31 = double(float(f13 * f0));
  // b 0x82131378
  goto loc_82131378;
loc_82131344:
  // lbz r11,26(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 26);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x8213135c
  if (!cr0.eq) goto loc_8213135C;
  // lfs f31,76(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r29) + 76);
  f31 = double(temp.f32);
  // mr r27,r28
  r27 = r28;
  // b 0x82131378
  goto loc_82131378;
loc_8213135C:
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // lfs f13,76(r29)
  lifted::ScalarFloatMode();
  temp.u32 = guest::LoadU32(uint32_t(r29) + 76);
  f13 = double(temp.f32);
  // ori r11,r11,10584
  r11 = r11 | 10584;
  // lfsx f11,r29,r11
  temp.u32 = guest::LoadU32(uint32_t(r29) + uint32_t(r11));
  f11 = double(temp.f32);
  // fmuls f12,f11,f12
  f12 = double(float(f11 * f12));
  // fdivs f0,f0,f12
  f0 = double(float(f0 / f12));
  // fmuls f31,f0,f13
  f31 = double(float(f0 * f13));
loc_82131378:
  // lbz r11,27(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 27);
  // lbz r10,25(r29)
  r10 = guest::LoadU8(uint32_t(r29) + 25);
  // cntlzw r11,r11
  r11 = uint32_t(r11) == 0 ? 32 : __builtin_clz(uint32_t(r11));
  // rlwinm r11,r11,27,31,31
  r11 = __builtin_rotateleft64(uint32_t(r11) | (r11 << 32), 27) & 0x1;
  // or r11,r11,r10
  r11 = r11 | r10;
  // clrlwi. r11,r11,24
  r11 = uint32_t(r11) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821313e8
  if (!cr0.eq) goto loc_821313E8;
  // mr r3,r29
  r3 = r29;
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x8212f538
  // (left out: dead in C++)
  { const lifted::Result o = lifted::f8212F538(r1, r13, r3, r4, r5, r6, r7, r8, f1); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r27,24
  r11 = uint32_t(r27) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x821313c0
  if (!cr0.eq) goto loc_821313C0;
  // lbz r10,26(r29)
  r10 = guest::LoadU8(uint32_t(r29) + 26);
  // cmplwi r10,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r10), 0, so);
  // bne 0x821313c0
  if (!cr0.eq) goto loc_821313C0;
  // lwz r11,0(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 0);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // b 0x821313e4
  goto loc_821313E4;
loc_821313C0:
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x821313e8
  if (!cr6.eq) goto loc_821313E8;
  // lbz r11,26(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 26);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x821313e8
  if (cr0.eq) goto loc_821313E8;
  // lwz r11,0(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 0);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x821313e8
  if (cr0.eq) goto loc_821313E8;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
loc_821313E4:
  // stw r11,0(r23)
  guest::StoreU32(uint32_t(r23) + 0, uint32_t(r11));
loc_821313E8:
  // addis r27,r29,1
  r27 = uint64_t(int64_t(int64_t(r29) + 65536));
  // addi r27,r27,-5440
  r27 = uint64_t(int64_t(int64_t(r27) + -5440));
  // lwz r11,0(r27)
  r11 = guest::LoadU32(uint32_t(r27) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq cr6,0x82131478
  if (cr6.eq) goto loc_82131478;
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r17,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r17));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r26,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r26));
  // lwz r10,0(r27)
  r10 = guest::LoadU32(uint32_t(r27) + 0);
  // addi r11,r29,96
  r11 = uint64_t(int64_t(int64_t(r29) + 96));
  // sth r24,6(r3)
  guest::StoreU16(uint32_t(r3) + 6, uint16_t(r24));
  // stw r11,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r11));
  // sth r10,4(r3)
  guest::StoreU16(uint32_t(r3) + 4, uint16_t(r10));
loc_82131478:
  // lbz r11,26(r29)
  r11 = guest::LoadU8(uint32_t(r29) + 26);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82131490
  if (cr0.eq) goto loc_82131490;
  // mr r3,r29
  r3 = r29;
  // fmr f1,f31
  lifted::ScalarFloatMode();
  f1 = f31;
  // bl 0x8212f778
  // (left out: dead in C++)
  { const lifted::Result o = lifted::f8212F778(r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
loc_82131490:
  // lwz r11,0(r23)
  r11 = guest::LoadU32(uint32_t(r23) + 0);
  // cmplwi cr6,r11,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne cr6,0x821314bc
  if (!cr6.eq) goto loc_821314BC;
  // lis r11,1
  r11 = uint64_t(int64_t(65536));
  // stfs f28,0(r16)
  lifted::ScalarFloatMode();
  temp.f32 = float(f28);
  guest::StoreU32(uint32_t(r16) + 0, temp.u32);
  // stb r31,24(r29)
  guest::StoreU8(uint32_t(r29) + 24, uint8_t(r31));
  // ori r11,r11,62444
  r11 = r11 | 62444;
  // stb r31,0(r25)
  guest::StoreU8(uint32_t(r25) + 0, uint8_t(r31));
  // stw r31,0(r23)
  guest::StoreU32(uint32_t(r23) + 0, uint32_t(r31));
  // stb r31,26(r29)
  guest::StoreU8(uint32_t(r29) + 26, uint8_t(r31));
  // stfsx f28,r29,r11
  temp.f32 = float(f28);
  guest::StoreU32(uint32_t(r29) + uint32_t(r11), temp.u32);
loc_821314BC:
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r28,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r28));
  // stw r26,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r26));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r19,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r19));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r20,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r20));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r21,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r21));
  // stw r28,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r28));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // stw r22,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r22));
  // stw r18,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r18));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // bl 0x8258caa0
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CAA0, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // li r11,36
  r11 = uint64_t(int64_t(36));
  // stw r31,4(r3)
  guest::StoreU32(uint32_t(r3) + 4, uint32_t(r31));
  // stw r31,8(r3)
  guest::StoreU32(uint32_t(r3) + 8, uint32_t(r31));
  // stw r11,0(r3)
  guest::StoreU32(uint32_t(r3) + 0, uint32_t(r11));
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // bl 0x8258ca50
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(0x8258CA50, LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r3,-21600(r30)
  r3 = guest::LoadU32(uint32_t(r30) + -21600);
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,32(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 32);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r11,8(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r3)
  r11 = guest::LoadU32(uint32_t(r3) + 0);
  // lwz r5,84(r29)
  r5 = guest::LoadU32(uint32_t(r29) + 84);
  // fmr f1,f28
  lifted::ScalarFloatMode();
  f1 = f28;
  // mr r4,r15
  r4 = r15;
  // lwz r11,16(r11)
  r11 = guest::LoadU32(uint32_t(r11) + 16);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(uint32_t(uint32_t(ctr)), LIFTED_ARGS); r3 = o.r3; f1 = o.f1; }
loc_82131668:
  // stb r31,27(r29)
  guest::StoreU8(uint32_t(r29) + 27, uint8_t(r31));
  // addi r1,r1,288
  r1 = uint64_t(int64_t(int64_t(r1) + 288));
  // addi r12,r1,-144
  r12 = uint64_t(int64_t(int64_t(r1) + -144));
  // bl 0x825a7964
  // (left out: dead in C++)
  // (left out: dead in C++)
  // b 0x825a6ea4
  // (left out: dead in C++)
  return {r3, f1};
}

}  // namespace screen_objects
