// The app's update and its state stack natively: adapters in front of
// src/app_states/ (docs/functions/app-states.md), with the register steps the
// harness shares (hooks/app_states.h).

#include "hooks/app_states.h"

#include "app_states/app.h"
#include "app_states/containers.h"
#include "base/flags.h"
#include "debug/menu_selection_scan.h"
#include "framework/run_loop.h"
#include "hooks/guest_call.h"

#include <fmt/format.h>

#include <string>
#include "hooks/heap.h"
#include "std_list/ptr_list.h"

ITC_FLAG_BOOL(native_app_states, true, "ITC",
              "Run the app's update and its state stack (docs/functions/app-states.md): the "
              "message vector's Clear sub_82092F98 (with Erase sub_82092E70, no adapter: only "
              "Clear calls it) and Allocate sub_82092B78, the operation list's AllocateNodes "
              "sub_820ACF10 and IncSize sub_820AD768 (no adapters: only the plain C++ below "
              "calls them in play), List::Clear sub_820ACF88, PtrList::Erase "
              "sub_82098E80, Delegate::Clone sub_820ACA48 (those that allocate or free only "
              "while --native_heap routes the game's heap to the allocation layer) and "
              "StateLayer::IsEmpty sub_820ACE50; with --native_heap, OpList::OpList sub_820AD0B8, "
              "OpList::BuyNode sub_820ACFF8 (no adapter: only Push's plain C++ calls it in play), "
              "StateLayer::Push sub_820ADAD0 and App::SendMessage "
              "sub_82094AB0 (with MessageVector::PushBack sub_820944E0, Insert sub_82094110 and "
              "InsertN sub_820938C0, no adapters: only SendMessage reaches them); "
              "AppStates::IsEmpty sub_820AE1A0 and App::IsRunning sub_82092928, their virtual "
              "calls guest calls where the target isn't ours; HandlerMap::ForEach sub_82092BF0, "
              "App::ForEachMessage sub_82092CD0 and ForEachRequest sub_82091C48, "
              "App::RunHandler sub_82092968 and RunListener sub_820929B0, App::DeliverMessage "
              "sub_82092D50, their functors and slots guest calls; with --native_heap "
              "App::ApplyRequest sub_82093BE0 (its split-off type 0 piece sub_82093C2C merged) "
              "and the +20 interface's add slots sub_82094830, sub_820948D0, sub_82094970 and "
              "sub_82094A10; App::Update sub_82093DC0, the run loop's tick handler 8, its virtual "
              "calls, button tests and functors guest calls) natively instead of through the translated code "
              "(the same results; checked by --run_harnesses=app_states)");

