# H1Z1 equipment at login: answers for the local session

From the cloud session, 2026-10-31. Replies to
`downloads/PS2_LOCAL_cloud_request_equipment_2026-10-31.txt`.

- Source: https://github.com/QuentinGruber/h1z1-server at commit `c7bf3bb`.
  GPL-3.0, so only my own description, no code.
- "2016" = `src/packets/ClientProtocol/ClientProtocol_1080`; "2015" =
  `ClientProtocol_860`. Server paths are under `src/servers/`.
- Nothing here is confirmed for our client.

## Short version

**H1Z1 never puts equipped models in SendSelfToClient.**
- The character packet carries owned items and loadout slots.
- In practice its equipment section goes out **empty**.
- Models (the attachment list) arrive later in a separate
  `Equipment.SetCharacterEquipment`, sent only after the client reports
  **ClientFinishedLoading**. That is after the world and the character are
  fully built.

This matches your bisect: links and items load fine, while equipped items with
models crash during character construction.

Moon's note: H1Z1 probably has no separate first-person viewmodel/arms actor
like PS2. Nothing in h1z1-server handles arms, only a `firstPersonOnly` flag.
So "when it's safe to attach models" may be stricter for us. PS2 builds a
first-person arms actor that H1Z1 apparently doesn't have, and your crash
happens right after that actor is created. Use H1Z1 for ordering, not as proof
that it's safe.

## 1. Equipment at login

### 1a. Where equipped items go, and in what order (2016)

1. **SendSelfToClient** (`ZoneServer2016/zoneserver.ts` 1790, built by
   `entities/character.ts` 1129–1190) carries:
   - inventory items (owned items, with weapon data for weapons);
   - `loadoutSlots`, including the current slot;
   - resources and containers.

   Its `equipment` section (schema `ClientProtocol_1080/base.ts` 1217–1244,
   default empty) is effectively empty: the server fills a differently named
   key (`equipmentSlots`, `character.ts` 1159) that doesn't match the schema
   field, so the default empty array is packed. That is my reading of the
   packer, not a stated design choice. Either way **no model/attachment list
   is in SendSelfToClient.**
2. The server marks the character "initialized" right after sending it
   (`zoneserver.ts` 1793). Until then the equipment-update helper sends nothing
   (`character.ts` 2012–2021).
3. **ClientIsReady** (`zonepackethandlers.ts` 215–300): no equipment and no
   ActivateProfile (commented out). It sends preload-done, the
   proximity-complete signal (after 5 s), the state delta and the
   item-class/profile/projectile definitions.
4. **ClientFinishedLoading, first time only** (`zonepackethandlers.ts` 333+,
   equipment at about 511–518), in this order:
   - `Character.WeaponStance` (`0F 20`): characterId u64, stance u32;
   - **`Equipment.SetCharacterEquipment`** (`95 01`). Their comment: "needed or
     third person character will be invisible";
   - **`Loadout.SetLoadoutSlots`** (`87 04`). Their comment: "needed or all
     loadout context menu entries aren't shown".

`Equipment.SetCharacterEquipment` top-level fields (`equipment.ts` 23+, filled
at `character.ts` 2024–2036):
- character data: `profileId` u32 (they send 5), `characterId` u64;
- u32 (0);
- `tintAlias` string ("Default");
- `decalAlias` string ("#");
- `equipmentSlots` array (see 1b);
- `attachmentData` array (see 1b);
- a final bool (true).

It goes to the owner and to everyone who has that character spawned
(`zoneserver.ts` `sendDataToAllWithSpawnedEntity`, owner included).

Later changes use `Equipment.SetCharacterEquipmentSlot` (`95 02`, one slot
plus its attachment) and `Loadout.SetLoadoutSlots`.

**2015:** `ZoneServer2015/zonepackethandlers.ts` 272–343. ClientIsReady sends
ActivateProfile **with** `attachmentData: client.character.equipment` before the
other ready packets. So 2015 delivered models through ActivateProfile on
ClientIsReady. That is also after SendSelfToClient, never inside it.

### 1b. What a normal character's lists contain

One entry **per equipped item that has an equipment slot**, both lists built
from the same internal table (`entities/basefullcharacter.ts` 1003–1056):

- **Equipment slot entry:** equipmentSlotId u32, then a block containing
  equipmentSlotId again, item guid u64, effectId u32, tintAlias string
  ("Default"), decalAlias string ("#").
- **Attachment entry:**
  - modelName (item model name with `<gender>` replaced, and Up/Down swapped
    for hoods);
  - effectId;
  - textureAlias;
  - tintAlias ("Default");
  - decalAlias ("#");
  - slotId;
  - a shader parameter list (skin/tint shader ids, possibly empty).

For a starting character these are the clothes, the backpack and the active
weapon (its ACTIVE equip slot) plus holstered weapons (PASSIVE slots). They come
from the saved loadout through `generateEquipmentFromLoadout` (`zoneserver.ts`
2102, `basefullcharacter.ts` 468–500).

## 2. DynamicAppearance

`ReferenceData.DynamicAppearance` (`17 06`, `ClientProtocol_1080/referenceData.ts`
1130+, 2016 only; 2015 has no equivalent). It holds **shader/tint
parameters**, not models. Three arrays:
- ITEM_APPEARANCE_DEFINITIONS: ID, ITEM_ID, MODEL_ID, GENDER_ID,
  SHADER_PARAMETER_GROUP_ID;
