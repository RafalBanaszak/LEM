#include "CppUTest/TestHarness.h"
#include "lem.h"
#include <algorithm>
#include <array>
#include <vector>

static lem_event_t *EO_last_event = NULL;
static std::vector<lem_event_t *> g_events;

static void EO_dummy(lem_event_t *event) {
    if (event) {
        EO_last_event = event;
        // events will be freed in teardown
    }
}

static lem_eo_t eo_array[] = {EO_dummy};

static lem_event_t *UT_alloc_for_EO_0() {
    lem_event_t *event = lem_alloc();
    if (event) {
        event->eo_id = 0;
    }
    return event;
}

// clang-format off
TEST_GROUP(LEM_UT_Timer){
    void setup() {
        lem_init_eo_table(eo_array);
        if(LEM_TIMERS_NUMBER > LEM_QUEUE_SIZE) {
            FAIL("Queue size must be greater than or equal to the number of timers to run the tests");
        }
    }

    void teardown() {
        EO_last_event = NULL;
        for (auto it = g_events.begin(); it != g_events.end(); it++){
            if(*it){
                lem_free(*it);
            }
        }
        //prevent 'memory leak' detection
        g_events.clear();
        g_events.shrink_to_fit();
    }
};
// clang-format on

TEST(LEM_UT_Timer, create_single) {
    g_events.push_back(UT_alloc_for_EO_0());
    lem_timer_create(1, g_events.back(), 0);

    lem_timer_tick();
    lem_dispatch();

    CHECK(EO_last_event == g_events.back());
}

TEST(LEM_UT_Timer, create_single_zero_time) {
    g_events.push_back(UT_alloc_for_EO_0());
    lem_timer_create(0, g_events.back(), 0);

    lem_timer_tick();
    lem_dispatch();

    CHECK(EO_last_event == g_events.back());
}

TEST(LEM_UT_Timer, create_same_time_different_priorities) {
    g_events.push_back(UT_alloc_for_EO_0());
    g_events.push_back(UT_alloc_for_EO_0());
    g_events.push_back(UT_alloc_for_EO_0());

    lem_timer_create(1, g_events[0], 0);
    lem_timer_create(1, g_events[1], 1);
    lem_timer_create(1, g_events[2], 2);

    lem_timer_tick();
    lem_dispatch();
    CHECK(EO_last_event == g_events[2]);
    lem_dispatch();
    CHECK(EO_last_event == g_events[1]);
    lem_dispatch();
    CHECK(EO_last_event == g_events[0]);
}

TEST(LEM_UT_Timer, create_max_same_time) {
    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        g_events.push_back(UT_alloc_for_EO_0());
        auto timer = lem_timer_create(1, g_events.back(), 0);
        CHECK(LEM_TIMERS_NUMBER > timer);
    }

    lem_timer_tick();
    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        lem_dispatch();
        CHECK(g_events.end() != std::find(g_events.begin(), g_events.end(), EO_last_event));
    }
}

TEST(LEM_UT_Timer, create_max_different_time) {
    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        g_events.push_back(UT_alloc_for_EO_0());
        auto timer = lem_timer_create(i + 1, g_events.back(), 0);
        CHECK(LEM_TIMERS_NUMBER > timer);
    }

    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        lem_timer_tick();
        lem_dispatch();
        CHECK(EO_last_event == g_events[i]);
    }
}

TEST(LEM_UT_Timer, create_up_down) {
    const lem_timer_index_t timers_num = LEM_TIMERS_NUMBER / 2;

    for (auto i = 0; i < timers_num; i++) {
        g_events.push_back(UT_alloc_for_EO_0());
        auto timer = lem_timer_create(i + 1, g_events.back(), 0);
    }

    for (auto i = timers_num; i < timers_num * 2; i++) {
        g_events.push_back(UT_alloc_for_EO_0());
        auto timer = lem_timer_create(timers_num * 2 - i, g_events.back(), 0);
    }

    for (auto i = 0; i < timers_num; i++) {
        auto last_event_index = timers_num * 2 - 1 - i;

        lem_timer_tick();
        lem_dispatch();
        CHECK(EO_last_event == g_events[i] || EO_last_event == g_events[last_event_index]);
        lem_dispatch();
        CHECK(EO_last_event == g_events[i] || EO_last_event == g_events[last_event_index]);
    }
}

TEST(LEM_UT_Timer, create_too_many) {
    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        g_events.push_back(UT_alloc_for_EO_0());
        lem_timer_create(i + 1, g_events.back(), 0);
    }

    for (auto i = 0; i < LEM_TIMERS_NUMBER + 2; i++) {
        g_events.push_back(UT_alloc_for_EO_0());
        lem_timer_index_t timer = lem_timer_create(1, g_events.back(), 0);
        CHECK(LEM_TIMERS_NUMBER <= timer);
        lem_free(g_events.back());
        g_events.pop_back();
    }

    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        lem_timer_tick();
        lem_dispatch();
        CHECK(EO_last_event == g_events[i]);
    }
}

TEST(LEM_UT_Timer, delete_single) {
    g_events.push_back(UT_alloc_for_EO_0());
    auto timer = lem_timer_create(1, g_events.back(), 0);
    lem_timer_delete(timer);

    lem_timer_tick();
    lem_dispatch();

    CHECK(NULL == EO_last_event);
}

TEST(LEM_UT_Timer, delete_single_after_expiration) {
    g_events.push_back(UT_alloc_for_EO_0());
    auto timer = lem_timer_create(1, g_events.back(), 0);

    lem_timer_tick();
    lem_timer_delete(timer);
    lem_dispatch();

    CHECK(g_events.back() == EO_last_event);
}

TEST(LEM_UT_Timer, delete_many) {
    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        g_events.push_back(UT_alloc_for_EO_0());
        auto timer = lem_timer_create(i + 1, g_events.back(), 0);
    }

    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        lem_timer_delete(i);
    }

    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        lem_timer_tick();
        lem_dispatch();
        CHECK(NULL == EO_last_event);
    }
}

TEST(LEM_UT_Timer, create_many_after_delete_many) {
    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        g_events.push_back(UT_alloc_for_EO_0());
        auto timer = lem_timer_create(i + 1, g_events.back(), 0);
    }

    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        lem_timer_delete(i);
        lem_free(g_events[i]); // free events to prevent pool starvation
    }
    g_events.clear();

    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        g_events.push_back(UT_alloc_for_EO_0());
        auto timer = lem_timer_create(i + 1, g_events.back(), 0);
        CHECK(LEM_TIMERS_NUMBER > timer);
    }

    for (auto i = 0; i < LEM_TIMERS_NUMBER; i++) {
        lem_timer_tick();
        lem_dispatch();
        CHECK(EO_last_event == g_events[i]);
    }
}