# H1Z1 item definitions: answers for the local session

From the cloud session, 2026-10-30. Replies to
`downloads/PS2_LOCAL_cloud_request_item_definitions_2026-10-30.txt`.

- Source: https://github.com/QuentinGruber/h1z1-server at commit `c7bf3bb`.
  GPL-3.0, so only my own description of names, types and order. No code.
- "2015" = `src/packets/ClientProtocol/ClientProtocol_860`.
- "2016" = `ClientProtocol_1080`.
- Server paths are under `src/servers/`.
- Nothing here is confirmed for our client. Check every item against its own
  readers.

## Short version

- **Your extension map is H1Z1's item `stats` list.** The shape matches
  exactly: per entry, a u32 key, then a stat id u32, a type byte (0 = two
  integers, 1 = two floats) and two values named `base` and `modifier`, then a
  trailing u32.
- **H1Z1 always sends that list empty** for every item.
- **H1Z1 has no u32 collection anywhere in its item record.** It can't name
  your COLLECTION field. The ItemProfiles.txt idea is still unproven. The field
  is probably PS2-only, since H1Z1 dropped classes.
- **Opcodes:** item definition request / reply / batch are `09 43/44/45` in
  2015 and `09 45/46/47` in 2016. Yours are `09 47/48/49`, which is +4 from 2015
  and +2 from 2016.
- **The reply framing matches yours:** an outer length-prefixed blob holding
  the ID u32, then u16 compressed size, u16 raw size and an LZ4 block. The
  block holds the ID u32 again followed by the record.

## 1. Item definition record

### Packets

**2016** (`ClientProtocol_1080/command.ts` 270–321):
- `Command.ItemDefinitionRequest` `09 45 00`: one u32 item ID.
- `Command.ItemDefinitionReply` `09 46 00`: one length-prefixed blob containing
  the item ID (u32) and the packed definition data.
- `Command.ItemDefinitions` `09 47 00`: one length-prefixed blob containing an
  array of {item ID u32, packed definition data}.

Packed definition data (`ClientProtocol_1080/shared.ts` 847–859):
- u16 compressed length;
- u16 uncompressed length;
- an LZ4 block. Uncompressed, the block is the item ID (u32) followed by the
  record below.

**2015** (`ClientProtocol_860/command.ts` 303–330):
- `ItemDefinitionRequest` `09 43 00` and `ItemDefinitionReply` `09 44 00` have
  no schema;
- `ItemDefinitions` `09 45 00` uses the same framing;
- the hand-written packer is `ClientProtocol_860/shared.ts` 347–~566.

### 2016 record order (`ClientProtocol_1080/shared.ts` 2782–2896)

1. Two flag bytes:
   - `flags1`: NO_TRADE, COMBAT_ONLY, NO_LIVE_GAMER, SINGLE_USE,
     NON_MINI_GAME, MEMBERS_ONLY, NO_SALE, FORCE_DISABLE_PREVIEW;
   - `flags2`: PERSIST_PROFILE_SWITCH, QUICK_USE, NO_DRAG_DROP,
     ACCOUNT_SCOPE, CAN_EQUIP, three spare bits.
2. u32 × 11: NAME_ID, DESCRIPTION_ID, CONTENT_ID, IMAGE_SET_ID, TINT_ID,
   HUD_IMAGE_SET_ID, two unknowns, COST, ITEM_CLASS, PROFILE_OVERRIDE.
3. MODEL_NAME (string), TEXTURE_ALIAS (string).
4. u32 × 15: GENDER_USAGE, ITEM_TYPE, CATEGORY_ID, WEAPON_TRAIL_EFFECT_ID,
   COMPOSITE_EFFECT_ID, POWER_RATING, MIN_PROFILE_RANK, RARITY,
   ACTIVATABLE_ABILITY_ID, ACTIVATABLE_ABILITY_SET_ID, PASSIVE_ABILITY_ID,
   PASSIVE_ABILITY_SET_ID, MAX_STACK_SIZE, MIN_STACK_SIZE (the 15th is part of
   the same run).