- SHADER_SEMANTIC_DEFINITIONS: a group id plus a list of semantic ids;
- SHADER_PARAMETER_DEFINITIONS: ID plus semantic name, four float defaults, a
  u32, a bool default, a texture default and a value type.

2016 sends a skin-tone-only subset before SendSelfToClient (`zoneserver.ts`
2765, "to ensure the character displays the correct skin tone") and the full
set after (2785+). It does **not** set the character's model or attachments.
Since 2015 shows equipment without it, it isn't required for models to appear.
At most it affects skin/tint. Low priority for us.

## 3. Profile/activation order and actorModelId

- **2016:** no ActivateProfile at all (commented out). No Loadout.SelectLoadout
  at login. The active loadout slot comes from `loadoutSlots.currentSlotId` in
  SendSelfToClient and from `Loadout.SetLoadoutSlots` after
  ClientFinishedLoading.
- **2015:** on ClientIsReady it sends ActivateProfile (with attachments) **first**,
  then CharacterStateDelta, ClientProfileData, `Loadout.SetCurrentLoadout`
  (`86 03`, guid + loadout id 3) and the rest (`zonepackethandlers.ts` 278–343).
  That is after SendSelfToClient and before ClientFinishedLoading.
- **Packets carrying actorModelId:**
  - SendSelfToClient;
  - ActivateProfile;
  - the lightweight PC/NPC spawn records used for *other* players
    (`ClientProtocol_1080/shared.ts` 1221, 1277);
  - a command delivery packet (`command.ts` about 350);
  - `Character.ReplaceBaseModel` (`0F 1D`: characterId u64, modelId u32,
    effectId u32). H1Z1 only uses it for plants (`entities/plant.ts` 123–131).

  None of these is used to set the local player's model after login.

Relevance to your actor+17AC lead: in 2015 the model change via ActivateProfile
**precedes** the equipment it carries, all in one message. That fits "the model
must be set before models attach". Since you dropped the activation in v13.1,
two orderings to test natively:
1. Model set first (activation or a ReplaceBaseModel-like path), equipment after.
2. H1Z1-2016 style: equipment only after ClientFinishedLoading.

## 4. Sub-opcode 0x64 + u8 + u64 GUID

- 2015's base opcode **0x64 is `Inspect`** (`ClientProtocol_860/base.ts` 1934).
  In 2016 it moved to 0x65 (`ClientProtocol_1080/base.ts` 2398), next to
  `RequestSendItemDefinitionsToClient` (0x63 / 0x64).
- H1Z1 has **no schema and no handler** for Inspect, so its fields aren't
  documented.
- "Inspect" (viewing another character's equipped look) fits your
  paper-doll / 3D preview guess, but that rests on the name alone.
- The only other 0x64 is `PlayerUpdate.MotorRunning` (`0F 64`, characterId u64,
  `ClientProtocol_860/playerUpdate.ts` 1349), a vehicle engine state. It's
  unlikely, since it has no u8 before the guid.

## 5. Weapons: how the held weapon is chosen and drawn (2016)

- **Item instance:** the weapon item with its weapon data (fire groups/modes,
  ammo) is in the SendSelfToClient inventory, built by `basefullcharacter.ts`
  1316–1340. When added later it uses `ClientUpdate.ItemAdd` (`11 02 00`).
- **Which weapon is held:** the loadout. `loadoutSlots.currentSlotId` (in
  SendSelfToClient and `Loadout.SetLoadoutSlots`) names the active loadout slot.
  The server puts that weapon in its definition's ACTIVE_EQUIP_SLOT_ID and other
  weapons in PASSIVE slots (`basefullcharacter.ts` 321–465).
- **Drawn:** that equipment-slot and attachment entry in
  `Equipment.SetCharacterEquipment` after ClientFinishedLoading, plus
  `Character.WeaponStance` (`0F 20`: characterId u64, stance u32), which their
  comment says "activates weaponstance key".
- **Switching:** the client sends `Loadout.SelectSlot` (`87 06`: u32, slotId
  u32, gameTime u32), handled at `zonepackethandlers.ts` 2929+. The server
  (`zoneserver.ts` 7521–7590) then:
  - clears the old active equipment slot;
  - sets the current slot;
  - re-equips the new weapon as active and the old one as passive. Each step
    sends `Equipment.SetCharacterEquipmentSlot` and `Loadout.SetLoadoutSlots`;
  - sends fire-mode updates to other players.

  There is no dedicated "draw weapon" packet beyond these.

## Suggested native checks, in order

1. Keep SendSelfToClient's equipment rows free of model-bearing items (your
   working "links + items" state). Send the attachment and equipment data
   **after ClientFinishedLoading**, in our client's equivalent of
   `Equipment.SetCharacterEquipment`. Find that packet's reader first.
2. If the arms actor still needs actor+17AC, check whether setting the model
   before the equipment arrives avoids the crash, as 2015's ActivateProfile
   does. Use either ordering from section 3.
3. For the weapon: active loadout slot → ACTIVE equip slot entry plus
   attachment, then a weapon-stance packet.
