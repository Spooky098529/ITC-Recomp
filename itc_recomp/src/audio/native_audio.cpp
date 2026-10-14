// Our own audio output (docs/native-audio.md).
//
// The game's XAudio mixes on the CPU (guest code) and hands 6-channel frames
// to a render driver. The SDK's driver runs the mixer on its Audio Worker
// thread, whose wait on 9 handles polls (WaitMultiple's trylock loop) and
// took ~80% of a core. Here the game's driver object calls ours instead:
// one guest thread runs each client's mixer whenever SDL has played a
// frame, waiting on a condition variable in between. The frames are
// converted for SDL as the SDK's driver does (same downmix and settings).
// The SDK's XMA decoder is kept.
//
// --native_audio=false brings back the SDK's driver, for comparison.

#include "audio/native_audio.h"

#include "guest.h"

#include <rex/audio/audio_driver.h>
#include <rex/audio/audio_system.h>
#include <rex/audio/conversion.h>
#include <rex/audio/downmix.h>
#include <rex/audio/flags.h>
#include <rex/audio/xma/decoder.h>
#include <rex/cvar.h>
#include <rex/logging.h>
#include <rex/system/function_dispatcher.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xthread.h>
#include <rex/thread.h>

#include <SDL3/SDL.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <deque>
#include <memory>
#include <mutex>
#include <vector>

REXCVAR_DEFINE_BOOL(native_audio, true, "ITC",
                    "Our own audio output instead of the SDK's render driver and Audio Worker");

REXCVAR_DECLARE(int32_t, audio_maxqframes);

namespace native_audio {
namespace {

using rex::X_STATUS;

// A frame as the game's XAudio mixes it: 256 samples a channel, 6 channels
// one after another, big-endian floats, at 48 kHz.
constexpr uint32_t kFrequency = 48000;
constexpr uint32_t kChannels = 6;
constexpr uint32_t kChannelSamples = 256;
constexpr uint32_t kFrameSamples = kChannels * kChannelSamples;

// Driver handles we give the game (kept at its driver object's +16).
constexpr uint32_t kHandleBase = 0x49540000;  // 'IT'
constexpr size_t kMaxClients = 8;

using Frame = std::array<float, kFrameSamples>;

class ItcAudioSystem;
ItcAudioSystem* g_system = nullptr;

struct Client {
  bool in_use = false;
  uint32_t callback = 0;
  // Guest word holding the callback's argument; the callback gets its
  // address, as from the SDK's driver.
  uint32_t wrapped_arg = 0;
  // Mixer calls allowed before SDL plays another frame (the SDK's
  // semaphore count).
  uint32_t credits = 0;
  std::deque<std::unique_ptr<Frame>> queued;
  std::vector<std::unique_ptr<Frame>> unused;
  SDL_AudioStream* stream = nullptr;
  int device_channels = 2;
  // Diagnostics, logged every 10 s.
  uint64_t frames_played = 0;
  uint64_t silence_fills = 0;
  uint64_t logged_frames_played = 0;
  uint64_t logged_silence_fills = 0;
  std::chrono::steady_clock::time_point logged_at = {};
};

class ItcAudioSystem : public rex::audio::AudioSystem {
 public:
  explicit ItcAudioSystem(rex::runtime::FunctionDispatcher* function_dispatcher)
      : AudioSystem(function_dispatcher) {
    queued_frames_limit_ = uint32_t(std::clamp<int32_t>(REXCVAR_GET(audio_maxqframes), 4, 64));
  }

  ~ItcAudioSystem() override {
    if (g_system == this) {
      g_system = nullptr;
    }
  }

  static std::unique_ptr<rex::system::IAudioSystem> Create(
      rex::runtime::FunctionDispatcher* function_dispatcher) {
    return std::make_unique<ItcAudioSystem>(function_dispatcher);
  }

  // Only the XMA decoder; the base class would also start the Audio Worker.
  X_STATUS Setup(rex::system::KernelState* kernel_state) override {
    X_STATUS result = xma_decoder_->Setup(kernel_state);
    if (result) {
      return result;
    }
    running_ = true;
    thread_ = rex::system::object_ref<rex::system::XHostThread>(new rex::system::XHostThread(
        kernel_state, 128 * 1024, 0, [this]() {
          ThreadMain();
          return 0;
        }));
    thread_->set_name("ITC Audio");
    thread_->Create();
    g_system = this;
    REXLOG_INFO("native audio: started ({} frames queued at most)", queued_frames_limit_);
    return X_STATUS_SUCCESS;
  }

