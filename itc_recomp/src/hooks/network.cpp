// The network start-up (from the network manager's set-up, sub_8214E190)
// without its kernel calls. System link and Xbox Live are Xbox-only: the
// system link socket isn't made, and the XNet wrappers give the SDK's "not
// available" answers as fixed values. And Network::Update stubbed with the
// data the game reads from it. See B5 in docs/rexglue-removal.md.

#include "guest.h"
#include "lifted/functions.h"
#include "hooks/native_entry.h"
#include "guest_call.h"

#include "schema/system_services.h"
#include "user/network.h"
#include "user/user.h"

#include <algorithm>
#include "lifted/sdk.h"

using guest_call::InFrame;

// WSAStartup: r3 = version, r4 = the WSADATA (out, in the caller's frame,
// never read). Returns 0, as the SDK did.
ITC_NATIVE_ENTRY_ALWAYS(82459B68) {
  
  ctx.r3.u64 = user::kWsaStarted;
}

// Makes the system link socket (UDP, port 1000, non-blocking): not made, so
// the handle is INVALID_SOCKET and system link stays off.
ITC_NATIVE_ENTRY_ALWAYS(82148958) {
  
  user::SystemLinkSocketNotMade(base);
}

// XNetGetTitleXnAddr: r3 = the XNADDR (36 bytes, out). The SDK's: the
// loopback address, no online address or port, CC for each Ethernet address
// byte, no online id.
ITC_NATIVE_ENTRY_ALWAYS(82459D50) {
  
  ctx.r3.u64 = user::XNetGetTitleXnAddr(base, ctx.r3.u32);
}

// XNetXnAddrToMachineId: r3 = the XNADDR, r4 = the id (out, left alone).
// Returns 1 (not signed in to Live), as the SDK did.
ITC_NATIVE_ENTRY_ALWAYS(82459D40) {
  
  ctx.r3.u64 = user::kNoMachineId;
}

// Network::Update (every tick, from sub_82107910) and the sign-in helper it
// ran (SignIn::Update, sub_8214F290, no longer called), stubbed: what the
// game reads from them in play is one signed-in local user (written once)
// and the profile read again when the front end asks. Left out, Xbox-only:
// the Live users' voice registration, XNet, the Live connection and 0x11
// notifications, voice chat, the system link state machine (unreachable: the
// game has no such mode), and the sign-in dialog and "VS MODE" message box
// (the bounds are 0-4 users, so one user never triggers them).
namespace {

using G = schema::SystemServicesGlobals;

constexpr uint32_t kNameSlotBytes = 32;

// The user table (0x826C7670): the users' names and XUIDs, the regions.
schema::NetUserTable UserTable(uint8_t* base) {
  return schema::NetUserTable(base, G::user_table_address());
}

// The sign-in poll: user 0 signed in locally (src/user/user.h), none on Live.
// True the first time (the users' names and XUIDs are written then).
bool PollUsers(uint8_t* base) {
  if (G::users_polled(base)) {
    return false;
  }
  G::set_users_polled(base, 1);
  uint32_t mask = 0, count = 0, first = 0xFFFFFFFF;
  for (uint32_t i = 0; i < user::kUserSlots; i++) {
    if (user::SigninState(i) != user::kNotSignedIn) {
      mask |= 1u << i;
      count++;
      first = std::min(first, i);
    }
  }
  G::set_signed_in_mask(base, mask);
  G::set_signed_in_count(base, count);
  G::set_live_mask(base, 0);
  G::set_first_user(base, first);
  G::set_sign_in_polled(base, 1);
  G::set_sign_in_checked(base, 1);
  return true;
}

// The signed-in users' names (UTF-16) and XUIDs, in order, after their count.
void WriteUserTable(uint8_t* base) {
  const schema::NetUserTable table = UserTable(base);
  uint8_t count = 0;
  for (uint32_t i = 0; i < user::kUserSlots; i++) {
    for (uint32_t b = 0; b < kNameSlotBytes; b++) {
      guest::StoreU8(base, table.names_address() + kNameSlotBytes * i + b, 0);
    }
  }
  for (uint32_t i = 0; i < user::kUserSlots; i++) {
    if (user::SigninState(i) == user::kNotSignedIn) {
      continue;
    }
    const uint32_t name = table.names_address() + kNameSlotBytes * count;
    for (uint32_t c = 0; c < user::kName.size(); c++) {
      guest::StoreU16(base, name + 2 * c, uint8_t(user::kName[c]));
    }
    const uint32_t xuid = table.xuids_address() + 8 * count;
    guest::StoreU32(base, xuid, uint32_t(user::kXuid >> 32));
    guest::StoreU32(base, xuid + 4, uint32_t(user::kXuid));
    count++;
  }
  table.set_count(count);
}

// After a read, the gamercard region from its results; 0 while they're
// missing (asked again then).
void CopyRegion(uint8_t* base) {
  G::set_profile_was_read(base, 0);
  if (!G::signed_in_count(base)) {
    return;
  }
  const schema::SysProfileSettings results = G::profile_results(base).settings();
  if (!results.address) {
    G::set_profile_requested(base, 1);
    return;
  }
  const uint32_t region = results.region_source() ? results.region_value() : 0;
  const schema::NetUserTable table = UserTable(base);
  table.set_region_slot0(region);
  table.set_region_slot1(region);
  table.set_region_slot2(region);
  table.set_region_slot3(region);
  G::set_region_copy_6728(base, region);
  G::set_region_copy_7288(base, region);
}

// The user whose profile is read: player 1's controller's if signed in, else
// the first signed-in user.
uint32_t ProfileUser(uint8_t* base) {
  const uint32_t player = G::player_controller(base);
  if (player < 32 && (G::signed_in_mask(base) >> player & 1)) {
    return player;
  }
  return G::first_user(base);
}

}  // namespace

ITC_NATIVE_ENTRY_ALWAYS(8214E268) {
  
  if (G::profile_overlapped(base) != 997) {
    if (G::profile_was_read(base)) {
      CopyRegion(base);
    }
    const bool first = PollUsers(base);
    if (first) {
      WriteUserTable(base);
    }
    if (first || G::profile_requested(base) == 1) {
      const uint32_t profile_user = ProfileUser(base);
      InFrame(ctx, base, 512, 0x8214E4A8, [&](uint32_t) {
        ctx.r3.u64 = profile_user;
        lifted::Enter(ctx, base, lifted::e82149178);  // the game's profile read
      });
      G::set_profile_requested(base, 0);
      G::set_profile_was_read(base, 1);
    }
  }
}
