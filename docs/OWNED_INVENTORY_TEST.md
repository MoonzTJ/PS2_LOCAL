# LocalVSTest owned inventory test

This test adds two verified generic starter equipment instances and their slot
GUID references. It checks actual receipt and linkage. Character appearance,
weapons, abilities, movement and spawning remain incomplete.

1. Close the previous local game and server windows.
2. Extract the complete `ps2_owned_inventory_test.zip` into a new folder outside
   the copied game. Keep its PS2_Server and PS2_Tools folders together.
3. Run `start_owned_inventory_test.cmd`, choose the copied **Local** game,
   select **LocalVSTest**, and press Play. Keep the server and launcher open.
4. When the world HUD appears, run `apply_and_capture_owned_inventory.cmd` once.
   It captures the initial state, checks the two native item definitions and
   equipment references, sends the two-item packet, then captures again.
5. Upload the **local-owned-inventory-….zip** shown at the end. Upload it even if
   the check stops or the game closes. Its before/after ZIPs are already inside.

If loading remains stuck, run the same CMD after 30 seconds. It will save a
failure report without sending when the prerequisites do not pass. If it says
to wait for reliable replies, allow a few seconds and rerun only if the report
says no delivery was attempted. Once delivery was attempted, start a fresh run
before testing again.

A screenshot of the infantry/loadout menu after the test is useful. Report any
visible change or crash. A populated inventory count alone is not a spawned or
controllable character. No location command is part of this test.

The helper uses the exact pinned copied executable and local dummy-session
guards. It makes bounded typed process reads, sends normal localhost protocol
packets and persists test-only state in PS2_Server/state. The two GUIDs are
server-assigned test identities, separate from item-definition IDs. Raw unknown
instance fields retain native constructor defaults. Equipment row zero is an
experimental row matching the observed raw selector, with no claimed class-ID
mapping. No online game capture or administrator launch is required.
