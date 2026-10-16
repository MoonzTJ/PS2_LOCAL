# Separate LocalVSTest character

This bundle adds **LocalVSTest** as a fifth menu entry, preserving LocalVS,
LocalNC, LocalTR and LocalNSO. It has its own synthetic local character identity,
0x1000000000000101 (1152921504606847233), faction VS. It is a server-provisioned
test preset; the game's Create Character UI and progress saving remain pending.

Extract into a new folder outside the game and previous helpers. Close the old
local game/server before running **start_vs_test_character.cmd**. Choose the
same copied PlanetSide 2 Local installation (0.0.1533.609523); use 64-bit Python
3.10+. The server and tools remain separate from the copied client files.
The new character is listed after the four existing characters (5/14 slots).

Use LocalVSTest for future character-initialization tests. No immediate repeat
location test is needed. If asked for a state report, select LocalVSTest, keep
the game/server consoles open and run **capture_vs_test_character.cmd**. Upload
the local-state-....zip from reports plus a screenshot. This capture explicitly
recognizes the fifth identity, retaining the same16KiB read limit and original
local-build/config/dummy-session guards. It does not modify the game process.

A shared server-side initializer creates separate starter records for LocalVS
and LocalVSTest in **PS2_Server/state**. Each uses the verified VS Light Assault
profile19/loadout17/tab0 mapping,14 resolved item-definition references and14
equipped backend slot links. Item-line IDs remain separate from item-definition
IDs. Backend item keys are isolated per character; they are not native item GUIDs.
Appearance, quantities, ammunition and native instance IDs stay explicitly
unresolved. Existing incompatible seed files are preserved and cause a clear
startup failure rather than being replaced. These files are immutable starter
seeds in this version, not gameplay progression saves.

The new identity can log in and receive the existing synthetic self record.
**Native inventory/loadout serialization, complete class definitions, correct
appearance, character activation and movement remain unfinished.** The preview
can still look wrong and the world view may remain at the origin. This bundle
creates a separate testing identity and backend starter state; it is not the
character appearance/equipment fix. The successful earlier location experiment
remains in its own frozen bundle and only targets original LocalVS.

Normal backend startup still exposes the original four characters. The new mode
requires --class-delivery-probe --vs-test-character --character-state-dir PATH,
and is deliberately separate from --location-probe. Health/reports identify the
mode, new character, starter/list/self fingerprints and remaining limitations.
No unknown numeric appearance/classification IDs or native readiness flags are
assigned. V31.2 timing, V30.1 controls and V29.1 preview behavior are preserved.