  void Shutdown() override {
    if (!running_) {
      return;
    }
    if (xma_decoder_) {
      xma_decoder_->Shutdown();
    }
    {
      std::lock_guard<std::mutex> lock(mutex_);
      running_ = false;
    }
    wake_.notify_all();
    if (thread_) {
      // The thread may be inside the game's mixer; give it time to return
      // before terminating, as the SDK's worker does.
      rex::thread::Thread* host_thread = thread_->thread();
      bool exited = host_thread && rex::thread::Wait(host_thread, false,
                                                     std::chrono::milliseconds(500)) ==
                                       rex::thread::WaitResult::kSuccess;
      if (!exited) {
        REXLOG_WARN("native audio: thread didn't exit within 500 ms; terminating");
        thread_->Terminate(0);
      }
      thread_.reset();
    }
    for (size_t i = 0; i < kMaxClients; i++) {
      if (clients_[i].in_use) {
        Unregister(kHandleBase | uint32_t(i));
      }
    }
  }

  // Returns the driver handle, or 0.
  uint32_t Register(uint32_t callback, uint32_t callback_arg) {
    size_t index = kMaxClients;
    {
      std::lock_guard<std::mutex> lock(mutex_);
      for (size_t i = 0; i < kMaxClients; i++) {
        if (!clients_[i].in_use) {
          index = i;
          break;
        }
      }
    }
    if (index == kMaxClients) {
      REXLOG_ERROR("native audio: no free client for callback {:08X}", callback);
      return 0;
    }
    Client& client = clients_[index];
    SDL_AudioStream* stream = nullptr;
    int device_channels = 2;
    if (!OpenStream(index, &stream, &device_channels)) {
      return 0;
    }
    uint32_t wrapped_arg = memory()->SystemHeapAlloc(4);
    rex::memory::store_and_swap<uint32_t>(memory()->TranslateVirtual(wrapped_arg), callback_arg);
    {
      std::lock_guard<std::mutex> lock(mutex_);
      client = {};
      client.in_use = true;
      client.callback = callback;
      client.wrapped_arg = wrapped_arg;
      client.credits = queued_frames_limit_;
      client.stream = stream;
      client.device_channels = device_channels;
    }
    REXLOG_INFO("native audio: client {} registered: callback {:08X}, argument {:08X}", index,
                callback, callback_arg);
    // SDL starts asking for frames once the device is resumed.
    SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(stream));
    wake_.notify_all();
    return kHandleBase | uint32_t(index);
  }

