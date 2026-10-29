# Opus Local Test v8 — body model 165 + which camera is active

Found in the game's own code: the first-person camera attaches to a bone named
"camera" on your first-person **arms** model, and detaches when there are no arms.
Your character has always had body model 0, so no arms and no camera follow.
This version sends body model **165** (VS Light Assault) in the activation
message the bootstrap diagnostic uses. The game code shows that number leads to
the first-person arms `FPArmor_VS_Male_LightAssault`. Everything else is as in v6:
faction character-select bodies, message capture and **Capture Camera.cmd**.

This is a new path; it could crash. If it does, that is a useful result.

1. Close the game and server. Extract this ZIP into a fresh folder.
2. Double-click **Start Opus Local Test.cmd** and click **Start local session**.
3. Select **LocalVSTest**, press **Play**, then run **Advanced → Existing
   bootstrap diagnostic (once)** and wait for THREE beeps.
4. Look: do you see arms or a weapon-less first-person view? Does the view move
   with W and the mouse? Take a screenshot.
5. Double-click **Capture Camera.cmd**, go back to the game within five seconds,
   at TWO beeps walk and turn until THREE beeps.
6. Upload `reports\local-camera-….json`, the `local-bootstrap-…` zip and the
   **Save server report ZIP**. If the game crashed, also the newest files in the
   copied game folder's **Logs** folder.

**v8 addition:** Capture Camera now also reports which camera type the game is
using (19 = first person; 21/22 = the timed spawn/deploy transition camera) and,
for the transition camera, its timer and the three fields that decide when it
hands over to first person. Same steps as above.
