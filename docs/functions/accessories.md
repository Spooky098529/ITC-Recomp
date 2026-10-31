# Known functions: the interior accessories

The accessories a car's interior can carry, at `820EF250`-`820F1078`:
probably a hanging ornament (a part model with a small skeleton whose
nodes swing as a chain). The interior record's +112 picks one of three
(3 is none: `sub_820D3840`, `scene-models.md`), and its +116 a texture
index for it. Mapped by the B7 loop (2026-10-31, from the lead "another
skeleton user"); none of the lead's functions had rows of their own
before (the rows in `model-animation.md`, `models.md`, `c-runtime.md`
and `files-and-saves.md` only named them as callers or callees).

- **The set** (`[0x826C5510]`, 32 bytes, `../known-data.md`): made once
  by `Accessories::Create` from the scene's constructor `sub_820C8A50`,
  released by `Accessories::Destroy` from `sub_820C9608`. Base vtable
  `0x8200672C`, its own `0x82006740` (slot 0 the deleting destructor,
  slot 1 `Accessories::Acquire`); +4, +8, +12 the three entries (or 0);
  +16 a 16-byte scale, (1, 1, 1, 1) by default. `CarInterior::Load`
  (`sub_820D79B0`, `scene-loaders.md`) acquires through slot 1,
  `Interior::Release` (`sub_820D75F8`) releases through
  `Accessories::Release`, `Interior::IsLoaded` asks
  `Accessories::AllLoaded` (both `scene-loaders.md`).
- **An entry** (1,104 bytes, `VirtualAlloc`): +0 a reference count, +4
  its file (an `AccessoryFile`, archive asset 23008 + the index), +48
  the accessory. When the file arrives, `AccessoryEntry::OnLoaded`
  hands its three parts (`DataFile::GetEntry` 0 to 2: the part model,
  its XPR and the node file) to the accessory's three files.
- **The accessory** (from entry + 48, 1,056 bytes): a car part model
  (`PartModel::PartModel`, 464 bytes, `models.md`) with vtable
  `0x820066C8`; +464 and +500 the model's and the XPR's files, +544 its
  skeleton (an `AccessorySkeleton`, 448 bytes); +992 the anchor
  position (a float4, w = 1), +1008 three angles, +1024 the scale (the
  set's +16); +1040 the texture index, +1044 visible (bit 0, 1 by
  default), +1048 loaded. `SceneModel::Update` (`scene-models.md`)
  moves it to the interior's accessory node (`Accessory::SetAnchor`)
  and steps the skeleton (`Accessory::Step`), and the per-car draw step
  `sub_820E53F0` (Wrapped, `functions.md`) sets the texture index and
  draws it (`Accessory::Draw`).
- **The skeleton** (accessory + 544): a `SkeletonBase`
  (`model-animation.md`, 176 bytes) with vtables `0x82006700` at +0 and
  `0x820066FC` at +176 (a second base); +180 the node file's
  `AccessoryFile`; +216 a 40-byte worker job (vtables `0x820066DC` and
  `0x82002908`, delegate `0x8200833C` at +240, as `DebugText`'s in
  `framework.md`) that runs its step; +256 the node whose name holds
  the string at `0x82006710` (not read), +260 set up, +264 1, +268 and
  +272 an angle and its speed (probably), +288 and +352 two matrices
  (this step's and the last), +416 and +432 two float4s (the last
  step's movement and a swing vector, probably).
- **A file** (`AccessoryFile`, 36 bytes, vtable `0x82006738` over the
  base `0x82010B5C`): +4 the asset id, +8 the archive's serial, +12 the
  state (0 none, 1 loading, 2 loaded), +16 the data, +20 its size, +24
  owned (freed with `VirtualFree` on reset), +28 and +32 a callback and
  its context, called when the archive's load completes.

In play (the coverage list), the set, the entries and the accessories'
loads ran (an accessory was acquired, loaded and released); its
anchor, step and draw never did (`SceneModel::Update`'s accessory branch
and `sub_820E53F0`'s +11616 were never taken), so the skeleton's step,
the draws and the deleting destructors are "Not seen in play".

Shared with other files, which keep their rows: `Accessories::AllLoaded`
and `Accessories::Release` (`sub_820F07D8`, `sub_820F0A90`,
`scene-loaders.md`); `sub_820F0E50`, the set's getter
(`scene-models.md`); `PartModel::PartModel` and `~PartModel`
(`sub_820CE928`, `sub_820CE990`), `Model::Load`, `Model::MaterialCount`,
`Model::Material`, `Model::RecordShape` and `sub_8259A9E0` (the
skeleton's set-up), in `models.md`; `SkeletonBase::SkeletonBase` and
`~SkeletonBase` (`sub_8259A140`, `sub_8259A9D0`, `model-animation.md`);
`Skeleton::UpdateNodeMatrices` (`sub_8259C3A8`, `models.md`);
`Recorder::RecordObject`, `Recorder::RecordSceneLights` and
`Model::RecordShapeDepth` (`scenery-lists.md`); the command list's
chunks and slots (`command-list.md`); `Archive::Load`, `Archive::Cancel`,
`Archive::AssetSize` and `DataFile::GetEntry` (`files-and-saves.md`);
`VirtualAlloc` and `VirtualFree` (`heap.md`); `memcpy`, `strstr` and
`atan2` (`c-runtime.md`); `Matrix::Copy`, `RotateX`, `RotateY`,
`RotateZ` (`matrix-math.md`); `sub_82094B10` (the job's wait, Wrapped,
`functions.md`). Not read and not seen in play: the skeleton's
`sub_8259ACE0`, `sub_8259AF20`, `sub_8259A400` and `sub_8259B660`, and
the skinning pair `sub_8258AB18` and `sub_82594FD0` under
`Accessory::BindSkin`. Beyond `820F1078` (`820F1090` on) is the course
scenery's manager (`[0x826C5514]`, `course-scenery.md`).

Status:

- **Native**: plain C++ in `itc_recomp/src/` that doesn't use the
  PowerPC context (`ctx`): normal arguments and host structs (it may
  still read and write guest memory through `src/guest.h` while
  recompiled code shares the data). The game reaches it without `ctx`,
  from native or overridden callers, and the recompiled original is no
  longer called (except behind a fallback flag or to check against it).
  No `REX_FUNC` override of it exists, not even an adapter kept only for
  a harness (that's Overridden until the adapter goes).
  The end state for every function.
- **Overridden**: an `extern "C" REX_FUNC(sub_XXXXXXXX)` override in
  `itc_recomp/src/` (or a macro defining one) does the work instead of
  the original: a thin adapter in front of native code, or a body that
  still uses `ctx` (calling guest code, reading registers). It may call
  the original when its flag is off, or to check itself against it.
- **Wrapped**: an override that does something of its own (skipping
  render-only passes, logging, timing) and still calls the original
  (`__imp__sub_XXXXXXXX`) for the work.
- **Dropped**: its work isn't needed on PC (hardware set-up, kernel calls
  the SDK already stubbed), so it no longer runs: an override in
  `itc_recomp/src/` replaces it with nothing (or a fixed return value), or
  its callers, made native, leave the call out (no override then; the row
  says which caller). Logs, a harness or the renderer's frame dumps have
  shown that nothing reads what it did. The original is no longer called,
  except behind a fallback flag while the drop is being checked. A
  function the game never calls in the first place isn't Dropped: its
  status stays blank and its row says it's never called.
- **Merged**: its work is done inside a native caller's plain C++ (a
  few lines only that caller needs), so it has no function or override of
  its own in `itc_recomp/src/`. The row names the caller(s) and the
  harness that covers the work. The original is no longer called, except
  behind the caller's fallback flag.
- **Patched**: the generated code was edited in place (search for
  `PATCH:`).

A function usually moves from recompiled or Wrapped to Overridden to
Native as the `ctx` edge moves up through its callers, or straight to
Dropped when its work isn't needed.
`itc_recomp/tools/check_function_statuses.py` checks this column, in
every file of the tables, against `src/` (an override marked Native, a
status with no override, an override with no row, a row in two files);
run it after changing either, on any branch.

| Address | Name | Status | Notes |
|---|---|---|---|
| `sub_820F1038` | `Accessories::Create` (probably) | | Makes the set once: when `[0x826C5510]` is 0, `sub_820F0F48`'s answer into it. Only caller the scene's constructor `sub_820C8A50` (a lead). Read in full. |
| `sub_820F0F48` | `Accessories::New` (probably) | | `VirtualAlloc(0, 32, 0x801000, 4)`, then `Accessories::Accessories` on it; returns it, or 0 when the allocation fails. Only caller `Accessories::Create`. Read in full. |
| `sub_820F0E60` | `Accessories::Accessories` | | r3 = the set: the base's constructor (`sub_820F07B8`), vtable `0x82006740`, +16 to +28 = 1.0 (`[0x82000A30]`; +28 written twice); returns r3. Only caller `Accessories::New`. Read in full. |
| `sub_820F07B8` | `AccessoriesBase::AccessoriesBase` (probably) | | r3 = the set: vtable `0x8200672C`, +4, +8, +12 (the three entries) = 0. Leaf. Only caller `Accessories::Accessories`. Read in full. |
| `sub_820F0F00` | `Accessories::SetScale` (probably) | | r3 = the set, r4 = a float4: copied to each entry's +1072 (its accessory's scale, +1024) and to the set's +16. Leaf. Only caller the scene's constructor `sub_820C8A50`. Read in full. |
| `sub_820F0EB8` | `Accessories::Acquire` | | Slot 1 of `0x82006740` (no direct callers; `CarInterior::Load`, `scene-loaders.md`, calls the set's vtable +4 with r4 = the record's +112, r5 = its +116): `Accessories::AcquireEntry(r3, r4, r5)`, and when that gives an accessory, the set's +16 (the scale) into its +1024. Returns the accessory or 0. Read in full. |
| `sub_820F09E0` | `Accessories::AcquireEntry` (probably) | | r3 = the set, r4 = the index (3 none: returns 0), r5 = the texture index. No entry at +4 + 4 × r4: `AccessoryEntry::New` stored there (0: returns 0), `Accessory::SetTexture(accessory, r5, -1)`, then `AccessoryFile::Load(entry + 4, 23008 + r4, 0, AccessoryEntry::OnLoaded, entry)`. An entry there: its count + 1 and `Accessory::SetTexture(accessory, r5, -1)`. Returns the entry + 48 (the accessory). Only caller `Accessories::Acquire`. 128-byte frame. Read in full. |
| `sub_820F0AD8` | `Accessories::ReleaseAll` (probably) | | r3 = the set: for each of the three entries that's set, its count − 1, and at 0 `AccessoryEntry::Destroy` on the entry's slot. From `sub_820C4D88` (a lead) and `AccessoriesBase::~AccessoriesBase`. Read in full. |
| `sub_820F0B58` | `AccessoriesBase::~AccessoriesBase` (probably) | | r3 = the set: vtable `0x8200672C`, `Accessories::ReleaseAll`, then vtable `0x82010B5C` (the list node's base, `archive.md`). From `Accessories::~Accessories`'s deleting destructor and `sub_820F0BA0`. Read in full. |
| `sub_820F0BA0` | | | The base's deleting destructor (r4 bit 0: `operator delete`), probably slot 0 of `0x8200672C` (no direct callers). Not seen in play. Read in full. |
| `sub_820F0FE0` | `Accessories::~Accessories` (deleting) | | Slot 0 of `0x82006740` (no direct callers; `Accessories::Destroy` reaches it): vtable `0x82006740`, `AccessoriesBase::~AccessoriesBase`, and with r4 bit 0, `operator delete`. Called with r4 = 0 in play. Returns r3. Read in full. |
| `sub_820F1078` | `Accessories::Destroy` (probably) | | When `[0x826C5510]` is set, a tail call to `sub_820F0F90` with r3 = `0x826C5510`. Only caller `sub_820C9608` (a lead), once the scene is gone. Read in full. |
| `sub_820F0F90` | `Accessories::Free` (probably) | | r3 = the set's slot: when set, the set's vtable slot 0 with r4 = 0 (the destructor without delete), then `AccessoryFile::FreeData` on the slot (`VirtualFree(set, 0, MEM_RELEASE)`, slot = 0). Only caller `Accessories::Destroy`. Read in full. |
| `sub_820F0AD0` | | | Empty (`blr`), between `Accessories::Release` and `Accessories::ReleaseAll`; no direct callers (probably a vtable slot). Not seen in play. |
| `sub_820F0920` | `AccessoryEntry::New` (probably) | | `VirtualAlloc(0, 1104, 0x801000, 4)`; +0 = 1 (the count), +4 `AccessoryFile::AccessoryFile`, +48 `Accessory::Accessory`; returns it, or 0. Only caller `Accessories::AcquireEntry`. Read in full. |
| `sub_820F0820` | `AccessoryEntry::OnLoaded` (probably) | | The entry file's callback (no direct callers; `AccessoryFile::OnArchiveDone` calls it with r6 = the entry): with the file's data ([entry + 20]), `AccessoryFile::SetData` on the accessory's +464 (`DataFile::GetEntry(data, 0, 0)`: the part model), +500 (entry 1: the XPR) and +724 (the skeleton's file, entry 2: the node file), each not owned; then `Accessory::Finish(accessory, 1)` and `Accessory::SetTexture(accessory, [+1040], -1)`. Read in full. |
| `sub_820F0988` | `AccessoryEntry::Destroy` (probably) | | r3 = the entry's slot (set + 4 + 4i): when set, `Accessory::~Accessory` (entry + 48), `AccessoryFile::~AccessoryFile` (entry + 4), then `AccessoryFile::FreeData` on the slot (`VirtualFree(entry, 0, MEM_RELEASE)`, slot = 0). From `Accessories::Release` (`scene-loaders.md`) and `Accessories::ReleaseAll`. Read in full. |
| `sub_820F0C50` | `AccessoryFile::AccessoryFile` | | r3 = a 36-byte file: vtable `0x82006738`, +4 and +8 = -1, +12 to +20 = 0, +24 = 1, +28 and +32 = 0. Leaf. From `Accessory::Accessory` (twice), `AccessorySkeleton::AccessorySkeleton` and `AccessoryEntry::New`. Read in full. |
| `sub_820F0BF0` | `AccessoryFile::SetData` (probably) | | r3 = the file, r4 = data, r5 = owned: +16 = r4, +24 = r5, +20 = 0, +4 and +8 = -1, +12 = 2 (loaded), +28 and +32 = 0. Leaf. Only caller `AccessoryEntry::OnLoaded` (r5 = 0). Read in full. |
| `sub_820F0C90` | `AccessoryFile::Load` (probably) | | r3 = the file, r4 = an asset id, r5 = a buffer or 0, r6 = a callback, r7 = its context: +4 = the id, +20 = `Archive::AssetSize(id)`; with r5, +16 = r5 and +24 = 0, else +16 = `VirtualAlloc(0, size, 0x801000, 4)` and +24 = 1; then +8 = `Archive::Load(id, +16, AccessoryFile::OnArchiveDone, the file, 0, 0)` (the serial), +12 = 1 (loading), +28 = r6, +32 = r7. Only caller `Accessories::AcquireEntry` (ids 23008 to 23010, no buffer). 128-byte frame. Read in full. |
| `sub_820F0C20` | `AccessoryFile::OnArchiveDone` (probably) | | The archive's callback (no direct callers; `AccessoryFile::Load` passes it to `Archive::Load`): r3 = the serial, r6 = the file. Nothing unless the file's +8 is r3; then +12 = 2 (loaded), and when +28 is set, a tail call to it with r6 = +32 (r3 to r5 as given). Read in full. |
| `sub_820F0D30` | `AccessoryFile::Reset` (probably) | | r3 = the file: when owned (+24), `Archive::Cancel(+8)` while loading (+12 = 1; the buffer isn't freed) or `AccessoryFile::FreeData(+16)` once loaded (2); then +12 = 0, +4 and +8 = -1, +16 and +20 = 0, +24 = 1, +28 and +32 = 0. From `Accessory::Finish` (the model's two files) and `AccessoryFile::~AccessoryFile`. Read in full. |
| `sub_820F08D0` | `AccessoryFile::FreeData` (probably) | | r3 = a word holding a block: when set, `VirtualFree(block, 0, 0x8000)` (MEM_RELEASE) and the word = 0. From `AccessoryFile::Reset`, `AccessoryEntry::Destroy` and `Accessories::Free`: a helper for any `VirtualAlloc`'d block. Read in full. |
| `sub_820F0DB8` | `AccessoryFile::~AccessoryFile` | | r3 = the file: vtable `0x82006738`, `AccessoryFile::Reset`, then vtable `0x82010B5C`. From `Accessory::~Accessory` (twice), `AccessorySkeleton::~AccessorySkeleton`, `AccessoryEntry::Destroy` and `sub_820F0E00`. Read in full. |
| `sub_820F0E00` | | | The file's deleting destructor (r4 bit 0: `operator delete`), probably slot 0 of `0x82006738` (no direct callers). Not seen in play. Read in full. |
| `sub_820EF258` | `Accessory::Accessory` | | r3 = the accessory (entry + 48): `PartModel::PartModel` (`models.md`), vtable `0x820066C8`, `AccessoryFile::AccessoryFile` at +464 and +500, `AccessorySkeleton::AccessorySkeleton` at +544; +992 to +1000 = 0.0 and +1004 = 1.0 (the anchor, w = 1), +1008 to +1016 = 0.0 (the angles), +1020 to +1036 = 1.0 (+1004, +1020 and +1036 written twice), +1044 = 1 (visible), +1040 and +1048 = 0. The floats from `[0x82000A30]` (1.0) and `[0x82000A34]` (0.0). Only caller `AccessoryEntry::New`. Read in full. |
| `sub_820EF310` | `Accessory::~Accessory` | | r3 = the accessory: vtable `0x820066C8`, `AccessorySkeleton::~AccessorySkeleton` (+544), `AccessoryFile::~AccessoryFile` (+500, +464), then `PartModel::~PartModel` (`models.md`). From `AccessoryEntry::Destroy` and the deleting destructor `sub_820EF740`. Read in full. |
| `sub_820EF740` | | | The accessory's deleting destructor (r4 bit 0: `operator delete`), probably slot 0 of `0x820066C8` (no direct callers). Not seen in play. Read in full. |
| `sub_820EF368` | `Accessory::Finish` (probably) | | r3 = the accessory, r4 = release the model's files. Nothing once loaded (+1048) or until the model's file is loaded (+476, its state, = 2); then `Model::Load(it, [+480] the part model, [+516] the XPR)`, and with r4, `AccessoryFile::Reset` on +464 and +500 (not owned: nothing freed); then when `AccessorySkeleton::SetUp(+544)` answers 1, the anchor (+992, 16 bytes) = the skeleton's node [+256]'s 16 bytes at [+632] (skeleton +88), and +1048 = 1. Returns +1048. Only caller `AccessoryEntry::OnLoaded` (r4 = 1). Read in full. |
| `sub_820EF568` | `Accessory::SetTexture` (probably) | | r3 = the accessory, r4 = a texture index, r5 = a shape or -1 (all). +1040 = r4; nothing more until loaded (+1048); then the texture provider's (+80) vtable +12 with r4 = 0 (probably its slot 0: the texture set), and only when that answer's +20 (probably its texture count) is above r4: for the shape (or every shape, +412), groups 0 and 1 (the words at `0x820066D0`), each material (`Model::MaterialCount`, `Model::Material`): its 8 words at +176 = r4 and the 8 at +144 = 0. From `Accessories::AcquireEntry`, `AccessoryEntry::OnLoaded` and `sub_820E53F0` (`functions.md`, when +1040 isn't its +156). 160-byte frame. Read in full. |
| `sub_820EF418` | `Accessory::SetAnchor` (probably) | | r3 = the accessory, r4 = a matrix: `AccessorySkeleton::Move(+544, r4)`, then the anchor (+992) = the matrix's row 3 (its translation). Only caller `SceneModel::Update` (`scene-models.md`; identity at the car model's accessory node times the model's matrix). Not seen in play. Read in full. |
| `sub_820EF250` | `Accessory::Step` (probably) | | r3 = the accessory: a jump to `AccessorySkeleton::Step` with r3 = +544 (r4 as given; 0 from its only caller `SceneModel::Update`). Not seen in play. Read in full. |
| `sub_820EFA78` | `Accessory::Draw` (probably) | | r3 = the accessory, r4 = a camera pass (`0x826BAC00` + 592 × i, or 0): waits for the skeleton's job (`sub_820EFB28`), builds the world matrix (`Matrix::FromScaleRotationTranslation(stack, angles +1008, anchor +992, scale +1024)`); when visible (+1044 bit 0): `Accessory::BindSkin(it, [+692], [+652], [[+624] + 4])` (the skeleton's +148, +108 and its node count), then `Accessory::DrawShapes(it, pass + 144, world, 0, 0, 0, 0, -1)`. Only caller `sub_820E53F0` (`functions.md`; only when its +11616 is set). Not seen in play. Read in full. |
| `sub_820EF468` | `Accessory::BindSkin` (probably) | | r3 = the accessory, r4, r5, r6 = the skeleton's matrices, its +108 and the node count: `sub_8258AB18(r4, r5, r6)` (not read), then for each shape (+412) whose +428 (in the 544-byte shapes at +40) is 1, `sub_82594FD0(it, that answer, r6, shape)` (not read; probably skinning). Only caller `Accessory::Draw`. 128-byte frame. Not seen in play. Read in full. |
| `sub_820EF9B8` | `Accessory::DrawShapes` (probably) | | As the model draws in `scenery-lists.md`: r3 = the accessory, r4 = the view-projection, r5 = the world matrix, r6 = a second view-projection or 0, r7 its world matrix, r8, r9 passed on, r10 = a shape or -1 (all; another index draws shape r10 only when it's 0). `Recorder::RecordObject(it, r4, r5)`, `Recorder::RecordSceneLights(it, r5)`, with r6 also `Recorder::RecordObject(it, r6, r7)`; then `Accessory::DrawShape` for each shape with f1 = the first record's +72. Only caller `Accessory::Draw`. 192-byte frame. Not seen in play. Read in full. |
| `sub_820EF790` | `Accessory::DrawShape` (probably) | | As `Model::DrawShape` (`scenery-lists.md`) with its own entries: r3 = the accessory, r4 = a shape, r5 = the object's constants, r6 = the lights', r7 = the second constants, r9 = draw the depth pass too, r10 and a stack word its keys, f1 = the depth. The sort keys `[0x8296B90C]` and `[0x8296B900]` (`../known-data.md`) raised by the shape's +436, each at most 682, and put back at the end. For groups 0 and 1 with materials: `Model::RecordShape(it, shape, group, 1, 0, 0)`, `Accessory::RecordEntry(r5, r6, that)` handed to the render queue (`[0x8296B91C]`'s slot 4; f1 = −depth, group 0's or group 1's key); with r9, also the depth pass's entry (group 0 `Model::RecordShapeDepth`, group 1 `Model::RecordShape` with r6 = 0; `RecordEntry(r7, r6, …)`, f1 = 0.0, r10's or the stack word's key). Only caller `Accessory::DrawShapes`. Not seen in play. Read in full. |
| `sub_820EF4E8` | `Accessory::RecordEntry` (probably) | | r4, r5, r6 = two constant chunks and a shape's record: on the scene's command list (`[0x8296B918]`), `CommandList::OpenChunk`, three 16-byte slots (`CommandList::AllocateSlots`, 1 each): +2 = 3 and +4 = r4; +2 = 3 and +4 = r5; +2 = 4 and +4 = r6; `CommandList::CloseChunk`; returns the first slot. r3 not read. Only caller `Accessory::DrawShape` (4 calls). 144-byte frame. Not seen in play. Read in full. |
| `sub_820EF660` | `Matrix::FromScaleRotationTranslation` (probably) | | r3 = out, r4 = three angles, r5 = a position, r6 = a scale: identity (`memcpy` of a stack identity), then the scale on the diagonal (+0, +20, +40), `Matrix::RotateX`, `RotateY`, `RotateZ` (`matrix-math.md`) by r4's three angles in turn, and the position into row 3 (+48 to +56). From `Accessory::Draw` and `sub_8217CAB8` (the front end's car draw lead, seen in play). 208-byte frame. Read in full. |
| `sub_820F03F0` | `AccessorySkeleton::AccessorySkeleton` | | r3 = the skeleton (accessory + 544): `SkeletonBase::SkeletonBase` (`model-animation.md`), +176 = `0x82010B5C`, then +0 `0x82006700` and +176 `0x820066FC`; `AccessoryFile::AccessoryFile` at +180; the job at +216 (+216 `0x820066DC`, +220 `0x82000AB4` then `0x82002908`, +224 and +228 0, the bytes +232 0, +233 1, +234 0, +240 `0x8200833C`, +244 0, +248 and +252 0); +288 and +352 identity (each row's +12 first set to 1.0, then overwritten by the `memcpy`), +416 to +424 0.0 and +428 1.0, +432 to +440 0.0 and +444 1.0, +256 0, +260 0, +264 1, +268 and +272 0.0. Only caller `Accessory::Accessory`. 224-byte frame. Read in full. |
| `sub_820EFFD8` | `AccessorySkeleton::~AccessorySkeleton` | | r3 = the skeleton: +0 `0x82006700`, +176 `0x820066FC`; the job's destructor inline (+240 `0x82000ABC`, the functors' base; +220 `0x82000BA8`, then `0x82000AB4`; +216 `0x82000EB8`); `AccessoryFile::~AccessoryFile` (+180); +176 = `0x82010B5C`; then `SkeletonBase::~SkeletonBase`. From `Accessory::~Accessory` and the deleting destructor `sub_820F05D8`. Read in full. |
| `sub_820F05D8` | | | The skeleton's deleting destructor (r4 bit 0: `operator delete`). From `sub_820F0068`. Not seen in play. Read in full. |
| `sub_820F0068` | | | r3 − 176, then a jump to `sub_820F05D8`: the deleting destructor through the second base (+176), probably slot 0 of `0x820066FC` (no direct callers). Not seen in play. Read in full. |
| `sub_820F0070` | `AccessorySkeleton::SetUp` (probably) | | r3 = the skeleton. Once (+260 = 0) and when its file (+180) is loaded (+192 = 2): `sub_8259A9E0(it, [+196] the node file, 0, 1)` (`models.md`); then, when the node file's view (+80) has nodes (+4; with none it stays not set up), the first node whose name ([+84] + 160 × i) holds the string at `0x82006710` (`strstr`) into +256 (kept as is when none does), +260 = 1 and `AccessorySkeleton::Reset`. Returns +260. Only caller `Accessory::Finish`. Read in full. |
| `sub_820EFB30` | `AccessorySkeleton::Reset` (probably) | | r3 = the skeleton: for each node (view +4): its 144-byte local entry ([+136] + 144 × i) gets the three 16-byte rows at [+88], [+92] and [+96] (+ 16 × i) at +0, +16, +32, and when the node's flags ([+84] + 160 × i + 132) have bit 0x40, its 16 bytes at +64 at +48. Then +268 and +272 = 0.0, +288 and +352 identity, +416 to +424 0.0 and +428 1.0, +432 to +440 0.0 and +444 1.0, +152 = 0. Only caller `AccessorySkeleton::SetUp`. 192-byte frame. Read in full. |
| `sub_820EFCF8` | `AccessorySkeleton::Move` (probably) | | r3 = the skeleton, r4 = a matrix: `Matrix::Copy(+288, r4)`; before the first step (+152 = 0), also into +352, and +416 to +424 = 0.0, +428 = 1.0. Only caller `Accessory::SetAnchor`. Not seen in play. Read in full. |
| `sub_820F0718` | `AccessorySkeleton::Step` (probably) | | r3 = the skeleton, r4 = a worker pool or 0: sets the job up (+224 and +228 0, the bytes +232 0, +233 1, +234 0, +236 a word from its own frame at r1 + 116, never set, +244 the skeleton, +248 `sub_820F0628` and +252 0); with r4, the pool's slot 1 (r4 = the job, r5 = 1), else `AccessorySkeleton::Update` at once (r4 = +220). From `Accessory::Step` (r4 = 0). 144-byte frame. Not seen in play. Read in full. |
| `sub_820F0628` | `AccessorySkeleton::Update` (probably) | | The job's work, r3 = the skeleton: with nodes (view +4), `sub_8259ACE0` (not read); for the swinging nodes (view +8 of them from index view +12) whose flags (+132) have bit 0x80, the local entry's +20 = the float at `0x82001FB4` + `atan2(+320, +328)` (the swing's angle, probably); then `sub_8259AF20` (not read); with nodes, `Skeleton::UpdateNodeMatrices` (`models.md`); with swinging nodes, `AccessorySkeleton::Swing`. Only caller through the job. 128-byte frame. Not seen in play. Read in full. |
| `sub_820F0120` | `AccessorySkeleton::Swing` (probably) | | r3 = the skeleton: the anchor's movement since the last step (+336 − +400, minus +416) scaled (constants at `0x82006720`, `0x820029F0`, `0x82001BE4`; `pow`, `c-runtime.md`), added into the swing vector (+432); `AccessorySkeleton::SwingNode` for each swinging node; after the first step (+152) with +264 set, the angle (+268) and its speed (+272) stepped from the swing's length and sign, +416 = this step's movement, and `Matrix::Copy(+352, +288)`; then `sub_8259A400` and `sub_8259B660` (not read) and +152 = 1. 176-byte frame. Not seen in play. Read, not in detail. |
| `sub_820EFD70` | `AccessorySkeleton::SwingNode` (probably) | | r3 = the skeleton, r4 = a swinging node's index, r5 = the swing vector: the node's matrix (`Matrix::Copy`) and its point moved by the vector and a constant (probably gravity), then put back at the node's length ([+84] node +140) from its parent (a square root and a divide), probably a chain's point. Only caller `AccessorySkeleton::Swing`. Not seen in play. Read, not in detail (393 lines). |
| `sub_820EFB28` | `AccessorySkeleton::WaitForStep` (probably) | | r3 + 216 (the job), then a jump to `sub_82094B10` (Wrapped, `functions.md`; the job's wait, as `Movie::Play`'s in `movies.md`). Only caller `Accessory::Draw`. Not seen in play. Read in full. |
