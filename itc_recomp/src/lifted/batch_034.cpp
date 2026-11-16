// Lift sweep batch 034 (docs/loops/lift-sweep.md): written by
// itc_recomp/tools/lift.py from generated/. Don't edit it by hand: run the
// tool again.

#include "lifted/entries.h"
#include "lifted/functions.h"
#include "lifted/names.h"
#include "lifted/sdk.h"

#include "base/flags.h"
#include "base/log.h"

#include <bit>
#include <cstdlib>

#include "helpers/clock.h"

ITC_FLAG_BOOL(native_lifted_034, true, "ITC",
              "Run lift sweep batch 034's functions as lifted C++ (src/lifted/, "
              "docs/loops/lift-sweep.md) instead of the recompiled ones.");

// A call by address or an import passes every argument register and its
// return address and r31 (the per-object recorder's pairing key reads it).
#define LIFTED_ARGS(lr, r31) \
  lifted::Args { r1, r13, {r3, r4, r5, r6, r7, r8, r9, r10}, \
                 {f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13}, {}, uint32_t(lr), \
                 uint32_t(r31) }

namespace lifted {

// sub_82095AF0: lifted from generated/ by lift.py.
lifted::Result f82095AF0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_82095AF0 (lifted)");
  uint64_t r11 = 0, r12 = 0, r26 = 0, r27 = 0, r28 = 0, r29 = 0, r30 = 0, r31 = 0, ctr = 0, reserved = 0, msr = 0;
  double f0 = 0.0, f2 = 0.0, f3 = 0.0, f4 = 0.0, f5 = 0.0, f6 = 0.0, f7 = 0.0, f8 = 0.0, f9 = 0.0, f10 = 0.0, f11 = 0.0, f12 = 0.0, f13 = 0.0, f31 = 0.0;
  lifted::Cr cr0, cr6;
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
  // stwu r1,-240(r1)
  ea = -240 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // lis r11,-32160
  r11 = uint64_t(int64_t(-2107637760));
  // mr r31,r3
  r31 = r3;
  // lwz r11,776(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 776);
  // cmpwi cr6,r11,0
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne cr6,0x82095bf0
  if (!cr6.eq) goto loc_82095BF0;
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258dcc8
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Create(base, r1, r13, r3, r7, r8, r9, r10); r3 = o.r3; f1 = o.f1; }
  // addi r29,r31,28
  r29 = uint64_t(int64_t(int64_t(r31) + 28));
  // lwz r11,248(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 248);
  // mr r3,r29
  r3 = r29;
  // lwz r10,0(r29)
  r10 = REX_LOAD_U32(uint32_t(r29) + 0);
  // stw r11,248(r31)
  REX_STORE_U32(uint32_t(r31) + 248, uint32_t(r11));
  // lwz r11,8(r10)
  r11 = REX_LOAD_U32(uint32_t(r10) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095B3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,248(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 248);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,248(r31)
  REX_STORE_U32(uint32_t(r31) + 248, uint32_t(r11));
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // beq 0x82095bd8
  if (cr0.eq) goto loc_82095BD8;
  // addi r28,r31,24
  r28 = uint64_t(int64_t(int64_t(r31) + 24));
loc_82095B54:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r30
  r3 = r30;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095B6C, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x8258dd78
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Signal(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095B88, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095B9C, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // bl 0x8258dd78
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Signal(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // lwz r11,248(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 248);
  // mr r3,r29
  r3 = r29;
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,248(r31)
  REX_STORE_U32(uint32_t(r31) + 248, uint32_t(r11));
  // lwz r11,0(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 0);
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095BC4, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,248(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 248);
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stw r11,248(r31)
  REX_STORE_U32(uint32_t(r31) + 248, uint32_t(r11));
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bne 0x82095b54
  if (!cr0.eq) goto loc_82095B54;
loc_82095BD8:
  // lwz r11,248(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 248);
  // addi r3,r1,80
  r3 = uint64_t(int64_t(int64_t(r1) + 80));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stw r11,248(r31)
  REX_STORE_U32(uint32_t(r31) + 248, uint32_t(r11));
  // bl 0x8258dd60
  // (left out: dead in C++)
  { const lifted::Result o = f8258DD60(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, f1, 0x82095BEC); r3 = o.r3; f1 = o.f1; }
  // b 0x82095dec
  goto loc_82095DEC;
loc_82095BF0:
  // cmpwi cr6,r11,1
  cr6 = lifted::Compare<int32_t>(int32_t(r11), 1, so);
  // bne cr6,0x82095dec
  if (!cr6.eq) goto loc_82095DEC;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x8258cfa0
  // (left out: dead in C++)
  { const lifted::Result o = f8258CFA0(base, r1, r13, r3, r4, r5, r6, r7, r8); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x8258d170
  // (left out: dead in C++)
  r3 = f8258D170(base, r1, r13, r3);
  // addi r27,r31,248
  r27 = uint64_t(int64_t(int64_t(r31) + 248));
loc_82095C0C:
  // mfmsr r10
  std::atomic_thread_fence(std::memory_order_seq_cst);
  r10 = lifted::CheckGlobalLock();
  // mtmsrd r13,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r13) & 0x8020) | (msr & ~0x8020);
  lifted::EnterGlobalLock();
  // lwarx r11,0,r27
  ea = uint32_t(r27);
  reserved = (reserved & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(*(uint32_t*)REX_RAW_ADDR(ea)));
  r11 = __builtin_bswap32(uint32_t(reserved));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stwcx. r11,0,r27
  ea = uint32_t(r27);
  cr0.lt = 0;
  cr0.gt = 0;
  cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(REX_RAW_ADDR(ea)), int32_t(reserved), __builtin_bswap32(int32_t(r11)));
  cr0.so = so;
  // mtmsrd r10,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r10) & 0x8020) | (msr & ~0x8020);
  lifted::LeaveGlobalLock();
  // bne 0x82095c0c
  if (!cr0.eq) goto loc_82095C0C;
  // addi r29,r31,76
  r29 = uint64_t(int64_t(int64_t(r31) + 76));
  // addi r26,r1,88
  r26 = uint64_t(int64_t(int64_t(r1) + 88));
  // addi r28,r31,108
  r28 = uint64_t(int64_t(int64_t(r31) + 108));
loc_82095C34:
  // mfmsr r10
  std::atomic_thread_fence(std::memory_order_seq_cst);
  r10 = lifted::CheckGlobalLock();
  // mtmsrd r13,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r13) & 0x8020) | (msr & ~0x8020);
  lifted::EnterGlobalLock();
  // lwarx r11,0,r27
  ea = uint32_t(r27);
  reserved = (reserved & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(*(uint32_t*)REX_RAW_ADDR(ea)));
  r11 = __builtin_bswap32(uint32_t(reserved));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stwcx. r11,0,r27
  ea = uint32_t(r27);
  cr0.lt = 0;
  cr0.gt = 0;
  cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(REX_RAW_ADDR(ea)), int32_t(reserved), __builtin_bswap32(int32_t(r11)));
  cr0.so = so;
  // mtmsrd r10,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r10) & 0x8020) | (msr & ~0x8020);
  lifted::LeaveGlobalLock();
  // bne 0x82095c34
  if (!cr0.eq) goto loc_82095C34;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x8258d0d8
  // (left out: dead in C++)
  { const lifted::Result o = f8258D0D8(base, r1, r13, r3); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // fmr f31,f1
  lifted::ScalarFloatMode();
  f31 = f1;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095C64); r3 = o.r3; f1 = o.f1; }
  // lfs f0,252(r31)
  lifted::ScalarFloatMode();
  temp.u32 = REX_LOAD_U32(uint32_t(r31) + 252);
  f0 = double(temp.f32);
  // lwz r11,84(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 84);
  // fadds f0,f0,f31
  f0 = double(float(f0 + f31));
  // stfs f0,252(r31)
  temp.f32 = float(f0);
  REX_STORE_U32(uint32_t(r31) + 252, temp.u32);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // bne 0x82095cb0
  if (!cr0.eq) goto loc_82095CB0;
  // addi r30,r31,100
  r30 = uint64_t(int64_t(int64_t(r31) + 100));
loc_82095C80:
  // mr r3,r28
  r3 = r28;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095C88); r3 = o.r3; f1 = o.f1; }
  // li r11,-1
  r11 = uint64_t(int64_t(-1));
  // addi r4,r1,80
  r4 = uint64_t(int64_t(int64_t(r1) + 80));
  // mr r3,r30
  r3 = r30;
  // stw r11,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r11));
  // bl 0x8258dd90
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Wait(base, r1, r13, r3, r4, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095CA4); r3 = o.r3; f1 = o.f1; }
  // lwz r11,84(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 84);
  // cmplwi r11,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r11), 0, so);
  // beq 0x82095c80
  if (cr0.eq) goto loc_82095C80;
