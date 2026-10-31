# Class / profile data in the historical PS2 emulator

From the cloud session, 2026-10-31. Replies to
`downloads/PS2_LOCAL_cloud_request_class_data_2026-10-31.txt`.

Sources (MIT):
- psemu/soe-network `lib/zonepackets.js`, commit `9db135b`;
- psemu/ps2-emu, commit `4754260`: `data/sendself.json`, a parsed capture,
  and `data/zone/*.dat`, raw captures.

I parsed `ReferenceData.ClientProfileData.dat` myself with the soe-network
schema. It consumed exactly 9784 of 9784 bytes as 87 records, so the old
layout below is complete for that capture.

All values are IDs and small scalars. No blobs are reproduced. This is an older
client, so confirm everything in ours.

## Key takeaways

1. The class list comes from two sources that must agree:
   - **ClientProfileData**: 87 class/profile *definitions*, which include the
     model words and image sets;
   - **SendSelfToClient `profiles`**: the 25 profiles the *character has*, plus
     a **type → profile map** right after `currentProfile`.

   We send one SendSelf profile and no type map. That probably explains the
   blank Classes tab.
2. In the old record, soe-network names the two model words
   **`firstPersonArms1/2`**. For profile 19 they are 165/166 (male/female).
3. Class profiles carry real values in both places: name/description id, type,
   icon, four floats, a u32 that is 1 for infantry and 3 for MAX, and image sets
   9018/9018 (MAX 9104/9103). In ours everything except id/name/type is zero.
4. The capture has no per-profile "unlocked/owned" flag. Owning a profile
   appears to mean **it is listed in SendSelf `profiles`**, and the type map
   names the active profile per class type.

## 1. SendSelfToClient profile entry (`zonepackets.js` 674–709, `profileDataSchema`)

Field order:
1. u32 × 8: profileId, nameId, descriptionId, type, iconId, three unknown u32.
2. bool.
3. u32.
4. Array of {u32, u32, u32}.
5. bool, u32, u32, bool.
6. u8.
7. bool, bool.
8. float × 4.
9. u32.
10. float.
11. u32 × 6.

That is 33 fields. Your client reads 7 u32, a collection, 2 u32, u8, 14 u32,
u8, so the layout has changed. Map by meaning, not by position.

Captured **profile 19** (`data/sendself.json`, `data.profiles`), non-zero fields
only:
- profileId 19, nameId 114, descriptionId 114, **type 3**, iconId 8;
- the u8 (field 6) = 1;
- floats 1.6, 0.95, 0.1, 0.3;
- the u32 after the floats = 1;
- the last float = 15;
- the fourth and fifth of the final six u32 = **9018, 9018** (image sets).

All other fields are 0/false, and the array is empty.

The other infantry classes match profile 19 except for name, type and icon:
- 17: type 1, name 112, icon 67;
- 20: type 4, name 115, icon 9;
- 21: type 5, name 116, icon 66;
- 22: type 6, name 117, icon 7.

MAX (23, type 7, name 118, icon 68) differs:
- first float 1.7, then 0.95, 0, 0;
- the u32 after the floats = 3;
- 4 and 350 in final-six positions 2–3;
- image sets 9104/9103.

## 2. Old ClientProfileData record (`zonepackets.js` 2966–3006, opcode `17 03`)

Packet: an array of {u32 key (= profile id), record}. The record:
1. u32 × 9: profileId, nameId, descriptionId, profileType, iconId, four
   unknown u32.
2. bool, bool.
3. u32, u32.
4. **Pair map**: array of {u32, u32}.
5. **`firstPersonArms1`** u32, **`firstPersonArms2`** u32.
6. u32.
7. **u32 list**: array of u32.
8. float × 4.
9. u32 × 3.
10. **`imageSetId1`**, **`imageSetId2`** (u32).

Profile 19, non-zero fields:
- nameId 114, descriptionId 114, profileType 3, iconId 8;
- **firstPersonArms 165 / 166**;
- the u32 after them = 1;
- **u32 list = [12, 16]**;
- floats 1.6, 0.95, 0.1, 0.3;
- imageSets 9018 / 9018.

The leading unknowns, both bools, the two u32 before the pair map, the pair map
itself (empty in all 87 records) and the first of the three final u32 are zero
in every record.

The same pattern holds across classes, which also lets you check cross-faction
ids:

| Profile | Type | Name | Arms | u32 list |
| --- | --- | --- | --- | --- |
| 4 | 3 | 37 | 159/160 | [12, 16] |
| 12 | 3 | 107 | 143/144 | [12, 16] |
| 19 | 3 | 114 | 165/166 | [12, 16] |
| 17 | 1 | 112 | 171/172 | [11, 16] |
| 20 | 4 | 115 | 163/164 | [13, 16] |
| 21 | 5 | 116 | 175/176 | [14, 16] |
| 22 | 6 | 117 | 173/174 | [15, 16] |
| 23 | 7 | 118 | 184/249 | [2, 16] |

