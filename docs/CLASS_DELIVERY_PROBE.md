# Local class delivery probe

This isolated experiment checks whether the matching Windows client receives
class records and creates its profile loader. It is **not an Indar loading fix**.
It supplies 24 known class identities, with all other fields left at the current
native constructor's zero/empty defaults. These are incomplete experimental
records, not recovered live-server definitions. Models, classification sets,
resource variants, equipment/inventory and complete class data remain pending.

Profile/faction pairs and name IDs match the captured local tables. Type IDs
come from the pinned public Census reference, which has no source-build marker;
the prior native Engineer/Heavy Assault/MAX predicate checks support those type
identities. This does not supply the remaining current class fields. Historical
model/classification values are not copied into this probe.

1. Extract `ps2_class_delivery_probe.zip` to its own Desktop folder, outside
   the game and existing V31.2 test folders. Close the old local game/server.
2. Double-click **start_class_probe.cmd**. Select the copied **PlanetSide 2 Local
   game folder containing PlanetSide2_x64.exe**, not the old test scripts folder
   or normal online installation. The matching EXE is required.
3. Its separate server, client and familiar test controls open. Select **LocalVS**,
   click **Capture class / loading state**, and hold the selection for five seconds.
4. Double-click **capture_probe_timeline.cmd**. At its prompt, press Enter,
   switch to the game, click **Play once**, and leave it open for **90 seconds**.
5. Click **Capture class / loading state** again, wait five seconds, then
   **Save report ZIP** in the controls. Send the screenshot plus both
   `local-loading-timeline-....zip` and `local-console-report-....zip` from this
   helper's **reports** folder. Send the report even if loading stalls or exits.

The key observations are class count (expected 24 after packet receipt),
loader presence, native profile-completion state and the existing preload flag.
The server's send log alone does not establish native acceptance. Empty model
and item queues in this probe do not demonstrate successful asset loading.

The helper has separate `PS2_Server` and `PS2_Tools` folders; the copied game is
the `PS2_Client` boundary. Nothing is installed over the existing V31.2 test.
The launcher verifies its packaged files, matching executable, server mode,
packet hash and instance before opening the client. Logs/captures remain in this
helper. The existing client launcher may prepare the same local config/UI
overlay it already uses; its source, V30.1 controls and V29.1 preview are preserved.

The opt-in server sends `ProfileDefinitions` once, on startup signal 73, before
`ZoneDoneSendingInitialData`. V31.2 still sends `PreloadDone` only in response
to the later readiness signal 04. Native flags are never patched. Default
server operation sends no probe. Close this probe's game/server and use your
original V31.2 launchers to return to the previous behavior.

Use the installed 64-bit Python 3.10+ as with prior tests. This is a local-only
test: no normal DBG launcher, online session or anti-cheat operation is involved.
Complete class-data recovery continues independently of this diagnostic result.
