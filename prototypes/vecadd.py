import struct
import numpy as np
import atomic
from time import sleep

class DrsAtomicInt32:
    def __init__(self, reg, off):
        self.region = reg
        self.offset = off

    def increment(self, inc):
        idx = self.offset / 4
        return atomic.add_int32(self.region, idx, inc)

    def set(self, val):
        struct.pack_into('i', self.region, self.offset, val)

    def get(self):
        return struct.unpack('i', self.region[self.offset : self.offset + 4])

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

VEC_EXTENT = 1024 * 1024

offset = 0
todo = DrsAtomicInt32(mm, offset)
offset += 4
complete = DrsAtomicInt32(mm, offset) # set when Python is done
offset += 4
go = DrsAtomicInt32(mm, offset) # wait for permission to start
offset += 4
a = DrsVectorDouble(mm, offset, VEC_EXTENT)
offset += a.capacity * 8
b = DrsVectorDouble(mm, offset, VEC_EXTENT)
offset += b.capacity * 8
c = DrsVectorDouble(mm, offset, VEC_EXTENT)
offset += c.capacity * 8

print "waiting...."
while go.get() == 0:
    continue
print "go"

my_work = 0
BS = 10000
while True:
    start = todo.increment(BS)
    if start >= c.capacity:
        break
    print "Python claimed", start, start + BS - 1
    for i in range(start, min(start + BS, c.capacity)):
        c[i] = a[i] + b[i]
    my_work += BS
    # sleep(0.05)

complete.set(1)
# close the map
del mm

print my_work