# Referencing SBX-Recomp

How information was taken from SBX-Recomp during B4 (files and saves, in
`docs/rexglue-removal.md`), so it can be checked again or used the same way
later. Written 2026-10-21.

## What it is, and why it was read

SBX-Recomp recompiles the Japanese release of the same game with the same SDK.
It started from this project and follows its layout, but the Japanese build was
compiled differently, so the same functions sit at other addresses. Its own
`docs/porting-from-itc.md` describes how the two line up.

It lives beside this repo, at `/home/sam/repos/SBX-Recomp`. CLAUDE.md's rule is
to search only inside this repo; the user made the exception and pointed to it
(2026-10-21, during B4 step 2's survey of the archive streaming), naming its
function-mapping TSV. Its modding work (the mod loader, addon ids, cut cars) is
to be ignored: no mod support here yet (the user's call). Use it only for
function identities and data layouts.

Read at SBX-Recomp commit `1fd0d3d` (2026-10-20, "More Research Notes"). It
changes over time: check `git -C /home/sam/repos/SBX-Recomp log -1` before
relying on line numbers or rows below.

## The two files used

- `docs/itc_to_sbx_functions.tsv`: one row per matched function. Columns:
  `src` (our address, ITC), `dst` (SBX's), `method`, `score`, `src_len`,
  `dst_len` (instruction counts), `shift_ok`, `calls_agree` (calls to already
  matched functions that land on their match, n/n). 13,839 rows. Methods, by
  its porting doc's "Matching": `exact` (identical code with addresses masked),
  `shape` (identical up to registers), `calls` (a callee at the same spot of
  matched callers), `order`/`gap` (between matched neighbors), `local`,
  `target`, `pinned` (checked by hand). Its confidence scale: exact; checked by
  hand; high (score 0.95 and up, or all calls agree with score 0.8 and up);
  otherwise "check".
- `docs/functions.md`: their table of named functions (`| Address | Name |
  Status | Notes |`, the same format as ours), with SBX addresses.

Not used: `docs/porting-from-itc.md` beyond its first sections (how the projects
line up, the images, the matching methods above); its mod docs. Their
`docs/game_data.md`, which several of their rows cite for the data formats, was
never committed (not in any branch at `1fd0d3d`).

## How the information was taken

Their rows are keyed by SBX addresses; ours by ITC's. Each of their rows in an
area of interest was mapped back through the TSV (`dst` to `src`) and printed
with the match's method and score:

```python
# python3 -I map_sbx_rows.py <first line> <last line> (of their docs/functions.md)
import csv, re, sys
SBX = "/home/sam/repos/SBX-Recomp/docs/"
ours = {}
for r in csv.DictReader(open(SBX + "itc_to_sbx_functions.tsv"), delimiter="\t"):
    ours.setdefault(r["dst"], []).append(f'{r["src"]} ({r["method"]} {r["score"]})')
first, last = int(sys.argv[1]), int(sys.argv[2])
for line in open(SBX + "functions.md").read().splitlines()[first:last]:
    m = re.match(r"\| `sub_([0-9A-F]{8})` \| ([^|]*)\|([^|]*)\|(.*)", line)
    if m:
        print(f"SBX {m.group(1)} -> ITC {', '.join(ours.get(m.group(1), ['?']))}"
              f" | {m.group(2).strip()} | {m.group(4).strip()[:300]}")
```

The area was found by searching their docs for the archives
(`grep -niE 'BUILD\.(DAT|TOC)|archive|decompress' docs/*.md`); rows 85-135 of
their `functions.md` held the archive section at `1fd0d3d`. One way only: to go
from one of our addresses to theirs, look up `src` instead.

Every fact taken was checked against our own code (`itc_recomp/generated/`) or
a log before it went into our docs; the match alone wasn't trusted.

## What came from it

All from their `docs/functions.md` archive section, mapped through the TSV:

| Ours | SBX | Match | Taken | Checked by |
|---|---|---|---|---|
| `sub_8209DE48` | `sub_8209DF80` | exact 1.000, calls 2/2 | The name `Archive::Load`; the TOC entry's three words are the start sector, the stored size and the unpacked size; r8 overrides the stored size | Reading our function and `sub_8209D490` (+68 stored, +72 unpacked, +60 sectors); the attract race's trace |
| `sub_8209E210` | `sub_8209E348` | calls 0.967, 21/21 | The name `Archive::Init` | Reading it (B4 step 2) |
| `sub_8209D8D8` | `sub_8209D9F8` | calls 0.966 | The name `Archive::FindLooseFile`, and that it returns a loose file's relative path | The trace: its answer for ids 22887-22889 was a path, and the loose reads' sizes matched the audio banks on disk |
| `sub_8209D3B0` | `sub_8209D4D0` | calls 0.964, 8/8 | The name `Archive::Shutdown` and what it does | Not read by us (its row says "Not read") |
| `sub_82294610` | `sub_82292050` | exact 1.000 | The name `Ucl::Unpack`: UCL in `uclpack`'s layout (an 18-byte header, then blocks of a big-endian unpacked size, a stored size and the data) | Reading it: the header word at +18, the per-block sizes, stored blocks copied; every unpack in play returned 1 (its size check) |
| `sub_822947F8` | `sub_82292238` | exact 1.000 | That it unpacks one UCL block | Its only caller is `Ucl::Unpack`; not read in detail |
| `sub_8209AC78` | `sub_8209AD48` | calls 0.938, 3/3 | That the read error ends in the dirty-disc screen | Not read by us (its row says "Not read") |

Also matched, but they only confirmed what we had found already: the loader
thread `sub_8209E118` (exact), its loose read `sub_8209DFA8` (exact), the
request queue `sub_8209DC18` (exact), the archive read `sub_8209DD00` (calls
0.935).

Our rows that use it say so: "Name from SBX-Recomp", "Name and format from
SBX-Recomp", "SBX-Recomp's notes" in `docs/functions/files-and-saves.md`.

## What didn't come from it

Everything else in B4: that loading continues during races (so loads stay
asynchronous), the request states and the lock protocol around the read's APC,
the cancel, the thread the callbacks run on, the file class's layout and users,
the debug font and audio loaders, the saves. Their notes stop at "queues a read
for the loader thread"; these came from reading `generated/` and the logs
(`docs/rexglue-removal.md`, B4).

## Checking it again

1. `git -C /home/sam/repos/SBX-Recomp log -1`: if it moved past `1fd0d3d`,
   their rows may have changed; rerun the script on the archive section (find
   it again with the grep above).
2. For each row in the table, look the address up in the TSV (`src` column)
   and confirm the method and score.
3. Confirm the fact against our function in `itc_recomp/generated/default/`
   (`grep -n "DEFINE_REX_FUNC(sub_XXXXXXXX)"`); for the two "Not read" rows,
   reading them is still owed.

## Using it later

It should help most at the next level: the load callbacks and parsers that read
the data (their car specs and part tables, BUILD 72-83; shader pairs; containers;
the save's layout). The same rules apply: map through the TSV, prefer exact or
high matches, confirm against our code before writing it down, say in the row
where it came from, and ignore the modding parts.
