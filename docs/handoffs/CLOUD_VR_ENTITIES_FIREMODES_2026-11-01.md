# VR entities (0F 55), fire modes, Map.gfx strings

From the cloud session, 2026-11-01. This replies to:
- `PS2_LOCAL_cloud_request_vr_entities_2026-11-01.txt`;
- `PS2_LOCAL_cloud_request_fire_modes_2026-11-01.txt`;
- `PS2_LOCAL_cloud_map_gfx_strings_2026-11-01.txt`.

Client sections are `loaded-20261010-032924`.

Tags: **TRACED** = read in the client code; **EXECUTED** = run in the harness;
**INFERENCE** = my reading, not proven.

## 1. `0F 55` AddLightweightNpc: executed layout and samples

**The reader now runs (EXECUTED).** The constructor still can't be emulated,
but the reader 998560 runs on a zeroed record with two fixes:
- 5 OS import slots stubbed;
- the name-hash default 8D8CE0 hooked.

The new oracles are in `validation/opus_npc/`:
- `probe3_check.c`: probe2 plus the 8D8CE0 hook;
- `probemap2.py`: the byte-map runner;
- `build_npc55.py`: the packet builder.

Results:
- **An all-zero body is 180 bytes, parse error 0.** That count includes the
  two opcode bytes `0F 55`: the reader itself reads them (into +0x08 and
  +0x10).
- Every wire byte was flipped once to map it to its record offset. This
  confirms and corrects the static list in CLOUD_NPC_SPAWN_REPLICATION §1.

### Wire layout (EXECUTED offsets; names are INFERENCE unless marked)

| Wire | Width | Record | Field |
| --- | --- | --- | --- |
| 0, 1 | u8, u8 | +0x08, +0x10 | `0F`, `55` |
| 2 | u64 | +0x18 | **guid** |
| 10 | packed uint, 1–4 bytes | +0x20 | **transientId** (see below) |
| next | 8 raw bytes | +0x24 | unknown, send 0 |
| next | u32 | +0x2C | nameId (text id) |
| next | u8 | +0x30 | probably **factionId** (h1z1-860 puts a u8 faction just before the model) |
| next | u32 | +0x34 | unknown |
| next | u32 | +0x38 | **modelId** (TRACED by the earlier static read) |
| next | 4 floats | +0x40 | **scale** |
| next | string, string | +0x50, +0x68 | unknown (h1z1: texture, string13) |
| next | u32 | +0x98 | unknown |
| next | 3 floats | +0x9C | **position** |
| next | 4 floats | +0xB0 | unknown vector |
| next | 4 floats | +0xC0 | **rotation** |
| next | u32 | +0xD0 | unknown |
| next | 3 strings | +0xD8, +0xF0, +0x108 | unknown |
| next | u32 ×3 | +0x120, +0x124, +0x128 | vehicleId, unknown, **npcDefinitionId** |
| next | u8 | +0x12C | positionUpdateType (h1z1 name) |
| next | u32 | +0x130 | **profileId** |
| next | 2 bytes | +0x1A4 | unknown |
| next | u8 | +0x134 | unknown |
| next | u32 | +0x138 | unknown |
| next | u64 | +0x140 | second guid |
| next | u64 | +0x150 | **sub-struct 994EA0: send 0** |
| next | u32 ×3, float, u32 | +0x190 … +0x1A0 | unknown |

Encoding notes:
- **transientId** uses a 2-bit length code in the low bits; value = raw >> 2.
  - Below 64 it is 1 byte (value × 4).
  - **100 is 2 bytes: the value 0x191** (100 × 4 + 1), little-endian.
- **Strings** are a u32 length plus bytes. A length of 0 is accepted.
- **Sub-struct 994EA0 (TRACED + EXECUTED):**
  - When its leading u64 is **0**, it is only those 8 bytes; the client fills
    in defaults.
  - When it is nonzero, it continues with two 9ABD70 values, a u32 and a
    992210 value. That path crashes the harness, so its size is unconfirmed.
  - **Send 0.**

### Samples (EXECUTED through 998560)

Both samples use:
- guid 0x4000000000000001;
- transientId 100;
- position (586.6, 467.2, −212.2), rotation (0, 0, 0, 1), scale (1, 1, 1, 1);
- faction byte 2;
- all other fields 0 / empty strings.

