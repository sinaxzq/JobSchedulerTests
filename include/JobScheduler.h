#pragma once

#include "ThreadPool.h"

#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

class JobScheduler {
public:
    explicit JobScheduler(int workerCount);
    ~JobScheduler();

    JobScheduler(const JobScheduler&) = delete;
    JobScheduler& operator=(const JobScheduler&) = delete;

    JobScheduler(JobScheduler&&) = delete;
    JobScheduler& operator=(JobScheduler&&) = delete;

    void shutdown();

    template <typename Func>
    auto scheduleAfter(
        std::chrono::milliseconds delay ,
        Func task
    ) -> std::future<std::invoke_result_t<Func>>
    {
        using Result = std::invoke_result_t<Func>;

        auto runAt = std::chrono::steady_clock::now() + delay;

        auto packagedTask = std::make_shared<std::packaged_task<Result()>>(
            std::move(task)
        );

        auto future = packagedTask->get_future();

        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (stopping_)
            {
                throw std::runtime_error("Cannot schedule task after scheduler shutdown");
            }

            jobs_.push(ScheduledJob{
                runAt,
                [packagedTask](){(*packagedTask)();}
            });
        }

        cv_.notify_one();

        return future;
    }

private:
    struct ScheduledJob {
        std::chrono::steady_clock::time_point runAt;
        std::function<void()> task;
    };

    struct CompareByRunAt {
        bool operator()(
            const ScheduledJob& left,
            const ScheduledJob& right
        ) const;
    };

    void schedulerLoop();

    ThreadPool pool_;
    std::thread schedulerThread_;

    std::priority_queue<
        ScheduledJob,
        std::vector<ScheduledJob>,
        CompareByRunAt
    > jobs_;

    std::mutex mutex_;
    std::condition_variable cv_;

    bool stopping_ = false;
};
