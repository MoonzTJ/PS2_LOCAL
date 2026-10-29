# VS Light Assault appearance test

This repeats the proven class-19 placement and motion test with one opt-in
change: the table-inferred VS Light Assault body models. Class 19 receives
model 165 (male) and 166 (female), and LocalVSTest's character-select row
receives head 1, model 165 and gender 1. The other four characters are
unchanged. Your asset check confirmed both bodies and their `_UI` preview files
exist; V29's old Body_*_Standard models do not. These are not recovered live
server values, and the actor's own model word is still 0.

1. Close the previous local game and server. Extract this ZIP into a fresh
   folder outside the game.
2. Run **start_vs_appearance_test.cmd** and choose your copied Local client.
3. At character select, select **LocalVSTest**, wait ten seconds and take a
   **screenshot** of the preview. Note whether it shows a VS body, the old
   headless body, or a loading spinner. Then press **Play**.
4. At the 3D world view, close map/menus/chat input. Run
   **appearance_place_and_capture.cmd once**.
5. Return to the game within **eight seconds** and stay idle. At **TWO beeps**
   hold **W**. At **ONE beep** release W. **THREE beeps** means finished.
6. Upload **local-profile-placement-….zip** from `reports`, including if the test
   stopped. It now also contains this run's header-only gateway log.
7. Report the character-select result, whether the view moved with W, mouse-look
   after capture, and anything visible about your own body or arms.

If the preview spins forever, that is a useful result: upload the report and
screenshot anyway. Do not rerun the command in the same server session.
