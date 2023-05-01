#include "response.h"

#include "response.h"

#include <stdio.h>
#include <string.h>

#include "constants.h"

extern const char *HTTP_RES_HEADER_STRS[];
extern const char *HTTP_VERSION_STRS[];

extern const char *HTTP_STATUS_STRS[];

char *formulateResponse(HTTP_RESPONSE *res) {

  // Getting the final length
  uint len = 0;
  {
    len = 5                                         // HTTP/
          + strlen(HTTP_VERSION_STRS[res->version]) // Version
          + 1                                       // SP
          + strlen(HTTP_STATUS_STRS[res->status])   // Status
          + 2;                                      // CRLF

    for (uint i = 0; i < NUM_HTTP_RES_HEADERS; i++) {
      if (res->headers[i]) {
        len += strlen(HTTP_RES_HEADER_STRS[i]) // header name
               + 1                             // :
               + strlen(res->headers[i])       // header value
               + 2;                            // CRLF
      }
    }

    len += 2                  // CRLF
           + res->max_body_sz // body
           + 1;               // NULL
  }

  uint i = 0;
  char *s = malloc(len + 1);

  if (!s) {
    perror("Out of Heap.\n");
    return NULL;
  }

  sprintf(s, "HTTP/%s %s\r\n", HTTP_VERSION_STRS[res->version],
          HTTP_STATUS_STRS[res->status]);

  i += 8 + strlen(HTTP_VERSION_STRS[res->version]) +
       strlen(HTTP_STATUS_STRS[res->status]);

  for (uint j = 0; j < NUM_HTTP_RES_HEADERS; j++) {
    if (res->headers[j]) {
      const char *name = HTTP_RES_HEADER_STRS[j];
      const char *val = res->headers[j];
      uint n = strlen(name);
      memcpy(s + i, name, n);
      i += n;
      s[i++] = ':';
      n = strlen(val);
      memcpy(s + i, val, n);
      i += n;
      s[i++] = '\r';
      s[i++] = '\n';
    }
  }

  s[i++] = '\n';
  s[i++] = '\r';

  memcpy(s + i, res->body, res->max_body_sz);
  i += res->max_body_sz;

  s[i] = 0;
  return s;
}