loc_82095CB0:
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // rotlwi r10,r11,0
  r10 = __builtin_rotateleft32(uint32_t(r11), 0);
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // cmplw cr6,r11,r10
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), uint32_t(r10), so);
  // bne cr6,0x82095cc8
  if (!cr6.eq) goto loc_82095CC8;
  // twi 31,r0,22
  lifted::Trap(22);
loc_82095CC8:
  // lwz r30,8(r11)
  r30 = REX_LOAD_U32(uint32_t(r11) + 8);
  // addi r3,r1,104
  r3 = uint64_t(int64_t(int64_t(r1) + 104));
  // lwz r11,4(r29)
  r11 = REX_LOAD_U32(uint32_t(r29) + 4);
  // mr r4,r29
  r4 = r29;
  // stw r29,88(r1)
  REX_STORE_U32(uint32_t(r1) + 88, uint32_t(r29));
  // stw r30,80(r1)
  REX_STORE_U32(uint32_t(r1) + 80, uint32_t(r30));
  // lwz r11,0(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 0);
  // stw r11,92(r1)
  REX_STORE_U32(uint32_t(r1) + 92, uint32_t(r11));
  // ld r5,0(r26)
  r5 = REX_LOAD_U64(uint32_t(r26) + 0);
  // bl 0x82098e80
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Erase(base, LIFTED_ARGS(0x82095CF0, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r28
  r3 = r28;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095CF8); r3 = o.r3; f1 = o.f1; }
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x8258d090
  // (left out: dead in C++)
  r3 = f8258D090(base, r1, r13, r3);
