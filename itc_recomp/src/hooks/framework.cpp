// The app framework natively: adapters in front of src/framework/
// (docs/functions/framework.md). The getters and the thunks are reached
// through the framework's vtables (and the folded getters also from other
// classes' code), so their adapters stay while recompiled code calls them.
// Word64 (sub_82090778), Word44 (sub_824892F8) and DebugFont::SetPosition
// (sub_8258BE08) have none: in play only the thunks' and
// DebugText::SetPosition's plain C++ call them.

#include "guest.h"

#include "base/flags.h"
#include "framework/framework.h"
#include "framework/run_loop.h"
#include "hooks/framework.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"

#include <optional>

ITC_FLAG_BOOL(native_framework, true, "ITC",
              "Run the app framework (docs/functions/framework.md): the framework's flags "
              "sub_82090760 and sub_82090768, the folded getters sub_82090770, sub_82090788 and "
              "sub_8248F030 (and without adapters, as only the thunks call them, sub_82090778 and "
              "sub_824892F8), the interfaces' thunks sub_82090828, "
              "sub_82090830, sub_82090838, sub_82090860, sub_82090868, sub_820908E0, sub_820908E8 "
              "and sub_82090908, DebugText::SetPosition sub_82091050 with DebugFont::SetPosition "
              "sub_8258BE08 (no adapter: only its plain C++ calls it); the run loop's handler maps "
              "(the iterator's increment sub_82099B18, and with --native_heap "
              "HandlerMap::HandlerMap sub_82099FB0 and Insert sub_82093740; without adapters, as "
              "only those call them: the decrement sub_82091AE8, RotateLeft sub_82092B10, "
              "RotateRight sub_82092A00, AllocateNodes sub_82099C80, NewHead sub_82099E88, NewNode "
              "sub_82099E20 and InsertNode sub_82093400) and request vector (Clear sub_8209A080, "
              "with Erase sub_82092EF8, no adapter; with --native_heap PushBack sub_8209A930, and "
              "without adapters, as only it calls them, Insert sub_8209A600 and InsertN "
              "sub_8209A0E8), RunLoop::RunLoop sub_8209A9F0 and "
              "ApplyRequest sub_8209A458 (with --native_heap); the thunks and dispatchers with "
              "their targets as guest calls: the delegate thunks sub_82096330 and sub_82096340, "
              "TickHandler::Invoke sub_82099A98, Framework::Start sub_820902B0 and OnTick "
              "sub_820902C8, the app's input source sub_820944C8 and sub_82094350, and with "
              "--native_heap RunLoop::AddTickHandler sub_8209AAA8 and AddListener sub_8209AB48; "
              "the constructors Framework::Framework sub_82090420, with --native_heap "
              "Framework::Create sub_820909B8 and DebugText::Create sub_82091160, "
              "DebugText::DebugText sub_82090EA8, FrameStart::FrameStart sub_82092710 and the "
              "PoolMakers sub_820AC8F8 and sub_820AC848, their calls guest calls) natively "
              "instead of "
              "through the translated code (the same results; checked by "
              "--run_harnesses=framework)");

namespace fw = framework;

#define ITC_FRAMEWORK_FALLBACK(address)  \
  REX_FUNC_PROLOGUE();                   \
  if (!ITC_FLAG(native_framework)) {     \
    __imp__sub_##address(ctx, base);     \
    return;                              \
  }

// A function of r3 answering a word in r3.
#define ITC_FRAMEWORK_WORD(address, function) \
  extern "C" REX_FUNC(sub_##address) {        \
    ITC_FRAMEWORK_FALLBACK(address)           \
    ctx.r3.u64 = fw::function(base, ctx.r3.u32); \
  }

ITC_FRAMEWORK_WORD(82090760, Flag133)
ITC_FRAMEWORK_WORD(82090768, Flag134)
ITC_FRAMEWORK_WORD(82090770, Word60)
ITC_FRAMEWORK_WORD(82090788, Word88)
ITC_FRAMEWORK_WORD(8248F030, Word68)
ITC_FRAMEWORK_WORD(82090830, InputManagerFrom36)
ITC_FRAMEWORK_WORD(82090828, RendererFrom52)
ITC_FRAMEWORK_WORD(82090838, WorkerPoolFrom52)
ITC_FRAMEWORK_WORD(82090860, RendererOrTextFrom24)
ITC_FRAMEWORK_WORD(82090868, SceneFrom52)
ITC_FRAMEWORK_WORD(820908E0, RendererFrom36)
ITC_FRAMEWORK_WORD(820908E8, WorkerPoolFrom36)
ITC_FRAMEWORK_WORD(82090908, SceneFrom36)

