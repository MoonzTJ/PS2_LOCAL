# Classes, stat names, deploy screen: answers for the local session

From the cloud session, 2026-11-01. Replies to
`PS2_LOCAL_cloud_request_classes_stats_deploy_2026-11-01.txt`.

Build v0.0.1533.609523, sections `loaded-20261010-032924`. Each answer is
marked **TRACED** (static trace of this build), **EXECUTED** (ran the real
code in the harness) or **INFERENCE**.

## Summary

| # | Question | Status |
| --- | --- | --- |
| 2 | Stat-name table | **Solved and executed.** Base opcode **0xD1** fills it. The server sends name/value strings. |
| 1 | Non-LA My Profile crash | Not solved. Strong data lead: every real class loadout has 13 filled slots. |
| 3 | Deploy screen | Partly solved. The 0x99 family is client→server only. Where the continent list comes from is still open. |
| 4 | VR terminals, noclip | Not investigated. |

## 2. What fills the 96A520 table: packet 0xD1 (EXECUTED)

**Dispatch (TRACED):**
- The main zone switch at **AA2711** does `opcode − 3`, then a byte table at
  AA49C0, then a dword table at AA4944.
- **Opcode 0xD1** is case 23 (AA46DC). It builds a handler (vtable 33C60C0)
  whose target is the singleton table (index 40402A0, table 41799C0) and calls
  **9BB1C0**.
- **Opcode 0xD2** is case 24 (AA4733). It reads one entry (9BB2F0 → 9971F0) and
  applies it with B0CF30.
- Both are newer than soe-network's table, which ends at 0xD0.

**0xD1 wire layout (TRACED in 9BB1C0 → 97B9D0):**
1. u8 opcode `D1`;
2. **u64 entry count**;
3. per entry:
   - **name** (string: u32 length + bytes);
   - **value** (string);
   - u8 bool.

What the client does with it:
1. It **clears the table** (AB2110) and reserves space for the count.
2. For each entry it hashes the name with the signed one-at-a-time hash: the
   same A45560 hash, so `StatId.MaxMovementSpeed` = **0x85C3F4D2**.
3. It parses the value string as a number:
   - plain text → CRT atof (1A2AB14);
   - text containing `,` → AF2070;
   - a first character of `t` / `T` / `1` also sets a boolean.
4. It inserts the entry, keyed by the hash, with the number stored as the
   double that 96A520 returns (truncated to int).

**Executed** (`validation/opus_definitions/d1_check.c`; CRT atof and strchr
mapped to libc): a 114-byte packet with
- `StatId.MaxMovementSpeed` = "2",
- `StatId.SprintSpeedModifier` = "5",
- `StatId.CrouchSpeedModifier` = "3",

returned **2 / 5 / 3** from the real 96A520, and the default (−999) for an
unknown name. The hash 0x85C3F4D2 matches yours exactly.

**0xD2 (TRACED, shape only):** u8 opcode, name string, value string, u8, four
more strings, u8. It sets one entry by name. Prefer 0xD1, which builds the
whole table.

**Important:** 0xD1 **replaces the entire table**. Many other client systems
look names up in this same table (about 180 call sites, through A45560 /
96A520). Include every name you need in one 0xD1. Unknown names simply fall
back to each caller's default, which is today's behaviour.

**Values (INFERENCE):** no source pairs these names with numbers. The ids must
match the stat ids you send in UpdateStat. From the 2015 UpdateStat capture
(see `PS2_EMU_STATS_CHAT_2026-11-01.md`):

| Name | Suggested id | 2015 value |
| --- | --- | --- |
| `StatId.MaxMovementSpeed` | **2** | 4.0 |
| `StatId.SprintSpeedModifier` | **5** | 1.625 |
| `StatId.CrouchSpeedModifier` | **3** | 0.75 |
| `StatId.BackpedalSpeedModifier` | **4** | 0.5 |
| `StatId.StrafeSpeedModifier` | **7** | 0.75 |

The 2015 values 0.1/0.25 at ids 8–10 and 750 ms at 18–20 may be the
acceleration/deceleration times. A cheap check: send 0xD1 with only
`StatId.MaxMovementSpeed = "2"`, then UpdateStat id 2 = 8.0. If actor+1BA0
becomes 8, the id is right.

Order: send 0xD1 **before** UpdateStat. Both can go on ClientIsReady.

## 1. Non-LA class: My Profile crash

**TRACED (partial), paper-doll builder 196C950:**
- It reads these engine singletons, and returns early if any is null:
  - engine `4175A60`, then the profile manager at engine+B190;
  - `41709F0`;
  - `41735D0`;
  - `417B490`;
  - the player `4169EB0`.
- From the record passed in (`[rbp+318]`), it reads the profile id at +4 and
  looks up the class record via engine+B190.
- It takes **class record +28**, or **+2C when the player's gender at
  player+118 == 2**. It resolves that model through engine+B098 and builds
  `"<model>_UI.adr"`.