loc_82095D00:
  // mfmsr r10
  std::atomic_thread_fence(std::memory_order_seq_cst);
  r10 = lifted::CheckGlobalLock();
  // mtmsrd r13,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r13) & 0x8020) | (msr & ~0x8020);
  lifted::EnterGlobalLock();
  // lwarx r11,0,r27
  ea = uint32_t(r27);
  reserved = (reserved & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(*(uint32_t*)REX_RAW_ADDR(ea)));
  r11 = __builtin_bswap32(uint32_t(reserved));
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // stwcx. r11,0,r27
  ea = uint32_t(r27);
  cr0.lt = 0;
  cr0.gt = 0;
  cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(REX_RAW_ADDR(ea)), int32_t(reserved), __builtin_bswap32(int32_t(r11)));
  cr0.so = so;
  // mtmsrd r10,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r10) & 0x8020) | (msr & ~0x8020);
  lifted::LeaveGlobalLock();
  // bne 0x82095d00
  if (!cr0.eq) goto loc_82095D00;
  // cmplwi cr6,r30,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r30), 0, so);
  // beq cr6,0x82095dc0
  if (cr6.eq) goto loc_82095DC0;
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // li r4,0
  r4 = uint64_t(int64_t(0));
  // mr r3,r30
  r3 = r30;
  // lwz r11,8(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 8);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095D3C, r31)); r3 = o.r3; f1 = o.f1; }
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,24(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 24);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095D50, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r3,24
  r11 = uint32_t(r3) & 0xFF;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // bne 0x82095d6c
  if (!cr0.eq) goto loc_82095D6C;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095D6C, r31)); r3 = o.r3; f1 = o.f1; }
