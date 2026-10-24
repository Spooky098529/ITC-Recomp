// The input manager and its pads and processors natively, and the C++
// library's std::list of pointers its listener lists are: adapters in front
// of src/input/input_manager.cpp and src/std_list/ptr_list.cpp
// (docs/functions/input.md). The functions that allocate run natively only
// while --native_heap routes the game's heap to the allocation layer, and
// hand the paths left to the original (see ptr_list.h) back to it from the
// start, nothing having been done. B1's input update is in input_update.cpp.

#include "guest.h"

#include "base/flags.h"
#include "frame_pacing/game_messages.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "input/input_manager.h"
#include "std_list/ptr_list.h"

ITC_FLAG_BOOL(native_input, true, "ITC",
              "Run the input manager's set-up and the C++ library's std::list of pointers "
              "(docs/functions/input.md: PtrList::AllocateNodes sub_82099C08, IncSize "
              "sub_820958B8 (no adapter), PtrList sub_820AD058 and Insert sub_82095958, with --native_heap; "
              "the pads' and processors' constructors sub_82591B38, sub_82591B68, sub_825920D0 "
              "(no adapter) and sub_8258CE60, LogicalPad::Bind sub_825918F0, the pads' values sub_82591B30 "
              "and sub_820988B8, InputProcessor::Assign sub_82098D60, the button tests "
              "sub_8258CEB0 and sub_8258CEE0 with sub_82592500 and sub_82592520 (no adapters), the manager's slots "
              "sub_82098CB8, sub_82098CD0 and sub_82098FE0; Input::Create sub_820993E8, "
              "InputManager::InputManager sub_820991E0 and AddListener sub_82099430, with "
              "--native_heap) "
              "natively instead of through the translated code (same results; checked by "
              "--run_harnesses=input)");

namespace {

// The functions that allocate: only over the allocation layer.
bool NativeLifetime() {
  return ITC_FLAG(native_input) && hooks::NativeHeapActive();
}

}  // namespace

// --- std::list of pointers ---
//
// IncSize (sub_820958B8) has no adapter: its only caller in play is
// Insert, whose plain C++ calls its plain C++ (a recompiled caller reaching
// it runs the original, and the flag switches Insert).

// AllocateNodes: r3 = the count; r3 the block.
extern "C" REX_FUNC(sub_82099C08) {
  REX_FUNC_PROLOGUE();
  if (NativeLifetime()) {
    if (const auto block = std_list::AllocateNodes(base, ctx.r3.u32)) {
      ctx.r3.u64 = *block;
      return;
    }
  }
  __imp__sub_82099C08(ctx, base);
}

// PtrList: r3 = the list; r3 left as it came.
extern "C" REX_FUNC(sub_820AD058) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() || !std_list::Construct(base, ctx.r3.u32)) {
    __imp__sub_820AD058(ctx, base);
  }
}

// Insert: r3 = the list, r4 = the iterator by value (its node in the low
// word), r5 = the value's address; r3 left as it came (the list).
extern "C" REX_FUNC(sub_82095958) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() || !std_list::Insert(base, ctx.r3.u32, ctx.r4.u32,
                                             guest::LoadU32(base, ctx.r5.u32))) {
    __imp__sub_82095958(ctx, base);
  }
}

// --- The pads and processors ---

// The flag's fallback.
#define ITC_INPUT_FALLBACK(address)    \
  REX_FUNC_PROLOGUE();                 \
  if (!ITC_FLAG(native_input)) {       \
    __imp__sub_##address(ctx, base);   \
    return;                            \
  }

// The constructors: r3 = the object; r3 comes back as it (LogicalPad's
// leaves it untouched). InputProcessorBase's (sub_825920D0) has no adapter:
// its only caller is InputProcessor's, whose plain C++ calls its plain C++.
extern "C" REX_FUNC(sub_82591B38) {
  ITC_INPUT_FALLBACK(82591B38)
  input::LogicalPadConstruct(base, ctx.r3.u32);
}
extern "C" REX_FUNC(sub_82591B68) {
  ITC_INPUT_FALLBACK(82591B68)
  input::PhysicalPadConstruct(base, ctx.r3.u32);
}
extern "C" REX_FUNC(sub_8258CE60) {
  ITC_INPUT_FALLBACK(8258CE60)
  input::InputProcessorConstruct(base, ctx.r3.u32);
}

