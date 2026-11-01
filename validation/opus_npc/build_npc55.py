"""Build a PlayerUpdate 0F 55 (AddLightweightNpc) body for this client build.

Layout executed through the client's reader 998560 (see
docs/handoffs/CLOUD_VR_ENTITIES_FIREMODES_2026-11-01.md). Prints hex.
Usage: python3 build_npc55.py [model] [faction_byte]
"""
import struct, sys


def s(x=b''):
    return struct.pack('<I', len(x)) + x


def packed_uint(v):
    # 2-bit length prefix in the low bits (reader 966990)
    for n in range(4):
        if v < (1 << (8 * (n + 1) - 2)):
            return ((v << 2) | n).to_bytes(n + 1, 'little')
    raise ValueError(v)


def build(guid=0x4000000000000001, tid=100, model=165, faction=2,
          pos=(586.6, 467.2, -212.2), rot=(0, 0, 0, 1), scale=(1, 1, 1, 1)):
    b = bytes([0x0F, 0x55]) + struct.pack('<Q', guid) + packed_uint(tid)
    b += bytes(8) + struct.pack('<I', 0) + bytes([faction])
    b += struct.pack('<II', 0, model) + struct.pack('<4f', *scale)
    b += s() + s() + struct.pack('<I', 0) + struct.pack('<3f', *pos)
    b += struct.pack('<4f', 0, 0, 0, 0) + struct.pack('<4f', *rot) + struct.pack('<I', 0)
    b += s() + s() + s() + struct.pack('<3I', 0, 0, 0) + bytes([0])
    b += struct.pack('<I', 0) + bytes(3) + struct.pack('<I', 0)
    b += struct.pack('<Q', 0) + struct.pack('<Q', 0)
    b += struct.pack('<3IfI', 0, 0, 0, 0.0, 0)
    return b


if __name__ == '__main__':
    m = int(sys.argv[1]) if len(sys.argv) > 1 else 165
    f = int(sys.argv[2]) if len(sys.argv) > 2 else 2
    out = build(model=m, faction=f)
    print(len(out), out.hex())
