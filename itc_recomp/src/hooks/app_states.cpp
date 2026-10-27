// The app's update and its state stack natively: adapters in front of
// src/app_states/ (docs/functions/app-states.md), with the register steps the
// harness shares (hooks/app_states.h).

#include "hooks/app_states.h"

#include "app_states/app.h"
#include "app_states/containers.h"
#include "base/flags.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "std_list/ptr_list.h"

ITC_FLAG_BOOL(native_app_states, true, "ITC",
              "Run the app's update and its state stack (docs/functions/app-states.md): the "
              "message vector's Clear sub_82092F98 (with Erase sub_82092E70, no adapter: only "
              "Clear calls it) and Allocate sub_82092B78, the operation list's AllocateNodes "
              "sub_820ACF10 and IncSize sub_820AD768, List::Clear sub_820ACF88, PtrList::Erase "
              "sub_82098E80, Delegate::Clone sub_820ACA48 (those that allocate or free only "
              "while --native_heap routes the game's heap to the allocation layer) and "
              "StateLayer::IsEmpty sub_820ACE50; with --native_heap, OpList::OpList sub_820AD0B8, "
              "OpList::BuyNode sub_820ACFF8, StateLayer::Push sub_820ADAD0 and App::SendMessage "
              "sub_82094AB0 (with MessageVector::PushBack sub_820944E0, Insert sub_82094110 and "
              "InsertN sub_820938C0, no adapters: only SendMessage reaches them); "
              "AppStates::IsEmpty sub_820AE1A0 and App::IsRunning sub_82092928, their virtual "
              "calls guest calls where the target isn't ours) natively instead of through the translated code "
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
    guest_call::InFrame(ctx, base, bytes, return_address, body);
  };
  return calls;
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
// Insert and InsertN, reached only from App::SendMessage's plain C++.

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
ITC_APP_STATES_ADAPTER(820ACF10, AppOpAllocateNodes)
ITC_APP_STATES_ADAPTER(820AD768, AppOpIncSize)
ITC_APP_STATES_ADAPTER(820ACF88, AppListClear)
ITC_APP_STATES_ADAPTER(82098E80, AppPtrListErase)
ITC_APP_STATES_ADAPTER(820ACA48, AppDelegateClone)
ITC_APP_STATES_ADAPTER(820ACE50, AppLayerIsEmpty)
ITC_APP_STATES_ADAPTER(820AD0B8, AppOpConstruct)
ITC_APP_STATES_ADAPTER(820ACFF8, AppOpBuyNode)
ITC_APP_STATES_ADAPTER(820ADAD0, AppLayerPush)
ITC_APP_STATES_ADAPTER(82094AB0, AppSendMessage)
ITC_APP_STATES_ADAPTER(820AE1A0, AppStatesIsEmpty)
ITC_APP_STATES_ADAPTER(82092928, AppIsRunning)
