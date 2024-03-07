#ifndef LEM_CONFIG_H
#define LEM_CONFIG_H

#include <stdint.h>

/*
Set the number of events that can be allocated simultaneously in the event pool.
*/
#define LEM_EVENT_POOL_SIZE 16U

/*
Set the number of events that can be stored in the queue.
The size must be equal or less than the maximum value of the lem_queue_pos_t type (defined in a platform file) minus 1.
For example, if lem_queue_pos_t is uint8_t, the maximum value is 255, so the maximum queue size is 254.
*/
#define LEM_QUEUE_SIZE 8U

/*
Set the number of queues (priority levels) that can be used in the system. Queues with a higher index have a higher priority.
The number must be less than the maximum value of the lem_queue_index_t type (defined in a platform file).
*/
#define LEM_QUEUES_NUMBER 3U

/*
Set the number of software timers that can be used simultaneously in the system.
The size must be less than the maximum value of the lem_timer_index_t type (defined in a platform file).
*/
#define LEM_TIMERS_NUMBER 8U

/*
Enable the 'data' field in the event structure.
The 'data' field is used to store additional information in the event structure (e.g., button number, ADC value, etc.).
Events are preallocated in the pool. If the 'data' field is enabled, the size of the pool will increase.
*/
#define LEM_EVENT_DATA_ENABLE
typedef uint8_t lem_event_data_t;

#endif /* LEM_CONFIG_H */