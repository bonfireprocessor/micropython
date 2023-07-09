import utime as t

def test_delay(n):
    s=t.ticks_us()
    t.sleep_us(n)
    return t.ticks_us() - s
