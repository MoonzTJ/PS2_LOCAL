import re,struct,json,sys
sys.path.insert(0,'.')
from lz4b import lz4_block
src=open('/home/user/ref/psemu_soe-network/lib/zonepackets.js').read()
blk=src[src.index('var baseItemDefinitionSchema = ['):]
blk=blk[:blk.index('];')]
# top-level fields only (bitflags inner flags skipped)
fields=[]; depth=0
for line in blk.split('\n')[1:]:
    m=re.search(r'name: "(\w+)",\s*type: "(\w+)"',line)
    if m and depth==0: fields.append(m.groups())
    depth+=line.count('[')-line.count(']')
raw=open('/home/user/ref/ps2-emu/data/zone/Command.ItemDefinitions.dat','rb').read()
print('opcode',raw[:3].hex(),'fields',len(fields))
o=3; L=struct.unpack_from('<I',raw,o)[0]; o+=4; d=raw[o:o+L]
n=struct.unpack_from('<I',d,0)[0]; p=4; items={}
for k in range(n):
    bs,bo=struct.unpack_from('<HH',d,p); blob=lz4_block(d[p+4:p+4+bs],bo); p+=4+bs
    q=0; rec={}
    for name,t in fields:
        if t in('uint32',): rec[name]=struct.unpack_from('<I',blob,q)[0]; q+=4
        elif t=='int32': rec[name]=struct.unpack_from('<i',blob,q)[0]; q+=4
        elif t in('uint8','bitflags','boolean'): rec[name]=blob[q]; q+=1
        elif t=='float': rec[name]=round(struct.unpack_from('<f',blob,q)[0],4); q+=4
        elif t=='string':
            sl=struct.unpack_from('<I',blob,q)[0]; q+=4; rec[name]=blob[q:q+sl].decode('latin1'); q+=sl
        else: raise SystemExit('type '+t)
    rec['_bytes_left']=len(blob)-q
    items[rec['itemId']]=rec
print('items',len(items),'ok' if p==len(d) else f'consumed {p}/{len(d)}')
lefts=set(r['_bytes_left'] for r in items.values()); print('leftover sizes',sorted(lefts)[:10])
json.dump(items,open('items2015.json','w'))
for iid in (44,19,21,44705,7):
    r=items.get(iid); print(iid, {k:r[k] for k in ('modelName','itemType','itemClass','param1','param2','param3','categoryId','nameId') if r and k in r} if r else None)
