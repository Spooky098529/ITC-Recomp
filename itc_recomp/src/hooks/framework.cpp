// The app framework natively: adapters in front of src/framework/
// (docs/functions/framework.md). The getters and the thunks are reached
// through the framework's vtables (and the folded getters also from other
// classes' code), so their adapters stay while recompiled code calls them.
// Word64 (sub_82090778), Word44 (sub_824892F8) and DebugFont::SetPosition
// (sub_8258BE08) have none: in play only the thunks' and
// DebugText::SetPosition's plain C++ call them.

#include "guest.h"
#include "lifted/functions.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "base/flags.h"
#include "framework/framework.h"
#include "framework/main_loop.h"
#include "framework/run_loop.h"
#include "hooks/app_start_up.h"
#include "hooks/framework.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "schema/framework.h"
#include "schema/input.h"

#include <optional>
#include "lifted/sdk.h"

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
              "the constructors, with --native_heap, Framework::Create sub_820909B8 (with "
              "Framework::Framework sub_82090420, no adapter) and DebugText::Create sub_82091160 "
              "(with DebugText::DebugText sub_82090EA8, no adapter), the debug text's load job "
              "DebugText::LoadFont sub_82090CC0 (with --native_file_layer; DebugFont::Init "
              "sub_8258BD58, no adapter, and sub_8258BE48's store; the open by name, File::Read "
              "and Close and XPR::Create guest calls), FrameStart::FrameStart "
              "sub_82092710 and the "
              "PoolMakers sub_820AC8F8 and sub_820AC848, their calls guest calls; the entry and "
              "the main loop, main sub_82090108, sub_82090378, sub_8209ABE8, App::Run "
              "sub_8209A760, ForEachTickHandler sub_82099D50 and ForEachRequest sub_82099EE8, "
              "their virtual calls and functors guest calls) natively "
              "instead of "
              "through the translated code (the same results; checked by "
              "--run_harnesses=framework)");

ITC_FLAG_DECLARE(bool, native_file_layer);

namespace fw = framework;

#define ITC_FRAMEWORK_FALLBACK(address)  \
  REX_FUNC_PROLOGUE();                   \
  if (!ITC_FLAG(native_framework)) {     \
    lifted::original::c##address(ctx); \
    return;                              \
  }

