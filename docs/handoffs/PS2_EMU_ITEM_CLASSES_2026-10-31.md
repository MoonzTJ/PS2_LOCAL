# Item classes and categories in the historical PS2 emulator

From the cloud session, 2026-10-31. Replies to
`downloads/PS2_LOCAL_cloud_request_item_classes_2026-10-31.txt`.

Sources:
- psemu/ps2-emu (MIT, `4754260`): `data/zone/ReferenceData.ItemClassDefinitions.dat`
  and `ReferenceData.ItemCategoryDefinitions.dat`, sent at
  `zonepackethandlers.js` 62–63;
- psemu/soe-network (MIT, `9db135b`): it has **no schema** for either
  (`zonepackets.js` 2964–2965);
- h1z1-server (GPL-3.0, reference only, `c7bf3bb`): the field names.

I parsed both captures myself, and both consumed every byte. Only IDs and small
numbers are quoted.

## Key takeaways

1. The old PS2 item class record is **ID + six more u32 + one u8**: 33 bytes
   with its key, 1902 records. H1Z1 names these exact fields, in order:
   1. ID;
   2. NAME_ID;
   3. ICON_ID;
   4. **WIELD_TYPE**;
   5. LIST_DISPLAY_STAT_ID;
   6. TOOLTIP_STRING_ID;
   7. SECONDARY_WIELD_TYPE;
   8. **FACTION_ID** (u8).

   H1Z1 then adds three more u32. Your "u32 key + seven u32" is very likely
   ID … SECONDARY_WIELD_TYPE. Check whether your reader still has a trailing
   faction byte.
2. In the capture the records are **almost empty**: key = ID, WIELD_TYPE = 1
   (for 1883 of 1902), a faction byte of 0–3, and everything else 0. Only class
   0 has a NAME_ID (124).

   So the client mainly needs each class **to exist**, with its wield type and
   faction. That fits your "invalid itemClass=0" log, and makes sending this
   table a good, low-risk test.
3. Item classes **don't link to categories**. Categories come from the item
   definition's CATEGORY_ID. The category packet is a flat list plus a
   parent → child tree.
4. No "ItemClassifications" packet exists in any of these sources.

## 1. ReferenceData.ItemClassDefinitions (`17 01`)

Wire: `17 01`, a u32 count (1902), then per entry a u32 key and a record of:
1. ID u32 (always equal to the key);
2. NAME_ID u32;
3. ICON_ID u32;
4. WIELD_TYPE u32;
5. LIST_DISPLAY_STAT_ID u32;
6. TOOLTIP_STRING_ID u32;
7. SECONDARY_WIELD_TYPE u32;
8. FACTION_ID u8.

Names come from H1Z1's 2016 schema (`src/packets/ClientProtocol/ClientProtocol_1080/referenceData.ts`
948–990), which continues with SALE_REWARD_SET_ID, SCORE_VALUE and SCRAP_VALUE
(u32 each). The old PS2 capture stops after FACTION_ID: 1902 × 33 bytes,
exactly.

Value distribution across all 1902 records:
- NAME_ID: 0 everywhere except class 0, which has 124;
- WIELD_TYPE: **1** for 1883 classes, 0 for 19 (classes 500–503, 4094,
  4099–4101, 4413–4415, 4506–4513);
- ICON_ID, LIST_DISPLAY_STAT_ID, TOOLTIP_STRING_ID, SECONDARY_WIELD_TYPE: 0 in
  every record;
- FACTION_ID: 0 for 487 classes, 1 for 447, 2 for 488, 3 for 480. Faction 1
  is VS in this capture (`factionId` 1).

Highest class id: 16044.

## 2. Your starter classes in the capture

