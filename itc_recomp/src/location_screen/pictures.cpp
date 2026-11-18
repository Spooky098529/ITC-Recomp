// The location screen's pictures: their loads and frees
// (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "schema/location_screen.h"

namespace location_screen {

namespace {

constexpr uint32_t kMemCommit = 0x1000;
constexpr uint32_t kMemRelease = 0x8000;
// The archive's load callbacks: LocationScreen::OnPictureLoadedA and B.
constexpr uint32_t kOnPictureLoadedA = 0x82228E88;
constexpr uint32_t kOnPictureLoadedB = 0x82228FC8;

}  // namespace

uint64_t LoadPicture(uint8_t* base, uint32_t sp, uint32_t screen, uint32_t id, uint32_t list,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222C270 LocationScreen::LoadPicture");
  const uint32_t frame = sp - 128;
  guest::StoreU32(base, frame, sp);
  const uint32_t size = archive::AssetSize(base, id);
  const uint64_t pages = calls.Call(kVirtualAlloc, frame, 0x8222C2A8, 0, size, kMemCommit, 4);
  c_runtime::XMemSet(base, uint32_t(pages), 0, size);
  uint32_t callback;
  if (list == 0) {
    callback = kOnPictureLoadedA;
  } else if (list == 1) {
    callback = kOnPictureLoadedB;
  } else {
    return pages;
  }
  const uint64_t args[4] = {id, uint32_t(pages), callback, screen};
  return calls.call(calls.user, kArchiveLoad, args, frame, 0x8222C2F4);
}

uint64_t PictureFree(uint8_t* base, uint32_t sp, uint32_t picture, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82228438 Picture::Free");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  const schema::LocationScreenPicture p(base, picture);
  const schema::LocationScreenPictureTexture record = p.texture();
  p.set_loaded(0);
  uint64_t r3 = record.address;
  if (record) {
    record.set_texture(0);
    record.set_width(0);
    record.set_height(0);
    r3 = calls.Call(kOperatorDelete, frame, 0x8222846C, record.address);
  }
  if (p.xpr().address != 0) {
    r3 = calls.Call(kXprRelease, frame, 0x82228484, p.xpr_address());
  }
  p.set_texture(0u);
  p.set_xpr(0u);
  return r3;
}

uint64_t FreePictures(uint8_t* base, uint32_t sp, uint32_t screen, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82228498 LocationScreen::FreePictures");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  uint64_t r3 = screen;
  const schema::LocationScreen s(base, screen);
  struct List {
    uint32_t pictures, count, lr;
  };
  for (const List& list : {List{s.pictures_a_address(), s.picture_count_a_address(), 0x822284E8},
                           List{s.pictures_b_address(), s.picture_count_b_address(), 0x82228534}}) {
    const uint32_t pictures = list.pictures;
    while (guest::LoadU32(base, list.count) != 0) {
      const uint32_t count = guest::LoadU32(base, list.count) - 1;
      guest::StoreU32(base, list.count, count);
      const uint32_t picture = guest::LoadU32(base, pictures + ((count << 2) & 0xFFFFFFFCu));
      PictureFree(base, frame, picture, calls);
      r3 = calls.Call(kVirtualFree, frame, list.lr, picture, 0, kMemRelease);
    }
  }
  return r3;
}

uint64_t OnPictureLoaded(uint8_t* base, uint32_t sp, uint32_t screen, uint32_t buffer,
                         uint32_t list, const Calls& calls) {
  if (list == 0)
    ITC_NATIVE_RUNNING("sub_82228E88 LocationScreen::OnPictureLoadedA");
  else
    ITC_NATIVE_RUNNING("sub_82228FC8 LocationScreen::OnPictureLoadedB");
  // The return addresses: B's code is A's 0x140 bytes on.
  const uint32_t at = list == 0 ? 0 : 0x140;
  const schema::LocationScreen s(base, screen);
  const uint32_t pictures = list == 0 ? s.pictures_a_address() : s.pictures_b_address();
  const uint32_t picture_count = list == 0 ? s.picture_count_a_address() : s.picture_count_b_address();
  const uint32_t ring = list == 0 ? s.queued_a_address() : s.queued_b_address();
  const uint32_t ring_count = list == 0 ? s.queued_count_a_address() : s.queued_count_b_address();
  const uint32_t ring_start = list == 0 ? s.queued_start_a_address() : s.queued_start_b_address();
  const uint32_t frame = sp - 160;
  guest::StoreU32(base, frame, sp);
  const uint32_t picture =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82228EB4 + at, 0, 16, 4096, 4));
  const schema::LocationScreenPicture p(base, picture);
  p.set_loaded(0);
  p.set_id(0xFFFFFFFFu);
  p.set_texture(0u);
  p.set_xpr(0u);
  uint64_t xpr = calls.Call(kOperatorNew, frame, 0x82228ED8 + at, 24);
  if (uint32_t(xpr) != 0) {
    xpr = calls.Call(kXprConstruct, frame, 0x82228EE4 + at, xpr);
  } else {
    xpr = 0;
  }
  p.set_xpr(uint32_t(xpr));
  calls.Call(kXprCreate, frame, 0x82228EF8 + at, xpr, buffer);
  const uint32_t record = uint32_t(calls.Call(kOperatorNew, frame, 0x82228F00 + at, 12));
  if (record != 0) {
    const schema::LocationScreenPictureTexture r(base, record);
    r.set_texture(0);
    r.set_width(0);
    r.set_height(0);
  }
  p.set_texture(record);
  const uint64_t texture =
      calls.Call(kXprGetTexture, frame, 0x82228F30 + at, p.xpr().address, 0);
  p.texture().set_texture(uint32_t(texture));
  const uint32_t start = guest::LoadU32(base, ring_start);
  guest::StoreU32(base, ring_count, guest::LoadU32(base, ring_count) - 1);
  guest::StoreU32(base, ring_start, start + 1);
  p.set_id(guest::LoadU32(base, ring + ((start << 2) & 0x7C)));
  calls.Call(kGetLevelDesc, frame, 0x82228F78 + at, p.texture().texture(), 0, frame + 80);
  p.texture().set_width(guest::LoadU32(base, frame + 104));
  p.texture().set_height(guest::LoadU32(base, frame + 108));
  const uint64_t r3 = calls.Call(kVirtualFree, frame, 0x82228FA4 + at, buffer, 0, kMemRelease);
  p.set_loaded(1);
  const uint32_t count = guest::LoadU32(base, picture_count);
  guest::StoreU32(base, picture_count, count + 1);
  guest::StoreU32(base, pictures + ((count << 2) & 0xFFFFFFFCu), picture);
  return r3;
}

}  // namespace location_screen
