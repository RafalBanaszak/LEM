#include "internal/lem_event.h"

typedef enum lem_event_state_t { LEM_EVENT_FREE = 0, LEM_EVENT_ALLOCATED } lem_event_state_t;
lem_event_pool_t lem_event_pool = {0};

lem_event_t *lem_event_alloc(void) {
    lem_event_t *event = NULL;
    lem_event_pool_pos_t initial_position = lem_event_pool.position;

    uint8_t expected_lock_value;
    do {
        expected_lock_value = LEM_EVENT_FREE;
        /* position incrementation do not need to be atomic
         * in worst case the same position will be checked more than once */
        lem_event_pool.position = increment_rewind_pool_pos(lem_event_pool.position, LEM_EVENT_POOL_SIZE);
        if (lem_CAS_byte(&lem_event_pool.pool[lem_event_pool.position].lock, &expected_lock_value, LEM_EVENT_ALLOCATED)) {
            event = (lem_event_t *)&lem_event_pool.pool[lem_event_pool.position];
            break;
        }
        /* if alloc failed try again with next pool position until you check the
         * whole pool */
    } while (lem_event_pool.position != initial_position);

    return event;
}

void lem_event_free(lem_event_t *const event) { event->lock = LEM_EVENT_FREE; }
