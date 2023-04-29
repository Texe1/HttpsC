#pragma once

#include <netinet/in.h>

#include "../util.h"


struct server_data;

typedef struct string(*respond)(int client_idx, struct string msg, struct server_data *data);

struct server_data {
  short port;
  respond respond;
  uint nClients;
  struct client_data {
    struct sockaddr addr;
    int socket;
		void* data; 
  } * clients;

  // specific data e.g. root path for HTTP
  void *data;
};

void *server_thread(struct server_data *data);