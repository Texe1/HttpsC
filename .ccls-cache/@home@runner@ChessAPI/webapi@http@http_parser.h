pragma once

#include <stdlib.h>

#include "../../util.h"

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

enum HTTP_VERSION {
  HTTP_1_0 = 2,
  HTTP_1_1 = 0,
  HTTP_2_0 = 1,
};

enum HTTP_REQ_HEADER_TYPE {
  A_IM,
  ACCEPT,
  ACCEPT_CHARSET,
  ACCEPT_ENCODING,
  ACCEPT_LAGUAGE,
  ACCEPT_DATETIME,
  ACCESS_CONTROL_REQUEST_METHOD,
  ACCESS_CONTROL_REQUEST_HEADERS,
  AUTHORIZATION,
  CACHE_CONTROL,
  CONNECTION,
  CONTENT_ENCODING,
  CONTENT_LENGTH,
  CONTENT_MD5,
  CONTENT_TYPE,
  COOKIE,
  DATE,
  EXPECT,
  FORWARDED,
  FROM,
  HOST,
  HTTP2_SETTINGS,
  IF_MATCH,
  IF_MODIFIED_SINCE,
  IF_NONE_MATCH,
  IF_RANGE,
  IF_UNMODIFIED_SINCE,
  MAX_FORWARDS,
  ORIGIN,
  PRAGMA,
  PREFER,
  PROXY_AUTHORIZATION,
  RANGE,
  REFERER,
  TE,
  TRAILER,
  TRANSFER_ENCODING,
  USER_AGENT,
  UPGRADE,
  VIA,
  WARNING
};

struct http_request {
  enum HTTP_METHOD method;
  struct string path;
  enum HTTP_VERSION version;
  struct http_header_line {
    uint n;
    struct http_header_values {
      uint n;
      char **keys;
      char **vals;
    } * value_groups;
  } header[WARNING + 1];
  struct string body;
};

struct http_request parse_request(struct string str);

void print_http_req(struct http_request);

void free_http_req(struct http_request);