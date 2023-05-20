#include "request.h"

#include <stdio.h>

void print_http_req(HTTP_REQUEST req) {
  printf("%s %s HTTP/%s\n", HTTP_METHOD_STRS[req.method], req.path.c,
         HTTP_VERSION_STRS[req.version]);

  for (uint i = 0; i < NUM_HTTP_REQ_HEADERS; i++) {
    if (req.header[i].c) {
      printf("%s: %s\n", HTTP_REQ_HEADER_STRS[i], req.header[i].c);
    }
  }

  if (req.body.c) {
    printf("%s\n", req.body.c);
  }
}

void free_http_req(HTTP_REQUEST *req) {
  delete_str(&req->path);
  delete_str(&req->body);
  for (uint i = 0; i < NUM_HTTP_REQ_HEADERS; i++) {
    delete_str(req->header + i);
  }
}
