#include "construct.h"

#include "../constants.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO use append_str for simplicity
char *http1_construct_response(HTTP_RESPONSE *res) {
  // get total length
  uint len;
  {
    // HTTP/1.x + space + status (e.g. 200 OK) + CRLF (+ CRLF at the end of
    // headers, + body)
    len = 13 + strlen(HTTP_STATUS_STRS[res->status]) + res->body.len;
    for (uint i = 0; i < NUM_HTTP_RES_HEADERS; i++) {
      if (res->headers[i].len) {
        // [header name] + ':' + [header value(s)] + CRLF
        len += 3 + strlen(HTTP_RES_HEADER_STRS[i]) + res->headers[i].len;
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

    // Version
    {
      NEXT('H');
      NEXT('T');
      NEXT('T');
      NEXT('P');
      NEXT('/');
      NEXT('1');
      NEXT('.');
      NEXT(res->version == HTTP1_0 ? '0' : '1');

      NEXT(' ');
    }

    // status
    {
      const char *status = HTTP_STATUS_STRS[res->status];
      uint status_len = strlen(status);
      memcpy(s + n, status, status_len);
      n += status_len;
    }

    NEXT('\r');
    NEXT('\n');

    // Headers
    {
      for (uint i = 0; i < NUM_HTTP_RES_HEADERS; i++) {
        if (res->headers[i].len) {
          uint name_len = strlen(HTTP_RES_HEADER_STRS[i]);
          memcpy(s + n, HTTP_RES_HEADER_STRS[i], name_len);
          n += name_len;
          NEXT(':');
          memcpy(s + n, res->headers[i].c, res->headers[i].len);
          n += res->headers[i].len;
          NEXT('\r');
          NEXT('\n');
        }
      }
      NEXT('\r');
      NEXT('\n');
    }

    memcpy(s + n, res->body.c, res->body.len);
    n += res->body.len;
    s[n] = 0;
#undef NEXT
  }

  return s;
}

// TODO use append_str for simplicity
char *http1_construct_request(HTTP_REQUEST *req) {

  uint len;
  // length
  {
    // [method] [path] HTTP/1.xCRLF (+ CRLF at end of headers + body)
    len = 14 + strlen(HTTP_METHOD_STRS[req->method]) + req->path.len +
          req->body.len;

    for (uint i = 0; i < NUM_HTTP_REQ_HEADERS; i++) {
      if (req->header[i].c) {
        //[header name]:[header val]CRLF
        len += 3 + strlen(HTTP_REQ_HEADER_STRS[i]) + req->header[i].len;
      }
    }
  }

  char *s = malloc(len + 1);

  // write
  {
    uint n = 0;
#define NEXT(c) s[n++] = (c)

    // Method
    {
      const char *m_str = HTTP_METHOD_STRS[req->method];
      memcpy(s, m_str, strlen(m_str));
      n += strlen(m_str);
    }

    NEXT(' ');

    // Path
    {
      memcpy(s + n, req->path.c, req->path.len);
      n += req->path.len;
    }

    NEXT(' ');

    // Version
    {
      NEXT('H');
      NEXT('T');
      NEXT('T');
      NEXT('P');
      NEXT('/');
      NEXT('1');
      NEXT('.');
      NEXT(req->version ? '1' : '0');
    }

    NEXT('\r');
    NEXT('\n');

    // Headers
    {
      for (uint i = 0; i < NUM_HTTP_REQ_HEADERS; i++) {
        if (req->header[i].c) {
          const char *name_str = HTTP_REQ_HEADER_STRS[i];
          uint name_len = strlen(name_str);
          memcpy(s + n, name_str, name_len);
          n += name_len;

          NEXT(':');

          memcpy(s + n, req->header[i].c, req->header[i].len);
          n += req->header[i].len;

          NEXT('\r');
          NEXT('\n');
        }
      }

      NEXT('\r');
      NEXT('\n');
    }

    // Body
    {
      memcpy(s + n, req->body.c, req->body.len);
      n += req->body.len;
      s[n] = 0;
    }
#undef NEXT
  }

  return s;
}