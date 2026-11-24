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

Done (2026-11-16, on `e5-native-16-11-26`), in four commits, each built and
checked:

- **The calls.** The 16,856 were 8,990 in the lifted batches and 7,866 in
  `originals.cpp` (the lifted originals hand ports fall back to with their
  flag off). By target: 13,731 a hand port's entry, 215 a lifted function
  (lifted in a later batch than its caller), 2,910 an address with no
  function of ours (1,266 functions with plain C++ but no adapter, such as
  `List16::Insert32` `sub_82154378`; their callers never run in play). Hand
  ports' entries are now `lifted::entry::e<address>` (no longer `static` in
  `ITC_NATIVE_ENTRY_DEFINE`), declared in `src/lifted/entries.h`, which
  `tools/entry_list.py` writes from a build's symbols (3,543 entries; the
  wrapper macros' forms don't read well from the sources); run it again
  after adding or removing an entry. `lift.py` and `lift_originals.py` call
  entries directly (the uniform `(base, Args)`: typed parameters come when
  a port is converted) and lifted functions typed; the 2,910 stay
  `lifted::Call`, which stops the program. The hand ports' own 235
  `lifted::CallFromContext(ctx, base, 0x8...)`: 227 became
  `lifted::Enter(ctx, base, <entry>)` (none reads `ctr` after; the lifted
  functions' uniform entries are now `lifted::e<address>` in `functions.h`
  for this), and 8 target no function of ours. Among those,
  `save_interface.cpp`'s call to `0x822900E0` (a "rep" save's parse) has a
  lifted original but no entry, so reaching it would stop the program: to
  look at. `lift.py reemit` writes every batch again from its registration
  table (checked first: today's output, but for one function's tail block
  placed elsewhere).
- **The names.** `tools/function_names.py` writes `src/lifted/names.h`:
  4,423 of our functions under `game::A::B` (1,120 lifted, 3,303 entries),
  each a reference to the function, from the rows' Name cells ("probably"
  names taken; constructors `Ctor`, destructors `Dtor`, `std` as `Std`, a
  keyword part with `_`, operators kept as `sub_X`; 40 names shared by
  several functions, or also a namespace of others, get `_<address>`). The
  lifted code calls by these names (15,851 sites), each definition's comment
  names its function, and the hand ports' 175 `lifted::Enter` to an entry
  use them. Run the tool and `lift.py reemit` again when rows are renamed.
- **One spelling for guest memory.** `guest.h`'s `guest::Load{U8,U16,U32,U64}`,
  `guest::Store*` and `guest::Pointer` (new: `LoadU64`, `StoreU64`,
  `Pointer`), written as plain inline functions with the same volatile
  accesses; `lift.py` emits them in place of `REX_LOAD_*`, `REX_STORE_*`
  and `REX_RAW_ADDR` (`own_spelling`), the 27 hand-port files that used the
  macros were rewritten the same way, and the macros are gone from
  `guest_code.h`.

The dispatch table is now reached by the 2,382 calls through pointers and
vtable slots (`lifted::Call(base, ctr)`), the 2,918 to addresses with no
function of ours, and the hand ports' calls by a computed address.

Corrected (2026-11-17): the calls to an address with no entry *are* reached
in play. An attract race (`itc_5753`) stopped at "a call to 825D55E0, which
has no entry": the lifted `sub_825D1830` calls `XACTTrackInstance::CheckDone`
by address, whose adapter was removed in B7 (only plain C++ called it then).
The call predates E5 (in `rexglue-removal-project` before step 0; 1 of the
last 40 logs). Fixed for all of them: `lift_originals.py --all` also lifts
every address lifted code or a hand port calls with no entry (2,369
originals, from 1,408), `originals.cpp` registers each as a fallback
(`lifted::RegisterFallbacks`), and `lifted::Call` and `Find` run the
fallback when an address has no entry, so the game's own code runs there,
as it did when recompiled. Four can't be lifted (two read their caller's
registers, two the float control register: `sub_825B32F0`, `sub_825B8778`,
`sub_825B0BB0`, `sub_825B0BE4`) and still stop the program if reached.
`save_interface.cpp`'s call to `0x822900E0` is covered too (its original
is a fallback now). Two attract races and the pad replay clean. Only
the first level: the originals' own calls aren't followed, since that
closure runs through most of the game (tried: 2,369 originals grew past
4,451 in ten rounds without settling); an original's call to an address
with no entry still stops the program with its address in the log, to be
lifted then (`lift_originals.py` with that function).

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

