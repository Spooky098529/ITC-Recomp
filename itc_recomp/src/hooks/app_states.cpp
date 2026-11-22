// The app's update and its state stack natively: adapters in front of
// src/app_states/ (docs/functions/app-states.md), with the register steps the
// harness shares (hooks/app_states.h).

#include "hooks/app_states.h"
#include "hooks/native_entry.h"
#include "lifted/originals.h"

#include "app_states/app.h"
#include "app_states/containers.h"
#include "app_states/layers.h"
#include "base/flags.h"
#include "base/log.h"
#include "debug/menu_selection_scan.h"
#include "framework/run_loop.h"
#include "hooks/app_start_up.h"
#include "hooks/guest_call.h"

#include <format>

#include <string>
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "schema/app_states.h"
#include "std_list/ptr_list.h"
#include "lifted/sdk.h"

ITC_FLAG_BOOL(native_app_states, true, "ITC",
              "Run the app's update and its state stack (docs/functions/app-states.md): the "
              "message vector's Clear sub_82092F98 (with Erase sub_82092E70; no adapters: only "
              "App::Update's plain C++ calls them in play) and Allocate sub_82092B78, the operation list's AllocateNodes "
              "sub_820ACF10 and IncSize sub_820AD768 (no adapters: only the plain C++ below "
              "calls them in play), List::Clear sub_820ACF88 (no adapter: only ApplyOps's plain C++ calls "
              "it in play), PtrList::Erase "
              "sub_82098E80, Delegate::Clone sub_820ACA48 (those that allocate or free only "
              "while --native_heap routes the game's heap to the allocation layer) and "
              "StateLayer::IsEmpty sub_820ACE50; with --native_heap, OpList::OpList sub_820AD0B8 (no adapter: "
              "only the layers' constructors' plain C++ call it in play), "
              "OpList::BuyNode sub_820ACFF8 (no adapter: only Push's plain C++ calls it in play), "
              "StateLayer::Push sub_820ADAD0 (no adapter: only StateLayer::StateLayer(state)'s plain C++ "
              "calls it in play) and App::SendMessage "
              "sub_82094AB0 (with MessageVector::PushBack sub_820944E0, Insert sub_82094110 and "
              "InsertN sub_820938C0, no adapters: only SendMessage reaches them); "
              "AppStates::IsEmpty sub_820AE1A0 and App::IsRunning sub_82092928, their virtual "
              "calls guest calls where the target isn't ours; HandlerMap::ForEach sub_82092BF0, "
              "App::ForEachMessage sub_82092CD0 (no adapter: only App::Update's plain C++ calls it "
              "in play) and ForEachRequest sub_82091C48, "
              "App::RunHandler sub_82092968 and RunListener sub_820929B0, App::DeliverMessage "
              "sub_82092D50, their functors and slots guest calls; with --native_heap "
              "App::ApplyRequest sub_82093BE0 (its split-off type 0 piece sub_82093C2C merged) "
              "and the +20 interface's add slots sub_82094830, sub_820948D0, sub_82094970 and "
              "sub_82094A10; App::Update sub_82093DC0, the run loop's tick handler 8, its virtual "
              "calls, button tests and functors guest calls; with --native_heap the state stack's "
              "set-up: AppStates::Create sub_820AE498, AppStates::AppStates sub_820AE348 (no adapter: only "
              "Create's plain C++ calls it in play), StateLayer::StateLayer sub_820AD118 (no adapter: "
              "only AppStates::AppStates's plain C++ calls it in play) and sub_820ADBD8, StateLayer::AddChild "
              "sub_820ADA08 (no adapters: only AddLayer's and ApplyOp's plain C++ call them in play), AppStates::AddLayer sub_820AE130 and App::Start sub_82092878, their "
              "virtual calls guest calls; PtrList::ForEach sub_820AD3A0 (no adapter: only the layers' "
              "plain C++ calls it in play) and OpList::ForEach "
              "sub_820AD440 (no adapter: only ApplyOps's plain C++ calls it in play), the layers' child functors sub_820ACD80, sub_820ACDE8 and "
              "sub_820ACD60, and with --native_heap StateLayer::ApplyOps sub_820ADE68 (no adapter: only the "
              "layers' updates' plain C++ call it in play), their "
              "slots and StateLayer::ApplyOp guest calls; StateLayer::Update sub_820ADF00 and "
              "UpdateToggled sub_820AE010 (no adapters: only the plain C++ below calls them in play), the layer's state slots 8 and 9 sub_820AE120 and "
              "sub_820AE128, AppStates::OnUpdate sub_820AE1B8 and OnUpdateToggled sub_820AE1C0, "
              "the top state's slots guest calls, a child layer's update its plain C++; "
              "StateLayer::ApplyOp sub_820ADC60 (adds and pushes, with --native_heap; the pop and the "
              "replace, never seen in play, the original), OnToggleOn sub_820AD4E0 and OnToggleOff "
              "sub_820AD680 (no adapters: only the plain C++ of their callers calls them in play), their functor sub_820ACD38 and the state slot 5 sub_820AD760, and "
              "AppStates::OnMessage sub_820AE1C8 (messages 0 and 2 and the ignored ones; 1, 5 and 6 "
              "the original)) natively instead of through the translated code "
              "(the same results; checked by --run_harnesses=app_states)");

