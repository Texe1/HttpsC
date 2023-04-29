#include "request.h"

#include <stdio.h>
#include <string.h>

const char *METHOD_STRS[] = {"GET",    "HEAD",    "POST",    "PUT",
                             "DELETE", "CONNECT", "OPTIONS", "TRACE"};

const char *HEADER_TYPE_STRS[NUM_REQ_HEADERS] = {
    "Cache-Control",
    "Connection",
    "Content-Encoding",
    "Content-Length",
    "Content-MD5",
    "Content-Type",
    "Date",
    "Pragma",
    "Trailer",
    "Transfer-Encoding",
    "Upgrade",
    "Via",
    "Warning",
    "A-IM",
    "Accept",
    "Accept-Charset",
    "Accept-Datetime",
    "Accept-Encoding",
    "Accept-Language",
    "Access-Control-Request-Method",
    "Access-Control-Request-Headers",
    "Authorization",
    "Cookie",
    "Expect",
    "Forwarded",
    "From",
    "Host",
    "HTTP2-Settings",
    "If-Match",
    "If-Modified-Since",
    "If-None-Match",
    "If-Range",
    "If-Unmodified-Since",
    "Max-Forwards",
    "Origin",
    "Prefer",
    "Proxy-Autorization",
    "Range",
    "Referer",
    "TE",
    "User-Agent"};

const char *HTTP_VERSION_STRS[] = {"1.0", "1.1", "2", "3"};

char *getHeader(HTTP_REQ_HEADER_FIELD_TYPE, char *req);

HTTP_REQUEST parseRequest(char *s) {

  while (s[0] == ' ')
    s++;

  HTTP_REQUEST req = {0};
  req.method = -1;

  for (uint i = 0; i < sizeof(METHOD_STRS) / sizeof(const char *); i++) {
    if (memcmp(METHOD_STRS[i], s, strlen(METHOD_STRS[i])) == 0) {
      req.method = i;
      s += strlen(METHOD_STRS[i]);
      break;
    }
  }

  while (s[0] == ' ')
    s++;

  if (s[0] == 0) {
    req.err = REQ_WRONG_FORMAT;
    return req;
  }

  uint path_len = 0;
  char c = s[path_len];
  while (c != ' ') {
    if (c == 0 || c == '\r' || c == '\n') {
      req.err = REQ_WRONG_FORMAT;
      return req;
    }
    c = s[++path_len];
  }
  req.path = malloc(path_len + 1);
  if (req.path == NULL) {
    req.err = OUT_OF_HEAP;
    perror("Out of heap space\n");
    return req;
  }
  memcpy(req.path, (void *)s, path_len);
  req.path[path_len] = 0;

  s += path_len;

  while (s[0] == ' ')
    s++;

  if (memcmp(s, "HTTP/", 5)) {
    req.err = REQ_WRONG_FORMAT;
    return req;
  }

  s += 5;

  req.version = -1;
  for (uint i = 0; i < sizeof(HTTP_VERSION_STRS) / sizeof(char *); i++) {
    if (!memcmp(HTTP_VERSION_STRS[i], s, strlen(HTTP_VERSION_STRS[i]))) {
      req.version = i;
      s += strlen(HTTP_VERSION_STRS[i]);
      break;
    }
  }

  if (req.version == -1) {
    return req;
  }

  while (s[0] == ' ')
    s++;

  if (s[0] == '\r')
    s++;

  if (s[0] != '\n') {
    req.version = -1;
    return req;
  }
  s++;

  for (uint i = 0; i < NUM_REQ_HEADERS; i++) {
    req.header[i] = getHeader(i, s);
  }

  while (s[0] != 0) {
    if (s[0] == '\r') {
      s++;
      if (s[0] == '\n')
        s++;
      if (s[0] == '\r')
        s++;
      if (s[0] == '\n') {
        s++;
        break;
      }
    }

    s++;
  }
  uint len = 0;

  while (s[len] != 0) {
    len++;
  }

  if (len != 0) {
    req.body = malloc(len + 1);
    if (!req.body) {
      req.err = OUT_OF_HEAP;
      return req;
    }
    memcpy(req.body, s, len);
    req.body[len] = 0;
  }

  return req;
}

char *getHeader(HTTP_REQ_HEADER_FIELD_TYPE type, char *req) {

  const char *str = HEADER_TYPE_STRS[type];
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

void printRequest(HTTP_REQUEST req) {
  printf("%s %s HTTP/%s\n", METHOD_STRS[req.method], req.path,
         HTTP_VERSION_STRS[req.version]);

  for (uint i = 0; i < NUM_REQ_HEADERS; i++) {
    if (req.header[i]) {
      printf("%s : %s\n", HEADER_TYPE_STRS[i], req.header[i]);
    }
  }

  if (req.body) {
    printf("%s\n", req.body);
  }
}