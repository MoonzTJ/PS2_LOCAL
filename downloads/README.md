## Current test: continent and character bootstrap

[Download ps2_bootstrap_test.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_bootstrap_test.zip)

Click **Download raw file**. Close the old local game/server, extract into a fresh
folder outside the game, then run **start_bootstrap_test.cmd**. Choose the copied
Local client and **LocalVSTest → Play**. At the3D view close map/menus/chat and
run **capture_bootstrap_test.cmd once**. Return within **eight seconds** and stay
idle until **THREE beeps**. **No W phase or commands.** Upload
**local-bootstrap-….zip**, including failures. [Procedure](../docs/BOOTSTRAP_TEST.md).

This measures continent metadata and redeploy prerequisites alongside character
initialization, using the exact previously tested server and setup packets. It
does not repair spawning or appearance. Unknown native checks stay unknown.
All405 Python tests and7 native lookup cases pass; two backend starts preserve
three saved-state files. All94 existing server/tools and26 earlier ZIPs remain
byte-identical. Actual Windows bootstrap measurements are pending.

ZIP3,004,460 bytes; SHA256:
`45b6b92526ce4c359a039acc923fa1e7f9226a47912ac525c34466d7c76753eb`.

## Completed test: local character admin permission

[Download ps2_local_admin_test.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_local_admin_test.zip)

Click **Download raw file**. Close the old local game/server, extract into a
fresh folder outside the game, then run **start_local_admin_test.cmd**.
Choose the copied Local client and **LocalVSTest → Play**. At the3D view close
menus/map/chat and run **activate_and_capture_admin.cmd once**. Return within
**eight seconds** and stay idle until **THREE beeps**. **No W-held phase**.
Upload **local-admin-access-….zip**, including failures. After capture try
**/loc** and report the exact response. [Procedure](../docs/LOCAL_ADMIN_TEST.md).

The server grants the verified IsAdminAccount login field to all five local
characters. The test measures the native permission and retains the existing
class19 activation/warpgate placement. It does not grant an admin client build
or implement every gameplay command. Windows permission delivery is now verified
for LocalVSTest in all three224846 captures. Observer and /loc produced no visible
effect; command execution remains unverified. Model/arms and camera remain incomplete; the CONTINENT header metadata
issue is tracked separately and unchanged.

All398 Python tests and16 native permission cases pass, including
all-five-character encrypted delivery and stable saves across two starts.
All25 previous helpers and87 parent members remain byte-identical.
ZIP2,996,397 bytes; SHA256:
`9643e66081e018c1ad060c3503cdf98b86fa967df62158749b9f5ee55d4ceaf8`.

## Completed test: class19 placement and movement

Actual220929 confirms actor position changes about9.48 game units between placed
idle and W-held, with nonzero ground positions and jumping cleared. User says
the view remained fixed and audio direction may have changed. Camera control,
appearance, safe floor and complete spawning remain unresolved. The old test
procedure is preserved below as history; no unchanged rerun is requested.

[Download ps2_profile_placement_test.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_profile_placement_test.zip)

Click **Download raw file**. Close the old local game/server and extract into a
fresh folder outside the game. Run **start_profile_placement_test.cmd**, choose
the copied Local client, then **LocalVSTest → Play**. At the3D view close
menus/map/chat and run **place_and_capture_motion.cmd once**. Return within
**eight seconds** and stay idle until **TWO beeps** (about25 seconds). Hold
**W** then release at **ONE beep**. **THREE beeps** means finished; upload
**local-profile-placement-….zip**, including failures. Report view translation;
try mouse-look separately after capture. See [the procedure](../docs/PROFILE_PLACEMENT_TEST.md).

Windows already verified class19 activation. This combines that exact message
with the existing location packet, then measures actor position at idle,
W-held and release at the northern-warpgate asset anchor. Model0 is preserved;
correct appearance, safe floor and movement remain unverified. There is no
new guessed state/stat/model/sync value or automatic character spawn.

The original format13 observer/16KiB budget and86 parent members are byte-exact.
All24 previous helper hashes remain unchanged. CRC/manifest/isolated imports
and44 native class19 location fixture cases verify. All391 Python tests pass,
including encrypted delivery, failure reports and stable saved state across
two starts. Windows outcome is pending.
ZIP2,990,073 bytes; SHA256:
`4019590682ba913c4bf33c814a2c46e42cc92ce37957a56cc2218a6abaeda6d4`.

