#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t threadCount = std::thread::hardware_concurrency())
        : _running(true)
    {
        for (size_t i = 0; i < threadCount; ++i) {
            _workers.emplace_back([this]() { this->workerLoop(); });
        }
    }

    ~ThreadPool() {
        join();
    }

    void submit(std::function<void()> task) {
        {
            std::unique_lock lock(_mutex);
            _tasks.push([this, task]() { task(); });
        }
        _cvNotEmpty.notify_all();
    }

    void join() {
        _running = false;
        _cvNotEmpty.notify_all();

        {
            std::unique_lock<std::mutex> lock(_mutex);

            _cvEmpty.wait(
                lock,
                [this]() {
                    return _tasks.empty();
                }
            );
        }

        _cvNotEmpty.notify_all();

        for (auto& worker: _workers) {
            if (worker.joinable()) worker.join();
        }
    }

private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;
    std::mutex _mutex;
    std::condition_variable _cvEmpty;
    std::condition_variable _cvNotEmpty;

    std::atomic<bool> _running;

    void workerLoop() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _cvNotEmpty.wait(
                    lock,
                    [this]() {
                        return !_tasks.empty() || !_running;
                    }
                );

                if (!_running && _tasks.empty()) {
                    _cvEmpty.notify_all();
                    return;
                } else if (_tasks.empty()) {
                    continue;
                }
                
                task = std::move(_tasks.front());
                _tasks.pop();
            }
            task();
            _cvEmpty.notify_all();
        }
    }
};