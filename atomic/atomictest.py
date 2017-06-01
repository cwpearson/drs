import atomic
import numpy as np

a = np.array([0,1], dtype='int32')

print hex(a.__array_interface__['data'][0])
print hex(id(a))
print hex(a.ctypes.data)

print a
atomic.add_int32(a, 0, 5)
atomic.add_int32(a, 1, 2)
print a

atomic.add_int32(0,0,0)
