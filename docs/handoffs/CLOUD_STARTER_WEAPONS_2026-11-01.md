# Remaining starter weapons: answers for the local session

From the cloud session, 2026-11-01. Replies to
`PS2_LOCAL_cloud_request_starter_weapons_2026-11-01.txt`.

Source: the 2015 ps2-emu capture (MIT), parsed with soe-network schemas:
- weapon definitions: 775 weapons, 751 fire groups;
- item definitions: 403 items, fully consumed, parser
  `validation/opus_weapon17/items2015.py`;
- SendSelf inventory instances: 224 weapons.

The item → PARAM1 keys in the capture **match yours exactly**:
44→17, 19→9, 21→13, 44705→75, 7→6.

**Full export:** Moon has three JSON files (sent directly, **not committed**:
bulk captured game data stays off the public repo):
- `ps2_2015_weapon_items.json`: every weapon item, with its definition, its
  weapon record, fire groups → mode ids, and its inventory instance;
- `ps2_2015_weapon_definitions.json`;
- `ps2_2015_item_definitions.json`.

Put them in your private workspace.

Fire-**mode** field values are not included: soe-network's fire-mode schema
doesn't match the capture. The mode **ids** are reliable.

## Correction for the knife (weapon 9)

The knife has its **own timings**, not the carbine's:
- equip 250, unequip 250, iron sights 150/150, iron-sights anim 125/150,
  sprint recovery 300;
- range string 673;
- **no weaponGroupId (0)**;
- animation "Knife", melee 0.15 × 0.1, pitch ±85;
- no ammo slots;
- groups 1 → [1], 7 → [39].

**Instance (item 19):**
- 0 ammo pairs;
- group 1 with **one** mode entry {0, 0, 500, 0};
- group 7 with **one** mode entry {0, 0, 500, 0};
- byte 1.

## Per weapon (2015 values)

Every weapon below has turn/move modifiers 1.0 and pitch −85 to 85.

| Item → weapon | Group / anim | Timings (equip, unequip, IS to/from, IS anim to/from, sprint) | Ammo slots (ammoId, clip, capacity) | Fire groups → modes | Instance |
| --- | --- | --- | --- | --- | --- |
| **21 → 13** (pistol, Weapon_VS_PistolBeamer001) | group 21, "Pistol" | 250, 250, 150, 150, 125, 150, 300; range string 673 | (1, **17, 85**) | 11 → [11, 111], flags 8 | ammo (17, 68); group 11 with 2 entries {0, 0, **167**, 0} |
| **44705 → 75** (grenade, model `…HandGrenade001_Held.adr` in 2015) | **flags 64**, "Knife" | 250, 250, 150, 150, 0, 0, 300 | (1, **1, 1**) | 75 → [75], flags 8 | ammo (1, 0); group 75 with 1 entry {0, 0, 0, 0} |
| **7 → 6** (spawn beacon) | "Rifle" | **450**, 250, 150, 150, 125, 150, 300; **nextUseDelay 300000** (5-minute cooldown) | (1, 1, 1) | 6 → [6], flags 8, **deployableId 3** | ammo (1, 0); group 6 with 1 entry {0, 0, 0, 0} |
| **802768 → 77851** (rocket rifle) | **not in the 2015 capture** (newer item) | | | | |
| **6015553 → 78837** (scanner) | **not in the 2015 capture** | | | | |

For the two missing weapons (INFERENCE): build a minimal record so the client
has *some* valid group and mode:
- the rocket rifle like the pistol pattern: one ammo slot (ammoId 1, e.g.
  clip 1, capacity 5), one fire group → one mode, selected 0;
- the scanner like the beacon pattern: ammo (1, 1, 1), one group → one mode.

Use any unused group/mode ids. The crash at 111F2CD only needs a current
group/mode entry to exist.

## Movement lock after weapon select: likely zeroed fire-mode modifiers (INFERENCE, strong)

Our fire-mode records are all zero. Aligning my 330-byte map
(`PS2_EMU_WEAPON17_2026-11-01.md`) with soe-network's field order:

| Wire offset (inner record) | Field |
| --- | --- |
| 142 (u16) | fireDetectRange |
| 144 | effectGroup |
| 148 | playerStateGroupId |
| **152** | **movementModifier** (float) |
| **156** | **turnModifier** (float) |
| 160, 164 | unknown |
| 168 | lockOnAngle |
| … | |

So while a weapon is drawn, **movement and turn are multiplied by 0.0**: the
"movement locks after select" symptom.

**Test:** in every fire-mode record, write **1.0f at wire bytes 152 and
156**. The alignment is by type and order, not executed, so try it and look.

While there, consider `defaultZoom` (a float later in the record, soe order
after the lock-on block). A zoom of 0 may matter for aiming; once 152/156 are
confirmed I can pin its offset.

## Medic / Engineer default rifle + pistol

The 2015 class loadouts give these item **lines** (slot → itemLineId):
- **Medic (profile 20 / loadout 18, tab 256; the capture has no tab 0):** slot 1 → **100**, slot 3 → **2**,
  slot 5 → 54, slot 8 → 3040, slot 9 → 14, slot 11 → 76501, slot 14 → 7;
- **Engineer (profile 21 / loadout 19, tab 0):** slot 1 → **3276**, slot 3 → **2**,
  slot 5 → 134, slot 8 → 138, slot 9 → 14, slot 11 → 140, slot 13 → 5872,
  slot 14 → 7, slot 15 → 800598, slot 16 → 856.

The HUD's default rifle + pistol most likely come from the client resolving
each class's **default loadout item lines** (slot 1 = primary, slot 3 =
pistol; line 2 is the shared pistol line) through its ItemLines table (rank-0
member). Look those lines up in your client's ItemLine/ItemLineMember tables
to get the item ids. Then send each item's definition, weapon record and
instance exactly like 44/21. That part is INFERENCE; the item-line ids are from
the capture.

## Carbine shows no model after 86 07

No Weapon_* asset is ever requested, so the draw never gets far enough to
attach the model. The movement lock suggests the weapon state machine is
stuck right after select. Fix the fire-mode modifiers first and retest: the
draw sequence uses the weapon's equip time (550 ms) and animation set.

If still no model, send a dump at the moment after 86 07. Which function is
waiting (the weapon state at the active slot) will show what's missing, for
example the animation set "Rifle" lookup or the item's ACTIVE equip slot.

## Questions for the local session

Please answer these in your next request (numbers help):

1. With 1.0 at fire-mode bytes 152/156: can you move while a weapon is drawn?
2. Does the carbine model appear now? Is any Weapon_* asset requested after `86 07`?
3. Which item ids did ItemLines 100 / 2 / 3276 resolve to in your client? (You sent 20 and 29000. Was the pistol line 2 resolved too?)
4. Any new crash on switching weapons: send the fault address.
