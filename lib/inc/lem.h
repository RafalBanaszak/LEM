#ifndef LEM_LEM_H
#define LEM_LEM_H

#ifdef __cplusplus
extern "C" {
#endif


/*
    Configure LEM.
    Please note LEM use ONLY static memory allocation.
    This implies that larger numbers mean more RAM usage.
*/
#define LEM_EVENT_POOL_SIZE 16U
#define LEM_QUEUE_SIZE_PLUS_1 9U
#define LEM_QUEUE_SIZE (LEM_QUEUE_SIZE_PLUS_1 - 1)
#define LEM_QUEUES_NUMBER 3U
#define LEM_TIMERS_NUMBER 8U

#include "interface/lem_interface.h"

#ifdef __cplusplus
}
#endif

#endif /* LEM_LEM_H */
