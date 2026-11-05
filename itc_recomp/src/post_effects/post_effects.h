// The post effects natively (docs/functions/post-effects.md): the 15
// effects at 0x826C5D80 (PostEffects::), the glow's and the color filter's
// setters, and the race's toggle over them. Plain C++ over guest memory
// (the effects are shared with recompiled code); the calls into code that
// still takes the context (the effects' vtable slots) go through the
// callbacks the adapters (src/hooks/post_effects.cpp) give.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace post_effects {

constexpr uint32_t kEffects = 0x826C5D80;  // 15 effect pointers
constexpr uint32_t kCount = 15;

// PostEffects::Get (sub_82133AE8): [kEffects + 4 * index] (the address
// wraps at 32 bits, as the original's).
uint32_t Get(uint8_t* base, uint32_t index);
// PostEffects::MarkUpdated (sub_82133A08): +48 = 1 on each effect.
void MarkUpdated(uint8_t* base);
// PostEffects::ToggleNoHistory (sub_82133A38): +44 = !+44 on each effect.
void ToggleNoHistory(uint8_t* base);
// ScreenObject::ToggleHidden (sub_8212EDE0): byte +25 = !+25.
void ToggleScreenObjectHidden(uint8_t* base, uint32_t object);
// Race::ToggleEffects (sub_8215B690): r3 = the race object. Returns r3 as
// the original leaves it (the race's +31160).
uint32_t ToggleRaceEffects(uint8_t* base, uint32_t race);
// PostEffects::DestroyRace (sub_82133A70): each effect without flag bit 2
// (+32 & 4) destroyed through its vtable slot 1 (`destroy(function,
// effect)` with r4 = 1, returning r3) and its pointer cleared. Returns r3 as
// the original leaves it.
uint32_t DestroyRace(uint8_t* base, const std::function<uint32_t(uint32_t, uint32_t)>& destroy);

// The glow's setters (vtable slots 2-12, sub_82136EF8 to sub_82136F48): the
// float at `offset` (8436 to 8476) = value, rounded to single.
void GlowSet(uint8_t* base, uint32_t glow, uint32_t offset, double value);
// Glow::SetFromSpeed (sub_82133878, slot 24): t from the speed, then the
// glow's slots 2, 6 and 8 called with their values (`set(function, value)`,
// r3 = the glow).
void GlowSetFromSpeed(uint8_t* base, uint32_t glow, double speed,
                      const std::function<void(uint32_t, double)>& set);

// ColorFilter::SetColor0 (sub_821350F8): +8080, +8084, +8088 = the three
// values, +8092 = 0.
void ColorFilterSetColor0(uint8_t* base, uint32_t filter, double r, double g, double b);
// ColorFilter::Set8112, Set8116, Set8120 (sub_82135138, sub_82135140,
// sub_82135148): the float at `offset` = value.
void ColorFilterSet(uint8_t* base, uint32_t filter, uint32_t offset, double value);


// The effects' recording helpers (r3 = the effect, not read by those that
// take only the list): each writes its slots on the command list `list`
// (command_list::AllocateSlots, without the list's diagnostics) and
// returns r3 as the original leaves it (its last slot), or nothing, with
// nothing written, when the list hasn't room for all its slots (the
// original prints a message there and writes through address 0: left to
// it).
//
// PostEffect::RecordBeginStates (sub_82133340): 24 slots (command 34's
// one left as it was).
std::optional<uint32_t> RecordBeginStates(uint8_t* base, uint32_t list);
// PostEffect::RecordEndStates (sub_82133608): 13 slots (command 35's one
// left as it was).
std::optional<uint32_t> RecordEndStates(uint8_t* base, uint32_t list);
// PostEffect::RecordTargets (sub_82133790): render targets 0-3 = surface,
// the depth surface: 7 slots.
std::optional<uint32_t> RecordTargets(uint8_t* base, uint32_t list, uint32_t surface,
                                      uint32_t depth);
// PostEffect::RecordPixelConstants (sub_82133D00): command 7 (pixel floats,
// `reg`, `count`) and one slot copied from the 16 bytes at `source`.
std::optional<uint32_t> RecordPixelConstants(uint8_t* base, uint32_t list, uint32_t source,
                                             uint32_t reg, uint32_t count);
// PostEffect::RecordPixelBool (sub_82133D70): command 7 (pixel bool, `reg`)
// and 4 bytes holding `value`.
std::optional<uint32_t> RecordPixelBool(uint8_t* base, uint32_t list, uint32_t value, uint32_t reg);
// PostEffect::RecordTexelSize (sub_82133DD8): c32 = (1/width, 1/height, 0,
// 0) through RecordPixelConstants' slots.
std::optional<uint32_t> RecordTexelSize(uint8_t* base, uint32_t list, double width, double height);
// PostEffect::AddQuad (sub_82133E30): three 40-byte vertices in the
// effect's vertex buffers; returns the first. Never fails.
uint32_t AddQuad(uint8_t* base, uint32_t effect, uint32_t width, uint32_t height, double value,
                 bool at_view);
