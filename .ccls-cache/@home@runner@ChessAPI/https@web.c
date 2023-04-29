#include "web.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <sys/socket.h>

uint nConnections = 0;
struct connection *connections = NULL;

void *start_server_thread(void *data) {}

int start_connection(struct connection conn) {
  // adding connection to the list of connections
  int idx = -1;

  if (!connections) {
    // allocating new array
    nConnections = 1;
    void *tmp = malloc(sizeof(struct connection));
    if (!tmp)
      return 1;
    connections = tmp;
    idx = 0;
  } else {
    for (uint i = 0; i < nConnections; i++) {
      if (~connections[i].active) {
        idx = i;
        break;
      }
    }
    if (idx < 0) {
      void *tmp =
          realloc(connections, sizeof(struct connection) * (nConnections + 1));
      if (!tmp)
        return 1;
      idx = nConnections;
      nConnections++;
    }
  }

  if (idx < 0)
    return 2;

  connections[idx] = conn;
  connections[idx].active = -1;

	pthread_create(&connections[idx].main_thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg)

  return 0;
}
int stop_connection(struct connection conn) { return 0; }