## Completed test: actor profile activation

Actual175452 confirmed actor class0 →19, cache19 installation and selection.
Saved/current profile19 and loadout17 already existed. Model0/missing arms and
origin position persisted with W. The new combined test above is the next
comparison; no unchanged activation rerun is requested. The older procedure
and validation below are preserved as history.

[Download ps2_profile_activation_test.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_profile_activation_test.zip)

Click **Download raw file**. Close the old local game/server and extract into a
fresh folder outside the game. Run **start_profile_activation_test.cmd**, choose
your copied Local client, then **LocalVSTest → Play**. At the 3D view close
map/menus/chat and run **apply_and_capture_profile.cmd** once. Return to the game
within **eight seconds** and remain idle. At **TWO beeps**, hold **W** until
**THREE beeps**. Upload **local-profile-activation-….zip**, including failures,
and report whether the view moved or appearance changed.
See [the procedure](../docs/PROFILE_ACTIVATION_TEST.md).

This sends one recovered current-build profile activation message to test actor
class **0 → 19** independently of saved loadout references. The exact native
dispatcher/cache/class setter passes in the offline fixture. Model 0 is preserved;
correct appearance, weapons and movement remain unverified. The helper records
before/idle/W-held snapshots and never retries an uncertain application.

385 Python tests and 10 native cases pass. Extracted imports/manifest, actual
encrypted UDP, health/status/report CRC and stable state across two starts verify.
All 23 previous helpers are preserved. ZIP: 2,984,244 bytes; SHA256:
`b1070038bba02a5db5223551376615cc845665557ed2ce0bda9d11d1a43a05fa`.

## Completed test: character initialization

[Previous diagnostic](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_character_initialization_test.zip)

Actual 081250 confirmed actor class 0 while saved/current profile 19 was present.
Class 19 model references remain zero; model 0 presence was unknown because the
search was incomplete, while its arms mapping was a complete miss. The next
test above applies the newly recovered activation route. No unchanged rerun of
the old read-only diagnostic is requested.

## Completed test: VS armor and helmet delivery

The112826 result verified both owned items and GUID/slot links. The measured
base actor state stayed unchanged. The first-person screenshot did not measure
third-person cosmetic attachments. No unchanged rerun is needed.

[Download ps2_cosmetic_inventory_test.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_cosmetic_inventory_test.zip)

Click **Download raw file**. Close the old local game/server and extract into a
new folder outside the copied game. Run **start_cosmetic_inventory_test.cmd**,
choose the copied Local client, select **LocalVSTest**, then Play. At the3D world
view close menus/chat, run **apply_and_capture_cosmetics.cmd** once and switch
back to the game. Stay idle until **THREE beeps** (about15 seconds); no W key is
required. Upload **local-cosmetic-inventory-....zip**, including a failure, and
report any armor/helmet/arms change. See [the procedure](../docs/COSMETIC_INVENTORY_TEST.md).

This new isolated server mode supplies the VS starter armor/helmet instances and
slot10/11 GUID links. It verifies actual client type39 metadata and model templates
before sending. Appearance application, full weapons and movement remain
unverified. Original preview, controls, timing and all21 older ZIPs stay frozen.
370 Python tests and24 native expectations pass (including2 expected metadata
boundaries). Extracted imports, manifests, actual encrypted delivery, status/report
CRC and stable saved state across2 starts verify.2,967,926 bytes; SHA256:
`859f60ee171595712237d23e06f1fe6d0125df789be9a1fb33eadd90750ee6db`.

## Completed test: character model and movement diagnostics

[Download ps2_actor_loading_test.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_actor_loading_test.zip)

Click **Download raw file**. Extract into a new folder outside the copied game;
close the old local game/server. Run **start_actor_loading_test.cmd**, select
the copied Local client and **LocalVSTest**, then Play. At the world HUD close
menus/chat and run **capture_actor_loading_test.cmd** once. Switch back to the
game within eight seconds. Remain idle until **TWO beeps**, then hold **W** until
**THREE beeps**. Upload **local-actor-loading-....zip**, including a failure.
See [the procedure](../docs/ACTOR_LOADING_TEST.md).

