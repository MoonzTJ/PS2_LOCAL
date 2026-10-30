# H1Z1 server as a login-sequence reference

Context for the local Claude session. Written by the cloud session on
2026-10-30. Nothing here has been confirmed in our client's native code yet.

## Source and limits

- Repo: https://github.com/QuentinGruber/h1z1-server (TypeScript).
  - Packet schemas: `src/packets/ClientProtocol/ClientProtocol_860` (2015) and
    `ClientProtocol_1080` (2016).
  - Login flow: `src/servers/ZoneServer2016/zoneserver.ts` (`sendInitData`,
    `sendCharacterData`) and `zonepackethandlers.ts` (`ClientIsReady`).
- **GPL-3.0: use it only as a reference.** Copying its code into PS2_LOCAL
  would put PS2_LOCAL under the GPL.
- H1Z1 was built from PS2's version of Forgelight, so it is a reasonable map of
  *what* the client expects and *in what order*. Its client changed after the
  split, so field layouts can differ.
- Confirm every packet's byte layout in our client's native code before sending
  it. The class-model words were a guess and crashed every login.

## Opcode alignment

Known ClientUpdate opcodes (`11 xx 00`) match ours with an offset of +2:

| Packet | H1Z1 | Ours (PS2 1533) |
| --- | --- | --- |
| UpdateLocation | `11 0A 00` | `11 0C 00` (confirmed) |
| ActivateProfile | `11 13 00` | `11 15 00` (confirmed) |
| DoneSendingPreloadCharacters | `11 19 00` | `11 1B 00` (confirmed) |
| RespawnLocations | `11 1F 00` (2016) | `11 21 00`? (unverified) |
| ModifyMovementSpeed | `11 20 00` (2016) | `11 22 00`? (unverified) |
| NetworkProximityUpdatesComplete | `11 35 00` (2016) | `11 37 00`? (unverified) |

Base zone packets in H1Z1: SendSelfToClient `03`, ClientIsReady `04`,
ZoneDoneSendingInitialData `05`, ClientBeginZoning `0B`,
ClientFinishedLoading `02`.

## ActivateProfile layout

H1Z1, in order:
1. A profile blob (byteswithlength).
2. An attachment array. Each entry contains:
   - `modelName` (string);
   - `defaultTextureAlias` (string);
   - `tintAlias` (string);
   - a decal string (default `"#"`);
   - a u32;
   - `enableDebug` (u32);
   - `slotId` (u32).
3. Scalars, including `actorModel`.
4. Two strings.

Ours (`profile_activation.py`):
1. `11 15 00`.
2. A counted 102-byte profile.
3. An **empty** attachment count.
4. Four u32 scalars. The third is the model, at wire offset 121.
5. Two empty strings.
6. A final bool.

This independently matches the model word found natively (9B9320 → outer+48 →
B9D0E0). In H1Z1 the attachment array carries the armor and gear models, so the
empty array is a strong lead for "correct body and equipment".

## Login order

**H1Z1 (2016):**
1. ClientBeginZoning
2. SendZoneDetails
3. ClientGameSettings
4. Command.ItemDefinitions
5. ReferenceData.DynamicAppearance
6. SendSelfToClient (full character, with equipment and loadouts)
7. When the client sends **ClientIsReady**, the server replies with:
   - ActivateProfile, with attachments (commented out in 2016, used in 2015);
   - DoneSendingPreloadCharacters (`done: true`, commented "Required for
     WaitForWorldReady");
   - NetworkProximityUpdatesComplete (sent after 5 s, also commented
     "Required for WaitForWorldReady");
   - Character.CharacterStateDelta;
   - the item, profile and projectile definitions;
   - Loadout.SetCurrentLoadout;
   - ZoneDoneSendingInitialData.
8. On ClientFinishedLoading: a game time sync.

**Ours today (`gateway.py` and the bootstrap):**
1. Zone details (IndarZoneDetails).
2. A minimal SendSelfToClient.
3. ZoneDoneSendingInitialData.
4. PreloadDone.
5. Class definitions, then activation (empty attachments, model 165), then
   placement (UpdateLocation).

Our client has the same readiness gate. Strings in its .rdata include:
- `WaitForWorldReady Status: PendingAssetCount=%d IsZoneReady=%d HaveProxiedCharacter=%d ...`
- `WaitForWorldReady Status: InitialZoneDataComplete=%d ReceivedPreloadDonePacket=%d RequiredObjects...`
- `WaitForWorldReady: Waiting for confirmation packet`

## How the gaps line up with the open problems

- **Missing equipment and the wrong look** fit the empty attachment array and
  the missing loadout.
- **The frozen camera:** the first-person arms are never flagged ready
  (target+30E8 stays 0x20, never ≥ 0x80), so AttachCameraToArms (E70700) never
  runs. That may depend on attachment/loadout data or on a proximity-complete
  signal.

## Tasks

1. Build a table comparing H1Z1's login sequence with ours, packet by packet.
   Mark each packet as sent, missing, or confirmed in native code.
2. Check the attachment-entry layout in our activation reader (native 9B9320,
   where it reads the attachment count) against H1Z1's entry.
3. Look for a NetworkProximityUpdatesComplete handler (try `11 37 00` in the
   ClientUpdate dispatch) and for what sets `HaveProxiedCharacter`.
4. Put anything confirmed behind new opt-in flags in the Opus Local Test panel,
   with defaults byte-identical, and package a Windows test for Moon.
5. Record findings in `docs/CURRENT_CHECKPOINT.md` (newest first).
6. At a good stopping point, commit and push to `claude/local-v11`.

Follow every rule in `CLAUDE.md`. In particular, stay inside the local PS2
project folders on Moon's computer.