| Model | Size | Parse error | Read back |
| --- | --- | --- | --- |
| 165 (soldier body) | **181 bytes** | 0 | all fields above as set |
| 221 (command console) | **181 bytes** | 0 | all fields above as set |

To get the exact bytes, run `build_npc55.py 165 2` or `build_npc55.py 221 2`
from `validation/opus_npc/`; it prints the length and hex. I'm not pasting the
blob here.

**About the name "Target":** no string in this record is obviously the display
name. The handler's failure log prints a "Name", but I haven't pinned which
field feeds it. nameId (+0x2C) is a text id. Send 0 there and leave the
strings empty for the first test. (INFERENCE)

### 2. What can make A50570 fail ("Failed to spawn ProxiedCharacter") — TRACED, partial

The handler:
1. builds a fresh record (constructor 9D2C80);
2. reads the packet (AD9620);
3. passes it through C14ED0 / ADC200;
4. calls the spawn routine 2220480 with about 15 arguments built from the
   record;
5. calls vtable +0x3E0 on the result. If the spawn returns nothing, it logs
   GUID / Name / FactionId to `#|LightweightProxy.log`.

I haven't traced what 2220480 needs. The likely requirements, by analogy with
the player path (INFERENCE):
- the model id is in the model manager (Models.txt);
- a faction from 1 to 3;
- the guid is not already in use.

npcDefinitionId 0 and profileId 0 are what I'd try first. If it fails, the log
line names the guid and faction it saw, which tells us whether the faction
byte is at +0x30.

### 3–5. Health/shootable components, the remove packet, 0F 56, terminals

These are not done yet:
- The 0x57 component Create layout is known (CLOUD_NPCS_REPLICATION), but not
  which class ids give health/damage, or their payloads.
- The remove packet for this build is still unmapped.
- 0F 56 (AddLightweightVehicle) = the 0F 55 body plus u64, u32, u32, a position
  update (994660) and a string (static, from the earlier doc). It can be
  executed with the same oracle next.
- Terminal use is not traced.

Please tell me which of these comes first once the 0F 55 target appears.

## 4. Fire modes (`PS2_LOCAL_cloud_request_fire_modes`)

### Q1: the 9A07F0 field map, with names where the 2015 order lines up

The offsets come from the executed byte map in PS2_EMU_WEAPON17 §1. The names
come from soe-network's fire-mode order, used **only where the run of field
widths matches exactly**. All names are INFERENCE.

| Wire | Width | Name |
| --- | --- | --- |
| 0–3 | 4 × u8 | flags1, flags2, flags3, **type** |
| 4 | u8 | new in this build |
| 5 | u32 | **ammoItemId** |
| 9 | u8 | **ammoSlot**: the ammo-slot index in the weapon record; the factory uses it |
| 10 | u8 | **burstCount** |
| 11–28 | 3 × u16, u8, 6 × u16 | **timing block, ambiguous**: soe has six u16 here (fireDuration, fireCooldownDuration, **refireTime**, fireDelay, autoFire, chargeUp). This build has nine u16 and a u8, so which six are soe's is not provable from widths |
| 30 | 4 bytes | **range** (float) |
| 34 | u8 | **ammoPerShot** |
| 35–43 | 5 × u16 | **reloadTime**, reloadChamber, reloadAmmoFill, reloadLoopStart, reloadLoopEnd |
| 45 | u8 | **pelletsPerShot** |
| 46 | u8 | new |
| 47–94 | 12 floats | pelletSpread, cofRecoil, cofScalar, cofScalarMoving, cofOverride, recoilAngleMin, recoilAngleMax, recoilHorizontalTolerance, recoilHorizontalMin, recoilHorizontalMax, recoilMagnitudeMin, recoilMagnitudeMax |
| 95 | u16 | recoilRecoveryDelayTime |
| 97, 101 | 2 floats | recoilRecoveryRate, recoilRecoveryAcceleration |
| 105 | u8 | recoilShotsAtMinMagnitude |
| 106–141 | 9 × 4 bytes | recoilMaxTotalMagnitude, recoilIncrease, recoilIncreaseCrouch, recoilFirstShotModifier, then 5 more (soe has 2) |
| 142 | u16 | fireDetectRange |
| 144, 148 | 2 × u32 | effectGroup, **playerStateGroupId** |
| 152, 156 | 2 floats | **movementModifier, turnModifier** (you already set these to 1.0) |
| 160–171 | 3 × 4 bytes | soe unknown23/24, lockOnAngle (alignment ends here) |
| 172 onward | | lock-on, zoom, first-person offset, sway and reticle groups. Not alignable by width; **ADS zoom (defaultZoom) is in here but unidentified** |

