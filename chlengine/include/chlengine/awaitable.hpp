//
// Created by kira on 01.08.2021.
//

#pragma once

#include <coroutine>
#include <exception>

#include "executor.hpp"

namespace chlengine {
namespace detail {
template <typename T>
class Promise;
}

template <typename T = void>
class Awaitable {
public:
    using promise_type = detail::Promise<T>;

    explicit Awaitable(std::coroutine_handle<promise_type> coroutine)
        : callee_{std::move(coroutine)} {}
    ~Awaitable() {
        if (callee_) {
            callee_.destroy();
        }
    }
    Awaitable(const Awaitable& other) = delete;
    Awaitable& operator=(const Awaitable& other) = delete;

    Awaitable(Awaitable&& other) : callee_{other.callee_} { other.callee_ = nullptr; }
    Awaitable& operator=(Awaitable&& other) {
        if (callee_) {
            callee_.destroy();
        }
        callee_ = other.callee_;
        other.callee_ = nullptr;
        return *this;
    }

    bool await_ready() const { return !callee_ || callee_.done(); }

    void await_suspend(std::coroutine_handle<promise_type> /*caller*/) {
        Executor::instance().schedule(callee_);
    }

    auto await_resume() { return callee_.promise().result(); }

    std::coroutine_handle<promise_type> handle() { return callee_; }

private:
    std::coroutine_handle<promise_type> callee_;
};

namespace detail {
class PromiseBase {
public:
    PromiseBase() = default;

    auto initial_suspend() noexcept { return std::suspend_always{}; }
    auto final_suspend() noexcept { return std::suspend_always{}; }
};

template <typename T>
class Promise final : public PromiseBase {
public:
    Promise() = default;

    Awaitable<T> get_return_object() {
        return Awaitable<T>{std::coroutine_handle<Promise>::from_promise(*this)};
    }
    void unhandled_exception() { exception_ = std::current_exception(); }
    void return_value(T&& value) { result_ = std::forward<T>(value); }
    T result() {
        if (exception_) {
            std::rethrow_exception(exception_);
        }
        return std::move(result_);
    }

private:
    T result_;
    std::exception_ptr exception_{};
};

template <typename T>
class Promise<T&> final : public PromiseBase {
public:
    Promise() = default;

    Awaitable<T&> get_return_object() {
        return Awaitable<T&>{std::coroutine_handle<Promise>::from_promise(*this)};
    }
    void unhandled_exception() { exception_ = std::current_exception(); }
    void return_value(T& value) { result_ = std::addressof(value); }
    T& result() {
        if (exception_) {
            std::rethrow_exception(exception_);
        }

        return *result_;
    }

private:
    T* result_{nullptr};
    std::exception_ptr exception_{};
};

template <>
class Promise<void> final : public PromiseBase {
public:
    Promise() = default;

    Awaitable<void> get_return_object() {
        return Awaitable<void>{std::coroutine_handle<Promise>::from_promise(*this)};
    }

    void return_void() {}

    void unhandled_exception() { exception_ = std::current_exception(); }

    void result() {
        if (exception_) {
            std::rethrow_exception(exception_);
        }
    }

private:
    std::exception_ptr exception_;
};
}  // namespace detail
}  // namespace chlengine
