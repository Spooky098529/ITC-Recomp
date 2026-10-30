// The app's second camera class natively (start_up.h,
// docs/functions/cameras.md: the object at the app's +120, made by App::App,
// with the camera it owns over Camera::Camera).

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

namespace app_start_up {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// Four words (a vector) from `from` to `to`, as the originals' ld/std pairs.
void Copy16(uint8_t* base, uint32_t to, uint32_t from) {
  uint32_t words[4];
  for (int i = 0; i < 4; i++) {
    words[i] = Load(base, from + 4 * i);
  }
  for (int i = 0; i < 4; i++) {
    Store(base, to + 4 * i, words[i]);
  }
}
void StoreVector(uint8_t* base, uint32_t to, uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
  Store(base, to + 0, x);
  Store(base, to + 4, y);
  Store(base, to + 8, z);
  Store(base, to + 12, w);
}

// The floats the constructors load (lfs) and store (stfs), as their bits.
constexpr uint32_t kOne = 0x3F800000;         // [0x82000A30]
constexpr uint32_t kZero = 0x00000000;        // [0x82000A34]
constexpr uint32_t kFortyFive = 0x42340000;   // [0x8200267C], [0x82002564]
constexpr uint32_t kMinusTen = 0xC1200000;    // [0x8200277C]
constexpr uint32_t kMinusFortyFive = 0xC2340000;  // [0x820027A0]
constexpr uint32_t kMinusEightyNine = 0xC2B20000;  // [0x8200279C]
constexpr uint32_t kEightyNine = 0x42B20000;  // [0x82002798]
constexpr uint32_t kPointZeroNine = 0x3DB851EC;  // [0x820027AC]
constexpr uint32_t kOnePointFive = 0x3FC00000;  // [0x820027A8]
constexpr uint32_t kThirty = 0x41F00000;      // [0x82000D44]
constexpr uint32_t kTwoHundred = 0x43480000;  // [0x820027A4]
constexpr uint32_t kTen = 0x41200000;         // [0x82001FD8]

constexpr uint32_t kCameraConstruct = 0x82593908;  // Camera::Camera
constexpr uint32_t kMemberDelegateVtable = 0x8200833C;

}  // namespace

uint32_t AppCameraEntryConstruct(uint8_t* base, uint32_t entry) {
  ITC_NATIVE_RUNNING("sub_820A75E8 AppCameraEntry::AppCameraEntry");
  Store(base, entry + 0, 0x82002580);
  Store(base, entry + 4, 0);
  Store(base, entry + 8, 0);
  Store(base, entry + 12, 0);
  return entry;
}

std::optional<uint32_t> AppCameraListConstruct(uint8_t* base, uint32_t list) {
  Store(base, list + 0, 0x82002780);
  const auto block = memory::crt::OperatorNew(base, 16);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A98C0 AppCameraList::AppCameraList");
  // A std::vector (16 bytes: +4 the first, +8 the last, +12 the end; +0 not
  // written), cleared (sub_820A9198: erase(begin, end), sub_820A8F80, its
  // iterator checks never failing on one vector).
  if (*block != 0) {
    Store(base, *block + 4, 0);
    Store(base, *block + 8, 0);
    Store(base, *block + 12, 0);
  }
  Store(base, list + 4, *block);
  if (*block != 0) {
    const uint32_t first = Load(base, *block + 4);
    const uint32_t last = Load(base, *block + 8);
    if (first != last) {
      Store(base, *block + 8, first);
    }
  }
  return list;
}

uint32_t AppCameraCameraConstruct(uint8_t* base, uint32_t camera, uint32_t frame,
                                  const AppCameraCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820A9E28 AppCamera::Camera::Camera");
  calls.call(kCameraConstruct, frame, camera, 0x820A9E40);
  Store(base, camera + 0, 0x820027B0);
  for (const uint32_t at : {412u, 428u, 444u, 460u, 476u, 492u, 508u, 524u, 572u, 588u, 604u,
                            620u, 636u, 652u}) {
    Store(base, camera + at, kOne);
  }
  StoreVector(base, frame + 80, kZero, kZero, kZero, kOne);
  StoreVector(base, frame + 96, kZero, kZero, kZero, kOne);
  Store(base, camera + 668, kOne);
  Store(base, camera + 812, kMinusFortyFive);
  Store(base, camera + 816, kFortyFive);
  Store(base, camera + 804, kMinusEightyNine);
  Store(base, camera + 808, kEightyNine);
  Store(base, camera + 672, kFortyFive);
  Store(base, camera + 676, kPointZeroNine);
  Store(base, camera + 680, kOnePointFive);
  Store(base, camera + 684, kZero);
  Store(base, camera + 688, kThirty);
  Store(base, camera + 692, kTwoHundred);
  Copy16(base, camera + 624, frame + 80);
  Copy16(base, camera + 640, frame + 96);
  StoreVector(base, frame + 80, kZero, kZero, kZero, kOne);
  StoreVector(base, frame + 96, kZero, kZero, kZero, kOne);
  StoreVector(base, frame + 112, kZero, kZero, kTen, kOne);
  Copy16(base, camera + 656, frame + 96);
  Copy16(base, camera + 400, frame + 80);
  Copy16(base, camera + 416, frame + 112);
  Copy16(base, camera + 432, camera + 272);  // Camera::Camera's position
  Copy16(base, camera + 448, camera + 416);
  Copy16(base, camera + 464, camera + 416);
  return camera;
}