The unchanged v2 server reproduces the verified two-item baseline. New read-only
observations check the matching actor's model, actor-definition name, first-person
arms flags and movement fields at idle and W-held, together with game focus.
This is a diagnostic test, not a movement or appearance fix. No PC REA setup.
365 Python tests and24 native getter cases pass; extracted imports, manifest,
health/status/report CRC and stable saved state across2 backend starts verify.
Actual Windows rendering, input consumption and playable spawning are pending.
All20 earlier ZIPs stay frozen.2,961,361 bytes; SHA256:
`d519855e9149c792c117fb4678d294155dfc87cecb3f3d79ef0a47ae0d5a5d5a`.

## Previous test: owned equipment revision2

[Download ps2_owned_inventory_test_v2.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_owned_inventory_test_v2.zip)

Click **Download raw file** on the GitHub file page. Close the previous local game
and server; extract this ZIP to a new folder outside the game. Run
**start_owned_inventory_test.cmd**, choose the copied Local client, select
**LocalVSTest** and press Play. At the world HUD run
**apply_and_capture_owned_inventory.cmd** once. Upload its
**local-owned-inventory-….zip**, including any failure report.
[Procedure](../docs/OWNED_INVENTORY_TEST.md).

The first report confirmed the equipment references and both item definitions,
but the helper rejected native lookup mode1. Revision2 validates both modes0/1
and requires the same exact single equipment row and slot/GUID links. Server
item/self packets and saved state are unchanged. Appearance, weapons, abilities,
control and spawning remain unfinished.

352 Python tests and13 focused native expectations pass, including the expected
missing-metadata boundary. The revised guard accepts the original unmodified
Windows report. The extracted bundle verifies imports, health/status/report CRC
and unchanged state across two starts. All19 older ZIPs stay frozen.
2,954,606 bytes; SHA256:
`5f363dc69369a0fd31653b7a406972967f1c616b6279da4d66802bf8b1a7bbe3`.

## Previous test: LocalVSTest owned equipment v1

[Download ps2_owned_inventory_test.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_owned_inventory_test.zip)

On the GitHub file page, click **Download raw file**. Extract into a new folder
outside the copied game. Close the old local game/server. Run
**start_owned_inventory_test.cmd**, choose the copied Local client, select
**LocalVSTest** and press Play. At the world HUD, run
**apply_and_capture_owned_inventory.cmd** once. Upload its
**local-owned-inventory-….zip**, even if the test stops or the game closes.
See [the procedure](../docs/OWNED_INVENTORY_TEST.md).

This checks two owned starter equipment instances and their GUID links.
Before sending, it captures and verifies actual character state and native item
metadata. It then sends once and captures again. Appearance, weapons, abilities,
movement and spawning remain incomplete. No location command is included.

351 Python tests pass.20 focused native cases meet expectations, including one
expected missing-metadata boundary. The actual incremental decoder creates both
items and resolves their slot references without changing the raw selection.
The extracted bundle verifies status/report CRC and unchanged state over two
starts. Actual Windows acceptance remains the next test. All18 previous ZIPs
stay frozen.2,953,503 bytes; SHA-256:
`d0cfe623d88d08a43bb00b1cdfb59b355274d008cd477ab55ca8bbe826264c84`.

## Previous test: character loadout delivery

[Download ps2_character_loadout_test.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_character_loadout_test.zip)

On the GitHub file page, click **Download raw file**. This separate test applies
the verified saved/current VS Light Assault loadout references to **LocalVSTest**
and captures their actual native state, owned/equipped counts and bounded shared
rank-zero item-line lookups. Appearance, owned items and playable control remain
unfinished; this is a character-data test, not a spawning fix.

Close the old local game/server, extract outside the copied game, run
**start_character_loadout_test.cmd**, choose the copied Local client, select
**LocalVSTest** and press Play. At the world HUD, run
**capture_character_loadout_test.cmd** and upload the new **local-state-....zip**.
If loading stays up, wait30 seconds and capture there. See
[the procedure](../docs/CHARACTER_LOADOUT_TEST.md). No repeat location probe.

345 Python tests and14 focused native checks pass. Extracted package imports,
live startup/report CRC and unchanged starter saves across2 starts verify.
Actual format8 Windows capture/rendering remains to be tested. All17 earlier
ZIPs stay frozen.2,944,599 bytes; SHA-256:
`3b4d3d4ae10dfcf0d511caf9f226193ec8e85ac8ee73b4a94f77037edd85f303`.

## Previous release: separate VS testing character

