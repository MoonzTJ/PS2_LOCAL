# Class switch without relog, and NPC health

From the cloud session, 2026-11-01. This replies to Moon's "trace both" (the
two open items in CLOUD_NPC_MODEL_REMOVE §4). It is static analysis plus
harness runs on `loaded-20261010-032924`.

Tags: **TRACED** = read in the client code; **EXECUTED** = run in the harness;
**INFERENCE** = my reading, not proven.

New builder: `validation/opus_npc/build_events.py` (`remove_player`,
`resource_set`, `activate_profile`). Every packet it builds was run through the
client's own reader, with 0 parse errors.

## A. Class switch in place: `ClientUpdate.ActivateProfile` = `11 15 00`

### What the client does with it (TRACED, ClientUpdate case 0x15 → A4D1F8)

ClientUpdate subs are u16 (A4BA53), so the header is `11`, `15 00`. After a
successful parse, the handler:

1. **parses the embedded profile bytes into the player's profile list**
   (97B600). This is the same function that reads profile records in another,
   larger packet reader (call at 989279), most likely SendSelf's profile list.
   The record is 0xF8 bytes (constructor BA4F20, reader 98A3D0, **102 bytes
   when all-zero**). If a profile with the same id exists, it is replaced;
2. gets the local player (C08AD0) and applies the profile id to the
   character-select / profile system (11E5880);
3. looks up the profile entry by **u32 #1** (BBB390) and marks it current
   (sets entry +0x1A8 and flag bits in +0x70);
4. **if u32 #3 differs from the player's current model (player+0x17AC):**
   1. sets the new model (vtable +0x858, B9D0E0);
   2. re-applies the **attachment array** (vtable +0x720 with the
      player-record attachment lists at +0x7C860 / +0x7C890).

   **That is the in-place body swap.**

There is no relog and no zone reload in this path.

### Wire layout (EXECUTED, reader 98BD00, constructor 9CE140)

| Wire | Field |
| --- | --- |
| u8 `11`, u16 `15 00` | opcode, sub |
| u32 length + bytes | **profile record** (one profile, as in SendSelf's profile list) |
| u32 count + elements | **attachments**. Each element: 4 strings (soe: modelName, string, tintAlias, string), then **4 × u32** (soe had 3; the last is probably slotId). **32 bytes when empty** |
| u32 | **#1 → record +0x44: the profile id to activate** (BBB390). INFERENCE for the name; TRACED use |
| u32 | #2 → +0x40 |
| u32 | **#3 → +0x48: the actor model id**, compared with player+0x17AC and applied if different (TRACED) |
| u32 | #4 → +0x4C |
| string, string | +0x50, +0x68 |
| u8 | +0x80 |

The sizes match:
- **all-zero body = 36 bytes**;
- with a 102-byte zero profile = 138;
- with one attachment (`Armor_VS_Male_LightAssault_Base.adr`, slot 1) =
  **205**. That run read back #1 = 19 and #3 = 165, with parse error 0.

### Suggested test (INFERENCE where noted)

1. For `!class <profile>`, send `11 15 00` with:
   - the **profile bytes your SendSelf writes for that profile id**, using the
     same profile-record writer;
   - attachments: the class armour (model name + slot), or none for the first
     test;
   - **#1 = the profile id** (for example 19 LA, 22 HA);
   - **#3 = that class's model word for the character's gender** (165 male LA,
     173 HA, …);
   - all other fields 0 / empty.
2. If the body changes but the weapons don't, follow with the class loadout:
   - `86 03` SetCurrentLoadout. Its reader takes 0x86, 0x03, a type byte, a
     mode byte, then loadoutId, tabId and a u32. Mode 1 applies the loadout
     (D2F680); mode 0 calls D2F420 with no arguments.
   - ps2-emu answered the client's class change with SetCurrentLoadout (type 2,
     loadoutId, tabId 256, last u32 1).
   - Then your equipment / weapon packets for the new kit.
3. Your v10–v13 ActivateProfile probes predate the ClientGameSettings, 0xD1,
   class-record and model-word fixes. Those are exactly what this path relies
   on (gender pattern, class records), so a retry is meaningful now.

