// The network start-up (from the network manager's set-up, sub_8214E190)
// without its kernel calls. System link and Xbox Live are Xbox-only: the
// system link socket isn't made, and the XNet wrappers give the SDK's "not
// available" answers as fixed values. And Network::Update stubbed with the
// data the game reads from it. See B5 in docs/rexglue-removal.md.

#include "guest.h"
#include "guest_call.h"

#include "user/user.h"

#include <algorithm>

using guest_call::InFrame;

namespace {

// The system link socket's handle (sub_82148958 keeps it here; read only by
// the system link code's sends and receives).
constexpr uint32_t kSystemLinkSocket = 0x826C6AA0;
constexpr uint32_t kInvalidSocket = 0xFFFFFFFF;

// XNetGetTitleXnAddr's answer: XNET_GET_XNADDR_STATIC.
constexpr uint32_t kXnAddrStatic = 4;

}  // namespace

// WSAStartup: r3 = version, r4 = the WSADATA (out, in the caller's frame,
// never read). Returns 0, as the SDK did.
extern "C" REX_FUNC(sub_82459B68) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = 0;
}

// Makes the system link socket (UDP, port 1000, non-blocking): not made, so
// the handle is INVALID_SOCKET and system link stays off.
extern "C" REX_FUNC(sub_82148958) {
  REX_FUNC_PROLOGUE();
  guest::StoreU32(base, kSystemLinkSocket, kInvalidSocket);
}

// XNetGetTitleXnAddr: r3 = the XNADDR (36 bytes, out). The SDK's: the
// loopback address, no online address or port, CC for each Ethernet address
// byte, no online id.
extern "C" REX_FUNC(sub_82459D50) {
  REX_FUNC_PROLOGUE();
  const uint32_t address = ctx.r3.u32;
  guest::StoreU32(base, address + 0, 0x7F000001);  // ina
  guest::StoreU32(base, address + 4, 0);           // inaOnline
  guest::StoreU16(base, address + 8, 0);           // wPortOnline
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU8(base, address + 10 + i, 0xCC);  // abEnet
  }
  for (uint32_t i = 0; i < 20; i++) {
    guest::StoreU8(base, address + 16 + i, 0);  // abOnline
  }
  ctx.r3.u64 = kXnAddrStatic;
}

// XNetXnAddrToMachineId: r3 = the XNADDR, r4 = the id (out, left alone).
// Returns 1 (not signed in to Live), as the SDK did.
extern "C" REX_FUNC(sub_82459D40) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = 1;
}

