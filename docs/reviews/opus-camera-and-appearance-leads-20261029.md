# Opus continuation: camera/preload and body-asset discriminators

October29,2026. Continuation of the paused `character-preview-contract-20261029`
checkpoint. Two read-only tools are added; no backend packet, saved state,
V29.1/V30.1/V31.2 behavior or frozen helper changes. No native execution was
reproduced here: the loaded-image inputs under `/workspace/ps2-native-input`
were not available in this environment, so native claims below cite existing
reviews/harness source only.

## Baseline

Handoff hashes match: source ZIP `32dcf1f2…`, preview evidence `1f8bbbc4…`,
manifest `cbc55ac9…`. The full suite on the unmodified source ran430 tests:
421 passed and9 errored only because frozen helper ZIPs were excluded from the
handoff. Those eight ZIPs were restored from `codex/class-definition-export`
(`929acb28`; activation `b1070038…` and placement `40195906…` match their
recorded hashes) and all9 then pass. This completes the interrupted rerun.

## A. Camera and actor are separately moved; the preload lead

Existing Windows results, combined:

| Run | Server action | Rendered view | Actor | Observer PreloadDone |
| --- | --- | --- | --- | --- |
| 023020 (Oct16) | UpdateLocation | moved to warpgate | moved to warpgate | true -> false |
| 220929 (Oct26) | class19 + UpdateLocation, then W | fixed after placement | moved 9.482 units while W held | true/true -> false |

So the view follows the server-applied location but not later actor
integration. A camera bound to this actor would have translated ~9.5 units.
This narrows the fixed view to camera association/control, not actor motion.

Code finding (source, verified): `gateway.py` answers opcode04 with PreloadDone
only while `preload_done_issued` is false. Any later04 is decoded and logged
with `preload_confirmation_duplicate=true` and no reply. This is the V31.2
once-per-session guard working as designed for startup.

Hypothesis (unverified): the large move puts the client back into a preload wait
(the flag change is observed in both runs) and it signals readiness again. The
backend would drop that signal, leaving the client waiting with its view
detached from the actor. Nothing proves the client re-sent04: no raw gateway
log was supplied with either result. The unplaced activation run had a fixed
view with PreloadDone true, but there the actor never left the origin either,
so it does not discriminate.

Discriminator, no new game run: the 220929 helper folder's
`reports/gateway-*-64044d49d88c2ce8.jsonl` already records every client zone
message header. Run:

```text
python3 gateway_timeline.py <gateway-…-64044d49d88c2ce8.jsonl or local-console-report ZIP>
```

- `ready_signals.after_marker_duplicate >= 1`: the client asked again and was
  ignored. The next narrow experiment answers exactly one post-placement04 with
  the existing Windows-accepted PreloadDone bytes, guarded by a location send
  since the last PreloadDone. First trace what cleared the observed flag on
  the native large-move path.
- `0`: this hypothesis is weakened. Trace `prefixes_first_seen_after_marker`
  natively instead. These are 4-byte prefixes only, not decoded requests.

## B. Why V29 spun, and the VS LA body pair

Native facts from existing reviews: the GUI requests `<model file>` with `.adr`
replaced by `_UI.adr`. Failed lookups fall back to `Armor_NC_Male_Base.adr`
(`196CAFD`/`3451448`), whose `_UI` preview renders as the headless body. V29
sent menu ModelId26 (`Body_VS_Male_Standard.adr`) and the preview spun forever.

Table facts (pinned `Models.txt`): `Body_*_Male_Standard` rows are early
low-ID rows (1/25/26 and duplicates95/96/97). The `Armor_<faction>_<gender>_<class>_Base`
family is a regular male/female pair per faction/class. VS LA has exactly one
pair: 165 (gender1) and 166 (gender2). The native fallback itself is the
same-family row7430 (`Armor_NC_Male_Base.adr`); its female twin is7431. 165/166
also map to the class-specific `FPArmor_VS_Male_LightAssault.adr` arms. The
26/97 ambiguity applies to the shared standard body, not to this unique pair.

