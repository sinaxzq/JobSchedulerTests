#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

class ThreadPool
{
public:
    explicit ThreadPool(int workerCount);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    void shutdown();

    template <typename Func> auto submit(Func task) -> std::future<std::invoke_result_t<Func>>
    {
        using Result = std::invoke_result_t<Func>;

        auto packagedTask = std::make_shared<std::packaged_task<Result()>>(std::move(task));

        auto future = packagedTask->get_future();

        {
            std::lock_guard<std::mutex> lock(mutex_);

            if (stopping_)
            {
                throw std::runtime_error("ThreadPool is stopped");
            }

            tasks_.push([packagedTask]() { (*packagedTask)(); });
        }

        cv_.notify_one();

        return future;
    }

private:
    void workerLoop();

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex mutex_;
    std::condition_variable cv_;

    bool stopping_ = false;
};
