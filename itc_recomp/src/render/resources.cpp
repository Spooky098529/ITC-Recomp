#include "render/resources.h"

#include "base/log.h"

#include <algorithm>
#include <cstdint>
#include <deque>
#include <mutex>
#include <vector>

namespace render {

namespace {

struct Event {
  enum Kind : uint8_t {
    kTextureCreated,
    kTextureWritten,
    kTextureDestroyed,
    kBufferCreated,
    kBufferWritten,
    kBufferDestroyed,
  } kind;
  uint32_t handle;
  BufferData data{};  // kBufferCreated's
};

// Events not yet applied (any thread, under g_mutex), numbered from
// g_first; each handed-over list's end in that numbering, not yet replayed;
// and the end of what goes before the list being replayed (render thread).
std::mutex g_mutex;
std::deque<Event> g_events;
uint64_t g_first = 0;
std::deque<uint64_t> g_marks;
uint64_t g_due = 0;
// Render thread: the frees held back until the next list.
std::vector<Event> g_held_frees;

void Queue(Event::Kind kind, uint32_t handle, const BufferData& data = {}) {
  // A harness's calls build their own world in scratch memory: not the
  // game's resources.
  if (!handle || base::log_detail::in_harness) {
    return;
  }
  std::lock_guard lock(g_mutex);
  g_events.push_back({kind, handle, data});
}

void Destroy(Backend& backend, const Event& free) {
  if (free.kind == Event::kTextureDestroyed) {
    backend.TextureDestroyed({free.handle});
  } else {
    backend.BufferDestroyed({free.handle});
  }
}

// A free of `kind` of `handle` held back in `frees`: applied first (the
// handle is being made again).
void DestroyHeld(Backend& backend, std::vector<Event>& frees, Event::Kind kind, uint32_t handle) {
  auto it = std::find_if(frees.begin(), frees.end(), [&](const Event& e) {
    return e.kind == kind && e.handle == handle;
  });
  if (it != frees.end()) {
    Destroy(backend, *it);
    frees.erase(it);
  }
}

}  // namespace

void TextureCreated(Texture texture) {
  Queue(Event::kTextureCreated, texture.address);
}

void TextureWritten(Texture texture) {
  Queue(Event::kTextureWritten, texture.address);
}

void TextureDestroyed(Texture texture) {
  Queue(Event::kTextureDestroyed, texture.address);
}

void BufferCreated(Buffer buffer, BufferData data) {
  Queue(Event::kBufferCreated, buffer.address, data);
}

void BufferWritten(Buffer buffer) {
  Queue(Event::kBufferWritten, buffer.address);
}

void BufferDestroyed(Buffer buffer) {
  Queue(Event::kBufferDestroyed, buffer.address);
}

void ResourceEvents_OnListHandedOver() {
  std::lock_guard lock(g_mutex);
  g_marks.push_back(g_first + g_events.size());
}

void ResourceEvents_OnReplayStart() {
  std::lock_guard lock(g_mutex);
  if (g_marks.empty()) {
    // Not handed over by WaitForFrame (as Renderer_OnReplayStart): all.
    g_due = g_first + g_events.size();
    return;
  }
  g_due = g_marks.front();
  g_marks.pop_front();
}

void ResourceEvents_Apply(Backend& backend) {
  static thread_local std::vector<Event> due;
  static thread_local std::vector<Event> frees;
  due.clear();
  {
    std::lock_guard lock(g_mutex);
    while (!g_events.empty() && g_first < g_due) {
      due.push_back(g_events.front());
      g_events.pop_front();
      g_first++;
    }
  }
  // The last list's frees: it's replayed.
  for (const Event& e : g_held_frees) {
    Destroy(backend, e);
  }
  g_held_frees.clear();
  frees.clear();
  for (const Event& e : due) {
    switch (e.kind) {
      case Event::kTextureCreated:
        DestroyHeld(backend, frees, Event::kTextureDestroyed, e.handle);
        backend.TextureCreated({e.handle});
        break;
      case Event::kTextureWritten:
        backend.TextureWritten({e.handle});
        break;
      case Event::kBufferCreated:
        DestroyHeld(backend, frees, Event::kBufferDestroyed, e.handle);
        backend.BufferCreated({e.handle}, e.data);
        break;
      case Event::kBufferWritten:
        backend.BufferWritten({e.handle});
        break;
      case Event::kTextureDestroyed:
      case Event::kBufferDestroyed:
        frees.push_back(e);
        break;
    }
  }
  g_held_frees.swap(frees);
}

}  // namespace render
