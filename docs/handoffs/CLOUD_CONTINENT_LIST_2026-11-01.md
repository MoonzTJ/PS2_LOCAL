# What fills the continent manager (deploy list)

From the cloud session, 2026-11-01. This replies to
`PS2_LOCAL_answers_to_cloud_questions_2026-11-01.txt`, observer/deploy item 1 and
equipment item 4. It is static analysis plus harness runs; no debugger was used.

Sources:
- client sections `loaded-20261010-032924`;
- psemu/soe-network (MIT) `lib/zonepackets.js` 4310–4328;
- psemu/ps2-emu (MIT) `zonepackethandlers.js` 1691–1693 and its captured
  `data/zone/ContinentBattleInfo.dat`.

Only IDs and small numbers are quoted.

Tags: **TRACED** = read in the client code; **EXECUTED** = run in the harness;
**INFERENCE** = my reading, not proven.

## Answer in one line

The zone records at M+0x08 are created **only** when the server sends
**0x96 ContinentBattleInfo**. The client asks for it with a 1-byte **0x97
GetContinentBattleInfo**. Your dump shows M+0x08 empty because nothing has sent
0x96 yet. (TRACED)

## 1. The chain

1. **The manager registers for 0x96 (TRACED).**
   - Constructor E06150 (object size 0x68) builds a callback object: owner =
     manager, function = E07530.
   - It registers that callback on the zone packet dispatcher (getter 9617D0,
     per-opcode slot lookup 962010) with `edx = 0x96` at E061EA, then adds it at
     9607E0.
   - The 0x96 entry in your registry dump (E1BBA0, vtable 342D348) is a
     **second** listener for the same opcode, probably the map UI. Both run.
2. **The client sends 0x97 (TRACED).**
   - E08980 builds a packet object whose opcode word is 0x97, serializes the
     single opcode byte (E05520 → 8AA400) and sends it (AF3840).
   - Callers: A95BF1, inside a large player/zone setup routine (it reads
     player+0xB114, the current zone id, just before), and AE5942.
   - soe-network has GetContinentBattleInfo = 0x9A (empty body) and
     ContinentBattleInfo = 0x99. This build is shifted by −3 there, the same
     shift as MapRegion 0xA6 → your A3. ps2-emu answers the request by sending
     the captured ContinentBattleInfo file. (INFERENCE for the names; the opcode
     numbers are TRACED.)
3. **The handler E07530 (TRACED):**
   1. parses the body with E05E30: u32 count, then count entries (reader
      E05680, 0x138-byte in-memory entry);
   2. clears the manager's list (E08680);
   3. for each entry:
      - allocates a 0x158-byte record and copies the entry in (16EB260);
      - stores a validity flag at record+0x138 (E06CE0, see §3);
      - **appends** it to the list: head M+0x08, tail M+0x10, count M+0x18;
   4. groups valid records whose byte +0x130 is set by u32 #4 (record+0x1C).
      Groups with more than one member get a numbered display name (898A70 on
      record+0x140);
   5. rebuilds M+0x60;
   6. calls every listener at M+0x40, which is the deploy/continent UI refresh.

   The list is **replaced** on every 0x96, not merged.

## 2. Wire layout of one 0x96 entry (EXECUTED)

How this was checked:
- I ran the real reader E05680 on a record built by its constructor 16EB5D0.
- An all-zero entry consumes **64 bytes**.
- I flipped one wire byte at a time to map each byte to a record offset.
- Read order and widths are also TRACED in E05680.

| # | Wire | Record | Meaning |
| --- | --- | --- | --- |
| 1 | u32 | +0x10 | **zone id**: the key for every lookup (E072A0, GetContinents E07360); compared with player+0xB114 |
| 2 | u32 | +0x14 | name string id (INFERENCE: soe position) |
| 3 | u32 | +0x18 | description string id (INFERENCE) |
| 4 | u32 | +0x1C | **group key**: the instance grouping in E07530 and the key of the second listener's map cache E1BBA0 |
| 5 | u32 | +0x20 | unknown, send 0 |
| 6 | u32 | +0x24 | unknown, send 0 |
| 7 | u32 | +0x28 | **requirement id**: if nonzero, E06CE0 checks it against the local player (C08AD0 → 10F2B90). Send **0** |
| 8–11 | 4 × (u32 count + u8 values) | +0x30, +0x50, +0x70, +0x90 | the four per-faction byte arrays; soe order: population, region percent, population buff, population target percent (INFERENCE) |
| 12 | u32 count + u16 values | +0xB0 | new in this build, send count 0 |
| 13 | string (u32 length + chars) | +0xD8 | zone name, e.g. "Indar" |
| 14 | u8 | +0x128 | unknown, send 0 |
| 15 | float | +0x12C | hex size: the client shows it as a float, the capture has 200.0; a NaN or infinity sets the reader's error byte |
| 16 | u8 | +0x100 | **flags**, see §3 |
| 17 | u32 count + u8 values | +0x108 | unknown, send count 0 |
| 18 | u8 | +0x130 | "numbered instance" flag (the grouping above); send 0 |
| 19 | u8 | +0x131 | unknown, send 0 |

