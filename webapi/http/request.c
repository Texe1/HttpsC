#include "request.h"

#include <stdio.h>

void printRequest(HTTP_REQUEST req) {
  printf("%s %s HTTP/%s\n", HTTP_METHOD_STRS[req.method], req.path,
         HTTP_VERSION_STRS[req.version]);

  for (uint i = 0; i < NUM_HTTP_REQ_HEADERS; i++) {
    if (req.header[i]) {
      printf("%s : %s\n", HTTP_REQ_HEADER_STRS[i], req.header[i]);
    }
  }

  if (req.body) {
    printf("%s\n", req.body);
  }
}
