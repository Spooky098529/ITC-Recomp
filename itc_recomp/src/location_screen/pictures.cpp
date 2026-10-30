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

}  // namespace location_screen