| Class | Starter item | WIELD_TYPE | FACTION_ID |
| --- | --- | --- | --- |
| 1 | knife | 1 | 1 |
| 2 | pistol | 1 | 1 |
| 10 | spawn beacon | 1 | 0 |
| 11 | carbine | 1 | 1 |
| 14 | grenade | 1 | 1 |
| 104 | rocket rifle | 1 | 0 |
| 108 | | 1 | 1 |
| 109 | armour look | 1 | 1 |
| 110 | helmet look | 1 | 1 |
| 191 | implant | 1 | 0 |
| 198 | implant | 1 | 0 |
| 2210 | | 1 | 0 |
| 50776 | | absent (newer than the capture) | |
| 51173 | | absent (newer than the capture) | |

For every class listed, NAME_ID, ICON_ID, LIST_DISPLAY_STAT_ID,
TOOLTIP_STRING_ID and SECONDARY_WIELD_TYPE are 0.

The faction 1 = VS reading is an inference. It fits VS-only weapon and armour
classes being 1 while the beacon, rocket rifle and implants are 0 (all
factions).

## 3. ReferenceData.ItemCategoryDefinitions (`17 02`)

Wire: `17 02`, then two arrays.

1. **Categories:** a u32 count (62), then per entry a u32 key and:
   - ID u32 (= key);
   - NAME_ID u32;
   - u32 (0 in all 62);
   - u32 (1 in 57; 0 for 100, 102, 104, 106, 108);
   - bool (1 in all).

   H1Z1's 2016 names (`referenceData.ts` 990–1005) are ID, NAME_ID,
   unknownDword1, SCRAP_VALUE_OVERRIDE and unknownBoolean1. The old capture has
   the same five fields.
2. **Category tree:** a u32 count (56), then triples of (parent key u32,
   parent id u32, child id u32). This array is not in H1Z1's schema. Groups:
   - 100 → 102, 103;
   - 101 → 104, 105;
   - **102 → 1–23**;
   - 103 → 106, 107;
   - 104 → 109–132;
   - 105 → 108;
   - 135 → 136, 137.

   The two arrays together consume all 1984 bytes.

Category ids present: 2–24, 99–137. NAME_IDs include, for example, 100 → 5785,
102 → 5786 and 2–16 → 49110–49124. Use your locale to name them. Category 1 is
listed as a child of 102 but has no record of its own.

**Classes vs categories:** the class record has no category field. An item gets
its category from its own item definition (CATEGORY_ID), so the categories your
starter items belong to are whatever their CATEGORY_ID says in your client's
item table.

## 4. ItemClassifications

There is no such packet in:
- soe-network's table;
- ps2-emu's captures or handlers;
- h1z1-server (both 2015 and 2016).

It is newer than these references. Its layout would have to come from your
client's own reader.

## 5. Send order in the references

- **ps2-emu (PS2):** on **ClientIsReady**, after DoneSendingPreload, UpdateStat
  and CharacterStateDelta (`zonepackethandlers.js` 61–65). Order:
  1. EnableCompositeEffects;
  2. **ItemClassDefinitions**;
  3. **ItemCategoryDefinitions**;
  4. ClientProfileData;
  5. ProjectileDefinitions;
  6. then the rest, then Loadout.SetCurrentLoadout, then
     ZoneDoneSendingInitialData.

  Weapon and item definitions came earlier, before SendSelfToClient.
- **H1Z1 2016:** also on **ClientIsReady**, after SendSelfToClient. Order:
  item class definitions, then profile definitions, then projectile
  definitions, sent from caches (`servers/ZoneServer2016/zonepackethandlers.ts`
  281–289; cache built at `zoneserver.ts` 1985–2008 from
  `data/2016/dataSources/ServerItemClassDefinitions.json`). H1Z1 doesn't send
  ItemCategoryDefinitions.

H1Z1's class data has 84 records, mostly zeros except ID and NAME_ID. So H1Z1 is
also a "the class must exist" case, not a data-heavy one.

## Suggested test

Send an ItemClassDefinitions-equivalent with records for every class your
items use. Set ID = key and WIELD_TYPE = 1, FACTION_ID as in the table above
(if your reader has the byte), and everything else 0. Send it on ClientIsReady
before the profile definitions, as both references do.

Then retry the model-bearing items and the class model words. If the crash
persists, add ItemCategoryDefinitions for the CATEGORY_IDs your items use.