// A function of r3 answering a word in r3.
#define ITC_FRAMEWORK_WORD(address, function) \
  ITC_NATIVE_ENTRY_ALWAYS(address) {        \
    ITC_FRAMEWORK_FALLBACK(address)           \
    ctx.r3.u64 = fw::function(ctx.r3.u32); \
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
// as the original leaves them (and r11, the font's position: not carried,
// none reads it).
ITC_NATIVE_ENTRY_ALWAYS(82091050) {
  ITC_FRAMEWORK_FALLBACK(82091050)
  const uint32_t x = schema::DebugTextCoordinate(ctx.r4.u32).value();
  const uint32_t y = schema::DebugTextCoordinate(ctx.r5.u32).value();
  fw::DebugTextSetPosition(ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = x;
  ctx.r4.u64 = y;
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

fw::Iterator IteratorAt(uint32_t address) {
  const schema::HandlerMapIterator it(address);
  return {it.map().address, it.node().address};
}

}  // namespace

// The map iterator's operator++: r3 = the iterator; r3 left.
ITC_NATIVE_ENTRY_ALWAYS(82099B18) {
  ITC_FRAMEWORK_FALLBACK(82099B18)
  fw::Iterator it = IteratorAt(ctx.r3.u32);
  if (!fw::MapIncrement(it)) {
    lifted::original::c82099B18(ctx);
    return;
  }
  schema::HandlerMapIterator(ctx.r3.u32).set_node(it.node);
}

// HandlerMap: r3 = the map; r3 the head.
ITC_NATIVE_ENTRY_ALWAYS(82099FB0) {
  
  // The app's start-up harness's recorder first (hooks/app_start_up.h:
  // App::App's callees).
  if (hooks::StartUpRecorded(ctx, 0x82099FB0)) {
    return;
  }
  if (!NativeHeap() || !fw::CanAllocate()) {
    lifted::original::c82099FB0(ctx);
    return;
  }
  ctx.r3.u64 = fw::MapConstruct(ctx.r3.u32);
}

// Insert: r3 = the result (an iterator and the inserted byte), r4 = the
// map, r5 = the value; r3 the result.
ITC_NATIVE_ENTRY_ALWAYS(82093740) {
  
  std::optional<fw::InsertResult> result;
  if (NativeHeap()) {
    result = fw::MapInsertUnique(ctx.r4.u32, ctx.r5.u32);
  }
  if (!result) {
    lifted::original::c82093740(ctx);
    return;
  }
  const schema::HandlerMapIterator answer(ctx.r3.u32);
  answer.set_map(result->where.container);
  answer.set_node(result->where.node);
  answer.set_inserted(result->inserted ? 1 : 0);
}

// RequestVector::Clear: r3 = the vector; r3 the result in its 96-byte frame
// (r1 - 16), as the original leaves it.
ITC_NATIVE_ENTRY_ALWAYS(8209A080) {
  ITC_FRAMEWORK_FALLBACK(8209A080)
  if (!fw::RequestsClear(ctx.r3.u32)) {
    lifted::original::c8209A080(ctx);
    return;
  }
  ctx.r3.u64 = ctx.r1.u32 - 16;
}

// RequestVector::PushBack: r3 = the vector, r4 = the request; r3 the
// vector, or Insert's result in its 96-byte frame (r1 - 16) when it took
// Insert, as the original leaves it.
ITC_NATIVE_ENTRY_ALWAYS(8209A930) {
  
  std::optional<bool> inserted;
  if (NativeHeap()) {
    inserted = fw::RequestsPushBack(ctx.r3.u32, ctx.r4.u32);
  }
  if (!inserted) {
    lifted::original::c8209A930(ctx);
    return;
  }
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 16;
  }
}

// RunLoop::ApplyRequest: r3 = the run loop, r4 = the type (high word) and
// key, r5 = the delegate (high word), both also stored in the caller's
// frame (r1 + 32, + 40) as the original does; r3 HandlerMap::Insert's
// result in its 128-byte frame (r1 - 32) when it inserted, else left.
ITC_NATIVE_ENTRY_ALWAYS(8209A458) {
  
  std::optional<bool> inserted;
  if (NativeHeap()) {
    inserted = fw::RunLoopApplyRequest(ctx.r3.u32, uint32_t(ctx.r4.u64 >> 32), ctx.r4.u32,
                                       uint32_t(ctx.r5.u64 >> 32));
  }
  if (!inserted) {
    lifted::original::c8209A458(ctx);
    return;
  }
  guest::StoreU64(ctx.r1.u32 + 32, ctx.r4.u64);
  guest::StoreU64(ctx.r1.u32 + 40, ctx.r5.u64);
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 32;
  }
}

// --- The thunks and dispatchers (guest calls into the targets) ---

namespace {

// A tail call: r3 = the object, the target through ctr, lr left.
void TailCall(lifted::Regs& ctx, fw::VirtualCall call) {
  ctx.r3.u64 = call.object;
  ctx.ctr.u64 = call.target;
  lifted::CallFromContext(ctx, ctx.ctr.u32);
}

}  // namespace

bool hooks::NativeFramework() { return ITC_FLAG(native_framework); }

void hooks::TickHandlerInvoke(lifted::Regs& ctx) {
  const uint32_t loop = ctx.r3.u32;
  const uint32_t delegate = ctx.r4.u32;
  guest::StoreU64(ctx.r1.u32 + 24, ctx.r4.u64);
  const uint32_t target = fw::TickHandlerTarget(delegate);
  guest_call::InFrame(ctx, 96, 0x82099AC4, [&](uint32_t) {
    ctx.r3.u64 = delegate;
    ctx.r4.u64 = loop;
    ctx.ctr.u64 = target;
    lifted::CallFromContext(ctx, ctx.ctr.u32);
  });
}

void hooks::DelegateSlot1Thunk(lifted::Regs& ctx) {
  const uint32_t target = fw::DelegateSlot1Target(ctx.r3.u32);
  TailCall(ctx, {ctx.r3.u32, target});
}

// The delegate thunk sub_82096330 (FrameStart's tick handler 4): r3 = the
// object; its slot 0 as a tail call.
ITC_NATIVE_ENTRY_ALWAYS(82096330) {
  ITC_FRAMEWORK_FALLBACK(82096330)
  TailCall(ctx, {ctx.r3.u32, fw::DelegateSlot0Target(ctx.r3.u32)});
}