## B. Making the target shootable / showing health

### Health is a character **resource**: opcode **0x8D ResourceEvent** (TRACED + EXECUTED)

- There is no health replication class. None of the 30 `*ReplicationData`
  classes is health; NpcReplicationData's Create payload is 112 bytes when
  all-zero, with no named health field.
- The client keeps health in **ClientCharacterResourceManager**, which logs
  "RemoveCharacterResource [local] / [target]", so it holds resources for
  **other** characters too. The resource types come from ResourceTypes.txt in
  the client data.
- Its packet is registry opcode **0x8D** (handler C56890, vtable 34053E0, next
  to those strings). That is **the same opcode and shape as H1Z1's
  ResourceEvent**.

**Header (TRACED):** u8 `8D`, u32 gameTime, **u8 eventType**, u64
characterId. The event types:
1. SetCharacterResources (C56C80);
2. **SetCharacterResource** (C56B10);
3. C56EB0, probably the update;
4. C569F0, probably remove.

**Type 2 body (EXECUTED, the real parser C52130; it returns its error flag,
0 = OK):**

| Wire (after the 14-byte header) | Record | Field (H1Z1 names where the order fits; INFERENCE) |
| --- | --- | --- |
| 0 | u16 → +0x00 | **resourceId** |
| 2 | u8 → +0x04 | **resourceType** (H1Z1: HEALTH = 1; check ResourceTypes.txt) |
| 3 | u32 count + elements | array (9B0C10), send 0 |
| 7 | u32 → +0x20 | **max_value** |
| 11 | u32 → +0x24 | **initial / current value** |
| 15, 19 | float, float → +0x28, +0x2C | |
| 23, 27 | two counted fields | send 0, 0 |
| 31–50 | 5 × u32 → +0x5C … +0x6F | 0 |
| 51, 52 | 2 bytes → +0x70, +0x71 | 0 |

- Minimum and recommended body: **53 bytes**, so the **whole packet is 67
  bytes**.
- Verified: characterId 0x4000000000000001, resourceId 1, type 1, max 1000,
  value 1000; error 0 and every value read back.
- `build_events.resource_set(guid, resource_id, resource_type, max_value,
  value)` builds it.

**To test:**
1. Send it right after the 0F 55 for that guid.
2. Then turn on the NPC hitpoint bar: the client script command
   **ToggleNpcHitpointBar** flips engine flag +0xB7EC, which only the UI reads.
3. If no bar appears, look at ResourceTypes.txt for the health resource's id
   and type, and try those values.

### Damage / death loop (not traced in this build; INFERENCE)

In the 2015 protocol, hit detection is client-side. The client reports hits to
the server (soe Weapon.ProjectileHitReport, family 0x85). The server lowers the
target's health resource (another 0x8D) and removes or kills the target at 0.

The client→server hit-report opcode in this build isn't in the handler
registry (that only lists server→client). Please **log every inbound packet
while firing at the target**. Once a shot works, the opcode will show up.

`0F 03` (UpdateHitpoints in soe naming) only carries a guid here (reader
9BF3E0). It looks up the actor and calls B90320. It is not a hit-point value
update.

## Summary of new packets

| Packet | Bytes | Builder | Status |
| --- | --- | --- | --- |
| `0F 01` RemovePlayer | 12 | `remove_player` | EXECUTED reader |
| `8D` ResourceEvent type 2 (health) | 67 | `resource_set` | EXECUTED parser |
| `11 15 00` ActivateProfile | 36 + profile + attachments | `activate_profile` | EXECUTED reader; handler TRACED |
| `86 03` SetCurrentLoadout | 2 + 1 + 1 + 3 × u32 | (simple) | TRACED |

## Questions for the local session

1. ActivateProfile: with the profile record your SendSelf writes for profile 22
   and #3 = 173, does the body change to HA in place? Any crash RVA?
2. Health: after the 67-byte 0x8D for the target's guid, does a health bar
   show (with ToggleNpcHitpointBar)? What do ResourceTypes.txt's first rows say
   (id / name / type)?
3. While firing at the target, which inbound opcodes does the client send?
