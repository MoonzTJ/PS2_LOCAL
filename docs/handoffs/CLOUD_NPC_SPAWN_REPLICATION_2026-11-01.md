# NPC spawn (0F 55), replication (0x57), melee: answers for the local session

From the cloud session, 2026-11-01. Replies to
`PS2_LOCAL_cloud_registry_and_melee_2026-11-01.txt`. Your registry table made
this possible. Marked TRACED / EXECUTED / INFERENCE.

## 1. NPC / ProxiedCharacter spawn = PlayerUpdate `0F 55` (TRACED)

**Correction:** A50769 is **not** in 0x41. 0x41 (A4F950) is a 12-entry
sub-switch (table A4FE10) whose cases all end by A4FE0x, and none calls into
A50xxx.

**0x0F = PlayerUpdate** (A50F70):
- the wire starts `0F | sub`;
- **111 sub-opcodes**, `sub − 1` indexes the jump table at **A57028**;
- each case builds a packet struct, runs its reader, then calls a handler.

**Sub `0x55`** (case A51214):
1. constructor 9D1590;
2. reader **998560**;
3. handler **A50570**, which contains the `Failed to spawn ProxiedCharacter
   GUID(%llu) Name(%s) FactionId(%d)` log at A50769.

This build's PlayerUpdate numbers are 2015's **+3**:

| This build | Reader → handler | 2015 equivalent |
| --- | --- | --- |
| `0F 54` | 998CF0 | AddLightweightPc (0F 51) |
| **`0F 55`** | 998560 → A50570 | **AddLightweightNpc** (0F 52) |
| **`0F 56`** | 999260 → A50830 | **AddLightweightVehicle** (0F 53). Its reader is 998560 + u64 + u32 + u32 + position update (994660) + string, exactly 2015's vehicle shape |
| `0F 5A` / `0F 5B` | 99A5F0 / 99AD30 | related (2015 0F 56/0F 57 LightweightToFull…) |

### `0F 55` wire layout

