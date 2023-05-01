#pragma once

#include <stdlib.h>

#include "constants.h"

typedef struct http_response {
  HTTP_VERSION version;
  HTTP_STATUS status;

  char *headers[NUM_HTTP_RES_HEADERS];

  uint max_body_sz;
  char *body;
} HTTP_RESPONSE;