// Framework::Start: r3 = the framework; the app's slot +36 as a tail call.
ITC_NATIVE_ENTRY_ALWAYS(820902B0) {
  ITC_FRAMEWORK_FALLBACK(820902B0)
  TailCall(ctx, fw::FrameworkStartCall(ctx.r3.u32));
}

namespace {

// The app's input source: InputManagerFrom36's plain C++ when that's the
// target (in play), else a tail call.
void AppInputSource(lifted::Regs& ctx, fw::VirtualCall call) {
  if (call.target == 0x82090830) {
    ctx.r3.u64 = fw::InputManagerFrom36(call.object);
    return;
  }
  TailCall(ctx, call);
}

}  // namespace

// sub_82094350: r3 = app + 4; r3 the input source's answer.
ITC_NATIVE_ENTRY_ALWAYS(82094350) {
  ITC_FRAMEWORK_FALLBACK(82094350)
  AppInputSource(ctx, fw::AppInputSourceCall(ctx.r3.u32));
}

// sub_820944C8: r3 = app + 16; as sub_82094350 from app + 4.
ITC_NATIVE_ENTRY_ALWAYS(820944C8) {
  ITC_FRAMEWORK_FALLBACK(820944C8)
  AppInputSource(ctx, fw::AppInputSourceFrom16Call(ctx.r3.u32));
}

// Framework::OnTick: r3 = the framework, r4 = the run loop; its guest calls
// from a 128-byte frame like the original's (the key at r1 + 80 there), with
// the original's return addresses. r3 as the last call leaves it.
ITC_NATIVE_ENTRY_ALWAYS(820902C8) {
  ITC_FRAMEWORK_FALLBACK(820902C8)
  const uint32_t framework = ctx.r3.u32;
  const uint32_t loop = ctx.r4.u32;
  guest_call::InFrame(ctx, 128, 0x820902FC, [&](uint32_t frame) {
    // `target` reads the object's vtable slot (after r3 and r4 are set).
    auto call = [&](uint32_t object, const auto& target, uint32_t r4, uint32_t return_address) {
      ctx.r3.u64 = object;
      ctx.r4.u64 = r4;
      ctx.ctr.u64 = target();
      ctx.lr = return_address;
      lifted::CallFromContext(ctx, ctx.ctr.u32);
      return ctx.r3.u32;
    };
    const fw::OnTickCalls calls{
        [&](uint32_t manager, uint32_t key) {
          // InputManager::GetProcessor (the input manager's slot 1, input.md;
          // input.toml's InputManager has no vtable type to name it).
          call(manager,
               [&] { return guest::LoadU32(schema::InputManager(manager).vtable() + 4); },
               key, 0x820902FC);
        },
        [&](uint32_t app) {
          ctx.r3.u64 = app;
          ctx.ctr.u64 = schema::FrameworkAppInterface(app).vtable().is_running();
          ctx.lr = 0x82090314;
          lifted::CallFromContext(ctx, ctx.ctr.u32);
          return ctx.r3.u32;
        },
        [&](uint32_t run_loop, uint32_t key) {
          call(run_loop,
               [&] { return schema::RunLoop(run_loop).vtable().remove_tick_handler(); }, key,
               0x8209033C);
        }};
    fw::FrameworkOnTick(framework, loop, frame + 80, calls);
  });
}

namespace {

// AddTickHandler and AddListener: r3 = the run loop, r4 = the key's
// address, r5 = the delegate; the copy made with its slot 2 and the request
// pushed, from a 112-byte frame like the original's (the request at r1 + 80
// there). r3 as PushBack leaves it.
void AddRequest(lifted::Regs& ctx, uint32_t type, uint32_t clone_return,
                uint32_t push_return) {
  const uint32_t loop = ctx.r3.u32;
  const uint32_t key = ctx.r4.u32;
  const uint32_t delegate = ctx.r5.u32;
  guest_call::InFrame(ctx, 112, clone_return, [&](uint32_t frame) {
    const uint32_t request = frame + 80;
    const auto clone = [&](uint32_t object) {
      ctx.r3.u64 = object;
      ctx.ctr.u64 = schema::RunLoopDelegate(object).vtable().clone();
      ctx.lr = clone_return;
      lifted::CallFromContext(ctx, ctx.ctr.u32);
      return ctx.r3.u32;
    };
    const auto inserted = fw::RunLoopAddRequest(loop, type, key, delegate, request, clone);
    if (!inserted) {
      ctx.r3.u64 = schema::RunLoop(loop).requests_address();
      ctx.r4.u64 = request;
      ctx.lr = push_return;
      lifted::original::c8209A930(ctx);
      return;
    }
    ctx.r3.u64 = *inserted ? frame - 16 : schema::RunLoop(loop).requests_address();
  });
}

}  // namespace

