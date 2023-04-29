#include "server.h"

#include "../../common/util.h"

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <pthread.h>

struct server server_connect(struct server server) {
  server.socket = socket(AF_INET, SOCK_STREAM, 0);

  int option = 1;
  setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  struct sockaddr_in srv_addr = {
      .sin_family = AF_INET,
      .sin_addr = INADDR_ANY,
      .sin_port = htons(server.port),
      .sin_zero = {0},
  };

  if (bind(server.socket, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
    perror("bind failed");
    exit(-1);
  }
  if (listen(server.socket, 8) < 0) {
    perror("listen() call failed");
    exit(-1);
  }

  set_non_block(server.socket);

  server.epoll = epoll_create1(0);

  struct epoll_event evt = {
      .events = EPOLLIN,
      .data.u64 = -1L,
  };

  epoll_ctl(server.epoll, EPOLL_CTL_ADD, server.socket, &evt);

  return server;
}

void *rcv_caller(void *data) {
  struct server *srv = *((struct server **)data);
  uint client_idx = *((uint *)(data + sizeof(struct server *)));

  srv->clients[client_idx].dispatched = 1;

  if (!srv->rcv)
    return NULL;

  srv->rcv(srv, client_idx);

  srv->clients[client_idx].dispatched = 0;
  return NULL;
}

void *loop_server(void *data) {

  struct server *srv = data;

  printf("starting server on port %d (fd = %d)\n", srv->port, srv->socket);

  struct epoll_event evts[16];

  printf("starting loop...\n");
  for (int a = 0x100; a > 0; a--) {
    int nEvt = epoll_wait(srv->epoll, evts, 16, 30000);

    if (nEvt < 0) {
      printf("Error: errno = %d\n", errno);
      return NULL;
    }

    printf("no of events: %d\n", nEvt);

    for (int i = 0; i < nEvt; i++) {
      struct epoll_event evt = evts[i];
      printf("data: %lx\n", evt.data.u64);
      if (evt.data.u64 == -1L) {
        struct sockaddr client_addr = {0};
        socklen_t len = 0;
        uint client_port = accept(srv->socket, &client_addr, &len);
        printf("accepted: %d\n", client_port);
        set_non_block(client_port);

        struct serverside_client_data client = {
            .addr = client_addr,
            .fd = client_port,
        };

        add_client(srv, client);
        /*// TMP
        {

          char buf[129];
          int n = 0;

          while ((n = recv(client_port, buf, 128, 0)) > 0) {
            buf[n] = 0;

            printf("%s", buf);
          }
          char response[] =
              "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
              "22\r\n\r\n<h1>Hello, World!</h1>";

          send(client_port, response, strlen(response), 0);

          close(client_port);
        }*/
        continue;
      }

      uint idx = (uint)evt.data.u64;
      printf("client %d has a new event:\n", idx);

      if (!srv->rcv)
        continue;

      if (!(srv->flags & SERVER_DISABLE_FORK_ON_RCV)) {
        srv->rcv(srv, evt.data.u64);
        continue;
      }

      if (srv->clients[idx].dispatched)
        continue;

      // create thread for receiving data
      {
        char data[sizeof(struct server *) + sizeof(uint)];

        *((struct server **)data) = srv;
        *((uint *)(data + sizeof(void *))) = evt.data.u64;

        pthread_t th;
        pthread_create(&(srv->clients[idx].th), NULL, rcv_caller, data);
      }
    }
  }

  return 0;
}

int add_client(struct server *srv, struct serverside_client_data client) {
  // memory management
  {
    if (srv->clients == NULL) {
      srv->nClients = 0;
    }
    if (srv->nClients == 0) {
      srv->nClients = 16;
      srv->clients = calloc(16, sizeof(struct serverside_client_data));
    } else if (srv->first_emtpy_client >= srv->nClients) {
      srv->first_emtpy_client = srv->nClients;
      ulong sz = (srv->nClients + 16) * sizeof(struct serverside_client_data);
      void *tmp = realloc(srv->clients, sz);
      if (!tmp) {
        perror("Error: out of heap space.\n");
        exit(-1);
      }

      struct serverside_client_data *new_clients =
          &(srv->clients[srv->nClients]);
      memset(new_clients, 0, 16 * sizeof(struct serverside_client_data));
    }
  }

  // actually adding the client to the list of clients and the epoll
  {
    client.valid = 1;
    client.dispatched = 0;

    srv->clients[srv->first_emtpy_client] = client;

    struct epoll_event evt = {
        .data.u64 = (ulong)srv->first_emtpy_client,
        .events = EPOLLIN,
    };

    epoll_ctl(srv->epoll, EPOLL_CTL_ADD, client.fd, &evt);
  }

  // search for next empty client
  {
    while (srv->first_emtpy_client < srv->nClients &&
           srv->clients[srv->first_emtpy_client].valid != 0)
      srv->first_emtpy_client++;
  }

  return 0;
}

int rm_client(struct server *srv, uint idx) {

  struct serverside_client_data *ptr = &(srv->clients[idx]);

  // there is no client with index [idx]
  if (idx >= srv->nClients || !ptr->valid)
    return -1;

  // client at [idx] is no longer valid
  ptr->valid = 0;

  // [idx] might now be the first empty spot
  if (idx < srv->first_emtpy_client)
    srv->first_emtpy_client = idx;

  // joining the thread currently reading
  if (ptr->dispatched) {
    pthread_join(ptr->th, NULL);
  }

  return 0;
}

void terminate_server(struct server *srv) {
  for (uint i = 0; i < srv->nClients; i++) {
    if (srv->clients[i].dispatched) {
      pthread_join(srv->clients[i].th, NULL);
    }
    if (srv->clients[i].valid) {
      close(srv->clients[i].fd);
    }
  }
  if (srv->port != 0) {
    close(srv->port);
  }
  if (srv->epoll != 0) {
    close(srv->epoll);
  }
}