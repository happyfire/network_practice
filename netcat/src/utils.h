#ifndef _UTILS_H
#define _UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <syslog.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

extern int use_syslog;
extern int use_tty, use_tty_stderr;

void* safe_malloc(size_t size);
void safe_free(void *buf);

void ipv4_ntop(uint32_t ip, char* ipstr);

void server_backtrace(int sig);

void LOG_ERROR(const char *s);

#define TIME_FORMAT "%F %T"

#define IS_VERBOSE(v) (g_config.verbose>=(v))

#define CHECK_USE_TTY()                     \
do {                                        \
    use_tty = isatty(STDOUT_FILENO);        \
    use_tty_stderr = isatty(STDERR_FILENO); \
} while (0)

#define USE_SYSLOG(ident)                           \
do{                                                 \
    use_syslog = 1;                                 \
    openlog((ident), LOG_CONS | LOG_PID, LOG_USER); \
}while(0)

#define LOG(format, ...)                                                                        \
do{                                                                                             \
    if(use_syslog){                                                                             \
        syslog(LOG_INFO, format, ##__VA_ARGS__);                                                \
    } else {                                                                                    \
        time_t now = time(NULL);                                                                \
        char timestr[20];                                                                       \
        strftime(timestr, 20, TIME_FORMAT, localtime(&now));                                    \
        if(use_tty){                                                                            \
            fprintf(stdout, "\e[01;32m %s [INFO] \e[0m"format"\n", timestr, ##__VA_ARGS__);     \
        }else{                                                                                  \
            fprintf(stdout, "%s [INFO] "format"\n", timestr, ##__VA_ARGS__);                    \
            fflush(stdout);                                                                     \
        }                                                                                       \
    }                                                                                           \
}while(0)

#define LOGV(v, format, ...)                                                                    \
do{                                                                                             \
  if(g_config.verbose>=v){                                                                      \
    if(use_syslog){                                                                             \
        syslog(LOG_INFO, "[V%d]" format, v, ##__VA_ARGS__);                                     \
    } else {                                                                                    \
        time_t now = time(NULL);                                                                \
        char timestr[20];                                                                       \
        strftime(timestr, 20, TIME_FORMAT, localtime(&now));                                    \
        if(use_tty){                                                                            \
            fprintf(stdout, "\e[01;34m %s [V%d] \e[0m"format" \e[01;36m(%s:%d, %s)\e[0m\n", timestr, v,   \
                ##__VA_ARGS__, __FILE__, __LINE__ , __FUNCTION__);                              \
        }else{                                                                                  \
            fprintf(stdout, "%s [V%d] "format" (%s:%d, %s)\n", timestr, v,                      \
                ##__VA_ARGS__, __FILE__, __LINE__ , __FUNCTION__);                              \
            fflush(stdout);                                                                     \
        }                                                                                       \
    }                                                                                           \
  }                                                                                             \
}while(0)

#define LOGE(format, ...)                                                                       \
do{                                                                                             \
    if(use_syslog){                                                                             \
        syslog(LOG_ERR, "[ERR] "format, ##__VA_ARGS__);                                                 \
    } else {                                                                                    \
        time_t now = time(NULL);                                                                \
        char timestr[20];                                                                       \
        strftime(timestr, 20, TIME_FORMAT, localtime(&now));                                    \
        if(use_tty_stderr){                                                                     \
            fprintf(stderr, "\e[01;31m %s [ERR] \e[0m"format" \e[01;36m(%s:%d, %s)\e[0m\n",     \
                 timestr, ##__VA_ARGS__, __FILE__, __LINE__ , __FUNCTION__);                    \
        }else{                                                                                  \
            fprintf(stderr, "%s [ERR] "format" (%s:%d, %s)\n", timestr,                         \
                    ##__VA_ARGS__, __FILE__, __LINE__ , __FUNCTION__);                          \
            fflush(stderr);                                                                     \
        }                                                                                       \
    }                                                                                           \
}while(0)


#endif // _UTILS_H