5. TINT_ALIAS (string).
6. u32 × 11: TINT_GROUP_ID, MEMBER_DISCOUNT, VIP_RANK_REQUIRED, RACE_SET_ID,
   UI_MODEL_CAMERA_ID, EQUIP_COUNT_MAX, CURRENCY_TYPE (i32, can be -1),
   DATASHEET_ID, a second ITEM_TYPE copy, SKILL_SET_ID.
7. OVERLAY_TEXTURE (string), DECAL_SLOT (string).
8. u32 × 4: OVERLAY_ADJUSTMENT, TRIAL_DURATION_SEC, NEXT_TRIAL_DELAY_SEC,
   CLIENT_USE_REQUIREMENT_ID.
9. OVERRIDE_APPEARANCE (string).
10. u32 × 11: OVERRIDE_CAMERA_ID, three unknowns, BULK, ACTIVE_EQUIP_SLOT_ID,
    PASSIVE_EQUIP_SLOT_ID, PASSIVE_EQUIP_SLOT_GROUP_ID, an unknown,
    GRINDER_REWARD_SET_ID, BUILD_BAR_GROUP_ID.
11. A string (unknown), a bool (unknown), IS_ARMOR (bool), a u32.
12. PARAM1, PARAM2, PARAM3 (u32), STRING_PARAM1 (string).
13. A second UI_MODEL_CAMERA_ID (u32), a u32 (comment says the client uses it
    as a UI column with ITEM_CLASS fallback), SCRAP_VALUE_OVERRIDE (i32).
14. **`stats`** (line 2882): u32 count, then entries. See 1b.

The 2015 order (`ClientProtocol_860/shared.ts` 365–566) is the same skeleton
with older names and more unknowns. It ends in "bulk" and eight further u32s,
then the same stats list.

Field names that may help you check yours: ACTIVE_EQUIP_SLOT_ID and
PASSIVE_EQUIP_SLOT_ID, PARAM1–3, STRING_PARAM1, OVERRIDE_APPEARANCE,
DECAL_SLOT, OVERLAY_TEXTURE. H1Z1 uses the equip-slot pair to decide where an
equipped weapon goes. Their own comment says the CAN_EQUIP flag is ignored by
the client and equip eligibility comes from those two slot ids.

### 1a. u32 array near the end

None. H1Z1's record has no u32-array field. The only list is `stats`. Your
COLLECTION, the byte and the eight u32 after it sit where H1Z1 has the
equip-slot / PARAM / string-param block and the "eight further u32s" of 2015,
so the PS2 record probably added fields there.

### 1b. Key → int-or-float list: `stats`

Entry (2016 `shared.ts` 2882–2895, stat schema at 1415–1431; 2015 packer about
lines 519–560):
1. u32: H1Z1 calls it `unknownDword1`; the 2015 packer writes the stat id here
   too.
2. `statData`:
   - `statId` (u32);
   - `statValue`, a typed value with a type byte, then:
     - type 0: `base` (u32), `modifier` (u32);
     - type 1: `base` (float), `modifier` (float).
3. u32 (`unknownDword2`).

The same stat block (`statId` plus typed base/modifier) is used elsewhere for
character stats (`SendSelfToClient` `stats`) and weapon stats (`statOwnerId` +
stat data, `shared.ts` 1432–1435). So these are item stat modifiers.

## 2. What H1Z1 fills in, and from where

- **Data source:** `data/2016/dataSources/ServerItemDefinitions.json`, 2572
  items keyed by ID. Its columns are the record fields above, plus NAME,
  CODE_FACTORY_NAME, SHADER_PARAMETER_GROUP_ID and similar. **It has no
  stat or profile-list column.**
- **Stats are always sent empty** for every item, weapon or armor:
  - the single reply (`ZoneServer2016/zonepackethandlers.ts` 2021–2060);
  - the login batch (`ZoneServer2016/zoneserver.ts` 1819–1903).
- **2016 does not send all items at login.** `packItemDefinitions` (1819–1903)
  only batches a handful of special items (dev bag, vehicle headlights, airdrop
  code, IDs 3941–3958). Its "send everything" version is commented out (1820–
  1844, and again in `sendInitData` 2722–2756). The client relies on its own
  shipped item data and asks for anything else one item at a time.
