#include <stdint.h>
#include <stdio.h>
#include <windows.h>
typedef char		my_bool;
#define __attribute__(A)
#include <mysql/my_pthread.h>

#define pthread_join(A,B) \
  ((WaitForSingleObject((A), INFINITE) != WAIT_OBJECT_0) || !CloseHandle(A))