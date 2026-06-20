#include "Tests.h"

#include <iostream>

int main()
{
    testThreadPoolRejectsZeroWorkers();
    testThreadPoolRunsSubmittedTask();
    testThreadPoolShutdownIsIdempotent();

    testJobSchedulerCanBeCreatedAndDestroyed();
    testJobSchedulerShutdownIsIdempotent();
    testJobSchedulerRunsDelayedTask();
    testJobSchedulerRunsEarlierTaskFirst();
    testJobSchedulerRejectsScheduleAfterShutdown();

    std::cout << "All tests passed\n";
    return 0;
}
