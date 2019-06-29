#ifndef DL_TIME_H
#define DL_TIME_H 1

#include "dl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

# if DL_IS_LINUX || DL_IS_APPLE
#   include <time.h>
#   include <sys/time.h>
# elif DL_IS_WINDOWS
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#   include <stdint.h>
# endif

  dl_api dl_natural dl_write_time(const char *fmt, char *buf, dl_natural buf_length);
  dl_api void dl_time(dl_natural *out_usec, dl_natural *out_sec);
  
#ifdef __cplusplus
}
#endif



#if DL_IMPLEMENTATION

dl_api dl_natural dl_write_time(const char *fmt, char *buf, dl_natural buf_length)
{
# if DL_IS_MSC
  struct tm ltime;
  time_t t = time(NULL);
  localtime_s(&ltime, &t);
  return strftime(buf, buf_length, fmt, &ltime);
# elif DL_IS_LINUX || DL_IS_APPLE
  time_t t = time(NULL);
  return strftime(buf, buf_length, fmt, localtime(&t));
# else
  return 0;
# endif
}

dl_api void dl_time(dl_natural *out_usec, dl_natural *out_sec)
{
# if DL_IS_WINDOWS
  static const uint64_t epoch = ((uint64_t)116444736000000000ULL);
  
  SYSTEMTIME system_time;
  FILETIME file_time;
  uint64_t time;
  long tv_sec, tv_usec;

  GetSystemTime(&system_time);
  SystemTimeToFileTime(&system_time, &file_time);
  time = ((uint64_t)file_time.dwLowDateTime);
  time += ((uint64_t)file_time.dwHighDateTime) << 32;

  *out_sec = (dl_natural)((time - epoch) / 10000000L);
  *out_usec = (dl_natural)(system_time.wMilliseconds * 1000);
# elif DL_IS_LINUX || DL_IS_APPLE
  struct timeval t1;
  gettimeofday(&t1, NULL);

  *out_sec = (dl_natural)t1.tv_sec;
  *out_usec = (dl_natural)t1.tv_usec;
# else
  *out_sec = 0;
  *out_usec = 0;
# endif
}

#endif /* DL_IMPLEMENTATION */

#endif
