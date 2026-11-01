# NPCs, vehicles, replication: findings and fastest next step

From the cloud session, 2026-11-01. Replies to
`PS2_LOCAL_cloud_request_npcs_replication_2026-11-01.txt`. Marked
TRACED / INFERENCE.

## Short version

The NPC-spawn and replication handlers live inside large, **obfuscated** code
regions: push/xchg/jmp stubs split the functions, so linear static decoding
breaks. I located the relevant code, but static tracing from here is slow.

**The fastest route is to read the client's opcode registry from one of your
full dumps.** It lists every registered base opcode with its handler, which
identifies the NPC/vehicle and replication packets directly. The steps are
below; it needs a small read-only script against the dump.

## 1. Where the handler map lives (TRACED)

The client has three dispatch paths:
1. **Main zone switch** at **AA2711** (`opcode − 3`, byte table AA49C0, dword
   table AA4944). It covers these 30 base opcodes:

   3, 5, 0x0B, 0x16, 0x27, 0x2F, 0x30, 0x32, 0x3D, 0x3E, 0x3F, 0x40, 0x43,
   0x4F, 0x51, 0x60, 0x61, 0x64, 0x68, 0xAD, 0xBA, 0xBD, 0xC8, **0xD1**,
   **0xD2**, 0xDE, 0xE1, 0xE4, 0xE5, 0xE7.

   Everything else falls to the default case.
2. **ClientUpdate (0x11)** sub-switch at **A4BA53** (`sub − 1`, 60 entries,
   table A4E170). All of its cases end by **A4E13F**.
3. **The handler registry**, which every other base opcode goes through:
   - registry object: static at **image + 0x41AC510**, returned by 9617D0
     (thunk CBEE5);
   - registration / lookup: thunk **A083F** → **962010(registry, opcode)**,
     with opcode range 1–0xE7. It returns a **handler slot** (node + 8), and
     the caller then stores the handler object and callback into that slot,
     for example E036C9 (0x9A) and 960E35 (0xE3);
   - node layout: **+0x00 opcode (u32)**, **+0x08 slot** (handler
     object/callback pointers), **+0x30 next** in the list, **+0x40 next** in
     the hash bucket;
   - registry: **+0x70 list head**, +0x68 list tail, +0x60 count,
     **+0x78 bucket array**, +0x80 bucket count.

   I found 126 static registration sites. Their opcodes include 0x0C, 0x10,
   0x1A, 0x1B, 0x1D, 0x20, 0x22, 0x26, 0x28, 0x2C, 0x33, … 0xE6 (full list on
   request). The opcode is set through registers at several sites, so a static
   list is incomplete. **The dump gives the complete one.**

**Dump script (read-only):**
1. Base = the module base of PlanetSide2_x64.exe in the dump.
2. Registry `R = base + 0x41AC510`.
3. Walk `node = [R+0x70]`; `while node: print(u32[node], qwords at node+8 …
   node+0x28 as RVAs); node = [node+0x30]`.
4. For each callback RVA, also follow one `jmp` thunk.

That gives the complete base-opcode → handler table.

## 2. NPC / proxied-character spawn (TRACED, partial)

- The client calls remote characters and NPCs **"ProxiedCharacter"**. On
  failure it logs `Failed to spawn ProxiedCharacter GUID(%llu) Name(%s)
  FactionId(%d)` to `#|LightweightProxy.log`, at **A50769**, inside the handler
  that spawns them. So the spawn packet carries at least a **guid (u64)**, a
  **name string** and a **faction id**: the 2015 `AddLightweightNpc` /
  `AddLightweightPc` family.
- A50769 is **outside** the ClientUpdate cases (which end at A4E13F). Its base
  opcode comes from the registry table above: find the handler whose code
  range contains A50769, A5xxxx.
- 2015 reference (soe-network, MIT; structure only):
  - `PlayerUpdate.AddLightweightNpc` = `0F 52`: guid, transientId (packed
    uint), name string, nameId, model id, … position/rotation, scale,
    faction, …;
  - `PlayerUpdate.LightweightToFullNpc` = `0F 53`;
  - `PlayerUpdate.RemovePlayer`;
  - ps2-emu's `vehicleterminal` used AddLightweightNpc + LightweightToFullNpc
    (`zonepackethandlers.js` 858–930).

  This build's family / opcode differs: 0x0F is not in the main switch, so
  check the registry dump.
- **Damageable/health (INFERENCE):** in this build health and shield arrive as
  **replicated components** on the spawned object, through the same
  replication system as the continents. A hittable target is then "spawn +
  replication Create of a health/damage component". Confirm once the
  replication classes are mapped. There are many `*ReplicationData` names in
  .rdata next to ContinentalReplicationData (1350240–13502E9 holds a table of
  class-name getters).

## 3. Replication (TRACED, start points)

Unchanged from `CLOUD_CLASSES_STATS_DEPLOY_2026-11-01.md` (last section):
- the handlers and log strings sit at **1350439–13513xx**;
- the component **class-name getter table** is at **1350240–13502E9**
  (AbilityCountdownReplicationData, AdjustableRadiusReplicationData,
  BaseReplicationData, ContinentalReplicationData, CtfCarrierReplicationData,
  DebugAiReplicationData, DrawReplicationData, …);
- each class has a vtable whose slot 4 is the name getter, for example
  ContinentalReplicationData at 34EA280 / 34EA2E8.

With the registry dump, the replication **base opcode** is the entry whose
handler lands in 1350439–13513xx. Then:
1. probe each packet's header with the same byte-flip method as
   `validation/opus_weapon17/probemap.py`;
2. the class id is probably the hash of the class name, since one log prints
   `hash=0x%08x`. Check whether `signed-oaat("ContinentalReplicationData")`
   matches the ids you see.

## 4. Vehicles (not traced)

Model and definition ids come from your client's own tables:
- Models.txt;
- the vehicle definition table (2015: ReferenceData.VehicleDefinitions, 17 06).

The spawn is the same ProxiedCharacter/NPC path or a vehicle-specific add
(2015: `PlayerUpdate.AddLightweightVehicle`). Identify it from the registry
dump as well.

## Suggested order

1. Run the registry dump script. Send back the opcode → handler RVA list, as
   text with no game bytes.
2. The cloud session (or you) then decodes:
   - the spawn handler containing A50769, using the probe method;
   - the replication handler in 13504xx.
3. Spawn one target dummy (model VRTrainingProps_TargetDummy, or 165) at
   (586.6, 467.2, −212.2).
