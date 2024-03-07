#include "lem.h"
#include "internal/lem_dispatcher.h"
#include "internal/lem_event.h"
#include "internal/lem_queue.h"
#include "internal/lem_soft_timer.h"

void lem_init(void) {
    // do noting
    // this init function is prepared for future use
}

void lem_init_eo_table(lem_eo_t *const eo_array) { lemi_init_eo_table(eo_array); }

lem_event_t *lem_alloc(void) { return lemi_event_alloc(); }

void lem_free(lem_event_t *event) { lemi_event_free(event); }

lem_bool_t lem_send(const lem_event_t *const event, const lem_queue_index_t queue) { return lemi_queue_put_lockless(event, &lem_queue_list[queue]); }

void lem_dispatch(void) { lemi_dispatch(); }

void lem_timer_tick(void) { lemi_soft_timer_tick(); }

lem_timer_index_t lem_timer_create(lem_timer_counter_t delay, lem_event_t *event, lem_queue_index_t queue_index) {
    return lemi_soft_timer_create(delay, event, queue_index);
}

void lem_timer_delete(lem_timer_index_t timer) { lemi_soft_timer_delete(timer); }
