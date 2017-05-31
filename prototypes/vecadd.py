import mmap
import struct

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
        d = struct.pack('d', item)
        self.region[off : off + 8] = d



with open("foo", "r+b") as f:
    # memory-map the file, size 0 means whole file
    mm = mmap.mmap(f.fileno(), 0)

    v = DrsVectorDouble(mm, 0, 1024 * 1024 / 8)


    for i in range(v.capacity):
        print v[i]

    # close the map
    mm.close()
    f.close()
