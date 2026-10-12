# Local PS2 test helpers

## Next capture: complete static class definitions

[Download ps2_class_definition_export.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_class_definition_export.zip)

Use a signed-in GitHub session and choose **Download raw file**. Extract the
ZIP to its own folder. Enter the normal online game, spawn once, leave that
game window open, and double-click **capture_class_definitions.cmd**. Upload
the generated **local-class-definitions-....zip** from its **reports** folder.
Close the local game window first so only one game process is running.

This read-only helper verifies the matching executable and exports only static
numeric VS class definitions, including both collections. A different build
produces a hash report before memory is opened. Keep V31.2 installed in the
local copy. This capture is separate from a local loading-screen trial.
Revision 2 fixes the first capture's missing executable path by querying Windows
directly with limited process access. No install-folder selection is needed.
The console displays **Read-only class exporter revision 2**. If Windows denies
access, upload the report with its stage and error code.
Standalone package checks and 217 Python tests pass; 357 native expectations
include full record reconstruction. The revised Windows helper still needs a
capture on the user's PC. See `docs/CLASS_DEFINITION_EXPORT.md`.

Revision 2 package SHA-256:
`b88b12f44428fb04ad9cf4f18f7b86efab5330bc1b888fea98152a2e839b3b56`.

## Earlier test: V31.2 preload timing

[Download ps2_preload_timing_v31_2.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/main/downloads/ps2_preload_timing_v31_2.zip)

On the GitHub file page, click **Download raw file** near the upper right. Use
your signed-in GitHub session.

1. Close the game, client launcher, controls window, and local server.
2. Save the patch ZIP, choose **Extract All**, and double-click
   **apply_preload_patch.cmd**.
3. Choose your existing **V31 test folder** containing
   `start_local_server.cmd` and `start_local_client.cmd`. The installer verifies
   the three original source files and backs them up automatically.
4. Restart the server, client and controls from that same test folder.
   Select **LocalVS** and wait before clicking Play.
5. Use your existing **ps2_loading_observer** helper below. Capture Play once
   for **90 seconds** and upload the timeline ZIP and same-run console report.

This patch sends PreloadDone after the client's native readiness message,
following the loading flag reset. The next check is whether the preload flag
stays true. Complete class definitions are still missing; world entry remains
unfinished. The packet bytes, starter data and other V31 controls are preserved.

Package SHA-256: `92f448427a181984f876a1602c45ab0ba539e94699ec88a9c18a7b057325c2fd`.

Validation: 151 Python tests and 83 native world checks passed, including native
loading reset and readiness-message serialization. Five installer checks passed,
including rejection of different source files/corrupt payloads and rollback
after an interrupted write. The Windows folder picker and patched live game
need the next Windows run. No executable, assets, credentials or logs are shipped.

## Optional launcher for one test folder

If you have several test folders, an optional helper can apply V31.2 and open
the server, client and controls from one verified folder:

[Download ps2_preload_timing_v31_2_launcher.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/main/downloads/ps2_preload_timing_v31_2_launcher.zip)

Close all old consoles, **Extract All**, and double-click
**start_patched_test.cmd**. Choose the existing V31 test folder. Wait for
**V31.2 SERVER VERIFIED**, then perform the same LocalVS timeline capture.
The helper refuses occupied server ports and checks the running build before
opening the client and controls. It preserves backups and also accepts the
original V31.2 patch. The server startup banner now explicitly identifies V31.2.
The original timing patch above remains valid; its packet behavior is unchanged.

Package SHA-256: `0b06458a7a3b668025f1767c2009e5214fdc2a43a2d347e291c84c534c36fba3`.

Validation: all 158 Python tests pass. A patched copy of the original V31 archive
starts the backend with the expected fingerprint and preserves the zone/starter
data. The launch-order and wrong-build guards pass. Actual Windows window/game
launches remain for Windows verification.

## Loading timeline helper

[Download ps2_loading_observer.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/main/downloads/ps2_loading_observer.zip)

1. Save the ZIP and choose **Extract All** in Windows.
2. Start your patched V31.2 local server and game. Select **LocalVS** in the
   character menu and wait before clicking Play.
3. In the extracted `ps2_loading_observer` folder, double-click
   **capture_loading_timeline.cmd**.
4. Press **Enter** when prompted, switch to the game and click **Play once**.
5. Leave the game open for **90 seconds**. Upload the
   **local-loading-timeline-....zip** from the folder that opens.
6. Export and upload the usual console report from that same run.

This helper records loading flags and bounded handler checks from the verified
local dummy-session client. It uses your installed Python. It does not change
the game or server. Short transitions can fall between samples.

Package SHA-256: `db349a677b16f305c3970db6997827dd7b21ce22db3167a8a1aca4d1160bd4b1`.

Validation: standalone ZIP/import and ten observation guards passed. The user
successfully captured two Windows timelines with 597 available samples each.
The current full 158-test regression suite passes.

## Loaded code capture

[Download ps2_native_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/main/downloads/ps2_native_capture.zip)

1. Save the ZIP and choose **Extract All** in Windows.
2. Start your existing V31 local server and game; leave the game open at the character menu or INDAR loading screen.
3. In the extracted `ps2_native_capture` folder, double-click **capture_loaded_image.cmd**.
4. Upload the capture ZIP it names. If it creates parts, upload every `.part` file and that folder's `manifest.json`.

The helper uses the existing default game folder:
`C:\Users\Public\Daybreak Game Company\Installed Games\PlanetSide 2 Local`.
It verifies the matching V31 executable and local dummy-session launch, then reads three loaded code sections. It automatically splits large results into 28 MiB upload parts.

Package SHA-256: `a551d187ce8e62a9889a60c79607db1508a9e78317c3b5b9a9bbcdb2b1c1c6c2`.

Validation: ZIP CRC and standalone import checks passed; the user successfully
captured and uploaded the matching loaded image with this helper.
