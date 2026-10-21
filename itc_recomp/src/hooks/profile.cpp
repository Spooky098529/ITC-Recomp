// XUserReadProfileSettings without its kernel call: the adapter in front of
// src/user/profile.cpp, for the two reads the game makes (the size, then the
// settings with an overlapped). See B5 in docs/rexglue-removal.md.

#include "guest.h"

#include "user/profile.h"
#include "user/user.h"

namespace {

constexpr uint32_t kErrorInsufficientBuffer = 122;
constexpr uint32_t kErrorIoPending = 997;

// The answer without an overlapped, or ERROR_IO_PENDING with the answer
// completed in it as the SDK did: its result, length (0, or -1 for an error)
// and extended error. The game's has no event or completion routine.
uint32_t Answer(uint8_t* base, uint32_t overlapped, uint32_t result) {
  if (!overlapped) {
    return result;
  }
  guest::StoreU32(base, overlapped + 0, result);
  guest::StoreU32(base, overlapped + 4, result ? 0xFFFFFFFF : 0);
  guest::StoreU32(base, overlapped + 24, result);
  return kErrorIoPending;
}

}  // namespace

// XUserReadProfileSettings: r4 user, r7 the buffer's size (in, out), r8 the
// buffer, r9 an overlapped (or 0); the title (r3), the count (r5) and the ids
// (r6) are always the network manager's. Returns ERROR_INSUFFICIENT_BUFFER
// with the size for the first read, then ERROR_IO_PENDING with the settings.
extern "C" REX_FUNC(sub_82576E78) {
  REX_FUNC_PROLOGUE();
  const uint32_t user_index = ctx.r4.u32, size_address = ctx.r7.u32, buffer = ctx.r8.u32;
  const uint32_t overlapped = ctx.r9.u32;
  const uint32_t size = guest::LoadU32(base, size_address);
  if (!buffer || size < user::kProfileBytes) {
    if (!size) {
      guest::StoreU32(base, size_address, user::kProfileBytes);
    }
    ctx.r3.u64 = kErrorInsufficientBuffer;
    return;
  }
  if (user_index) {
    ctx.r3.u64 = Answer(base, overlapped, user::kErrorNoSuchUser);
    return;
  }
  std::vector<uint8_t> bytes(user::kProfileBytes);
  for (uint32_t i = 0; i < user::kProfileBytes; i++) {
    bytes[i] = guest::LoadU8(base, buffer + i);
  }
  user::ProfileSettings(user_index, buffer, bytes);
  for (uint32_t i = 0; i < user::kProfileBytes; i++) {
    guest::StoreU8(base, buffer + i, bytes[i]);
  }
  ctx.r3.u64 = Answer(base, overlapped, 0);
}
