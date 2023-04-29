pragma once

#include <netinet/in.h>
#include <stdlib.h>

typedef enum HTTP_VERSION {
  HTTP1_0 = 0,
  HTTP1_1 = 1,
  HTTP2_0 = 2,
  HTTP3_0 = 3,
  HTTPS1_0 = 4,
  HTTPS1_1 = 5,
  HTTPS2_0 = 6,
  HTTPS3_0 = 7,
} HTTP_VERSION;

struct server {

  uint socket;
  uint epoll;

  /* the mimimum version of the HTTP protocol to use.
   * if this is a HTTP version (i.e. 0 - 3), the corresponding HTTPS versions
   * may also be allowed (e.g HTTP 2.0 doesn't allow for HTTPS 1.1, but will
   * take HTTPS 2.0) */
  HTTP_VERSION min_version;

  uint addr_len;
  struct sockaddr addr;
};