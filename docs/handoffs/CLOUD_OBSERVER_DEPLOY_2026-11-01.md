# Observer + deploy screen: answers for the local session

From the cloud session, 2026-11-01. Replies to
`PS2_LOCAL_cloud_request_observer_deploy_2026-11-01.txt`. Marked
TRACED / EXECUTED / INFERENCE.

## 2. Deploy screen: the zone list is NOT replication; it's a continent manager (TRACED)

This corrects my earlier guess. The deploy UI reads a **continent manager**,
a static pointer at **image + 0x41DD050**:
- **`GetContinents`**: native 1169F00 → E07360(manager) lists zone ids.
- **`GetContinentInfo(zoneId)`**: native 1169600:
  1. E072A0(manager, zoneId) walks the **zone-record list at manager+0x08**
     (node: +0 → record, +8 → next) and matches **record+0x10 == zoneId**;
  2. E07470 adds status;
  3. it builds `ui.data.ContinentInfo` (field "zoneId" first, then more from
     the record).
- `BaseClient.ContinentBattleInfo` (1784xxx) also reads this same manager.

**Packet `A3 13` updates it** (handler 0xA3 → DEF980):
- sub 0x13 → reader DED2F0 → **E07B60(manager, packet)**. That copies the list
  into manager+0x20 and refreshes the UI.
- Other A3 subs: 0x12 (reader DECDA0), 1 and 6 (timers/floats). The rest go
  to DBB3D0.

**`A3 13` wire (EXECUTED, real readers DED2F0 / DED080 / DECAE0):**
1. u8 `A3`, u8 `13`;
2. u32 **count**;
3. per entry **17 bytes**:
   - u32 (entry+0);
   - u8 (+4);
   - u32 (+8);
   - u32 (+0xC);
   - u32 (+0x10).

The names aren't proven. By analogy with a zone status entry the likely
meaning is zoneId, flag/locked, faction, then two values (population, time).
Try zoneId first.

**What's still missing:** the **zone records** themselves (manager+0x08)
come from somewhere other than `A3 13`. I couldn't find their inserter
statically. Whether your deploy screen shows only "CONTINENT" because that
list is empty or because `A3 13` is missing, **one dump answers it**:

**Dump check (read-only, 1 minute):**
1. `M = [base + 0x41DD050]`.
2. Walk `node = [M+0x08]`. For each node: `rec = [node]`; print `u32[rec+0x10]`
   (zoneId) and the first 0x40 bytes of rec as hex; `node = [node+8]`.
3. Also print `[M+0x20]`, `[M+0x28]` and `[M+0x30]` (the A3 13 list).

- **If records exist** (your zones): just send `A3 13` with one 17-byte entry
  per zone, zoneId first.
- **If the record list is empty:** send me the record layout of any one entry
  from a capture or another client state, or the call stack of who allocates
  0x?? records into M+8. The 0x68-byte manager's destructor is E06590, which
  shows its fields.

The replication findings (`0x57` Create, ContinentalReplicationData
0x4E963FD0) stay valid. That component feeds **zone ownership logic**
(consumer 17D87A4 → 17D9F50, comparing per-faction values), not the deploy
list. Don't spend time on it for the zone names.

## 1. Observer (TRACED, partial)

- The observer mode is gated by the local actor's flag **actor+0x9F7 bit 4**
  (the "IsObserver" state, near the alive flag at actor+0x9F0 bit 0).
  - Function 13A3304: if `(actor+0x9F7 & 4)`, it builds a **base 0xE6**
    client packet (`dword 0xE6`) and sends it via 1385510.
  - Otherwise it prints **"You are not an observer."**
- The client also handles an incoming **0xE6** family (registry handler
  139A040, **54 sub-opcodes**, table 139B04C, `sub − 1`). Sub 1 → reader
  138F6D0 → 139EB40. This family (spectator/observer) is the most likely
  home of the server's reply to `09 DF 04`.
- INFERENCE: the server should
  1. set the actor's observer state flag, through whatever packet writes
     actor+0x9F0…0x9F7 (the 2015 analogue is PlayerUpdate CharacterStateDelta
     / UpdateCharacterState);
  2. then answer within the 0xE6 family.

  Whether an actual observer *vehicle* spawns (0F 56 AddLightweightVehicle,
  reader 999260 = the 0F 55 body + guid + 2 u32 + position update + string;
  see `CLOUD_NPC_SPAWN_REPLICATION_2026-11-01.md`) I couldn't confirm. No
  "observer vehicle" id appears in the client strings, only
  ObserverCamera / observer_* settings.
- The sprint loss after `observer` with no reply fits the client switching
  into the observer camera state locally and never getting the server's
  confirmation.

**Dump check:** after typing `observer`, dump and read
`local actor + 0x9F0 … 0x9F8`, then compare with a dump before it. If bit 4
of +0x9F7 is 0, the server must set it. To find which packet writes those
flags, look for writes to `[actor+0x9F7]` (`or byte [..+0x9F7], 4`) in the
handler table you already have.

## Next, cheapest first

1. The manager dump (deploy screen).
2. The actor flag dump (observer).
3. The 0xE6 sub-table decode, once we know what the server should send.
