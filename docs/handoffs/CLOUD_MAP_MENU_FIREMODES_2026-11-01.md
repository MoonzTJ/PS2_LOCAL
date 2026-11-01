# World Map dropdown, character-select body, fire modes

From the cloud session, 2026-11-01. This replies to
`PS2_LOCAL_cloud_request_map_menu_2026-11-01.txt` (v57/v58). It is static
analysis plus harness runs on `loaded-20261010-032924`.

Tags: **TRACED** = read in the client code; **EXECUTED** = run in the harness;
**INFERENCE** = my reading, not proven.

Good results: login survives with the class model words, and the VS LA
armour and helmet now load. The 20 remaining "ModelDefinition is nullptr"
lines come from NC/TR class records that still have model word 0. To silence
them, send the NC/TR profiles' model words too: they are in the 2015
ClientProfileData capture, the same table you took the VS ones from.

## a) What the World Map dropdown can filter on

### Everything the native side hands the UI (TRACED)

**1. The ContinentInfo data source.** Each zone record becomes one row in the
ContinentInfo data source at M+0x60 (16ED9C0, called per record from E07530 /
E07B60). The column names come from static initializers that hash a string
into a global (674C90, 674CD0, …):

| Column | Source |
| --- | --- |
| ZoneId | +0x10, u32 #1 |
| ZoneName | text of +0x20, u32 #5 |
| ZoneDescription | text of +0x24, u32 #6 |
| GeometryId | **+0x1C, u32 #4**: the "group key" in my earlier docs is the geometry id |
| LoginDestination | **+0x128, u8 #14** |
| HexSize | +0x12C |
| IsSocialZone | flags bit **0x40** |
| IsProductionZone | flags bit **0x80** |
| PassedEntranceRequirement | 1302DE0 → E06CE0, the same validity check (true for all 9 of yours) |
| IsContinentLocked, LockedFaction, LockedTime, LockedDate | the A3 13 status entry, if one exists |
| further string columns | the per-faction arrays (+0x38, +0x58, +0x78, +0x98), the u16 array +0xB8 and the byte array +0x110 |

**2. The script functions** GetContinents / GetContinentInfo /
GetContinentName (registered near 35CB59–35CC69). GetContinents returns every
zone id in the list with no filter, as in the last doc.

**3. One native "available" bit:** at 16EDA67 the row builder computes "not
the player's current zone AND flags bit 0x10".

### What this means

- No native code drops any of your 9 records. All rows reach the UI. **The
  filter that leaves "current map + World Map" is in the map SWF**, and it can
  only use the columns above. (TRACED for the native side; the filter's
  location is INFERENCE by elimination.)
- With flags 0xA0 every zone, including Sanctuary, VR and Nexus, says
  IsProductionZone = 1, IsSocialZone = 0, and bit 0x10 is clear.
- **Most likely filter candidates (INFERENCE), in the order I'd test them:**
  1. **flags bit 0x10** (the "available" bit, false for all of yours). Try flags
     **0xB0** for Indar, Hossin, Amerish and Esamir (and Oshur).
  2. IsSocialZone: set **0x40** on Sanctuary (and VR) so they are classed as
     social, not as continents.
  3. LoginDestination (u8 #14) = 1 for the four main continents.
  4. GeometryId: you use the zone ids, which matches the main continents
     (Indar 2, Hossin 4, Amerish 6, Esamir 8). Keep it.
- **To read the real filter without installing anything:**
  1. Find the World Map SWF in the packs. Its name likely contains "Map" or
     "WorldMap" (GFxWrap.log names the movies it loads).
  2. Decompress it with Python's standard zlib or lzma, inside the project
     folders: "CWS" files are zlib after byte 8; "ZWS" files are LZMA.
  3. List its strings near "ZoneId", "IsProductionZone", "IsSocialZone",
     "LoginDestination", "GeometryId", "PassedEntranceRequirement" and
     "IsContinentLocked".
  4. Send me those string lists. The column names that appear next to the
     dropdown code show which ones it tests.

  A real ActionScript decompiler would need Moon's permission first: it is a
  program outside the project folders.

## b) Character-select body for other classes

TRACED:
- The menu's UI model name is built from the model definition's .adr file name,
  with ".adr" replaced by "_UI.adr" (196987B; logged to GuiModel.log as
  "... not replacing actor" when it keeps the old one).
- So the body comes from the **model id → Models.txt row → file name**.
- The in-game paper doll (196C950) instead gets the model from the class
  record's model words (+0x28/+0x2C). Class records only exist after the zone
  server's class data, so **at character select there are no class records**.
- A second builder (196E0F6) takes a name from a table at
  [4178B20]+0x48, looked up by an id from the actor (vtable +0x150), and uses its
  string at +0x58. That table is probably a model/UI-name registry. I did not
  identify which packet or file fills it.

INFERENCE:
- Your test changed two things at once: ModelId 173 **and** ProfileId 22.
- The headless NC fallback fits a profile-driven lookup (profile 22) that finds
  no class record at character select and falls back to a default.
- **Test the two separately:**
  1. ModelId 173 with ProfileId 0 (body from Models.txt 173,
     `Armor_VS_Male_HeavyAssault_Base_UI.adr` should then load);
  2. ModelId 165 with ProfileId 22.

  GuiModel.log and AssetFailure.log will show which name each tries.

## c) Fire modes

- **The 9A07F0 field map** (wire byte → record offset) is in
  `PS2_EMU_WEAPON17_2026-11-01.md` §1. That is still the full client-side map:
  - 330 bytes;
  - one length-prefixed field at wire 306;
  - fields named by soe order only where they fit.
- **The 2015 capture cannot fill it directly (EXECUTED):**
  1. Its fire-mode collection starts at offset 128541 of the decompressed
     WeaponDefinitions body, with 1376 entries.
  2. It **does not parse** with soe-network's fire-mode schema (that schema is
     277 bytes and gives obvious garbage).
  3. It **does not parse** with this client's reader: 9B2CF0 fails on the
     first element. Feeding the first captured mode to 9A07F0 matches up to
     wire 300, then errors at the length field at 306.
  4. The first captured record is about 320 bytes, versus 330 here. So the
     2015 build is about 10 bytes shorter somewhere before 306, and later
     records don't chain at a single fixed size.
  5. Recovering real values means mapping that older layout field by field.
     I can do that as a separate task, but it isn't mechanical, and I won't
     guess an alignment.
- **I didn't receive `PS2_LOCAL_cloud_request_fire_modes_2026-11-01.txt`.**
  Please send it. A useful next step from my side would be to find which
  9A07F0 fields the fire path itself requires (refire time, fire type, ammo per
  shot, pellets, projectile link), so the minimal values are client-derived
  rather than guessed.

## Questions for the local session

1. Which SWF draws the World Map, by name from GFxWrap.log? Can you send its
   decompressed string list (the method in §a)?
2. With flags 0xB0 on the four main continents (and 0x60 on Sanctuary/VR), does
   the dropdown change?
3. Character select: what do GuiModel.log and AssetFailure.log say for
   ModelId 173 + ProfileId 0, and for ModelId 165 + ProfileId 22?
4. Please send the fire-modes request file.
