#include "postgres.h"
#include "libpq/libpq-be.h"
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

    elog(LOG, "[pid %d] NEW BGWORKER FOR LIBRARY:\nname: %s\ntype: %s\nfname %s\nlname %s\nextra: %s\n", 
        getpid(),
        rw->rw_worker.bgw_name,
        rw->rw_worker.bgw_type,
        rw->rw_worker.bgw_function_name,
        library_name,
        rw->rw_worker.bgw_extra);

    log_hostname();

    ns_fd = open("/home/extio1/nsfiles/uts", O_RDONLY);

    if(ns_fd == -1){
        err = errno;
        elog(LOG, "open() failed: %s", strerror(err));
        return err;
    }

    if( strcmp(library_name, "test_module") == 0 ){
        if( (err = setns(ns_fd, CLONE_NEWUTS)) != 0 ){
            err = errno;
            elog(LOG, "setns() failed: %s", strerror(err));
        } else {
            elog(LOG, "%s ushared new namespace", library_name);
        }
    }

    log_hostname();

    return err;
}

int 
IsolateBackend(Port *port)
{
    elog(LOG, "[%d] NEW BACKEND:"
    "\ndatabase: %s\nuser: %s\naplication_name %s"
    "\nremote_host %s\nremote_hostname: %s\n", 
        getpid(),
        port->database_name,
        port->user_name,
        port->application_name,
        port->remote_host,
        port->remote_hostname
    );

    return 0;
}

void
log_hostname()
{
    char host[256];
    if(gethostname(host, sizeof(host)) != 0){
        elog(ERROR, "gethostname() error");
    }

    elog(LOG, "Current Host Name: %s", host);
}