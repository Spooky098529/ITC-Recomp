export const meta = {
  name: 'e5-step5-swarm',
  description: 'E5 step 5 as one swarm: one agent per unit (a format\'s loaders with their areas\' objects) moves the game\'s files into PC memory (edits only; the lead builds and checks after)',
  phases: [
    { title: 'Move', detail: 'one agent per unit; units sharing files never run side by side' },
  ],
}

// args.units: [{ name, loaders, types, hint, files }] (docs/loops/e5-loop.md,
// "Step 5 as one swarm"): `loaders` the functions calling Archive::Load for
// this unit's files, `types` the allocated objects of its areas, `hint` what
// it covers, `files` the hand-port and schema files naming them (the
// scheduler's: two units sharing a file never run at once).
const UNITS = args.units

// At most 6 agents at once (the user's call), and never two units sharing a
// file: a unit starts when a slot is free and none of its files is held.
const CONCURRENCY = 6
async function schedule(items, fn) {
  const out = new Array(items.length).fill(null)
  const held = new Set()
  const waiting = items.map((_, k) => k)
  let running = 0
  return await new Promise(resolve => {
    function pump() {
      if (!waiting.length && !running) { resolve(out); return }
      for (let i = 0; i < waiting.length && running < CONCURRENCY; ) {
        const k = waiting[i]
        if (items[k].files.some(f => held.has(f))) { i++; continue }
        waiting.splice(i, 1)
        items[k].files.forEach(f => held.add(f))
        running++
        Promise.resolve().then(() => fn(items[k], k)).catch(() => null).then(r => {
          out[k] = r || null
          items[k].files.forEach(f => held.delete(f))
          running--
          pump()
        })
      }
    }
    pump()
  })
}

const RULES = `
The repo root is where CLAUDE.md is; branch e5-native-16-11-26 (stay on it). Several agents edit this working tree at once, one per unit, so these rules are strict:
- Never build (no cmake, no ninja), never run the game, never run git commands that change anything (read-only git such as git show and git log is fine), never run tools/lift.py, tools/lift_originals.py, tools/type_map.py or the full tools/schema_gen.py. Only: python3 -I itc_recomp/tools/schema_gen.py --area <area>, python3 -I itc_recomp/tools/schema_check.py <area>, python3 -I itc_recomp/tools/e5_movable.py <area>, and read-only commands.
- Never hand-edit itc_recomp/src/schema/ or itc_recomp/src/lifted/ (generated), itc_recomp/tools/, or docs/functions/ rows (propose row changes in your report: the user reviews rows before they're written). itc_recomp/src/harnesses/ isn't built any more: leave it alone.
- Your unit's files (below) are yours for this run: no other agent holds them. Another file may be edited by another agent at the same time: edit one only when your unit needs it, keep the edit small, and if the Edit tool says the file changed since you read it, read it again and redo the edit. Touch only your unit's types' schema entries.
- American English. Nothing the player can see may change: the same game, tick for tick, on the same input (the lead checks with the state digest, tools/tick_replay.sh). Keep the same loads and stores in the same order; where an object lives and how it's allocated may change (the user's call, docs/e5-native.md "Decided").`

const HOW = `Read first: docs/loops/e5-loop.md, "Step 4: shared objects" (from "Groundwork: done" on: the pilot, batch 1, "Step 4 as one swarm" and "The swarm: done") and "Step 5 as one swarm"; the step 5 pilot's commit (git show 4f7d8d9f: the track's mesh and route files made blobs, two schema edits) and the blob groundwork (git show 2f23bdfe). What you have:
- storage = "blob" for a file's data or another buffer the game uses as raw bytes: the block lives in PC memory behind a handle (memory::NewBlob), its bytes in the guest's byte order, and everything reaching it behaves as on guest memory: guest::Load*/Store* by address, guest::Pointer (gives the blob's bytes), memcpy and memset, the archive's reads (they write through guest::ForEachHostPart, which uses guest::Pointer), the game's frees. Its view is a guest view (by address) with New(base, size) and Delete(). Pointers the game fixes up inside a file become values inside the blob's handle span: they resolve the same way. In hand ports, allocate it with schema::<Type>::New(base, size) (or memory::NewBlob(size, "<Type>")) where the original allocated the buffer; in lifted code, list the call in the blob type's schema entry: blob_alloc = [{ call = 0x<return address>, reg = "r4" }] (the register holding the byte size at the call: r4 for VirtualAlloc, r3 for operator new and malloc, r5 for HeapAlloc; the return address is LIFTED_ARGS's first argument, the instruction after the bl). A buffer type without a schema entry gets one (a name, size 0, origin "allocated", storage = "blob", no fields).
- storage = "host" or "dual" for ordinary objects, as in step 4 (New/Delete at hand-port sites, host_alloc/host_alloc_array/host_free for lifted ones, embed fields; a generic type or one also embedded in guest owners is dual). A host or dual view reads its own members only on its own type's handle and goes by address through the owner's layout otherwise, so views of objects inside other owners are safe.
- Raw accesses aren't fatal: every guest::Load*/Store* reaches host objects and blobs through their handles.
What stays in guest memory in step 5 (leave it, say so): anything the renderer reads (vertex and index data, textures and their pixels, shader microcode, XPR texture packs, the command buffer: step 7; the renderer reads guest memory by address through its mirrors, never through handles), physical allocations (XPhysicalAlloc, MmAllocatePhysicalMemory: GPU memory), and kernel objects the SDK's imports read. A file whose buffer mixes data the renderer reads with data only the game's code reads stays guest as a whole unless the loader already splits it. Data our own host code reads through a raw pointer (FFmpeg reading XMA through src/audio/xma_stream.h, the movies) works with a blob only when that code reads through guest::Pointer, guest::ForEachHostPart or guest loads; check it, and convert a base + address read to guest::Pointer where needed (that's the same bytes for guest memory).`