// RunLoop::AddTickHandler (vtable slot 2) and AddListener (slot 1).
ITC_NATIVE_ENTRY_ALWAYS(8209AAA8) {
  
  if (!NativeHeap()) {
    lifted::original::c8209AAA8(ctx);
    return;
  }
  AddRequest(ctx, 0, 0x8209AAE0, 0x8209AAF0);
}

ITC_NATIVE_ENTRY_ALWAYS(8209AB48) {
  
  if (!NativeHeap()) {
    lifted::original::c8209AB48(ctx);
    return;
  }
  AddRequest(ctx, 2, 0x8209AB80, 0x8209AB90);
}

// --- The framework's and its services' constructors ---
//
// Framework::Framework and DebugText::DebugText have no adapters: in play
// only Framework::Create's and DebugText::Create's plain C++ call them; nor
// has Framework::Create, called only from main's plain C++
// (hooks::MainLoopCalls).

namespace {

// A guest call of `object`'s vtable slot (`target` reads it, after r3 is
// set) with the original's return address; r3 its answer.
template <typename Target>
uint32_t VirtualCallFrom(lifted::Regs& ctx, uint32_t object, const Target& target,
                         uint32_t return_address) {
  ctx.r3.u64 = object;
  ctx.ctr.u64 = target();
  ctx.lr = return_address;
  lifted::CallFromContext(ctx, ctx.ctr.u32);
  return ctx.r3.u32;
}

// The PoolMakers: r3 = the maker, r4 = the interface; its slot 1 from a
// 96-byte frame; r3 the maker.
void PoolMaker(lifted::Regs& ctx, uint32_t vtable, uint32_t return_address) {
  const uint32_t maker = ctx.r3.u32;
  const uint32_t interface = ctx.r4.u32;
  guest_call::InFrame(ctx, 96, return_address, [&](uint32_t) {
    fw::PoolMakerConstruct(maker, vtable, interface, [&](uint32_t object) {
      return VirtualCallFrom(
          ctx, object,
          [&] { return schema::FrameworkInterface(object).vtable().slot_1(); },
          return_address);
    });
  });
  ctx.r3.u64 = maker;
}

}  // namespace

ITC_NATIVE_ENTRY_ALWAYS(820AC8F8) {
  ITC_FRAMEWORK_FALLBACK(820AC8F8)
  PoolMaker(ctx, 0x82002848, 0x820AC92C);
}

ITC_NATIVE_ENTRY_ALWAYS(820AC848) {
  ITC_FRAMEWORK_FALLBACK(820AC848)
  PoolMaker(ctx, 0x82002840, 0x820AC87C);
}

// FrameStart::FrameStart: r3 = it, r4 = the block; the run loop's slots
// from a 144-byte frame (the key and delegates there, as the original's);
// r3 it.
ITC_NATIVE_ENTRY_ALWAYS(82092710) {
  ITC_FRAMEWORK_FALLBACK(82092710)
  const uint32_t frame_start = ctx.r3.u32;
  const uint32_t block = ctx.r4.u32;
  guest_call::InFrame(ctx, 144, 0x82092780, [&](uint32_t frame) {
    fw::FrameStartConstruct(frame_start, block, frame,
        [&](uint32_t loop, uint32_t offset, uint32_t key, uint32_t delegate) {
          ctx.r4.u64 = key;
          ctx.r5.u64 = delegate;
          const auto slot = [&] {
            const schema::RunLoopVtable vtable = schema::RunLoop(loop).vtable();
            return offset == 8 ? vtable.add_tick_handler() : vtable.add_listener();
          };
          VirtualCallFrom(ctx, loop, slot, offset == 8 ? 0x82092780 : 0x820927CC);
        });
  });
  ctx.r3.u64 = frame_start;
}

