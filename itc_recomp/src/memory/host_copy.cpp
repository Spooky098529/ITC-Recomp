#include "memory/host_copy.h"

#include "base/log.h"
#include "guest.h"
#include "memory/handles.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <set>
#include <utility>

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
  return {static_cast<uint8_t*>(info.object) + info.offset, info.offset, info.layout, info.type};
}

void Swap(uint8_t* p, uint32_t width) {
  for (uint32_t i = 0; i < width / 2; i++) {
    std::swap(p[i], p[width - 1 - i]);
  }
}

// Swaps, in `bytes` (a copy of the host object's range from `offset`, `size`
// bytes), each scalar field of `layout` the range covers: guest order to
// host order or back (the same operation).
void SwapFields(uint8_t* bytes, const Layout* layout, uint32_t offset, uint32_t size,
                const char* type) {
  if (!layout) {
    return;
  }
  for (uint32_t i = 0; i < layout->count; i++) {
    const LayoutField& f = layout->fields[i];
    const uint64_t end = uint64_t(offset) + size;
    if (f.offset + f.width <= offset || f.offset >= end) {
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

// The window an overlay touches, widened to the whole members it cuts.
void OverlayWindow(const Layout& layout, uint32_t offset, uint32_t width, uint32_t* lo,
                   uint32_t* hi) {
  *lo = offset;
  *hi = offset + width;
  for (uint32_t i = 0; i < layout.count; i++) {
    const LayoutField& f = layout.fields[i];
    if (f.offset < *hi && *lo < f.offset + f.width) {
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
  SwapFields(scratch, &layout, lo, hi - lo, "overlay");
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
  SwapFields(scratch, &layout, lo, hi - lo, "overlay");
  for (uint32_t i = 0; i < width; i++) {
    scratch[offset - lo + width - 1 - i] = uint8_t(value >> (8 * i));
  }
  SwapFields(scratch, &layout, lo, hi - lo, "overlay");
  std::memcpy(object + lo, scratch, hi - lo);
}

namespace {
std::mutex g_raw_mutex;
std::set<std::pair<const char*, uint32_t>> g_raw_seen;

uint8_t* RawObject(uint32_t address, uint32_t width, HandleInfo* info) {
  if (!DescribeHandle(address, info) || uint64_t(info->offset) + width > info->size) {
    ITC_LOG_ERROR("handles: a raw {}-byte access at {:08X}, no live object there", width, address);
    std::abort();
  }
  {
    std::lock_guard lock(g_raw_mutex);
    if (g_raw_seen.emplace(info->type, info->offset).second) {
      ITC_LOG_WARN("handles: a raw {}-byte access at +{} of a {} (code in guest shape)", width,
                   info->offset, info->type);
    }
  }
  return static_cast<uint8_t*>(info->object);
}

const Layout kNoLayout = {nullptr, 0};
}  // namespace

uint64_t RawHandleLoad(uint32_t address, uint32_t width) {
  HandleInfo info;
  const uint8_t* object = RawObject(address, width, &info);
  return LoadOverlay(object, info.layout ? *info.layout : kNoLayout, info.offset, width);
}

void RawHandleStore(uint32_t address, uint32_t width, uint64_t value) {
  HandleInfo info;
  uint8_t* object = RawObject(address, width, &info);
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
    std::memmove(to.bytes, from.bytes, size);
    return true;
  }
  if (to_host) {
    const Span to = SpanOf(destination, size);
    guest::ForEachHostPart(base, source, size, [&](uint8_t* host, uint32_t at, uint32_t n) {
      std::memmove(to.bytes + at, host, n);
    });
    SwapFields(to.bytes, to.layout, to.offset, size, to.type);
    return true;
  }
  const Span from = SpanOf(source, size);
  guest::ForEachHostPart(base, destination, size, [&](uint8_t* host, uint32_t at, uint32_t n) {
    std::memmove(host, from.bytes + at, n);
  });
  // Swapped in place in guest memory, field by field (a field split across
  // the 0xE0000000 views' change isn't handled: no host object copies there).
  SwapFields(guest::Pointer(base, destination), from.layout, from.offset, size, from.type);
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
