import subprocess,json,sys
import os; N=os.environ.get('PS2_NATIVE','/workspace/ps2-native-input/loaded-20261010-032924')
fn,size,rs=sys.argv[1],int(sys.argv[2]),sys.argv[3]
def run(buf):
    open('p.bin','wb').write(buf)
    o=subprocess.run(['./probe',N,fn,rs,'p.bin'],capture_output=True,text=True,timeout=20)
    if o.returncode: return None
    return json.loads(o.stdout)
base=run(bytes(size+600)); B=bytes.fromhex(base['record'])
res=[]
for p in range(size):
    b=bytearray(size+600); b[p]=0x5A
    r=run(bytes(b))
    if r is None: res.append((p,'CRASH',None)); continue
    R=bytes.fromhex(r['record']); d=[i for i in range(len(R)) if R[i]!=B[i]]
    res.append((p, 'LEN' if r['consumed']!=base['consumed'] or r['parse_error'] else '', d[:6], r['consumed']))
# group into fields
fields=[];cur=None
for p,kind,d,*c in res:
    tgt=d[0] if d else None
    if cur and kind=='' and cur['kind']=='' and tgt is not None and cur['dst'] is not None and tgt==cur['dst']+(p-cur['p0']) :
        cur['w']+=1; continue
    cur=dict(p0=p,w=1,kind=kind,dst=tgt,extra=d,cons=c); fields.append(cur)
for f in fields: print(f"in {f['p0']:3d} w{f['w']} {f['kind']:5} -> rec {hex(f['dst']) if f['dst'] is not None else '-'} {f['extra'] if f['w']==1 else ''} {f['cons'] if f['kind'] else ''}")
