import subprocess,json,sys,os
N='/workspace/ps2-native-input/loaded-20261010-032924'
size=int(sys.argv[1]); dump=sys.argv[2]; kind=sys.argv[3] if len(sys.argv)>3 else '0'
def run(buf):
    open('rp.bin','wb').write(buf)
    o=subprocess.run(['./rep2',N,'rp.bin',kind,dump],capture_output=True,text=True,timeout=20)
    if o.returncode or not o.stdout.startswith('{'): return None
    return json.loads(o.stdout)
b1=run(bytes(size+600)); b2=run(bytes(size+600))
B=bytes.fromhex(b1['record']); B2=bytes.fromhex(b2['record'])
noise={i for i in range(len(B)) if B[i]!=B2[i]}
print('consumed',b1['consumed'],'noise',len(noise))
cur=None
def pr(c): print(f"wire {c['p']:3d} w{c['w']:2d} {c['f']:14s} -> {hex(c['r']) if c['r'] is not None else '-'} {c['o'][:5] if c['w']==1 and len(c['o'])>1 else ''}")
for p in range(size):
    b=bytearray(size+600); b[p]=0x5A; r=run(bytes(b))
    if r is None: f,o='CRASH',[]
    else:
        R=bytes.fromhex(r['record']); o=[hex(i) for i in range(len(R)) if R[i]!=B[i] and i not in noise]
        f=('LEN %d e%d'%(r['consumed'],r['parse_error'])) if r['consumed']!=b1['consumed'] or r['parse_error'] else ''
    first=int(o[0],16) if o else None
    if cur and not f and not cur['f'] and first is not None and cur['r'] is not None and first==cur['r']+(p-cur['p']): cur['w']+=1; continue
    if cur: pr(cur)
    cur=dict(p=p,w=1,f=f,r=first,o=o)
pr(cur)
