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
  enum Kind : uint8_t { kTextureCreated, kTextureWritten, kTextureDestroyed } kind;
  uint32_t handle;
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
std::vector<uint32_t> g_held_frees;

void Queue(Event::Kind kind, uint32_t handle) {
  // A harness's calls build their own world in scratch memory: not the
  // game's textures.
  if (!handle || base::log_detail::in_harness) {
    return;
  }
  std::lock_guard lock(g_mutex);
  g_events.push_back({kind, handle});
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
  static thread_local std::vector<uint32_t> frees;
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
  for (uint32_t handle : g_held_frees) {
    backend.TextureDestroyed({handle});
  }
  g_held_frees.clear();
  frees.clear();
  for (const Event& e : due) {
    switch (e.kind) {
      case Event::kTextureCreated:
        if (auto it = std::find(frees.begin(), frees.end(), e.handle); it != frees.end()) {
          frees.erase(it);
          backend.TextureDestroyed({e.handle});
        }
        backend.TextureCreated({e.handle});
        break;
      case Event::kTextureWritten:
        backend.TextureWritten({e.handle});
        break;
      case Event::kTextureDestroyed:
        frees.push_back(e.handle);
        break;
    }
  }
  g_held_frees.swap(frees);
}

}  // namespace render
