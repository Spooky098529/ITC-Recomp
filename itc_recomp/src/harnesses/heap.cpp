// The native heap's adapters (src/hooks/heap.cpp) against the game's own
// allocators. The two hand out different addresses, so each case compares
// what can be seen of a block without its address: that one came back, its
// alignment, HeapSize, the contents kept or cleared, and the frees' results.
// The recompiled side runs under hooks::RecompiledHeapScope, which makes the
// adapters call the game's functions (HeapReAlloc's own calls to HeapAlloc
// and HeapFree included); each side frees what it allocated.

#include "harness.h"

#include "hooks/heap.h"

#include <algorithm>
#include <initializer_list>
#include <optional>
#include <string>
#include <vector>

namespace harness {

namespace {

constexpr uint32_t kProcessHeapHandle = 0x8297149C;

// Calls `f` with r3, r4, ... set from `args`; returns r3.
uint32_t Call(Context& h, GuestFunction f, std::initializer_list<uint32_t> args) {
  PPCContext t = CallContext(h);
  PPCRegister* registers[] = {&t.r3, &t.r4, &t.r5, &t.r6, &t.r7};
  int i = 0;
  for (uint32_t a : args) {
    registers[i++]->u64 = a;
  }
  f(t, h.base);
  return t.r3.u32;
}

void Append(std::vector<uint8_t>& out, uint32_t value) {
  for (int i = 0; i < 4; i++) {
    out.push_back(uint8_t(value >> (24 - 8 * i)));
  }
}

bool AllZero(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    if (guest::LoadU8(base, address + i) != 0) {
      return false;
    }
  }
  return true;
}

uint8_t Pattern(uint32_t i) {
  return uint8_t(i * 7 + 1);
}

void WritePattern(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, address + i, Pattern(i));
  }
}

bool HasPattern(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    if (guest::LoadU8(base, address + i) != Pattern(i)) {
      return false;
    }
  }
  return true;
}

// Runs `body` once on each side and compares what it appended.
template <typename Body>
void Case(Results& results, const std::string& what, Body body) {
  std::vector<uint8_t> result[2];
  {
    hooks::RecompiledHeapScope recompiled;
    body(result[0]);
  }
  body(result[1]);
  results.Check(result[0], result[1], what);
}

void HeapCases(Context& h) {
  Results results("heap: HeapAlloc, HeapSize, HeapFree");
  const uint32_t heap = guest::LoadU32(h.base, kProcessHeapHandle);
  for (uint32_t size : {0u, 1u, 7u, 8u, 13u, 24u, 100u, 1000u, 4096u, 70000u}) {
    for (uint32_t flags : {0u, 8u}) {
      Case(results, fmt::format("size {} flags {:#x}", size, flags), [&](std::vector<uint8_t>& out) {
        const uint32_t p = Call(h, sub_82299D60, {heap, flags, size});
        Append(out, p != 0);
        if (p == 0) {
          return;
        }
        Append(out, p % 16);
        Append(out, Call(h, sub_82298FF8, {heap, 0, p}));
        if (flags & 8) {
          Append(out, AllZero(h.base, p, size));
        }
        WritePattern(h.base, p, size);
        Append(out, HasPattern(h.base, p, size));
        Append(out, Call(h, sub_8229A648, {heap, 0, p}) != 0);
      });
    }
  }
  results.Report();
}

void ReallocCases(Context& h) {
  Results results("heap: HeapReAlloc");
  const uint32_t heap = guest::LoadU32(h.base, kProcessHeapHandle);
  const std::pair<uint32_t, uint32_t> sizes[] = {
      {16, 8}, {16, 16}, {16, 0}, {16, 24}, {16, 4000}, {100, 100}, {4000, 16}, {24, 70000},
      {70000, 200000}, {1, 2},
  };
  for (auto [from, to] : sizes) {
    for (uint32_t flags : {0u, 8u}) {
      Case(results, fmt::format("{} to {} flags {:#x}", from, to, flags),
           [&](std::vector<uint8_t>& out) {
             const uint32_t p = Call(h, sub_82299D60, {heap, 0, from});
             WritePattern(h.base, p, from);
             // Something after the block, so growing may have to move it.
             const uint32_t after = Call(h, sub_82299D60, {heap, 0, 8});
             const uint32_t q = Call(h, sub_8229A930, {heap, flags, p, to});
             Append(out, q != 0);
             if (q != 0) {
               const uint32_t size = Call(h, sub_82298FF8, {heap, 0, q});
               Append(out, size);
               Append(out, q % 16);
               Append(out, HasPattern(h.base, q, std::min(from, size)));
               if ((flags & 8) && size > from) {
                 Append(out, AllZero(h.base, q + from, size - from));
               }
               Append(out, Call(h, sub_8229A648, {heap, 0, q}) != 0);
             }
             Append(out, Call(h, sub_8229A648, {heap, 0, after}) != 0);
           });
    }
  }
  // A null block.
  Case(results, "a null block", [&](std::vector<uint8_t>& out) {
    Append(out, Call(h, sub_8229A930, {heap, 0, 0, 16}));
  });
  results.Report();
}

void PageCases(Context& h) {
  Results results("heap: VirtualAlloc, VirtualFree");
  for (uint32_t size : {1u, 4096u, 5000u, 70000u, 1u << 20}) {
    for (uint32_t type : {0x1000u, 0x801000u, 0x20801000u}) {
      Case(results, fmt::format("size {} type {:#x}", size, type), [&](std::vector<uint8_t>& out) {
        const uint32_t p = Call(h, sub_82295398, {0, size, type, 4});
        Append(out, p != 0);
        if (p == 0) {
          return;
        }
        Append(out, p % ((type & 0x20000000) ? 65536 : 4096));
        // MEM_NOZERO (0x800000) leaves the SDK's pages as they were.
        if (!(type & 0x800000)) {
          Append(out, AllZero(h.base, p, size));
        }
        WritePattern(h.base, p, size);
        Append(out, HasPattern(h.base, p, size));
        Append(out, Call(h, sub_82295330, {p, 0, 0x8000}) != 0);
      });
    }
  }
  results.Report();
}

void PhysicalCases(Context& h) {
  Results results("heap: XPhysicalAlloc, XPhysicalAllocEx, XPhysicalFree");
  for (uint32_t size : {4096u, 0x10000u, 100000u}) {
    for (uint32_t alignment : {0u, 4096u, 0x10000u}) {
      for (uint32_t protection : {0x4u, 0x20000004u, 0x404u}) {
        for (bool ex : {false, true}) {
          Case(results,
               fmt::format("{} size {} alignment {:#x} protection {:#x}",
                           ex ? "XPhysicalAllocEx" : "XPhysicalAlloc", size, alignment, protection),
               [&](std::vector<uint8_t>& out) {
                 const uint32_t p =
                     ex ? Call(h, sub_82296550, {size, 0, 0x1FFFFFFF, alignment, protection})
                        : Call(h, sub_822964C8, {size, 0xFFFFFFFF, alignment, protection});
                 Append(out, p != 0);
                 if (p == 0) {
                   return;
                 }
                 // Which physical view (by page size), and the alignment.
                 Append(out, p >> 29);
                 Append(out, p % std::max(alignment, 4096u));
                 WritePattern(h.base, p, size);
                 Append(out, HasPattern(h.base, p, size));
                 Call(h, sub_822965C8, {p});
               });
        }
      }
    }
  }
  results.Report();
}

}  // namespace

void Heap(Context& h) {
  HeapCases(h);
  ReallocCases(h);
  PageCases(h);
  PhysicalCases(h);
}

}  // namespace harness
