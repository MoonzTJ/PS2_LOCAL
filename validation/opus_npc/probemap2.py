import subprocess,json,sys,os
N='/workspace/ps2-native-input/loaded-20261010-032924'
fn,size,rs=sys.argv[1],int(sys.argv[2]),sys.argv[3]
env=dict(os.environ)
def run(buf):
    open('p2.bin','wb').write(buf)
    o=subprocess.run([os.environ.get('PROBE','./probe2'),N,fn,rs,'p2.bin'],capture_output=True,text=True,timeout=20,env=env)
    if o.returncode: return None
    return json.loads(o.stdout)
b1=run(bytes(size+600)); b2=run(bytes(size+600))
B=bytes.fromhex(b1['record']); B2=bytes.fromhex(b2['record'])
noise=set(i for i in range(len(B)) if B[i]!=B2[i])
print('baseline consumed',b1['consumed'],'noise bytes',len(noise))
for p in range(size):
    b=bytearray(size+600); b[p]=0x5A
    r=run(bytes(b))
    if r is None: print(p,'CRASH'); continue
    R=bytes.fromhex(r['record']); d=[i for i in range(len(R)) if R[i]!=B[i] and i not in noise]
    print(p, ('LEN %d err %d'%(r['consumed'],r['parse_error'])) if r['consumed']!=b1['consumed'] or r['parse_error'] else '', [hex(x) for x in d[:8]])
