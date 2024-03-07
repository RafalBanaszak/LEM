#include "CppUTest/TestHarness.h"
#include "lem.h"

static void EO_dummy(lem_event_t *event) { (void *)event; }

static lem_eo_t eo_array[] = {EO_dummy};

// clang-format off
TEST_GROUP(LEM_UT_Send){
    void setup() {
        lem_init_eo_table(eo_array);
    }
    void teardown() {}
};
// clang-format on

TEST(LEM_UT_Send, send_single) {
    lem_event_t *event = lem_alloc();

    CHECK(lem_send(event, 0) == lem_success);

    lem_dispatch();
    lem_free(event);
}

TEST(LEM_UT_Send, send_full) {
    lem_event_t *events[LEM_QUEUE_SIZE] = {0};

    for (size_t i = 0; i < LEM_QUEUE_SIZE; i++) {
        events[i] = lem_alloc();
        CHECK(lem_send(events[i], 0) == lem_success);
    }

    for (size_t i = 0; i < LEM_QUEUE_SIZE; i++) {
        lem_dispatch();
        lem_free(events[i]);
    }
}

TEST(LEM_UT_Send, send_single_overflow) {
    lem_event_t *events[LEM_QUEUE_SIZE] = {0};

    for (size_t i = 0; i < LEM_QUEUE_SIZE; i++) {
        events[i] = lem_alloc();
        lem_send(events[i], 0);
    }

    lem_event_t *extra_event = lem_alloc();
    CHECK(lem_send(events[LEM_QUEUE_SIZE], 0) == lem_fail);

    for (size_t i = 0; i < LEM_QUEUE_SIZE; i++) {
        lem_dispatch();
        lem_free(events[i]);
    }
    lem_free(extra_event);
}

TEST(LEM_UT_Send, send_single_overflow_with_shift) {
    lem_event_t *events[LEM_QUEUE_SIZE] = {0};

    for (size_t i = 0; i < LEM_QUEUE_SIZE / 2; i++) {
        lem_event_t *shift_event = lem_alloc();
        lem_send(shift_event, 0);
        lem_dispatch();
        lem_free(shift_event);
    }

    for (size_t i = 0; i < LEM_QUEUE_SIZE; i++) {
        events[i] = lem_alloc();
        lem_send(events[i], 0);
    }

    lem_event_t *extra_event = lem_alloc();
    CHECK(lem_send(events[LEM_QUEUE_SIZE], 0) == lem_fail);
    lem_free(extra_event);

    for (size_t i = 0; i < LEM_QUEUE_SIZE; i++) {
        lem_dispatch();
        lem_free(events[i]);
    }
}

TEST(LEM_UT_Send, send_multiple_overflow) {
    lem_event_t *events[LEM_QUEUE_SIZE] = {0};

    for (size_t i = 0; i < LEM_QUEUE_SIZE; i++) {
        events[i] = lem_alloc();
        lem_send(events[i], 0);
    }

    for (size_t i = 0; i < LEM_QUEUE_SIZE + 2; i++) {
        lem_event_t *extra_event = lem_alloc();
        CHECK(lem_send(events[LEM_QUEUE_SIZE], 0) == lem_fail);
        lem_free(extra_event);
    }

    for (size_t i = 0; i < LEM_QUEUE_SIZE; i++) {
        lem_dispatch();
        lem_free(events[i]);
    }
}