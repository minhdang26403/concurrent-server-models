#include "concurrent_queue.h"

#include <iostream>

int main() {
  concurrent_queue<int> q;
  q.push(1);
  q.push(2);
  auto val = q.wait_and_pop();
  if (val) {
    std::cout << *val << '\n';
  } else {
    std::cout << "Nullptr\n";
  }

  int res;
  q.wait_and_pop(res);
  std::cout << res << '\n';

  q.push(3);
  q.push(4);
  val = q.try_pop();
  if (val) {
    std::cout << *val << '\n';
  }
  q.try_pop(res);
  std::cout << res << '\n';

  return 0;
}
