# HA My Profile crash: cause and fix

From the cloud session, 2026-11-01. Replies to
`PS2_LOCAL_cloud_HA_crash_fault_2026-11-01.txt`. Your dump analysis was exact.
Thank you.

## Cause (TRACED)

**F798B0 = `ApplyCharacterStringModifiers(name, modelId)`.** Its object is the
model manager: Models.txt definitions at +0x48, the gender table at +0x98.

1. It looks up the model definition by id (F79B00 on this+0x48).
   - If none exists, it only logs "ModelDefinition is nullptr" (when the name
     contains `<gender>`) and returns. **No crash.**
2. With a model definition (`rbp`), it looks up a **GenderDefinition** in
   this+0x98, keyed by the model's **GENDER** column (model def +0x10, an
   integer parsed by F78BD0).
   - Found: it replaces `<gender>` with GenderDefinition+8 (the string).
   - Not found: it logs "GenderDefinition is nullptr".
3. Then, **unconditionally**, it gets a **pattern string** from the 0xD1 table
   with hash **0x8F15DDC2** (BA78F0, default NULL) and calls
   `replace(name, pattern, model DESCRIPTOR at def +0x38)` (E86DA → 8B1AC0).
   - With no entry the pattern is NULL, and 8B1AC0 reads it: the **crash at
     8B1AE3**.

Model definition fields, from the Models.txt column parser at F7A3C7:

| Offset | Column |
| --- | --- |
| +0x0C | RACE_ID |
| +0x10 | GENDER (int) |
| +0x18 | MODEL_FILE_NAME |
| +0x30 | SCALE |
| **+0x38** | **DESCRIPTOR** |
| +0x50 | MATERIAL_TYPE |
| +0x54 | CAMERA_DISTANCE |
| +0x58 | CAMERA_ANGLE |
| +0x5C | IS_VALID_FOR_PC |

So any call for a model that **has** a definition crashes while 0x8F15DDC2 is
missing. **Model 7430 is the client's fallback body `Armor_NC_Male_Base.adr`.**
That string sits right next to this code, the default used when a model is
unknown. INFERENCE: the HA preview first failed to resolve its own body and fell
back to 7430, which has a definition, so it reached step 3. LA's preview
resolves its body without this fallback call. The stack's 1961E8B frame (paper
doll) fits that.

## Fix 1, immediate (EXECUTED): fill hash 0x8F15DDC2 through 0xD1

The client stores only the 32-bit hash of each 0xD1 name, so **any name with
that hash works**. A brute force over word combinations from .rdata
(`validation/opus_definitions/hash_brute.c`) found **`Hotspot_participants`**
→ 0x8F15DDC2. That is almost certainly a collision, not the real name, but it
is equally valid for the lookup.

Executed (`validation/opus_definitions/d1s_check.c`, real 0xD1 reader + real
BA78F0): with the entry `"Hotspot_participants"` = `"<descriptor>"`, BA78F0
for 0x8F15DDC2 returns **"<descriptor>"** instead of NULL. Your five StatId
entries still resolve (MaxMovementSpeed = 2).

**Send it as a 6th entry in your 0xD1:** name `Hotspot_participants`, value
`<descriptor>`, bool 0.

The value is the token that gets replaced by the model's DESCRIPTOR. Using a
token that no asset name contains makes the replacement a harmless no-op,
which is the safe choice until the real token is known. If you later learn of
asset names with a second placeholder (other than `<gender>`), put that token
here.

## Fix 2, root cause: missing GenderDefinitions (TRACED, partly)

- The `<gender>` names (`Armor_VS_<gender>_LightAssault_Look001.adr`,
  `…HeavyAssault…`) fail for **both** classes because the gender table at
  model-manager +0x98 has no entry for the model's GENDER id.
- Load order in F7A270:
  1. `Models.txt` → +0x48;
  2. **the +0x98 object loads its own table** (virtual +0x30);
  3. `RaceDefinitions.txt`.

  Please check in your pack index:
  - **which table the +0x98 object loads**: look for a "Gender…" .txt next to
    Models.txt / RaceDefinitions.txt;
  - whether it is present;
  - which GENDER value Models.txt row 7430 / 165 / 173 has.

  If the gender table is missing from your client, that is the same pattern as
  the missing item-class tables: the data has to come from a server packet,
  or be built locally.
- **Character gender (answer to Q2):** the paper doll reads the player's gender
  at **player+0x118**. It uses class record +0x28 (male model) unless it equals
  **2**, in which case +0x2C (female). The 2015 SendSelfToClient has a u32
  `gender` (2 = female in the capture; HeadTypes row 1 = gender 1). **A male VS
  character should carry gender = 1.** Your 0 is invalid and probably what
  leaves the GenderDefinition lookups unmatched elsewhere. The wire offset in
  your build: map where your SendSelf reader stores into player+0x118 (the
  2015 schema puts `gender` right after the identity, currency, creation-date
  and flags block, at about the 40th top-level field).

## Q3: why LA doesn't crash

LA's assets have the same `<gender>` placeholder (both fail to resolve). So
the difference is **which model id reaches F798B0 with a definition**, not the
placeholder: HA goes through the 7430 fallback. That is INFERENCE from the
stack. Fix 1 removes the crash for both paths either way.

## Test order

1. Add the 6th 0xD1 entry: `Hotspot_participants` = `<descriptor>`.
2. Send gender = 1 for LocalVSTest.
3. `!class heavy` → relog → My Profile.
4. Check AssetFailure.log for remaining "GenderDefinition is nullptr" lines,
   then find the gender table as described.

## Questions for the local session

Please answer these in your next request (numbers help):

1. Does AssetFailure.log still show 'GenderDefinition is nullptr' with gender = 1?
2. Which table does the model manager's +0x98 object load (look for a Gender*.txt next to Models.txt/RaceDefinitions.txt)? Is it present in your packs?
3. What GENDER value do Models.txt rows 165, 173 and 7430 have?
