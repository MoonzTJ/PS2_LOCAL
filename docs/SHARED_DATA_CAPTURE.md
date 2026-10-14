# Next test: complete shared data pack

Download:
https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_shared_data_capture.zip

1. Extract the new ZIP to your Desktop, outside the game folder.
2. Double-click `collect_shared_data.cmd` inside the extracted folder.
3. Choose **PlanetSide 2 Local**, the copied game folder that contains
   `PlanetSide2_x64.exe` and `Resources`. Select the game folder, not its Assets
   subfolder or the normal online installation.
4. Wait for verification to finish. A results folder opens automatically.
5. Upload the **`local-shared-data-….zip` inside that results folder** here.
   Upload the small report ZIP even if the capture stops.

No game or backend needs to be running. Keep V31.2 installed; this helper does
not replace it. Python is already installed on your machine, so no manual
Python commands are needed.

The new capture exports the entire fixed `Resources/Assets/data_x64_0.pack2`:
9,976,544 bytes, 2,647 indexed resources. The prior exports selected filenames;
the complete pack also supplies resources whose names we have not recovered.
Its verified index contains ArmorInfo, ResistInfo, Resources, ResourceTypes and
other tables useful for cross-checking public Census records and native fields.
The expected input pins come from your previous matching pack-index upload.

The helper checks the local executable, exact pack size/index, both table
anchors, every resource's stored CRC and compressed-stream bounds, then checks
that the pack and executable stayed unchanged. Empty uncompressed entries are
also verified. It hashes the complete pack and its decoded resources, verifies
the ZIP and upload copy, and rejects excessive sizes. The output is below the
upload limit and needs no splitting. Failures export only a report/manifest.

It reads the shipped static pack and hashes the executable from disk. It does
not read a running process, contact services, launch the game, collect account
files or modify the game/backend. Only helper reports are written, outside
the game folder. This is a data test; complete class/weapon definitions and
successful Indar entry still need verification.
