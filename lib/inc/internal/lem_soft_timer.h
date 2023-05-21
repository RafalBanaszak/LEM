#ifndef LEM_SOFT_TIMER_H
#define LEM_SOFT_TIMER_H
#include "lem.h"

#define LEM_SOFT_TIMER_FREE NULL
#define LEM_SOFT_TIMER_LAST ((void *)8)

typedef enum lem_timer_status_t { LEM_TIMER_FREE = 0, LEM_TIMER_RESERVED, LEM_TIMER_READY, LEM_TIMER_HANDLED } lem_timer_status_t;

typedef struct __attribute__((packed)) lem_timer_t {
    lem_timer_counter_t expiration_time;
    lem_event_t *event;
    lem_queue_index_t queue_index;
    uint8_t lock;
} lem_timer_t;

extern volatile lem_timer_counter_t lem_time;
extern volatile lem_timer_t lem_timers[LEM_TIMERS_NUMBER];

static inline lem_timer_index_t rewind_once_timer_index(lem_timer_index_t val, lem_timer_index_t max_val_plus_one) {
    if (val >= max_val_plus_one) {
        val -= max_val_plus_one;
    }
    return val;
}

static inline lem_bool_t lem_soft_timer_check_expired(const volatile lem_timer_t *const timer) {
    return ((lem_timer_diff_t)(timer->expiration_time - lem_time)) <= 0;
}

static inline void lem_soft_timer_tick(void) {
    lem_bool_t CAS_success;
    lem_timer_status_t expected_timer_status;

    lem_time++;
    for (lem_timer_index_t i = 0; i < LEM_TIMERS_NUMBER; i++) {
        if (LEM_TIMER_READY != lem_timers[i].lock || !lem_soft_timer_check_expired(&lem_timers[i])) {
            continue;
        };

        expected_timer_status = LEM_TIMER_READY;
        CAS_success = lem_CAS_byte(&(lem_timers[i].lock), (uint8_t *)&expected_timer_status, (uint8_t)LEM_TIMER_HANDLED);
        if (!CAS_success) {
            continue;
        }

        /* ABA protection */
        if (!lem_soft_timer_check_expired(&lem_timers[i])) {
            lem_timers[i].lock = LEM_TIMER_READY;
            continue;
        }

        lem_send(lem_timers[i].event, lem_timers[i].queue_index);
        lem_timers[i].lock = LEM_TIMER_FREE;
    }
}

lem_timer_index_t lem_soft_timer_create(lem_timer_diff_t delay, lem_event_t *event, lem_queue_index_t queue_index);
void lem_soft_timer_delete(lem_timer_index_t timer);

#endif /* LEM_SOFT_TIMER_H */