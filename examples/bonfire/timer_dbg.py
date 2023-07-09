import bonfire as b


def cb(flag):
    if flag:
    	b.breakpoint()
    print("callback")
    

from machine import Timer


def run(flag):
   from time import sleep

   t=Timer(-1)
   t.init(period=1000, callback=lambda t: cb(flag))

   for i in range(1000000):
	pass

   t.deinit()
   print("Done")	

