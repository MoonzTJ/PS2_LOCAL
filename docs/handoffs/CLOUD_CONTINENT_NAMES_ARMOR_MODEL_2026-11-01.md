# Continent dropdown names and the armour "ModelDefinition is nullptr"

From the cloud session, 2026-11-01. This replies to
`PS2_LOCAL_cloud_continent_list_live_2026-11-01.txt` (v55). It is static
analysis of the client sections `loaded-20261010-032924`; no debugger was used.

Tags: **TRACED** = read in the client code; **EXECUTED** = run in the harness;
**INFERENCE** = my reading, not proven.

Good news first: 0x96 is working. M+0x18 = 9 and all records are valid, which
matches the CLOUD_CONTINENT_LIST trace. The v51 ClientGameSettings fix also
cleared the "nothing equipped" error.

## a) Why the continent dropdown is blank

### Correction to CLOUD_CONTINENT_LIST §2

**The name and description text ids are u32 #5 and #6 (record +0x20 / +0x24),
not #2 / #3.** You send 0 in #5 and #6, so every zone's name resolves to an
empty string. That explains both symptoms: the current entry is blank, and the
list shows no readable zones. (TRACED)

Evidence:
- **GetContinentInfo 1169600**, the script function that builds
  `ui.data.ContinentInfo`:
  1. finds the record by zone id (E072A0) and pushes `zoneId` from +0x10;
  2. looks up the text for **+0x20** through the locale string table (vtable
     slot +0x10) and pushes it as **`name`**;
  3. looks up the text for **+0x24** and pushes it as **`description`**;
  4. then pushes `population` (array 1, +0x38 data) and `region` (array 3,
     +0x78 data).
- **The UI property code at 17847DC–1784840** does the same: it sends +0x20 and
  +0x24 to the text lookup F02FB0, and pushes +0x10 and +0x1C as plain numbers.
- No script path reads +0x14 or +0x18. Their meaning is unknown, so leave your
  current values or 0.

### Fix

Per zone, put the locale text id you now send in #2 into **#5**, and the
description id into **#6** (0 if you have none; that only blanks the
description). For example, Indar = 2, then #2 5, #3 616, #4 2, **#5 5, #6
616**, #7 0. Keeping #2/#3 as they are is fine.

### Your other candidate filters

- **GetContinents (1169F00)** returns the zone id of **every** record in M+0x08
  (E07360). It applies no filter at all. (TRACED)
- **`locked`** in ContinentInfo is true whenever the status vector at M+0x20
  (your A3 13) **has an entry** for that zone. `owner` and `lockTime` are read
  only from that entry. So **A3 13 is a list of locked continents**. Keep not
  sending it unless you want a zone shown as locked. (TRACED)
- **The player's current zone (339)** is only compared with the zone id for the
  "is current zone" flag (flags bit 0x10 path at 178498B). It does not remove
  entries. (TRACED)
- **Description id 0:** only a blank description. **Flags 0x40 / 0x10:** only
  exposed as properties. Neither hides an entry. (TRACED as reads; the UI's use
  of them is in the SWF, not checked.)
- **The second listener E1BBA0** caches by group key (#4) and loads by the name
  string at +0xD8. It is not part of the dropdown path. (INFERENCE)

If the names show up but the list is still empty, the filter is inside the UI
SWF script (it receives zoneId, name, description, population, region,
locked, owner, lockTime). Please send the SWF-side log lines then.

## c) Armour "ModelDefinition is nullptr" with model id 0

**Which call logs it (TRACED):**
- F798B0 logs `('<asset>', <id>) - ModelDefinition is nullptr` when its third
  argument (the model id) is not in the model manager (F79B00 on manager+0x48).
- For character attachments, that id comes from the appearance builder
  **118C880**:
  1. it walks a list of entries (from its third argument, list at +0x10);
  2. for each entry it resolves **entry+0x28 and entry+0x2C** as model
     definitions (F79C70);
  3. it then applies each attachment's string modifiers with **entry+0x28** as
     the model id (118CC31–118CC3E → 118EAE0 → F798B0 at 118EB9B).
- A stored copy of the attachments is replayed later the same way
  (118D8E3–118D8F1).
- The in-world callers (196A79F, 196B0FA) instead use the character's model id
  at character+0x7C83C.

**What +0x28 / +0x2C are:** the class record's two model words (male/female),
165/166 for the VS LA, profile 19. The same offsets are read by the paper-doll
builder 196C950, which picks +0x28, or +0x2C when gender == 2; see
CLOUD_CLASSES_STATS_DEPLOY §1 and CLOUD_HA_CRASH. (TRACED for the paper doll;
that the 118C880 entries are the same class records is INFERENCE, from the
identical +0x28/+0x2C male/female pair.)

**So the 0 comes from the class-definition model words, which are 0 because
`--vs-la-class-models` is off.** The character row's ModelId and the item
definition are not this argument. (INFERENCE, strong)

### Why turning them on crashed before, and why it may not now

- The earlier 165/166 login crashes came before the 0xD1 ordering fix.
- Your v49 crash showed what happens when a model-bearing class or attachment
  reaches F798B0 before the 0xD1 table exists: the `<gender>` pattern lookup
  (hash 0x8F15DDC2) returns NULL, and 8B1AE3 dereferences it.
- Valid model words make F798B0 take the path that uses that pattern. With
  model 0 it never gets there, which is exactly why "off" didn't crash.
- Since v50, 0xD1 (with the Hotspot_participants / "<descriptor>" entry) is sent
  before SendSelf. So **retest `--vs-la-class-models` (165 male / 166 female for
  profile 19)** on top of v55. (INFERENCE)

If it still crashes, please send the fault RVA and registers. 8B1AE3 with
r10 = 0x8F15DDC2 again would mean the pattern table is still missing at that
moment. Any other RVA is a new problem.

## Order to test

1. 0x96: move the text ids into #5 / #6. Check the World Map dropdown.
2. Turn on the class model words (165/166) for profile 19 only. Check that login
   survives, then check My Profile for the Look001 armour, then
   AssetFailure.log.
3. Keep A3 13 off.

## Questions for the local session

1. After the #5/#6 change, does the dropdown list the 9 zones by name, with
   Sanctuary (339) as the current entry?
2. With class model words 165/166: does login survive? Does the "('Armor_VS_
   <gender>_LightAssault_Look001.adr', 0)" line disappear, or change to 165?
3. If the armour still doesn't show, what does AssetFailure.log or
   FailedLoadAssets.log say for that asset after the change?
4. Your fire-mode request: please send it with your current WeaponDefinitions
   fire-mode values (all zero except ammo slot and ammo item, per
   PS2_EMU_WEAPON17?). Firing doing nothing fits zero fire-mode data, so the
   next step is filling those from the 2015 export.
