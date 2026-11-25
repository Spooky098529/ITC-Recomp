export const meta = {
  name: 'e5-step4-swarm',
  description: 'E5 step 4 as one swarm: one agent per unit (an allocated owner with its embedded types) moves it to host memory (edits only; the lead builds and checks after)',
  phases: [
    { title: 'Move', detail: 'one agent per unit; units sharing files never run side by side' },
  ],
}

// args.units: [{ name, types, embedded, areas, files }] (docs/loops/e5-loop.md,
// "Step 4 as one swarm"): `types` the allocated owners, `embedded` the types
// found inside them (embed fields, "Split from" notes, T@+N measure names),
// `files` the hand-port files and schema files naming them (the scheduler's:
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

const HOW = `Read first: docs/loops/e5-loop.md, "Step 4: shared objects" (from "The ground" to the end: the groundwork, the pilot and "Batch 1: done") and "Step 4 as one swarm"; the pilot's commit (git show ae77718d: AppStates and StateLayer) and batch 1's (git show 89257ed8 --stat, then the parts for a type like yours), and the step 4 swarm groundwork commit (git log -1 --grep "swarm groundwork"). What you have:
- storage = "host" in a type's schema entry: its view holds a handle (memory/handles.h) and reads a host struct laid out at the guest offsets (tools/schema_gen.py's host_type). storage = "dual": the view tells a handle from a guest address at each access; use it for types with instances in both shapes (generic containers, a generic getter's parameter type, a type embedded in guest owners as well as host ones, a type the measure joins with stack or image objects).
- Views of host and dual types keep the guest views' accessors, including <field>_address() (address + offset: an interior handle, which every guest::Load*/Store* resolves), plus <field>_bits(), load_at/store_at, New(base), NewArray(base, count) (an array allocated at once, one handle; at(i) is element i), and Delete() (frees the block; 0 does nothing).
- An embedded object is an embed field of its owner ({ name = "...", offset = N, type = "embed", to = "<Type>" }, count for arrays): its view is the owner's address + offset, from either shape. A type embedded in a host owner must be host or dual itself (schema_gen checks).
- Allocation and free sites: in hand ports (itc_recomp/src outside schema/ and lifted/), call schema::<Type>::New(base) or NewArray and keep the handle where the address was kept; Delete() where it was freed (VirtualFree and HeapFree return 1). In lifted code (a function with no hand port: src/lifted/batch_*.cpp and originals.cpp), list the call in the type's schema entry instead: host_alloc = [0x<return address>], host_alloc_array = [{ call = 0x<return address>, reg = "r3" }] (the register holding the byte size: r3 for operator new and malloc, r4 for VirtualAlloc, r5 for HeapAlloc), host_free = [{ call = 0x<return address>, reg = "r3", returns = 1 }] (returns left out for operator delete and free). The return address is LIFTED_ARGS's first argument at the call (the instruction after the bl). A shared deleting destructor needs nothing: the game's frees tell handles from guest addresses.
- Raw accesses are no longer fatal: every guest::Load*/Store* (hand ports and lifted code alike) reaches a host object through its handle, logged once per type and offset ("handles: a raw N-byte access at +X of a T"). Prefer views in the hand ports you touch, but a raw access left in lifted code or a file outside your unit isn't a reason to skip.
- Virtual calls: lifted code reads vtable words through lifted::VtableWord, the hand ports through guest::VtableWord (both take handles). memcpy, memmove, memset, XMemCpy and XMemSet take handles on either side (memory/host_copy.h: host to host a byte copy, host and guest each field swapped).
What still stops a type (skip it, with the reason and the sites): host bytes reached through guest::Pointer or another raw host pointer into guest memory (a handle has no guest bytes; look for guest::Pointer, memcpy into a host buffer from guest memory, std::memcpy with base + address); kernel objects inside it handed to the SDK's imports (events, critical sections, semaphores: the SDK reads them in guest memory); a file read into it in place, a save written from it, the GPU or the renderer reading it (steps 5 and 7); a value an import or the SDK reads raw. An embedded type tagged file, gpu or save stops only itself if the owner can keep it as guest memory elsewhere; usually it stops the owner: say so.`

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
          storage: { type: 'string', description: 'host, dual, or embed (stays a view inside its owner)' },
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
    row_changes: { type: 'string', description: 'docs/functions rows you propose changing (status Host where every object a function touches is now host), in the table format, or "none"' },
    for_lead: { type: 'string', description: 'changes needed in files you left alone, risks, what to look at in the build or the replay (brief)' },
  },
  required: ['unit', 'moved', 'skipped', 'raw_left', 'lifted_concerns', 'row_changes', 'for_lead'],
}

phase('Move')
const results = await schedule(UNITS, unit => agent(
`You're one of several agents doing E5 step 4 (shared objects to host memory) as one swarm, each for one unit of this game port's types. Your unit: \`${unit.name}\`.
- Its allocated owners: ${unit.types.join(', ')} (areas: ${unit.areas.join(', ')}; docs/functions/<area>.md).
- Types the measure found inside them: ${unit.embedded.length ? unit.embedded.join(', ') : 'none listed (look for objects the code reaches at an owner + offset: machines, screens, lists, interfaces carved out of the block)'}.
- Your files (held for you while you run): ${unit.files.join(', ')}.
${RULES}
${HOW}

Your work:
1. For each owner: python3 -I itc_recomp/tools/e5_movable.py <its area> (its allocation sites, the functions touching it, those with untyped accesses); its schema entry and the entries of what's embedded in it; the rows of every function touching them (docs/functions/); and the code: every allocation (VirtualAlloc, HeapAlloc, operator new, XMemAlloc and the like with this size), where its pointer is kept, every free, every user, hand ports (grep itc_recomp/src outside schema/ and lifted/ for schema::<Type>, its global's and holder field's names, the allocation and free call sites' addresses) and lifted code (grep src/lifted for the allocating call's return address and the functions touching it).
2. Find what's embedded: objects reached at the owner's address + offset (the "Split from" notes, T@+N measure names, constructors called on owner + N, interfaces at +4 or +16 handed out). Add embed fields for them in the owner's entry and make each embedded type host (only inside this owner) or dual (also inside guest owners, or a generic type).
3. Decide what moves (see "What still stops a type"). Move what you can: storage, embed fields, the allocation and free sites (hand ports New/NewArray/Delete; lifted host_alloc/host_alloc_array/host_free), users that took a guest address through the view or <field>_address(). Every hand-port mention of a moved type must compile with the host view: grep for them. Initial values the original wrote stay as setter calls, in the same order.
4. python3 -I itc_recomp/tools/schema_gen.py --area <area> for each area you changed (it must accept the types) and python3 -I itc_recomp/tools/schema_check.py <area> (no new accesses that fit no field).
Then report: what moved (with its allocation and free sites), what you skipped and why, raw accesses you know remain, the lifted code you judged, proposed row changes, and notes for the lead.`,
  { label: `move:${unit.name}`, phase: 'Move', schema: REPORT }))

const reports = results.filter(Boolean)
log(`${reports.length} of ${UNITS.length} units reported; ${reports.reduce((n, r) => n + r.moved.length, 0)} types moved`)
return {
  units: reports.length,
  missing: UNITS.filter((u, k) => !results[k]).map(u => u.name),
  reports,
}
