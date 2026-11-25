// The location screen's pictures: their loads and frees
// (docs/functions/location-screen.md).

#include "location_screen/location_screen.h"

#include "archive/archive.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "hooks/heap.h"
#include "memory/handles.h"
#include "memory/heap.h"
#include "schema/location_screen.h"

namespace location_screen {

namespace {

constexpr uint32_t kMemRelease = 0x8000;
// The archive's load callbacks: LocationScreen::OnPictureLoadedA and B.
constexpr uint32_t kOnPictureLoadedA = 0x82228E88;
constexpr uint32_t kOnPictureLoadedB = 0x82228FC8;

}  // namespace

uint64_t LoadPicture(uint32_t sp, uint32_t screen, uint32_t id, uint32_t list,
                     const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8222C270 LocationScreen::LoadPicture");
  // The original's 128-byte frame held no slots (E5 step 6): the calls are
  // made from `sp`, their frames below it.
  const uint32_t size = archive::AssetSize(id);
  // The picture's file is a blob (E5 step 7) in place of the original's
  // VirtualAlloc(0, size, MEM_COMMIT, read-write) at 0x8222C2A8: whole pages,
  // as VirtualAlloc's. Only the game's code reads it (XPR::Create copies the
  // texture data out into physical memory); OnPictureLoaded deletes it.
  const uint64_t pages = schema::LocationScreenPictureFile::New(size).address;
  c_runtime::XMemSet(uint32_t(pages), 0, size);
  uint32_t callback;
  if (list == 0) {
    callback = kOnPictureLoadedA;
  } else if (list == 1) {
    callback = kOnPictureLoadedB;
  } else {
    return pages;
  }
  const uint64_t args[4] = {id, uint32_t(pages), callback, screen};
  return calls.call(calls.user, kArchiveLoad, args, sp, 0x8222C2F4);
}

uint64_t PictureFree(uint32_t sp, uint32_t picture, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82228438 Picture::Free");
  const uint32_t frame = sp - 112;
  guest::StoreU32(frame, sp);
  const schema::LocationScreenPicture p(picture);
  const schema::LocationScreenPictureTexture record = p.texture();
  p.set_loaded(0);
  uint64_t r3 = record.address;
  if (record) {
    record.set_texture(0);
    record.set_width(0);
    record.set_height(0);
    // The record is a host object (E5 step 4): deleted in place of the
    // original's operator delete at 0x8222846C. Its r3 (left as the
    // record) is overwritten by XPR::Release below: a loaded picture
    // always has its XPR.
    record.Delete();
  }
  if (p.xpr().address != 0) {
    // The picture is a host object (E5 step 3): XPR::Release takes the
    // slot's address, so it gets a frame word holding the XPR (cleared by
    // it, as the picture's +12 is just below).
    const uint32_t slot = frame + 80;
    guest::StoreU32(slot, p.xpr().address);
    r3 = calls.Call(kXprRelease, frame, 0x82228484, slot);
    p.set_xpr(guest::LoadU32(slot));
  }
  p.set_texture(0u);
  p.set_xpr(0u);
  return r3;
}

uint64_t FreePictures(uint32_t sp, uint32_t screen, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82228498 LocationScreen::FreePictures");
  // The original's 112-byte frame held no slots (E5 step 6): Picture::Free
  // makes its frame below `sp`.
  uint64_t r3 = screen;
  const schema::LocationScreen s(screen);
  struct List {
    uint32_t pictures, count;
  };
  for (const List& list : {List{s.pictures_a_address(), s.picture_count_a_address()},
                           List{s.pictures_b_address(), s.picture_count_b_address()}}) {
    const uint32_t pictures = list.pictures;
    while (guest::LoadU32(list.count) != 0) {
      const uint32_t count = guest::LoadU32(list.count) - 1;
      guest::StoreU32(list.count, count);
      const uint32_t picture = guest::LoadU32(pictures + ((count << 2) & 0xFFFFFFFCu));
      PictureFree(sp, picture, calls);
      // The host picture (OnPictureLoaded, E5 step 3): the original's
      // VirtualFree (0x822284E8 for list A, 0x82228534 for B), which
      // returned 1.
      schema::LocationScreenPicture(picture).Delete();
      r3 = 1;
    }
  }
  return r3;
}

uint64_t OnPictureLoaded(uint32_t sp, uint32_t screen, uint32_t buffer,
                         uint32_t list, const Calls& calls) {
  if (list == 0)
    ITC_NATIVE_RUNNING("sub_82228E88 LocationScreen::OnPictureLoadedA");
  else
    ITC_NATIVE_RUNNING("sub_82228FC8 LocationScreen::OnPictureLoadedB");
  // The return addresses: B's code is A's 0x140 bytes on.
  const uint32_t at = list == 0 ? 0 : 0x140;
  const schema::LocationScreen s(screen);
  const uint32_t pictures = list == 0 ? s.pictures_a_address() : s.pictures_b_address();
  const uint32_t picture_count = list == 0 ? s.picture_count_a_address() : s.picture_count_b_address();
  const uint32_t ring = list == 0 ? s.queued_a_address() : s.queued_b_address();
  const uint32_t ring_count = list == 0 ? s.queued_count_a_address() : s.queued_count_b_address();
  const uint32_t ring_start = list == 0 ? s.queued_start_a_address() : s.queued_start_b_address();
  const uint32_t frame = sp - 160;
  guest::StoreU32(frame, sp);
  // The picture lives in host memory (E5 step 3): the original's
  // VirtualAlloc(16) at 0x82228EB4 (+at); the list keeps its handle.
  const schema::LocationScreenPicture p = schema::LocationScreenPicture::New();
  const uint32_t picture = p.address;
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
  // The texture record lives in host memory (E5 step 4): the original's
  // operator new(12) at 0x82228F00 (+at); the picture's +8 keeps its handle
  // (UiMovie::SetPictureTexture puts it in movie 21's texture table, whose
  // readers load its words through guest::LoadU32, which resolves handles).
  const schema::LocationScreenPictureTexture r = schema::LocationScreenPictureTexture::New();
  const uint32_t record = r.address;
  r.set_texture(0);
  r.set_width(0);
  r.set_height(0);
  p.set_texture(record);
  const uint64_t texture =
      calls.Call(kXprGetTexture, frame, 0x82228F30 + at, p.xpr().address, 0);
  p.texture().set_texture(uint32_t(texture));
  const uint32_t start = guest::LoadU32(ring_start);
  guest::StoreU32(ring_count, guest::LoadU32(ring_count) - 1);
  guest::StoreU32(ring_start, start + 1);
  p.set_id(guest::LoadU32(ring + ((start << 2) & 0x7C)));
  calls.Call(kGetLevelDesc, frame, 0x82228F78 + at, p.texture().texture(), 0, frame + 80);
  p.texture().set_width(guest::LoadU32(frame + 104));
  p.texture().set_height(guest::LoadU32(frame + 108));
  // VirtualFree(buffer, 0, MEM_RELEASE): the picture's file is a blob (E5
  // step 7, LoadPicture), deleted (the original's VirtualFree returned 1);
  // anything else as its adapter answers it (E5 step 6: hooks::VirtualFree),
  // else the call.
  std::optional<uint32_t> freed;
  if (buffer != 0 && memory::IsHandle(buffer)) {
    schema::LocationScreenPictureFile(buffer).Delete();
    freed = 1;
  } else if (buffer != 0) {
    freed = hooks::VirtualFree(buffer, 0, kMemRelease, 0x82228FA4 + at);
  }
  const uint64_t r3 =
      freed ? *freed : calls.Call(kVirtualFree, frame, 0x82228FA4 + at, buffer, 0, kMemRelease);
  p.set_loaded(1);
  const uint32_t count = guest::LoadU32(picture_count);
  guest::StoreU32(picture_count, count + 1);
  guest::StoreU32(pictures + ((count << 2) & 0xFFFFFFFCu), picture);
  return r3;
}

}  // namespace location_screen
