#ifndef CONCURRENT_QUEUE_H_
#define CONCURRENT_QUEUE_H_

#include <iostream>
#include <memory>
#include <mutex>

template <typename T>
class concurrent_queue {
 public:
  concurrent_queue() : head(std::make_unique<node>()), tail(head.get()) {}

  concurrent_queue(const concurrent_queue &) = delete;

  concurrent_queue &operator=(const concurrent_queue &) = delete;

  void push(T new_value) {
    auto new_data = std::make_shared<T>(std::move(new_value));
    auto p = std::make_unique<node>();
    {
      std::lock_guard<std::mutex> tail_lock(tail_mutex);
      tail->data = new_data;
      node *const new_tail = p.get();
      tail->next = std::move(p);
      tail = new_tail;
    }
    data_cond.notify_one();
  }

  std::shared_ptr<T> wait_and_pop() {
    // `old_head` is deleted here
    const std::unique_ptr<node> old_head = wait_pop_head();
    return old_head->data;
  }

  void wait_and_pop(T &value) { wait_pop_head(value); }

  std::shared_ptr<T> try_pop() {
    std::unique_ptr<node> old_head = try_pop_head();
    return old_head ? old_head->data : std::shared_ptr<T>();
  }

  bool try_pop(T &value) { return try_pop_head(value); }

  bool empty() const {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    return head.get() == get_tail();
  }

 private:
  struct node {
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;
  };

  node *get_tail() const {
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    return tail;
  }

  std::unique_ptr<node> pop_head() {
    std::unique_ptr<node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
  }

  std::unique_lock<std::mutex> wait_for_data() {
    std::unique_lock<std::mutex> head_lock(head_mutex);
    data_cond.wait(head_lock, [this] { return head.get() != get_tail(); });
    return head_lock;
  }

  std::unique_ptr<node> wait_pop_head() {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    return pop_head();
  }

  void wait_pop_head(T &value) {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    value = std::move(*head->data);
    pop_head();
  }

  std::unique_ptr<node> try_pop_head() {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if (head.get() == get_tail()) {
      return std::unique_ptr<node>();
    }
    return pop_head();
  }

  bool try_pop_head(T &value) {
    std::lock_guard<std::mutex> head_lock(head_mutex);
    if (head.get() == get_tail()) {
      return false;
    }
    value = std::move(*head->data);
    pop_head();
    return true;
  }

  mutable std::mutex head_mutex;
  std::unique_ptr<node> head;
  mutable std::mutex tail_mutex;
  node *tail;
  std::condition_variable data_cond;
};

#endif  // CONCURRENT_QUEUE_H_