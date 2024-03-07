#include "CppUTest/TestHarness.h"
#include "lem.h"

#define MAKE_EO(fname, postfix)                                                                                                                      \
    static void fname##_##postfix(lem_event_t *const event) {                                                                                        \
        if (event) {                                                                                                                                 \
            EO_last_event[postfix] = event;                                                                                                          \
            lem_free(event);                                                                                                                         \
        }                                                                                                                                            \
    }

static lem_event_t *EO_last_event[3] = {0};
MAKE_EO(EO, 0)
MAKE_EO(EO, 1)
MAKE_EO(EO, 2)

static lem_eo_t eo_array[] = {EO_0, EO_1, EO_2};

// clang-format off
TEST_GROUP(LEM_UT_Dispatcher){
    void setup() {
        lem_init_eo_table(eo_array);
    }
    void teardown() {
        for (size_t i = 0; i < 3; i++) {
            EO_last_event[i] = NULL;
        }
    }
};
// clang-format on

TEST(LEM_UT_Dispatcher, dispatch_empty) {
    lem_event_t *event = lem_alloc();
    /* event not send */
    lem_dispatch();
    CHECK(NULL == EO_last_event[0]);
    CHECK(NULL == EO_last_event[1]);
    CHECK(NULL == EO_last_event[2]);
    lem_free(event);
}

TEST(LEM_UT_Dispatcher, dispatch_single_queue) {
    lem_event_t *event = lem_alloc();
    event->eo_id = 0;
    lem_send(event, 0);
    lem_dispatch();
    CHECK(EO_last_event[0] == event);
}

TEST(LEM_UT_Dispatcher, dispatch_multi_queues_low_priority_first) {
    lem_event_t *event_lp = lem_alloc();
    lem_event_t *event_mp = lem_alloc();
    lem_event_t *event_hp = lem_alloc();
    event_lp->eo_id = 0;
    event_mp->eo_id = 0;
    event_hp->eo_id = 0;

    lem_send(event_lp, 0);
    lem_send(event_mp, 1);
    lem_send(event_hp, 2);
    lem_dispatch();
    CHECK(EO_last_event[0] == event_hp);
    lem_dispatch();
    CHECK(EO_last_event[0] == event_mp);
    lem_dispatch();
    CHECK(EO_last_event[0] == event_lp);
}

TEST(LEM_UT_Dispatcher, dispatch_multi_queues_high_priority_first) {
    lem_event_t *event_lp = lem_alloc();
    lem_event_t *event_mp = lem_alloc();
    lem_event_t *event_hp = lem_alloc();
    event_lp->eo_id = 0;
    event_mp->eo_id = 0;
    event_hp->eo_id = 0;

    lem_send(event_hp, 2);
    lem_send(event_mp, 1);
    lem_send(event_lp, 0);
    lem_dispatch();
    CHECK(EO_last_event[0] == event_hp);
    lem_dispatch();
    CHECK(EO_last_event[0] == event_mp);
    lem_dispatch();
    CHECK(EO_last_event[0] == event_lp);
}

TEST(LEM_UT_Dispatcher, dispatch_single_queue_events_fifo_order) {
    lem_event_t *events[LEM_QUEUE_SIZE] = {0};

    for (size_t i = 0; i < LEM_QUEUE_SIZE; i++) {
        events[i] = lem_alloc();
        events[i]->eo_id = 0;
        lem_send(events[i], 0);
    }

    for (size_t i = 0; i < LEM_QUEUE_SIZE; i++) {
        lem_dispatch();
        CHECK(EO_last_event[0] == events[i]);
    }
}