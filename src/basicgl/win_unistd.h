#ifndef WIN_UNISTD
#define WIN_UNISTD

#ifdef WIN32

#include <time.h>

#define CLOCK_REALTIME                  0

typedef int clockid_t;
typedef __int64 useconds_t;

#ifdef __cplusplus
    extern "C" {
#endif
void usleep(useconds_t usec);
int clock_gettime(clockid_t type, struct timespec *spec);
#ifdef __cplusplus
    }
#endif

#endif

#endif
