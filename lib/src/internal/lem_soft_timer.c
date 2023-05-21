#include "internal/lem_soft_timer.h"

volatile lem_timer_counter_t lem_time = 0;
volatile lem_timer_t lem_timers[LEM_TIMERS_NUMBER];
volatile lem_timer_index_t lem_timers_array_start_index = 0;

lem_timer_index_t lem_soft_timer_create(lem_timer_diff_t delay, lem_event_t *event, lem_queue_index_t queue_index) {
    lem_timer_index_t start_index = lem_timers_array_start_index;
    lem_timer_index_t i, current_index;
    lem_bool_t CAS_success;
    uint8_t CAS_expected_val;
    for (i = 0; i < LEM_TIMERS_NUMBER; i++) {
        current_index = rewind_once_timer_index(i + start_index, LEM_TIMERS_NUMBER);
        if (LEM_TIMER_FREE != lem_timers[current_index].lock) {
            continue;
        }

        CAS_expected_val = LEM_TIMER_FREE;
        CAS_success = lem_CAS_byte(&(lem_timers[current_index].lock), &CAS_expected_val, (uint8_t)LEM_TIMER_RESERVED);
        if (!CAS_success) {
            continue;
        }
        lem_timers_array_start_index = rewind_once_timer_index(current_index + 1, LEM_TIMERS_NUMBER);
        lem_timers[current_index].expiration_time = lem_time + delay;
        lem_timers[current_index].event = event;
        lem_timers[current_index].queue_index = queue_index;
        /* fence here */
        lem_timers[current_index].lock = LEM_TIMER_READY;

        return current_index;
    }
    return LEM_TIMERS_NUMBER;
}

void lem_soft_timer_delete(lem_timer_index_t timer) {
    uint8_t CAS_expected_val = LEM_TIMER_READY;
    lem_CAS_byte(&(lem_timers[timer].lock), &CAS_expected_val, (uint8_t)LEM_TIMER_FREE);
}