[Download ps2_vs_test_character.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_vs_test_character.zip)

Adds **LocalVSTest** as a fifth character with a separate identity. The original
LocalVS/NC/TR/NSO records remain unchanged. A shared server initializer persists
separate starter seeds for LocalVS and LocalVSTest: VS Light Assault profile19,
loadout17,14 resolved equipment references and per-character backend slot links.
Native appearance/loadout/inventory delivery and playable control remain pending.
This is a new testing identity, not the equipment/character-rendering fix.

Extract into a separate folder outside the copied game. Close the previous local
game/server, run **start_vs_test_character.cmd**, select the copied Local client
and choose **LocalVSTest** (fifth entry). No repeat location test needed yet.
See [the procedure](../docs/VS_TEST_CHARACTER.md). An opt-in five-row read-only
capture is supplied for future initialization tests.

333 Python tests and3 pinned native decoder cases pass. Extracted bundle start,
report integrity and stable starter seeds across a restart are verified.
Windows display/login of the fifth character remains unverified.
All16 prior helpers stay frozen.2,931,086 bytes; SHA-256:
`ea996259d2d39303f332baa7e9e521b043ff5c28283519c338c23dddcc2e8b8a`.

## Previous test: one-shot LocalVS placement

[Download ps2_location_delivery_probe.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_location_delivery_probe.zip)

The matching actor was measured at the origin. This opt-in experiment sends the
native UpdateLocation message once, targeting an exact Indar spawn-tube asset
anchor. A safe player spawn and rendered camera attachment remain unverified.

Close the previous local game/server. Extract into a new folder outside the game,
run **start_location_probe.cmd**, select the copied Local client and enter
**LocalVS**. When its world view/HUD appears, run **apply_location_once.cmd**.
Upload the resulting **local-location-....zip** and a screenshot. Do not resend
in the same run. See [the procedure](../docs/LOCATION_DELIVERY_PROBE.md).

**325 Python tests** and **44 pinned native location checks** pass. Fixtures
isolate services/notifications and camera output; actual decoder, actor setter
and transform math execute. Windows acceptance remains the next test.
All15 previous helper ZIPs remain frozen. 2,930,319 bytes; SHA-256:
`95f8904b53e0d15ef22b1b1c6231e5ebb864a1774d6f269a30f01a10df902ef7`.

## Earlier test: LocalVS actor identity and position

[Download ps2_local_actor_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_local_actor_capture.zip)

The previous capture found an active actor but did not measure its position.
This read-only diagnostic adds verified actor identity and clean position-cache
observations. It does not change spawn or equipment data.

Keep the local class-probe server and existing **LocalVS** world view open.
Extract into a new Desktop folder, run **capture_local_actor.cmd**, select the
copied Local game folder and keep the view still. Upload the **local-state-....zip**
from this helper's **reports** folder and a screenshot. It does not start the
server; use the original **start_class_probe.cmd** if needed and leave its READY
console open. See [the procedure](../docs/LOCAL_ACTOR_CAPTURE.md).

**317 Python tests** and **16 pinned native actor getter cases** pass. Native
objects/Lua output sinks are fixtures; actual getters and original vtables run.
Windows actor measurement is pending. All14 older ZIPs retain their hashes.
31,351 bytes; SHA-256:
`4e1059776186140a72e66c9db2be21ffdfd3a525663127f28861a64df6e3b4ba`.

## Completed capture: active world actor and graphics location

[Download ps2_local_scene_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_local_scene_capture.zip)

The previous state capture succeeded and the screenshot shows 3D map geometry.
This read-only diagnostic checks whether the client has an active world actor
and observes the graphics position used by GetPlayerLoc's fallback. It does
not change spawn data or claim to fix placement.

Keep the existing local class-probe server and game open on **LocalVS**. Extract
this ZIP into its own Desktop folder, run **capture_local_scene.cmd**, choose
the copied Local game folder and hold the view still. Upload **local-state-....zip**
from its **reports** folder and a screenshot. This helper does not start the
server; use the original **start_class_probe.cmd** if needed and leave its READY
server window open. No repeat timeline. See [the procedure](../docs/LOCAL_SCENE_CAPTURE.md).