const REPORT = {
  type: 'object',
  properties: {
    unit: { type: 'string' },
    moved: {
      type: 'array',
      items: {
        type: 'object',
        properties: {
          type: { type: 'string' },
          storage: { type: 'string', description: 'blob, host, dual, or embed (stays a view inside its owner)' },
          alloc_sites: { type: 'string', description: 'function and call site of each allocation (hand port New/NewArray, or schema host_alloc), and where the handle is kept' },
          free_sites: { type: 'string', description: 'function and call site of each free, or "never freed"' },
          files: { type: 'array', items: { type: 'string' } },
        },
        required: ['type', 'storage', 'alloc_sites', 'free_sites', 'files'],
      },
    },
    skipped: {
      type: 'array',
      items: {
        type: 'object',
        properties: { type: { type: 'string' }, why: { type: 'string' } },
        required: ['type', 'why'],
      },
    },
    raw_left: { type: 'string', description: 'raw accesses to moved types you know remain (lifted functions, other files), which the runtime will log, or "none known"' },
    lifted_concerns: { type: 'string', description: 'lifted functions that reach a moved type in ways the tools may not handle (an address passed to an import, guest::Pointer, vector loads) and why you judged them safe, or "none"' },
    row_changes: { type: 'string', description: 'docs/functions rows you propose changing, in the table format (four cells, exactly as written), or "none"' },
    for_lead: { type: 'string', description: 'changes needed in files you left alone, risks, what to look at in the build or the replay (brief)' },
  },
  required: ['unit', 'moved', 'skipped', 'raw_left', 'lifted_concerns', 'row_changes', 'for_lead'],
}

phase('Move')
const results = await schedule(UNITS, unit => agent(
`You're one of several agents doing E5 step 5 (the game's files into PC memory) as one swarm, each for one unit of this game port. Your unit: \`${unit.name}\`: ${unit.hint}.
- Its file loaders (functions calling Archive::Load, sub_8209DE48, with the buffer it reads into): ${unit.loaders.length ? unit.loaders.join(', ') : 'none listed (find the buffers your unit\'s files are read into)'}.
- Its areas' allocated objects (the measure tags them "file" or "save" only because loader or save areas touch them: most are ordinary objects): ${unit.types.length ? unit.types.join(', ') : 'none listed'}.
- Your files (held for you while you run): ${unit.files.join(', ')}.
${RULES}
${HOW}

Your work:
1. For each loader: its row (docs/functions/), its code (the hand port in itc_recomp/src, else src/lifted/batch_*.cpp or originals.cpp; grep src for the address), where its buffer is allocated (and its size register), what the archive reads into it, who reads the data after (fix-ups, the game's code, the renderer through a resource or fetch constant made from an address inside it, our FFmpeg), and where it's freed. Make the buffer a blob unless the renderer or a raw host pointer that can't take a handle reads it.
2. For each object: as step 4 (python3 -I itc_recomp/tools/e5_movable.py <its area> for its allocation sites and users): host or dual with its allocation and free sites, embed fields for what's inside it; or a blob when it's a raw buffer. Skip what the renderer reads.
3. python3 -I itc_recomp/tools/schema_gen.py --area <area> for each area you changed and python3 -I itc_recomp/tools/schema_check.py <area>.
Then report: what moved (blob, host or dual, with its allocation and free sites), what stays in guest memory and why, raw accesses you know remain, the lifted code you judged, proposed row changes, and notes for the lead.`,
  { label: `move:${unit.name}`, phase: 'Move', schema: REPORT }))

const reports = results.filter(Boolean)
log(`${reports.length} of ${UNITS.length} units reported; ${reports.reduce((n, r) => n + r.moved.length, 0)} types moved`)
return {
  units: reports.length,
  missing: UNITS.filter((u, k) => !results[k]).map(u => u.name),
  reports,
}
