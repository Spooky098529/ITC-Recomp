// The location screen's pictures: their loads and frees
// (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"

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
  const uint32_t record = guest::LoadU32(base, picture + 8);
  guest::StoreU32(base, picture, 0);
  uint64_t r3 = record;
  if (record != 0) {
    for (uint32_t offset : {0u, 4u, 8u}) {
      guest::StoreU32(base, record + offset, 0);
    }
    r3 = calls.Call(kOperatorDelete, frame, 0x8222846C, record);
  }
  const uint32_t slot = picture + 12;
  if (guest::LoadU32(base, slot) != 0) {
    r3 = calls.Call(kXprRelease, frame, 0x82228484, slot);
  }
  guest::StoreU32(base, picture + 8, 0);
  guest::StoreU32(base, slot, 0);
  return r3;
}

uint64_t FreePictures(uint8_t* base, uint32_t sp, uint32_t screen, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82228498 LocationScreen::FreePictures");
  const uint32_t frame = sp - 112;
  guest::StoreU32(base, frame, sp);
  uint64_t r3 = screen;
  struct List {
    uint32_t offset, lr;
  };
  for (const List& list : {List{8, 0x822284E8}, List{276, 0x82228534}}) {
    const uint32_t pictures = screen + list.offset;
    while (guest::LoadU32(base, pictures + 128) != 0) {
      const uint32_t count = guest::LoadU32(base, pictures + 128) - 1;
      guest::StoreU32(base, pictures + 128, count);
      const uint32_t picture = guest::LoadU32(base, pictures + ((count << 2) & 0xFFFFFFFCu));
      PictureFree(base, frame, picture, calls);
      r3 = calls.Call(kVirtualFree, frame, list.lr, picture, 0, kMemRelease);
    }
  }
  return r3;
}

uint64_t OnPictureLoaded(uint8_t* base, uint32_t sp, uint32_t screen, uint32_t buffer,
                         uint32_t list, const Calls& calls) {
  ITC_NATIVE_RUNNING(list == 0 ? "sub_82228E88 LocationScreen::OnPictureLoadedA"
                               : "sub_82228FC8 LocationScreen::OnPictureLoadedB");
  // The return addresses: B's code is A's 0x140 bytes on.
  const uint32_t at = list == 0 ? 0 : 0x140;
  const uint32_t pictures = screen + (list == 0 ? 8 : 276);
  const uint32_t ring = screen + (list == 0 ? 140 : 408);
  const uint32_t frame = sp - 160;
  guest::StoreU32(base, frame, sp);
  const uint32_t picture =
      uint32_t(calls.Call(kVirtualAlloc, frame, 0x82228EB4 + at, 0, 16, 4096, 4));
  guest::StoreU32(base, picture + 0, 0);
  guest::StoreU32(base, picture + 4, 0xFFFFFFFFu);
  guest::StoreU32(base, picture + 8, 0);
  guest::StoreU32(base, picture + 12, 0);
  uint64_t xpr = calls.Call(kOperatorNew, frame, 0x82228ED8 + at, 24);
  if (uint32_t(xpr) != 0) {
    xpr = calls.Call(kXprConstruct, frame, 0x82228EE4 + at, xpr);
  } else {
    xpr = 0;
  }
  guest::StoreU32(base, picture + 12, uint32_t(xpr));
  calls.Call(kXprCreate, frame, 0x82228EF8 + at, xpr, buffer);
  const uint32_t record = uint32_t(calls.Call(kOperatorNew, frame, 0x82228F00 + at, 12));
  if (record != 0) {
    guest::StoreU32(base, record + 0, 0);
    guest::StoreU32(base, record + 4, 0);
    guest::StoreU32(base, record + 8, 0);
  }
  guest::StoreU32(base, picture + 8, record);
  const uint64_t texture =
      calls.Call(kXprGetTexture, frame, 0x82228F30 + at, guest::LoadU32(base, picture + 12), 0);
  guest::StoreU32(base, guest::LoadU32(base, picture + 8), uint32_t(texture));
  const uint32_t start = guest::LoadU32(base, ring + 132);
  guest::StoreU32(base, ring + 128, guest::LoadU32(base, ring + 128) - 1);
  guest::StoreU32(base, ring + 132, start + 1);
  guest::StoreU32(base, picture + 4, guest::LoadU32(base, ring + ((start << 2) & 0x7C)));
  calls.Call(kGetLevelDesc, frame, 0x82228F78 + at,
             guest::LoadU32(base, guest::LoadU32(base, picture + 8)), 0, frame + 80);
  guest::StoreU32(base, guest::LoadU32(base, picture + 8) + 4, guest::LoadU32(base, frame + 104));
  guest::StoreU32(base, guest::LoadU32(base, picture + 8) + 8, guest::LoadU32(base, frame + 108));
  const uint64_t r3 = calls.Call(kVirtualFree, frame, 0x82228FA4 + at, buffer, 0, kMemRelease);
  guest::StoreU32(base, picture + 0, 1);
  const uint32_t count = guest::LoadU32(base, pictures + 128);
  guest::StoreU32(base, pictures + 128, count + 1);
  guest::StoreU32(base, pictures + ((count << 2) & 0xFFFFFFFCu), picture);
  return r3;
}

}  // namespace location_screen
