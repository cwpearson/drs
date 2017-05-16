import mmap
import time

PATH="foo"
SIZE=4096

# write a simple example file
with open(PATH, "wb") as f:
    f.truncate(SIZE)


with open(PATH, "r+b") as f:
    # memory-map the file, size 0 means whole file
    mm = mmap.mmap(f.fileno(), 0)

    while True:
        print mm[0]
        time.sleep(1)
    # read content via standard file methods
    # print mm.readline()  # prints "Hello Python!"
    # # read content via slice notation
    # print mm[:5]  # prints "Hello"
    # # update content using slice notation;
    # # note that new content must have same size
    # mm[6:] = " world!\n"
    # # ... and read again using standard file methods
    # mm.seek(0)
    # print mm.readline()  # prints "Hello  world!"
    # close the map
    mm.close()
    