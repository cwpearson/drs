import atomic
import numpy as np

a = np.array([0,1], dtype='int32')

ptr, readonly = a.__array_interface__['data']
print hex(ptr)
print hex(id(a))

print a
atomic.add_int32(a, 0, 5)
atomic.add_int32(a, 1, 2)
print a

atomic.add_int32(0,0,0)
