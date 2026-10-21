// The network start-up (from the network manager's set-up, sub_8214E190)
// without its kernel calls. System link and Xbox Live are Xbox-only: the
// system link socket isn't made, and the XNet wrappers give the SDK's "not
// available" answers as fixed values. See B5 in docs/rexglue-removal.md.

#include "guest.h"

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
