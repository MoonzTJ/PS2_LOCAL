# PlanetSide Arena as a local reference

From the cloud session, 2026-11-01.

Moon has a local copy of PlanetSide Arena's game assets. Arena is a 2019
Forgelight fork of PS2 that shut down in January 2020. **Keep every Arena file
local. Never commit any of it.**

## Why it matters

- It is **not newer than our client**. Our build is DX11; PS2 moved to DX11
  around 2021. Arena sits between the 2015 references (ps2-emu, soe-network)
  and our build.
- No Arena server, emulator or capture is known. Its value is as a second
  client to compare against, not as a server reference.

## Possible uses, in order of cost

1. **Data tables our client lacks.** List the table files in Arena's .pack2
   files and compare them with ours. Look especially for item classes, item
   categories, item classifications, requirements, or anything matching the
   ReferenceData names our client expects. Report the columns and a few rows
   (IDs and small values only). Arena's IDs and content differ from PS2's:
   use them for structure and typical values, and confirm against our client.
2. **Protected code.** If Arena's executable is available, check whether the
   code that sets actor+30E8 bit 0x80 (arms ready), protected in our build,
   is readable there. Report the logic only, in your own words.
3. **Reader layouts.** Where one of our readers is unclear (the equipment row,
   the class records), Arena's version of the same reader is a midpoint
   between 2015 and our build.

## Open cloud requests (not yet answered; usage limit reached on 2026-11-01)

- `downloads/PS2_LOCAL_cloud_request_weapon17_2026-11-01.txt`
- `downloads/PS2_LOCAL_cloud_request_stats_chat_2026-11-01.txt`

The parsed weapon data from the 2015 capture (weapon 17, fire groups 15/1015,
ammo 30/210, the item 44 instance) is already in
`PS2_EMU_WEAPONS_WORLD_2026-10-31.md`.
