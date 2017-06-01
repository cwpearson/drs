import struct
import numpy as np
import atomic

class DrsAtomicInt32:
    def __init__(self, reg, off, val):
        self.region = reg
        self.offset = off
        struct.pack_into('i', self.region, self.offset, val)

    def increment(self, inc):
        idx = self.offset / 4
        return atomic.add_int32(self.region, idx, inc)

class DrsVectorDouble:
    def __init__(self, reg, off, cap):
        self.region = reg
        self.offset = off
        self.capacity = cap

    def __getitem__(self, idx):
        off = self.offset + idx * 8
        d, = struct.unpack('d', self.region[off : off + 8])
        return d

    def __setitem__(self, idx, item):
        off = self.offset + idx * 8
        struct.pack_into('d', self.region, off, item)

# array-like memmap using numpy
mm = np.memmap("foo", dtype='byte', mode='r+')

offset = 0
i32 = DrsAtomicInt32(mm, 0, 0)
offset += 4
a = DrsVectorDouble(mm, offset, 1024 * 1024)
offset += a.capacity * 8
b = DrsVectorDouble(mm, offset, 1024 * 1024)
offset += b.capacity * 8
c = DrsVectorDouble(mm, offset, 1024 * 1024)
offset += c.capacity * 8

BS = 10
while True:
    start = i32.increment(BS)
    if start >= c.capacity:
        break
    print "Python @", start
    for i in range(start, min(BS, c.capacity)):
        c[i] = a[i] + b[i]

# close the map
print "Python done!"
del mm
