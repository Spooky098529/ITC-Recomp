// The profile read (src/hooks/profile.cpp) against the recompiled one, which
// asks the SDK, on the game's two reads with its 26 ids (82008EA0): the size
// query, then the settings with an overlapped (and without one, and for
// another user). Compared: the buffer, the size, the overlapped (but its
// context, the thread handle the SDK writes and nothing reads) and r3.

#include "harness.h"

#include <rex/logging.h>

namespace harness {

void Profile(Context& h) {
  uint8_t* base = h.base;
  const uint32_t data = h.data;
  // The buffer (4,192 bytes) doesn't fit the scratch area: below it, past the
  // calls' stack (which starts 1 KB under it).
  const uint32_t buffer = data - 16384, size = data, overlapped = data + 16;
  const uint32_t ids = 0x82008EA0;
  struct Case {
    const char* what;
    uint32_t user, size, buffer, overlapped;
  };
  const Case cases[] = {
      {"the size query", 0, 0, 0, 0},
      {"the settings, overlapped", 0, 4192, buffer, overlapped},
      {"the settings, no overlapped", 0, 4192, buffer, 0},
      {"user 1, overlapped", 1, 4192, buffer, overlapped},
      {"a short buffer", 0, 100, buffer, overlapped},
  };
  Results results("XUserReadProfileSettings (sub_82576E78)");
  for (const Case& c : cases) {
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      Fill(base, data, kScratchBytes);
      Fill(base, buffer, 4192, 0);
      guest::StoreU32(base, size, c.size);
      Fill(base, overlapped, 28, 0);
      PPCContext t = CallContext(h);
      t.r3.u64 = 0;
      t.r4.u64 = c.user;
      t.r5.u64 = 26;
      t.r6.u64 = ids;
      t.r7.u64 = size;
      t.r8.u64 = c.buffer;
      t.r9.u64 = c.overlapped;
      (v == 0 ? __imp__sub_82576E78 : sub_82576E78)(t, base);
      guest::StoreU32(base, overlapped + 8, 0);  // the SDK's thread handle
      result[v] = Read(base, buffer, 4192);
      for (uint8_t b : Read(base, size, 4 + 12 + 28)) {
        result[v].push_back(b);
      }
      for (int i = 0; i < 4; i++) {
        result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
      }
    }
    results.Check(result[0], result[1], c.what);
  }
  results.Report();
}

}  // namespace harness
