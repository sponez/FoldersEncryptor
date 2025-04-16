#pragma once

#include <queue>
#include <optional>
#include <mutex>
#include <atomic>
#include <condition_variable>

template <typename T>
class BlockingStrictIndexedQueue {
    public:
        BlockingStrictIndexedQueue() {}

        void push(T item, std::size_t index) {
            std::unique_lock<std::mutex> lock(_mutex);

            _pushNext.wait(
                lock,
                [&]() {
                    return index == _currentPush;
                }
            );

            _queue.push(std::move(item));
            ++_currentPush;

            _pushNext.notify_all();
            _cvEmpty.notify_all();
        }

        std::optional<std::size_t> pop(T& out) {
            std::unique_lock<std::mutex> lock(_mutex);

            while (true) {
                _cvEmpty.wait(lock, [&]() {
                    return !_queue.empty() || _closed;
                });

                if (!_queue.empty()) {
                    out = std::move(_queue.front());
                    _queue.pop();
                    return _currentPop++;
                }

                if (_closed) {
                    return std::nullopt;
                }
            }
        }

        void close() {
            std::lock_guard lock(_mutex);
            _closed = true;
            _cvEmpty.notify_all();
        }

        bool closed() const {
            std::lock_guard lock(_mutex);
            return _closed;
        }

    private:
        std::size_t _currentPush = 0;
        std::size_t _currentPop = 0;
        std::queue<T> _queue;

        std::condition_variable _pushNext;
        std::condition_variable _cvEmpty;
        std::mutex _mutex;

        bool _closed = false;
};