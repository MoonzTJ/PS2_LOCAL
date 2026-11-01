# Fire path, weapon switching, client packets (reply to v66)

From the cloud session, 2026-11-01. This replies to
`PS2_LOCAL_cloud_v66_results_2026-11-01.txt`. Client sections are
`loaded-20261010-032924`. Builders are in `validation/opus_npc/build_events.py`
(new: `projectile_record`, `projectile_definitions_body`,
`projectile_mapping_element`, `set_current_slot`).

Tags: **TRACED** = read in the client code; **EXECUTED** = run in the harness;
**INFERENCE** = my reading, not proven.

The in-place class switch and the remove packet working is great.

## 1. Why primary fire does nothing (TRACED)

The shot path in the client (around BE2CB9 → BE2DB4):

1. It gets the **projectile id for the current fire mode and ammo** (14C17A0 →
   14C4AD0):
   - fire mode = the weapon's selected group (+0x84) / mode (+0x88) → that
     fire mode's id;
   - first it checks a per-weapon stat override. If there is none, it asks
     the weapon catalog's **fire-mode → projectile mapping** for
     (fireModeId, current ammoId), the ammo id coming from 14C1B70.
2. **If the projectile id is ≤ 0, the shot is dropped silently.** You send the
   mapping collection empty, so this is where your shots stop: no log, no
   sound, no ammo change. That matches what you see.
3. If the id is > 0 but missing from the **projectile definition table**
   (singleton 41646F0), it prints "Invalid projectile ID %d - fire mode: %u,
   ammo ID: %d" and drops the shot.

So two pieces of data are missing:

### a) The projectile mapping: 5th WeaponDefinitions collection (EXECUTED element)

- The collection (reader 9B2BA0) is a u32 count, then **{u32 key, 12-byte
  element}**. The element (9910C0) is three u32, all read back in order.
- **Field order (INFERENCE, consistent with two independent sources):**
  **{fireModeId, ammoId, projectileId}**, with key = fireModeId.
  - The client's XML loader for the same table stores {fireMode, fireMode,
    AMMO_ID, PROJECTILE_ID} (1244904–124492A).
  - H1Z1 2016 names it {ID, INDEX, PROJECTILE_DEFINITION_ID}.
- **One entry per fire mode** of each weapon, for example for the pistol's
  modes. Use **ammoId = the weapon's ammo slot ammoId (1)** and
  **projectileId = 1**.

### b) ProjectileDefinitions: ReferenceData 0x17, type "ProjectileDefinitions"

- ReferenceData (opcode **0x17**, handler A59200) is selected by a **type-name
  string**: WeaponDefinitions, ItemClasses, ItemCategories,
  ItemClassifications, ProfileDefinitions, **ProjectileDefinitions**,
  VehicleDefinitions, Facility, Requirements, NeedIndicatorSettings,
  Deployables.
- Send it **exactly like your WeaponDefinitions packet**, with the type
  "ProjectileDefinitions": the same decompression step (F7D320), then one
  collection (9AADF0) into the projectile manager.
- **Body:** u32 count, then records. **Each record starts with its own id**;
  there is no separate key. The 2015 capture confirms the same framing: 427
  records, the first id 1.
- **Record (reader 99ECE0, EXECUTED): 176 bytes with empty strings.**

| Wire | Field (H1Z1 order where the widths match; INFERENCE) |
| --- | --- |
| 0 | u32 **id** |
| 4, 5 | u8 flags1, u8 flags2 |
| 6 | string **MODEL_FILE_NAME** |
| next | string **FP_MODEL_FILE_NAME** |
| 25-byte block | u32 audio, float **SPEED**, float velocity inherit, float acceleration, u8 **flight type**, u32 projectile effect, u32 land effect |
| 40-byte block | float **LIFESPAN**, float max speed, float turn rate, float drag, float **GRAVITY**, float damage, 4 × u32 tracer fields |
| next | string (new in this build) |
| then | 59 fixed bytes, one counted array (send 0), 28 bytes |

- The 2015 capture's projectile records are an older layout (about 27 bytes
  shorter), so they can't be replayed.
