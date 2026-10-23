#include "std_string/std_string.h"

#include "guest.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

namespace std_string {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) {
  return guest::LoadU32(base, address);
}
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

// Whether operator delete frees `block` natively: 0, or a block the layer
// gave (else free's error path, the original's).
bool Deletable(uint32_t block) {
  return block == 0 || memory::IsHeapBlock(block);
}

// Whether memcpy_s copies `count` bytes natively with the host's memcpy: a
// destination and source (neither null), the count within the destination's
// size, outside the physical views (memcpy's override records those for the
// renderer's mirrors) and in one view each, not sharing memory.
bool CopyFits(uint32_t destination, uint32_t size, uint32_t source, uint32_t count) {
  return destination != 0 && source != 0 && count <= size && destination < 0xA0000000u &&
         c_runtime::InOneView(destination, count) && c_runtime::InOneView(source, count) &&
         !c_runtime::MayShare(destination, count, source, count);
}

}  // namespace

uint32_t Text(uint8_t* base, uint32_t string) {
  return Load(base, string + kCapacity) < kInPlace ? string + kText : Load(base, string + kText);
}

bool Tidy(uint8_t* base, uint32_t string, bool built, uint32_t length) {
  ITC_NATIVE_RUNNING("sub_82091328 std::string::_Tidy");
  if (built && Load(base, string + kCapacity) >= kInPlace) {
    const uint32_t block = Load(base, string + kText);
    if (!Deletable(block) ||
        (length != 0 && !CopyFits(string + kText, kInPlace, block, length))) {
      return false;
    }
    if (length != 0) {
      c_runtime::Memcpy(base, string + kText, block, length);
    }
    memory::crt::OperatorDelete(block);
  }
  Store(base, string + kLength, length);
  Store(base, string + kCapacity, 15);
  guest::StoreU8(base, string + kText + length, 0);
  return true;
}

std::optional<uint32_t> Allocate(uint8_t* base, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_820912B0 std::allocator<char>::allocate");
  return memory::crt::OperatorNew(base, count);
}

}  // namespace std_string
