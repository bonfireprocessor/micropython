def mountroot():
    import uos as o
    import bonfire as b

    o.mount(o.VfsLfs2(b.Flash()),"/")
    # f=b.Flash()
    # fs=o.VfsLfs2(f)
    # o.mount(fs,"/")

print("_boot.py")
mountroot()