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
