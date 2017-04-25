#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

#define MAX_MESSAGE 32

int getConnection() {

  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd == -1) {
    std::cerr << "error on socket\n";
    std::cerr << "errno: " << strerror(errno) << '\n';
    return -1;
  }

  int one = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) < 0) {
    std::cerr << "error on SO_REUSEADDR\n";
    std::cerr << "errno: " << strerror(errno) << '\n';
    return -1;
  }

  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(struct sockaddr_in));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(8875);

  if (bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
    std::cerr << "error on bind\n";
    std::cerr << "errno: " << strerror(errno) << '\n';
    return -1;
  }

  if (listen(listenfd, 2) == -1) {
    std::cerr << "errno: " << strerror(errno) << '\n';
    std::cerr << "error on listen\n";
    return -1;
  }

  int fd = accept(listenfd, (struct sockaddr*)NULL, NULL);
  close(listenfd);
  return fd;
}

std::atomic<int> global;
std::atomic<bool> shouldExit;

void processMsg(int socket, char const *msg) {

  std::cout << "msg: " << msg << '\n';
  if (msg[0] == 's') {
    char const *beg = &msg[2];
    char const *end = &msg[3];
    while (*end >= '0' && *end <= '9')
      ++end;
    
    std::string res(beg, end - beg);
    try {
      int temp = std::stoi(res);
      global = temp;
      std::cout << "global: " << global << '\n';
    } catch(...) {
      std::cout << "error on convertion: " << res << '\n';
    } 
  }
  else if (msg[0] == 'g') {
    std::string value = std::to_string(global);
    value += '\n';
    send(socket, value.data(), value.size(), 0);
  }
}

void runServer() {

  int sock = getConnection();
  if (sock != -1) {

    int delimiter = '\n';
    char recvMsg[MAX_MESSAGE] = { 0 };
    char msgBuff[MAX_MESSAGE] = { 0 };
    while (1) {

      memset(recvMsg, 0, MAX_MESSAGE);
      memset(msgBuff, 0, MAX_MESSAGE);
      int len = recv(sock, recvMsg, MAX_MESSAGE, 0);
      if (len <= 0) {
        std::cout << "soket closed\n";
        close(sock);
        shouldExit = true;
        break;
      }

      char const *pDelimiter = strchr(recvMsg, delimiter);
      while (pDelimiter) {
        memcpy(msgBuff, recvMsg, pDelimiter - recvMsg + 1);
        processMsg(sock, msgBuff);
        pDelimiter = strchr(pDelimiter + 1, delimiter);
      }
    }
  }
}

void func() {

  std::thread thr(runServer);
  thr.detach();
}

int main() {

  func();

  while (!shouldExit) {

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
