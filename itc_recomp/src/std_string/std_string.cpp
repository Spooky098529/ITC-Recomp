#include "std_string/std_string.h"

#include "guest.h"
#include "schema/file_layer.h"
#include "schema/std_string.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <cstdlib>

namespace std_string {

namespace {

schema::StdString Str(uint32_t string) {
  return schema::StdString(string);
}
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
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

// Whether operator new gives `size` bytes natively (malloc's checks; the
// layer aborts when out of memory): else the new handler and
// std::bad_alloc, the original's.
bool Allocatable(uint32_t size) {
  return size <= 0xFFFFF000u && memory::crt::GetProcessHeap() != 0;
}

// A copy whose checks were made before anything changed, into a block just
// allocated (which shares memory with nothing live): the host's memcpy, or
// a logged abort when it can't be.
void CopyChecked(uint32_t destination, uint32_t size, uint32_t source,
                 uint32_t count, const char* what) {
  if (count == 0) {
    return;
  }
  if (!CopyFits(destination, size, source, count) ||
      !c_runtime::Memcpy(destination, source, count)) {
    ITC_LOG_ERROR("std::string: {}: memcpy_s({:08X}, {}, {:08X}, {}) failed after its checks",
                  what, destination, size, source, count);
    std::abort();
  }
}

// A callee whose checks the caller made: aborts when it refuses anyway.
void Require(bool done, const char* what) {
  if (!done) {
    ITC_LOG_ERROR("std::string: {} refused after its checks", what);
    std::abort();
  }
}

uint32_t TextOf(uint32_t string);

// Whether the memcpy_s after _Grow(string, new_length, false) copies natively:
// into the block _Copy makes when the capacity is less than `new_length` (a
// fresh block: only the source is checked here; CopyChecked does the rest),
// else into the text at `offset`, `room` bytes of room as the caller passes.
bool CopyAfterGrowFits(uint32_t string, uint32_t new_length, uint32_t offset,
                       uint32_t source, uint32_t count) {
  if (count == 0) {
    return true;
  }
  const uint32_t capacity = Str(string).capacity();
  if (capacity < new_length) {
    return source != 0 && c_runtime::InOneView(source, count);
  }
  return CopyFits(TextOf(string) + offset, capacity - offset, source, count);
}

// Whether `text` points inside the string's own text (from its start up to
// its end), as assign's and append's checks.
bool Inside(uint32_t string, uint32_t text) {
  const uint32_t own = TextOf(string);
  return text >= own && own + Str(string).length() > text;
}

uint32_t TextOf(uint32_t string) {
  const schema::StdString s = Str(string);
  return s.capacity() < kInPlace ? s.text_address() : s.block();
}

}  // namespace

uint32_t Text(uint32_t string) {
  return TextOf(string);
}

bool Tidy(uint32_t string, bool built, uint32_t length) {
  ITC_NATIVE_RUNNING("sub_82091328 std::string::_Tidy");
  if (built && Str(string).capacity() >= kInPlace) {
    const uint32_t block = Str(string).block();
    if (!Deletable(block) ||
        (length != 0 && !CopyFits(Str(string).text_address(), kInPlace, block, length))) {
      return false;
    }
    if (length != 0) {
      c_runtime::Memcpy(Str(string).text_address(), block, length);
    }
    memory::crt::OperatorDelete(block);
  }
  Str(string).set_length(length);
  Str(string).set_capacity(15);
  guest::StoreU8(Str(string).text_address() + length, 0);
  return true;
}

std::optional<uint32_t> Allocate(uint32_t count) {
  ITC_NATIVE_RUNNING("sub_820912B0 std::allocator<char>::allocate");
  return memory::crt::OperatorNew(count);
}

bool Copy(uint32_t string, uint32_t new_length, uint32_t keep) {
  ITC_NATIVE_RUNNING("sub_82091458 std::string::_Copy");
  const uint32_t old_capacity = Str(string).capacity();
  uint32_t capacity = new_length | 15;
  if (capacity > 0xFFFFFFFEu) {
    capacity = new_length;
  } else {
    const uint32_t half = old_capacity >> 1;
    if (capacity / 3 < half && old_capacity <= 0xFFFFFFFEu - half) {
      capacity = half + old_capacity;
    }
  }
  const uint32_t size = capacity + 1;
  const uint32_t old_text = Text(string);
  if (!Allocatable(size) ||
      (keep != 0 && (old_text == 0 || keep > size || !c_runtime::InOneView(old_text, keep))) ||
      (old_capacity >= kInPlace && !Deletable(Str(string).block()))) {
    return false;
  }
  const auto block = Allocate(size);
  Require(block.has_value(), "_Copy's allocate");
  CopyChecked(*block, size, old_text, keep, "_Copy");
  Require(Tidy(string, true, 0), "_Copy's _Tidy");
  Str(string).set_capacity(capacity);
  Str(string).set_block(*block);
  const uint32_t text = capacity < kInPlace ? Str(string).text_address() : *block;
  Str(string).set_length(keep);
  guest::StoreU8(text + keep, 0);
  return true;
}

std::optional<bool> Grow(uint32_t string, uint32_t new_length, bool trim) {
  ITC_NATIVE_RUNNING("sub_82091540 std::string::_Grow");
  if (new_length > 0xFFFFFFFEu) {
    return std::nullopt;
  }
  const uint32_t capacity = Str(string).capacity();
  if (capacity < new_length) {
    if (!Copy(string, new_length, Str(string).length())) {
      return std::nullopt;
    }
  } else if (trim && new_length < kInPlace) {
    const uint32_t length = Str(string).length();
    if (!Tidy(string, true, new_length < length ? new_length : length)) {
      return std::nullopt;
    }
  } else if (new_length == 0) {
    const uint32_t text =
        capacity < kInPlace ? Str(string).text_address() : Str(string).block();
    Str(string).set_length(0);
    guest::StoreU8(text, 0);
  }
  return new_length != 0;
}

bool AssignFrom(uint32_t string, uint32_t source, uint32_t offset,
                uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82091720 std::string::assign(const string&, size_t, size_t)");
  const uint32_t source_length = Str(source).length();
  if (source_length < offset || string == source) {
    return false;
  }
  uint32_t n = source_length - offset;
  if (count < n) {
    n = count;
  }
  if (!CopyAfterGrowFits(string, n, 0, Text(source) + offset, n)) {
    return false;
  }
  const auto grown = Grow(string, n, false);
  if (!grown) {
    return false;
  }
  if (*grown) {
    const uint32_t from = Text(source) + offset;
    const uint32_t text = Text(string);
    CopyChecked(text, Str(string).capacity(), from, n, "assign");
    Str(string).set_length(n);
    guest::StoreU8(Text(string) + n, 0);
  }
  return true;
}

bool AssignText(uint32_t string, uint32_t text, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82091808 std::string::assign(const char*, size_t)");
  if (Inside(string, text) || !CopyAfterGrowFits(string, count, 0, text, count)) {
    return false;
  }
  const auto grown = Grow(string, count, false);
  if (!grown) {
    return false;
  }
  if (*grown) {
    CopyChecked(Text(string), Str(string).capacity(), text, count,
                "assign");
    Str(string).set_length(count);
    guest::StoreU8(Text(string) + count, 0);
  }
  return true;
}

bool Append(uint32_t string, uint32_t text, uint32_t count) {
  ITC_NATIVE_RUNNING("sub_820A6F60 std::string::append(const char*, size_t)");
  if (Inside(string, text)) {
    return false;
  }
  const uint32_t length = Str(string).length();
  if (!(0xFFFFFFFFu - length > count) || length + count < length) {
    return false;
  }
  if (count == 0) {
    return true;
  }
  const uint32_t new_length = length + count;
  if (!CopyAfterGrowFits(string, new_length, length, text, count)) {
    return false;
  }
  const auto grown = Grow(string, new_length, false);
  if (!grown) {
    return false;
  }
  if (*grown) {
    const uint32_t capacity = Str(string).capacity();
    const uint32_t own = Text(string);
    const uint32_t kept = Str(string).length();
    CopyChecked(kept + own, capacity - kept, text, count, "append");
    Str(string).set_length(new_length);
    guest::StoreU8(Text(string) + new_length, 0);
  }
  return true;
}

void Construct(uint32_t string) {
  ITC_NATIVE_RUNNING("sub_825929E8 std::string::string");
  Require(Tidy(string, false, 0), "string()'s _Tidy");
}

bool ConstructFromText(uint32_t string, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_825931D0 std::string::string(const char*)");
  Require(Tidy(string, false, 0), "string(const char*)'s _Tidy");
  uint32_t end = text;
  while (guest::LoadU8(end) != 0) {
    end++;
  }
  return AssignText(string, text, end - text);
}

bool ConstructFromTextWithAllocator(uint32_t string, uint32_t text) {
  ITC_NATIVE_RUNNING("sub_82593248 std::string::string(const char*, const allocator&)");
  return ConstructFromText(string, text);
}

bool ConstructFrom(uint32_t string, uint32_t source, uint32_t offset,
                   uint32_t count) {
  ITC_NATIVE_RUNNING("sub_82593040 std::string::string(const string&, size_t, size_t)");
  Require(Tidy(string, false, 0), "string(const string&, ...)'s _Tidy");
  return AssignFrom(string, source, offset, count);
}

uint32_t WildcardSearch(uint32_t text, uint32_t pattern, uint32_t offset_out,
                        uint32_t length_out) {
  ITC_NATIVE_RUNNING("sub_82592740 WildcardSearch");
  auto byte = [&](uint32_t address) { return guest::LoadU8(address); };
  auto signed_byte = [&](uint32_t address) { return int32_t(int8_t(guest::LoadU8(address))); };
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
        schema::StdStringMatchWord(offset_out).set_value(match - text);
      }
      if (length_out != 0) {
        schema::StdStringMatchWord(length_out).set_value(last - match + 1);
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

uint32_t Find(uint32_t string, uint32_t pattern, uint32_t offset_out,
              uint32_t length_out, uint32_t from) {
  ITC_NATIVE_RUNNING("sub_82592970 String::Find");
  if (Str(string).length() == 0) {
    return 0;
  }
  const uint32_t match =
      WildcardSearch(Text(string) + from, pattern, offset_out, length_out);
  if (match == 0) {
    return 0;
  }
  if (offset_out != 0) {
    const schema::StdStringMatchWord offset(offset_out);
    offset.set_value(offset.value() + from);
  }
  return match;
}

std::optional<uint32_t> ReplaceAll(uint32_t string, uint32_t pattern,
                                   uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82593088 String::ReplaceAll");
  Store(frame + 80, 0);
  Store(frame + 84, 0);
  if (Find(string, pattern, frame + 80, frame + 84, 0) != 0) {
    return std::nullopt;
  }
  return Text(string);
}

std::optional<int32_t> InitGlobalString8265EE7C(const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825DA540 (the string at 0x8265EE7C's static set-up)");
  if (!AssignText(schema::FileLayerGlobals::prefix_string_address(), 0x8206CC74, 9)) {
    return std::nullopt;
  }
  return c_runtime::AtexitFrom(0x825DB328, call);
}

std::optional<int32_t> InitGlobalString8265EE98(const c_runtime::AtexitCall& call) {
  ITC_NATIVE_RUNNING("sub_825DA580 (the string at 0x8265EE98's static set-up)");
  if (!AssignText(schema::FileLayerGlobals::project_string_address(), 0x8206CC80, 13)) {
    return std::nullopt;
  }
  return c_runtime::AtexitFrom(0x825DB380, call);
}

}  // namespace std_string
