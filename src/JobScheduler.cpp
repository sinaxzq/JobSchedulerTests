#include "JobScheduler.h"

JobScheduler::JobScheduler(int workerCount)
    : pool_(workerCount)
{
    schedulerThread_ = std::thread([this]() {
        schedulerLoop();
    });
}

JobScheduler::~JobScheduler()
{
    shutdown();
}

void JobScheduler::shutdown()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if (stopping_) {
            return;
        }

        stopping_ = true;
    }

    cv_.notify_all();

    if (schedulerThread_.joinable()) {
        schedulerThread_.join();
    }

    pool_.shutdown();
}

bool JobScheduler::CompareByRunAt::operator()(
    const ScheduledJob& left,
    const ScheduledJob& right
) const
{
    return left.runAt > right.runAt;
}

void JobScheduler::schedulerLoop()
{
    while (true)
    {
        std::function<void()> taskToRun;

        {
            std::unique_lock<std::mutex> lock(mutex_);

            cv_.wait(lock , [this]()
            {
                return stopping_ || !jobs_.empty();
            });

            if (stopping_)
            {
                return;
            }

            auto nextRunAt = jobs_.top().runAt;

            if (std::chrono::steady_clock::now() < nextRunAt)
            {
                cv_.wait_until(lock , nextRunAt);
                continue;
            }
            taskToRun = jobs_.top().task;
            jobs_.pop();
        }
        pool_.submit(std::move(taskToRun));

       
    }
}

