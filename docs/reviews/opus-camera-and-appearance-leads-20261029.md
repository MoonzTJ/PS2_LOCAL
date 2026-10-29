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
  (5 tests; synthetic PACK2 indexes; corrupt actor payloads prove index-only reads).
- `gateway_timeline.py`, `gateway_timeline_tests.py` (4 tests; logs produced by the
  real `GatewayApplication`/`Gateway.event`, including a dropped post-placement04).
- This review.

Full suite with these additions: see `CURRENT_CHECKPOINT.md`. Limits: fixtures
do not read the user's packs; no Windows run; no native execution here.