// PostEffect::RecordResolve (sub_82133F58): command 25 and its 68-byte
// item (6 slots).
std::optional<uint32_t> RecordResolve(uint8_t* base, uint32_t effect, uint32_t list,
                                      uint32_t texture, uint32_t flags, uint32_t more_flags,
                                      bool by_view);

// --- The glare's definition (the glow's +8064) ---

// GlareDef::GlareDef (sub_8212ED78): its floats 0 (+12 left as it was),
// its star definition's lines, inclination and rotation 0, the w of its
// eight colors 1.
void GlareDefConstruct(uint8_t* base, uint32_t glare);
// GlareDef::Init (sub_8212EC90): the glare table's entry `type` copied in
// (0x82617280, 44 bytes an entry), then StarDef::Init from the star table
// (0x826171D8, 28 bytes an entry) for its star type. Returns 0.
uint32_t GlareDefInit(uint8_t* base, uint32_t glare, uint32_t type);
// StarDef::Init (sub_8212EA98): +0 = lines, +100 = inclination, +104 =
// rotation, each line's passes, sample length, attenuation and angle, the
// eight colors at +112. Returns 0.
uint32_t StarDefInit(uint8_t* base, uint32_t star, uint32_t lines, uint32_t passes,
                     double sample_length, double attenuation, double inclination,
                     uint32_t rotation);

// --- The render textures and surfaces ---

// RenderTexture::Release (sub_82133070): one user taken from the entry
// (+20) when it has any; at 0, its memory (+28, XPhysicalFree) and header
// (+24, operator delete) freed and both cleared. Returns r3 as the
// original leaves it (the entry, or the header after the frees), or
// nothing, with nothing done, when the header isn't a block of the
// allocation layer's (the original then runs).
std::optional<uint32_t> RenderTextureRelease(uint8_t* base, uint32_t entry);
// RenderTextures::Release (sub_821332F0): RenderTexture::Release on the
// entry at `at`, then an entry of id 20 (its own) deleted and *at = 0.
// Returns r3 (the entry), or nothing as RenderTexture::Release (or for an
// own entry not the layer's).
std::optional<uint32_t> RenderTexturesRelease(uint8_t* base, uint32_t at);

// The guest calls PostEffect::ReleaseSurface makes: the video output
// (`[0x8266D088]`'s vtable slot 7, then that object's slot 3; `at` 0 or 1
// picks the original's return addresses) and Resource::Release.
struct SurfaceCalls {
  std::function<uint32_t(int at)> video_output;
  std::function<void(uint32_t resource)> release;
};
// PostEffect::ReleaseSurface (sub_82133C60): with the device locked
// (VideoOutput::LockDevice's plain C++, the device kept at `frame` + 80, the
// original's frame), Resource::Release(*at) and *at = 0. Returns r3 (the
// video output).
uint32_t ReleaseSurface(uint8_t* base, uint32_t frame, uint32_t at, const SurfaceCalls& calls);

// TargetFilter::TargetFilter (sub_82135AA8): the base vtable, the table
// entry's 36 bytes copied to +4 (memcpy, c_runtime::Memcpy), the fields
// cleared, its vtable, +8064 = Device::CreateSurface(+16, +20, 0x18280186,
// 0, the 12 zero bytes at `frame` + 80). Returns r3 (the filter), or
// nothing, with nothing done, when the copy can't be made natively.
std::optional<uint32_t> TargetFilterConstruct(uint8_t* base, uint32_t frame, uint32_t filter,
                                              uint32_t entry);

// PostEffects::InitEdramBase (sub_825D99D0, probably a static
// initializer): [0x826C5DC0] = Surface::EdramTiles(1024, 576, 0x1A2201BF,
// 1), the EDRAM base the effects' surfaces are given. Returns r3 (the
// tiles).
constexpr uint32_t kEdramBase = 0x826C5DC0;
uint32_t InitEdramBase(uint8_t* base);

// --- Making the effects (create.cpp) ---
//
// Each takes `stack`, the guest r1 at its entry: its frame is where the
// original's is, and the words the original writes there are written there.
// They need the allocation layer (--native_heap).

// RenderTexture::Create (sub_821330D0): the key stored in the entry (+0
// format, +4, +8 the size, +12, +16 the key's words, +32 the id), a 52-byte
// texture header (operator new, +24) set by XGSetTextureHeader, its memory
// (XPhysicalAlloc(size, -1, 0, 0x404), +28) and the header relocated to it.
// Returns 1, or 0 when the memory wasn't made.
uint32_t RenderTextureCreate(uint8_t* base, uint32_t stack, uint32_t entry, uint32_t format,
                             uint32_t width, uint32_t height, uint32_t id, uint32_t key0,
                             uint32_t key1);
