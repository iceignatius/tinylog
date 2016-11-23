/**
 * @file
 * @brief     Tiny Logger client API.
 * @author    王文佑
 * @date      2016/11/23
 * @copyright ZLib Licence
 */
#ifndef _TLOG_H_
#define _TLOG_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Log levels.
 */
enum tlog_lev_t
{
    TLOG_LEV_FAIL   = 0,    ///< Failure message.
    TLOG_LEV_ERROR  = 1,    ///< Error message.
    TLOG_LEV_WARN   = 2,    ///< Warning message.
    TLOG_LEV_NOTE   = 3,    ///< Notification message.
    TLOG_LEV_INFO   = 4,    ///< Information message.
    TLOG_LEV_DEBUG  = 5,    ///< Debug message.
};

/**
 * Output targets.
 */
enum tlog_output_t
{
    TLOG_OUTPUT_STDOUT  = 1,    ///< Output to standard output.
    TLOG_OUTPUT_STDERR  = 2,    ///< Output to standard error output.
    TLOG_OUTPUT_LOGFILE = 4,    ///< Output to a file.
};

/**
 * @name Logger configuration functions.
 * @{
 */

void tlog_set_loglevel(unsigned level);
void tlog_set_output(int targets);
void tlog_set_logfile(const char *filename);

/**
 * @}
 */

#ifdef __GNUC__
    #define TLOG_PRINT_CHECK(fmtidx, chkidx) __attribute__ ((format(printf, fmtidx, chkidx)))
#else
    #define TLOG_PRINT_CHECK(fmtidx, chkidx)
#endif

int tlog_print_detail(const char *module, const char *func, unsigned level, const char *format, ...)
    TLOG_PRINT_CHECK(4,5);

/**
 * @name Logger print functions.
 * @{
 */

/**
 * @fn int tlog_print(module, level, format, args...)
 * @brief Print log message for customised level.
 *
 * @param module Name of the module who generate the message.
 * @param level  Level of the message, it can be any values defined in ::tlog_lev_t.
 * @param format String of the message which follows the "printf" format.
 * @param args   Arguments that just like in "printf".
 * @return ZERO if success; and others if failed.
 */
#define tlog_print(module, level, format, args...) \
        tlog_print_detail(module, __func__, level, format, ##args)

/**
 * @fn int tlog_print_fail(module, format, args...)
 * @brief Print log message for fatal.
 *
 * @param module Name of the module who generate the message.
 * @param format String of the message which follows the "printf" format.
 * @param args   Arguments that just like in "printf".
 * @return ZERO if success; and others if failed.
 */
#define tlog_print_fail(module, format, args...) \
        tlog_print_detail(module, __func__, TLOG_LEV_FAIL, format, ##args)

/**
 * @fn int tlog_print_error(module, format, args...)
 * @brief Print log message for error.
 *
 * @param module Name of the module who generate the message.
 * @param format String of the message which follows the "printf" format.
 * @param args   Arguments that just like in "printf".
 * @return ZERO if success; and others if failed.
 */
#define tlog_print_error(module, format, args...) \
        tlog_print_detail(module, __func__, TLOG_LEV_ERROR, format, ##args)

/**
 * @fn int tlog_print_warn(module, format, args...)
 * @brief Print log message for warning.
 *
 * @param module Name of the module who generate the message.
 * @param format String of the message which follows the "printf" format.
 * @param args   Arguments that just like in "printf".
 * @return ZERO if success; and others if failed.
 */
#define tlog_print_warn(module, format, args...) \
        tlog_print_detail(module, __func__, TLOG_LEV_WARN, format, ##args)

/**
 * @fn int tlog_print_note(module, format, args...)
 * @brief Print log message for notification.
 *
 * @param module Name of the module who generate the message.
 * @param format String of the message which follows the "printf" format.
 * @param args   Arguments that just like in "printf".
 * @return ZERO if success; and others if failed.
 */
#define tlog_print_note(module, format, args...) \
        tlog_print_detail(module, __func__, TLOG_LEV_NOTE, format, ##args)

/**
 * @fn int tlog_print_info(module, format, args...)
 * @brief Print log message for information.
 *
 * @param module Name of the module who generate the message.
 * @param format String of the message which follows the "printf" format.
 * @param args   Arguments that just like in "printf".
 * @return ZERO if success; and others if failed.
 */
#define tlog_print_info(module, format, args...) \
        tlog_print_detail(module, __func__, TLOG_LEV_INFO, format, ##args)

/**
 * @fn int tlog_print_debug(module, format, args...)
 * @brief Print log message for debug.
 *
 * @param module Name of the module who generate the message.
 * @param format String of the message which follows the "printf" format.
 * @param args   Arguments that just like in "printf".
 * @return ZERO if success; and others if failed.
 */
#define tlog_print_debug(module, format, args...) \
        tlog_print_detail(module, __func__, TLOG_LEV_DEBUG, format, ##args)

/**
 * @}
 */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
