import atomic
import numpy as np

a = np.array([0,1], dtype='int')

print a
atomic.add_int(a, 0, 5)
print a