// RenderTextures::Acquire (sub_82133190): the render texture for `id` from
// the 20 entries at 0x826C5DC8, made when not in use, shared when the key
// matches; id 20, or one in use with another key, a new entry of its own
// (id 20). Its users +1.
uint32_t RenderTexturesAcquire(uint8_t* base, uint32_t stack, uint32_t id, uint32_t format,
                               uint32_t width, uint32_t height, uint32_t key0, uint32_t key1);
// The constructors from a table entry (r3 the object, r4 the entry):
// Glow::Glow (sub_82138610), ColorFilter::ColorFilter (sub_82135008) and
// MotionBlur::MotionBlur (sub_82136130). Return the object.
uint32_t GlowConstruct(uint8_t* base, uint32_t stack, uint32_t glow, uint32_t entry);
uint32_t ColorFilterConstruct(uint8_t* base, uint32_t stack, uint32_t filter, uint32_t entry);
uint32_t MotionBlurConstruct(uint8_t* base, uint32_t stack, uint32_t blur, uint32_t entry);

// The guest calls PostEffects::Create makes: the video output (as
// SurfaceCalls', `at` 0 before and 1 after) and the recompiled constructors
// of kinds 1 and 4 (never made in play: `construct(function, object, entry,
// device)`, returning r3).
struct CreateCalls {
  std::function<uint32_t(int at)> video_output;
  std::function<uint32_t(uint32_t, uint32_t, uint32_t, uint32_t)> construct;
};
// PostEffects::Create (sub_82138A18): with the device locked when `device`
// is 0, the effect `index` made from its table entry when it isn't there.
// The device word is kept at `stack` + 20 (the caller's frame), as the
// original. Returns r3 as the original leaves it.
uint32_t Create(uint8_t* base, uint32_t stack, uint32_t device, uint32_t index,
                const CreateCalls& calls);

// The guest calls PostEffects::InitBoot makes: the video output (as
// SurfaceCalls', `at` 0 before and 1 after), memcpy where
// CreateVertexDeclaration's plain C++ hands it back, and a frame for
// PostEffects::Create (`in_create_frame(create)` runs `create` with the
// guest calls Create makes, from a frame like the original's, returning its
// r3).
struct InitBootCalls {
  std::function<uint32_t(int at)> video_output;
  std::function<void(uint32_t to, uint32_t from, uint32_t size)> memcpy_call;
  std::function<uint32_t(const std::function<uint32_t(const CreateCalls&)>&)> in_create_frame;
};
// PostEffects::InitBoot (sub_82138C38): with the device locked when
// `device` is 0 (kept at `stack` + 20), the effects' vertex declaration
// [0x826C5DBC] = CreateVertexDeclaration(0x826176C4), then
// PostEffects::Create(device, 14). Returns r3 as the original leaves it.
constexpr uint32_t kVertexDeclaration = 0x826C5DBC;
uint32_t InitBoot(uint8_t* base, uint32_t stack, uint32_t device, const InitBootCalls& calls);

// --- The records (record.cpp) ---

// A record's guest calls: any guest function `function` from the frame at
// `sp` with `lr`, r3 on from `regs` (`count`, all 64 bits), f1 on from
// `fregs` (`fcount`), returning its r3 (the render manager's getters, the
// render queue, CameraPasses::RenderTarget through its diagnostics' wrap);
// tan's original (f1 = x) where c_runtime::Tan can't stand in, returning
// its f1. Both leave flush-to-zero off.
struct RecordCalls {
  std::function<uint64_t(uint32_t function, const uint64_t* regs, int count, const double* fregs,
                         int fcount, uint32_t sp, uint32_t lr)>
      call;
  std::function<double(double x, uint32_t sp, uint32_t lr)> tan;
};
// ColorFilter::Record (sub_821351F0, the color filter's vtable slot 0): its
// chunk on the scene's list (the frame resolved into its texture, c37 to
// c39, b0 and b1, shader pair 31 over the view), handed to the render queue
// with its key (+8); +40 = 0. Returns r3 (the queue's call's), or nothing
// (only the list's getters called) when the list hasn't room for its 75
// slots or a chunk is open.
std::optional<uint32_t> ColorFilterRecord(uint8_t* base, uint32_t stack, uint32_t filter,
                                          const RecordCalls& calls);
// TargetFilter::Record (sub_82135B50, the target filter's vtable slot 0):
// by its flags (+32) render target 4 (bit 0, camera pass 12 active) or 3
// (bit 1, pass 11 active), drawn into +8064 and resolved back twice (shader
// pairs 24 and 25, or 28 and 29), handed on with its key; +40 = 0. Returns
// r3 (the filter when nothing's recorded, 0 without a target, else the
// queue's call's), or nothing (CameraPasses::RenderTarget and the list's
// getters called) when the list hasn't room for its 91 slots or a chunk is
// open.
std::optional<uint32_t> TargetFilterRecord(uint8_t* base, uint32_t stack, uint32_t filter,
                                           const RecordCalls& calls);

