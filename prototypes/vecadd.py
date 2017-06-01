import struct
import mmap
import numpy as np
import atomic
from time import sleep
import posix_ipc

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
shmem = posix_ipc.SharedMemory("/foo")
mm = mmap.mmap(shmem.fd, 0)
region = np.ndarray(shape=(shmem.size),
                            dtype='byte',
                            buffer=mm,
                            offset=0,
                            strides=(1),
                            order='C')
# mm = np.memmap(shmem.fd, dtype='byte', mode='r+')

VEC_EXTENT = 1024 * 1024

offset = 0
todo = DrsAtomicInt32(region, offset)
offset += 4
complete = DrsAtomicInt32(region, offset) # set when Python is done
offset += 4
go = DrsAtomicInt32(region, offset) # wait for permission to start
offset += 4
a = DrsVectorDouble(region, offset, VEC_EXTENT)
offset += a.capacity * 8
b = DrsVectorDouble(region, offset, VEC_EXTENT)
offset += b.capacity * 8
c = DrsVectorDouble(region, offset, VEC_EXTENT)
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

del region
shmem.unlink()

print my_work