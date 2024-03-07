#ifndef LEM_LEM_H
#define LEM_LEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config/lem_config.h"
#include "config/lem_config_check.h"
#include "platform/lem_platform.h"

#include <stdint.h>

typedef struct lem_event_t {
#ifdef LEM_EVENT_DATA_ENABLE
    lem_event_data_t data;
#endif
    struct PACKED {
        uint8_t ATOMIC lock;
        uint8_t eo_id;
    };
} lem_event_t;

#define lem_success lem_true
#define lem_fail lem_false

typedef void (*lem_eo_t)(lem_event_t *const event);

void lem_init(void);
void lem_init_eo_table(lem_eo_t *const eo_array);
lem_event_t *lem_alloc(void);
void lem_free(lem_event_t *const event);
lem_bool_t lem_send(const lem_event_t *const event, const lem_queue_index_t queue);
void lem_dispatch(void);
void lem_timer_tick(void);
lem_timer_index_t lem_timer_create(lem_timer_counter_t delay, lem_event_t *event, lem_queue_index_t queue_index);
void lem_timer_delete(lem_timer_index_t timer);

#ifdef __cplusplus
}
#endif

#endif /* LEM_LEM_H */