// DebugText::SetPosition: r4, r5 = pointers to x and y; r3, r4 the words
// and r11 as the original leaves them.
extern "C" REX_FUNC(sub_82091050) {
  ITC_FRAMEWORK_FALLBACK(82091050)
  const uint32_t x = guest::LoadU32(base, ctx.r4.u32);
  const uint32_t y = guest::LoadU32(base, ctx.r5.u32);
  fw::DebugTextSetPosition(base, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = x;
  ctx.r4.u64 = y;
  ctx.r11.u64 = fw::kDebugFontPosition;
}

// --- The run loop's containers (src/framework/run_loop.h) ---
//
// Only the functions recompiled code calls in play have adapters (the
// iterator's increment, HandlerMap::HandlerMap, Insert and Clear); the
// decrement, the rotations, AllocateNodes, NewHead, NewNode, InsertNode,
// Erase, and the request vector's Insert and InsertN are reached only from
// their plain C++ callers, and the flag switches those.

namespace {

// The functions that allocate: only over the allocation layer.
bool NativeHeap() { return ITC_FLAG(native_framework) && hooks::NativeHeapActive(); }

fw::Iterator IteratorAt(uint8_t* base, uint32_t address) {
  return {guest::LoadU32(base, address), guest::LoadU32(base, address + 4)};
}

}  // namespace

// The map iterator's operator++: r3 = the iterator; r3 left.
extern "C" REX_FUNC(sub_82099B18) {
  ITC_FRAMEWORK_FALLBACK(82099B18)
  fw::Iterator it = IteratorAt(base, ctx.r3.u32);
  if (!fw::MapIncrement(base, it)) {
    __imp__sub_82099B18(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 4, it.node);
}

// HandlerMap: r3 = the map; r3 the head.
extern "C" REX_FUNC(sub_82099FB0) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap() || !fw::CanAllocate(base)) {
    __imp__sub_82099FB0(ctx, base);
    return;
  }
  ctx.r3.u64 = fw::MapConstruct(base, ctx.r3.u32);
}

// Insert: r3 = the result (an iterator and the inserted byte), r4 = the
// map, r5 = the value; r3 the result.
extern "C" REX_FUNC(sub_82093740) {
  REX_FUNC_PROLOGUE();
  std::optional<fw::InsertResult> result;
  if (NativeHeap()) {
    result = fw::MapInsertUnique(base, ctx.r4.u32, ctx.r5.u32);
  }
  if (!result) {
    __imp__sub_82093740(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r3.u32 + 0, result->where.container);
  guest::StoreU32(base, ctx.r3.u32 + 4, result->where.node);
  guest::StoreU8(base, ctx.r3.u32 + 8, result->inserted ? 1 : 0);
}

// RequestVector::Clear: r3 = the vector; r3 the result in its 96-byte frame
// (r1 - 16), as the original leaves it.
extern "C" REX_FUNC(sub_8209A080) {
  ITC_FRAMEWORK_FALLBACK(8209A080)
  if (!fw::RequestsClear(base, ctx.r3.u32)) {
    __imp__sub_8209A080(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u32 - 16;
}

// RequestVector::PushBack: r3 = the vector, r4 = the request; r3 the
// vector, or Insert's result in its 96-byte frame (r1 - 16) when it took
// Insert, as the original leaves it.
extern "C" REX_FUNC(sub_8209A930) {
  REX_FUNC_PROLOGUE();
  std::optional<bool> inserted;
  if (NativeHeap()) {
    inserted = fw::RequestsPushBack(base, ctx.r3.u32, ctx.r4.u32);
  }
  if (!inserted) {
    __imp__sub_8209A930(ctx, base);
    return;
  }
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 16;
  }
}

// RunLoop::RunLoop: r3 = the run loop; r3 left.
extern "C" REX_FUNC(sub_8209A9F0) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap() || !fw::CanAllocate(base)) {
    __imp__sub_8209A9F0(ctx, base);
    return;
  }
  fw::RunLoopConstruct(base, ctx.r3.u32);
}

