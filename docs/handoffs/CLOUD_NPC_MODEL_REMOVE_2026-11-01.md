# NPC body model, remove packet

From the cloud session, 2026-11-01. This replies to
`PS2_LOCAL_cloud_npc_spawn_result_2026-11-01.txt` (v61). It is static analysis
plus harness runs on `loaded-20261010-032924`.

Tags: **TRACED** = read in the client code; **EXECUTED** = run in the harness;
**INFERENCE** = my reading, not proven.

The NPCs spawning and staying is a big step. The headless NC body explains
itself below.

## 1. Which field selects the body: **+0x34, not +0x38** (TRACED)

The handler A50570 creates a spawn-parameter struct (constructor 9D2C80) and
fills it from the packet record with AD9620:

1. **record +0x34** → model manager lookup (engine+0xB098, F79C70):
   - **If found:** the model's file name (model definition +0x18) is copied
     in as the body .adr (params+0xE0), and its scale (+0x30) is applied.
   - **If not found:** params+0xE0 keeps a built-in default name, loaded from
     a global just before. With +0x34 = 0, which is what you send, you get
     that default: the **headless NC soldier**.
   - +0x34 is also copied to params+0x544.
2. **record +0x38** → the same lookup, **only if > 0**. Its file name goes to a
   separate slot (params+0x578). That fits a **head (or second attachment)
   model**.

**My earlier table had these two swapped. Sorry about that.** The fix:
- put the **body model in wire 24–27** (+0x34);
- put a **head model id, or 0, in wire 28–31** (+0x38).

`validation/opus_npc/build_npc55.py` now takes body, faction and head:
`build_npc55.py 165 2 <headModelId>`. It still parses through 998560 at
181 bytes, error 0, and reads back (165, head). (EXECUTED)

For the soldier target:
- body **165**;
- head = the Models.txt id whose file is `Head_Male_Caucasian.adr`. I don't
  have Models.txt here; please look it up.

If the head doesn't attach, try 0 and see whether the body alone renders.

## 2. Strings, display name, faction (TRACED moves; meanings INFERENCE)

AD9620 copies the record into the spawn parameters as follows:

| Record | Goes to params | Notes |
| --- | --- | --- |
| +0x30 u8 | +0xD8 (as a dword) | still most likely the faction: the h1z1 order puts factionId here. The NC look came from the default body, not from this byte, so try 1 once the body is right |
| +0x18 guid | +0x00 | |
| +0x20 transientId | +0x08 | |
| +0x40 scale | +0x530 | |
| +0x50, +0x68 strings | +0x340, +0x3D8 | h1z1 calls the first "texture"; leave empty |
| +0x98 | +0x508 | |
| +0x9C position, +0xB0 vector | +0x510, +0x520 | |
| +0xD0 | +0x540 | |
| +0xD8, +0xF0, +0x108 strings | +0x178, +0x210, +0x2A8 | the actor factory C14ED0 copies the last two onto the actor |
| +0x124, +0x128 (npcDefinitionId) | +0x550, +0x554 | |
| +0x138 | +0x568 | |
| +0x140 guid | +0x548 | |
| +0x1A0 | +0x574 | |

- **The head is a model id (+0x38), not a string.**
- **The display name:** I haven't pinned it. The candidates are the text id at
  +0x2C, or the +0xD8 string.

  Try "Target" in +0xD8 with +0x2C = 0. If no nameplate appears, send a
  locale text id in +0x2C instead.
- **Bind pose:** probably the default body having no animation set. Check it
  again once +0x34 points at a real model. If it persists, the animation likely
  comes from npcDefinitionId or a later packet. (INFERENCE)

## 3. Remove / despawn: `0F 01` (EXECUTED reader, TRACED handler)

PlayerUpdate sub **0x01** (table A57028, case A527E4) reads with 99C230:

| Wire | Field |
| --- | --- |
| u8 `0F`, u8 `01` | opcode, sub |
| **u16** | **variant: 0 = remove** |
| **u64** | **guid** |

- That is **12 bytes**, the same as soe-network's `PlayerUpdate.RemovePlayer`
  (0F 01 00 00 + guid).
- I ran it with guid 0x4000000000000001: consumed 12, parse error 0, variant
  0, guid read back exactly.
- The handler takes the variant-0 branch (A52B61) for that.
- Variant 1 reads more fields, like soe's "RemovePlayerGracefully" (guid,
  bool, 5 × u32). It isn't needed for despawning a target.

## 4. Not done yet

1. **Health / damage components (0x57 Create class ids and payloads)**: not
   traced.
2. **Class switch without relog**: not traced.
   - In soe-network the 2015 path is Loadout / ActivateProfile, a ClientUpdate
     family sub.
   - I can look for the client handler that changes the current profile in
     place (it must reload the class record, model words and equipment)
     after the body/head test.

Tell me which of the two comes first.

## Questions for the local session

1. With **body 165 in +0x34** (and head 0), does the target show the VS LA
   body? Is it still in the bind pose?
2. Then with the Head_Male_Caucasian model id in +0x38: does a head appear?
3. Faction byte 1 vs 2: does anything visible change (nameplate colour,
   hostility)?
4. Does `0F 01 00 00 <guid>` remove the target?
5. Next: health/damage components first, or class switch without relog?