All **303 Python tests pass** and **four pinned native GetPlayerLoc branch
cases pass**. The actor virtual getter/Lua sink in those native fixtures are
explicit substitutes; actual Windows scene observation is the next test.
All 13 previous ZIPs retain their hashes. SHA-256:
`47564f556151144fb1895d7976f075a40b3156d59f9d813f60438dc0159f1663`.

## Completed capture: current local game state

[Download ps2_local_state_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_local_state_capture.zip)

The received Windows timeline confirms 24 classes, completed profile preloading
and state 27, matching the gameplay HUD/map screenshots. The 3D view is sky-only;
spawn, terrain and item/inventory delivery remain unfinished. The original
probe's detailed snapshot worker had an import-path packaging error.

Leave the local game and class-probe server open. Extract this new helper into
its own Desktop folder, run **capture_local_state.cmd**, choose the copied local
game folder containing PlanetSide2_x64.exe, and upload **local-state-....zip** from
the **reports** folder that opens. No repeat 90-second timeline, game patch,
server replacement or manual Python commands are needed.
See [the complete procedure](../docs/LOCAL_STATE_CAPTURE.md).

This read-only capture verifies the local server instance/mode and matching
local process, then exports bounded class/loader/player state. Two decoded
player-record vectors are observed without claiming live camera or valid-spawn
semantics. A failure ZIP contains no partial snapshot or install paths.
All twelve earlier helper ZIPs and game packet sources are unchanged.
New ZIP SHA-256: `2b9570421ef9043a2b22081ea32ed2d82de4d7f7564de7da4717dd7c3f5d6dcb`.
292 Python tests pass, including standalone/direct-subprocess regressions.
The earlier native execution counts are retained history, not newly rerun.

## Completed client test: isolated class delivery probe

[Download ps2_class_delivery_probe.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_class_delivery_probe.zip)

Extract outside your game and old V31.2 test folder. Close the old game/server.
Run **start_class_probe.cmd** and choose the copied **PlanetSide 2 Local game
folder containing PlanetSide2_x64.exe**. Select **LocalVS** and capture its class
state. Run **capture_probe_timeline.cmd**, press Enter, click Play once and wait
90 seconds. Capture class/loading state again, wait five seconds and save a
console report. Send both ZIPs from the helper's **reports** folder plus a screenshot.
See [the complete procedure](../docs/CLASS_DELIVERY_PROBE.md).

This tests receipt of 24 identity-only class records and loader creation. Native
constructor defaults fill the other fields; these are incomplete experimental
records. **It is not an Indar loading fix.** Full class-data research continues.
V31.2 timing and the existing controls/preview are preserved. The helper has its
own server/tools folders and installs nothing over the existing test.

282 Python tests pass; eight focused native constructor/delivery checks,
54 profile regressions (including three expected guarded overreads), and 83
world-confirmation checks meet expectations. Windows acceptance is unverified.
All eleven earlier helper ZIPs retain their hashes. New ZIP SHA-256:
`5f4b6e35922ac80e18b5225b07e9e1168320322097951fad84abd4f4e84d2f7e`.

The sections below retain completed tests and earlier evidence.

## Latest result: complete shared data received

The full pack verifies: all 2,647 resources and all 42 earlier tables match.
It supplies 225 tables, including the previously missing client requirement
tables. Jump-jet expression 3519 and its operands resolve, alongside all twelve
nonzero starter client expression references. The real Light Assault criterion
queries classification 12; full class memberships and Indar entry remain open.
Keep V31.2. No repeat capture or unchanged loading test is requested.
See `docs/reviews/shared-data-result-20261014.md` and the current checkpoint.
The helper procedure below describes the completed test.

## Earlier test: complete shared data pack

[Download ps2_shared_data_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_shared_data_capture.zip)

Extract to your Desktop, outside the game folder. Double-click
**collect_shared_data.cmd**, choose the copied **PlanetSide 2 Local** game folder
containing `PlanetSide2_x64.exe` and `Resources`, then upload the
**local-shared-data-….zip inside the results folder** that opens. Upload the
small failure ZIP too if verification stops. No running game/server, launcher,
administrator startup or manual Python commands are needed. Keep V31.2.

This captures the complete 10 MB shared asset pack with 2,647 resources,
including entries with unrecovered filenames. It verifies every resource,
anchors, exact matching pins, stable bytes and output hashes. The prior asset
test is complete; no repeat of that test or the online capture is requested.
Complete definitions and Indar entry remain unfinished.

