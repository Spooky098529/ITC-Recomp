// Gamepads read with SDL (see gamepads.h and docs/rexglue-removal.md, A5).
//
// Threads: SDL calls the event watch on whichever thread pumps events (the
// UI thread here, possibly SDL's own joystick thread on some platforms), with
// SDL's joystick lock held. The watch only updates the slots under our mutex
// and never calls SDL. Everything else calls SDL only without our mutex held,
// so the two locks are never taken in the other order (the deadlock the
// SDK's driver comments on). Pads are opened and closed on the UI thread,
// outside the watch, queued by the watch when one connects or disconnects.

#include "gamepads.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <SDL3/SDL.h>

#include <atomic>
#include <cstring>
#include <mutex>
#include <vector>

REXCVAR_DEFINE_BOOL(log_gamepads, false, "ITC",
                    "Log our gamepad reading's button changes (src/input/gamepads.cpp)");

namespace input {
namespace {

// SDL clamps to SDL_MAX_RUMBLE_DURATION_MS, which is not a public constant;
// rumble is held until the next call, as XInput's is.
constexpr uint32_t kRumbleDurationMs = 0xFFFF;

struct Slot {
  SDL_JoystickID id = 0;       // 0: empty
  SDL_Gamepad* pad = nullptr;  // ours from SDL_OpenGamepad, once opened
  GamepadState state = {};
};

std::mutex g_mutex;
Slot g_slots[kGamepadSlots];
std::vector<SDL_Gamepad*> g_to_close;  // unplugged while open
bool g_started = false;

UiThreadQueue g_queue;
std::atomic<bool> g_pump_queued{false};
std::atomic<bool> g_open_queued{false};

int SlotOf(SDL_JoystickID id) {
  for (int i = 0; i < kGamepadSlots; i++) {
    if (g_slots[i].id == id) {
      return i;
    }
  }
  return -1;
}

// With g_mutex held. The lowest free slot, or -1 (a fifth pad isn't used).
int Assign(SDL_JoystickID id) {
  int slot = SlotOf(id);
  if (slot >= 0) {
    return slot;
  }
  for (int i = 0; i < kGamepadSlots; i++) {
    if (!g_slots[i].id) {
      g_slots[i] = {};
      g_slots[i].id = id;
      g_slots[i].state.changes = 1;
      return i;
    }
  }
  return -1;
}

void OpenPending();

void QueueOpen() {
  if (!g_open_queued.exchange(true)) {
    g_queue([] {
      g_open_queued = false;
      OpenPending();
    });
  }
}

// UI thread: opens newly assigned pads and closes unplugged ones.
void OpenPending() {
  std::vector<SDL_JoystickID> to_open;
  std::vector<SDL_Gamepad*> to_close;
  {
    std::lock_guard lock(g_mutex);
    if (!g_started) {
      return;
    }
    for (const Slot& slot : g_slots) {
      if (slot.id && !slot.pad) {
        to_open.push_back(slot.id);
      }
    }
    to_close.swap(g_to_close);
  }
  for (SDL_Gamepad* pad : to_close) {
    SDL_CloseGamepad(pad);
  }
  for (SDL_JoystickID id : to_open) {
    SDL_Gamepad* pad = SDL_OpenGamepad(id);
    if (!pad) {
      REXLOG_WARN("gamepads: couldn't open SDL gamepad {}: {}", id, SDL_GetError());
      continue;
    }
    int slot;
    {
      std::lock_guard lock(g_mutex);
      slot = SlotOf(id);
      if (slot >= 0) {
        g_slots[slot].pad = pad;
      }
    }
    if (slot < 0) {
      // Unplugged in the meantime.
      SDL_CloseGamepad(pad);
      continue;
    }
    const char* name = SDL_GetGamepadName(pad);
    REXLOG_INFO("gamepads: \"{}\" in slot {} (SDL gamepad {}, vendor 0x{:04X}, product 0x{:04X})",
                name ? name : "", slot, id, SDL_GetGamepadVendor(pad), SDL_GetGamepadProduct(pad));
  }
}

bool OnEvent(void*, SDL_Event* event) {
  int slot = -1;
  uint32_t logged_buttons = 0;
  bool log_buttons = false;
  bool queue_open = false;
  {
    std::lock_guard lock(g_mutex);
    if (!g_started) {
      return true;
    }
    switch (event->type) {
      case SDL_EVENT_GAMEPAD_ADDED:
        slot = Assign(event->gdevice.which);
        queue_open = slot >= 0;
        break;
      case SDL_EVENT_GAMEPAD_REMOVED:
        slot = SlotOf(event->gdevice.which);
        if (slot >= 0) {
          if (g_slots[slot].pad) {
            g_to_close.push_back(g_slots[slot].pad);
          }
          g_slots[slot] = {};
          queue_open = true;
        }
        break;
      case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        slot = SlotOf(event->gaxis.which);
        if (slot >= 0 && event->gaxis.axis < SDL_GAMEPAD_AXIS_COUNT) {
          g_slots[slot].state.axes[event->gaxis.axis] = event->gaxis.value;
          g_slots[slot].state.changes++;
        }
        break;
      case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
      case SDL_EVENT_GAMEPAD_BUTTON_UP:
        slot = SlotOf(event->gbutton.which);
        if (slot >= 0 && event->gbutton.button < 32) {
          uint32_t bit = 1u << event->gbutton.button;
          uint32_t& buttons = g_slots[slot].state.buttons;
          buttons = event->gbutton.down ? buttons | bit : buttons & ~bit;
          g_slots[slot].state.changes++;
          logged_buttons = buttons;
          log_buttons = REXCVAR_GET(log_gamepads);
        }
        break;
      default:
        break;
    }
  }
  if (event->type == SDL_EVENT_GAMEPAD_REMOVED && slot >= 0) {
    REXLOG_INFO("gamepads: slot {} unplugged (SDL gamepad {})", slot, event->gdevice.which);
  }
  if (log_buttons) {
    REXLOG_INFO("gamepads: slot {} buttons {:08X}", slot, logged_buttons);
  }
  if (queue_open) {
    QueueOpen();
  }
  return true;
}

void QueuePump() {
  if (!g_pump_queued.exchange(true)) {
    g_queue([] {
      SDL_PumpEvents();
      g_pump_queued = false;
    });
  }
}

}  // namespace

void Gamepads_Start(UiThreadQueue queue) {
  if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD)) {
    REXLOG_ERROR("gamepads: SDL's gamepad subsystem failed to start: {}", SDL_GetError());
    return;
  }
  g_queue = std::move(queue);
  {
    std::lock_guard lock(g_mutex);
    g_started = true;
  }
  SDL_AddEventWatch(OnEvent, nullptr);
  // Pads already connected sent their SDL_EVENT_GAMEPAD_ADDED before the
  // watch existed. Assign skips any the watch has seen since.
  int count = 0;
  SDL_JoystickID* ids = SDL_GetGamepads(&count);
  {
    std::lock_guard lock(g_mutex);
    for (int i = 0; i < count; i++) {
      Assign(ids[i]);
    }
  }
  SDL_free(ids);
  REXLOG_INFO("gamepads: started, {} connected", count);
  QueueOpen();
}