From reader 998560, in order (TRACED; the names come from aligning with
soe-network's 2015 `lightWeightNpcSchema`, which matches field for field):

| Wire | Field | Struct offset |
| --- | --- | --- |
| u8 `0F`, u8 `55` | opcode, sub | |
| u64 | **guid** | +0x18 |
| packed uint | **transientId** (2-bit-length varint, as in 2015; reader 966990) | +0x20 |
| 8 raw bytes | new in this build | +0x24 |
| u32 | nameId | +0x2C |
| u8 | | +0x30 |
| u32 | | +0x34 |
| u32 | **modelId** | +0x38 |
| float × 4 | **scale** (1, 1, 1, 1) | +0x40 |
| string | | +0x50 |
| string | | +0x68 |
| u32 | | +0x98 |
| float × 3 | **position** | +0x9C |
| float × 4 | vector | +0xB0 |
| float × 4 | **rotation** | +0xC0 |
| u32 | | +0xD0 |
| string × 3 | | +0xD8, +0xF0, +0x108 |
| u32 × 3 | vehicleId, u32, **npcDefinitionId** | +0x120 … +0x128 |
| u8 | | +0x12C |
| u32 | **profileId** | +0x130 |
| 2 raw bytes | | +0x1A4 |
| u8 | | +0x134 |
| u32 | | +0x138 |
| u64 | guid | +0x140 |
| sub-struct (994EA0) | begins with a u64 | +0x150 |
| u32 × 3 | | +0x190 … +0x198 |
| float | | +0x19C |
| u32 | | +0x1A0 |

Not executed: its constructor pulls in CRT heap/thread initialization the
harness doesn't emulate yet. The static field list may miss a field the
extractor didn't recognize. Verify by sending one packet and reading
LightweightProxy.log / AssetFailure.log.

ps2-emu's working example (`zonepackethandlers.js` 858–930) used:
- nameId 0;
- two u32 = 242919 / 310060;
- the byte = 1;
- the model id;
- scale (1, 1, 1, 1);
- position = the player's;
- rotation (−π/2, 0, 0, 0)…;
- then LightweightToFullNpc.

**Remove:** 2015's `PlayerUpdate.RemovePlayer` was `0F 01` with nested sub-ids
(`0F 01 00 00`: guid u64). Sub 1 in this build's table is case A5xxxx; check it
from A57028 entry 0.

**Hittable (INFERENCE):** health, shield and damage state come as replicated
components on the spawned object (section 2). Spawn first, then attach a
health component through a 0x57 Create.

## 2. Replication `0x57` (TRACED + EXECUTED)

Handler 13504B0. Wire `57 | sub`; `sub + 1` indexes the jump table at
**1351CD0**:

| Sub | Meaning (from its log strings) | Reader (struct at handler rbp−0x10) |
| --- | --- | --- |
| 0 | **Create** | 1336410 |
| 1 | Destroy | 966990… |
| 2 | Update | 1336AC0 |
| 3 | UpdateFull | 1336990 |
| 4 | component op | 1336110 |
| 5 | destroy component by class id | 1336790 |
| 6 | destroy component by hash | 13361F0 |

Each reader starts at the opcode byte.

**Create wire (EXECUTED, real reader 1336410):**

| Wire | Field | Struct offset |
| --- | --- | --- |
| u8 `57`, u8 `00` | opcode, sub | |
| u32 | **repId** | +0x18 |
| u8 | **kind** (passed to the component's deserializer) | +0x1C |
| u32 | **data type** = component class id | +0x20 |
| packed uint | **owner transientId** | +0x24 |
| u8 | flag | +0x34 |
| rest of packet | **payload** (pointer +0x28, length +0x30) | |

Example: `57 00 | 44 33 22 11 | 55 | 99 88 77 66 | 0C | P …` gives repId
0x11223344, kind 0x55, type 0x66778899, transientId 3, flag 'P', with the rest
as payload.

**Handler logic (TRACED):**
1. Find the owner by transientId in the actor context (C09160 on singleton
   4178640), or the local player if player+0x20 == transientId.
2. Unknown owner: log "…ReplicationCreatePacket for transientId=%u, but
   object is not known", and stop.
3. Create the component with factory **134E3A0**(type, repId, owner guid). It
   returns component + 0x38, an interface.
4. Attach it to the owner (145D510). A duplicate gives "Failed to add
   replication id … type already exists".
5. Deserialize the payload with interface `vtable+0x28`(reader, kind).
   Leftover bytes are logged as "had %d bytes remaining".

**Class ids = signed one-at-a-time hash of the class name** (the A45560
hash):

| Class | Id | Notes |
| --- | --- | --- |
| **ContinentalReplicationData** | **0x4E963FD0** | factory case 134EBAB: 0x6F8-byte object, constructor 133F290, interface vtable 34EA2E8 |
| BaseReplicationData | 0xE9BB72BD | |
| AbilityCountdownReplicationData | 0xA61610C6 | |
| DrawReplicationData | 0x788EC6FD | |

The other class names sit in the getter table at 1350240–13502E9.

**ContinentalReplicationData payload (EXECUTED, real constructor + real
reader; 33 bytes when all-zero):**

| Payload bytes | Field |
| --- | --- |
| 0–12 | 13 raw bytes → object +0x58 … +0x64 |
| 13–16 | u32 count → array at +0x78, **16-byte** elements |
| 17–24 | 8 bytes → +0x6C8 |
| 25–28 | u32 count → array at +0x6D0, **8-byte** elements |
| 29–32 | u32 count → elements not mapped (a count of 1 with zero bytes sets the error flag, so an element has its own required content) |

Which of these feed which of the 34 ContinentBattleInfo columns is not yet
mapped. The 13 raw bytes and the per-faction arrays are the likely carriers;
the consumer at 17D8836 compares per-faction triplets.

**Owner problem:** the Create needs an **owner object that already exists**
(by transientId). For continents that is probably a world/zone object spawned
first (an NPC-like object via `0F 55`, or a special world object). INFERENCE:
send one owner object, then one Create per zone with type 0x4E963FD0.

Tools: `validation/opus_definitions/rep_check.c` (Continental payload) and
`probe2_check.c` (probe with an optional `CTOR=` env var and IAT stubs).

## 3. Weapons

**Melee crash (TRACED from your dump):** the equipped knife has no current
fire group/mode, because no weapon definition exists for it. This is the same
cause as the carbine before.
- Your starter mapping: **knife item 19 → weapon key 9**.
- The 2015 capture for weapon 9: animation set "Knife", melee detect 0.15 ×
  0.1, **no ammo slots**, fire groups **[1, 7]** → modes [1] and [39].
- Add weapon 9 with groups 1 → [1] and 7 → [39], plus fire-mode records 1 and
  39, the same way as weapon 17 (see `PS2_EMU_WEAPON17_2026-11-01.md` and
  `validation/opus_weapon17/build.py`).
- In item 19's inventory detail, use **0 ammo pairs**, groups 1 and 7 with
  their modes, and **selected 0**.
- Do the same for every starter weapon: pistol 21 → 13, grenade 44705 → 75,
  and so on. Otherwise switching to any of them hits the same NULL.

**Carbine model not visible (INFERENCE):** the carbine now has valid state but
no visible model. The model comes from its item definition MODEL_NAME
(Weapon_VS_Carbine007.adr, usage 3 on bone R_weapon), attached once the
equipment slot is active. Check AssetFailure.log for that ADR, and check that
the item's ACTIVE equip slot matches the slot you equip. Also check the
first-person arms: weapons attach to the arms' weapon bone in first person.
Send a dump or log if it persists.

## Questions for the local session

Please answer these in your next request (numbers help):

1. Send one `0F 55` (layout in section 1) for a target dummy at (586.6, 467.2, -212.2). What do LightweightProxy.log and AssetFailure.log say? Does it appear?
2. If the client rejects or misreads `0F 55`, send a dump at the moment it is handled (A51214 → A50570).
3. For a hittable target: which component class names show up for NPCs in your client (the getter table at 1350240-13502E9)? Is there a health or damage one we should try?