// MotionBlur::Record (sub_821363F0, the motion blur's vtable slot 0): its
// vertex c100 x4 upload chunk
// (its previous view-projection, the copy frame interpolation notes:
// interpolation::BlendState_MotionBlurCopy), the pair moved on (+8144 =
// +8080, +8080 = its camera pass's +144) on logic passes, then the blur's
// chunk (velocities into +8072 with shader pair 32, resolved into +8064's
// texture, the frame into +8068's, shader pair 33 over the view with c40
// and c41); its four chunks handed on; +40 = 0. Returns r3 (the last
// queue call's), or nothing (the first getters called) when the list
// hasn't room for its 131 slots or a chunk is open.
std::optional<uint32_t> MotionBlurRecord(uint8_t* base, uint32_t stack, uint32_t blur,
                                         const RecordCalls& calls);

// Glow::RecordStarPass (sub_82134060): one pass of a star line, into the
// glow's chunk already open: `samples` (8 from Glow::RecordStar) offsets
// i x (dx, dy) and weights color_i x attenuation^(i x step) x (pass + 1) x
// the constant at 0x82008834 (zeroed with the offsets where an offset is
// past the constant at 0x82002A2C) in two 16-register blocks built in its
// frame (`stack` - 768, the words it doesn't write left as they were) and
// copied as c0 and c16, `texture` as sampler 0, shader pair 17 over a
// quarter-size quad, resolved into `dest` (flags 256, -3). `colors` holds
// the samples' colors (16 bytes each). The caller has made sure the list
// has room for its 48 slots. Returns r3 (RecordResolve's).
uint32_t GlowRecordStarPass(uint8_t* base, uint32_t stack, uint32_t glow, uint32_t texture,
                            uint32_t samples, double attenuation, double step, double dx,
                            double dy, uint32_t colors, uint32_t pass, uint32_t dest,
                            const RecordCalls& calls);
// Glow::RecordStar (sub_82136FA8, from Glow::Record inside its chunk): 280
// bytes of constants, the 16 samples' colors (the star's eight, two passes'
// chromatic aberration) in its frame (`stack` - 672), the star's turn from
// the camera pass's view when it's active (Matrix::Inverse,
// Vec4::Transform, the normalize sequence, acos), each line's passes
// (Glow::RecordStarPass), then the lines merged into +8384's texture with
// shader pairs 18, 23 and 22. Returns r3 (its last slot), or nothing (only
// the list's getters called) when the list hasn't room for its 80 + 48 per
// pass slots, a line has more than 2 passes (the original reads its colors
// past the 16 it builds, from its register save area) or the star has more
// than 64 lines (never in the game: 4 lines of 2 passes).
std::optional<uint32_t> GlowRecordStar(uint8_t* base, uint32_t stack, uint32_t glow,
                                       const RecordCalls& calls);

// --- The destructors ---

// The calls the destructors make, each returning r3 as it leaves it:
// PostEffect::ReleaseSurface and RenderTextures::Release on a field's
// address (the adapters' paths: native or the original).
struct DestroyCalls {
  std::function<uint32_t(uint32_t at)> release_surface;
  std::function<uint32_t(uint32_t at)> release_texture;
};
// Glow::~Glow (sub_82136E30): its vtable, its six surfaces (+8432 down to
// +8412) and 15 render textures (+8408 down to +8352) released, the base
// vtable. Returns r3 (the last release's).
uint32_t GlowDestroy(uint8_t* base, uint32_t glow, const DestroyCalls& calls);
// ColorFilter::~ColorFilter (sub_821351A8): its render texture (+8064).
uint32_t ColorFilterDestroy(uint8_t* base, uint32_t filter, const DestroyCalls& calls);
// MotionBlur::~MotionBlur (sub_82136390): its surfaces (+8076, +8072) and
// render textures (+8068, +8064).
uint32_t MotionBlurDestroy(uint8_t* base, uint32_t blur, const DestroyCalls& calls);

enum class EffectKind { kGlow, kColorFilter, kMotionBlur };
// The deleting destructors (vtable slot 1: sub_821389C8, sub_82138570,
// sub_821385C0): the destructor, then operator delete of the effect when
// `flags` bit 0 is set. Returns r3 (the effect), or nothing, with nothing
// done, when the effect to delete isn't a block of the layer's.
std::optional<uint32_t> DeleteEffect(uint8_t* base, EffectKind kind, uint32_t effect,
                                     uint32_t flags, const DestroyCalls& calls);

}  // namespace post_effects
