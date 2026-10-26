# Character initialization diagnostic

This test checks the selected actor's own class ID, class model references,
model-definition lookup and first-person-arms mapping. It is a read-only
diagnostic; it is not an appearance or movement fix. The server and its initial
packets are identical to the previous cosmetic test. No incremental items are
sent by this procedure.

1. Close the old local game and local server windows.
2. Extract `ps2_character_initialization_test.zip` into a fresh folder outside
   the game folder. Run `start_character_initialization_test.cmd` and select
   your separate Local client folder.
3. Select **LocalVSTest** and press **Play**. Wait until the 3D world view appears,
   even if it still shows the underside of the map. Close the map, other menus
   and chat input.
4. Run `capture_character_initialization.cmd`. Switch back to the game within
   eight seconds and remain idle until **three beeps**. Leave both game and server
   open. No W-held phase, inventory apply command or location command is needed.
5. Upload the `local-character-initialization-….zip` from this helper's `reports`
   folder, including if capture stopped. State whether the view changed.

Use the capture CMD from this newly extracted helper, so the report contains
format12 and the actor-initialization fields. No REA installation is required
on your PC. Existing Python/launcher requirements are the same as before.

Zero class/model IDs, absent records and incomplete bounded searches are saved
as observations. The actor's class is distinct from the saved/current loadout's
profile ID. Metadata/mapping presence does not establish that resources loaded,
the character rendered or movement became available.