// RunLoop::ApplyRequest: r3 = the run loop, r4 = the type (high word) and
// key, r5 = the delegate (high word), both also stored in the caller's
// frame (r1 + 32, + 40) as the original does; r3 HandlerMap::Insert's
// result in its 128-byte frame (r1 - 32) when it inserted, else left.
extern "C" REX_FUNC(sub_8209A458) {
  REX_FUNC_PROLOGUE();
  std::optional<bool> inserted;
  if (NativeHeap()) {
    inserted = fw::RunLoopApplyRequest(base, ctx.r3.u32, uint32_t(ctx.r4.u64 >> 32), ctx.r4.u32,
                                       uint32_t(ctx.r5.u64 >> 32));
  }
  if (!inserted) {
    __imp__sub_8209A458(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r4.u64);
  REX_STORE_U64(ctx.r1.u32 + 40, ctx.r5.u64);
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 32;
  }
}

// --- The thunks and dispatchers (guest calls into the targets) ---

namespace {

// A tail call: r3 = the object, the target through ctr, lr left.
void TailCall(PPCContext& ctx, uint8_t* base, fw::VirtualCall call) {
  ctx.r3.u64 = call.object;
  ctx.ctr.u64 = call.target;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
}

}  // namespace

bool hooks::NativeFramework() { return ITC_FLAG(native_framework); }

void hooks::TickHandlerInvoke(PPCContext& ctx, uint8_t* base) {
  const uint32_t loop = ctx.r3.u32;
  const uint32_t delegate = ctx.r4.u32;
  REX_STORE_U64(ctx.r1.u32 + 24, ctx.r4.u64);
  const uint32_t target = fw::TickHandlerTarget(base, delegate);
  guest_call::InFrame(ctx, base, 96, 0x82099AC4, [&](uint32_t) {
    ctx.r3.u64 = delegate;
    ctx.r4.u64 = loop;
    ctx.ctr.u64 = target;
    REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  });
}

void hooks::DelegateSlot1Thunk(PPCContext& ctx, uint8_t* base) {
  const uint32_t target = fw::DelegateSlot1Target(base, ctx.r3.u32);
  TailCall(ctx, base, {ctx.r3.u32, target});
}

// The delegate thunk sub_82096330 (FrameStart's tick handler 4): r3 = the
// object; its slot 0 as a tail call.
extern "C" REX_FUNC(sub_82096330) {
  ITC_FRAMEWORK_FALLBACK(82096330)
  TailCall(ctx, base, {ctx.r3.u32, fw::DelegateSlot0Target(base, ctx.r3.u32)});
}

// Framework::Start: r3 = the framework; the app's slot +36 as a tail call.
extern "C" REX_FUNC(sub_820902B0) {
  ITC_FRAMEWORK_FALLBACK(820902B0)
  TailCall(ctx, base, fw::FrameworkStartCall(base, ctx.r3.u32));
}

namespace {

// The app's input source: InputManagerFrom36's plain C++ when that's the
// target (in play), else a tail call.
void AppInputSource(PPCContext& ctx, uint8_t* base, fw::VirtualCall call) {
  if (call.target == 0x82090830) {
    ctx.r3.u64 = fw::InputManagerFrom36(base, call.object);
    return;
  }
  TailCall(ctx, base, call);
}

}  // namespace

// sub_82094350: r3 = app + 4; r3 the input source's answer.
extern "C" REX_FUNC(sub_82094350) {
  ITC_FRAMEWORK_FALLBACK(82094350)
  AppInputSource(ctx, base, fw::AppInputSourceCall(base, ctx.r3.u32));
}

// sub_820944C8: r3 = app + 16; as sub_82094350 from app + 4.
extern "C" REX_FUNC(sub_820944C8) {
  ITC_FRAMEWORK_FALLBACK(820944C8)
  AppInputSource(ctx, base, fw::AppInputSourceFrom16Call(base, ctx.r3.u32));
}

