// The game's CreateFile (src/game_data/create_file.h,
// docs/functions/files-and-saves.md) against the recompiled one. Both call
// the SDK's NtCreateFile for real, so the cases only open and never create,
// truncate or delete: OPEN_EXISTING on files and directories that are there
// or not, CREATE_NEW and OPEN_ALWAYS only on paths that exist (a name
// collision, or an open), the dispositions it refuses before the call
// (0, 6 and later, TRUNCATE_EXISTING without write access), no write access
// and no FILE_FLAG_DELETE_ON_CLOSE. Each handle opened is closed after its
// side. Compared exactly: whether r3 is -1 (handles differ from open to
// open) and its high word, the last error (set to a random value first),
// the scratch area and the 192-byte frame below the stack pointer without
// the handle's word (+96) and the register saves (r26 to r31 and lr, the
// top 56 bytes). The I/O status block's information word (+116) only when
// the open succeeded: on a failure the SDK's NtCreateFile writes an
// uninitialized local there (its file_action), which the original never
// reads. No empty or null paths: the SDK logs those as errors.

#include "harness.h"

#include "hooks/create_file.h"
#include "hooks/last_error.h"

#include <initializer_list>
#include <string>

namespace harness {

namespace {

constexpr uint32_t kPath = 0x100;

uint32_t Pick(Random& random, std::initializer_list<uint32_t> choices) {
  return *(choices.begin() + random.Below(uint32_t(choices.size())));
}

struct Case {
  std::string path;
  uint32_t access, sharing, disposition, flags, error;
};

Case MakeCase(Random& r) {
  static const char* const kExisting[] = {"game:\\default.xex", "game:\\", "game:\\default.xex\\",
                                          "game:"};
  static const char* const kMissing[] = {"game:\\itc_harness_missing.bin",
                                         "game:\\itc_harness_missing\\",
                                         "game:\\itc_harness_missing\\file.bin"};
  Case c;
  c.access = Pick(r, {0x80000000u, 0x80100000u, 0, 1, 0x80000080u});
  c.sharing = r.Below(8);
  // Win32 flags whose NT options are harmless, with some attributes.
  c.flags = 0;
  for (const uint32_t bit : {0x80000000u, 0x40000000u, 0x20000000u, 0x10000000u, 0x08000000u,
                             0x02000000u, 0x80u, 0x01u, 0x02u, 0x04u, 0x20u, 0x100u}) {
    if (r.Below(3) == 0) {
      c.flags |= bit;
    }
  }
  c.error = r.Below(2) == 0 ? 0 : uint32_t(r.Next());
  switch (r.Below(5)) {
    case 0:  // refused before the call
      c.path = kExisting[r.Below(4)];
      c.disposition = Pick(r, {0u, 5u, 6u, 7u, 0xFFFFFFFFu});
      break;
    case 1:  // a path that isn't there, opened
      c.path = kMissing[r.Below(3)];
      c.disposition = 3;
      break;
    case 2:  // CREATE_NEW on a path that's there: a collision
    case 3:  // OPEN_ALWAYS on a path that's there: opened
      // Only the two paths sure to be there (a miss would create).
      c.path = kExisting[r.Below(2)];
      c.disposition = r.Below(2) == 0 ? 1 : 4;
      break;
    default:  // OPEN_EXISTING on a path that's there
      c.path = kExisting[r.Below(4)];
      c.disposition = 3;
  }
  return c;
}

}  // namespace

void CreateFile(Context& h) {
  Results results("sub_82295508 CreateFile");
  const uint32_t pcr = h.ctx.r13.u32;
  const uint32_t saved_error = last_error::Get(h.base, pcr);
  for (int k = 0; k < 120; k++) {
    const Case c = MakeCase(h.random);
    const uint64_t fill = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      Write(h.base, h.data + kPath, c.path.c_str(), uint32_t(c.path.size() + 1));
      PPCContext t = CallContext(h);
      const uint32_t sp = t.r1.u32;
      for (uint32_t i = 0; i < 256; i += 4) {
        guest::StoreU32(h.base, sp - 256 + i, uint32_t(fill >> (i % 32)) + i);
      }
      last_error::Set(h.base, pcr, c.error);
      t.r3.u64 = h.data + kPath;
      t.r4.u64 = c.access;
      t.r5.u64 = c.sharing;
      t.r6.u64 = 0;
      t.r7.u64 = c.disposition;
      t.r8.u64 = c.flags;
      (v == 0 ? __imp__sub_82295508 : hooks::CreateFileSteps)(t, h.base);
      const bool failed = t.r3.u32 == 0xFFFFFFFFu;
      if (!failed) {
        PPCContext close = CallContext(h);
        close.r3.u64 = t.r3.u32;
        __imp__NtClose(close, h.base);
      }
      out[v].Word(t.r1.u32);
      out[v].Word(failed ? 1 : 0);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(last_error::Get(h.base, pcr));
      out[v].Bytes(Read(h.base, h.data, kScratchBytes));
      out[v].Bytes(Read(h.base, sp - 192, 96));
      out[v].Bytes(Read(h.base, sp - 92, 16));
      if (!failed) {
        out[v].Bytes(Read(h.base, sp - 76, 4));
      }
      out[v].Bytes(Read(h.base, sp - 72, 16));
    }
    results.Check(out[0], out[1],
                  "case " + std::to_string(k) + " (" + c.path +
                      ", disposition " + std::to_string(c.disposition) + ")");
  }
  last_error::Set(h.base, pcr, saved_error);
  results.Report();
}

}  // namespace harness
