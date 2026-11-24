// The app's second camera class natively (start_up.h,
// docs/functions/cameras.md: the object at the app's +120, made by App::App,
// with the camera it owns over Camera::Camera).

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/cameras.h"
#include "schema/video_output.h"

namespace app_start_up {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void StoreVector(uint8_t* base, uint32_t to, uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
  Store(base, to + 0, x);
  Store(base, to + 4, y);
  Store(base, to + 8, z);
  Store(base, to + 12, w);
}

// The objects are host (E5 step 4): their words through the view, at the
// field's address (`at`, an interior handle) less the object's.
template <typename V>
uint32_t LoadIn(const V& v, uint32_t at) {
  return uint32_t(v.load_at(at - v.address, 4));
}
template <typename V>
void StoreIn(const V& v, uint32_t at, uint32_t value) {
  v.store_at(at - v.address, 4, value);
}
// Four words (a vector) from `from` to `to`, as the originals' ld/std pairs:
// from guest memory (the frame) into a host object.
template <typename V>
void Copy16In(uint8_t* base, const V& v, uint32_t to, uint32_t from) {
  uint32_t words[4];
  for (int i = 0; i < 4; i++) {
    words[i] = Load(base, from + 4 * i);
  }
  for (int i = 0; i < 4; i++) {
    StoreIn(v, to + 4 * i, words[i]);
  }
}
// The same within a host object.
template <typename V>
void Copy16Within(const V& v, uint32_t to, uint32_t from) {
  uint32_t words[4];
  for (int i = 0; i < 4; i++) {
    words[i] = LoadIn(v, from + 4 * i);
  }
  for (int i = 0; i < 4; i++) {
    StoreIn(v, to + 4 * i, words[i]);
  }
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
  const schema::AppCameraEntry e(base, entry);
  e.set_vtable(0x82002580);
  e.set_unknown_4(0);
  e.set_unknown_8(0);
  e.set_unknown_12(0);
  return entry;
}

std::optional<uint32_t> AppCameraListConstruct(uint8_t* base, uint32_t list) {
  const schema::AppCameraList l(base, list);
  l.set_vtable(0x82002780);
  const auto block = memory::crt::OperatorNew(base, 16);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820A98C0 AppCameraList::AppCameraList");
  // A std::vector (16 bytes: +4 the first, +8 the last, +12 the end; +0 not
  // written), cleared (sub_820A9198: erase(begin, end), sub_820A8F80, its
  // iterator checks never failing on one vector).
  const schema::NewAny_MessageVector_Allocate vector(base, *block);
  if (*block != 0) {
    vector.set_first(0);
    vector.set_last(0);
    vector.set_end(0);
  }
  l.set_vector(*block);
  if (*block != 0) {
    const uint32_t first = vector.first();
    const uint32_t last = vector.last();
    if (first != last) {
      vector.set_last(first);
    }
  }
  return list;
}

uint32_t AppCameraCameraConstruct(uint8_t* base, uint32_t camera, uint32_t frame,
                                  const AppCameraCalls& calls) {
  ITC_NATIVE_RUNNING("sub_820A9E28 AppCamera::Camera::Camera");
  calls.call(kCameraConstruct, frame, camera, 0x820A9E40);
  const schema::AppCameraCamera c(base, camera);
  c.set_vtable(0x820027B0);
  // Each vector's fourth word.
  for (const uint32_t at :
       {c.unknown_400_address(), c.unknown_416_address(), c.unknown_432_address(),
        c.unknown_448_address(), c.unknown_464_address(), c.unknown_480_address(),
        c.unknown_496_address(), c.unknown_512_address(), c.unknown_560_address(),
        c.unknown_576_address(), c.unknown_592_address(), c.unknown_608_address(),
        c.unknown_624_address(), c.unknown_640_address()}) {
    StoreIn(c, at + 12, kOne);
  }
  StoreVector(base, frame + 80, kZero, kZero, kZero, kOne);
  StoreVector(base, frame + 96, kZero, kZero, kZero, kOne);
  StoreIn(c, c.unknown_656_address() + 12, kOne);
  StoreIn(c, c.unknown_812_address(), kMinusFortyFive);
  StoreIn(c, c.unknown_816_address(), kFortyFive);
  StoreIn(c, c.unknown_804_address(), kMinusEightyNine);
  StoreIn(c, c.unknown_808_address(), kEightyNine);
  StoreIn(c, c.unknown_672_address(), kFortyFive);
  StoreIn(c, c.unknown_676_address(), kPointZeroNine);
  StoreIn(c, c.unknown_680_address(), kOnePointFive);
  StoreIn(c, c.unknown_684_address(), kZero);
  StoreIn(c, c.unknown_688_address(), kThirty);
  StoreIn(c, c.unknown_692_address(), kTwoHundred);
  Copy16In(base, c, c.unknown_624_address(), frame + 80);
  Copy16In(base, c, c.unknown_640_address(), frame + 96);
  StoreVector(base, frame + 80, kZero, kZero, kZero, kOne);
  StoreVector(base, frame + 96, kZero, kZero, kZero, kOne);
  StoreVector(base, frame + 112, kZero, kZero, kTen, kOne);
  Copy16In(base, c, c.unknown_656_address(), frame + 96);
  Copy16In(base, c, c.unknown_400_address(), frame + 80);
  Copy16In(base, c, c.unknown_416_address(), frame + 112);
  Copy16Within(c, c.unknown_432_address(), c.position_x_address());  // Camera::Camera's position
  Copy16Within(c, c.unknown_448_address(), c.unknown_416_address());
  Copy16Within(c, c.unknown_464_address(), c.unknown_416_address());
  return camera;
}

std::optional<uint32_t> AppCameraConstruct(uint8_t* base, uint32_t object, uint32_t block,
                                           uint32_t frame, const AppCameraCalls& calls) {
  const schema::AppCamera a(base, object);
  const schema::AppCameraBlock b(base, block);
  a.set_unknown_4(0);
  a.set_vtable_8(0x82002680);
  a.set_vtable_12(0x82002690);
  a.set_vtable(0x820026B8);
  a.set_vtable_8(0x820026A8);
  a.set_vtable_12(0x8200269C);
  a.set_service(b.service());
  // The fourth word of each vector: the two at +48, the matrices' rows.
  for (const uint32_t at :
       {a.unknown_48_address(), a.unknown_48_address() + 16, a.unknown_96_address(),
        a.unknown_96_address() + 16, a.unknown_96_address() + 32, a.unknown_96_address() + 48,
        a.unknown_160_address(), a.unknown_160_address() + 16, a.unknown_160_address() + 32,
        a.unknown_160_address() + 48, a.unknown_240_address(), a.unknown_240_address() + 16,
        a.unknown_240_address() + 32, a.unknown_240_address() + 48}) {
    StoreIn(a, at + 12, kOne);
  }
  // The camera, the list and the entry in host memory (E5 step 4; operator
  // new's blocks before: 0x820A8C34, 0x820A8C58 and 0x820A8C7C).
  static_assert(schema::AppCameraCamera::kSize == kAppCameraCameraBytes);
  const uint32_t camera_block = schema::AppCameraCamera::New(base).address;
  ITC_NATIVE_RUNNING("sub_820A8B88 AppCamera::AppCamera");
  a.set_camera(calls.camera(camera_block));
  const uint32_t list = calls.list(schema::AppCameraList::New(base).address);
  a.set_list(list);
  const uint32_t entry = AppCameraEntryConstruct(base, schema::AppCameraEntry::New(base).address);
  // The list's slot 1 with the entry; the camera's slot 1 (45.0).
  calls.virtual_call(a.list().address, 4, frame, entry, {}, {}, 0x820A8CAC);
  calls.virtual_call(a.camera().address, 4, frame, {}, {}, kFortyFive, 0x820A8CC8);
  a.set_unknown_316(0);
  a.set_unknown_320(0);
  a.set_unknown_340(0);
  a.set_unknown_324(1);
  a.set_unknown_328(1);
  a.set_unknown_332(10);
  a.set_unknown_336(10);
  a.set_unknown_344(2);
  // The block's +4 interface: its slot 1's object's slot 3 (the screen: the
  // video output, video-output.toml's).
  const uint32_t service = calls.virtual_call(b.service().address, 4, frame, {}, {}, {}, 0x820A8D08);
  const uint32_t screen = calls.virtual_call(service, 12, frame, {}, {}, {}, 0x820A8D18);
  Store(base, frame + 92, kOne);
  Store(base, frame + 108, kOne);
  Store(base, frame + 80, kZero);
  Store(base, frame + 96, kZero);
  Store(base, frame + 100, kZero);
  Store(base, frame + 104, kZero);
  Store(base, frame + 84, kMinusTen);
  Store(base, frame + 88, kMinusTen);
  const auto camera = a.camera();
  Copy16In(base, camera, camera.position_x_address(), frame + 80);
  Copy16In(base, a.camera(), a.camera().angles_x_address(), frame + 96);
  // The camera's slot 0 (the screen's size), then slot 1 again (45.0).
  calls.virtual_call(a.camera().address, 0, frame, schema::VideoOutput(base, screen).display_width(),
                     schema::VideoOutput(base, screen).display_height(), kZero, 0x820A8D98);
  calls.virtual_call(a.camera().address, 4, frame, {}, {}, kFortyFive, 0x820A8DB0);
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
    calls.virtual_call(b.handlers().address, h.slot, frame, frame + 80, h.delegate, {}, h.ret);
  }
  return object;
}

}  // namespace app_start_up