// Framework::OnTick: r3 = the framework, r4 = the run loop; its guest calls
// from a 128-byte frame like the original's (the key at r1 + 80 there), with
// the original's return addresses. r3 as the last call leaves it.
extern "C" REX_FUNC(sub_820902C8) {
  ITC_FRAMEWORK_FALLBACK(820902C8)
  const uint32_t framework = ctx.r3.u32;
  const uint32_t loop = ctx.r4.u32;
  guest_call::InFrame(ctx, base, 128, 0x820902FC, [&](uint32_t frame) {
    auto call = [&](uint32_t object, uint32_t offset, uint32_t r4, uint32_t return_address) {
      ctx.r3.u64 = object;
      ctx.r4.u64 = r4;
      ctx.ctr.u64 = fw::VirtualTarget(base, object, offset);
      ctx.lr = return_address;
      REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
      return ctx.r3.u32;
    };
    const fw::OnTickCalls calls{
        [&](uint32_t manager, uint32_t key) { call(manager, 4, key, 0x820902FC); },
        [&](uint32_t app) {
          ctx.r3.u64 = app;
          ctx.ctr.u64 = fw::VirtualTarget(base, app, 44);
          ctx.lr = 0x82090314;
          REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
          return ctx.r3.u32;
        },
        [&](uint32_t run_loop, uint32_t key) { call(run_loop, 16, key, 0x8209033C); }};
    fw::FrameworkOnTick(base, framework, loop, frame + 80, calls);
  });
}

namespace {

// AddTickHandler and AddListener: r3 = the run loop, r4 = the key's
// address, r5 = the delegate; the copy made with its slot 2 and the request
// pushed, from a 112-byte frame like the original's (the request at r1 + 80
// there). r3 as PushBack leaves it.
void AddRequest(PPCContext& ctx, uint8_t* base, uint32_t type, uint32_t clone_return,
                uint32_t push_return) {
  const uint32_t loop = ctx.r3.u32;
  const uint32_t key = ctx.r4.u32;
  const uint32_t delegate = ctx.r5.u32;
  guest_call::InFrame(ctx, base, 112, clone_return, [&](uint32_t frame) {
    const uint32_t request = frame + 80;
    const auto clone = [&](uint32_t object) {
      ctx.r3.u64 = object;
      ctx.ctr.u64 = fw::VirtualTarget(base, object, 8);
      ctx.lr = clone_return;
      REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
      return ctx.r3.u32;
    };
    const auto inserted = fw::RunLoopAddRequest(base, loop, type, key, delegate, request, clone);
    if (!inserted) {
      ctx.r3.u64 = loop + fw::kRequests;
      ctx.r4.u64 = request;
      ctx.lr = push_return;
      __imp__sub_8209A930(ctx, base);
      return;
    }
    ctx.r3.u64 = *inserted ? frame - 16 : loop + fw::kRequests;
  });
}

}  // namespace

// RunLoop::AddTickHandler (vtable slot 2) and AddListener (slot 1).
extern "C" REX_FUNC(sub_8209AAA8) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap()) {
    __imp__sub_8209AAA8(ctx, base);
    return;
  }
  AddRequest(ctx, base, 0, 0x8209AAE0, 0x8209AAF0);
}

extern "C" REX_FUNC(sub_8209AB48) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap()) {
    __imp__sub_8209AB48(ctx, base);
    return;
  }
  AddRequest(ctx, base, 2, 0x8209AB80, 0x8209AB90);
}

// --- The framework's and its services' constructors ---

// Framework::Framework: r3 = the framework; r3 left.
extern "C" REX_FUNC(sub_82090420) {
  ITC_FRAMEWORK_FALLBACK(82090420)
  fw::FrameworkConstruct(base, ctx.r3.u32);
}

// Framework::Create: r3 = the holder; r3 left.
extern "C" REX_FUNC(sub_820909B8) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap() || !fw::FrameworkCreate(base, ctx.r3.u32)) {
    __imp__sub_820909B8(ctx, base);
  }
}

