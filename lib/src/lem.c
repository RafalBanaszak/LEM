#include "lem.h"
#include "internal/lem_dispatcher.h"
#include "internal/lem_event.h"
#include "internal/lem_queue.h"
#include "internal/lem_soft_timer.h"

void lem_init(void) {
    // do noting
    // this init function is prepared for future use
}

void lem_register_eo_table(lem_eo_t *const eo_array) { lem_dispatcher_assign_eo_array(eo_array); }

lem_event_t *lem_alloc(void) { return lem_event_alloc(); }

void lem_free(lem_event_t *event) { lem_event_free(event); }

lem_bool_t lem_send(const lem_event_t *const event, const lem_queue_index_t queue) {
    return lem_queue_put_lockless(event, &lem_queue_list[queue]);
}

void lem_dispatch(void) { lem_dispatch_internal(); }

void lem_timer_tick(void) { lem_soft_timer_tick(); }

lem_timer_index_t lem_timer_create(lem_timer_diff_t delay, lem_event_t *event, lem_queue_index_t queue_index) {
    return lem_soft_timer_create(delay, event, queue_index);
}

void lem_timer_delete(lem_timer_index_t timer) { lem_soft_timer_delete(timer); }
