
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#include <cstdio>

const char *PATH = "foo";
const size_t SIZE = 4096;

int main()
{
    int fd;
    fd = open(PATH, O_RDWR);
    if (fd == -1 && errno == ENOENT) // file does not exist. create
    {

        fprintf(stderr, "creating\n");
        fd = open(PATH, O_RDWR | O_CREAT, 0666);
        if (fd == -1)
        {
            fprintf(stderr, "create failed: %s\n", strerror(errno));
            return -1;
        }
        ftruncate(fd, SIZE); // resize
    }
    else if (fd == -1)
    {
        fprintf(stderr, "open failed: %s\n", strerror(errno));
        return -1;
    }
    fprintf(stderr, "already exists\n");

    const int pagesize = getpagesize();
    const int offset = 0;
    char *data = (char *)mmap((caddr_t)0, pagesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
    if (data == (caddr_t)(-1))
    {
        fprintf(stderr, "mmap failed: %s\n", strerror(errno));
        return -1;
    }
}