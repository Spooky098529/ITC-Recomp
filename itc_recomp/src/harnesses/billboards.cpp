// The light glows' billboard functions (src/hooks/billboards.cpp and the
// vertex builder's adapter in src/interpolation/blend_state.cpp) against the
// recompiled ones.

#include "harness.h"

#include "base/log.h"

#include <algorithm>
#include <bit>
#include <cmath>

namespace harness {

namespace {

// The vertex ring and its globals (the vertex builder's outputs outside the
// scratch area), saved before the cases and put back after.
constexpr uint32_t kRingCount = 0x826A509C;
constexpr uint32_t kRingHandle = 0x826A50A8;
constexpr uint32_t kRingIndex = 0x826A5098;
constexpr uint32_t kRingVertices = 0x8266D098;
constexpr uint32_t kRingBytes = 2 * 4096 * 28;

// The scratch area's layout for a vertex builder case.
constexpr uint32_t kList = 0;        // the command list (48 bytes)
constexpr uint32_t kSlots = 256;     // its slots (16 bytes each)
constexpr uint32_t kPass = 512;      // the camera pass (552 bytes)
constexpr uint32_t kMaterial = 1100; // the material (40 bytes)
constexpr uint32_t kSet = 1144;      // the billboard set (its +0, the pass)
constexpr uint32_t kLights = 1200;   // the lights (48 bytes each)
constexpr uint32_t kMostLights = 40;

float Uniform(Random& r, float low, float high) {
  return low + (high - low) * float(r.Below(1u << 24)) / float(1u << 24);
}

uint32_t Bits(float f) {
  return std::bit_cast<uint32_t>(f);
}

}  // namespace

void Billboards(Context& h) {
  uint8_t* base = h.base;
  const uint32_t data = h.data;
  Random& r = h.random;

  // The material's set-up: over 0xCD and random fills.
  {
    Results results("BillboardMaterial::Init (sub_8209F5A0)");
    for (int i = 0; i < 20; i++) {
      uint32_t fill[16];
      for (uint32_t& word : fill) {
        word = uint32_t(r.Next());
      }
      Both(h, results,
           [&] {
             if (i % 2) {
               for (uint32_t k = 0; k < 16; k++) {
                 guest::StoreU32(base, data + 4 * k, fill[k]);
               }
             }
           },
           [&](PPCContext& t) { t.r3.u64 = data; }, __imp__sub_8209F5A0, sub_8209F5A0, data, 64,
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("BillboardMaterial::SetLights (sub_8209F5C8)");
    for (int i = 0; i < 40; i++) {
      uint32_t count = i < 10 ? 0 : (r.Below(3) ? r.Below(64) : uint32_t(r.Next()));
      uint32_t lights = uint32_t(r.Next());
      Both(h, results, [] {},
           [&](PPCContext& t) {
             t.r3.u64 = data;
             t.r4.u64 = count;
             t.r5.u64 = lights;
           },
           __imp__sub_8209F5C8, sub_8209F5C8, data, 64,
           fmt::format("count {} lights {:08X}", count, lights));
    }
    results.Report();
  }

  // The vertex builder: a camera pass (eye, look-at, a view matrix, a draw
  // distance below and above the cap), lights around it (in front, behind,
  // too far, without a size, odd colors: negative, above 255, NaN), every
  // alpha mode the game tests and some it doesn't, ring counts near its room
  // and denormals now and then. Compared: the scratch area, the ring, its
  // count and r3.
  {
    Results results("Billboards::BuildVertices (sub_8209C6E0)");
    const uint32_t saved_count = guest::LoadU32(base, kRingCount);
    const uint32_t saved_index = guest::LoadU32(base, kRingIndex);
    const uint32_t saved_handle = guest::LoadU32(base, kRingHandle);
    const std::vector<uint8_t> saved_ring = Read(base, kRingVertices, kRingBytes);
    const int32_t modes[] = {1, 2, 3, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, -64, 0};
    const uint32_t odd[] = {0x7FC00000, 0xFF800000, 0x7F800000, 0x00000001, 0x80000000,
                            0x00400000, 0x4F000000, 0xCF000000, 0x5F800000};
    for (int i = 0; i < 400; i++) {
      // The inputs, made once for both versions.
      std::vector<uint32_t> pass(552 / 4);
      for (uint32_t& word : pass) {
        word = Bits(Uniform(r, -1, 1));
      }
      const float ex = Uniform(r, -8000, 8000), ey = Uniform(r, -50, 50), ez = Uniform(r, -8000, 8000);
      pass[400 / 4] = Bits(ex);
      pass[404 / 4] = Bits(ey);
      pass[408 / 4] = Bits(ez);
      pass[416 / 4] = Bits(ex + Uniform(r, -10, 10));
      pass[420 / 4] = Bits(ey + Uniform(r, -2, 2));
      pass[424 / 4] = Bits(ez + Uniform(r, -10, 10));
      if (r.Below(20) == 0) {  // looking straight up or down: no direction
        pass[416 / 4] = pass[400 / 4];
        pass[424 / 4] = pass[408 / 4];
      }
      pass[548 / 4] = Bits(r.Below(3) ? Uniform(r, 0, 600) : Uniform(r, 600, 100000));
      const uint32_t lights = r.Below(8) == 0 ? 0 : 1 + r.Below(kMostLights);
      std::vector<uint32_t> light(lights * 12);
      for (uint32_t k = 0; k < lights; k++) {
        uint32_t* l = &light[k * 12];
        const float reach = r.Below(4) == 0 ? 2000 : 300;
        l[0] = Bits(ex + Uniform(r, -reach, reach));
        l[1] = Bits(ey + Uniform(r, -20, 20));
        l[2] = Bits(ez + Uniform(r, -reach, reach));
        l[3] = uint32_t(r.Next());
        for (int c = 4; c < 8; c++) {
          l[c] = Bits(r.Below(10) == 0 ? Uniform(r, -300, 600) : Uniform(r, 0, 255));
          if (r.Below(40) == 0) {
            l[c] = odd[r.Below(9)];
          }
        }
        l[8] = Bits(r.Below(6) == 0 ? (r.Below(2) ? 0.0f : -1.0f) : Uniform(r, 0, 5));
        l[9] = Bits(r.Below(6) == 0 ? (r.Below(2) ? 0.0f : -1.0f) : Uniform(r, 0, 5));
        if (r.Below(30) == 0) {
          l[r.Below(3)] = odd[r.Below(9)];
        }
        if (r.Below(30) == 0) {  // a light at the eye: no offset
          l[0] = pass[400 / 4];
          l[2] = pass[408 / 4];
        }
        l[10] = uint32_t(r.Next());
        l[11] = uint32_t(r.Next());
      }
      if (r.Below(30) == 0) {  // a denormal in the eye's offset
        pass[404 / 4] = 0x00000003;
        if (lights) {
          light[1] = 0x80000005;
        }
      }
      const int32_t mode = modes[r.Below(16)];
      const uint32_t index = r.Below(4) == 0 ? 1 : 0;
      uint32_t count = r.Below(4) == 0 ? 4090 + r.Below(8) : r.Below(4000);
      if (index == 1 && count + 4 * lights > 4096) {
        count = 4096 - 4 * lights;
      }
      const uint32_t handle = uint32_t(r.Next());

      // Where the vertices go, and a margin on each side inside the ring (the
      // globals after it are the game's).
      const uint32_t start = kRingVertices + (index * 4096 + count) * 28;
      const uint32_t window = std::max(kRingVertices, start - 64);
      const uint32_t kWindowBytes =
          std::min(kRingVertices + kRingBytes, start + kMostLights * 4 * 28 + 64) - window;
      std::vector<uint8_t> result[2];
      for (int v = 0; v < 2; v++) {
        Fill(base, data, kScratchBytes);
        guest::StoreU32(base, kRingCount, count);
        guest::StoreU32(base, kRingIndex, index);
        guest::StoreU32(base, kRingHandle, handle);
        Fill(base, window, kWindowBytes, 0xCD);
        guest::StoreU32(base, data + kList + 8, 1000);  // room for every slot
        guest::StoreU32(base, data + kList + 12, 0);
        guest::StoreU32(base, data + kList + 44, data + kSlots);
        for (uint32_t k = 0; k < pass.size(); k++) {
          guest::StoreU32(base, data + kPass + 4 * k, pass[k]);
        }
        guest::StoreU32(base, data + kSet, data + kPass);
        guest::StoreU32(base, data + kMaterial + 0, uint32_t(mode));
        guest::StoreU32(base, data + kMaterial + 32, lights);
        guest::StoreU32(base, data + kMaterial + 36, data + kLights);
        for (uint32_t k = 0; k < light.size(); k++) {
          guest::StoreU32(base, data + kLights + 4 * k, light[k]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = data + kSet;
        t.r4.u64 = data + kList;
        t.r5.u64 = data + kMaterial;
        (v == 0 ? __imp__sub_8209C6E0 : sub_8209C6E0)(t, base);
        result[v] = Read(base, data, kScratchBytes);
        const std::vector<uint8_t> ring = Read(base, window, kWindowBytes);
        result[v].insert(result[v].end(), ring.begin(), ring.end());
        for (uint32_t word : {guest::LoadU32(base, kRingCount), t.r3.u32}) {
          for (int b = 0; b < 4; b++) {
            result[v].push_back(uint8_t(word >> (24 - 8 * b)));
          }
        }
      }
      // Only the 32 bytes from the first difference are logged (the outputs
      // are ~9 KB).
      size_t at = 0;
      while (at < result[0].size() && result[0][at] == result[1][at]) {
        at++;
      }
      const size_t from = at < result[0].size() ? at : 0;
      const size_t to = std::min(result[0].size(), from + 32);
      results.Check({result[0].begin() + from, result[0].begin() + to},
                    {result[1].begin() + from, result[1].begin() + to},
                    fmt::format("case {}: mode {} lights {} ring {} at {}, from byte {}", i, mode,
                                lights, index, count, from));
    }
    guest::StoreU32(base, kRingCount, saved_count);
    guest::StoreU32(base, kRingIndex, saved_index);
    guest::StoreU32(base, kRingHandle, saved_handle);
    Write(base, kRingVertices, saved_ring.data(), kRingBytes);
    results.Report();
  }
}

}  // namespace harness
