# Copy the online client build

The class report confirms that executable detection works. Its online executable
hash differs from the local reference build, so no class memory was read.
This separate helper copies only `PlanetSide2_x64.exe` from disk for offline
comparison. It does not enable class capture on an unverified build.

1. Extract this helper ZIP into a fresh folder.
2. Close the local game. Open the normal online game and leave it at the character
   menu; spawning is unnecessary for a disk-file copy.
3. Double-click `capture_client_build.cmd` in the extracted helper folder.
4. Wait for the upload folder to open, then upload **all files inside that folder**.
   This is either one `local-client-build-....zip`, or several `.part` files plus
   `manifest.json`. The helper splits large ZIPs automatically into 28 MiB parts.

The original ZIP is also retained under `reports`. No manual Python commands or
install-folder selection are needed. Use 64-bit Python 3 already installed on
the game PC. If copying fails, send the console error text.

The only process access is QUERY_LIMITED_INFORMATION to find the image path.
No process memory, command line, account information, configuration, inventory,
chat or neighboring game files are read. The on-disk executable is copied with
SHA-256 verification before and after; the ZIP has a file checksum manifest and
CRC verification. Neither the source file nor the game process is changed.

The executable must be a bounded 64-bit PE file. Its SHA-256 and generic section
layout are recorded without local install paths. A different hash remains a
different build; it does not become the validated local reference. Static class
values are not collected by this helper. The online layout and compatibility of
any eventual class values must be established separately. Keep V31.2 installed.
