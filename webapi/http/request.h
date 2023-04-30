#pragma once

#include <stdlib.h>

#include "http_constants.h"

typedef enum http_req_err {
  NO_ERR = 0,
  GENERIC,
  REQ_WRONG_FORMAT,
  OUT_OF_HEAP,
} HTTP_REQ_ERR;

typedef struct http_request {
  HTTP_METHOD method;
  HTTP_VERSION version;
  char *path;

  char *header[NUM_REQ_HEADERS];

  char *body;

  HTTP_REQ_ERR err;

} HTTP_REQUEST;

HTTP_REQUEST parseRequest(char *s);

void printRequest(HTTP_REQUEST);