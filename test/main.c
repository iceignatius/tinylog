#include <assert.h>
#include <stdio.h>
#include "tlog.h"

int main(void)
{
    static const char modname[] = "tinylog-test";

    tlog_set_loglevel(TLOG_LEV_WARN);
    tlog_set_logfile_prefix("./tlog-");

    assert( 0 == tlog_print_fail (modname, "Sample int=%d, str=%s.", 3, "text") );
    assert( 0 == tlog_print_error(modname, "Sample int=%d, str=%s.", 3, "text") );
    assert( 0 == tlog_print_warn (modname, "Sample int=%d, str=%s.", 3, "text") );
    assert( 0 == tlog_print_note (modname, "Sample int=%d, str=%s.", 3, "text") );
    assert( 0 == tlog_print_info (modname, "Sample int=%d, str=%s.", 3, "text") );
    assert( 0 == tlog_print_debug(modname, "Sample int=%d, str=%s.", 3, "text") );

    printf("Log file generated. Now please check the file manually.\n");

    return 0;
}
