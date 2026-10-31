# How the historical PS2 emulator equipped a character

From the cloud session, 2026-10-31. A follow-up to `H1Z1_EQUIPMENT_2026-10-31.md`.
H1Z1 is a cousin. These two are actual **PlanetSide 2** references, both
already in Moon's reference list (`REFERENCE_LINKS.md` /
`docs/context/ps2_reference_links.md`):

- **psemu/soe-network** (MIT, commit `9db135b`, 2015-02-17). This is the
  library H1Z1's server grew from, and its description says it was built for
  PlanetSide 2. The PS2 packet table is `lib/zonepackets.js`.
- **psemu/ps2-emu** (MIT, commit `4754260`, 2015-02-17). The PS2 server built on
  it. It replays blobs captured from the live game (`data/zone/*.dat`,
  `data/sendself.json`).

Codex reviewed ps2-emu for inventory on 2026-10-12
(`docs/reviews/emulator-inventory-gap-20261012.md`). This note covers the
equipment question only.

Caveats:
- This is an older client. Codex found the item-definition opcodes there are
  43/44/45 while ours are 47/48/49, and the inventory headers differ.
- Use it for structure and order, never as bytes to replay.
- Don't commit its captured live data (`data/zone/*.dat`, `sendself.json`) into
  PS2_LOCAL.

## How close it is to our client

**ClientUpdate matches exactly, with no offset:**

| Packet | soe-network | Ours |
| --- | --- | --- |
| UpdateLocation | `11 0C 00` | `11 0C 00` |
| ActivateProfile | `11 15 00` | `11 15 00` |
| DoneSendingPreloadCharacters | `11 1B 00` | `11 1B 00` |

The captured PreloadDone (`data/zone/ClientUpdateDoneSendingPreloadCharacters.dat`)
is `11 1B 00 00`, byte-identical to ours.

**ActivateProfile** (`zonepackets.js` 2865–2884):
- the profile blob;
- the attachment array. Each entry has 4 strings (modelName, a string,
  tintAlias, a string) and then **3** u32s. You found 4 u32s in 1533, so a
  word was added later;
- 4 u32;
- 2 strings.

That is the same shape you mapped.

## The answer to the equipment question

**The real PS2 login flow sends no Equipment packet and no ActivateProfile.**
Equipment is described as **loadouts plus owned items**. A
`Loadout.SetCurrentLoadout` on ClientIsReady makes it take effect.

Login order (`ps2-emu/zoneservertest.js`, `server.on("login")` from line 74):
1. ReferenceData.WeaponDefinitions;
2. InitializationParameters;
3. SendZoneDetails;
4. ZonePopulation, RespawnLocations;
5. ClientGameSettings;
6. **Command.ItemDefinitions** (whole batch);
7. VehicleDefinitions;
8. **SendSelfToClient**;
9. SetBattleRank.

On **ClientIsReady** (`zonepackethandlers.js` 9–88):
1. QuickChat;
2. DoneSendingPreloadCharacters;
3. UpdateStat;
4. ActivityManager profile list;
5. ClearMissions;
6. ZoneSetting.Data, which includes `zonesetting.deploy.on.login` = 1;
7. CharacterStateDelta;
8. GameTimeSync;
9. EnableCompositeEffects;
10. ItemClass and ItemCategory definitions;
11. **ClientProfileData**;
12. ProjectileDefinitions;
13. Facility data, rental and ability definitions;
14. **`Loadout.SetCurrentLoadout`**;
15. POI definitions;
16. **ZoneDoneSendingInitialData**;
17. world commands.

Nothing named Equipment.* or ActivateProfile is sent at any point.

### What SendSelfToClient carries for equipment (`zonepackets.js` 1578–2284)

There is **no equipment or attachment section**. The relevant parts are:
- `profiles` (line 1632, `profileDataSchema` at 674) and `currentProfile`
  (1633). The capture has 25 profiles. The faction-1 class profiles and their
  `type` values are 17→1, 19→3, 20→4, 21→5, 22→6, 23→7, so **profile 19 has
  type 3, matching your Light Assault profile 19**;
