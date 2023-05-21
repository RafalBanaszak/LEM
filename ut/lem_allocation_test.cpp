#include "CppUTest/TestHarness.h"
#include "lem.h"

// clang-format off
TEST_GROUP(LEM_UT_Alloc){
    void setup() {
    }
    void teardown() {
    }
};
// clang-format on

TEST(LEM_UT_Alloc, alloc_single) {
    lem_event_t *event = lem_alloc();
    CHECK(NULL != event);
    lem_free(event);
}

TEST(LEM_UT_Alloc, alloc_all) {
    lem_event_t *events[LEM_EVENT_POOL_SIZE] = {0};
    for (lem_event_pool_pos_t i = 0; i < LEM_EVENT_POOL_SIZE; i++) {
        events[i] = lem_alloc();
        CHECK(NULL != events[i]);
    }

    for (lem_event_pool_pos_t i = 0; i < LEM_EVENT_POOL_SIZE; i++) {
        lem_free(events[i]);
    }
}

TEST(LEM_UT_Alloc, alloc_pool_starvation) {
    lem_event_t *events[LEM_EVENT_POOL_SIZE + 1] = {0};
    for (lem_event_pool_pos_t i = 0; i < LEM_EVENT_POOL_SIZE + 1; i++) {
        events[i] = lem_alloc();
    }

    CHECK(NULL == events[LEM_EVENT_POOL_SIZE]);

    for (lem_event_pool_pos_t i = 0; i < LEM_EVENT_POOL_SIZE; i++) {
        lem_free(events[i]);
    }
}

TEST(LEM_UT_Alloc, alloc_deep_starvation) {
    lem_event_t *events[LEM_EVENT_POOL_SIZE * 3] = {0};
    for (size_t i = 0; i < LEM_EVENT_POOL_SIZE * 3; i++) {
        events[i] = lem_alloc();
        if (i >= LEM_EVENT_POOL_SIZE) {
            CHECK(NULL == events[i]);
        }
    }

    for (lem_event_pool_pos_t i = 0; i < LEM_EVENT_POOL_SIZE; i++) {
        lem_free(events[i]);
    }
}