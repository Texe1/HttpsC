#pragma once
#include "server.h"

#include "../../common/string.h"

enum HTTP_METHOD {
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  CONNECT,
  OPTIONS,
  TRACE,
  PATCH
};

extern string HTTP_METHOD_NAMES[];

// TODO implement
void *http_rcv_cb(struct server *srv, uint idx);

// TODO immplement
struct server start_std_http_server(ushort port);
