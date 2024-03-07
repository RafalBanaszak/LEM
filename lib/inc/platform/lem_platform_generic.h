#ifndef LEM_PLATFORM_GENERIC_H
#define LEM_PLATFORM_GENERIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef LEM_UT
typedef enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
} memory_order;
#define ATOMIC
#else
#include <stdatomic.h>
#define ATOMIC _Atomic
#endif

#define PACKED __attribute__((packed))

typedef bool lem_bool_t;
#define lem_true true
#define lem_false false

typedef uint_fast8_t lem_event_pool_pos_t;
#define LEM_EVENT_POOL_SIZE_MAX UINT_FAST8_MAX

typedef uint_fast8_t lem_queue_pos_t;
#define LEM_QUEUE_SIZE_MAX UINT_FAST8_MAX

typedef int_fast8_t lem_queue_index_t; /* This must be SIGNED type*/
#define LEM_QUEUES_NUMBER_MAX INT_FAST8_MAX

typedef uint_fast8_t lem_timer_index_t;
#define LEM_TIMER_INDEX_MAX UINT_FAST8_MAX
typedef uint32_t lem_timer_counter_t;
typedef int32_t lem_timer_diff_t;

static inline lem_bool_t lem_atomic_CAS_byte(volatile uint8_t ATOMIC *value, uint8_t *expected, uint8_t desired) {
#ifdef LEM_UT
    /* Mock CAS operation for UT */
    if (*value == *expected) {
        *value = desired;
        return lem_true;
    }
    *expected = *value;
    return lem_false;
#else
    return (lem_bool_t)atomic_compare_exchange_strong(value, expected, desired);
#endif
}

static inline lem_queue_pos_t lem_atomic_load_queue_pos(volatile lem_queue_pos_t ATOMIC *value, memory_order order) {
#ifdef LEM_UT
    (void)order;
    return *value;
#else
    return atomic_load_explicit(value, order);
#endif
}

static inline lem_bool_t lem_atomic_CAS_queue_position(volatile lem_queue_pos_t ATOMIC *value, lem_queue_pos_t *expected, lem_queue_pos_t desired) {
#ifdef LEM_UT
    /* Mock CAS operation for UT */
    if (*value == *expected) {
        *value = desired;
        return lem_true;
    }
    *expected = *value;
    return lem_false;
#else
    return (lem_bool_t)atomic_compare_exchange_strong(value, expected, desired);
#endif
}

#ifdef __cplusplus
}
#endif

#endif /* LEM_PLATFORM_GENERIC_H */
