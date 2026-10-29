# VS Light Assault appearance v2 — menu body only

v1 gave LocalVSTest a correct VS Light Assault body and head at character
select. It also put models 165/166 into the class-19 definition, and with that
change logging into **any** character crashed the game right after the class
data arrived. v2 keeps only the menu change: LocalVSTest's character-select row
gets head 1, model 165, gender 1. Class definitions are back to their original
bytes, so the normal Local Test panel checks all run unchanged. LocalVS, LocalNC,
LocalTR and LocalNSO are unchanged.

1. Close the game and server. Extract this ZIP into a fresh folder.
2. Double-click **Start VS Appearance Test.cmd**, choose the game folder and
   click **Start local session**.
3. Confirm LocalVSTest still shows the VS body at character select.
4. Select **LocalVSTest** and press **Play**. Note: crash, loading screen, or
   3D world. If it reaches the world, try W and mouse-look.
5. Click **Save server report ZIP** and upload it. If the game crashed, also send
   the newest files from the copied game folder's **Logs** folder.

Expected: login works again for every character, and LocalVSTest keeps its VS
preview. A crash with this version would point at the menu row itself.
