// The game's CreateFile as plain C++ (game_data/create_file.h).

#include "game_data/create_file.h"

#include "guest.h"

#include "base/log.h"
#include "helpers/object_attributes.h"
#include "hooks/last_error.h"
#include "hooks/lifted_call.h"

#include <bit>

namespace game_data {

namespace {

constexpr uint32_t kInvalidParameter = 0xC000000D;
constexpr uint32_t kNameCollision = 0xC0000035;
constexpr uint32_t kFileIsADirectory = 0xC00000BA;
constexpr uint32_t kDosDevices = 0xFFFFFFFD;  // the root: ObDosDevices
constexpr uint32_t kCaseInsensitive = 0x40;   // OBJ_CASE_INSENSITIVE

uint32_t Rotl(uint32_t value, int bits) { return std::rotl(value, bits); }

// The NT create options the original builds from the Win32 flags, step by
// step as its rotates and masks do.
uint32_t CreateOptions(uint32_t flags) {
  const uint32_t random_access = flags & 0x10000000;     // FILE_FLAG_RANDOM_ACCESS
  const uint32_t sequential_scan = flags & 0x08000000;   // FILE_FLAG_SEQUENTIAL_SCAN
  const uint32_t backup_semantics = flags & 0x02000000;  // FILE_FLAG_BACKUP_SEMANTICS
  // The write-through (0x80000000) and no-buffering (0x20000000) bits
  // moved down as the original's rotates move them.
  uint32_t r11 = (Rotl(flags, 28) & 0x08000000) | (flags & ~0x08000000u);
  r11 = Rotl(r11, 31) & 0x1C000000;
  r11 &= ~0x08000000u;
  r11 |= sequential_scan;
  r11 = Rotl(r11, 24) & 0x00FFFFFF;
  r11 |= random_access;
  r11 = Rotl(r11, 26) & 0x03FFFFFF;
  r11 |= backup_semantics;
  r11 = Rotl(r11, 21) & 0x001FFFFF;
  // FILE_SYNCHRONOUS_IO_NONALERT unless FILE_FLAG_OVERLAPPED (0x40000000).
  r11 |= Rotl(~flags, 7) & 0x20;
  return r11;
}

}  // namespace

uint64_t CreateFile(uint8_t* base, uint32_t sp, uint64_t r13, uint32_t path, uint32_t access,
                    uint32_t sharing, uint32_t disposition, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_82295508 CreateFile");
  const uint32_t frame = lifted_call::PushFrame(base, sp, 192);
  const uint32_t pcr = uint32_t(r13);
  uint32_t nt_disposition = 0;
  switch (disposition) {
    case 1: nt_disposition = 2; break;  // CREATE_NEW: FILE_CREATE
    case 2: nt_disposition = 5; break;  // CREATE_ALWAYS: FILE_OVERWRITE_IF
    case 3: nt_disposition = 1; break;  // OPEN_EXISTING: FILE_OPEN
    case 4: nt_disposition = 3; break;  // OPEN_ALWAYS: FILE_OPEN_IF
    case 5:                             // TRUNCATE_EXISTING: FILE_OVERWRITE
      if ((access & 0x40000000) != 0) {
        nt_disposition = 4;
        break;
      }
      [[fallthrough]];
    default:
      last_error::SetFromStatus(base, pcr, kInvalidParameter);
      return ~uint64_t(0);
  }
  // The name (RtlInitAnsiString's rule) and whether it ends in a backslash.
  const uint32_t name = frame + 104;
  const helpers::AnsiString string = helpers::AnsiString_Init(
      path != 0 ? reinterpret_cast<const char*>(guest::Pointer(base, path)) : nullptr, path);
  guest::StoreU16(base, name, string.length);
  guest::StoreU16(base, name + 2, string.maximum_length);
  guest::StoreU32(base, name + 4, string.buffer);
  const uint32_t length = guest::LoadU16(base, name);
  const bool trailing_backslash = length > 1 && guest::LoadU8(base, path + length - 1) == '\\';
  // The object attributes at +120, the create options at +84 (the call's
  // ninth argument, on the frame).
  uint32_t options = CreateOptions(flags);
  guest::StoreU32(base, frame + 120, kDosDevices);
  guest::StoreU32(base, frame + 128, kCaseInsensitive);
  guest::StoreU32(base, frame + 124, name);
  if ((flags & 0x04000000) != 0) {  // FILE_FLAG_DELETE_ON_CLOSE
    options |= 0x1000;
    access |= 0x10000;  // DELETE
  }
  if ((flags & 0x02000000) == 0) {
    options |= 0x40;  // FILE_NON_DIRECTORY_FILE
  }
  guest::StoreU32(base, frame + 84, options);
  const uint32_t status = uint32_t(
      lifted_call::Import(base, __imp__NtCreateFile, frame, r13,
                          {frame + 96, access | 0x100000 | 0x80, frame + 120, frame + 112, 0,
                           flags & 0x7FA7, sharing, nt_disposition})
          .r3);
  if (int32_t(status) < 0) {
    last_error::SetFromStatus(base, pcr, status);
    if (status == kNameCollision) {
      last_error::Set(base, pcr, 80);  // ERROR_FILE_EXISTS
    } else if (status == kFileIsADirectory) {
      last_error::Set(base, pcr, trailing_backslash ? 3 : 5);
    }
    return ~uint64_t(0);
  }
  // ERROR_ALREADY_EXISTS when CREATE_ALWAYS overwrote or OPEN_ALWAYS opened.
  const uint32_t information = guest::LoadU32(base, frame + 116);
  const bool existed = (disposition == 2 && information == 3) ||
                       (disposition == 4 && information == 1);
  last_error::Set(base, pcr, existed ? 183 : 0);
  return guest::LoadU32(base, frame + 96);
}

}  // namespace game_data
