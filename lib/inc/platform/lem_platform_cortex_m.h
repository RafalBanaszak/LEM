#ifndef LEM_PLATFORM_CORTEX_M_H
#define LEM_PLATFORM_CORTEX_M_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if LEM_PLATFORM == LEM_PLATFORM_ARMV7M || LEM_PLATFORM == LEM_PLATFORM_ARMV8M
#include <stdatomic.h>
#else
static inline void lem_disable_interrupts(void) {
    __asm volatile ("cpsid i" : : : "memory");
}

static inline void lem_enable_interrupts(void) {
    __asm volatile ("cpsie i" : : : "memory");
}
#endif

typedef bool lem_bool_t;
#define lem_true true
#define lem_false false

typedef uint32_t lem_event_pool_pos_t;
#define LEM_EVENT_POOL_SIZE_MAX UINT32_MAX

typedef uint32_t lem_queue_pos_t;
#define LEM_QUEUE_SIZE_MAX (UINT32_MAX-1)

typedef int32_t lem_queue_index_t; /* This must be SIGNED type*/
#define LEM_QUEUES_NUMBER_MAX INT32_MAX

typedef uint32_t lem_timer_index_t;
#define LEM_TIMER_INDEX_MAX UINT32_MAX
typedef uint32_t lem_timer_counter_t;
typedef int32_t lem_timer_diff_t;

/*
Currently all CAS share the same code. Only the type of arguments changes.
*/
static inline lem_bool_t lem_CAS_byte(volatile uint8_t *value, uint8_t *expected, uint8_t desired) {
#if LEM_PLATFORM == LEM_PLATFORM_ARMV7M || LEM_PLATFORM == LEM_PLATFORM_ARMV8M
    return (lem_bool_t)atomic_compare_exchange_strong(value, expected, desired);

#elif LEM_PLATFORM == LEM_PLATFORM_ARMV6M

    lem_disable_interrupts();
    if (*value == *expected) {
        *value = desired;
        return lem_true;
    }
    *expected = *value;
    lem_enable_interrupts();
    return lem_false;

#else
#error "LEM: No supported architecture is defined in cortex-m CAS code."
#endif
}

static inline lem_bool_t lem_CAS_queue_position(volatile lem_queue_pos_t *value, lem_queue_pos_t *expected, lem_queue_pos_t desired) {
#if LEM_PLATFORM == LEM_PLATFORM_ARMV7M || LEM_PLATFORM == LEM_PLATFORM_ARMV8M
	return (lem_bool_t)atomic_compare_exchange_strong(value, expected, desired);

#elif LEM_PLATFORM == LEM_PLATFORM_ARMV6M
	lem_disable_interrupts();
	if (*value == *expected) {
		*value = desired;
		return lem_true;
	}
	*expected = *value;
	lem_enable_interrupts();
	return lem_false;

#else
#error "LEM: No supported architecture is defined in cortex-m CAS code."
#endif
}

#ifdef __cplusplus
}
#endif

#endif /* LEM_PLATFORM_CORTEX_M_H */
