// The game's file class (src/hooks/file_class.cpp) against the recompiled one,
// which opens through the SDK, on real files. Compared after each step: r3,
// the object (but the handle's value: whether it's open) and the bytes read.
// Two groups:
// - The game's own use (open, read the whole file or its start, seek to 0 and
//   read again, as the loader thread's loose reads do, close): the same.
// - Seeks elsewhere (expected to differ): ours reads at the position set
//   (checked against the files: 1000, 1356, the last 300 and 50 bytes); a
//   ReadFile through the SDK after its SetFilePointer reads elsewhere (bytes
//   from the start, or none after a seek from the end). The game never seeks
//   elsewhere in play (its loose reads seek to 0), so it's noted, not
//   followed (B4 step 2 in docs/rexglue-removal.md).
// Not a case: a missing file (the recompiled one shows the dirty-disc error).

#include "harness.h"

#include <cstring>
#include <iterator>

namespace harness {

void FileClass(Context& h) {
  uint8_t* base = h.base;
  const uint32_t file = h.data, offset = h.data + 16, method = h.data + 20,
                 size = h.data + 24, buffer = h.data + 64, path = h.data + 3072;
  constexpr uint32_t kBuffer = 2048;
  struct Step {
    const char* what;
    int kind;  // 0 open, 1 read, 2 seek, 3 close
    int32_t offset;
    uint32_t method, size;
  };
  const Step own_use[] = {
      {"open", 0, 0, 0, 0},
      {"read 2048", 1, 0, 0, kBuffer},
      {"seek to 0", 2, 0, 0, 0},
      {"read 512", 1, 0, 0, 512},
      {"close", 3, 0, 0, 0},
  };
  const Step elsewhere[] = {
      {"open", 0, 0, 0, 0},
      {"read 512", 1, 0, 0, 512},
      {"seek to 1000", 2, 1000, 0, 0},
      {"read 256", 1, 0, 0, 256},
      {"seek on 100", 2, 100, 1, 0},
      {"read 256 more", 1, 0, 0, 256},
      {"seek to the end less 300", 2, -300, 2, 0},
      {"read 512 at the end", 1, 0, 0, 512},
      {"read past the end", 1, 0, 0, 64},
      {"seek back 50", 2, -50, 1, 0},
      {"read 2048 at the end", 1, 0, 0, kBuffer},
      {"close", 3, 0, 0, 0},
  };
  const char* files[] = {
      "game:\\DATA\\DEBUG\\DEBFONT.XPR",
      "game:\\DATA\\BUILD\\TXR\\BUILD_CRS.TOC",
      "game:\\DATA\\AUDIO\\CAR_SE.XWB",  // car_se.xwb on disk
  };
  auto run = [&](Results& results, const Step* steps, size_t count) {
  for (const char* name : files) {
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      Fill(base, h.data, kScratchBytes);
      Fill(base, file, 16, 0);
      guest::StoreU32(base, file + 4, 0xFFFFFFFF);
      Write(base, path, name, uint32_t(std::strlen(name)) + 1);
      for (size_t i = 0; i < count; i++) {
        const Step& s = steps[i];
        PPCContext t = CallContext(h);
        t.r3.u64 = file;
        if (s.kind == 0) {
          t.r4.u64 = path;
          (v == 0 ? __imp__sub_8258C688 : sub_8258C688)(t, base);
        } else if (s.kind == 1) {
          Fill(base, buffer, kBuffer);
          guest::StoreU32(base, size, s.size);
          t.r4.u64 = buffer;
          t.r5.u64 = size;
          (v == 0 ? __imp__sub_8258C820 : sub_8258C820)(t, base);
        } else if (s.kind == 2) {
          guest::StoreU32(base, offset, uint32_t(s.offset));
          guest::StoreU32(base, method, s.method);
          t.r4.u64 = offset;
          t.r5.u64 = method;
          (v == 0 ? __imp__sub_8258C768 : sub_8258C768)(t, base);
        } else {
          (v == 0 ? __imp__sub_8258C718 : sub_8258C718)(t, base);
        }
        std::vector<uint8_t> step = Read(base, file, 16);
        uint32_t handle = guest::LoadU32(base, file + 4);
        for (int i = 4; i < 8; i++) {
          step[i] = handle == 0xFFFFFFFF ? 0xFF : 0x01;  // open or not
        }
        if (s.kind == 1) {
          for (uint8_t b : Read(base, buffer, kBuffer)) {
            step.push_back(b);
          }
        }
        if (s.kind != 3) {
          for (int i = 0; i < 4; i++) {
            step.push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
          }
        }
        result[v].insert(result[v].end(), step.begin(), step.end());
      }
    }
    results.Check(result[0], result[1], name);
  }
  };
  Results own("the file class (sub_8258C688, sub_8258C820, sub_8258C768, sub_8258C718)");
  run(own, own_use, std::size(own_use));
  own.Report();
  Results other("the file class, seeks elsewhere (expected to differ)");
  run(other, elsewhere, std::size(elsewhere));
  other.Report();
}

}  // namespace harness