// LogicalPad::Bind: r3 = the pad, r4 = the index's address, r5 = the device.
extern "C" REX_FUNC(sub_825918F0) {
  ITC_INPUT_FALLBACK(825918F0)
  input::LogicalPadBind(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32), ctx.r5.u32);
}

// InputProcessor::Assign: r3 = the destination, r4 = the source.
extern "C" REX_FUNC(sub_82098D60) {
  ITC_INPUT_FALLBACK(82098D60)
  input::InputProcessorAssign(base, ctx.r3.u32, ctx.r4.u32);
}

// The manager's slots 1, 2 and 5: r3 = the manager, r4 = the index's address.
extern "C" REX_FUNC(sub_82098CB8) {
  ITC_INPUT_FALLBACK(82098CB8)
  ctx.r3.u64 = input::ManagerProcessor(ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32));
}
extern "C" REX_FUNC(sub_82098CD0) {
  ITC_INPUT_FALLBACK(82098CD0)
  ctx.r3.u64 = input::ManagerLogicalPad(ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32));
}
extern "C" REX_FUNC(sub_82098FE0) {
  ITC_INPUT_FALLBACK(82098FE0)
  ctx.r3.u64 = input::ManagerPhysicalPad(ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32));
}

#undef ITC_INPUT_FALLBACK

// --log_input_reads (moved from src/frame_pacing/game_messages.cpp): the
// button tests, r3 = the processor, r4 = the mask's address, returning a
// bool, and the pads' values (their vtables' slot 1, r3 = the pad), counted
// by caller and pass. Input::Update reads the values itself; any other
// caller reads the stick. TestPressed and TestRepeat call AnyPressed and
// PressedOrRepeat natively, and count those calls as the originals' did
// (from their return addresses in the wrappers).

namespace {

void NoteRead(uint32_t function, uint32_t caller, uint32_t object, uint32_t mask,
              uint32_t result) {
  if (frame_pacing::GameMessages_LogInputReads()) {
    frame_pacing::GameMessages_NoteInputRead(function, caller, object, mask, result);
  }
}

}  // namespace

