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

    // getting headers length
    uint len;
    {
      for (len = 0;; len++) {
        if (((s[len] == '\r' && s[len + 1] == '\n') || s[len] == '\n') &&
            s[len - 1] == '\n') {
          break;
        }
        if (s[0] == 0) {
          ERR_RET(H1_ERR_MISSING_LF);
        }
      }
    }

    char *headers = malloc(len);
    if (!headers)
      ERR_RET(-1);

    memcpy(headers, s, len);
    headers[len] = 0;

    for (uint i = 0; i < NUM_HTTP_REQ_HEADERS; i++) {
      char *h = getHeader(i, headers);
      if (h != (char *)(-1L))
        req.header[i] = h;
    }

    free(headers);

    s += len;
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

char *getHeader(HTTP_REQ_HEADER_FIELD_TYPE type, char *s) {

  const char *str = HTTP_REQ_HEADER_STRS[type];
  uint len = strlen(str);

  char *val = NULL;
  uint val_len = 0;

  for (uint i = 0; s[i] != 0; i++) {
    if (i == 0 || s[i - 1] == '\n') {
      if (s[i] == '\n' || (s[i] == '\r' && s[i] == '\n'))
        break;
      if (s[i] == 1) { // header is obscured -> was already processed
        i = *(int *)(s + i + 1);
        continue;
      }
      // header name
      if (memcmp(s + i, str, len))
        continue;

      uint start = i;
      i += len;
      // optional whitespace
      while (s[i] == ' ')
        i++;
      // ':'
      if (s[i++] != ':') {
        return (char *)(-1L);
      }

      // optional whitespace
      while (s[i] == ' ')
        i++;

      uint n = 0;
      // get value length
      while (s[i + n] != '\r' && s[i + n] != 0) {
        n++;
      }
      // save value
      {
        if (!val) {
          val = malloc(n + 1);
          if (!val)
            return (char *)(-1L);
        } else {
          void *tmp = realloc(val, val_len + n + 2);
          if (!tmp) {
            return val;
          }
          val = tmp;
          val[val_len] = ',';
          val_len++;
        }

        memcpy(val + val_len, s + i, n);
        val_len += n;
        val[val_len] = 0;
      }

      // obscure header
      {
        s[start] = 1;
        i += n;
        *(int *)(s + start + 1) = i;
      }
    }
  }
  return val;
}

HTTP_RESPONSE parse_http_1_response(char *s) {
  HTTP_RESPONSE res = {0};

  return res;
}