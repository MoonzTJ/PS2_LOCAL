# Capture the active actor and graphics fallback location

The last capture succeeded and the screenshot shows 3D map geometry. This test
checks whether the client has an active world actor and reads the graphics
location used when GetPlayerLoc has no actor. It is a diagnostic, not a spawn fix.

1. Keep the local class-probe server and game open. Use the existing **LocalVS**
   preset and reach the HUD/world view. If closed, use **start_class_probe.cmd**
   from the original class-probe folder and leave its server console open.
2. Extract **ps2_local_scene_capture.zip** to a new Desktop folder outside the
   game and previous helper folders.
3. Double-click **capture_local_scene.cmd**. Select the copied Local game folder
   containing **PlanetSide2_x64.exe**. Keep the view still while it captures.
4. Upload the new **local-state-....zip** from this helper's **reports** folder,
   plus a screenshot of the world view. A small failure report is useful too.

This helper does not start the server. If it stops at server_health, check that
the separate class-probe server window says **READY: local login + gateway
listeners**. If startup fails, retain the start launcher's error instead of
repeatedly running the capture. New-character creation/saving is not implemented;
use the existing preset. No repeat timeline or new game/server patch is needed.

The matching EXE, local config, dummy launch, server mode/packet/instance and
native reader signatures remain verified. The new fields are anchored to the
known synthetic local player. The actor lookup reads pointer presence only;
it does not inspect an unverified actor layout or call native functions.
The graphics singleton must match the engine pointer, and its position/root
bytes are checked again. Moving/racing or unavailable fields remain unavailable.
Actor and graphics observations are sequential, not an atomic scene snapshot.

The existing **16 KiB shared read budget** is unchanged. Reports contain no raw
addresses, install paths, command lines, arbitrary memory or online account data.
Nothing is written to the game, its process or server; no elevation, suspension,
injection or access-protection change is requested. The observed graphics
fallback is not proof of a valid spawn, camera-to-actor association, collision
or complete terrain streaming. Existing 13 helper ZIPs remain unchanged.
