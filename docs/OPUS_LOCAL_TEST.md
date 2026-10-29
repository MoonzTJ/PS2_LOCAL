# Opus Local Test v5 — faction bodies + 60-second placement message capture

The normal Local Test panel with two opt-in changes; class definitions keep
their original bytes.

**1. Character-select bodies (confirmed).** VS rows 165, NC 159, TR 143; NSO
unchanged.

**2. Full-byte capture around the placement (new).** In the last run's log the
client's steady 15-per-second `2:02` messages stopped right after placement,
and new `2:fe` and `0:02` messages appeared. Logs normally keep only 4 bytes per
message. This version records the full bytes (up to 256 each) of the client's
zone messages from the class activation until 60 seconds after placement, plus
the first 30 `2:02` messages at the origin and the first copy of every rare
message type. Nothing new is sent to the game. (The earlier extra PreloadDone
was accepted but did not unlock input, so it is off here.)

1. Close the game and server. Extract this ZIP into a fresh folder.
2. Double-click **Start Opus Local Test.cmd** and click **Start local session**.
3. Select **LocalVSTest**, press **Play** and wait for the 3D view.
4. Hold **W** for two seconds and move the mouse (this fills the origin sample).
5. Run **Advanced → Existing bootstrap diagnostic (once)** and follow its prompt
   until THREE beeps. **Right after the beeps** (within 30 seconds): hold W for
   three seconds, release, then move the mouse left and right for three seconds,
   then hold W again while turning. The capture stops 60 s after placement.
6. Click **Save server report ZIP** and upload it with the `local-bootstrap-…` zip.
