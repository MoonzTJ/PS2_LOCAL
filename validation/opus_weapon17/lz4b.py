def lz4_block(src, outsize):
    dst=bytearray(); i=0
    while i < len(src):
        t=src[i]; i+=1; l=t>>4
        if l==15:
            while True:
                b=src[i]; i+=1; l+=b
                if b!=255: break
        dst+=src[i:i+l]; i+=l
        if i>=len(src) or len(dst)>=outsize: break
        off=src[i]|(src[i+1]<<8); i+=2; m=t&15
        if m==15:
            while True:
                b=src[i]; i+=1; m+=b
                if b!=255: break
        m+=4; s=len(dst)-off
        for k in range(m): dst.append(dst[s+k])
    return bytes(dst)
