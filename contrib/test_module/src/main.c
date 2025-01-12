#include "postgres.h"
#include "utils/builtins.h"
#include "fmgr.h"
#include "tcop/utility.h"

/* --- bgworker --- */
#include "miscadmin.h"
#include "postmaster/bgworker.h"
#include "postmaster/interrupt.h"
#include "storage/ipc.h"
#include "storage/latch.h"
#include "storage/lwlock.h"
#include "storage/proc.h"
#include "storage/shmem.h"

/* these headers are used by this particular worker's code */
#include "access/xact.h"
#include "executor/spi.h"
#include "fmgr.h"
#include "lib/stringinfo.h"
#include "pgstat.h"
#include "utils/builtins.h"
#include "utils/snapmgr.h"
#include "tcop/utility.h"

PG_MODULE_MAGIC;

/* --- Local fuctions declaration --- */

void _PG_init(void);  

/* --- Local vars declaration --- */

/* --- _PG_* --- */
void
_PG_init(void)
{
    BackgroundWorker worker;

    // prev_ProcessUtility = ProcessUtility_hook;
    // ProcessUtility_hook = my_ProcessUtility;  

	elog(LOG, "module launched");

    memset(&worker, 0, sizeof(worker));
	worker.bgw_flags = BGWORKER_SHMEM_ACCESS |
		BGWORKER_BACKEND_DATABASE_CONNECTION;
	worker.bgw_start_time = BgWorkerStart_RecoveryFinished;
	worker.bgw_restart_time = BGW_DEFAULT_RESTART_INTERVAL;
	sprintf(worker.bgw_library_name, "test_module");
	sprintf(worker.bgw_function_name, "test_main");
    sprintf(worker.bgw_name, "test_module main process");
	worker.bgw_notify_pid = 0;

    RegisterBackgroundWorker(&worker);

	elog(LOG, "module main end");

}