#ifndef DL_LOGGING_H
#define DL_LOGGING_H 1

#include "dl.h"

#if !DL_IS_ATLEAST_C99
#  error "DL_LOGGING is not available for C90 or lower."
#endif

#include <stdio.h>
#include <stdarg.h>



#ifdef __cplusplus
extern "C" {
#endif
  
  typedef enum {
    DL_LOG_INFO = 1,
    DL_LOG_WARNING = 2,
    DL_LOG_ERROR = 3,
    DL_LOG_TEST = 4,
    DL_LOG_MESSAGE = 5
  } dl_log_channel;

  extern dl_natural (*dl_active_log_handler)(dl_log_channel, const char *, dl_natural, const char *, const char *);

  dl_natural dl_log_message(dl_log_channel ch, const char *file, dl_natural line, const char *function, const char *fmt, ...);

# if DL_IS_GNUC || DL_IS_CLANG || DL_IS_MSC || DL_IS_MINGW
#   define DL_INFO(...) dl_log_message(DL_LOG_INFO, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define DL_WARN(...) dl_log_message(DL_LOG_WARNING, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define DL_ERROR(...) dl_log_message(DL_LOG_WARNING, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define DL_TEST(...) dl_log_message(DL_LOG_TEST, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
#   define DL_MSG(...) dl_log_message(DL_LOG_MESSAGE, __FILE__, __LINE__, __func__, ## __VA_ARGS__)
# else
#   define DL_INFO(...) dl_log_message(DL_LOG_INFO, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define DL_WARN(...) dl_log_message(DL_LOG_WARNING, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define DL_ERROR(...) dl_log_message(DL_LOG_WARNING, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define DL_TEST(...) dl_log_message(DL_LOG_TEST, __FILE__, __LINE__, "", ## __VA_ARGS__)
#   define DL_MSG(...) dl_log_message(DL_LOG_MESSAGE, __FILE__, __LINE__, "", ## __VA_ARGS__)
# endif

# if DL_USE_SAFETY_CHECKS
#   undef dl_safety
#   define dl_safety(x) (dl_unlikely(x) ? DL_ERROR("Safety triggered") || 1 : 0)
# endif

#ifdef __cplusplus
}
#endif



#if DL_IMPLEMENTATION

dl_natural _default_log_handler(dl_log_channel ch, const char *file, dl_natural line, const char *function, const char *msg) {
  char time_buf[20];

#if defined(DL_TIME_H)
  dl_write_time("%F %T", time_buf, 20);
#else
  time_buf[0] = (char)0;
#endif

  switch (ch) {
  case DL_LOG_INFO:
    fprintf(stdout, "%s:%lu:%s\n[LOG %s] %s\n", file, (unsigned long)line, function, time_buf, msg);
    return ch;
  case DL_LOG_WARNING:
    fprintf(stdout, "%s:%lu:%s\n[WRN %s] %s\n", file, (unsigned long)line, function, time_buf, msg);
    return ch;
  case DL_LOG_ERROR:
    fprintf(stderr, "%s:%lu:%s\n[ERR %s] %s\n", file, (unsigned long)line, function, time_buf, msg);
    return ch;
  case DL_LOG_TEST:
    fprintf(stdout, "[TST %s] %s\n", time_buf, msg);
    return ch;
  case DL_LOG_MESSAGE:
    fprintf(stdout, "[MSG %s] %s\n", time_buf, msg);
    return ch;
  default:
    return 0;
  }
}

dl_natural (*dl_active_log_handler)(dl_log_channel ch, const char *, dl_natural, const char *, const char *) = _default_log_handler;

dl_natural dl_log_message(dl_log_channel ch, const char *file, dl_natural line, const char *function, const char *fmt, ...) {
  char buf[256];
  va_list args1, args2;
  va_start(args1, fmt);
  va_copy(args2, args1);
  dl_natural count = 1 + vsnprintf(NULL, 0, fmt, args1);
  count = count > 254 ? 254 : count;
  va_end(args1);
  vsnprintf(buf, count, fmt, args2);
  buf[count + 1] = 0;
  va_end(args2);

  return dl_active_log_handler(ch, file, line, function, buf);
}

#endif /* DL_IMPLEMENTATION */

#endif
