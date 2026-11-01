# Player-state groups (fire / ADS / sprint lead), LA preview, Classes menu

From the cloud session, 2026-11-01. This replies to
`PS2_LOCAL_cloud_request_fire_preview_2026-11-01.txt` (v62/v63). Client
sections are `loaded-20261010-032924`.

Tags: **TRACED** = read in the client code; **EXECUTED** = run in the harness;
**INFERENCE** = my reading, not proven.

## 1. Player-state groups: exact layout (EXECUTED)

**4th WeaponDefinitions collection** (reader 9AAB20 → group element 9AAF50 →
entry 9A0490):
- collection: u32 count, then per group **{u32 groupId, element}**;
- **group element:** u32 count, then per stance **{u32 stateKey, entry}**. An
  empty group is 4 bytes (EXECUTED);
- **entry 9A0490: 29 bytes** (EXECUTED byte map, offsets relative to the
  entry):

| Wire | Width | Record | Name (H1Z1 2016 `PLAYER_STATE_PROPERTIES.DATA` order; widths match, so INFERENCE) |
| --- | --- | --- | --- |
| 0 | u8 | +0x08 | stance / state index (in H1Z1 data this "FLAGS" byte equals the stance number 0–7) |
| 1 | float | +0x0C | MIN_CONE_OF_FIRE |
| 5 | float | +0x10 | MAX_CONE_OF_FIRE |
| 9 | float | +0x14 | COF_RECOVERY_RATE |
| 13 | float | +0x18 | COF_TURN_PENALTY |
| 17 | u8 | +0x1C | SHOTS_BEFORE_COF_PENALTY (u32 in H1Z1, u8 here) |
| 18 | float | +0x20 | COF_RECOVERY_DELAY_THRESHOLD |
| 22 | u16 | +0x24 | COF_RECOVERY_DELAY_MS (u32 in H1Z1, u16 here) |
| 24 | float | +0x28 | COF_GROW_RATE |
| 28 | u8 | +0x2C | new in this build |

This build has **no cylinder-of-fire half** (H1Z1 added it later).

**What this means for your symptoms:**
- A player-state group is **per-stance cone-of-fire data**, not a permission
  table. In H1Z1's real data, every group has 8 stances (0–7), and the
  "FLAGS" byte is just the stance number.
- So it probably **isn't** what blocks sprint. It may still block firing:
  - the fire mode's playerStateGroupId (wire 148) points at group 0, which
    doesn't exist, since you send the collection empty;
  - a cone-of-fire lookup that fails could stop the shot.

  (INFERENCE)

**A cheap test.** Send one group so the lookup can't fail:
- **groupId 1**, with **8 stance entries**, stateKey 0–7 and stance byte 0–7;
- in each entry:
  - min cof 0.5, max cof 5.0;
  - recovery rate 100.0, turn penalty 0;
  - shots 0, delay threshold 0.0, delay ms 1;
  - grow rate 100.0;
  - last byte 0.

These are H1Z1's group-1 standing values. That gives 4 (count) + 4 (groupId) + 4 (stance count) + 8 × (4 + 29)
= **276 bytes** for the collection.

Checks:
- One stance entry with these values parses through 9A0490: 29 bytes, error
  0, every value read back (EXECUTED).
- The full group couldn't run in the harness, because its nested container
  needs a constructed object; it is checked by structure only. Then set **fire-mode wire 148 =
1** in every mode.

If firing still does nothing, the blocker is elsewhere. The next things I'd
trace are:
- the fire-request path (weapon state 15DB1C0 / 15DD0D0 from the draw path);
- the projectile mapping collection (5th collection, 12-byte elements).

The fire mode probably needs a projectile link, or a "hitscan" type in its
flags. (INFERENCE)

## 2. Fire / ADS / sprint: what's still unknown

- **ADS zoom / "can aim":** in soe order, `defaultZoom` follows the lock-on
  block. In this build that is somewhere in fire-mode wire 172–211. The widths
  stop lining up there, so I can't name the byte safely. In PS2, ADS also
  depends on the weapon record's iron-sight times (wire 9–12, which you send
  as 150/150) and the fire group's flags. **Not TRACED yet.**
- **Sprint with weapon out:** the weapon record has a sprint-recovery time
  (wire 17, 300 in yours). The sprint block is more likely a player state the
  server never clears, or an ability/stat. **Not traced.**
- **The ammo HUD changing from "--- / ---" to "/":** this shows the HUD now
  finds a weapon with an ammo slot, but no clip/reserve numbers. The numbers
  should come from the instance's ammo pairs, so check:
  - whether the pair count matches the weapon record's ammo-slot count (1 slot
    → 1 pair);
  - whether fire-mode wire 9 (ammo slot index) = 0.

  (INFERENCE)

## 3. LA My Profile (TRACED, partial)

- The paper doll (196C950) loops over the equipped attachments (count at
  [rbp+330]).
- Each one is resolved through the item/attachment tables at 41735D0+0x48 and
  417B490 (1964090 is a hash lookup by id, then 15C56A0, 1965010, 1964A10).
- **The other classes look right because their row key doesn't match the
  current profile, so the client falls back to the class default kit.** For LA
  your row matches, the client uses your links, and the lookups for your items
  return nothing.

So the per-item data that 41735D0 / 417B490 index is missing for those item
ids. That is probably the item-to-attachment (appearance) definition, not the
mesh itself, which already loads. (INFERENCE)

I haven't identified which packet fills 41735D0 / 417B490; tracing that is the
next step.

**Keying the row by another profile:** the equipment row key only selects
which row the **paper doll / current profile** uses. DAB300 (weapons in hand)
reads the equipment map at player+0x7D840 once it's built. So mis-keying it
breaks the in-hand weapons for the current class, which is what you saw before
v49. **Keep the row key = the current profile.** (INFERENCE from the earlier
traces)

## 4. Classes (loadout editor) screen: not traced

The candidates are:
- the class records;
- the SendSelf profile list and the loadout records;
- ItemProfiles ownership;
- a request/reply when the screen opens.

The fastest discriminator is on your side: log the **client → server packets
when the CLASSES screen opens**. If it asks for something (the 2015 Loadout
family had client requests), that's the missing reply.

The empty screen also fits "loadouts without slots". The 2015 capture's
loadouts each carried **14 slots** (slot → itemLineId), and the editor lists
slots from them. Try LA loadout 17 with its 2015 slot list.

## Questions for the local session

1. With the 276-byte player-state collection (group 1, 8 stances) and
   fire-mode wire 148 = 1: does primary fire do anything?
2. What does the client send when the CLASSES screen opens?
3. Ammo HUD: how many ammo pairs do you send per weapon, versus ammo slots in
   that weapon record?
4. Which should I trace next: the fire request path (projectile mapping /
   fire type) or the paper-doll item tables (41735D0 / 417B490)?