namespace hooks {

namespace as = app_states;

namespace {

// The functions that allocate or free: only over the allocation layer.
bool Heap() { return hooks::NativeHeapActive(); }

uint32_t High(uint64_t value) { return uint32_t(value >> 32); }

}  // namespace

void AppMessagesErase(PPCContext& ctx, uint8_t* base) {
  if (!as::MessagesErase(base, ctx.r4.u32, High(ctx.r5.u64), ctx.r5.u32, High(ctx.r6.u64),
                         ctx.r6.u32)) {
    __imp__sub_82092E70(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r5.u64);
  REX_STORE_U64(ctx.r1.u32 + 40, ctx.r6.u64);
  REX_STORE_U64(ctx.r3.u32, ctx.r5.u64);
}

void AppMessagesClear(PPCContext& ctx, uint8_t* base) {
  if (!as::MessagesClear(base, ctx.r3.u32)) {
    __imp__sub_82092F98(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u32 - 16;
}

void AppMessagesAllocate(PPCContext& ctx, uint8_t* base) {
  if (Heap()) {
    if (const auto block = as::MessagesAllocate(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_82092B78(ctx, base);
}

void AppOpAllocateNodes(PPCContext& ctx, uint8_t* base) {
  if (Heap()) {
    if (const auto block = as::OpAllocateNodes(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_820ACF10(ctx, base);
}

void AppOpIncSize(PPCContext& ctx, uint8_t* base) {
  if (!as::OpIncSize(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820AD768(ctx, base);
  }
}

void AppListClear(PPCContext& ctx, uint8_t* base) {
  if (!Heap()) {
    __imp__sub_820ACF88(ctx, base);
    return;
  }
  ctx.r3.u64 = as::ListClear(base, ctx.r3.u32);
}

void AppPtrListErase(PPCContext& ctx, uint8_t* base) {
  std::optional<uint32_t> next;
  if (Heap()) {
    next = std_list::Erase(base, ctx.r4.u32, High(ctx.r5.u64), ctx.r5.u32);
  }
  if (!next) {
    __imp__sub_82098E80(ctx, base);
    return;
  }
  const uint64_t result = (ctx.r5.u64 & 0xFFFFFFFF00000000ull) | *next;
  REX_STORE_U64(ctx.r1.u32 + 32, result);
  REX_STORE_U64(ctx.r3.u32, result);
}

void AppDelegateClone(PPCContext& ctx, uint8_t* base) {
  if (Heap()) {
    if (const auto copy = as::DelegateClone(base, ctx.r3.u32)) {
      ctx.r3.u64 = *copy;
      return;
    }
  }
  __imp__sub_820ACA48(ctx, base);
}

void AppLayerIsEmpty(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = as::LayerIsEmpty(base, ctx.r3.u32);
}

as::AppCalls AppStatesCalls(PPCContext& ctx, uint8_t* base) {
  as::AppCalls calls;
  calls.call = [&ctx, base](uint32_t target, uint32_t r3, std::optional<uint64_t> r4,
                            std::optional<uint64_t> r5, uint32_t return_address) {
    ctx.r3.u64 = r3;
    if (r4) {
      ctx.r4.u64 = *r4;
    }
    if (r5) {
      ctx.r5.u64 = *r5;
    }
    ctx.ctr.u64 = target;
    if (return_address != 0) {
      ctx.lr = return_address;
    }
    REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
    return ctx.r3.u32;
  };
  calls.in_frame = [&ctx, base](uint32_t bytes, uint32_t return_address,
                                const std::function<void(uint32_t)>& body) {
    uint64_t kept[18];
    for (int i = 0; i < 18; i++) {
      kept[i] = (&ctx.r14)[i].u64;
    }
    guest_call::InFrame(ctx, base, bytes, return_address, body);
    for (int i = 0; i < 18; i++) {
      (&ctx.r14)[i].u64 = kept[i];
    }
  };
  calls.set_register = [&ctx](int reg, uint64_t value) { (&ctx.r0)[reg].u64 = value; };
  calls.increment = [&ctx, base](uint32_t iterator, uint32_t return_address) {
    ctx.r3.u64 = iterator;
    ctx.lr = return_address;
    __imp__sub_82099B18(ctx, base);
  };
  calls.clone = [&ctx, base](uint32_t delegate, uint32_t return_address) {
    const uint32_t target = as::VirtualTarget(base, delegate, 8);
    if (target == 0x820ACA48 && ITC_FLAG(native_app_states) && Heap()) {
      if (const auto copy = as::DelegateClone(base, delegate)) {
        ctx.r3.u64 = *copy;
        return *copy;
      }
    }
    ctx.r3.u64 = delegate;
    ctx.ctr.u64 = target;
    ctx.lr = return_address;
    REX_CALL_INDIRECT_FUNC(ctx.ctr.u32);
    return ctx.r3.u32;
  };
  return calls;
}

void AppHandlerMapForEach(PPCContext& ctx, uint8_t* base) {
  const uint32_t result = ctx.r3.u32;
  const uint64_t first = ctx.r4.u64, last = ctx.r5.u64, functor = ctx.r6.u64,
                 adjust = ctx.r7.u64;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(112, uint32_t(ctx.lr), [&](uint32_t frame) {
    as::HandlerMapForEach(base, result, first, last, functor, adjust, frame, calls);
  });
  ctx.r3.u64 = result;
}

void AppForEachMessage(PPCContext& ctx, uint8_t* base) {
  const uint32_t result = ctx.r3.u32;
  const uint64_t first = ctx.r4.u64, last = ctx.r5.u64, functor = ctx.r6.u64,
                 adjust = ctx.r7.u64;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(128, uint32_t(ctx.lr), [&](uint32_t frame) {
    as::ForEachMessage(base, result, first, last, functor, adjust, frame, calls);
  });
  ctx.r3.u64 = result;
}

void AppForEachRequest(PPCContext& ctx, uint8_t* base) {
  const uint32_t result = ctx.r3.u32;
  const uint64_t first = ctx.r4.u64, last = ctx.r5.u64, functor = ctx.r6.u64,
                 adjust = ctx.r7.u64;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(128, uint32_t(ctx.lr), [&](uint32_t frame) {
    as::ForEachRequest(base, result, first, last, functor, adjust, frame, calls);
  });
  ctx.r3.u64 = result;
}

namespace {

void RunHandlerStep(PPCContext& ctx, uint8_t* base, bool listener) {
  const uint32_t app = ctx.r3.u32;
  const uint64_t entry = ctx.r4.u64;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  uint32_t answer = 0;
  guest_call::InFrame(ctx, base, 96, uint32_t(ctx.lr), [&](uint32_t frame) {
    answer = as::RunHandler(base, app, entry, listener, frame, calls);
  });
  ctx.r3.u64 = answer;
}

}  // namespace

void AppRunHandler(PPCContext& ctx, uint8_t* base) { RunHandlerStep(ctx, base, false); }
void AppRunListener(PPCContext& ctx, uint8_t* base) { RunHandlerStep(ctx, base, true); }

void AppDeliverMessage(PPCContext& ctx, uint8_t* base) {
  const uint32_t app = ctx.r3.u32;
  const uint32_t message = ctx.r4.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(128, uint32_t(ctx.lr), [&](uint32_t frame) {
    as::DeliverMessage(base, app, message, frame, calls);
  });
  ctx.r3.u64 = ctx.r1.u32 - 32;
}

void AppApplyRequest(PPCContext& ctx, uint8_t* base) {
  std::optional<bool> inserted;
  const uint32_t type = High(ctx.r4.u64);
  if (Heap() && (type > 7 || framework::CanAllocate(base))) {
    inserted = as::AppApplyRequest(base, ctx.r3.u32, type, ctx.r4.u32, High(ctx.r5.u64));
  }
  if (!inserted) {
    __imp__sub_82093BE0(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r4.u64);
  REX_STORE_U64(ctx.r1.u32 + 40, ctx.r5.u64);
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 32;
  }
}

void AppUpdate(PPCContext& ctx, uint8_t* base) {
  const uint32_t app = ctx.r3.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(144, uint32_t(ctx.lr), [&](uint32_t frame) {
    if (as::AppUpdate(base, app, frame, calls)) {
      ctx.r3.u64 = frame - 16;
    }
  });
}

namespace {

// The +20 interface's add slots: r3 = the interface, r4 = the key's
// address, r5 = the delegate; the copy made with its slot 2 and the request
// pushed, from a 112-byte frame like the original's (the request at r1 + 80
// there). r3 as PushBack leaves it.
void AddRequestStep(PPCContext& ctx, uint8_t* base, uint32_t type, uint32_t clone_return,
                    uint32_t push_return) {
  const uint32_t interface = ctx.r3.u32;
  const uint32_t key = ctx.r4.u32;
  const uint32_t delegate = ctx.r5.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  guest_call::InFrame(ctx, base, 112, clone_return, [&](uint32_t frame) {
    const uint32_t request = frame + 80;
    const auto inserted =
        as::AppAddRequest(base, interface, type, key, delegate, request, clone_return, calls);
    const uint32_t vector = interface + as::kInterfaceRequests;
    if (!inserted) {
      ctx.r3.u64 = vector;
      ctx.r4.u64 = request;
      ctx.lr = push_return;
      __imp__sub_8209A930(ctx, base);
      return;
    }
    ctx.r3.u64 = *inserted ? frame - 16 : vector;
  });
}

}  // namespace

void AppAddHandler(PPCContext& ctx, uint8_t* base) {
  AddRequestStep(ctx, base, 0, 0x82094868, 0x82094878);
}
void AppAddToggledHandler(PPCContext& ctx, uint8_t* base) {
  AddRequestStep(ctx, base, 2, 0x82094908, 0x82094918);
}
void AppAddAlwaysHandler(PPCContext& ctx, uint8_t* base) {
  AddRequestStep(ctx, base, 4, 0x820949A8, 0x820949B8);
}
void AppAddListener(PPCContext& ctx, uint8_t* base) {
  AddRequestStep(ctx, base, 6, 0x82094A48, 0x82094A58);
}

void AppOpConstruct(PPCContext& ctx, uint8_t* base) {
  if (!Heap() || !as::OpConstruct(base, ctx.r3.u32)) {
    __imp__sub_820AD0B8(ctx, base);
  }
}

void AppOpBuyNode(PPCContext& ctx, uint8_t* base) {
  std::optional<uint32_t> node;
  if (Heap()) {
    node = as::OpBuyNode(base, ctx.r4.u32, ctx.r5.u32, guest::LoadU32(base, ctx.r6.u32),
                         guest::LoadU32(base, ctx.r6.u32 + 4));
  }
  if (!node) {
    __imp__sub_820ACFF8(ctx, base);
    return;
  }
  ctx.r3.u64 = *node;
}

void AppLayerPush(PPCContext& ctx, uint8_t* base) {
  if (!Heap() || !as::LayerPush(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820ADAD0(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + as::kLayerOps;
}

void AppMessagesInsertN(PPCContext& ctx, uint8_t* base) {
  if (!Heap() || !as::MessagesInsertN(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                      guest::LoadU32(base, ctx.r6.u32))) {
    __imp__sub_820938C0(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 24, ctx.r4.u64);
}

void AppMessagesInsert(PPCContext& ctx, uint8_t* base) {
  std::optional<uint32_t> where;
  if (Heap()) {
    where = as::MessagesInsert(base, ctx.r4.u32, High(ctx.r5.u64), ctx.r5.u32, ctx.r6.u32);
  }
  if (!where) {
    __imp__sub_82094110(ctx, base);
    return;
  }
  REX_STORE_U64(ctx.r1.u32 + 32, ctx.r5.u64);
  guest::StoreU32(base, ctx.r3.u32 + 0, ctx.r4.u32);
  guest::StoreU32(base, ctx.r3.u32 + 4, *where);
}

void AppMessagesPushBack(PPCContext& ctx, uint8_t* base) {
  std::optional<bool> inserted;
  if (Heap()) {
    inserted = as::MessagesPushBack(base, ctx.r3.u32, ctx.r4.u32);
  }
  if (!inserted) {
    __imp__sub_820944E0(ctx, base);
    return;
  }
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 16;
  }
}

void AppSendMessage(PPCContext& ctx, uint8_t* base) {
  std::optional<bool> inserted;
  if (Heap()) {
    inserted = as::AppSendMessage(base, ctx.r3.u32, ctx.r4.u32);
  }
  if (!inserted) {
    __imp__sub_82094AB0(ctx, base);
    return;
  }
  ctx.r3.u64 = *inserted ? ctx.r1.u32 - 16 : ctx.r3.u32 + as::kInterfaceMessages;
}

void AppStatesIsEmpty(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = as::StatesIsEmpty(base, ctx.r3.u32, AppStatesCalls(ctx, base));
}

void AppIsRunning(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = as::AppIsRunning(base, ctx.r3.u32, AppStatesCalls(ctx, base));
  ctx.r11.u64 = ctx.r3.u64;
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off. MessageVector::Erase has none: in play only Clear calls it, and
// Clear's plain C++ calls its plain C++; nor have MessageVector::PushBack,
// Insert and InsertN, reached only from App::SendMessage's plain C++, nor
// OpList::AllocateNodes, BuyNode and IncSize (removed 2026-10-27: in play
// only OpList::OpList's and StateLayer::Push's plain C++ call them; the
// flag switches those).

#define ITC_APP_STATES_ADAPTER(address, step) \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_app_states)) {       \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base);                   \
  }

ITC_APP_STATES_ADAPTER(82092F98, AppMessagesClear)
ITC_APP_STATES_ADAPTER(82092B78, AppMessagesAllocate)
ITC_APP_STATES_ADAPTER(820ACF88, AppListClear)
ITC_APP_STATES_ADAPTER(82098E80, AppPtrListErase)
ITC_APP_STATES_ADAPTER(820ACA48, AppDelegateClone)
ITC_APP_STATES_ADAPTER(820ACE50, AppLayerIsEmpty)
ITC_APP_STATES_ADAPTER(820AD0B8, AppOpConstruct)
ITC_APP_STATES_ADAPTER(820ADAD0, AppLayerPush)
ITC_APP_STATES_ADAPTER(82094AB0, AppSendMessage)
ITC_APP_STATES_ADAPTER(820AE1A0, AppStatesIsEmpty)
ITC_APP_STATES_ADAPTER(82092928, AppIsRunning)
ITC_APP_STATES_ADAPTER(82092BF0, AppHandlerMapForEach)
ITC_APP_STATES_ADAPTER(82092CD0, AppForEachMessage)
ITC_APP_STATES_ADAPTER(82091C48, AppForEachRequest)
ITC_APP_STATES_ADAPTER(820929B0, AppRunListener)
ITC_APP_STATES_ADAPTER(82092D50, AppDeliverMessage)
ITC_APP_STATES_ADAPTER(82093BE0, AppApplyRequest)
ITC_APP_STATES_ADAPTER(82094830, AppAddHandler)
ITC_APP_STATES_ADAPTER(820948D0, AppAddToggledHandler)
ITC_APP_STATES_ADAPTER(82094970, AppAddAlwaysHandler)
ITC_APP_STATES_ADAPTER(82094A10, AppAddListener)
ITC_APP_STATES_ADAPTER(82093DC0, AppUpdate)

// App::RunHandler, with --find_menu_selection's wrap (moved here from
// src/debug/menu_selection_scan.cpp, as one override per function can
// exist): r4's low word is a component (a delegate), whose vtable slot 1
// it calls; logged as "component <address>" with that slot's target (a
// delegate's, sub_8223A3F0 or sub_82233460, is at +8).
extern "C" REX_FUNC(sub_82092968) {
  REX_FUNC_PROLOGUE();
  const auto run = [&] {
    if (!ITC_FLAG(native_app_states)) {
      __imp__sub_82092968(ctx, base);
      return;
    }
    hooks::AppRunHandler(ctx, base);
  };
  if (!debug::MenuSelectionScan_Active()) {
    run();
    return;
  }
  const uint32_t component = ctx.r4.u32;
  uint32_t update = component ? guest::LoadU32(base, guest::LoadU32(base, component) + 4) : 0;
  if (update == 0x8223A3F0 || update == 0x82233460) {
    update = guest::LoadU32(base, component + 8);
  }
  debug::MenuSelectionScan_Before(base);
  run();
  static thread_local std::string kind;
  kind = fmt::format("component {:08X}", component);
  debug::MenuSelectionScan_After(base, kind.c_str(), update);
}
