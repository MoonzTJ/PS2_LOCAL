# LocalVSTest actor loading test

This new test checks the selected character's model ID, actor-definition asset
name, first-person arms callback flags and native movement diagnostics. It
reproduces the previously successful two-item baseline using the unchanged v2
server. It does not implement character movement or promise a correct preview.

1. Close the old **local** game and server. Extract this ZIP into a **new folder
   outside the copied game**. Keep its PS2_Server and PS2_Tools folders together.
2. Run **start_actor_loading_test.cmd** and choose the matching copied Local
   client. Keep the server console open. Select **LocalVSTest**, then Play.
3. Once the world HUD appears, close the map/menus/chat. Run
   **capture_actor_loading_test.cmd** once. Switch straight back to the game;
   the helper allows eight seconds. Keep the game focused and do not press
   movement keys yet.
4. After **TWO beeps**, hold **W**. Release W after **THREE beeps**. Do not switch
   windows during this interval. If you cannot hear the beeps, keep the report
   and tell us; do not guess the timings.
5. Upload the **local-actor-loading-....zip** from the reports folder, including
   a failure report. Say whether the view moved and whether any soldier/weapon
   appeared. The archive contains before, idle and W-held captures when each
   phase succeeds. If the game closes, also use **Save report ZIP** in controls.

Use the copied Local client and ordinary 64-bit Python. No REA installation on
your PC is needed. Do not run this against the online game. The helper checks
the exact executable, local config, dummy session, server identity and native
interfaces. It reads bounded fields; it does not invoke native getters, change
readiness flags, inject input or send a location command. Each phase has one
shared 16 KiB native-read budget. It samples only whether this game's process is
in the foreground and W is held immediately before and after the capture; this
does not establish that the game consumed input.

Arms-ready/failed bits describe **first-person arms**, not the whole character.
A nonzero model/name is not proof of successful rendering. Fields changing
during a capture are marked unavailable. This test gives us new evidence to
target the missing activation step. Restart the helper's server for any repeat;
the known item-delivery baseline has a single-attempt guard.
