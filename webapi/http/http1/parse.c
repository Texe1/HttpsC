#include "parse.h"

#include <stdio.h>
#include <string.h>

#include "../constants.h"

char *getHeader(HTTP_REQ_HEADER_FIELD_TYPE, char *);

HTTP_REQUEST parse_http_1_request(char *s) {
#define ERR_RET(_ERR)                                                          \
  {                                                                            \
    req.err = _ERR;                                                            \
    return req;                                                                \
  }

  HTTP_REQUEST req = {.method = -1, .version = -1, 0};

  // Method
  {
    for (uint i = 0; i < NUM_HTTP_METHODS; i++) {
      const char *m_str = HTTP_METHOD_STRS[i];
      if (memcmp(s, m_str, strlen(m_str)) == 0) {
        req.method = i;
        s += strlen(m_str);
        break;
      }
    }

    if (req.method == -1) {
      ERR_RET(H1_ERR_NO_VALID_METHOD);
    }
  }

  // Path
  {
    while (s[0] == ' ')
      s++;

    uint len = 0;
    while (s[len] != ' ') {
      if (s[len] == 0) {
        ERR_RET(H1_ERR_NO_VALID_VERSION);
      }
      len++;
    }

    req.path = malloc(len + 1);
    if (req.path == NULL) {
      ERR_RET(-1);
    }
    memcpy(req.path, s, len);
    req.path[len] = 0;

    s += len;

    while (s[0] == ' ')
      s++;
  }

  // Version
  {
    if (memcmp(s, "HTTP/1.", 7)) {
      ERR_RET(H1_ERR_NO_VALID_VERSION);
    }
    s += 7;

    switch (s[0]) {
    case '0':
      req.version = HTTP1_0;
      break;
    case '1':
      req.version = HTTP1_1;
      break;
    default:
      ERR_RET(H1_ERR_NO_VALID_VERSION);
    }

    s++;

    while (s[0] == ' ')
      s++;

    if (s[0] == '\r')
      s++;

    if (s[0] != '\n')
      ERR_RET(H1_ERR_MISSING_LF);
    s++;
  }

  // Headers
  {
    for (uint i = 0; i < NUM_HTTP_REQ_HEADERS; i++) {
      req.header[i] = getHeader(i, s);
    }

    // searching for empty line
    for (;; s++) {
      if (((s[0] == '\r' && s[1] == '\n') || s[0] == '\n') && s[-1] == '\n') {
        break;
      }
      if (s[0] == 0) {
        ERR_RET(H1_ERR_MISSING_LF);
      }
    }
  }

  // body
  {
    uint len = 0;
    while (s[len++] != 0)
      ;

    req.body = malloc(len);
    if (!req.body)
      ERR_RET(-1);

    memcpy(req.body, s, len);

    req.max_body_len = len - 1;
  }

  return req;
#undef ERR_RET
}

char *getHeader(HTTP_REQ_HEADER_FIELD_TYPE type, char *req) {

  const char *str = HTTP_REQ_HEADER_STRS[type];
  uint len = strlen(str);

  char *val = NULL;
  uint val_len = 0;

  for (uint i = 0; req[i] != 0; i++) {
    if ((i == 0 || req[i - 1] == '\n') && !memcmp(req + i, str, len)) {

      if (req[i] == '\r' || req[i] == '\n')
        break;
      i += len;
      while (req[i] == ' ')
        i++;
      if (req[i] != ':') {
        continue;
      }
      i++;

      while (req[i] == ' ')
        i++;

      uint start = i;

      while (req[i] != '\r' && req[i] != '\n') {
        if (req[i] == 0)
          return NULL;
        i++;
      }
      uint len = i - start;

      void *tmp = NULL;
      if (val == NULL) {
        tmp = malloc(len + 1);
      } else {
        tmp = realloc(val, val_len + len + 2);
      }
      if (tmp == NULL) {
        perror("Out of heap in getHeader()\n");
        exit(-1);
      }
      val = tmp;
      if (val_len > 0) {
        val[val_len] = ',';
        val_len++;
      }
      memcpy(val + val_len, req + start, len);
      val_len += len;
      val[val_len] = 0;
    }
  }
  return val;
}

HTTP_RESPONSE parse_http_1_response(char *s) {
  HTTP_RESPONSE res = {0};

  return res;
}