loc_82095D6C:
  // lwz r11,0(r30)
  r11 = REX_LOAD_U32(uint32_t(r30) + 0);
  // mr r3,r30
  r3 = r30;
  // lwz r11,12(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 12);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x82095D80, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r30,r31,136
  r30 = uint64_t(int64_t(int64_t(r31) + 136));
  // mr r3,r30
  r3 = r30;
  // bl 0x825a6d28
  // (left out: dead in C++)
  { const lifted::Result o = game::EnterCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095D8C); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,88
  r3 = uint64_t(int64_t(int64_t(r31) + 88));
  // addi r11,r1,96
  r11 = uint64_t(int64_t(int64_t(r1) + 96));
  // addi r5,r1,80
  r5 = uint64_t(int64_t(int64_t(r1) + 80));
  // lwz r10,4(r3)
  r10 = REX_LOAD_U32(uint32_t(r3) + 4);
  // stw r3,96(r1)
  REX_STORE_U32(uint32_t(r1) + 96, uint32_t(r3));
  // stw r10,100(r1)
  REX_STORE_U32(uint32_t(r1) + 100, uint32_t(r10));
  // ld r4,0(r11)
  r4 = REX_LOAD_U64(uint32_t(r11) + 0);
  // bl 0x82095958
  // (left out: dead in C++)
  { const lifted::Result o = game::PtrList::Insert(base, LIFTED_ARGS(0x82095DAC, r31)); r3 = o.r3; f1 = o.f1; }
  // addi r3,r31,104
  r3 = uint64_t(int64_t(int64_t(r31) + 104));
  // bl 0x8258dd78
  // (left out: dead in C++)
  { const lifted::Result o = game::Event::Signal(base, r1, r13, r3, r5, r6, r7, r8, r9, r10, f1); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // bl 0x8258e578
  // (left out: dead in C++)
  { const lifted::Result o = game::LeaveCriticalSection(base, r1, r13, r3, r4, r5, r6, r7, r8, r9, r10, 0x82095DBC); r3 = o.r3; f1 = o.f1; }
  // b 0x82095c34
  goto loc_82095C34;
loc_82095DC0:
  // mfmsr r10
  std::atomic_thread_fence(std::memory_order_seq_cst);
  r10 = lifted::CheckGlobalLock();
  // mtmsrd r13,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r13) & 0x8020) | (msr & ~0x8020);
  lifted::EnterGlobalLock();
  // lwarx r11,0,r27
  ea = uint32_t(r27);
  reserved = (reserved & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(*(uint32_t*)REX_RAW_ADDR(ea)));
  r11 = __builtin_bswap32(uint32_t(reserved));
  // addi r11,r11,-1
  r11 = uint64_t(int64_t(int64_t(r11) + -1));
  // stwcx. r11,0,r27
  ea = uint32_t(r27);
  cr0.lt = 0;
  cr0.gt = 0;
  cr0.eq = __sync_bool_compare_and_swap(reinterpret_cast<uint32_t*>(REX_RAW_ADDR(ea)), int32_t(reserved), __builtin_bswap32(int32_t(r11)));
  cr0.so = so;
  // mtmsrd r10,1
  std::atomic_thread_fence(std::memory_order_seq_cst);
  msr = (uint32_t(r10) & 0x8020) | (msr & ~0x8020);
  lifted::LeaveGlobalLock();
  // bne 0x82095dc0
  if (!cr0.eq) goto loc_82095DC0;
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x8258d1e0
  // (left out: dead in C++)
  r3 = f8258D1E0(base, r1, r13, r3);
  // addi r3,r1,112
  r3 = uint64_t(int64_t(int64_t(r1) + 112));
  // bl 0x8258d020
  // (left out: dead in C++)
  f8258D020(base, r3);
loc_82095DEC:
  // addi r1,r1,240
  r1 = uint64_t(int64_t(int64_t(r1) + 240));
  // lfd f31,-64(r1)
  lifted::ScalarFloatMode();
  f31 = std::bit_cast<double>(uint64_t(REX_LOAD_U64(uint32_t(r1) + -64)));
  // b 0x825a6ed0
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822BE278 game::D3DXBuffer::Release: lifted from generated/ by lift.py.
lifted::Result f822BE278(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822BE278 (lifted)");
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
  // lwz r11,4(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 4);
  // addi r31,r11,-1
  r31 = uint64_t(int64_t(int64_t(r11) + -1));
  // cmplwi cr6,r11,1
  cr6 = lifted::Compare<uint32_t>(uint32_t(r11), 1, so);
  // stw r31,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r31));
  // bne cr6,0x822be2b0
  if (!cr6.eq) goto loc_822BE2B0;
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822BE2B0, r31)); r3 = o.r3; f1 = o.f1; }
loc_822BE2B0:
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

