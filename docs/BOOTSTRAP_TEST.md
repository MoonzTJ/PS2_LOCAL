# Continent and character bootstrap test

This diagnostic measures continent metadata and redeploy prerequisites alongside
the selected character. It uses the exact previously tested server and existing
class19 activation/warpgate placement. No new gameplay message or state override.

1. Close the old local game and server. Extract the new ZIP into a fresh folder
   outside the game.
2. Run **start_bootstrap_test.cmd**, select the copied Local client, then
   **LocalVSTest → Play**.
3. Once the 3D world view appears, close the map, menus and chat input. Run
   **capture_bootstrap_test.cmd once**.
4. Return to the game within **eight seconds** and stay idle until **THREE beeps**.
   There is no W-held phase and no command to type.
5. Upload **local-bootstrap-….zip** from the reports folder, even if the test
   stops early. Do not retry an uncertain activation/placement in that session.

The report contains before, activated-origin and placed snapshots. The world map
does not need to be open. A fixed view or missing weapon is still a useful result.

The new snapshot shares one16KiB read budget. It checks the bounded continent
list and zone2 name/description IDs, the redeploy settings-service and state-context
presence, the matched actor's raw9F7 byte/observer bit, and existing class/model/
arms observations. Unsupported types, races and incomplete searches remain
explicit unknown/unavailable results. No native functions or process writes.

The settings lookup value and two native context predicates are not evaluated.
Known missing prerequisites can rule out the recovered CanRedeploy predicate;
otherwise eligibility remains unknown. This does not measure every condition
of the actual UI button, prove a game-mode choice or repair spawning/appearance.
The admin flag is retained as baseline data; this is not another admin-grant test.