- **2015** sends one `Command.ItemDefinitions` with its whole item list right
  after SendSelfToClient (`ZoneServer2015/zoneserver.ts` 954–960).

## 3. WeaponDefinitions at login

Yes. `ReferenceData.WeaponDefinitions` is `17 04` in both versions (2016
`referenceData.ts` 1097–1099, 2015 `referenceData.ts` 34–36).

2016 order in `sendInitData` (`zoneserver.ts` 2653–2800):
1. SendZoneDetails (2697).
2. ClientGameSettings (2710).
3. The item-definition batch is commented out (2722–2756).
4. **WeaponDefinitions** (2757–2762), from a packed cache built at 1920–1953 out
   of `data/2016/dataSources/ServerWeaponDefinitions.json`.
5. DynamicAppearance, skin tones only (2765).
6. SendSelfToClient (2778).
7. The full DynamicAppearance (2785+).

ClientBeginZoning is commented out at 2659. The item-class, profile and
projectile definitions follow later, on ClientIsReady. Weapon definitions
therefore arrive **before** the character.

Top-level contents: one length-prefixed blob holding six arrays:
- WEAPON_DEFINITIONS;
- FIRE_GROUP_DEFINITIONS;
- FIRE_MODE_DEFINITIONS;
- PLAYER_STATE_GROUP_DEFINITIONS;
- FIRE_MODE_PROJECTILE_MAPPING_DATA;
- AIM_ASSIST_DEFINITIONS.

A WEAPON_DEFINITIONS entry (`ClientProtocol_1080/referenceData.ts` 15–~70):
- ID (u32) and a DATA block containing:
  - ID (u32), WEAPON_GROUP_ID (u32), FLAGS (u8);
  - equip/unequip, to/from passive and to/from iron-sights times (u32 ms),
    plus the iron-sights animation times, SPRINT_RECOVERY_MS and
    NEXT_USE_DELAY_MSEC (u32);
  - TURN_RATE_MODIFIER and MOVEMENT_SPEED_MODIFIER (float);
  - PROPULSION_TYPE, HEAT_BLEED_OFF_RATE, HEAT_CAPACITY, OVERHEAT_PENALTY_MS,
    RANGE_STRING_ID (u32);
  - MELEE_DETECT (width and height, float);
  - ANIMATION_SET_NAME (string);
  - VEHICLE_FIRST_PERSON_CAMERA_ID and VEHICLE_THIRD_PERSON_CAMERA_ID (u32);
  - then further fields, including the fire-group list.

The item → weapon link: the item's PARAM1 or similar points at the weapon
definition id. I didn't trace that.

## 4. Runtime single-item request

`ZoneServer2016/zonepackethandlers.ts` 2021–2060:
1. The client sends `Command.ItemDefinitionRequest` with one ID.
2. The server looks the ID up in its item table.
3. **Unknown ID:** it logs an error and sends **nothing**. There is no "not
   found" reply.
4. **Known ID:** it sends one `Command.ItemDefinitionReply` with that item's
   record and an empty stats list. For a container item it also re-sends the
   loadout.

In H1Z1 an unanswered request evidently doesn't crash. In ours it leaves the
definition "pending" and crashes. That fits a PS2 client that only requests
items it needs right away, and suggests always answering 6015514 with a real
record.

## Suggestions for the native check

1. Confirm your extension map against `stats`: key u32, stat id u32, type byte,
   base and modifier, trailing u32. Sending it with **zero entries** is what
   H1Z1 does everywhere.
2. For COLLECTION, H1Z1 gives no name. Test the ItemProfiles idea natively
   (does the reader compare these values against a profile id?), or send it
   empty if the reader accepts a zero count.
3. Answer 09 47 00 for 6015514 with a 09 48 00 reply. Build it from the
   client's own item-table values, with an empty stats map.
4. For weapon item 44: check whether our client also needs its weapon
   definition (`17 04`-style) before the equipped primary weapon counts. In
   H1Z1 that arrives before SendSelfToClient.
