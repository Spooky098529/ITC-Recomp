// The movie player's file data source (src/hooks/movie_files.cpp) against the
// recompiled one, which opens through the SDK and reads ahead in 128 KB
// blocks, on the boot movie. Each step as the decoder makes them: Seek to
// the start (+56, recompiled in both), Skip on (+52, recompiled in both),
// Read 2048 bytes; reads at the start, inside a block, across a block's end,
// far ahead and back (outside the recompiled one's blocks: it cancels and
// reads ahead again), at the end and past it. Compared after each step: r3,
// whether +44 holds a handle, +48, +64, +88, the bytes read (r6) and the
// destination. The recompiled one is called by its __imp__ entries; the
// methods it calls through the vtable reach our overrides, which hand a
// handle that isn't ours to the recompiled ones.

#include "harness.h"

#include <cstring>
#include <string>

namespace harness {

void MovieFiles(Context& h) {
  uint8_t* base = h.base;
  const uint32_t source = h.data, read_out = h.data + 160, buffer = h.data + 256,
                 path = h.data + 3072;
  constexpr uint32_t kVtable = 0x8204DD50, kBuffer = 2048, kSize = 8094414;
  const char* name = "game:\\DATA\\TXR\\MOVIE\\UBI_LOGO.wmv";
  struct Step {
    const char* what;
    int kind;  // 0 open, 1 seek to the start and skip, 2 read, 3 seek to the end, 4 close
    uint64_t skip;
  };
  const Step steps[] = {
      {"open", 0, 0},
      {"read at 0", 2, 0},
      {"read on", 2, 0},
      {"skip to 68626", 1, 68626},
      {"read inside the first block", 2, 0},
      {"skip to 130048", 1, 130048},
      {"read across the first block's end", 2, 0},
      {"skip to 1000000", 1, 1000000},
      {"read far ahead", 2, 0},
      {"skip to 300000", 1, 300000},
      {"read back", 2, 0},
      {"skip to the end less 1000", 1, kSize - 1000},
      {"read at the end (short)", 2, 0},
      {"read past the end", 2, 0},
      {"seek to the end", 3, 0},
      {"read after it", 2, 0},
      {"close", 4, 0},
  };
  Results results("the movie data source (sub_82487C48, sub_82487F68, sub_82488598, "
                  "sub_824880D8)");
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    Fill(base, h.data, kScratchBytes);
    Fill(base, source, 136, 0);
    guest::StoreU32(base, source, kVtable);
    guest::StoreU32(base, source + 44, 0xFFFFFFFF);
    guest::StoreU32(base, source + 48, 1);
    Write(base, path, name, uint32_t(std::strlen(name)) + 1);
    for (const Step& s : steps) {
      PPCContext t = CallContext(h);
      t.r3.u64 = source;
      if (s.kind == 0) {
        // As the decoder's open (sub_82487E68): 8 blocks of 128 KB, 1 ahead.
        t.r4.u64 = path;
        t.r6.u64 = 0;
        t.r7.u64 = 0x20000;
        t.r8.u64 = 8;
        t.r9.u64 = 1;
        (v == 0 ? __imp__sub_82487C48 : sub_82487C48)(t, base);
        guest::StoreU32(base, source + 120, 1);
      } else if (s.kind == 1 || s.kind == 3) {
        t.r4.u64 = s.kind == 1 ? 1 : 3;
        sub_82488548(t, base);
        if (s.kind == 1) {
          PPCContext k = CallContext(h);
          k.r3.u64 = source;
          k.r4.u64 = s.skip;
          sub_82488508(k, base);
        }
      } else if (s.kind == 2) {
        Fill(base, buffer, kBuffer);
        t.r4.u64 = buffer;
        t.r5.u64 = kBuffer;
        t.r6.u64 = read_out;
        (v == 0 ? __imp__sub_82488598 : sub_82488598)(t, base);
      } else {
        (v == 0 ? __imp__sub_824880D8 : sub_824880D8)(t, base);
      }
      std::vector<uint8_t> step;
      step.push_back(guest::LoadU32(base, source + 44) == 0xFFFFFFFF ? 0xFF : 0x01);
      for (uint32_t offset : {48u, 64u, 68u, 88u, 92u}) {
        for (int i = 0; i < 4; i++) {
          step.push_back(uint8_t(guest::LoadU32(base, source + offset) >> (24 - 8 * i)));
        }
      }
      if (s.kind == 2) {
        std::vector<uint8_t> out = Read(base, read_out, 4);
        step.insert(step.end(), out.begin(), out.end());
        out = Read(base, buffer, kBuffer);
        step.insert(step.end(), out.begin(), out.end());
      }
      if (s.kind != 1 && s.kind != 3) {
        for (int i = 0; i < 4; i++) {
          step.push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
        }
      }
      result[v].insert(result[v].end(), step.begin(), step.end());
      if (v == 1) {
        // Each step on its own as well, so a difference names its step.
        size_t at = result[1].size() - step.size();
        std::vector<uint8_t> recompiled(result[0].begin() + at,
                                        result[0].begin() + at + step.size());
        results.Check(recompiled, step, std::string(name) + ": " + s.what);
      }
    }
  }
  results.Report();
}

}  // namespace harness
