# Faction Light Assault bodies v3 — character select only

v2 confirmed the character-select body is safe: LocalVSTest showed the VS Light
Assault body and head, and every character logged in without crashing. v3 uses
the same menu-row change for each faction's male Light Assault base body:

| Character | Body model | File |
| --- | --- | --- |
| LocalVS, LocalVSTest | 165 | Armor_VS_Male_LightAssault_Base |
| LocalNC | 159 | Armor_NC_Male_LightAssault_Base |
| LocalTR | 143 | Armor_TR_Male_LightAssault_Base |
| LocalNSO | unchanged | no row in this family yet |

All use head 1 and male gender. Your asset check found every file and its
`_UI` preview variant. Class definitions keep their original bytes (class body
words crashed login in v1). The starter armor, helmet and weapon are not drawn
yet because the character-select loadout is still empty.

1. Close the game and server. Extract this ZIP into a fresh folder.
2. Double-click **Start VS Appearance Test.cmd** and click **Start local session**.
3. Click each character and take a screenshot of LocalVS, LocalNC and LocalTR.
4. Play with any character to confirm login still works, then click **Save
   server report ZIP** and upload it with the screenshots.
