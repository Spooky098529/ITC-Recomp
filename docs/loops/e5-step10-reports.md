# E5 step 10: the swarm's reports

Run `wf_4fc71676-c70` (2026-11-25), `itc_recomp/tools/workflows/e5-step10-swarm.js`;
see `docs/loops/e5-loop.md`, "Step 10 as one swarm". 8 units, the 39 files the
compiler still rejected after the codemod dropping `base` and the lead's
scripted fixes; each agent fixed its files by reading. The agents' reports,
as written.

## unit1

- `itc_recomp/src/hooks/ui_movies_game.cpp`: CallFrom's function pointer parameter kept an unnamed uint8_t* (`void (*)(lifted::Regs&, uint8_t*)`), so `function(ctx)` and every CallFrom / from_unload / call use (ITC_REGS_FN gives `void (*)(lifted::Regs&)`) failed; the parameter is now `void (*function)(lifted::Regs&)`, which fixes all 19 listed errors
- `itc_recomp/src/sprites/billboard_record.cpp`: Writer's accessor `uint8_t* base() const { return base_; }` returned the removed member; deleted it (nothing calls it)
- `itc_recomp/src/race_world/race_event.cpp`: ListIndex built the view as `GroupEntry(nullptr, address)` (a null base the codemod didn't match); now `GroupEntry(entries + GroupEntry::kSize * i)`, the explicit address constructor
- `itc_recomp/src/network_mode/machines.cpp`: OpenAdapter<I> kept an unnamed `uint8_t*` first parameter, so it no longer matched OpenFn and Screen1OpenMovie in the ternary; the parameter is dropped
- `itc_recomp/src/native/textures.cpp`: GatherTiledRow's `PhysicalHostAddress(const_cast<uint8_t*>(), address)` (the codemod emptied the cast of base) is now `PhysicalHostAddress(address)`, matching state.h's one-argument signature

**For the lead:** none. The only other local named base in these files is textures.cpp's `bool base` parameter in the `strides` lambda, which the codemod left alone. I didn't build anything. I read the rest of each file for the same patterns: emptied casts or calls, nullptr bases, unnamed uint8_t* parameters, and base members. I found none.

## unit2

- `itc_recomp/src/hooks/network_mode.cpp`: ITC_NET_MACHINE's explicit instantiations still named the old (lifted::Regs&, uint8_t*) signature, which no longer matched the templates. They now use (lifted::Regs&), and the macro's backslashes are realigned. This covers all 13 instantiations (0, 1, 3-13), not only the four indexes in the error list.
- `itc_recomp/src/threads/tls.cpp`: TlsFree kept an unnamed uint8_t* parameter (TlsFree(uint8_t*, const lifted::Args&)). The parameter is dropped, so it fits NativeImport (Result(*)(const Args&)). That also clears the std::size(kEntries) error that followed from the bad initializer.
- `itc_recomp/src/alt_hud/alt_hud.cpp`: Holder() returned the view by copy-list-initialization ({holder}), which the explicit View(uint32_t) constructor no longer allows. It now returns schema::AltHudHolder(holder). No other braced view returns or initializations are in the file.
- Left `itc_recomp/src/hooks/strings_and_font.cpp:324`: lifted::Sdk(ctx, hooks::g_text_draw_stand_in) fails because hooks::GuestStandIn, declared in itc_recomp/src/hooks/strings_and_font.h (not one of my files), is still void (*)(lifted::Regs&, uint8_t*). The call is already right as it stands: once the header's type is fixed it resolves to Sdk(Regs&, RegsFunction*). No change is needed in the .cpp.

**For the lead:** In itc_recomp/src/hooks/strings_and_font.h line 15, change `using GuestStandIn = void (*)(lifted::Regs&, uint8_t*);` to `using GuestStandIn = void (*)(lifted::Regs&);`. That fixes the strings_and_font.cpp:324 error. Its only other user is src/harnesses/ui_movies_game.cpp, which isn't built since stage 5. Nothing else in the four files still holds base: no members, locals, emptied calls or captures. I didn't build anything, per the swarm's rules.

## unit3

- `itc_recomp/src/hooks/title_screen_native.cpp`: ModeType4MachineOf: dropped the leftover `(void)base;` (line 360).
- `itc_recomp/src/hooks/title_screen_native.cpp`: ITC_TITLE_LOAD_DEBUG's explicit instantiations still named `(lifted::Regs&, uint8_t*)`: now `template void name<index>(lifted::Regs&);`, matching the templates and title_screen_native.h (fixes lines 392-407).
- `itc_recomp/src/save_data/save_data.cpp`: SetDefaultsNative: the three `Destination(nullptr, save, kCopy...)` calls (the old null base) are now `Destination(save, kCopy...)` (lines 252-256).
- `itc_recomp/src/collision/shapes.cpp`: BoxCorner and BoxFace: `Box(nullptr, box)` is now `Box(box)`, the view's explicit address constructor (lines 42, 46).
- `itc_recomp/src/xact/xact.cpp`: WaveEntryGetLoopRegion: `schema::XACTWaveEntry(nullptr, entry)` is now `schema::XACTWaveEntry(entry)` (line 428).

**For the lead:** none. I didn't build, as the rules say. I read the rest of all four files: no `base` identifiers left, no `(nullptr, ` view or call patterns, no emptied calls. The signatures I checked match their headers: title_screen.h's LoadInit, ScreenInit, LoadDebugPage and LoadDebugStep, title_screen_native.h's debug templates, buffers.h's Buffers_Memcpy and strings.h's MayShare. Two old comments still mention base: save_data.cpp:236 ("nothing read through `base`") and xact.cpp:427 ("no memory base"). I left them alone to follow the no-restyling rule.

## unit4

- `itc_recomp/src/vehicle_accessors/vehicle_accessors.cpp`: 13 views built as View(nullptr, address) (the old base was a literal nullptr, for address-only math) at lines 99, 104, 109, 123, 222-257, 370: now Vehicle(vehicle), Body(body), Contact(contact)
- `itc_recomp/src/hooks/ai_drivers.cpp`: line 366: deleted the leftover `call.base = base;` (UpdateCall has no base member now); comment at 211 no longer says the adapter body uses `base`. The three lifted::Sdk errors (356, 421, 446) come from hooks::AiStandIn in ai_drivers.h (not my file), see for_lead
- `itc_recomp/src/track_collision/track_cell.cpp`: S16(uint8_t* base, uint32_t address), which loads a halfword, collided with S16(uint32_t value) once base was dropped. Renamed the loading one to LoadS16, and changed its only caller (line 304, G::linked_tried_address() + 2 * k, which was S16(base, ...) before) to LoadS16. Every other S16 call was the value form already
- `itc_recomp/src/c_runtime/core.cpp`: line 555 (LockFile): deleted the leftover `(void)base;`
- Left `itc_recomp/src/hooks/ai_drivers.cpp:356`: lifted::Sdk(ctx, g_update_all_stand_in) (also 421 and 446) fails because hooks::AiStandIn in itc_recomp/src/hooks/ai_drivers.h still has a uint8_t* parameter. This needs the header fix in for_lead; nothing to change in this file
- Left `itc_recomp/src/hooks/race_world.cpp:584`: lifted::Sdk(ctx, hooks::g_recorded_stand_in) (also 595, g_give_vehicle_stand_in) fails because hooks::RaceWorldStandIn in itc_recomp/src/hooks/race_world.h still has a uint8_t* parameter. This needs the header fix in for_lead; nothing to change in this file

**For the lead:** Two headers outside my unit still have the old base parameter and cause the 5 remaining Sdk errors in ai_drivers.cpp and race_world.cpp. Drop the uint8_t* parameter from both so the pointers match lifted::RegsFunction (void(Regs&)): 1. itc_recomp/src/hooks/ai_drivers.h:33: `using AiStandIn = void (*)(lifted::Regs&, uint8_t*);` becomes `using AiStandIn = void (*)(lifted::Regs&);` 2. itc_recomp/src/hooks/race_world.h:163: `using RaceWorldStandIn = void (*)(lifted::Regs&, uint8_t*);` becomes `using RaceWorldStandIn = void (*)(lifted::Regs&);` The functions harnesses assign to these pointers are in src/harnesses/, which isn't built, so nothing else needs to change. None of my edits changes the order of loads or stores.

## unit5

- `itc_recomp/src/hooks/scene.cpp`: `using Step = void (*)(lifted::Regs&, uint8_t*)` lost its uint8_t* parameter, so `step(ctx)` and StepCall with ITC_REGS_FN(...) now match. The three captureless step lambdas `[](lifted::Regs& c, uint8_t* b)` became `[](lifted::Regs& c)`, calling SceneModelUpdate(c), SceneUpdateModels(c), AccessoriesDestroy(c) and lifted::Sdk(c, original). This fixes all the errors at lines 340-446, including the std::function assignments that cascaded from them.
- `itc_recomp/src/input/input_manager.cpp`: `schema::LogicalPad(nullptr, pad)`, `schema::PhysicalPad(nullptr, pad)` and three `schema::InputManager(nullptr, manager)` now use the explicit one-address constructor (lines 76, 81, 167, 172, 177).
- `itc_recomp/src/xaudio/voices.cpp`: Deleted the leftover `(void)base;` in ApiEnter and ApiLeave. Their parameter was already gone.
- `itc_recomp/src/debug/frame_state_diff.cpp`: Deleted the leftover `(void)base;` in ReadableGuestRanges.
- `itc_recomp/src/ai_drivers/ai_drivers.cpp`: Two file-local RingEntry functions differed only by base: RingEntry(path, at) at line 569 and RingEntry(base, path, k) at line 2420, which checks the entry count and adds +16. Dropping base made them collide. The second is renamed CountedRingEntry, along with every call that used the base form in the old tree (lines 2452-4593, 18 calls, each checked against d0383928~1). The calls at lines 577 and 1152, which had no base, stay on RingEntry.

**For the lead:** none

## unit6

- `itc_recomp/src/hooks/scene_loaders.cpp`: CreateAGuest::Construct's `Original` function pointer type still took an unnamed uint8_t*: now `void (*)(lifted::Regs&)`. That fixes the 7 array initializers on lines 591-592 and the one-argument call on line 599.
- `itc_recomp/src/threads/threads.cpp`: Deleted the leftover `thread_local uint8_t* t_base` and its `t_base = base;` assignment in Run's thread lambda (line 200).
- `itc_recomp/src/threads/threads.cpp`: CurrentThreadIdIfOurs (line 435) is now `guest::LoadU32(guest::LoadU32(t_pcr + kPcrCurrentThread) + kThreadId)`: the same two loads in the same order, without t_base.
- `itc_recomp/src/threads/threads.cpp`: Dropped the unnamed uint8_t* parameter from GetCurrentProcessType and RegisterTitleTerminateNotification so they match NativeImport. That fixes the kEntries initializers (lines 406-407) and the std::size error that followed from them (line 410).
- `itc_recomp/src/hooks/vehicle_accessors.cpp`: `schema::RigidBody(nullptr, ctx.r3.u32)` became `schema::RigidBody(ctx.r3.u32)` in all four places (lines 100-106). Three macro comments that still said the bodies use `base` were trimmed.
- `itc_recomp/src/course_scenery/course_scenery.cpp`: `schema::AreaLights(nullptr, object)` and `schema::AreaModel(nullptr, object)` became single-address view constructors (lines 228 and 234).
- `itc_recomp/src/hooks/c_runtime_core.cpp`: Dropped the unnamed uint8_t* from the out-of-line definition of hooks::CoreValidateExecute (line 326), so it matches its declaration in c_runtime_core.h.

**For the lead:** None. I read all five files for the same patterns past the reported errors (base members and captures, `(nullptr, x)` view constructors, emptied calls, uint8_t* parameters) and found nothing more. I also checked the pre-step-10 versions: the only `base` names in these files were the guest base, so there was nothing unrelated to restore. Nothing was built, following the swarm's rules. One file outside my unit still passes the old function: itc_recomp/src/harnesses/c_runtime_core.cpp:4165 hands hooks::CoreValidateExecute to ITC_CONTEXT_FN, but that harness isn't built, so it needs no change for the build.

## unit7

- `itc_recomp/src/native/passes.cpp`: `s.guest_base = base;` in Passes::Init and the State member `uint8_t* guest_base` deleted; `MirrorByTickFrameStart(s.guest_base)` now called with no argument.
- `itc_recomp/src/native/passes.cpp`: The glow cross-fade log's `guest::LoadFloat(vertices, call.vertex_address + N)` calls read from the draw mirror, not from guest memory, because inline vertices sit past the physical range at kInlineVertices. They are now a local lambda that byte-swaps the big-endian word at `draw_mirror().mapped + address`. It reads the same bytes as before.
- `itc_recomp/src/native/passes.cpp`: The member `uint8_t* last_base`, which only recorded that a draw had run, is now `bool drew_any`. It is set where `last_base = base;` was and checked in the late-write check and before DumpGuestResolves. `TextureCache::DecodeGuest(last_base, ...)` lost the stale first argument.
- `itc_recomp/src/native/renderer.cpp`: `uint8_t* g_base` is now `bool g_commands_seen`. The three `g_base = base;` lines set it to true. StartRenderer calls `g_renderer->Init()` with no argument and Renderer_EndFrame tests the bool. The behavior is the same: the renderer starts only after the first command hook, and EndFrame does nothing before that.
- `itc_recomp/src/hooks/ui_movies.cpp`: `(void)base;` removed from the UI_MOVIE_ACCESSOR macro.
- `itc_recomp/src/hooks/car_specs.cpp`: In the ITC_CAR_SPECS_AERO macro, the `base` argument the codemod left in the multi-line `guest::LoadU32(base, ...)` call was dropped.
- `itc_recomp/src/debug/state_capture.cpp`: `(void)base;` removed from ReadableGuestRanges.

**For the lead:** None. I compared every line the codemod changed in these 5 files with the pre-step-10 tree (git diff d0383928~1). Every other change is a plain removal of base. The Walk, GuestKeys, GuestTracks and GuestAtexit initializers and the multi-line RecordQuad call in ui_movies.cpp were already correct.

## unit8

- `itc_recomp/src/hooks/camera_modes.cpp`: SetModeCallsAt's kSteps table was typed void(*)(lifted::Regs&, uint8_t*) while the steps take only Regs& (6 init errors, plus the 1-argument call at 1335): dropped the uint8_t* parameter from the array's type
- `itc_recomp/src/hooks/second_set.cpp`: ITC_SECOND_SET_POINTS_DESTRUCT's body kept `(void)base;` (6 errors at its uses): line deleted
- `itc_recomp/src/threads/spin_locks.cpp`: RaiseIrqlToDpcLevel and LowerIrql kept an unnamed uint8_t* parameter, so they didn't match NativeImport (and std::size(kEntries) failed after): parameter dropped
- `itc_recomp/src/collision/boxes.cpp`: BoxCopyFits built its views as Box(nullptr, box): now Box(box) and source(from) with the explicit address constructor
- `itc_recomp/src/type67_modes/screens.cpp`: DialogOpenMovies kept `(void)base;`: line deleted
- `itc_recomp/src/frame_pacing/per_frame_logic.cpp`: g_rival_logo_draw_stand_in's definition kept (lifted::Regs&, uint8_t*) against race_hud.h's (lifted::Regs&): parameter dropped

**For the lead:** none. I also checked the rest of each file for the same patterns: no remaining uint8_t* base parameters or members, no emptied calls, no nullptr view constructions and no unrelated variables named base. The atexit structs in camera_modes.cpp (base member removed by the lead) are initialized as {ctx, sp}, which is consistent. I didn't build anything, so the lead's build has to confirm these fixes.
