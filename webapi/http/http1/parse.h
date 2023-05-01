#pragma once

#include "../request.h"
#include "../response.h"

typedef enum http1_parse_err {
  H1_NO_ERR = 0,
  H1_ERR,
  H1_ERR_NO_VALID_METHOD,
  H1_ERR_NO_VALID_PATH,
  H1_ERR_NO_VALID_VERSION,
  H1_ERR_MISSING_LF,
} HTTP1_PARSE_ERR;

HTTP_REQUEST parse_http_1_request(char *s);
HTTP_RESPONSE parse_http_1_response(char *s);