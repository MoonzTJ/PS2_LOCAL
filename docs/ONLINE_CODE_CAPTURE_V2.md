# Online code capture — revision 2

This is the next focused capture test. It helps compare the supplied online
build's class/weapon code with the local reference. It does not change the
local server or solve Indar loading by itself. Keep V31.2 installed.

1. Extract this ZIP into a fresh folder.
2. Close the local game. Start the normal online game through the normal DBG
   launcher and wait at the character menu. Leave it open. No spawning is needed.
3. Double-click **capture_online_image_v2.cmd** in the extracted folder.
4. Upload **every file inside the folder that opens**: either one capture ZIP,
   or all `.part` files plus `manifest.json`. Upload the small failure ZIP if
   Windows denies access or the build has changed.

Your existing 64-bit Python 3 installation is used automatically. No manual
Python commands or administrator startup are needed. Large captures split into
28 MiB upload parts automatically. Do not zip the upload folder again.

The earlier capture reported OS error 5 under one broad initialization stage.
Revision 2 requests only `VM_READ` (0x0010) for memory, verifies the executable
through a separate `QUERY_LIMITED_INFORMATION` (0x1000) handle, and uses the
documented Toolhelp32 module snapshot. Each failing API gets its own sanitized
stage and error code. This narrows requested access; Windows may still deny it.
The helper stops on denial. It changes no privileges or game protections.

The exact supplied online executable is required: version 0.0.1534.609692,
SHA-256 `6538c6cf5b7964974df70d12f5f78f6a97e5f104c4819995d5cf3ea45983bf4b`.
A different disk hash stops the collector before memory reads. Process and
main-module paths must identify that same executable. Every selected section
must retain its expected name, position, size and read-only flags.

Only the verified main module's PE header and three read-only sections are
read: `.text`, `.rdata`, `_RDATA`, totaling 67,429,376 section bytes. A second
read verifies stability and all reads have a fixed scope and byte budget.
Writable data, heap, class records, accounts, characters, inventory, command
lines and configuration are excluded. No process writes, function calls,
injection, suspension, debugger attachment, game launches or protection changes
are performed. Failure archives contain only the sanitized report and manifest;
partial section files and install paths are omitted.

A successful capture provides loaded code/constants for offline comparison.
It does not export real class values, establish matching layouts, or prove local
Indar entry. If access remains denied, the new report identifies the precise
failing step so the next decision can be based on evidence.
