#include "thread_pool.h"

#include <iostream>
#include <string>

void hello1() { std::cout << "Hello from 1\n"; }

void hello2() { std::cout << "Hello from 2\n"; }

void hello(int i) { std::cout << "Hello " << i << '\n'; }

void print(std::string s) { std::cout << s << '\n'; }

int main() {
  thread_pool pool;
  pool.submit(hello1);
  pool.submit(hello2);

  pool.submit(hello, 1);
  pool.submit(print, std::string("abc"));

  return 0;
}