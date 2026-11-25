export const meta = {
  name: 'e5-step3-swarm',
  description: 'E5 step 3, one batch: one agent per area moves its private types to host memory (edits only; the lead builds and checks after)',
  phases: [
    { title: 'Move', detail: 'one agent per area: its movable types to storage = "host", their allocation and free sites, their users' },
  ],
}

// args.areas: this batch's areas (docs/loops/e5-loop.md, "Step 3 as a
// swarm"). args.lifted_sites: true once lift.py replaces allocation and free
// calls in lifted code (the schema's host_alloc and host_free); before
// that, a type allocated or freed in lifted code is skipped.
const AREAS = args.areas
const LIFTED_SITES = !!args.lifted_sites

// At most 6 agents at once (the user's call for step 2, kept).
const CONCURRENCY = 6
async function pool(items, fn) {
  const out = new Array(items.length).fill(null)
  let next = 0
  async function worker() {
    while (next < items.length) {
      const k = next++
      try { out[k] = await fn(items[k], k) } catch (e) { out[k] = null }
    }
  }
  await Promise.all(Array.from({ length: CONCURRENCY }, worker))
  return out
}

const RULES = `
The repo root (where CLAUDE.md is), branch e5-native-16-11-26 (stay on it). Several agents edit this working tree at the same time, one per area, so these rules are strict:
- Never build (no cmake, no ninja), never run the game, never run git commands that change anything, never run tools/lift.py, tools/lift_originals.py, tools/type_map.py or the full tools/schema_gen.py (only: python3 -I itc_recomp/tools/schema_gen.py --area <your area>, and python3 -I itc_recomp/tools/schema_check.py <your area>).
- Edit only your own files: itc_recomp/schema/<area>.toml and your area's hand-port files (itc_recomp/src/<area with _>/ and itc_recomp/src/hooks/<area with _>.cpp/.h, or the files whose functions are your area's rows). A change needed in another file goes in your report. Never hand-edit itc_recomp/src/schema/ or itc_recomp/src/lifted/ (generated). Don't change docs/functions/ rows: propose row changes in your report (the user reviews rows before they're written).
- If the Edit tool says a file changed since you read it, read it again and redo the edit.
- American English. Nothing the player can see may change: the same game, tick for tick, on the same input (the lead checks with the state digest). Where an object lives and how it's allocated may change (the user's call, docs/e5-native.md "Decided").`

const PILOT = `How a move goes: docs/loops/e5-loop.md, "Step 3: private objects to the host" and its "The pilot" (read them first), and the pilot's commit (git show 98123e00: Type2DialogData; read-only git is fine). In short: the type gets storage = "host" in its schema file; its view then holds a handle (memory/handles.h) and reads a host struct (schema_gen.py's host_type: Data members, accessors, _bits(), New, Delete; no _address()); where the original allocated it, the code calls schema::<Type>::New(base) and keeps the handle where it kept the address (a global, a field, a register it returns); where the original freed it, <view>.Delete(), returning what the free returned (VirtualFree and HeapFree: 1). Lifted code reaching it through typed loads and stores is re-emitted by the lead (lift.py turns them into _bits() calls); a raw access the tools couldn't type faults on the handle when reached and names the type.`

