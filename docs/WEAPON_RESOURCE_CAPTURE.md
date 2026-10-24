# Next test: local weapon and ability resources

This disk-only test looks for additional weapon/projectile/ability tables in
your matching copied local game and preserves its PACK2 indexes for later
offline searches. The earlier profile capture requested 32 tables and omitted
many of these candidates; it saved index statistics/hashes but not index entries.
The online module snapshot was denied. This helper uses no process access.

1. Extract the helper ZIP into a fresh folder.
2. Double-click **collect_weapon_resources.cmd**.
3. In the folder picker, choose your **PlanetSide 2 Local** game folder containing
   `PlanetSide2_x64.exe` and `Resources/Assets`. Use the same copied local build
   used for V31.2, not the normal online install.
4. Wait for the upload folder to open. Upload **every file inside it** separately:
   either the capture ZIP, or all `.part` files plus `manifest.json`. Send the
   small failure ZIP if the build/asset checks stop the collector.

No game, server or DBG launcher needs to be running. No administrator launch or
manual Python command is needed. Your existing Python installation is used;
large reports split into 28 MiB parts automatically.

The exact local executable SHA-256 and the ClientItemDefinitions/Models anchor
hashes and all 132 pack names/index hashes must match the earlier verified local
capture. This binds new tables to that same saved asset set. The helper reads PACK2
index records and up to 25 specifically named tables. It checks stored-resource
CRC32C, bounded decompression, duplicate versions, repeated reads and final pack
indexes/executable identity. It exports raw index records, selected table bytes,
file hashes and capture metadata. Anchors are verified but not copied wholesale.
Index filenames contain only pack basenames; installation paths are omitted.

It copies no EXE, full pack payloads, account/character data or configuration.
It makes no network requests, game-process access, game launches, writes to the game/backend or
protection changes. On failure, partial resource/index data is omitted.
The original nine helper ZIPs and V31.2 stay unchanged.

The candidates come from literal filenames in the matching captured client;
they are not verified mappings to server class/weapon fields. Missing candidates
are recorded, and preserving full indexes lets us check further names offline
without repeating the whole export. The output may supply useful data or establish
that a candidate is absent. It does not itself fix Indar loading or establish
complete class/weapon definitions, successful assets or gameplay.