namespace {

// A guest call of `object`'s slot at `offset` with the original's return
// address; r3 its answer.
uint32_t VirtualCallFrom(PPCContext& ctx, uint8_t* base, uint32_t object, uint32_t offset,
                         uint32_t return_address) {
  ctx.r3.u64 = object;
  ctx.ctr.u64 = fw::VirtualTarget(base, object, offset);
  ctx.lr = return_address;
  REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
  return ctx.r3.u32;
}

// The PoolMakers: r3 = the maker, r4 = the interface; its slot 1 from a
// 96-byte frame; r3 the maker.
void PoolMaker(PPCContext& ctx, uint8_t* base, uint32_t vtable, uint32_t return_address) {
  const uint32_t maker = ctx.r3.u32;
  const uint32_t interface = ctx.r4.u32;
  guest_call::InFrame(ctx, base, 96, return_address, [&](uint32_t) {
    fw::PoolMakerConstruct(base, maker, vtable, interface, [&](uint32_t object) {
      return VirtualCallFrom(ctx, base, object, 4, return_address);
    });
  });
  ctx.r3.u64 = maker;
}

// DebugText::DebugText from the caller's frame at ctx.r1: its 160-byte
// frame, the stack word it reads there (+100), the job's post.
uint32_t DebugTextIn(PPCContext& ctx, uint8_t* base, uint32_t text, uint32_t block) {
  const uint32_t stack_word = guest::LoadU32(base, ctx.r1.u32 - 160 + 100);
  guest_call::InFrame(ctx, base, 160, 0x82090F78, [&](uint32_t) {
    fw::DebugTextConstruct(base, text, block, stack_word, [&](uint32_t interface, uint32_t job) {
      ctx.r4.u64 = job;
      ctx.r5.u64 = 1;
      VirtualCallFrom(ctx, base, interface, 4, 0x82090F78);
    });
  });
  return text;
}

}  // namespace

extern "C" REX_FUNC(sub_820AC8F8) {
  ITC_FRAMEWORK_FALLBACK(820AC8F8)
  PoolMaker(ctx, base, 0x82002848, 0x820AC92C);
}

extern "C" REX_FUNC(sub_820AC848) {
  ITC_FRAMEWORK_FALLBACK(820AC848)
  PoolMaker(ctx, base, 0x82002840, 0x820AC87C);
}

// FrameStart::FrameStart: r3 = it, r4 = the block; the run loop's slots
// from a 144-byte frame (the key and delegates there, as the original's);
// r3 it.
extern "C" REX_FUNC(sub_82092710) {
  ITC_FRAMEWORK_FALLBACK(82092710)
  const uint32_t frame_start = ctx.r3.u32;
  const uint32_t block = ctx.r4.u32;
  guest_call::InFrame(ctx, base, 144, 0x82092780, [&](uint32_t frame) {
    fw::FrameStartConstruct(
        base, frame_start, block, frame,
        [&](uint32_t loop, uint32_t offset, uint32_t key, uint32_t delegate) {
          ctx.r4.u64 = key;
          ctx.r5.u64 = delegate;
          VirtualCallFrom(ctx, base, loop, offset, offset == 8 ? 0x82092780 : 0x820927CC);
        });
  });
  ctx.r3.u64 = frame_start;
}

// DebugText::DebugText: r3 = it, r4 = the block; r3 it.
extern "C" REX_FUNC(sub_82090EA8) {
  ITC_FRAMEWORK_FALLBACK(82090EA8)
  ctx.r3.u64 = DebugTextIn(ctx, base, ctx.r3.u32, ctx.r4.u32);
}

// DebugText::Create: r3 = the block; operator new(52) and the constructor,
// its post from the constructor's 160-byte frame below Create's 96-byte one
// (the stack word read there); r3 the debug text or 0.
extern "C" REX_FUNC(sub_82091160) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap()) {
    __imp__sub_82091160(ctx, base);
    return;
  }
  const uint32_t stack_word = guest::LoadU32(base, ctx.r1.u32 - 96 - 160 + 100);
  const auto text = fw::DebugTextCreate(
      base, ctx.r3.u32, stack_word, [&](uint32_t interface, uint32_t job) {
        guest_call::InFrame(ctx, base, 96, 0x8209118C, [&](uint32_t) {
          guest_call::InFrame(ctx, base, 160, 0x82090F78, [&](uint32_t) {
            ctx.r4.u64 = job;
            ctx.r5.u64 = 1;
            VirtualCallFrom(ctx, base, interface, 4, 0x82090F78);
          });
        });
      });
  if (!text) {
    __imp__sub_82091160(ctx, base);
    return;
  }
  ctx.r3.u64 = *text;
}