// Network::Update (every tick, from sub_82107910) and the sign-in helper it
// ran (SignIn::Update, sub_8214F290, no longer called), stubbed: what the
// game reads from them in play is one signed-in local user (written once)
// and the profile read again when the front end asks. Left out, Xbox-only:
// the Live users' voice registration, XNet, the Live connection and 0x11
// notifications, voice chat, the system link state machine (system link
// doesn't work), and the sign-in dialog and "VS MODE" message box (the
// bounds are 0-4 users, so one never triggers them).
namespace {

constexpr uint32_t kProfileOverlapped = 0x826C6080;  // 997 while a read is pending
constexpr uint32_t kProfileResults = 0x826C6AD0;
constexpr uint32_t kProfileRequested = 0x826C7938;  // set by the controller assignments
constexpr uint32_t kProfileWasRead = 0x826C793C;
constexpr uint32_t kSignInPolled = 0x826C7950;
constexpr uint32_t kSignedInMask = 0x826C7954;
constexpr uint32_t kSignedInCount = 0x826C7958;
constexpr uint32_t kLiveMask = 0x826C795C;
constexpr uint32_t kSignInChecked = 0x826C796C;
constexpr uint32_t kUsersPolled = 0x826C7998;
constexpr uint32_t kFirstUser = 0x82618B84;
constexpr uint32_t kPlayerControllers = 0x826BABA4;  // the user on player 1's controller
constexpr uint32_t kUserTable = 0x826C7670;          // +12 count, +14 names, +144 XUIDs
constexpr uint32_t kNameSlotBytes = 32;
// Where the gamercard region (profile setting 3) is copied after a read.
constexpr uint32_t kRegionCopies[] = {0x826C77AC, 0x826C77B0, 0x826C77B4, 0x826C77B8,
                                      0x826C6728, 0x826C7288};

// The sign-in poll: user 0 signed in locally (src/user/user.h), none on Live.
// True the first time (the users' names and XUIDs are written then).
bool PollUsers(uint8_t* base) {
  if (guest::LoadU32(base, kUsersPolled)) {
    return false;
  }
  guest::StoreU32(base, kUsersPolled, 1);
  uint32_t mask = 0, count = 0, first = 0xFFFFFFFF;
  for (uint32_t i = 0; i < user::kUserSlots; i++) {
    if (user::SigninState(i) != user::kNotSignedIn) {
      mask |= 1u << i;
      count++;
      first = std::min(first, i);
    }
  }
  guest::StoreU32(base, kSignedInMask, mask);
  guest::StoreU32(base, kSignedInCount, count);
  guest::StoreU32(base, kLiveMask, 0);
  guest::StoreU32(base, kFirstUser, first);
  guest::StoreU32(base, kSignInPolled, 1);
  guest::StoreU32(base, kSignInChecked, 1);
  return true;
}

// The signed-in users' names (UTF-16) and XUIDs, in order, after their count.
void WriteUserTable(uint8_t* base) {
  uint8_t count = 0;
  for (uint32_t i = 0; i < user::kUserSlots; i++) {
    for (uint32_t b = 0; b < kNameSlotBytes; b++) {
      guest::StoreU8(base, kUserTable + 14 + kNameSlotBytes * i + b, 0);
    }
  }
  for (uint32_t i = 0; i < user::kUserSlots; i++) {
    if (user::SigninState(i) == user::kNotSignedIn) {
      continue;
    }
    const uint32_t name = kUserTable + 14 + kNameSlotBytes * count;
    for (uint32_t c = 0; c < user::kName.size(); c++) {
      guest::StoreU16(base, name + 2 * c, uint8_t(user::kName[c]));
    }
    const uint32_t xuid = kUserTable + 144 + 8 * count;
    guest::StoreU32(base, xuid, uint32_t(user::kXuid >> 32));
    guest::StoreU32(base, xuid + 4, uint32_t(user::kXuid));
    count++;
  }
  guest::StoreU8(base, kUserTable + 12, count);
}

// After a read, the gamercard region from its results; 0 while they're
// missing (asked again then).
void CopyRegion(uint8_t* base) {
  guest::StoreU32(base, kProfileWasRead, 0);
  if (!guest::LoadU32(base, kSignedInCount)) {
    return;
  }
  const uint32_t results = guest::LoadU32(base, guest::LoadU32(base, kProfileResults) + 4);
  if (!results) {
    guest::StoreU32(base, kProfileRequested, 1);
    return;
  }
  const uint32_t region =
      guest::LoadU32(base, results + 120) ? guest::LoadU32(base, results + 152) : 0;
  for (uint32_t address : kRegionCopies) {
    guest::StoreU32(base, address, region);
  }
}

// The user whose profile is read: player 1's controller's if signed in, else
// the first signed-in user.
uint32_t ProfileUser(uint8_t* base) {
  const uint32_t player = guest::LoadU32(base, kPlayerControllers);
  if (player < 32 && (guest::LoadU32(base, kSignedInMask) >> player & 1)) {
    return player;
  }
  return guest::LoadU32(base, kFirstUser);
}

}  // namespace

extern "C" void NetworkLog_Before(uint8_t* base);
extern "C" void NetworkLog_After(uint8_t* base);

extern "C" REX_FUNC(sub_8214E268) {
  REX_FUNC_PROLOGUE();
  NetworkLog_Before(base);
  if (guest::LoadU32(base, kProfileOverlapped) != 997) {
    if (guest::LoadU32(base, kProfileWasRead)) {
      CopyRegion(base);
    }
    const bool first = PollUsers(base);
    if (first) {
      WriteUserTable(base);
    }
    if (first || guest::LoadU32(base, kProfileRequested) == 1) {
      const uint32_t profile_user = ProfileUser(base);
      InFrame(ctx, base, 512, 0x8214E4A8, [&](uint32_t) {
        ctx.r3.u64 = profile_user;
        sub_82149178(ctx, base);  // the game's profile read
      });
      guest::StoreU32(base, kProfileRequested, 0);
      guest::StoreU32(base, kProfileWasRead, 1);
    }
  }
  NetworkLog_After(base);
}
