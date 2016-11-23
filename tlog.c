#include "tlog.h"

//------------------------------------------------------------------------------
void tlog_set_loglevel(unsigned level)
{
    /**
     * Change the record filter level.
     *
     * @param level It can be any level that defined in ::tlog_lev_t,
     *              and any records which level are great then the value will be discared.
     *              The default value is ::TLOG_LEV_INFO.
     */
}
//------------------------------------------------------------------------------
void tlog_set_output(int targets)
{
    /**
     * Change the output targets.
     *
     * @param targets The combination of ::tlog_output_t,
     *                and that indicated where the log message should be output to.
     *                The default compination is TLOG_OUTPUT_STDOUT | TLOG_OUTPUT_LOGFILE.
     */
}
//------------------------------------------------------------------------------
void tlog_set_logfile(const char *filename)
{
    /**
     * Change the log file path and name.
     *
     * @param filename The path and name of the log file.
     *                 The default is "/var/log/tlog.log".
     */
}
//------------------------------------------------------------------------------
int tlog_print_detail(const char *module, const char *func, unsigned level, const char *format, ...)
{
    /*
     * Build a message and print it to the output targets.
     *
     * @param module Name of the module who generate the message.
     * @param func   Name of the function who generate the message.
     * @param level  Level of the message, it can be any values defined in ::tlog_lev_t.
     * @param format String of the message which follows the "printf" format.
     * @param ...    Arguments that just like in "printf".
     * @return ZERO if success; and others if failed.
     */
}
//------------------------------------------------------------------------------
