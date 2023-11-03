#ifndef UTILS_H_
#define UTILS_H_
#include <netinet/in.h>

#include <cstdint>

void perror_die(const char *msg);
void report_client_connected(const struct sockaddr_in &sa, socklen_t salen);
int listen_inet_socket(uint16_t portnum);

#endif  // UTILS_H_