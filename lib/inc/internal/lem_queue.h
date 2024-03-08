#ifndef LEM_QUEUE_H
#define LEM_QUEUE_H
#include "lem.h"
#include "internal/lem_event.h"

typedef enum lem_queue_status_t { LEM_QUEUE_OK = 0, LEM_QUEUE_FULL, LEM_QUEUE_EMPTY } lem_queue_status_t;

typedef struct lem_queue_t {
    volatile lem_queue_pos_t start;
    volatile lem_queue_pos_t end;
    volatile lem_event_t *buffer[LEM_QUEUE_SIZE + 1];
} lem_queue_t;

extern lem_queue_t lem_queue_list[LEM_QUEUES_NUMBER];

static inline lem_queue_pos_t increment_rewind_queue_pos(lem_queue_pos_t val, lem_queue_pos_t max_val_plus_one) {
    if (++val >= max_val_plus_one) {
        val = 0;
    }
    return val;
}

void lem_queue_init(lem_queue_t *const queue);
lem_bool_t lem_queue_is_empty(const lem_queue_t *const queue);
lem_bool_t lem_queue_put_lockless(const lem_event_t *const event, lem_queue_t *const queue);
lem_event_t *lem_queue_pop_naive(lem_queue_t *const queue);

#endif /* LEM_QUEUE_H */
