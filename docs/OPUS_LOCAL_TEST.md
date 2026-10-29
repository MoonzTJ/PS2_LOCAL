# Opus Local Test v6 — camera vs character position

The normal Local Test panel with the confirmed faction character-select bodies,
the 60-second message capture around the placement, and one new read-only tool.

**Why:** decoded messages show your character walks, turns and stays on the
floor after the placement, while the view stays where the teleport put it. The
game also keeps describing your character's ID as a zero-size object at the
teleport point. **Capture Camera.cmd** reads your character's position and the
game's camera object position side by side, six times about 1.5 seconds apart.
If the camera stays fixed while your character moves, the camera is bound to
the frozen copy. It only reads memory; nothing is sent to or changed in the game.

1. Close the game and server. Extract this ZIP into a fresh folder.
2. Double-click **Start Opus Local Test.cmd** and click **Start local session**.
3. Select **LocalVSTest**, press **Play**, then run **Advanced → Existing
   bootstrap diagnostic (once)** and wait for THREE beeps.
4. Double-click **Capture Camera.cmd**. Go back to the game within five seconds.
   At **TWO beeps** hold **W** and turn with the mouse; keep walking until
   **THREE beeps** (about eight seconds).
5. Upload the **local-camera-….json** from `reports` (the window prints the
   result too), plus **Save server report ZIP** and the `local-bootstrap-…` zip.