Hypothesis (unverified): V29 requested a `_UI` asset absent from this client,
so its callback never completed. 165/166 are then the only PC-valid VS LA base
pair. They remain table-inferred, not recovered live profile19 values.

Discriminator, no game launch: `check_actor_assets.cmd` reads only the exe hash,
pack indexes and three pinned tables. It writes `local-actor-assets-….json`
with index presence for every PC-valid Models.txt ADR and its `_UI` variant,
both `<gender>` expansions of each head, the native fallback, and any
FirstPersonArms ADR (if that table matches its pin). Actor payloads are never
read, and presence does not prove dependencies load.

Expected readings:
- `Body_VS_Male_Standard_UI.adr` absent while `Armor_NC_Male_Base_UI.adr` is
  present supports the V29 explanation.
- `Armor_VS_{Male,Female}_LightAssault_Base{,_UI}.adr` present makes an opt-in,
  explicitly table-inferred class19 +28/+2C = 165/166 trial reasonable. Wire
  offsets42/46 per record are already native-verified by the preview cases.
  Absent `_UI` files predict a spinner for that pair too; do not send it then.

## C. Activation model word — exact native input needed

`native_profile_activation_check.c --packet` reads the decoded model ID at
outer `+48` and component key at `+44`. The handoff does not show which of the
envelope's four wire scalars lands at `+48`. Before any model assignment there,
run that mode once with four distinct marker scalars (for example
`11111111/22222222/33333333/44444444`) against the pinned loaded sections. That
identifies the wire position without guessing. `B9D0E0 -> 1111B60 -> 110CAC0 ->
EDF4F0` then predicts arms `FPArmor_VS_Male_LightAssault.adr` for165.

## Files and validation

- `check_actor_assets.py`, `check_actor_assets.cmd`, `check_actor_assets_tests.py`
  (6 tests; synthetic PACK2 indexes; arms table key column not assumed; corrupt actor payloads prove index-only reads).
- `gateway_timeline.py`, `gateway_timeline_tests.py` (4 tests; logs produced by the
  real `GatewayApplication`/`Gateway.event`, including a dropped post-placement04).
- This review.

Full suite with these additions: see `CURRENT_CHECKPOINT.md`. Limits: fixtures
do not read the user's packs; no Windows run; no native execution here.

## Result: asset check and appearance candidate

The Windows asset report (`validation/evidence/local-actor-assets-20261029-195412-729110.json`)
shows 103 of 126 candidate names present. Every PC `Armor_*_Base` body and its
`_UI` variant exists, including 165/166 and the native fallback 7430. All
`Body_*_Standard` rows (1/25/26/95/96/97) and `FPArms_*_Standard` arms are
absent, which explains V29's spinner. `FPArmor_VS_Male_LightAssault.adr` exists.

`--vs-la-appearance-candidate` is opt-in and only valid with the isolated VS
test character. It sets class19 model words 165/166 and LocalVSTest's menu row
to head1/model165/gender1. The other four characters are the on-screen control.
Defaults are pinned byte-identical. `ps2_vs_appearance_test.zip` is the client
test: expect a VS LA body for LocalVSTest at character select (the others stay
the headless NC fallback), and class19 refs 165/166 in the captures. The actor
model word remains 0, so first-person arms are not expected yet. Gateway gives
`model_pairs` to `profile_probe` only when set, so older helper bundles that
ship the original module keep working.

## Windows result: menu body works; class model words crash login

Menu row head1/model165/gender1 rendered the VS LA base body with an attached
head for LocalVSTest; LocalVS (unchanged) stayed the NC fallback. With class19
model words 165/166 also delivered, every character's login crashed the client
after ProfileDefinitions (the class packet goes to every character; the menu
rows of LocalNC/TR/NSO were unchanged). The class-word path through 118C9D0 is
the native class asset loader whose successful callback path was never executed
offline; trace it before sending class words again. Menu-only v2 restores the
original class bytes. The full VS LA look in the user's live reference also
needs the equipped loadout (armor/helmet/weapon) in the character row.

## Result: PreloadDone is not the control lock