All **275 Python tests pass**; setup and backend status/report export verify.
Prior native outcomes are retained history. All ten earlier helper ZIPs are
unchanged. SHA-256:
`5e821048f2b4273ad6191079b325b48058ac2099fc67c976d9b2aadd936fe110`.
See [the procedure](../docs/SHARED_DATA_CAPTURE.md).

## Latest result: local asset test completed successfully

The uploaded capture verifies all 132 matching pack indexes and recovers 19
tables, including 16 new ability/skill and weapon display/effect tables. The
results are saved. No repeat capture or new Windows loading test is requested.
Keep V31.2. Complete native definitions and requirements remain incomplete.
See `docs/reviews/weapon-resources-result-20261014.md` and the current checkpoint.
The helper procedure below describes the already completed test.

## Earlier test: local weapon and ability resources

[Download ps2_weapon_resource_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_weapon_resource_capture.zip)

Extract the ZIP into a fresh folder, then double-click
**collect_weapon_resources.cmd**. Choose the copied **PlanetSide 2 Local** game
folder containing `PlanetSide2_x64.exe` and `Resources/Assets`. Upload every file
inside the folder that opens: the capture ZIP, or all parts plus `manifest.json`.
Upload the small failure ZIP if the exact build/asset checks stop the capture.

No game, server, DBG launcher, administrator startup or manual Python commands
are needed. This reads only disk assets. It looks for 25 weapon/ability/skill table
candidates and preserves all matching pack indexes for further offline searches.
The executable, two anchors and all 132 saved index pins must match. Candidates
are not verified server-field mappings; Indar entry remains unfinished.
Keep V31.2 installed. All nine earlier helper ZIPs remain unchanged.

260 Python tests and 38 new native checks pass. Package CRC, source manifests,
standalone imports and backend status/export verify. Windows capture remains to
test. SHA-256: `31c6627613c149edccffe80e2578709b0fec58201ecb118367de83e54218c6d2`. See [the procedure](../docs/WEAPON_RESOURCE_CAPTURE.md).

## Latest result: revision-2 capture completed

The supplied online build and executable identity verify, but the module
snapshot is denied with OS error 5 before any process-memory reads. No unchanged
retry or new Windows test is requested. Keep V31.2 installed. See
`docs/reviews/online-module-snapshot-denied-20261014.md` and the current checkpoint
for the retained failure report and matching offline weapon-delivery trace.
The helper download/procedure below describes the already completed test.

## Next test: online code collector revision 2

[Download ps2_online_code_capture_v2.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/codex/class-definition-export/downloads/ps2_online_code_capture_v2.zip)

Extract into a fresh folder. Close the local game, launch the normal online game
through DBG and leave it at the character menu. Double-click
**capture_online_image_v2.cmd**. Upload every file inside the folder that opens,
including the small failure ZIP if access is denied. No spawning, administrator
launch or manual Python commands are needed.

This separate collector requests narrower read access and reports the exact
failing API. It keeps the verified online build/section guards and stops on
denial. A successful capture supplies code/constants for offline class-layout
comparison; it exports no class values and does not change local Indar loading.
Keep V31.2 installed. The eight earlier helper ZIPs remain unchanged.

SHA-256: `77bed3eb596a26974641a329db4410317bc5e2b8e4ec27579992829b007955b3`. All 246 Python tests pass; package CRC, source manifest
and standalone imports verify. Real Windows capture remains for this test.
See [the procedure](../docs/ONLINE_CODE_CAPTURE_V2.md).

# Local PS2 test helpers

## Current status

Local source work now executes named loadout membership, item-line-to-definition
resolution and packed loadout/slot key operations alongside received records,
owned-item indexes and bounded weapon detail: 233 Python tests and 731 native
expectations pass. Equipment wire/reverse GUID indexes now execute, and slots
resolve to real owned objects; native removal breaks stale references. Selected
equipment-row key/mode and shared catalog/progression/options are fixtures.
Native weapon conversion, metadata lookup and current/old loadout auxiliary
references now execute. GUID and definition lookups can select different owned
copies. Real ammunition/group/mode initialization and nonempty instance detail
now execute through the factory, including all four full player fixtures.
Partial auxiliary/service values remain fixtures; actor setup and notifications
are observed boundaries. Shared catalog
delivery, complete class/item/weapon values, publication and Windows Indar entry
remain unfinished. These changes are
validation tools; no new Windows helper or game-loading patch is published.
Keep V31.2 installed. See `docs/reviews/weapon-initialization-20261014.md`.

