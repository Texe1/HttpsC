#pragma once

#include "../request.h"
#include "../response.h"

#include <netinet/in.h>
#include <stdlib.h>

// TODO
typedef int (*SERVER_CALLBACK)(HTTP_REQUEST *req, HTTP_RESPONSE *res);

enum SERVER_CALLBACK_RET {
  SCB_NOP, // do nothing
  SCB_CLS, // close connection
  SCB_UGR, // upgrade version
  SCB_DGR, // downgrade version
};

typedef struct http_server_settings {
  ushort port;
  u_char use_https : 1, min_version : 2;
  SERVER_CALLBACK cb;
} HTTP_SERVER_SETTINGS;

typedef struct http_server {
  uint socket;
  uint epoll;

  struct sockaddr addr;

  SERVER_CALLBACK cb;
  HTTP_SERVER_SETTINGS config;

  uint n_conn;

  struct http_srv_conn {
    u_char present : 1, close : 1;
    HTTP_VERSION version;
    uint fd;
    struct sockaddr addr;
  } * clients;
} HTTP_SERVER;

HTTP_SERVER *http_create_server(HTTP_SERVER_SETTINGS *);
void *server_loop(void *);