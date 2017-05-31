import struct
import numpy as np
import atomic

class DrsVectorDouble:
    def __init__(self, reg, off, cap):
        self.region = reg
        self.offset = off
        self.capacity = cap

    def __getitem__(self, key):
        off = (self.offset + key) * 8
        d, = struct.unpack('d', self.region[off : off + 8])
        return d

    def __setitem__(self, key, item):
        off = (self.offset + key) * 8
        struct.pack_into('d', self.region, off, item)

# array-like memmap using numpy
mm = np.memmap("foo", dtype='byte', mode='r+')

v = DrsVectorDouble(mm, 0, 1024 * 1024 / 8)

print v[0]
atomic.add_int(mm,0,2)
print v[0]

#for i in range(v.capacity):
#    print v[i]
#    v[i] += 1
#    print v[i]

# close the map
del mm
