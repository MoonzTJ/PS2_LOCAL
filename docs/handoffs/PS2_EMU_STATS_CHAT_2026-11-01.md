# Movement stats and chat: answers for the local session

From the cloud session, 2026-11-01. Replies to
`downloads/PS2_LOCAL_cloud_request_stats_chat_2026-11-01.txt`.

Sources:
- psemu/ps2-emu (MIT, `4754260`);
- psemu/soe-network (MIT, `9db135b`);
- h1z1-server (GPL-3.0, reference only, `c7bf3bb`).

I parsed the captured `data/zone/ClientUpdateUpdateStat.dat` myself (514
bytes, fully consumed).

**Not done:** I did no native check of the movement controller in this pass.
Item 2's "which stat id is base speed" is therefore an inference from the
capture, not a confirmation.

## 1. Stat ids and values

**No name → id table exists in these sources.** ps2-emu's `stat` admin command
(`zonepackethandlers.js` 138–166) looks up `"StatId." + name` in a database
collection (`string_hash_to_value`, then `character_stats`) that isn't in the
repo. It then sends `ClientUpdate.UpdateStat` with one entry: the stat id,
type 1, base = the value, modifier 0. So the 28 names can't be mapped to ids
from these sources. The hash → id link has to come from your client's
registry (BC61B0 / A45560).

The **captured UpdateStat** (`11 07 00`, sent on ClientIsReady,
`zonepackethandlers.js` 12) has 39 entries. Each is: stat id u32, type u8,
then two values (type 1 = floats, type 0 = u32). All modifiers are 0.

| Stat | Type | Base |
| --- | --- | --- |
| 1 | 1 | 1.0 |
| **2** | 1 | **4.0** |
| 3 | 1 | 0.75 |
| 4 | 1 | 0.5 |
| **5** | 1 | **1.625** |
| 6 | 1 | 1.0 |
| 7 | 1 | 0.75 |
| 8 | 1 | 0.1 |
| 9 | 1 | 0.25 |
| 10 | 1 | 0.1 |
| 11 | 0 | 3000 |
| 12 | 1 | 3.0 |
| 13 | 0 | 1000 |
| 14 | 1 | 0.25 |
| 15 | 1 | 0 |
| 16 | 1 | 0 |
| 17 | 0 | 25000 |
| 18 | 0 | 750 |
| 19 | 0 | 750 |
| 20 | 0 | 750 |
| 21 | 1 | 0.2 |
| 22 | 1 | 0.15 |
| 23 | 1 | 48.0 |
| 24 | 1 | 0.2 |
| 25 | 1 | 0.2 |
| 26 | 1 | 0.2 |
| 27 | 1 | 0.15 |
| 28 | 1 | 0.15 |
| 29 | 1 | 0.15 |
| 30 | 1 | 1.0 |
| 31 | 1 | 1.0 |
| 32 | 1 | 0 |
| 33 | 1 | 0 |
| 34 | 1 | 1.0 |
| 35 | 1 | 1.0 |
| 36 | 1 | 1.0 |
| 38 | 1 | 8.0 |
| 39 | 1 | 1.0 |
| 40 | 1 | 1.0 |

Stat 37 is absent.

**Inferences, unverified:**
- **Stat 2 = 4.0 is very likely the base run speed.** Your gateway captures
  measured the actor walking at exactly 4.0 units/s.
- **Stat 5 = 1.625 looks like the sprint speed modifier.**
- Stats 3, 4 and 7 (0.75 / 0.5 / 0.75) look like crouch, backpedal and strafe
  modifiers.
- 18–20 (750) look like times in ms, plausibly acceleration/deceleration.
- An old commented-out proxy hack (`soe-network/lib/zoneproxy.js` 104–127)
  rewrote stats 3, 9, 11, 12 and 18–20 in this same packet. Its
  author treated these as the movement tunables, which fits.

**SendSelfToClient stats section:** yes, `stats` (`zonepackets.js` 1806,
statId plus the same typed value). In the capture it has 40 entries, ids 1–40,
**all zero**. The real values arrive later in UpdateStat.

That suggests sprint "does nothing" for you because no stats are ever sent:
stat 5's modifier is effectively 0 or 1.

## 2. Minimal UpdateStat

The wire shape matches your reader (9BA0A0 / 987CE0): `11 | 07 00`, then a u32
count, then per entry:
- key u32;
- id u32;
- type u8;
- two words;
- trailing u32.

In the 2015 capture each entry is only id + type + two values (13 bytes). Your
build adds the extra key and trailing u32, like the item extension map.

Suggested test, after native confirmation:
1. Send one entry: id 2, type 1, base 8.0, modifier 0. Put the key the same as
   the id, as in your item maps, and the trailing u32 = 0. That should double
   the walk speed if stat 2 is the base speed.
2. If it doesn't, send all 39 captured values (keeping ids), then change one at
   a time.

Send UpdateStat on ClientIsReady, after DoneSendingPreload, as ps2-emu does.

## 3. Chat

**`Chat.Chat` `06 01 00`** (`soe-network/lib/zonepackets.js` 2291–2318).
Field order:
1. u16 (`unknown2`);
2. **channel** u16;
3. **characterId1** u64;
4. **characterId2** u64;
5. three u32;
6. **characterName1** (string);
7. a string;
8. three u32;
9. **characterName2** (string);
10. a string;
11. **message** (string);
12. position (float × 4; w = 1.0);
13. a u64 guid;
14. u32;
15. color1 u32;
16. color2 u32;
17. u8;
18. bool.

Your 133-byte client packet fits this with a few extra bytes: message length
at 68 instead of the schema's 63, w = 1.0 just after the message, and the
FF FF FF FF tail plausibly in the guid/colour area. So your build has inserted
about 5 bytes somewhere before the message. Map them in the client reader.

**Filling the sender:**
- **characterId1** = sender's character GUID;
- **characterName1** = sender name. H1Z1-2015 fills only `channel`,
  `characterName1`, `message` and `color1: 1` when relaying player chat
  (`servers/ZoneServer2015/zoneserver.ts` 2404–2422);
- characterName2 / characterId2 = the target, for tells.

**Channel ids:** no source has an enum.
- Both soe-network and H1Z1 use **channel 2 for system/server messages**, with
  empty names (`soe-network/lib/zoneserver.js` 101–129).
- H1Z1 relays player chat on whatever channel the client sent.
- Your client's first u16 is 6. By soe-network's order that is the `unknown2`
  field, not `channel`. Check whether your build swapped the two. "[YELL]" with
  value 6 suggests 6 may itself be the channel.
- Simple test: echo with the name set and the channel field set to 1, then 0,
  and see which label ("SAY", etc.) appears. Find the channel → label table in
  the client's chat UI code to be sure.

No chat packet is captured in ps2-emu.