Bootstrap 212014: the client sent no second ready 04; the unsolicited
post-placement PreloadDone (5 s) restored received_preload_done to true, and
the user still could not move or mouse-look. Section A's hypothesis is
rejected. The lock is elsewhere (spawn/deploy lifecycle or controller/camera
activation) and needs native tracing rather than further message guesses.

## Decoded capture 214141: input reaches the actor; the view does not follow

2:02 (31 bytes) is the client's position report: `02 10`, a 32-bit ms-like
counter, two zero bytes, float32 X/Y/Z, four facing-like bytes, flags `00 00 04 28`.
At the origin X/Y/Z are 0 and it streams at ~15 Hz (the actor keeps falling,
velocity Y -0.0556). After placement the first reports carry 2946.908/65.77/126.08,
Y settles, and the stream pauses while idle. 2:fe (41-43 bytes) carries the same
header and position plus extra fields; 2:01 (16-18 bytes) is a short state update.
From 20.8 s to 25.2 s after placement (the user's W/mouse attempt) ~80 2:02/2:fe/
2:01/2:ff reports were sent: the client simulates and reports movement. 0:8e
(67 bytes, 9x while moving) contains the LocalVSTest GUID twice, the current
position (2946.38, 65.49, 127.65) and the placement position, a likely
position-mismatch/acknowledgement request the server never answers. Channel-1
99 05 events name the UI "Deployable Status" and "InstructionEventManager"
opening after placement. Conclusion: movement input is not blocked; the camera
is not following the local actor. Capture v2 extends the window to 60 s.

## Capture 215523 (60 s): the actor walks, turns and stays grounded; only the view is detached

Correction to the layout above: X/Y/Z start at offset 7 of 2:02 (`02 10`, 4-byte
counter, one zero byte). Decoded positions after placement: settle to Y 65.35;
19.0-21.4 s Z 126.08 -> 135.43 at ~4.0 units/s (matches ExpectedSpeed 4.0);
21.4-27.6 s heading changes (X increases at ~0.8 units/s while Z barely moves);
27.7 s a ~0.56 vertical hop; 27.8-30.9 s Z -> 148.9. Byte 27 is 2 while grounded.
About 23 units of grounded locomotion with the user's W and mouse, while the
screen stayed fixed. Input, local simulation, gravity, collision and heading
all work. The open defect is camera attachment to the local actor. 0:8e (67 B,
85 samples) repeats the GUID twice with current and placement coordinates.
Do not pursue input-lock hypotheses further.

## 0:8e decoded across 85 samples (capture 215523)

All 67 bytes, little-endian. Only bytes 3-4, 7-9, 11-13 and 19 ever change.

| Offset | Value | Observation |
| --- | --- | --- |
| 0-2 | `8e 01 00` | constant header |
| 3, 7, 11 | float X/Y/Z | follows the walking actor, ~+0.8 Y above its feet (body centre) |
| 15 | 126.081 | placement Z |
| 19 | u32 6-8 | small varying count/state |
| 23, 27 | 65.784, 65.784 | placement Y twice |
| 31, 39 | 0x1000000000000101 twice | the LocalVSTest GUID |
| 47, 51 | 2946.908, 2946.908 | placement X twice |
| 55 | 2 | constant |
| 59 | 126.081 | placement Z |
| 63 | 0 | constant |

The fixed part reads as the player's own GUID with a zero-size min/max box at
the placement point, while the varying part is the live actor. Hypothesis
(unverified): the client holds the moving local actor and a separate frozen
representation of the same GUID at the UpdateLocation target, and the view is
bound to the frozen one. This matches the October 15 result where the graphics
location equalled the target exactly while the actor differed. Discriminator:
read the graphics-singleton location (scene_observer.graphics_location_state)
after walking; if it stays at the target while the actor moves, the camera
follows that object. Native tracing of what retargets the camera to the local
actor is still the direct route.

2:fe (41-42 B) and 2:ff (43-44 B; two extra bytes `05 10` before the position)
share the 2:02 header and float position, then the same four varying bytes,
`00 00 04 28 00 80 00 00`, a varying 2-4 byte field and `00 00 00 02 00 00 00`
(byte 2 again marks grounded). They appear at movement changes (start, turn,
hop, stop) and read as full-state keyframes between 2:02 deltas.

## Confirmed on Windows: the camera object is frozen at the UpdateLocation target

`validation/evidence/local-camera-20261029-221053-232974-d170502a.json` (sha 49f9fb2dcf3f6c00…),
six read-only samples 1.5 s apart while the user walked after the bootstrap placement:
the actor clean position moved (2946.9, 65.61, 126.08) -> (2948.5, 65.45, 147.72),
velocity up to 4.0, grounded; the graphics singleton camera object
(engine+B028 -> +580 -> +180) stayed bit-identical at (2946.908, 65.784, 126.081),
exactly the UpdateLocation target (not the actor's settled Y). GetPlayerLoc's
active-world-actor branch is taken (actor present). Verdict:
camera_frozen_while_actor_moved, actor span 21.69, camera span 0.0.

The placement wrote the camera position once; no per-frame follow update runs.

### Exact native question (for whoever has the loaded sections)

1. Find every writer of `camera+0x180` where `camera = [[engine+0xB028]+0x580]`.
   One is reached from the UpdateLocation path (A4C615 -> BDFAC0/BDF9B0 -> ...),
   which explains the single write. Find the per-frame/update-tick writer that
   copies the controlled actor's transform (or a view target) into it.
2. Trace that writer's caller back to its gate: which field selects the actor or
   view target it follows, and which flag/state skips the update. Report the
   current values of those fields as bounded observer reads in the next capture.
3. Identify the server message whose handler sets that target/state
   (spawn/deploy/"controlled entity"/camera-mode). That message is the fix.

No guessed values, forced flags or native calls are needed for steps 1-2.

## Native trace with the restored loaded sections (Opus, same day)

Inputs: `/workspace/ps2-native-input/loaded-20261010-032924` from Moon/Codex.
Section SHA-256 match the handoff (.text 1c006b0e…, .rdata 9b77a95d…, _RDATA
2f15e694…). `native_character_preview_cases.py` reproduces 23/23 here.

**Camera attachment.** `.rdata` strings `Attaching camera to arms - actor (%u)`
(3437860), `Camera successfully attached to bone ID %d.` (3437898), `ERROR -
failed to find camera bone on the first person arms actor (%u)!` (34378D0) and
`Detaching camera from arms.` (3437960) are referenced only by E70700. E70700
reads an arms-actor id through `this+0x258` (thunk 9279E); if it is 0 it takes
the `Detaching camera from arms.` path (E70C3E). Otherwise it looks up the bone
named `camera` (string 3437890) on the arms actor (thunk 4AF61) and builds the
camera transform from it. The local actor has model 0, no FirstPersonArms
mapping and no arms resource in every capture, so the camera is never attached
and only the UpdateLocation path (A4C95F -> [engine+B040] -> B32760) moves it.

**Activation model word.** The native outer reader 9B9320 (`--packet` mode) on
a marker packet with scalars 11111111/22222222/33333333/44444444 returns
component_key 0x11111111 (outer+44) and model_id 0x33333333 (outer+48): the
model is the third scalar, wire offset 121 of the 138-byte message. The default
packet stays byte-identical (SHA 517264…). B9D0E0 takes the engine via the
4175A60 XOR table, looks the model up in engine+B098 (thunk DE0E) and, if
found, calls actor virtual+530 (Codex: 1111B60 -> 110CAC0 -> EDF4F0 arms
lookup; 165 -> FPArmor_VS_Male_LightAssault.adr). `--dispatch` with 165 faults
at B9D108 only because the fixture never populates that engine table.

**Next client test.** `--activation-model-id 165` sends the bootstrap activation
with model 165. Expected if the chain holds: actor_model_id 165, arms resource
present, and the camera attached to the arms (the Camera capture verdict
becomes camera_moved_with_actor). It is a different path from the class model
words that crashed login, but it is not proven safe.