### Q2: minimal values

I can't give native-verified minimal values yet.
- The 2015 capture's fire modes don't parse with this client's reader
  (CLOUD_MAP_MENU_FIREMODES §c).
- The fire path that would reveal the required fields isn't traced.

Two leads worth one test each (INFERENCE):
1. **`playerStateGroupId` (wire 148) and the player-state-group collection**
   (the 4th WeaponDefinitions collection, which you send empty). In PS2,
   player state groups carry the per-mode stance rules (sprint, ADS, crouch
   and so on). An unknown group id 0 with no groups is a good candidate for
   "can't sprint with the weapon out" and "ADS does nothing".

   The element is 29 bytes (PS2_EMU_WEAPON17 §1), field meanings unknown, so
   this needs native work before values can be sent. **Say if you want me to
   trace that collection's use next.**
2. **The timing block.** With refireTime 0 and fireDuration 0, the client may
   treat the mode as never ready. As a cheap probe, set every u16 in wire 11–28
   to 100 and wire 17 to 1. Also set **ammoPerShot (34) = 1, pelletsPerShot
   (45) = 1, range (30) = 300.0, reloadTime (35) = 1500**. Then see whether
   primary fire produces anything (log, sound, ammo change).

### Q3: the ammo HUD showing "--- / ---"

Not traced yet. The candidates are:
- the instance's ammo pairs (you send 17 / 68);
- the ammo slot index (wire 9);
- an item definition for ammo item 1, which you haven't sent.

The HUD most likely needs the ammo **item** to exist, so send an item
definition for ammo 1 (from the 2015 export) as a first test. (INFERENCE)

### Q4: is there a server round-trip for firing?

Not traced in this build. In the 2015 protocol, firing is client-side, with
the client reporting projectile and hit events to the server (Weapon family,
soe 0x85xx). So nothing should block the first shot waiting on a server reply.
(INFERENCE from the reference protocol)

## 5. Map.gfx strings

Your string dump changes the picture. The dropdown is fed by
**`ContinentBattleInfoRow`** (WorldInfoManager → `m_dsContinentBattleInfo`). Its
columns include zoneGeometryId, isProductionZone, isContinentLocked,
**isInstanced, hasQueue, destinationType** and missionObjectiveTarget. The
dropdown also has **ZoneIds constants** (ZONE_ID_INDAR … ZONE_ID_VR_VS,
GEOM_ID_SANCTUARY, LOGIN_DESTINATION_VR).

- The native strings "HasQueue", "LoginDestination" and
  "MissionObjectiveTarget" sit with the property code at 1784600–1784D60, the
  same getter that reads flags bits 0x80 / 0x40 / 0x10 and compares the
  current zone (TRACED, from the last doc). So **isInstanced, hasQueue and
  destinationType come from fields of the same zone record.** The exact field
  for each is not mapped yet.
- **Most likely filter (INFERENCE):** validateContinentBattleInfoData or the
  dropdown compares zoneId / zoneGeometryId against the ZoneIds constants and
  skips anything it doesn't know. **Your ids must equal the client's constants
  exactly**, including Sanctuary and VR.
- **To settle it:** send me the decompressed Map.gfx (1,064,968 bytes) as a
  private upload, not on GitHub, since it comes from the game. I can then
  decode the ABC int pool (the ZONE_ID_* values) and the code of
  validateContinentBattleInfoData / ContinentSelectionDropdown.

## Questions for the local session

1. Send the 181-byte 0F 55 (model 165, faction byte 2). Does the target
   appear? What does LightweightProxy.log say? If it says "Failed to spawn",
   which GUID and FactionId does it print?
2. Same with model 221.
3. Can you upload the decompressed Map.gfx privately?
4. Fire: with the probe values in §4 Q2, does anything change (sound, log,
   ammo HUD)? Should I trace the player-state groups next, or the fire path's
   required fields?
5. For the ammo HUD: does sending item definition 1 (ammo) change "--- / ---"?
