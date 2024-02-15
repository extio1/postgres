#include "postgres.h"
#include "postmaster/isolate.h"

#include <string.h>
#include <errno.h>

#define _GNU_SOURCE
#include <sched.h>

int 
IsolateBackgroundWorker(RegisteredBgWorker *rw)
{
    int err;
    char* library_name = rw->rw_worker.bgw_library_name;

    elog(LOG, "NEW BGWORKER FOR LIBRARY:\nname: %s\ntype: %s\nfname %s\nlname %s\nextra: %s\n", 
        rw->rw_worker.bgw_name,
        rw->rw_worker.bgw_type,
        rw->rw_worker.bgw_function_name,
        library_name,
        rw->rw_worker.bgw_extra);

    if(strcmp(library_name, "test_module") == 0){
        if( (err = unshare(CLONE_NEWPID)) != 0 ){
            err = errno;
            elog(FATAL, "unshare() failed: %s", strerror(err));
        } else {
            elog(LOG, "%s ushared new namespace", library_name);
        }
    }

    return err;
}