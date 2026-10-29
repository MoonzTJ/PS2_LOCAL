# VS Light Assault appearance — Local Test panel

This is the normal Local Test panel, unchanged except for one opt-in change: the
table-inferred VS Light Assault body. Class 19 gets model 165 (male) and 166
(female). LocalVSTest's character-select row gets head 1, model 165, gender 1.
LocalVS, LocalNC, LocalTR and LocalNSO are unchanged, so they show the old
headless body for comparison. These are not recovered live server values, and
the actor's own model word is still 0, so first-person arms are not expected.

1. Close the previous local game and server. Extract this ZIP into a fresh
   folder outside the game.
2. Double-click **Start VS Appearance Test.cmd**, choose the copied game folder
   and click **Start local session**.
3. At character select, click **LocalVSTest**, wait ten seconds and take a
   **screenshot**. Click one unchanged character (e.g. LocalVS) and take a second.
4. With LocalVSTest selected, click **Capture class / loading state** once.
5. Press **Play**. At the 3D view, use **Advanced → Existing bootstrap diagnostic
   (once)** and follow its prompt (return within eight seconds, stay idle until
   THREE beeps). Then hold **W** for a few seconds and try mouse-look.
6. Click **Save server report ZIP** and upload it with the screenshots and any
   `local-bootstrap-…` report from `reports`.

Report what LocalVSTest's preview shows (VS body, headless body, or a spinner),
and whether the in-world view moved. A spinner is a useful result too.
