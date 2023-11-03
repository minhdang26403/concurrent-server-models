#include "utils.h"

#include <netdb.h>

#include <iostream>

static constexpr int N_BACKLOG = 64;

void perror_die(const char *msg) {
  std::perror(msg);
  std::exit(EXIT_FAILURE);
}

void report_client_connected(const struct sockaddr_in &sa, socklen_t salen) {
  char hostbuf[NI_MAXHOST];
  char portbuf[NI_MAXSERV];
  if (getnameinfo(reinterpret_cast<const struct sockaddr *>(&sa), salen,
                  hostbuf, NI_MAXHOST, portbuf, NI_MAXSERV, 0) == 0) {
    std::cout << "Client (" << hostbuf << ", " << portbuf << ") connected\n";
  } else {
    std::cout << "Client (unknown) connected\n";
  }
}

int listen_inet_socket(uint16_t portnum) {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror_die("ERROR opening listening socket");
  }

  // The implementation of TCP prevents binding the same address until a timeout
  // expires (several minutes). The SO_REUSEADDR socket option bypasses this
  // restriction.
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
      0) {
    perror_die("setsockopt");
  }

  struct sockaddr_in server_addr {};
  server_addr.sin_family = AF_INET;
  // bind to all available network interfaces on the host
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(portnum);

  if (bind(server_socket, reinterpret_cast<struct sockaddr *>(&server_addr),
           sizeof(server_addr)) < 0) {
    perror_die("ERROR on binding");
  }

  if (listen(server_socket, N_BACKLOG) < 0) {
    perror_die("ERROR on listen");
  }

  return server_socket;
}
