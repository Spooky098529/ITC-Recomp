// The command list's functions (src/hooks/command_list.cpp) against the
// recompiled ones. Each case runs on a made-up list in the scratch area (its
// buffers there too), never the game's; the open chunk count (a guest
// global the game's list shares) is set per case and put back at the end.

#include "harness.h"

#include "base/log.h"
#include "command_list/command_list.h"

#include <array>
#include <functional>

namespace harness {

namespace {

// One case through both versions: the scratch area filled with 0xCD,
// `setup(list)` writes the list's fields (and the open count), `args(t)`
// sets the registers past r3; the whole scratch area, the open count and r3
// compared.
void Case(Context& h, Results& results, GuestFunction recompiled, GuestFunction native,
          const std::function<void(uint32_t)>& setup, const std::function<void(PPCContext&)>& args,
          const std::string& what) {
  const uint32_t list = h.data;
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, h.data, kScratchBytes);
    guest::StoreU32(h.base, command_list::kOpenChunks, 0);
    setup(list);
    PPCContext t = CallContext(h);
    t.r3.u64 = list;
    args(t);
    (v == 0 ? recompiled : native)(t, h.base);
    result[v] = Read(h.base, h.data, kScratchBytes);
    std::vector<uint8_t> open = Read(h.base, command_list::kOpenChunks, 4);
    result[v].insert(result[v].end(), open.begin(), open.end());
    for (int i = 0; i < 8; i++) {
      result[v].push_back(uint8_t(t.r3.u64 >> (56 - 8 * i)));
    }
  }
  results.Check(result[0], result[1], what);
}

// A list's fields from random words (the open flag's byte too): drawn once
// a case, the same for both versions.
struct Fields {
  uint32_t words[13];
  explicit Fields(Context& h) {
    for (uint32_t& word : words) {
      word = uint32_t(h.random.Next());
    }
  }
  void Write(Context& h, uint32_t list) const {
    for (uint32_t i = 0; i < 13; i++) {
      guest::StoreU32(h.base, list + 4 + 4 * i, words[i]);
    }
  }
};

}  // namespace

