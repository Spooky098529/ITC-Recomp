// The player as the game sees them, as plain C++: one local user in slot 0,
// signed in, with the name and XUID the SDK gave
// (rexglue-sdk/src/system/xam/user_profile.cpp), and this title's id. The
// game's XDK wrappers (XUserGetName and the others) answer from here through
// src/hooks/user.cpp. See B5 in docs/rexglue-removal.md.

#pragma once

#include <algorithm>
#include <cstdint>
#include <string_view>

namespace user {

constexpr uint32_t kUserSlots = 4;
constexpr uint64_t kXuid = 0xB13EBABEBABEBABEull;
constexpr std::string_view kName = "User";
// XUserGetName copies at most this many bytes, the terminator included.
constexpr uint32_t kNameBytes = 16;

// This title's id (the image's execution info, +12).
constexpr uint32_t kTitleId = 0x555307EE;

// Sign-in states.
constexpr uint32_t kNotSignedIn = 0;
constexpr uint32_t kSignedInLocally = 1;

// The kernel's answers: HRESULTs (XamUserGetName's) and Win32 errors.
constexpr uint32_t kInvalidArgument = 0x80070057;  // E_INVALIDARG
constexpr uint32_t kNoSuchUserResult = 0x80070525;
constexpr uint32_t kErrorInvalidParameter = 87;
constexpr uint32_t kErrorNoSuchUser = 1317;
constexpr uint32_t kErrorFunctionFailed = 1627;

// XUserGetSigninState (sub_82295028).
constexpr uint32_t SigninState(uint32_t index) {
  return index == 0 ? kSignedInLocally : kNotSignedIn;
}

// XUserGetName (sub_82295020) for a buffer of `room` bytes: the HRESULT, and
// in `*bytes` how many bytes of `name` to write (the characters that fit and
// a terminator; 0 for no room or an error).
inline uint32_t Name(uint32_t index, uint32_t room, char (&name)[kNameBytes], uint32_t* bytes) {
  *bytes = 0;
  if (index >= kUserSlots) {
    return kInvalidArgument;
  }
  if (index != 0) {
    return kNoSuchUserResult;
  }
  const uint32_t limit = std::min(room, kNameBytes);
  if (limit == 0) {
    return 0;
  }
  const uint32_t characters = std::min(uint32_t(kName.size()), limit - 1);
  std::copy_n(kName.data(), characters, name);
  name[characters] = '\0';
  *bytes = characters + 1;
  return 0;
}

// XUserGetXUID (sub_82295268): a Win32 error; `*xuid` is set (0 for no
// user) unless the index is out of range.
constexpr uint32_t Xuid(uint32_t index, uint64_t* xuid) {
  if (index >= kUserSlots) {
    *xuid = 0;
    return kErrorInvalidParameter;
  }
  *xuid = index == 0 ? kXuid : 0;
  return index == 0 ? 0 : kErrorNoSuchUser;
}

// The title id check before the profile and stats calls (sub_82296EE0): 0
// for none or this title (its high half), else ERROR_FUNCTION_FAILED.
constexpr uint32_t CheckTitleId(uint32_t title_id) {
  return title_id == 0 || (title_id >> 16) == (kTitleId >> 16) ? 0 : kErrorFunctionFailed;
}

}  // namespace user
