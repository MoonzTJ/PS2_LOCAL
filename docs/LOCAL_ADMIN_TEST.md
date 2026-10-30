# Local character admin access

This server mode grants the current client's **IsAdminAccount** permission at
login to LocalVS, LocalNC, LocalTR, LocalNSO and LocalVSTest. The server controls
the grant using its configured character identities; the client cannot request
it by changing the character name. Normal startup without `--local-admin`
retains the previous non-admin packet bytes.

1. Close the previous local game and server. Extract this ZIP into a fresh
   folder outside the game.
2. Run **start_local_admin_test.cmd**, select your copied Local client, then
   **LocalVSTest → Play** for the measured test.
3. At the 3D world view, close menus/map/chat input. Run
   **activate_and_capture_admin.cmd once**.
4. Return within **eight seconds** and remain idle until **THREE beeps**.
   There is **no W-held phase**. The helper checks the admin permission,
   activates class19 and applies the existing warpgate placement once.
5. Upload **local-admin-access-….zip** from this helper's reports folder,
   including an early failure. Never repeat an uncertain application in the
   same server session.
6. After capture, try **/loc** in chat and report the exact response. This
   alias exists in the shipped command list; successful execution is still
   unverified. Report any camera, audio, menu or command-access change.

The native decoder, named IsAdminAccount getter and an admin-command permission
predicate have been checked against the pinned offline build. The helper's
format14 observer measures the matching player's actual decoded permission;
it does not call or write game code. Its shared16KiB budget and original
identity/signature/local-launch guards remain. Model0 and existing class/
placement bodies are preserved; no inventory application is included.

Admin account permission is separate from an admin client build. Commands that
need server gameplay handlers, spawning, items, territory or a Lua runtime
still need those implementations. The grant does not make every shipped alias
functional. Actual Windows command execution and camera control remain pending.
The map header reading CONTINENT is a separate metadata issue; no unverified
display-name ID is substituted. The report records the current name ID.
