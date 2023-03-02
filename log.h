#ifndef __LOG_H
#define __LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
typedef enum 
{
    LOG_LEVEL_DEBUG, /**< Lowest level */
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_CRITICAL
}log_logLevel_t;

typedef enum
{
    LOG_DISABLE,
    LOG_ENABLE
}log_loggingState_t;


#define LOG_DEBUG(format, ... )    log_log(LOG_LEVEL_DEBUG, __FUNCTION__, format, ## __VA_ARGS__)
#define LOG_INFO(format, ... )     log_log(LOG_LEVEL_INFO, __FUNCTION__, format, ## __VA_ARGS__)
#define LOG_WARN(format, ... )     log_log(LOG_LEVEL_WARN, __FUNCTION__, format, ## __VA_ARGS__)
#define LOG_ERROR(format, ... )    log_log(LOG_LEVEL_ERROR, __FUNCTION__, format, ## __VA_ARGS__)
#define LOG_CRITICAL(format, ... ) log_log(LOG_LEVEL_CRITICAL, __FUNCTION__, format, ## __VA_ARGS__)


/**
 * 
 * @brief The callback can be used for example to store log messeges into a file.
 * @note don't call log functions inside the callback.
 */
typedef void (*log_writeFn)(log_logLevel_t level,
                            const char * const func_name,
                            const char * const format,
                            va_list args);

/**
 * @brief The callback can be used for example to store log messeges into a file.
 *        
 * @param func Pointer to the callback function.
 * @param cb_level a seperate log level for callback function.
 * @return int
 * @retval [0] if success.
 * @retval [-1] if a callback already registered. 
 */
int log_registerCallback(log_writeFn func, log_logLevel_t cb_level);

/**
 * @brief Set the log level.
 * Message levels lower than this value will be discarded.
 * The default log level is INFO.
 * 
 * @param level 
 */
void log_setLogLevel(log_logLevel_t level);

/**
 * @brief returns corresponding level string.
 * 
 * @param level 
 * @return const char* 
 */
const char * log_getLevelString (log_logLevel_t level);

/**
 * @brief Enable or disable the logging system.
 * The difault log state is ENABLE.
 * 
 * @param enable [in] LOG_ENABLE for enabling log system.
 *                    LOG_DISABLE for disabling log system.
 */
void log_setLogState(log_loggingState_t enable);

/**
 * @brief Logs a message to stderr.
 *        Calls the callback if registered.
 *        No need to put "\n" at the end of the messege.
 * 
 * @param level 
 * @param func_name 
 * @param format 
 * @param args 
 */
void log_log(log_logLevel_t level,
             const char * const func_name,
             const char * const format,
              ... );

#ifdef __cplusplus
}
#endif

#endif//__LOG_H