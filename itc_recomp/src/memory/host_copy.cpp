#include "memory/host_copy.h"

#include "base/log.h"
#include "guest.h"
#include "memory/handles.h"

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <vector>

namespace memory {
namespace {

// The handle's object, the offset of `address` into it, and its layout;
// a range past the object's end stops the program (a copy over the end of
// a host object would write past its allocation).
struct Span {
  uint8_t* bytes;  // at `address`
  uint32_t offset;
  const Layout* layout;
  const char* type;
  uint32_t element;  // the layout repeats every `element` bytes (an array)
};

Span SpanOf(uint32_t address, uint32_t size) {
  HandleInfo info;
  if (!DescribeHandle(address, &info)) {
    ITC_LOG_ERROR("host copy: {:08X} is in the handles' range but no live object's", address);
    std::abort();
  }
  if (uint64_t(info.offset) + size > info.size) {
    ITC_LOG_ERROR("host copy: {} bytes at +{} of a {} ({} bytes): past its end", size,
                  info.offset, info.type, info.size);
    std::abort();
  }
  return {static_cast<uint8_t*>(info.object) + info.offset, info.offset, info.layout, info.type,
          info.element};
}

// The first field of `layout` (sorted by offset, at most 8 bytes wide) that
// may reach `offset` or past it: the search's start, so big layouts (a
// track's zones, 18,555 fields) aren't scanned from the start.
uint32_t FirstFrom(const Layout& layout, uint32_t offset) {
  const uint32_t from = offset >= 8 ? offset - 8 : 0;
  const LayoutField* f = std::lower_bound(
      layout.fields, layout.fields + layout.count, from,
      [](const LayoutField& a, uint32_t o) { return a.offset < o; });
  return uint32_t(f - layout.fields);
}

void Swap(uint8_t* p, uint32_t width) {
  for (uint32_t i = 0; i < width / 2; i++) {
    std::swap(p[i], p[width - 1 - i]);
  }
}

// Swaps, in `bytes` (a copy of the host object's range from `offset`, `size`
// bytes), each scalar field of `layout` the range covers: guest order to
// host order or back (the same operation).
void SwapFieldsOne(uint8_t* bytes, const Layout* layout, uint32_t offset, uint32_t size,
                   const char* type) {
  if (!layout) {
    return;
  }
  const uint64_t end = uint64_t(offset) + size;
  for (uint32_t i = FirstFrom(*layout, offset); i < layout->count; i++) {
    const LayoutField& f = layout->fields[i];
    if (f.offset >= end) {
      break;
    }
    if (f.offset + f.width <= offset) {
      continue;
    }
    if (f.offset < offset || f.offset + f.width > end) {
      static bool logged = false;
      if (!logged) {
        logged = true;
        ITC_LOG_ERROR("host copy: a copy of +{} ({} bytes) cuts a {}'s field at +{} ({} bytes): "
                      "its bytes come out in the other order",
                      offset, size, type, f.offset, f.width);
      }
      continue;
    }
    Swap(bytes + (f.offset - offset), f.width);
  }
}

// The same over an array's elements (`element` bytes each, one layout
// repeated): each element's part of the range swapped with its offsets.
void SwapFields(uint8_t* bytes, const Layout* layout, uint32_t offset, uint32_t size,
                const char* type, uint32_t element) {
  if (!layout || !element) {
    return;
  }
  uint32_t done = 0;
  while (done < size) {
    const uint32_t at = offset + done;
    const uint32_t inside = at % element;
    const uint32_t n = std::min(size - done, element - inside);
    SwapFieldsOne(bytes + done, layout, inside, n, type);
    done += n;
  }
}

// The window an overlay touches, widened to the whole members it cuts.
void OverlayWindow(const Layout& layout, uint32_t offset, uint32_t width, uint32_t* lo,
                   uint32_t* hi) {
  *lo = offset;
  *hi = offset + width;
  for (uint32_t i = FirstFrom(layout, offset); i < layout.count; i++) {
    const LayoutField& f = layout.fields[i];
    if (f.offset >= *hi) {
      break;
    }
    if (*lo < f.offset + f.width) {
      *lo = std::min(*lo, f.offset);
      *hi = std::max(*hi, f.offset + f.width);
    }
  }
}

}  // namespace

uint64_t LoadOverlay(const uint8_t* object, const Layout& layout, uint32_t offset, uint32_t width) {
  uint32_t lo, hi;
  OverlayWindow(layout, offset, width, &lo, &hi);
  uint8_t scratch[64];
  if (hi - lo > sizeof(scratch)) {
    std::abort();
  }
  std::memcpy(scratch, object + lo, hi - lo);
  SwapFieldsOne(scratch, &layout, lo, hi - lo, "overlay");
  uint64_t v = 0;
  for (uint32_t i = 0; i < width; i++) {
    v = v << 8 | scratch[offset - lo + i];
  }
  return v;
}

void StoreOverlay(uint8_t* object, const Layout& layout, uint32_t offset, uint32_t width,
                  uint64_t value) {
  uint32_t lo, hi;
  OverlayWindow(layout, offset, width, &lo, &hi);
  uint8_t scratch[64];
  if (hi - lo > sizeof(scratch)) {
    std::abort();
  }
  std::memcpy(scratch, object + lo, hi - lo);
  SwapFieldsOne(scratch, &layout, lo, hi - lo, "overlay");
  for (uint32_t i = 0; i < width; i++) {
    scratch[offset - lo + width - 1 - i] = uint8_t(value >> (8 * i));
  }
  SwapFieldsOne(scratch, &layout, lo, hi - lo, "overlay");
  std::memcpy(object + lo, scratch, hi - lo);
}

namespace {
// Raw accesses logged once per (type, offset): a lock-free table of the
// sites seen (a hash of both, never 0), checked without a lock on every
// access; full after kRawSites sites, then nothing more is logged.
constexpr uint32_t kRawSites = 4096;
constexpr uint32_t kRawSlots = 16384;
std::atomic<uint64_t> g_raw_seen[kRawSlots];
std::atomic<uint32_t> g_raw_count{0};

void NoteRaw(const HandleInfo& info, uint32_t width) {
  if (g_raw_count.load(std::memory_order_relaxed) >= kRawSites) {
    return;
  }
  uint64_t key = (reinterpret_cast<uintptr_t>(info.type) * 0x9E3779B97F4A7C15ull) ^
                 (uint64_t(info.offset) * 0xC2B2AE3D27D4EB4Full);
  key |= 1;
  for (uint32_t i = 0; i < kRawSlots; i++) {
    std::atomic<uint64_t>& slot = g_raw_seen[(key + i) % kRawSlots];
    uint64_t seen = slot.load(std::memory_order_relaxed);
    if (seen == key) {
      return;
    }
    if (seen == 0) {
      if (!slot.compare_exchange_strong(seen, key) && seen == key) {
        return;  // another thread logged it
      } else if (seen != 0 && seen != key) {
        continue;  // taken by another site meanwhile
      }
      g_raw_count.fetch_add(1, std::memory_order_relaxed);
      ITC_LOG_WARN("handles: a raw {}-byte access at +{} of a {} (code in guest shape)", width,
                   info.offset, info.type);
      return;
    }
  }
}

// The element (an array's, or the object) holding the access, with
// info->offset made the offset into it.
uint8_t* RawObject(uint32_t address, uint32_t width, HandleInfo* info, bool note = true) {
  if (!DescribeHandle(address, info) || uint64_t(info->offset) + width > info->size) {
    ITC_LOG_ERROR("handles: a raw {}-byte access at {:08X}, no live object there", width, address);
    std::abort();
  }
  if (note) {
    NoteRaw(*info, width);
  }
  const uint32_t inside = info->offset % info->element;
  uint8_t* element = static_cast<uint8_t*>(info->object) + (info->offset - inside);
  info->offset = inside;
  return element;
}

const Layout kNoLayout = {nullptr, 0};
}  // namespace

uint64_t RawHandleLoad(uint32_t address, uint32_t width) {
  // A blob: its bytes in the guest's order, read as guest memory would be.
  uint32_t left = 0;
  if (const uint8_t* p = BlobPointer(address, &left); p && left >= width) {
    uint64_t v = 0;
    for (uint32_t i = 0; i < width; i++) {
      v = v << 8 | p[i];
    }
    return v;
  }
  HandleInfo info;
  const uint8_t* object = RawObject(address, width, &info);
  return LoadOverlay(object, info.layout ? *info.layout : kNoLayout, info.offset, width);
}

void RawHandleStore(uint32_t address, uint32_t width, uint64_t value) {
  uint32_t left = 0;
  if (uint8_t* p = BlobPointer(address, &left); p && left >= width) {
    for (uint32_t i = 0; i < width; i++) {
      p[width - 1 - i] = uint8_t(value >> (8 * i));
    }
    return;
  }
  HandleInfo info;
  uint8_t* object = RawObject(address, width, &info);
  StoreOverlay(object, info.layout ? *info.layout : kNoLayout, info.offset, width, value);
}

uint64_t OwnerLoad(uint32_t address, uint32_t width) {
  HandleInfo info;
  const uint8_t* object = RawObject(address, width, &info, false);
  return LoadOverlay(object, info.layout ? *info.layout : kNoLayout, info.offset, width);
}

void OwnerStore(uint32_t address, uint32_t width, uint64_t value) {
  HandleInfo info;
  uint8_t* object = RawObject(address, width, &info, false);
  StoreOverlay(object, info.layout ? *info.layout : kNoLayout, info.offset, width, value);
}

bool CopyWithHandles(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size) {
  const bool to_host = IsHandle(destination), from_host = IsHandle(source);
  if (!to_host && !from_host) {
    return false;
  }
  if (size == 0) {
    return true;
  }
  if (to_host && from_host) {
    const Span to = SpanOf(destination, size), from = SpanOf(source, size);
    // The same layout at the same place in it: the bytes are ordered alike.
    if (to.layout == from.layout && to.offset % to.element == from.offset % from.element &&
        to.element == from.element) {
      std::memmove(to.bytes, from.bytes, size);
      return true;
    }
    // Else through the guest's order: the source's fields swapped by its
    // layout, then the destination's by its own (a record kept as bytes in
    // one owner and as members in another).
    std::vector<uint8_t> bytes(from.bytes, from.bytes + size);
    SwapFields(bytes.data(), from.layout, from.offset, size, from.type, from.element);
    SwapFields(bytes.data(), to.layout, to.offset, size, to.type, to.element);
    std::memcpy(to.bytes, bytes.data(), size);
    return true;
  }
  if (to_host) {
    const Span to = SpanOf(destination, size);
    guest::ForEachHostPart(base, source, size, [&](uint8_t* host, uint32_t at, uint32_t n) {
      std::memmove(to.bytes + at, host, n);
    });
    SwapFields(to.bytes, to.layout, to.offset, size, to.type, to.element);
    return true;
  }
  const Span from = SpanOf(source, size);
  guest::ForEachHostPart(base, destination, size, [&](uint8_t* host, uint32_t at, uint32_t n) {
    std::memmove(host, from.bytes + at, n);
  });
  // Swapped in place in guest memory, field by field (a field split across
  // the 0xE0000000 views' change isn't handled: no host object copies there).
  SwapFields(guest::Pointer(base, destination), from.layout, from.offset, size, from.type,
             from.element);
  return true;
}

bool FillWithHandles(uint8_t* base, uint32_t destination, uint8_t value, uint32_t size) {
  (void)base;
  if (!IsHandle(destination)) {
    return false;
  }
  if (size) {
    std::memset(SpanOf(destination, size).bytes, value, size);
  }
  return true;
}

}  // namespace memory
