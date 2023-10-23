#include <iostream>

#include "utils.h"

enum class ProcessingState { WAIT_FOR_MSG, IN_MSG };
static constexpr size_t BUF_SIZE = 1024;

void serve_connection(int connection_socket) {
  // "Ack" message from the server
  if (send(connection_socket, "*", 1, 0) < 1) {
    perror_die("send");
  }

  auto state = ProcessingState::WAIT_FOR_MSG;
  uint8_t buf[BUF_SIZE];

  while (true) {
    std::memset(buf, 0, sizeof(buf));
    int len = recv(connection_socket, buf, sizeof(buf), 0);
    if (len < 0) {
      perror_die("recv");
    } else if (len == 0) {
      // no messages are available and client has done an orderly shutdown
      break;
    }

    for (int i = 0; i < len; i++) {
      switch (state) {
        case ProcessingState::WAIT_FOR_MSG:
          if (buf[i] == '^') {
            state = ProcessingState::IN_MSG;
          }
          break;
        case ProcessingState::IN_MSG:
          if (buf[i] == '$') {
            state = ProcessingState::WAIT_FOR_MSG;
          } else {
            buf[i] += 1;
            if (send(connection_socket, &buf[i], 1, 0) < 1) {
              std::perror("send error");
              shutdown(connection_socket, SHUT_RDWR);
              return;
            }
          }
          break;
      }
    }
  }

  shutdown(connection_socket, SHUT_RDWR);
}

int main(int argc, char* argv[]) {
  uint16_t portnum = 9090;
  if (argc >= 2) {
    portnum = atoi(argv[1]);
  }

  std::cout << "Serving on port " << portnum << '\n';

  int server_socket = listen_inet_socket(portnum);

  while (true) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    int connection_socket =
        accept(server_socket, reinterpret_cast<struct sockaddr*>(&client_addr),
               &client_addr_len);

    if (connection_socket < 0) {
      perror_die("ERROR on accept");
    }

    report_client_connected(client_addr, client_addr_len);
    serve_connection(connection_socket);
    std::cout << "Client done\n";
  }

  return 0;
}