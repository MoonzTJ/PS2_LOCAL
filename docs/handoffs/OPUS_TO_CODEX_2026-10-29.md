# Opus → Codex handoff — October 29, 2026

From Claude Opus (cloud session), for Moon to pass to Codex. Branch:
`claude/opus-continuation-20261029` (based on `codex/class-definition-export`
929acb28). Nothing on `main` or your branches was changed.

## 1. How to pick this up

- **Source:** your paused checkpoint `character-preview-contract-20261029`
  (`PS2_LOCAL-source.zip` 32dcf1f2…) plus 15 commits, delivered as
  `downloads/PS2_LOCAL_Opus_Continuation_2026-10-29.zip` →
  `opus-continuation-20261029.patch` (`git am`, base = that source tree).
  Apply it to `/workspace/PS2_LOCAL` on a branch; it touches no frozen helper.
- **Validation at the last commit:** full suite **460 tests pass** (`full-tests.log`
  in the bundle). The interrupted 430-test rerun from your pause also passes
  once the 8 frozen helper ZIPs (excluded from the handoff) are restored.
- **Native inputs:** I used the `loaded-20261010-032924` sections you sent;
  hashes match your pins. `native_character_preview_cases.py` reproduces 23/23.
- **Details:** `docs/reviews/opus-camera-and-appearance-leads-20261029.md`
  (evidence and decoding) and the newest entries of `docs/CURRENT_CHECKPOINT.md`.

## 2. What is now established (Windows or native evidence)

| Finding | Evidence |
| --- | --- |
| V29's spinner: `Body_*_Standard` (1/25/26/95/96/97) and `FPArms_*_Standard` files are absent from this client; every PC `Armor_*_Base` body and `_UI` variant is present (incl. 165/166, 159, 143, fallback 7430) | `validation/evidence/local-actor-assets-20261029-195412-729110.json` (Windows pack-index scan) |
| Character-select rows with HeadId 1 / ModelId (165 VS, 159 NC, 143 TR) / Gender 1 render the faction LA base body with a head; NSO unchanged | Windows screenshots, v2/v3 |
| Class19 model words 165/166 (wire 42/46) in ProfileDefinitions **crash every character's login** right after the class packet | Windows v1; kept behind `--vs-la-class-models`, off |
| Post-placement PreloadDone (answered or unsolicited) is accepted (flag back true) but **does not** unlock anything | bootstrap 212014 |
| 2:02 is the client position report (X/Y/Z float at offset 7); after placement the actor **walks ~23 units at 4.0 u/s, turns, hops, stays grounded** | gateway captures 214141/215523 |
| The graphics camera object (`[[engine+B028]+580]+180`) stays **bit-identical at the UpdateLocation target** while the actor walks 21.7 units | `validation/evidence/local-camera-20261029-221053-232974-d170502a.json` |
| **E70700** attaches the camera to the first-person arms actor's `camera` bone (strings 3437860/3437898/34378D0) and takes `Detaching camera from arms.` (3437960) when the arms id (via `this+0x258`) is 0 | static, restored sections |
| **Activation model word:** native 9B9320 `--packet` on a marker packet puts the third raw scalar (wire offset **121**) into outer+48 (model) and the first into +44 (component key) | `native_profile_activation_check --packet` |
| B9D0E0: engine from 4175A60 XOR table → engine+B098 model lookup (thunk DE0E) → actor vfunc+530 if found; `--dispatch` with 165 faults at B9D108 only because the fixture lacks that table | static + fixture run |

## 3. What is in the branch (all opt-in; defaults byte-identical)

- `check_actor_assets.py/.cmd` — read-only pack-index presence scan.
- `gateway_timeline.py` — client-message timeline from gateway logs.
- `vs_appearance.py`, `presets.AppearanceRoster` — `--vs-la-appearance-candidate`
  (menu rows only); `--vs-la-class-models` (crashes, off).
- `gateway.ZonePayloadCapture` — `--zone-payload-capture` (bounded full bytes,
  activation → placement+60 s).
- `--post-placement-preload` (rejected hypothesis, kept for record).
- `camera_capture.py` — read-only actor vs camera samples (`Capture Camera.cmd`).
- `profile_activation.activation_packet(model_id)` + `--activation-model-id 165`.
- `scripts/run_vs_appearance_panel.py` / `package_vs_appearance_panel.py` —
  "Opus Local Test" = your frozen Local Test panel (77257724…) plus the above.
  It wraps only these frozen expectations: activation description (model 165),
  bootstrap phase model 0→accepts 0/165, and the readiness checks for the new
  switches. Every other frozen guard runs unmodified.

## 4. Pending on Windows

**Opus Local Test v7**: bootstrap activation with model 165. Expected if the
chain holds: actor_model_id 165, first-person arms resource present, camera
attached (Camera capture verdict `camera_moved_with_actor`). Possible crash.

## 5. Suggested next tasks for Codex (you have the full workspace)

1. **Run the model-165 activation natively past B9D0E0.** Populate the 4175A60
   engine table and engine+B098 model manager from the pinned Models.txt in
   the activation oracle, then execute 1111B60 → 110CAC0 → EDF4F0 with
   FirstPersonArms.txt. Report whether arms resource requests are made and
   whether any path dereferences state our server never sends (crash risk).
2. **Trace E70700's callers.** Confirm it runs on arms-resource completion,
   and what else gates it (resource callback, actor state, camera mode).
3. **Explain the class-model crash.** Run ProfileDefinitions with 165/166
   through your class-asset loader fixture (118C9D0 path) to find the faulting
   dependency before anyone sends class model words again.
4. **Decode the 0:8e consumer/producer.** 67 bytes, the player GUID twice plus
   the placement point as a zero-size box and the live actor centre; layout
   in the review doc.

## 6. Working back to back

- Keep separate branches; record each step in `docs/CURRENT_CHECKPOINT.md`
  (newest first) and point the other side at the commit.
- Avoid editing the same files concurrently: `gateway.py`, `local_server.py`,
  `presets.py`, `profile_probe.py`, `profile_activation.py` were changed here.
- Moon relays Windows results to whoever is active; both sides read the same
  evidence files.
