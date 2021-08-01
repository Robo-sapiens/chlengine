//
// Created by kira on 31.07.2021.
//

#include <chrono>
#include <iomanip>
#include <iostream>

#include "chlengine/awaitable.hpp"

using chlengine::Awaitable;
using chlengine::Executor;
using namespace std::chrono_literals;

Awaitable<void> sleep_some(std::chrono::nanoseconds duration) {
    auto start = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() < start + duration) {
        co_await std::suspend_always{};
    }
    co_return;
}

Awaitable<void> print_world(size_t i) {
    co_await sleep_some(10s);
    std::cout << "world" << i << std::endl;
}

Awaitable<void> print_hello(size_t i) {
    std::cout << "hello" << i << std::endl;
    co_await print_world(i);
}

int main() {
    auto start = std::chrono::system_clock::now();

    std::vector<Awaitable<void>> tasks;
    for (size_t i = 0; i != 3; ++i) {
        tasks.emplace_back(print_hello(i));
        Executor::instance().create_task(tasks.back().handle());
    }
    Executor::instance().spin();

    std::cout << std::setprecision(2) << std::fixed
              << std::chrono::duration<double>{std::chrono::system_clock::now() - start}.count()
              << " seconds elapsed" << std::endl;
    return 0;
}
