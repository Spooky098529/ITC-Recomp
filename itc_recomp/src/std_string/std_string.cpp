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

uint32_t WildcardSearch(uint8_t* base, uint32_t text, uint32_t pattern, uint32_t offset_out,
                        uint32_t length_out) {
  ITC_NATIVE_RUNNING("sub_82592740 WildcardSearch");
  auto byte = [&](uint32_t address) { return guest::LoadU8(base, address); };
  auto signed_byte = [&](uint32_t address) { return int32_t(int8_t(guest::LoadU8(base, address))); };
  if (text == 0) {
    return 0;
  }
  const int32_t first = signed_byte(pattern);
  uint32_t start = text;  // where this try starts (0: stop)
  for (;;) {
    uint32_t t = start;    // the text's position
    uint32_t p = pattern;  // the pattern's position (0: this try is over)
    uint32_t match = 0;    // the match's first character
    uint32_t last = 0;     // its last
    if (first == 2 && start != text) {
      return 0;
    }
    while (p != 0) {
      const int32_t c = signed_byte(p);
      if (c == 0) {
        p = 0;
      } else if (c == 2) {
        p++;
      } else if (c == 3) {
        if (byte(t) != 0) {
          start = 0;
          last = 0;
          match = 0;
        }
        break;
      } else if (c == 42) {
        // '*': the wildcards after it skipped, then the next pattern
        // character against the text's next one.
        uint32_t skip = 0;
        for (;;) {
          const int32_t next = signed_byte(p + 1 + skip);
          if (next != 63 && next != 42) {
            break;
          }
          skip++;
        }
        if (byte(t) == 0) {
          const int32_t after = signed_byte(skip + p + 1);
          if (after != 0 && after != 3) {
            start = 0;
            last = 0;
            match = 0;
          }
          break;
        }
        const uint32_t next = skip + p + 1;
        if (byte(next) == byte(t + 1)) {
          last = t;
          t = t + 1;
          p = next;
        } else {
          if (match == 0) {
            match = t;
          }
          last = t;
          t = t + 1;
        }
      } else {
        bool same;
        if (c == 63) {
          if (byte(t) == 0) {
            return 0;
          }
          same = true;
        } else {
          const int32_t have = signed_byte(t);
          if (have == 0) {
            return 0;
          }
          same = c == have;
        }
        if (same) {
          if (match == 0) {
            match = t;
          }
          last = t;
          p++;
          t++;
        } else {
          match = 0;
          last = 0;
          p = 0;
        }
      }
    }
    if (match != 0 && last != 0) {
      if (offset_out != 0) {
        Store(base, offset_out, match - text);
      }
      if (length_out != 0) {
        Store(base, length_out, last - match + 1);
      }
      return match;
    }
    if (start == 0) {
      return 0;
    }
    start++;
    if (start == 0) {
      return 0;
    }
  }
}

uint32_t Find(uint8_t* base, uint32_t string, uint32_t pattern, uint32_t offset_out,
              uint32_t length_out, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_82592970 String::Find");
  if (Load(base, string + kLength) == 0) {
    return 0;
  }
  const uint32_t match =
      WildcardSearch(base, Text(base, string) + from, pattern, offset_out, length_out);
  if (match == 0) {
    return 0;
  }
  if (offset_out != 0) {
    Store(base, offset_out, Load(base, offset_out) + from);
  }
  return match;
}

std::optional<uint32_t> ReplaceAll(uint8_t* base, uint32_t string, uint32_t pattern,
                                   uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82593088 String::ReplaceAll");
  Store(base, frame + 80, 0);
  Store(base, frame + 84, 0);
  if (Find(base, string, pattern, frame + 80, frame + 84, 0) != 0) {
    return std::nullopt;
  }
  return Text(base, string);
}

}  // namespace std_string