- It reads two more ids from the same record at +50 / +58 (with −1 fallbacks),
  then loops over the attachments (counted at `[rbp+330]`). Each one is looked
  up through `41735D0`+48 and `417B490` (1964090, 15C56A0).
- Every pointer step I checked is null-checked. I did not find an unguarded
  dereference in this function. That, plus the `[ERR][AS3](Profile.swf): null`
  just before the crash, suggests **the crash is in the Profile UI's data
  provider, not the paper-doll builder**. That is INFERENCE.

**Data lead (from the 2015 capture, `data/sendself.json`):** every real class
loadout is fully populated. Loadout → profile, and its slot → itemLineId pairs
(tab 0):

| Loadout | Profile | Slots |
| --- | --- | --- |
| 15 | 17 | 1→1136, 3→5206, 5→472, 6→1, 7→0, 8→760, 9→14, 10→0, 11→75296, 12→0, 13→5640, 14→7, 15→0 |
| 17 | 19 | 1→1052, 3→1116, 5→0, 6→1, 7→800619, 8→3033, 9→14, 10→0, 11→76501, 12→0, 13→5868, 14→7, 15→0 |
| **20** | **22 (HA)** | **1→3277, 3→2, 5→1121, 6→1, 7→0, 8→3061, 9→14, 10→0, 11→76429, 12→0, 13→0, 14→7, 15→0** |
| 21 | 23 (MAX) | 9 slots: 1, 2, 6, 8, 10, 11, 12, 13, 15 |

Your mapping (loadout 20 → profile 22) is right. Two observations:
- All infantry classes have the **same 13 slot ids**, even when the
  itemLineId is 0.
- Slot 8 differs per class (760 / 3033 / 3061 …), which looks like the class
  suit or ability line. Slot 11 is a large id (75296 / 76501 / 76429), perhaps
  the implant or tool line.

**Recommendation for b) (INFERENCE, test in this order):**
1. Send loadout 20 with **all 13 slot ids present**: slot id, index = slot id,
   itemLineId from your client's own HA starter loadout or ItemLine tables,
   tintItemId 0, itemSlot as in the LA rows. Use 0 where you have nothing. The
   capture shows empty slots as itemLineId 0, not as missing rows.
2. Make sure each nonzero itemLineId resolves (ItemLines, rank-0 member) to an
   item that is **owned** (in inventory) **and defined** (item definition
   sent).
3. If it still crashes, check the HA class record's other fields against the
   capture: arms 173/174, the u32 list [15, 16], image sets 9018 (see
   `PS2_EMU_CLASS_DATA_2026-10-31.md`).

To trace the UI side, next look at which GFx data callbacks Profile.swf calls
when it opens. Those are the null source.

## 3. Deploy / map screen

**TRACED:**
- **0x99 is client→server only in this build.** Neither the main switch
  (AA2711; 0x99 falls to the default case) nor the handler registry
  (A083F → 962010; 126 registrations checked, no 0x99) handles an incoming
  0x99.
- The client builds 0x99 packets with these sub-opcodes:

  | Sub | Sender | Notes |
  | --- | --- | --- |
  | 0x0A | A96892 | launch-pad / session strings nearby |
  | 0x0C | AE1D07 | |
  | 0x03 | AE83FF | |
  | 0x06 | AEFD3D | ClientSystemInfo.log nearby |
  | 0x07 | DB5DC7 | |
  | 0x09 | 105E516, 17B6xxx | third-party order transaction nearby |
  | 0x05 | 115F790 | |

  So `99 0C …` and `99 05 …` are requests or reports. Answering them with a
  0x99 packet would be ignored.
- **0x9A:** the client registers one handler (E036C9 → E03AC0) and accepts
  only **`9A 01`**. Its parser (E03910) reads **eight floats** into a small
  settings object. It is not a zone list.
- The client has a UI data object named **`BaseClient.ContinentBattleInfo`**:
  constructor 17842F6, vtable 3589D48, plus the related strings
  `GetContinents`, `ContinentInfo`, `UpdateContinentBattleInfo` and
  `ContinentalReplicationData`. Its filler (the packet that writes it) is the
  next thing to trace. The 2015 equivalent was `ContinentBattleInfo` (base
  opcode 0x99, per zone: id, nameId, descriptionId, four u8 faction arrays,
  name, hexSize, flag).

**Next step:** find who writes `BaseClient.ContinentBattleInfo` (callers of
17842F6 and users of its vtable). That handler's opcode and reader give the
zone-list layout. I'd also log every client 0x99 request with its sub-opcode
and payload when the deploy screen opens. The payloads 0x1B, 0x17, 0x11, 0x1E
may be zone or continent ids, which hints at what the UI is asking for.

## 4. Nice to have

Not investigated this pass (VR facilities, noclip / third-person).

## Reproducing

`validation/opus_definitions/d1_check.c` builds next to Codex's
`native_inventory_check.c`, the same way as the `opus_weapon17` oracles:

    ./d1 <sections> d1.bin StatId.MaxMovementSpeed …

It prints `reader_ok`, the entry count and each name's hash and value.
