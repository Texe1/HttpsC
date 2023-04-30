#pragma once

#include <stdlib.h>

#include "HTTP_CONSTANTS.h"

typedef struct http_response {
  HTTP_VERSION version;
  HTTP_STATUS status;

  char *headers[NUM_RES_HEADERS];

  uint max_body_sz;
  char *body;
} HTTP_RESPONSE;