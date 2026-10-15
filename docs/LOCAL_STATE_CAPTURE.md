# Capture the current local game state

The received timeline confirms 24 class records, completed profile preloading
and state 27 in Windows. The older probe's detailed snapshot worker had a
packaging import error. This separate helper supplies that missing snapshot
from the already-running game; no repeat 90-second timeline is needed.

1. Leave the current local game and its class-probe server open. If they are
   already closed, start the existing class probe and enter the game once.
2. Extract `ps2_local_state_capture.zip` to its own Desktop folder outside
   the game and previous helper folders.
3. Double-click **capture_local_state.cmd**. Choose the copied **PlanetSide 2
   Local game folder containing PlanetSide2_x64.exe**.
4. Upload the **local-state-....zip** inside the **reports** folder that opens.
   Upload the small failure report too if capture stops.

No install, new game patch, server replacement or manual Python commands are
needed. Use the existing 64-bit Python 3.10+. Exactly one PlanetSide2_x64 process
must be open, from the matching copied local installation and dummy session.
The helper checks the local server mode/packet/instance before and after reading.
The EXE, local config, launch arguments and existing code signatures are verified.
There is no elevation or change to access protections.

The report includes the existing bounded local character/class/loader/player
observations, plus two stable decoded player-record vectors at +220/+230.
Their reader signatures, local player identity, finite floats and stable bytes/
pointers are checked. These are **not a verified live camera position or valid
spawn**. They help test the existing placeholder-vector hypothesis without
guessing replacement coordinates. A failed optional field remains unavailable.

Memory access remains read-only, with the existing shared 16 KiB read budget.
No raw addresses, process command lines, keys, arbitrary memory dumps or online
account data are exported. The helper sends only read-only health requests to
the loopback local server. It does not write to the process, game or backend,
load code into the game, suspend it, or operate on DBG/anti-cheat services.
Reports stay outside the selected game directory; a failed capture exports no
partial snapshot or install path. Server identity is checked separately from
the process identity, rather than claiming a cryptographic binding between them.

Full class/item/owned-inventory data, live camera/terrain loading and valid spawn
remain separate unfinished work. The original twelve helper ZIPs are unchanged.
