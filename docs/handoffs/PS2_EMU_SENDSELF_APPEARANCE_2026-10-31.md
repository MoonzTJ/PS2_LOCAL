# SendSelfToClient appearance and equipment: answers for the local session

From the cloud session, 2026-10-31. Replies to
`downloads/PS2_LOCAL_cloud_request_sendself_equipment_2026-10-31.txt`.

Sources:
- psemu/ps2-emu (MIT, `4754260`), `data/sendself.json`, a parsed live capture;
- psemu/soe-network (MIT, `9db135b`), `lib/zonepackets.js`;
- h1z1-server (GPL-3.0, reference only, `c7bf3bb`).

IDs and short asset names only.

## Key takeaways

1. **The real capture's head actor string is NOT empty:**
   `headActor = "Head_Female_Caucasian.adr"`. Ours is "". You found that the
   paper-doll builder only attaches the head when this string is set, and
   head/helmet pieces borrow the body skeleton. So this is the **top lead**.
   H1Z1 also always fills it, deriving it from actorModelId.
2. **actorModelId follows the current profile and gender.** The capture's
   currentProfile is 21 (VS Engineer, type 5), the character is female, and
   actorModelId = **176**, which is exactly profile 21's second model word
   (175/176 in ClientProfileData). For profile 19 male that gives 165, which
   is what you already send.
3. The capture's "current look" is a **copy of the current loadout** inside
   `characterLoadoutData`. There is no equipment array and no clear
   "equipped" flag on inventory items.
4. Your build's equipment row keyed by profile id matches **H1Z1-2016's
   `equipment` section** exactly in shape. Your entry's "two words" are very
   likely its two **empty strings** (tint alias and decal alias, each a u32
   length of 0). H1Z1 sends that section empty at login and delivers
   equipment later, after ClientFinishedLoading.

## 1. Identity / appearance block of the capture

Schema: `zonepackets.js` 1580–1597. Values: `data/sendself.json`, `data`.

| Field | Value |
| --- | --- |
| actorModelId | **176** |
| headActor (string) | **"Head_Female_Caucasian.adr"** |
| unknownString1 (your hair string) | "" |
| unknownDword4, unknownDword5 (your two tints) | 0, 0 |
| unknownString2, 3, 4 (your three strings) | "", "", "" |
| headId | **5** |
| unknownDword6 | **7** |
| factionId | 1 (VS) |
| unknownDword9, unknownDword10 | 0, 0 |
| position | (0, 700, 0, 1) |
| rotation-like vector | (0, −0.707, 0, 0.707) |
| identity | characterName "LocalPlayer", other fields 0/"" |
| gender | 2 |

Later in the record:
- `currentProfile` 21;
- the type → profile map (see the class data doc).

**Your mapping:**
- wire 25 actorModelId: 165 ✓;
- wire 29 head string: should be a head ADR name, not "";
- wire 33 hair: "" in the real capture too;
- wire 37/41 tints: 0 in the real capture;
- wire 45–53 strings: "" in the real capture;
- wire 57 headId: real 5 for that female head; yours is 1.

I didn't confirm that unknownDword6 = 7 sits at an offset your decoder knows
about. It follows headId directly, before factionId.

For the head string, use your client's HeadTypes table for headId 1, male,
rather than guessing a name. In the capture, headId 5 paired with
`Head_Female_Caucasian.adr`.

## 2. Anything linking items to slots for the current look?

- **No equipment or attachment array** in this build's SendSelfToClient (the
  2015-era schema, `zonepackets.js` 1578–2284).
- **Current loadout copy:** `characterLoadoutData` (2120–2152) ends with two
  loadout-shaped blocks and four u32. The first block (`unknownData1`, line
  2146) is the **current loadout**:
  - loadoutId 19;
  - linked profile 21 (the profile-id word), u8 1;
  - tab 0;
  - empty name;
  - 14 slots.

  The slots are (slot id → itemLineId, itemSlot):

  | Slot | itemLineId | itemSlot |
  | --- | --- | --- |
  | 1 | 11 | 7 |
  | 3 | 2 | 7 |
  | 5 | 134 | 7 |
  | 6 | 1 | 0 |
  | 7 | 0 | 7 |
  | 8 | 138 | 0 |
  | 9 | 14 | 13 |
  | 10 | 0 | 3 |
  | 11 | 140 | 6 |
  | 12 | 0 | 0 |
  | 13 | 5872 | 0 |
  | 14 | 7 | 0 |
  | 15 | 800598 | 0 |
  | 16 | 856 | 0 |

  The four trailing u32 are 256, 0, 1, 1.

  This is an Engineer, not the LA. The LA loadout in the same capture is
  loadout 17 (see the equipment reference doc). The second block
  (`unknownData2`) is empty.
