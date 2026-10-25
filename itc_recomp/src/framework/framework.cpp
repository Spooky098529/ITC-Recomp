// The app framework natively (docs/functions/framework.md).

#include "framework/framework.h"

#include "guest.h"

#include "base/log.h"

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

}  // namespace framework