void Gamepads_Stop() {
  std::vector<SDL_Gamepad*> to_close;
  {
    std::lock_guard lock(g_mutex);
    if (!g_started) {
      return;
    }
    g_started = false;
    for (Slot& slot : g_slots) {
      if (slot.pad) {
        to_close.push_back(slot.pad);
      }
      slot = {};
    }
    to_close.insert(to_close.end(), g_to_close.begin(), g_to_close.end());
    g_to_close.clear();
  }
  SDL_RemoveEventWatch(OnEvent, nullptr);
  for (SDL_Gamepad* pad : to_close) {
    SDL_CloseGamepad(pad);
  }
  SDL_QuitSubSystem(SDL_INIT_GAMEPAD);
}

bool Gamepads_Read(int slot, GamepadState* state) {
  if (slot < 0 || slot >= kGamepadSlots) {
    return false;
  }
  bool started;
  bool connected;
  {
    std::lock_guard lock(g_mutex);
    started = g_started;
    connected = started && g_slots[slot].id;
    if (connected) {
      *state = g_slots[slot].state;
    }
  }
  if (started) {
    QueuePump();
  }
  return connected;
}

bool Gamepads_Rumble(int slot, uint16_t low, uint16_t high) {
  if (slot < 0 || slot >= kGamepadSlots) {
    return false;
  }
  SDL_JoystickID id;
  {
    std::lock_guard lock(g_mutex);
    id = g_slots[slot].pad ? g_slots[slot].id : 0;
  }
  if (!id) {
    return false;
  }
  QueuePump();
  // Looked up again by its id: if it was closed since, SDL finds nothing.
  SDL_Gamepad* pad = SDL_GetGamepadFromID(id);
  return pad && SDL_RumbleGamepad(pad, low, high, kRumbleDurationMs);
}

}  // namespace input
