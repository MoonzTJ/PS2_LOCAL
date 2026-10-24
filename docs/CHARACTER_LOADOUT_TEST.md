# LocalVSTest character loadout test

This test sends the VS Light Assault saved/current loadout references that were
previously empty, then measures their actual native state. It uses LocalVSTest,
profile19/loadout17/tab0 and14 verified item-line slots. Appearance, owned item
instances, equipped GUIDs and player movement remain unfinished. A headless
preview or fixed under-map view can still occur; this is not a spawning fix.

1. Close the old local game and local server windows.
2. Extract this ZIP into a new folder outside the copied game folder.
3. Run **start_character_loadout_test.cmd** and choose the matching copied
   **PlanetSide 2 Local** folder containing PlanetSide2_x64.exe. Keep the server,
   client and controls windows open.
4. Select **LocalVSTest** (the fifth character) and press **Play**.
5. When the world HUD appears, run **capture_character_loadout_test.cmd**.
   If the loading screen stays up, wait30 seconds and capture there instead.
6. Upload the new **local-state-....zip** from this helper's **reports** folder.
   Explorer opens that folder after capture. If possible, include a screenshot
   of the Infantry Loadout page and say whether WASD or mouse look works.

If the game exits before capture, use **Save report ZIP** in the local test
controls and upload that report. Upload a small capture failure ZIP too if one
is created. Do not run the older local-state/LocalVSTest capture for this test.

The new snapshot is format8 and reads the matched local player's saved/current
slots, owned/equipped counts and bounded shared rank-zero item-line lookups.
It retains the16KiB total process-read limit, exact executable/config/identity
checks and local dummy-session guard. No process memory is written or native
function called. Read failures or incomplete lookups are reported explicitly.

Server health binds this test to its exact class, self-packet and starter
fingerprints. Other four characters retain their previous player packets and
the menu preview rollback. Startup/preload confirmation ordering is unchanged.
The location experiment stays disabled. Server state is in PS2_Server/state;
client binaries/assets remain in the copied game folder, tools in PS2_Tools.
