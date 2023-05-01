#include "construct.h"

#include "../constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *http1_construct_response(HTTP_RESPONSE *res) {
  // get total length
  uint len;
  {
    // HTTP/1.x + space + status (e.g. 200 OK) + CRLF (+ CRLF at the end of
    // headers, + body)
    len = 13 + strlen(HTTP_STATUS_STRS[res->status]) + res->max_body_sz;
    for (uint i = 0; i < NUM_HTTP_RES_HEADERS; i++) {
      if (res->headers[i]) {
        // [header name] + ':' + [header value(s)] + CRLF
        len += 3 + strlen(HTTP_RES_HEADER_STRS[i]) + strlen(res->headers[i]);
      }
    }
  }

  char *s = malloc(len + 1);
  if (!s)
    return NULL;

  // write
  {
#define NEXT(c) s[n++] = (c)

    uint n = 0;
    NEXT('H');
    NEXT('T');
    NEXT('T');
    NEXT('P');
    NEXT('/');
    NEXT('1');
    NEXT('.');
    NEXT(res->version == HTTP1_0 ? '0' : '1');

    NEXT(' ');

    // status
    {
      const char *status = HTTP_STATUS_STRS[res->status];
      uint status_len = strlen(status);
      memcpy(s + n, status, status_len);
      n += status_len;
    }

    NEXT('\r');
    NEXT('\n');

    for (uint i = 0; i < NUM_HTTP_RES_HEADERS; i++) {
      if (res->headers[i]) {
        uint name_len = strlen(HTTP_RES_HEADER_STRS[i]);
        memcpy(s + n, HTTP_RES_HEADER_STRS[i], name_len);
        n += name_len;
        NEXT(':');
        uint val_len = strlen(res->headers[i]);
        memcpy(s + n, res->headers[i], val_len);
        n += val_len;
        NEXT('\r');
        NEXT('\n');
      }
    }
    NEXT('\r');
    NEXT('\n');

    memcpy(s + n, res->body, res->max_body_sz);
    n += res->max_body_sz;
    s[n] = 0;
#undef NEXT
    /*

sprintf(s, "HTTP/1.%c %s\r\n", (res->version == HTTP1_0) ? '0' : '1',
        HTTP_STATUS_STRS[res->status]);

uint n = 11 + strlen(HTTP_STATUS_STRS[res->status]);

for (uint i = 0; i < NUM_HTTP_RES_HEADERS; i++) {
  if (res->headers[i]) {
    sprintf(s + n, "%s:%s\r\n", HTTP_RES_HEADER_STRS[i], res->headers[i]);
    n += 3 + strlen(HTTP_RES_HEADER_STRS[i]) + strlen(res->headers[i]);
  }
}

s[n++] = '\r';
s[n++] = '\n';

memcpy(s + n, res->body, res->max_body_sz);
n += res->max_body_sz;
if (n != len) {
  perror("!!!n = %d; len = %d!!!\n");
}
s[n] = 0;
                                                    */
  }

  return s;
}