// The app framework natively (docs/functions/framework.md).

#include "framework/framework.h"

#include "guest.h"

#include "base/log.h"
#include "framework/run_loop.h"
#include "memory/c_runtime_heap.h"

namespace framework {

uint32_t Flag133(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82090760 Framework flag +133");
  return guest::LoadU8(base, object + 133);
}

uint32_t Flag134(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82090768 Framework flag +134");
  return guest::LoadU8(base, object + 134);
}

uint32_t Word60(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82090770 word +60");
  return guest::LoadU32(base, object + 60);
}

uint32_t Word64(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82090778 word +64");
  return guest::LoadU32(base, object + 64);
}

uint32_t Word88(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82090788 word +88");
  return guest::LoadU32(base, object + 88);
}

uint32_t Word68(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8248F030 word +68");
  return guest::LoadU32(base, object + 68);
}

uint32_t Word44(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_824892F8 word +44");
  return guest::LoadU32(base, object + 44);
}

uint32_t InputManagerFrom36(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090830 Framework +36 slot 1");
  return Word64(base, self - 12);
}

uint32_t RendererFrom52(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090828 Framework +52 slot 2");
  return Word68(base, self - 44);
}

uint32_t WorkerPoolFrom52(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090838 Framework +52 slot 1");
  return Word44(base, self - 28);
}

uint32_t RendererOrTextFrom24(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090860 Framework +24 slot 4");
  return Word68(base, self - 16);
}

uint32_t SceneFrom52(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090868 Framework +52 slot 3");
  return Word60(base, self - 32);
}

uint32_t RendererFrom36(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_820908E0 Framework +36 slot 2");
  return Word68(base, self - 28);
}

uint32_t WorkerPoolFrom36(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_820908E8 Framework +36 slot 4");
  return Word44(base, self - 12);
}

uint32_t SceneFrom36(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82090908 Framework +36 slot 3");
  return Word60(base, self - 16);
}

void DebugFontSetPosition(uint8_t* base, uint32_t x, uint32_t y) {
  ITC_NATIVE_RUNNING("sub_8258BE08 DebugFont::SetPosition");
  guest::StoreU32(base, kDebugFontPosition + 8, x);
  guest::StoreU32(base, kDebugFontPosition + 12, y);
  guest::StoreU32(base, kDebugFontPosition + 0, x);
  guest::StoreU32(base, kDebugFontPosition + 4, y);
}

void DebugTextSetPosition(uint8_t* base, uint32_t x, uint32_t y) {
  ITC_NATIVE_RUNNING("sub_82091050 DebugText::SetPosition");
  DebugFontSetPosition(base, guest::LoadU32(base, x), guest::LoadU32(base, y));
}

uint32_t VirtualTarget(uint8_t* base, uint32_t object, uint32_t offset) {
  return guest::LoadU32(base, guest::LoadU32(base, object) + offset);
}

uint32_t DelegateSlot0Target(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82096330 Delegate slot 0 thunk");
  return VirtualTarget(base, object, 0);
}

uint32_t DelegateSlot1Target(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_82096340 Delegate slot 1 thunk");
  return VirtualTarget(base, object, 4);
}

uint32_t TickHandlerTarget(uint8_t* base, uint32_t delegate) {
  ITC_NATIVE_RUNNING("sub_82099A98 TickHandler::Invoke");
  return VirtualTarget(base, delegate, 4);
}

VirtualCall FrameworkStartCall(uint8_t* base, uint32_t framework) {
  ITC_NATIVE_RUNNING("sub_820902B0 Framework::Start");
  const uint32_t app = guest::LoadU32(base, framework + 120);
  return {app, VirtualTarget(base, app, 36)};
}

VirtualCall AppInputSourceCall(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_82094350 App input source");
  const uint32_t source = guest::LoadU32(base, self + 20);
  return {source, VirtualTarget(base, source, 4)};
}

VirtualCall AppInputSourceFrom16Call(uint8_t* base, uint32_t self) {
  ITC_NATIVE_RUNNING("sub_820944C8 App +16 slot +36");
  return AppInputSourceCall(base, self - 12);
}

void FrameworkOnTick(uint8_t* base, uint32_t framework, uint32_t loop, uint32_t key,
                     const OnTickCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820902C8 Framework::OnTick");
  guest::StoreU32(base, key, 0);
  calls.get_processor(guest::LoadU32(base, framework + 88), key);
  const uint32_t app = guest::LoadU32(base, framework + 120);
  guest::StoreU8(base, framework + 133, 0);
  if ((calls.app_continues(app) & 0xFF) == 0) {
    guest::StoreU32(base, key, 7);
    calls.remove_tick_handler(loop, key);
    guest::StoreU8(base, framework + 134, 1);
  }
}

std::optional<bool> RunLoopAddRequest(uint8_t* base, uint32_t loop, uint32_t type, uint32_t key,
                                      uint32_t delegate, uint32_t request,
                                      const std::function<uint32_t(uint32_t)>& clone) {
  if (type == 0) {
    ITC_NATIVE_RUNNING("sub_8209AAA8 RunLoop::AddTickHandler");
  } else {
    ITC_NATIVE_RUNNING("sub_8209AB48 RunLoop::AddListener");
  }
  guest::StoreU32(base, request + 0, type);
  guest::StoreU32(base, request + 4, guest::LoadU32(base, key));
  guest::StoreU32(base, request + 8, clone(delegate));
  return RequestsPushBack(base, loop + kRequests, request);
}

void FrameworkConstruct(uint8_t* base, uint32_t framework) {
  ITC_NATIVE_RUNNING("sub_82090420 Framework::Framework");
  static constexpr uint32_t kVtables[16] = {
      0x82000B90, 0x82000B8C, 0x82000B84, 0x82000B80, 0x82000B7C, 0x82000B70,
      0x82000B4C, 0x82000B38, 0x82000B28, 0x82000AFC, 0x82000AF4, 0x82000AEC,
      0x82000AE4, 0x82000AD4, 0x82000ACC, 0x82000AC8};
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU32(base, framework + 4 * i, kVtables[i]);
  }
  for (uint32_t at : {64u, 76u, 80u, 68u, 72u, 88u, 92u, 96u, 100u, 104u, 124u, 128u}) {
    guest::StoreU32(base, framework + at, 0);
  }
  guest::StoreU8(base, framework + 132, 0);
  guest::StoreU8(base, framework + 133, 0);
  guest::StoreU8(base, framework + 134, 0);
}

