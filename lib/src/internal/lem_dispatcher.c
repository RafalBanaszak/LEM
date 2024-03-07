#include "internal/lem_dispatcher.h"
#include "internal/lem_event.h"
#include "internal/lem_queue.h"

lem_eo_t *lem_eo_array;

void lemi_init_eo_table(lem_eo_t eo_array[]) { lem_eo_array = eo_array; }

void lemi_dispatch(void) {
    lem_event_t *event = NULL;
    /* Highest priority queue was empty or not known so find the right one */
    for (lem_queue_index_t i = LEM_QUEUES_NUMBER - 1; i >= 0; i--) {
        if (!lem_queue_is_empty(&(lem_queue_list[i]))) {
            event = lem_queue_pop_naive(&lem_queue_list[i]);
            break;
        }
    }

    if (NULL != event) {
        lem_eo_array[event->eo_id](event);
    }
}
