# One-shot local actor placement test

Use the same copied V31.2 local client (0.0.1533.609523), 64-bit Python 3.10+
installed, and LocalVS. Keep the online game closed. Extract this new ZIP into
its own folder outside either game installation; do not merge it into an older
helper or game folder. This bundle supplies a separate PS2_Server and PS2_Tools.

1. Close the old local game and its server/client/control consoles.
2. Run **start_location_probe.cmd**. Choose the copied **PlanetSide 2 Local**
   folder containing PlanetSide2_x64.exe. Keep the new consoles open.
3. Select **LocalVS**, press PLAY, return to the game view and wait for the HUD
   and world geometry to appear. The initial under-map view is expected.
4. Run **apply_location_once.cmd** once. It captures the original actor state,
   checks the local session and running state, sends one location message, waits
   five seconds and captures again. The native client may briefly show loading.
5. Upload the **local-location-....zip** that the helper shows, plus a screenshot
   of the resulting game view. Upload the ZIP even if the helper reports failure.
   Keep both the game and server open until capture finishes. Do not resend in
   the same server run. A timeout can mean the packet was already queued.

The target is the exact inferred position of Common_Props_SpawnTube.adr,
Indar.zone group27/instance84: (2946.908203125, 65.78419494628906,
126.0810546875, 1). Facing is an explicit horizontal test vector (0,0,1,0).
This is an asset anchor near SpawnRoom2 instance1, not a verified player-feet
position, faction assignment or safe spawn. No height offset is invented.
This test establishes whether the actor moves and whether the view follows.
Ground/collision, movement, territory ownership, full classes and equipment
remain separate work; weapons and menus may still be empty.

V31.2 initial packet order/timing, V30.1 controls and V29.1 preview stay fixed.
The location message is off by default and is sent only by the manual test
command after local identity/initialization checks. The before capture requires
Running/LocalVS/Indar and the matching clean actor at the origin. The server
requires the same LocalVS gateway session, completed initialization, an empty
reliable queue and no prior attempt. Exactly one application message is allowed;
normal reliable transport retries remain enabled. No native readiness flags are
forced. The client executable is unchanged; the Windows inspector only reads.
The normal local launcher still creates/restores its existing local config.

Offline validation runs the matching native strict parser, ClientUpdate handler,
actor setter and transform math with object/service fixtures. Camera output and
unrelated notifications are isolated; actual rendering needs this Windows test.
Reports contain bounded before/after snapshots and packet/instance fingerprints.
No normal online process capture or administrator startup is required.
