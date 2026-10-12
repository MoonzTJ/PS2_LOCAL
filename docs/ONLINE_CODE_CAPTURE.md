# Online loaded code and constants

The supplied executable is online version **0.0.1534.609692**, different from
the local **0.0.1533.609523** reference. The three uploaded parts and the
executable/metadata hashes are verified. Section positions changed, and the
class functions/constants are stored in protected form on disk in both builds.
Loaded code is needed to compare their actual class layouts. This helper is
locked to the exact online executable you supplied.

1. Extract the helper ZIP into a fresh folder.
2. Close the local game. Open the normal online game and wait at its character
   menu. Leave it open; spawning is unnecessary for this code capture.
3. Double-click `capture_online_image.cmd` and wait for the upload folder to open.
4. Upload **every file inside that folder separately**. This is either one
   `online-loaded-code-....zip`, or every `.part` file plus `manifest.json`.
   Send the small report ZIP if the capture fails. No folder needs to be zipped.

The helper uses your existing 64-bit Python 3 installation. Large results are
split into 28 MiB parts automatically. No manual Python commands are needed.
If the online launcher updates the executable, the helper stops before memory
reads and creates a report containing the different hash.

Process access is QUERY_INFORMATION | VM_READ. The collector reads only the
verified main module's header, first `.text`, `.rdata`, and `_RDATA` sections:
67,429,376 code/constant bytes, each checked by a second read. Writable `.data`,
the heap, account/character lists, inventory, command lines and configuration
are excluded. It does not call functions, start/change the game, modify memory,
suspend the process, attach a debugger or change access protections. A Windows
access denial is recorded and remains a failure.

Every selected section must retain its verified name, position, size and
read-only flags. Expected named class constants must be present after loading.
These are capture guards; they do not establish class layout compatibility.
The capture ZIP and optional upload parts include SHA-256 manifests and CRC
verification. Failure reports contain no partial section data or install paths.

The old class exporter's hash/layout guards remain unchanged. This collector
exports no class values and proves no local Indar entry. Keep V31.2 installed.