std::optional<uint32_t> FrameworkCreate(uint8_t* base, uint32_t holder) {
  if (!CanAllocate(base)) {
    return std::nullopt;
  }
  const auto block = memory::crt::OperatorNew(base, 136);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820909B8 Framework::Create");
  uint32_t framework = 0;
  if (*block != 0) {
    FrameworkConstruct(base, *block);
    framework = *block;
  }
  guest::StoreU32(base, holder, framework);
  return holder;
}

uint32_t PoolMakerConstruct(uint8_t* base, uint32_t maker, uint32_t vtable, uint32_t interface,
                            const std::function<uint32_t(uint32_t)>& pool) {
  if (vtable == 0x82002848) {
    ITC_NATIVE_RUNNING("sub_820AC8F8 PoolMaker::PoolMaker");
  } else {
    ITC_NATIVE_RUNNING("sub_820AC848 PoolMaker2::PoolMaker2");
  }
  guest::StoreU32(base, maker + 0, vtable);
  guest::StoreU32(base, maker + 4, pool(interface));
  return maker;
}

uint32_t FrameStartConstruct(uint8_t* base, uint32_t frame_start, uint32_t block, uint32_t frame,
                             const std::function<void(uint32_t loop, uint32_t offset,
                                                      uint32_t key, uint32_t delegate)>& add) {
  ITC_NATIVE_RUNNING("sub_82092710 FrameStart::FrameStart");
  constexpr uint32_t kDelegateVtable = 0x82002850;
  const uint32_t key = frame + 80;
  const uint32_t tick = frame + 88;
  const uint32_t listener = frame + 104;
  guest::StoreU32(base, tick + 4, frame_start);
  guest::StoreU32(base, frame_start + 0, 0x82000D84);
  guest::StoreU32(base, tick + 0, kDelegateVtable);
  guest::StoreU32(base, key, 4);
  guest::StoreU32(base, tick + 8, 0x82096330);
  guest::StoreU32(base, frame_start + 4, guest::LoadU32(base, block + 4));
  add(guest::LoadU32(base, block + 0), 8, key, tick);
  // The tick handler's delegate destroyed (its base vtable), the listener's
  // built.
  guest::StoreU32(base, listener + 4, frame_start);
  guest::StoreU32(base, tick + 0, 0x82000ABC);
  guest::StoreU32(base, listener + 0, kDelegateVtable);
  guest::StoreU32(base, listener + 8, 0x82096340);
  guest::StoreU32(base, key, 2);
  add(guest::LoadU32(base, block + 0), 4, key, listener);
  return frame_start;
}

uint32_t DebugTextConstruct(uint8_t* base, uint32_t text, uint32_t block, uint32_t stack_word,
                            const std::function<void(uint32_t interface, uint32_t job)>& post) {
  ITC_NATIVE_RUNNING("sub_82090EA8 DebugText::DebugText");
  const uint32_t job = text + 8;
  guest::StoreU32(base, text + 0, 0x82000BC8);
  guest::StoreU32(base, text + 4, guest::LoadU32(base, block + 0));
  guest::StoreU32(base, job + 0, 0x820066DC);
  guest::StoreU32(base, job + 4, 0x82002908);
  guest::StoreU32(base, job + 8, 0);
  guest::StoreU32(base, job + 12, 0);
  guest::StoreU8(base, job + 16, 0);
  guest::StoreU8(base, job + 17, 1);
  guest::StoreU8(base, job + 18, 0);
  guest::StoreU32(base, job + 20, stack_word);
  guest::StoreU32(base, job + 24, 0x82002850);
  guest::StoreU32(base, job + 28, text);
  guest::StoreU32(base, job + 32, 0x82090CC0);  // the debug font's load
  guest::StoreU32(base, text + 48, 0);
  guest::StoreU32(base, kDebugTextGlobal, text);
  post(guest::LoadU32(base, block + 4), job);
  return text;
}

std::optional<uint32_t> DebugTextCreate(
    uint8_t* base, uint32_t block, uint32_t stack_word,
    const std::function<void(uint32_t interface, uint32_t job)>& post) {
  if (!CanAllocate(base)) {
    return std::nullopt;
  }
  const auto text = memory::crt::OperatorNew(base, 52);
  if (!text) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82091160 DebugText::Create");
  if (*text == 0) {
    return 0u;
  }
  return DebugTextConstruct(base, *text, block, stack_word, post);
}

}  // namespace framework