The online code collector verified the supplied build but Windows denied process
access (error 5). Its report contains no code sections or class values. The user
launches normally through DBG without administrator startup; the exact cause
is not established. The report is saved. No unchanged capture or new Windows
loading test is requested. Local-image class analysis continues; keep V31.2.
See `docs/CURRENT_CHECKPOINT.md` and
`docs/reviews/weapon-initialization-20261014.md` for the current review.

## Earlier capture: online loaded code and constants

[Download ps2_online_code_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_online_code_capture.zip)

Sign into GitHub and choose **Download raw file**, then extract into a fresh
folder. Close the local game; leave the normal online game at its character
menu. Double-click **capture_online_image.cmd**. Upload **every file inside the
folder that opens separately**: one ZIP, or all `.part` files plus `manifest.json`.
Send its failure report too. No spawning or folder-zipping is needed.

All three supplied disk-copy parts and the executable verify. The online version
is 0.0.1534.609692; the local reference is 0.0.1533.609523. Section positions
changed, and code/class constants are protected on disk. This collector is
locked to the supplied online hash and captures only loaded code and read-only
constants for comparison. It exports no class values or writable/heap regions.
All 232 Python tests pass. Actual Windows code capture and class compatibility
still need verification. Keep V31.2 installed. See `docs/ONLINE_CODE_CAPTURE.md`.

Package SHA-256:
`dc559123ca9f9f9d2b01633238c98a4daedea60ef01bd31b8040dac4ab98697e`.

## Earlier capture: compare the online executable

[Download ps2_client_build_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_client_build_capture.zip)

Sign into GitHub and choose **Download raw file**. Extract into a fresh folder.
Close the local game, open the normal online game at its character menu, and
double-click **capture_client_build.cmd**. Upload **all files in the folder that
opens**: either one ZIP or every `.part` file plus `manifest.json`. Large files
are split automatically; no manual Python commands or spawning are needed.

The revision 2 class report confirms executable detection, but the online file's
SHA-256 differs from the local reference. This separate helper copies only the
on-disk executable and metadata for offline comparison. It reads no process
memory and does not authorize an unverified layout for class capture.
The original matching executable passes the disk-copy, CRC/checksum and three-part
reconstruction check. All 224 Python tests pass. Actual online capture remains
pending. Keep V31.2 installed. See `docs/CLIENT_BUILD_CAPTURE.md`.

Package SHA-256:
`363ce00d0f12510fcce2ec5b3e6ff268a145bfabc72b378e843a85b3a2cd6592`.

## Earlier capture: complete static class definitions

[Download ps2_class_definition_export.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/codex/class-definition-export/downloads/ps2_class_definition_export.zip)

Use a signed-in GitHub session and choose **Download raw file**. Extract the
ZIP to its own folder. Enter the normal online game, spawn once, leave that
game window open, and double-click **capture_class_definitions.cmd**. Upload
the generated **local-class-definitions-....zip** from its **reports** folder.
Close the local game window first so only one game process is running.

This read-only helper verifies the matching executable and exports only static
numeric VS class definitions, including both collections. A different build
produces a hash report before memory is opened. Keep V31.2 installed in the
local copy. This capture is separate from a local loading-screen trial.
Revision 2 fixes the first capture's missing executable path by querying Windows
directly with limited process access. No install-folder selection is needed.
The console displays **Read-only class exporter revision 2**. If Windows denies
access, upload the report with its stage and error code.
Standalone package checks and 217 Python tests pass; 357 native expectations
include full record reconstruction. The revised Windows helper still needs a
capture on the user's PC. See `docs/CLASS_DEFINITION_EXPORT.md`.

Revision 2 package SHA-256:
`b88b12f44428fb04ad9cf4f18f7b86efab5330bc1b888fea98152a2e839b3b56`.

## Earlier test: V31.2 preload timing

[Download ps2_preload_timing_v31_2.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/main/downloads/ps2_preload_timing_v31_2.zip)

On the GitHub file page, click **Download raw file** near the upper right. Use
your signed-in GitHub session.

1. Close the game, client launcher, controls window, and local server.
2. Save the patch ZIP, choose **Extract All**, and double-click
   **apply_preload_patch.cmd**.
