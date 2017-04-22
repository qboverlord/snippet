#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>


void sig_handler(int sig) {

  if (sig == SIGUSR1)
    std::cout << "recv SIGUSR1 " << std::this_thread::get_id() << '\n';
  else if (sig == SIGUSR2)
    std::cout << "recv SIGUSR2 " << std::this_thread::get_id() << '\n';
}

void func() {

  if (signal(SIGUSR1, sig_handler) == SIG_ERR)
    std::cout << "Can't register SIGUSR1\n";
  
  if (signal(SIGUSR2, sig_handler) == SIG_ERR)
    std::cout << "Can't register SIGUSR2\n";

  while (1) {
    std::cout << "working: " << std::this_thread::get_id() << '\n';
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

}


int main() {
  func();

  return 0;
}
