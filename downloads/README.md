## Next test: complete shared data pack

[Download ps2_shared_data_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_shared_data_capture.zip)

Extract to your Desktop, outside the game folder. Double-click
**collect_shared_data.cmd**, choose the copied **PlanetSide 2 Local** game folder
containing `PlanetSide2_x64.exe` and `Resources`, then upload the
**local-shared-data-….zip inside the results folder** that opens. Upload the
small failure ZIP too if verification stops. No running game/server, launcher,
administrator startup or manual Python commands are needed. Keep V31.2.

This captures the complete 10 MB shared asset pack with 2,647 resources,
including entries with unrecovered filenames. It verifies every resource,
anchors, exact matching pins, stable bytes and output hashes. The prior asset
test is complete; no repeat of that test or the online capture is requested.
Complete definitions and Indar entry remain unfinished.

All **275 Python tests pass**; setup and backend status/report export verify.
Prior native outcomes are retained history. All ten earlier helper ZIPs are
unchanged. SHA-256:
`5e821048f2b4273ad6191079b325b48058ac2099fc67c976d9b2aadd936fe110`.
See [the procedure](../docs/SHARED_DATA_CAPTURE.md).

## Earlier test: local weapon and ability resources

[Download ps2_weapon_resource_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_weapon_resource_capture.zip)

Extract the ZIP into a fresh folder, then double-click
**collect_weapon_resources.cmd**. Choose the copied **PlanetSide 2 Local** game
folder containing `PlanetSide2_x64.exe` and `Resources/Assets`. Upload every file
inside the folder that opens: the capture ZIP, or all parts plus `manifest.json`.
Upload the small failure ZIP if the exact build/asset checks stop the capture.

No game, server, DBG launcher, administrator startup or manual Python commands
are needed. This reads only disk assets. It looks for 25 weapon/ability/skill table
candidates and preserves all matching pack indexes for further offline searches.
The executable, two anchors and all 132 saved index pins must match. Candidates
are not verified server-field mappings; Indar entry remains unfinished.
Keep V31.2 installed. All nine earlier helper ZIPs remain unchanged.

260 Python tests and 38 new native checks pass. Package CRC, source manifests,
standalone imports and backend status/export verify. Windows capture remains to
test. SHA-256: `31c6627613c149edccffe80e2578709b0fec58201ecb118367de83e54218c6d2`. See [the procedure](../docs/WEAPON_RESOURCE_CAPTURE.md).

## Next test: online code collector revision 2

[Download ps2_online_code_capture_v2.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_online_code_capture_v2.zip)

Extract into a fresh folder. Close the local game, launch the normal online game
through DBG and leave it at the character menu. Double-click
**capture_online_image_v2.cmd**. Upload every file inside the folder that opens,
including the small failure ZIP if access is denied. No spawning, administrator
launch or manual Python commands are needed.

This separate collector requests narrower read access and reports the exact
failing API. It keeps the verified online build/section guards and stops on
denial. A successful capture supplies code/constants for offline class-layout
comparison; it exports no class values and does not change local Indar loading.
Keep V31.2 installed. The eight earlier helper ZIPs remain unchanged.

SHA-256: `77bed3eb596a26974641a329db4410317bc5e2b8e4ec27579992829b007955b3`. All 246 Python tests pass; package CRC, source manifest
and standalone imports verify. Real Windows capture remains for this test.
See [the procedure](../docs/ONLINE_CODE_CAPTURE_V2.md).

# Local PS2 test helpers

## Next capture: online loaded code and constants

[Download ps2_online_code_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_online_code_capture.zip)

Sign into GitHub and choose **Download raw file**, then extract into a fresh
folder. Close the local game; leave the normal online game at its character
menu. Double-click **capture_online_image.cmd**. Upload **every file inside the
folder that opens separately**: one ZIP, or all `.part` files plus `manifest.json`.
Send its failure report too. No spawning or folder-zipping is needed.

All three supplied disk-copy parts and the executable verify. The online version
is 0.0.1534.609692; the local reference is 0.0.1533.609523. Section positions
changed, and code/class constants are protected on disk. This collector is
locked to the supplied online hash and captures only loaded code and read-only
constants for comparison. It exports no class values or writable/heap regions.
All 232 Python tests pass. Actual Windows code capture and class compatibility
still need verification. Keep V31.2 installed. See `docs/ONLINE_CODE_CAPTURE.md`.

Package SHA-256:
`dc559123ca9f9f9d2b01633238c98a4daedea60ef01bd31b8040dac4ab98697e`.

## Earlier capture: compare the online executable

[Download ps2_client_build_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_client_build_capture.zip)

Sign into GitHub and choose **Download raw file**. Extract into a fresh folder.
Close the local game, open the normal online game at its character menu, and
double-click **capture_client_build.cmd**. Upload **all files in the folder that
opens**: either one ZIP or every `.part` file plus `manifest.json`. Large files
are split automatically; no manual Python commands or spawning are needed.

The revision 2 class report confirms executable detection, but the online file's
SHA-256 differs from the local reference. This separate helper copies only the
on-disk executable and metadata for offline comparison. It reads no process
memory and does not authorize an unverified layout for class capture.
The original matching executable passes the disk-copy, CRC/checksum and three-part
reconstruction check. All 224 Python tests pass. Actual online capture remains
pending. Keep V31.2 installed. See `docs/CLIENT_BUILD_CAPTURE.md`.

Package SHA-256:
`363ce00d0f12510fcce2ec5b3e6ff268a145bfabc72b378e843a85b3a2cd6592`.

## Earlier capture: complete static class definitions

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
