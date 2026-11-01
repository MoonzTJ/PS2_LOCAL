"""Builders for packets traced on 2026-11-01 (see CLOUD_CLASS_SWITCH_HEALTH_2026-11-01.md).

remove_player(guid)            -> PlayerUpdate 0F 01, 12 bytes (executed through 99C230)
resource_set(char_guid, ...)   -> ResourceEvent 0x8D type 2, 67 bytes (executed through C52130)
activate_profile(profile, ...) -> ClientUpdate 11 15 00 (executed through 98BD00)
"""
import struct


def s(x=b''):
    return struct.pack('<I', len(x)) + x


def remove_player(guid):
    return bytes([0x0F, 0x01]) + struct.pack('<HQ', 0, guid)


def resource_set(char_guid, resource_id=1, resource_type=1, max_value=1000,
                 value=1000, game_time=0):
    head = bytes([0x8D]) + struct.pack('<I', game_time) + bytes([2]) + struct.pack('<Q', char_guid)
    body = struct.pack('<H', resource_id) + bytes([resource_type])
    body += struct.pack('<I', 0)                       # array count (9B0C10), empty
    body += struct.pack('<IIff', max_value, value, 0.0, 0.0)
    body += struct.pack('<I', 0) + struct.pack('<I', 0)  # two more counts, empty
    body += struct.pack('<5I', 0, 0, 0, 0, 0) + bytes(2)
    return head + body


def activate_profile(profile_bytes, attachments=(), profile_word=0, word2=0,
                     model_id=0, word4=0, str1=b'', str2=b'', last=0):
    """profile_bytes: one profile record as written in SendSelf's profile list
    (reader 98A3D0; 102 bytes when all-zero). attachments: tuples of
    (model_name, str2, tint_alias, str4, u32a, u32b, u32c, slot_id)."""
    b = bytes([0x11]) + struct.pack('<H', 0x15) + s(profile_bytes)
    b += struct.pack('<I', len(attachments))
    for a in attachments:
        b += s(a[0]) + s(a[1]) + s(a[2]) + s(a[3]) + struct.pack('<4I', *a[4:8])
    # wire order of the four u32: record +0x44, +0x40, +0x48 (model), +0x4C
    b += struct.pack('<4I', profile_word, word2, model_id, word4)
    b += s(str1) + s(str2) + bytes([last])
    return b


if __name__ == '__main__':
    print('remove', len(remove_player(0x4000000000000001)))
    print('health', len(resource_set(0x4000000000000001)))
    print('activate(empty profile)', len(activate_profile(bytes(102), model_id=165)))


# ---- 2026-11-01 (CLOUD_FIRE_PATH_SWITCH): firing and weapon switching ----

def set_current_slot(slot_id, type_byte=1, mode=0):
    """Loadout 86 06 SetCurrentSlot (handler D2B490): opcode, sub, type, mode, u32 slot.
    mode 0 sets the active slot (loadout manager +0x250) and switches to it, but
    only if slot_id is in the current loadout's slot list (+0x820)."""
    return bytes([0x86, 0x06, type_byte, mode]) + struct.pack('<I', slot_id)


def projectile_mapping_element(fire_mode_id, ammo_id, projectile_id):
    """5th WeaponDefinitions collection element (9910C0, 12 bytes). Collection:
    u32 count, then {u32 key = fire_mode_id, element}. Field order is INFERENCE."""
    return struct.pack('<3I', fire_mode_id, ammo_id, projectile_id)


def projectile_record(pid, model=b'', fp_model=b'', speed=600.0, lifespan=2.0,
                      max_speed=600.0, gravity=0.0, damage=0.0):
    """One ProjectileDefinitions record (reader 99ECE0). The record starts with
    its own id; there is no separate collection key. 176 bytes with empty
    strings. Names follow H1Z1's order where the widths match (INFERENCE)."""
    b = struct.pack('<I', pid) + bytes([0, 0]) + s(model) + s(fp_model)
    # 25-byte block: audio, SPEED, velocity inherit, acceleration, flight type,
    # projectile effect, land effect
    b += struct.pack('<I', 0) + struct.pack('<fff', speed, 1.0, 0.0) + bytes([0])
    b += struct.pack('<II', 0, 0)
    # 40-byte block: LIFESPAN, max speed, turn rate, drag, GRAVITY, damage,
    # 4 tracer fields
    b += struct.pack('<6f', lifespan, max_speed, 0.0, 0.0, gravity, damage)
    b += struct.pack('<4I', 0, 0, 0, 0)
    # rest: one string, then fixed fields, one counted array (empty), 28 bytes
    b += s() + bytes(13 + 5 + 13 + 6 + 2 + 16 + 4 + 2) + struct.pack('<I', 0) + bytes(28)
    return b


def projectile_definitions_body(records):
    """Body for ReferenceData (0x17) type 'ProjectileDefinitions', before the
    same compression/envelope you use for WeaponDefinitions."""
    return struct.pack('<I', len(records)) + b''.join(records)
