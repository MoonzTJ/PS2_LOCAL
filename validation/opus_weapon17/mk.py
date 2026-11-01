import sys,struct
sys.path.insert(0,'..'); sys.path.insert(0,'../..')
import build
from native_weapon_refresh_cases import base_wire
from native_weapon_init_cases import group_detail
W=[(17,build.weapon())]; G=[(15,build.group([15,115])),(1015,build.group([1015,1115]))]
def fm(ammo_slot=0, ammo_item=1):
    r=bytearray(330); struct.pack_into('<I',r,5,ammo_item); r[9]=ammo_slot; return bytes(r)
def wire(modes=True):
    M=[(m,fm()+struct.pack('<I',m)) for m in (15,115,1015,1115)] if modes else []
    return build.coll(W)+build.coll(G)+build.coll(M)+build.coll([])*3
def item(detail=b''): return base_wire([44,0,0,0,0,0,0,0],0x1234567887654321)+detail
def case(name, w, d):
    open(name,'wb').write(struct.pack('<I',len(w))+w+item(d))
case('c_nomodes.bin', wire(False), b'')
case('c_full.bin', wire(), b'')
mode=(0,0,143,0)
cap = group_detail(((15,2,(mode,mode)),(1015,2,(mode,mode))), ((30,180),(1,3),(1,1),(1,1)), selected=0)
case('c_detail.bin', wire(), cap)
cap_m1 = group_detail(((15,2,(mode,mode)),(1015,2,(mode,mode))), ((30,180),), selected=-1)
case('c_detail_m1.bin', wire(), cap_m1)
