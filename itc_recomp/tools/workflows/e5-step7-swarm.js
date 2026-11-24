export const meta = {
  name: 'e5-step7-swarm',
  description: 'E5 step 7 as one swarm: one agent per unit takes what the GPU never reads out of physical memory, and the image loaders\' files into PC memory (edits only; the lead builds and checks after)',
  phases: [
    { title: 'Move', detail: 'one agent per unit; units sharing files never run side by side' },
  ],
}

// args.units: [{ name, loaders, types, hint, files }] (docs/loops/e5-loop.md,
// "Step 7 as one swarm"): `loaders` the functions allocating physical
// memory or loading image files for this unit, `types` its types, `hint`
// what it covers, `files` the hand-port and schema files (the scheduler's:
// two units sharing a file never run at once).
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

const HOW = `Read first: docs/e5-native.md, "7. The renderer without mirrors"; docs/loops/e5-loop.md, "Step 5 as one swarm" (blobs: what they are, storage = "blob", blob_alloc), "Step 6 as one swarm" and "Step 7 as one swarm" (what's done: the physical memory, the GPU's, is mapped on its own outside guest memory, the renderer reads it there, and guest.h translates the guest's addresses for it: 0x7F000000, 0xA0000000, 0xC0000000, 0xE0000000); the groundwork commit (git show 3987bc46) and step 5's blob groundwork (git show 2f23bdfe).
What to do: every physical allocation in your unit (XPhysicalAlloc, XPhysicalAllocEx, MmAllocatePhysicalMemoryEx, memory::AllocPhysical, models::AllocPhysical(Ex), Scenery::AllocPhysical, and the lifted calls to them: game::XPhysicalAlloc...) holds either data the renderer reads (a texture's pixels or a texture pack the renderer reads in place, vertex or index data, shader microcode, a render target or resolve destination, the command buffer, anything whose physical address goes into a fetch constant, a shader object or a draw) or data only the game's code reads (sound buffers our FFmpeg reads through guest::Pointer, lists, records, a file the loader copies the GPU's parts out of). Find out which by reading the code that fills and reads it (and the renderer's reads: src/native/, PhysicalHostAddress and GpuPhysicalAddress). Data the renderer reads stays in physical memory (it's outside guest memory now): leave it, and say so. Data only the game's code reads becomes a blob: in a hand port, memory::NewBlob(size, "<Name>") or a blob type's New(base, size) where it called the physical allocator (keep the alignment the code relies on: a blob is page-aligned), and the matching free (memory::FreePhysical and the game's frees take handles); in lifted code, a blob type in the schema with blob_alloc = [{ call = 0x<return address>, reg = "r3" }] (XPhysicalAlloc's size is r3). Physical memory has no byte-order difference from a blob (both keep the guest's bytes). Watch for code that turns the buffer's address into a physical one (address & 0x1FFFFFFF, or a GPU address) or checks the address range (>= 0xA0000000): a blob's handle isn't physical; such code means the GPU or a physical-only path reads it, so it stays.
For the image loaders (files read whole by Archive::Load into a buffer: XPR texture packs, TGA images): if the texture's pixels are copied out into physical memory before the renderer reads them (XPR::Create copies? read it), the file's buffer becomes a blob; if the renderer reads the file's buffer in place, it stays physical.`

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
`You're one of several agents doing E5 step 7 (the renderer's memory: what the GPU never reads out of physical memory) as one swarm, each for one unit of this game port. Your unit: \`${unit.name}\`: ${unit.hint}.
- Its functions (physical allocations or file loads): ${unit.loaders.length ? unit.loaders.join(', ') : 'see the hint'}.
- Its types: ${unit.types.length ? unit.types.join(', ') : 'none listed'}.
- Your files (held for you while you run): ${unit.files.join(', ')}.
${RULES}
${HOW}

Your work: each allocation and loader in your unit: who fills it, who reads it (the renderer or only the game's code), and where it's freed; move what only the game's code reads into blobs; leave the rest. python3 -I itc_recomp/tools/schema_gen.py --area <area> and schema_check.py <area> for the areas you changed. Then report: what moved (storage "blob", with its allocation and free sites), what stays in physical memory and why (skipped), raw accesses you know remain, the lifted code you judged, proposed row changes, and notes for the lead.`,
  { label: `move:${unit.name}`, phase: 'Move', schema: REPORT }))

const reports = results.filter(Boolean)
log(`${reports.length} of ${UNITS.length} units reported; ${reports.reduce((n, r) => n + r.moved.length, 0)} types moved`)
return {
  units: reports.length,
  missing: UNITS.filter((u, k) => !results[k]).map(u => u.name),
  reports,
}
