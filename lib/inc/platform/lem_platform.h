#ifndef LEM_PLATFORM_PLATFORM_H
#define LEM_PLATFORM_PLATFORM_H

/*
    Select platform.
    It is intended to set LEM_PLATFORM value by the build system.
    Currently supported values:
*/
#define LEM_PLATFORM_GENERIC   1U /* - Used during unit tests and good start point to create a custom platrofm configuration. */
#define LEM_PLATFORM_ARMV6M    2U /* - Cortex M0 */
#define LEM_PLATFORM_ARMV7M    3U /* - Cortex M3/M4/M7 */
#define LEM_PLATFORM_ARMV8M    4U /* - Cortex M23/M33 */

#if !defined(LEM_PLATFORM)
#error "LEM: Platform isn't defined!"
#endif

#if LEM_PLATFORM == LEM_PLATFORM_ARMV6M || LEM_PLATFORM == LEM_PLATFORM_ARMV7M || LEM_PLATFORM == LEM_PLATFORM_ARMV8M
#include "platform/lem_platform_cortex_m.h"
#elif LEM_PLATFORM == LEM_PLATFORM_GENERIC
#include "platform/lem_platform_generic.h"
#else
#error "LEM: Defined platform isn't supported. Check LEM_PLATFORM value."
#endif

#if LEM_EVENT_POOL_SIZE > LEM_EVENT_POOL_SIZE_MAX
#error "LEM: Pool size larger than maximum allowed!"
#endif

#if LEM_QUEUE_SIZE_PLUS_1 > LEM_QUEUE_SIZE_PLUS_1_MAX
#error "LEM: Queue size larger than maximum allowed!"
#endif

#if LEM_QUEUES_NUMBER > LEM_QUEUES_NUMBER_MAX
#error "LEM: Number of queues greater than maximum allowed!"
#endif

#if LEM_EO_NUMBER > LEM_EO_NUMBER_MAX
#error "LEM: Number of EOs greater than maximum allowed!"
#endif

#endif /* LEM_PLATFORM_PLATFORM_H*/