- **Verified test record:** id 1, model and FP model
  `Bullet_Heavy_Medium_TR.adr` (from the 2015 capture), speed 600, lifespan
  2.0, max speed 600. It is **228 bytes**, and 99ECE0 consumed exactly 228,
  error 0, with the id, speed and lifespan read back.
- `build_events.projectile_record(1, model, fp_model)` builds it.
- **Not executed:** the full collection reader 9AADF0 (it needs the manager's
  container) and the fire itself.

**Test order:**
1. ProjectileDefinitions (one record, id 1).
2. WeaponDefinitions with mapping entries {mode, 1, 1} for every fire mode you
   define.
3. Primary fire. If it now prints "Invalid projectile ID", the
   ProjectileDefinitions packet didn't land.

## 2. Weapon switching: `86 04` (client) and `86 06` (server) (TRACED)

- **The client's `86 04 01 00 00 03 00 00 00 <u32>`** is Loadout.**SelectSlot**,
  the same as soe 0x8904: type 1, two bytes, **loadoutSlotId 3**, gameTime.
  That is the switch request ("go to slot 3"). ps2-emu didn't answer it, but
  this client has a server→client setter for exactly that state.
- **Server reply `86 06` SetCurrentSlot (handler D2B490):** 8 bytes:
  1. `86`, `06`;
  2. type u8 (echo 1);
  3. **mode u8 = 0**;
  4. **u32 slotId**.

  With mode 0, the client:
  1. stores the slot in **loadout manager +0x250** (the active slot you
     mentioned);
  2. searches the current loadout's **slot list at +0x820** for it;
  3. if found, sets the index (+0x838 / +0x83C) and calls the switch (vtable
     +0x28).

  **If the slot isn't in that list, nothing happens.** (Mode 1 sets +0x254, a
  second slot.)
- **The likely cause of the unreliable switching and the empty CLASSES screen
  is the same thing:** since v59 your loadouts carry **no slots**. The slot
  list at +0x820 comes from the current loadout's slots. With it empty, a
  switch has nothing to resolve, and the loadout editor has nothing to list.
  (TRACED for the switch; INFERENCE for the editor.)
- **To test:**
  1. Send the class loadouts **with their slot lists**. The 2015 capture's
     loadouts have 14 slots each (slot → itemLineId; the LA is loadout 17,
     see PS2_EMU_EQUIPMENT_REFERENCE).
  2. Answer every `86 04` with `86 06 01 00 <same slotId>`
     (`build_events.set_current_slot(slot)`).
- **MAX:** the arm weapons (17000 / 29030) live in MAX loadout slots too, so
  the same fix applies first.

## 3. The other client packets

| Packet | Identification |
| --- | --- |
| `84 15 01` (CLASSES screen) | 0x84 is the **Facility** family in this build (35 subs, next to the CTF strings; soe 0x87 after the −3 shift). Sub 0x15 is a facility/spawn query, not the class list, so it is probably unrelated. With loadout slots sent, check whether the screen fills without answering it. |
| `86 04 …` | Loadout.SelectSlot, see §2 |
| `82 00 … ` on channel 3 | **Weapon.Weapon** (0x82 = soe 0x85): u32 game time, then an inner Weapon sub. It appears at login, not on fire, which confirms the client never starts a shot (§1). Once firing works, expect FireStateUpdate / Fire / ProjectileHitReport inside 0x82 |
| `45 01 …` (210 bytes) | 0x45 is probably soe 0x48 **ClientLog**. If it contains readable text, please send it: it may say what failed |

## 4. Ammo HUD "/"

Not traced to the end. The numbers are probably drawn for the current fire
mode's ammo (projectile / ammo path), so the projectile fix may change it.
Re-check after §1.

## Questions for the local session

1. After ProjectileDefinitions + mapping: does primary fire do anything? Do
   you see an "Invalid projectile ID" message or 0x82 packets?
2. With loadouts carrying slots, and `86 06` answering `86 04`: does switching
   become reliable? Does the CLASSES screen fill?
3. What is the readable text in the 210-byte `45 01` packet?
4. LA preview (41735D0 / 417B490): still next on my list after these.
