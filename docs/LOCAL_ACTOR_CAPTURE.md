# Capture LocalVS actor identity and position

This test reads the selected world actor's identity and clean position cache.
It helps distinguish an actor placement problem from the separate zero-filled
player record and graphics fallback. It does not change spawning or equipment.

1. Keep the local class-probe server and game open. Select the existing **LocalVS**
   and reach the HUD/world view. If closed, use the original **start_class_probe.cmd**
   and leave its separate READY server console open.
2. Extract **ps2_local_actor_capture.zip** into a new Desktop folder.
3. Run **capture_local_actor.cmd** inside **ps2_local_actor_capture**. Choose the
   copied Local game folder containing **PlanetSide2_x64.exe**. Keep the view still.
4. Upload the **local-state-....zip** from this new helper's **reports** folder
   and a screenshot of the world view. Upload the small report even if it fails.

The helper does not start the server. No repeat timeline is needed. It uses the
same local build, config, dummy launch and matching class-probe server checks.
New-character creation/saving remains unfinished; use the existing preset.

Native format7 adds **scene_location.actor_transform**. Four verified actor
interface pairs are supported. The actor GUID, manager's cached identity, local
player roots, interface slots, transform pointer, dirty flag and position bytes
are checked with bounded sequential rereads. A different actor type, identity
mismatch, dirty/changing cache or unreadable field produces an unavailable
measurement. No native function is called to refresh a dirty cache. Rereads
reduce races; they are not an atomic snapshot and cannot exclude an ABA change.

The existing shared **16 KiB** read limit remains. No raw memory addresses,
install paths, arbitrary memory or online account data are exported. Nothing
is written to the game, process or server. No elevation, suspension or access
changes are requested. The graphics fallback remains a separate observation;
neither position establishes rendered-camera attachment, safe ground, collision,
complete streaming, owned inventory or a working equipped loadout.
