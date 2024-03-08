#ifndef LEM_CONFIG_H
#define LEM_CONFIG_H
#include <stdint.h>

/*
Event pool size
Set the number of events that can be allocated at the same time.
*/
#define LEM_EVENT_POOL_SIZE 16U

/*
Queue size
Set the number of events that can be stored in the queue.
The size must be smaller than maximum value of the lem_queue_pos_t type (defined in a platform file) minus 1.
Example: lem_queue_pos_t is uint8_t, so the maximum value is 255, so the maximum queue size is 254.
*/
#define LEM_QUEUE_SIZE 8U

/*
Number of queues (priority levels)
Set the number of queues that can be used in the system. Queues with higher index have higher priority.
The number must be smaller than maximum value of the lem_queue_index_t type (defined in a platform file).
*/
#define LEM_QUEUES_NUMBER 3U

/*
Software timer pool size
Set the number of software timers that can be used in the system at the same time.
The size must be smaller than maximum value of the lem_timer_index_t type (defined in a platform file).
*/
#define LEM_TIMERS_NUMBER 8U

/*
Enable 'data' field in the event structure
The 'data' field is used to store additional information in the event structure (e.g. button number, ADC value, etc.).
Events are preallocated in the pool. If the 'data' field is enabled, the size of the pool will be larger.
*/
#define LEM_EVENT_DATA_ENABLE
typedef uint8_t lem_event_data_t;

#endif //LEM_CONFIG_H
