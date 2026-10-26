# LocalVSTest VS armor and helmet test

This test delivers the VS Light Assault starter armor and helmet as owned
cosmetic instances, with their slot/GUID links. It checks whether they register
and whether appearance or actor resources change. Complete character rendering,
weapon setup and movement remain unfinished.

1. Close the old **local** game and server. Extract this ZIP into a **new folder
   outside the copied game**. Keep PS2_Server and PS2_Tools together.
2. Run **start_cosmetic_inventory_test.cmd**, choose the matching copied Local
   client, select **LocalVSTest**, then Play. Keep the server console open.
3. Once the 3D world/HUD appears, close the map, menus and chat input. The view
   can still be under the terrain. Run **apply_and_capture_cosmetics.cmd** once.
4. Switch straight back to the game and remain idle until **THREE beeps**.
   Allow about15 seconds. No movement key is required.
5. Upload **local-cosmetic-inventory-....zip** from reports, including a failure
   report. Tell us whether armor, a helmet, arms or another visual change appeared.
   Include a screenshot if anything changes. If the game closes, also use
   **Save report ZIP** in the server controls.

Use ordinary64-bit Python and the copied Local client. No PC REA installation
is needed. Before sending, the helper verifies the exact local EXE/dummy launch,
server instance/mode, LocalVSTest/Indar/Running state, starter records, matching
actor, empty owned inventory, exact slot10/11 links and both native metadata
records with type39 and matching model-name templates. A failed guard sends
nothing. A timeout never retries delivery; restart the helper's server for a
fresh attempt. Upload its report instead of changing the guard.

Each phase shares one16KiB read budget for state, ownership and actor fields.
Only the client’s existing network decoder creates the instances. The helper
does not patch the EXE, write readiness flags, inject input or send a location
command. Appearance/model IDs and the old preview rollback are preserved.
This isolated probe replaces the earlier two generic test items with the two
catalog-backed cosmetic items; it is not the complete starter inventory.