// sub_822BE450 game::D3DXCreateBuffer: lifted from generated/ by lift.py.
lifted::Result f822BE450(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822BE450 (lifted)");
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
  // stwu r1,-112(r1)
  ea = -112 + uint32_t(r1);
  REX_STORE_U32(ea, uint32_t(r1));
  r1 = (r1 & 0xFFFFFFFF00000000ull) | uint32_t(uint32_t(ea));
  // mr r29,r4
  r29 = r4;
  // mr r30,r3
  r30 = r3;
  // cmplwi cr6,r29,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r29), 0, so);
  // bne cr6,0x822be478
  if (!cr6.eq) goto loc_822BE478;
  // lis r3,-30602
  r3 = uint64_t(int64_t(-2005532672));
  // ori r3,r3,2156
  r3 = r3 | 2156;
  // b 0x822be514
  goto loc_822BE514;
loc_822BE478:
  // lis r4,9345
  r4 = uint64_t(int64_t(612433920));
  // li r3,16
  r3 = uint64_t(int64_t(16));
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x822BE484, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // beq 0x822be4b4
  if (cr0.eq) goto loc_822BE4B4;
  // lis r11,-32254
  r11 = uint64_t(int64_t(-2113798144));
  // li r10,0
  r10 = uint64_t(int64_t(0));
  // addi r11,r11,-25152
  r11 = uint64_t(int64_t(int64_t(r11) + -25152));
  // li r9,1
  r9 = uint64_t(int64_t(1));
  // mr r31,r3
  r31 = r3;
  // stw r10,12(r3)
  REX_STORE_U32(uint32_t(r3) + 12, uint32_t(r10));
  // stw r11,0(r3)
  REX_STORE_U32(uint32_t(r3) + 0, uint32_t(r11));
  // stw r10,8(r3)
  REX_STORE_U32(uint32_t(r3) + 8, uint32_t(r10));
  // stw r9,4(r3)
  REX_STORE_U32(uint32_t(r3) + 4, uint32_t(r9));
  // b 0x822be4b8
  goto loc_822BE4B8;
loc_822BE4B4:
  // li r31,0
  r31 = uint64_t(int64_t(0));
loc_822BE4B8:
  // cmplwi cr6,r31,0
  cr6 = lifted::Compare<uint32_t>(uint32_t(r31), 0, so);
  // bne cr6,0x822be4cc
  if (!cr6.eq) goto loc_822BE4CC;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x822be514
  goto loc_822BE514;
loc_822BE4CC:
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
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822BE4E4, r31)); r3 = o.r3; f1 = o.f1; }
  // mr. r30,r3
  r30 = r3;
  cr0 = lifted::Compare<int32_t>(int32_t(r30), 0, so);
  // bge 0x822be50c
  if (!cr0.lt) goto loc_822BE50C;
  // lwz r11,0(r31)
  r11 = REX_LOAD_U32(uint32_t(r31) + 0);
  // li r4,1
  r4 = uint64_t(int64_t(1));
  // mr r3,r31
  r3 = r31;
  // lwz r11,20(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 20);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822BE504, r31)); r3 = o.r3; f1 = o.f1; }
  // mr r3,r30
  r3 = r30;
  // b 0x822be514
  goto loc_822BE514;
loc_822BE50C:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r31,0(r29)
  REX_STORE_U32(uint32_t(r29) + 0, uint32_t(r31));
loc_822BE514:
  // addi r1,r1,112
  r1 = uint64_t(int64_t(int64_t(r1) + 112));
  // b 0x825a6edc
  // (left out: dead in C++)
  return {r3, f1};
}