std::optional<uint32_t> AppCameraConstruct(uint8_t* base, uint32_t object, uint32_t block,
                                           uint32_t frame, const AppCameraCalls& calls) {
  Store(base, object + 4, 0);
  Store(base, object + 8, 0x82002680);
  Store(base, object + 12, 0x82002690);
  Store(base, object + 0, 0x820026B8);
  Store(base, object + 8, 0x820026A8);
  Store(base, object + 12, 0x8200269C);
  Store(base, object + 16, Load(base, block + 4));
  for (const uint32_t at : {60u, 76u, 108u, 124u, 140u, 156u, 172u, 188u, 204u, 220u, 252u, 268u,
                            284u, 300u}) {
    Store(base, object + at, kOne);
  }
  const auto camera_block = memory::crt::OperatorNew(base, kAppCameraCameraBytes);
  if (!camera_block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A8B88 AppCamera::AppCamera");
  Store(base, object + 28,
        *camera_block != 0 ? calls.camera(*camera_block) : 0);
  const auto list_block = memory::crt::OperatorNew(base, 8);
  uint32_t list = 0;
  if (list_block && *list_block != 0) {
    list = calls.list(*list_block);
  } else if (!list_block) {
    list = calls.call(0x825A6880, frame, 8, 0x820A8C58);  // operator new's original
    if (list != 0) {
      list = calls.list(list);
    }
  }
  Store(base, object + 32, list);
  const auto entry_block = memory::crt::OperatorNew(base, 16);
  uint32_t entry = 0;
  if (entry_block) {
    entry = *entry_block != 0 ? AppCameraEntryConstruct(base, *entry_block) : 0;
  } else {
    entry = calls.call(0x825A6880, frame, 16, 0x820A8C7C);
    if (entry != 0) {
      AppCameraEntryConstruct(base, entry);
    }
  }
  // The list's slot 1 with the entry; the camera's slot 1 (45.0).
  calls.virtual_call(Load(base, object + 32), 4, frame, entry, {}, {}, 0x820A8CAC);
  calls.virtual_call(Load(base, object + 28), 4, frame, {}, {}, kFortyFive, 0x820A8CC8);
  Store(base, object + 316, 0);
  Store(base, object + 320, 0);
  Store(base, object + 340, 0);
  Store(base, object + 324, 1);
  Store(base, object + 328, 1);
  Store(base, object + 332, 10);
  Store(base, object + 336, 10);
  Store(base, object + 344, 2);
  // The block's +4 interface: its slot 1's object's slot 3 (the screen: its
  // +296 and +300).
  const uint32_t service = calls.virtual_call(Load(base, block + 4), 4, frame, {}, {}, {}, 0x820A8D08);
  const uint32_t screen = calls.virtual_call(service, 12, frame, {}, {}, {}, 0x820A8D18);
  Store(base, frame + 92, kOne);
  Store(base, frame + 108, kOne);
  Store(base, frame + 80, kZero);
  Store(base, frame + 96, kZero);
  Store(base, frame + 100, kZero);
  Store(base, frame + 104, kZero);
  Store(base, frame + 84, kMinusTen);
  Store(base, frame + 88, kMinusTen);
  const uint32_t camera = Load(base, object + 28);
  Copy16(base, camera + 272, frame + 80);
  Copy16(base, Load(base, object + 28) + 288, frame + 96);
  // The camera's slot 0 (the screen's size), then slot 1 again (45.0).
  calls.virtual_call(Load(base, object + 28), 0, frame, Load(base, screen + 296),
                     Load(base, screen + 300), kZero, 0x820A8D98);
  calls.virtual_call(Load(base, object + 28), 4, frame, {}, {}, kFortyFive, 0x820A8DB0);
  // Three handlers (key 0 each) on the block's +0 interface: slots 4, 3 and
  // 1 with member delegates on the frame (+112, +128, +144), the word before
  // the second and third left 0x82000ABC.
  struct Handler {
    uint32_t function, delegate, slot, ret;
  };
  const Handler handlers[] = {
      {0x820A8860, frame + 112, 16, 0x820A8DF8},
      {0x820A8840, frame + 128, 12, 0x820A8E44},
      {0x820A8850, frame + 144, 4, 0x820A8E90},
  };
  for (int i = 0; i < 3; i++) {
    const Handler& h = handlers[i];
    Store(base, frame + 84, 0);
    Store(base, frame + 80, h.function);
    Store(base, h.delegate + 0, kMemberDelegateVtable);
    Store(base, h.delegate + 4, object);
    Store(base, h.delegate + 8, h.function);
    Store(base, h.delegate + 12, 0);
    if (i > 0) {
      Store(base, handlers[i - 1].delegate, 0x82000ABC);
    }
    Store(base, frame + 80, 0);
    calls.virtual_call(Load(base, block + 0), h.slot, frame, frame + 80, h.delegate, {}, h.ret);
  }
  return object;
}

}  // namespace app_start_up
