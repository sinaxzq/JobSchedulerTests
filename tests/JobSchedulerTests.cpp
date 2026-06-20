#include "Tests.h"
#include <cassert>
#include "JobScheduler.h"

void testJobSchedulerCanBeCreatedAndDestroyed()
{
    JobScheduler scheduler(2);
}

void testJobSchedulerShutdownIsIdempotent()
{
    JobScheduler scheduler(2);

    scheduler.shutdown();
    scheduler.shutdown();
}

void testJobSchedulerRunsDelayedTask()
{
    JobScheduler scheduler(2);

    auto start = std::chrono::steady_clock::now();

    constexpr auto msDelay = 50;
    auto future = scheduler.scheduleAfter
    (std::chrono::milliseconds(msDelay) , 
        []() { 
        return 123; 
    });

    auto status = future.wait_for(std::chrono::seconds(1));

    assert(status == std::future_status::ready);
    assert(future.get() == 123);

    auto end = std::chrono::steady_clock::now();
    auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start
    ).count();

    assert(elapsedMs >= msDelay);
}

void testJobSchedulerRunsEarlierTaskFirst()
{
    JobScheduler scheduler(2);

    auto future1 = scheduler.scheduleAfter(
        std::chrono::milliseconds(200) ,
        []()
    {
        return std::chrono::steady_clock::now();
    }
    );

    auto future2 = scheduler.scheduleAfter(
        std::chrono::milliseconds(100) ,
        []()
    {
        return std::chrono::steady_clock::now();
    }
    );

    assert(future1.wait_for(std::chrono::seconds(1)) == std::future_status::ready);
    assert(future2.wait_for(std::chrono::seconds(1)) == std::future_status::ready);

    auto time1 = future1.get();
    auto time2 = future2.get();

    assert(time2 <= time1);
}

void testJobSchedulerRejectsScheduleAfterShutdown()
{
    JobScheduler scheduler(2);
    
    bool catched = false; 

    scheduler.shutdown();
   
      try
    {
          auto future = scheduler.scheduleAfter(std::chrono::milliseconds(0) , []() {});

        future.get();
    }
    catch (const std::runtime_error&)
    {
        catched = true;
    }

    assert(catched);
}