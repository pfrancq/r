#ifndef __SYS_TIME_H__
#define __SYS_TIME_H__
#include <Winsock2.h> //for timeval struct (crazy isn't it?)
#include <time.h>

#define asctime_r(a, b) asctime_s((b),(a))

int gettimeofday(struct timeval *tp, void *tzp);

#endif // __SYS_TIME_H__