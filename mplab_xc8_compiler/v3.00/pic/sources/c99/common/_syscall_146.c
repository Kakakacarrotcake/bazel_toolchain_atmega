#include <stdlib.h>
#include <sys/uio.h>

long _syscall_146(long a, long b, long c, long d, long e)
{
    char *buf;
    int f, i, j, k, n;
    struct iovec *piov;
    extern void putch(char);

    f = (int)a;
    if (!((f == 1) || (f == 2))) {
        return 0;
    }

    n = (int)c;
    k = 0;
    i = 0;
    while (i < n) {
        piov = &((struct iovec *)b)[i];
        buf = (char *)(piov->iov_base);
        j = 0;
        while (j < piov->iov_len) {
            putch(buf[j]);
            ++j;
            ++k;
        }
        ++i;
    }
    
    return k;
}
