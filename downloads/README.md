# Local PS2 test helpers

## Next test: loading timeline

[Download ps2_loading_observer.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/main/downloads/ps2_loading_observer.zip)

1. Save the ZIP and choose **Extract All** in Windows.
2. Start your existing V31 local server and game. Select **LocalVS** in the
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

Validation: standalone ZIP/import checks, ten observation guard cases, all
149 Python regression checks, eight native profile cases and 75 native world
cases passed in the cloud workspace. The new timeline helper still needs its
first Windows run.

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
