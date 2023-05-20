#include "parse.h"

#include <stdio.h>
#include <string.h>

#include "../constants.h"

STRING getHeader(HTTP_REQ_HEADER_FIELD_TYPE, char *);

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

    req.path = malloc_str(len);
    if (req.path.c == NULL) {
      ERR_RET(-1);
    }
    memcpy(req.path.c, s, len);
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
      STRING h = getHeader(i, headers);
      if (h.len != 0)
        req.header[i] = h;
    }

    free(headers);

    s += len;
  }

  // body
  req.body = str_from_char_ptr(s);

  return req;
#undef ERR_RET
}

STRING getHeader(HTTP_REQ_HEADER_FIELD_TYPE type, char *s) {

  STRING type_str = CONST_STR(HTTP_REQ_HEADER_STRS[type]);

  STRING val = {0};

  for (uint i = 0; s[i] != 0; i++) {
    if (i == 0 || s[i - 1] == '\n') {
      if (s[i] == '\n' || (s[i] == '\r' && s[i] == '\n'))
        break;
      if (s[i] == 1) { // header is obscured -> was already processed
        i = *(int *)(s + i + 1);
        continue;
      }
      // header name
      if (memcmp(s + i, type_str.c, type_str.len))
        continue;

      uint start = i;
      i += type_str;
      // optional whitespace
      while (s[i] == ' ')
        i++;
      // ':'
      if (s[i++] != ':') {
        return (STRING){0};
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
        if (!val.len) {
          val = str_from_char_ptr_fixed_len(s + i, n);
          if (!val.len)
            return (STRING){0};
        } else {
          void *tmp = realloc(val.c, val.len + n + 2);
          if (!tmp) {
            return val;
          }
          val.c = tmp;
          val.c[val.len] = ',';
          val.len++;

          memcpy(val.c + val.len, s + i, n);
					val.len += n;
					val.c[val.len] = 0;
        }
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