#include "postgres.h"

#include <stdbool.h>
#include <unistd.h>

PGDLLEXPORT int test_main();

int 
test_main()
{
    elog(LOG, "!!!!!!!!!! proccess succesfully launched");

    sleep(2);

    // for(int i = 0; i < 15; ++i){
    //     elog(LOG, "send signal start");
    //     SendPostmasterSignal(PMSIGNAL_START_AUTOVAC_WORKER);
    //     elog(LOG, "send signal end");
    //     sleep(1);
    // }

    return 0;
}