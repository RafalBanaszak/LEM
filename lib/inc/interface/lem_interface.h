#ifndef LEM_INTERFACE_H
#define LEM_INTERFACE_H

#include "platform/lem_platform.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lem_event_t {
    struct __attribute__((packed)) {
        uint16_t user_space;
        uint8_t eo_id;
        uint8_t lock;
    };
    void *payload;
} lem_event_t;
#define LEM_EVENT_USER_SPACE_MAX UINT16_MAX;
#define LEM_EO_NUMBER_MAX UINT8_MAX;

#define LEM_SEND_SUCCESS lem_true
#define LEM_SEND_FAIL lem_false

typedef void (*lem_eo_t)(lem_event_t *const event);

void lem_init(void);
void lem_register_eo_table(lem_eo_t *const eo_array);
lem_event_t *lem_alloc(void);
void lem_free(lem_event_t *const event);
lem_bool_t lem_send(const lem_event_t *const event, const lem_queue_index_t queue);
void lem_dispatch(void);
void lem_timer_tick(void);
lem_timer_index_t lem_timer_create(lem_timer_diff_t delay, lem_event_t *event, lem_queue_index_t queue_index);
void lem_timer_delete(lem_timer_index_t timer);

#ifdef __cplusplus
}
#endif

#endif /* LEM_INTERFACE_H */
