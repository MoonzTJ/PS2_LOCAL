# Weapons and world state in the historical PS2 emulator

From the cloud session, 2026-10-31. Replies to
`downloads/PS2_LOCAL_cloud_request_weapons_world_2026-10-31.txt`.

Sources (MIT):
- psemu/soe-network, commit `9db135b`;
- psemu/ps2-emu, commit `4754260`, captured blobs in `data/zone/`.

I decompressed and parsed the captured `ReferenceData.WeaponDefinitions.dat`
myself. Only IDs and small numbers are quoted. This is an older client, so
confirm everything in ours.

## Part A: Weapons

### Key takeaways

- **The old build's weapon ids line up with yours.** Item 44's captured
  inventory instance uses fire groups 15 and 1015. Those are exactly weapon
  **17**'s fire groups in the captured definitions, and 17 is your key for item
  44.
- The **weapon instance in SendSelfToClient carries the ammo state**: per ammo
  slot, the clip and the reserve. That matches the definition's clip size and
  capacity (30 + 180 = 210).
- The old WeaponDefinitions has **three** collections that soe-network parses
  (weapons, fire groups, fire modes), followed by more data it doesn't parse.
  Your build has six, consistent with H1Z1's later list.
- **soe-network's fire-mode schema does not match the capture.** Weapons and
  fire groups parse sensibly; fire modes come out as garbage. Don't take
  fire-mode field values from it. Use it for field names and meaning only.

### 1. WeaponDefinitions schema (`soe-network/lib/zoneprotocol.js` 98–246)

Wire: `17 04`, then a length-prefixed blob holding two u32 sizes and an **LZ4
block** (`zonepackets.js` 3007–3011, decoded at `zoneprotocol.js` 98–103).
Decompressed in the capture: 661,709 bytes. soe-network's three collections end
at 515,201; the remaining 146,508 bytes are unparsed and presumably the other
collections.

**Weapon record** (`weaponDefinitions` array):
- weaponId u32;
- weaponGroupId u32;
- flags u8;
- u16 times: equip, unequip, to/from iron sights, to/from iron-sights anim,
  sprint recovery;
- nextUseDelay u32;
- turnRate and movementSpeed modifiers (float);
- heatBleedOffRate u16, overheatPenaltyTime u16;
- rangeStringId u32;
- melee detect width and height (float);
- animationSetName (string);
- vehicle first/third-person camera ids (u16);
- overheatEffectId u32;
- min/max pitch (float);
- audioGameObjectHash u32;
- **ammoSlots** array of {ammoId u32, clipSize u16, capacity u32,
  clipAttachmentSlot u16, clip model name, reload weapon bone, reload
  character bone (strings)};
- **fireGroups**: array of u32 **fire group ids**;
- a trailing u16.

**Fire group** (`fireGroups` array):
- fireGroupId u32;
- **fireModes**: array of u32 **fire mode ids**;
- flags u8;
- chamberDuration u16;
- imageSetOverride u32;
- transitionDuration u16;
- animActorSlotOverride u8;
- deployableId u8;
- spinUp, spoolUp and spoolUpInitialRefire times (u16).

