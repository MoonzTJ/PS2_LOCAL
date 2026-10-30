# PS2_LOCAL — context for Claude Code

Moon is building a local PlanetSide 2 emulator: a Python server for their own
copied Windows client, build v0.0.1533.609523. This is local emulator work only:
no live services, no bypassing anti-cheat or access controls.

## How we work

- The agent writes patches, tests and Windows test packages. Moon runs them on
  Windows and sends back the JSON reports and logs.
- Work on a branch, never `main`. Commit or push only when Moon asks. Claude's
  branch is `claude/opus-continuation-20261029`. Codex keeps its own saved
  checkpoints (latest: `PS2_LOCAL_Codex_Camera_Binding_2026-10-29.zip`).
- Build Windows tests on the "Opus Local Test" panel
  (`scripts/run_vs_appearance_panel.py` and
  `scripts/package_vs_appearance_panel.py`). It wraps the frozen Local Test panel
  `downloads/ps2_local_test.zip` (sha256 `77257724…`). Older helpers open too
  many windows.
- Write each step into `docs/CURRENT_CHECKPOINT.md`, newest entry first.

## Milestone

A playable VS Light Assault through normal login:
- correct body, head and equipment;
- spawning at the Indar warpgate;
- looking, walking and jumping;
- first-person arms and a weapon;
- logout and reconnect.

## Rules

- On Moon's computer, work only inside the local PS2 project folders: the
  PS2_LOCAL repo, the copied PS2 game folder, its reports/test output folders
  and the native-input folder. Don't read, change, move, delete or install
  anything elsewhere: no system settings, other programs, other user folders,
  global Python/npm packages or environment variables. If something outside
  those folders seems necessary, stop and ask Moon first.
- Don't invent mappings, force native ready/context flags, or label assumed
  defaults "recovered".
- Keep V29.1 preview rollback, V30.1 controls, V31.2 delayed preload and all 28
  frozen helper ZIPs unchanged.
- New behavior is opt-in, with defaults byte-identical.
- Don't call anything "playable" based only on offline fixtures.
- Never put the native game sections (`loaded-20261010-032924`) or anything
  copied from the game on the public GitHub repo.
- Don't put model identifiers in commits.

## Established

- **Character select:** rows with HeadId 1 and ModelId 165 (VS), 159 (NC) or
  143 (TR) render the faction LA bodies.
- **Class-definition model words:** 165/166 crash every login. That option
  stays off (`--vs-la-class-models`).
- **Activation model word:** model 165 (`--activation-model-id 165`) gives the
  VS LA body with no crash.
- **Movement:** after placement the body walks (2:02 position reports, X/Y/Z
  floats at offset 7), but the camera stays at the teleport point.
- **Capture 225019:** the active camera is already first person (type 19,
  vtable 3436CD8), not the transition camera (21/22).
- **Arms gate (Codex):**
  - The type-19 update E7F100 reads its target at camera+258.
  - The arms-id getter 110FA30 returns 0 unless target+30E8 >= 0x80 ("arms
    ready"). Observed flags are 0x20.
  - So AttachCameraToArms (E70700) is never called.
- **Arms ready bit (Claude, unconfirmed):**
  - No instruction sets bit 0x80 of +30E8 directly.
  - It is cleared (`and 0x7f`) when the actor's arms watcher is reset, at
    1109ADE and 110CB1D. The watcher is at actor+30E0, vtables 35972C0 and
    3597310.
  - So the arms-resource completion path probably sets it. Watcher slot 4
    (17D71D0) calls into the owner.

## Next

1. Analyze Moon's v9 run (`ps2_camera_binding_v9.zip`). Its Capture Camera adds
   `first_person_binding`: camera target vs local actor, cached arms id and raw
   flags. The run produces the `local-camera-….json`, bootstrap ZIP and server
   report ZIP.
2. Trace what sets the arms-ready bit, and which asset or server message it
   waits for.

Read first:
- `docs/handoffs/OPUS_TO_CODEX_2026-10-29.md`
- `docs/CURRENT_CHECKPOINT.md`
- `docs/reviews/camera-arms-gate-20261029.md` (in Codex's checkpoint)
- `docs/reviews/opus-camera-and-appearance-leads-20261029.md`
