# Read-only class definition export

Use the normal online PlanetSide 2 client for this capture. Keep the V31.2
local copy installed for later local tests. This helper runs separately and
does not install or patch anything in either game folder.

1. Extract the helper ZIP to its own folder.
2. Close the local test game window, so exactly one game window is open.
3. Open the normal online client, enter the world, and spawn once with an
   existing character, preferably VS. Any continent is suitable for this capture.
4. Leave the game open and double-click `capture_class_definitions.cmd` in the
   extracted helper folder.
5. Upload the ZIP created in the helper's `reports` folder. Its name starts
   with `local-class-definitions-`.

If the helper reports a different client build, upload that report as well.
It records the executable hash and stops before reading process memory. If
the class map is empty or exceeds the capture limits, the report marks it
incomplete and exports no fabricated record values.

The helper uses 64-bit Python 3 and Windows PowerShell. It verifies the selected
executable against the matching build's SHA-256 and checks the loaded native
signatures. Access uses QUERY_INFORMATION and VM_READ only. If Windows denies
access, send the generated report; the helper does not change access protections.

Only static numeric definitions for VS class keys 17/19/20/21/22/23 are exported.
The helper does not read command lines, credentials, account/character lists,
inventory, chat, player positions, or arbitrary memory regions. No packets are
sent and no native functions are called. Exported data contains scalars, flags,
floats, bounded nested-map triples and classification IDs, with pointers omitted.
Stable records include a wire reconstruction and SHA-256 for offline decoding.

The capture budget is 16 KiB. Each record supports up to eight nested-map entries
and sixteen classification IDs. Larger collections retain their count and are
explicitly incomplete. A completed export establishes the observed static values
and wire structure; class delivery, item metadata, inventory, assets and local
Indar entry still require separate validation.
