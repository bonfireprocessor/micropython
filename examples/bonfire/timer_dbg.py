import bonfire as b


def cb(flag):
    if flag:
    	b.breakpoint()
    print("callback")
    

from machine import Timer


def run(flag):
   from time import sleep
   from sys import stdin
   import select

   t=Timer(-1)
   t.init(period=1000, callback=lambda t: cb(flag))

   p=select.poll()
   p.register(stdin,select.POLLIN)
   print("Wait for keypress")
   print(p.poll())
   stdin.read(1)
   
   t.deinit()
   print("Done")	

