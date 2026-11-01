#include <stdio.h>
#include "../lock.h"

static int passed = 0;
static int failed = 0;

static void check(int condition, const char *name)
{
    printf("%s: %s\n", condition ? "PASS" : "FAIL", name);

    if (condition) {
        passed++;
    } else {
        failed++;
    }
}

static void test_table(void)
{
    const state_t expected_next[N_STATES][N_EVENTS] = {
        { GOT1, LOCKED, LOCKED, LOCKED },
        { GOT2, LOCKED, GOT1, LOCKED },
        { OPEN, LOCKED, GOT2, LOCKED },
        { OPEN, OPEN, LOCKED, LOCKED }
    };

    const action_t expected_action[N_STATES][N_EVENTS] = {
        { A_NONE, A_BEEP, A_NONE, A_NONE },
        { A_NONE, A_BEEP, A_NONE, A_NONE },
        { A_UNLOCK, A_BEEP, A_NONE, A_NONE },
        { A_NONE, A_NONE, A_LOCK, A_LOCK }
    };

    for (int s = 0; s < N_STATES; s++) {
        for (int e = 0; e < N_EVENTS; e++) {
            action_t action = A_UNDEFINED;
            state_t next = lock_step((state_t)s, (event_t)e, &action);
            char name[80];

            snprintf(name, sizeof name, "table %s / %s",
                     state_name((state_t)s), event_name((event_t)e));

            check(lock_table[s][e].next == expected_next[s][e] &&
                  lock_table[s][e].action == expected_action[s][e] &&
                  next == expected_next[s][e] &&
                  action == expected_action[s][e], name);
        }
    }
}

static void check_event(state_t s, char c, int expected)
{
    char name[80];

    snprintf(name, sizeof name, "lock_event %s character=%d",
             state_name(s), (unsigned char)c);

    check(lock_event(s, c) == expected, name);
}

static void test_events(void)
{
    check_event(LOCKED, '3', E_RIGHT);
    check_event(LOCKED, '0', E_WRONG);
    check_event(GOT1, '1', E_RIGHT);
    check_event(GOT1, '3', E_WRONG);
    check_event(GOT2, '4', E_RIGHT);
    check_event(GOT2, '1', E_WRONG);

    for (int c = '0'; c <= '9'; c++) {
        check_event(OPEN, (char)c, E_WRONG);
    }

    for (int s = 0; s < N_STATES; s++) {
        check_event((state_t)s, 'c', E_CLOSE);
        check_event((state_t)s, 't', E_TIMEOUT);
        check_event((state_t)s, 'x', -1);
        check_event((state_t)s, ' ', -1);
        check_event((state_t)s, '\n', -1);
    }
}

static void check_sequence(const char *input, state_t expected_state,
                           int expected_unlocks, int expected_locks,
                           int expected_beeps)
{
    state_t state = LOCKED;
    int unlocks = 0;
    int locks = 0;
    int beeps = 0;
    char name[80];

    for (int i = 0; input[i] != '\0'; i++) {
        int event = lock_event(state, input[i]);

        if (event == -1) {
            continue;
        }

        action_t action = A_UNDEFINED;
        state = lock_step(state, (event_t)event, &action);

        unlocks += action == A_UNLOCK;
        locks += action == A_LOCK;
        beeps += action == A_BEEP;
    }

    snprintf(name, sizeof name, "sequence %s", input);

    check(state == expected_state &&
          unlocks == expected_unlocks &&
          locks == expected_locks &&
          beeps == expected_beeps, name);
}

int main(void)
{
    test_table();
    test_events();

    check_sequence("314", OPEN, 1, 0, 0);
    check_sequence("3314", LOCKED, 0, 0, 3);
    check_sequence("31t4", LOCKED, 0, 0, 1);
    check_sequence("314c", LOCKED, 1, 1, 0);

    printf("Summary: %d passed, %d failed\n", passed, failed);

    return failed == 0 ? 0 : 1;
}
