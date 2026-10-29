# Opus Local Test v3 — faction bodies + post-placement PreloadDone

The normal Local Test panel with two opt-in changes. Class definitions keep
their original bytes (class body words crashed login in an earlier test).

**1. Character-select bodies (confirmed working).** LocalVS/LocalVSTest show the
VS Light Assault base body (165), LocalNC the NC one (159), LocalTR the TR one
(143). LocalNSO is unchanged.

**2. One PreloadDone after placement (new).** In five of five Windows runs the
bootstrap placement turned the client's "PreloadDone received" flag false, and
movement and mouse-look stayed locked. After that placement the server now
sends the same PreloadDone message once more: immediately if the client sends
its ready signal again, otherwise on its own after five seconds. The report
records which path happened. This tests the input lock only; a real spawn
sequence, body model and camera height are separate work.

1. Close the game and server. Extract this ZIP into a fresh folder.
2. Double-click **Start Opus Local Test.cmd** and click **Start local session**.
3. Select **LocalVSTest** and press **Play**.
4. At the 3D view, run **Advanced → Existing bootstrap diagnostic (once)** and
   follow its prompt until THREE beeps.
5. Wait about ten more seconds, then try **mouse-look** and hold **W**. Take a
   screenshot if anything changes.
6. Click **Save server report ZIP** and upload it together with the
   `local-bootstrap-…` zip. Report whether you could look around or move.
