import struct
def s(x): b=x.encode(); return struct.pack('<I',len(b))+b
def weapon(group=24,flags=0,times=(550,250,150,150,125,150,300),ammo=((1,30,210),),fgs=(15,1015)):
    w=struct.pack('<IB',group,flags)+struct.pack('<7H',*times)
    w+=struct.pack('<Iff',0,1.0,1.0)+struct.pack('<H',0)   # nextUseDelay, turn, move, heatBleed (14 bytes at 19)
    w+=struct.pack('<H',0)                                 # overheatPenalty (33)
    w+=struct.pack('<Iff',674,0.0,0.0)                     # rangeStringId + melee w/h (35, 12 bytes)
    w+=s('Rifle')                                          # 47 animation set
    w+=struct.pack('<4H',0,0,0,0)                          # 51..58
    w+=struct.pack('<Iff',0,-85.0,85.0)+struct.pack('<II',0,0)  # 59..78 (20 bytes)
    w+=struct.pack('<I',len(ammo))+b''.join(struct.pack('<IHIBB',a,c,cap,0,0)+s('')+s('')+s('') for a,c,cap in ammo)
    w+=struct.pack('<I',len(fgs))+b''.join(struct.pack('<I',g) for g in fgs)
    w+=struct.pack('<H',0)+struct.pack('<IIff',0,0,0.0,0.0)
    return w
def group(modes,flags=8,transition=500):
    return struct.pack('<I',len(modes))+b''.join(struct.pack('<I',m) for m in modes)+struct.pack('<BHIHBBHHH',flags,0,0,transition,0,0,0,0,0)
def mode(): return bytes(330)
def coll(entries): return struct.pack('<I',len(entries))+b''.join(struct.pack('<I',k)+v for k,v in entries)
def body(weapons, groups, modes):
    return coll(weapons)+coll(groups)+coll(modes)+coll([])+coll([])+coll([])
