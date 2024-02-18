#include "postgres.h"
#include "postmaster/isolate.h"

#include <string.h>
#include <unistd.h>
#include <errno.h>

#define _GNU_SOURCE
#include <sched.h>
#include <fcntl.h>

static void log_hostname();

int 
IsolateBackgroundWorker(RegisteredBgWorker *rw)
{
    int err;
    int ns_fd;
    char* library_name = rw->rw_worker.bgw_library_name;

    elog(LOG, "NEW BGWORKER FOR LIBRARY:\nname: %s\ntype: %s\nfname %s\nlname %s\nextra: %s\n", 
        rw->rw_worker.bgw_name,
        rw->rw_worker.bgw_type,
        rw->rw_worker.bgw_function_name,
        library_name,
        rw->rw_worker.bgw_extra);

    // elog(LOG, "curr wd: %s", get_current_dir_name());    

    log_hostname();

    ns_fd = open("/home/extio1/pgsql_isolate/isolate/uts_namespace", O_RDONLY);

    if(ns_fd == -1){
        err = errno;
        elog(FATAL, "open() failed: %s", strerror(err));
        return err;
    }

    if(strcmp(library_name, "test_module") == 0){
        if( (err = setns(ns_fd, CLONE_NEWUTS)) != 0 ){
            err = errno;
            elog(FATAL, "setns() failed: %s", strerror(err));
        }
    }

    log_hostname();

    close(ns_fd);
    return err;
}

void
log_hostname()
{
    char host[256];
    if(gethostname(host, sizeof(host)) != 0){
        elog(ERROR, "gethostname() error");
    }

    elog(INFO, "Current Host Name: %s", host);
}