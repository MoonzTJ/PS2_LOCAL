PS2 native input supplement for the October 29, 2026 Opus handoff

Extract ps2-native-loaded-20261010-032924.zip into:
  /workspace/ps2-native-input/

This creates exactly:
  loaded-20261010-032924/manifest.json
  loaded-20261010-032924/section-00-text.bin
  loaded-20261010-032924/section-01-rdata.bin
  loaded-20261010-032924/section-09-_RDATA.bin

ZIP SHA256:
  d5fc812680308e4ea9afae14620760fddbeb40d0a098e42621007b2607d8a72e
ZIP bytes: 23497344

The original manifest and section bytes are unchanged. Verify the sections
against their sizes and SHA256 values in manifest.json before executing checks.
This is the captured local-build input, not a fresh online-game capture.

From the latest source snapshot provided in the earlier handoff:
  gcc -O0 -fno-pie -no-pie validation/native_character_preview_check.c -o /tmp/ps2-preview-check
  python3 validation/native_character_preview_cases.py /tmp/ps2-preview-check /workspace/ps2-native-input/loaded-20261010-032924 /tmp/ps2-preview-results.json

The native check runs captured instructions in an isolated Linux address space;
it does not require attaching to the user's game. Read its fixture boundaries
and the main handoff before interpreting a pass as rendering or gameplay.

The original PlanetSide2_x64.exe is not part of this requested folder/archive.
The supplied native preview checker uses these sections and the manifest; other
PE/disassembly research scripts may additionally require the matching executable.
Do not substitute another executable or capture silently.
