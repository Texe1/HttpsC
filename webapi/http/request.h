#pragma once

#include <stdlib.h>

#include "constants.h"

typedef struct http_request {
  HTTP_METHOD method;
  HTTP_VERSION version;
  char *path;

  char *header[NUM_HTTP_REQ_HEADERS];

  uint max_body_len;
  char *body;

  int err;

} HTTP_REQUEST;

HTTP_REQUEST parseRequest(char *s);

void printRequest(HTTP_REQUEST);