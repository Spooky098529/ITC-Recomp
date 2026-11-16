# E5: the game made native (a draft for the user's review)

E5 is the last step of `docs/rexglue-removal.md`: the code we run, which is
the game's PowerPC code translated into C++, becomes real C++ over host
memory. Since E4 (merged into `rexglue-removal-project` 2026-11-16) nothing
of ReXGlue runs but its shader translator; what's left is the Xbox 360's
*shape*: the code reads and writes the game's objects as big-endian bytes
at 32-bit 360 addresses inside one 4.5 GB block of our memory laid out like
the 360's (`src/memory/mapping.cpp`), calls functions by their 360
addresses, keeps its stack frames in that block and finds its per-thread
state through r13. E5 removes that shape, area by area, until the block is
gone.

This document is the plan. It follows the way E1 and E2 were done: a
measure first, then small steps that each leave the game working, each
recorded here as "Done (date): ..." with its checks.

## Where things start (2026-11-16, after E4)

The code:

| What | How much |
| --- | --- |
| Lifted functions (`src/lifted/`, `lift.py`'s translation of `generated/`) | about 4,300, 545,000 lines |
| Hand ports (`src/hooks/` and the subsystem folders: the B7 loop's) | 750 files, 309,000 lines; 885 entry macros (each making one or more entries) |
| Reachable from `main` (`tools/static_map.py`) | 11,866 functions; ours 10,187; unported 9, none run in play |
| Subsystem files in `docs/functions/` | 136 |

The 360 shape in that code (counted in the sources, 2026-11-16):

| The shape | Count | Where |
| --- | --- | --- |
| Big-endian loads and stores at guest addresses (`REX_LOAD_*`, `REX_STORE_*`) | 101,893 | lifted |
| The same through `guest::Load*` and `guest::Store*` | 17,012 | hand ports |
| Raw guest pointers (`REX_RAW_ADDR`: memcpy, vector loads) | 2,858 | mostly lifted |
| Calls by constant 360 address through the dispatch table (`lifted::Call(base, 0x8...)`) | 16,856 | lifted |
| Calls through a function pointer or vtable slot (`lifted::Call(base, ctr)`) | 2,295 | lifted |
| Stack frames made in guest memory (`stwu r1`) | 3,631 functions | lifted |
| Addresses built in the image's range (`lis rN,-32xxx`: globals, tables, vtables, strings) | 9,724 | lifted |
| Reads of the per-thread block (r13: +256 the thread, +268 the CPU) | a handful of sites | lifted, `threads/` |

The memory (`docs/rexglue-removal.md`, "E2: the plan"): the image at
`0x82000000` (10 MB from `default.xex`), our allocation layer's heap at
`0x10000` (384 MB), physical memory at `0xA0000000`, `0xC0000000` and
`0xE0000000` (the same 512 MB three times, plus once flat at base + 4 GB for
the renderer), our threads' stacks and per-thread blocks.

The renderer (`docs/native-renderer.md`): it replays the game's command
data, reads vertices, indices, textures and shader microcode at the GPU
addresses the game computes, through one mirror of physical memory per
frame in flight (Vulkan refused to import the game's memory directly:
"Guest memory is mirrored, not imported"); the mirrors take what memcpy
wrote, once a frame.

What we know about the game's objects: `docs/known-data.md` (the globals
and object fields worked out, 595 lines) and the 136 subsystem files'
notes (fields named per function, "+24 the started word"). Much of it is
written per function, not per type.

## The goal

When E5 is done:

- The game's objects are C++ structs and classes in host memory, in host
  byte order, holding host pointers, made with `new` (or containers), with
  their functions as member functions or plain functions taking them.
- Calls are C++ calls: direct calls, virtual calls for what the game's
  vtables did, `std::function` or function pointers for its callbacks. No
  dispatch table by address.
- Locals are on the host stack; per-thread state is `thread_local`.
- The game's files (`DATA/`) are read by loaders that turn each format into
  host structures once, at load. Saves keep their on-disk format.
- The renderer gets vertex, index, texture and constant data from native
  code that writes it into Vulkan buffers the renderer owns, with real frame
  synchronization: no mirrors, no GPU address math, no physical aliases.
- The image's data (constants, strings, tables) is compiled into the
  executable; `default.xex` is no longer read (E3's end). The player's
  `DATA/` stays the only game data needed.
- The 4.5 GB mapping, `REX_LOAD_*`, `guest::Load*`, `lifted::Call` by
  address and r13 are gone.

## How it's done: three ideas

**Name first, then move.** Every field gets a name and a type before any
object changes where it lives. A schema of the game's types (step 2) is the
one place that says "the race machine is 40 bytes; +24 is `started`, a
32-bit word; +8 is `next_page`, a pointer to a page". Code switches from
offsets to the names with nothing in memory changing; then, type by type,
the storage behind the names changes. The schema also drives the file
loaders and the handle tables, and is checked against the code by a tool.

**Ownership decides the order.** An object only one area touches can move to
the host alone; one that many areas touch, that a file holds, or that the
GPU reads moves last. The measure (step 1) finds which is which; the steps
then work from the leaves of that graph inward.

**A bridge while both shapes exist.** Code not yet converted still holds
32-bit addresses. A host object that guest-shaped code must still name gets
a *handle*: a 32-bit value in a range of the guest address space nobody
maps, looked up in a table. Guest-shaped code passes it around unchanged;
converted code turns it into the host pointer. The bridge shrinks to
nothing as areas convert.

## The steps

Each step: smallest and most independent first, the game working after
each, its checks below and under "Checks". Steps 3 to 7 run as loops over
areas (as B7 did), not as one change.

### 0. Groundwork

- **Calls by constant address made direct.** The 16,856 `lifted::Call(base,
  0x8...)` with a fixed target name a function we have (a lifted function, a
  hand port's entry, an import thunk's): `lift.py` and a sweep over the hand
  ports call it as a C++ function (its typed parameters, as lifted functions
  already call each other). The dispatch table is then only for the 2,295
  calls through pointers and vtable slots. Mechanical; checked by the build
  and the game.
- **Entries by name.** The dispatch table's registrations keep the 360
  address as the key (the indirect calls still need it), but every entry
  gets a C++ name from its function row (`sub_821A1258` →
  `BuildScreen::BackToBuilds`) where one is known, so the code reads in the
  game's terms. Rows without a name keep `sub_X`.
- **The guest memory macros under our own names** (`REX_LOAD_U32` →
  `guest::LoadU32`-style, one spelling for lifted and hand-ported code), so
  the later steps' tools see one form.

### 1. The measure: types, owners, and what crosses

Recorded here, as E1's and E2's measures were; it picks the order of steps
3 to 7.

1. **Types from allocations.** Every place memory is allocated
   (`memory::Alloc`, the C runtime heap, `operator new`'s callers, the
   game's pools, stack frames) with its size and the constructor or
   function that fills it; each becomes a candidate type. The vtable
   pointer a constructor stores (a `0x82...` address) names the class.
2. **Fields from accesses.** For each type, every load and store relative to
   a pointer known to hold it (followed through the lifted code's registers,
   as `static_map.py` follows calls): offset, width, float or integer,
   whether the value stored is itself a pointer (to which type) or a
   function address. A tool (`tools/type_map.py`, new) writes this per type;
   `docs/known-data.md` and the function rows fill in names.
3. **Owners.** For each type, which areas' functions touch it (the 136
   subsystem files are the areas). A type touched by one area is *private*;
   by several, *shared*.
4. **What can't move by itself.** Types the game reads from `DATA/` in
   place (models, XPR textures, sound banks, the archive's tables, string
   tables), types the GPU reads (vertex and index buffers, textures,
   constants, command data), the image's data, saves.
5. **The graph.** Areas as nodes, shared types as edges, weighted by how many
   functions touch them. Leaves first.

Done when the tables exist for every type the static map reaches, with the
share of loads and stores the tool could attribute to a type (the rest
listed, to read by hand).

### 2. The schema and named fields (no change in memory)

- **The schema** (`itc_recomp/schema/`, one file per area, in a form the
  tools read and humans edit): each type's size, fields (name, offset,
  type: integer widths, float, a pointer to a type, a function pointer, an
  array, an embedded type), vtable, and where it comes from (allocated,
  file, image, GPU). Started from step 1's tables and the docs; names from
  `docs/known-data.md` and the rows. Unknown fields stay `unknown_24` until
  read.
- **Generated views** (`src/schema/`, written by a tool from the schema): a
  small class per type over guest memory, `Ref<RaceMachine>` with
  `m.started()` and `m.set_started(v)` (big-endian underneath, as now) and
  `m.next_page()` returning a `Ref<Page>`. No new storage: a view is
  `base` and a 32-bit address.
- **Code switched to the views**, area by area: hand ports by hand (they
  already name most fields in comments), lifted functions by a tool that
  rewrites `REX_LOAD_U32(r31 + 24)` to `machine.started()` where step 1 knows
  r31's type. What the tool can't type stays as it is, listed.
- **A check tool**: every access in the code against the schema (the
  offset and width must be a field of the type the pointer holds).

This step changes nothing the game sees: the same bytes, the same
addresses. It's what makes the rest possible: once code says
`machine.started()`, where `started` lives can change in one place.

### 3. Private objects to the host

For each area, the types step 1 found private (touched by that area only,
not from a file, not read by the GPU):

- The type becomes a C++ struct (host order, host pointers, real members),
  made with `new` or kept in a container; the generated view's accessors
  now read the struct.
- Pointers to it held in guest-shaped objects of other areas become
  *handles* (a 32-bit value in an unmapped range, looked up in a table) until
  those areas convert.
- Its functions become member functions or take it by reference; their
  stack frames stay in guest memory until step 6 if they still pass guest
  addresses of locals.

Likely first areas (to be confirmed by the measure): our C runtime's state
(locks, streams, `errno`, the per-thread data), XAudio's internal objects
(voices, the mixer's lists), the input manager and processors, the action
mappers, the menus' state machines (the race menu, the options screen, the
garage screen: each a machine and its pages), the UI movies' own records.

Each area: ported, built, checked (see "Checks"), its rows updated (a new
status, **Host**: "its objects are host structs").

### 4. Shared objects, from the leaves inward

The types several areas touch, in the order of step 1's graph: each moves
when the last area touching it in guest shape has converted its other
types, or moves with handles for the areas that haven't. The game's big
object graph lives here: the scene and its models, the cars (physics,
models, draw), the race world and its racers, the app's state stack and
modes, the UI movies. This is the bulk of E5's work, and where the loops
spend most of their runs.

Virtual calls: when a class with a vtable moves, its vtable becomes C++
virtual functions (or a table of `std::function` where the game swaps
entries), and the `lifted::Call(base, ctr)` sites that call through it
become virtual calls. Callbacks stored as addresses (functors, the archive's
completion callbacks, the race flow's transitions) become function objects.

### 5. The game's files: loaders

Each format the game reads into memory and uses in place gets a loader that
reads it once into host structures (the schema describes the file's layout
as well as the host type's):

- the archive and its table of contents (`archive.md`, `file-layer.md`);
- models and their materials, skeletons, morphs (`models.md`,
  `scene-loaders.md`, `car-models.md`);
- XPR texture packs (the textures' headers; the pixel data goes to the
  renderer, step 7);
- XACT sound banks and wave banks, and the music (`xact*.md`); the XMA data
  stays bytes for FFmpeg;
- the string tables and fonts, the data tables (`strings-and-font.md`,
  `data-tables.md`, `car-specs.md`);
- the UI movies' data (`ui-movies.md`);
- saves (`save-data.md`, `saves/`): converted to and from host structures;
  saves from older builds may stop loading (the user's call, 2026-11-16,
  under "Decided").

A type the game reads from a file and also builds in memory (a model the
game edits, the vinyl sheets) moves with its loader.

### 6. Control flow and threads

- **The guest stack.** Functions whose frames no longer hold anything other
  code addresses by guest address (after steps 3 to 5 most locals are host
  objects or values) drop their guest frames; once no function makes one,
  the threads' guest stacks go.
- **The per-thread block.** r13 and the PCR (+256 the thread object, +268
  the CPU number, +336, the TLS slots, the last error) become `thread_local`
  state of our threads (`src/threads/`); `GetLastError`, `TlsGetValue` and the
  graphics library's per-CPU workers read it.
- **The kernel imports.** Our answers to the 28 imports in play
  (`src/threads/`, `src/memory/memory_imports.cpp`, `src/imports/`) become
  plain calls to host primitives at their call sites (a critical section a
  `std::recursive_mutex` member, an event a host event), and
  `lifted::Import` goes.
- **The float modes.** The MXCSR switches the original made (flush-to-zero
  for vector work, off for scalar) stay where the results depend on them;
  that decision is per area, by reading.

### 7. The renderer without mirrors

The renderer replays the game's command data (`render_queue.md`,
`device-state.md`, `command-buffer.md`): device state, shader objects,
vertex and index buffers and textures named by GPU address. Once the code
filling them is native (steps 3 to 5):

- **Buffers.** Vertex and index data (models, the dynamic geometry the game
  writes each frame: sprites, the HUD, particles, the minimap) is written
  into Vulkan buffers the renderer owns, per frame in flight, with fences;
  the game's own fence waits on its buffers (Dropped in B7) come back as
  ours where a buffer is reused.
- **Textures.** Loaded from XPR data by step 5's loader straight into Vulkan
  images; the render targets the game resolves into are the renderer's
  images by name, not by GPU address.
- **Constants and state.** The device's state setters (native since B7)
  hand the renderer typed state instead of register words; the renderer's
  register file (`native/state.h`, the SDK's register layouts) goes, and with
  it the translator's need for register words beyond the shader key.
- **Then** the mirrors, the flat physical range at base + 4 GB, the three
  physical views and `PhysicalHostAddress` go. The texture reloads on
  memcpy's writes go with them (the loaders know when data changes).

The shader translator stays (the user's call, 2026-11-16): the shaders are
still the game's Xenos microcode, translated once per key and cached; what
changes is only where their inputs come from. (The one-time bake stays a
lead: `docs/rexglue-removal.md`, E4's step 5.)

### 8. The image's data compiled in

What the code still reads at `0x82000000`: constants and float tables
(often loaded with `lis` and an offset), strings, the game's static tables
(car data, menus, course layouts, sound tables), vtables (gone with step 4)
and initialized globals. Each becomes C++ data (a `constexpr` array, a
string literal, a global of a host type) by a tool reading the image (the
player's `default.xex`, loaded by our loader at build time on the
developer's machine, as the HLSL shaders were stored). Which tables may be
committed (they're the game's data) is a decision for the user.

### 9. The end

With no access left at a guest address: the mapping
(`src/memory/mapping.cpp`), our allocation layer's guest side
(`src/memory/heap.cpp`: its host side stays as the allocator, or goes for
`new`), `guest.h`, `guest_code.h`, `lifted::Call`, the dispatch table,
`src/lifted/`'s runtime and the image loader go. `generated/` stays as long
as the user keeps it as the reference (the user's call, 2026-11-16).

## Checks

The game is the check, as it has been since the rewiring's stage 4:

- After each change: the build; `timeout 120 attract_race.sh 40`; the pad
  replay (`scripted_run.sh itc_recomp/pad_recordings/race_test.jsonl`,
  ~127 s, no error lines, the race at the frame cap); a frame dump.
- **A state digest** (new, step 0 or 1; proposed): `--state_digest`, a line a
  tick with a hash of the state that matters (the racers' positions and
  speeds, the race's progress, the menus' selections, the app's state
  stack), so two builds can be compared over the same pad replay tick by
  tick, and the first tick that differs found. The pad replay already makes
  the game's input fixed; what can still vary between runs (threads'
  timing, the attract mode's random filter, `rand`'s seed) is measured
  first and either fixed under a flag or left out of the digest.
- Frame dumps at fixed points of the pad replay, compared (dumps vary
  ~0.25% of pixels between runs today: the comparison allows that).
- Saves: after each step that touches the save path, a save made on that
  build loads again (older builds' saves may break: "Decided").
- A drive by the user before each merge into `rexglue-removal-project`.
- `generated/` stays the reference for what the original did when a check
  differs (read, not run).

Performance isn't checked step by step (as for B7: native may be slower
first); a profile against `rexglue-removal-project` before E5 at the end of
steps 4, 7 and 9.

## Tracking

- Each area's file in `docs/functions/` gets the new status **Host** for
  functions whose objects are host structs, and its types' rows in the
  schema.
- A status tool (`tools/e5_status.py`, new): the counts in "Where things
  start" (guest loads and stores, calls by address, guest frames, image
  references), per area and in total, recorded after each step in a table
  at the end of this document, as "The boundary over time" was.

## Decisions for the user

- **Behavior.** B7's rule was "no behavior change, match the recompiled
  code exactly". E5 changes where data lives, so byte-for-byte equality
  with memory is gone by design. Proposed: no change the player can see or
  the state digest shows (the same game, tick for tick, on the same input),
  but layouts, allocation order and frees may change.
- **Saves.** Proposed: saves made by any earlier build keep loading, and
  saves made after E5 load on earlier builds too (the on-disk format
  unchanged).
- **Order.** Proposed: bottom-up (step 0, the measure, the schema, private
  objects, shared objects, files, control flow, the renderer, the image).
  The other order (the renderer first, for the mirrors) isn't possible
  alone: the renderer's inputs are written by the game code in guest shape.
- **The schema's form.** A data format the tools read and generate C++ from
  (proposed: Python modules or TOML files in `itc_recomp/schema/`), versus
  hand-written C++ headers the tools parse.
- **The state digest** (new tool), and what it hashes.
- **The image's data committed** (step 8): the game's tables as C++ data in
  the public repo (as the 14 HLSL shaders' microcode already is), or
  extracted from the player's `default.xex` at start-up.
- **Process.** The B7 loop with migration agents per area (its rules carried
  over: precedent first, the user's limits per run), and which steps run as
  loops (proposed: 2 to 5).
- **The names.** Entry and function names from the rows (step 0) where the
  row's name is marked "probably": take them, or keep `sub_X` until read.

Decided (the user, 2026-11-16), on `e5-native-16-11-26`:

- **Behavior**: as proposed. No change a player can see or the state
  digest shows (the same game, tick for tick, on the same input); layouts,
  allocation order and frees may change.
- **Saves**: breaking saves from older builds is fine. The on-disk format
  doesn't have to be kept for compatibility, either way. (The plan's save
  check and the risk "byte-for-byte what the game wrote" no longer apply.)
- **Order**: bottom-up, as drafted.
- **The schema's form**: TOML files, one per area, in
  `itc_recomp/schema/`; the tools read them and generate the C++.
- **The state digest**: built as drafted (`--state_digest`).
- **The image's data**: committed as C++ data, extracted by a tool from
  the player's `default.xex` on the developer's machine; `default.xex` is
  then no longer read.
- **Process**: steps 2 to 5 run as loops with migration agents per area
  (B7's rules carried over); steps 0, 1 and 6 to 9 done directly.
- **The names**: rows' names taken even where marked "probably"; rows
  without a name keep `sub_X`.

## Risks

- **Scale.** E5 touches every function; B7 took 47 loop runs for less. The
  schema and the tools (steps 1 and 2) decide how much is mechanical.
- **Types the tool can't follow.** Pointers that pass through untyped
  containers (the game's lists and pools, `void*` callbacks) hide their
  type; these are read by hand, area by area.
- **Aliasing and unions.** The same memory read as two types (a pool reused
  for several, a union, a block reinterpreted) is only found by reading;
  the check tool flags accesses that fit no field.
- **The GPU's view.** Data the game writes for the GPU in place (dynamic
  vertex buffers, the command buffer's inline data) can only move when both
  the writer and the renderer move together (step 7).
- **Timing.** Host objects and direct calls change timing between threads
  (the frame driver's workers, the tiling workers, the audio threads); the
  state digest and drives are the check.
- **Saves.** A save written from host structures must load back the same
  game state (older builds' saves need not load: "Decided").
