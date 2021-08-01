//
// Created by kira on 01.08.2021.
//

#pragma once

#include <coroutine>
#include <unordered_map>
#include <vector>

namespace chlengine {
class Executor {
    Executor() = default;

public:
    static auto& instance() {
        static Executor executor{};
        return executor;
    }

    void create_task(std::coroutine_handle<> h) { call_stacks_[h.address()].emplace_back(h); }

    void schedule(std::coroutine_handle<> h) { new_task_ = h; }

    void spin_once() {
        for (auto it = call_stacks_.begin(); it != call_stacks_.end();) {
            if (it->second.empty()) {
                it = call_stacks_.erase(it);
                continue;
            }
            auto& call_stack = it->second;
            if (call_stack.back().done()) {
                call_stack.pop_back();
            } else {
                call_stack.back().resume();
                if (new_task_) {
                    call_stack.push_back(new_task_);
                    new_task_ = nullptr;
                }
            }
            ++it;
        }
    }

    void spin() {
        while (!call_stacks_.empty()) {
            spin_once();
        }
    }

private:
    std::unordered_map<void*, std::vector<std::coroutine_handle<>>> call_stacks_{};
    std::coroutine_handle<> new_task_{};
};
}  // namespace chlengine