namespace hooks {

namespace as = app_states;

namespace {

// The functions that allocate or free: only over the allocation layer.
bool Heap() { return hooks::NativeHeapActive(); }

uint32_t High(uint64_t value) { return uint32_t(value >> 32); }

}  // namespace

ITC_REGS_STEP(AppMessagesErase) {
  if (!as::MessagesErase(base, ctx.r4.u32, High(ctx.r5.u64), ctx.r5.u32, High(ctx.r6.u64),
                         ctx.r6.u32)) {
    lifted::original::c82092E70(ctx, base);
    return;
  }
  guest::StoreU64(base, ctx.r1.u32 + 32, ctx.r5.u64);
  guest::StoreU64(base, ctx.r1.u32 + 40, ctx.r6.u64);
  guest::StoreU64(base, ctx.r3.u32, ctx.r5.u64);
}

ITC_REGS_STEP(AppMessagesClear) {
  if (!as::MessagesClear(base, ctx.r3.u32)) {
    lifted::original::c82092F98(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r1.u32 - 16;
}

ITC_REGS_STEP(AppMessagesAllocate) {
  if (Heap()) {
    if (const auto block = as::MessagesAllocate(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  lifted::original::c82092B78(ctx, base);
}

ITC_REGS_STEP(AppOpAllocateNodes) {
  if (Heap()) {
    if (const auto block = as::OpAllocateNodes(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  lifted::original::c820ACF10(ctx, base);
}

ITC_REGS_STEP(AppOpIncSize) {
  if (!as::OpIncSize(base, ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c820AD768(ctx, base);
  }
}

ITC_REGS_STEP(AppListClear) {
  if (!Heap()) {
    lifted::original::c820ACF88(ctx, base);
    return;
  }
  ctx.r3.u64 = as::ListClear(base, ctx.r3.u32);
}

ITC_REGS_STEP(AppPtrListErase) {
  std::optional<uint32_t> next;
  if (Heap()) {
    next = std_list::Erase(base, ctx.r4.u32, High(ctx.r5.u64), ctx.r5.u32);
  }
  if (!next) {
    lifted::original::c82098E80(ctx, base);
    return;
  }
  const uint64_t result = (ctx.r5.u64 & 0xFFFFFFFF00000000ull) | *next;
  guest::StoreU64(base, ctx.r1.u32 + 32, result);
  guest::StoreU64(base, ctx.r3.u32, result);
}

ITC_REGS_STEP(AppDelegateClone) {
  if (Heap()) {
    if (const auto copy = as::DelegateClone(base, ctx.r3.u32)) {
      ctx.r3.u64 = *copy;
      return;
    }
  }
  lifted::original::c820ACA48(ctx, base);
}

ITC_REGS_STEP(AppLayerIsEmpty) {
  ctx.r3.u64 = as::LayerIsEmpty(base, ctx.r3.u32);
}

as::AppCalls AppStatesCalls(lifted::Regs& ctx, uint8_t* base) {
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
    lifted::CallFromContext(ctx, base, ctx.ctr.u32);
    return ctx.r3.u32;
  };
  calls.in_frame = [&ctx, base](uint32_t bytes, uint32_t return_address,
                                const std::function<void(uint32_t)>& body) {
    // (r14 to r31 were kept and put back around the body: nothing to keep
    // without a context)
    guest_call::InFrame(ctx, base, bytes, return_address, body);
  };
  // The registers set (r11, r22 to r31) are the original's nonvolatile and
  // scratch values, which reached callees only as the bytes they save: not
  // carried by the view.
  calls.set_register = [](int, uint64_t) {};
  calls.increment = [&ctx, base](uint32_t iterator, uint32_t return_address) {
    ctx.r3.u64 = iterator;
    ctx.lr = return_address;
    lifted::original::c82099B18(ctx, base);
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
    lifted::CallFromContext(ctx, base, ctx.ctr.u32);
    return ctx.r3.u32;
  };
  calls.native_heap = Heap();
  return calls;
}

ITC_REGS_STEP(AppHandlerMapForEach) {
  const uint32_t result = ctx.r3.u32;
  const uint64_t first = ctx.r4.u64, last = ctx.r5.u64, functor = ctx.r6.u64,
                 adjust = ctx.r7.u64;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(112, uint32_t(ctx.lr), [&](uint32_t frame) {
    as::HandlerMapForEach(base, result, first, last, functor, adjust, frame, calls);
  });
  ctx.r3.u64 = result;
}

ITC_REGS_STEP(AppForEachMessage) {
  const uint32_t result = ctx.r3.u32;
  const uint64_t first = ctx.r4.u64, last = ctx.r5.u64, functor = ctx.r6.u64,
                 adjust = ctx.r7.u64;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(128, uint32_t(ctx.lr), [&](uint32_t frame) {
    as::ForEachMessage(base, result, first, last, functor, adjust, frame, calls);
  });
  ctx.r3.u64 = result;
}

ITC_REGS_STEP(AppForEachRequest) {
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

void RunHandlerStep(lifted::Regs& ctx, uint8_t* base, bool listener) {
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

ITC_REGS_STEP(AppRunHandler) { RunHandlerStep(ctx, base, false); }
ITC_REGS_STEP(AppRunListener) { RunHandlerStep(ctx, base, true); }

ITC_REGS_STEP(AppDeliverMessage) {
  const uint32_t app = ctx.r3.u32;
  const uint32_t message = ctx.r4.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(128, uint32_t(ctx.lr), [&](uint32_t frame) {
    as::DeliverMessage(base, app, message, frame, calls);
  });
  ctx.r3.u64 = ctx.r1.u32 - 32;
}

ITC_REGS_STEP(AppApplyRequest) {
  std::optional<bool> inserted;
  const uint32_t type = High(ctx.r4.u64);
  if (Heap() && (type > 7 || framework::CanAllocate(base))) {
    inserted = as::AppApplyRequest(base, ctx.r3.u32, type, ctx.r4.u32, High(ctx.r5.u64));
  }
  if (!inserted) {
    lifted::original::c82093BE0(ctx, base);
    return;
  }
  guest::StoreU64(base, ctx.r1.u32 + 32, ctx.r4.u64);
  guest::StoreU64(base, ctx.r1.u32 + 40, ctx.r5.u64);
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 32;
  }
}

ITC_REGS_STEP(AppUpdate) {
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
void AddRequestStep(lifted::Regs& ctx, uint8_t* base, uint32_t type, uint32_t clone_return,
                    uint32_t push_return) {
  const uint32_t interface = ctx.r3.u32;
  const uint32_t key = ctx.r4.u32;
  const uint32_t delegate = ctx.r5.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  guest_call::InFrame(ctx, base, 112, clone_return, [&](uint32_t frame) {
    const uint32_t request = frame + 80;
    const auto inserted =
        as::AppAddRequest(base, interface, type, key, delegate, request, clone_return, calls);
    const uint32_t vector = schema::AppInterface(base, interface).requests_address();
    if (!inserted) {
      ctx.r3.u64 = vector;
      ctx.r4.u64 = request;
      ctx.lr = push_return;
      lifted::original::c8209A930(ctx, base);
      return;
    }
    ctx.r3.u64 = *inserted ? frame - 16 : vector;
  });
}

}  // namespace

ITC_REGS_STEP(AppAddHandler) {
  AddRequestStep(ctx, base, 0, 0x82094868, 0x82094878);
}
ITC_REGS_STEP(AppAddToggledHandler) {
  AddRequestStep(ctx, base, 2, 0x82094908, 0x82094918);
}
ITC_REGS_STEP(AppAddAlwaysHandler) {
  AddRequestStep(ctx, base, 4, 0x820949A8, 0x820949B8);
}
ITC_REGS_STEP(AppAddListener) {
  AddRequestStep(ctx, base, 6, 0x82094A48, 0x82094A58);
}

ITC_REGS_STEP(AppOpConstruct) {
  if (!Heap() || !as::OpConstruct(base, ctx.r3.u32)) {
    lifted::original::c820AD0B8(ctx, base);
  }
}

ITC_REGS_STEP(AppOpBuyNode) {
  std::optional<uint32_t> node;
  if (Heap()) {
    const schema::AppStateOp op(base, ctx.r6.u32);
    node = as::OpBuyNode(base, ctx.r4.u32, ctx.r5.u32, op.type(), op.state());
  }
  if (!node) {
    lifted::original::c820ACFF8(ctx, base);
    return;
  }
  ctx.r3.u64 = *node;
}

ITC_REGS_STEP(AppLayerPush) {
  if (!Heap() || !as::LayerPush(base, ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c820ADAD0(ctx, base);
    return;
  }
  ctx.r3.u64 = schema::StateLayer(base, ctx.r3.u32).ops_list().address;
}

ITC_REGS_STEP(AppMessagesInsertN) {
  if (!Heap() || !as::MessagesInsertN(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                      schema::AppMessageWord(base, ctx.r6.u32).value())) {
    lifted::original::c820938C0(ctx, base);
    return;
  }
  guest::StoreU64(base, ctx.r1.u32 + 24, ctx.r4.u64);
}

ITC_REGS_STEP(AppMessagesInsert) {
  std::optional<uint32_t> where;
  if (Heap()) {
    where = as::MessagesInsert(base, ctx.r4.u32, High(ctx.r5.u64), ctx.r5.u32, ctx.r6.u32);
  }
  if (!where) {
    lifted::original::c82094110(ctx, base);
    return;
  }
  guest::StoreU64(base, ctx.r1.u32 + 32, ctx.r5.u64);
  guest::StoreU32(base, ctx.r3.u32 + 0, ctx.r4.u32);
  guest::StoreU32(base, ctx.r3.u32 + 4, *where);
}

ITC_REGS_STEP(AppMessagesPushBack) {
  std::optional<bool> inserted;
  if (Heap()) {
    inserted = as::MessagesPushBack(base, ctx.r3.u32, ctx.r4.u32);
  }
  if (!inserted) {
    lifted::original::c820944E0(ctx, base);
    return;
  }
  if (*inserted) {
    ctx.r3.u64 = ctx.r1.u32 - 16;
  }
}

ITC_REGS_STEP(AppSendMessage) {
  std::optional<bool> inserted;
  if (Heap()) {
    inserted = as::AppSendMessage(base, ctx.r3.u32, ctx.r4.u32);
  }
  if (!inserted) {
    lifted::original::c82094AB0(ctx, base);
    return;
  }
  ctx.r3.u64 = *inserted ? ctx.r1.u32 - 16
                          : schema::AppInterface(base, ctx.r3.u32).messages_address();
}

ITC_REGS_STEP(AppStatesIsEmpty) {
  ctx.r3.u64 = as::StatesIsEmpty(base, ctx.r3.u32, AppStatesCalls(ctx, base));
}

ITC_REGS_STEP(AppIsRunning) {
  ctx.r3.u64 = as::AppIsRunning(base, ctx.r3.u32, AppStatesCalls(ctx, base));
  // (r11 = r3 was a leftover for recompiled callers: none now)
}

namespace {

// A step over the allocation layer from the original's frame of `bytes`:
// `body(frame)` gives false where the original must run (nothing done).
template <typename Body>
bool InOwnFrame(lifted::Regs& ctx, uint8_t* base, uint32_t bytes, const as::AppCalls& calls,
                Body body) {
  if (!Heap()) {
    return false;
  }
  bool ours = false;
  calls.in_frame(bytes, uint32_t(ctx.lr), [&](uint32_t frame) { ours = body(frame); });
  return ours;
}

void LayerConstructStep(lifted::Regs& ctx, uint8_t* base, bool with_state,
                        lifted::RegsFunction* original) {
  const uint32_t layer = ctx.r3.u32, owner = ctx.r4.u32;
  const std::optional<uint32_t> state =
      with_state ? std::optional<uint32_t>(ctx.r5.u32) : std::nullopt;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  // The lists' constructors hand back without the process heap.
  if (!Heap() || memory::crt::GetProcessHeap(base) == 0) {
    lifted::Sdk(ctx, base, original);
    return;
  }
  calls.in_frame(with_state ? 112 : 96, uint32_t(ctx.lr), [&](uint32_t frame) {
    as::LayerConstruct(base, layer, owner, state, frame, calls);
  });
  ctx.r3.u64 = layer;
}

}  // namespace

ITC_REGS_STEP(AppLayerConstruct) {
  LayerConstructStep(ctx, base, false, lifted::original::c820AD118);
}
ITC_REGS_STEP(AppLayerConstructWithState) {
  LayerConstructStep(ctx, base, true, lifted::original::c820ADBD8);
}

ITC_REGS_STEP(AppLayerAddChild) {
  const uint32_t layer = ctx.r3.u32, child = ctx.r4.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  if (!InOwnFrame(ctx, base, 112, calls, [&](uint32_t frame) {
        as::LayerAddChild(base, layer, child, frame, calls);
        return true;
      })) {
    lifted::original::c820ADA08(ctx, base);
    return;
  }
  ctx.r3.u64 = schema::StateLayer(base, layer).children_list().address;
}

ITC_REGS_STEP(AppStatesAddLayer) {
  const uint32_t states = ctx.r3.u32, state = ctx.r4.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  if (!InOwnFrame(ctx, base, 112, calls, [&](uint32_t frame) {
        return as::StatesAddLayer(base, states, state, frame, calls);
      })) {
    lifted::original::c820AE130(ctx, base);
    return;
  }
  ctx.r3.u64 = schema::AppStates(base, states).root().children_list().address;
}

ITC_REGS_STEP(AppStatesConstruct) {
  const uint32_t states = ctx.r3.u32, setup = ctx.r4.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  if (!InOwnFrame(ctx, base, 176, calls, [&](uint32_t frame) {
        return as::StatesConstruct(base, states, setup, frame, calls);
      })) {
    lifted::original::c820AE348(ctx, base);
    return;
  }
  ctx.r3.u64 = states;
}

ITC_REGS_STEP(AppStatesCreate) {
  const uint32_t setup = ctx.r3.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  std::optional<uint32_t> states;
  if (!InOwnFrame(ctx, base, 96, calls, [&](uint32_t frame) {
        states = as::StatesCreate(base, setup, frame, calls);
        return states.has_value();
      })) {
    lifted::original::c820AE498(ctx, base);
    return;
  }
  ctx.r3.u64 = *states;
}

ITC_REGS_STEP(AppStart) {
  const uint32_t interface = ctx.r3.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  std::optional<uint32_t> r3;
  if (!InOwnFrame(ctx, base, 96, calls, [&](uint32_t frame) {
        r3 = as::AppStart(base, interface, frame, calls);
        return r3.has_value();
      })) {
    lifted::original::c82092878(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

namespace {

void ListForEachStep(lifted::Regs& ctx, uint8_t* base, bool ops) {
  const uint32_t result = ctx.r3.u32;
  const uint64_t first = ctx.r4.u64, last = ctx.r5.u64, functor = ctx.r6.u64,
                 adjust = ctx.r7.u64;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  calls.in_frame(112, uint32_t(ctx.lr), [&](uint32_t frame) {
    as::ListForEach(base, result, first, last, functor, adjust, ops, frame, calls);
  });
  ctx.r3.u64 = result;
}

void UpdateChildStep(lifted::Regs& ctx, uint8_t* base, bool toggled) {
  const uint32_t layer = ctx.r3.u32, child = ctx.r4.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  uint32_t answer = 0;
  calls.in_frame(112, uint32_t(ctx.lr), [&](uint32_t frame) {
    answer = as::UpdateChild(base, layer, child, toggled, frame, calls);
  });
  ctx.r3.u64 = answer;
}

}  // namespace

ITC_REGS_STEP(AppPtrListForEach) { ListForEachStep(ctx, base, false); }
ITC_REGS_STEP(AppOpListForEach) { ListForEachStep(ctx, base, true); }
ITC_REGS_STEP(AppUpdateChild) { UpdateChildStep(ctx, base, false); }
ITC_REGS_STEP(AppUpdateChildToggled) { UpdateChildStep(ctx, base, true); }

ITC_REGS_STEP(AppToggleOffChild) {
  const uint32_t layer = ctx.r3.u32, child = ctx.r4.u32;
  ctx.r3.u64 =
      as::ToggleOffChild(base, layer, child, uint32_t(ctx.lr), AppStatesCalls(ctx, base));
}

ITC_REGS_STEP(AppLayerApplyOps) {
  if (!Heap()) {
    lifted::original::c820ADE68(ctx, base);
    return;
  }
  const uint32_t layer = ctx.r3.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  uint32_t answer = 0;
  calls.in_frame(128, uint32_t(ctx.lr), [&](uint32_t frame) {
    answer = as::LayerApplyOps(base, layer, frame, calls);
  });
  ctx.r3.u64 = answer;
}

namespace {

// The updates, from a 144-byte frame (the layer's own, or the one the slot
// thunks and the stack's targets tail-call into).
template <typename Update>
void UpdateStep(lifted::Regs& ctx, uint8_t* base, Update update) {
  const uint32_t r3 = ctx.r3.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  uint32_t answer = 0;
  calls.in_frame(144, uint32_t(ctx.lr), [&](uint32_t frame) { answer = update(r3, frame, calls); });
  ctx.r3.u64 = answer;
}

}  // namespace

ITC_REGS_STEP(AppLayerUpdate) {
  UpdateStep(ctx, base, [&](uint32_t layer, uint32_t frame, const as::AppCalls& calls) {
    return as::LayerUpdate(base, layer, false, frame, calls);
  });
}
ITC_REGS_STEP(AppLayerUpdateToggled) {
  UpdateStep(ctx, base, [&](uint32_t layer, uint32_t frame, const as::AppCalls& calls) {
    return as::LayerUpdate(base, layer, true, frame, calls);
  });
}
ITC_REGS_STEP(AppLayerStateUpdate) {
  UpdateStep(ctx, base, [&](uint32_t state, uint32_t frame, const as::AppCalls& calls) {
    return as::LayerStateUpdate(base, state, false, frame, calls);
  });
}
ITC_REGS_STEP(AppLayerStateUpdateToggled) {
  UpdateStep(ctx, base, [&](uint32_t state, uint32_t frame, const as::AppCalls& calls) {
    return as::LayerStateUpdate(base, state, true, frame, calls);
  });
}
ITC_REGS_STEP(AppStatesOnUpdate) {
  UpdateStep(ctx, base, [&](uint32_t states, uint32_t frame, const as::AppCalls& calls) {
    return as::StatesOnUpdate(base, states, false, frame, calls);
  });
}
ITC_REGS_STEP(AppStatesOnUpdateToggled) {
  UpdateStep(ctx, base, [&](uint32_t states, uint32_t frame, const as::AppCalls& calls) {
    return as::StatesOnUpdate(base, states, true, frame, calls);
  });
}

ITC_REGS_STEP(AppLayerApplyOp) {
  const uint32_t layer = ctx.r3.u32;
  const uint64_t op = ctx.r4.u64;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  std::optional<uint32_t> answer;
  calls.in_frame(128, uint32_t(ctx.lr),
                 [&](uint32_t frame) { answer = as::LayerApplyOp(base, layer, op, frame, calls); });
  if (!answer) {
    ctx.r4.u64 = op;
    lifted::original::c820ADC60(ctx, base);
    return;
  }
  ctx.r3.u64 = *answer;
}

namespace {

void ToggleStep(lifted::Regs& ctx, uint8_t* base, bool off) {
  const uint32_t layer = ctx.r3.u32;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  uint32_t answer = 0;
  calls.in_frame(128, uint32_t(ctx.lr),
                 [&](uint32_t frame) { answer = as::LayerToggle(base, layer, off, frame, calls); });
  ctx.r3.u64 = answer;
}

}  // namespace

ITC_REGS_STEP(AppLayerToggleOn) { ToggleStep(ctx, base, false); }
ITC_REGS_STEP(AppLayerToggleOff) { ToggleStep(ctx, base, true); }

ITC_REGS_STEP(AppToggleOnChild) {
  const uint32_t child = ctx.r4.u32;
  ctx.r3.u64 = as::ToggleOnChild(base, child, uint32_t(ctx.lr), AppStatesCalls(ctx, base));
}

ITC_REGS_STEP(AppLayerStateToggleOff) {
  const uint32_t state = ctx.r3.u32;
  ctx.r3.u64 = as::LayerStateToggleOff(base, state, uint32_t(ctx.lr), AppStatesCalls(ctx, base));
}

ITC_REGS_STEP(AppStatesOnMessage) {
  const uint32_t states = ctx.r3.u32, message = ctx.r5.u32;
  const uint64_t r4 = ctx.r4.u64;
  const as::AppCalls calls = AppStatesCalls(ctx, base);
  std::optional<uint32_t> answer;
  calls.in_frame(112, uint32_t(ctx.lr), [&](uint32_t frame) {
    answer = as::StatesOnMessage(base, states, message, frame, calls);
  });
  if (!answer) {
    ctx.r3.u64 = states;
    ctx.r4.u64 = r4;
    ctx.r5.u64 = message;
    lifted::original::c820AE1C8(ctx, base);
    return;
  }
  ctx.r3.u64 = *answer;
}

}  // namespace hooks

// The adapters: the register steps above, or the originals with the flag
// off. MessageVector::Erase has none: in play only Clear calls it, and
// Clear's plain C++ calls its plain C++; nor have MessageVector::PushBack,
// Insert and InsertN, reached only from App::SendMessage's plain C++, nor
// OpList::AllocateNodes, BuyNode and IncSize (removed 2026-10-27: in play
// only OpList::OpList's and StateLayer::Push's plain C++ call them; the
// flag switches those), nor MessageVector::Clear and App::ForEachMessage
// (removed 2026-10-27: in play only App::Update's plain C++ calls them),
// nor OpList::OpList, StateLayer::Push, StateLayer::StateLayer and
// AppStates::AppStates (removed 2026-10-27: in play only the layers'
// constructors', StateLayer::StateLayer(state)'s and AppStates::Create's
// plain C++ call them), nor List::Clear and OpList::ForEach (removed
// 2026-10-27: in play only StateLayer::ApplyOps's plain C++ calls them),
// nor StateLayer::ApplyOps, Update and UpdateToggled (removed 2026-10-27:
// in play only the layers' updates' and their callers' plain C++ call
// them), nor StateLayer::StateLayer(state), AddChild, OnToggleOn,
// OnToggleOff and PtrList::ForEach (removed 2026-10-27: in play only
// AppStates::AddLayer's, ApplyOp's, the toggles' callers' and the layers'
// walks' plain C++ call them).

#define ITC_APP_STATES_ADAPTER(address, step) \
  ITC_NATIVE_ENTRY(address, ITC_FLAG(native_app_states)) { \
    hooks::step(ctx, base);                   \
  }

ITC_APP_STATES_ADAPTER(82092B78, AppMessagesAllocate)
ITC_APP_STATES_ADAPTER(82098E80, AppPtrListErase)
ITC_APP_STATES_ADAPTER(820ACA48, AppDelegateClone)
ITC_APP_STATES_ADAPTER(820ACE50, AppLayerIsEmpty)
ITC_APP_STATES_ADAPTER(82094AB0, AppSendMessage)
ITC_APP_STATES_ADAPTER(820AE1A0, AppStatesIsEmpty)
ITC_APP_STATES_ADAPTER(82092928, AppIsRunning)
ITC_APP_STATES_ADAPTER(82092BF0, AppHandlerMapForEach)
ITC_APP_STATES_ADAPTER(82091C48, AppForEachRequest)
ITC_APP_STATES_ADAPTER(820929B0, AppRunListener)
ITC_APP_STATES_ADAPTER(82092D50, AppDeliverMessage)
ITC_APP_STATES_ADAPTER(82093BE0, AppApplyRequest)
ITC_APP_STATES_ADAPTER(82094830, AppAddHandler)
ITC_APP_STATES_ADAPTER(820948D0, AppAddToggledHandler)
ITC_APP_STATES_ADAPTER(82094970, AppAddAlwaysHandler)
ITC_APP_STATES_ADAPTER(82094A10, AppAddListener)
ITC_APP_STATES_ADAPTER(82093DC0, AppUpdate)
ITC_APP_STATES_ADAPTER(820AE130, AppStatesAddLayer)
// AppStates::Create: the app's start-up harness's recorder first
// (hooks/app_start_up.h: App::App's callees).
ITC_NATIVE_ENTRY_ALWAYS(820AE498) {
  
  if (hooks::StartUpRecorded(ctx, 0x820AE498)) {
    return;
  }
  if (!ITC_FLAG(native_app_states)) {
    lifted::original::c820AE498(ctx, base);
    return;
  }
  hooks::AppStatesCreate(ctx, base);
}
ITC_APP_STATES_ADAPTER(82092878, AppStart)
ITC_APP_STATES_ADAPTER(820ACD80, AppUpdateChild)
ITC_APP_STATES_ADAPTER(820ACDE8, AppUpdateChildToggled)
ITC_APP_STATES_ADAPTER(820ACD60, AppToggleOffChild)
ITC_APP_STATES_ADAPTER(820AE120, AppLayerStateUpdate)
ITC_APP_STATES_ADAPTER(820AE128, AppLayerStateUpdateToggled)
ITC_APP_STATES_ADAPTER(820AE1B8, AppStatesOnUpdate)
ITC_APP_STATES_ADAPTER(820AE1C0, AppStatesOnUpdateToggled)
ITC_APP_STATES_ADAPTER(820ADC60, AppLayerApplyOp)
ITC_APP_STATES_ADAPTER(820ACD38, AppToggleOnChild)
ITC_APP_STATES_ADAPTER(820AD760, AppLayerStateToggleOff)
ITC_APP_STATES_ADAPTER(820AE1C8, AppStatesOnMessage)

// App::RunHandler, with --find_menu_selection's wrap (moved here from
// src/debug/menu_selection_scan.cpp, as one override per function can
// exist): r4's low word is a component (a delegate), whose vtable slot 1
// it calls; logged as "component <address>" with that slot's target (a
// delegate's, sub_8223A3F0 or sub_82233460, is at +8).
ITC_NATIVE_ENTRY_ALWAYS(82092968) {
  
  const auto run = [&] {
    if (!ITC_FLAG(native_app_states)) {
      lifted::original::c82092968(ctx, base);
      return;
    }
    hooks::AppRunHandler(ctx, base);
  };
  if (!debug::MenuSelectionScan_Active()) {
    run();
    return;
  }
  const uint32_t component = ctx.r4.u32;
  const schema::AppDelegate delegate(base, component);
  uint32_t update = component ? delegate.vtable().invoke() : 0;
  if (update == 0x8223A3F0 || update == 0x82233460) {
    update = delegate.function();
  }
  debug::MenuSelectionScan_Before(base);
  run();
  static thread_local std::string kind;
  kind = std::format("component {:08X}", component);
  debug::MenuSelectionScan_After(base, kind.c_str(), update);
}