**Fire mode** (`fireModes` array; schema doesn't match the capture):
- fireModeId;
- three flag bytes, then type u8;
- **ammoItemId** u32, **ammoSlot** u8 (links to the weapon's ammo slot);
- burstCount;
- fire duration, cooldown, **refire**, fire delay, auto-fire and charge-up
  times;
- range;
- ammoPerShot;
- **reloadTime**, reload chamber and loop times;
- pellets and spread;
- cone of fire (CoF) and recoil values;
- recoil recovery;
- effectGroup u32;
- **playerStateGroupId** u32 (link to player-state groups);
- movement and turn modifiers;
- lock-on values;
- defaultZoom;
- first-person offset x/y/z;
- reticleId;
- heat values;
- sway;
- armsFovScalar;
- anim kick and recoil magnitudes;
- descriptionId;
- indirectEffectId;
- bulletArcKickAngle;
- projectileSpeedOverride;
- **inheritFromId** (fire mode inheritance);
- targetRequirement;
- fireAnimDuration;
- several unknowns.

**Links:**
- weapon.fireGroups → fireGroup.fireGroupId;
- fireGroup.fireModes → fireMode.fireModeId;
- fireMode.ammoSlot → weapon.ammoSlots index;
- fireMode.ammoItemId → ammo item;
- fireMode.playerStateGroupId → player-state group;
- fireMode.inheritFromId → base fire mode.

The projectile link isn't in this schema. H1Z1's separate fire-mode →
projectile mapping list supplies it.

### 2. Captured weapon 17 and the other starters

Weapon **17** (item 44's key):
- weaponGroupId 24, animationSetName "Rifle";
- equip 550, unequip 250, iron sights 150/150, iron-sights anim 125/150, sprint
  recovery 300 (ms);
- turn and move modifiers 1.0;
- rangeStringId 674;
- pitch −85 to 85;
- 4 ammo slots:
  - slot 0: ammoId 1, **clip 30, capacity 210**;
  - slots 1–3: clip 1, capacity 4/2/2. Their clipAttachmentSlot reads 15616,
    which looks suspicious; treat slots 1–3 with care;
- **fire groups [15, 1015]**:
  - group 15 → fire modes [15, 115], flags 8, transition 500;
  - group 1015 → fire modes [1015, 1115], flags 8, transition 500.

Fire-mode field values: not reliable (see above).

Weapon **13** (pistol key):
- group 21, "Pistol";
- equip 250;
- ammo slot clip **17**, capacity **85**;
- fire group [11] → modes [11, 111].

Weapon **9** (knife key):
- "Knife";
- melee detect 0.15 × 0.1;
- no ammo slots;
- fire groups [1, 7].

### 3. Weapon instance in SendSelfToClient's inventory

Schema: `zonepackets.js` 988–1011 (`itemWeaponDetailSchema`). Inventory items
are a base item plus a detail. Weapons use this detail:
1. bool.
2. **Ammo array**: {u32, u32} per ammo slot, which is **(clip, reserve)**.
3. **Fire-group array** (u8 count): per fire group, the fireGroupId and a u8
   count of per-fire-mode entries, each {u8, u32, u32, u32}.
4. u8, u8, u32, u8, float, u8, u32.
5. Two stat arrays (stat owner + stat).

Captured **item 44** (`data/sendself.json`, inventory):
- ammo [(30, 180), (1, 3), (1, 1), (1, 1)];
- fire groups:
  - 15: two mode entries, each {0, 0, **143**, 0};
  - 1015: the same;
- the second u8 after the arrays = 1.

The per-mode value 143 looks like a refire time in ms. That is a guess: the
pistol (item 21, ammo (17, 68), group 11) shows 167, and the knife shows 500.

There is no explicit "selected fire group/mode" field that I could identify.
The fire-group array order may imply it.

Other weapons follow the same pattern, for example item 2 ammo (15, 60) with
group 2. Non-weapon items use the one-bool detail.

### 4. Fire / reload packets

Weapon packets are wrapped in **`Weapon.Weapon` (`85 00`)**: a game time u32,
then the sub-packet (`zonepackets.js` 1150–1180 and 3640). The sub-packets
(`zonepackets.js` 1014–1140):

| Sub-packet | Opcode | Fields |
| --- | --- | --- |
| FireStateUpdate | `85 01` | guid u64, two u8 |
| Fire | `85 03` | guid, position, three u32 |
| FireNoProjectile | `85 05` | |
| ProjectileHitReport | `85 06` | |
| ReloadRequest | `85 07` | guid |
| Reload | `85 08` | |
| ReloadInterrupt | `85 09` | |
| ReloadComplete | `85 0A` | |
| AddAmmo | `85 0B` | guid, u8, u32 count, two bools |
| SetAmmo | `85 0C` | |
| SwitchFireModeRequest | `85 0D` | guid, three u8 |
| StatUpdate | `85 13` | |
| Add / Remove / ReplaceFireGroup | `85 15`–`85 17` | |

The guid is the item instance guid.

ps2-emu doesn't handle firing at all; firing evidently ran client-side. It only
*sends* AddAmmo (its `ammo` command, `zonepackethandlers.js` 168–182, guid = the
held item's instance guid) and StatUpdate (weapon stat tweaks).
H1Z1 (GPL, reference only) adds a reload flow:
- the client sends ReloadRequest;
- the server answers Reload, then ReloadComplete.

## Part B: World state

**Opcode caution:** ps2-emu's captured files disagree with soe-network's table
on two ClientUpdate opcodes.
- `ClientUpdateZonePopulation.dat` starts `11 21 00` and holds a 3-byte
  population array: [100, 0, 0].
- `ClientUpdateRespawnLocations.dat` starts `11 22 00`: 450 bytes, which look
  like respawn entries with a guid.

soe-network lists RespawnLocations as `11 21 00` and ZonePopulation as
`11 22 00`. The contents suggest **0x21 = ZonePopulation, 0x22 =
RespawnLocations**. Confirm with your dispatch handlers at A4D8D6 / A4D945.

### 5. ZonePopulation and RespawnLocations (sent at login, `zoneservertest.js` 92–93)

- **ZonePopulation:** a u32 count plus u8 per faction. The capture is
  [100, 0, 0]. soe-network leaves the schema commented out
  (`zonepackets.js` 2900–2904).
- **RespawnLocations** (`zonepackets.js` 2905+, entry schema
  `respawnLocationDataSchema` at 1548):
  - u8 flags;
  - locations array;
  - two u32;
  - a second locations array.

  Each location:
  - guid u64;
  - respawnType u8;
  - position (float × 4);
  - two u32;
  - iconId1, iconId2;
  - respawnTotalTime;
  - u32;
  - nameId;
  - distance (float);
  - two u8;
  - a block of five u8 plus a u32;
  - two u8.

  The capture begins with flags 0xE0 and 4 locations.

### 6. Ownership, continent list and empire strength

- **`ContinentBattleInfo` (`99`)**, schema `zonepackets.js` 4310–4326. The
  client requests it (`GetContinentBattleInfo` `9A`) and ps2-emu replies with
  the capture (`zonepackethandlers.js` 1691–1693). Per zone:
  - id, nameId, descriptionId;
  - four u8-per-faction arrays: population, regionPercent, populationBuff,
    populationTargetPercent;
  - name (string);
  - hexSize (float);
  - isProductionZone u8.

  The capture has 7 zones:

  | Zone | Id | nameId | descriptionId | Arrays (pop, region %, buff, target %) | hexSize | Flag |
  | --- | --- | --- | --- | --- | --- | --- |
  | Indar | 2 | 5 | 616 | [40,30,30], [0,16,20], [31,23,44], [6,4,4] | 200 | 128 |
  | Esamir | 8 | 6 | 681 | | 200 | 128 |
  | Amerish | 6 | 4 | 619 | | 200 | 128 |
  | Hossin | 4 | 558595 | 557503 | | 200 | 0 |
  | VR training (×3) | 96, 97, 98 | 557721 | | | 335 | |

  This is the likely source of the **Warzones list and the population /
  "Empire Strength" bars**.
- **`MapRegion.Data` (`A6 02`)**, `zonepackets.js` 4543–4567: two header values,
  then a regions array. Each region:
  - regionId, regionId2, nameId;
  - facilityId, facilityType u8;
  - currencyId u8;
  - **ownerFactionId u8**;
  - hexes {x i32, y i32, type u32};
  - flags u8;
  - three arrays;
  - connectionFacilityId.

  This is the **territory colours** source. Related packets:
  `MapRegion.GlobalData` (`A6 01`, two floats), Update / UpdateAll
  (`A6 04`/`A6 05`), Population (`A6 07`), RequestContinentData (`A6 08`).
  ps2-emu never sends MapRegion packets, so there's no capture.
- **`Facility.ReferenceData` (`87 01`, length-prefixed blob)** is sent on
  ClientIsReady (capture 184 bytes). **`Facility.FacilityData` (`87 02`)**:
  facilities array of {facilityId, facilityType u8, regionId, nameId,
  location x/y/z, imageSetId}. Not sent by ps2-emu.
  `Facility.FacilityUpdate` (`87 06`) and the capture/spawn notifications run
  `87 03`–`87 18` (`zonepackets.js` 3645–3782).

### 7. Facility objects (terminals, teleporters, spawn tubes)

ps2-emu shows server-spawned interactables only for its own `vehicleterminal`
admin command (`zonepackethandlers.js` 858–930). It sends
`PlayerUpdate.AddLightweightNpc` (`0F 52`: guid, transientId, nameId, model
and other ids), then `PlayerUpdate.LightweightToFullNpc`, which spawns a
working terminal as an NPC.

It sends nothing for the static base objects. I can't tell from these
references whether the live server spawned base terminals and spawn tubes as
NPCs, or whether the client creates them from the zone file once
facility/region ownership is known.

`Facility.FacilityData` / `MapRegion.Data` carry the facility ids and owners
those objects would key off. That makes them the first thing to send. Then
check natively whether terminals appear.

## Suggested order

1. **Weapons:**
   - fill weapon 17's record (times, ammo slot 30/210, fire groups 15/1015);
   - fill the fire-group records (modes 15/115, 1015/1115);
   - give item 44's inventory instance ammo (30, 180) and the fire-group /
     mode entries.
2. **World:** confirm the 0x21/0x22 opcodes, answer GetContinentBattleInfo,
   then MapRegion.Data / Facility.FacilityData for Indar ownership.
