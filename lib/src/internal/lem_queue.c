#include "internal/lem_queue.h"
#include "internal/lem_event.h"

lem_queue_t lem_queue_list[LEM_QUEUES_NUMBER] = {0};

lem_bool_t lem_queue_is_empty(const lem_queue_t *const queue) { return (queue->start == queue->end); }

lem_bool_t lem_queue_put_lockless(const lem_event_t *const event, lem_queue_t *const queue) {
    lem_bool_t retval = LEM_SEND_SUCCESS;
    /* queue end atomic read */
    /* consider use an union on 32bit targets to read queue start and end in
     * single instruction */
    lem_queue_pos_t new_q_end;
    lem_queue_pos_t q_end = queue->end;

    do {
        new_q_end = increment_rewind_queue_pos(q_end, LEM_QUEUE_SIZE + 1);

        /* queue start atomic read - queue start could only move to the
        right so it's thread safe (we can got only false negative (queue
        has space but we think it hasn't) not false positive )*/
        if (new_q_end == queue->start) {
            retval = LEM_SEND_FAIL;
            goto exit;
        }
        /* try to allocate space in a queue - increment queue end */
    } while (!lem_CAS_queue_position(&queue->end, &q_end, new_q_end));

    queue->buffer[q_end] = (lem_event_t *)event;
exit:
    return retval;
}

lem_event_t *lem_queue_pop_naive(lem_queue_t *const queue) {
    const volatile lem_event_t *event = NULL;

    /* check if queue is empty */
    if (lem_queue_is_empty(queue)) {
        goto exit;
    }

    event = queue->buffer[queue->start];
    queue->start = increment_rewind_queue_pos(queue->start, LEM_QUEUE_SIZE + 1);

exit:
    return (lem_event_t *)event;
}