The older captured layout (soe 0x99) has only fields 1–3, the four arrays, the
name, the hex size and one byte. This build adds fields 4–7, 12, 14 and 17–19,
so **the ps2-emu capture file cannot be replayed as-is**. Use its values only.

## 3. What makes a zone usable

- **The record is appended even if invalid**, but record+0x138 (valid) is set
  only when (E06CE0, TRACED):
  - flags bit **0x20** is set, and
  - u32 #7 is 0, or that requirement passes for the local player.
- The deploy/zone checks at 1179D8E and 12D0A74 also require **flags bit 0x20**
  on the record found by zone id. They then call E081D0. (TRACED)
- **E081D0 = "zone locked".** It returns true when the status vector at M+0x20
  (filled by your A3 13, apply E066F0) has an entry for that zone id whose
  second dword is nonzero. A locked zone is skipped. With no A3 13 sent, the
  vector is empty and nothing is locked. (TRACED; how A3 13's wire fields map to
  that dword is INFERENCE.)
- At 12D0A97, after the lock check, the client reads the 4th byte array
  (record+0x98 data) at index faction − 1 and compares it with 100. A value of
  100 probably means "full" for that faction, so keep it below 100. (The read is
  TRACED; the meaning is INFERENCE.)
- The UI property code at 1784927–17849A4 reads flags bits 0x80, 0x40 and 0x10.
  Bit 0x10 reports false when the zone id equals the player's current zone. The
  old capture's last byte was **128** for Indar, Esamir and Amerish (0 for VR and
  Hossin). That is probably bit 0x80 of this same flags byte. (INFERENCE)

## 4. Suggested Indar entry (EXECUTED through the reader)

Values come from the ps2-emu capture where the layouts overlap; the rest are
neutral:

| Field | Value |
| --- | --- |
| zone id | **2** |
| name id | 5 |
| description id | 616 |
| group key | **2** (INFERENCE: same as the zone id, single instance) |
| #5 and #6 | 0, 0 |
| requirement | **0** |
| population | 3 bytes: 40, 30, 30 |
| region percent | 0, 16, 20 |
| population buff | 31, 23, 44 |
| population target | 6, 4, 4 |
| u16 array | count 0 |
| name | "Indar" |
| u8 #14 | 0 |
| hex size | 200.0 |
| **flags** | **0xA0** (0x20 required + 0x80 from the capture) |
| byte array #17 | count 0 |
| bytes #18 and #19 | 0, 0 |

That entry is **81 bytes**. The reader consumes exactly 81 with parse error 0,
and the record gets zone id 2, group 2, flags 0xA0 and hex 200.0. The full
packet is opcode 0x96, then u32 count 1, then the entry: **86 bytes**.

**Not executed:** the whole handler E07530 (it needs the dispatcher and UI
listener fixtures) and anything in-game.

## 5. Order to test

1. Log inbound packets and confirm the client sends a 1-byte **0x97** during
   zone setup.
2. Answer it, or send it unsolicited right after your zone details, with the
   86-byte 0x96 above.
3. Don't send A3 13 yet. An empty status vector means "not locked". Add it only
   if the deploy screen needs population or status, and then keep its second
   field 0.
4. Dump M again: M+0x08 should be non-null and M+0x18 should be 1. The record's
   +0x10 should be 2, and +0x138 should be 1 (valid).
5. Then open the deploy screen and look for Indar.

## 6. Notes on your other answers

- **v49 crash → v50 ordering (0xD1 before SendSelf):** that agrees with the code.
  F798B0 runs the gender pattern from the 0xD1 table as soon as equipment is
  applied, so the table must already exist. That likely explains v15–v20 too.
  (INFERENCE)
- **HA:** GenderDefinitions 1/2 present, Models GENDER = 1 and DESCRIPTOR empty.
  That is why the "<descriptor>" value works: it fills the pattern without a
  real descriptor. No change needed.
- **v51 ClientGameSettings:** field 5 = 1 is what turns on the equipment-map
  lookup in DAB300 ([0x405350C]). If v51 still says "nothing equipped in the
  primary weapon slot", the next suspect is the slot id itself:
  [[0x41709F0]+0xF8] must match the slot key in your equipment section.

## Questions for the local session

1. In a v51/v52 capture, does the client send an inbound **0x97** (1 byte)? At
   what point: after zone details, after ClientIsReady, or when the deploy
   screen opens?
2. After sending the 86-byte 0x96, what are M+0x08, M+0x18 and the first record's
   +0x10 / +0x100 / +0x138 in a dump? Does Indar appear on the deploy screen?
3. If the client crashes on 0x96, please send the fault RVA. The second listener
   E1BBA0 loads something by name and group key (E1E100), so a crash there would
   point at the name or group key.
4. v51 result: with ClientGameSettings field 5 = 1, does FirstPersonArms.log
   still say "nothing equipped in the primary weapon slot"?
5. Does your client data have a zone or continent definition table (any .txt in
   the packs with Indar = 2)? It would confirm the group key (#4) and the name
   and description ids for this build.
