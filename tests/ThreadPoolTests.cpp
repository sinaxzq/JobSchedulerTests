#include "Tests.h"

#include "ThreadPool.h"

#include <cassert>
#include <stdexcept>

void testThreadPoolRejectsZeroWorkers()
{
    bool caught = false;

    try {
        ThreadPool pool(0);
    } catch (const std::invalid_argument&) {
        caught = true;
    }

    assert(caught);
}

void testThreadPoolRunsSubmittedTask()
{
    ThreadPool pool(2);

    auto future = pool.submit([]() {
        return 42;
    });

    assert(future.get() == 42);
}

void testThreadPoolShutdownIsIdempotent()
{
    ThreadPool pool(2);

    pool.shutdown();
    pool.shutdown();
}

