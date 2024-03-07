#ifndef LEM_CONFIG_CHECK_H
#define LEM_CONFIG_CHECK_H

#include "config/lem_config.h"
#include "platform/lem_platform.h"

/* check if provided configuration is valid */
#if LEM_EVENT_POOL_SIZE > LEM_EVENT_POOL_SIZE_MAX
#error "LEM: Pool size larger than maximum allowed!"
#endif

#if LEM_QUEUE_SIZE > LEM_QUEUE_SIZE_MAX - 1
#error "LEM: Queue size larger than maximum allowed!"
#endif

#if LEM_QUEUES_NUMBER > LEM_QUEUES_NUMBER_MAX
#error "LEM: Number of queues greater than maximum allowed!"
#endif

#endif /* LEM_CONFIG_CHECK_H */