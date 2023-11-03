#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <atomic>
#include <future>

#include "concurrent_queue.h"
#include "function_wrapper.h"
#include "join_threads.h"

class thread_pool {
 public:
  thread_pool() : done(false), joiner(threads) {
    const unsigned thread_count = std::thread::hardware_concurrency();
    try {
      for (unsigned i = 0; i < thread_count; i++) {
        threads.push_back(std::thread(&thread_pool::worker_thread, this));
      }
    } catch (...) {
      done = true;
      throw;
    }
  }

  ~thread_pool() {
    // cancel all pending tasks
    // while (!work_queue.empty()) {
    //   ;
    // }
    done = true;
  }

  // template< class F, class... ArgTypes >
  // class result_of<F(ArgTypes...)>;
  // removed in C++20

  // instead, use (since C++17):
  // template< class F, class... ArgTypes >
  // class invoke_result;

  template <typename F, typename... Args>
  std::future<std::invoke_result_t<F, Args...>> submit(F &&f, Args &&...args) {
    using result_type = std::invoke_result_t<F, Args...>;
    // no argument types in the template of `packaged_task` since we already
    // bind these arguments to the function
    std::packaged_task<result_type()> task(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<result_type> res = task.get_future();
    work_queue.push(function_wrapper(std::move(task)));

    return res;
  }

  void run_pending_task() {
    function_wrapper task;
    if (work_queue.try_pop(task)) {
      task();
    } else {
      std::this_thread::yield();
    }
  }

 private:
  void worker_thread() {
    while (!done) {
      run_pending_task();
    }
  }

  std::atomic<bool> done;
  concurrent_queue<function_wrapper> work_queue;
  std::vector<std::thread> threads;
  join_threads joiner;
};

#endif  // THREAD_POOL_H_