// DebugText::Create: r3 = the block; operator new(52) and the constructor,
// its post from the constructor's 160-byte frame below Create's 96-byte one
// (the stack word read there); r3 the debug text or 0.
ITC_NATIVE_ENTRY_ALWAYS(82091160) {
  
  if (!NativeHeap()) {
    lifted::original::c82091160(ctx);
    return;
  }
  const uint32_t stack_word = guest::LoadU32(ctx.r1.u32 - 96 - 160 + 100);
  const auto text = fw::DebugTextCreate(ctx.r3.u32, stack_word, [&](uint32_t interface, uint32_t job) {
        guest_call::InFrame(ctx, 96, 0x8209118C, [&](uint32_t) {
          guest_call::InFrame(ctx, 160, 0x82090F78, [&](uint32_t) {
            ctx.r4.u64 = job;
            ctx.r5.u64 = 1;
            VirtualCallFrom(
                ctx, interface,
                [&] { return schema::WorkerPoolInterface(interface).vtable().post(); },
                0x82090F78);
          });
        });
      });
  if (!text) {
    lifted::original::c82091160(ctx);
    return;
  }
  ctx.r3.u64 = *text;
}

// The debug font's guest calls (framework::GuestCall) through `ctx`: r1,
// r3 to r5 and lr set, the function run through the function table (its
// override), r1 put back.
namespace {
fw::GuestCall DebugFontCall(lifted::Regs& ctx) {
  return [&ctx](uint32_t function, uint32_t r1, uint32_t lr, uint64_t r3, uint64_t r4,
                      uint64_t r5) -> uint64_t {
    const uint64_t stack = ctx.r1.u64;
    ctx.r1.u64 = r1;
    ctx.lr = lr;
    ctx.r3.u64 = r3;
    ctx.r4.u64 = r4;
    ctx.r5.u64 = r5;
    lifted::CallFromContext(ctx, function);
    ctx.r1.u64 = stack;
    return ctx.r3.u64;
  };
}
}  // namespace

void hooks::DebugTextLoadFontSteps(lifted::Regs& ctx) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = fw::DebugTextLoadFont(ctx.r3.u32, ctx.r1.u32, DebugFontCall(ctx));
  ctx.lr = lr;
}

void hooks::DebugFontInitSteps(lifted::Regs& ctx) {
  const uint64_t lr = ctx.lr;
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = fw::DebugFontInit(ctx.r3.u32, ctx.r1.u32, DebugFontCall(ctx));
  ctx.lr = lr;
}

// DebugText::LoadFont: r3 = the debug text (the job's delegate calls it;
// no direct callers). DebugFont::Init has no adapter: only its plain C++
// calls it.
ITC_NATIVE_ENTRY_ALWAYS(82090CC0) {
  
  if (!NativeHeap() || !ITC_FLAG(native_file_layer)) {
    lifted::original::c82090CC0(ctx);
    return;
  }
  hooks::DebugTextLoadFontSteps(ctx);
}

// --- The entry and the main loop (src/framework/main_loop.h) ---
//
// Only main has an adapter (xstart, recompiled, calls it); sub_82090378,
// sub_8209ABE8, App::Run, ForEachRequest, ForEachTickHandler (whose other
// caller, the run loop's event send sub_8209A008, never ran) and
// RunLoop::RunLoop are reached in play only from main's plain C++, so the
// flag switches them through main. Their register steps stay for the
// harness.

