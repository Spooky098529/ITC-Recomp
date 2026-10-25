// The app framework natively (docs/functions/framework.md).

#include "framework/framework.h"

#include "guest.h"

#include "base/log.h"
#include "framework/run_loop.h"

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

}  // namespace framework
