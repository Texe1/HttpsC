#pragma once

#include <netinet/in.h>
#include <stdlib.h>

#include <pthread.h>

struct server;

typedef void *(*rcv_cb)(struct server *server, uint client_idx);

enum SERVER_FLAGS {
  SERVER_DISABLE_AUTO_THREAD, // Doesn't create a Thread to run the Server on
  SERVER_DISABLE_FORK_ON_RCV, // Doesn't create a Thread to receive (and
                              // respond) to a message
  SERVER_DISABLE_AUTO_REPLY,  // The Server doesn't automatically reply with the
                              // return value ov its [rcv] function
};

struct server {
  uint socket;
  ushort port;
  uint flags;
  uint epoll;
  struct serverside_client_data {
    char valid : 1, dispatched : 1;
    pthread_t th;
    uint fd;
    struct sockaddr addr;
    void *data;
  } * clients;
  uint nClients;
  uint first_emtpy_client;

  rcv_cb rcv;
};

struct server server_connect(struct server);
void *loop_server(void *);
int add_client(struct server *, struct serverside_client_data);
int rm_client(struct server *srv, uint idx);
void terminate_server(struct server *);