// sub_822BE2D0: lifted from generated/ by lift.py.
lifted::Result f822BE2D0(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822BE2D0 (lifted)");
  uint64_t r5 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // mr r30,r4
  r30 = r4;
  // mr r31,r3
  r31 = r3;
  // lis r4,9345
  r4 = uint64_t(int64_t(612433920));
  // mr r3,r30
  r3 = r30;
  // bl 0x822976f8
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemAlloc(base, LIFTED_ARGS(0x822BE2F8, r31)); r3 = o.r3; f1 = o.f1; }
  // cmplwi r3,0
  cr0 = lifted::Compare<uint32_t>(uint32_t(r3), 0, so);
  // stw r3,12(r31)
  REX_STORE_U32(uint32_t(r31) + 12, uint32_t(r3));
  // bne 0x822be310
  if (!cr0.eq) goto loc_822BE310;
  // lis r3,-32761
  r3 = uint64_t(int64_t(-2147024896));
  // ori r3,r3,14
  r3 = r3 | 14;
  // b 0x822be318
  goto loc_822BE318;
loc_822BE310:
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // stw r30,8(r31)
  REX_STORE_U32(uint32_t(r31) + 8, uint32_t(r30));
loc_822BE318:
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

// sub_822BE3E8 game::D3DXBuffer::Dtor: lifted from generated/ by lift.py.
lifted::Result f822BE3E8(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, uint64_t r7, uint64_t r8, uint64_t r9, uint64_t r10, double f1) {
  ITC_NATIVE_RUNNING("sub_822BE3E8 (lifted)");
  uint64_t r11 = 0, r12 = 0, r30 = 0, r31 = 0;
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
  // lis r11,-32254
  r11 = uint64_t(int64_t(-2113798144));
  // mr r31,r3
  r31 = r3;
  // addi r11,r11,-25152
  r11 = uint64_t(int64_t(int64_t(r11) + -25152));
  // mr r30,r4
  r30 = r4;
  // lis r4,9345
  r4 = uint64_t(int64_t(612433920));
  // lwz r3,12(r31)
  r3 = REX_LOAD_U32(uint32_t(r31) + 12);
  // stw r11,0(r31)
  REX_STORE_U32(uint32_t(r31) + 0, uint32_t(r11));
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x822BE41C, r31)); r3 = o.r3; f1 = o.f1; }
  // clrlwi. r11,r30,31
  r11 = uint32_t(r30) & 0x1;
  cr0 = lifted::Compare<int32_t>(int32_t(r11), 0, so);
  // beq 0x822be430
  if (cr0.eq) goto loc_822BE430;
  // lis r4,9345
  r4 = uint64_t(int64_t(612433920));
  // mr r3,r31
  r3 = r31;
  // bl 0x82297790
  // (left out: dead in C++)
  { const lifted::Result o = game::XMemFree(base, LIFTED_ARGS(0x822BE430, r31)); r3 = o.r3; f1 = o.f1; }
loc_822BE430:
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