  void Unregister(uint32_t handle) {
    size_t index = handle & 0xFFFF;
    if ((handle & 0xFFFF0000) != kHandleBase || index >= kMaxClients) {
      REXLOG_ERROR("native audio: unregistering unknown driver handle {:08X}", handle);
      return;
    }
    // Destroying the stream stops its SDL callback; do it outside our lock,
    // which that callback takes.
    SDL_AudioStream* stream;
    {
      std::lock_guard<std::mutex> lock(mutex_);
      stream = clients_[index].stream;
      clients_[index].stream = nullptr;
    }
    if (stream) {
      SDL_DestroyAudioStream(stream);
      SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
    uint32_t wrapped_arg;
    {
      std::lock_guard<std::mutex> lock(mutex_);
      Client& client = clients_[index];
      REXLOG_INFO("native audio: client {} unregistered ({} frames played, {} silence fills)",
                  index, client.frames_played, client.silence_fills);
      wrapped_arg = client.wrapped_arg;
      client = {};
    }
    if (wrapped_arg) {
      memory()->SystemHeapFree(wrapped_arg);
    }
  }

  void Submit(uint32_t handle, uint32_t samples) {
    size_t index = handle & 0xFFFF;
    if ((handle & 0xFFFF0000) != kHandleBase || index >= kMaxClients) {
      REXLOG_ERROR("native audio: frame for unknown driver handle {:08X}", handle);
      return;
    }
    const float* input = memory()->TranslateVirtual<const float*>(samples);
    std::lock_guard<std::mutex> lock(mutex_);
    Client& client = clients_[index];
    if (!client.in_use) {
      return;
    }
    std::unique_ptr<Frame> frame;
    if (client.unused.empty()) {
      frame = std::make_unique<Frame>();
    } else {
      frame = std::move(client.unused.back());
      client.unused.pop_back();
    }
    std::memcpy(frame->data(), input, sizeof(Frame));
    client.queued.push_back(std::move(frame));
  }

 protected:
  // Not used: the game's driver object calls Register, not the kernel's
  // XAudioRegisterRenderDriverClient.
  X_STATUS CreateDriver(size_t index, rex::thread::Semaphore*,
                        rex::audio::AudioDriver**) override {
    REXLOG_ERROR("native audio: the SDK's render driver was asked for (client {})", index);
    return X_STATUS_NOT_IMPLEMENTED;
  }
  void DestroyDriver(rex::audio::AudioDriver*) override {}

 private:
  // Runs each client's mixer while it has credits. The mixer submits its
  // frame (Submit) before returning.
  void ThreadMain() {
    auto* thread_state = thread_->thread_state();
    std::unique_lock<std::mutex> lock(mutex_);
    while (running_) {
      size_t index = kMaxClients;
      for (size_t i = 0; i < kMaxClients; i++) {
        if (clients_[i].in_use && clients_[i].credits) {
          index = i;
          break;
        }
      }
      if (index == kMaxClients) {
        wake_.wait(lock);
        continue;
      }
      Client& client = clients_[index];
      client.credits--;
      uint32_t callback = client.callback;
      uint64_t args[] = {client.wrapped_arg};
      lock.unlock();
      function_dispatcher()->Execute(thread_state, callback, args, 1);
      lock.lock();
    }
  }

  bool OpenStream(size_t index, SDL_AudioStream** out_stream, int* out_channels) {
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
      REXLOG_ERROR("native audio: SDL_InitSubSystem(SDL_INIT_AUDIO) failed: {}", SDL_GetError());
      return false;
    }
    SDL_AudioSpec spec = {};
    spec.freq = kFrequency;
    spec.format = SDL_AUDIO_F32LE;
    spec.channels = kChannels;
    auto* userdata = reinterpret_cast<void*>(index);
    SDL_AudioStream* stream =
        SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, StreamCallback, userdata);
    SDL_AudioSpec device_spec = {};
    if (stream && !SDL_GetAudioDeviceFormat(SDL_GetAudioStreamDevice(stream), &device_spec,
                                            nullptr)) {
      device_spec = spec;
    }
    // As the SDK's driver: stereo (or mono) devices get our stereo fold.
    if (stream && device_spec.channels <= 2) {
      SDL_DestroyAudioStream(stream);
      spec.channels = 2;
      stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, StreamCallback,
                                         userdata);
    }
    if (!stream) {
      REXLOG_ERROR("native audio: SDL_OpenAudioDeviceStream failed: {}", SDL_GetError());
      SDL_QuitSubSystem(SDL_INIT_AUDIO);
      return false;
    }
    const char* name = SDL_GetAudioDeviceName(SDL_GetAudioStreamDevice(stream));
    REXLOG_INFO("native audio: endpoint '{}': {} ch, {} Hz; submitting {} ch", name ? name : "?",
                device_spec.channels, device_spec.freq, spec.channels);
    *out_stream = stream;
    *out_channels = spec.channels;
    return true;
  }

  // SDL's audio thread: plays queued frames (silence when there are none)
  // and gives the mixer a credit for each frame played.
  static void StreamCallback(void* userdata, SDL_AudioStream* stream, int additional_amount,
                             int) {
    ItcAudioSystem* self = g_system;
    if (!self) {
      return;
    }
    size_t index = reinterpret_cast<size_t>(userdata);
    const rex::audio::StereoFold fold = rex::audio::GetStereoFold();
    const rex::audio::SurroundMix mix = rex::audio::GetSurroundMix();
    const float gain = rex::audio::GetOutputGain();
    std::array<float, kFrameSamples> data;
    bool played = false;
    {
      std::lock_guard<std::mutex> lock(self->mutex_);
      Client& client = self->clients_[index];
      const int len = int(sizeof(float)) * int(kChannelSamples) * client.device_channels;
      while (additional_amount > 0) {
        if (client.queued.empty()) {
          std::memset(data.data(), 0, len);
          client.silence_fills++;
        } else {
          std::unique_ptr<Frame> frame = std::move(client.queued.front());
          client.queued.pop_front();
          if (REXCVAR_GET(audio_mute)) {
            std::memset(data.data(), 0, len);
          } else if (client.device_channels == 6) {
            rex::audio::conversion::sequential_6_BE_to_interleaved_6_LE(
                data.data(), frame->data(), kChannelSamples, mix, gain);
          } else {
            rex::audio::conversion::sequential_6_BE_to_interleaved_2_LE(
                data.data(), frame->data(), kChannelSamples, fold, gain);
          }
          client.unused.push_back(std::move(frame));
          client.credits++;
          client.frames_played++;
          played = true;
        }
        if (!SDL_PutAudioStreamData(stream, data.data(), len)) {
          REXLOG_ERROR("native audio: SDL_PutAudioStreamData failed: {}", SDL_GetError());
          break;
        }
        additional_amount -= len;
      }
      // Silence fills are frames the mixer didn't have ready in time: heard
      // as gaps or crackle.
      auto now = std::chrono::steady_clock::now();
      if (client.logged_at == std::chrono::steady_clock::time_point{}) {
        client.logged_at = now;
      } else if (now - client.logged_at >= std::chrono::seconds(10)) {
        REXLOG_INFO("native audio: client {}: {} frames played, {} silence fills in {:.1f} s; {} "
                    "queued",
                    index, client.frames_played - client.logged_frames_played,
                    client.silence_fills - client.logged_silence_fills,
                    std::chrono::duration<double>(now - client.logged_at).count(),
                    client.queued.size());
        client.logged_frames_played = client.frames_played;
        client.logged_silence_fills = client.silence_fills;
        client.logged_at = now;
      }
    }
    if (played) {
      self->wake_.notify_one();
    }
  }

  uint32_t queued_frames_limit_ = 8;
  rex::system::object_ref<rex::system::XHostThread> thread_;
  bool running_ = false;
  std::mutex mutex_;
  std::condition_variable wake_;
  std::array<Client, kMaxClients> clients_;
};