// AnyPressed and PressedOrRepeat (sub_82592500, sub_82592520) have no
// adapters: their only callers in play are these wrappers, whose plain C++
// they are. The wrappers count their calls as the originals' did (from the
// return addresses in the wrappers), the flag on or off.
#define ITC_INPUT_WRAPPER(address, test, function, return_address)            \
  extern "C" REX_FUNC(sub_##address) {                                         \
    REX_FUNC_PROLOGUE();                                                       \
    const uint32_t caller = uint32_t(ctx.lr);                                  \
    const uint32_t processor = ctx.r3.u32;                                     \
    const uint32_t mask = guest::LoadU32(base, ctx.r4.u32);                    \
    if (ITC_FLAG(native_input)) {                                              \
      ctx.r3.u64 = function(base, processor, mask);                            \
    } else {                                                                   \
      __imp__sub_##address(ctx, base);                                         \
    }                                                                          \
    NoteRead(0x##test, return_address, processor, mask, ctx.r3.u32 & 0xFF);    \
    NoteRead(0x##address, caller, processor, mask, ctx.r3.u32 & 0xFF);         \
  }
ITC_INPUT_WRAPPER(8258CEB0, 82592500, input::InputProcessorAnyPressed, 0x8258CED0)
ITC_INPUT_WRAPPER(8258CEE0, 82592520, input::InputProcessorPressedOrRepeat, 0x8258CF00)
#undef ITC_INPUT_WRAPPER

#define ITC_INPUT_VALUES(address, function)                                    \
  extern "C" REX_FUNC(sub_##address) {                                         \
    REX_FUNC_PROLOGUE();                                                       \
    const uint32_t caller = uint32_t(ctx.lr);                                  \
    const uint32_t pad = ctx.r3.u32;                                           \
    if (ITC_FLAG(native_input)) {                                              \
      ctx.r3.u64 = function(pad);                                              \
    } else {                                                                   \
      __imp__sub_##address(ctx, base);                                         \
    }                                                                          \
    NoteRead(0x##address, caller, pad, 0, 0);                                  \
  }
ITC_INPUT_VALUES(82591B30, input::LogicalPadValues)
ITC_INPUT_VALUES(820988B8, input::PhysicalPadValues)
#undef ITC_INPUT_VALUES

// --- The manager ---

namespace {

constexpr uint32_t kFunctorVtable = 0x82002850;  // the handler functor's
constexpr uint32_t kFunctorDone = 0x82000ABC;    // set back after the call
constexpr uint32_t kUpdateHandler = 0x820990F0;  // Input::Update, message 5
constexpr uint32_t kMessage7Handler = 0x82098CE8;

// InputManager::InputManager natively: r3 = the manager, r4 = the argument
// block. Its plain C++ around the two guest calls registering its handlers
// with the dispatcher (block +0), each with a functor on the stack, in the
// original's frame (192 bytes) at its offsets. False, with nothing done,
// where the original must run.
bool ConstructManager(PPCContext& ctx, uint8_t* base) {
  const uint32_t manager = ctx.r3.u32;
  const uint32_t block = ctx.r4.u32;
  if (!NativeLifetime() || !input::ManagerConstructMembers(base, manager, block)) {
    return false;
  }
  const uint64_t saved_lr = ctx.lr;
  guest_call::InFrame(ctx, base, 192, 0x820992C0, [&](uint32_t frame) {
    // Message 5 through the dispatcher's slot +8.
    guest::StoreU32(base, frame + 92, manager);
    guest::StoreU32(base, frame + 88, kFunctorVtable);
    guest::StoreU32(base, frame + 96, kUpdateHandler);
    guest::StoreU32(base, frame + 80, 5);
    uint32_t dispatcher = guest::LoadU32(base, block);
    ctx.r3.u64 = dispatcher;
    ctx.r4.u64 = frame + 80;
    ctx.r5.u64 = frame + 88;
    REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, dispatcher) + 8));
    // Message 7 through its slot +4.
    dispatcher = guest::LoadU32(base, block);
    guest::StoreU32(base, frame + 108, manager);
    guest::StoreU32(base, frame + 88, kFunctorDone);
    guest::StoreU32(base, frame + 104, kFunctorVtable);
    guest::StoreU32(base, frame + 112, kMessage7Handler);
    guest::StoreU32(base, frame + 80, 7);
    ctx.r3.u64 = dispatcher;
    ctx.r4.u64 = frame + 80;
    ctx.r5.u64 = frame + 104;
    ctx.lr = 0x8209930C;
    REX_CALL_INDIRECT_FUNC(guest::LoadU32(base, guest::LoadU32(base, dispatcher) + 4));
    guest::StoreU32(base, frame + 104, kFunctorDone);
  });
  input::ManagerBindPads(base, manager);
  ctx.lr = saved_lr;
  ctx.r3.u64 = manager;
  return true;
}

}  // namespace

extern "C" REX_FUNC(sub_820991E0) {
  REX_FUNC_PROLOGUE();
  if (!ConstructManager(ctx, base)) {
    __imp__sub_820991E0(ctx, base);
  }
}

// Input::Create: r3 = the argument block; r3 the manager, or 0 when the
// allocation gave none. The manager's constructor is called in the
// original's frame (96 bytes), from its call's return address.
extern "C" REX_FUNC(sub_820993E8) {
  REX_FUNC_PROLOGUE();
  const auto manager = NativeLifetime() ? input::ManagerAllocate(base) : std::nullopt;
  if (!manager) {
    __imp__sub_820993E8(ctx, base);
    return;
  }
  if (*manager == 0) {
    ctx.r3.u64 = 0;
    return;
  }
  const uint32_t block = ctx.r3.u32;
  const uint64_t saved_lr = ctx.lr;
  guest_call::InFrame(ctx, base, 96, 0x82099414, [&](uint32_t) {
    ctx.r3.u64 = *manager;
    ctx.r4.u64 = block;
    if (!ConstructManager(ctx, base)) {
      __imp__sub_820991E0(ctx, base);
    }
  });
  ctx.lr = saved_lr;
}

// AddListener (slot 3): r3 = the manager, r4 = the index's address, r5 = the
// listener; r3 the listener.
extern "C" REX_FUNC(sub_82099430) {
  REX_FUNC_PROLOGUE();
  const uint32_t listener = ctx.r5.u32;
  if (!NativeLifetime() || !input::ManagerAddListener(base, ctx.r3.u32,
                                                      guest::LoadU32(base, ctx.r4.u32),
                                                      listener)) {
    __imp__sub_82099430(ctx, base);
    return;
  }
  ctx.r3.u64 = listener;
}
