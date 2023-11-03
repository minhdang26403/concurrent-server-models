#ifndef JOIN_THREADS_H_
#define JOIN_THREADS_H_

#include <thread>
#include <vector>

class join_threads {
 public:
  explicit join_threads(std::vector<std::thread> &threads_)
      : threads(threads_) {}

  ~join_threads() {
    for (auto &thread : threads) {
      if (thread.joinable()) {
        thread.join();
      }
    }
  }

 private:
  std::vector<std::thread> &threads;
};

#endif  // JOIN_THREADS_H_