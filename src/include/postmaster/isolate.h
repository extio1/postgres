#ifndef ISOLATE_H
#define ISOLATE_H

#include "postmaster/bgworker_internals.h"

extern int IsolateBackgroundWorker(RegisteredBgWorker *);
extern int IsolateBackend(Port *);

#endif