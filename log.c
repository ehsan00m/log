#include <stdio.h>
#include "log.h"

struct callback
{
    log_writeFn fn;
    log_logLevel_t level; /**< callback log level seperated from system loglevel*/
};

struct log
{
    log_logLevel_t system_log_level;
    log_loggingState_t enable;
    struct callback cb;
};

/*set initial values*/
static struct log log_settings =
{
    .system_log_level = LOG_LEVEL_INFO,
    .enable = LOG_ENABLE,
    .cb.fn = NULL,
    .cb.level = LOG_LEVEL_INFO
};

static const char * log_level_strings[] ={"DEBUG",
                                          "INFO",
                                          "WARNING",
                                          "ERROR",
                                          "CRITICAL"};


const char * log_getLevelString (log_logLevel_t level)
{
    switch (level)
    {
    case LOG_LEVEL_DEBUG:     return log_level_strings[LOG_LEVEL_DEBUG];
    case LOG_LEVEL_INFO:      return log_level_strings[LOG_LEVEL_INFO];
    case LOG_LEVEL_WARN:      return log_level_strings[LOG_LEVEL_WARN];
    case LOG_LEVEL_ERROR:     return log_level_strings[LOG_LEVEL_ERROR];
    case LOG_LEVEL_CRITICAL:  return log_level_strings[LOG_LEVEL_CRITICAL];
    
    default: return "";
    }
}

int log_registerCallback(log_writeFn func, log_logLevel_t cb_level)
{
    /*check if a callback already registered.*/
    if(!log_settings.cb.fn)
    {
        log_settings.cb.fn = func;
        return 0;
    }
    else
        return -1;
}

void log_setLogLevel(log_logLevel_t level)
{
    log_settings.system_log_level = level;
}

void log_setLogState(log_loggingState_t enable)
{
    log_settings.enable = enable;
}
 
static void printLog(log_logLevel_t level, const char * const func_name, const char * const format, va_list args)
{
    // [level][func_name]:
    fprintf(
    stderr, " [%s] [%s]:",  
    log_getLevelString(level), func_name);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    fflush(stderr);
}

void log_log(log_logLevel_t level, const char * const func_name, const char * const format, ... )
{
    va_list args;
    if ((log_settings.enable != LOG_DISABLE) && (log_settings.system_log_level <= level)) 
    {
    va_start(args, format);
    printLog(level,func_name,format,args);
    //invoke callback function if registered.
    if (log_settings.cb.fn)
    {
        if (log_settings.cb.level <= level)
        {
            log_settings.cb.fn(level, func_name, format,args);
        }
    }
    va_end(args);
    }
}

