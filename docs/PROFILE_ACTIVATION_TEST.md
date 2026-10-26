# Actor profile activation test

This test sends one current-build active-profile message for VS Light Assault 19.
It tests whether the actor changes from class 0 to 19, separately from the saved
loadout references already present. The packet preserves the observed model 0.
Correct VS appearance, weapons, grounding and movement are still unverified.
The previous character preview and all existing test archives are preserved.

1. Close the old local game and local server windows. Extract this ZIP into a
   fresh folder outside the game folder.
2. Run `start_profile_activation_test.cmd` and select your separate Local client.
   Select **LocalVSTest**, press **Play**, and wait for the 3D world view.
3. Close map/menus/chat input. Run `apply_and_capture_profile.cmd` **once**.
4. Switch back to the game within **eight seconds** and remain idle.
5. At **two beeps**, hold **W** until **three beeps**, then release it.
6. Upload `local-profile-activation-….zip` from this helper's `reports` folder,
   including if the test stopped. State whether the view moved or appearance changed.

If three beeps happen before the two-beep signal, the test stopped early; upload
its report. A failed preflight sends nothing. An uncertain send is never retried
in that server run. The old ownership and location apply commands are not part
of this test. No PC REA installation is needed.

The helper requires the observed actor class 0/model 0, absent active-profile 19
record, saved/current starter refs and first-row equipment lookup mode before
sending. It captures before, after-idle and W-held with read-only format13 under
one 16 KiB budget per snapshot. It never calls a game function or writes process
memory. Native profile decoding and class assignment occur through the normal
server packet. The after report distinguishes class activation from complete
character rendering or playable movement.
