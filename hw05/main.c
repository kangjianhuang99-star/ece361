#include <stdio.h>
#include "lock.h"

int main(void)
{
    state_t state = LOCKED;
    int c;

    while ((c = getchar()) != EOF) {
        int event = lock_event(state, (char)c);

        if (event == -1) {
            continue;
        }

        action_t action;
        state_t next = lock_step(state, (event_t)event, &action);

        printf("%s %c %s -> %s %s\n",
               state_name(state),
               c,
               event_name((event_t)event),
               state_name(next),
               action_name(action));

        state = next;
    }

    printf("final: %s\n", state_name(state));

    return 0;
}
