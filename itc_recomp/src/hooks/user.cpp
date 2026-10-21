// The game's XDK user wrappers, without their kernel calls: adapters in front
// of src/user/user.h. And XUserSetContextEx (presence, Xbox-only) dropped.
// See B5 in docs/rexglue-removal.md.

#include "guest.h"
#include "last_error.h"

#include "user/user.h"

// XUserGetName: r3 = user, r4 = buffer, r5 = its size; returns an HRESULT.
extern "C" REX_FUNC(sub_82295020) {
  REX_FUNC_PROLOGUE();
  char name[user::kNameBytes];
  uint32_t bytes;
  const uint32_t result = user::Name(ctx.r3.u32, ctx.r5.u32, name, &bytes);
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, ctx.r4.u32 + i, uint8_t(name[i]));
  }
  ctx.r3.u64 = result;
}

// XUserGetSigninState: r3 = user.
extern "C" REX_FUNC(sub_82295028) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = user::SigninState(ctx.r3.u32);
}

// XUserGetXUID: r3 = user, r4 = the XUID (out); returns a Win32 error.
extern "C" REX_FUNC(sub_82295268) {
  REX_FUNC_PROLOGUE();
  const uint32_t out = ctx.r4.u32;
  if (!out) {
    ctx.r3.u64 = user::kErrorInvalidParameter;
    return;
  }
  uint64_t xuid;
  const uint32_t result = user::Xuid(ctx.r3.u32, &xuid);
  guest::StoreU32(base, out, uint32_t(xuid >> 32));
  guest::StoreU32(base, out + 4, uint32_t(xuid));
  ctx.r3.u64 = result;
}

// The title id check: r3 = a title id; returns 0 or ERROR_FUNCTION_FAILED.
extern "C" REX_FUNC(sub_82296EE0) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = user::CheckTitleId(ctx.r3.u32);
}

// XUserSetContextEx without an overlapped (every call; sub_82294ED8 with one
// is no longer called): the user's presence context, which nothing reads
// back. The SDK answered 0 and cleared the last error, which the wrapper
// then read; the same here.
extern "C" REX_FUNC(sub_82294F70) {
  REX_FUNC_PROLOGUE();
  last_error::Set(base, ctx.r13.u32, 0);
  ctx.r3.u64 = 0;
}