void CommandList(Context& h) {
  const uint32_t saved_open = guest::LoadU32(h.base, command_list::kOpenChunks);
  auto no_args = [](PPCContext&) {};

  {
    Results results("CommandList::CommandList (sub_8258C960)");
    for (int i = 0; i < 20; i++) {
      const Fields fields(h);
      Case(h, results, __imp__sub_8258C960, sub_8258C960,
           [&](uint32_t list) { fields.Write(h, list); }, no_args, fmt::format("random {}", i));
    }
    results.Report();
  }

  {
    Results results("CommandList::Flip (sub_8258C9C0)");
    std::vector<uint32_t> buffers = {0, 1, 2, 0xFFFFFFFF, 0xFFFFFFFE};
    for (int i = 0; i < 20; i++) {
      buffers.push_back(uint32_t(h.random.Next()));
    }
    for (uint32_t buffer : buffers) {
      const Fields fields(h);
      Case(h, results, __imp__sub_8258C9C0, sub_8258C9C0,
           [&](uint32_t list) {
             fields.Write(h, list);
             guest::StoreU32(h.base, list + 4, buffer);
           },
           no_args, fmt::format("buffer {:X}", buffer));
    }
    results.Report();
  }

  {
    // +4 is 0 or 1 in play; 2-4 read +32 to +40, inside the list.
    Results results("CommandList::BeginTick (sub_8258C998)");
    for (uint32_t buffer = 0; buffer < 5; buffer++) {
      for (int i = 0; i < 10; i++) {
        const Fields fields(h);
        Case(h, results, __imp__sub_8258C998, sub_8258C998,
             [&](uint32_t list) {
               fields.Write(h, list);
               guest::StoreU32(h.base, list + 4, buffer);
             },
             no_args, fmt::format("buffer {} random {}", buffer, i));
      }
    }
    results.Report();
  }

  {
    // Open or not (any nonzero byte is open), the open count 0 (play) or
    // not (the nested message, left to the original).
    Results results("CommandList::OpenChunk (sub_8258C9D0)");
    const uint8_t flags[] = {0, 1, 0x80, 0xFF};
    const uint32_t counts[] = {0, 1, 2, 0xFFFFFFFF};
    for (uint8_t flag : flags) {
      for (uint32_t count : counts) {
        for (int i = 0; i < 5; i++) {
          const Fields fields(h);
          Case(h, results, __imp__sub_8258C9D0, sub_8258C9D0,
               [&](uint32_t list) {
                 fields.Write(h, list);
                 guest::StoreU8(h.base, list + 52, flag);
                 guest::StoreU32(h.base, command_list::kOpenChunks, count);
               },
               no_args, fmt::format("open {:02X} count {:X} random {}", flag, count, i));
        }
      }
    }
    results.Report();
  }

  {
    Results results("CommandList::CloseChunk (sub_8258CA50)");
    const uint8_t flags[] = {0, 1, 0x80, 0xFF};
    const uint32_t counts[] = {0, 1, 2, 0xFFFFFFFF};
    for (uint8_t flag : flags) {
      for (uint32_t count : counts) {
        for (int i = 0; i < 5; i++) {
          const Fields fields(h);
          Case(h, results, __imp__sub_8258CA50, sub_8258CA50,
               [&](uint32_t list) {
                 fields.Write(h, list);
                 guest::StoreU8(h.base, list + 52, flag);
                 guest::StoreU32(h.base, command_list::kOpenChunks, count);
               },
               no_args, fmt::format("open {:02X} count {:X} random {}", flag, count, i));
        }
      }
    }
    results.Report();
  }

  {
    // Chunks of whole slots, odd sizes and negative ones (end before start).
    Results results("CommandList::ChunkSlots (sub_8258CA88)");
    for (int i = 0; i < 60; i++) {
      const uint32_t start = uint32_t(h.random.Next());
      uint32_t end = start + 16 * h.random.Below(1000);
      if (i % 3 == 1) {
        end = start + h.random.Below(1000);
      } else if (i % 3 == 2) {
        end = uint32_t(h.random.Next());
      }
      const Fields fields(h);
      Case(h, results, __imp__sub_8258CA88, sub_8258CA88,
           [&](uint32_t list) {
             fields.Write(h, list);
             guest::StoreU32(h.base, list + 36, start);
             guest::StoreU32(h.base, list + 40, end);
           },
           no_args, fmt::format("start {:X} end {:X}", start, end));
    }
    results.Report();
  }

  // The allocators: a list with its write pointer inside the scratch area
  // (the header is the only slot written), capacities and counts that fit,
  // that just fit, and that don't (the original's message, then 0); the
  // header byte set or not (r5 = 0x100 has none), but never a header on a
  // full list (the original then writes it at guest address 0).
  struct Allocator {
    const char* name;
    GuestFunction recompiled, native;
    int kind;  // 0 vertices, 1 bytes, 2 resolve
  };
  const Allocator allocators[] = {
      {"CommandList::AllocateVertices (sub_8258CB08)", __imp__sub_8258CB08, sub_8258CB08, 0},
      {"CommandList::AllocateBytes (sub_8258CB98)", __imp__sub_8258CB98, sub_8258CB98, 1},
      {"CommandList::AllocateResolve (sub_8258CC30)", __imp__sub_8258CC30, sub_8258CC30, 2},
  };
  for (const Allocator& a : allocators) {
    Results results(a.name);
    for (int i = 0; i < 300; i++) {
      const uint32_t limit = h.random.Below(300);
      uint32_t used = h.random.Below(320);
      if (i < 4) {
        used = 0xFFFFFFFF - i;  // the used count wraps
      }
      const uint32_t header_byte =
          std::array<uint32_t, 4>{0, 1, 0x100, 0x1FF}[h.random.Below(4)];
      if ((header_byte & 0xFF) != 0 && used + 1 >= limit) {
        continue;  // a full header: written at guest address 0
      }
      uint32_t size;
      switch (h.random.Below(4)) {
        case 0:
          size = h.random.Below(64);
          break;
        case 1:  // just fits or just doesn't: (limit - used) slots' worth, around
          size = (a.kind == 0 ? (limit - used) * 16 / 24 : (limit - used) * 16) +
                 h.random.Below(40) - 20;
          break;
        case 2:
          size = h.random.Below(4096);
          break;
        default:  // wraps in the size's arithmetic
          size = uint32_t(h.random.Next());
          break;
      }
      const uint32_t write = h.data + 256 + 16 * h.random.Below(200);
      const Fields fields(h);
      Case(h, results, a.recompiled, a.native,
           [&](uint32_t list) {
             fields.Write(h, list);
             guest::StoreU32(h.base, list + 8, limit);
             guest::StoreU32(h.base, list + 12, used);
             guest::StoreU32(h.base, list + 44, write);
           },
           [&](PPCContext& t) {
             if (a.kind == 2) {
               t.r4.u64 = header_byte;
             } else {
               t.r4.u64 = size;
               t.r5.u64 = header_byte;
             }
           },
           fmt::format("limit {} used {:X} size {:X} header {:X}", limit, used, size,
                       header_byte));
    }
    results.Report();
  }

  guest::StoreU32(h.base, command_list::kOpenChunks, saved_open);
}

}  // namespace harness