- `inventory.items` (1663), with 1003 owned items in the capture;
- **`characterLoadoutData`** (2120–2152), described below.

`characterLoadoutData`:
- `loadouts` array. Each entry is `loadoutId` (u32), a tab u32 (0 or 256), and
  a loadout block (`loadoutDataSubSchema1`, line 1372):
  - `loadoutId`;
  - {u32, u8}. In the capture this u32 is the **profile id**: loadout 17 →
    profile 19;
  - the tab u32;
  - {u32, `loadoutName` string};
  - `tintItemId`, a u32, `decalItemId`;
  - **`loadoutSlots`** array. Each slot is `loadoutSlotId` u32, then
    `loadoutSlotData` containing:
    - `index` u32;
    - `loadoutSlotItem` { **`itemLineId`** u32, `flags` u8, `attachments`
      [`attachmentId` u32], `attachmentClasses` [`classId` u32,
      `attachmentId` u32] };
    - `tintItemId` u32;
    - `itemSlot` u32;
- two more arrays, two more loadout-shaped blocks, then four u32s (256, 0, 1, 1
  in the capture).

The capture has 10 loadouts: ids 15/17/18/19/20/21, each on tabs 0 and/or 256,
with 9–14 slots each.

A slot names an **item line**, not an item instance or a model. The server
resolves an item line to its rank-0 member item (`zonepackethandlers.js` 94–117,
`Loadout.SelectSlot`, via an `item_line_members` lookup), then finds that item
among the owned inventory items. The client then gets the model from the item
definition. Weapon attachments (scopes and so on) ride on the slot as
attachment ids.

### Loadout.SetCurrentLoadout

`zonepackets.js` 3803–3810, opcode `89 03` in that build; verify ours:
- type u8: 2;
- u8: 0;
- `loadoutId` u32;
- `tabId` u32;
- u32: 1.

The captured blob (`data/zone/Loadout.SetCurrentLoadout.dat`, 16 bytes) is
`89 03 | 02 00 | 11 00 00 00 | 00 01 00 00 | 01 00 00 00`, which is loadout 17
(the profile-19 loadout), tab 256.

ps2-emu sends it on ClientIsReady, and again on `Command.SetProfile` (class
change, lines 1048–1055).

Codex noted the starter tables select tab 0 in our build, not 256.

Related packets (3812–3828): `Loadout.SelectSlot` `89 04` (type, two u8,
`loadoutSlotId`, gameTime) and `Loadout.SetCurrentSlot` `89 06` (type, u8,
`slotId`).

`Equipment.SetCharacterEquipment` (`97 01`, 4217–4243) does exist in this
table. It has the same shape as H1Z1's: profileId, characterId, u32, 2 strings,
an equipment slot array {slotId, {slotId, guid, 2 strings}} and an attachment
array. **But ps2-emu never sends it.** It is presumably for other players or
for later changes.

## What this suggests for the crash (hypotheses)

1. In real PS2 the client builds the equipped look **itself** from the current
   loadout + owned items + item definitions, after
   `Loadout.SetCurrentLoadout`. That arrives on ClientIsReady, after the
   character exists, together with ClientProfileData.
2. Model-bearing items were never placed directly in SendSelfToClient equipment
   rows at character construction. That fits your crash, and also fits "no
   model = loads fine".
3. The order puts item definitions (and weapon definitions) **before**
   SendSelfToClient, and profile data plus the loadout selection **after**
   ClientIsReady.

## Suggested native checks

1. Find our client's `characterLoadoutData` reader inside SendSelfToClient:
   - the loadout entry (loadoutId, tab, the profile-id word, slots);
   - the slot fields (itemLineId, flags, attachments, attachmentClasses, tint,
     itemSlot).
   Check that `itemLineId` resolves through the client's item-line table to
   item 44 for the primary slot.