3. Choose your existing **V31 test folder** containing
   `start_local_server.cmd` and `start_local_client.cmd`. The installer verifies
   the three original source files and backs them up automatically.
4. Restart the server, client and controls from that same test folder.
   Select **LocalVS** and wait before clicking Play.
5. Use your existing **ps2_loading_observer** helper below. Capture Play once
   for **90 seconds** and upload the timeline ZIP and same-run console report.

This patch sends PreloadDone after the client's native readiness message,
following the loading flag reset. The next check is whether the preload flag
stays true. Complete class definitions are still missing; world entry remains
unfinished. The packet bytes, starter data and other V31 controls are preserved.

Package SHA-256: `92f448427a181984f876a1602c45ab0ba539e94699ec88a9c18a7b057325c2fd`.

Validation: 151 Python tests and 83 native world checks passed, including native
loading reset and readiness-message serialization. Five installer checks passed,
including rejection of different source files/corrupt payloads and rollback
after an interrupted write. The Windows folder picker and patched live game
need the next Windows run. No executable, assets, credentials or logs are shipped.

## Optional launcher for one test folder

If you have several test folders, an optional helper can apply V31.2 and open
the server, client and controls from one verified folder:

[Download ps2_preload_timing_v31_2_launcher.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/main/downloads/ps2_preload_timing_v31_2_launcher.zip)

Close all old consoles, **Extract All**, and double-click
**start_patched_test.cmd**. Choose the existing V31 test folder. Wait for
**V31.2 SERVER VERIFIED**, then perform the same LocalVS timeline capture.
The helper refuses occupied server ports and checks the running build before
opening the client and controls. It preserves backups and also accepts the
original V31.2 patch. The server startup banner now explicitly identifies V31.2.
The original timing patch above remains valid; its packet behavior is unchanged.

Package SHA-256: `0b06458a7a3b668025f1767c2009e5214fdc2a43a2d347e291c84c534c36fba3`.

Validation: all 158 Python tests pass. A patched copy of the original V31 archive
starts the backend with the expected fingerprint and preserves the zone/starter
data. The launch-order and wrong-build guards pass. Actual Windows window/game
launches remain for Windows verification.

## Loading timeline helper

[Download ps2_loading_observer.zip](https://github.com/MoonzTJ/PS2_LOCAL/blob/main/downloads/ps2_loading_observer.zip)

1. Save the ZIP and choose **Extract All** in Windows.
2. Start your patched V31.2 local server and game. Select **LocalVS** in the
   character menu and wait before clicking Play.
3. In the extracted `ps2_loading_observer` folder, double-click
   **capture_loading_timeline.cmd**.
4. Press **Enter** when prompted, switch to the game and click **Play once**.
5. Leave the game open for **90 seconds**. Upload the
   **local-loading-timeline-....zip** from the folder that opens.
6. Export and upload the usual console report from that same run.

This helper records loading flags and bounded handler checks from the verified
local dummy-session client. It uses your installed Python. It does not change
the game or server. Short transitions can fall between samples.

Package SHA-256: `db349a677b16f305c3970db6997827dd7b21ce22db3167a8a1aca4d1160bd4b1`.

Validation: standalone ZIP/import and ten observation guards passed. The user
successfully captured two Windows timelines with 597 available samples each.
The current full 158-test regression suite passes.

## Loaded code capture

[Download ps2_native_capture.zip](https://github.com/MoonzTJ/PS2_LOCAL/raw/refs/heads/main/downloads/ps2_native_capture.zip)

1. Save the ZIP and choose **Extract All** in Windows.
2. Start your existing V31 local server and game; leave the game open at the character menu or INDAR loading screen.
3. In the extracted `ps2_native_capture` folder, double-click **capture_loaded_image.cmd**.
4. Upload the capture ZIP it names. If it creates parts, upload every `.part` file and that folder's `manifest.json`.

The helper uses the existing default game folder:
`C:\Users\Public\Daybreak Game Company\Installed Games\PlanetSide 2 Local`.
It verifies the matching V31 executable and local dummy-session launch, then reads three loaded code sections. It automatically splits large results into 28 MiB upload parts.

Package SHA-256: `a551d187ce8e62a9889a60c79607db1508a9e78317c3b5b9a9bbcdb2b1c1c6c2`.

Validation: ZIP CRC and standalone import checks passed; the user successfully
captured and uploaded the matching loaded image with this helper.