- **Inventory items:** base item fields (`zonepackets.js` 898+):
  - itemId;
  - u32;
  - instance guid;
  - u32 (1 for most items, higher for stacks);
  - u32 (0 for most, 1–4 for a few);
  - two u32;
  - bool (always true);
  - u32;
  - a flag byte;
  - a typed extra block.

  The flag byte takes values 0, 0x08, 0x40, 0x50 and 0x58. Weapon item 44 and
  the base armour items (108/109/110) are 0, while the grenades are 0x08. Nothing
  separates "equipped" items, so I don't see an equipped flag.

So in that build, the current look was the current profile + actorModelId +
headActor + current loadout + owned items. No per-slot equipped records
existed.

## 3. h1z1-server 2016 SendSelfToClient equipment

Schema: `src/packets/ClientProtocol/ClientProtocol_1080/base.ts` 1217–1244,
after implants, before `loadoutSlots`. It is an array named `equipment`; each
row has:
1. profileId u32 (the row key);
2. a block of {u32, tintAlias string, decalAlias string};
3. `equipmentSlots` array. Each entry is equipmentSlotId u32, then
   {equipmentSlotId u32, item guid u64, tintAlias string, decalAlias string}.

That matches your "row keyed by profile; entry = slot id, raw word, item GUID,
two words". The "raw word" is the slot id repeated, and the two words are the
two strings, each a u32 length of 0 when empty. Check whether your reader
treats them as strings.

**What H1Z1 puts there at login:** effectively nothing. Its builder fills a
differently named key (`servers/ZoneServer2016/entities/character.ts` 1159), so
the schema's default empty array is packed. The equipment, including
**attachment models**, arrives in `Equipment.SetCharacterEquipment` after
ClientFinishedLoading (`zonepackethandlers.ts` about 511–518, commented
"needed or third person character will be invisible"). See
`H1Z1_EQUIPMENT_2026-10-31.md`.

The 2016 appearance fields before it (`base.ts` 86–99):
- actorModelId;
- **headActor**;
- **hairModel**;
- hairTint and eyeTint (u32);
- `emptyTexture` (string);
- two strings;
- headId;
- u32;
- shaderGroupId.

That is the same block as yours, with names for your hair and tint fields.
H1Z1 always fills headActor (`entities/baselightweightcharacter.ts` 155: the
head actor is derived from actorModelId).

## 4. Ordering and dependencies

- **ps2-character-view** (PS2, MIT): loads the body, then the head, then the
  helmet, then the weapon. Each step starts only after the previous load
  completes (`view.html` 93–96). Heads and helmets borrow the body skeleton;
  weapons attach to body bone `R_weapon` (`Actor.js` 298–312).
- **H1Z1 2016:**
  - head and hair are part of SendSelfToClient;
  - equipment attachments come only after ClientFinishedLoading;
  - its comment says equipment is needed for the third-person body to be
    visible.
- **ps2-emu:** body model, head actor, current loadout and owned items all go
  in SendSelfToClient. The loadout is activated afterwards by
  SetCurrentLoadout on ClientIsReady. No source attaches models before the
  body and head exist.

No source states "the head must be set before attachments" in so many words.
The ordering in all three is body → head → attachments, and the real capture
always has a head actor.

## Suggested next test

1. Set the head string to the HeadTypes ADR for headId 1 (male), keeping
   actorModelId 165.
2. Keep the equipment row present but empty, as H1Z1 does at login.
3. Retry the class model words and one model-bearing item: armour look
   first, then the carbine.
4. If items still crash inside SendSelfToClient, move them to
   SetCurrentLoadout plus the loadout, or to a post-ClientFinishedLoading
   equipment packet.
