// The network manager's set-up natively (start_up.h,
// docs/functions/system-services.md): the sign-in helper's and the network
// start-up's words, the XNet answers B5 plugged in (user/network.h) and the
// voice set-up without the engine (hooks/voice.h).

#include "app_start_up/start_up.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"
#include "hooks/voice.h"
#include "user/network.h"

namespace app_start_up {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Half(uint8_t* base, uint32_t address) { return guest::LoadU16(base, address); }
void Clear(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, address + i, 0);
  }
}

constexpr uint32_t kXNetFailed = 0x80004005;  // E_FAIL
constexpr uint32_t kXNetStartupFailed = 1627;  // ERROR_FUNCTION_FAILED

}  // namespace

uint32_t SignInSetUp(uint8_t* base, uint32_t fewest, uint32_t most, uint32_t word7948,
                     uint32_t word8B80, uint32_t app) {
  ITC_NATIVE_RUNNING("sub_8214F1C0 SignIn::SetUp");
  Store(base, 0x826C7944, fewest);
  Store(base, 0x82618B7C, most);
  Store(base, 0x826C7948, word7948);
  Store(base, 0x82618B80, word8B80);
  if (app != 0) {
    Store(base, 0x826C799C, app);
  }
  if (Load(base, 0x826C794C) != 0) {
    return fewest;
  }
  Store(base, 0x826C7994, 0);
  Store(base, 0x826C7964, 0);
  Store(base, 0x826C794C, user::kNoListener);  // XNotifyCreateListener(3)
  return user::kNoListener;
}

uint32_t XNetConfigure(uint8_t* base, uint32_t config, uint32_t source,
                       const MemcpyCall& memcpy_call) {
  ITC_NATIVE_RUNNING("sub_82577560 XNet::Configure");
  if (!c_runtime::Memcpy(base, config, source, 44)) {
    memcpy_call(config, source, 44);
  }
  if (Load(base, config + 8) & 1) {
    return kXNetFailed;
  }
  uint32_t next = source + 44;
  const uint32_t size38 = Half(base, config + 38);
  if (size38 != 0) {
    Store(base, config + 60, next);
    next += size38;
  }
  const uint32_t count40 = Half(base, config + 40);
  const uint32_t size42 = Half(base, config + 42);
  const uint32_t product = size42 * count40;
  const uint32_t words40 = count40 << 2;
  Store(base, config + 44, next);
  const uint32_t at48 = next + words40;
  Store(base, config + 48, at48);
  const uint32_t at52 = at48 + product;
  const uint32_t at56 = Load(base, config + 20) - words40 - size38 - product - 44;
  Store(base, config + 56, at56);
  Store(base, config + 52, at52);
  const uint32_t at64 = at56 + at52;
  Store(base, config + 64, at64);
  const uint32_t at68 = Half(base, config + 26) * Half(base, config + 24) + at64;
  Store(base, config + 68, at68);
  Store(base, config + 72, (Half(base, config + 32) << 1) + at68);
  return 0;
}

uint32_t XNetStartup(uint8_t* base, const MemcpyCall& memcpy_call) {
  ITC_NATIVE_RUNNING("sub_82575B08 XNetStartup");
  // WSAStartup(2, the WSADATA in its frame): ours answers 0 and writes
  // nothing.
  const uint32_t result = XNetConfigure(base, kXNetConfig, Load(base, kXNetConfigSource),
                                        memcpy_call);
  return int32_t(result) >= 0 ? 0 : kXNetStartupFailed;
}

uint32_t NetworkStartUp(uint8_t* base, uint32_t app, const MemcpyCall& memcpy_call) {
  ITC_NATIVE_RUNNING("sub_82148868 Network::StartUp");
  XNetStartup(base, memcpy_call);
  Store(base, 0x826C6AB4, user::kNoListener);  // XNotifyCreateListener(2)
  Store(base, 0x826C6AB8, user::kNoListener);  // XNotifyCreateListener(1)
  Store(base, 0x826C6AB0, 0);
  Store(base, 0x826C6ABC, 0xFFFFFFFF);
  Clear(base, 0x826C6650, 60);
  Clear(base, 0x826C6080, 28);
  Store(base, 0x826C72FC, 0);
  Store(base, 0x826C7300, app);
  Clear(base, kUserTable, 384);
  return kUserTable;
}

uint32_t NetworkSetUp(uint8_t* base, uint32_t app, uint32_t frame, const MemcpyCall& memcpy_call) {
  ITC_NATIVE_RUNNING("sub_8214E190 Network::SetUp");
  SignInSetUp(base, 0, 4, 0, 1, app);
  NetworkStartUp(base, app, memcpy_call);
  user::SystemLinkSocketNotMade(base);
  user::XNetGetTitleXnAddr(base, frame + 80);  // answers at once: no loop
  // XNetXnAddrToMachineId(frame + 80, the user table): not signed in to
  // Live, nothing written.
  const uint32_t ticks = voice::SetUpWithoutEngine(base, kNetworkManager, kUserTable);
  Store(base, kSessionState, 0);
  Store(base, 0x826C6AD4, 0);
  Store(base, 0x826C7938, 0);
  Store(base, 0x826C793C, 0);
  Store(base, 0x826C7940, 0);
  return ticks;
}

}  // namespace app_start_up
