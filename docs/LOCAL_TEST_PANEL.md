# Local Test panel

Extract this ZIP into a new folder outside the copied game. Close the previous
local game/server. Double-click **Start Local Test.cmd**, choose the copied
Local game folder and click **Start local session**.

Only one server CMD console, this panel and the game stay open. Client monitoring
runs in the background and writes its log to **reports**. The startup CMD exits;
no separate client, controls or capture CMD files are needed. The local server
continues independently; type **quit** in its console when finished. Closing the
panel does not stop the server or game.

Use the panel's **Capture class / loading state** and **Save server report ZIP**
buttons when requested. Creator capture and the existing one-shot bootstrap
diagnostic are under **Advanced**. Bootstrap still requires returning to the
game within eight seconds and staying idle until THREE beeps. Its activation/
placement and all native guards are unchanged; it is not a new gameplay fix.
Do not repeat it in a session. Reports stay in the reports folder; Explorer is
not opened automatically.

The faction/character-creation UI uses its original faction and character-row
draw methods: D08 labels and the D12 native-row debug panel are no longer added.
The local MoonSol server-label fallback and Indar loading-screen fix remain.
The external Moon_local status watermark is disabled in this launcher.

The clean UI is built from original assets into a separate clean overlay. A
recognized generated local config is backed up before switching it; customized
configs are preserved and rejected. Original assets/config and the old overlay
remain intact. Start older diagnostic ZIPs only with their matching config.
The new package has one startup entry; old helper ZIPs are retained as historical
baselines rather than mixed into the current test folder.

Requires Windows, 64-bit Python 3.10+ with Tkinter and py/pyw, and the matching
copied local client. This is a launcher/UI cleanup over the existing bootstrap
backend. Character creation, correct VS appearance/loadout, movement and logout
are still unfinished. This package does not request another unchanged gameplay
test. Native Windows window behavior and clean UI rendering need confirmation
when the next useful client test is run.
