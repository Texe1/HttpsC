#pragma once

#include <stdlib.h>

#include "../../util/string.h"
#include "constants.h"

typedef struct http_request {
  HTTP_METHOD method;
  HTTP_VERSION version;
  STRING path;

  STRING header[NUM_HTTP_REQ_HEADERS];

  STRING body;

  int err;

} HTTP_REQUEST;

void print_http_req(HTTP_REQUEST);

void free_http_req(HTTP_REQUEST *);