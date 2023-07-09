
def fractal():
    from sys import stdout
    from io import StringIO

    for y in range(-12,12):
        out=StringIO(100) # Line buffer to improve performance
        for x in range(-39,40):
            ca = x*0.0458
            cb = y*0.08333333
            a=ca
            b=cb
            hit=False
            for i in range(0,16):
               sa=a*a
               sb=b*b
               t= sa - sb + ca
               b= 2*a*b + cb
               a=t
               hit=(sa+sb)>4
               if hit:
                   out.write(chr((i+48+7) if i>9 else (i+48)))
                   break

            if not hit:
                out.write(' ')

        stdout.write(out.getvalue())
        stdout.write('\r\n')

def empty():
    from io import StringIO
    from sys import stdout

    for y in range(-12,12):
        out=StringIO(100)
        for x in range(-39,40):

            out.write('X')

        stdout.write(out.getvalue())
        stdout.write('\r\n')

