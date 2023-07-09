def tohex(dw):
    b = dw.to_bytes(4,'big')
    return f'0x{b[0]:02x}{b[1]:02x}{b[2]:02x}{b[3]:02x}'