const REPORT = {
  type: 'object',
  properties: {
    area: { type: 'string' },
    moved: {
      type: 'array',
      items: {
        type: 'object',
        properties: {
          type: { type: 'string' },
          alloc_sites: { type: 'string', description: 'function and call site of each allocation (and how the handle is kept)' },
          free_sites: { type: 'string', description: 'function and call site of each free, or "never freed"' },
          files: { type: 'array', items: { type: 'string' } },
        },
        required: ['type', 'alloc_sites', 'free_sites', 'files'],
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
    lifted_concerns: { type: 'string', description: 'lifted functions that reach a moved type in ways lift.py may not handle (an address passed on, memcpy, an untyped access) and why you judged them safe, or "none"' },
    row_changes: { type: 'string', description: 'docs/functions rows you propose changing, in the table format, or "none"' },
    for_lead: { type: 'string', description: 'changes needed in files not yours, risks, decisions for the user (brief)' },
  },
  required: ['area', 'moved', 'skipped', 'lifted_concerns', 'row_changes', 'for_lead'],
}

const LIFTED_RULE = LIFTED_SITES
  ? `An allocation or free in lifted code (itc_recomp/src/lifted/, a function with no hand port) is replaced by lift.py from your schema file: list it in the type's host_alloc and host_free (see docs/loops/e5-loop.md, "lift.py's host_alloc and host_free", for the form) instead of editing lifted code.`
  : `If any allocation or free of a type is in lifted code (itc_recomp/src/lifted/, a function with no hand port: check src/lifted/entries.h for an entry e<address>), skip the type ("allocated in lifted code"): lift.py can't replace those calls yet.`

phase('Move')
const results = await pool(AREAS, area => agent(
`You're one of several agents doing a batch of E5 step 3 (private objects to host memory) at once, each for one area of this game port. Your area: \`${area}\` (docs/functions/${area}.md).
${RULES}
${PILOT}

Your work:
1. List your area's movable types: python3 -I itc_recomp/tools/e5_movable.py ${area} (their sizes, allocation sites, and the functions touching them that also make accesses the measure couldn't type). Skip the movie library's types if any (a replaced library: it never runs).
2. For each type, read its schema entry (itc_recomp/schema/${area}.toml), the rows of every function touching it (docs/functions/), and the code: the allocation sites (VirtualAlloc, HeapAlloc, operator new, XMemAlloc and the like: who calls them with this type's size), every place its pointer is kept (a global, another object's field, a frame word), every free, and every user. Grep the hand ports (itc_recomp/src, not schema/ or lifted/) for schema::<Type>, its global's and holder field's names, and the addresses of its allocation and free call sites.
3. Decide whether it can move. Skip it (and say why) when:
   - ${LIFTED_RULE}
   - its memory is used as memory: memcpy or memset over it, a file read into it, written to a save, read by the GPU, or handed to code in another area or to an import that reads it raw;
   - it's one block holding several objects (an array allocated at once): New makes one object;
   - a lifted function among the "untyped accesses" ones (step 1) may reach it raw (read the lifted function in src/lifted/ to judge; a raw access faults on the handle, so a wrong judgment stops the game: when unsure, skip);
   - its fields overlap (schema_gen refuses a host type whose fields overlap).
4. Move each type you keep: storage = "host" in its schema entry (with a comment: since E5 step 3, where it's made and freed, where its handle is kept); its allocation sites call schema::<Type>::New(base) instead (initial values the original wrote stay as setter calls, in the same order); its frees call Delete(). Users that took its guest address (an _address() accessor, offsets from its pointer) go through the view instead. Every hand-port mention of the type must compile with the host view (no _address(); New and Delete; the same accessors otherwise): grep for them.
5. python3 -I itc_recomp/tools/schema_gen.py --area ${area} (it must accept the host types) and python3 -I itc_recomp/tools/schema_check.py ${area} (0 accesses that fit no field).
Then report: what moved (with its allocation and free sites), what you skipped and why, the lifted code you judged, proposed row changes, and notes for the lead.`,
  { label: `move:${area}`, phase: 'Move', schema: REPORT }))

const reports = results.filter(Boolean)
log(`${reports.length} of ${AREAS.length} areas reported; ${reports.reduce((n, r) => n + r.moved.length, 0)} types moved`)
return {
  areas: reports.length,
  missing: AREAS.filter(a => !reports.some(r => r.area === a)),
  moved: reports.flatMap(r => r.moved.map(m => ({ area: r.area, ...m }))),
  skipped: reports.flatMap(r => r.skipped.map(s => ({ area: r.area, ...s }))),
  lifted_concerns: reports.filter(r => r.lifted_concerns && r.lifted_concerns !== 'none').map(r => ({ area: r.area, note: r.lifted_concerns })),
  row_changes: reports.filter(r => r.row_changes && r.row_changes !== 'none').map(r => ({ area: r.area, rows: r.row_changes })),
  for_lead: reports.filter(r => r.for_lead && r.for_lead.length > 3).map(r => ({ area: r.area, note: r.for_lead })),
}
