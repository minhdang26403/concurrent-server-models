#ifndef FUNCTION_WRAPPER_H_
#define FUNCTION_WRAPPER_H_

#include <memory>
#include <utility>

struct impl_base {
  virtual void call() = 0;
  virtual ~impl_base() {}
};

template <typename F>
struct impl_type : impl_base {
  F f;
  impl_type(F &&f_) : f(std::forward<F>(f_)) {}
  void call() override { f(); }
};

class function_wrapper {
 public:
  function_wrapper() = default;

  template <typename F>
  function_wrapper(F &&f)
      : impl(std::make_unique<impl_type<F>>(std::forward<F>(f))) {}

  function_wrapper(function_wrapper &&other) : impl(std::move(other.impl)) {}

  function_wrapper &operator=(function_wrapper &&other) {
    impl = std::move(other.impl);
    return *this;
  }

  function_wrapper(const function_wrapper &) = delete;
  function_wrapper(function_wrapper &) = delete;
  function_wrapper &operator=(const function_wrapper &) = delete;

  void operator()() { impl->call(); }

 private:
  std::unique_ptr<impl_base> impl;
};

#endif  // FUNCTION_WRAPPER_H_