fw::MainLoopCalls hooks::MainLoopCalls(lifted::Regs& ctx) {
  fw::MainLoopCalls calls;
  calls.call = [&ctx](uint32_t target, uint32_t r3, std::optional<uint64_t> r4,
                            std::optional<uint64_t> r5, uint32_t return_address) {
    ctx.r3.u64 = r3;
    if (r4) {
      ctx.r4.u64 = *r4;
    }
    if (r5) {
      ctx.r5.u64 = *r5;
    }
    ctx.ctr.u64 = target;
    ctx.lr = return_address;
    lifted::CallFromContext(ctx, ctx.ctr.u32);
    return ctx.r3.u32;
  };
  calls.in_frame = [&ctx](uint32_t bytes, const std::function<void(uint32_t)>& body) {
    // (r14 to r31 were kept and put back around the body: nothing to keep
    // without a context)
    guest_call::InFrame(ctx, bytes, uint32_t(ctx.lr), body);
  };
  // The registers set are the original's nonvolatile ones, which reached
  // callees only as the bytes they save: not carried by the view.
  calls.set_register = [](int, uint64_t) {};
  calls.increment = [&ctx](uint32_t iterator) {
    ctx.r3.u64 = iterator;
    ctx.lr = 0x82099DFC;
    lifted::original::c82099B18(ctx);
  };
  calls.clear = [&ctx](uint32_t vector) {
    ctx.r3.u64 = vector;
    ctx.lr = 0x8209A870;
    lifted::original::c8209A080(ctx);
  };
  calls.construct_run_loop = [&ctx](uint32_t loop) {
    if (NativeHeap() && fw::CanAllocate()) {
      fw::RunLoopConstruct(loop);
      return;
    }
    ctx.r3.u64 = loop;
    ctx.lr = 0x8209AC04;
    lifted::original::c8209A9F0(ctx);
  };
  calls.create_framework = [&ctx](uint32_t holder) {
    if (NativeHeap() && fw::FrameworkCreate(holder)) {
      return;
    }
    ctx.r3.u64 = holder;
    ctx.lr = 0x82090120;
    lifted::original::c820909B8(ctx);
  };
  calls.destroy_run_loop = [&ctx](uint32_t loop) {
    ctx.r3.u64 = loop;
    ctx.lr = 0x8209AC18;
    lifted::Enter(ctx, lifted::e8209A6E0);
  };
  calls.delete_framework = [&ctx](uint32_t holder) {
    ctx.r3.u64 = holder;
    ctx.lr = 0x82090130;
    lifted::Enter(ctx, lifted::e82090350);
  };
  return calls;
}

void hooks::ForEachTickHandler(lifted::Regs& ctx) {
  const uint32_t result = ctx.r3.u32;
  const uint64_t first = ctx.r4.u64, last = ctx.r5.u64, functor = ctx.r6.u64;
  const fw::MainLoopCalls calls = MainLoopCalls(ctx);
  guest_call::InFrame(ctx, 128, uint32_t(ctx.lr), [&](uint32_t frame) {
    fw::ForEachTickHandler(result, first, last, functor, frame, calls);
  });
  ctx.r3.u64 = result;
}

void hooks::ForEachRequest(lifted::Regs& ctx) {
  const uint32_t result = ctx.r3.u32;
  const uint64_t first = ctx.r4.u64, last = ctx.r5.u64, functor = ctx.r6.u64;
  const fw::MainLoopCalls calls = MainLoopCalls(ctx);
  guest_call::InFrame(ctx, 128, uint32_t(ctx.lr), [&](uint32_t frame) {
    fw::ForEachRequest(result, first, last, functor, frame, calls);
  });
  ctx.r3.u64 = result;
}

void hooks::AppRun(lifted::Regs& ctx) {
  const uint32_t loop = ctx.r3.u32;
  const uint32_t framework = ctx.r4.u32;
  const fw::MainLoopCalls calls = MainLoopCalls(ctx);
  uint32_t answer = 0;
  guest_call::InFrame(ctx, 208, uint32_t(ctx.lr), [&](uint32_t frame) {
    answer = fw::AppRun(loop, framework, frame, calls);
  });
  ctx.r3.u64 = answer;
}

void hooks::RunFramework(lifted::Regs& ctx) {
  const uint32_t framework = ctx.r3.u32;
  const fw::MainLoopCalls calls = MainLoopCalls(ctx);
  uint32_t answer = 0;
  guest_call::InFrame(ctx, 160, uint32_t(ctx.lr), [&](uint32_t frame) {
    answer = fw::RunFramework(framework, frame, calls);
  });
  ctx.r3.u64 = answer;
}

void hooks::RunFrameworkAt(lifted::Regs& ctx) {
  const uint32_t holder = ctx.r3.u32;
  const fw::MainLoopCalls calls = MainLoopCalls(ctx);
  ctx.r3.u64 = fw::RunFrameworkAt(holder, calls);
}

void hooks::Main(lifted::Regs& ctx) {
  const fw::MainLoopCalls calls = MainLoopCalls(ctx);
  uint32_t answer = 0;
  guest_call::InFrame(ctx, 112, uint32_t(ctx.lr),
                      [&](uint32_t frame) { answer = fw::Main(frame, calls); });
  ctx.r3.u64 = answer;
}

// main: the guest's entry on the main thread (from xstart); r3 the run's
// answer (never, in play).
ITC_NATIVE_ENTRY_ALWAYS(82090108) {
  ITC_FRAMEWORK_FALLBACK(82090108)
  hooks::Main(ctx);
}