Measured (2026-11-17, on `e5-native-16-11-26`), for the user's review:
`itc_recomp/tools/type_map.py` (it reads code, changes none; ~4 s) writes
`itc_recomp/coverage/type_map.tsv` (a row a type: functions, accesses,
fields with offset, width and int/float/pointee, allocation sizes and
sites, vtables, owner, areas, tags), `type_map_areas.tsv` (the graph's
nodes: an area's private and shared types, its shared weight, its heaviest
neighbors) and `type_map_split.tsv` (an area's typed accesses by kind).

How it reads the code: the original PowerPC instructions (the comments in
`generated/`) of the 8,289 functions the static map reaches from `main`
that are ours or unported (the replaced libraries left out), following
pointers through registers in a straight read of each function (branches
not followed: a simplification). Types come from four sources:
- a row's class (`X::Y` takes an `X` in r3), joined only when the function
  uses r3 as a pointer: a name like `Controls::ButtonIndex` is a free
  function;
- allocations: 573 sites (`operator new`, `new[]`, `malloc`, `XMemAlloc`,
  `HeapAlloc`, the memory blocks', `VirtualAlloc`, the physical ones)
  named by the constructor they're handed to, else `new<size>@caller`;
- paths: what a field or an image word holds (`App.+24`, `[826C5C80]`),
  joined with the type stored there when every store agrees;
- parameters and returns (`sub_X.r4`, `Find.ret`), joined with what every
  caller passes or every return gives. A generic function (lists, copies)
  keeps a type of its own.

The joins are made again each pass from the last pass's votes (four
passes; the output is the same from run to run), and two classes the rows name differently are never merged (463
such joins refused: base and derived classes, generic helpers). The first
runs without those rules merged half the game into one type (`Mesh` with
`Car`, `CarScene`, `Skeleton`...); what's left may still hold such merges,
smaller, to read by hand.

The figures (2026-11-17):

| Loads and stores (192,480 in the 8,289 functions) | Count | Share |
| --- | --- | --- |
| Through a typed pointer (fields) | 80,800 | 42.0% |
| On the stack (the frame and locals' addresses) | 68,266 | 35.5% |
| At an image address (2,819 addresses; 2,134 used by one area) | 18,021 | 9.4% |
| Untyped (a register the read lost: after an indirect call, a loop's join) | 10,933 | 5.7% |
| Indexed by an untyped register | 7,311 | 3.8% |
| An array inside a typed object (`[i]`), or indexed and typed | 6,417 | 3.3% |
| Other constant addresses, the thread block | 732 | 0.4% |

Of the accesses not on the stack, **80.6% are attributed** (to a type or the
image); the rest are listed per function by the tool. 4,884 types: 4,573
named (by a class, a path, a parameter or a return), 311 allocations
without a class; **4,468 private, 416 shared**. 180 constructors' vtables
found.

Typed accesses by their types' kind: **36.5% on types tagged file, GPU or
save** (touched by an area that reads `DATA/` in place, feeds the GPU, or
saves: steps 5 and 7; the tags are by area, not proof), **36.2% private**
(step 3), **27.4% shared** (step 4).

What can't move by itself (the tags, by type): 831 touched by a file area,
251 by a GPU area, 85 by a save area; 86 types are what an image word
points to (the game's singletons and globals, `[826C5C80]`, the scene at
`[826C2E44]`); 4,501 types have no allocation seen (embedded objects, file
data, globals' contents, or allocations through a path the tool doesn't
follow).

The most shared types (areas, functions): the UI movie records
(`UiMovies::Find`'s return: 39, 937; `UiMovie`: 27, 303;
`UiMovie::Child`'s: 17, 167), the save data (`SaveData::Of`'s: 36, 165),
`Racers` (38, 195), `SceneLights` (22, 69), the objects at `[826C5C80]`
and `[826BABA0]` (15 and 14), the stand scene's 24-byte allocation (12,
81). These are step 4's hubs, moved last or behind handles.

The order it gives (to confirm with the user):
- **Step 3 first**, the areas most on private types (share of their typed
  accesses): XAudio's objects (99.5%), our heap (94.7%), the type 2 dialog
  (90.6%), the C runtime (88.4%), the framework (87.0%), XAudio (86.4%),
  the track zones (84.5%), the app states (81.2%), the race menu (80.3%),
  the action mappers (79.5%), then the race world, collision, XAudio's
  engine, the car sounds, the type 5 mode, the movies, the game modes,
  today's result and the AI drivers (67% to 73%). That's the plan's guess
  (the C runtime, XAudio, the action mappers, the menus' machines).
- **The leaves** of the graph (fewest shared ties): `std::string`, XACT's
  core, the morph jobs, the shader objects, the file layer, the archive,
  the data tables, the framework.
- **The heaviest ties** (functions on both sides of a shared type): the
  tuning pages and the tuning screen (336), the AI drivers and the car
  physics (215), the tuning screen and the UI movies (171), and every
  front-end screen with the UI movies (130 to 135): those pairs move
  together, the UI movies' records last.
- **Steps 5 and 7** are as big as step 3 by accesses (36.5%): the file
  loaders and the renderer's inputs aren't a tail.

Four fixes found by step 2 (2026-11-17; the figures above are after
them): a function without an area no longer takes it from a callee called
from more than 20 places or in a library area (the C runtime, the heap,
the math: operator new and delete filed every deleting destructor under
the heap, `__ultod` and `log` 44 image filters under the C runtime); a
name the rows give several functions carries the address in the measure's
names (`memcpy_825A7410.r3`); a function's second allocation of the same size is another
type (`new<68>@Type2Dialog::OnEnter#2`: the dialog's menu and data were
one), and a constructor calling its base class's constructor on its own
`this` (`ScreenMachine`'s `StateMachine`) no longer makes them one type.

The functions still filed in `lifted.md` (the lift sweep's catch-all) were
given their callers' area; those with no caller in the code (reached by a
pointer) stay there as one area (2,573 typed accesses).

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

Done (2026-11-21, on `e5-native-16-11-26`): the
tools (`schema_seed.py`, `schema_gen.py` with `--area`, `schema_check.py`,
`schema_lib.py`; `lift.py` names lifted fields from the schema), and every
area converted: one at a time first (the type 2 dialog, the pilot; the
heap; the C runtime; XAudio's objects), then the rest, by the user's call,
as a swarm of parallel agents ("Step 2 as a swarm" in
`docs/loops/e5-loop.md`): 130 areas' schema files and hand ports, an
integration pass, and each area's accesses to other areas' types. The
schema: 134 areas, 1,608 types, every access in the code fitting a field.
The lifted code re-emitted with the names, built, an attract race, the pad
replay and a frame dump checked. Then (2026-11-22) the measure's offset
and class-name faults fixed (a pointer into an object votes for the
object embedded there; a function's row name doesn't override what every
caller passes) and the schema brought along: 1,938 types. The agents' reports are
in `docs/loops/e5-swarm-reports.md`; what's left (the `fault_*` fields still
reached, the seeded types' names) under "For
the lead after the swarm" there.

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

Done 2026-11-24 (`docs/loops/e5-loop.md`, "Step 8"): `.rdata` and `.data`
(the constants, strings, tables and initialized globals; 748 KB without
their zero runs) are C++ data in `src/image/`, written by
`itc_recomp/tools/image_data.py` from the player's `default.xex`, and
copied to `0x82000000` at start-up (`image::CopyInto`); `default.xex` is no
longer read, and our XEX loader is gone. The code sections and the rest
aren't compiled in (nothing reads them: the tick replay matched with them
zeroed). Naming the tables one by one (a `constexpr` array or a typed
global for each) goes on with the type-by-type work after step 9.

### 9. The heap and the image out of the guest mapping

Added 2026-11-24 (the user's call): steps 3 to 5 moved the objects that
are allocated on their own and could be cleared of raw accesses; about
1,590 types are still in our allocation layer's heap at their guest
addresses (most embedded in other objects, a caller's locals, or types the
measure never saw allocated), and nothing in steps 6 to 8 moves them. Done
as the physical memory was in step 7 (`docs/loops/e5-loop.md`, "Step 7 as
one swarm"):

- **The heap**: our allocation layer's range (`0x10000` up to the handles'
  range) becomes a block of host memory of its own, and `guest.h`
  translates its addresses in every load, store and `guest::Pointer`;
  everything in it is then in host memory at once, its bytes as the guest
  laid them out.
- **The image**: after step 8 compiles its data in, what's still read at
  `0x82000000` (anything step 8 leaves) is a block of its own, translated
  the same way.
- **Then** the guest mapping's scaffolding (`src/memory/mapping.cpp`'s 4 GB
  reservation, the memory file and its views) goes: no address resolves
  through `base` any more. A raw `base + address` left anywhere faults,
  named by the fault handler, as the physical memory's did.

Type-by-type conversion of what's left (step 4's units, as swarms) can go
on after it, for the code's sake: it no longer decides where the memory
is.

Done 2026-11-24 (`docs/loops/e5-loop.md`, "Step 9 as one swarm"): the
heap and the image are blocks of host memory of their own
(`memory::g_heap`, `memory::g_image`), translated by `guest.h`; the memory
file and its views are gone. `base` stays only as an unmapped 4 GB range
until step 10, so a raw `base + address` faults and is named. A swarm of 16
read every hand port for code assuming the old mapping and found none left.

### 10. The end

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
- **A state digest** (built 2026-11-22: `--state_digest`, with the pad replayed
  by tick, `tools/tick_replay.sh`; `docs/pad-replay.md` "By tick"; as first
  proposed:) `--state_digest`, a line a
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
  (B7's rules carried over); steps 0, 1 and 6 to 10 done directly.
- **The names**: rows' names taken even where marked "probably"; rows
  without a name keep `sub_X`.
- **The Host status** (2026-11-17): added, as proposed: a function whose
  objects are host structs (steps 3 to 5); in every file's status
  definitions and in `tools/check_function_statuses.py` (checked as
  Native).

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

## The counts over time

From `tools/e5_status.py` (lifted code / hand ports) and the schema's
storage.

| When | Guest loads and stores | `guest::Pointer` | Calls by constant address | Calls through a pointer | Vtable words through `VtableWord` | Guest frames | Image addresses | Schema types host / dual / guest |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Step 4's swarm (2026-11-24) | 118,428 / 24,566 | 3,041 / 45 | 4,007 | 2,515 | 2,166 / 20 | 4,286 | 11,660 | 155 / 118 / 1,665 (before it: 38 / 54 / 1,845) |
| Step 5's swarm (2026-11-24) | 112,420 / 24,530 | 3,041 / 51 | 4,007 | 2,515 | 2,166 / 20 | 4,286 | 11,660 | 193 / 132 / 1,590, and 37 blobs |
| Step 8 (2026-11-24) | 112,420 / 24,272 | 3,041 / 67 | 4,007 | 2,515 | 2,166 / 20 | 4,286 | 11,660 | 193 / 132 / 1,590, and 41 blobs |
| Step 9 (2026-11-24) | 112,420 / 24,272 | 3,041 / 67 | 4,007 | 2,515 | 2,166 / 20 | 4,286 | 11,660 | 193 / 132 / 1,590, and 41 blobs; all in host memory (the heap, the image and the physical memory in blocks of their own) |
