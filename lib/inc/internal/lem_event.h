#ifndef LEM_EVENT_H
#define LEM_EVENT_H
#include "lem.h"

typedef struct lem_event_pool_t {
    volatile lem_event_t pool[LEM_EVENT_POOL_SIZE];
    volatile lem_event_pool_pos_t position;
} lem_event_pool_t;

extern lem_event_pool_t lem_event_pool;

static inline lem_event_pool_pos_t increment_rewind_pool_pos(lem_event_pool_pos_t val, lem_event_pool_pos_t max_val_plus_one) {
    if (++val >= max_val_plus_one) {
        val = 0;
    }
    return val;
}

lem_event_t *lem_event_alloc(void);
void lem_event_free(lem_event_t *const event);

#endif /* LEM_EVENT_H */
