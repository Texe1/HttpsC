#pragma once

#include <stdlib.h>

#include "../../common/string.h"

enum HTTP_METHOD {
  GET,
  POST,
  PUT, // TODO other methods
};

enum HTTP_VERSION {
  HTTP_0_9, // NOT IMPLEMENTED
  HTTP_1_0, // NOT IMPLEMENTED
  HTTP_1_1, // NOT IMPLEMENTED
  HTTP_2_0, // NOT IMPLEMENTED
  HTTP_3_0, // NOT IMPLEMENTED
};





typedef struct http_request {
  enum HTTP_METHOD method;
  void *path; // TODO parse paths
  enum HTTP_VERSION version;

	

} http_request;
