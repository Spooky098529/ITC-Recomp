// The network answers the game gets (B5's step 4, src/hooks/network.cpp,
// and the notifications' listener, src/hooks/notify.cpp) as plain C++, for
// the adapters and native callers alike. System link and Xbox Live are
// Xbox-only: the socket isn't made and the XNet calls give the SDK's "not
// available" answers. See B5 in docs/rexglue-removal.md.

#pragma once

#include "guest.h"

#include <cstdint>

namespace user {

// What the listener wrapper (sub_822952B8, XNotifyCreateListener) answers:
// no listener, but not 0 (see src/hooks/notify.cpp).
constexpr uint32_t kNoListener = 0x4E4F4C53;

// The system link socket's handle (sub_82148958 keeps it here; read only by
// the system link code's sends and receives) and the one it gets.
constexpr uint32_t kSystemLinkSocket = 0x826C6AA0;
constexpr uint32_t kInvalidSocket = 0xFFFFFFFF;

// XNetGetTitleXnAddr's answer: XNET_GET_XNADDR_STATIC.
constexpr uint32_t kXnAddrStatic = 4;
// XNetXnAddrToMachineId's: 1 (not signed in to Live), nothing written.
constexpr uint32_t kNoMachineId = 1;
// WSAStartup's: 0, the WSADATA left alone.
constexpr uint32_t kWsaStarted = 0;

// sub_82148958: the system link socket not made.
inline void SystemLinkSocketNotMade(uint8_t* base) {
  guest::StoreU32(base, kSystemLinkSocket, kInvalidSocket);
}

// XNetGetTitleXnAddr (sub_82459D50) into the 36-byte XNADDR at `address`:
// the loopback address, no online address or port, CC for each Ethernet
// address byte, no online id. Returns kXnAddrStatic.
inline uint32_t XNetGetTitleXnAddr(uint8_t* base, uint32_t address) {
  guest::StoreU32(base, address + 0, 0x7F000001);  // ina
  guest::StoreU32(base, address + 4, 0);           // inaOnline
  guest::StoreU16(base, address + 8, 0);           // wPortOnline
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU8(base, address + 10 + i, 0xCC);  // abEnet
  }
  for (uint32_t i = 0; i < 20; i++) {
    guest::StoreU8(base, address + 16 + i, 0);  // abOnline
  }
  return kXnAddrStatic;
}

}  // namespace user
