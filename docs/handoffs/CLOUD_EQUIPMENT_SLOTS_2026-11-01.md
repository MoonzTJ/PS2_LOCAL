# Equipping the kit in the world: answers for the local session

From the cloud session, 2026-11-01. Replies to
`PS2_LOCAL_cloud_request_equipment_slots_2026-11-01.txt`. Marked
TRACED / EXECUTED / INFERENCE.

## The likely blocker: ClientGameSettings' 5th field = equipment lookup mode (TRACED)

The check that logs "player does not have anything equipped in the primary
weapon slot!" is at **111492A–111499D**:
1. Player = singleton 4169EB0.
2. Primary slot id = **BC5500()** = `[singleton 41709F0] + 0xF8`.
3. Item = **DAB300(player, slotId)**. **NULL → the error.**
4. Otherwise: item `vtable+0x28` (get the weapon) → weapon+0xC0
   `vtable+0x130` (equip).

**DAB300 / DAB120 / DAB4E0 have two paths, chosen by the global
`[image + 0x405350C]`:**
- **≠ 0:** look the slot up in the **equipment map at player+0x7D840**
  (filled from the SendSelf equipment section), then resolve that entry's
  instance GUID in the owned-items map at player+0x7CAA0.
- **= 0:** a different path (via player+0x218). It **does not read the
  equipment map**, so your v49 equipment links would be ignored.

**Only one writer** sets that global: **C4E1D0(value)**. It stores the value
once, guarded by `[0x4053510]`. It is called only from the main zone switch,
case **opcode 0x60 = ClientGameSettings** (AA4087–AA413B, reader 9BA4F0).

ClientGameSettings in this build is the same field order as 2015's
(soe-network `ClientGameSettings`, 0x61 there):

| Field | Type | Goes to | ps2-emu 2015 value |
| --- | --- | --- | --- |
| dword1 | u32 | engine+B48C | 0 |
| dword2 | u32 | engine+B490 | 7 |
| bool1 | u8 | engine+B494 | true |
| float1 | float | AD74D0 | 1.0 |
| **dword3** | **u32** | **C4E1D0 = equipment lookup mode** | **1** |
| dword4 | u32 | BA1F00(dword4, dword5) | 1 |
| dword5 | u32 | BA1F00(dword4, dword5) | 0 |
| float2 | float | engine+B498 | 12 |
| float3 | float | engine+B49C | 110 |
| byte | u8 | engine+B4A0 (new in this build) | |

**Action:** send ClientGameSettings (`60 …`) at login with **dword3 = 1**,
and use the 2015 values for the rest. If you already send it with 0 there,
that alone explains the empty-slot error. The guard means only the **first**
ClientGameSettings sets it. That is INFERENCE from the guard: send it once,
early, before SendSelfToClient, as ps2-emu did (`zoneservertest.js`, right
after SendZoneDetails).

## 1. What fills the equipment slot map (TRACED + EXECUTED by Codex)

**The SendSelf equipment section**, read by **9AA720** into player+0x7D830
(the map lives at +0x7D840). Codex executed it in
`validation/native_equipment_cases.py` / `native_weapon_refresh_check.c`,
including slot → owned-GUID lookup.

Wire (Codex's builders `equipment_map_wire` / `equipment_record_wire` /
`equipment_slot_wire`):
- **section** = u32 rowCount, then rows of {u32 **key = profile id**,
  record};
- **record** = u32 identifier, string, string, u32 slotCount, then
  {u32 **slot id**, slot};
- **slot** = u32 raw, **u64 instance GUID**, string, string.

Your per-link encoding (u32 slot, u32 0, u64 guid, u32 0, u32 0) is exactly a
slot with raw 0 and two empty strings. Make sure that:
- the **row key** is the current profile (19 for LA). In the lookup, the
  selected profile is player+0x218;
- each GUID is an **owned** instance in the inventory section of the same
  SendSelf;
- the weapon's item definition and weapon definition exist **before**
  SendSelf. You already send them.

The loadout path (`86 07` → 15D9B60) also equips the active slot from the
current loadout into player+0x7D830 later. The SendSelf section is what
populates it at login.

## 2. Primary weapon slot

The slot id comes from `[41709F0]+0xF8`, a field of the manager that also
holds the active slot. INFERENCE: it's the loadout's primary slot, **slot 1**
(where item 44 is). Requirements for that slot:
- an equipment-map entry for the slot;
- its GUID found among owned items;
- the item's `vtable+0x28` returning a weapon object, which needs item type
  26 + a weapon definition.

Dump `[[base+41709F0 decoded]+0xF8]` to confirm the slot number.

## 3. Why v15–v20 crashed at player build (INFERENCE)

Back then, equipped weapons had **no weapon definitions** (no fire
groups/modes) and items had no class records. Building an equipped weapon
dereferences its current fire group/mode: the same NULL as the melee crash at
111F2CD, which we've since fixed. With definitions sent before SendSelf (as
now), the equipment section should no longer crash. If it does, the dump
will say.

## 4. Continent manager zone records (still open)

`A3 13` (from the last answer) fills manager+0x20, not the zone-record list
at manager+0x08. The A3 default path (DBB3D0) handles A3 subs 0x0C/0x0D for
another manager (engine+D588), not zone records. I haven't found the inserter
for M+0x08 yet.

Cheapest way to find it: on Windows, set a hardware **write breakpoint on
`[M]+0x08`** (x64dbg: Breakpoint → Hardware, Write, 8 bytes) before login,
and note the RIP that writes it. If it never fires, the client expects the
records from a packet you don't send yet. Try `A3 13` anyway and report.

## Questions for the local session

Please answer these in your next request (numbers help):

1. Do you currently send ClientGameSettings (base opcode 0x60) at login? If yes, what is the 5th field (dword3)? Please change it to 1, then report whether the 'nothing equipped in the primary weapon slot' error goes away.
2. After that change, dump `[[base+0x41709F0 decoded]+0xF8]` (the primary slot id). Is it 1?
3. In the same dump, is the equipment map at player+0x7D840 populated (count at +0x8+0xA0 > 0)?
4. If the v49 equipment section crashes the player build, send the Breakpad fault address and stack.
5. Deploy screen: set a hardware write breakpoint on `[M]+0x08` (M = [base+0x41DD050]) during login and report the writing RIP, or say that it never fires.