2. Find our `Loadout.SetCurrentLoadout` opcode and reader (type, u8, loadoutId,
   tabId, u32). Check what its handler does with actor+17AC and the arms.
   It may be the path that sets the model and equips the look, replacing the
   activation.
3. Test order: SendSelfToClient with the equipment rows empty, the profile-19
   loadout filled and owned items present. Then send SetCurrentLoadout on
   ClientIsReady, after the profile and item definitions.

## Addendum: how a PS2 character is assembled (psemu/ps2-character-view)

Source: psemu/ps2-character-view (MIT, commit `0ea04ac`, 2015-02-14). It's a
browser viewer that assembles a TR Heavy Assault from real ADR files in
`assets/actors/`.

**Assembly order** (`view.html` 85–103, `Character.js`): body (armor), then
head, then helmet, then the weapon attachment. Each step starts only after the
previous one has finished loading.

**Each ADR's `<Usage>` element says how it joins the character.** Parsed in
`Actor.js` 48–51; values from the four bundled ADRs:

| ADR | actorUsage | borrowSkeleton | Bone | Other |
| --- | --- | --- | --- | --- |
| `Armor_TR_Male_HeavyAssault_Look001.adr` | 1 (body) | 0 (owns the skeleton) | none | `AnimationNetwork ThirdPerson.mrn`, set `Male` |
| `Head_Male_Asian.adr` | 2 | 1 (uses the body's skeleton) | none | |
| `Helmet_TR_Male_All_PS037.adr` | 2 | 1 (uses the body's skeleton) | none | |
| `Weapon_TR_LMG001.adr` | 3 | 0 | `boneAttachmentName="R_weapon"` | `Rifle.mrn` |

The weapon is parented to the body skeleton's `R_weapon` bone, found by the
Jenkins one-at-a-time hash of the uppercased bone name (`Actor.js` 298–312).

**Why this matters for the crash (hypothesis):**
- Heads, helmets and other usage-2 pieces **borrow the body's skeleton**.
- Weapons need a **bone on the body's skeleton**.
- So every model-bearing equipment piece depends on a loaded body actor.

If the body/actor model isn't set when SendSelfToClient builds the character
(your actor+17AC = 0 lead), those pieces have no skeleton to bind to. Items
without models never need one. That fits all of your bisect results:
- links only or items only: fine;
- model-less equipment: fine;
- armor/helmet/weapons: crash.

It also fits v12.1, where the same Look001 armor/helmet loaded fine through
ActivateProfile, which sets the model first.

Note: here the "Look001" armor ADR is itself a **body** actor (usage 1, owns
the skeleton and animation network), not a skinned add-on.

**Native checks:**
1. Read `<Usage>` (actorUsage, borrowSkeleton, boneAttachmentName) in our
   client's own ADRs:
   - `Armor_VS_Male_LightAssault_Look001.adr`;
   - `Helmet_VS_Male_LightAssault_Look001.adr`;
   - the base body `Armor_VS_Male_LightAssault_Base.adr`;
   - the weapon `Weapon_VS_Carbine007.adr`.
2. In the crashing attach path, check whether it dereferences the owner's
   skeleton or model (actor+17AC) for borrowSkeleton or bone-attachment
   pieces.
3. If so, deliver the equipment only after the body model is set. Options:
   the loadout path (SetCurrentLoadout after ClientIsReady), or a model set
   before the equipment, as in v12.1.

The other references in Moon's list were reviewed by Codex on 2026-10-09/12
and are about assets, maps and transport:
- soe-pack, dbg-pack, Mandible, ps2ls/ps2ls2, forgelight-dme/zone,
  binary-templates;
- Sanctuary.SoeProtocol (transport only);
- PSF-LoginServer (PS1);
- patchside (PS1).

None of them carries PS2 equipment or loadout game packets. soe-locale or
Mandible's locale tools could resolve name IDs, for example to confirm item
44's name or the profile names, if needed.