Profiles 4 and 12 are the NC and TR Light Assault, consistent with your
159 (NC) / 143 (TR) menu models. For MAX, the u32 after the arms is 3, the last
two of the three final u32 are 4 / 450, and the image sets are 9104 / 9103.

Types across the 87 records: 69 type 0 (non-class profiles) and three each of
types 1 and 3–7, one per faction.

**Likely UI drivers** (inference): nameId and descriptionId (text), iconId and
the two image sets (class icon/art), profileType (which class slot it fills),
the arms/model words (preview), and the u32 list. The list differs per class
type (11–15, MAX 2) plus a shared 16. It might be item classes or equip groups
the class may use. I couldn't confirm that.

Codex's earlier comparison (9 vs 7 leading words, pair map vs triple map, 4 vs
2 floats) still stands. Use the values above to fill the matching fields by
meaning.

## 3. The 25 SendSelf profiles

| Profile | Type |
| --- | --- |
| 17 | 1 |
| 19 | 3 |
| 20 | 4 |
| 21 | 5 |
| 22 | 6 |
| 23 | 7 |
| 54, 55, 57, 62, 63, 67, 69, 70, 71, 72, 73, 75, 76, 77, 78, 79, 87, 88, 91 | 0 |

The type-0 entries are non-class profiles, which mostly have only an iconId,
sometimes a name. They are probably vehicles or other avatar profiles. The
capture is faction 1 (VS): `factionId` 1, gender 2.

`currentProfile` = 21, which is type 5.

Directly after it, the {i32, i32} array (`zonepackets.js` 1634–1637) holds
**5 entries mapping class type → profile id**:

| Type | Profile |
| --- | --- |
| 3 | 19 |
| 4 | 20 |
| 5 | 21 |
| 6 | 22 |
| 7 | 23 |

Type 1 (Infiltrator, profile 17) is absent even though 17 is in the list. No
explicit unlocked/owned flag exists. The single u8 (field 6) is 1 for all six
class profiles and 0 for the rest, so it could be a "selectable class" marker.
That is a guess.

Loadouts tie to profiles separately: loadout 17 → profile 19 (see
`PS2_EMU_EQUIPMENT_REFERENCE_2026-10-31.md`).

## 4. SetBattleRank and rank data

- `PlayerUpdate.SetBattleRank` (`zonepackets.js` 2670–2675, opcode `0F 42`):
  characterId u64, battleRank u32. It isn't captured. ps2-emu sends it itself
  right after SendSelfToClient with battleRank **100** (`zoneservertest.js`
  about 121–124).
- SendSelfToClient also carries **`playerRanks`** (`zonepackets.js` 2025–2034):
  an array of {rankId u32, {rankId, score, u32, rank, rankProgress}}. The
  capture has one entry: rankId 0, score 24640, rank 8, rankProgress 8.
- `Experience.SetExperienceRanks` (`8A 02`, 3880+) holds the rank table: per
  rank, experience required and per-faction {nameId, u32, imageSetId}, plus
  rewards. A 16 KB capture exists in data/zone, but ps2-emu doesn't send it.
- Your "BR 0" probably comes from playerRanks and/or SetBattleRank.

## 5. Other packets a Classes / loadout UI may need

All of these are sent on ClientIsReady (`zonepackethandlers.js` 9–79) unless
noted. These are top-level descriptions only; most have no schema in
soe-network.

- **`ReferenceData.ItemClassDefinitions`** (`17 01`): 62,772-byte capture,
  count 1902. No schema in soe-network.
- **`ReferenceData.ItemCategoryDefinitions`** (`17 02`): 1,984 bytes, count 62.
  Entries begin with the category id twice and then u32s. No schema.
- **`ClientUpdate.UpdateStat`** (`11 07 00`): the stats array, using the stat
  schema (id + typed base/modifier). The capture has 39 stats.
- **`ActivityManager.ProfileList`** (`63 01`): 6 bytes, an empty list.
- **`Abilities.SetAbilityActivationManager`** (`A3 04`) and
  `Ability.AddAbilityDefinition` (`10 11`): no schemas.
- **`Items.LoadItemRentalDefinitionManager`** (`B2 01`).
- **`ZoneSetting.Data`** (`B9 01`): hashed settings. `deploy.on.login` = 1 is
  among them.
- **`Loadout.SetCurrentLoadout`**, then **ZoneDoneSendingInitialData**.
- On request only:
  - `ProfileStats.PlayerProfileStats` (`96 02 00`, reply to
    GetPlayerProfileStats, 1683–1686);
  - RewardBuffInfo;
  - ContinentBattleInfo (`99`).

Suggested order for the local test:
1. ClientProfileData-equivalent records with the values above for the VS
   classes.
2. SendSelf `profiles` with the six class profiles (and their real values),
   plus the type → profile map.
3. `currentProfile` 19.
4. playerRanks / SetBattleRank.
5. Then check whether the Classes tab fills before adding ItemClass and
   ItemCategory definitions.