// sub_822BE330: lifted from generated/ by lift.py.
lifted::Result f822BE330(uint8_t* base, uint64_t r1, uint64_t r13, uint64_t r3, uint64_t r4, uint64_t r5, uint64_t r6, double f1) {
  ITC_NATIVE_RUNNING("sub_822BE330 (lifted)");
  uint64_t r7 = 0, r8 = 0, r9 = 0, r10 = 0, r11 = 0, r12 = 0, r31 = 0, ctr = 0;
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
  // li r9,0
  r9 = uint64_t(int64_t(0));
  // mr r11,r4
  r11 = r4;
  // lis r10,-32254
  r10 = uint64_t(int64_t(-2113798144));
  // addi r10,r10,-4888
  r10 = uint64_t(int64_t(int64_t(r10) + -4888));
  // stw r9,0(r5)
  REX_STORE_U32(uint32_t(r5) + 0, uint32_t(r9));
  // addi r9,r11,16
  r9 = uint64_t(int64_t(int64_t(r11) + 16));
loc_822BE354:
  // lbz r8,0(r11)
  r8 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r7,0(r10)
  r7 = REX_LOAD_U8(uint32_t(r10) + 0);
  // subf. r8,r7,r8
  r8 = r8 - r7;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x822be374
  if (!cr0.eq) goto loc_822BE374;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // bne cr6,0x822be354
  if (!cr6.eq) goto loc_822BE354;
loc_822BE374:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // beq 0x822be3b4
  if (cr0.eq) goto loc_822BE3B4;
  // mr r11,r4
  r11 = r4;
  // lis r10,-32254
  r10 = uint64_t(int64_t(-2113798144));
  // addi r9,r11,16
  r9 = uint64_t(int64_t(int64_t(r11) + 16));
  // addi r10,r10,-4920
  r10 = uint64_t(int64_t(int64_t(r10) + -4920));
loc_822BE38C:
  // lbz r8,0(r11)
  r8 = REX_LOAD_U8(uint32_t(r11) + 0);
  // lbz r7,0(r10)
  r7 = REX_LOAD_U8(uint32_t(r10) + 0);
  // subf. r8,r7,r8
  r8 = r8 - r7;
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x822be3ac
  if (!cr0.eq) goto loc_822BE3AC;
  // addi r11,r11,1
  r11 = uint64_t(int64_t(int64_t(r11) + 1));
  // addi r10,r10,1
  r10 = uint64_t(int64_t(int64_t(r10) + 1));
  // cmpw cr6,r11,r9
  cr6 = lifted::Compare<int32_t>(int32_t(r11), int32_t(r9), so);
  // bne cr6,0x822be38c
  if (!cr6.eq) goto loc_822BE38C;
loc_822BE3AC:
  // cmpwi r8,0
  cr0 = lifted::Compare<int32_t>(int32_t(r8), 0, so);
  // bne 0x822be3d0
  if (!cr0.eq) goto loc_822BE3D0;
loc_822BE3B4:
  // stw r3,0(r5)
  REX_STORE_U32(uint32_t(r5) + 0, uint32_t(r3));
  // lwz r11,0(r3)
  r11 = REX_LOAD_U32(uint32_t(r3) + 0);
  // lwz r11,4(r11)
  r11 = REX_LOAD_U32(uint32_t(r11) + 4);
  // mtctr r11
  ctr = r11;
  // bctrl
  // (left out: dead in C++)
  { const lifted::Result o = lifted::Call(base, uint32_t(uint32_t(ctr)), LIFTED_ARGS(0x822BE3C8, r31)); r3 = o.r3; f1 = o.f1; }
  // li r3,0
  r3 = uint64_t(int64_t(0));
  // b 0x822be3d8
  goto loc_822BE3D8;
loc_822BE3D0:
  // lis r3,-32768
  r3 = uint64_t(int64_t(-2147483648));
  // ori r3,r3,16386
  r3 = r3 | 16386;
loc_822BE3D8:
  // addi r1,r1,96
  r1 = uint64_t(int64_t(int64_t(r1) + 96));
  // lwz r12,-8(r1)
  r12 = REX_LOAD_U32(uint32_t(r1) + -8);
  // mtlr r12
  // (left out: dead in C++)
  // blr
  return {r3, f1};
}

lifted::Result e82095AF0(uint8_t* base, const lifted::Args& a) {
  return f82095AF0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822BE278(uint8_t* base, const lifted::Args& a) {
  return f822BE278(base, a.r1, a.r13, a.r[0], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822BE450(uint8_t* base, const lifted::Args& a) {
  return f822BE450(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822BE2D0(uint8_t* base, const lifted::Args& a) {
  return f822BE2D0(base, a.r1, a.r13, a.r[0], a.r[1], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822BE3E8(uint8_t* base, const lifted::Args& a) {
  return f822BE3E8(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.r[4], a.r[5], a.r[6], a.r[7], a.f[0]);
}

lifted::Result e822BE330(uint8_t* base, const lifted::Args& a) {
  return f822BE330(base, a.r1, a.r13, a.r[0], a.r[1], a.r[2], a.r[3], a.f[0]);
}

namespace {

const Registration kEntries[] = {
    {0x82095AF0, e82095AF0},
    {0x822BE278, e822BE278},
    {0x822BE450, e822BE450},
    {0x822BE2D0, e822BE2D0},
    {0x822BE3E8, e822BE3E8},
    {0x822BE330, e822BE330},
};

const bool kRegistered =
    (Register(kEntries, std::size(kEntries), &ITC_FLAG(native_lifted_034)), true);

}  // namespace

}  // namespace lifted
