def _mountroot():
    import uos as o
    import bonfire as b

    o.mount(o.VfsLfs2(b.Flash(),readsize=16,progsize=16,mtime=False),"/")
    
#print("_boot.py")
_mountroot()