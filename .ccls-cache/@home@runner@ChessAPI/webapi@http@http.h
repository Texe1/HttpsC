#pragma once

#include <pthread.h>

#include "../web.h"

#define HTTP_DEFAULT_PORT 3000

struct http_server_data {
  char *root_path;
};

pthread_t start_http_server(short port, struct http_server_data http_data);