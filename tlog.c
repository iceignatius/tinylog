#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
#else
    #include <sys/file.h>
#endif

#include <gen/ascii.h>
#include <gen/jmpbk.h>
#include <gen/bufstm.h>
#include <gen/systime.h>
#include <gen/timeinf.h>
#include "tlog.h"

typedef struct record_t
{
    long long   uxtime;
    const char *modname;
    const char *funcname;
    unsigned    level;
    char        msg[TLOG_MSG_MAXSIZE];
} record_t;

typedef struct log_param_t
{
    unsigned level;
    int      targets;
    char     logfile[1024];
} log_param_t;

static log_param_t logparam =
{
    .level   = TLOG_LEV_WARN,
    .targets = TLOG_OUTPUT_STDERR | TLOG_OUTPUT_LOGFILE,
    .logfile = "/var/log/tlog.log",
};

//------------------------------------------------------------------------------
void tlog_set_loglevel(unsigned level)
{
    /**
     * Change the record filter level.
     *
     * @param level It can be any level that defined in ::tlog_lev_t,
     *              and any records which level are great then the value will be discard.
     *              The default value is ::TLOG_LEV_WARN.
     */
    logparam.level = level;
}
//------------------------------------------------------------------------------
void tlog_set_output(int targets)
{
    /**
     * Change the output targets.
     *
     * @param targets The combination of ::tlog_output_t,
     *                and that indicated where the log message should be output to.
     *                The default combination is TLOG_OUTPUT_STDERR | TLOG_OUTPUT_LOGFILE.
     */
    logparam.targets = targets;
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
    strncpy(logparam.logfile, filename, sizeof(logparam.logfile)-1);
}
//------------------------------------------------------------------------------
static
void record_collect_info(record_t   *record,
                         const char *module,
                         const char *funcname,
                         unsigned    level,
                         const char *msg_format,
                         va_list     msg_args)
{
    record->uxtime   = systime_get_local();
    record->modname  = module;
    record->funcname = funcname;
    record->level    = level;
    vsnprintf(record->msg, sizeof(record->msg), msg_format, msg_args);
}
//------------------------------------------------------------------------------
static
const char* get_level_str(unsigned level)
{
    switch( level )
    {
    case TLOG_LEV_FAIL  :   return "FAIL";
    case TLOG_LEV_ERROR :   return "ERROR";
    case TLOG_LEV_WARN  :   return "WARN";
    case TLOG_LEV_NOTE  :   return "NOTE";
    case TLOG_LEV_INFO  :   return "INFO";
    case TLOG_LEV_DEBUG :   return "DEBUG";
    default             :   return "";
    }
}
//------------------------------------------------------------------------------
static
void replace_ascii_ctrl_to_star(char *str)
{
    for(; *str; ++str)
    {
        if( iscntrl(*str) )
            *str = '*';
    }
}
//------------------------------------------------------------------------------
static
bool push_record_item(bufostm_t *stream, const char *tag, const char *content, bool is_last)
{
    if( !bufostm_write(stream, tag, strlen(tag)) ) return false;
    if( !bufostm_write(stream, ": ", 2) ) return false;
    if( !bufostm_write(stream, content, strlen(content)) ) return false;
    if( !bufostm_putbyte(stream, ( is_last ? ASCII_LF : ASCII_HT )) ) return false;

    return true;
}
//------------------------------------------------------------------------------
static
bool record_encode_str(record_t *record, char *buf, size_t bufsize)
{
    bufostm_t stream;
    bufostm_init(&stream, buf, bufsize);

    bool res = false;
    do
    {
        char content[sizeof(record->msg)] = {0};

        timeinf_t timeinf = timeinf_from_uxtime(record->uxtime);
        timeinf_to_datetimestr(&timeinf, content, sizeof(content)-1, "/", ":", " ");
        if( !push_record_item(&stream, "time", content, false) ) break;

        if( !push_record_item(&stream, "mod", record->modname, false) ) break;
        if( !push_record_item(&stream, "func", record->funcname, false) ) break;
        if( !push_record_item(&stream, "lev", get_level_str(record->level), false) ) break;

        replace_ascii_ctrl_to_star(record->msg);
        if( !push_record_item(&stream, "msg", record->msg, true) ) break;

        res = true;
    } while(false);

    return res;
}
//------------------------------------------------------------------------------
#ifdef _WIN32
static
bool lock_file(FILE *file, OVERLAPPED *overlapped)
{
    return LockFileEx((HANDLE) _get_osfhandle(_fileno(file)),
                      LOCKFILE_EXCLUSIVE_LOCK,
                      0,
                      -1,
                      -1,
                      overlapped);
}
#else
static
bool lock_file(FILE *file)
{
    return !flock(fileno(file), LOCK_EX);
}
#endif
//------------------------------------------------------------------------------
#ifdef _WIN32
static
bool unlock_file(FILE *file, OVERLAPPED *overlapped)
{
    return UnlockFileEx((HANDLE) _get_osfhandle(_fileno(file)),
                        0,
                        -1,
                        -1,
                        overlapped);
}
#else
static
bool unlock_file(FILE *file)
{
    return !flock(fileno(file), LOCK_UN);
}
#endif
//------------------------------------------------------------------------------
static
bool print_str_to_file(FILE *file, const char *str)
{
#ifdef _WIN32
    OVERLAPPED overlapped = {0};
    lock_file(file, &overlapped);
#else
    lock_file(file);
#endif

    bool res = ( 1 == fwrite(str, strlen(str), 1, file) );

#ifdef _WIN32
    unlock_file(file, &overlapped);
#else
    unlock_file(file);
#endif

    return res;
}
//------------------------------------------------------------------------------
static
bool print_str_to_targets(const log_param_t *param, const char *str)
{
    FILE *logfile = false;

    bool res = false;
    do
    {
        if( param->targets & TLOG_OUTPUT_STDOUT )
        {
            if( !print_str_to_file(stdout, str) )
                break;
        }

        if( param->targets & TLOG_OUTPUT_STDERR )
        {
            if( !print_str_to_file(stderr, str) )
                break;
        }

        if( param->targets & TLOG_OUTPUT_LOGFILE )
        {
            if( !( logfile = fopen(param->logfile, "ab") ) )
                break;
            if( !print_str_to_file(logfile, str) )
                break;
        }

        res = true;
    } while(false);

    if( logfile )
        fclose(logfile);

    return res;
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
     *
     * @remarks The message should be shorter than ::TLOG_MSG_MAXSIZE,
     *          or it will be truncated.
     */
    if( level > logparam.level ) return 0;

    va_list msg_args;
    va_start(msg_args, format);

    int res;
    JMPBK_BEGIN
    {
        record_t record;
        record_collect_info(&record, module, func, level, format, msg_args);

        char recstr[2*sizeof(record.msg)] = {0};
        if( !record_encode_str(&record, recstr, sizeof(recstr)-1) ) JMPBK_THROW(0);

        if( !print_str_to_targets(&logparam, recstr) ) JMPBK_THROW(0);
    }
    JMPBK_FINAL
    {
        res = JMPBK_ERRCODE;
    }
    JMPBK_END

    va_end(msg_args);

    return res;
}
//------------------------------------------------------------------------------