bool IsOurHandle(uint32_t handle) {
  return (handle & 0xFFFF0000) == kHandleBase;
}

}  // namespace

void Configure(rex::RuntimeConfig& config) {
  if (REXCVAR_GET(native_audio)) {
    config.audio_factory = REX_AUDIO_BACKEND(ItcAudioSystem);
  }
}

}  // namespace native_audio

using native_audio::g_system;

// The game's render driver object (XAudio's): r3 = object; the callback's
// argument at +12, the driver handle at +16.

// Register: r4 = the mixer callback (0 only unregisters). Unregisters any
// earlier handle first. Returns 0 (S_OK).
extern "C" REX_FUNC(sub_824657E0) {
  REX_FUNC_PROLOGUE();
  if (!g_system) {
    __imp__sub_824657E0(ctx, base);
    return;
  }
  uint32_t object = ctx.r3.u32;
  uint32_t callback = ctx.r4.u32;
  uint32_t handle = guest::LoadU32(base, object + 16);
  if (handle) {
    g_system->Unregister(handle);
    guest::StoreU32(base, object + 16, 0);
  }
  ctx.r3.u64 = 0;
  if (callback) {
    handle = g_system->Register(callback, guest::LoadU32(base, object + 12));
    guest::StoreU32(base, object + 16, handle);
    if (!handle) {
      ctx.r3.u64 = uint32_t(0x80004005);  // E_FAIL
    }
  }
}

// Submit: r4 = the mixed buffer; sub_8245F718 gives its descriptor, whose
// third word (+8) is the samples' address. Returns 0, or the descriptor's
// failure.
extern "C" REX_FUNC(sub_82465870) {
  REX_FUNC_PROLOGUE();
  if (!g_system) {
    __imp__sub_82465870(ctx, base);
    return;
  }
  uint32_t object = ctx.r3.u32;
  guest::StackFrame frame(ctx, 112);
  ctx.r3.u64 = ctx.r4.u64;
  ctx.r4.u64 = frame.Local(80);
  sub_8245F718(ctx, base);
  if (ctx.r3.s32 < 0) {
    return;
  }
  g_system->Submit(guest::LoadU32(base, object + 16), guest::LoadU32(base, frame.Local(88)));
  ctx.r3.u64 = 0;
}

// Destructor: unregisters the handle at +16 if it's ours, so the original
// (which would call the kernel's XAudioUnregisterRenderDriverClient) sees
// none.
extern "C" REX_FUNC(sub_824658C8) {
  REX_FUNC_PROLOGUE();
  uint32_t object = ctx.r3.u32;
  uint32_t handle = guest::LoadU32(base, object + 16);
  if (g_system && native_audio::IsOurHandle(handle)) {
    g_system->Unregister(handle);
    guest::StoreU32(base, object + 16, 0);
  }
  __imp__sub_824658C8(ctx, base);
}
